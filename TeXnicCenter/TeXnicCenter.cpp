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
#include "ChildFrm.h"
#include "LatexDoc.h"
#include "LatexEdit.h"
#include "global.h"
#include "Configuration.h"
#include "DocumentNewDialog.h"
#include "ProjectNewDialog.h"
#include "OptionDialog.h"
#include "ActiveProfileDialog.h"
#include "AboutBox.h"
#include "OutputWizard.h"

#include <direct.h>
#include "Splash.h"
#include "UserTool.h"
#include "Profile.h"
#include "ProfileDialog.h"
#include <locale.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// class CTCCommandLineInfo
//-------------------------------------------------------------------
class CTCCommandLineInfo : public CCommandLineInfo
{
// Construction/destruction
public:
	CTCCommandLineInfo();

// Overridings
protected:
	virtual void ParseParam( LPCTSTR lpszParam, BOOL bFlag, BOOL bLast );

// Attributes
public:
	/** 
	Line number, if one was specified with the -line or -l parameter 
	or -1, if no line has been specified.
	*/
	int m_nDocumentLine;

	/**
	DDE-command, if one was specified, otherwise an empty string.
	*/
	CString m_strDdeCommand;
};


CTCCommandLineInfo::CTCCommandLineInfo()
:	CCommandLineInfo(),
	m_nDocumentLine( -1 )
{}


void CTCCommandLineInfo::ParseParam( LPCTSTR lpszParam, BOOL bFlag, BOOL bLast )
{
	CString			strParam( lpszParam );

	// handle line nummber
	static BOOL	bLastWasLineFlag = FALSE;

	if( bLastWasLineFlag )
	{
		if( !bFlag )
			m_nDocumentLine = _ttol( lpszParam );
		else
			TRACE0("Line number expected on command line\n");

		bLastWasLineFlag = FALSE;
		return;
	}

	if (strParam.CompareNoCase(_T("line")) == 0 || strParam.CompareNoCase(_T("l")) == 0)
	{
		bLastWasLineFlag = TRUE;
		return;
	}

	// handle request to send dde command
	static BOOL	bLastWasDdeCommandFlag = FALSE;

	if (bLastWasDdeCommandFlag)
	{
		if (!bFlag)
		{
			// store dde-command
			m_strDdeCommand = lpszParam;
			m_strDdeCommand.TrimLeft(_T('"'));
			m_strDdeCommand.TrimRight(_T('"'));
			m_strDdeCommand.Replace(_T('\''), _T('"'));
		}
		else
			TRACE0("DDE-command expected on command line\n");

		bLastWasDdeCommandFlag = FALSE;
		return;
	}

	if (strParam.CompareNoCase(_T("ddecmd")) == 0)
	{
		bLastWasDdeCommandFlag = TRUE;
		return;
	}
	else if (strParam.CompareNoCase(_T("nosplash")) == 0)
	{
		m_bShowSplash = FALSE;
		return;
	}


	CCommandLineInfo::ParseParam( lpszParam, bFlag, bLast );
	return;
}


//-------------------------------------------------------------------
// class CTeXnicCenterApp
//-------------------------------------------------------------------

// the one and only
CTeXnicCenterApp theApp;


BEGIN_MESSAGE_MAP(CTeXnicCenterApp, CProjectSupportingWinApp)
	ON_COMMAND(CG_IDS_TIPOFTHEDAY, ShowTipOfTheDay)
	//{{AFX_MSG_MAP(CTeXnicCenterApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_TIP, OnTip)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_SAVE_ALL, OnFileSaveAll)
	ON_UPDATE_COMMAND_UI(ID_FILE_CLOSE, OnDisableStdCmd)
	ON_COMMAND(ID_PROJECT_OPEN, OnProjectOpen)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_CLOSE, OnUpdateProjectClose)
	ON_COMMAND(ID_LATEX_NEW, OnLatexNew)
	ON_COMMAND(ID_EXTRAS_OPTIONS, OnExtrasOptions)
	ON_COMMAND(ID_LATEX_PROFILE, OnLatexProfile)
	ON_COMMAND(ID_DOCUMENT_SAVED, OnDocumentSaved)
	ON_UPDATE_COMMAND_UI(ID_FILE_MRU_FILE1, OnUpdateFileMRU)
	ON_COMMAND(ID_LATEX_PROFILE_SEL, OnLatexProfileSel)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_APP_LICENSE, OnAppLicense)
	ON_COMMAND(ID_PROJECT_SAVE, OnProjectSave)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_SAVE, OnUpdateProjectSave)
	ON_COMMAND(ID_LATEX_EDITPROFILES, OnLatexEditProfiles)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_CLOSE_ALL, OnUpdateWindowCloseAll)
	ON_COMMAND(ID_WINDOW_CLOSE_ALL, OnWindowCloseAll)
	ON_COMMAND(ID_HELP, OnHelp)
	ON_COMMAND(ID_BG_UPDATE_PROJECT, OnUpdateProject)
	ON_COMMAND(ID_PROJECT_NEW_FROM_FILE, OnProjectNewFromFile)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_NEW_FROM_FILE, OnUpdateProjectNewFromFile)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnDisableStdCmd)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnDisableStdCmd)
	//}}AFX_MSG_MAP
	// Dateibasierte Standard-Dokumentbefehle
	//ON_COMMAND(ID_FILE_NEW, CProjectSupportingWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CProjectSupportingWinApp::OnFileOpen)
	// Standard-Druckbefehl "Seite einrichten"
	ON_COMMAND(ID_FILE_PRINT_SETUP, CProjectSupportingWinApp::OnFilePrintSetup)
	// handle most recently used projects
	ON_UPDATE_COMMAND_UI(ID_FILE_MRU_PROJECT_FIRST, OnUpdateFileMRUProjectList)
	ON_COMMAND_RANGE(ID_FILE_MRU_PROJECT_FIRST, ID_FILE_MRU_PROJECT_LAST, OnFileMRUProject)
	// handle most recently used files
//	ON_COMMAND_RANGE(ID_FILE_MRU_FILE1, ID_FILE_MRU_FILE10, OnFileMRUFile)
	ON_UPDATE_COMMAND_UI_RANGE(ID_URL_FIRST, ID_URL_LAST, OnUpdateURL)
	ON_COMMAND_RANGE(ID_URL_FIRST, ID_URL_LAST, OnURL)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTeXnicCenterApp Konstruktion

CTeXnicCenterApp::CTeXnicCenterApp()
:	CProjectSupportingWinApp(),
	CBCGWorkspace(TRUE),
	m_bEndSession( FALSE ),
	m_bSavingAll( FALSE ),
	m_recentProjectList( 1, _T("Recent Project List"), _T("Project%d"), 4 ),
	m_bTabFlatBorders(FALSE),
	m_pSpell(NULL),
	m_pBackgroundThread(NULL)
{
	InitializeCriticalSection( &m_csLazy );
}


