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

#include "stdafx.h"
#include "TeXnicCenter.h"
#include "ProjectTemplate.h"
#include "ProjectSupportingWinApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// AFX declarations
//-------------------------------------------------------------------

BOOL AFXAPI AfxComparePath(LPCTSTR lpszPath1, LPCTSTR lpszPath2);
#define _countof(array) (sizeof(array)/sizeof(array[0]))
BOOL AFXAPI AfxFullPath(LPTSTR lpszPathOut, LPCTSTR lpszFileIn);
BOOL AFXAPI AfxResolveShortcut(CWnd* pWnd, LPCTSTR pszShortcutFile, LPTSTR pszPath, int cchPath);


//-------------------------------------------------------------------
// CProjectTemplate
//-------------------------------------------------------------------

IMPLEMENT_DYNAMIC(CProjectTemplate, CCmdTarget)

BEGIN_MESSAGE_MAP(CProjectTemplate, CCmdTarget)
	//{{AFX_MSG_MAP(CProjectTemplate)
	ON_COMMAND(ID_PROJECT_OPEN, OnProjectOpen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CProjectTemplate::CProjectTemplate(UINT nIDResource, CRuntimeClass *pProjectClass, int nIconIndex /*= -1*/)
{
	ASSERT_VALID_IDR(nIDResource);
	ASSERT(pProjectClass == NULL ||
		pProjectClass->IsDerivedFrom(RUNTIME_CLASS(CProject)));

	m_nIDResource = nIDResource;
	m_pProjectClass = pProjectClass;
	m_nIconIndex = nIconIndex;

	// add to pStaticList if constructed as static instead of on heap
	if (CProjectManager::bStaticInit)
	{
		m_bAutoDelete = FALSE;
		if (CProjectManager::pStaticList == NULL)
			CProjectManager::pStaticList = new CList<CProjectTemplate*, CProjectTemplate*>;
		if (CProjectManager::pStaticProjectManager == NULL)
			CProjectManager::pStaticProjectManager = new CProjectManager;
		CProjectManager::pStaticList->AddTail(this);
	}
	else
	{
		m_bAutoDelete = TRUE;   // usually allocated on the heap
		LoadTemplate();
	}
}


CProjectTemplate::~CProjectTemplate()
{}


void CProjectTemplate::LoadTemplate()
{
	if (m_strProjectStrings.IsEmpty() && !m_strProjectStrings.LoadString(m_nIDResource))
	{
		TRACE1("Warning: no project names in string for template #%d.\n", m_nIDResource);
	}
}


void CProjectTemplate::AddProject(CProject* pProject)
{
	ASSERT_VALID(pProject);
	ASSERT(pProject->m_pProjectTemplate == NULL);   // no template attached yet
	pProject->m_pProjectTemplate = this;
}


void CProjectTemplate::RemoveProject(CProject* pProject)
{
	ASSERT_VALID(pProject);
	ASSERT(pProject->m_pProjectTemplate == this);   // must be attached to us
	pProject->m_pProjectTemplate = NULL;
}


CProjectTemplate::Confidence CProjectTemplate::MatchProjectType(LPCTSTR lpszPathName, CProject*& rpProjectMatch)
{
	ASSERT(lpszPathName != NULL);
	rpProjectMatch = NULL;

	// go through all documents
	POSITION pos = GetFirstProjectPosition();
	while (pos != NULL)
	{
		CProject* pProject = GetNextProject(pos);
		if (AfxComparePath(pProject->GetPathName(), lpszPathName))
		{
			// already open
			rpProjectMatch = pProject;
			return yesAlreadyOpen;
		}
	}

	// see if it matches our default suffix
	CString strFilterExt;
	if (GetProjectString(strFilterExt, CProjectTemplate::filterExt) &&
	  !strFilterExt.IsEmpty())
	{
		// see if extension matches
		ASSERT(strFilterExt[0] == '.');
		LPCTSTR lpszDot = _tcsrchr(lpszPathName, '.');
		if (lpszDot != NULL && lstrcmpi(lpszDot, strFilterExt) == 0)
			return yesAttemptNative; // extension matches, looks like ours
	}

	// otherwise we will guess it may work
	return yesAttemptForeign;
}


CProject* CProjectTemplate::CreateNewProject()
{
	// default implementation constructs one from CRuntimeClass
	if (m_pProjectClass == NULL)
	{
		TRACE0("Error: you must override CProjectTemplate::CreateNewProject.\n");
		ASSERT(FALSE);
		return NULL;
	}

	CProject* pProject = (CProject*)m_pProjectClass->CreateObject();
	if (pProject == NULL)
	{
		TRACE1("Warning: Dynamic create of project type %hs failed.\n",
			m_pProjectClass->m_lpszClassName);
		return NULL;
	}
	ASSERT_KINDOF(CProject, pProject);
	AddProject(pProject);
	return pProject;
}


BOOL CProjectTemplate::SaveAllModified()
{
	POSITION pos = GetFirstProjectPosition();
	while (pos != NULL)
	{
		CProject* pProject = GetNextProject(pos);
		if (!pProject->SaveModified())
			return FALSE;
	}
	return TRUE;
}


void CProjectTemplate::CloseAllProjects(BOOL bEndSession)
{
	POSITION pos = GetFirstProjectPosition();
	while (pos != NULL)
	{
		CProject* pProject = GetNextProject(pos);
		pProject->OnCloseProject();
	}
}


void CProjectTemplate::OnProjectOpen() 
{
}


BOOL CProjectTemplate::GetProjectString(CString& rString, enum ProjectStringIndex index) const
{
	return AfxExtractSubString(rString, m_strProjectStrings, (int)index);
}


//-------------------------------------------------------------------
// CSingleProjectTemplate
//-------------------------------------------------------------------

IMPLEMENT_DYNAMIC(CSingleProjectTemplate, CProjectTemplate)

CSingleProjectTemplate::CSingleProjectTemplate(UINT nIDResource, CRuntimeClass* pProjectClass, int nIconIndex /*= -1*/)
:	CProjectTemplate(nIDResource, pProjectClass, nIconIndex)
{
	m_pOnlyProject = NULL;
}


CSingleProjectTemplate::~CSingleProjectTemplate()
{
#ifdef _DEBUG
	if (m_pOnlyProject != NULL)
		TRACE0("Warning: destroying CSingleProjectTemplate with live project.\n");
#endif
}


void CSingleProjectTemplate::AddProject(CProject *pProject)
{
	ASSERT(m_pOnlyProject == NULL);     // one at a time please
	ASSERT_VALID(pProject);

	CProjectTemplate::AddProject(pProject);
	m_pOnlyProject = pProject;
}


void CSingleProjectTemplate::RemoveProject(CProject *pProject)
{
	ASSERT(m_pOnlyProject == pProject);     // must be this one
	ASSERT_VALID(pProject);

	CProjectTemplate::RemoveProject(pProject);
	m_pOnlyProject = NULL;
}


POSITION CSingleProjectTemplate::GetFirstProjectPosition() const
{
	return (m_pOnlyProject == NULL) ? NULL : BEFORE_START_POSITION;
}


CProject* CSingleProjectTemplate::GetNextProject(POSITION& pos) const
{
	CProject* pProject = NULL;
	if (pos == BEFORE_START_POSITION)
	{
		// first time through, return a real document
		ASSERT(m_pOnlyProject != NULL);
		pProject = m_pOnlyProject;
	}
	pos = NULL;        // no more
	return pProject;
}


CProject* CSingleProjectTemplate::OpenProjectFile(LPCTSTR lpszPathName)
{
	CProject* pProject = NULL;
	BOOL bCreated = FALSE;      // => projected created
	BOOL bWasModified = FALSE;

	if (m_pOnlyProject != NULL)
	{
		// already have a document - reinit it
		pProject = m_pOnlyProject;
		if (!pProject->SaveModified())
			return NULL;        // leave the original one

		// close current project
		pProject->OnCloseProject();
		RemoveProject(pProject);
		delete pProject;
	}

	// create a new project
	pProject = CreateNewProject();
	bCreated = TRUE;

	if (pProject == NULL)
	{
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
		return NULL;
	}
	ASSERT(pProject == m_pOnlyProject);

	if (lpszPathName == NULL)
	{
		// create a new document
		SetDefaultTitle(pProject);

		// avoid creating temporary compound file when starting up invisible
		if (!pProject->OnNewProject())
		{
			// if project has been newly created, then destroy
			if (bCreated)
			{
				pProject->OnCloseProject();
				RemoveProject(pProject);
				delete pProject;
			}

			// user has been alerted to what failed in OnNewProject
			TRACE0("CProject::OnNewProject returned FALSE.\n");
			return NULL;
		}
	}
	else
	{
		CWaitCursor wait;

		// open an existing document
		bWasModified = pProject->IsModified();
		pProject->SetModifiedFlag(FALSE);  // not dirty for open

		if (!pProject->OnOpenProject(lpszPathName))
		{
			// user has been alerted to what failed in OnOpenProject
			TRACE0("CProject::OnOpenProject returned FALSE.\n");
			if (!pProject->IsModified())
			{
				// original document is untouched
				pProject->SetModifiedFlag(bWasModified);
			}
			else
			{
				// we corrupted the original document
				SetDefaultTitle(pProject);

				if (!pProject->OnNewProject())
				{
					TRACE0("Error: OnNewProject failed after trying to open a document - trying to continue.\n");
					// assume we can continue
				}
			}
			return NULL;        // open failed
		}
		pProject->SetPathName(lpszPathName);
	}

	return pProject;
}


void CSingleProjectTemplate::SetDefaultTitle(CProject* pProject)
{
	CString strProjectName;
	if (!GetProjectString(strProjectName, CProjectTemplate::docName) ||
		strProjectName.IsEmpty())
	{
		// use generic 'untitled'
		VERIFY(strProjectName.LoadString(AFX_IDS_UNTITLED));
	}
	pProject->SetTitle(strProjectName);
}


//-------------------------------------------------------------------
// CProjectManager
//-------------------------------------------------------------------

#define _countof(array) (sizeof(array)/sizeof(array[0]))

AFX_STATIC_DATA const TCHAR _afxShellOpenFmt[] = _T("%s\\shell\\open\\%s");
AFX_STATIC_DATA const TCHAR _afxShellPrintFmt[] = _T("%s\\shell\\print\\%s");
AFX_STATIC_DATA const TCHAR _afxShellPrintToFmt[] = _T("%s\\shell\\printto\\%s");
AFX_STATIC_DATA const TCHAR _afxDefaultIconFmt[] = _T("%s\\DefaultIcon");
AFX_STATIC_DATA const TCHAR _afxShellNewFmt[] = _T("%s\\ShellNew");

#define DEFAULT_ICON_INDEX 0

AFX_STATIC_DATA const TCHAR _afxIconIndexFmt[] = _T(",%d");
AFX_STATIC_DATA const TCHAR _afxCommand[] = _T("command");
AFX_STATIC_DATA const TCHAR _afxOpenArg[] = _T(" \"%1\"");
AFX_STATIC_DATA const TCHAR _afxPrintArg[] = _T(" /p \"%1\"");
AFX_STATIC_DATA const TCHAR _afxPrintToArg[] = _T(" /pt \"%1\" \"%2\" \"%3\" \"%4\"");
AFX_STATIC_DATA const TCHAR _afxDDEArg[] = _T(" /dde");

AFX_STATIC_DATA const TCHAR _afxDDEExec[] = _T("ddeexec");
AFX_STATIC_DATA const TCHAR _afxDDEOpen[] = _T("[open(\"%1\")]");
AFX_STATIC_DATA const TCHAR _afxDDEPrint[] = _T("[print(\"%1\")]");
AFX_STATIC_DATA const TCHAR _afxDDEPrintTo[] = _T("[printto(\"%1\",\"%2\",\"%3\",\"%4\")]");

AFX_STATIC_DATA const TCHAR _afxShellNewValueName[] = _T("NullFile");
AFX_STATIC_DATA const TCHAR _afxShellNewValue[] = _T("");

// recursively remove a registry key if and only if it has no subkeys
BOOL AFXAPI _AfxDeleteRegKey(LPCTSTR lpszKey)
{
	// copy the string
	LPTSTR lpszKeyCopy = _tcsdup(lpszKey);
	LPTSTR lpszLast = lpszKeyCopy + lstrlen(lpszKeyCopy);

	// work until the end of the string
	while (lpszLast != NULL)
	{
		*lpszLast = '\0';
		lpszLast = _tcsdec(lpszKeyCopy, lpszLast);

		// try to open that key
		HKEY hKey;
		if (::RegOpenKey(HKEY_CLASSES_ROOT, lpszKeyCopy, &hKey) != ERROR_SUCCESS)
			break;

		// enumerate the keys underneath
		TCHAR szScrap[_MAX_PATH+1];
		DWORD dwLen = _countof(szScrap);
		BOOL bItExists = FALSE;

		if (::RegEnumKey(hKey, 0, szScrap, dwLen) == ERROR_SUCCESS)
			bItExists = TRUE;
		::RegCloseKey(hKey);

		// found one?  quit looping
		if (bItExists)
			break;

		// otherwise, delete and find the previous backwhack
		::RegDeleteKey(HKEY_CLASSES_ROOT, lpszKeyCopy);
		lpszLast = _tcsrchr(lpszKeyCopy, '\\');
	}

	// release the string and return
	free(lpszKeyCopy);
	return TRUE;
}

AFX_STATIC BOOL AFXAPI
_AfxSetRegKey(LPCTSTR lpszKey, LPCTSTR lpszValue, LPCTSTR lpszValueName = NULL)
{
	if (lpszValueName == NULL)
	{
		if (::RegSetValue(HKEY_CLASSES_ROOT, lpszKey, REG_SZ,
			  lpszValue, lstrlen(lpszValue) * sizeof(TCHAR)) != ERROR_SUCCESS)
		{
			TRACE1("Warning: registration database update failed for key '%s'.\n",
				lpszKey);
			return FALSE;
		}
		return TRUE;
	}
	else
	{
		HKEY hKey;

		if(::RegCreateKey(HKEY_CLASSES_ROOT, lpszKey, &hKey) == ERROR_SUCCESS)
		{
			LONG lResult = ::RegSetValueEx(hKey, lpszValueName, 0, REG_SZ,
				(CONST BYTE*)lpszValue, (lstrlen(lpszValue) + 1) * sizeof(TCHAR));

			if(::RegCloseKey(hKey) == ERROR_SUCCESS && lResult == ERROR_SUCCESS)
				return TRUE;
		}
		TRACE1("Warning: registration database update failed for key '%s'.\n", lpszKey);
		return FALSE;
	}
}


AFX_STATIC void AFXAPI _AfxAppendFilterSuffix(CString& filter, OPENFILENAME& ofn,
	CProjectTemplate* pTemplate, CString* pstrDefaultExt)
{
	ASSERT_VALID(pTemplate);
	ASSERT_KINDOF(CProjectTemplate, pTemplate);

	CString strFilterExt, strFilterName;
	if (pTemplate->GetProjectString(strFilterExt, CProjectTemplate::filterExt) &&
	 !strFilterExt.IsEmpty() &&
	 pTemplate->GetProjectString(strFilterName, CProjectTemplate::filterName) &&
	 !strFilterName.IsEmpty())
	{
		// a file based document template - add to filter list
		ASSERT(strFilterExt[0] == '.');
		if (pstrDefaultExt != NULL)
		{
			// set the default extension
			*pstrDefaultExt = ((LPCTSTR)strFilterExt) + 1;  // skip the '.'
			ofn.lpstrDefExt = (LPTSTR)(LPCTSTR)(*pstrDefaultExt);
			ofn.nFilterIndex = ofn.nMaxCustFilter + 1;  // 1 based number
		}

		// add to filter
		filter += strFilterName;
		ASSERT(!filter.IsEmpty());  // must have a file type name
		filter += (TCHAR)'\0';  // next string please
		filter += (TCHAR)'*';
		filter += strFilterExt;
		filter += (TCHAR)'\0';  // next string please
		ofn.nMaxCustFilter++;
	}
}


IMPLEMENT_DYNAMIC(CProjectManager, CObject)


CList<CProjectTemplate*, CProjectTemplate*>	*CProjectManager::pStaticList = NULL;
CProjectManager	*CProjectManager::pStaticProjectManager = NULL;
BOOL CProjectManager::bStaticInit = TRUE;


CProjectManager::CProjectManager()
{
}


CProjectManager::~CProjectManager()
{
	// for cleanup - delete all project templates
	POSITION pos = m_templateList.GetHeadPosition();
	while (pos != NULL)
	{
		POSITION posTemplate = pos;
		CProjectTemplate* pTemplate = (CProjectTemplate*)m_templateList.GetNext(pos);
		if (pTemplate->m_bAutoDelete)
		{
			m_templateList.RemoveAt(posTemplate);
			delete pTemplate;
		}
	}
}


void CProjectManager::AddProjectTemplate(CProjectTemplate* pTemplate)
{
	if (pTemplate == NULL)
	{
		if (pStaticList != NULL)
		{
			POSITION pos = pStaticList->GetHeadPosition();
			while (pos != NULL)
			{
				CProjectTemplate* pTemplate =
					(CProjectTemplate*)pStaticList->GetNext(pos);
				AddProjectTemplate(pTemplate);
			}
			delete pStaticList;
			pStaticList = NULL;
		}
		bStaticInit = FALSE;
	}
	else
	{
		ASSERT_VALID(pTemplate);
		ASSERT(m_templateList.Find(pTemplate, NULL) == NULL);// must not be in list
		pTemplate->LoadTemplate();
		m_templateList.AddTail(pTemplate);
	}
}


POSITION CProjectManager::GetFirstProjectTemplatePosition() const
{
	return m_templateList.GetHeadPosition();
}


CProjectTemplate* CProjectManager::GetNextProjectTemplate(POSITION& pos) const
{
	return (CProjectTemplate*)m_templateList.GetNext(pos);
}


void CProjectManager::RegisterShellFileTypes(BOOL bCompat)
{
	ASSERT(!m_templateList.IsEmpty());  // must have some doc templates

	CString strPathName, strTemp;

	AfxGetModuleShortFileName(AfxGetInstanceHandle(), strPathName);

	POSITION pos = m_templateList.GetHeadPosition();
	for (int nTemplateIndex = 1; pos != NULL; nTemplateIndex++)
	{
		CProjectTemplate* pTemplate = (CProjectTemplate*)m_templateList.GetNext(pos);

		CString strOpenCommandLine = strPathName;
		//CString strPrintCommandLine = strPathName;
		//CString strPrintToCommandLine = strPathName;
		CString strDefaultIconCommandLine = strPathName;

		if (bCompat)
		{
			CString strIconIndex;
			HICON		hIcon = (pTemplate->m_nIconIndex == -1)
				? NULL
				: ::ExtractIcon(AfxGetInstanceHandle(), strPathName, pTemplate->m_nIconIndex);

			if (hIcon != NULL)
			{
				strIconIndex.Format(_afxIconIndexFmt, pTemplate->m_nIconIndex);
				DestroyIcon(hIcon);
			}
			else
			{
				strIconIndex.Format(_afxIconIndexFmt, DEFAULT_ICON_INDEX);
			}
			strDefaultIconCommandLine += strIconIndex;
		}

		CString strFilterExt, strFileTypeId, strFileTypeName;
		if (pTemplate->GetProjectString(strFileTypeId,
		   CProjectTemplate::regFileTypeId) && !strFileTypeId.IsEmpty())
		{
			// enough info to register it
			if (!pTemplate->GetProjectString(strFileTypeName,
			   CProjectTemplate::regFileTypeName))
				strFileTypeName = strFileTypeId;    // use id name

			ASSERT(strFileTypeId.Find(' ') == -1);  // no spaces allowed

			// first register the type ID of our server
			if (!_AfxSetRegKey(strFileTypeId, strFileTypeName))
				continue;       // just skip it

			if (bCompat)
			{
				// path\DefaultIcon = path,1
				strTemp.Format(_afxDefaultIconFmt, (LPCTSTR)strFileTypeId);
				if (!_AfxSetRegKey(strTemp, strDefaultIconCommandLine))
					continue;       // just skip it
			}

			// If MDI Application
			if (!pTemplate->GetProjectString(strTemp, CProjectTemplate::windowTitle) ||
				strTemp.IsEmpty())
			{
				// path\shell\open\ddeexec = [open("%1")]
				strTemp.Format(_afxShellOpenFmt, (LPCTSTR)strFileTypeId,
					(LPCTSTR)_afxDDEExec);
				if (!_AfxSetRegKey(strTemp, _afxDDEOpen))
					continue;       // just skip it

				if (bCompat)
				{
					/*
					// path\shell\print\ddeexec = [print("%1")]
					strTemp.Format(_afxShellPrintFmt, (LPCTSTR)strFileTypeId,
						(LPCTSTR)_afxDDEExec);
					if (!_AfxSetRegKey(strTemp, _afxDDEPrint))
						continue;       // just skip it

					// path\shell\printto\ddeexec = [printto("%1","%2","%3","%4")]
					strTemp.Format(_afxShellPrintToFmt, (LPCTSTR)strFileTypeId,
						(LPCTSTR)_afxDDEExec);
					if (!_AfxSetRegKey(strTemp, _afxDDEPrintTo))
						continue;       // just skip it
					*/
					
					// path\shell\open\command = path /dde
					// path\shell\print\command = path /dde
					// path\shell\printto\command = path /dde
					strOpenCommandLine += _afxDDEArg;
					//strPrintCommandLine += _afxDDEArg;
					//strPrintToCommandLine += _afxDDEArg;
				}
				else
				{
					strOpenCommandLine += _afxOpenArg;
				}
			}
			else
			{
				// path\shell\open\command = path filename
				// path\shell\print\command = path /p filename
				// path\shell\printto\command = path /pt filename printer driver port
				strOpenCommandLine += _afxOpenArg;
				/*
				if (bCompat)
				{
					strPrintCommandLine += _afxPrintArg;
					strPrintToCommandLine += _afxPrintToArg;
				}
				*/
			}

			// path\shell\open\command = path filename
			strTemp.Format(_afxShellOpenFmt, (LPCTSTR)strFileTypeId,
				(LPCTSTR)_afxCommand);
			if (!_AfxSetRegKey(strTemp, strOpenCommandLine))
				continue;       // just skip it

			/*
			if (bCompat)
			{
				// path\shell\print\command = path /p filename
				strTemp.Format(_afxShellPrintFmt, (LPCTSTR)strFileTypeId,
					(LPCTSTR)_afxCommand);
				if (!_AfxSetRegKey(strTemp, strPrintCommandLine))
					continue;       // just skip it

				// path\shell\printto\command = path /pt filename printer driver port
				strTemp.Format(_afxShellPrintToFmt, (LPCTSTR)strFileTypeId,
					(LPCTSTR)_afxCommand);
				if (!_AfxSetRegKey(strTemp, strPrintToCommandLine))
					continue;       // just skip it
			}
			*/

			pTemplate->GetProjectString(strFilterExt, CProjectTemplate::filterExt);
			if (!strFilterExt.IsEmpty())
			{
				ASSERT(strFilterExt[0] == '.');

				LONG lSize = _MAX_PATH * 2;
				LONG lResult = ::RegQueryValue(HKEY_CLASSES_ROOT, strFilterExt,
					strTemp.GetBuffer(lSize), &lSize);
				strTemp.ReleaseBuffer();

				if (lResult != ERROR_SUCCESS || strTemp.IsEmpty() ||
					strTemp == strFileTypeId)
				{
					// no association for that suffix
					if (!_AfxSetRegKey(strFilterExt, strFileTypeId))
						continue;

					if (bCompat)
					{
						strTemp.Format(_afxShellNewFmt, (LPCTSTR)strFilterExt);
						(void)_AfxSetRegKey(strTemp, _afxShellNewValue, _afxShellNewValueName);
					}
				}
			}
		}
	}
}


void CProjectManager::UnregisterShellFileTypes()
{
	ASSERT(!m_templateList.IsEmpty());  // must have some doc templates

	CString strPathName, strTemp;

	AfxGetModuleShortFileName(AfxGetInstanceHandle(), strPathName);

	POSITION pos = m_templateList.GetHeadPosition();
	for (int nTemplateIndex = 1; pos != NULL; nTemplateIndex++)
	{
		CProjectTemplate* pTemplate = (CProjectTemplate*)m_templateList.GetNext(pos);

		CString strFilterExt, strFileTypeId, strFileTypeName;
		if (pTemplate->GetProjectString(strFileTypeId,
		   CProjectTemplate::regFileTypeId) && !strFileTypeId.IsEmpty())
		{
			// enough info to register it
			if (!pTemplate->GetProjectString(strFileTypeName,
			   CProjectTemplate::regFileTypeName))
				strFileTypeName = strFileTypeId;    // use id name

			ASSERT(strFileTypeId.Find(' ') == -1);  // no spaces allowed

			strTemp.Format(_afxDefaultIconFmt, (LPCTSTR)strFileTypeId);
			_AfxDeleteRegKey(strTemp);

			// If MDI Application
			if (!pTemplate->GetProjectString(strTemp, CProjectTemplate::windowTitle) ||
				strTemp.IsEmpty())
			{
				// path\shell\open\ddeexec = [open("%1")]
				strTemp.Format(_afxShellOpenFmt, (LPCTSTR)strFileTypeId,
					(LPCTSTR)_afxDDEExec);
				_AfxDeleteRegKey(strTemp);

				/*
				// path\shell\print\ddeexec = [print("%1")]
				strTemp.Format(_afxShellPrintFmt, (LPCTSTR)strFileTypeId,
					(LPCTSTR)_afxDDEExec);
				_AfxDeleteRegKey(strTemp);

				// path\shell\printto\ddeexec = [printto("%1","%2","%3","%4")]
				strTemp.Format(_afxShellPrintToFmt, (LPCTSTR)strFileTypeId,
					(LPCTSTR)_afxDDEExec);
				_AfxDeleteRegKey(strTemp);
				*/
			}

			// path\shell\open\command = path filename
			strTemp.Format(_afxShellOpenFmt, (LPCTSTR)strFileTypeId,
				(LPCTSTR)_afxCommand);
			_AfxDeleteRegKey(strTemp);

			/*
			// path\shell\print\command = path /p filename
			strTemp.Format(_afxShellPrintFmt, (LPCTSTR)strFileTypeId,
				(LPCTSTR)_afxCommand);
			_AfxDeleteRegKey(strTemp);

			// path\shell\printto\command = path /pt filename printer driver port
			strTemp.Format(_afxShellPrintToFmt, (LPCTSTR)strFileTypeId,
				(LPCTSTR)_afxCommand);
			_AfxDeleteRegKey(strTemp);
			*/

			pTemplate->GetProjectString(strFilterExt, CProjectTemplate::filterExt);
			if (!strFilterExt.IsEmpty())
			{
				ASSERT(strFilterExt[0] == '.');

				LONG lSize = _MAX_PATH * 2;
				LONG lResult = ::RegQueryValue(HKEY_CLASSES_ROOT, strFilterExt,
					strTemp.GetBuffer(lSize), &lSize);
				strTemp.ReleaseBuffer();

				if (lResult != ERROR_SUCCESS || strTemp.IsEmpty() ||
					strTemp == strFileTypeId)
				{
					strTemp.Format(_afxShellNewFmt, (LPCTSTR)strFilterExt);
					_AfxDeleteRegKey(strTemp);

					// no association for that suffix
					_AfxDeleteRegKey(strFilterExt);
				}
			}
		}
	}
}


CProject* CProjectManager::OpenProjectFile(LPCTSTR lpszFileName)
{
	// find the highest confidence
	POSITION pos = m_templateList.GetHeadPosition();
	CProjectTemplate::Confidence bestMatch = CProjectTemplate::noAttempt;
	CProjectTemplate* pBestTemplate = NULL;
	CProject* pOpenProject = NULL;

	TCHAR szPath[_MAX_PATH];
	ASSERT(lstrlen(lpszFileName) < _countof(szPath));
	TCHAR szTemp[_MAX_PATH];
	if (lpszFileName[0] == '\"')
		++lpszFileName;
	lstrcpyn(szTemp, lpszFileName, _MAX_PATH);
	LPTSTR lpszLast = _tcsrchr(szTemp, '\"');
	if (lpszLast != NULL)
		*lpszLast = 0;
	AfxFullPath(szPath, szTemp);
	TCHAR szLinkName[_MAX_PATH];
	if (AfxResolveShortcut(AfxGetMainWnd(), szPath, szLinkName, _MAX_PATH))
		lstrcpy(szPath, szLinkName);

	while (pos != NULL)
	{
		CProjectTemplate* pTemplate = (CProjectTemplate*)m_templateList.GetNext(pos);
		ASSERT_KINDOF(CProjectTemplate, pTemplate);

		CProjectTemplate::Confidence match;
		ASSERT(pOpenProject == NULL);
		match = pTemplate->MatchProjectType(szPath, pOpenProject);
		if (match > bestMatch)
		{
			bestMatch = match;
			pBestTemplate = pTemplate;
		}
		if (match == CProjectTemplate::yesAlreadyOpen)
			break;      // stop here
	}

	if (pBestTemplate == NULL)
	{
		AfxMessageBox(AFX_IDP_FAILED_TO_OPEN_DOC);
		return NULL;
	}

	return pBestTemplate->OpenProjectFile(szPath);
}


BOOL CProjectManager::SaveAllModified()
{
	POSITION pos = m_templateList.GetHeadPosition();
	while (pos != NULL)
	{
		CProjectTemplate* pTemplate = (CProjectTemplate*)m_templateList.GetNext(pos);
		ASSERT_KINDOF(CProjectTemplate, pTemplate);
		if (!pTemplate->SaveAllModified())
			return FALSE;
	}
	return TRUE;
}


void CProjectManager::CloseAllProjects(BOOL bEndSession)
{
	POSITION pos = m_templateList.GetHeadPosition();
	while (pos != NULL)
	{
		CProjectTemplate* pTemplate = (CProjectTemplate*)m_templateList.GetNext(pos);
		ASSERT_KINDOF(CProjectTemplate, pTemplate);
		pTemplate->CloseAllProjects(bEndSession);
	}
}


int CProjectManager::GetOpenProjectCount()
{
	int nOpen = 0;
	POSITION pos = m_templateList.GetHeadPosition();
	while (pos != NULL)
	{
		CProjectTemplate* pTemplate = (CProjectTemplate*)m_templateList.GetNext(pos);
		POSITION pos2 = pTemplate->GetFirstProjectPosition();
		while (pos2)
		{
			if (pTemplate->GetNextProject(pos2) != NULL)
				nOpen++;
		}
	}
	return nOpen;
}


BOOL CProjectManager::DoPromptFileName(CString& fileName, UINT nIDSTitle,
																			 DWORD lFlags, BOOL bOpenFileDialog, 
																			 CProjectTemplate* pTemplate)
{
	CFileDialog dlgFile(bOpenFileDialog);

	CString title;
	VERIFY(title.LoadString(nIDSTitle));

	dlgFile.m_ofn.Flags |= lFlags;

	CString strFilter;
	CString strDefault;
	if (pTemplate != NULL)
	{
		ASSERT_VALID(pTemplate);
		_AfxAppendFilterSuffix(strFilter, dlgFile.m_ofn, pTemplate, &strDefault);
	}
	else
	{
		// do for all doc template
		POSITION pos = m_templateList.GetHeadPosition();
		BOOL bFirst = TRUE;
		while (pos != NULL)
		{
			CProjectTemplate* pTemplate = (CProjectTemplate*)m_templateList.GetNext(pos);
			_AfxAppendFilterSuffix(strFilter, dlgFile.m_ofn, pTemplate,
				bFirst ? &strDefault : NULL);
			bFirst = FALSE;
		}
	}

	// append the "*.*" all files filter
	CString allFilter;
	VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
	strFilter += allFilter;
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.*");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

	int nResult = dlgFile.DoModal();
	fileName.ReleaseBuffer();
	return nResult == IDOK;
}


BOOL CProjectManager::OnDDECommand(LPTSTR lpszCommand)
{
	CString strCommand = lpszCommand;
	CProject* pProject = NULL;

	// open format is "[open("%s")]" - no whitespace allowed, one per line
	// print format is "[print("%s")]" - no whitespace allowed, one per line
	// print to format is "[printto("%s","%s","%s","%s")]" - no whitespace allowed, one per line
	CCommandLineInfo cmdInfo;
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileDDE;

	if (strCommand.Left(7) == _T("[open(\""))
	{
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileOpen;
		strCommand = strCommand.Right(strCommand.GetLength() - 7);
	}
	/*
	else if (strCommand.Left(8) == _T("[print(\""))
	{
		cmdInfo.m_nShellCommand = CCommandLineInfo::FilePrint;
		strCommand = strCommand.Right(strCommand.GetLength() - 8);
	}
	else if (strCommand.Left(10) == _T("[printto(\""))
	{
		cmdInfo.m_nShellCommand = CCommandLineInfo::FilePrintTo;\
		strCommand = strCommand.Right(strCommand.GetLength() - 10);
	}
	*/
	else
		return FALSE; // not a command we handle

	int i = strCommand.Find('"');
	if (i == -1)
		return FALSE; // illegally terminated

	cmdInfo.m_strFileName = strCommand.Left(i);
	strCommand = strCommand.Right(strCommand.GetLength() - i);

	CCommandLineInfo* pOldInfo = NULL;
	BOOL bRetVal = FALSE;

	// If we were started up for DDE retrieve the Show state
	if (AfxGetApp()->m_pCmdInfo != NULL)
	{
		AfxGetApp()->m_nCmdShow = (int)AfxGetApp()->m_pCmdInfo;
		AfxGetApp()->m_pCmdInfo = &cmdInfo;
	}
	else
		pOldInfo = AfxGetApp()->m_pCmdInfo;

	POSITION	pos = m_templateList.GetHeadPosition();
	while (pos)
	{
		CProjectTemplate	*pTemplate = (CProjectTemplate*)m_templateList.GetNext(pos);
		CString	strTemplateExtension;
		pTemplate->GetProjectString(strTemplateExtension, CProjectTemplate::filterExt);

		if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileOpen && cmdInfo.m_strFileName.Right(strTemplateExtension.GetLength()).CompareNoCase(strTemplateExtension) == 0)
		{
			// show the application window
			CWnd* pMainWnd = AfxGetApp()->m_pMainWnd;
			int nCmdShow = AfxGetApp()->m_nCmdShow;
			if (nCmdShow == -1 || nCmdShow == SW_SHOWNORMAL)
			{
				if (pMainWnd->IsIconic())
					nCmdShow = SW_RESTORE;
				else
					nCmdShow = SW_SHOW;
			}
			pMainWnd->ShowWindow(nCmdShow);
			if (nCmdShow != SW_MINIMIZE)
				pMainWnd->SetForegroundWindow();

			// then open the document
			ASSERT(dynamic_cast<CProjectSupportingWinApp*>(AfxGetApp()));
			dynamic_cast<CProjectSupportingWinApp*>(AfxGetApp())->OpenProjectFile(cmdInfo.m_strFileName);
			bRetVal = TRUE;

			// user is now "in control" of the application
			if (!AfxOleGetUserCtrl())
				AfxOleSetUserCtrl(TRUE);

			// next time, show the window as default
			AfxGetApp()->m_nCmdShow = -1;
			break;
		}
	}

	AfxGetApp()->m_pCmdInfo = pOldInfo;
	return bRetVal;
}


