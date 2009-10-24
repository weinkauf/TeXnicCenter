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
#pragma once

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"

#include "LatexProject.h"
//#include "MDIFrameManager.h"
#include "ProjectSupportingWinApp.h"
#include "ProjectTemplate.h"
#include "StyleFileContainer.h"
#include "SpellerBackgroundThread.h"

extern class CTeXnicCenterApp theApp;

class CTeXnicCenterApp :
			public CProjectSupportingWinApp,
			public SpellerSource
{
	UINT m_nApplicationLook;
	mutable CString module_name_;
	CDocTemplate* bibtex_doc_template_;
	CDocTemplate* metapost_doc_template_;

public:
	UINT GetApplicationLook() const;
	void SetApplicationLook(UINT val);

	const CString& GetModuleFileName() const;

	HINSTANCE scintilla_;


public:
	CTeXnicCenterApp();

// overridings
public:
	//virtual void ForwardCommands( HCONV hconvServer );

	// Override from CBCGWorkspace
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

//{{AFX_VIRTUAL(CTeXnicCenterApp)
public:
	virtual BOOL InitInstance();

	virtual int ExitInstance();
	virtual BOOL OnCmdMsg(UINT nID,int nCode,void* pExtra,AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL OnDDECommand(LPTSTR lpszCommand);
	//}}AFX_VIRTUAL

// message handlers
protected:
	//{{AFX_MSG(CTeXnicCenterApp)
	afx_msg void OnAppAbout();
	afx_msg void OnTip();
	afx_msg void OnFileNew();
	afx_msg void OnFileSaveAll();
	afx_msg void OnDisableStdCmd(CCmdUI* pCmdUI);
	afx_msg void OnProjectOpen();
	afx_msg void OnUpdateProjectClose(CCmdUI* pCmdUI);
	afx_msg void OnLatexNew();
	afx_msg void OnExtrasOptions();
	afx_msg void OnLaTeXProfile();
	afx_msg void OnDocumentSaved();
	afx_msg void OnUpdateFileMRU(CCmdUI* pCmdUI);
	afx_msg void OnLatexProfileSel();
	afx_msg void OnFileOpen();
	afx_msg void OnAppLicense();
	afx_msg void OnProjectSave();
	afx_msg void OnUpdateProjectSave(CCmdUI* pCmdUI);
	afx_msg void OnLatexEditProfiles();
	afx_msg void OnWindowCloseAll();
	afx_msg void OnHelp();
	afx_msg void OnUpdateProject();
	afx_msg void OnProjectNewFromFile();
	afx_msg void OnUpdateProjectNewFromFile(CCmdUI* pCmdUI);
	afx_msg void OnPackageSetup();
	afx_msg void OnUpdateDoForAllOpenWindows(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnUpdateFileMRUProjectList(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileMRUFileList(CCmdUI* pCmdUI);
	afx_msg void OnFileMRUProject(UINT unID);
	//	afx_msg void OnFileMRUFile( UINT unID );
	afx_msg void OnUpdateURL(CCmdUI* pCmdUI);
	afx_msg void OnURL(UINT unID);
	DECLARE_MESSAGE_MAP()
private:
	void ShowTipAtStartup(void);
private:
	void ShowTipOfTheDay(void);

// operations
public:
	/**
	Returns a pointer to the document template, all LaTeX-documents
	are based on.
	 */
	CMultiDocTemplate *GetLatexDocTemplate();

	/**
	Returns a pointer to the active LaTeXView-object or NULL, if there is
	none.
	 */
	LaTeXView* GetActiveEditView();

	/** Returns a pointer to the LaTeX-document with the specified path and read-only
	attribute. If the document is not open, it is NOT opened and NULL is returned.

	@param lpszFileName
	        Path of the file to find.
	@param bReadOnly
	        TRUE, if the file is write protected.

	@return
	        A pointer to an already opened document, or NULL if the document is
	        not already opend.
	 */
	CDocument *GetOpenLatexDocument(LPCTSTR lpszFileName,BOOL bReadOnly = FALSE);

	/**
	Returns a pointer to the LaTeX-document with the specified path.

	If this document is already opened with the same read-write-status,
	this method returns a pointer to this open document, otherwise the
	document is created at read.

	@param lpszFileName
	        Path of the file to open.
	@param bReadOnly
	        TRUE, if the file should be opened write protected.

	@return
	        A pointer to the specified document or NULL, if no such document
	        exists.
	 */
	CDocument* GetLatexDocument(LPCTSTR lpszFileName, BOOL bReadOnly = FALSE);

	/** Opens the specified file using OpenLatexDocument.

	Overwritten from CWinApp. Uses directly OpenLatexDocument to
	fulfill its task.

	@param lpszFileName
	        Path of the file to open.

	@return A pointer to the document.
	 */
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);

	/**
	Opens the specified File at the specified line. If the file is already
	open, its main view is activated and the cursor is set to the specified
	line.

	@param lpszFileName
	        Path of the file to open.
	@param bReadOnly
	        TRUE, it the file should be opened write protected.
	@param nLineNumber
	        Number of the line to set the caret on or -1.
	@param bError
	        If bError is TRUE, a mark is set in the document on the specified line.
	@param bAskForProjectLoad
	        If true and a project with a similar name is found, the user will be asked
	        to load this project.

	@return A pointer to the document.
	 */
	CDocument* OpenLatexDocument(LPCTSTR lpszFileName,BOOL bReadOnly = FALSE,
	                             int nLineNumber = -1,BOOL bError = FALSE,
	                             bool bAskForProjectLoad = true);

	/**
	Returns a string associated with the project document template.

	See CDocTemplate::GetDocString() for further explanation.
	 */
	CString GetProjectString(enum CProjectTemplate::ProjectStringIndex index);

	/**
	Returns a string associated with the latex document template.

	See CDocTemplate::GetDocString() for further explanation.
	 */
	CString GetLatexString(enum CDocTemplate::DocStringIndex index);

	/**
	Returns a complete filter string for the lpszFilter parameter to the
	CFileDialog-constructor.
	 */
	CString GetProjectFileFilter();

	/**
	Returns a complete filter string for the lpszFilter parameter to the
	CFileDialog-constructor.
	 */
	CString GetLatexFileFilter();

	/**
	Saves all modified files, without prompting the user.
	 */
	void SaveAllModifiedWithoutPrompt();

	/**
	Opens the procject with the specified path.

	If there is already a project open, it is closed using
	OnCloseProject().

	@see #OnCloseProject

	@param lpszPath
	        Path of the project to open.

	@return
	        TRUE if successfull, FALSE otherwise.
	 */
	BOOL OpenProject(LPCTSTR lpszPath);

	/**
	Returns a pointer to the one and only <var>CLaTeXProject</var>-object
	of the application or <var>NULL</var> if no such object exists.
	 */
	CLaTeXProject *GetProject();

	/**
	Returns the path of the directory, that was the current one, while
	TeXnicCenter was started (working direcotry).
	 */
	CString GetWorkingDir();

	/**
	Prepares for ending the session.

	Returns to the caller, before application has quit. Does some simple
	cleaning up, i.e. closing current project, etc.
	 */
	void EndSession();

	/**
	Updates the ComboBox for output profile selection.
	 */
	void UpdateLaTeXProfileSel();

	/**
	Opens the specified URL.

	@return
	        FALSE if an error occurd, TRUE otherwise.
	 */
	BOOL OpenURL(LPCTSTR lpszURL);

	/**
	Retrieves the background thread.

	@return
	        Pointer to background thread.
	        else NULL.
	 */
	SpellerBackgroundThread* GetSpellerThread();

// overrides
public:
	// CSpellerSource
	virtual Speller* GetSpeller();

// implementation helpers
protected:
	/**
	Fills the menu of the most recent used files specified by the
	pCmdUI parameter with the files from the specified list.

	@param pCmdUI
	        Points to the submenu to fill with the most recent used files.
	@param recentFileList
	        List of recent used files, that should be used to build the menu.
	@param unFirstCommandId
	        Command ID to be used for the first entry in the menu, the
	        following entries will retrieve the increased values.
	@param unNoFileStringId
	        Resource identifier of the string to use instead, if the list
	        of recent files is empty.
	 */
	static void UpdateRecentFileList(CCmdUI *pCmdUI,CRecentFileList &recentFileList,UINT unFirstCommandId,UINT unNoFileStringId);

// attributes
public:
	CStyleFileContainer m_AvailableCommands;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	/** Accelerator manager */
	//CBCGKeyboardManager m_keyboardManager;

	/** Context menu manager */
	//CBCGContextMenuManager m_contextMenuManager;

	/** TRUE, if the session is ending, FALSE otherwise. */
	BOOL m_bEndSession;

	/** The path of the application in the registry unter "HKCU\\Software". */
	CString m_strRegistryRoot;

	/** Frame manager that handle the MDI-child-frames. */
	//CMDIFrameManager *m_pMDIFrameManager;

	/** List of recently used projects. */
	CRecentFileList m_recentProjectList;

private:
	/** Specifies, whether to show document tabs or not */
	BOOL m_bMDITabs;

public:
	bool GetShowMDITabs() const;
	void SetShowMDITabs(bool val = true);

	/** Specifies, whether to show icons on document tabs or not */
	BOOL m_bMDITabIcons;

	/** Location, where to display the document tabs. */
	int m_nMDITabLocation;

	/** Style, how the document tabs are displayed. */
	int m_nMDITabStyle;

	/** Infos about current windows version */
	CSystemInfo m_SystemInfo;
protected:
	void FindPackageFiles();
	void FindPackageFilesRecursive(CString dir);
	const CString GetDDEServerName() const;
	/** Document template for normal latex-documents */
	CMultiDocTemplate *m_pLatexDocTemplate;

	/** Document template for projects */
	CSingleProjectTemplate *m_pProjectDocTemplate;

	/** TRUE, if we are currently saving all files, FALSE otherwise. */
	BOOL m_bSavingAll;

	/**
	Working directory .
	(Directory that was the current one while start up of TeXnicCenter).
	 */
	CString m_strWorkingDir;

	/**
	Handle to localized version of TXC-resource-DLL.
	 */
	HINSTANCE m_hTxcResources;

	/** Spell checker */
	Speller *m_pSpell;

	/** Background thread that processes spelling and other tasks */
	SpellerBackgroundThread *m_pBackgroundThread;

	/** Critical section to protect lazy resource initialization */
	CRITICAL_SECTION m_csLazy;

protected:
	int DoMessageBox(LPCTSTR prompt, UINT type, UINT prompt_id);
	virtual bool DoTaskDialog( HWND hWnd, LPCTSTR prompt, UINT nType, int& result);
	void OnBibTeXNew();
	void OnMetaPostNew();

public:
	CDocTemplate* GetBibTeXDocTemplate() const;
	CDocTemplate* GetMetaPostDocTemplate() const;
};

inline
CLaTeXProject *CTeXnicCenterApp::GetProject()
{
	if (!m_pProjectDocTemplate)
		return NULL;

	POSITION pos = m_pProjectDocTemplate->GetFirstProjectPosition();
	if (!pos)
		return NULL;

	return (CLaTeXProject*) m_pProjectDocTemplate->GetNextProject(pos);
}

inline
CString CTeXnicCenterApp::GetWorkingDir()
{
	return m_strWorkingDir;
}

inline
CMultiDocTemplate *CTeXnicCenterApp::GetLatexDocTemplate()
{
	return m_pLatexDocTemplate;
}
