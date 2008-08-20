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
#include "TeXnicCenter.h"

#include "FontOccManager.h"
#include "AboutBox.h"


#define SCROLLAMOUNT				-1
#define DISPLAY_SLOW				70
#define DISPLAY_MEDIUM			40
#define DISPLAY_FAST				10
#define DISPLAY_SPEED				DISPLAY_FAST

#define RED					RGB(255,0,0)
#define GREEN				RGB(0,255,0)
#define BLUE				RGB(0,0,255)
#define DARKBLUE		RGB(0x33,0x66,0x99)
#define WHITE   		RGB(255,255,255)
#define YELLOW  		RGB(255,255,0)
#define TURQUOISE 	RGB(0,255,255)
#define PURPLE  		RGB(255,0,255)
#define BLACK       RGB(0,0,0)

#define BACKGROUND_COLOR        WHITE
#define TOP_LEVEL_TITLE_COLOR		DARKBLUE
#define TOP_LEVEL_GROUP_COLOR   DARKBLUE
#define GROUP_TITLE_COLOR       DARKBLUE
#define NORMAL_TEXT_COLOR				DARKBLUE

// You can set font heights here to suit your taste
const int TOP_LEVEL_TITLE_HEIGHT = 21;
const int TOP_LEVEL_GROUP_HEIGHT =  19;
const int GROUP_TITLE_HEIGHT =    	17;
const int NORMAL_TEXT_HEIGHT = 15;



// these define the escape sequences to suffix each array line with. They
// are listed in order of descending font size.
const TCHAR TOP_LEVEL_TITLE     = _T('\n');
const TCHAR TOP_LEVEL_GROUP     =_T('\r');
const TCHAR GROUP_TITLE         = _T('\t');
const TCHAR NORMAL_TEXT         = _T('\f');
const TCHAR DISPLAY_BITMAP      = _T('\b');

// Enter the text and bitmap resource string identifiers which are to
// appear in the scrolling list. Append each non-empty string entry with
// an escape character corresponding to the desired font effect. Empty
// strings do not get a font assignment.
// (see #defines above)
//
// Array count. Make sure this keeps up with how many lines there are
// in the array!
LPCTSTR const pArrCredit[] =
{
	_T("Copyright © 1999-2008\t"),
	_T("www.ToolsCenter.org\t"),
	_T(""),
	_T("Portions Copyright © 1998-2002 by Cristi Posea\f"),
	_T(""),
	_T(""),
	_T("IDB_SEPARATOR\b"),
	_T(""),
	_T("Idea\r"),
	_T(""),
	_T("Sven Wiegand\f"),
	_T(""),
	_T(""),
	_T("IDB_SEPARATOR\b"),
	_T(""),
	_T("Development\r"),
	_T(""),
	_T("Main Code\t"),
	_T(""),
	_T("Sergiu Dotenco\f"),
	_T("Chris Norris\f"),
	_T("Stephan Schrader\f"),
	_T("Paul Selormey\f"),
	_T("Pavel Vacha\f"),
	_T("Tino Weinkauf\f"),
	_T("Christian Welzel\f"),
	_T("Sven Wiegand\f"),
	_T("Oliver Wieland\f"),
	_T(""),
	_T(""),
	_T("Scintilla Source Code Editing Component\t"),
	_T(""),
	_T("Neil Hodgson, et al.\f"),
	_T(""),
	_T(""),
	_T("Hunspell Spell Checker\t"),
	_T(""),
	_T("László Németh\f"),
	_T(""),
	_T(""),
	_T("Additional Codes\t"),
	_T(""),
	_T("Loïc d'Anterroches, Stefan Ascher, Adam Clauss, Michael Dunn\f"),
	_T("Raffi Enficiaud, Mark Findlay, Martin Stanek\f"),
	_T(""),
	_T(""),
	_T("IDB_SEPARATOR\b"),
	_T(""),
	_T("Online-Help & Tips\r"),
	_T(""),
	_T("Gerald Binder\f"),
	_T("Marckus Kraft\f"),
	_T("Rüdiger Stuible\f"),
	_T("Sven Wiegand\f"),
	_T(""),
	_T(""),
	_T("IDB_SEPARATOR\b"),
	_T(""),
	_T("TeXnicCenter Templates\r"),
	_T(""),
	_T("Christoph Börensen, Mark Müller, Gyorgy Szeidl, Tino Weinkauf\f"),
	_T(""),
	_T(""),
	_T("IDB_SEPARATOR\b"),
	_T(""),
	_T("Graphics\r"),
	_T(""),
	_T("Additional Graphics\t"),
	_T(""),
	_T("Felix Kaiser\f"),
	_T(""),
	_T(""),
	_T("IDB_SEPARATOR\b"),
	_T(""),
	_T("Many thanks to all the users at the TeXnicCenter-Users mailing list\f"),
	_T("for their tips and suggestions!\f"),
	_T(""),
	_T(""),
	_T("IDB_SEPARATOR\b"),
	_T(""),
	_T("TeXnicCenter is distributed under the terms of the\f"),
	_T("GNU General Public License\f")
	_T(""),
	_T(""),
	_T("IDB_SEPARATOR\b"),
	_T(""),
	_T("(All names are listed in alphabetical order)\f"),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T("")
};