void CProjectManager::OnProjectNew()
{
	if (m_templateList.IsEmpty())
	{
		TRACE0("Error: no document templates registered with CWinApp.\n");
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
		return;
	}

	CProjectTemplate* pTemplate = (CProjectTemplate*)m_templateList.GetHead();
	/* //TODO
	if (m_templateList.GetCount() > 1)
	{
		// more than one document template to choose from
		// bring up dialog prompting user
		CNewTypeDlg dlg(&m_templateList);
		int nID = dlg.DoModal();
		if (nID == IDOK)
			pTemplate = dlg.m_pSelectedTemplate;
		else
			return;     // none - cancel operation
	}
	*/

	ASSERT(pTemplate != NULL);
	ASSERT_KINDOF(CProjectTemplate, pTemplate);

	pTemplate->OpenProjectFile(NULL);
		// if returns NULL, the user has already been alerted
}


void CProjectManager::OnProjectOpen()
{
	// prompt the user (with all document templates)
	CString newName;
	if (!DoPromptFileName(newName, AFX_IDS_OPENFILE,
	  OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, TRUE, NULL))
		return; // open cancelled

	ASSERT(dynamic_cast<CProjectSupportingWinApp*>(AfxGetApp()));
	dynamic_cast<CProjectSupportingWinApp*>(AfxGetApp())->OpenProjectFile(newName);
		// if returns NULL, the user has already been alerted
}


int CProjectManager::GetProjectCount()
{
	// count all documents
	int nCount = 0;
	POSITION pos = m_templateList.GetHeadPosition();
	while (pos != NULL)
	{
		CProjectTemplate* pTemplate = (CProjectTemplate*)m_templateList.GetNext(pos);
		POSITION pos2 = pTemplate->GetFirstProjectPosition();
		while (pos2 != NULL)
		{
			pTemplate->GetNextProject(pos2);
			++nCount;
		}
	}
	return nCount;
}
