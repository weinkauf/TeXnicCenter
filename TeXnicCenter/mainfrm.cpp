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

#include "global.h"

//#include "StructureView.h"
//#include "EnvironmentView.h"
//#include "FileView.h"
#include "BuildView.h"
#include "GrepView.h"
#include "Configuration.h"
#include "Splash.h"
#include "BCGToolbarCustomizeEx.h"
#include "UserTool.h"
#include "Latexedit.h"
#include "ProfileDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// class CMainFrame
//-------------------------------------------------------------------

IMPLEMENT_DYNAMIC(CMainFrame, CBCGMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CBCGMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_EXTRAS_CUSTOMIZE, OnExtrasCustomize)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_ACTIVATE()
	ON_COMMAND(ID_OPTIONS_CHANGED, OnOptionsChanged)
	ON_COMMAND(ID_WINDOW_EDITOR, OnWindowEditor)
	ON_COMMAND(ID_WINDOW_NAVIGATOR, OnWindowNavigator)
	ON_COMMAND(ID_WINDOW_ENVIRONMENT, OnWindowEnvironment)
	ON_COMMAND(ID_WINDOW_FILES, OnWindowFiles)
	ON_COMMAND(ID_WINDOW_STRUCTURE, OnWindowStructure)
	ON_COMMAND(ID_WINDOW_OUTPUT, OnWindowOutput)
	ON_COMMAND(ID_WINDOW_BUILDRESULT, OnWindowBuildResult)
	ON_COMMAND(ID_WINDOW_GREP1, OnWindowGrep1)
	ON_COMMAND(ID_WINDOW_GREP2, OnWindowGrep2)
	ON_COMMAND(ID_WINDOW_LIST, OnWindowList)
	ON_COMMAND(ID_HELP_FINDER, OnHelpSearch)
	ON_COMMAND(ID_HELP_INDEX, OnHelpIndex)
	ON_COMMAND(ID_VIEW_FULLSCREEN, OnViewFullScreen)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FULLSCREEN, OnUpdateViewFullScreen)
	ON_COMMAND(ID_HELP_KEYMAPPING, OnHelpKeyMapping)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_NEXT, OnUpdateWindowNext)
	ON_COMMAND(ID_WINDOW_NEXT, OnWindowNext)
	ON_COMMAND(ID_WINDOW_PREVIOUS, OnWindowPrevious)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_PREVIOUS, OnUpdateWindowPrevious)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_HELP_CONTENTS, OnHelpContents)
	ON_WM_HELPINFO()
	ON_COMMAND(ID_VIEW_DOCTAB_BOTTOM, OnViewDocTabsBottom)
	ON_COMMAND(ID_VIEW_DOCTAB_OFF, OnViewDocTabsOff)
	ON_COMMAND(ID_VIEW_DOCTAB_TOP, OnViewDocTabsTop)
	ON_COMMAND(ID_VIEW_DOCTAB_ICONS, OnViewDocTabsIcons)
	ON_COMMAND(ID_VIEW_DOCTAB_NOTE, OnViewDocTabsNote)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DOCTAB_BOTTOM, OnUpdateViewDocTabs)
	ON_COMMAND(ID_TOOLS_CANCEL, OnToolsCancel)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	ON_COMMAND(ID_WINDOW_REFERENCES, OnWindowReferences)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DOCTAB_OFF, OnUpdateViewDocTabs)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DOCTAB_TOP, OnUpdateViewDocTabs)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DOCTAB_ICONS, OnUpdateViewDocTabs)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DOCTAB_NOTE, OnUpdateViewDocTabs)
	ON_COMMAND(ID_WINDOW_PARSE, OnWindowParse)
	//}}AFX_MSG_MAP
	// Globale Hilfebefehle
	ON_COMMAND(ID_HELP_FINDER, CBCGMDIFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, CBCGMDIFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CBCGMDIFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CBCGMDIFrameWnd::OnHelpFinder)
	ON_COMMAND_RANGE(ID_MATH1, ID_MATH16, OnToggleMathBar)
	ON_COMMAND_RANGE(ID_VIEW_USER_TOOLBAR_FIRST, ID_VIEW_USER_TOOLBAR_LAST, OnToggleCtrlBar)
	ON_COMMAND_EX(ID_VIEW_MATH, OnToggleCtrlBar)
	ON_COMMAND_EX(ID_VIEW_NAVIGATION, OnToggleCtrlBar)
	ON_COMMAND_EX(ID_VIEW_OUTPUT, OnToggleCtrlBar)
	ON_COMMAND_EX(ID_VIEW_TOOLBAR, OnToggleCtrlBar)
	ON_COMMAND_EX(ID_VIEW_FORMAT, OnToggleCtrlBar)
	ON_COMMAND_EX(ID_VIEW_FORMAT_RECENTLY_USED, OnToggleCtrlBar)
	ON_COMMAND_EX(ID_VIEW_INSERT, OnToggleCtrlBar)
	ON_COMMAND_EX(ID_VIEW_LATEX, OnToggleCtrlBar)
	ON_COMMAND_EX(ID_VIEW_STATUS_BAR, OnToggleCtrlBar)
	ON_COMMAND_EX(ID_VIEW_MENU_BAR, OnToggleCtrlBar)
	ON_COMMAND_EX(ID_VIEW_FIND, OnToggleCtrlBar)
	ON_UPDATE_COMMAND_UI_RANGE(ID_MATH1, ID_MATH16, OnCheckCtrlBarVisible)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_USER_TOOLBAR_FIRST, ID_VIEW_USER_TOOLBAR_LAST, OnCheckCtrlBarVisible)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MATH, OnCheckCtrlBarVisible)
	ON_UPDATE_COMMAND_UI(ID_VIEW_NAVIGATION, OnCheckCtrlBarVisible)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTPUT, OnCheckCtrlBarVisible)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR, OnCheckCtrlBarVisible)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FORMAT, OnCheckCtrlBarVisible)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FORMAT_RECENTLY_USED, OnCheckCtrlBarVisible)
	ON_UPDATE_COMMAND_UI(ID_VIEW_INSERT, OnCheckCtrlBarVisible)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LATEX, OnCheckCtrlBarVisible)
	ON_UPDATE_COMMAND_UI(ID_VIEW_STATUS_BAR, OnCheckCtrlBarVisible)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MENU_BAR, OnCheckCtrlBarVisible)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FIND, OnCheckCtrlBarVisible)
	ON_REGISTERED_MESSAGE(BCGM_TOOLBARMENU, OnToolbarContextMenu)
	ON_REGISTERED_MESSAGE(BCGM_RESETTOOLBAR, OnResetToolbar)
	ON_REGISTERED_MESSAGE(BCGM_CUSTOMIZETOOLBAR, OnCustomizationMode)
	ON_UPDATE_COMMAND_UI_RANGE(ID_USER_TOOL_FIRST, ID_USER_TOOL_LAST, OnUpdateUserTool)
	ON_COMMAND_RANGE(ID_USER_TOOL_FIRST, ID_USER_TOOL_LAST, OnExecuteUserTool)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,			// Statusleistenanzeige
	ID_EDIT_INDICATOR_POSITION,
	//ID_EDIT_INDICATOR_COL,
	ID_EDIT_INDICATOR_CRLF,
	ID_INDICATOR_OVR,
	ID_EDIT_INDICATOR_READ,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame Konstruktion/Zerstörung

