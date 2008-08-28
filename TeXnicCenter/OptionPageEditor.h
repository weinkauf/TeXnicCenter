#pragma once

#include "PropertyPage.h"

class COptionPageEditor :
	public PropertyPage
{
	DECLARE_DYNCREATE(COptionPageEditor)

private:
	CEdit m_wndFixedColumn;
	UINT m_nTabWidth;
	UINT m_nFixedColumn;
	int m_nWordWrapStyle;
	BOOL m_bShowLineNumbers;

	BOOL show_line_below_fold_;
	BOOL show_line_below_no_fold_;
	BOOL show_line_above_fold_;
	BOOL show_line_above_no_fold_;
	BOOL use_spaces_;
	
// Construction
public:
	COptionPageEditor();
	~COptionPageEditor();

	enum { IDD = IDD_OPTIONS_EDITOR	};	

protected:
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
	afx_msg void UpdateControlStates();
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
