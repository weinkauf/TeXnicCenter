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
	m_parsingFinished( FALSE, TRUE ),
	m_pwndStructureView( NULL ),
	m_pwndEnvironmentView( NULL ),
	m_pwndFileView(NULL),
	m_bUseBibTex(FALSE),
	m_bUseMakeIndex(FALSE),
	m_nInitialNavigatorTab(0)
{
	// initialization
	m_pStructureParser = new CStructureParser( this/*, this */);
	ASSERT( m_pStructureParser );
	if( !m_pStructureParser )
		AfxThrowMemoryException();

	// Initialize the control bars and main frame pointer members
	m_pwndMainFrame     = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(m_pwndMainFrame);
	m_pwndWorkspaceBar  = m_pwndMainFrame->GetWorkspaceBar();
	m_pwndOutputBar     = m_pwndMainFrame->GetOutputBar();

	// We can parse
	m_parsingFinished.SetEvent();
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
}


BOOL CLatexProject::OnNewProject()
{
	if (!CProject::OnNewProject())
		return FALSE;

	// add views to the docking bars in the frame wnd
	m_nInitialNavigatorTab = 0;
	CreateProjectViews();

	CProjectNewDialog	dlg;

	dlg.AddTemplateFilter(_T("*.tex"), RUNTIME_CLASS(CFileBasedProjectTemplateItem));
	dlg.AddTemplateFilter(_T("*.dll"), RUNTIME_CLASS(CWizardBasedProjectTemplateItem));
	for (int i = 0; i < g_configuration.m_astrProjectTemplatePaths.GetSize(); i++)
		dlg.AddSearchDir(g_configuration.m_astrProjectTemplatePaths[i]);

	if (dlg.DoModal() == IDCANCEL)
		return FALSE;

	DoFileSave();
	theApp.m_recentProjectList.Add(GetPathName());

	return TRUE;
}


#define	TC_PROJECT_SESSION_EXT	_T("tps")