BOOL CTeXnicCenterApp::InitInstance()
{
	// check if we should use an existing instance
	if( !CProjectSupportingWinApp::InitInstance() )
		return FALSE;

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// loading localized resource DLL for BCGCB
	m_hInstBCGCBRes = LoadLibrary(CString((LPCTSTR)STE_BCGCBRESDLL));
	if (m_hInstBCGCBRes)
		BCGCBSetResourceHandle(m_hInstBCGCBRes);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// load configuration from registry

	// set registry key
	SetRegistryKey( _T("ToolsCenter") );
	m_strRegistryRoot = CString( "Software\\" ) + m_pszRegistryKey;
	m_strRegistryRoot+= _T("\\");
	m_strRegistryRoot+= m_pszAppName;

	LoadStdProfileSettings();

	g_configuration.Serialize(CConfiguration::Load);

	// Set the locale
	_tsetlocale( LC_ALL, g_configuration.m_strLocale );

	// parse command line
	CTCCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// handle DDE-command on command line
	if (!cmdInfo.m_strDdeCommand.IsEmpty())
	{
		// try to forward dde command to an existing instance ...
		if (CDdeCommand::SendCommand(_T("TEXCNTR"), cmdInfo.m_strDdeCommand, _T("System")))
			// ... if successfull, exit this instance
			return TRUE;
	}
	
	// show splash screen
	if (cmdInfo.m_bShowSplash)
	{
		CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);
		CSplashWnd::ShowSplashScreen( NULL );
	}

	// Standardinitialisierung
	if( !AfxOleInit() )
		return FALSE;

#ifdef _AFXDLL
	Enable3dControls();			// Diese Funktion bei Verwendung von MFC in gemeinsam genutzten DLLs aufrufen
#else
	Enable3dControlsStatic();	// Diese Funktion bei statischen MFC-Anbindungen aufrufen
#endif
	
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Get Working Directory (directory, TeXnicCenter was installed in)
	GetModuleFileName( NULL, m_strWorkingDir.GetBuffer( MAX_PATH ), MAX_PATH );
	m_strWorkingDir.ReleaseBuffer();
	m_strWorkingDir = CPathTool::GetDirectory( m_strWorkingDir );

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Initialize BCG
	SetRegistryBase( _T("BCGWorkspace") );
	InitContextMenuManager();
	InitKeyboardManager();
	InitSkinManager(g_configuration.m_strSkinDirectory);
	GetSkinManager()->EnableSkinsDownload(g_configuration.m_strSkinUrl);
	CBCGToolbarComboBoxButton::SetFlatMode();

	EnableUserTools(
		ID_TOOLS_ENTRY, ID_USER_TOOL_FIRST, ID_USER_TOOL_LAST, 
		RUNTIME_CLASS(CUserTool), 
		0, IDR_POPUP_PLACEHOLDER_DIR);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Dokumentvorlagen registrieren
	// LaTeX-Document
	m_pLatexDocTemplate = new CMultiDocTemplate(
		IDR_LATEXTYPE,
		RUNTIME_CLASS(CLatexDoc),
		RUNTIME_CLASS(CChildFrame), // Benutzerspezifischer MDI-Child-Rahmen
		RUNTIME_CLASS(CLatexEdit) );
	AddDocTemplate(m_pLatexDocTemplate);

	m_pProjectDocTemplate = new CSingleProjectTemplate(
		IDR_LATEXPROJECT,
		RUNTIME_CLASS(CLatexProject),
		2	// icon index in the executable
	);
	AddProjectTemplate(m_pProjectDocTemplate);

	// Haupt-MDI-Rahmenfenster erzeugen
	CMainFrame *pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// install frame manager
	m_pMDIFrameManager = new CMDIFrameManager();
	m_pMDIFrameManager->Install( pMainFrame );

	// enable file open via drag & drop
	m_pMainWnd->DragAcceptFiles();

	// initialize file change notifier
	//m_FileChangeNotifier.Create(pMainFrame);

	// enable DDE commands
	EnableShellOpen();
	RegisterShellFileTypes( TRUE );
	//RegisterFileTypes();

	// parse command line
	//CCommandLineInfo cmdInfo;
	//ParseCommandLine(cmdInfo);

	// do not create new file
	if( cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew )
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;

	// distribute command line commands
	CString	strProjectExtension;
	m_pProjectDocTemplate->GetProjectString( strProjectExtension, CProjectTemplate::filterExt );

	// 1. Is it a DDE file?
	if( cmdInfo.m_nShellCommand == CCommandLineInfo::FileDDE )
	{
		g_configuration.m_strLastProject.Empty();
		if( !ProcessShellCommand( cmdInfo ) )
			return FALSE;
	}
	// 2. ... have we got a DDE-command on the command line?
	else if (!cmdInfo.m_strDdeCommand.IsEmpty())
	{
		// forward command to DDE-processor
		CDdeCommand::SendCommand(_T("TEXCNTR"), cmdInfo.m_strDdeCommand, _T("System"));
		//OnDDECommand(const_cast<LPTSTR>((LPCTSTR)cmdInfo.m_strDdeCommand));
	}
	// 3. ...are we to open a file and it is a project type?
	else if( (cmdInfo.m_nShellCommand == CCommandLineInfo::FileOpen) && 
		(cmdInfo.m_strFileName.Right( 4 ) == strProjectExtension) )
	{
		g_configuration.m_strLastProject.Empty();
		OpenProject( cmdInfo.m_strFileName );
	}
	// 4. ...are we to open a file and it is NOT a project type?
	else if( (cmdInfo.m_nShellCommand == CCommandLineInfo::FileOpen) && 
		(cmdInfo.m_strFileName.Right( 4 ) != strProjectExtension) )
	{
		g_configuration.m_strLastProject.Empty(); // Prevent a project from opening
		if( !ProcessShellCommand( cmdInfo ) )
			return FALSE;
	}
	// 5. Finally, do the default handling of files.
	else if( !ProcessShellCommand(cmdInfo) )
		return FALSE;

	// show main frame
	pMainFrame->ShowWindow( m_nCmdShow );
	pMainFrame->UpdateWindow();

	// hide the splash, if still there
	if (CSplashWnd::c_pSplashWndPublic)
	{
		CSplashWnd::c_pSplashWndPublic->HideSplashScreen();
	}

	//
	UpdateLatexProfileSel();

	// load last project of last session
	if( g_configuration.m_bLoadLastProject && !g_configuration.m_strLastProject.IsEmpty() )
	{
		OpenProject(g_configuration.m_strLastProject);
	}

	// update frame window
	m_pMainWnd->SendMessage( WM_COMMAND, ID_OPTIONS_CHANGED );

	// Change extension for help file
  CString strHelpFilePath = m_pszHelpFilePath;
  strHelpFilePath.Replace(".HLP", ".chm");
	CString	strHelpFile = CPathTool::GetFile(strHelpFilePath);
	CString	strHelpDir = CPathTool::Cat(CPathTool::GetDirectory(strHelpFilePath), _T("Help"));
	strHelpFilePath = CPathTool::Cat(strHelpDir, strHelpFile);
  free((void*)m_pszHelpFilePath);
  m_pszHelpFilePath = _tcsdup(strHelpFilePath);

	// Show Tip
	ShowTipAtStartup();

	// start output type wizard, if no output types are existing
	if (g_ProfileMap.GetActiveProfileKey().IsEmpty())
	{
		COutputWizard	dlg(g_ProfileMap, m_pMainWnd);
		dlg.DoModal();
		UpdateLatexProfileSel();
	}

	return TRUE;
}


