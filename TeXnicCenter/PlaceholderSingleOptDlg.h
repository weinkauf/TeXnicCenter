/********************************************************************
 *
 * This file is part of the TeXnicCenter-system
 *
 * Copyright (C) 1999-2000 Sven Wiegand
 * Copyright (C) 2000-$CurrentYear$ ToolsCenter
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * If you have further questions or if you want to support
 * further TeXnicCenter development, visit the TeXnicCenter-homepage
 *
 *    http://www.ToolsCenter.org
 *
 *********************************************************************/

/********************************************************************
 *
 * $Id$
 *
 ********************************************************************/

#if !defined(AFX_PLACEHOLDERSINGLEOPTDLG_H__A8DC67B4_57C2_11D6_AF10_00201855324E__INCLUDED_)
#define AFX_PLACEHOLDERSINGLEOPTDLG_H__A8DC67B4_57C2_11D6_AF10_00201855324E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
This dialog lets the user define all possible placeholders for single files
together with their options (forward slash and 8.3-Format).

<b>Usage:</b>

First call SetPlaceholderBase() to set the base for the placeholder.
Valid options are "m" and "c" (for main and current file).
Then call SetAllowOptions() to enable/disable Options-Support.

Then call DoModal().

After the user finished the dialog with the Ok-Button,
you will find the defined placeholder in strPlaceholder.
If the dialog was canceled, strPlaceholder is empty.

@ingroup dialogs

@author Tino Weinkauf
 */

class CPlaceholderSingleOptDlg : public CDialog {
    // Construction
public:
    CPlaceholderSingleOptDlg(CWnd* pParent = NULL); // standard constructor

    // Dialog Data
    //{{AFX_DATA(CPlaceholderSingleOptDlg)

    enum {
        IDD = IDD_PLACEHOLDER_SINGLE
    };
    CButton m_ShortBtn;
    CButton m_ForwardBtn;
    CComboBox m_PHTypeCombo;
    CStatic m_WhatStatic;
    CStatic m_ResultStatic;
    CEdit m_ExampleEdit;
    BOOL m_bForwardSlash;
    BOOL m_bShortFormat;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPlaceholderSingleOptDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CPlaceholderSingleOptDlg)
    virtual void OnOK();
    virtual void OnCancel();
    virtual BOOL OnInitDialog();
    afx_msg void ShowExample();
    afx_msg void OnPHTypeChange();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

    // Implementation
public:
    /**Sets the basic string value of the placeholder.
    Call this before calling DoModal().
     */
    void SetPlaceholderBase(LPCTSTR lpszPHBase);

    /** Enables/Disables Support of Options.
    Call this before calling DoModal().
     */
    void SetAllowOptions(bool bEnable = true);

protected:
    void ConstructPlaceholder();
    bool CorrectPlaceholderBase();
    void GetPossibleOptions(bool* pbForward, bool* pbShort);

    // Attributes
public:
    /**Use this to retrieve the Placeholder String with all options
    defined by the user - after DoModal().
     */
    CString strPlaceholder;

private:
    CString m_strPlaceholderBase;

    ///true, if Options (forward slash, 8.3-Format) are allowed to be configured 
    bool m_bAllowOptions;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLACEHOLDERSINGLEOPTDLG_H__A8DC67B4_57C2_11D6_AF10_00201855324E__INCLUDED_)
