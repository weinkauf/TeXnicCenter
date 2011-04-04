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

#include "stdafx.h"  // z.B. stdafx.h
#include "resource.h"  // z.B. resource.h

#include "Splash.h"  // z.B. splash.h
#include "RunTimeHelper.h"
#include "FontOccManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//   Klasse Begr��ungsbildschirm

//Statics
BOOL CSplashWnd::c_bShowSplashWnd;

std::auto_ptr<CSplashWnd> CSplashWnd::c_pSplashWnd;

CSplashWnd::CSplashWnd()
{
}

CSplashWnd::~CSplashWnd()
{
	// Statischen Fensterzeiger l�schen
	ASSERT(c_pSplashWnd.get() == this);
}

BEGIN_MESSAGE_MAP(CSplashWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CSplashWnd::EnableSplashScreen(BOOL bEnable /*= TRUE*/)
{
	c_bShowSplashWnd = bEnable;
}

void CSplashWnd::ShowSplashScreen(CWnd* pParentWnd /*= NULL*/)
{
	if (!c_bShowSplashWnd || c_pSplashWnd.get())
		return;

	// Neuen Begr��ungsbildschirm reservieren und erstellen
	c_pSplashWnd.reset(new CSplashWnd);

	if (!c_pSplashWnd->Create(pParentWnd))
		c_pSplashWnd.reset();
	else
		c_pSplashWnd->UpdateWindow();
}

BOOL CSplashWnd::PreTranslateAppMessage(MSG* pMsg)
{
	if (!c_pSplashWnd.get())
		return FALSE;

	// Begr��ungsbildschirm ausblenden, falls eine Tastatur- oder Mausnachricht empfangen wird.
	if (pMsg->message == WM_KEYDOWN ||
	        pMsg->message == WM_SYSKEYDOWN ||
	        pMsg->message == WM_LBUTTONDOWN ||
	        pMsg->message == WM_RBUTTONDOWN ||
	        pMsg->message == WM_MBUTTONDOWN ||
	        pMsg->message == WM_NCLBUTTONDOWN ||
	        pMsg->message == WM_NCRBUTTONDOWN ||
	        pMsg->message == WM_NCMBUTTONDOWN)
	{
		c_pSplashWnd->HideSplashScreen();
		return TRUE; // Die Nachricht wurde hier behandelt
	}

	return FALSE; // Die Nachricht wurde nicht behandelt
}

BOOL CSplashWnd::Create(CWnd* pParentWnd /*= NULL*/)
{
	if (!m_bitmap.LoadBitmap(IDB_SPLASH))
		return FALSE;

	BITMAP bm;
	m_bitmap.GetBitmap(&bm);

	UINT classStyle = 0;

	if (RunTimeHelper::IsWindowsXPOrHigher())
		classStyle |= CS_DROPSHADOW;

	return CreateEx(WS_EX_TOOLWINDOW /*| WS_EX_TOPMOST*/, //Do not hide other windows with WS_EX_TOPMOST
	                AfxRegisterWndClass(classStyle, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),
	                NULL, WS_POPUP | WS_VISIBLE, 0, 0, bm.bmWidth, bm.bmHeight, pParentWnd->GetSafeHwnd(), NULL);
}

void CSplashWnd::HideSplashScreen()
{
	// Fenster entfernen und Hauptrahmen aktualisieren
	DestroyWindow();

	if (AfxGetMainWnd())
		AfxGetMainWnd()->UpdateWindow();
}

void CSplashWnd::PostNcDestroy()
{
	// C++-Klasse freigeben
	c_pSplashWnd.reset();
}

int CSplashWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Fenster zentrieren
	CenterWindow();

#if 0
	if (RunTimeHelper::IsCommCtrl6())
	{
		const int offset = 20, cy = 20;

		BITMAP bm;
		m_bitmap.GetBitmap(&bm);

		const CRect rect(offset,bm.bmHeight - offset - cy,bm.bmWidth - offset,bm.bmHeight - offset);
		progress_bar_.Create(WS_VISIBLE|WS_CHILD|PBS_MARQUEE,rect,this,123);

		progress_bar_.SendMessage(PBM_SETMARQUEE,TRUE,1);
	}
#endif // 0

	const LOGFONT lf = GetDisplayFont();
	font_.CreateFontIndirect(&lf);

	BITMAP bm;
	m_bitmap.GetBitmap(&bm);

	CFileVersionInfo fv(::GetModuleHandle(0));

	CString version;
	version.Format(_T("Version %s. %s\n%s."),fv.GetProductVersion(),
	               CString(MAKEINTRESOURCE(IDS_LOADING)),fv.GetLegalCopyright());

	CWindowDC dc(this);
	CFont* oldfont = dc.SelectObject(&font_);

	CRect rc;
	dc.DrawText(version,&rc,DT_CENTER|DT_CALCRECT);
	dc.SelectObject(oldfont);

	const CSize size = rc.Size();

	const int offset = 20, cy = size.cy;
	const CRect rect(offset,bm.bmHeight - offset - cy,bm.bmWidth - offset,bm.bmHeight - offset);

	message_.Create(version,WS_VISIBLE|WS_CHILD|SS_CENTER,rect,this,123);
	message_.ModifyStyleEx(0,WS_EX_TRANSPARENT);
	message_.SetFont(&font_);

	// Zeitgeber festlegen, um den Begr��ungsbildschirm zu entfernen.
	SetTimer(1, 4000, NULL);

	return 0;
}

void CSplashWnd::OnPaint()
{
	CPaintDC dc(this);
}

void CSplashWnd::OnTimer(UINT_PTR /*nIDEvent*/)
{
	// Begr��ungsbildschirm entfernen
	HideSplashScreen();
}

HBRUSH CSplashWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr;

	if (pWnd == &message_)
	{
		hbr = reinterpret_cast<HBRUSH>(::GetStockObject(HOLLOW_BRUSH));
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255,255,255));
	}
	else
		hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

BOOL CSplashWnd::OnEraseBkgnd(CDC* pDC)
{
	CDC dcImage;

	if (!dcImage.CreateCompatibleDC(pDC))
		return FALSE;

	BITMAP bm;
	m_bitmap.GetBitmap(&bm);

	// Bild zeichnen
	CBitmap* pOldBitmap = dcImage.SelectObject(&m_bitmap);
	pDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcImage, 0, 0,SRCCOPY);
	dcImage.SelectObject(pOldBitmap);

	return TRUE;
}

CSplashWnd* CSplashWnd::GetInstance()
{
	return c_pSplashWnd.get();
}

bool CSplashWnd::IsActive()
{
	return GetInstance() != 0;
}