void CTeXnicCenterApp::PreLoadState()
{
	GetContextMenuManager()->AddMenu(STE_POPUP_EDITOR, IDR_POPUP_EDITOR);
	GetContextMenuManager()->AddMenu(STE_POPUP_ITEM, IDR_POPUP_ITEM);
	GetContextMenuManager()->AddMenu(STE_POPUP_MAINWINDOWAREA, IDR_POPUP_MDICLIENT);
}


void CTeXnicCenterApp::LoadCustomState ()
{
	BOOL bControlBarCaption = GetInt(_T("ControlBarCaption"));
	BOOL bGradientCaption = bControlBarCaption ? GetInt(_T("GradientCaption")) : FALSE;
	m_bTabFlatBorders = GetInt(_T("TabFlatBorders"));
	m_bMDITabs = GetInt(_T("MDITabs"), TRUE);
	m_bMDITabIcons = GetInt(_T("MDITabIcons"), TRUE);
	m_nMDITabLocation = GetInt(_T("MDITabLocation"), CBCGTabWnd::LOCATION_BOTTOM);
	m_recentProjectList.ReadList();
	g_ProfileMap.SerializeFromRegistry();

	CBCGSizingControlBar::SetCaptionStyle (bControlBarCaption, bGradientCaption);
}


void CTeXnicCenterApp::SaveCustomState ()
{
	// BCG specific stuff
	WriteInt(_T("ControlBarCaption"), CBCGSizingControlBar::IsDrawCaption());
	WriteInt(_T("GradientCaption"), CBCGSizingControlBar::IsCaptionGradient());
	WriteInt(_T("TabFlatBorders"), m_bTabFlatBorders);
	WriteInt(_T("MDITabs"), m_bMDITabs);
	WriteInt(_T("MDITabIcons"), m_bMDITabIcons);
	WriteInt(_T("MDITabLocation"), m_nMDITabLocation);
	m_recentProjectList.WriteList();
	g_ProfileMap.SerializeToRegistry();

	// store global configuration
	g_configuration.Serialize( CConfiguration::Save );

	// speller
	if ( m_pSpell && m_pSpell->ismodified() && g_configuration.m_strSpellPersonalDictionary.Compare(_T("")))
		m_pSpell->save_personal_dictionary( g_configuration.m_strSpellPersonalDictionary );
}


void CTeXnicCenterApp::EndSession()
{
	///////////////////////////////////////////////////////////////////
	// disable file change notifier
	//m_FileChangeNotifier.Destroy();

	///////////////////////////////////////////////////////////////////
	// close project
	theApp.m_bEndSession = TRUE;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_PROJECT_CLOSE);
}


int CTeXnicCenterApp::ExitInstance() 
{
	// Shutdown background thread
	if ( m_pBackgroundThread )
	{
		m_pBackgroundThread->m_bAutoDelete = FALSE;
		m_pBackgroundThread->PostThreadMessage( WM_QUIT, 0, 0 );
		DWORD dwError;
		do {
			// Wait for the thread to terminate
			Sleep(0);
			::GetExitCodeThread( m_pBackgroundThread->m_hThread, &dwError );
		} while ( dwError == STILL_ACTIVE );

		delete m_pBackgroundThread;
		m_pBackgroundThread = NULL;
	}

	if( m_pMDIFrameManager )
		delete m_pMDIFrameManager;

	DeleteCriticalSection( &m_csLazy );
	delete m_pSpell;

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// unloading localized resource DLL for BCGCB
	if (m_hInstBCGCBRes)
		FreeLibrary(m_hInstBCGCBRes);

	BCGCBCleanUp();

	return CProjectSupportingWinApp::ExitInstance();
}


/*
void CTeXnicCenterApp::ForwardCommands( HCONV hconvServer )
{
	//
	CTCCommandLineInfo	cmdInfo;
	CString							strCmd;
	ParseCommandLine( cmdInfo );

	switch( cmdInfo.m_nShellCommand )
	{
		case CCommandLineInfo::FileOpen:
			if( cmdInfo.m_nDocumentLine > -1 )
				strCmd.Format( _T("[goto(\"%s\",\"%d\")]"), cmdInfo.m_strFileName, cmdInfo.m_nDocumentLine );
			else
				strCmd.Format( _T("[open(\"%s\")]"), cmdInfo.m_strFileName );

			SendDdeCommand( strCmd );
			break;
	}
}
*/


void CTeXnicCenterApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


CLatexEdit *CTeXnicCenterApp::GetActiveEditView()
{
	// get active frame
	if (!m_pMainWnd)
		return NULL;

	CFrameWnd	*pFrame = ((CMDIFrameWnd*)m_pMainWnd)->GetActiveFrame();
	if( !pFrame || !pFrame->IsKindOf( RUNTIME_CLASS(CChildFrame) ) )
		return NULL;

	// get active view
	CView	*pView = pFrame->GetActiveView();
	if( !pView || !pView->IsKindOf( RUNTIME_CLASS(CLatexEdit) ) )
		return NULL;

	return (CLatexEdit*)pView;
}


CDocument *CTeXnicCenterApp::GetOpenLatexDocument(LPCTSTR lpszFileName, BOOL bReadOnly /*= FALSE*/)
{
	// get the full path name of the file
	TCHAR		lpszFilePath[_MAX_PATH];
	LPSTR		lpszDummy;

	GetFullPathName( lpszFileName, _MAX_PATH, lpszFilePath, &lpszDummy );
	CString		strDocPath = lpszFilePath;

	// get the document template
	CDocTemplate	*pDocTemplate = m_pLatexDocTemplate;

	// try to find a document that represents the requested file
	CDocument	*pDoc = NULL;
	POSITION	pos = pDocTemplate->GetFirstDocPosition();
	BOOL		bFound = FALSE;

	while( pos )
	{
		pDoc = (CLatexDoc*)pDocTemplate->GetNextDoc( pos );
		if( pDoc && pDoc->GetPathName().CompareNoCase(strDocPath)==0 && pDoc->IsKindOf(RUNTIME_CLASS(CLatexDoc)) )
		{
			// check if the document has the required read/write mode
			if ((bReadOnly && !((CLatexDoc*)pDoc)->m_pTextBuffer->GetReadOnly()) || 
				  (!bReadOnly && ((CLatexDoc*)pDoc)->m_pTextBuffer->GetReadOnly()))
				continue;

			bFound = TRUE;
			break;
		}
	}

	if( !bFound  || !pDoc )
		return NULL;

	// set write protection
	if (pDoc && bReadOnly)
		((CLatexDoc*)pDoc)->m_pTextBuffer->SetReadOnly();

	return pDoc;
}


