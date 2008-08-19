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

/********************************************************************
 *
 * $Id$
 *
 ********************************************************************/

#include "stdafx.h"
#include "resource.h"
#include "tipdlg.h"

#include "TeXnicCenter.h"
#include "RunTimeHelper.h"

#include <sys\stat.h>
#include <sys\types.h>

const int MAX_BUFLEN = 1000;

static const TCHAR szSection[] = _T("Tip");
static const TCHAR szIntFilePos[] = _T("FilePos");
static const TCHAR szTimeStamp[] = _T("TimeStamp");
static const TCHAR szIntStartup[] = _T("StartUp");

CTipDlg::CTipDlg(CWnd* pParent /*=NULL*/)
		: CDialog(IDD_TIP,pParent)
{
	//{{AFX_DATA_INIT(CTipDlg)
	m_bStartup = TRUE;
	//}}AFX_DATA_INIT

	// We need to find out what the startup and file position parameters are
	// If startup does not exist, we assume that the Tips on startup is checked TRUE.
	m_bStartup = !theApp.GetProfileInt(szSection,szIntStartup,0);
	UINT iFilePos = theApp.GetProfileInt(szSection,szIntFilePos,0);

	// Now try to open the tips file
	CString sPath;
	sPath = theApp.GetWorkingDir() + _T("\\") + CString((LPCTSTR) STE_TIP_FILE);

	m_pStream = _tfopen(sPath,_T("r"));
	if (m_pStream == NULL)
	{
		m_strTip.LoadString(CG_IDS_FILE_ABSENT);
		return;
	}

	// If the timestamp in the INI file is different from the timestamp of
	// the tips file, then we know that the tips file has been modified
	// Reset the file position to 0 and write the latest timestamp to the
	// ini file
	struct _stat buf;
	_fstat(_fileno(m_pStream),&buf);

	CString strCurrentTime = _tctime(&buf.st_ctime);
	strCurrentTime.TrimRight();
	CString strStoredTime =
	    theApp.GetProfileString(szSection,szTimeStamp,NULL);
	if (strCurrentTime != strStoredTime)
	{
		iFilePos = 0;
		theApp.WriteProfileString(szSection,szTimeStamp,strCurrentTime);
	}

	if (fseek(m_pStream,iFilePos,SEEK_SET) != 0)
	{
		AfxMessageBox(CG_IDS_FILE_CORRUPT);
	}
	else
	{
		GetNextTipString(m_strTip);
	}
}

CTipDlg::~CTipDlg()
{
	// This destructor is executed whether the user had pressed the escape key
	// or clicked on the close button. If the user had pressed the escape key,
	// it is still required to update the filepos in the ini file with the
	// latest position so that we don't repeat the tips!

	// But make sure the tips file existed in the first place....
	if (m_pStream != NULL)
	{
		CWinApp* pApp = AfxGetApp();
		pApp->WriteProfileInt(szSection,szIntFilePos,ftell(m_pStream));
		fclose(m_pStream);
	}
}

void CTipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTipDlg)
	DDX_Check(pDX,IDC_STARTUP,m_bStartup);
	//	DDX_Text(pDX, IDC_TIPSTRING, m_strTip);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTipDlg,CDialog)
	//{{AFX_MSG_MAP(CTipDlg)
	ON_BN_CLICKED(IDC_NEXTTIP,OnNextTip)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTipDlg message handlers

void CTipDlg::OnNextTip()
{
	GetNextTipString(m_strTip);
	UpdateData(FALSE);

	GetDlgItem(IDC_TIPSTRING)->SetWindowText(m_strTip);
}

void CTipDlg::GetNextTipString(CString& strNext)
{
	LPTSTR lpsz = strNext.GetBuffer(MAX_BUFLEN);

	// This routine identifies the next string that needs to be
	// read from the tips file
	BOOL bStop = FALSE;
	while (!bStop)
	{
		if (_fgetts(lpsz,MAX_BUFLEN,m_pStream) == NULL)
		{
			// We have either reached EOF or encountered some problem
			// In both cases reset the pointer to the beginning of the file
			// This behavior is same as VC++ Tips file
			if (fseek(m_pStream,0,SEEK_SET) != 0)
				AfxMessageBox(CG_IDS_FILE_CORRUPT);
		}
		else
		{
			if (*lpsz != _T(' ') && *lpsz != _T('\t') &&
			        *lpsz != _T('\n') && *lpsz != _T(';'))
			{
				// There should be no space at the beginning of the tip
				// This behavior is same as VC++ Tips file
				// Comment lines are ignored and they start with a semicolon
				bStop = TRUE;
			}
		}
	}
	strNext.ReleaseBuffer();

	// replace '\n' with newline and '\p' with double newline (paragraph)
	strNext.Remove(_T('\r'));
	strNext.Remove(_T('\n'));

	strNext.Replace(_T("\\n"),_T("\r\n"));
	strNext.Replace(_T("\\p"),_T("\r\n \r\n"));
}

HBRUSH CTipDlg::OnCtlColor(CDC* pDC,CWnd* pWnd,UINT nCtlColor)
{
	HBRUSH brush;
	int id = pWnd->GetDlgCtrlID();

	switch (id)
	{
		case IDC_TIPSTRING:
		case IDC_TITLE:
			brush = reinterpret_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));
			
			if (id == IDC_TITLE)
				pDC->SetTextColor(title_color_);

			break;
		default:
			brush = CDialog::OnCtlColor(pDC,pWnd,nCtlColor);
	}

	return brush;
}

void CTipDlg::OnOK()
{
	CDialog::OnOK();

	// Update the startup information stored in the INI file
	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileInt(szSection,szIntStartup,!m_bStartup);
}

BOOL CTipDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// If Tips file does not exist then disable NextTip
	if (m_pStream == NULL)
		GetDlgItem(IDC_NEXTTIP)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_TIPSTRING)->SetWindowText(m_strTip);

	CWindowDC dc(this);
	LOGFONT lf;

	if (!RunTimeHelper::IsVista())
	{
		GetFont()->GetLogFont(&lf);
		title_color_ = RGB(0,51,153);
	}
	else
	{
		HTHEME theme = ::OpenThemeData(m_hWnd,VSCLASS_TEXTSTYLE);
		ASSERT(theme);

#ifdef UNICODE
		::GetThemeFont(theme,dc,TEXT_MAININSTRUCTION,0,TMT_FONT,&lf);
#else
		LOGFONTW lfw;
		::GetThemeSysFont(theme,TMT_CAPTIONFONT,&lfw);

		CFont tf;
		tf.Attach(::CreateFontIndirectW(&lfw));
		tf.GetLogFont(&lf);
#endif
		if (FAILED(::GetThemeColor(theme,TEXT_MAININSTRUCTION,0,TMT_TEXTCOLOR,&title_color_)))
			title_color_ = 0;

		::CloseThemeData(theme);
	}

	lf.lfHeight = 135; // 13.5pt
	title_font_.CreatePointFontIndirect(&lf,&dc);

	GetDlgItem(IDC_TITLE)->SetFont(&title_font_);
	GetDlgItem(IDC_TITLE)->SetWindowText(CString(MAKEINTRESOURCE(CG_IDS_DIDYOUKNOW)));	

	return TRUE; // return TRUE unless you set the focus to a control
}

int CTipDlg::DoModal()
{
	if (m_pStream != NULL)
		return CDialog::DoModal();
	else
	{
		return IDOK;
	}
}
