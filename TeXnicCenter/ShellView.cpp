/********************************************************************
 *
 * This file is part of the TeXnicCenter-system
 *
 * Copyright (C) 1999-2000 Sven Wiegand
 * Copyright (C) 2000-$CurrentYear$ ToolsCenter
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * If you have further questions or if you want to support
 * further TeXnicCenter development, visit the TeXnicCenter-homepage
 *
 *    http://www.ToolsCenter.org
 *
 *********************************************************************/

#include "StdAfx.h"
#include "TeXnicCenter.h"
#include "ShellView.h"

#pragma push_macro("BUFSIZE")
#undef BUFSIZE

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShellView

IMPLEMENT_DYNCREATE(CShellView, CEditView)

CShellView::CShellView()
		: m_lf()
{
	m_dwProcessId = DWORD(-1);
	m_pReadThread = NULL;

	m_hChildStdinRd = NULL;
	m_hChildStdinWr = NULL;
	m_hChildStdinWrDup = NULL;
	m_hChildStdoutRd = NULL;
	m_hChildStdoutWr = NULL;
	m_hChildStdoutRdDup = NULL;

	//	Initialize LOGFONT structure
	m_lf.lfWeight = FW_NORMAL;
	m_lf.lfCharSet = GB2312_CHARSET; //ANSI_CHARSET;
	m_lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	m_lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	m_lf.lfQuality = DEFAULT_QUALITY;
	m_lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	lstrcpy(m_lf.lfFaceName, _T("FixedSys"));

	m_nHistoryCounter = 0;
}

CShellView::~CShellView()
{
}