CDocument* CTeXnicCenterApp::GetLatexDocument(LPCTSTR lpszFileName, BOOL bReadOnly /*= FALSE*/)
{

	// get the full path name of the file
	TCHAR		lpszFilePath[_MAX_PATH];
	LPSTR		lpszDummy;

	GetFullPathName( lpszFileName, _MAX_PATH, lpszFilePath, &lpszDummy );
	CString		strDocPath = lpszFilePath;

	// get the document template
	CDocTemplate	*pDocTemplate = m_pLatexDocTemplate;

	// try to find a document that represents the requested file
	CDocument	*pDoc;
	POSITION	pos = pDocTemplate->GetFirstDocPosition();
	BOOL			bFound = FALSE;

	while( pos )
	{
		pDoc = (CLatexDoc*)pDocTemplate->GetNextDoc( pos );
		if( pDoc && pDoc->GetPathName().CompareNoCase(strDocPath)==0 && pDoc->IsKindOf(RUNTIME_CLASS(CLatexDoc)) )
		{
			// check if the document has the required read/write mode
			if ((bReadOnly && !((CLatexDoc*)pDoc)->m_pTextBuffer->GetReadOnly()) || 
				  (!bReadOnly && ((CLatexDoc*)pDoc)->m_pTextBuffer->GetReadOnly()))
				continue;

			bFound = TRUE;
			break;
		}
	}

	// create new document, if we found no one
	if( !bFound )
	{
		pDoc = pDocTemplate->CreateNewDocument();
		if (!pDoc)
			return NULL;

		// open document
		if (!pDoc->IsKindOf(RUNTIME_CLASS(CLatexDoc)) || !pDoc->OnOpenDocument(strDocPath))
		{
			pDocTemplate->RemoveDocument(pDoc);
			delete pDoc;
			return NULL;
		}
		pDoc->SetPathName(strDocPath);
		if (bReadOnly)
			((CLatexDoc*)pDoc)->m_pTextBuffer->SetReadOnly();
	}

	if( !pDoc )
		return NULL;

	// set write protection
	if (pDoc && bReadOnly)
		((CLatexDoc*)pDoc)->m_pTextBuffer->SetReadOnly();

	return pDoc;
}


CDocument* CTeXnicCenterApp::OpenDocumentFile(LPCTSTR lpszFileName)
{
	return OpenLatexDocument(lpszFileName, FALSE, -1, FALSE, true);
};


CDocument* CTeXnicCenterApp::OpenLatexDocument(LPCTSTR lpszFileName, BOOL bReadOnly /*= FALSE*/,
											   int nLineNumber /*= -1*/, BOOL bError /*= FALSE*/,
											   bool bAskForProjectLoad /*= true*/)
{
	// get the full path name of the file
	TCHAR		lpszFilePath[_MAX_PATH];
	LPSTR		lpszDummy;

	GetFullPathName(lpszFileName, _MAX_PATH, lpszFilePath, &lpszDummy);
	CString		strDocPath = lpszFilePath;

	// get the document template
	CDocTemplate	*pDocTemplate = m_pLatexDocTemplate;

	// try to find a document that represents the requested file
	CDocument	*pDoc = NULL;
	POSITION	pos = pDocTemplate->GetFirstDocPosition();
	BOOL			bFound = FALSE;

	while( pos )
	{
		pDoc = pDocTemplate->GetNextDoc( pos );
		if (pDoc && pDoc->GetPathName().CompareNoCase(strDocPath)==0 && pDoc->IsKindOf(RUNTIME_CLASS(CLatexDoc)))
		{
			// check if the document has the required read/write mode
			if ((bReadOnly && !((CLatexDoc*)pDoc)->m_pTextBuffer->GetReadOnly()) || 
				  (!bReadOnly && ((CLatexDoc*)pDoc)->m_pTextBuffer->GetReadOnly()))
				continue;

			bFound = TRUE;
			break;
		}
	}

	//Open new document, if we could not find one
	// If we found one above, we just activate its view
	if (!bFound)
	{
		bool bLoaded = false;
		//Check for existing project, that may be the project of this file.
		if (bAskForProjectLoad)
		{
			CString strProjectPath = CLatexProject::GetProjectFileNameFromDoc(strDocPath);

			//Do we have this project already opened?
			bool bOpened = false;
			CLatexProject* pLProject = GetProject();
			if (pLProject)
			{
				//We just care, if the existing project file is different
				if (pLProject->GetPathName() == strProjectPath)
					bOpened = true;
			}

			if (!bOpened && CLatexProject::CheckExistingProjectFile(strProjectPath))
			{
				//Yes, load the project
				// ==> But close all other documents before
				// skipping annoying question in this version --
				// just closing the documents.
				// lets see, what the users will say ...
				// save modified documents
				if (pDocTemplate->SaveAllModified())
				{
					pDocTemplate->CloseAllDocuments(m_bEndSession);

					if (OpenProject(strProjectPath))
					{
						//... and the specified file (in most cases the main file)
						pDoc = OpenLatexDocument(lpszFileName, bReadOnly, nLineNumber, bError, false);
						bLoaded = true;
					}
				}
			}
		}

		//Just load the file, if a project was not loaded
		if (!bLoaded)
			pDoc = pDocTemplate->OpenDocumentFile(strDocPath);
	}

	if( !pDoc )
		return NULL;

	// set write protection
	if( pDoc && bReadOnly && !((CLatexDoc*)pDoc)->m_pTextBuffer->GetReadOnly() )
		((CLatexDoc*)pDoc)->m_pTextBuffer->SetReadOnly();

	// set error mark
	if( bError )
		((CLatexDoc*)pDoc)->SetErrorMark( nLineNumber );

	// activate view and go to specified line
	POSITION	viewpos = pDoc->GetFirstViewPosition();
	CView			*pView = pDoc->GetNextView( viewpos );
	if( !pView )
		return pDoc;

	CFrameWnd	*pFrame = pView->GetParentFrame();
	if( !pFrame )
		return pDoc;

	pFrame->ActivateFrame();
	pView = pFrame->GetActiveView();
	if( !pView )
		return pDoc;

	if( !pView->IsKindOf( RUNTIME_CLASS( CLatexEdit ) ) )
		return pDoc;

	if( nLineNumber >= 0 )
	{
		if (nLineNumber > 0)
			nLineNumber--;

		((CLatexEdit*)pView)->GoToLine(nLineNumber);
	}

	// give input focus to view
	pView->SetFocus();
	//((CMainFrame*)m_pMainWnd)->SetActiveView( pView );
	
	return pDoc;
}


CString CTeXnicCenterApp::GetProjectString( CProjectTemplate::ProjectStringIndex index )
{
	CString	strResult;
	m_pProjectDocTemplate->GetProjectString( strResult, index );
	return strResult;
}