CMainFrame::CMainFrame()
:	m_bFullScreen(FALSE),
	m_bFSModeShowMenuBar(FALSE),
	m_bFSModeShowDocTabs(FALSE),
	m_bMaxChild(FALSE),
	m_pwndFullScrnToolBar(FALSE)
{
	m_pTargetWindow = NULL;
}


CMainFrame::~CMainFrame()
{}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// load user images
	if( !m_userImages.Load( theApp.GetWorkingDir() + _T("\\UserImages.bmp") ) )
	{
		TRACE( _T("Failed to load user images\n") );
		ASSERT( FALSE );
	}
	else
	{
		CBCGToolBar::SetUserImages( &m_userImages );
	}

	// create menu bar
	if (!g_configuration.m_bOptimizeMenuForVisuallyHandicappedUsers)
	{
		if( !m_wndMenuBar.Create( this ) )
		{
			TRACE0( "Failed to create menubar\n" );
			return -1;
		}
		m_wndMenuBar.SetBarStyle( 
			m_wndMenuBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );
	}
	else
	{
		// load menu bars
		m_stdMenu.LoadMenu( IDR_MAINFRAME );
		m_projectMenu.LoadMenu( IDR_LATEXPROJECT );
		MDISetMenu(&m_stdMenu, NULL);
	}

	// Allow disabled menu items to be highligted...
	CBCGMenuBar::HighlightDisabledItems ();

	// create main tool bar
	if( !CreateToolBar( &m_wndToolBar, IDR_MAINFRAME, STE_TB_MAINFRAME ) )
		return -1;

	// create find bar
	if( !CreateToolBar( &m_wndFindBar, IDR_FIND, STE_TB_FIND ) )
		return -1;

	// create insert bar
	if( !CreateToolBar( &m_wndInsertBar, IDR_INSERT, STE_TB_INSERT ) )
		return -1;

	// create format recently used tool bar
	if( !CreateToolBar( &m_wndFormatRUBar, IDR_FORMAT_RECENTLY_USED, STE_TB_FORMAT_RECENTLY_USED ) )
		return -1;

	// create format tool bar
	if( !CreateToolBar( &m_wndFormatBar, IDR_FORMAT, STE_TB_FORMAT, FALSE ) )
		return -1;

	// create latex tool bar
	if( !CreateToolBar( &m_wndLatexBar, IDR_LATEX, STE_TB_LATEX ) )
		return -1;

	// create math tool bar
	if( !CreateToolBar( &m_wndMathBar, IDR_MATH, STE_TB_MATH ) )
		return -1;

	// create all math bars
	for( int i = 0; i < MATHBAR_COUNT; i++ )
		if( !CreateToolBar( &m_awndMathBar[i], IDR_MATHBAR1 + i, STE_TB_MATH1 + i, FALSE ) )
			return -1;

	// create status bar
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;		// creation failed
	}

	// initialization
	CBCGToolBar::EnableQuickCustomization();
	CBCGToolBar::AddToolBarForImageCollection( IDR_IMAGES );

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Create docking control windows
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if( !m_wndNavigationBar.Create( CString( (LPCTSTR)STE_NAVIGATIONBAR_TITLE ), this, CSize( 200, 200 ), TRUE, ID_VIEW_NAVIGATION, WS_CHILD | WS_VISIBLE | CBRS_LEFT ) )
		return -1;
	m_wndNavigationBar.SetCaptionStyle( TRUE, TRUE );

	if( !m_wndOutputBar.Create( CString( (LPCTSTR)STE_OUTPUT_TITLE ), this, CSize( 200, 100 ), TRUE, ID_VIEW_OUTPUT, WS_CHILD | WS_VISIBLE | CBRS_BOTTOM ) )
		return -1;
	m_wndOutputBar.SetCaptionStyle( TRUE, TRUE );

	// dock the bars
	if (!g_configuration.m_bOptimizeMenuForVisuallyHandicappedUsers)
		m_wndMenuBar.EnableDocking( CBRS_ALIGN_ANY );

	m_wndToolBar.EnableDocking( CBRS_ALIGN_ANY );
	m_wndFindBar.EnableDocking( CBRS_ALIGN_ANY );
	m_wndInsertBar.EnableDocking( CBRS_ALIGN_ANY );
	m_wndFormatRUBar.EnableDocking( CBRS_ALIGN_ANY );
	m_wndFormatBar.EnableDocking( CBRS_ALIGN_ANY );
	m_wndLatexBar.EnableDocking( CBRS_ALIGN_ANY );
	m_wndMathBar.EnableDocking( CBRS_ALIGN_ANY );
	for( i = 0; i < MATHBAR_COUNT; i++ )
		m_awndMathBar[i].EnableDocking( CBRS_ALIGN_ANY );

	m_wndNavigationBar.EnableDocking( CBRS_ALIGN_ANY );
	m_wndOutputBar.EnableDocking( CBRS_ALIGN_ANY );

	EnableDocking(CBRS_ALIGN_ANY);

	if (!g_configuration.m_bOptimizeMenuForVisuallyHandicappedUsers)
		DockControlBar( &m_wndMenuBar );

	DockControlBar( &m_wndToolBar );
	DockControlBarLeftOf(&m_wndLatexBar, &m_wndToolBar);
	DockControlBar( &m_wndInsertBar );
	DockControlBarLeftOf( &m_wndFindBar, &m_wndInsertBar );
	DockControlBar( &m_wndFormatBar );
	DockControlBar( &m_wndFormatRUBar );
	DockControlBarLeftOf( &m_wndMathBar, &m_wndFormatRUBar );
	DockControlBar( &m_wndNavigationBar );
	DockControlBar( &m_wndOutputBar );

	// loading toolbars
	CString 	strControlBarRegEntry = theApp.m_strRegistryRoot + _T("\\CtrlBar\\");

	CBCGToolBar::LoadParameters( strControlBarRegEntry );

	// load user defined toolbars
	InitUserToobars( NULL, IDR_USER_TOOLBAR_FIRST, IDR_USER_TOOLBAR_LAST );

	// Float the math bars
	CRect	rect;
	m_wndMathBar.GetWindowRect(rect);
	for( i = 0; i < MATHBAR_COUNT; i++ )
	{
		CPoint	pt(rect.left + i*m_wndMathBar.GetColumnWidth(), rect.top);
		FloatControlBar(&m_awndMathBar[i], pt, CBRS_ALIGN_LEFT);
	}

	// Enable window list manager...
	if (!g_configuration.m_bOptimizeMenuForVisuallyHandicappedUsers)
		EnableWindowsDialog(ID_WINDOW_LIST, ID_WINDOW_LIST_MENU, TRUE);
	else
	{
		// add entry for window list to the end of window menu
		CMenu		*pmnuWindowMenu = m_stdMenu.GetSubMenu(m_stdMenu.GetMenuItemCount()-2);
		pmnuWindowMenu->AppendMenu(MF_SEPARATOR);
		pmnuWindowMenu->AppendMenu(MF_BYPOSITION, ID_WINDOW_LIST, CString((LPCTSTR)ID_WINDOW_LIST_MENU));
	}

	return 0;
}