BEGIN_MESSAGE_MAP(CShellView, CEditView)
	//{{AFX_MSG_MAP(CShellView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	//}}AFX_MSG_MAP
	// Standard-Druckbefehle
	ON_COMMAND(ID_FILE_PRINT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CEditView::OnFilePrintPreview)
	ON_CONTROL_REFLECT_EX(EN_CHANGE, OnEditChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShellView drawing

void CShellView::OnDraw(CDC* /*pDC*/)
{
	//CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CShellView diagnostics

#ifdef _DEBUG

void CShellView::AssertValid() const
{
	CEditView::AssertValid();
}

void CShellView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CShellView message handlers

int CShellView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEditView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//	GetEditCtrl().SetReadOnly(TRUE);
	DWORD dwStyle = GetWindowLong(GetEditCtrl().GetSafeHwnd(), GWL_STYLE);
	if (dwStyle)
	{
		dwStyle |= DS_LOCALEDIT | ES_WANTRETURN; //TeXnicCenter specifics--ES_WANTRETURN
		SetWindowLong(GetEditCtrl().GetSafeHwnd(), GWL_STYLE, dwStyle);
	}

	if (m_defFont.CreateFontIndirect(&m_lf))
		GetEditCtrl().SetFont(&m_defFont);
	if (!CreateShellRedirect())
		return -1;

	return 0;
}

const int BUFSIZE = 4096;

BOOL CShellView::CreateShellRedirect()
{
	SECURITY_ATTRIBUTES saAttr;
	BOOL bSuccess;

	// Set the bInheritHandle flag so pipe handles are inherited.
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	// The steps for redirecting child process's STDOUT:
	//	 1. Save current STDOUT, to be restored later.
	//	 2. Create anonymous pipe to be STDOUT for child process.
	//	 3. Set STDOUT of the parent process to be write handle to
	//		the pipe, so it is inherited by the child process.
	//	 4. Create a noninheritable duplicate of the read handle and
	//		close the inheritable read handle.

	// Save the handle to the current STDOUT.
	HANDLE hSaveStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	// Create a pipe for the child process's STDOUT.
	if (!CreatePipe(&m_hChildStdoutRd, &m_hChildStdoutWr, &saAttr, 0))
	{
		TRACE0("Stdout pipe creation failed\n");
		return FALSE;
	}

	// Set a write handle to the pipe to be STDOUT.
	if (!SetStdHandle(STD_OUTPUT_HANDLE, m_hChildStdoutWr))
	{
		TRACE0("Redirecting STDOUT failed\n");
		return FALSE;
	}

	// Create noninheritable read handle and close the inheritable read handle.
	bSuccess = DuplicateHandle(GetCurrentProcess(), m_hChildStdoutRd,
	                           GetCurrentProcess(), &m_hChildStdoutRdDup,
	                           0, FALSE,
	                           DUPLICATE_SAME_ACCESS);
	if (!bSuccess)
	{
		TRACE0("DuplicateHandle failed\n");
		return FALSE;
	}
	CloseHandle(m_hChildStdoutRd);

	// The steps for redirecting child process's STDIN:
	//	 1.  Save current STDIN, to be restored later.
	//	 2.  Create anonymous pipe to be STDIN for child process.
	//	 3.  Set STDIN of the parent to be the read handle to the
	//		 pipe, so it is inherited by the child process.
	//	 4.  Create a noninheritable duplicate of the write handle,
	//		 and close the inheritable write handle.

	// Save the handle to the current STDIN.
	HANDLE hSaveStdin = GetStdHandle(STD_INPUT_HANDLE);

	// Create a pipe for the child process's STDIN.
	if (!CreatePipe(&m_hChildStdinRd, &m_hChildStdinWr, &saAttr, 0))
	{
		TRACE0("Stdin pipe creation failed\n");
		return FALSE;
	}
	// Set a read handle to the pipe to be STDIN.
	if (!SetStdHandle(STD_INPUT_HANDLE, m_hChildStdinRd))
	{
		TRACE0("Redirecting Stdin failed\n");
		return FALSE;
	}
	// Duplicate the write handle to the pipe so it is not inherited.
	bSuccess = DuplicateHandle(GetCurrentProcess(), m_hChildStdinWr,
	                           GetCurrentProcess(), &m_hChildStdinWrDup,
	                           0, FALSE, // not inherited
	                           DUPLICATE_SAME_ACCESS);
	if (!bSuccess)
	{
		TRACE0("DuplicateHandle failed\n");
		return FALSE;
	}
	CloseHandle(m_hChildStdinWr);

	// Now create the child process.
	if (!CreateChildProcess())
	{
		TRACE0("CreateChildProcess failed\n");
		return FALSE;
	}
	// After process creation, restore the saved STDIN and STDOUT.
	if (!SetStdHandle(STD_INPUT_HANDLE, hSaveStdin))
	{
		TRACE0("Re-redirecting Stdin failed\n");
		return FALSE;
	}
	if (!SetStdHandle(STD_OUTPUT_HANDLE, hSaveStdout))
	{
		TRACE0("Re-redirecting Stdout failed\n");
		return FALSE;
	}
	m_pReadThread = AfxBeginThread((AFX_THREADPROC)ReadPipeThreadProc, (LPVOID)this);
	if (!m_pReadThread)
	{
		TRACE0("Cannot start read-redirect thread!\n");
		return FALSE;
	}
	return TRUE;
}

BOOL CShellView::CreateChildProcess()
{
	PROCESS_INFORMATION piProcInfo;
	STARTUPINFO siStartInfo;

	// Set up members of STARTUPINFO structure.
	ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO);

	siStartInfo.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	siStartInfo.wShowWindow = SW_HIDE;
	siStartInfo.hStdInput = m_hChildStdinRd;
	siStartInfo.hStdOutput = m_hChildStdoutWr;
	siStartInfo.hStdError = m_hChildStdoutWr;

	TCHAR shellCmd[MAX_PATH];
	if (!GetEnvironmentVariable(_T("ComSpec"), shellCmd, MAX_PATH))
		return FALSE;
#ifdef _UNICODE
	_tcscat(shellCmd, _T(" /U"));
#else
	_tcscat(shellCmd, _T(" /A"));
#endif
	// Create the child process.
	BOOL bRet = CreateProcess(NULL,
	                          shellCmd, // applicatin name
	                          NULL, // process security attributes
	                          NULL, // primary thread security attributes
	                          TRUE, // handles are inherited
	                          CREATE_NEW_CONSOLE, // creation flags
	                          NULL, // use parent's environment
	                          theApp.GetWorkingDir(), // TeXnicCenter specific--working directory
	                          //			NULL, 		  // use parent's current directory
	                          &siStartInfo, // STARTUPINFO pointer
	                          &piProcInfo); // receives PROCESS_INFORMATION
	if (bRet)
		m_dwProcessId = piProcInfo.dwProcessId;
	return bRet;
}

