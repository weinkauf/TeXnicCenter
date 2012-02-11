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

/********************************************************************
 *
 * $Id$
 *
 ********************************************************************/

#include "stdafx.h"
#include "resource.h"

#include "MainFrm.h"
#include "Project.hpp"
#include "ProjectView.h"
#include "ProjectTemplate.h"
#include "ProjectSupportingWinApp.h"
#include "RunTimeHelper.h"
#include "CodeDocument.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// class CProject
//-------------------------------------------------------------------

IMPLEMENT_DYNAMIC(CProject, CCmdTarget)

BEGIN_MESSAGE_MAP(CProject, CCmdTarget)
	//{{AFX_MSG_MAP(CProject)
	ON_COMMAND(ID_PROJECT_CLOSE, OnProjectClose)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_CLOSE, OnUpdateProjectClose)
	ON_COMMAND(ID_PROJECT_SAVE, OnProjectSave)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_SAVE, OnUpdateProjectSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CProject::CProject()
		: m_bModified(FALSE)
		, m_pProjectTemplate(NULL)
		, m_bIsClosing(false)
{
}

CProject::~CProject()
{
}

void CProject::AddView(CProjectView *pView)
{
	ASSERT(!pView->IsAttached()); // must not be already attached
	ASSERT(m_viewList.Find(pView, NULL) == NULL); // must not be in list

	m_viewList.AddTail(pView);
	ASSERT(pView->m_pProject == NULL); // must be un-attached
	pView->m_pProject = this;
	pView->OnOpeningProject();

	OnChangedViewList(); // must be the last thing done to the document
}

POSITION CProject::GetFirstViewPosition() const
{
	return m_viewList.GetHeadPosition();
}

CProjectView *CProject::GetNextView(POSITION &pos) const
{
	ASSERT(pos != BEFORE_START_POSITION);
	// use CProject::GetFirstViewPosition instead !

	if (pos == NULL)
		return NULL; // nothing left

	CProjectView* pView = dynamic_cast<CProjectView*>(m_viewList.GetNext(pos));
	ASSERT(pView);
	return pView;
}

void CProject::RemoveView(CProjectView *pView)
{
	if (pView->IsAttached()) {
		m_viewList.RemoveAt(m_viewList.Find(pView));
		pView->m_pProject = NULL;
		pView->Clear();

		OnChangedViewList(); // must be the last thing done to the document
	}
}

void CProject::SetModifiedFlag(BOOL bModified /*= TRUE*/)
{
	m_bModified = bModified;
}

void CProject::SetPathName(LPCTSTR lpszPathName)
{
	// store the path fully qualified
	TCHAR szFullPath[MAX_PATH];
	LPTSTR szDummy;
	GetFullPathName(lpszPathName, MAX_PATH, szFullPath, &szDummy);
	m_strPathName = szFullPath;
	ASSERT(!m_strPathName.IsEmpty()); // must be set to something
	ASSERT_VALID(this);

	// set the document title based on path name
	TCHAR szTitle[_MAX_FNAME];
	if (GetFileTitle(szFullPath, szTitle, _MAX_FNAME) == 0)
		SetTitle(szTitle);

	ASSERT_VALID(this);
}

void CProject::SetTitle(LPCTSTR lpszTitle)
{
	m_strTitle = lpszTitle;
}

void CProject::UpdateAllViews(CProjectView *pSender, LPARAM lHint/*= 0*/, LPVOID pHint/*= NULL*/)
{
	ASSERT(pSender == NULL || !m_viewList.IsEmpty());
	// must have views if sent by one of them

	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CProjectView* pView = GetNextView(pos);
		if (pView != pSender)
			pView->OnUpdate(pSender, lHint, pHint);
	}
}

void CProject::DeleteContents()
{
}

void CProject::OnChangedViewList()
{
}

