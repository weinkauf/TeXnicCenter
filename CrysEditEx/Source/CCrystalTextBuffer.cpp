////////////////////////////////////////////////////////////////////////////
//	File:		CCrystalTextBuffer.cpp
//	Version:	1.0.0.0
//	Created:	29-Dec-1998
//
//	Author:		Stcherbatchenko Andrei
//	E-mail:		windfall@gmx.de
//
//	Implementation of the CCrystalTextBuffer class, a part of Crystal Edit -
//	syntax coloring text editor.
//
//	You are free to use or modify this code to the following restrictions:
//	- Acknowledge me somewhere in your about box, simple "Parts of code by.."
//	will be enough. If you can't (or don't want to), contact me personally.
//	- LEAVE THIS HEADER INTACT
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
//	17-Feb-99
//	+	FIX: unnecessary 'HANDLE' in CCrystalTextBuffer::SaveToFile
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
//	21-Feb-99
//		Paul Selormey, James R. Twine:
//	+	FEATURE: description for Undo/Redo actions
//	+	FEATURE: multiple MSVC-like bookmarks
//	+	FEATURE: 'Disable backspace at beginning of line' option
//	+	FEATURE: 'Disable drag-n-drop editing' option
//
//	+	FEATURE: changed layout of SUndoRecord. Now takes less memory
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
//	??-Aug-99
//		Sven Wiegand (search for "//BEGIN SW" to find my changes):
//	+ FEATURE: Remembering the text-position of the latest change.
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
//	24-Oct-99
//		Sven Wiegand
//	+ FIX: Setting m_ptLastChange to the beginning of the selection in
//		     InternalDeleteText(), so that position is valid in any case.
//		     Editor won't crash any more i.e. by selecting whole buffer and
//		     deleting it and then executing ID_EDIT_GOTO_LAST_CHANGE-command.
////////////////////////////////////////////////////////////////////////////

/********************************************************************
*
* $RCSfile$
* $Revision$
* $Date$
*
* $Author$
*
* $Log$
* Revision 1.6  2002/03/26 16:37:00  cnorris
* Fixed UNICODE allocation size error
*
* Revision 1.5  2002/03/26 16:24:58  cnorris
* Re-wrote LineInfo to manage its own data and manipulation
*
* Revision 1.4  2002/03/04 18:29:10  cnorris
* Match delete [] with new []
*
* Revision 1.3  2002/03/04 18:03:58  cnorris
* Better handling of text files with bad eol sequences
*
* Revision 1.2  2002/02/27 19:21:37  cnorris
* Change LoadFromFile to free buffer after successful read
*
* Revision 1.1.1.1  2002/02/26 08:11:53  svenwiegand
* Initial revision
*
* Revision 1.0  2000-05-31 21:55:23+02  sven_wiegand
* Initial revision
*
*
********************************************************************/

#include "stdafx.h"
#include <malloc.h>
#include "editcmd.h"
#include "CCrystalTextBuffer.h"
#include "CCrystalTextView.h"

#ifndef __AFXPRIV_H__
#pragma message("Include <afxpriv.h> in your stdafx.h to avoid this message")
#include <afxpriv.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define		UNDO_BUF_SIZE				1024

const TCHAR crlf[] = _T("\r\n");

#ifdef _DEBUG
#define _ADVANCED_BUGCHECK	1
#endif


/////////////////////////////////////////////////////////////////////////////
// CCrystalTextBuffer::SUndoRecord

void CCrystalTextBuffer::SUndoRecord::SetText(LPCTSTR pszText)
{
	m_pszText = NULL;
	if (pszText != NULL && pszText[0] != _T('\0'))
	{
		int nLength = _tcslen(pszText);
		if (nLength > 1)
		{
			m_pszText = new TCHAR[nLength + 1];
			_tcscpy(m_pszText, pszText);
		}
		else
		{
			m_szText[0] = pszText[0];
		}
	}
}