void CShellView::WriteToPipe(LPCTSTR line)
{
	DWORD dwWritten;

	WriteFile(m_hChildStdinWrDup, line, _tcslen(line) * sizeof(TCHAR),
	          &dwWritten, NULL);
}

UINT CShellView::ReadPipeThreadProc(LPVOID pParam)
{
	DWORD dwRead;
	TCHAR chBuf[BUFSIZE];
	CShellView* pView = (CShellView*)pParam;

	TRACE0("ReadPipe Thread begin run\n");
	for (;;)
	{
		if (!ReadFile(pView->m_hChildStdoutRdDup, chBuf,
		              BUFSIZE, &dwRead, NULL) || dwRead == 0)
			break;
		chBuf[dwRead / sizeof(TCHAR)] = _T('\0');
		pView->AddTexts(chBuf);
		pView->m_nLength = pView->GetEditCtrl().SendMessage(WM_GETTEXTLENGTH);
	}
	CloseHandle(pView->m_hChildStdinRd);
	CloseHandle(pView->m_hChildStdoutWr);
	CloseHandle(pView->m_hChildStdinWrDup);
	CloseHandle(pView->m_hChildStdoutRdDup);
	pView->m_pReadThread = NULL;
	pView->m_dwProcessId = DWORD(-1);
	pView->PostMessage(WM_CLOSE);
	return 1;
}

// original version does not work under Win9x,
// new version see below

/*void CShellView::OnDestroy()
{
        if (m_dwProcessId != DWORD(-1))
        {
                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_dwProcessId);
                if (hProcess)
                {
                        TerminateProcess(hProcess, 0);
                        CloseHandle(hProcess);
                }
        }
        if (m_pReadThread)
        {
                TerminateThread(m_pReadThread->m_hThread, 0);
                delete m_pReadThread;
        }
        CEditView::OnDestroy();
}*/

void CShellView::OnDestroy()
{
	if (m_dwProcessId != DWORD(-1))
	{
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_dwProcessId);
		if (hProcess)
		{
			TerminateProcess(hProcess, 0);

			WriteToPipe(_T("exit\r\n"));
			//WaitForSingleObject(hProcess, INFINITE);
			WaitForSingleObject(hProcess, 10000);

			CloseHandle(hProcess);
		}
	}
	if (m_pReadThread)
	{
		TerminateThread(m_pReadThread->m_hThread, 0);
		delete m_pReadThread;
		m_pReadThread = NULL;
	}
	CEditView::OnDestroy();
}

// original version,
// new version see below

/*void CShellView::AddTexts(LPCTSTR string)
{
        MoveToEnd();
        GetEditCtrl().ReplaceSel(string);
}*/

void CShellView::AddTexts(LPCTSTR string, BOOL bReplace)
{
	// replace '\n' by '\r''\n'
	int k, counter = 0, strLen = _tcslen(string);
	for (k = 0; k <= strLen; k++)
	{
		if (string[k] == _T('\n') && (k == 0 || string[k - 1] != _T('\r')))
			counter++;
	}
	LPTSTR str = (LPTSTR)GlobalAlloc(0, (strLen + counter + 1) * sizeof(TCHAR));
	for (k = strLen; k >= 0; k--)
	{
		str[k + counter] = string[k];
		if (string[k] == _T('\n') && (k == 0 || string[k - 1] != _T('\r')))
		{
			--counter;
			str[k + counter] = _T('\r');
		}
	}

	if (bReplace)
		MoveToPrompt();
	else
		MoveToEnd();
	GetEditCtrl().ReplaceSel(str);
	GlobalFree(str);
}

