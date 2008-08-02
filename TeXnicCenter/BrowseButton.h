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

#if !defined(AFX_BROWSEBUTTON_H__AF58958C_0A74_11D5_A222_006097239934__INCLUDED_)
#define AFX_BROWSEBUTTON_H__AF58958C_0A74_11D5_A222_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
This class implements a button, when pressed it will open a dialog 
for selecting a folder or a file. When the browse dialog has been 
closed by pressing OK, the returned path will be inserted into the
edit control, attached to the button.

@ingroup guielements

@author Sven Wiegand
 */
class CBrowseButton : public CButton {
    // construction/destruction
public:
    /**
    When clicked, the button will show a dialog for selecting a folder,
    using the SHBrowseForFolder() function.

    @param unCtrlId
            Control-ID of the dialog control, which's should be set to the 
            path of	the selected folder.
    @param other Parameters
            See BROWSEINFO-structure in the MSDN for details.
     */
    CBrowseButton(UINT unCtrlId, LPCTSTR lpszTitle,
                  bool bUseEditForInitFolder = true,
                  LPCTSTR lpszInitFolder = NULL, UINT unFlags = 0,
                  BFFCALLBACK lpfn = NULL, LPARAM lParam = 0);

    /**
    When clicked, the button will show a dialog for selecting a file,
    using the CFileDialog class.

    @param unCtrlId
            Control-ID of the dialog control, which's should be set to the 
            path of	the selected file.
    @param other Parameters
            See CFileDialog::CFileDialog() in the MSDN for details.
     */
    CBrowseButton(UINT unCtrlId,
                  BOOL bOpenFileDialog = TRUE, LPCTSTR lpszDefExt = NULL,
                  DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                  LPCTSTR lpszFilter = NULL);

    // implementation
protected:
    virtual void Browse();

    // overridings
protected:
    //{{AFX_VIRTUAL(CBrowseButton)
    //}}AFX_VIRTUAL

    // message handlers
protected:
    //{{AFX_MSG(CBrowseButton)
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()

    // attributes
protected:
    /**
    ID of the control, which's text should be set to the choosen path.
     */
    UINT m_unCtrlId;

    /** 
    TRUE if the m_wndFileDialog-member is valid, FALSE if the 
    m_wndFolderDialog-member is valid.
     */
    BOOL m_bBrowseForFile;

    ///Holds the Name of the Folder to be selected using the Callback function.
    CString m_strInitFolder;

    ///Whether to get the initial folder for the BrowseForFolder-Dialog from
    /// the Edit Control or from the given InitFolder-Value given in the constructor.
    bool m_bUseEditForInitFolder;

    /**
    Data for file dialog
     */
    struct tagFileDialogInfo {
        BOOL bOpenFileDialog;
        CString strDefExt;
        DWORD dwFlags;
        CString strFilter;
    } m_dataFileDialog;

    /**
    Data for folder dialog
     */
    struct tagFolderDialogInfo {
        CString strTitle;
        UINT unFlags;
        BFFCALLBACK lpfn;
        LPARAM lParam;
    } m_dataFolderDialog;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

/**
This is the callback function for the FolderSelect-Dialog.
Using this we are able to send a message to the dialog,
which selects a new folder. The pointer to the initial folder
needs to be given in the BROWSEINFO::lParam parameter.

@author Tino Weinkauf
 */
int CALLBACK BrowseDlgCallback(HWND hwnd, UINT uMsg,
                               LPARAM lParam, LPARAM lpData);


#endif // AFX_BROWSEBUTTON_H__AF58958C_0A74_11D5_A222_006097239934__INCLUDED_
