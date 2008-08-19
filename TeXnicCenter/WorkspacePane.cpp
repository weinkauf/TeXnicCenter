#include "stdafx.h"
#include "TeXnicCenter.h"
#include "WorkspacePane.h"


IMPLEMENT_DYNAMIC(WorkspacePane, CDockablePane)

WorkspacePane::WorkspacePane()
: client_(0)
{
}

BEGIN_MESSAGE_MAP(WorkspacePane, CDockablePane)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

// WorkspacePane message handlers

void WorkspacePane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	AdjustLayout(CRect(0,0,cx,cy));
}

void WorkspacePane::SetClient(CWnd* p)
{
	client_ = p;

	if (m_hWnd && client_ && client_->GetParent()->GetSafeHwnd() != m_hWnd)
	{
		client_->SetParent(this);
		client_->SetOwner(this);
		AdjustLayout();
	}
}

void WorkspacePane::AdjustLayout(const CRect& rc)
{
	if (client_)
		client_->SetWindowPos(0,rc.left,rc.top,rc.Width(),rc.Height(),SWP_NOACTIVATE|SWP_NOZORDER);
}

void WorkspacePane::AdjustLayout(void)
{
	CRect rect;
	GetClientRect(&rect);

	AdjustLayout(rect);
}

void WorkspacePane::OnDestroy()
{
	CDockablePane::OnDestroy();
	client_ = 0;
}

void WorkspacePane::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	if (client_)
		client_->SetFocus();
}

int WorkspacePane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (client_ && client_->GetParent() != this)
	{
		client_->SetParent(this);
		AdjustLayout();
	}

	return 0;
}

CWnd* WorkspacePane::GetClient(void) const
{
	return client_;
}

BOOL WorkspacePane::OnEraseBkgnd(CDC* pDC)
{
	return client_ ? 1 : CDockablePane::OnEraseBkgnd(pDC);
}

void WorkspacePane::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	if (NavigatorTreeCtrl* t = dynamic_cast<NavigatorTreeCtrl*>(client_))
		t->ShowContextMenu(point);
}
