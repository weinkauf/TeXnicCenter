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

#if !defined(AFX_PROJECTNEWDIALOG_H__5588FFA4_2C94_11D3_929E_444553540000__INCLUDED_)
#define AFX_PROJECTNEWDIALOG_H__5588FFA4_2C94_11D3_929E_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LatexProject.h"
#include "IProjectWizard.h"
#include "BrowseButton.h"
#include "TemplateDialog.h"

/** @addtogroup templates
        @{
 */

/**
Abstract base class for all elements listed in the template list
control of the new-project dialog.

@see CProjectNewDialog

@author Sven Wiegand
 */
class CProjectTemplateItem : public CTemplateItem {
    // operations
public:
    /** 
    Called to init the specified project based on the template item.

    @param pProject
            Pointer to the new project.
    @param lpszCr
            String to use as carriage return.

    @return
            Nonzero if the project creation should be finished and FALSE
            if the project creation should be cancelled.
     */
    virtual BOOL InitProject(CLaTeXProject *pProject, LPCTSTR lpszCr) = 0;
};

/**
Represents a project template with an empty main file.

@see CProjectNewDialog

@author Sven Wiegand
 */
class CEmptyProjectTemplateItem : public CProjectTemplateItem {
    DECLARE_DYNCREATE(CEmptyProjectTemplateItem);

    // construction/destruction
public:
    CEmptyProjectTemplateItem();

    // overridings
public:
    BOOL InitItem(LPCTSTR lpszPath, CImageList &ImageList32, CImageList &ImageList16);
    const CString GetTitle() const;
    const CString GetDescription() const;
    int GetImageIndex() const;
    BOOL InitProject(CLaTeXProject *pProject, LPCTSTR lpszCr);

    // implementation helpers
protected:
    /**
    Creates the empty file.

    The file is not really empty -- it contains one line feed to store
    the format choosen by the user.

    @param lpszTargetPath
            Path, m_strPath should be copied to.
    @param lpszCrLf
            String to use as line feed.
     */
    BOOL CreateMainFile(LPCTSTR lpszTargetPath, LPCTSTR lpszCrLf);

    // attributes
private:
    /** the index to the image list */
    int m_nImageIndex;
};

/**
Represents a file based template for the project new dialog.

A file based template simply provides a file for the project's main
file.

@see CProjectNewDialog

@author Sven Wiegand
 */
class CFileBasedProjectTemplateItem : public CProjectTemplateItem {
    DECLARE_DYNCREATE(CFileBasedProjectTemplateItem);

    // construction/destruction
public:
    CFileBasedProjectTemplateItem();

    // overridings
public:
    BOOL InitItem(LPCTSTR lpszPath, CImageList &ImageList32, CImageList &ImageList16);
    const CString GetTitle() const;
    const CString GetDescription() const;
    int GetImageIndex() const;
    BOOL InitProject(CLaTeXProject *pProject, LPCTSTR lpszCr);

    // implementation helpers
protected:
    /**
    Copies the specified source file to the specified destination file
    whereby any leading description lines in the source will be skipped
    and the line feeds will be translated.

    @param lpszTargetPath
            Path, m_strPath should be copied to.
    @param lpszCrLf
            String to use as line feed.
     */
    BOOL CreateMainFile(LPCTSTR lpszTargetPath, LPCTSTR lpszCrLf);

    // attributes
private:
    /** the full path of the template file */
    CString m_strPath;

    /** the title */
    CString m_strTitle;

    /** the description */
    CString m_strDescription;

    /** the index to the image list */
    int m_nImageIndex;
};

/**
Represents a project wizard for the project new dialog.

@see CProjectNewDialog

@author Sven Wiegand
 */
class CWizardBasedProjectTemplateItem : public CProjectTemplateItem {
    DECLARE_DYNCREATE(CWizardBasedProjectTemplateItem);

    // construction/destruction
public:
    CWizardBasedProjectTemplateItem();

    // overridings
public:
    BOOL InitItem(LPCTSTR lpszPath, CImageList &ImageList32, CImageList &ImageList16);
    const CString GetTitle() const;
    const CString GetDescription() const;
    int GetImageIndex() const;
    BOOL InitProject(CLaTeXProject *pProject, LPCTSTR lpszCr);

    // attributes
private:
    /** 
    Full path to the dynamic link library (DLL) that implements the
    wizard.
     */
    CString m_strPath;

    /** Wrapper for the COM-interface to the wizard */
    InterfaceAccessWrappers::IProjectWizard m_wizard;

    /** the index to the image list */
    int m_nImageIndex;
};

/**
Dialog for creating a new project based on a template or a wizard.

@ingroup dialogs

@author Sven Wiegand
 */
class CProjectNewDialog : public CTemplateDialog {
    // construction/destruction
public:
    CProjectNewDialog(CWnd* pParent = NULL);
    virtual ~CProjectNewDialog();


    // implementation helpers
protected:
    /**
    Creates the full path to the specified directory.
     */
    BOOL CreateDirectoryRecursive(LPCTSTR lpszDirectory);

    // overridings
protected:
    void UpdateControlStates();
    void Create();

    //{{AFX_VIRTUAL(CProjectNewDialog)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    //}}AFX_VIRTUAL

    // implementation
protected:
    //{{AFX_MSG(CProjectNewDialog)
    afx_msg void OnChangeProjectName();
    afx_msg void OnChangeProjectPath();
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()

    // dialog data
protected:
    //{{AFX_DATA(CProjectNewDialog)
    enum {
        IDD = IDD_PROJECT_NEW
    };
    CButton m_wndOkButton;
    CBrowseButton m_wndBrowseButton;
    CString m_strProjectName;
    CString m_strProjectPath;
    int m_nFileFormat;
    BOOL m_bUseBibTex;
    BOOL m_bUseMakeIndex;
    //}}AFX_DATA

    // attributes
protected:
    /** String that specifies the base path for the project */
    CString m_strProjectBasePath;

    /** 
    TRUE if the current call to OnChangeProjectPath should be ignored.
     */
    BOOL m_bIgnoreProjectPathChange;
};

/** @} */


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_PROJECTNEWDIALOG_H__5588FFA4_2C94_11D3_929E_444553540000__INCLUDED_