void CShellView::AddTexts(TCHAR ch)
{
	TCHAR string[2];
	string[0] = ch;
	string[1] = _T('\0');
	AddTexts((LPCTSTR)string);
}

void CShellView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	int nPos = GetCurrentPosition();
	if (nChar == 8 && nPos <= m_nLength)
		return;
	if (nPos < m_nLength)
		MoveToEnd();
	CEditView::OnChar(nChar, nRepCnt, nFlags);
	if (nChar == 13)
	{
		CString input, history;
		GetUserInput(input);

		if (input.Right(2) == _T("\r\n"))
			history = input.Left(input.GetLength() - 2);
		else
			history = input;
		history.TrimLeft();
		history.TrimRight();

		if (!history.IsEmpty() && (m_History.GetSize() == 0 || m_History.GetAt(m_History.GetSize() - 1) != history))
			m_History.Add(history);
		m_nHistoryCounter = m_History.GetSize();

		WriteToPipe(input);

		if (history == _T("exit"))
			GetParentFrame()->SendMessage(WM_CLOSE);
	}
}

void CShellView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_LEFT || nChar == VK_RIGHT)
		return;
	if (nChar == VK_UP)
	{
		int nHistorySize = m_History.GetSize();
		if (nHistorySize == 0)
			return;
		if (--m_nHistoryCounter < 0)
			m_nHistoryCounter = 0;
		AddTexts(m_History.GetAt(m_nHistoryCounter), TRUE);
		return;
	}
	if (nChar == VK_DOWN)
	{
		int nHistorySize = m_History.GetSize();
		if (nHistorySize == 0)
			return;
		if (++m_nHistoryCounter > nHistorySize)
			m_nHistoryCounter = nHistorySize;
		if (m_nHistoryCounter == nHistorySize)
			AddTexts(_T(""), TRUE);
		else
			AddTexts(m_History.GetAt(m_nHistoryCounter), TRUE);
		return;
	}
	if (nChar == VK_DELETE)
	{
		if (GetCurrentPosition() < m_nLength)
			return;
	}
	CEditView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CShellView::MoveToEnd()
{
	int nLen = GetEditCtrl().SendMessage(WM_GETTEXTLENGTH);
	GetEditCtrl().SetSel(nLen, nLen);
}

void CShellView::MoveToPrompt()
{
	int nLen = GetEditCtrl().SendMessage(WM_GETTEXTLENGTH);
	GetEditCtrl().SetSel(m_nLength, nLen);
}

int CShellView::GetCurrentPosition()
{
	GetEditCtrl().SetSel(-1, -1);
	int nstart, nstop;
	GetEditCtrl().GetSel(nstart, nstop);
	return nstart;
}

// original version does not work under Win9x,
// new version see below

/*void CShellView::GetUserInput(CString &input)
{
        int where = GetCurrentPosition();
        HLOCAL hBuffer = GetEditCtrl().GetHandle();
        if (hBuffer)
        {
                LPCTSTR szBuffer = (LPCTSTR)LocalLock(hBuffer);
                if (szBuffer)
                {
                        input = CString(szBuffer+m_nLength, (where-m_nLength));
                        LocalUnlock(hBuffer);
                }
        }
}*/

void CShellView::GetUserInput(CString &input)
{
	int where = GetCurrentPosition();
	{
		LPCTSTR szBuffer = LockBuffer();
		if (szBuffer)
		{
			input = CString(szBuffer + m_nLength, (where - m_nLength));
			UnlockBuffer();
		}
	}
}

int CShellView::GetSelLength()
{
	int nstart, nstop;
	GetEditCtrl().GetSel(nstart, nstop);
	return (nstart - nstop);
}

BOOL CShellView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CEditView::PreCreateWindow(cs);
}

void CShellView::OnEditPaste()
{
	MoveToEnd();
	CEditView::OnEditPaste();
}

BOOL CShellView::OnEditChange()
{
	ASSERT_VALID(this);
	//	GetDocument()->SetModifiedFlag(); //TeXnicCenter specific--no doc
	ASSERT_VALID(this);

	return FALSE; // continue routing
}

#pragma pop_macro("BUFSIZE")