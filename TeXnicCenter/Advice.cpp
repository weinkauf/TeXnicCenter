// Advice.cpp : implementation file
//

#include "stdafx.h"
#include "texniccenter.h"
#include "Advice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdvice

CAdvice::CAdvice()
{
}

CAdvice::~CAdvice()
{
	DestroyWindow();
}


BEGIN_MESSAGE_MAP(CAdvice, CStatic)
	//{{AFX_MSG_MAP(CAdvice)
	ON_WM_PAINT()
	ON_WM_CAPTURECHANGED()
	ON_WM_ACTIVATEAPP()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdvice message handlers

BOOL CAdvice::Create(LPCTSTR lpszText, DWORD dwStyle,const RECT& rect, CWnd* pParentWnd, UINT nID) 
{
	//VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTL_BAR_REG));

	BOOL bResult = CWnd::CreateEx(NULL, _T("STATIC"), NULL,
		WS_POPUP | dwStyle, // force WS_POPUP
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		pParentWnd->GetSafeHwnd(), NULL, NULL);

	if (bResult)
		SetOwner(pParentWnd);
	return bResult;
}

void CAdvice::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect r;
	CString s;

	GetClientRect(&r);
	dc.FillSolidRect(&r, ADVICE_BK_COLOR);
	dc.Draw3dRect(&r, RGB(255,255,255), RGB(0,0,0));	
	InflateRect(&r, -1, -1);
	GetWindowText(s);
	// Save these value to restore them when done drawing.
	COLORREF crOldTextColor = dc.GetTextColor();
	COLORREF crOldBkColor = dc.GetBkColor();

	HGDIOBJ hOldFont = dc.SelectStockObject(DEFAULT_GUI_FONT);
	dc.SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
	dc.SetBkColor(ADVICE_BK_COLOR);

	dc.DrawText(
	  s,
	  s.GetLength(),
	  &r,
	  DT_VCENTER|DT_TABSTOP);
	dc.SetTextColor(crOldTextColor);
	dc.SetBkColor(crOldBkColor);
	dc.SelectObject(hOldFont);

}


void CAdvice::SetWindowText(LPCTSTR text)
{
	//CStatic::SetWindowText(CString(text) + _T(" (CTRL + SPACE)"));
	CStatic::SetWindowText(text);
	CDC *pDC = GetDC();
	CRect r;
	CString s;
	GetWindowText(s);
	// Compute needed window size 
	HGDIOBJ hOldFont = pDC->SelectStockObject(DEFAULT_GUI_FONT);
	pDC->DrawText(
	  s,
	  &r,
	  DT_VCENTER|DT_CALCRECT);
	pDC->SelectObject(hOldFont);

	ReleaseDC(pDC);
	//TRACE("Advice: Change window size: %d, %d\n", r.Width(), r.Height());
	SetWindowPos(NULL, 0, 0, r.Width() + 6, r.Height() + 4, SWP_NOMOVE|SWP_NOZORDER);
}

void CAdvice::OnCaptureChanged(CWnd *pWnd) 
{
	CStatic::OnCaptureChanged(pWnd);
	if (pWnd != NULL) {
		ShowWindow(SW_HIDE);
	}
}

void CAdvice::OnActivateApp(BOOL bActive, HTASK hTask) 
{
	CStatic::OnActivateApp(bActive, hTask);
	if (!bActive) {
		ShowWindow(SW_HIDE);
	}
}

void CAdvice::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CStatic::OnTimer(nIDEvent);
	ShowWindow(SW_HIDE);
	KillTimer(nIDEvent);
}
