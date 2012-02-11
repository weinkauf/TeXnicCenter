#pragma once

class WorkspacePaneBase
	: public CDockablePane
{
	DECLARE_DYNAMIC(WorkspacePaneBase)
public:
	WorkspacePaneBase();
	virtual ~WorkspacePaneBase();

	CMFCAutoHideBar* SetAutoHideMode(BOOL bMode, DWORD dwAlignment, 
		CMFCAutoHideBar* pCurrAutoHideBar /* = NULL */, BOOL bUseTimer /* = TRUE */);

protected:
	DECLARE_MESSAGE_MAP()

	/// Gets a value indicating whether the pane has the focus.
	virtual bool IsFocused() const;

	/// Sets the focus on one of pane's children or pane itself.
	virtual void Focus();
};
