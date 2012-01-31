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

#include "OutputView.h"
#include "OutputInfo.h"
#include "FileGrep.h"
#include "OutputBuilder.h"
#include "StructureParser.h"
#include "Nullable.h"

class ErrorListPane;


class COutputDoc :
			public CCmdTarget,
			public CFileGrepHandler,
			public CParseOutputHandler
{
//Types and Friends
public:
	typedef enum tagHint
	{
		hintSelectBuildLine = 1,
		hintParsingFinished,
		hintSelectParseLine,
		hintSelectGrep1Line
		// skip as many values as there are grep windows
		// = 6
	} HINT;

protected:
	DECLARE_DYNCREATE(COutputDoc)

public:
	COutputDoc();
	virtual ~COutputDoc();

// operations
public:
	/**
	Activates the error, warning, bad box or grep result, that was
	reported in the	specified line of the LaTeX-output.
	 */
	void ActivateMessageByOutputLine(int nLine, COutputView* view);

	void ActivateGrepMessageByOutputLine(int nLine);
	void ActivateParseMessageByOutputLine(int nLine);
	void ActivateBuildMessageByOutputLine(int nLine);
	/**
	Must be called when a output view is activated.

	@param pView
	        Pointer to the output view that has been activated.
	 */
	void SetActiveView(OutputViewBase *pView);

	void SetAllViews(COutputView* pBuildView, COutputView* pGrepView1,
	                 COutputView* pGrepView2, COutputView* pParseView);

	/**
	Empties the build view and clears the error, warning and badbox
	arrays.
	 */
	void ClearBuildMessages();

	/**
	Adds an error to the error list.
	 */
	void AddError(COutputInfo& error);

	/**
	Adds a warning to the warning list.
	 */
	void AddWarning(COutputInfo& warning);

	/**
	Adds a bad box to the bad box list.
	 */
	void AddBadBox(COutputInfo& badbox);

	///Sets the pointer to the error list view
	void SetErrorListView(ErrorListPane* v) { errorListView_ = v; }

// implementation helpers
protected:
	/**
	Returns the full path of the specified file in the project.

	@param lpszFile
	        File to get the full path for.
	 */
	CString GetFilePath(LPCTSTR lpszFile);

	/** For updating the views attached to this "document" */
	void UpdateAllViews(COutputView* pSender, LPARAM lHint, CObject* pHint);

	/**
	Builds the project or the current file, depending on the
	m_bActiveFileOperation-attribute
	 */
	void DoLaTeXRun();

	/**
	Executes BibTeX for the project or the current file, depending on
	the m_bActiveFileOperation-attribute
	 */
	void DoBibTexRun();

	/**
	Executes MakeIndex for the project or the current file, depending on
	the m_bActiveFileOperation-attribute
	 */
	void DoMakeIndexRun();

	/**
	Returns the active document or NULL if there is none.
	 */
	CDocument *GetActiveDocument() const;

	///Assures, that the main file is saved and ready to be compiled.
	bool AssureExistingMainFile();

public:

// overridings
protected:
	//{{AFX_VIRTUAL(COutputDoc)
	//}}AFX_VIRTUAL

// debugging
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// message handlers
protected:
	//{{AFX_MSG(COutputDoc)
	afx_msg void OnNextError();
	afx_msg void OnPrevError();
	afx_msg void OnLatexView();
	afx_msg void OnUpdateLatexView(CCmdUI* pCmdUI);
	afx_msg void OnNextBadbox();
	afx_msg void OnUpdateNextPrevBadbox(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNextPrevError(CCmdUI* pCmdUI);
	afx_msg void OnNextWarning();
	afx_msg void OnUpdateNextPrevWarning(CCmdUI* pCmdUI);
	afx_msg void OnPrevBadbox();
	afx_msg void OnPrevWarning();
	afx_msg void OnEditFindInFiles();
	afx_msg void OnUpdateEditFindInFiles(CCmdUI* pCmdUI);
	afx_msg void OnEditNextGrepResult();
	afx_msg void OnUpdateGrepResultStep(CCmdUI* pCmdUI);
	afx_msg void OnEditPrevGrepResult();
	afx_msg void OnLatexStopBuild();
	afx_msg void OnUpdateLatexStopBuild(CCmdUI* pCmdUI);
	afx_msg void OnLatexRun();
	afx_msg void OnUpdateLatexRun(CCmdUI* pCmdUI);
	afx_msg void OnBibTex();
	afx_msg void OnUpdateBibTex(CCmdUI* pCmdUI);
	afx_msg void OnMakeIndex();
	afx_msg void OnUpdateMakeIndex(CCmdUI* pCmdUI);
	afx_msg void OnFileCompile();
	afx_msg void OnUpdateFileCompile(CCmdUI* pCmdUI);
	afx_msg void OnFileBibTex();
	afx_msg void OnUpdateFileBibTex(CCmdUI* pCmdUI);
	afx_msg void OnFileMakeIndex();
	afx_msg void OnUpdateFileMakeIndex(CCmdUI* pCmdUI);
	afx_msg void OnLatexClean();
	afx_msg void OnUpdateLatexClean(CCmdUI* pCmdUI);
	afx_msg void OnLatexRunAndView();
	afx_msg void OnLatexFileCompileAndView();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	bool IsBuildViewActive() const;

// operations
public:
	/**
	Gets the path of the main file of this project.

	@return
	        The path of the file that is the main file for this project. The
	        returned string is empty, if no main file has been set.
	 */
	CString GetMainPath() const;

	/**
	Gets the working directory for this project.

	@return
	        The working directory for this project. Latex will generate its
	        output in this directory. The returned string is empty, if no
	        working directory is set.
	 */
	CString GetWorkingDir() const;

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

// CTextSourceManager virtuals
public:
	/** @see CTextSourceManager::GetTextSource */
	//	virtual CTextSource *GetTextSource( LPCTSTR lpszPath );

// CStructureParserHandler virtuals
public:
	//	/** @see CStructureParserHandler::OnParsingFinished */
	//	virtual void OnParsingFinished( BOOL bSuccess );

// CFileGrepHandler virtuals
protected:
	virtual void OnFileGrepHit(CFileGrep *pFileGrep, LPCTSTR lpszPath,
	                           int nLine, LPCTSTR lpszLine);
	virtual void OnFileGrepError(CFileGrep *pFileGrep, LPCTSTR lpszPath);
	virtual void OnFileGrepFinished(CFileGrep *pFileGrep, int nHits);

//CParseOutputHandler
protected:
	virtual void OnParseLineInfo(COutputInfo &line, int nLevel, int nSeverity);
	virtual void OnParseBegin(bool bCancelState);
	virtual void OnParseEnd(bool bResult, int nFiles, int nLines);

protected:
	/**
	Shows the latex error with the specified index.

	@param nIndex Zero based index of the latex error to show.
	 */
	void ShowError(int nIndex);

	/**
	Shows the latex warning with the specified index.

	@param nIndex Zero based index of the latex warning to show.
	 */
	void ShowWarning(int nIndex);

	/**
	Shows the bad box with the specified index.

	@param nIndex Zero based index of the bad box to show.
	 */
	void ShowBadBox(int nIndex);

	/**
	Shows the grep result with the specified index.

	@param nIndex Zero based index of the grep result to show.
	 */
	void ShowGrepResult(int nIndex);

	/**
	Shows the parser warning with the specified index.

	@param nIndex Zero based index of the parser warning to show.
	 */
	void ShowParseWarning(int nIndex);

	/**
	Shows the parser information message with the specified index.

	@param nIndex Zero based index of the parser information message to show.
	 */
	void ShowParseInfo(int nIndex);

	/** Tries to open a file, if linenumber is non-zero.

	        Beeps otherwise.
	 */
	bool TryOpenFile(LPCTSTR lpszFilename, const Nullable<int>& nLineNumber);
	LRESULT DoTryOpenFile(WPARAM w, LPARAM l);
	// attributes
public:

protected:
	/** Pointer to attached parser-view. */
	COutputView *m_pParseView;

	/** Array containing parser information */
	COutputInfoArray m_aParseInfo;
	int m_nParseInfoIndex;

	/** Array containing parser warning messages */
	COutputInfoArray m_aParseWarning;
	int m_nParseWarningIndex;

	/** Array that contains information about the errors of the latex-compiler.*/
	COutputInfoArray m_aErrors;

	/** Array that contains information about the warnings of the latex-compiler.*/
	COutputInfoArray m_aWarnings;

	/** Array that contains information about the bad boxes of the latex-compiler.*/
	COutputInfoArray m_aBadBoxes;

	/** Index of the actual error in m_aLatexOutput */
	int m_nActualErrorIndex;

	/** Index of the actual warning in m_aLatexWarning */
	int m_nActualWarningIndex;

	/** Index of the actual bad box in m_aBadBoxes */
	int m_nActualBadBoxIndex;

	/** Pointer to the attached build-view. */
	COutputView *m_pBuildView;

	/** Pointer to the attached grep-views. */
	COutputView *m_apGrepView[2];

	/** File grep object */
	CFileGrep m_fileGrep;

	/** FALSE if file grep is running, TRUE otherwise */
	BOOL m_bCanGrep;

	/** Index of the currently working file grep output */
	int m_nWorkingFileGrep;

	/** Index of active file grep output */
	int m_nActiveFileGrep;

	/** Index of the active output line while grepping. */
	int m_nWorkingGrepOutputLine;

	/** structure for storing grep-hits. */
	typedef struct tagGrepHit
	{
		/** Path of the file, the hit occured in */
		CString strPath;

		/** Line in the file, the hit occured on */
		int nLine;

		/** Line in the grep view, this hit is listed on */
		int nOutputLine;
	} GREPHIT;

	typedef CArray<GREPHIT, GREPHIT&> CGrepHitArray;

	/** Array of grep hits for each grep window. */
	CGrepHitArray *m_apaGrepHits[2];

	/** Array of grep hits for grep view 1. */
	CGrepHitArray m_aGrepHits1;

	/** Array of grep hits for grep view 2. */
	CGrepHitArray m_aGrepHits2;

	/** Active grep result. */
	int m_anActiveGrepResult[2];

	/** Active output view. */
	OutputViewBase *m_pActiveOutputView;

	///List view showing all errors, warnings, bad boxes.
	ErrorListPane* errorListView_;

private:
	/** TRUE, if we can start latex compiler, FALSE otherwise. */
	BOOL m_bCanRunLatex;

	/**
	TRUE, if the following operations are not related to the project,
	but to the current file.
	 */
	BOOL m_bActiveFileOperation;

	/** Object used to build the output. */
	COutputBuilder m_builder;

public:
	// Clears all the warnings, errors, bad boxes etc.
	void ClearMessages();

	void ClearParseMessages();
	const CString GetCurrentProcessName() const;

	int GetBuildErrorCount() const;
	int GetBuildBadBoxCount() const;
	int GetBuildWarningCount() const;

	bool HasBuildErrors() const;
	bool HasBadBoxes() const;
	bool HasBuildWarnings() const;

	const CString GetTranscriptFilePath() const;
};
