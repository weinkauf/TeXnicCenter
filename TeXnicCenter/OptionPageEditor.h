#if !defined(AFX_OPTIONPAGEEDITOR_H__B6E5736A_65D4_42E7_AFEA_64BA8D49405D__INCLUDED_)
#define AFX_OPTIONPAGEEDITOR_H__B6E5736A_65D4_42E7_AFEA_64BA8D49405D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionPageEditor.h : header file
//

#include "PropertyPage.h"

/////////////////////////////////////////////////////////////////////////////
// COptionPageEditor dialog

class COptionPageEditor : 
    public PropertyPage
{
    DECLARE_DYNCREATE(COptionPageEditor)

    // Construction
public:
    COptionPageEditor();
    ~COptionPageEditor();

    // Dialog Data
    //{{AFX_DATA(COptionPageEditor)

    enum {
        IDD = IDD_OPTIONS_EDITOR
    };
    CEdit m_wndFixedColumn;
    UINT m_nTabWidth;
    UINT m_nFixedColumn;
    int m_nWordWrapStyle;
    BOOL m_bShowLineNumbers;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(COptionPageEditor)
public:
    virtual void OnOK();
protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(COptionPageEditor)
    afx_msg void UpdateControlStates();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONPAGEEDITOR_H__B6E5736A_65D4_42E7_AFEA_64BA8D49405D__INCLUDED_)