CString CTeXnicCenterApp::GetProjectFileFilter()
{
	return GetProjectString( CProjectTemplate::filterName )
		+ _T("|*")
		+ GetProjectString( CProjectTemplate::filterExt )
		+ _T('|') 
		+ AfxLoadString( STE_ALL_FILES_FILTER );
}


CString CTeXnicCenterApp::GetLatexString( CDocTemplate::DocStringIndex index )
{
	CString strResult;
	m_pLatexDocTemplate->GetDocString( strResult, index );
	return strResult;
}


CString CTeXnicCenterApp::GetLatexFileFilter()
{
	return 
		GetLatexString( CDocTemplate::filterName )
		+ _T("|*")
		+ GetLatexString( CDocTemplate::filterExt )
		+ _T('|') 
		+ AfxLoadString( STE_ALL_FILES_FILTER );
}

void CTeXnicCenterApp::SaveAllModifiedWithoutPrompt()
{
	m_bSavingAll = TRUE;

	// save all projects
	POSITION	pos = m_pProjectDocTemplate->GetFirstProjectPosition();
	while( pos )
	{
		CProject	*pDoc = m_pProjectDocTemplate->GetNextProject( pos );
		if( pDoc->GetPathName().IsEmpty() )
			pDoc->DoFileSave();
		else
			pDoc->OnSaveProject( pDoc->GetPathName() );
	}

	// save all latex-docs
	pos = m_pLatexDocTemplate->GetFirstDocPosition();
	while( pos )
	{
		CDocument	*pDoc = m_pLatexDocTemplate->GetNextDoc( pos );
		if (!pDoc->GetPathName().IsEmpty() && pDoc->IsModified())
			pDoc->OnSaveDocument( pDoc->GetPathName() );
	}

	m_bSavingAll = FALSE;
}


BOOL CTeXnicCenterApp::OpenProject( LPCTSTR lpszPath )
{
	//Close the current project
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_PROJECT_CLOSE);

	CProject* pDoc = m_pProjectDocTemplate->OpenProjectFile(lpszPath);
	// test for success
	if( !pDoc )
	{
		AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_PROJECT_CLOSE);
		return FALSE;
	}

	// add to recent project list
	m_recentProjectList.Add( pDoc->GetPathName() );

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CTeXnicCenterApp-Nachrichtenbehandlungsroutinen

void CTeXnicCenterApp::ShowTipAtStartup(void)
{
	CTCCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	if (
		(cmdInfo.m_nShellCommand == CCommandLineInfo::FileNothing || cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew) 
		&& cmdInfo.m_bShowSplash && !cmdInfo.m_bRunAutomated && !cmdInfo.m_bRunEmbedded)
	{
		CTipDlg dlg;
		if (dlg.m_bStartup)
			dlg.DoModal();
	}
}


void CTeXnicCenterApp::ShowTipOfTheDay(void)
{
	CTipDlg dlg;
	dlg.DoModal();
}


void CTeXnicCenterApp::OnTip() 
{
	ShowTipOfTheDay();	
}


void CTeXnicCenterApp::OnFileNew() 
{
	// get current project's directory
	CString				strDefaultDir;
	CLatexProject	*pProject = GetProject();
	if (pProject)
		strDefaultDir = CPathTool::GetDirectory(pProject->GetMainPath());
	else
		strDefaultDir = g_configuration.m_strDefaultPath;

	// create file from template
	CDocumentNewDialog	dlg(AfxGetMainWnd());

	for (int i = 0; i < g_configuration.m_astrDocumentTemplatePaths.GetSize(); i++)
		dlg.AddSearchDir(g_configuration.m_astrDocumentTemplatePaths[i]);

	dlg.AddTemplateFilter(_T("*.tex"), RUNTIME_CLASS(CFileBasedDocumentTemplateItem));
	dlg.AddTemplateFilter(_T("*.dll"), RUNTIME_CLASS(CWizardBasedDocumentTemplateItem));

	dlg.DoModal();
}


void CTeXnicCenterApp::OnFileSaveAll() 
{
	// save all
	SaveAllModifiedWithoutPrompt();

	// reparse project
	m_pMainWnd->SendMessage( WM_COMMAND, ID_PROJECT_PARSE );
}

void CTeXnicCenterApp::OnDisableStdCmd(CCmdUI* pCmdUI) 
{
	// disables standard document commands for project document
	pCmdUI->Enable( (BOOL)m_pLatexDocTemplate->GetFirstDocPosition() );	
}


void CTeXnicCenterApp::OnFileOpen() 
{
	CFileDialogEx	dlg( 
		TRUE, 
		GetLatexString( CDocTemplate::filterExt ), NULL,  
		OFN_FILEMUSTEXIST, CString( (LPCTSTR)STE_FILE_LATEXFILTER ) );

	//Get default path
	CString strInitialDir = AfxGetDefaultDirectory();
	dlg.m_ofn.lpstrInitialDir = strInitialDir;

	if( dlg.DoModal() != IDOK )
	{
		AfxSetLastDirectory( CPathTool::GetDirectory(dlg.GetPathName()) );
		return;
	}

	AfxSetLastDirectory( CPathTool::GetDirectory(dlg.GetPathName()) );
	// open file
	OpenLatexDocument(dlg.GetPathName(), dlg.GetReadOnlyPref(), -1, false, true);
}


void CTeXnicCenterApp::OnProjectOpen() 
{
	// display file dialog
	CFileDialogEx dialog(TRUE, NULL, NULL,
						OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, 
						GetProjectFileFilter(), AfxGetMainWnd());
	
	//Get default path
	CString strInitialDir = AfxGetDefaultDirectory();
	dialog.m_ofn.lpstrInitialDir = strInitialDir;

	if( dialog.DoModal() == IDCANCEL )
	{
		AfxSetLastDirectory( CPathTool::GetDirectory(dialog.GetPathName()) );
		return;
	}

	AfxSetLastDirectory( CPathTool::GetDirectory(dialog.GetPathName()) );
	// open project file
	OpenProject(dialog.GetPathName());
}

/*
void CTeXnicCenterApp::OnProjectClose() 
{
	// close latex-documents?

	// skipping annoying question in this version --
	// just closing the documents.
	// lets see, what the users will say ...
	if (!m_pLatexDocTemplate->SaveAllModified())
		return;

	// close project
	POSITION	pos = m_pProjectDocTemplate->GetFirstProjectPosition();
	if( !(BOOL)pos )
		return;

	// close document
	CProject	*pDoc = m_pProjectDocTemplate->GetNextProject( pos );
	if( !pDoc )
		return;

	if( !pDoc->SaveModified() )
		return;

	// remeber last project
	if( m_bEndSession )
		g_configuration.m_strLastProject = pDoc->GetPathName();
	else
		g_configuration.m_strLastProject.Empty();

	pDoc->OnCloseProject();
	m_pProjectDocTemplate->RemoveProject( pDoc );
	delete pDoc;

	// lets close all the documents
	m_pLatexDocTemplate->CloseAllDocuments(FALSE);
}
*/


