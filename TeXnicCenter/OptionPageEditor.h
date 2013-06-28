#pragma once

#include "PropertyPage.h"

class COptionPageEditor :
	public CMFCPropertyPage
{
	DECLARE_DYNCREATE(COptionPageEditor)

private:
	UINT m_nTabWidth;
	UINT m_nFixedColumn;
	int m_nVerticalEdgeMode;
	UINT m_nVerticalEdgeColumn;
	COLORREF m_aVariableEdgeColor;

	BOOL show_line_below_fold_;
	BOOL show_line_below_no_fold_;
	BOOL show_line_above_fold_;
	BOOL show_line_above_no_fold_;
	BOOL use_spaces_;
	BOOL fold_compact_;
	BOOL close_toolwindows_on_escape_;

// Construction
public:
	COptionPageEditor();
	~COptionPageEditor();

	enum { IDD = IDD_OPTIONS_EDITOR	};

protected:
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support

	void UpdateControlStates();
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnEditorVerticalEdgeColor();
	CMFCColorButton m_wndVariableEdgeColorPicker;
	virtual BOOL OnInitDialog();
};