const int ARRAYCOUNT = sizeof(pArrCredit)/sizeof(TCHAR*);

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
		,lf_(GetDisplayFont())
		, CopyrightText(_T(""))
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT

	// retrieve version and display
	CFileVersionInfo fvi;
	if (fvi.Create())
	{
		m_strVersion = fvi.GetProductVersion();
		CopyrightText = fvi.GetLegalCopyright();
	}
	else
	{
		m_strVersion = _T("Unknown");
		CopyrightText = _T("(c) 1999-2008 The TeXnicCenter Team");
	}
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX,IDC_URL_TOOLSCENTER,m_wndURLToolsCenter);
	DDX_Text(pDX,IDC_STATIC_VERSION,m_strVersion);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_COPYRIGHT, CopyrightText);
}

BEGIN_MESSAGE_MAP(CAboutDlg,CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// set URLs
	m_wndURLToolsCenter.SetURL(_T("http://www.ToolsCenter.org/"));
	m_wndURLToolsCenter.SizeToContent(TRUE,FALSE);

	// initialize scrolling credits
	BOOL bRet;
	UINT nRet;

	nCurrentFontHeight = NORMAL_TEXT_HEIGHT;

	CClientDC dc(this);
	bRet = m_dcMem.CreateCompatibleDC(&dc);

	m_bProcessingBitmap = FALSE;

	nArrIndex = 0;
	nCounter = 1;
	nClip = 0;
	m_bFirstTime = TRUE;
	m_bDrawText = FALSE;
	m_hBmpOld = 0;

	m_pDisplayFrame = (CWnd*) GetDlgItem(IDC_STATIC_CREDITS);

	// If you assert here, you did not assign your static display control
	// the IDC_ value that was used in the GetDlgItem(...). This is the
	// control that will display the credits.
	_ASSERTE(m_pDisplayFrame);

	m_pDisplayFrame->GetClientRect(&m_ScrollRect);

	nRet = SetTimer(DISPLAY_TIMER_ID,DISPLAY_SPEED,NULL);
	_ASSERTE(nRet != 0);

	return TRUE;
}

void CAboutDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	PAINTSTRUCT ps;
	CDC* pDc = m_pDisplayFrame->BeginPaint(&ps);

	pDc->SetBkMode(TRANSPARENT);

	//*********************************************************************
	// FONT SELECTION
	CFont m_fntArial;
	CFont* pOldFont;
	BOOL bSuccess;

	BYTE bUnderline;
	BYTE bItalic;

	if (!m_szWork.IsEmpty())
	{
		LOGFONT lf = lf_;

		switch (m_szWork[m_szWork.GetLength() - 1])
		{
			case NORMAL_TEXT:
			default:
				bItalic = FALSE;
				bUnderline = FALSE;
				nCurrentFontHeight = NORMAL_TEXT_HEIGHT;
				bSuccess = m_fntArial.CreateFont(NORMAL_TEXT_HEIGHT, 0, 0, 0,
				                                 FW_THIN, bItalic, bUnderline, 0,
				                                 ANSI_CHARSET,
				                                 OUT_DEFAULT_PRECIS,
				                                 CLIP_DEFAULT_PRECIS,
				                                 PROOF_QUALITY,
				                                 VARIABLE_PITCH | 0x04 | FF_DONTCARE,
				                                 lf.lfFaceName);
				pDc->SetTextColor(NORMAL_TEXT_COLOR);
				pOldFont = pDc->SelectObject(&m_fntArial);
				break;

			case TOP_LEVEL_GROUP:
				bItalic = FALSE;
				bUnderline = FALSE;
				nCurrentFontHeight = TOP_LEVEL_GROUP_HEIGHT;
				bSuccess = m_fntArial.CreateFont(TOP_LEVEL_GROUP_HEIGHT, 0, 0, 0,
				                                 FW_BOLD, bItalic, bUnderline, 0,
				                                 ANSI_CHARSET,
				                                 OUT_DEFAULT_PRECIS,
				                                 CLIP_DEFAULT_PRECIS,
				                                 PROOF_QUALITY,
				                                 VARIABLE_PITCH | 0x04 | FF_DONTCARE,
				                                 lf.lfFaceName);
				pDc->SetTextColor(TOP_LEVEL_GROUP_COLOR);
				pOldFont = pDc->SelectObject(&m_fntArial);
				break;



			case GROUP_TITLE:
				bItalic = FALSE;
				bUnderline = FALSE;
				nCurrentFontHeight = GROUP_TITLE_HEIGHT;
				bSuccess = m_fntArial.CreateFont(GROUP_TITLE_HEIGHT, 0, 0, 0,
				                                 FW_BOLD, bItalic, bUnderline, 0,
				                                 ANSI_CHARSET,
				                                 OUT_DEFAULT_PRECIS,
				                                 CLIP_DEFAULT_PRECIS,
				                                 PROOF_QUALITY,
				                                 VARIABLE_PITCH | 0x04 | FF_DONTCARE,
				                                 lf.lfFaceName);
				pDc->SetTextColor(GROUP_TITLE_COLOR);
				pOldFont = pDc->SelectObject(&m_fntArial);
				break;


			case TOP_LEVEL_TITLE:
				bItalic = FALSE;
				bUnderline = FALSE;
				nCurrentFontHeight = TOP_LEVEL_TITLE_HEIGHT;
				bSuccess = m_fntArial.CreateFont(TOP_LEVEL_TITLE_HEIGHT, 0, 0, 0,
				                                 FW_BOLD, bItalic, bUnderline, 0,
				                                 ANSI_CHARSET,
				                                 OUT_DEFAULT_PRECIS,
				                                 CLIP_DEFAULT_PRECIS,
				                                 PROOF_QUALITY,
				                                 VARIABLE_PITCH | 0x04 | FF_DONTCARE,
				                                 lf.lfFaceName);
				pDc->SetTextColor(TOP_LEVEL_TITLE_COLOR);
				pOldFont = pDc->SelectObject(&m_fntArial);
				break;

			case DISPLAY_BITMAP:
				if (!m_bProcessingBitmap)
				{
					CString szBitmap = m_szWork.Left(m_szWork.GetLength() - 1);

					if (!m_bmpWork.LoadBitmap((LPCTSTR) szBitmap))
					{
						CString str;
						str.Format(_T("Could not find bitmap resource \"%s\". ")
						           _T("Be sure to assign the bitmap a QUOTED resource name"),szBitmap);
						KillTimer(DISPLAY_TIMER_ID);
						AfxMessageBox(str);
						return;
					}

					m_bmpCurrent = &m_bmpWork;
					m_bmpCurrent->GetObject(sizeof(BITMAP),&m_bmpInfo);

					m_size.cx = m_bmpInfo.bmWidth; // width  of dest rect
					RECT workRect;
					m_pDisplayFrame->GetClientRect(&workRect);
					m_pDisplayFrame->ClientToScreen(&workRect);
					ScreenToClient(&workRect);
					// upper left point of dest
					m_pt.x = (workRect.right -
					          ((workRect.right - workRect.left) / 2) - (m_bmpInfo.bmWidth / 2));
					m_pt.y = workRect.bottom;

					pBmpOld = m_dcMem.SelectObject(m_bmpCurrent);

					if (m_hBmpOld == 0)
						m_hBmpOld = (HBITMAP) pBmpOld->GetSafeHandle();

					m_bProcessingBitmap = TRUE;
				}
				break;

		}
	}

	CBrush bBrush(BACKGROUND_COLOR);
	CBrush* pOldBrush;
	pOldBrush = pDc->SelectObject(&bBrush);
	// Only fill rect comprised of gap left by bottom of scrolling window
	r = m_ScrollRect;
	r.top = r.bottom - abs(SCROLLAMOUNT);
	pDc->DPtoLP(&r);

	if (m_bFirstTime)
	{
		m_bFirstTime = FALSE;
		pDc->FillRect(&m_ScrollRect,&bBrush);
	}
	else
		pDc->FillRect(&r,&bBrush);

	r = m_ScrollRect;
	r.top = r.bottom - nClip;

	if (!m_bProcessingBitmap)
	{
		/*int x = */
		pDc->DrawText((LPCTSTR) m_szWork,m_szWork.GetLength() - 1,&r,DT_TOP | DT_CENTER |
		              DT_NOPREFIX | DT_SINGLELINE);
		m_bDrawText = FALSE;
	}
	else
	{
		dc.StretchBlt(m_pt.x,m_pt.y - nClip,m_size.cx,nClip,
		              &m_dcMem,0,0,m_bmpInfo.bmWidth - 1,nClip,
		              SRCCOPY);
		if (nClip > m_bmpInfo.bmHeight)
		{
			m_bmpWork.DeleteObject();
			m_bProcessingBitmap = FALSE;
			nClip = 0;
			m_szWork.Empty();
			nCounter = 1;
		}
		pDc->SelectObject(pOldBrush);
		bBrush.DeleteObject();
		m_pDisplayFrame->EndPaint(&ps);
		return;
	}


	pDc->SelectObject(pOldBrush);
	bBrush.DeleteObject();

	if (!m_szWork.IsEmpty())
	{
		pDc->SelectObject(pOldFont);
		m_fntArial.DeleteObject();
	}

	m_pDisplayFrame->EndPaint(&ps);

	// Do not call CDialog::OnPaint() for painting messages
}

