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

#if !defined(AFX_PROJECT_H__980BE3C3_25FC_11D5_A222_006097239934__INCLUDED_)
#define AFX_PROJECT_H__980BE3C3_25FC_11D5_A222_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// forwards
class CProjectView;
class CProjectTemplate;

/**
This class can be used for classes that are implementing a project
management to the user (some kind of document).

It provides the functionality known from CDocument but in difference
to CDocument objects of this class can always receive commands, even
if they don't have any views or if no one of the document's views
are active.

Views, attached to objects, of this class don't have to be derived
from CView -- they just have to be derived from CWnd and they have
to implement the pure vitual class CProjectView.

This class handles the command messages ID_PROJECT_OPEN, 
ID_PROJECT_SAVE and ID_PROJECT_CLOSE.

@author Sven Wiegand
 */
class CProject : public CCmdTarget {
    DECLARE_DYNAMIC(CProject)

    // construction/destruction
public:
    CProject();
    virtual ~CProject();

    // operations
public:
    void AddView(CProjectView *pView);
    virtual POSITION GetFirstViewPosition() const;
    virtual CProjectView *GetNextView(POSITION &pos) const;
    const CString &GetPathName() const;
    const CString &GetTitle() const;
    BOOL IsModified() const;
    void RemoveView(CProjectView *pView);
    void SetModifiedFlag(BOOL bModified = TRUE);
    virtual void SetPathName(LPCTSTR lpszPathName);
    virtual void SetTitle(LPCTSTR lpszTitle);
    void UpdateAllViews(CProjectView *pSender, LPARAM lHint = 0, LPVOID pHint = NULL);
    CProjectTemplate *GetProjectTemplate() const;
    BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace = TRUE);
    BOOL DoFileSave();

    // overridables
public:
    virtual void DeleteContents();
    virtual void OnChangedViewList();
    virtual void OnCloseProject();
    virtual BOOL OnNewProject();
    virtual BOOL OnNewProjectFromDoc(LPCTSTR lpszDocPathName);
    virtual BOOL OnOpenProject(LPCTSTR lpszPathName);
    virtual BOOL OnSaveProject(LPCTSTR lpszPathName);
    virtual void ReportSaveLoadException(LPCTSTR lpszPathName, CException *e, BOOL bSaving, UINT nIDPDefault);
    virtual CFile *GetFile(LPCTSTR lpszFileName, UINT nOpenFlags, CFileException *pError);
    virtual void ReleaseFile(CFile *pFile, BOOL bAbort);
    virtual BOOL SaveModified();

    // overridings
protected:
    //{{AFX_VIRTUAL(CProject)
    //}}AFX_VIRTUAL

    // message handlers
protected:
    //{{AFX_MSG(CProject)
    afx_msg void OnProjectClose();
    afx_msg void OnUpdateProjectClose(CCmdUI* pCmdUI);
    afx_msg void OnProjectSave();
    afx_msg void OnUpdateProjectSave(CCmdUI* pCmdUI);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()

    // attributes
public:
    CString m_strTitle;
    CString m_strPathName;
    BOOL m_bModified;
    CList<CProjectView*, CProjectView*> m_viewList;
    CProjectTemplate *m_pProjectTemplate;

private:
    bool m_bIsClosing;

public:
    bool IsClosing() const;
};


// inlines

inline
const CString &CProject::GetPathName() const
{
    return m_strPathName;
}

inline
const CString &CProject::GetTitle() const
{
    return m_strTitle;
}

inline
BOOL CProject::IsModified() const
{
    return m_bModified;
}

inline
CProjectTemplate *CProject::GetProjectTemplate() const
{
    return m_pProjectTemplate;
}


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_PROJECT_H__980BE3C3_25FC_11D5_A222_006097239934__INCLUDED_
