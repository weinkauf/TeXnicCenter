#pragma once


// WorkspacePaneBase

class WorkspacePaneBase : public CDockablePane
{
	DECLARE_DYNAMIC(WorkspacePaneBase)

public:
	WorkspacePaneBase();
	~WorkspacePaneBase();

protected:
	DECLARE_MESSAGE_MAP()

	virtual bool IsFocused() const;

	virtual void Focus();

	void OnSlide( BOOL bSlideOut );

	CMFCAutoHideBar* SetAutoHideMode( BOOL bMode, DWORD dwAlignment, CMFCAutoHideBar* pCurrAutoHideBar /* = NULL */, BOOL bUseTimer /* = TRUE */ );
};