void CMainFrame::OnClose() 
{
	// If we are still in full screen mode, restore normal mode before closing
	// to ensure proper control position saving.
	if (m_bFullScreen)
		FullScreenOff();	
	
	theApp.EndSession();

	CBCGMDIFrameWnd::OnClose();
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CBCGMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	return TRUE;
}


BOOL CMainFrame::SetMenu( CMenu *pMenu )
{
	if (!g_configuration.m_bOptimizeMenuForVisuallyHandicappedUsers)
		m_wndMenuBar.CreateFromMenu( pMenu->m_hMenu, FALSE );
	return TRUE;
}


BOOL CMainFrame::CreateToolBar( CBCGToolBar *pToolBar, UINT unID, UINT unTitleID, BOOL bVisible /*= TRUE*/ )
{
	if ( !pToolBar->Create( this, 
					bVisible?
					WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC :
					WS_CHILD | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,
					unID
		) ||
		!pToolBar->LoadToolBar( unID ))
	{
		TRACE( "!Failed to create %s tool bar\n", AfxLoadString( unTitleID ) );
		return FALSE;	   // creation failed
	}

	pToolBar->SetWindowText( AfxLoadString( unTitleID ) );
	pToolBar->EnableTextLabels( FALSE );

	return TRUE;
}

void CMainFrame::ToggleControlBar( CControlBar *pCtrlBar )
{
	ASSERT( pCtrlBar );
	ASSERT( IsWindow( pCtrlBar->m_hWnd ) );

	// control bars for math symbols are exclusive
	ShowControlBar( pCtrlBar, !IsControlBarVisible( pCtrlBar ), FALSE );
}


BOOL CMainFrame::IsControlBarVisible( CControlBar *pCtrlBar )
{
	ASSERT( pCtrlBar );
	ASSERT( IsWindow( pCtrlBar->m_hWnd ) );
	return (pCtrlBar->GetStyle() & WS_VISIBLE);
}


CControlBar *CMainFrame::GetControlBarByCmd( UINT unID )
{
	// one of the math-symbol bars?
	if( unID >= ID_MATH1 && unID <= ID_MATH16 )
		return &m_awndMathBar[unID - ID_MATH1];

	// one of the user defined bars?
	if( unID >= ID_VIEW_USER_TOOLBAR_FIRST && unID <= ID_VIEW_USER_TOOLBAR_LAST )
		return GetUserBarByIndex( unID - ID_VIEW_USER_TOOLBAR_FIRST );

	switch( unID )
	{
		case ID_VIEW_MATH:
			return &m_wndMathBar;

		case ID_VIEW_NAVIGATION:
			return &m_wndNavigationBar;

		case ID_VIEW_OUTPUT:
			return &m_wndOutputBar;

		case ID_VIEW_TOOLBAR:
			return &m_wndToolBar;

		case ID_VIEW_FIND:
			return &m_wndFindBar;

		case ID_VIEW_INSERT:
			return &m_wndInsertBar;

		case ID_VIEW_FORMAT:
			return &m_wndFormatBar;

		case ID_VIEW_FORMAT_RECENTLY_USED:
			return &m_wndFormatRUBar;

		case ID_VIEW_LATEX:
			return &m_wndLatexBar;

		case ID_VIEW_STATUS_BAR:
			return &m_wndStatusBar;

		case ID_VIEW_MENU_BAR:
			return &m_wndMenuBar;

		default:
			return NULL;
	}
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame Diagnose

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CBCGMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CBCGMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame Nachrichten-Handler


BOOL CMainFrame::DestroyWindow() 
{
	return CBCGMDIFrameWnd::DestroyWindow();
}

#include "CustomizePageEditor.h"
void CMainFrame::OnExtrasCustomize() 
{
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// create dialog object

	CBCGToolbarCustomizeEx* pDlgCust = new CBCGToolbarCustomizeEx( 
		this, TRUE, 
		BCGCUSTOMIZE_MENU_SHADOWS|BCGCUSTOMIZE_TEXT_LABELS| 
			BCGCUSTOMIZE_LOOK_2000|BCGCUSTOMIZE_MENU_ANIMATIONS|BCGCUSTOMIZE_SELECT_SKINS|
			BCGCUSTOMIZE_NOHELP);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// enable create/delete of user defined toolbars
	pDlgCust->EnableUserDefinedToolbars();

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// add menus, toolbars and commands
	pDlgCust->AddMenu( IDR_LATEXPROJECT );
	pDlgCust->AddMenu( IDR_MAINFRAME );
	pDlgCust->AddMenu( IDR_HIDDEN_CMDS );
	pDlgCust->AddMenu( IDR_POPUP_ITEM );

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// create combobox for output typess
	CBCGToolbarComboBoxButton	profileButton( 
		ID_LATEX_PROFILE_SEL, 
		CImageHash::GetImageOfCommand( ID_LATEX_PROFILE_SEL, FALSE ), 
		CBS_DROPDOWNLIST | CBS_SORT );

	// add available outputtypes to combo box
	CStringArray	astrProfiles;
	g_ProfileMap.GetKeyList(astrProfiles);

	for (int i = 0; i < astrProfiles.GetSize(); i++)
		profileButton.AddItem(astrProfiles[i]);
	profileButton.SelectItem(g_ProfileMap.GetActiveProfileKey());

	// replace button
	pDlgCust->ReplaceButton(ID_LATEX_PROFILE_SEL, profileButton);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// display dialog
	pDlgCust->Create();

	theApp.UpdateLatexProfileSel();
}


void CMainFrame::OnTimer(UINT nIDEvent) 
{
	switch( nIDEvent )
	{
		case timerSave: // save and parse project
			theApp.SaveAllModifiedWithoutPrompt();
		case timerParse:	// parse project
			SendMessage( WM_COMMAND, ID_PROJECT_PARSE );
			break;
	}
	
	CBCGMDIFrameWnd::OnTimer(nIDEvent);
}


void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CBCGMDIFrameWnd::OnActivate(nState, pWndOther, bMinimized);

	// Check if files have changed
	CMultiDocTemplate	*pTemplate = theApp.GetLatexDocTemplate();
	if (pTemplate && (nState == WA_ACTIVE || nState == WA_CLICKACTIVE))
	{
		POSITION	pos = pTemplate->GetFirstDocPosition();
		while (pos)
		{
			CLatexDoc	*pDoc = dynamic_cast<CLatexDoc*>(pTemplate->GetNextDoc(pos));
			if (pDoc)
				pDoc->CheckForFileChanges();
		}
	}

	// activate editor
	if (nState == WA_ACTIVE || nState == WA_CLICKACTIVE)
	{
		CView	*pView = theApp.GetActiveEditView();
		if( pView )
			pView->SetFocus();
	}
}


