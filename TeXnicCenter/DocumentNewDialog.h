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

#if !defined(AFX_DOCUMENTNEWDIALOG_H__431478E5_B4F5_11D5_A222_006097239934__INCLUDED_)
#define AFX_DOCUMENTNEWDIALOG_H__431478E5_B4F5_11D5_A222_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LatexProject.h"
#include "IDocumentWizard.h"
#include "BrowseButton.h"
#include "TemplateDialog.h"

/** @addtogroup templates
        @{
 */

/**
Abstract base class for all elements listed in the template list
control of the document new dialog.

@see CDocumentNewDialog

@author Sven Wiegand
 */
class CDocumentTemplateItem : public CTemplateItem {
    // operations
public:
    /** 
    Called to init the specified document based on the template item.

    @param lpszPath
            Path of the generated file.
    @param lpszCr
            String to use as carriage return.

    @return
            Nonzero if the creation should be finished and FALSE if the 
            creation should be cancelled.
     */
    virtual BOOL InitDocument(LPCTSTR lpszPath, LPCTSTR lpszCr) = 0;
};

/**
Represents an empty document template.

@see CDocumentNewDialog

@author Sven Wiegand
 */
class CEmptyDocumentTemplateItem : public CDocumentTemplateItem {
    DECLARE_DYNCREATE(CEmptyDocumentTemplateItem);

    // construction/destruction
public:
    CEmptyDocumentTemplateItem();

    // overridings
public:
    BOOL InitItem(LPCTSTR lpszPath, CImageList &ImageList32, CImageList &ImageList16);
    const CString GetTitle() const;
    const CString GetDescription() const;
    int GetImageIndex() const;
    BOOL InitDocument(LPCTSTR lpszPath, LPCTSTR lpszCr);

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
    BOOL CreateFile(LPCTSTR lpszTargetPath, LPCTSTR lpszCrLf);

    // attributes
private:
    /** the index to the image list */
    int m_nImageIndex;
};

/**
Represents a file based template for the document new dialog.

@see CDocumentNewDialog

@author Sven Wiegand
 */
class CFileBasedDocumentTemplateItem : public CDocumentTemplateItem {
    DECLARE_DYNCREATE(CFileBasedDocumentTemplateItem);

    // construction/destruction
public:
    CFileBasedDocumentTemplateItem();

    // overridings
public:
    BOOL InitItem(LPCTSTR lpszPath, CImageList &ImageList32, CImageList &ImageList16);
    const CString GetTitle() const;
    const CString GetDescription() const;
    int GetImageIndex() const;
    BOOL InitDocument(LPCTSTR lpszPath, LPCTSTR lpszCr);

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
    BOOL CreateFile(LPCTSTR lpszTargetPath, LPCTSTR lpszCrLf);

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
Represents a document wizard for the document new dialog.

@see CDocumentNewDialog

@author Sven Wiegand
 */
class CWizardBasedDocumentTemplateItem : public CDocumentTemplateItem {
    DECLARE_DYNCREATE(CWizardBasedDocumentTemplateItem);

    // construction/destruction
public:
    CWizardBasedDocumentTemplateItem();

    // overridings
public:
    BOOL InitItem(LPCTSTR lpszPath, CImageList &ImageList32, CImageList &ImageList16);
    const CString GetTitle() const;
    const CString GetDescription() const;
    int GetImageIndex() const;
    BOOL InitDocument(LPCTSTR lpszPath, LPCTSTR lpszCr);

    // attributes
private:
    /** 
    Full path to the dynamic link library (DLL) that implements the
    wizard.
     */
    CString m_strPath;

    /** Wrapper for the COM-interface to the wizard */
    InterfaceAccessWrappers::IDocumentWizard m_wizard;

    /** the index to the image list */
    int m_nImageIndex;
};

/**
Dialog for creating a new document based on a template or a wizard.

@ingroup dialogs

@author Sven Wiegand
 */
class CDocumentNewDialog : public CTemplateDialog {
    // construction/destruction
public:
    CDocumentNewDialog(CWnd* pParent = NULL);
    virtual ~CDocumentNewDialog();

    // overridings
protected:
    void Create();

    //{{AFX_VIRTUAL(CDocumentNewDialog)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    //}}AFX_VIRTUAL

    // implementation
protected:
    //{{AFX_MSG(CDocumentNewDialog)
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()

    // dialog data
protected:
    //{{AFX_DATA(CDocumentNewDialog)
    enum {
        IDD = IDD_DOCUMENT_NEW
    };
    int m_nFileFormat;
    //}}AFX_DATA

    // attributes
protected:
};

/** @} */


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_DOCUMENTNEWDIALOG_H__431478E5_B4F5_11D5_A222_006097239934__INCLUDED_