BOOL CLatexProject::OnOpenProject(LPCTSTR lpszPathName) 
{
	/*
	if (!CProject::OnOpenDocument(lpszPathName))
		return FALSE;
	*/

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// load project information
	CIniFile	file( lpszPathName );
	if (!file.ReadFile())
		return FALSE;

	SetProjectDir(CPathTool::GetDirectory(lpszPathName));

	// check if ini file contains any data, if not assume, that we 
	// have to use an archive instead
	if (file.GetNumKeys() < 2)
	{
		CFile			file;
		CArchive	*pAr = NULL;

		try
		{
			if (!file.Open(lpszPathName, CFile::modeRead))
				throw FALSE;

			pAr = new CArchive(&file, CArchive::load);
			Serialize(*pAr);
			pAr->Close();
			file.Close();
			delete pAr;
		}
		catch(...)
		{
			if (pAr)
			{
				pAr->Abort();
				delete pAr;
			}

			file.Abort();
		}
	}
	else
		Serialize(file, FALSE);

	SetModifiedFlag(FALSE);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// load project session
	CIniFile	session(CPathTool::GetBase(lpszPathName) + _T('.') + TC_PROJECT_SESSION_EXT);
	if (session.ReadFile())
		SerializeSession(session, FALSE);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// add views to the docking bars in the frame wnd
	CreateProjectViews();

	//m_wndFileView.PopulateTree(m_strMainPath);
	
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// parse project
	OnProjectParse();

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
	CIniFile	session(CPathTool::GetBase(GetPathName()) + _T('.') + TC_PROJECT_SESSION_EXT);
	session.Reset();
	SerializeSession(session, TRUE);
	session.WriteFile();

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// close latex-documents?

	// skipping annoying question in this version --
	// just closing the documents.
	// lets see, what the users will say ...
	theApp.GetLatexDocTemplate()->CloseAllDocuments(theApp.m_bEndSession);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// remeber last project
	if (theApp.m_bEndSession)
		g_configuration.m_strLastProject = GetPathName();
	else
		g_configuration.m_strLastProject.Empty();



	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Clean up

	// add views to the docking bars in the frame wnd
	CMainFrame	*pwndMainFrame = (CMainFrame*)AfxGetMainWnd();

	if( !pwndMainFrame )
		return;

	// cancel structure parsing
	if( !m_pStructureParser )
		return;

	m_pStructureParser->CancelParsing();
	WaitForSingleObject( m_parsingFinished, INFINITE );

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
	ON_UPDATE_COMMAND_UI(ID_ITEM_GOTO, OnUpdateItemCmd)
	ON_UPDATE_COMMAND_UI(ID_ITEM_INSERT_PAGEREF, OnUpdateItemCmd)
	ON_UPDATE_COMMAND_UI(ID_ITEM_INSERT_REF, OnUpdateItemCmd)
	ON_COMMAND(ID_ITEM_INSERT_LABEL, OnItemInsertLabel)
	ON_UPDATE_COMMAND_UI(ID_ITEM_INSERT_LABEL, OnUpdateItemCmd)
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


#define KEY_FORMATINFO								_T("FormatInfo")
#define VAL_FORMATINFO_TYPE						_T("Type")
#define VAL_FORMATINFO_VERSION				_T("Version")

#define KEY_PROJECTINFO								_T("ProjectInfo")
#define VAL_PROJECTINFO_WORKINGDIR		_T("WorkingDir")
#define VAL_PROJECTINFO_MAINFILE			_T("MainFile")
#define VAL_PROJECTINFO_USEBIBTEX			_T("UseBibTeX")
#define VAL_PROJECTINFO_USEMAKEINDEX	_T("UseMakeIndex")

#define CURRENTFORMATVERSION				2
#define	FORMATTYPE									_T("TeXnicCenterProjectInformation")

BOOL CLatexProject::Serialize( CIniFile &ini, BOOL bWrite )
{
	if( bWrite )
	{
		// setting format information
		ini.SetValue( KEY_FORMATINFO, VAL_FORMATINFO_TYPE, FORMATTYPE );
		ini.SetValue( KEY_FORMATINFO, VAL_FORMATINFO_VERSION, CURRENTFORMATVERSION );

		// setting project information
		ini.SetValue(KEY_PROJECTINFO, VAL_PROJECTINFO_MAINFILE, CPathTool::GetRelativePath(GetProjectDir(), m_strMainPath));
		ini.SetValue(KEY_PROJECTINFO, VAL_PROJECTINFO_USEBIBTEX, (int)m_bUseBibTex);
		ini.SetValue(KEY_PROJECTINFO, VAL_PROJECTINFO_USEMAKEINDEX, (int)m_bUseMakeIndex);

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

		m_bUseBibTex = ini.GetValue(KEY_PROJECTINFO, VAL_PROJECTINFO_USEBIBTEX, FALSE);
		m_bUseMakeIndex = ini.GetValue(KEY_PROJECTINFO, VAL_PROJECTINFO_USEMAKEINDEX, FALSE);

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


#define KEY_FORMATINFO									_T("FormatInfo")
#define VAL_FORMATINFO_TYPE							_T("Type")
#define VAL_FORMATINFO_VERSION					_T("Version")

#define KEY_SESSIONINFO									_T("SessionInfo")
#define VAL_SESSIONINFO_ACTIVETAB				_T("ActiveTab")
#define VAL_SESSIONINFO_FRAMECOUNT			_T("FrameCount")

#define KEY_FRAMEINFO										_T("Frame%d")

#define CURRENTFORMATVERSION						1
#define	FORMATTYPE											_T("TeXnicCenterProjectSessionInformation")

void CLatexProject::SerializeSession(CIniFile &ini, BOOL bWrite)
{
	if (bWrite)
	{
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// storing format information
		ini.SetValue(KEY_FORMATINFO, VAL_FORMATINFO_TYPE, FORMATTYPE);
		ini.SetValue(KEY_FORMATINFO, VAL_FORMATINFO_VERSION, CURRENTFORMATVERSION);

		// store active tab of navigator
		CBCGTabWnd* pwndTabs = &m_pwndWorkspaceBar->GetTabWnd();
		if (pwndTabs && IsWindow(pwndTabs->m_hWnd))
			ini.SetValue(KEY_SESSIONINFO, VAL_SESSIONINFO_ACTIVETAB, pwndTabs->GetActiveTab());

		// storing frame information
		CWnd	*pWnd = AfxGetMainWnd()->GetWindow(GW_CHILD)->GetWindow(GW_CHILD);
		pWnd = pWnd->GetWindow(GW_HWNDLAST); // Start with the last child
		int		nFrame = 0;

		ini.SetValue(KEY_SESSIONINFO, VAL_SESSIONINFO_FRAMECOUNT, 0);

		// iterate through all MDI-childs
		while (pWnd && IsWindow(pWnd->m_hWnd))
		{
			if (pWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
			{
				// store information for this MDI-window
				CChildFrame			*pChildFrame = (CChildFrame*)pWnd;
				CString					strKey;

				strKey.Format(KEY_FRAMEINFO, nFrame);
				((CChildFrame*)pWnd)->Serialize(ini, strKey, bWrite);

				nFrame++;
			}

			// get previous window
			pWnd = pWnd->GetNextWindow(GW_HWNDPREV);
		}

		// store number of frames
		ini.SetValue(KEY_SESSIONINFO, VAL_SESSIONINFO_FRAMECOUNT, nFrame);
	}
	else
	{
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// reading format information
		CString	strKey;
		int			nFrameCount = ini.GetValue(KEY_SESSIONINFO, VAL_SESSIONINFO_FRAMECOUNT, 0);
		int			nFrame;
		CCreateContext	cc;

		ZeroMemory(&cc, sizeof(cc));
		cc.m_pNewViewClass = RUNTIME_CLASS(CLatexEdit);

		for (nFrame = 0; nFrame < nFrameCount; nFrame++)
		{
			CChildFrame	*pChildFrame = new CChildFrame();

			ASSERT(pChildFrame);
			if (!pChildFrame)
				continue;

			strKey.Format(KEY_FRAMEINFO, nFrame);
			if (!pChildFrame->Serialize(ini, strKey, bWrite))
				delete pChildFrame;
		}

		// restore navigator tab selection
		m_nInitialNavigatorTab = ini.GetValue(KEY_SESSIONINFO, VAL_SESSIONINFO_ACTIVETAB, 0);
	}
}



void CLatexProject::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_strMainPath << GetProjectDir();
	}
	else
	{
		ar >> m_strMainPath >> m_strProjectDir;
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
	//return m_strWorkingDir;
	return CPathTool::GetDirectory(m_strMainPath);
}


void CLatexProject::OnProjectProperties() 
{
	CProjectPropertyDialog	dlg( theApp.m_pMainWnd );
	dlg.m_strProjectDir = GetProjectDir();
	dlg.m_strMainFile = m_strMainPath;
	dlg.m_bUseBibTex = m_bUseBibTex;
	dlg.m_bUseMakeIndex = m_bUseMakeIndex;

	if( dlg.DoModal() != IDOK )
		return;

	if (
		m_strMainPath != dlg.m_strMainFile ||
		m_bUseBibTex != dlg.m_bUseBibTex ||
		m_bUseMakeIndex != dlg.m_bUseMakeIndex)
	{
		SetModifiedFlag();
	}

	m_strMainPath = dlg.m_strMainFile;
	m_bUseBibTex = dlg.m_bUseBibTex;
	m_bUseMakeIndex = dlg.m_bUseMakeIndex;
}


void CLatexProject::OnProjectOpenMainfile() 
{
	theApp.OpenLatexDocument( m_strMainPath, FALSE, 0 );
}

/////////////////////////////////////////////////////////////////////
// implementation of CTextSourceManager virtuals

/*CTextSource *CLatexProject::GetTextSource( LPCTSTR lpszPath )
{	
	CTextSourceFile	*pTextSource = NULL;

	pTextSource = new CTextSourceFile();
	if( pTextSource && !pTextSource->Create( lpszPath ) )
	{
		delete pTextSource;
		pTextSource = NULL;
	}

	return pTextSource;
}
*/

/////////////////////////////////////////////////////////////////////
// implementation of CStructureParserHandler virtuals

void CLatexProject::OnParsingFinished( BOOL bSuccess )
{
	m_parsingFinished.SetEvent();

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

	// change to working dir
	SetCurrentDirectory(CPathTool::GetDirectory(m_strMainPath));

	// start parsing
	m_bCanParse = FALSE;
	m_parsingFinished.ResetEvent();
	m_pStructureParser->StartParsing( m_strMainPath, GetWorkingDir() );
}


void CLatexProject::OnProjectParsed() 
{
	const CStructureItemArray *pasi = m_pStructureParser->LockStructureItems();

	// copy items to local array
	m_aStructureItems.RemoveAll();
//	for( int i = 0; i < pasi->GetSize(); i++ )
//		m_aStructureItems.Add( pasi->GetAt( i ) );
	m_aStructureItems.Copy(*pasi);

	m_pStructureParser->UnlockStructureItems();

	UpdateAllViews( NULL, hintParsingFinished, (CObject*)&m_aStructureItems );
	m_bCanParse = TRUE;
}


void CLatexProject::SetCurrentStructureItem( int nIndex )
{
	ASSERT( nIndex >= -1 && nIndex < m_aStructureItems.GetSize() );
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
	CStructureItem	&si = m_aStructureItems[m_nCurrentStructureItem];

	switch( pCmdUI->m_nID )
	{
		case ID_ITEM_GOTO:
			switch( si.m_nType )
			{
			case CStructureParser::file:
				pCmdUI->Enable( !si.m_strPath.IsEmpty() );
				break;
			default:
				pCmdUI->Enable( si.m_nLine > 0 && !si.m_strPath.IsEmpty() );
			}
			break;
		case ID_ITEM_INSERT_LABEL:
		case ID_ITEM_INSERT_REF:
		case ID_ITEM_INSERT_PAGEREF:
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

	CStructureItem	&si = m_aStructureItems[m_nCurrentStructureItem];

	switch( si.m_nType )
	{
		case CStructureParser::file:
			theApp.OpenLatexDocument( GetFilePath( si.m_strPath ), FALSE, -1 );
			break;
		default:
			theApp.OpenLatexDocument( GetFilePath( si.m_strPath ), FALSE, si.m_nLine );
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

	pView->InsertText( 
		AfxFormatString1( 
			STE_LATEX_REF, 
			m_aStructureItems[m_nCurrentStructureItem].m_strLabel ) );
	pView->SetFocus();
}

void CLatexProject::OnChangedViewList() 
{
	//UpdateFrameCounts();
	//CProject::OnChangedViewList();
}

BOOL CLatexProject::CreateProjectViews()
{
	ASSERT(m_pwndMainFrame);
	ASSERT(m_pwndWorkspaceBar);
	ASSERT(m_pwndOutputBar);

	// Check if views are allready existing
	if( m_pwndStructureView && IsWindow( m_pwndStructureView->m_hWnd ) )
		return FALSE;

	// Initialize create context
	/*
	CCreateContext	cc;
	ZeroMemory( &cc, sizeof( cc ) );
	*/

	// Create views:
	CRect						rectDummy;
	rectDummy.SetRectEmpty();
	CBCGTabWnd* pwndTabs = &m_pwndWorkspaceBar->GetTabWnd();

	m_pwndStructureView   = new CStructureView;
	m_pwndEnvironmentView = new CEnvironmentView;
	m_pwndFileView = new CFileView;

	if( !m_pwndStructureView || !m_pwndEnvironmentView || !m_pwndFileView )
	{
		if( m_pwndStructureView )
			delete m_pwndStructureView;
		if( m_pwndEnvironmentView )
			delete m_pwndEnvironmentView;
		if( m_pwndFileView )
			delete m_pwndFileView;
	}

	/*
	cc.m_pNewViewClass = RUNTIME_CLASS( CStructureView );
	if( !m_pwndStructureView->Create( NULL, NULL, AFX_WS_DEFAULT_VIEW & ~WS_BORDER, 
		rectDummy, pwndTabs, 0, &cc ) )
	*/
	if (!m_pwndStructureView->Create(pwndTabs))
	{
		TRACE0("Failed to create structure view\n");
		return FALSE;
	}
	m_pwndStructureView->SetOwner(m_pwndWorkspaceBar);

	/*
	cc.m_pNewViewClass = RUNTIME_CLASS( CEnvironmentView );
	if( !m_pwndEnvironmentView->Create( NULL, NULL, AFX_WS_DEFAULT_VIEW & ~WS_BORDER, 
		rectDummy, pwndTabs, 0, &cc ) )
	*/
	if (!m_pwndEnvironmentView->Create(pwndTabs))
	{
		TRACE0("Failed to create environment view\n");
		return FALSE;
	}
	m_pwndEnvironmentView->SetOwner(m_pwndWorkspaceBar);

	/*
	cc.m_pNewViewClass = RUNTIME_CLASS( CFileView );
	if( !m_pwndFileView->Create( NULL, NULL, AFX_WS_DEFAULT_VIEW & ~WS_BORDER, 
		rectDummy, pwndTabs, 0, &cc ) )
	*/
	if (!m_pwndFileView->Create(pwndTabs))
	{
		TRACE0("Failed to create file view\n");
		return FALSE;
	}
	m_pwndFileView->SetOwner(m_pwndWorkspaceBar);

	// Create views:
	/*
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | 
							  TVS_LINESATROOT   | TVS_HASBUTTONS |
							  TVS_SHOWSELALWAYS | TVS_EDITLABELS;
	
	if (!m_wndFileView.Create (dwViewStyle, rectDummy, pwndTabs, 1))
	{
		TRACE0("Failed to create workspace view\n");
		return FALSE;      // fail to create
	}
	*/

//	m_wndFileView.PopulateTree();

	// Attach views to tab:
	pwndTabs->AddTab( m_pwndStructureView, CString( (LPCTSTR)STE_TAB_STRUCTURE ), 0 );
	pwndTabs->AddTab( m_pwndEnvironmentView, CString( (LPCTSTR)STE_TAB_ENVIRONMENTS ), 1 );
	pwndTabs->AddTab( m_pwndFileView/*&m_wndFileView*/, CString( (LPCTSTR)STE_TAB_FILES ), 2 );

	if (m_nInitialNavigatorTab < 0 || m_nInitialNavigatorTab > 2)
		m_nInitialNavigatorTab = 0;
	pwndTabs->SetActiveTab(m_nInitialNavigatorTab);

	// Attach views to document (project)
	AddView( m_pwndStructureView );
	AddView( m_pwndEnvironmentView );
	AddView( m_pwndFileView );

	// Sorry, but we have to activate one view manually, so that the
	// project-document can receive messages
//	((CFrameWnd*)AfxGetMainWnd())->SetActiveView( m_pwndStructureView );
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

	// Detach views from tab...
	CBCGTabWnd* pwndTabs = &m_pwndWorkspaceBar->GetTabWnd();
	pwndTabs->RemoveAllTabs();

	// ... and destroy them
	delete m_pwndStructureView;
	delete m_pwndEnvironmentView;
	delete m_pwndFileView;

	// mark pointers as NULL
	m_pwndStructureView   = NULL;
	m_pwndEnvironmentView = NULL;
	m_pwndFileView        = NULL;
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
