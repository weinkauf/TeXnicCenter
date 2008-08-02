#pragma once

#include "StructureTreeCtrl.h"
#include "PrivateToolBar.h"

// StructureView

class StructureView : public CDockablePane
{
	DECLARE_DYNAMIC(StructureView)

private:
    StructureTreeCtrl tree_;
    PrivateToolBar toolbar_;

public:
	//StructureView();
	//~StructureView();

protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    void AdjustLayout(const CRect& rect);
    void AdjustLayout();
    afx_msg void OnSetFocus(CWnd* pOldWnd);

    CProjectView* GetProjectView();
};