void CTeXnicCenterApp::OnUpdateProjectClose(CCmdUI* pCmdUI) 
{
	// enabled, if there is an open document
	pCmdUI->Enable( (BOOL)m_pProjectDocTemplate->GetFirstProjectPosition() );	
}


void CTeXnicCenterApp::OnProjectSave() 
{
	CLatexProject* pProject = GetProject();
	if (pProject)
		pProject->DoProjectSave();
}


void CTeXnicCenterApp::OnUpdateProjectSave(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetProject()	? TRUE : FALSE);
}


void CTeXnicCenterApp::OnLatexNew() 
{
	CDocument	*pDoc = m_pLatexDocTemplate->OpenDocumentFile( NULL );
	if (pDoc && g_configuration.m_bSaveNewDocuments)
		pDoc->DoFileSave();
}


void CTeXnicCenterApp::OnExtrasOptions() 
{
	COptionDialog		dlg;

	if( dlg.DoModal() != IDOK )
		return;

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// send information about change
	m_pMainWnd->SendMessage( WM_COMMAND, ID_OPTIONS_CHANGED );

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// write options to registry
	g_configuration.Serialize(CConfiguration::Save);
}


void CTeXnicCenterApp::OnLatexEditProfiles() 
{
	CProfileDialog	dlg;

	if (dlg.DoModal() != IDOK)
		return;

	g_ProfileMap.SerializeToRegistry();
	UpdateLatexProfileSel();
}


void CTeXnicCenterApp::OnLatexProfile() 
{
	CActiveProfileDialog	dlg( m_pMainWnd );
	dlg.DoModal();
	UpdateLatexProfileSel();
}


void CTeXnicCenterApp::OnLatexProfileSel() 
{
	CMainFrame	*pMainFrame = (CMainFrame*)m_pMainWnd;

	if (!m_pMainWnd || !IsWindow(m_pMainWnd->m_hWnd))
		return;

	// get currently selected outputtype
	POSITION									pos = NULL;
	CBCGToolbarComboBoxButton	*pButton = 
		(CBCGToolbarComboBoxButton*)pMainFrame->GetToolbarButton(ID_LATEX_PROFILE_SEL, pos);
	if (!pButton)
		return;

	CComboBox	*pBox = pButton->GetComboBox();
	if (!pBox || !IsWindow(pBox->m_hWnd))
		return;

	CString	strActiveProfile;
	pBox->GetLBText(pButton->GetCurSel(), strActiveProfile);
	g_ProfileMap.SetActiveProfile(strActiveProfile);
}


void CTeXnicCenterApp::UpdateLatexProfileSel() 
{
	// get selection button
	CMainFrame	*pMainFrame = (CMainFrame*)m_pMainWnd;

	if( !m_pMainWnd || !IsWindow( m_pMainWnd->m_hWnd ) )
		return;

	// get available profiles and active profile
	CString				strActiveProfile = g_ProfileMap.GetActiveProfileKey();
	CStringArray	astrProfiles;
	g_ProfileMap.GetKeyList(astrProfiles);

	// update all instances of this button
	POSITION									pos = NULL;
	CBCGToolbarComboBoxButton	*pButton = NULL;

	while( pButton =
		(CBCGToolbarComboBoxButton*)pMainFrame->GetToolbarButton( ID_LATEX_PROFILE_SEL, pos ) )
	{
		// refill Button
		pButton->RemoveAllItems();

		for (int i = 0; i < astrProfiles.GetSize(); i++)
			pButton->AddItem(astrProfiles[i]);

		// select item
		CComboBox	*pComboBox = pButton->GetComboBox();
		if (!pComboBox || !IsWindow(pComboBox->m_hWnd) || pComboBox->GetDroppedState())
			continue;

		// we have to do it manually, because the pButton->SelectItem() does not work correctly
		int	nIndex = pComboBox->FindStringExact(-1, strActiveProfile);
		pButton->SelectItem(nIndex);
		pComboBox->SetCurSel(nIndex);
		pButton->NotifyCommand(CBN_SELENDOK);
		pComboBox->GetParent()->UpdateWindow();
	}
}


void CTeXnicCenterApp::OnDocumentSaved() 
{
	if( m_bSavingAll )
		return;

	// reparse project
	m_pMainWnd->SendMessage( WM_COMMAND, ID_PROJECT_PARSE );
}


BOOL CTeXnicCenterApp::PreTranslateMessage(MSG* pMsg)
{
	if (CSplashWnd::PreTranslateAppMessage(pMsg))
		return TRUE;

	return CProjectSupportingWinApp::PreTranslateMessage(pMsg);
}

BOOL CTeXnicCenterApp::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if( m_pMDIFrameManager && 
		m_pMDIFrameManager->OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) )
		return TRUE;
	
	return CProjectSupportingWinApp::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void CTeXnicCenterApp::OnUpdateFileMRUProjectList( CCmdUI *pCmdUI )
{
	if( !m_recentProjectList.GetSize() || m_recentProjectList[0].IsEmpty() )
	{
		pCmdUI->SetText( AfxLoadString( STE_FILE_MRU_PROJECT_NONE ) );
		pCmdUI->Enable( FALSE );
		return;
	}

	// update message from menu bar?
	if( !pCmdUI->m_pOther || !pCmdUI->m_pOther->IsKindOf( RUNTIME_CLASS(CBCGPopupMenuBar) ) )
		return;

	// insert all entries of recent project list
	CBCGPopupMenuBar	*pMenu = (CBCGPopupMenuBar*)pCmdUI->m_pOther;
	BOOL							bChange = FALSE;

	for( int i = 0; i < m_recentProjectList.GetSize() && i < 4; i++ )
	{
		// get project path
		CString	strDisplayName;
		CString	strCurrentDir;

		GetCurrentDirectory( _MAX_PATH, strCurrentDir.GetBuffer( _MAX_PATH ) );
		strCurrentDir.ReleaseBuffer();

		if( !m_recentProjectList.GetDisplayName( 
			strDisplayName,
			i,
			strCurrentDir, strCurrentDir.GetLength() ) )
			break;

		// add number for the first ten projects
		if( i < 10 )
		{
			CString	strFormat;
			if( i == 9 )
				strFormat.Format( _T("1&0 %s"), strDisplayName );
			else
				strFormat.Format( _T("&%d %s"), i + 1, strDisplayName );
			strDisplayName = strFormat;
		}

		if( pMenu->GetCount() > i && pMenu->GetButtonText( i ) != strDisplayName )
		{
			pMenu->SetButtonText( i, strDisplayName );
			bChange = TRUE;
		}
		else if( pMenu->GetCount() <= i )
		{
			int	nIndex = pMenu->InsertButton( CBCGToolbarMenuButton( ID_FILE_MRU_PROJECT_FIRST + i, NULL, -1, strDisplayName ), i );
			ASSERT( nIndex > -1 );

			bChange = TRUE;
		}
	}

	// repaint menu
	if( bChange )
		pMenu->AdjustLayout();
}


