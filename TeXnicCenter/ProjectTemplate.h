/********************************************************************
*
* This file is part of the TeXnicCenter-system
*
* Copyright (C) 1999-2000 Sven Wiegand
* Copyright (C) 2000-$CurrentYear$ ToolsCenter
* 
* This program is free software you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation either version 2 of
* the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
* If you have further questions or if you want to support
* further TeXnicCenter development, visit the TeXnicCenter-homepage
*
*    http://www.ToolsCenter.org
*
*********************************************************************/

#if !defined(AFX_PROJECTTEMPLATE_H__980BE3C6_25FC_11D5_A222_006097239934__INCLUDED_)
#define AFX_PROJECTTEMPLATE_H__980BE3C6_25FC_11D5_A222_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Project.h"

/**
This class is to CProject, what CDocTemplate is to CDocument.

See CDocTemplate for documentation.

@author Sven Wiegand
*/
class CProjectTemplate : public CCmdTarget
{
	DECLARE_DYNAMIC(CProjectTemplate)

// construction/destruction
protected:
	CProjectTemplate(UINT nIDResource, CRuntimeClass *pProjectClass, int nIconIndex = -1);

public:
	virtual ~CProjectTemplate();

// constants
public:
	enum ProjectStringIndex
	{
		windowTitle,        // default window title
		docName,            // user visible name for default document
		fileNewName,        // user visible name for FileNew
		// for file based documents:
		filterName,         // user visible name for FileOpen
		filterExt,          // user visible extension for FileOpen
		// for file based documents with Shell open support:
		regFileTypeId,      // REGEDIT visible registered file type identifier
		regFileTypeName,    // Shell visible registered file type name
	};

	enum Confidence
	{
		noAttempt,
		maybeAttemptForeign,
		maybeAttemptNative,
		yesAttemptForeign,
		yesAttemptNative,
		yesAlreadyOpen
	};

// operations
public:
	virtual void LoadTemplate();
	virtual POSITION GetFirstProjectPosition() const = 0;
	virtual CProject* GetNextProject(POSITION& rPos) const = 0;
	virtual void AddProject(CProject* pProject);      // must override
	virtual void RemoveProject(CProject* pProject);   // must override
	virtual Confidence MatchProjectType(LPCTSTR lpszPathName, CProject*& rpProjectMatch);
	virtual CProject* CreateNewProject();
	virtual BOOL SaveAllModified();     // for all documents
	virtual void CloseAllProjects(BOOL bEndSession);
	virtual CProject* OpenProjectFile(LPCTSTR lpszPathName) = 0;
	virtual void SetDefaultTitle(CProject* pProject) = 0;
	virtual BOOL GetProjectString(CString& rString, enum ProjectStringIndex index) const; // get one of the info strings

// overridings
protected:
	//{{AFX_VIRTUAL(CProjectTemplate)
	//}}AFX_VIRTUAL

// message handler
protected:
	//{{AFX_MSG(CProjectTemplate)
	afx_msg void OnProjectOpen();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

// attributes
public:
	BOOL m_bAutoDelete;

	int m_nIconIndex; // index of the icon in the executable, to use for this project type

protected:
	CRuntimeClass* m_pProjectClass;     // class for creating new projects

	CString m_strProjectStrings;    // '\n' separated names
	UINT m_nIDResource;                 // IDR_ for frame/menu/accel as well
		// The document names sub-strings are represented as _one_ string:
		// windowTitle\ndocName\n ... (see DocStringIndex enum)
};


/**
This class is to CProject, what CSingleDocTemplate is to CDocument.

Only single project templates are supported at the moment.

@author Sven Wiegand
*/
class CSingleProjectTemplate : public CProjectTemplate
{
	DECLARE_DYNAMIC(CSingleProjectTemplate)

// construction/destruction
public:
	CSingleProjectTemplate(UINT nIDResource, CRuntimeClass* pProjectClass, int nIconIndex = -1);
	~CSingleProjectTemplate();

// implementation
public:
	virtual void AddProject(CProject *pProject);
	virtual void RemoveProject(CProject *pProject);
	virtual POSITION GetFirstProjectPosition() const;
	virtual CProject* GetNextProject(POSITION& pos) const;
	virtual CProject* OpenProjectFile(LPCTSTR lpszPathName);
	virtual void SetDefaultTitle(CProject* pProject);

protected:  // standard implementation
	CProject *m_pOnlyProject;
};


/**
Handles all the available project types (CProjectTemplate).
*/
class CProjectManager : public CObject
{
	DECLARE_DYNAMIC(CProjectManager)

// construction/destruction
public:
	CProjectManager();
	~CProjectManager();

// operations
public:
	virtual void AddProjectTemplate(CProjectTemplate* pTemplate);
	virtual POSITION GetFirstProjectTemplatePosition() const;
	virtual CProjectTemplate* GetNextProjectTemplate(POSITION& pos) const;
	virtual void RegisterShellFileTypes(BOOL bCompat);
	void UnregisterShellFileTypes();
	virtual CProject* OpenProjectFile(LPCTSTR lpszFileName);
	virtual BOOL SaveAllModified();
	virtual void CloseAllProjects(BOOL bEndSession);
	virtual int GetOpenProjectCount();

	// helper for standard commdlg dialogs
	virtual BOOL DoPromptFileName(CString& fileName, UINT nIDSTitle,
			DWORD lFlags, BOOL bOpenFileDialog, CProjectTemplate* pTemplate);

//Commands
public:
	virtual BOOL OnDDECommand(LPTSTR lpszCommand);
	virtual void OnProjectNew();
	virtual void OnProjectOpen();

// Implementation
protected:
	int GetProjectCount(); // helper to count number of total documents

protected:
	CPtrList m_templateList;
	int GetDocumentCount(); // helper to count number of total documents

public:
	static BOOL bStaticInit;            // TRUE during static initialization
	static CList<CProjectTemplate*, CProjectTemplate*> *pStaticList;
	static CProjectManager *pStaticProjectManager; 
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_PROJECTTEMPLATE_H__980BE3C6_25FC_11D5_A222_006097239934__INCLUDED_