void CAboutDlg::OnTimer(UINT nIDEvent)
{
	if (nIDEvent != DISPLAY_TIMER_ID)
	{
		CDialog::OnTimer(nIDEvent);
		return;
	}

	if (!m_bProcessingBitmap)
	{
		if (nCounter++ % nCurrentFontHeight == 0) // every x timer events, show new line
		{
			nCounter = 1;
			m_szWork = pArrCredit[nArrIndex++];

			if (nArrIndex > ARRAYCOUNT - 1)
				nArrIndex = 0;
			nClip = 0;
			m_bDrawText = TRUE;
		}
	}

	m_pDisplayFrame->ScrollWindow(0,SCROLLAMOUNT,&m_ScrollRect,&m_ScrollRect);
	nClip = nClip + abs(SCROLLAMOUNT);

	CRect r;
	CWnd* pWnd = GetDlgItem(IDC_STATIC_CREDITS);
	ASSERT_VALID(pWnd);
	pWnd->GetClientRect(&r);
	pWnd->ClientToScreen(r);
	ScreenToClient(&r);
	InvalidateRect(r,FALSE); // FALSE does not erase background

	CDialog::OnTimer(nIDEvent);
}

void CAboutDlg::OnDestroy()
{
	CDialog::OnDestroy();

	m_dcMem.SelectObject(CBitmap::FromHandle(m_hBmpOld));
	m_bmpWork.DeleteObject();

}

HBRUSH CAboutDlg::OnCtlColor(CDC* /*pDC*/, CWnd* /*pWnd*/, UINT /*nCtlColor*/)
{
	return (HBRUSH) GetStockObject(WHITE_BRUSH);
}

void CAboutDlg::OnLButtonUp(UINT nFlags,CPoint point)
{
	CDialog::OnLButtonUp(nFlags,point);
	CDialog::EndDialog(IDOK);
}
