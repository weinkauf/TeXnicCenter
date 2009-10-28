// Advice.cpp : implementation file
//

#include "stdafx.h"
#include "texniccenter.h"
#include "Advice.h"
#include "FontOccManager.h"
#include "Configuration.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdvice

CAdvice::CAdvice()
{
	font_.Attach(CreateDefaultDisplayFont());
}

CAdvice::~CAdvice()
{
	DestroyWindow();
}

BEGIN_MESSAGE_MAP(CAdvice,CStatic)
	//{{AFX_MSG_MAP(CAdvice)
	ON_WM_PAINT()
	ON_WM_CAPTURECHANGED()
	ON_WM_ACTIVATEAPP()
	ON_WM_TIMER()
	ON_MESSAGE(WM_FLOATSTATUS,&CAdvice::OnFloatStatus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdvice message handlers

BOOL CAdvice::Create(LPCTSTR /*lpszText*/,DWORD dwStyle,const RECT& /*rect*/,CWnd* pParentWnd,UINT /*nID*/)
{
	//VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTL_BAR_REG));

	return CWnd::CreateEx(NULL,WC_STATIC,NULL,
	                              WS_POPUP | dwStyle,// force WS_POPUP
	                              CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
	                              pParentWnd->GetSafeHwnd(),NULL,NULL);
}

void CAdvice::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect r;
	CString s;

	GetClientRect(&r);
	COLORREF back = ::GetSysColor(COLOR_INFOBK);
	dc.FillSolidRect(&r,back/*ADVICE_BK_COLOR*/);
	//dc.Draw3dRect(&r,RGB(255,255,255),RGB(0,0,0));
	InflateRect(&r,-1,-1);
	GetWindowText(s);
	// Save these value to restore them when done drawing.
	COLORREF crOldTextColor = dc.GetTextColor();
	COLORREF crOldBkColor = dc.GetBkColor();

	HGDIOBJ hOldFont = dc.SelectObject(&font_);//dc.SelectStockObject(DEFAULT_GUI_FONT);
	dc.SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
	dc.SetBkColor(back);

	dc.DrawText(s,s.GetLength(),&r,MAKEWORD(DT_VCENTER | DT_TABSTOP | DT_EXPANDTABS,
	                                        CConfiguration::GetInstance()->m_nTabWidth & 0xf)); // bits 8-15 contain the tab stop width in characters

	dc.SetTextColor(crOldTextColor);
	dc.SetBkColor(crOldBkColor);
	dc.SelectObject(hOldFont);
}

void CAdvice::SetWindowText(LPCTSTR text)
{
	//CStatic::SetWindowText(CString(text) + _T(" (CTRL + SPACE)"));
	CStatic::SetWindowText(text);
	
	CRect r;
	
	{
		CWindowDC dc(this);
		CString s;
		GetWindowText(s);
		// Compute needed window size
		HGDIOBJ hOldFont = dc.SelectObject(&font_);//pDC->SelectStockObject(DEFAULT_GUI_FONT);
		dc.DrawText(
			s,
			&r,
			DT_VCENTER | DT_CALCRECT);
		dc.SelectObject(hOldFont);
	}

	//TRACE("Advice: Change window size: %d, %d\n", r.Width(), r.Height());
	SetWindowPos(NULL,0,0,r.Width() + 6,r.Height() + 4,SWP_NOMOVE | SWP_NOZORDER);
}

void CAdvice::OnCaptureChanged(CWnd *pWnd)
{
	CStatic::OnCaptureChanged(pWnd);

	if (pWnd != NULL)
	{
		ShowWindow(SW_HIDE);
	}
}

void CAdvice::OnActivateApp(BOOL bActive,DWORD dwThreadId)
{
	CStatic::OnActivateApp(bActive,dwThreadId);

	if (!bActive)
	{
		ShowWindow(SW_HIDE);
	}
}

void CAdvice::OnTimer(UINT_PTR nIDEvent)
{
	CStatic::OnTimer(nIDEvent);
	KillTimer(nIDEvent);

	ShowWindow(SW_HIDE);
}

void CAdvice::EnableFocusChange()
{
	m_IgnoreFocusChange = FALSE;
}

void CAdvice::DisableFocusChange()
{
	m_IgnoreFocusChange = TRUE;
}

LRESULT CAdvice::OnFloatStatus(WPARAM wParam, LPARAM /*lParam*/)
{
	LRESULT result;

	if (wParam == FS_SYNCACTIVE)
	{
		// Respond to this message to prevent the main frame from losing the focus
		result = (GetStyle() & WS_VISIBLE) == WS_VISIBLE;
	}
	else
		result = 0;

	return result;
}
BOOL CAdvice::PreTranslateMessage(MSG* pMsg)
{
	BOOL result;

	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE && (GetStyle() & WS_VISIBLE) == WS_VISIBLE) {
		ShowWindow(SW_HIDE);
		result = TRUE;
	}
	else
		result = CStatic::PreTranslateMessage(pMsg);

	return result;
}
