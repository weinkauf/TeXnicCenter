#pragma once

#include "StructureTreeCtrl.h"
#include "PrivateToolBar.h"
#include "WorkspacePaneBase.h"

// StructurePane

class StructurePane : public WorkspacePaneBase
{
	DECLARE_DYNAMIC(StructurePane)

private:
	StructureTreeCtrl tree_;
	PrivateToolBar toolbar_;

public:
	//StructurePane();
	//~StructurePane();
	void AdjustLayout(const CRect& rect);
	void AdjustLayout();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);	
	afx_msg void OnSetFocus(CWnd* pOldWnd);

public:
	CProjectView* GetProjectView();

protected:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};