void CTeXnicCenterApp::OnUpdateFileMRU( CCmdUI* pCmdUI ) 
{
	// We are handling the MRU-List on our own
	OnUpdateFileMRUFileList( pCmdUI );
}


void CTeXnicCenterApp::OnUpdateFileMRUFileList( CCmdUI *pCmdUI )
{
	if( !m_pRecentFileList || !m_pRecentFileList->GetSize() || (*m_pRecentFileList)[0].IsEmpty() )
	{
		pCmdUI->SetText( AfxLoadString( STE_FILE_MRU_FILE_NONE ) );
		pCmdUI->Enable( FALSE );
		return;
	}

	// update message from menu bar?
	if( !pCmdUI->m_pOther || !pCmdUI->m_pOther->IsKindOf( RUNTIME_CLASS(CBCGPopupMenuBar) ) )
		return;

	// insert all entries of recent file list
	CBCGPopupMenuBar	*pMenu = (CBCGPopupMenuBar*)pCmdUI->m_pOther;
	BOOL							bChange = FALSE;

	for( int i = 0; i < m_pRecentFileList->GetSize() && i < 4; i++ )
	{
		// get project path
		CString	strDisplayName;
		CString	strCurrentDir;

		GetCurrentDirectory( _MAX_PATH, strCurrentDir.GetBuffer( _MAX_PATH ) );
		strCurrentDir.ReleaseBuffer();

		if( !m_pRecentFileList->GetDisplayName( 
			strDisplayName,
			i,
			strCurrentDir, strCurrentDir.GetLength() ) )
			break;

		// add number for the first ten files
		if( i < 10 )
		{
			CString	strFormat;
			if( i == 9 )
				strFormat.Format( _T("1&0 %s"), strDisplayName );
			else
				strFormat.Format( _T("&%d %s"), i + 1, strDisplayName );
			strDisplayName = strFormat;
		}

		if( pMenu->GetCount() > i && pMenu->GetButtonText( i ) != strDisplayName )
		{
			pMenu->SetButtonText( i, strDisplayName );
			bChange = TRUE;
		}
		else if( pMenu->GetCount() <= i )
		{
			int	nIndex = pMenu->InsertButton( CBCGToolbarMenuButton( ID_FILE_MRU_FILE1 + i, NULL, -1, strDisplayName ), i );
			ASSERT( nIndex > -1 );

			bChange = TRUE;
		}
	}

	// repaint menu
	if( bChange )
		pMenu->AdjustLayout();
}


void CTeXnicCenterApp::OnFileMRUProject( UINT unID )
{
	int	nIndex = unID - ID_FILE_MRU_PROJECT_FIRST;

	ASSERT( nIndex >= 0 && nIndex < m_recentProjectList.GetSize() );
	if( !OpenProject( m_recentProjectList[nIndex] ) )
		m_recentProjectList.Remove( nIndex );
}

//void CTeXnicCenterApp::OnFileMRUFile(UINT unID)
//{
//	int	nIndex = unID - ID_FILE_MRU_FILE1;
//
//	ASSERT(nIndex >= 0 && nIndex < m_pRecentFileList->GetSize());
//	if (!OpenLatexDocument((*m_pRecentFileList)[nIndex], false, -1, false, true))
//		m_pRecentFileList->Remove(nIndex);
//}


BOOL CTeXnicCenterApp::OnDDECommand(LPTSTR lpszCommand) 
{
	CString		strCommand( lpszCommand );
	CString		strFileName( lpszCommand );
	CString		strProjectExtension;

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// handle goto command => [goto("filepath", "linenumber")]
	if( strCommand.Left( 7 ) == _T("[goto(\"") )
	{
		int			nStart, nEnd;
		CString	strLineNumber;
		int			nLineNumber;

		// get file name
		nStart = strCommand.Find( _T('"') );
		nEnd = strCommand.Find( _T('"'), nStart + 1 );
		if( nStart == -1 || nEnd == -1 || nEnd - nStart <= 1 )
			return FALSE;

		strFileName = strCommand.Mid( nStart + 1, nEnd - (nStart + 1) );

		// get line number
		nStart = strCommand.Find( _T('"'), nEnd + 1 );
		nEnd = strCommand.Find( _T('"'), nStart + 1 );
		if( nStart == -1 || nEnd == -1 || nEnd - nStart <= 1 )
			return FALSE;

		strLineNumber = strCommand.Mid( nStart + 1, nEnd - (nStart + 1) );
		nLineNumber = _ttol( strLineNumber );

		if( !OpenLatexDocument(strFileName, FALSE, nLineNumber, FALSE, true) )
			return FALSE;
	}

	/*
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// handle projec specific stuff
	m_pProjectDocTemplate->GetProjectString( strProjectExtension, CProjectTemplate::filterExt );

	if( strFileName.Left( 7 ) == _T("[open(\"") )
		strFileName = strFileName.Mid( 7, strFileName.GetLength() - 7 - 3 );
	else if( strFileName.Left( 8 ) == _T("[print(\"") )
		strFileName = strFileName.Mid( 8, strFileName.GetLength() - 8 - 3 );
	else if( strFileName.Left( 10 ) == _T("[printto(\"") )
		strFileName = strFileName.Mid( 10, strFileName.GetLength() - 10 - 3 );

	// Handle only project-files here
	if( strFileName.Right( 4 ) == strProjectExtension )
	{
		// we only support opening for project-files
		if( strCommand.Left( 7 ) != _T("[open(\"") )
			return FALSE;

		// open new project-file
		OpenProject( strFileName );
	}	
	*/
	
	return CProjectSupportingWinApp::OnDDECommand(lpszCommand);
}


BOOL CTeXnicCenterApp::OpenURL( LPCTSTR lpszURL )
{
	if( ::ShellExecute( NULL, NULL, lpszURL, NULL, NULL, NULL ) < (HINSTANCE)32)
		return FALSE;

	return TRUE;
}


void CTeXnicCenterApp::OnUpdateURL( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
}


void CTeXnicCenterApp::OnURL( UINT unID )
{
	// text behind the second '\n'-character in the description of the command
	// is the full URL
	CString	strURL( (LPCTSTR)unID );
	int			nIndex = strURL.Find( _T('\n') );

	if( nIndex < 0 )
		return;

	if( (nIndex + 1) < strURL.GetLength() && (nIndex = strURL.Find( _T('\n'), nIndex + 1 )) < 0 )
		return;

	if( ++nIndex >= strURL.GetLength() )
		return;

	if( !OpenURL( strURL.Mid( nIndex, strURL.GetLength() - 1 ) ) )
		AfxMessageBox( STE_URL_CANNOTOPEN, MB_ICONSTOP | MB_OK );
}


void CTeXnicCenterApp::OnAppLicense() 
{
	OpenURL( CPathTool::Cat( GetWorkingDir(), CString( (LPCTSTR)STE_LICENSE_FILE ) ) );
}


void CTeXnicCenterApp::OnUpdateWindowCloseAll(CCmdUI* pCmdUI) 
{
	CMultiDocTemplate	*pDocTemplate = GetLatexDocTemplate();
	if (!pDocTemplate)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(pDocTemplate->GetFirstDocPosition() != NULL);
}


