#pragma once

#include "PropertyPage.h"

class COptionPageEditor :
			public PropertyPage
{
	DECLARE_DYNCREATE(COptionPageEditor)

// Construction
public:
	COptionPageEditor();
	~COptionPageEditor();

	enum { IDD = IDD_OPTIONS_EDITOR	};

	CEdit m_wndFixedColumn;
	UINT m_nTabWidth;
	UINT m_nFixedColumn;
	int m_nWordWrapStyle;
	BOOL m_bShowLineNumbers;

protected:
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
	afx_msg void UpdateControlStates();
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
