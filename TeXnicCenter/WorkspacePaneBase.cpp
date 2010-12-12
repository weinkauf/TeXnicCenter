#include "stdafx.h"
#include "WorkspacePaneBase.h"

IMPLEMENT_DYNAMIC(WorkspacePaneBase, CDockablePane)

WorkspacePaneBase::WorkspacePaneBase()
{

}

WorkspacePaneBase::~WorkspacePaneBase()
{
}

BEGIN_MESSAGE_MAP(WorkspacePaneBase, CDockablePane)
END_MESSAGE_MAP()

CMFCAutoHideBar* WorkspacePaneBase::SetAutoHideMode(BOOL bMode, DWORD dwAlignment,
	CMFCAutoHideBar* pCurrAutoHideBar, BOOL bUseTimer)
{
	bool wasFocused = IsFocused();
	CMFCAutoHideBar* result = CDockablePane::SetAutoHideMode(bMode, dwAlignment,
		pCurrAutoHideBar, bUseTimer);

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
