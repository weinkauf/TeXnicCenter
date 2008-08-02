/********************************************************************
 *
 * This file is part of the TeXnicCenter-system
 *
 * Copyright (C) 2007 Tino Weinkauf
 * Copyright (C) 2007-$CurrentYear$ ToolsCenter
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

#if !defined(AFX_TEXTOUTSOURCEDLG_H__C7A77122_26FF_42F7_ABDF_8FD7C14BDCDA__INCLUDED_)
#define AFX_TEXTOUTSOURCEDLG_H__C7A77122_26FF_42F7_ABDF_8FD7C14BDCDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BrowseButton.h"

/** Outsource selected text to a new file.

@ingroup dialogs

@author Tino Weinkauf
 */
class CTextOutsourceDlg : public CDialog {
    // Construction
public:
    CTextOutsourceDlg(CWnd* pParent = NULL); // standard constructor

    ///Complete path of the new file
    CPathTool NewPath;

    // Dialog Data
    //{{AFX_DATA(CTextOutsourceDlg)

    enum {
        IDD = IDD_OUTSOURCE
    };
    CButton m_wndRadioOpenBackground;
    CBrowseButton m_BrowseFolderButton;
    CButton m_wndOkButton;
    CEdit m_CmdRightCtrl;
    CEdit m_CmdLeftCtrl;
    CString CmdLeft;
    CString CmdRight;
    CString m_Directory;
    CString m_Filename;
    int m_nIncludeType;
    BOOL m_bOpenNewFile;
    int m_nOpenNewFileType;
    //}}AFX_DATA

    CButton m_wndRadioOpenForeground;

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTextOutsourceDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    void CheckAndFixFileName();

    // Generated message map functions
    //{{AFX_MSG(CTextOutsourceDlg)
    virtual void OnOK();
    afx_msg void OnIncludeTypeChanged();
    virtual BOOL OnInitDialog();
    afx_msg void UpdateControlStates();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTOUTSOURCEDLG_H__C7A77122_26FF_42F7_ABDF_8FD7C14BDCDA__INCLUDED_)