void CMainFrame::OnOptionsChanged() 
{
	// install/uninstall timer for automatic saving
	KillTimer( timerSave );
	if( g_configuration.m_bSaveAutomatic )
		SetTimer( timerSave, g_configuration.m_unSaveInterval * 1000 * 60, NULL );
}


void CMainFrame::OnToggleCtrlBar( UINT nIDEvent )
{
	CControlBar *pCtrlBar = GetControlBarByCmd( nIDEvent );
	ASSERT( pCtrlBar );
	ASSERT( IsWindow( pCtrlBar->m_hWnd ) );
	ToggleControlBar( pCtrlBar );
}


void CMainFrame::OnToggleMathBar( UINT nIDEvent )
{
	CControlBar *pCtrlBar = GetControlBarByCmd( nIDEvent );
	ASSERT( pCtrlBar );
	ASSERT( IsWindow( pCtrlBar->m_hWnd ) );

	// should we enable the bar?
	if (!pCtrlBar->IsWindowVisible())
	{
		// ... otherwise ensure, that all other math bars are hidden
		for (int i = 0; i < MATHBAR_COUNT; i++)
		{
			if ((CControlBar*)&m_awndMathBar[i] != pCtrlBar)
			{
				if (m_awndMathBar[i].IsWindowVisible())
					ShowControlBar(&m_awndMathBar[i], FALSE, FALSE);
			}
		}

		// enable
		ShowControlBar(pCtrlBar, TRUE, FALSE);
	}
	else
		// disable
		ShowControlBar(pCtrlBar, FALSE, FALSE);
}


void CMainFrame::OnCheckCtrlBarVisible( CCmdUI *pCmdUI )
{
	CControlBar *pCtrlBar = GetControlBarByCmd( pCmdUI->m_nID );
	ASSERT( pCtrlBar );
	ASSERT( IsWindow( pCtrlBar->m_hWnd ) );
	pCmdUI->SetCheck( IsControlBarVisible( pCtrlBar ) );
}


LRESULT CMainFrame::OnToolbarContextMenu( WPARAM wp, LPARAM lp )
{
	CPoint point( LOWORD (lp), HIWORD(lp) );

	CMenu menu;
	VERIFY(menu.LoadMenu( IDR_POPUP_TOOLBAR ));

	CMenu* pPopup = menu.GetSubMenu( 0 );
	ASSERT(pPopup != NULL);

	// Add user-defined toolbar names to the end
	for( int i = 0; i < (IDR_USER_TOOLBAR_LAST - IDR_USER_TOOLBAR_FIRST + 1); i++ )
	{
		CBCGToolBar *pToolBar = GetUserBarByIndex( i );
		if( pToolBar != NULL )
		{
			// add title of current tool bar to menu
			CString strToolbarName;
			pToolBar->GetWindowText( strToolbarName );
			pPopup->InsertMenu( pPopup->GetMenuItemCount() - 2, MF_STRING | MF_BYPOSITION, ID_VIEW_USER_TOOLBAR_FIRST + i, strToolbarName );
		}
	}

	// display menu
	CBCGPopupMenu* pPopupMenu = new CBCGPopupMenu;
	pPopupMenu->Create( this, point.x, point.y, pPopup->Detach () );

	return 0;
}

void CMainFrame::OnWindowEditor() 
{
	// get active frame
	CFrameWnd	*pFrame = GetActiveFrame();
	if( !pFrame )
		return;

	// get active view
	CView	*pView = pFrame->GetActiveView();
	if( !pView || !pView->IsKindOf( RUNTIME_CLASS(CLatexEdit) ) )
		return;

	// activate view
	pView->SetFocus();
}


void CMainFrame::OnWindowNavigator() 
{
	if( !IsWindow( m_wndNavigationBar.m_hWnd ) || !IsWindow( m_wndNavigationBar.GetTabWnd().m_hWnd ) )
		return;

	// make bar visible if it is not
	if( !m_wndNavigationBar.IsVisible() )
		ShowControlBar( &m_wndNavigationBar, TRUE, FALSE );

	// get active window and set focus on it
	CWnd* pwnd = m_wndNavigationBar.GetTabWnd().GetActiveWnd();

	if( !pwnd || !IsWindow( pwnd->m_hWnd ) )
		return;

	pwnd->SetFocus();
}


void CMainFrame::ActivateNavigationTab( int nTab )
{
	if( !IsWindow( m_wndNavigationBar.m_hWnd ) || !IsWindow( m_wndNavigationBar.GetTabWnd().m_hWnd ) )
		return;

	// select structure tab
	if( m_wndNavigationBar.GetTabWnd().GetActiveTab() != nTab )
		m_wndNavigationBar.GetTabWnd().SetActiveTab( nTab );

	// Activate navigator window
	OnWindowNavigator();
}


void CMainFrame::OnWindowStructure() 
{
	ActivateNavigationTab( navigatorTabStructure );
}


void CMainFrame::OnWindowEnvironment() 
{
	ActivateNavigationTab( navigatorTabEnvironments );
}


void CMainFrame::OnWindowFiles() 
{
	ActivateNavigationTab( navigatorTabFiles );
}

void CMainFrame::OnWindowReferences() 
{
	ActivateNavigationTab( navigatorTabReferences );
}

void CMainFrame::OnWindowOutput()
{
	ActivateOutputBar(true);
}


void CMainFrame::ActivateOutputBar(bool bSetFocus)
{
	if( !IsWindow( m_wndOutputBar.m_hWnd ) || !IsWindow( m_wndOutputBar.GetTabWnd().m_hWnd ) )
		return;

	// make bar visible if it is not
	if( !m_wndOutputBar.IsVisible() )
		ShowControlBar( &m_wndOutputBar, TRUE, FALSE );

	// get active window and set focus on it
	if (bSetFocus)
	{
		CWnd* pwnd = m_wndOutputBar.GetTabWnd().GetActiveWnd();
		if (pwnd && IsWindow(pwnd->m_hWnd)) pwnd->SetFocus();
	}
}


void CMainFrame::ActivateOutputTab(int nTab, bool bSetFocus)
{
	if( !IsWindow( m_wndOutputBar.m_hWnd ) || !IsWindow( m_wndOutputBar.GetTabWnd().m_hWnd ) )
		return;

	// select output tab
	if (m_wndOutputBar.GetTabWnd().GetActiveTab() != nTab)
	{
		//SetActiveTab sets the focus. So we need to restore, if this shall not be.
		CWnd* pwndLastFocus = bSetFocus ? NULL : CWnd::GetFocus();
		m_wndOutputBar.GetTabWnd().SetActiveTab(nTab);
		//Set focus back
		if (pwndLastFocus && IsWindow(pwndLastFocus->m_hWnd)) pwndLastFocus->SetFocus();
	}

	// Activate output window
	ActivateOutputBar(bSetFocus);
}

void CMainFrame::OnWindowBuildResult() 
{
	ActivateOutputTab(outputTabBuildResult, true);
}


void CMainFrame::OnWindowGrep1() 
{
	ActivateOutputTab(outputTabGrep1, true);
}


