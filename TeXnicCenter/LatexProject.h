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

#if !defined(AFX_LATEXPROJECT_H__75A37CC1_20BE_11D3_929E_D32D9B17C664__INCLUDED_)
#define AFX_LATEXPROJECT_H__75A37CC1_20BE_11D3_929E_D32D9B17C664__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OutputInfo.h"
#include "StructureParser.h"
#include "MainFrm.h"
#include "StructureTreeCtrl.h"
#include "EnvironmentView.h"
#include "FileView.h"
#include "BibView.h"
#include "Project.h"
#include "IProject.h"
#include "WorkspacePane.h"

class CLaTeXProject :
			public CProject,
			public CStructureParserHandler
{
	CBaseTabbedPane* tabbed_pane_;

protected:
	CLaTeXProject(); // Dynamische Erstellung verwendet geschützten Konstruktor
	DECLARE_DYNCREATE(CLaTeXProject)

public:
	virtual ~CLaTeXProject();

// operations
public:
	/**
	Specifies whether to run BibTeX or not.
	 */
	void SetRunBibTex(BOOL bRunBibTex);

	/**
	Specifies whether to run MakeIndex or not.
	 */
	void SetRunMakeIndex(BOOL bRunMakeIndex);

	/**
	Returns TRUE if BibTeX should be run on compilation and FALSE
	otherwise.
	 */
	BOOL GetRunBibTex() const;

	/**
	Returns TRUE if MakeIndex should be run on compilation and FALSE
	otherwise.
	 */
	BOOL GetRunMakeIndex() const;

	/**
	Sets the path of the main file of this project.

	@param lpszMainPath
	        Path of the file that is the main file for this project. This
	        file is send to the latex-compiler.
	 */
	void SetMainPath(LPCTSTR lpszMainPath);

	/**
	Gets the path of the main file of this project.

	@return
	        The path of the file that is the main file for this project. The
	        returned string is empty, if no main file has been set.
	 */
	const CString& GetMainPath() const;

	/**
	Gets the working directory for this project.

	@return
	        The working directory for this project. Latex will generate its
	        output in this directory. The returned string is empty, if no
	        working directory is set.
	 */
	CString GetWorkingDir() const;

	/**
	Sets the project's directory.

	@param lpszProjectDir
	        The full path of the directory, the project file is placed in.
	 */
	void SetProjectDir(LPCTSTR lpszProjectDir);

	/**
	Gets the directory, the project file is placed in.
	 */
	const CString GetProjectDir() const;

	/**
	Returns the full path of the specified file in the project.

	@param lpszFile
	        File to get the full path for.
	 */
	const CString GetFilePath(LPCTSTR lpszFile) const;

	/**
	Returns the full path to the project session file
	 */
	const CString GetSessionPathName(LPCTSTR lpszPath = NULL) const;

	/**
	Sets the actual selected structure item.

	@param nIndex
	        Index of the current selected structure item or -1 if
	        no item is selected.
	 */
	void SetCurrentStructureItem(int nIndex);

	void DoProjectSave();

	Interfaces::IProject *GetProjectInterface();

// implementation helpers
protected:
	/**
	Called for serializing project's properties.

	@param ini
	        The ini-file, the project's information should be read from
	        or written to. You do not have to call the Read() or Write()
	        method of the ini file object. That is done by the framework.
	@param bWrite
	        FALSE if the information should be loaded from the ini file,
	        TRUE if it should be written to.

	@return
	        Should return FALSE only, if the important project data has
	        not been read successfully and TRUE otherwise.
	 */
	BOOL Serialize(CIniFile &ini, BOOL bWrite);

	/**
	Called to serialize the project's session (open documents, etx.).

	@param ini
	        The ini-file to read the session from or to write the session to.
	@param bWrite
	        FALSE if the session should be read from the file, TRUE if it
	        should be written to the file.
	 */
	void SerializeSession(CIniFile &ini, BOOL bWrite);

	void DeleteProjectViews();
	BOOL CreateProjectViews();

//Statics
public:
	///Returns a possible project file name, if doc (tex) file name is given.
	static CString GetProjectFileNameFromDoc(LPCTSTR lpszDocPathName);

	/** Checks for an existing project file given by lpszPathName.

	        Asks the user to load the exiting project file.

	        @returns true, if the existing one shall be loaded.
	        @returns false, if no project is existing or the user does not care about it.
	 */
	static bool CheckExistingProjectFile(LPCTSTR lpszPathName);

// overridings
public:
	virtual void OnCloseProject();
	virtual BOOL OnOpenProject(LPCTSTR lpszPathName);
	virtual BOOL OnNewProjectFromDoc(LPCTSTR lpszDocPathName);
	virtual void SetPathName(LPCTSTR lpszPathName);
	virtual BOOL OnSaveProject(LPCTSTR lpszPathName);
	virtual BOOL SaveModified();
protected:
	virtual BOOL OnNewProject();

// debugging
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// message handlers
protected:
	CMainFrame* m_pwndMainFrame;
	//CWorkspaceBar* m_pwndWorkspaceBar;
	//COutputBar* m_pwndOutputBar;

	const CString GetIgnoredWordsFileName() const;

	//{{AFX_MSG(CLaTeXProject)
	afx_msg void OnProjectProperties();
	afx_msg void OnProjectOpenMainfile();
	afx_msg void OnNextBadbox();
	afx_msg void OnProjectParsed();
	afx_msg void OnProjectParse();
	afx_msg void OnItemProperties();
	afx_msg void OnUpdateItemCmd(CCmdUI* pCmdUI);
	afx_msg void OnItemGoto();
	afx_msg void OnItemInsertPageref();
	afx_msg void OnItemInsertRef();
	afx_msg void OnItemInsertLabel();
	afx_msg void OnSpellProject();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// CStructureParserHandler virtuals
public:
	/** @see CStructureParserHandler::OnParsingFinished */
	virtual void OnParsingFinished(BOOL bSuccess);

// properties
protected:
	/** path of the main latex-file of the project */
	CString m_strMainPath;

	/** Path of the directory, the project file is placed in */
	CString m_strProjectDir;

	/** TRUE if the project uses BibTeX */
	BOOL m_bUseBibTex;

	/** TRUE if the project uses MakeIndex */
	BOOL m_bUseMakeIndex;

	/** Project specific used language and dialect **/
	CString m_strProjectLanguage;
	CString m_strProjectDialect;

// attributes
public:
	/** Contains result of parsing. */
	StructureItemContainer m_aStructureItems;

	const StructureItemContainer& GetStructureItems() const;
	//void SetStructureItems(const StructureItemContainer& val);

protected:
	/** Pointer to the structure parser object that parses the project structure */
	CStructureParser *m_pStructureParser;

	/** Actual parsing item or -1 if no one is selected. */
	int m_nCurrentStructureItem;

private:
	/** TRUE, if we are allowed to parse the project, FALSE otherwise. */
	BOOL m_bCanParse;

	/** TRUE, if we can start latex compiler, FALSE otherwise. */
	BOOL m_bCanRunLatex;

	///** View which shows the document structure. */
	//CStructureView* m_pwndStructureView;

	///** View which shows all objects of the document. */
	//CEnvironmentView* m_pwndEnvironmentView;

	///** View which shows all files of the document. */
	//CFileView* m_pwndFileView;

	///** View which shows all bibtex items the document. */
	//CBibView* m_pwndBibView;

	/**
	After loading the project, this is the index of the navigator
	tab to activate.
	 */
	mutable int m_nInitialNavigatorTab;

// aggregates
public:
	DECLARE_INTERFACE_MAP()

	BEGIN_INTERFACE_PART(Project, InterfaceImplementationWrappers::IProject)
	CString GetTitle_() const;
	CString GetPath_() const;
	CString GetMainFile_() const;
	BOOL GetUsesBibTex_() const;
	void SetUsesBibTex_(BOOL bUseBibTex);
	BOOL GetUsesMakeIndex_() const;
	void SetUsesMakeIndex_(BOOL bUseMakeIndex);
	END_INTERFACE_PART(Project)
	int GetInitialNavigatorTabIndex() const;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_LATEXPROJECT_H__75A37CC1_20BE_11D3_929E_D32D9B17C664__INCLUDED_