void CCrystalTextBuffer::SUndoRecord::FreeText()
{
	if (HIWORD((DWORD) m_pszText) != 0)
	{
		delete [] m_pszText;
		m_pszText = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCrystalTextBuffer::CLineInfo

void CCrystalTextBuffer::CLineInfo::GrowTo(int nMinSize)
{
	ASSERT( nMinSize > m_nLength || 
		(m_nLength == 0 && nMinSize == 0) ); // new size is large enough to contain current line
	if (nMinSize == 0)
		Destroy();
	else
	{
		int nMax = nMinSize+1;
		if (nMax & 0x0f)
			nMax = (nMax & ~(int)0x0f) + 0x10; // round up to multiple of 16
		ASSERT(nMax > nMinSize);
		if (nMax != m_nMax)
		{
			TCHAR *pcLine = new TCHAR[nMax];
			if (m_pcLine)
				_tcsncpy(pcLine, m_pcLine, m_nLength);
			pcLine[m_nLength] = _T('\0');
			m_nMax = nMax;
			delete [] m_pcLine;
			m_pcLine = pcLine;
		}
	}
}

void CCrystalTextBuffer::CLineInfo::Append(LPCTSTR pszLine, int nLength /* = -1*/)
{
	if ( pszLine == NULL || nLength == 0 || (nLength == -1 && ((nLength = _tcsclen(pszLine)) == 0)) )
		return;

	int nNewLength = nLength + m_nLength;
	GrowTo(nNewLength);
	_tcsncat(m_pcLine+m_nLength, pszLine, nLength);
	m_nLength = nNewLength;
	m_pcLine[nNewLength] = _T('\0');
}

void CCrystalTextBuffer::CLineInfo::InsertText(int nPos, LPCTSTR pszLine, int nLength /* = -1*/)
{
	ASSERT( nPos >= 0 && nPos <= m_nLength ); // nPos is within string
	if ( pszLine == NULL || nLength == 0 || (nLength == -1 && ((nLength = _tcsclen(pszLine)) == 0)) )
		return;

	int nNewLength = nLength + m_nLength;
	GrowTo(nNewLength);
	memmove(m_pcLine+nPos+nLength, m_pcLine+nPos, sizeof(TCHAR)nLength);
	_tcsncpy(m_pcLine+nPos, pszLine, nLength);
	m_nLength = nNewLength;
	m_pcLine[nNewLength] = _T('\0');
}

void CCrystalTextBuffer::CLineInfo::RemoveText(int nPos, int nCount, boolean bCompact /*= false*/)
{
	ASSERT( nPos >= 0  && nPos < m_nLength ); // nPos is within string
	ASSERT( nCount > 0 ); // nCount is valid
	ASSERT( nPos + nCount <= m_nLength );  // nCount will not overrun string

	int nRightCount = m_nLength - nCount - nPos;
	memmove(m_pcLine+nPos, m_pcLine+nPos+nCount, nRightCount);
	m_nLength -= nCount;
	m_pcLine[m_nLength] = _T('\0');
	if ( bCompact )
		FreeExtra();
}

void CCrystalTextBuffer::CLineInfo::TrimText(int nPos, boolean bCompact /*= false*/)
{
	ASSERT( nPos >= 0  && nPos <= m_nLength ); // nPos is within string
	m_pcLine[nPos] = _T('\0');
	m_nLength = nPos;
	if ( bCompact )
		FreeExtra();
}


/////////////////////////////////////////////////////////////////////////////
// CCrystalTextBuffer::CUpdateContext

void CCrystalTextBuffer::CInsertContext::RecalcPoint(CPoint &ptPoint)
{
	ASSERT(m_ptEnd.y > m_ptStart.y ||
		   m_ptEnd.y == m_ptStart.y && m_ptEnd.x >= m_ptStart.x);
	if (ptPoint.y < m_ptStart.y)
		return;
	if (ptPoint.y > m_ptStart.y)
	{
		ptPoint.y += (m_ptEnd.y - m_ptStart.y);
		return;
	}
	if (ptPoint.x <= m_ptStart.x)
		return;
	ptPoint.y += (m_ptEnd.y - m_ptStart.y);
	ptPoint.x = m_ptEnd.x + (ptPoint.x - m_ptStart.x);
}

void CCrystalTextBuffer::CDeleteContext::RecalcPoint(CPoint &ptPoint)
{
	ASSERT(m_ptEnd.y > m_ptStart.y ||
		   m_ptEnd.y == m_ptStart.y && m_ptEnd.x >= m_ptStart.x);
	if (ptPoint.y < m_ptStart.y)
		return;
	if (ptPoint.y > m_ptEnd.y)
	{
		ptPoint.y -= (m_ptEnd.y - m_ptStart.y);
		return;
	}
	if (ptPoint.y == m_ptEnd.y && ptPoint.x >= m_ptEnd.x)
	{
		ptPoint.y = m_ptStart.y;
		ptPoint.x = m_ptStart.x + (ptPoint.x - m_ptEnd.x);
		return;
	}
	if (ptPoint.y == m_ptStart.y)
	{
		if (ptPoint.x > m_ptStart.x)
			ptPoint.x = m_ptStart.x;
		return;
	}
	ptPoint = m_ptStart;
}


/////////////////////////////////////////////////////////////////////////////
// CCrystalTextBuffer

IMPLEMENT_DYNCREATE(CCrystalTextBuffer, CCmdTarget)

CCrystalTextBuffer::CCrystalTextBuffer()
{
	m_bInit = FALSE;
	m_bReadOnly = FALSE;
	m_bModified = FALSE;
	m_bCreateBackupFile = FALSE;
	m_nUndoPosition = 0;
	//BEGIN SW
	m_ptLastChange.x = m_ptLastChange.y = -1;
	//END SW
}

CCrystalTextBuffer::~CCrystalTextBuffer()
{
	ASSERT(! m_bInit);			//	You must call FreeAll() before deleting the object
}


BEGIN_MESSAGE_MAP(CCrystalTextBuffer, CCmdTarget)
	//{{AFX_MSG_MAP(CCrystalTextBuffer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCrystalTextBuffer message handlers

void CCrystalTextBuffer::InsertLine(LPCTSTR pszLine, int nLength /*= -1*/, int nPosition /*= -1*/)
{
	CLineInfo li(pszLine, nLength);
	if (nPosition == -1)
		m_aLines.Add(li);
	else
		m_aLines.InsertAt(nPosition, li);

#ifdef _DEBUG
	int nLines = m_aLines.GetSize();
	if (nLines % 5000 == 0)
		TRACE1("%d lines loaded!\n", nLines);
#endif
}


void CCrystalTextBuffer::AppendLine(int nLineIndex, LPCTSTR pszChars, int nLength /*= -1*/)
{
	m_aLines[nLineIndex].Append(pszChars, nLength);
}


void CCrystalTextBuffer::FreeAll()
{
	//	Free text
	int nCount = m_aLines.GetSize();
	for (int I = 0; I < nCount; I ++)
		m_aLines[I].Destroy();
	m_aLines.RemoveAll();

	//	Free undo buffer
	int nBufSize = m_aUndoBuf.GetSize();
	for (I = 0; I < nBufSize; I ++)
		m_aUndoBuf[I].FreeText();
	m_aUndoBuf.RemoveAll();

	m_bInit = FALSE;
	//BEGIN SW
	m_ptLastChange.x = m_ptLastChange.y = -1;
	//END SW
}

BOOL CCrystalTextBuffer::InitNew(int nCrlfStyle /*= CRLF_STYLE_DOS*/)
{
	ASSERT(! m_bInit);
	ASSERT(m_aLines.GetSize() == 0);
	ASSERT(nCrlfStyle >= 0 && nCrlfStyle <= 2);
	InsertLine(_T(""));
	m_bInit = TRUE;
	m_bReadOnly = FALSE;
	m_nCRLFMode = nCrlfStyle;
	m_bModified = FALSE;
	m_nSyncPosition = m_nUndoPosition = 0;
	m_bUndoGroup = m_bUndoBeginGroup = FALSE;
	m_nUndoBufSize = UNDO_BUF_SIZE;
	ASSERT(m_aUndoBuf.GetSize() == 0);
	UpdateViews(NULL, NULL, UPDATE_RESET);
	//BEGIN SW
	m_ptLastChange.x = m_ptLastChange.y = -1;
	//END SW
	return TRUE;
}

BOOL CCrystalTextBuffer::GetReadOnly() const
{
	ASSERT(m_bInit);	//	Text buffer not yet initialized.
						//	You must call InitNew() or LoadFromFile() first!
	return m_bReadOnly;
}

void CCrystalTextBuffer::SetReadOnly(BOOL bReadOnly /*= TRUE*/)
{
	ASSERT(m_bInit);	//	Text buffer not yet initialized.
						//	You must call InitNew() or LoadFromFile() first!
	m_bReadOnly = bReadOnly;
}

static const char *crlfs[] =
{
	"\x0d\x0a",			//	DOS/Windows style
	"\x0a",				//	UNIX style
	"\x0a\x0d"			//	Macintosh style
};

DWORD CCrystalTextBuffer::LoadFromFile(LPCTSTR pszFileName, int nCrlfStyle /*= CRLF_STYLE_AUTOMATIC*/)
{
	HANDLE hFile = NULL;
	int nCurrentMax = 256;
	char *pcLineBuf = new char[nCurrentMax];

	DWORD result = 1;
	__try
	{
		DWORD dwFileAttributes = ::GetFileAttributes(pszFileName);
		if (dwFileAttributes == (DWORD) -1)
			__leave;

		hFile = ::CreateFile(pszFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
					OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			__leave;

		int nCurrentLength = 0;
		const DWORD dwBufSize = 32768;
		char *pcBuf = (char *) _alloca(dwBufSize);
		DWORD dwCurSize;
		if (! ::ReadFile(hFile, pcBuf, dwBufSize, &dwCurSize, NULL))
			__leave;

		FreeAll();  // release text buffer only after successful read

		if (nCrlfStyle == CRLF_STYLE_AUTOMATIC)
		{
			//	Try to determine current CRLF mode
			for (DWORD I = 0; I < dwCurSize; I ++)
			{
				if (pcBuf[I] == _T('\x0a'))
					break;
			}
			if (I == dwCurSize)
			{
				//	By default (or in the case of empty file), set DOS style
				nCrlfStyle = CRLF_STYLE_DOS;
			}
			else
			{
				//	Otherwise, analyse the first occurance of line-feed character
				if (I > 0 && pcBuf[I - 1] == _T('\x0d'))
				{
					nCrlfStyle = CRLF_STYLE_DOS;
				}
				else
				{
					if (I < dwCurSize - 1 && pcBuf[I + 1] == _T('\x0d'))
						nCrlfStyle = CRLF_STYLE_MAC;
					else
						nCrlfStyle = CRLF_STYLE_UNIX;
				}
			}
		}

		ASSERT(nCrlfStyle >= 0 && nCrlfStyle <= 2);
		m_nCRLFMode = nCrlfStyle;

		m_aLines.SetSize(0, 4096);

		DWORD dwBufPtr = 0;
		USES_CONVERSION;
		while (dwBufPtr < dwCurSize)
		{
			int c = pcBuf[dwBufPtr];
			dwBufPtr ++;
			if (dwBufPtr == dwCurSize && dwCurSize == dwBufSize)
			{
				if (! ::ReadFile(hFile, pcBuf, dwBufSize, &dwCurSize, NULL))
					__leave;
				dwBufPtr = 0;
			}

			// Strip \r ('\x0d') from input buffer
			if ((char)c == '\x0d')
				continue;

			pcLineBuf[nCurrentLength] = (char) c;
			nCurrentLength ++;
			if (nCurrentLength == nCurrentMax)
			{
				//	Reallocate line buffer
				nCurrentMax += 256;
				char *pcNewBuf = new char[nCurrentMax];
				memcpy(pcNewBuf, pcLineBuf, nCurrentLength);
				delete[] pcLineBuf;
				pcLineBuf = pcNewBuf;
			}

			if ((char) c == '\x0a')
			{
				pcLineBuf[nCurrentLength-1] = 0;
				InsertLine(A2T(pcLineBuf), nCurrentLength-1);
				nCurrentLength = 0;
			}
		}

		pcLineBuf[nCurrentLength] = 0;
		InsertLine(A2T(pcLineBuf), nCurrentLength);

		ASSERT(m_aLines.GetSize() > 0);		//	At least one empty line must present

		m_bInit = TRUE;
		m_bReadOnly = (dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0;
		m_bModified = FALSE;
		m_bUndoGroup = m_bUndoBeginGroup = FALSE;
		m_nUndoBufSize = UNDO_BUF_SIZE;
		m_nSyncPosition = m_nUndoPosition = 0;
		ASSERT(m_aUndoBuf.GetSize() == 0);
		result = 0;

		UpdateViews(NULL, NULL, UPDATE_RESET);
	}
	__finally
	{
		if (result)
			result = ::GetLastError();
		if (pcLineBuf != NULL)
			delete[] pcLineBuf;
		if (hFile != NULL)
			::CloseHandle(hFile);
	}
	//BEGIN SW
	m_ptLastChange.x = m_ptLastChange.y = -1;
	//END SW
	return result;
}

DWORD CCrystalTextBuffer::SaveToFile(LPCTSTR pszFileName, int nCrlfStyle /*= CRLF_STYLE_AUTOMATIC*/, BOOL bClearModifiedFlag /*= TRUE*/)
{
	ASSERT(nCrlfStyle == CRLF_STYLE_AUTOMATIC || nCrlfStyle == CRLF_STYLE_DOS||
			nCrlfStyle == CRLF_STYLE_UNIX || nCrlfStyle == CRLF_STYLE_MAC);
	ASSERT(m_bInit);
	HANDLE hTempFile = INVALID_HANDLE_VALUE;
	HANDLE hSearch = INVALID_HANDLE_VALUE;
	TCHAR szTempFileDir[_MAX_PATH + 1];
	TCHAR szTempFileName[_MAX_PATH + 1];
	TCHAR szBackupFileName[_MAX_PATH + 1];
	DWORD result = 1;
	__try
	{
		TCHAR drive[_MAX_PATH], dir[_MAX_PATH], name[_MAX_PATH], ext[_MAX_PATH];
#ifdef _UNICODE
		_wsplitpath(pszFileName, drive, dir, name, ext);
#else
		_splitpath(pszFileName, drive, dir, name, ext);
#endif
		lstrcpy(szTempFileDir, drive);
		lstrcat(szTempFileDir, dir);
		lstrcpy(szBackupFileName, pszFileName);
		lstrcat(szBackupFileName, _T(".bak"));

		if (::GetTempFileName(szTempFileDir, _T("CRE"), 0, szTempFileName) == 0)
			__leave;

		hTempFile = ::CreateFile(szTempFileName, GENERIC_WRITE, 0, NULL,
					CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hTempFile == INVALID_HANDLE_VALUE)
			__leave;

		if (nCrlfStyle == CRLF_STYLE_AUTOMATIC)
			nCrlfStyle = m_nCRLFMode;

		ASSERT(nCrlfStyle >= 0 && nCrlfStyle <= 2);
		const char *pszCRLF = crlfs[nCrlfStyle];
		int nCRLFLength = strlen(pszCRLF);

		int nLineCount = m_aLines.GetSize();
		USES_CONVERSION;
		for (int nLine = 0; nLine < nLineCount; nLine ++)
		{
			int nLength = m_aLines[nLine].GetLength();
			DWORD dwWrittenBytes;
			if (nLength > 0)
			{
				if (! ::WriteFile(hTempFile, T2A(m_aLines[nLine].m_pcLine), nLength, &dwWrittenBytes, NULL))
					__leave;
				if (nLength != (int) dwWrittenBytes)
					__leave;
			}
			if (nLine < nLineCount - 1)	//	Last line must not end with CRLF
			{
				if (! ::WriteFile(hTempFile, pszCRLF, nCRLFLength, &dwWrittenBytes, NULL))
					__leave;
				if (nCRLFLength != (int) dwWrittenBytes)
					__leave;
			}
		}
		::CloseHandle(hTempFile);
		hTempFile = INVALID_HANDLE_VALUE;

		if (m_bCreateBackupFile)
		{
			WIN32_FIND_DATA wfd;
			hSearch = ::FindFirstFile(pszFileName, &wfd);
			if (hSearch != INVALID_HANDLE_VALUE)
			{
				//	File exist - create backup file
				::DeleteFile(szBackupFileName);
				if (! ::MoveFile(pszFileName, szBackupFileName))
					__leave;
				::FindClose(hSearch);
				hSearch = INVALID_HANDLE_VALUE;
			}
		}
		else
		{
			::DeleteFile(pszFileName);
		}

		//	Move temporary file to target name
		if (! ::MoveFile(szTempFileName, pszFileName))
			__leave;

		if (bClearModifiedFlag)
		{
			SetModified(FALSE);
			m_nSyncPosition = m_nUndoPosition;
		}
		result = 0;
	}
	__finally
	{
		if ( result )
			result = ::GetLastError();
		if (hSearch != INVALID_HANDLE_VALUE)
			::FindClose(hSearch);
		if (hTempFile != INVALID_HANDLE_VALUE)
			::CloseHandle(hTempFile);
		::DeleteFile(szTempFileName);
	}
	return result;
}

int CCrystalTextBuffer::GetCRLFMode()
{
	return m_nCRLFMode;
}

void CCrystalTextBuffer::SetCRLFMode(int nCRLFMode)
{
	ASSERT(nCRLFMode == CRLF_STYLE_DOS||
			nCRLFMode == CRLF_STYLE_UNIX ||
			nCRLFMode == CRLF_STYLE_MAC);
	m_nCRLFMode = nCRLFMode;
}

int CCrystalTextBuffer::GetLineCount()
{
	ASSERT(m_bInit);	//	Text buffer not yet initialized.
						//	You must call InitNew() or LoadFromFile() first!
	return m_aLines.GetSize();
}

int CCrystalTextBuffer::GetLineLength(int nLine)
{
	ASSERT(m_bInit);	//	Text buffer not yet initialized.
						//	You must call InitNew() or LoadFromFile() first!
	return m_aLines[nLine].GetLength();
}

LPTSTR CCrystalTextBuffer::GetLineChars(int nLine)
{
	ASSERT(m_bInit);	//	Text buffer not yet initialized.
						//	You must call InitNew() or LoadFromFile() first!
	return m_aLines[nLine].m_pcLine;
}

DWORD CCrystalTextBuffer::GetLineFlags(int nLine)
{
	ASSERT(m_bInit);	//	Text buffer not yet initialized.
						//	You must call InitNew() or LoadFromFile() first!
	return m_aLines[nLine].GetFlags();
}

static int FlagToIndex(DWORD dwFlag)
{
	int nIndex = 0;
	while ((dwFlag & 1) == 0)
	{
		dwFlag = dwFlag >> 1;
		nIndex ++;
		if (nIndex == 32)
			return -1;
	}
	dwFlag = dwFlag & 0xFFFFFFFE;
	if (dwFlag != 0)
		return -1;
	return nIndex;

}

int CCrystalTextBuffer::FindLineWithFlag(DWORD dwFlag)
{
	int nSize = m_aLines.GetSize();
	for (int L = 0; L < nSize; L ++)
	{
		if ((m_aLines[L].GetFlags() & dwFlag) != 0)
			return L;
	}
	return -1;
}

int CCrystalTextBuffer::GetLineWithFlag(DWORD dwFlag)
{
	int nFlagIndex = ::FlagToIndex(dwFlag);
	if (nFlagIndex < 0)
	{
		ASSERT(FALSE);		//	Invalid flag passed in
		return -1;
	}
	return FindLineWithFlag(dwFlag);
}

void CCrystalTextBuffer::SetLineFlag(int nLine, DWORD dwFlag, BOOL bSet, BOOL bRemoveFromPreviousLine /*= TRUE*/)
{
	ASSERT(m_bInit);	//	Text buffer not yet initialized.
						//	You must call InitNew() or LoadFromFile() first!

	int nFlagIndex = ::FlagToIndex(dwFlag);
	if (nFlagIndex < 0)
	{
		ASSERT(FALSE);		//	Invalid flag passed in
		return;
	}

	if (nLine == -1)
	{
		ASSERT(! bSet);
		nLine = FindLineWithFlag(dwFlag);
		if (nLine == -1)
			return;
		bRemoveFromPreviousLine = FALSE;
	}

	DWORD dwNewFlags = m_aLines[nLine].GetFlags();
	if (bSet)
		dwNewFlags = dwNewFlags | dwFlag;
	else
		dwNewFlags = dwNewFlags & ~dwFlag;

	if (m_aLines[nLine].GetFlags() != dwNewFlags)
	{
		if (bRemoveFromPreviousLine)
		{
			int nPrevLine = FindLineWithFlag(dwFlag);
			if (bSet)
			{
				if (nPrevLine >= 0)
				{
					ASSERT((m_aLines[nPrevLine].GetFlags() & dwFlag) != 0);
					//m_aLines[nPrevLine].m_dwFlags &= ~dwFlag;
					m_aLines[nPrevLine].SetFlags(m_aLines[nPrevLine].GetFlags() & ~dwFlag);
					UpdateViews(NULL, NULL, UPDATE_SINGLELINE | UPDATE_FLAGSONLY, nPrevLine);
				}
			}
			else
			{
				ASSERT(nPrevLine == nLine);
			}
		}

		m_aLines[nLine].SetFlags(dwNewFlags);
		UpdateViews(NULL, NULL, UPDATE_SINGLELINE | UPDATE_FLAGSONLY, nLine);
	}
}

void CCrystalTextBuffer::GetText(int nStartLine, int nStartChar, int nEndLine, int nEndChar, CString &text, LPCTSTR pszCRLF /*= NULL*/)
{
	ASSERT(m_bInit);	//	Text buffer not yet initialized.
						//	You must call InitNew() or LoadFromFile() first!
	ASSERT(nStartLine >= 0 && nStartLine < m_aLines.GetSize());
	ASSERT(nStartChar >= 0 && nStartChar <= m_aLines[nStartLine].GetLength());
	ASSERT(nEndLine >= 0 && nEndLine < m_aLines.GetSize());
	ASSERT(nEndChar >= 0 && nEndChar <= m_aLines[nEndLine].GetLength());
	ASSERT(nStartLine < nEndLine || nStartLine == nEndLine && nStartChar < nEndChar);
	
	if (pszCRLF == NULL)
		pszCRLF = crlf;
	int nCRLFLength = lstrlen(pszCRLF);
	ASSERT(nCRLFLength > 0);

	int nBufSize = 0;
	for (int L = nStartLine; L <= nEndLine; L ++)
	{
		nBufSize += m_aLines[L].GetLength();
		nBufSize += nCRLFLength;
	}

	LPTSTR pszBuf = text.GetBuffer(nBufSize);
	LPTSTR pszCurPos = pszBuf;

	if (nStartLine < nEndLine)
	{
		int nCount = m_aLines[nStartLine].GetLength() - nStartChar;
		if (nCount > 0)
		{
			memcpy(pszBuf, m_aLines[nStartLine].m_pcLine + nStartChar, sizeof(TCHAR) * nCount);
			pszBuf += nCount;
		}
		memcpy(pszBuf, pszCRLF, sizeof(TCHAR) * nCRLFLength);
		pszBuf += nCRLFLength;
		for (int I = nStartLine + 1; I < nEndLine; I ++)
		{
			nCount = m_aLines[I].GetLength();
			if (nCount > 0)
			{
				memcpy(pszBuf, m_aLines[I].m_pcLine, sizeof(TCHAR) * nCount);
				pszBuf += nCount;
			}
			memcpy(pszBuf, pszCRLF, sizeof(TCHAR) * nCRLFLength);
			pszBuf += nCRLFLength;
		}
		if (nEndChar > 0)
		{
			memcpy(pszBuf, m_aLines[nEndLine].m_pcLine, sizeof(TCHAR) * nEndChar);
			pszBuf += nEndChar;
		}
	}
	else
	{
		int nCount = nEndChar - nStartChar;
		memcpy(pszBuf, m_aLines[nStartLine].m_pcLine + nStartChar, sizeof(TCHAR) * nCount);
		pszBuf += nCount;
	}
	pszBuf[0] = _T('\0');
	text.ReleaseBuffer();
	text.FreeExtra();
}

void CCrystalTextBuffer::AddView(CCrystalTextView *pView)
{
	m_lpViews.AddTail(pView);
}

void CCrystalTextBuffer::RemoveView(CCrystalTextView *pView)
{
	POSITION pos = m_lpViews.GetHeadPosition();
	while (pos != NULL)
	{
		POSITION thispos = pos;
		CCrystalTextView *pvw = m_lpViews.GetNext(pos);
		if (pvw == pView)
		{
			m_lpViews.RemoveAt(thispos);
			return;
		}
	}
	ASSERT(FALSE);
}

void CCrystalTextBuffer::UpdateViews(CCrystalTextView *pSource, CUpdateContext *pContext, DWORD dwUpdateFlags, int nLineIndex /*= -1*/)
{
	POSITION pos = m_lpViews.GetHeadPosition();
	while (pos != NULL)
	{
		CCrystalTextView *pView = m_lpViews.GetNext(pos);
		pView->UpdateView(pSource, pContext, dwUpdateFlags, nLineIndex);
	}
}

BOOL CCrystalTextBuffer::InternalDeleteText(CCrystalTextView *pSource, int nStartLine, int nStartChar, int nEndLine, int nEndChar)
{
	ASSERT(m_bInit);	//	Text buffer not yet initialized.
						//	You must call InitNew() or LoadFromFile() first!
	ASSERT(nStartLine >= 0 && nStartLine < m_aLines.GetSize());
	ASSERT(nStartChar >= 0 && nStartChar <= m_aLines[nStartLine].GetLength());
	ASSERT(nEndLine >= 0 && nEndLine < m_aLines.GetSize());
	ASSERT(nEndChar >= 0 && nEndChar <= m_aLines[nEndLine].GetLength());
	ASSERT(nStartLine < nEndLine || nStartLine == nEndLine && nStartChar < nEndChar);
	if (m_bReadOnly)
		return FALSE;

	CDeleteContext context;
	context.m_ptStart.y = nStartLine;
	context.m_ptStart.x = nStartChar;
	context.m_ptEnd.y = nEndLine;
	context.m_ptEnd.x = nEndChar;
	if (nStartLine == nEndLine)
	{
		m_aLines[nStartLine].RemoveText(nStartChar, nEndChar-nStartChar);
		UpdateViews(pSource, &context, UPDATE_SINGLELINE | UPDATE_HORZRANGE, nStartLine);
	}
	else
	{
		CLineInfo &start = m_aLines[nStartLine];
		CLineInfo &end = m_aLines[nEndLine];
		int nFirstDelLine = nStartLine;
		if (nStartChar)
		{
			// Append the right portion of the last line to the first line
			start.TrimText(nStartChar);
			start.Append(m_aLines[nEndLine].m_pcLine+nEndChar);
			++nFirstDelLine;
			++nEndLine;
		}
		else if (end.GetLength() == 0 || nEndChar != end.GetLength())
		{
			// First line is being deleted so remove the left portion of last line, if any
			if (nEndChar)
				end.RemoveText(0, nEndChar);
		}
		else
		{
			// First and last lines are being completly deleted
			++nEndLine;
		}

		for (int i = nFirstDelLine; i < nEndLine; ++i)
			m_aLines[i].Destroy();

		int nDelCount = nEndLine - nFirstDelLine;
		m_aLines.RemoveAt(nFirstDelLine, nDelCount);

		UpdateViews(pSource, &context, UPDATE_HORZRANGE | UPDATE_VERTRANGE, nStartLine);
	}

	if (! m_bModified)
		SetModified(TRUE);

	//BEGIN SW
	// remember current cursor position as last editing position
	m_ptLastChange = context.m_ptStart;
	//END SW

	return TRUE;
}

BOOL CCrystalTextBuffer::InternalInsertText(CCrystalTextView *pSource, int nLine, int nPos, LPCTSTR pszText, int &nEndLine, int &nEndChar)
{
	ASSERT(m_bInit);	//	Text buffer not yet initialized.
						//	You must call InitNew() or LoadFromFile() first!
	ASSERT(nLine >= 0 && nLine < m_aLines.GetSize());
	ASSERT(nPos >= 0 && nPos <= m_aLines[nLine].GetLength());
	if (m_bReadOnly)
		return FALSE;

	CInsertContext context;
	context.m_ptStart.x = nPos;
	context.m_ptStart.y = nLine;

	CLineInfo &li = m_aLines[nLine];
	int nRestCount = li.GetLength() - nPos;
	LPTSTR pszRestChars = NULL;
	if (nRestCount > 0)
	{
		// Save the right portion on the first insert line
		pszRestChars = (TCHAR*) _alloca(sizeof(TCHAR)*nRestCount);
		_tcsncpy(pszRestChars, li.m_pcLine+nPos, nRestCount);
		li.TrimText(nPos);
	}

	int nCurrentLine = nLine;
	BOOL bNewLines = FALSE;
	int nTextPos;
	for (;;)
	{
		nTextPos = 0;
		while (pszText[nTextPos] != _T('\0') && pszText[nTextPos] != _T('\r'))
		{
			ASSERT(pszText[nTextPos] != _T('\n')); // invalid string format
			nTextPos ++;
		}

		if (nCurrentLine == nLine)
		{
			m_aLines[nLine].Append(pszText, nTextPos);
		}
		else
		{
			InsertLine(pszText, nTextPos, nCurrentLine);
			bNewLines = TRUE;
		}

		if (pszText[nTextPos] == _T('\0'))
		{
			CLineInfo &currentLine = m_aLines[nCurrentLine];
			nEndLine = nCurrentLine;
			nEndChar = currentLine.GetLength();
			currentLine.Append(pszRestChars, nRestCount);
			break;
		}

		nCurrentLine ++;
		nTextPos ++;

		if (pszText[nTextPos] == _T('\n'))
		{
			nTextPos ++;
		}
		else
		{
			ASSERT(FALSE);			//	Invalid line-end format passed
		}

		pszText += nTextPos;
	}

	context.m_ptEnd.x = nEndChar;
	context.m_ptEnd.y = nEndLine;

	if (bNewLines)
		UpdateViews(pSource, &context, UPDATE_HORZRANGE | UPDATE_VERTRANGE, nLine);
	else
		UpdateViews(pSource, &context, UPDATE_SINGLELINE | UPDATE_HORZRANGE, nLine);

	if (! m_bModified)
		SetModified(TRUE);

	//BEGIN SW
	// remember current cursor position as last editing position
	m_ptLastChange = context.m_ptEnd;
	//END SW

	return TRUE;
}

BOOL CCrystalTextBuffer::CanUndo()
{
	ASSERT(m_nUndoPosition >= 0 && m_nUndoPosition <= m_aUndoBuf.GetSize());
	return m_nUndoPosition > 0;
}

BOOL CCrystalTextBuffer::CanRedo()
{
	ASSERT(m_nUndoPosition >= 0 && m_nUndoPosition <= m_aUndoBuf.GetSize());
	return m_nUndoPosition < m_aUndoBuf.GetSize();
}

POSITION CCrystalTextBuffer::GetUndoDescription(CString &desc, POSITION pos /*= NULL*/)
{
	ASSERT(CanUndo());		//	Please call CanUndo() first
	ASSERT((m_aUndoBuf[0].m_dwFlags & UNDO_BEGINGROUP) != 0);

	int nPosition;
	if (pos == NULL)
	{
		//	Start from beginning
		nPosition = m_nUndoPosition;
	}
	else
	{
		nPosition = (int) pos;
		ASSERT(nPosition > 0 && nPosition < m_nUndoPosition);
		ASSERT((m_aUndoBuf[nPosition].m_dwFlags & UNDO_BEGINGROUP) != 0);
	}

	//	Advance to next undo group
	nPosition --;
	while ((m_aUndoBuf[nPosition].m_dwFlags & UNDO_BEGINGROUP) == 0)
		nPosition --;

	//	Read description
	if (! GetActionDescription(m_aUndoBuf[nPosition].m_nAction, desc))
		desc.Empty();		//	Use empty string as description

	//	Now, if we stop at zero position, this will be the last action,
	//	since we return (POSITION) nPosition
	return (POSITION) nPosition;
}

POSITION CCrystalTextBuffer::GetRedoDescription(CString &desc, POSITION pos /*= NULL*/)
{
	ASSERT(CanRedo());		//	Please call CanRedo() before!
	ASSERT((m_aUndoBuf[0].m_dwFlags & UNDO_BEGINGROUP) != 0);
	ASSERT((m_aUndoBuf[m_nUndoPosition].m_dwFlags & UNDO_BEGINGROUP) != 0);

	int nPosition;
	if (pos == NULL)
	{
		//	Start from beginning
		nPosition = m_nUndoPosition;
	}
	else
	{
		nPosition = (int) pos;
		ASSERT(nPosition > m_nUndoPosition);
		ASSERT((m_aUndoBuf[nPosition].m_dwFlags & UNDO_BEGINGROUP) != 0);
	}

	//	Read description
	if (! GetActionDescription(m_aUndoBuf[nPosition].m_nAction, desc))
		desc.Empty();		//	Use empty string as description

	//	Advance to next undo group
	nPosition ++;
	while (nPosition < m_aUndoBuf.GetSize() &&
		(m_aUndoBuf[nPosition].m_dwFlags & UNDO_BEGINGROUP) == 0)
		nPosition --;

	if (nPosition >= m_aUndoBuf.GetSize())
		return NULL;	//	No more redo actions!
	return (POSITION) nPosition;
}

BOOL CCrystalTextBuffer::Undo(CPoint &ptCursorPos)
{
	ASSERT(CanUndo());
	ASSERT((m_aUndoBuf[0].m_dwFlags & UNDO_BEGINGROUP) != 0);
	for (;;)
	{
		m_nUndoPosition --;
		const SUndoRecord &ur = m_aUndoBuf[m_nUndoPosition];
		if (ur.m_dwFlags & UNDO_INSERT)
		{
#ifdef _ADVANCED_BUGCHECK
			//	Try to ensure that we undoing correctly...
			//	Just compare the text as it was before Undo operation
			CString text;
			GetText(ur.m_ptStartPos.y, ur.m_ptStartPos.x, ur.m_ptEndPos.y, ur.m_ptEndPos.x, text);
			ASSERT(lstrcmp(text, ur.GetText()) == 0);
#endif
			VERIFY(InternalDeleteText(NULL, ur.m_ptStartPos.y, ur.m_ptStartPos.x, ur.m_ptEndPos.y, ur.m_ptEndPos.x));
			ptCursorPos = ur.m_ptStartPos;
		}
		else
		{
			int nEndLine, nEndChar;
			VERIFY(InternalInsertText(NULL, ur.m_ptStartPos.y, ur.m_ptStartPos.x, ur.GetText(), nEndLine, nEndChar));
#ifdef _ADVANCED_BUGCHECK
			ASSERT(ur.m_ptEndPos.y == nEndLine);
			ASSERT(ur.m_ptEndPos.x == nEndChar);
#endif
			ptCursorPos = ur.m_ptEndPos;
		}
		if (ur.m_dwFlags & UNDO_BEGINGROUP)
			break;
	}
	if (m_bModified && m_nSyncPosition == m_nUndoPosition)
		SetModified(FALSE);
	if (! m_bModified && m_nSyncPosition != m_nUndoPosition)
		SetModified(TRUE);
	return TRUE;
}

BOOL CCrystalTextBuffer::Redo(CPoint &ptCursorPos)
{
	ASSERT(CanRedo());
	ASSERT((m_aUndoBuf[0].m_dwFlags & UNDO_BEGINGROUP) != 0);
	ASSERT((m_aUndoBuf[m_nUndoPosition].m_dwFlags & UNDO_BEGINGROUP) != 0);
	for (;;)
	{
		const SUndoRecord &ur = m_aUndoBuf[m_nUndoPosition];
		if (ur.m_dwFlags & UNDO_INSERT)
		{
			int nEndLine, nEndChar;
			VERIFY(InternalInsertText(NULL, ur.m_ptStartPos.y, ur.m_ptStartPos.x, ur.GetText(), nEndLine, nEndChar));
#ifdef _ADVANCED_BUGCHECK
			ASSERT(ur.m_ptEndPos.y == nEndLine);
			ASSERT(ur.m_ptEndPos.x == nEndChar);
#endif
			ptCursorPos = ur.m_ptEndPos;
		}
		else
		{
#ifdef _ADVANCED_BUGCHECK
			CString text;
			GetText(ur.m_ptStartPos.y, ur.m_ptStartPos.x, ur.m_ptEndPos.y, ur.m_ptEndPos.x, text);
			ASSERT(lstrcmp(text, ur.GetText()) == 0);
#endif
			VERIFY(InternalDeleteText(NULL, ur.m_ptStartPos.y, ur.m_ptStartPos.x, ur.m_ptEndPos.y, ur.m_ptEndPos.x));
			ptCursorPos = ur.m_ptStartPos;
		}
		m_nUndoPosition ++;
		if (m_nUndoPosition == m_aUndoBuf.GetSize())
			break;
		if ((m_aUndoBuf[m_nUndoPosition].m_dwFlags & UNDO_BEGINGROUP) != 0)
			break;
	}
	if (m_bModified && m_nSyncPosition == m_nUndoPosition)
		SetModified(FALSE);
	if (! m_bModified && m_nSyncPosition != m_nUndoPosition)
		SetModified(TRUE);
	return TRUE;
}

//	[JRT] Support For Descriptions On Undo/Redo Actions
void CCrystalTextBuffer::AddUndoRecord(BOOL bInsert, const CPoint &ptStartPos, const CPoint &ptEndPos, LPCTSTR pszText, int nActionType)
{
	//	Forgot to call BeginUndoGroup()?
	ASSERT(m_bUndoGroup);
	ASSERT(m_aUndoBuf.GetSize() == 0 || (m_aUndoBuf[0].m_dwFlags & UNDO_BEGINGROUP) != 0);

	//	Strip unnecessary undo records (edit after undo)
	int nBufSize = m_aUndoBuf.GetSize();
	if (m_nUndoPosition < nBufSize)
	{
		for (int I = m_nUndoPosition; I < nBufSize; I++)
			m_aUndoBuf[I].FreeText();
		m_aUndoBuf.SetSize(m_nUndoPosition);
	}

	//	If undo buffer size is close to critical, remove the oldest records
	ASSERT(m_aUndoBuf.GetSize() <= m_nUndoBufSize);
	nBufSize = m_aUndoBuf.GetSize();
	if (nBufSize >= m_nUndoBufSize)
	{
		int nIndex = 0;
		for (;;)
		{
			m_aUndoBuf[nIndex].FreeText();
			nIndex ++;
			if (nIndex == nBufSize || (m_aUndoBuf[nIndex].m_dwFlags & UNDO_BEGINGROUP) != 0)
				break;
		}
		m_aUndoBuf.RemoveAt(0, nIndex);
	}
	ASSERT(m_aUndoBuf.GetSize() < m_nUndoBufSize);

	//	Add new record
	SUndoRecord ur;
	ur.m_dwFlags = bInsert ? UNDO_INSERT : 0;
	ur.m_nAction = nActionType;
	if (m_bUndoBeginGroup)
	{
		ur.m_dwFlags |= UNDO_BEGINGROUP;
		m_bUndoBeginGroup = FALSE;
	}
	ur.m_ptStartPos = ptStartPos;
	ur.m_ptEndPos = ptEndPos;
	ur.SetText(pszText);

	m_aUndoBuf.Add(ur);
	m_nUndoPosition = m_aUndoBuf.GetSize();

	ASSERT(m_aUndoBuf.GetSize() <= m_nUndoBufSize);
}

BOOL CCrystalTextBuffer::InsertText(CCrystalTextView *pSource, int nLine, int nPos, LPCTSTR pszText, 
									int &nEndLine, int &nEndChar, int nAction)
{
	if (! InternalInsertText(pSource, nLine, nPos, pszText, nEndLine, nEndChar))
		return FALSE;

	BOOL bGroupFlag = FALSE;
	if (! m_bUndoGroup)
	{
		BeginUndoGroup();
		bGroupFlag = TRUE;
	}
	AddUndoRecord(TRUE, CPoint(nPos, nLine), CPoint(nEndChar, nEndLine), pszText, nAction);
	if (bGroupFlag)
		FlushUndoGroup(pSource);
	return TRUE;
}

BOOL CCrystalTextBuffer::DeleteText(CCrystalTextView *pSource, int nStartLine, int nStartChar, 
									int nEndLine, int nEndChar, int nAction)
{
	CString sTextToDelete;
	GetText(nStartLine, nStartChar, nEndLine, nEndChar, sTextToDelete);

	if (! InternalDeleteText(pSource, nStartLine, nStartChar, nEndLine, nEndChar))
		return FALSE;

	BOOL bGroupFlag = FALSE;
	if (! m_bUndoGroup)
	{
		BeginUndoGroup();
		bGroupFlag = TRUE;
	}
	AddUndoRecord(FALSE, CPoint(nStartChar, nStartLine), CPoint(nEndChar, nEndLine), sTextToDelete, nAction);
	if (bGroupFlag)
		FlushUndoGroup(pSource);
	return TRUE;
}

BOOL CCrystalTextBuffer::GetActionDescription(int nAction, CString &desc)
{
	HINSTANCE hOldResHandle = AfxGetResourceHandle();
#ifdef CRYSEDIT_RES_HANDLE
	AfxSetResourceHandle(CRYSEDIT_RES_HANDLE);
#else
	if (CCrystalTextView::s_hResourceInst != NULL)
		AfxSetResourceHandle(CCrystalTextView::s_hResourceInst);
#endif
	BOOL bSuccess = FALSE;
	switch (nAction)
	{
	case CE_ACTION_UNKNOWN:
		bSuccess = desc.LoadString(IDS_EDITOP_UNKNOWN);
		break;
	case CE_ACTION_PASTE:
		bSuccess = desc.LoadString(IDS_EDITOP_PASTE);
		break;
	case CE_ACTION_DELSEL:
		bSuccess = desc.LoadString(IDS_EDITOP_DELSELECTION);
		break;
	case CE_ACTION_CUT:
		bSuccess = desc.LoadString(IDS_EDITOP_CUT);
		break;
	case CE_ACTION_TYPING:
		bSuccess = desc.LoadString(IDS_EDITOP_TYPING);
		break;
	case CE_ACTION_BACKSPACE:
		bSuccess = desc.LoadString(IDS_EDITOP_BACKSPACE);
		break;
	case CE_ACTION_INDENT:
		bSuccess = desc.LoadString(IDS_EDITOP_INDENT);
		break;
	case CE_ACTION_DRAGDROP:
		bSuccess = desc.LoadString(IDS_EDITOP_DRAGDROP);
		break;
	case CE_ACTION_REPLACE:
		bSuccess = desc.LoadString(IDS_EDITOP_REPLACE);
		break;
	case CE_ACTION_DELETE:
		bSuccess = desc.LoadString(IDS_EDITOP_DELETE);
		break;
	case CE_ACTION_AUTOINDENT:
		bSuccess = desc.LoadString(IDS_EDITOP_AUTOINDENT);
		break;
	}
	AfxSetResourceHandle(hOldResHandle);
	return bSuccess;
}

void CCrystalTextBuffer::SetModified(BOOL bModified /*= TRUE*/)
{
	m_bModified = bModified;
}

void CCrystalTextBuffer::BeginUndoGroup(BOOL bMergeWithPrevious /*= FALSE*/)
{
	ASSERT(! m_bUndoGroup);
	m_bUndoGroup = TRUE;
	m_bUndoBeginGroup = m_nUndoPosition == 0 || ! bMergeWithPrevious;
}

void CCrystalTextBuffer::FlushUndoGroup(CCrystalTextView *pSource)
{
	ASSERT(m_bUndoGroup);
	if (pSource != NULL)
	{
		ASSERT(m_nUndoPosition == m_aUndoBuf.GetSize());
		if (m_nUndoPosition > 0)
		{
			m_bUndoBeginGroup = TRUE;
			pSource->OnEditOperation(m_aUndoBuf[m_nUndoPosition - 1].m_nAction, m_aUndoBuf[m_nUndoPosition - 1].GetText());
		}
	}
	m_bUndoGroup = FALSE;
}

int CCrystalTextBuffer::FindNextBookmarkLine(int nCurrentLine)
{
	BOOL bWrapIt = TRUE;
	DWORD dwFlags = GetLineFlags(nCurrentLine);
	if ((dwFlags & LF_BOOKMARKS) != 0)
		nCurrentLine++;

	int nSize = m_aLines.GetSize();
	for (;;)
	{ 
		while (nCurrentLine < nSize)
		{
			if ((m_aLines[nCurrentLine].GetFlags() & LF_BOOKMARKS) != 0)
				return nCurrentLine;
			// Keep going
			nCurrentLine++;
		}
		// End of text reached
		if (!bWrapIt)
			return -1;

		// Start from the beginning of text
		bWrapIt = FALSE;
		nCurrentLine = 0;
	}
	return -1;
}

int CCrystalTextBuffer::FindPrevBookmarkLine(int nCurrentLine)
{
	BOOL bWrapIt = TRUE;
	DWORD dwFlags = GetLineFlags(nCurrentLine);
	if ((dwFlags & LF_BOOKMARKS) != 0)
		nCurrentLine--;

	int nSize = m_aLines.GetSize();
	for (;;)
	{ 
		while (nCurrentLine >= 0)
		{
			if ((m_aLines[nCurrentLine].GetFlags() & LF_BOOKMARKS) != 0)
				return nCurrentLine;
			// Keep moving up
			nCurrentLine--;
		}
		// Beginning of text reached
		if (!bWrapIt)
			return -1;

		// Start from the end of text
		bWrapIt = FALSE;
		nCurrentLine = nSize - 1;
	}
	return -1;
}

//BEGIN SW
CPoint CCrystalTextBuffer::GetLastChangePos() const
{
	return m_ptLastChange;
}
//END SW