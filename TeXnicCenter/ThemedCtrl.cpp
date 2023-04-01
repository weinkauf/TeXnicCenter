// ThemedCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ThemedCtrl.h"

// Helper for drawing theme client edge

inline bool AtlDrawThemeClientEdge(HTHEME hTheme,HWND hWnd,HRGN hRgnUpdate = NULL,HBRUSH hBrush = NULL,int nPartID = 0,int nStateID = 0)
{
	ATLASSERT(hTheme != NULL);
	ATLASSERT(hWnd);

	CWindowDC dc(CWnd::FromHandle(hWnd));
	if (!dc.m_hDC)
		return false;

	// Get border size
	int cxBorder = GetSystemMetrics(SM_CXBORDER);
	int cyBorder = GetSystemMetrics(SM_CYBORDER);
	if (SUCCEEDED(::GetThemeInt(hTheme,nPartID,nStateID,TMT_SIZINGBORDERWIDTH,&cxBorder)))
		cyBorder = cxBorder;

	RECT rect;
	::GetWindowRect(hWnd,&rect);

	// Remove the client edge from the update region
	int cxEdge = GetSystemMetrics(SM_CXEDGE);
	int cyEdge = GetSystemMetrics(SM_CYEDGE);
	::InflateRect(&rect,-cxEdge,-cyEdge);
	CRgn rgn;
	rgn.CreateRectRgnIndirect(&rect);

	if (!rgn.m_hObject)
		return false;

	if (hRgnUpdate != NULL)
		::CombineRgn(rgn,hRgnUpdate,rgn,RGN_AND);

	::OffsetRect(&rect,-rect.left,-rect.top);

	::OffsetRect(&rect,cxEdge,cyEdge);
	dc.ExcludeClipRect(&rect);
	::InflateRect(&rect,cxEdge,cyEdge);

	::DrawThemeBackground(hTheme,dc,nPartID,nStateID,&rect,NULL);

	// Use background brush too, since theme border might not cover everything
	if (cxBorder < cxEdge && cyBorder < cyEdge)
	{
		if (hBrush == NULL)
			// need conditional code because types don't match in winuser.h
#ifdef _WIN64
			hBrush = (HBRUSH)::GetClassLongPtr(hWnd,GCLP_HBRBACKGROUND);
#else
			hBrush = (HBRUSH)UlongToPtr(::GetClassLongPtr(hWnd,GCLP_HBRBACKGROUND));
#endif

		::InflateRect(&rect,cxBorder - cxEdge,cyBorder - cyEdge);
		::FillRect(dc,&rect,hBrush);
	}

	::DefWindowProc(hWnd,WM_NCPAINT,(WPARAM)rgn.m_hObject,0L);

	return true;
}

LRESULT ThemedCtrl::OnDestroy(UINT /*uMsg*/,WPARAM /*wParam*/,LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// Let the parent process the message
	DefWindowProc();
	// now unsubclass
	UnsubclassWindow();

	if (m_hTheme)
		::CloseThemeData(m_hTheme);

	return 0;
}

LRESULT ThemedCtrl::OnNcPaint(UINT /*uMsg*/,WPARAM wParam,LPARAM /*lParam*/,BOOL& bHandled)
{
	bHandled = FALSE;

	if ((GetExStyle() & WS_EX_CLIENTEDGE) != 0)
	{
		if (m_hTheme != NULL)
		{
			HRGN hRgn = (wParam != 1) ? (HRGN)wParam : NULL;

			if (AtlDrawThemeClientEdge(m_hTheme,m_hWnd,hRgn))
				bHandled = TRUE;
		}
	}

	return 0;
}

void ThemedCtrl::Initialize()
{
	ModifyStyleEx(0,WS_EX_CLIENTEDGE);

	if (::IsAppThemed())
		m_hTheme = ::OpenThemeData(m_hWnd,L"TREEVIEW");
	else
		m_hTheme = 0;

	RedrawWindow(0,0,RDW_FRAME|RDW_INVALIDATE);
}
