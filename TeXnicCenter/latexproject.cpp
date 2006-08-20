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

#include "stdafx.h"
#include "TeXnicCenter.h"
#include "MainFrm.h"
#include "LatexProject.h"
#include "Configuration.h"
#include "global.h"
#include "ProjectPropertyDialog.h"
#include "Process.h"
#include "TextSourceFile.h"
#include "ItemPropertyDialog.h"
#include "ChildFrm.h"
#include "ProjectNewDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// standard beep
#define BEEP						MessageBeep( MB_OK )

//-------------------------------------------------------------------
// class CLatexProject
//-------------------------------------------------------------------

IMPLEMENT_DYNCREATE(CLatexProject, CProject)


BEGIN_INTERFACE_MAP(CLatexProject, CProject)
	INTERFACE_PART(CLatexProject, Interfaces::IID_IProject, Project)
END_INTERFACE_MAP()


CLatexProject::CLatexProject()
: 	m_bCanParse( TRUE ),
	m_bCanRunLatex( TRUE ),
	m_pStructureParser( NULL ),
	m_nCurrentStructureItem( -1 ),
	m_pwndStructureView( NULL ),
	m_pwndEnvironmentView( NULL ),
	m_pwndFileView(NULL),
	m_pwndBibView(NULL),
	m_bUseBibTex(FALSE),
	m_bUseMakeIndex(FALSE),
	m_nInitialNavigatorTab(0),
	m_strProjectLanguage(""),
	m_strProjectDialect("")
{
	// initialization
	// Initialize the control bars and main frame pointer members
	m_pwndMainFrame     = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(m_pwndMainFrame);
	m_pwndWorkspaceBar = m_pwndMainFrame->GetWorkspaceBar();
	ASSERT(m_pwndWorkspaceBar);
	m_pwndOutputBar = m_pwndMainFrame->GetOutputBar();
	ASSERT(m_pwndOutputBar);

	m_pStructureParser = new CStructureParser( this, m_pwndOutputBar->GetOutputDoc() /*, this */);
	ASSERT( m_pStructureParser );
	if( !m_pStructureParser )
		AfxThrowMemoryException();
}


CLatexProject::~CLatexProject()
{
	if( m_pStructureParser )
		delete m_pStructureParser;

	// check for undeleted views and clean up here
	if (m_pwndStructureView)
		delete m_pwndStructureView;
	if (m_pwndEnvironmentView)
		delete m_pwndEnvironmentView;
	if (m_pwndFileView)
		delete m_pwndFileView;
	if (m_pwndBibView)
		delete m_pwndBibView;
}


BOOL CLatexProject::OnNewProject()
{
	if (!CProject::OnNewProject())
		return FALSE;

	//Add views to the docking bars in the frame wnd
	m_nInitialNavigatorTab = 0;
	CreateProjectViews();

	CProjectNewDialog	dlg;

	dlg.AddTemplateFilter(_T("*.tex"), RUNTIME_CLASS(CFileBasedProjectTemplateItem));
	dlg.AddTemplateFilter(_T("*.dll"), RUNTIME_CLASS(CWizardBasedProjectTemplateItem));
	for (int i = 0; i < g_configuration.m_astrProjectTemplatePaths.GetSize(); i++)
		dlg.AddSearchDir(g_configuration.m_astrProjectTemplatePaths[i]);

	if (dlg.DoModal() == IDCANCEL)
		return FALSE;

	//Save and add to LRU
	DoFileSave();
	theApp.m_recentProjectList.Add(GetPathName());

	//Trigger analysis - parse project
	AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_PROJECT_PARSE);

	return TRUE;
}

BOOL CLatexProject::OnNewProjectFromDoc(LPCTSTR lpszDocPathName)
{
	if (!CProject::OnNewProjectFromDoc(lpszDocPathName))
		return false;

	if (!CPathTool::Exists(lpszDocPathName))
	{
		AfxMessageBox(STE_PROJECT_MAINFILENOTFOUND, MB_ICONSTOP | MB_OK);
		return false;
	}

	//Set the path of the project file
	CString ProjectFileName = GetProjectFileNameFromDoc(lpszDocPathName);
	if (ProjectFileName.IsEmpty()) return false;

	//Does it already exist?
	if (CheckExistingProjectFile(ProjectFileName))
	{
		// save modified documents
		if (!theApp.SaveAllModified()) return false;

		// close latex-documents
		// skipping annoying question in this version --
		// just closing the documents.
		// lets see, what the users will say ...
		theApp.GetLatexDocTemplate()->CloseAllDocuments(theApp.m_bEndSession);

		//Open the project
		OnOpenProject(ProjectFileName);
		//... and (at least) the main file
		OnProjectOpenMainfile();

		return true;
	}

	//Set it
	SetPathName(ProjectFileName);

	//Set the main latex file
	SetMainPath(lpszDocPathName);

	//Open the properties dialog for the user to change some values.
	OnProjectProperties();

	//Save and add to LRU
	//DoFileSave(); ==> This brings up the Save-Dialog, if tcp is not there - not wanted
	OnSaveProject(GetPathName());
	theApp.m_recentProjectList.Add(GetPathName());

	//Add views to the docking bars in the frame wnd
	m_nInitialNavigatorTab = 0;
	CreateProjectViews();

	//Open the main file (just to be sure)
	OnProjectOpenMainfile();

	//Trigger analysis - parse project
	AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_PROJECT_PARSE);

	return true;
}