void CMainFrame::OnWindowGrep2() 
{
	ActivateOutputTab(outputTabGrep2, true);
}

void CMainFrame::OnWindowParse() 
{
	ActivateOutputTab(outputTabParse, true);
}


BOOL CMainFrame::ReplaceToolbarButton( UINT unCmdID, const CBCGToolbarButton &button )
{
	// replace the first occurence of the button
	if( m_wndFindBar.CommandToIndex( unCmdID ) > -1 )
	{
		m_wndFindBar.ReplaceButton( unCmdID, button );
		return TRUE;
	}
	if( m_wndFormatBar.CommandToIndex( unCmdID ) > -1 )
	{
		m_wndFormatBar.ReplaceButton( unCmdID, button );
		return TRUE;
	}
	if( m_wndFormatRUBar.CommandToIndex( unCmdID ) > -1 )
	{
		m_wndFormatRUBar.ReplaceButton( unCmdID, button );
		return TRUE;
	}
	if( m_wndInsertBar.CommandToIndex( unCmdID ) > -1 )
	{
		m_wndInsertBar.ReplaceButton( unCmdID, button );
		return TRUE;
	}
	if( m_wndLatexBar.CommandToIndex( unCmdID ) > -1 )
	{
		m_wndLatexBar.ReplaceButton( unCmdID, button );
		return TRUE;
	}
	if( m_wndMathBar.CommandToIndex( unCmdID ) > -1 )
	{
		m_wndMathBar.ReplaceButton( unCmdID, button );
		return TRUE;
	}
	if( m_wndToolBar.CommandToIndex( unCmdID ) > -1 )
	{
		m_wndToolBar.ReplaceButton( unCmdID, button );
		return TRUE;
	}
	
	for( int i = 0; i < MATHBAR_COUNT; i++ )
	{
		if( m_awndMathBar[i].CommandToIndex( unCmdID ) > -1 )
		{
			m_awndMathBar[i].ReplaceButton( unCmdID, button );
			return TRUE;
		}
	}

	// button not found
	return FALSE;
}


CBCGToolbarButton *CMainFrame::GetToolbarButton( UINT unCmdID, POSITION &pos ) const
{
	// find and return specified occurence of the button
	int nIndex;
	int nStartBar = (int)pos;

	if( nStartBar < 1 && (nIndex = m_wndFindBar.CommandToIndex( unCmdID )) > -1 )
	{
		pos = (POSITION)1;
		return m_wndFindBar.GetButton( nIndex );
	}
	if( nStartBar < 2 && (nIndex = m_wndFormatBar.CommandToIndex( unCmdID )) > -1 )
	{
		pos = (POSITION)2;
		return m_wndFormatBar.GetButton( nIndex );
	}
	if( nStartBar < 3 && (nIndex = m_wndFormatRUBar.CommandToIndex( unCmdID )) > -1 )
	{
		pos = (POSITION)3;
		return m_wndFormatRUBar.GetButton( nIndex );
	}
	if( nStartBar < 4 && (nIndex = m_wndInsertBar.CommandToIndex( unCmdID )) > -1 )
	{
		pos = (POSITION)4;
		return m_wndInsertBar.GetButton( nIndex );
	}
	if( nStartBar < 5 && (nIndex = m_wndLatexBar.CommandToIndex( unCmdID )) > -1 )
	{
		pos = (POSITION)5;
		return m_wndLatexBar.GetButton( nIndex );
	}
	if( nStartBar < 6 && (nIndex = m_wndMathBar.CommandToIndex( unCmdID )) > -1 )
	{
		pos = (POSITION)6;
		return m_wndMathBar.GetButton( nIndex );
	}
	if( nStartBar < 7 && (nIndex = m_wndToolBar.CommandToIndex( unCmdID )) > -1 )
	{
		pos = (POSITION)7;
		return m_wndToolBar.GetButton( nIndex );
	}
	
	//Look for the button in the Math-Toolbars
	if (nStartBar < 8 + MATHBAR_COUNT - 1)
	{
		for( int i = 0; i < MATHBAR_COUNT; i++ )
		{
			if( nStartBar < (i + 8) && (nIndex = m_awndMathBar[i].CommandToIndex( unCmdID )) > -1 )
			{
				pos = (POSITION)(i + 8);
				return m_awndMathBar[i].GetButton( nIndex );
			}
		}
	}

	//Find the Toolbutton in the UserToolbars
	for( int i = 0; i < (IDR_USER_TOOLBAR_LAST - IDR_USER_TOOLBAR_FIRST + 1); i++ )
	{
		CBCGToolBar *pToolBar = GetUserBarByIndex( i );
		if (!pToolBar) continue;
		if( nStartBar < (i + 8 + MATHBAR_COUNT) && (nIndex = pToolBar->CommandToIndex( unCmdID )) > -1 )
		{
			pos = (POSITION)(i + 8 + MATHBAR_COUNT);
			return pToolBar->GetButton( nIndex );
		}
	}


	// button not found
	return NULL;
}


LRESULT CMainFrame::OnResetToolbar( WPARAM wParam, LPARAM lParam )
{
	// modify toolbar buttons

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// create combobox for output typess
	CBCGToolbarComboBoxButton	profileButton( 
		ID_LATEX_PROFILE_SEL, 
		CImageHash::GetImageOfCommand( ID_LATEX_PROFILE_SEL, FALSE ), 
		CBS_DROPDOWNLIST | CBS_SORT );

	// add available outputtypes to combo box
	CStringArray	astrProfiles;
	g_ProfileMap.GetKeyList(astrProfiles);

	for (int i = 0; i < astrProfiles.GetSize(); i++)
		profileButton.AddItem(astrProfiles[i]);
	profileButton.SelectItem(g_ProfileMap.GetActiveProfileKey());

	// replace button
	ReplaceToolbarButton(ID_LATEX_PROFILE_SEL, profileButton);

	return 0L;
}


BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// forward notifications from ComboBoxes as command
	if( HIWORD( wParam ) == CBN_SELENDOK || HIWORD( wParam ) == CBN_EDITCHANGE )
		SendMessage( WM_COMMAND, (WPARAM)LOWORD( wParam ) );	
	
	// Bug fix: 688063: Tab context menu closes wrong file
	// We have to handle file close event from context menu seperately
	if (wParam == ID_FILE_CLOSE) {
		::SendMessage(m_wndClientArea.GetMDITabs(), WM_LBUTTONDOWN, 0, 0);
		OnFileClose();
		return TRUE;
	}
	return CBCGMDIFrameWnd::OnCommand(wParam, lParam);
}


void CMainFrame::OnWindowList() 
{
	// Display the window list dialog...
	ShowWindowsDialog();	
}


BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	CWnd* pWndFocus = GetFocus ();

	if ((pWndFocus != NULL) &&
		((m_wndNavigationBar.IsChild (pWndFocus))	||
		(m_wndOutputBar.IsChild (pWndFocus))))	
	{
		// see if the window can handle this message
		if (pWndFocus->OnCmdMsg (nID, nCode, pExtra, pHandlerInfo))
		{
			return TRUE;
		}
	}

