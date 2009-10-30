// WorkspacePaneBase.cpp : implementation file
//

#include "stdafx.h"
#include "TeXnicCenter.h"
#include "WorkspacePaneBase.h"


// WorkspacePaneBase

IMPLEMENT_DYNAMIC(WorkspacePaneBase, CDockablePane)

WorkspacePaneBase::WorkspacePaneBase()
{

}

WorkspacePaneBase::~WorkspacePaneBase()
{
}


BEGIN_MESSAGE_MAP(WorkspacePaneBase, CDockablePane)
END_MESSAGE_MAP()

void WorkspacePaneBase::OnSlide( BOOL bSlideOut )
{
	__super::OnSlide(bSlideOut);

#if 0
	if (bSlideOut) {
		SetFocus();
	}
#endif // 0
}

CMFCAutoHideBar* WorkspacePaneBase::SetAutoHideMode( BOOL bMode, DWORD dwAlignment, CMFCAutoHideBar* pCurrAutoHideBar /* = NULL */, BOOL bUseTimer /* = TRUE */ )
{
	bool wasFocused = IsFocused();
	CMFCAutoHideBar* result = __super::SetAutoHideMode(bMode, dwAlignment, pCurrAutoHideBar, bUseTimer);

	if (wasFocused)
		SetFocus();

	return result;
}

bool WorkspacePaneBase::IsFocused() const
{
	CWnd* focus = GetFocus();
	return focus && (focus == this || IsChild(focus));
}

void WorkspacePaneBase::Focus()
{
	SetFocus();
}

// WorkspacePaneBase message handlers