CString CLatexProject::GetProjectFileNameFromDoc(LPCTSTR lpszDocPathName)
{
	CString ProjectFileName = (CPathTool::GetFileExtension(lpszDocPathName) == _T("tex"))
														?
														CPathTool::GetBase(lpszDocPathName) + _T(".tcp")
														:
														"";
	return ProjectFileName;
};

bool CLatexProject::CheckExistingProjectFile(LPCTSTR lpszPathName)
{
	if (!CPathTool::Exists(lpszPathName)) return false;

	//Ask the user what to do
	CString strMsg;
	strMsg.Format(STE_PROJECT_EXISTS, lpszPathName);
	return (AfxMessageBox(strMsg, MB_ICONQUESTION | MB_YESNO) == IDYES);
};

#define	TC_PROJECT_SESSION_EXT	_T("tps")

BOOL CLatexProject::OnOpenProject(LPCTSTR lpszPathName) 
{
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// load project information
	CIniFile file(lpszPathName);
	if (!file.ReadFile())
		return FALSE;

	//Check if ini file contains any useful data.
	if (file.GetNumKeys() < 2)
	{
		//tcp-file not valid: inform the user about this error.
		CString	strMsg;
		strMsg.Format(STE_FILE_INUSE, 
			AfxLoadString(IDS_OPEN),
			lpszPathName,
			AfxLoadString(STE_TCP_INVALID));

		AfxMessageBox(strMsg, MB_ICONEXCLAMATION|MB_OK);

		//and return without loading
		return false;
	}

	//Set the working dir of the project
	SetProjectDir(CPathTool::GetDirectory(lpszPathName));
	//Modify the LastOpenedFolder-Value to be the working dir
	AfxSetLastDirectory(CPathTool::GetDirectory(lpszPathName));

	//Load the Project Information
	Serialize(file, FALSE);

	SetModifiedFlag(FALSE);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// load project session
	CIniFile session(GetSessionPathName(lpszPathName));
	if (session.ReadFile())
		SerializeSession(session, FALSE);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// add views to the docking bars in the frame wnd
	CreateProjectViews();

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Activate Editor window
	if (m_pwndMainFrame)
	{
		m_pwndMainFrame->SendMessage(WM_COMMAND, ID_WINDOW_EDITOR);
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// parse project
	if (m_pwndMainFrame)
	{
		m_pwndMainFrame->PostMessage( WM_COMMAND, ID_PROJECT_PARSE );
	}

	return TRUE;
}


BOOL CLatexProject::OnSaveProject(LPCTSTR lpszPathName) 
{
	//return CProject::OnSaveDocument(lpszPathName);
	CIniFile	file(lpszPathName);
	file.Reset();
	if (!Serialize(file, TRUE))
		return FALSE;

	file.WriteFile();
	SetModifiedFlag(FALSE);
	return TRUE;
}


BOOL CLatexProject::SaveModified()
{
	if (!CProject::SaveModified())
		return FALSE;

	if (!theApp.GetLatexDocTemplate()->SaveAllModified())
		return FALSE;

	return TRUE;
}


void CLatexProject::OnCloseProject() 
{	
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// store project session
	CString SessionPathName = GetSessionPathName();
	if (!SessionPathName.IsEmpty())
	{
		CIniFile session(SessionPathName);
		session.Reset();
		SerializeSession(session, TRUE);
		session.WriteFile();
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// close latex-documents?

	// skipping annoying question in this version --
	// just closing the documents.
	// lets see, what the users will say ...
	theApp.GetLatexDocTemplate()->CloseAllDocuments(theApp.m_bEndSession);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// remember last project
	if (theApp.m_bEndSession)
		g_configuration.m_strLastProject = GetPathName();
	else
		g_configuration.m_strLastProject.Empty();

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Clean up

	// add views to the docking bars in the frame wnd
	CMainFrame	*pwndMainFrame = (CMainFrame*)AfxGetMainWnd();

	if (!pwndMainFrame)
		return;

	// cancel structure parsing
	if (m_pStructureParser)
	{
		m_pStructureParser->CancelParsing();
		WaitForSingleObject( m_pStructureParser->m_evtParsingDone, INFINITE );
	}

	// delete views
	DeleteProjectViews();
}


BEGIN_MESSAGE_MAP(CLatexProject, CProject)
	//{{AFX_MSG_MAP(CLatexProject)
	ON_COMMAND(ID_PROJECT_PROPERTIES, OnProjectProperties)
	ON_COMMAND(ID_PROJECT_OPEN_MAINFILE, OnProjectOpenMainfile)
	ON_COMMAND(ID_PROJECT_PARSED, OnProjectParsed)
	ON_COMMAND(ID_PROJECT_PARSE, OnProjectParse)
	ON_COMMAND(ID_ITEM_PROPERTIES, OnItemProperties)
	ON_UPDATE_COMMAND_UI(ID_ITEM_PROPERTIES, OnUpdateItemCmd)
	ON_COMMAND(ID_ITEM_GOTO, OnItemGoto)
	ON_COMMAND(ID_ITEM_INSERT_PAGEREF, OnItemInsertPageref)
	ON_COMMAND(ID_ITEM_INSERT_REF, OnItemInsertRef)
	ON_COMMAND(ID_ITEM_INSERT_LABEL, OnItemInsertLabel)
	ON_UPDATE_COMMAND_UI(ID_ITEM_GOTO, OnUpdateItemCmd)
	ON_UPDATE_COMMAND_UI(ID_ITEM_INSERT_PAGEREF, OnUpdateItemCmd)
	ON_UPDATE_COMMAND_UI(ID_ITEM_INSERT_REF, OnUpdateItemCmd)
	ON_UPDATE_COMMAND_UI(ID_ITEM_INSERT_LABEL, OnUpdateItemCmd)
	ON_COMMAND(ID_SPELL_PROJECT, OnSpellProject)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


#ifdef _DEBUG
void CLatexProject::AssertValid() const
{
	CProject::AssertValid();
}

void CLatexProject::Dump(CDumpContext& dc) const
{
	CProject::Dump(dc);
}
#endif //_DEBUG


#define KEY_FORMATINFO						_T("FormatInfo")
#define VAL_FORMATINFO_TYPE					_T("Type")
#define VAL_FORMATINFO_VERSION				_T("Version")

#define KEY_PROJECTINFO						_T("ProjectInfo")
#define VAL_PROJECTINFO_WORKINGDIR			_T("WorkingDir")
#define VAL_PROJECTINFO_MAINFILE			_T("MainFile")
#define VAL_PROJECTINFO_USEBIBTEX			_T("UseBibTeX")
#define VAL_PROJECTINFO_USEMAKEINDEX		_T("UseMakeIndex")
#define VAL_PROJECTINFO_ACTIVEPROFILE		_T("ActiveProfile")

#define VAL_PROJECTINFO_PLANGUAGE			_T("ProjectLanguage")
#define VAL_PROJECTINFO_PDIALECT			_T("ProjectDialect")

#define CURRENTFORMATVERSION				4
#define	FORMATTYPE							_T("TeXnicCenterProjectInformation")

BOOL CLatexProject::Serialize( CIniFile &ini, BOOL bWrite )
{
	if( bWrite )
	{
		// setting format information
		ini.SetValue( KEY_FORMATINFO, VAL_FORMATINFO_TYPE,				FORMATTYPE );
		ini.SetValue( KEY_FORMATINFO, VAL_FORMATINFO_VERSION,			CURRENTFORMATVERSION );

		// setting project information
		ini.SetValue(KEY_PROJECTINFO, VAL_PROJECTINFO_MAINFILE,			CPathTool::GetRelativePath(GetProjectDir(), m_strMainPath));
		ini.SetValue(KEY_PROJECTINFO, VAL_PROJECTINFO_USEBIBTEX,		(int)m_bUseBibTex);
		ini.SetValue(KEY_PROJECTINFO, VAL_PROJECTINFO_USEMAKEINDEX,		(int)m_bUseMakeIndex);
		ini.SetValue(KEY_PROJECTINFO, VAL_PROJECTINFO_ACTIVEPROFILE,	g_ProfileMap.GetActiveProfileKey());

		ini.SetValue(KEY_PROJECTINFO, VAL_PROJECTINFO_PLANGUAGE,		m_strProjectLanguage);
		ini.SetValue(KEY_PROJECTINFO, VAL_PROJECTINFO_PDIALECT,			m_strProjectDialect);
		return TRUE;
	}
	else
	{
		// getting format information
		CString	strType = ini.GetValue( KEY_FORMATINFO, VAL_FORMATINFO_TYPE, "" );
		int			nVersion = ini.GetValue( KEY_FORMATINFO, VAL_FORMATINFO_VERSION, 0 );

		if( strType != FORMATTYPE )
			return FALSE;

		// setting project information
		if (!ini.VerifyValue(KEY_PROJECTINFO, VAL_PROJECTINFO_MAINFILE))
			return FALSE;

		if (nVersion == 1)
			m_strProjectDir = ini.GetValue(KEY_PROJECTINFO, VAL_PROJECTINFO_WORKINGDIR, "");

		m_strMainPath = ini.GetValue(KEY_PROJECTINFO, VAL_PROJECTINFO_MAINFILE, "");
		if (nVersion > 1)
			m_strMainPath = CPathTool::Cat(GetProjectDir(), m_strMainPath);

		m_bUseBibTex			= ini.GetValue(KEY_PROJECTINFO, VAL_PROJECTINFO_USEBIBTEX,		FALSE);
		m_bUseMakeIndex			= ini.GetValue(KEY_PROJECTINFO, VAL_PROJECTINFO_USEMAKEINDEX,	FALSE);

		m_strProjectLanguage	= ini.GetValue(KEY_PROJECTINFO, VAL_PROJECTINFO_PLANGUAGE,		g_configuration.m_strLanguageDefault);
		m_strProjectDialect		= ini.GetValue(KEY_PROJECTINFO, VAL_PROJECTINFO_PDIALECT,		g_configuration.m_strLanguageDialectDefault);

		if (nVersion > 2)
		{
			if (g_ProfileMap.SetActiveProfile(ini.GetValue(KEY_PROJECTINFO, VAL_PROJECTINFO_ACTIVEPROFILE, ""), false))
			{
				//Successfull change of the active profile - update UI
				theApp.UpdateLatexProfileSel();
			}
			else
			{
				//Saved profile name not found - other profile used - set project to be modified
				SetModifiedFlag(true);
			}
		}

		return TRUE;
	}
}

#undef KEY_FORMATINFO			
#undef VAL_FORMATINFO_TYPE			
#undef VAL_FORMATINFO_VERSION
#undef KEY_PROJECTINFO	
#undef VAL_PROJECTINFO_WORKINGDIR
#undef VAL_PROJECTINFO_MAINFILE
#undef CURRENTFORMATVERSION
#undef FORMATTYPE


#define KEY_FORMATINFO							_T("FormatInfo")
#define VAL_FORMATINFO_TYPE						_T("Type")
#define VAL_FORMATINFO_VERSION					_T("Version")

#define KEY_SESSIONINFO							_T("SessionInfo")
#define VAL_SESSIONINFO_ACTIVETAB				_T("ActiveTab")
#define VAL_SESSIONINFO_FRAMECOUNT				_T("FrameCount")
#define VAL_SESSIONINFO_ACTIVEFRAME				_T("ActiveFrame")

#define KEY_FRAMEINFO							_T("Frame%d")

#define CURRENTFORMATVERSION					2
#define	FORMATTYPE								_T("TeXnicCenterProjectSessionInformation")

void CLatexProject::SerializeSession(CIniFile &ini, BOOL bWrite)
{
	if (bWrite)
	{
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//Format information
		ini.SetValue(KEY_FORMATINFO, VAL_FORMATINFO_TYPE, FORMATTYPE);
		ini.SetValue(KEY_FORMATINFO, VAL_FORMATINFO_VERSION, CURRENTFORMATVERSION);

		//Store active tab of navigator
		CBCGTabWnd* pwndTabs = &m_pwndWorkspaceBar->GetTabWnd();
		if (pwndTabs && IsWindow(pwndTabs->m_hWnd))
		{
			ini.SetValue(KEY_SESSIONINFO, VAL_SESSIONINFO_ACTIVETAB, pwndTabs->GetActiveTab());
		}

		//Store frame information
		if (m_pwndMainFrame)
		{
			//Get the MDI Childs (sorted by Tabs)
			CArray<CChildFrame*, CChildFrame*> MDIChildArray;
			const int nActiveFrame = m_pwndMainFrame->GetMDIChilds(MDIChildArray, true);

			//Set Number of Frames
			ini.SetValue(KEY_SESSIONINFO, VAL_SESSIONINFO_FRAMECOUNT, MDIChildArray.GetSize());
			//Set active MDI Child
			ini.SetValue(KEY_SESSIONINFO, VAL_SESSIONINFO_ACTIVEFRAME, nActiveFrame);

			for(int i=0;i<MDIChildArray.GetSize();i++)
			{
				//Store information for this MDI-window
				CString strKey;
				strKey.Format(KEY_FRAMEINFO, i);

				MDIChildArray.GetAt(i)->Serialize(ini, strKey, bWrite);
			}
		}
	}
	else
	{
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// reading format information
		CString	strKey;
		const int nFrameCount = ini.GetValue(KEY_SESSIONINFO, VAL_SESSIONINFO_FRAMECOUNT, 0);
		const int nActiveFrame = ini.GetValue(KEY_SESSIONINFO, VAL_SESSIONINFO_ACTIVEFRAME, -1);
		CCreateContext cc;

		ZeroMemory(&cc, sizeof(cc));
		cc.m_pNewViewClass = RUNTIME_CLASS(CLatexEdit);

		//Open all stored frames
		bool bCouldOpenAllFrames(true);
		CChildFrame* pChildToBeActivated = NULL;
		for (int nFrame = 0; nFrame < nFrameCount; nFrame++)
		{
			CChildFrame* pChildFrame = new CChildFrame();

			ASSERT(pChildFrame);
			if (!pChildFrame) continue;

			strKey.Format(KEY_FRAMEINFO, nFrame);
			if (!pChildFrame->Serialize(ini, strKey, bWrite))
			{
				bCouldOpenAllFrames = false;
				delete pChildFrame;
			}
			else
			{
				if (nActiveFrame == nFrame)
				{
					pChildToBeActivated = pChildFrame;
				}
			}
		}

		//Restore navigator tab selection
		m_nInitialNavigatorTab = ini.GetValue(KEY_SESSIONINFO, VAL_SESSIONINFO_ACTIVETAB, 0);

		//Activate the frame that was active when closing the project
		if (pChildToBeActivated)
		{
			pChildToBeActivated->ActivateFrame(SW_SHOW);
			//NOTE: We give the focus later to this window
		}

		//Give message, if we could not open all files / frames
		if (!bCouldOpenAllFrames)
		{
			AfxMessageBox(STE_SESSION_RESTORE_NOTALLWINDOWSRESTORED,
							MB_ICONINFORMATION|MB_OK);
		}
	}
}



/////////////////////////////////////////////////////////////////////
// paths, open, save, etc.
void CLatexProject::SetPathName(LPCTSTR lpszPathName) 
{
	CProject::SetPathName(lpszPathName);
	SetProjectDir(CPathTool::GetDirectory(lpszPathName));
}


void CLatexProject::DoProjectSave() 
{
	OnSaveProject( GetPathName() );
}


void CLatexProject::SetMainPath( LPCTSTR lpszMainPath )
{
	if( m_strMainPath != lpszMainPath )
		SetModifiedFlag();
	m_strMainPath = lpszMainPath;
}


CString CLatexProject::GetMainPath() const
{
	return m_strMainPath;
}


void CLatexProject::SetProjectDir(LPCTSTR lpszProjectDir)
{
	m_strProjectDir = lpszProjectDir;
}


CString CLatexProject::GetProjectDir() const
{
	return m_strProjectDir;
}


CString CLatexProject::GetFilePath( LPCTSTR lpszFile )
{
	CString		strPath;
	LPTSTR		dummy;

	SetCurrentDirectory(CPathTool::GetDirectory(m_strMainPath));
	GetFullPathName( lpszFile, _MAX_PATH, strPath.GetBuffer( _MAX_PATH ), &dummy );
	strPath.ReleaseBuffer();

	return strPath;
}

//Returns the full path to the project session file
CString CLatexProject::GetSessionPathName(LPCTSTR lpszPath /*= NULL*/) const
{
	if (!lpszPath && GetPathName().IsEmpty())
		return CString();

	return ( CPathTool::GetBase( (lpszPath != NULL) ? lpszPath : GetPathName() )
						+ _T('.') + TC_PROJECT_SESSION_EXT );
}


void CLatexProject::SetRunBibTex(BOOL bRunBibTex)
{
	if (bRunBibTex != m_bUseBibTex)
		SetModifiedFlag();

	m_bUseBibTex = bRunBibTex;
}


void CLatexProject::SetRunMakeIndex(BOOL bRunMakeIndex)
{
	if (bRunMakeIndex != m_bUseMakeIndex)
		SetModifiedFlag();

	m_bUseMakeIndex = bRunMakeIndex;
}


BOOL CLatexProject::GetRunBibTex() const
{
	return m_bUseBibTex;
}


BOOL CLatexProject::GetRunMakeIndex() const
{
	return m_bUseMakeIndex;
}


CString CLatexProject::GetWorkingDir() const
{
	return CPathTool::GetDirectory(m_strMainPath);
}


void CLatexProject::OnProjectProperties() 
{
	CProjectPropertyDialog	dlg(theApp.m_pMainWnd);
	dlg.m_strProjectDir		= GetProjectDir();
	dlg.m_strMainFile		= m_strMainPath;
	dlg.m_bUseBibTex		= m_bUseBibTex;
	dlg.m_bUseMakeIndex		= m_bUseMakeIndex;
	dlg.m_strLanguageCurrent= m_strProjectLanguage;
	dlg.m_strDialectCurrent	= m_strProjectDialect;

	if(dlg.DoModal() != IDOK )
		return;

	if (m_strMainPath			!= dlg.m_strMainFile	||
		m_bUseBibTex			!= dlg.m_bUseBibTex		||
		m_bUseMakeIndex			!= dlg.m_bUseMakeIndex)
	{
		SetModifiedFlag();
	}

	if(	m_strProjectLanguage	!= dlg.m_strLanguageCurrent	||
		m_strProjectDialect		!= dlg.m_strDialectCurrent)
	{
		SetModifiedFlag();
		// TO DO: restart the speller on the new language
	}


	m_strMainPath	= dlg.m_strMainFile;
	m_bUseBibTex	= dlg.m_bUseBibTex;
	m_bUseMakeIndex = dlg.m_bUseMakeIndex;
	m_strProjectLanguage	= dlg.m_strLanguageCurrent;
	m_strProjectDialect		= dlg.m_strDialectCurrent;
}


void CLatexProject::OnProjectOpenMainfile() 
{
	theApp.OpenLatexDocument(m_strMainPath, FALSE, -1, FALSE, false);
}


/////////////////////////////////////////////////////////////////////
// implementation of CStructureParserHandler virtuals

void CLatexProject::OnParsingFinished( BOOL bSuccess )
{
	// synchronize with main thread
	if( bSuccess )
		theApp.m_pMainWnd->SendMessage( WM_COMMAND, (WPARAM)ID_PROJECT_PARSED );

}


/////////////////////////////////////////////////////////////////////
// manage project parsing
void CLatexProject::OnProjectParse() 
{
	if( !m_bCanParse )
		return;

	// Update the background thread
	AfxGetMainWnd()->PostMessage( WM_COMMAND, ID_BG_UPDATE_PROJECT );

	// change to working dir
	SetCurrentDirectory(CPathTool::GetDirectory(m_strMainPath));

	// start parsing
	m_bCanParse = FALSE;
	m_pStructureParser->StartParsing( m_strMainPath, GetWorkingDir() );
}


void CLatexProject::OnProjectParsed() 
{
	m_pStructureParser->GetStructureItems( &m_aStructureItems );
	UpdateAllViews( NULL, COutputDoc::hintParsingFinished, (CObject*)&m_aStructureItems );
	m_bCanParse = TRUE;
}


void CLatexProject::SetCurrentStructureItem( int nIndex )
{
	ASSERT( (nIndex == 0) || (nIndex < m_aStructureItems.GetSize()) ); // invalid index
	if (nIndex < 0 || nIndex >= m_aStructureItems.GetSize())
		return;

	m_nCurrentStructureItem = nIndex;
}

void CLatexProject::OnUpdateItemCmd(CCmdUI* pCmdUI) 
{
	ASSERT( m_nCurrentStructureItem >= -1 && m_nCurrentStructureItem < m_aStructureItems.GetSize() );

	// disable command, if no valid item is selected
	if( m_nCurrentStructureItem == -1 )
	{
		pCmdUI->Enable( FALSE );
		return;
	}

	// check commands
	const CStructureItem	&si = m_aStructureItems[m_nCurrentStructureItem];

	switch( pCmdUI->m_nID )
	{
		case ID_ITEM_GOTO:
			switch( si.m_nType )
			{
			case CStructureParser::texFile:
			case CStructureParser::bibFile:
				pCmdUI->Enable( !si.m_strPath.IsEmpty() );
				break;
			default:
				pCmdUI->Enable( si.m_nLine > 0 && !si.m_strPath.IsEmpty() );
			}
			break;
		
		case ID_ITEM_INSERT_PAGEREF: // makes no sense for BibTeX entries
			if (si.m_nType != CStructureParser::bibItem) {
				pCmdUI->Enable( !si.m_strLabel.IsEmpty() && theApp.GetActiveEditView() );
			} else {
				pCmdUI->Enable(FALSE);
			}
			break;
		case ID_ITEM_INSERT_LABEL:
		case ID_ITEM_INSERT_REF:
			pCmdUI->Enable( !si.m_strLabel.IsEmpty() && theApp.GetActiveEditView() );
			break;
		default:
			pCmdUI->Enable();
	}
}

void CLatexProject::OnItemProperties() 
{
	ASSERT( m_nCurrentStructureItem >= 0 && m_nCurrentStructureItem < m_aStructureItems.GetSize() );
	CItemPropertyDialog	dialog( m_aStructureItems[m_nCurrentStructureItem] );
	dialog.DoModal();
}

void CLatexProject::OnItemGoto() 
{
	if( !(m_nCurrentStructureItem >= 0 && m_nCurrentStructureItem < m_aStructureItems.GetSize()) )
		return;

	const CStructureItem	&si = m_aStructureItems[m_nCurrentStructureItem];

	switch( si.m_nType )
	{
		case CStructureParser::texFile:
		case CStructureParser::bibFile:
			theApp.OpenLatexDocument(GetFilePath( si.m_strPath ), FALSE, -1, FALSE, false);
			break;
		case CStructureParser::missingTexFile:
		case CStructureParser::missingGraphicFile:
		case CStructureParser::missingBibFile:
		case CStructureParser::graphicFile:
			theApp.OpenLatexDocument(GetFilePath( si.m_strComment ), FALSE, si.m_nLine, FALSE, false);
			break;
		default:
			theApp.OpenLatexDocument(GetFilePath( si.m_strPath ), FALSE, si.m_nLine, FALSE, false);
	}
}


void CLatexProject::OnItemInsertLabel() 
{
	if( !(m_nCurrentStructureItem >= 0 && m_nCurrentStructureItem < m_aStructureItems.GetSize()) )
		return;

	CLatexEdit	*pView = theApp.GetActiveEditView();
	if( !pView )
		return;

	pView->InsertText(m_aStructureItems[m_nCurrentStructureItem].m_strLabel);
	pView->SetFocus();
}


void CLatexProject::OnItemInsertPageref() 
{
	if( !(m_nCurrentStructureItem >= 0 && m_nCurrentStructureItem < m_aStructureItems.GetSize()) )
		return;

	CLatexEdit	*pView = theApp.GetActiveEditView();
	if( !pView )
		return;

	pView->InsertText( 
		AfxFormatString1( 
			STE_LATEX_PAGEREF, 
			m_aStructureItems[m_nCurrentStructureItem].m_strLabel ) );
	pView->SetFocus();
}

void CLatexProject::OnItemInsertRef() 
{
	if( !(m_nCurrentStructureItem >= 0 && m_nCurrentStructureItem < m_aStructureItems.GetSize()) )
		return;

	CLatexEdit	*pView = theApp.GetActiveEditView();
	if( !pView )
		return;

	UINT strID = 0;
	switch (m_aStructureItems[m_nCurrentStructureItem].m_nType)
	{
		case CStructureParser::equation:
			strID = STE_LATEX_EQREF;
			break;
		case CStructureParser::bibItem:
			strID = STE_LATEX_CITE;
			break;
		default:
			strID = STE_LATEX_REF;
			break;
	}

	pView->InsertText( 
		AfxFormatString1(strID,
				m_aStructureItems[m_nCurrentStructureItem].m_strLabel) );
	pView->SetFocus();
}


void CLatexProject::OnSpellProject() 
{
	MySpell *pSpell = theApp.GetSpeller();
	
	// Setup path for ignored words
	CString sIgnoredPath = 
		CString(theApp.GetProject()->GetProjectDir() +
		_T("\\ignored") + 
		theApp.GetProject()->GetTitle() +
		g_configuration.m_strGuiLanguage + _T(".sc"));

	if (pSpell == NULL)
		return;

	CCrystalResources	cr;
	CSpellCheckDlg dlg( NULL, NULL );
	dlg.m_bDoneMessage = false;
	dlg.m_bSelection = false;

	// Get own copy of strucutre items.
	CStructureItemArray aStructureItems;
	m_pStructureParser->GetStructureItems( &aStructureItems );

	for (int i = 0; i < aStructureItems.GetSize(); ++i)
	{
		CStructureItem& si = aStructureItems[i];
		if ( si.m_nType == CStructureParser::texFile )
		{
			boolean bWasOpen = true;
			CDocument *pDoc = theApp.GetOpenLatexDocument(GetFilePath(si.m_strPath), FALSE);
			if ( pDoc == NULL )
			{
				pDoc = theApp.OpenLatexDocument(GetFilePath(si.m_strPath), FALSE, -1, FALSE, false);
				bWasOpen = false;
			}
			if ( pDoc == NULL )
				// Can't open document, try another one.
				continue;

			POSITION pos = pDoc->GetFirstViewPosition();
			CLatexEdit* pView = (CLatexEdit*) pDoc->GetNextView( pos );
			ASSERT( pView );
			if ( pView == NULL )
				// View is NULL??
				continue;

			dlg.Reset(pView, pSpell);

			// Save selection
			CPoint ptStart, ptEnd;
			pView->GetSelection(ptStart, ptEnd);
			pView->SetShowInteractiveSelection(TRUE);

			// begin ow
			if (CPathTool::Exists(sIgnoredPath)) {
				theApp.GetSpeller()->open_ignored_words(sIgnoredPath);
			}
			// end ow
			int result = dlg.DoModal();
			cr.RestorePrevResources();

			// begin ow
			if (theApp.GetSpeller()->ismodified()) {		
				//TRACE("SC was modified, save ign words to %s...\n", sPath);		
				theApp.GetSpeller()->save_ignored_words(sIgnoredPath);				
			}
			// end ow

			// Restore selection
			pView->SetShowInteractiveSelection(FALSE);
			pView->SetSelection(ptStart, ptEnd);

			if ( !bWasOpen )
				pView->SendMessage(WM_COMMAND, ID_FILE_CLOSE);
			if ( result == IDABORT )
				break;
			else if ( result != IDOK )
				if ( AfxMessageBox(AfxLoadString(IDS_SPELL_CONTINUE), MB_YESNO|MB_ICONQUESTION) != IDYES )
					break;
		}
	}
}


BOOL CLatexProject::CreateProjectViews()
{
	ASSERT(m_pwndMainFrame);
	ASSERT(m_pwndWorkspaceBar);
	ASSERT(m_pwndOutputBar);

	// Check if views are allready existing
	if( m_pwndStructureView && IsWindow( m_pwndStructureView->m_hWnd ) )
		return FALSE;

	// Create views:
	CRect						rectDummy;
	rectDummy.SetRectEmpty();
	CBCGTabWnd* pwndTabs = &m_pwndWorkspaceBar->GetTabWnd();

	m_pwndStructureView   = new CStructureView;
	m_pwndEnvironmentView = new CEnvironmentView;
	m_pwndFileView = new CFileView;
	m_pwndBibView = new CBibView;

	if( !m_pwndStructureView || !m_pwndEnvironmentView || !m_pwndFileView || !m_pwndBibView )
	{
		if( m_pwndStructureView )
			delete m_pwndStructureView;
		if( m_pwndEnvironmentView )
			delete m_pwndEnvironmentView;
		if( m_pwndFileView )
			delete m_pwndFileView;
		if( m_pwndBibView )
			delete m_pwndBibView;
	}

	if (!m_pwndStructureView->Create(pwndTabs))
	{
		TRACE0("Failed to create structure view\n");
		return FALSE;
	}
	m_pwndStructureView->SetOwner(m_pwndWorkspaceBar);

	if (!m_pwndEnvironmentView->Create(pwndTabs))
	{
		TRACE0("Failed to create environment view\n");
		return FALSE;
	}
	m_pwndEnvironmentView->SetOwner(m_pwndWorkspaceBar);

	if (!m_pwndFileView->Create(pwndTabs))
	{
		TRACE0("Failed to create file view\n");
		return FALSE;
	}
	m_pwndFileView->SetOwner(m_pwndWorkspaceBar);

	if (!m_pwndBibView->Create(pwndTabs))
	{
		TRACE0("Failed to create bib view\n");
		return FALSE;
	}
	m_pwndBibView->SetOwner(m_pwndWorkspaceBar);

	// Attach views to tab:
	pwndTabs->AddTab( m_pwndStructureView, CString( (LPCTSTR)STE_TAB_STRUCTURE ), 0 );
	pwndTabs->AddTab( m_pwndEnvironmentView, CString( (LPCTSTR)STE_TAB_ENVIRONMENTS ), 1 );
	pwndTabs->AddTab( m_pwndFileView, CString( (LPCTSTR)STE_TAB_FILES ), 2 );
	pwndTabs->AddTab( m_pwndBibView, CString( (LPCTSTR)STE_TAB_BIBENTRIES ), 3 );

	if (m_nInitialNavigatorTab < 0 || m_nInitialNavigatorTab > 3)
		m_nInitialNavigatorTab = 0;
	pwndTabs->SetActiveTab(m_nInitialNavigatorTab);

	// Attach views to document (project)
	AddView( m_pwndStructureView );
	AddView( m_pwndEnvironmentView );
	AddView( m_pwndFileView );
	AddView( m_pwndBibView );

	return TRUE;
}

void CLatexProject::DeleteProjectViews()
{
	ASSERT(m_pwndMainFrame);
	ASSERT(m_pwndWorkspaceBar);
	ASSERT(m_pwndOutputBar);
	
	// Check if views are valid windows
	if( !m_pwndStructureView || !IsWindow( m_pwndStructureView->m_hWnd ) )
		return;

	// Detach views from document
	RemoveView( m_pwndStructureView );
	RemoveView( m_pwndEnvironmentView );
	RemoveView( m_pwndFileView );
	RemoveView( m_pwndBibView );

	// Detach views from tab...
	CBCGTabWnd* pwndTabs = &m_pwndWorkspaceBar->GetTabWnd();
	pwndTabs->RemoveAllTabs();

	// ... and destroy them
	delete m_pwndStructureView;
	delete m_pwndEnvironmentView;
	delete m_pwndFileView;
	delete m_pwndBibView;

	// mark pointers as NULL
	m_pwndStructureView   = NULL;
	m_pwndEnvironmentView = NULL;
	m_pwndFileView        = NULL;
	m_pwndBibView         = NULL;
}


Interfaces::IProject *CLatexProject::GetProjectInterface()
{
	m_xProject.AddRef();
	return &m_xProject;
}


//-------------------------------------------------------------------
// class CLatexProject::XProject
//-------------------------------------------------------------------

IMPLEMENT_AGGREGATED_IUNKNOWN(CLatexProject, Project);


CString CLatexProject::XProject::GetTitle_() const
{
	GET_OUTER(CLatexProject, Project);
	
	return pThis->GetTitle();
}


CString CLatexProject::XProject::GetPath_() const
{
	GET_OUTER(CLatexProject, Project);

	return pThis->GetWorkingDir();
}


CString CLatexProject::XProject::GetMainFile_() const
{
	GET_OUTER(CLatexProject, Project);

	return pThis->GetMainPath();
}


BOOL CLatexProject::XProject::GetUsesBibTex_() const
{
	GET_OUTER(CLatexProject, Project);

	return pThis->GetRunBibTex();
}


void CLatexProject::XProject::SetUsesBibTex_(BOOL bUsesBibTex)
{
	GET_OUTER(CLatexProject, Project);

	pThis->SetRunBibTex(bUsesBibTex);
}


BOOL CLatexProject::XProject::GetUsesMakeIndex_() const
{
	GET_OUTER(CLatexProject, Project);

	return pThis->GetRunMakeIndex();
}


void CLatexProject::XProject::SetUsesMakeIndex_(BOOL bUsesMakeIndex)
{
	GET_OUTER(CLatexProject, Project);

	pThis->SetRunMakeIndex(bUsesMakeIndex);
}

