// StructurePane.cpp : implementation file
//

#include "stdafx.h"
#include "TeXnicCenter.h"
#include "StructureView.h"


// StructurePane

IMPLEMENT_DYNAMIC(StructurePane, CDockablePane)

//StructurePane::StructurePane()
//{
//
//}
//
//StructurePane::~StructurePane()
//{
//}

BEGIN_MESSAGE_MAP(StructurePane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()


// StructurePane message handlers

int StructurePane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: The toolbar is invisible for now
	if (!toolbar_.Create(this,AFX_DEFAULT_TOOLBAR_STYLE & ~WS_VISIBLE) || !toolbar_.LoadToolBar(IDR_STRUCTURE,0,0,TRUE))
		return -1;

	toolbar_.AdjustStyle();

	if (!tree_.Create(this))
		return -1;

	AdjustLayout();

	return 0;
}

void StructurePane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void StructurePane::AdjustLayout(const CRect& rect)
{
	if (toolbar_ && tree_)
	{
		CSize size(0,0);

		if ((toolbar_.GetStyle() & WS_VISIBLE) == WS_VISIBLE)
		{
			size = toolbar_.CalcFixedLayout(TRUE,TRUE);
			toolbar_.SetWindowPos(0,rect.left,rect.top,
			                      rect.right - rect.left,rect.top + size.cy,SWP_NOACTIVATE|SWP_NOZORDER);
		}

		const int y = rect.top + size.cy;
		tree_.SetWindowPos(0,rect.left,y,rect.right,rect.bottom - y,
		                   SWP_NOACTIVATE|SWP_NOZORDER);
	}
}

void StructurePane::AdjustLayout()
{
	CRect rect;
	GetClientRect(&rect);
	AdjustLayout(rect);
}

void StructurePane::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	if (tree_)
		tree_.SetFocus();
}

CProjectView* StructurePane::GetProjectView()
{
	return &tree_;
}

void StructurePane::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	tree_.ShowContextMenu(point); // We're done. Do not pass the message to the base class
}
