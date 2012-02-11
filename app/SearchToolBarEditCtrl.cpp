#include "stdafx.h"
#include "SearchToolBarEditCtrl.h"

IMPLEMENT_DYNAMIC(SearchToolBarEditCtrl, CMFCToolBarEditCtrl)

SearchToolBarEditCtrl::SearchToolBarEditCtrl(CMFCToolBarEditBoxButton& button)
	: CMFCToolBarEditCtrl(button)
{
}

SearchToolBarEditCtrl::~SearchToolBarEditCtrl()
{
}

BOOL SearchToolBarEditCtrl::PreTranslateMessage(MSG* pMsg)
{
	BOOL result = FALSE;

	if (!(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)) {
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_TAB) {
			// Handle switch to the next window using Tab and Shift+Tab
			// For now, there's only a switch between two windows:
			// the edit box and the list control
			ASSERT(GetParent() && GetParent()->GetParent());
			GetParent()->GetParent()->GetNextDlgTabItem(GetParent(),
				::GetKeyState(VK_SHIFT) >> 15 & 1)->SetFocus();
			result = TRUE;
		}
		else
			result = CMFCToolBarEditCtrl::PreTranslateMessage(pMsg);
	}
	else if (GetFocus() == this) {
		if (GetWindowTextLength() > 0) {
			// Not empty: clear the edit box
			SetWindowText(NULL); 
			result = TRUE;
		}
		else // otherwise let the base deactivate the view
			result = CMFCToolBarEditCtrl::PreTranslateMessage(pMsg);
	}

	return result;
}
