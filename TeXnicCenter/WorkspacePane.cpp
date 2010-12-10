#include "stdafx.h"
#include "navigatorview.h"
#include "WorkspacePane.h"

IMPLEMENT_DYNAMIC(WorkspacePane, WorkspacePaneBase)

WorkspacePane::WorkspacePane()
: client_(0)
{
}

BEGIN_MESSAGE_MAP(WorkspacePane, WorkspacePaneBase)
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
	WorkspacePaneBase::OnSize(nType, cx, cy);

	CRect rc(0,0,cx,cy);
	AdjustLayout(rc);
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

void WorkspacePane::AdjustLayout( CRect& rc )
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
	WorkspacePaneBase::OnDestroy();
	client_ = 0;
}

void WorkspacePane::OnSetFocus(CWnd* pOldWnd)
{
	WorkspacePaneBase::OnSetFocus(pOldWnd);
	SetClientFocus();
}

int WorkspacePane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (WorkspacePaneBase::OnCreate(lpCreateStruct) == -1)
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
	return client_ ? 1 : WorkspacePaneBase::OnEraseBkgnd(pDC);
}

void WorkspacePane::OnContextMenu(CWnd* pWnd, CPoint point)
{
	if (NavigatorTreeCtrl* t = dynamic_cast<NavigatorTreeCtrl*>(client_))
		t->ShowContextMenu(point);
	else
		WorkspacePaneBase::OnContextMenu(pWnd,point);
}

void WorkspacePane::SetClientFocus()
{
	if (client_)
		client_->SetFocus();
}

void WorkspacePane::Focus()
{
	SetClientFocus();
}