//	if ((m_wndNavigationBar) && 
//		m_wndNavigationBar.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
//		return TRUE;
//
//	if ((m_wndOutputBar) && 
//		m_wndOutputBar.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
//		return TRUE;

//	if ((m_wndNavigationBar.GetOutputDoc() != NULL) &&	
//		m_wndNavigationBar.GetOutputDoc()->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
//		return TRUE;

	if ((m_wndOutputBar.GetOutputDoc() != NULL) &&	
		m_wndOutputBar.GetOutputDoc()->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;
	
	return CBCGMDIFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::GetControlBarsEx(CArray<CControlBar*, CControlBar*>& arrBars)
{
	if (m_wndMenuBar)
		arrBars.Add(&m_wndMenuBar); 	   
	if (m_wndStatusBar)
		arrBars.Add(&m_wndStatusBar);	   
	if (m_wndToolBar)
		arrBars.Add(&m_wndToolBar); 	   
	if (m_wndFormatBar)
		arrBars.Add(&m_wndFormatBar);
	if (m_wndFormatRUBar)
		arrBars.Add(&m_wndFormatRUBar);
	if (m_wndInsertBar)
		arrBars.Add(&m_wndInsertBar);
	if (m_wndLatexBar)
		arrBars.Add(&m_wndLatexBar);
	
	if (m_wndNavigationBar)
		arrBars.Add(&m_wndNavigationBar);
						 
	if (m_wndMathBar)
		arrBars.Add(&m_wndMathBar); 	 
	if (m_wndOutputBar)
		arrBars.Add(&m_wndOutputBar);	   
	if (m_wndFindBar)
		arrBars.Add(&m_wndFindBar);
	
	for (int i = 0; i < MATHBAR_COUNT; i++)
	{
		if (m_awndMathBar[i])
			arrBars.Add(&m_awndMathBar[i]);
	}

	// Finally, for the user toolbars
	int nCount = IDR_USER_TOOLBAR_LAST - IDR_USER_TOOLBAR_FIRST + 1;
	for (int j = 0; j < nCount; j++)
	{
		CControlBar* pToolBar = GetUserBarByIndex(j);
		if (pToolBar != NULL)
			arrBars.Add(pToolBar);
	}
}


BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	if (!CBCGMDIFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	m_wndNavigationBar.SetFlatTabs(theApp.m_bTabFlatBorders);
	m_wndOutputBar.SetFlatTabs(theApp.m_bTabFlatBorders);
	DisplayDocumentTabs();

	return TRUE;
}

			 
void CMainFrame::OnHelpContents() 
{
	::HtmlHelp(NULL, AfxGetApp()->m_pszHelpFilePath, HH_DISPLAY_TOC, 0L);	
}


void CMainFrame::OnHelpSearch() 
{
	HH_FTS_QUERY query;
	ZeroMemory(&query, sizeof(query));
	query.cbStruct		  = sizeof(HH_FTS_QUERY);		   
	query.fStemmedSearch  = FALSE;
	query.fTitleOnly	  = FALSE; 
	query.fExecute		  = FALSE;
	query.pszWindow 	  = NULL;

	::HtmlHelp(NULL, AfxGetApp()->m_pszHelpFilePath, HH_DISPLAY_SEARCH, (DWORD)&query); 
}


void CMainFrame::OnHelpIndex() 
{
	::HtmlHelp(NULL, AfxGetApp()->m_pszHelpFilePath, HH_DISPLAY_INDEX, (DWORD)_T(""));	
}


void CMainFrame::WinHelp(DWORD dwData, UINT nCmd) 
{
	::HtmlHelp(NULL, AfxGetApp()->m_pszHelpFilePath, HH_HELP_CONTEXT, nCmd == HELP_CONTEXT ? dwData : 0);	
}


void CMainFrame::DisplayDocumentTabs()
{
	EnableMDITabs(theApp.m_bMDITabs, theApp.m_bMDITabIcons,
					(CBCGTabWnd::Location)theApp.m_nMDITabLocation,
					false, false,
					(CBCGTabWnd::Style)theApp.m_nMDITabStyle);
}

void CMainFrame::OnViewDocTabsBottom() 
{
	theApp.m_bMDITabs = true;
	theApp.m_nMDITabLocation = CBCGTabWnd::LOCATION_BOTTOM;
	DisplayDocumentTabs();
}

void CMainFrame::OnViewDocTabsTop() 
{
	theApp.m_bMDITabs = true;
	theApp.m_nMDITabLocation = CBCGTabWnd::LOCATION_TOP;
	DisplayDocumentTabs();
}

void CMainFrame::OnViewDocTabsOff() 
{
	theApp.m_bMDITabs = false;
	DisplayDocumentTabs();
}

void CMainFrame::OnViewDocTabsIcons() 
{
	theApp.m_bMDITabIcons = !theApp.m_bMDITabIcons;
	DisplayDocumentTabs();
}

void CMainFrame::OnViewDocTabsNote() 
{
	if (theApp.m_nMDITabStyle == CBCGTabWnd::STYLE_3D_SCROLLED)
	{
		theApp.m_nMDITabStyle = CBCGTabWnd::STYLE_3D_ONENOTE;
	}
	else
	{
		theApp.m_nMDITabStyle = CBCGTabWnd::STYLE_3D_SCROLLED;
	}
	DisplayDocumentTabs();
}

void CMainFrame::OnUpdateViewDocTabs(CCmdUI* pCmdUI) 
{
	switch (pCmdUI->m_nID)
	{
		case ID_VIEW_DOCTAB_BOTTOM:
			pCmdUI->SetRadio(theApp.m_bMDITabs && (theApp.m_nMDITabLocation == CBCGTabWnd::LOCATION_BOTTOM) );
			break;

		case ID_VIEW_DOCTAB_TOP:
			pCmdUI->SetRadio(theApp.m_bMDITabs && (theApp.m_nMDITabLocation == CBCGTabWnd::LOCATION_TOP) );
			break;

		case ID_VIEW_DOCTAB_OFF:
			pCmdUI->SetRadio(!theApp.m_bMDITabs);
			break;

		case ID_VIEW_DOCTAB_ICONS:
			pCmdUI->SetCheck(theApp.m_bMDITabIcons);
			pCmdUI->Enable(theApp.m_bMDITabs);
			break;

		case ID_VIEW_DOCTAB_NOTE:
			pCmdUI->SetCheck(theApp.m_nMDITabStyle == CBCGTabWnd::STYLE_3D_ONENOTE);
			pCmdUI->Enable(theApp.m_bMDITabs);
			break;

		default:
			ASSERT(false);
			break;
	}
}
	

void CMainFrame::HideControlBars() 
{
	m_pwndControlBars = new CArray<FSControlBars, FSControlBars&>;
	FSControlBars xControlBar;
	CArray<CControlBar*, CControlBar*> arrBars;
	GetControlBarsEx(arrBars);
	for (int i = 0; i < arrBars.GetSize(); i++)
	{
		xControlBar.pControlBar = arrBars[i];
		if (m_bFSModeShowMenuBar && 
			xControlBar.pControlBar->IsKindOf(RUNTIME_CLASS(CBCGMenuBar)))
			continue;

		xControlBar.bWasVisible = xControlBar.pControlBar->IsWindowVisible();
		m_pwndControlBars->Add(xControlBar);
		ShowControlBar(xControlBar.pControlBar, FALSE, FALSE);
	}
}


void CMainFrame::ShowControlBars() 
{
	FSControlBars xControlBar;
	// Iterate through all the control bar information...
	for (int i = 0; i < m_pwndControlBars->GetSize(); i++)
	{	
		// Obtain one at the given index...
		xControlBar = m_pwndControlBars->GetAt(i);
		// Restore the show/hide state before fullscreen view
		ShowControlBar(xControlBar.pControlBar, 
			xControlBar.bWasVisible, FALSE);
	}

	// Now, destroy the control bar information container
	if (m_pwndControlBars != NULL)
	{
		m_pwndControlBars->RemoveAll();
		delete m_pwndControlBars;
		m_pwndControlBars = NULL;
	}
}


void CMainFrame::FullScreenOn()
{
	// available only if there is an active doc
	CMDIChildWnd* pChild = MDIGetActive();
	DWORD dwStyle = 0;
	if (pChild != NULL)
	{
		dwStyle = ::GetWindowLong(pChild->m_hWnd, GWL_STYLE);
	}

	// Save the necessary options for later restoration process
	m_bMaxChild 		=(dwStyle & WS_MAXIMIZE) ? TRUE : FALSE;
	GetWindowRect(&m_rcMainFrame);
	int cxScrn	 = ::GetSystemMetrics(SM_CXSCREEN);
	int cyScrn	 = ::GetSystemMetrics(SM_CYSCREEN);
	
	HideControlBars();	 // Hide the bars
		
	// The full screen toolbar array! The array form was chosen just in case
	// you wish to add more buttons to the floating toolbar
	UINT uIDToolBar[] = 
	{
		ID_VIEW_FULLSCREEN
	};
	
	// 1. Get the child window and hide it to avoid flicker. 
	if (pChild != NULL)
		pChild->ShowWindow(SW_HIDE);
	
	// 2. Remove the caption of the main window and resize it to the full screen
	dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	dwStyle &= ~WS_CAPTION;
	::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
	// Now resize the main window
	cxScrn	 = ::GetSystemMetrics(SM_CXSCREEN);
	cyScrn	 = ::GetSystemMetrics(SM_CYSCREEN);
	int cxBorder = ::GetSystemMetrics(SM_CXBORDER);
	int cyBorder = ::GetSystemMetrics(SM_CYBORDER);
	SetWindowPos(NULL, -cxBorder, -cyBorder, cxScrn + cxBorder * 2, 
		cyScrn + cyBorder * 2, SWP_NOZORDER);
	
	// 3. Create the new full screen toolbar, for toggling on mode
	m_pwndFullScrnToolBar = new CToolBar;
	if (!m_pwndFullScrnToolBar->CreateEx(this, TBSTYLE_FLAT, 
		WS_VISIBLE | CBRS_TOOLTIPS | 
		CBRS_FLYBY | CBRS_SIZE_DYNAMIC | CBRS_FLOATING) || 
		!m_pwndFullScrnToolBar->LoadToolBar(IDR_FULLSCREEN))
	{
		TRACE0(_T("Failed to create the full screen toolbar\n"));
		return;
	}
	m_pwndFullScrnToolBar->SetButtons(uIDToolBar, 
		sizeof(uIDToolBar) / sizeof(UINT));
	m_pwndFullScrnToolBar->EnableDocking(0);
	m_pwndFullScrnToolBar->SetWindowText(_T("Full Screen"));
	CRect rcTB;
	m_pwndFullScrnToolBar->GetItemRect(0, rcTB);
	// position the full screen toolbar at some convenient location
	FloatControlBar(m_pwndFullScrnToolBar, 
		CPoint(cxScrn - rcTB.Width() * 3, /*cyScrn - */rcTB.Height() * 3));
	
	// 4. display child window
	if (pChild != NULL)
		pChild->ShowWindow(SW_SHOWMAXIMIZED);
	RecalcLayout();
	m_bFullScreen = TRUE;
}


void CMainFrame::FullScreenOff()
{
	// Display the hidden bars
	ShowControlBars();	 
		
	if (m_pwndFullScrnToolBar != NULL)
	{
		m_pwndFullScrnToolBar->DestroyWindow();
		delete m_pwndFullScrnToolBar;
	}
	
	// Hidding and showing the child window may not be the best solution,
	// but it helps reducing the flicker
	CMDIChildWnd* pChild = MDIGetActive();
	if (pChild)
		pChild->ShowWindow(SW_HIDE);	
	
	LONG dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	dwStyle |= WS_CAPTION;
	::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
	
	MoveWindow(&m_rcMainFrame);
	RecalcLayout();
	
	if (pChild)
	{
		if (m_bMaxChild)
			MDIMaximize(pChild);
		else 
			MDIRestore(pChild);
		
		pChild->ShowWindow(SW_SHOW);
	}
	m_bFullScreen = FALSE;
}


void CMainFrame::RebuildToolsMenu()
{
	// we only need to do this, when we do not use the BCG menu bar
	if (!g_configuration.m_bOptimizeMenuForVisuallyHandicappedUsers)
		return;

	//
	// Get the name of the tools menu from the reference menu
	//
	// (if MDI-childs are maximized, then the menu of our main frame
	// also contains items for the MDI childs system menu and the 
	// minimize, maximize and close buttons for the MDI child. So we
	// cannot determine the tools submenu by index, because the index
	// may change from time to time depending on if the MDI childs are
	// maximized or not).
	const int	nToolsMenuIndexOffset = 3;
	CMenu	mnuRef;
	VERIFY(mnuRef.LoadMenu(IDR_MAINFRAME));

	CString strToolsMenuTitle;
	mnuRef.GetMenuString(mnuRef.GetMenuItemCount()-nToolsMenuIndexOffset, strToolsMenuTitle, MF_BYPOSITION);

	//
	// Get the tools menu from the current menu
	//
	int nToolsMenuIndex = -1;
	for (int nItem = 0; nItem < m_stdMenu.GetMenuItemCount(); ++nItem)
	{
		CString strItem;
		m_stdMenu.GetMenuString(nItem, strItem, MF_BYPOSITION);
		if (strItem == strToolsMenuTitle)
		{
			nToolsMenuIndex = nItem;
			break;
		}
	}
	if (nToolsMenuIndex < 0)
	{
		ASSERT(FALSE);
		return;
	}

	CMenu *pToolsMenu = m_stdMenu.GetSubMenu(nToolsMenuIndex);
	if (!IsMenu(pToolsMenu->GetSafeHmenu()))
	{
		ASSERT(FALSE);
		return;
	}

	// Remove all current tool related entries
	int nLastEntryIndex = pToolsMenu->GetMenuItemCount()-1;
	while (
		pToolsMenu->GetMenuItemID(nLastEntryIndex)==ID_TOOLS_ENTRY ||
		(pToolsMenu->GetMenuItemID(nLastEntryIndex)>=ID_USER_TOOL_FIRST &&pToolsMenu->GetMenuItemID(nLastEntryIndex)<=ID_USER_TOOL_LAST))
	{
		pToolsMenu->DeleteMenu(nLastEntryIndex, MF_BYPOSITION);
		nLastEntryIndex = pToolsMenu->GetMenuItemCount()-1;
	}

	// add new tool entries
	const CObList	&toolsList = theApp.GetUserToolsManager()->GetUserTools();
	POSITION			pos = toolsList.GetHeadPosition();
	while (pos)
	{
		CBCGUserTool	*pTool = (CBCGUserTool*)toolsList.GetNext(pos);
		pToolsMenu->AppendMenu(MF_STRING, pTool->GetCommandId(), pTool->m_strLabel);
	}
}


int CMainFrame::GetMDIChilds(CArray<CChildFrame*, CChildFrame*>& MDIChildArray,
							  const bool bSortByTabs /*= true*/)
{
	int ActiveChild = -1;

	//Clean up
	MDIChildArray.RemoveAll();

	//Ordered by tabs or ordered as in the window list?
	//If tabs are not shown, we give the childs in window cycling order.
	if (bSortByTabs && theApp.m_bMDITabs)
	{
		//Get the tabs
		const CBCGTabWnd& Tabs = GetMDITabs();
		const int nTabs = Tabs.GetTabsNum();

		//Go through the tabs and get the childs
		for(int i=0;i<nTabs;i++)
		{
			CWnd* pWnd = Tabs.GetTabWnd(i);
			if (pWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
			{
				CChildFrame* pChild = dynamic_cast<CChildFrame*>(pWnd);

				//Add child to the output array
				if (pChild)
				{
					MDIChildArray.Add(pChild);

					//Mark as active
					if (Tabs.GetActiveTab() == i)
					{
						ActiveChild = MDIChildArray.GetSize() - 1;
					}
				}
			}
		}
	}
	else
	{
		//Get frame information
		int nFrame = 0;
		CWnd* pWnd = GetWindow(GW_CHILD)->GetWindow(GW_CHILD);

		//Start with the last child, if it exists
		if (pWnd && IsWindow(pWnd->m_hWnd))
		{
			pWnd = pWnd->GetWindow(GW_HWNDLAST);
		}

		//Iterate through all MDI Childs
		while (pWnd && IsWindow(pWnd->m_hWnd))
		{
			if (pWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
			{
				CChildFrame* pChild = dynamic_cast<CChildFrame*>(pWnd);
				MDIChildArray.Add(pChild);
			}

			// get previous window
			pWnd = pWnd->GetNextWindow(GW_HWNDPREV);
		}

		ActiveChild = MDIChildArray.GetSize() - 1;
	}

	return ActiveChild;
}


void CMainFrame::OnViewFullScreen() 
{
	if (m_bFullScreen)
		FullScreenOff();
	else 
		FullScreenOn();
}


void CMainFrame::OnUpdateViewFullScreen(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bFullScreen);
}


void CMainFrame::OnHelpKeyMapping() 
{
	CBCGKeyMapDlg	dlg(this, TRUE);
	dlg.DoModal();
}


LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// Pass message through file change notifier
	//theApp.m_FileChangeNotifier.PreTranslateMessage(message, wParam, lParam);
	
	return CBCGMDIFrameWnd::WindowProc(message, wParam, lParam);
}


void CMainFrame::OnUpdateWindowNext(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(MDIGetActive() != NULL);
}


void CMainFrame::OnWindowNext() 
{
	::SendMessage(m_hWndMDIClient, WM_MDINEXT, 0, FALSE);
}


void CMainFrame::OnUpdateWindowPrevious(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(MDIGetActive() != NULL);
}


void CMainFrame::OnWindowPrevious() 
{
	::SendMessage(m_hWndMDIClient, WM_MDINEXT, 0, TRUE);
}


void CMainFrame::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// We have to use the right window to get right coordinates ;-)
	::ScreenToClient(pWnd->GetSafeHwnd(), &point);

	if (pWnd->m_hWnd == m_hWndMDIClient || pWnd->m_hWnd == m_wndClientArea.GetMDITabs())
	{
		// determine target window for context menu 
		const int nTab = m_wndClientArea.GetMDITabs().GetTabFromPoint(point);
		if (nTab >= 0)
		{
			CChildFrame* wndEdit = dynamic_cast<CChildFrame*>(m_wndClientArea.GetMDITabs().GetTabWnd(nTab));
			// save pointer for later use
			if (wndEdit)
			{
				m_pTargetWindow = dynamic_cast<CLatexEdit *>(wndEdit->GetActiveView());
			}
		}

		// translate menu coordinates and show menu
		::ClientToScreen(pWnd->GetSafeHwnd(), &point);
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_MDICLIENT, point.x, point.y, this);
	}
	else
	{
		m_pTargetWindow = NULL; // mark as invalid
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_TOOLBAR, point.x, point.y, this);		
	}
}