void CProject::OnClosing()
{
	POSITION pos = GetFirstViewPosition();
	while (pos)
	{
		CProjectView *pView = GetNextView(pos);
		if (!pView)
			continue;

		pView->OnClosingProject();
		pView->m_pProject = NULL;
	}

	m_viewList.RemoveAll();

	// clean up contents of document
	DeleteContents();
}

BOOL CProject::OnNewProject()
{
	if (IsModified())
	{
		TRACE0("Warning: OnNewProject replaces an unsaved document.\n");
	}

	DeleteContents();
	m_strPathName.Empty(); // no path name yet
	SetModifiedFlag(FALSE); // make clean

	return TRUE;
}

BOOL CProject::OnNewProjectFromDoc(LPCTSTR /*lpszDocPathName*/)
{
	//Standard Implementation is like OnNewProject
	return CProject::OnNewProject();
}

BOOL CProject::OnOpenProject(LPCTSTR lpszPathName)
{
	if (IsModified())
	{
		TRACE0("Warning: OnOpenProject replaces an unsaved document.\n");
	}

	CFileException fe;
	CFile* pFile = GetFile(lpszPathName,
	                       CFile::modeRead | CFile::shareDenyWrite, &fe);
	if (pFile == NULL)
	{
		ReportSaveLoadException(lpszPathName, &fe,
		                        FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		return FALSE;
	}

	DeleteContents();
	SetModifiedFlag(); // dirty during de-serialize

	CArchive loadArchive(pFile, CArchive::load | CArchive::bNoFlushOnDelete);
	loadArchive.m_pDocument = reinterpret_cast<CDocument*>(this);
	loadArchive.m_bForceFlat = FALSE;

	try
	{
		CWaitCursor wait;
		if (pFile->GetLength() != 0)
			Serialize(loadArchive); // load me
		loadArchive.Close();
		ReleaseFile(pFile, FALSE);
	}
	catch (CException *pE)
	{
		ReleaseFile(pFile, TRUE);
		DeleteContents(); // remove failed contents

		try
		{
			ReportSaveLoadException(lpszPathName, pE,
			                        FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		}
		catch (CException *pE)
		{
			delete pE;
		}
		catch (...)
		{
		}

		delete pE;
		return FALSE;
	}
	catch (...)
	{
	}

	UpdateFrameTitle();

	SetModifiedFlag(FALSE); // start off with unmodified

	return TRUE;
}

BOOL CProject::OnSaveProject(LPCTSTR lpszPathName)
{
	CFileException fe;
	CFile* pFile = NULL;
	pFile = GetFile(lpszPathName, CFile::modeCreate |
	                CFile::modeReadWrite | CFile::shareExclusive, &fe);

	if (pFile == NULL)
	{
		ReportSaveLoadException(lpszPathName, &fe,
		                        TRUE, AFX_IDP_INVALID_FILENAME);
		return FALSE;
	}

	CArchive saveArchive(pFile, CArchive::store | CArchive::bNoFlushOnDelete);
	saveArchive.m_pDocument = reinterpret_cast<CDocument*>(this);
	saveArchive.m_bForceFlat = FALSE;
	try
	{
		CWaitCursor wait;
		Serialize(saveArchive); // save me
		saveArchive.Close();
		ReleaseFile(pFile, FALSE);
	}
	catch (CException *pE)
	{
		ReleaseFile(pFile, TRUE);

		try
		{
			ReportSaveLoadException(lpszPathName, pE,
			                        TRUE, AFX_IDP_FAILED_TO_SAVE_DOC);
		}
		catch (CException *pE)
		{
			delete pE;
		}
		catch (...)
		{
		}

		delete pE;
		return FALSE;
	}
	catch (...)
	{
	}

	SetModifiedFlag(FALSE); // back to unmodified

	return TRUE; // success
}

void CProject::ReportSaveLoadException(LPCTSTR lpszPathName, CException *e, BOOL bSaving, UINT nIDPDefault)
{
	UINT nIDP = nIDPDefault;
	UINT nHelpContext = nIDPDefault;
	CString prompt;

	if (e != NULL)
	{
		ASSERT_VALID(e);
		if (e->IsKindOf(RUNTIME_CLASS(CUserException)))
			return; // already reported

		if (e->IsKindOf(RUNTIME_CLASS(CArchiveException)))
		{
			switch (((CArchiveException*)e)->m_cause)
			{
				case CArchiveException::badSchema :
				case CArchiveException::badClass :
				case CArchiveException::badIndex :
				case CArchiveException::endOfFile :
					nIDP = AFX_IDP_FAILED_INVALID_FORMAT;
					break;
				default:
					break;
			}
		}
		else if (e->IsKindOf(RUNTIME_CLASS(CFileException)))
		{
			TRACE1("Reporting file I/O exception on Save/Load with lOsError = $%lX.\n",
			       ((CFileException*)e)->m_lOsError);

			CFileException* pFileException = (CFileException*)e;
			if (pFileException->m_strFileName.IsEmpty())
				pFileException->m_strFileName = lpszPathName;

			LPTSTR lpszMessage = prompt.GetBuffer(255);
			ASSERT(lpszMessage != NULL);
			if (!e->GetErrorMessage(lpszMessage, 256, &nHelpContext))
			{
				switch (((CFileException*)e)->m_cause)
				{
					case CFileException::fileNotFound :
					case CFileException::badPath :
						nIDP = AFX_IDP_FAILED_INVALID_PATH;
						break;
					case CFileException::diskFull :
						nIDP = AFX_IDP_FAILED_DISK_FULL;
						break;
					case CFileException::accessDenied :
						nIDP = bSaving ? AFX_IDP_FAILED_ACCESS_WRITE :
						       AFX_IDP_FAILED_ACCESS_READ;
						break;

					case CFileException::badSeek :
					case CFileException::genericException :
					case CFileException::tooManyOpenFiles :
					case CFileException::invalidFile :
					case CFileException::hardIO :
					case CFileException::directoryFull :
						break;

					default:
						break;
				}
			}
			prompt.ReleaseBuffer();
		}
	}

	if (prompt.IsEmpty())
	{
		TCHAR szTitle[MAX_PATH];
		GetFileTitle(lpszPathName, szTitle, MAX_PATH);
		AfxFormatString1(prompt, nIDP, szTitle);
	}

	AfxMessageBox(prompt, MB_ICONEXCLAMATION, nHelpContext);
}

CFile *CProject::GetFile(LPCTSTR lpszFileName, UINT nOpenFlags, CFileException *pError)
{
	CMirrorFile* pFile = new CMirrorFile;
	ASSERT(pFile != NULL);
	if (!pFile->Open(lpszFileName, nOpenFlags, pError))
	{
		delete pFile;
		pFile = NULL;
	}
	return pFile;
}

void CProject::ReleaseFile(CFile *pFile, BOOL bAbort)
{
	ASSERT_KINDOF(CFile, pFile);
	if (bAbort)
		pFile->Abort(); // will not throw an exception
	else
		pFile->Close();
	delete pFile;
}

BOOL CProject::Save(LPCTSTR path)
{
	return OnSaveProject(path);
}

BOOL CProject::Open(LPCTSTR path)
{
	return OnOpenProject(path);
}

BOOL CProject::SaveModified()
{
	if (!IsModified())
		return TRUE; // ok to continue

	// get name/title of document
	CString name;
	if (m_strPathName.IsEmpty())
	{
		// get name based on caption
		name = m_strTitle;
		if (name.IsEmpty())
			VERIFY(name.LoadString(AFX_IDS_UNTITLED));
	}
	else
	{
		// get name based on file title of path name
		name = m_strPathName;
		GetFileTitle(m_strPathName, name.GetBuffer(MAX_PATH), MAX_PATH);
		name.ReleaseBuffer();
	}

	CString prompt;
	AfxFormatString1(prompt, AFX_IDP_ASK_TO_SAVE, name);

	int button;

	if (!RunTimeHelper::IsVista())
	{
		button = AfxMessageBox(prompt, MB_YESNOCANCEL, AFX_IDP_ASK_TO_SAVE);
	}
	else
	{
		button = ShowSaveTaskDialog(prompt);
	}

	switch (button)
	{
		case IDCANCEL:
			return FALSE; // don't continue
			break;

		case IDYES:
			// If so, either Save or Update, as appropriate
			if (!DoFileSave())
				return FALSE; // don't continue
			break;

		case IDNO:
			// If not saving changes, revert the document
			return TRUE;
			break;

		default:
			ASSERT(FALSE);
			break;
	}
	return TRUE; // keep going
}

BOOL CProject::DoFileSave()
{
	DWORD dwAttrib = GetFileAttributes(m_strPathName);
	if (dwAttrib & FILE_ATTRIBUTE_READONLY)
	{
		// we do not have read-write access or the file does not (now) exist
		if (!DoSave(NULL))
		{
			TRACE0("Warning: File save with new name failed.\n");
			return FALSE;
		}
	}
	else
	{
		if (!DoSave(m_strPathName))
		{
			TRACE0("Warning: File save failed.\n");
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CProject::DoSave(LPCTSTR lpszPathName, BOOL bReplace)
// Save the document data to a file
// lpszPathName = path name where to save document file
// if lpszPathName is NULL then the user will be prompted (SaveAs)
// note: lpszPathName can be different than 'm_strPathName'
// if 'bReplace' is TRUE will change file name if successful (SaveAs)
// if 'bReplace' is FALSE will not change path name (SaveCopyAs)
{
	CString newName = lpszPathName;
	if (newName.IsEmpty())
	{
		CProjectTemplate* pTemplate = GetProjectTemplate();
		ASSERT(pTemplate != NULL);

		newName = m_strPathName;
		if (bReplace && newName.IsEmpty())
		{
			newName = m_strTitle;
			// check for dubious filename
			int iBad = newName.FindOneOf(_T(" #%;/\\"));
			if (iBad != -1)
				newName.ReleaseBuffer(iBad);

			// append the default suffix if there is one
			CString strExt;
			if (pTemplate->GetProjectString(strExt, CProjectTemplate::filterExt) &&
			        !strExt.IsEmpty())
			{
				ASSERT(strExt[0] == '.');
				newName += strExt;
			}
		}

		// application class not derived from CProjectSupportingWinApp!!!
		ASSERT(dynamic_cast<CProjectSupportingWinApp*>(AfxGetApp()));
		if (!dynamic_cast<CProjectSupportingWinApp*>(AfxGetApp())->DoPromptFileName(newName,
		        bReplace ? AFX_IDS_SAVEFILE : AFX_IDS_SAVEFILECOPY,
		        OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, FALSE, pTemplate))
			return FALSE; // don't even attempt to save
	}

	CWaitCursor wait;

	if (!OnSaveProject(newName))
	{
		if (lpszPathName == NULL)
		{
			// be sure to delete the file
			try
			{
				CFile::Remove(newName);
			}
			catch (CException *pE)
			{
				TRACE0("Warning: failed to delete file after failed SaveAs.\n");
				delete pE;
			}
		}
		return FALSE;
	}

	// reset the title and change the document name
	if (bReplace)
		SetPathName(newName);

	return TRUE; // success
}

void CProject::OnProjectClose()
{
	m_bIsClosing = true;

	if (!SaveModified())
		return;

	// shut it down
	OnClosing();

	// destroy the document
	GetProjectTemplate()->RemoveProject(this);

	UpdateFrameTitle();

	delete this;
}

void CProject::OnUpdateProjectClose(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}

void CProject::OnProjectSave()
{
	DoFileSave();
}

void CProject::OnUpdateProjectSave(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsModified());
}

bool CProject::IsClosing() const
{
	return m_bIsClosing;
}

void CProject::UpdateFrameTitle()
{
	CMainFrame* frame = static_cast<CMainFrame*>(AfxGetMainWnd());
	
	if (frame)
		frame->UpdateFrameTitle();
}