void CTeXnicCenterApp::OnWindowCloseAll() 
{
	CMultiDocTemplate	*pDocTemplate = GetLatexDocTemplate();
	if (!pDocTemplate)
		return;

	// save modified documents
	if (!pDocTemplate->SaveAllModified())
		return;

	pDocTemplate->CloseAllDocuments(FALSE);
}


void CTeXnicCenterApp::OnHelp() 
{
	CWinApp::OnHelp();
}


MySpell* CTeXnicCenterApp::GetSpeller()
{
	::EnterCriticalSection( &m_csLazy );
	if (m_pSpell == NULL)
	{
		CWaitCursor wait;
		CString dicName, affName;
		// Create dictionary name and path
		dicName.Format(_T("%s\\%s_%s.dic"),
			g_configuration.m_strSpellDictionaryPath, 
			g_configuration.m_strLanuage,
			g_configuration.m_strLanguageDialect);
		// Create affix name and path
		affName.Format(_T("%s\\%s_%s.aff"),
			g_configuration.m_strSpellDictionaryPath, 
			g_configuration.m_strLanuage,
			g_configuration.m_strLanguageDialect);
		try
		{
			if ( !::PathFileExists(affName) )
				throw AfxFormatString1(STE_DICTIONARY_OPEN_FAIL, affName);
			if ( !::PathFileExists(dicName) )
				throw AfxFormatString1(STE_DICTIONARY_OPEN_FAIL, dicName);

			m_pSpell = new MySpell( T2CA((LPCTSTR)affName), T2CA((LPCTSTR)dicName) );

			// Create the personal dictionary if we have a name
			if (g_configuration.m_strSpellPersonalDictionary && 
				g_configuration.m_strSpellPersonalDictionary.Compare(_T("")))
			{
				m_pSpell->set_personal_dictionary(T2CA((LPCTSTR)g_configuration.m_strSpellPersonalDictionary));

				if ( ::PathFileExists(g_configuration.m_strSpellPersonalDictionary) )
				{
					// The path exists, so let's try to open it
					if ( m_pSpell->open_personal_dictionary() != 0 )
					{
						CString str;
						str.Format(STE_FILE_INUSE, AfxLoadString(IDS_OPEN), g_configuration.m_strSpellPersonalDictionary, "");
						::MessageBox( NULL, str, NULL, MB_OK | MB_ICONINFORMATION | MB_TASKMODAL );
					}
				}
			}
		}
		catch ( CString str )
		{
			// One of the dictionary files does not exist.
			g_configuration.m_bSpellEnable = FALSE;
			::MessageBox( NULL, str, NULL, MB_OK | MB_ICONEXCLAMATION | MB_TASKMODAL );
		}
		catch ( ... )
		{
			// There was an error while creating the dictionary. This may be due 
			// to a corrupted file system or insufficient operating system privileges. 
			// Whatever the cause, it deserves a little stronger warning message.
			g_configuration.m_bSpellEnable = FALSE;
			::MessageBox( NULL, AfxFormatString1(STE_DICTIONARY_CREATE_FAIL, _T("")),
				NULL, MB_OK | MB_ICONERROR | MB_TASKMODAL );
		}
	}

	if ( m_pSpell )
		m_pSpell->suggest_main(g_configuration.m_bSpellMainDictOnly);
	::LeaveCriticalSection( &m_csLazy );
	return m_pSpell;
}


CWinThread* CTeXnicCenterApp::GetBackgroundThread()
{
	::EnterCriticalSection( &m_csLazy );
	if (m_pBackgroundThread == NULL )
	{
		m_pBackgroundThread = new CBackgroundThread();
		ASSERT( m_pBackgroundThread );
		m_pBackgroundThread->CreateThread();
		m_pBackgroundThread->SetThreadPriority(THREAD_PRIORITY_LOWEST);

		// Get the background thread to trigger the dictionary creation which 
		// may take several seconds. This allows the user to continue working
		// without interruption.
		m_pBackgroundThread->PostThreadMessage(ID_BG_ENABLE_SPELLER, g_configuration.m_bSpellEnable, NULL);
		if ( g_configuration.m_bSpellEnable )
		{
			CSpellerSource *pSource = static_cast<CSpellerSource*>(this);
			m_pBackgroundThread->PostThreadMessage(ID_BG_RESET_SPELLER, 0, (long) pSource);
		}
	}
	::LeaveCriticalSection( &m_csLazy );
	return m_pBackgroundThread;
}


void CTeXnicCenterApp::OnUpdateProject() 
{
	// Inform the background thread to update all of the active views.
	CWinThread *pBackgroundThread = GetBackgroundThread();

	// get the document template
	CDocTemplate	*pDocTemplate = m_pLatexDocTemplate;

	CDocument	*pDoc = NULL;
	POSITION	pos = pDocTemplate->GetFirstDocPosition();

	while( pos )
	{
		pDoc = pDocTemplate->GetNextDoc( pos );
		if( pDoc )
		{
			// Get the first document view
			POSITION pos = pDoc->GetFirstViewPosition();
			if ( pos )
			{
				CCrystalTextView *pView = (CCrystalTextView *)pDoc->GetNextView( pos );

				if (pView && pView->IsKindOf(RUNTIME_CLASS(CCrystalTextView)))
					pBackgroundThread->PostThreadMessage(ID_BG_UPDATE_BUFFER, 0, (long)pView);
			}
		}
	}
}


void CTeXnicCenterApp::OnProjectNewFromFile() 
{
	CLatexEdit* pEdit = GetActiveEditView();
	if (!pEdit) return;

	CLatexDoc* pDoc = pEdit->GetDocument();
	if (!pDoc) return;

	//Empty (not yet saved) path name?
	CString DocPathName = pDoc->GetPathName();
	if (DocPathName.IsEmpty())
	{
		//Ask whether to save and proceed OR to cancel the action   
		if (AfxMessageBox(STE_DOCUMENT_SAVEBEFOREPROCEED, MB_ICONINFORMATION | MB_OKCANCEL) == IDCANCEL)
			return;

		//Save it
		if (!pDoc->DoSave(DocPathName)) return;

		DocPathName = pDoc->GetPathName();
	}

	//Do we have an opened project?
	CLatexProject* pLProject = GetProject();
	if (pLProject)
	{
		//We just care, if the existing main file is the same as the new main file
		if (pLProject->GetMainPath() == DocPathName)
		{
			//It is the same ==> the user wants to recreate the current project
			// So we just show him the Properties dialog.
			// This is similar to the behaviour in OnProjectNewFromDocument
			// and saves the user from closing all documents.
			AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_PROJECT_PROPERTIES);
			return;
		}
	}

	m_pProjectManager->OnProjectNewFromDocument(DocPathName);
}

void CTeXnicCenterApp::OnUpdateProjectNewFromFile(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pLatexDocTemplate->GetFirstDocPosition() != NULL);
}