BOOL CMainFrame::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	return TRUE;//CBCGMDIFrameWnd::OnHelpInfo(pHelpInfo);
}


LRESULT CMainFrame::OnCustomizationMode(WPARAM wParam, LPARAM lParam)
{
	if (!wParam)
	{
		// customization mode is about to be deactivated
		RebuildToolsMenu();
	}

	return 0L;
}


void CMainFrame::OnUpdateUserTool(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
}


void CMainFrame::OnExecuteUserTool(UINT nIDEvent)
{
	theApp.GetUserToolsManager()->InvokeTool(nIDEvent);
}


void CMainFrame::OnToolsCancel() 
{
	//Dependent on the current mode, we do some fancy stuff here.
	// Current edit view does NOT have focus: It gets it.
	// Current edit view is in incremental search mode: Cancel search.
	// Current edit view does have focus: Close output bar.

	//Get active frame
	CFrameWnd* pFrame = GetActiveFrame();
	if (!pFrame) return;

	//Get active view/edit
	CLatexEdit* pEdit = NULL;
	CView* pView = pFrame->GetActiveView();
	if (pView) pEdit = dynamic_cast<CLatexEdit*>(pView);
	if (!pEdit) return;

	if (GetFocus() != pEdit)
	{
		//Activate view
		pEdit->SetFocus();
	}
	else
	{
		//Cancel incremental search. Returns false, if it was not in search mode.
		if (!(pEdit->OnEditFindIncrementalStop(false)))
		{
			//Close output bar
			ASSERT(m_wndOutputBar);
			ASSERT(IsWindow(m_wndOutputBar.m_hWnd));
			if (IsControlBarVisible(&m_wndOutputBar))
			{
				ShowControlBar(&m_wndOutputBar, FALSE, FALSE);
			}
		}
	}
}

void CMainFrame::OnFileClose() 
{
	// TODO: Add your command handler code here
	if (m_pTargetWindow != NULL) {
		TRACE("OnFileClose :-) %s\n", m_pTargetWindow->GetDocument()->GetPathName());
		m_pTargetWindow->GetDocument()->OnCloseDocument();
		m_pTargetWindow = NULL;
	}
}

