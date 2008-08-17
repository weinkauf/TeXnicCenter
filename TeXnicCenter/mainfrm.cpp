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
#include "MainFrm.h"

#include "global.h"
#include "TeXnicCenter.h"
#include "BuildView.h"
#include "GrepView.h"
#include "Configuration.h"
#include "Splash.h"
#include "BCGToolbarCustomizeEx.h"
#include "UserTool.h"
#include "ProfileDialog.h"
#include "RunTimeHelper.h"
#include "LaTeXDocument.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// class CMainFrame
//-------------------------------------------------------------------

enum
{
	StartPaneAnimation = WM_USER + 1,
	StopPaneAnimation = WM_USER
};

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
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
	ON_COMMAND(ID_WINDOW_REFERENCES, OnWindowReferences)
	ON_COMMAND(ID_WINDOW_PARSE, OnWindowParse)
	ON_COMMAND(ID_WINDOW_CLOSE_SELECTEDTAB, OnWindowCloseSelectedTab)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_CLOSE_SELECTEDTAB, OnUpdateWindowCloseSelectedTab)
	ON_COMMAND(ID_WINDOW_CLOSE_ALL_BUTACTIVE, OnWindowCloseAllButActive)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_CLOSE_ALL_BUTACTIVE, OnUpdateWindowCloseAllButActive)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DOCTAB_OFF, OnUpdateViewDocTabs)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DOCTAB_TOP, OnUpdateViewDocTabs)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DOCTAB_ICONS, OnUpdateViewDocTabs)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DOCTAB_NOTE, OnUpdateViewDocTabs)
	//}}AFX_MSG_MAP
	// Globale Hilfebefehle
	ON_COMMAND(ID_HELP_FINDER, &CMDIFrameWndEx::OnHelpFinder)
	ON_COMMAND(ID_HELP, &CMDIFrameWndEx::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CMDIFrameWndEx::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CMDIFrameWndEx::OnHelpFinder)
	ON_COMMAND_EX_RANGE(ID_MATH1, ID_MATH16, OnToggleMathBar)
	//ON_COMMAND_EX_RANGE(ID_VIEW_USER_TOOLBAR_FIRST, ID_VIEW_USER_TOOLBAR_LAST, OnToggleCtrlBar)
	//ON_COMMAND_EX(ID_VIEW_MATH, OnToggleCtrlBar)
	//ON_COMMAND_EX(ID_VIEW_NAVIGATION, OnToggleCtrlBar)
	//ON_COMMAND_EX(ID_VIEW_OUTPUT, OnToggleCtrlBar)
	//ON_COMMAND_EX(ID_VIEW_TOOLBAR, OnToggleCtrlBar)
	//ON_COMMAND_EX(ID_VIEW_FORMAT, OnToggleCtrlBar)
	//ON_COMMAND_EX(ID_VIEW_FORMAT_RECENTLY_USED, OnToggleCtrlBar)
	//ON_COMMAND_EX(ID_VIEW_INSERT, OnToggleCtrlBar)
	//ON_COMMAND_EX(ID_VIEW_LATEX, OnToggleCtrlBar)
	//ON_COMMAND_EX(ID_VIEW_STATUS_BAR, OnToggleCtrlBar)
	//ON_COMMAND_EX(ID_VIEW_MENU_BAR, OnToggleCtrlBar)
	//ON_COMMAND_EX(ID_VIEW_FIND, OnToggleCtrlBar)
	ON_UPDATE_COMMAND_UI_RANGE(ID_MATH1, ID_MATH16, OnCheckCtrlBarVisible)
	//ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_USER_TOOLBAR_FIRST, ID_VIEW_USER_TOOLBAR_LAST, OnCheckCtrlBarVisible)
	//ON_UPDATE_COMMAND_UI(ID_VIEW_MATH, OnCheckCtrlBarVisible)
	//ON_UPDATE_COMMAND_UI(ID_VIEW_NAVIGATION, OnCheckCtrlBarVisible)
	//ON_UPDATE_COMMAND_UI(ID_VIEW_OUTPUT, OnCheckCtrlBarVisible)
	//ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR, OnCheckCtrlBarVisible)
	//ON_UPDATE_COMMAND_UI(ID_VIEW_FORMAT, OnCheckCtrlBarVisible)
	//ON_UPDATE_COMMAND_UI(ID_VIEW_FORMAT_RECENTLY_USED, OnCheckCtrlBarVisible)
	//ON_UPDATE_COMMAND_UI(ID_VIEW_INSERT, OnCheckCtrlBarVisible)
	//ON_UPDATE_COMMAND_UI(ID_VIEW_LATEX, OnCheckCtrlBarVisible)
	//ON_UPDATE_COMMAND_UI(ID_VIEW_STATUS_BAR, OnCheckCtrlBarVisible)
	//ON_UPDATE_COMMAND_UI(ID_VIEW_MENU_BAR, OnCheckCtrlBarVisible)
	//ON_UPDATE_COMMAND_UI(ID_VIEW_FIND, OnCheckCtrlBarVisible)
	//ON_REGISTERED_MESSAGE(AFX_WM_TOOLBARMENU, OnToolbarContextMenu)
	ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR, OnResetToolbar)
	ON_REGISTERED_MESSAGE(AFX_WM_CUSTOMIZETOOLBAR, OnCustomizationMode)
	ON_UPDATE_COMMAND_UI_RANGE(ID_USER_TOOL_FIRST, ID_USER_TOOL_LAST, OnUpdateUserTool)
	ON_COMMAND_RANGE(ID_USER_TOOL_FIRST, ID_USER_TOOL_LAST, OnExecuteUserTool)
	ON_COMMAND_RANGE(ID_VIEW_APP_LOOK_WIN_2000,ID_VIEW_APP_LOOK_OFFICE_2007_AQUA, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APP_LOOK_WIN_2000, ID_VIEW_APP_LOOK_OFFICE_2007_AQUA, &CMainFrame::OnUpdateApplicationLook)
	ON_MESSAGE_VOID(StartPaneAnimation,OnStartPaneAnimation)
	ON_MESSAGE_VOID(StopPaneAnimation,OnStopPaneAnimation)
END_MESSAGE_MAP()

const UINT BuildAnimationPane = 1;


static UINT indicators[] =
{
	ID_SEPARATOR, // Statusleistenanzeige
	IDB_BUILD_ANIMATION, // Build animation
	ID_EDIT_INDICATOR_POSITION,
	ID_EDIT_INDICATOR_ENCODING,
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
		: m_pContextMenuTargetWindow(NULL)
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	OnApplicationLook(theApp.GetApplicationLook());

	// load user images
	if (!m_userImages.Load(theApp.GetWorkingDir() + _T("\\UserImages.bmp")))
	{
		TRACE(_T("Failed to load user images\n"));
		//ASSERT(FALSE);
	}
	else
	{
		CMFCToolBar::SetUserImages(&m_userImages);
	}

	// create menu bar
	if (!CConfiguration::GetInstance()->m_bOptimizeMenuForVisuallyHandicappedUsers)
	{
		if (!m_wndMenuBar.Create(this))
		{
			TRACE0("Failed to create menubar\n");
			return -1;
		}

		m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() |
		                          CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);
	}
	else
	{
		// load menu bars
		m_stdMenu.LoadMenu(IDR_MAINFRAME);
		m_projectMenu.LoadMenu(IDR_LATEXPROJECT);
		MDISetMenu(&m_stdMenu, NULL);
	}

	// create status bar
	if (!m_wndStatusBar.Create(this) ||
	        !m_wndStatusBar.SetIndicators(indicators,
	                                      sizeof(indicators) / sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1; // creation failed
	}

	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Allow disabled menu items to be highlighted...
	//CMFCMenuBar::HighlightDisabledItems();
	CMFCToolBar::EnableQuickCustomization();
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	// create main tool bar
	if (!CreateToolBar(&m_wndToolBar, IDR_MAINFRAME, STE_TB_MAINFRAME))
		return -1;

	// create find bar
	if (!CreateToolBar(&m_wndFindBar, IDR_FIND, STE_TB_FIND))
		return -1;

	// create insert bar
	if (!CreateToolBar(&m_wndInsertBar, IDR_INSERT, STE_TB_INSERT))
		return -1;

	// create format recently used tool bar
	if (!CreateToolBar(&m_wndFormatRUBar, IDR_FORMAT_RECENTLY_USED, STE_TB_FORMAT_RECENTLY_USED))
		return -1;

	// create format tool bar
	if (!CreateToolBar(&m_wndFormatBar, IDR_FORMAT, STE_TB_FORMAT))
		return -1;

	// create latex tool bar
	if (!CreateToolBar(&m_wndLatexBar, IDR_LATEX, STE_TB_LATEX))
		return -1;

	// create math tool bar
	if (!CreateToolBar(&m_wndMathBar, IDR_MATH, STE_TB_MATH))
		return -1;

	// create all math bars
	for (int i = 0; i < MATHBAR_COUNT; i++)
		if (!CreateToolBar(&m_awndMathBar[i], IDR_MATHBAR1 + i, STE_TB_MATH1 + i))
			return -1;

	// load user defined toolbars
	InitUserToolbars(NULL, IDR_USER_TOOLBAR_FIRST, IDR_USER_TOOLBAR_LAST);

	// initialization
	CMFCToolBar::AddToolBarForImageCollection(IDR_IMAGES);
	CMFCToolBar::AddToolBarForImageCollection(IDR_ADDITIONAL_COMMANDS);
	CMFCToolBar::AddToolBarForImageCollection(IDR_NAVIGATION_BAR);
	CMFCToolBar::AddToolBarForImageCollection(IDR_OUTPUT_BAR);

	// dock the bars
	if (!CConfiguration::GetInstance()->m_bOptimizeMenuForVisuallyHandicappedUsers)
		m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndFindBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndInsertBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndFormatRUBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndFormatBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndLatexBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndMathBar.EnableDocking(CBRS_ALIGN_ANY);

	EnableFullScreenMode(ID_VIEW_FULLSCREEN);
	EnableDocking(CBRS_ALIGN_ANY);

	for (int i = 0; i < MATHBAR_COUNT; i++)
		m_awndMathBar[i].EnableDocking(CBRS_ALIGN_ANY);

	if (!CConfiguration::GetInstance()->m_bOptimizeMenuForVisuallyHandicappedUsers)
		DockPane(&m_wndMenuBar);

	DockPane(&m_wndLatexBar);
	DockPaneLeftOf(&m_wndToolBar,&m_wndLatexBar);

	DockPane(&m_wndFindBar);
	DockPaneLeftOf(&m_wndFormatRUBar,&m_wndFindBar);
	DockPaneLeftOf(&m_wndInsertBar,&m_wndFormatRUBar);

	DockPane(&m_wndMathBar);
	DockPaneLeftOf(&m_wndFormatBar,&m_wndMathBar);

	for (int i = 0; i < MATHBAR_COUNT; ++i)
		DockPane(m_awndMathBar + i);

	CreateNavigationViews();
	CreateOutputViews();

	// loading toolbars
	//CString strControlBarRegEntry = theApp.m_strRegistryRoot + _T("\\CtrlBar\\");

	// Enable window list manager...
	if (!CConfiguration::GetInstance()->m_bOptimizeMenuForVisuallyHandicappedUsers)
		EnableWindowsDialog(ID_WINDOW_LIST, ID_WINDOW_LIST_MENU, TRUE);
	else
	{
		// add entry for window list to the end of window menu
		CMenu *pmnuWindowMenu = m_stdMenu.GetSubMenu(m_stdMenu.GetMenuItemCount() - 2);
		pmnuWindowMenu->AppendMenu(MF_SEPARATOR);
		pmnuWindowMenu->AppendMenu(MF_BYPOSITION, ID_WINDOW_LIST, CString((LPCTSTR)ID_WINDOW_LIST_MENU));
	}

	EnablePaneMenu(TRUE, ID_EXTRAS_CUSTOMIZE,
	               CString(MAKEINTRESOURCE(IDS_TOOLBAR_CUSTOMIZE)),ID_VIEW_TOOLBAR,TRUE,FALSE);

	build_animation_.m_hImageList = ::ImageList_LoadImage(AfxGetResourceHandle(),MAKEINTRESOURCE(IDB_BUILD_ANIMATION),16,1,
	                                RGB(192,192,192),IMAGE_BITMAP,LR_CREATEDIBSECTION);

	// Recalculate layout
	m_wndStatusBar.SetPaneText(BuildAnimationPane,0);

	m_wndStatusBar.SetPaneAnimation(BuildAnimationPane,build_animation_);
	m_wndStatusBar.SetPaneAnimation(BuildAnimationPane,0);

	int cx, cy;

	if (::ImageList_GetIconSize(build_animation_,&cx,&cy))
		m_wndStatusBar.SetPaneWidth(BuildAnimationPane,cx); // Adjust the width

	m_wndStatusBar.SetPaneStyle(BuildAnimationPane,SBPS_NOBORDERS);

	//CList<UINT, UINT> lstBasicCommands;

	//lstBasicCommands.AddTail(ID_FILE_NEW);
	//lstBasicCommands.AddTail(ID_FILE_OPEN);
	//lstBasicCommands.AddTail(ID_FILE_SAVE);
	//lstBasicCommands.AddTail(ID_FILE_PRINT);
	//lstBasicCommands.AddTail(ID_APP_EXIT);
	//lstBasicCommands.AddTail(ID_EDIT_CUT);
	//lstBasicCommands.AddTail(ID_EDIT_PASTE);
	//lstBasicCommands.AddTail(ID_EDIT_UNDO);
	//lstBasicCommands.AddTail(ID_APP_ABOUT);
	//lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	//lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);

	//CMFCToolBar::SetBasicCommands(lstBasicCommands);

	SendMessage(WM_UPDATEUISTATE, MAKEWPARAM(UIS_SET, UISF_HIDEACCEL | UISF_HIDEFOCUS));

	return 0;
}

void CMainFrame::OnClose()
{
	// If we are still in full screen mode, restore normal mode before closing
	// to ensure proper control position saving.
	if (IsFullScreen())
		ShowFullScreen();

	theApp.EndSession();

	CMDIFrameWndEx::OnClose();
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CMDIFrameWndEx::PreCreateWindow(cs))
		return FALSE;
	return TRUE;
}

BOOL CMainFrame::SetMenu(CMenu *pMenu)
{
	CMDIFrameWnd::SetMenu(pMenu);
	//if (!CConfiguration::GetInstance()->m_bOptimizeMenuForVisuallyHandicappedUsers)
	//	m_wndMenuBar.CreateFromMenu( pMenu->m_hMenu, FALSE );
	return TRUE;
}

BOOL CMainFrame::CreateToolBar(CMFCToolBar *pToolBar, UINT unID, UINT unTitleID, BOOL bVisible /*= TRUE*/)
{
	DWORD style = AFX_DEFAULT_TOOLBAR_STYLE;

	if (!bVisible)
		style &= ~WS_VISIBLE;

	if (!pToolBar->Create(this, style, unID) ||
	        !pToolBar->LoadToolBar(unID))
	{
		TRACE1("!Failed to create %s tool bar\n", AfxLoadString(unTitleID));
		return FALSE; // creation failed
	}

	pToolBar->SetWindowText(AfxLoadString(unTitleID));
	pToolBar->EnableTextLabels(FALSE);
	pToolBar->EnableCustomizeButton(TRUE, ID_EXTRAS_CUSTOMIZE, IDS_TOOLBAR_CUSTOMIZE);

	return TRUE;
}

void CMainFrame::ToggleControlBar(CBasePane* pCtrlBar)
{
	ASSERT(pCtrlBar);
	ASSERT(IsWindow(pCtrlBar->m_hWnd));

	// control bars for math symbols are exclusive
	ShowPane(pCtrlBar,!IsControlBarVisible(pCtrlBar), FALSE, TRUE);
}

BOOL CMainFrame::IsControlBarVisible(CBasePane* pCtrlBar)
{
	ASSERT(pCtrlBar);
	ASSERT(IsWindow(pCtrlBar->m_hWnd));
	return pCtrlBar->IsPaneVisible();//(pCtrlBar->GetStyle() & WS_VISIBLE);
}

CBasePane* CMainFrame::GetControlBarByCmd(UINT unID)
{
	CBasePane* result;

	if (unID >= ID_MATH1 && unID <= ID_MATH16)
		result = &m_awndMathBar[unID - ID_MATH1];
	else if (unID >= ID_VIEW_USER_TOOLBAR_FIRST && unID <= ID_VIEW_USER_TOOLBAR_LAST)
		result = GetUserToolBarByIndex(unID - ID_VIEW_USER_TOOLBAR_FIRST);
	else
		result = GetPane(unID);

	return result;

	//// one of the math-symbol bars?
	//if (unID >= ID_MATH1 && unID <= ID_MATH16)
	//    return &m_awndMathBar[unID - ID_MATH1];

	//// one of the user defined bars?
	//if (unID >= ID_VIEW_USER_TOOLBAR_FIRST && unID <= ID_VIEW_USER_TOOLBAR_LAST)
	//    return GetUserToolBarByIndex(unID - ID_VIEW_USER_TOOLBAR_FIRST);

	//switch (unID) {
	//    case ID_VIEW_MATH:
	//        return &m_wndMathBar;
	//    case ID_VIEW_FILES_PANE:
	//        return &file_view_pane_;
	//    case ID_VIEW_STRUCT_PANE:
	//        return &structure_view_;
	//    case ID_VIEW_BIB_ENTRIES_PANE:
	//        return &bib_view_pane_;
	//    case ID_VIEW_ENV_PANE:
	//        return &env_view_pane_;
	//    case ID_VIEW_BUILD_PANE:
	//        return &build_view_pane_;
	//    case ID_VIEW_GREP_1_PANE:
	//        return &grep_view_1_pane_;
	//    case ID_VIEW_GREP_2_PANE:
	//        return &grep_view_2_pane_;
	//    case ID_VIEW_PARSE_PANE:
	//        return &parse_view_pane_;
	//    case ID_VIEW_TOOLBAR:
	//        return &m_wndToolBar;
	//    case ID_VIEW_FIND:
	//        return &m_wndFindBar;
	//    case ID_VIEW_INSERT:
	//        return &m_wndInsertBar;
	//    case ID_VIEW_FORMAT:
	//        return &m_wndFormatBar;
	//    case ID_VIEW_FORMAT_RECENTLY_USED:
	//        return &m_wndFormatRUBar;
	//    case ID_VIEW_LATEX:
	//        return &m_wndLatexBar;
	//    case ID_VIEW_STATUS_BAR:
	//        return &m_wndStatusBar;
	//    case ID_VIEW_MENU_BAR:
	//        return &m_wndMenuBar;
	//    default:
	//        return NULL;
	//}
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame Diagnose

#ifdef _DEBUG

void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame Nachrichten-Handler

BOOL CMainFrame::DestroyWindow()
{
	return CMDIFrameWndEx::DestroyWindow();
}

void CMainFrame::OnExtrasCustomize()
{
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// create dialog object

	MyToolBarCustomize* pDlgCust = new MyToolBarCustomize(
	    this, TRUE,
	    AFX_CUSTOMIZE_MENU_SHADOWS | AFX_CUSTOMIZE_TEXT_LABELS |
	    AFX_CUSTOMIZE_MENU_ANIMATIONS | AFX_CUSTOMIZE_NOHELP);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// enable create/delete of user defined toolbars
	pDlgCust->EnableUserDefinedToolbars();

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// add menus, toolbars and commands
	pDlgCust->AddMenu(IDR_LATEXPROJECT);
	pDlgCust->AddMenu(IDR_MAINFRAME);
	pDlgCust->AddMenu(IDR_HIDDEN_CMDS);
	pDlgCust->AddMenu(IDR_POPUP_ITEM);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// create combobox for output typess

	CMFCToolBarComboBoxButton profileButton(
	    ID_LATEX_PROFILE_SEL,
	    1,
	    //CImageHash::GetImageOfCommand( ID_LATEX_PROFILE_SEL, FALSE ),
	    CBS_DROPDOWNLIST | CBS_SORT);

	// add available outputtypes to combo box
	CStringArray astrProfiles;
	CProfileMap::GetInstance()->GetKeyList(astrProfiles);

	for (int i = 0; i < astrProfiles.GetSize(); i++)
		profileButton.AddItem(astrProfiles[i]);

	profileButton.SelectItem(CProfileMap::GetInstance()->GetActiveProfileKey());

	// replace button
	pDlgCust->ReplaceButton(ID_LATEX_PROFILE_SEL, profileButton);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// display dialog
	pDlgCust->Create();

	theApp.UpdateLaTeXProfileSel();
}

void CMainFrame::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
		case timerSave: // save and parse project
			if (CConfiguration::GetInstance()->m_unSaveInterval < 1)
				CConfiguration::GetInstance()->m_unSaveInterval = 1;

			theApp.SaveAllModifiedWithoutPrompt();
			break;
	}

	CMDIFrameWndEx::OnTimer(nIDEvent);
}

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CMDIFrameWndEx::OnActivate(nState, pWndOther, bMinimized);

	// Check if files have changed
	CMultiDocTemplate *pTemplate = theApp.GetLatexDocTemplate();

	if (pTemplate && (nState == WA_ACTIVE || nState == WA_CLICKACTIVE))
	{
		POSITION pos = pTemplate->GetFirstDocPosition();

		while (pos)
		{
			LaTeXDocument *pDoc = dynamic_cast<LaTeXDocument*>(pTemplate->GetNextDoc(pos));

			if (pDoc)
				pDoc->CheckForFileChanges();
		}
	}

	// activate editor
	if (nState == WA_ACTIVE || nState == WA_CLICKACTIVE)
	{
		CView *pView = theApp.GetActiveEditView();

		if (pView)
			pView->SetFocus();
	}
}

void CMainFrame::OnOptionsChanged()
{
	// install/uninstall timer for automatic saving
	KillTimer(timerSave);

	if (CConfiguration::GetInstance()->m_bSaveAutomatic)
		SetTimer(timerSave, CConfiguration::GetInstance()->m_unSaveInterval * 1000 * 60, NULL);
}

BOOL CMainFrame::OnToggleCtrlBar(UINT nIDEvent)
{
	CBasePane* pCtrlBar = GetControlBarByCmd(nIDEvent);
	ASSERT(pCtrlBar);
	ASSERT(IsWindow(pCtrlBar->m_hWnd));
	ToggleControlBar(pCtrlBar);
	return TRUE;
}

BOOL CMainFrame::OnToggleMathBar(UINT nIDEvent)
{
	CBasePane* pCtrlBar = GetControlBarByCmd(nIDEvent);
	ASSERT(pCtrlBar);
	ASSERT(IsWindow(pCtrlBar->m_hWnd));

	ShowPane(pCtrlBar,!pCtrlBar->IsPaneVisible(), FALSE, TRUE);

	return TRUE;
}

void CMainFrame::OnCheckCtrlBarVisible(CCmdUI *pCmdUI)
{
	CBasePane* pCtrlBar = GetControlBarByCmd(pCmdUI->m_nID);
	//if (pCtrlBar) {
	ASSERT(pCtrlBar);
	ASSERT(IsWindow(pCtrlBar->m_hWnd));
	pCmdUI->SetCheck(IsControlBarVisible(pCtrlBar));
	//}
}

LRESULT CMainFrame::OnToolbarContextMenu(WPARAM /*wp*/, LPARAM lp)
{
	CPoint point(LOWORD(lp), HIWORD(lp));

	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_POPUP_TOOLBAR));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	// Add user-defined toolbar names to the end
	for (int i = 0; i < (IDR_USER_TOOLBAR_LAST - IDR_USER_TOOLBAR_FIRST + 1); i++)
	{
		CMFCToolBar *pToolBar = GetUserToolBarByIndex(i);
		if (pToolBar != NULL)
		{
			// add title of current tool bar to menu
			CString strToolbarName;
			pToolBar->GetWindowText(strToolbarName);
			pPopup->InsertMenu(pPopup->GetMenuItemCount() - 2, MF_STRING | MF_BYPOSITION, ID_VIEW_USER_TOOLBAR_FIRST + i, strToolbarName);
		}
	}

	// display menu
	CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;
	pPopupMenu->Create(this, point.x, point.y, pPopup->Detach());

	return 0;
}

void CMainFrame::OnWindowEditor()
{
	// get active frame
	CFrameWnd *pFrame = GetActiveFrame();

	if (!pFrame)
		return;

	// get active view
	CView *pView = pFrame->GetActiveView();

	if (!pView || !pView->IsKindOf(RUNTIME_CLASS(LaTeXView)))
		return;

	// activate view
	pView->SetFocus();
}

void CMainFrame::OnWindowNavigator()
{
	//if (!IsWindow(m_wndNavigationBar.m_hWnd) || !IsWindow(m_wndNavigationBar.GetTabWnd().m_hWnd))
	//    return;

	//// make bar visible if it is not
	//if (!m_wndNavigationBar.IsVisible())
	//    ShowPane(&m_wndNavigationBar, TRUE, FALSE, FALSE);

	//// get active window and set focus on it
	//CWnd* pwnd = m_wndNavigationBar.GetTabWnd().GetActiveWnd();

	//if (!pwnd || !IsWindow(pwnd->m_hWnd))
	//    return;

	//pwnd->SetFocus();
}

void CMainFrame::ActivateNavigationTab(int /*nTab*/)
{
	//if (!IsWindow(m_wndNavigationBar.m_hWnd) || !IsWindow(m_wndNavigationBar.GetTabWnd().m_hWnd))
	//    return;

	//// select structure tab
	//if (m_wndNavigationBar.GetTabWnd().GetActiveTab() != nTab)
	//    m_wndNavigationBar.GetTabWnd().SetActiveTab(nTab);

	// Activate navigator window
	OnWindowNavigator();
}

void CMainFrame::OnWindowStructure()
{
	ActivateNavigationTab(navigatorTabStructure);
}

void CMainFrame::OnWindowEnvironment()
{
	ActivateNavigationTab(navigatorTabEnvironments);
}

void CMainFrame::OnWindowFiles()
{
	ActivateNavigationTab(navigatorTabFiles);
}

void CMainFrame::OnWindowReferences()
{
	ActivateNavigationTab(navigatorTabReferences);
}

void CMainFrame::OnWindowOutput()
{
	//ActivateOutputBar(true);
}

void CMainFrame::ActivateOutputTab(int nTab, bool /*bSetFocus*/)
{
	CDockablePane* const panes[] = {&build_view_pane_,&grep_view_1_pane_,&grep_view_2_pane_,&parse_view_pane_,&error_list_view_};
	const int count = sizeof(panes) / sizeof(*panes);

	if (nTab >= 0 && nTab < count && (panes[nTab]->IsVisible() || panes[nTab]->IsAutoHideMode()))
	{
		// Special case: Activate the build view only if the error list view isn't active
		if (nTab != outputTabBuildResult || !error_list_view_.IsWindowVisible())
			panes[nTab]->ShowPane(TRUE,TRUE,TRUE);
	}
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

BOOL CMainFrame::ReplaceToolbarButton(UINT unCmdID, const CMFCToolBarButton &button)
{
	// replace the first occurrence of the button
	if (m_wndFindBar.CommandToIndex(unCmdID) > -1)
	{
		m_wndFindBar.ReplaceButton(unCmdID, button);
		return TRUE;
	}

	if (m_wndFormatBar.CommandToIndex(unCmdID) > -1)
	{
		m_wndFormatBar.ReplaceButton(unCmdID, button);
		return TRUE;
	}

	if (m_wndFormatRUBar.CommandToIndex(unCmdID) > -1)
	{
		m_wndFormatRUBar.ReplaceButton(unCmdID, button);
		return TRUE;
	}

	if (m_wndInsertBar.CommandToIndex(unCmdID) > -1)
	{
		m_wndInsertBar.ReplaceButton(unCmdID, button);
		return TRUE;
	}

	if (m_wndLatexBar.CommandToIndex(unCmdID) > -1)
	{
		m_wndLatexBar.ReplaceButton(unCmdID, button);
		return TRUE;
	}

	if (m_wndMathBar.CommandToIndex(unCmdID) > -1)
	{
		m_wndMathBar.ReplaceButton(unCmdID, button);
		return TRUE;
	}

	if (m_wndToolBar.CommandToIndex(unCmdID) > -1)
	{
		m_wndToolBar.ReplaceButton(unCmdID, button);
		return TRUE;
	}

	for (int i = 0; i < MATHBAR_COUNT; i++)
	{
		if (m_awndMathBar[i].CommandToIndex(unCmdID) > -1)
		{
			m_awndMathBar[i].ReplaceButton(unCmdID, button);
			return TRUE;
		}
	}

	// button not found
	return FALSE;
}

CMFCToolBarButton *CMainFrame::GetToolBarButton(UINT unCmdID, POSITION &pos) const
{
	// find and return specified occurence of the button
	int nIndex;
	int nStartBar = (int)pos;

	if (nStartBar < 1 && (nIndex = m_wndFindBar.CommandToIndex(unCmdID)) > -1)
	{
		pos = (POSITION)1;
		return m_wndFindBar.GetButton(nIndex);
	}

	if (nStartBar < 2 && (nIndex = m_wndFormatBar.CommandToIndex(unCmdID)) > -1)
	{
		pos = (POSITION)2;
		return m_wndFormatBar.GetButton(nIndex);
	}

	if (nStartBar < 3 && (nIndex = m_wndFormatRUBar.CommandToIndex(unCmdID)) > -1)
	{
		pos = (POSITION)3;
		return m_wndFormatRUBar.GetButton(nIndex);
	}

	if (nStartBar < 4 && (nIndex = m_wndInsertBar.CommandToIndex(unCmdID)) > -1)
	{
		pos = (POSITION)4;
		return m_wndInsertBar.GetButton(nIndex);
	}

	if (nStartBar < 5 && (nIndex = m_wndLatexBar.CommandToIndex(unCmdID)) > -1)
	{
		pos = (POSITION)5;
		return m_wndLatexBar.GetButton(nIndex);
	}

	if (nStartBar < 6 && (nIndex = m_wndMathBar.CommandToIndex(unCmdID)) > -1)
	{
		pos = (POSITION)6;
		return m_wndMathBar.GetButton(nIndex);
	}

	if (nStartBar < 7 && (nIndex = m_wndToolBar.CommandToIndex(unCmdID)) > -1)
	{
		pos = (POSITION)7;
		return m_wndToolBar.GetButton(nIndex);
	}

	//Look for the button in the Math-Toolbars
	if (nStartBar < 8 + MATHBAR_COUNT - 1)
	{
		for (int i = 0; i < MATHBAR_COUNT; i++)
		{
			if (nStartBar < (i + 8) && (nIndex = m_awndMathBar[i].CommandToIndex(unCmdID)) > -1)
			{
				pos = (POSITION)(i + 8);
				return m_awndMathBar[i].GetButton(nIndex);
			}
		}
	}

	//Find the Toolbutton in the UserToolbars
	for (int i = 0; i < (IDR_USER_TOOLBAR_LAST - IDR_USER_TOOLBAR_FIRST + 1); i++)
	{
		CMFCToolBar *pToolBar = GetUserToolBarByIndex(i);

		if (!pToolBar)
			continue;

		if (nStartBar < (i + 8 + MATHBAR_COUNT) && (nIndex = pToolBar->CommandToIndex(unCmdID)) > -1)
		{
			pos = (POSITION)(i + 8 + MATHBAR_COUNT);
			return pToolBar->GetButton(nIndex);
		}
	}


	// button not found
	return NULL;
}

LRESULT CMainFrame::OnResetToolbar(WPARAM wParam, LPARAM /*lParam*/)
{
	const UINT id = wParam;
	// modify toolbar buttons

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// create combobox for output types
	CMFCToolBarComboBoxButton profileButton(
	    ID_LATEX_PROFILE_SEL,
	    -1, //CImageHash::GetImageOfCommand( ID_LATEX_PROFILE_SEL, FALSE ),
	    CBS_DROPDOWNLIST | CBS_SORT);

	// add available outputtypes to combo box
	CStringArray astrProfiles;
	CProfileMap::GetInstance()->GetKeyList(astrProfiles);

	for (int i = 0; i < astrProfiles.GetSize(); i++)
		profileButton.AddItem(astrProfiles[i]);

	profileButton.SelectItem(CProfileMap::GetInstance()->GetActiveProfileKey());

	// replace button
	ReplaceToolbarButton(ID_LATEX_PROFILE_SEL, profileButton);

	if (m_wndFormatRUBar && id == m_wndFormatRUBar.GetDlgCtrlID())
	{
		CMFCColorMenuButton b1(ID_FORMAT_TEXT_BACK_COLOR,_T("Test"));
		b1.EnableDocumentColors(_T("Default"));
		b1.EnableAutomaticButton(_T("Automatic"),0);
		b1.EnableOtherButton(_T("Other"));
		b1.EnableTearOff(IDR_TEXT_BACK_COLOR);

		m_wndFormatRUBar.ReplaceButton(b1.m_nID,b1);

		CMFCColorMenuButton b2(ID_FORMAT_TEXT_FORE_COLOR,_T("Test"));
		b2.EnableDocumentColors(_T("Default"));
		b2.EnableAutomaticButton(_T("Automatic"),0);
		b2.EnableOtherButton(_T("Other"));
		b2.EnableTearOff(IDR_TEXT_FORE_COLOR);

		m_wndFormatRUBar.ReplaceButton(b2.m_nID,b2);
	}

	return 0L;
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// forward notifications from ComboBoxes as command
	if (HIWORD(wParam) == CBN_SELENDOK || HIWORD(wParam) == CBN_EDITCHANGE)
		SendMessage(WM_COMMAND, (WPARAM)LOWORD(wParam));

	return CMDIFrameWndEx::OnCommand(wParam, lParam);
}

void CMainFrame::OnWindowList()
{
	// Display the window list dialog...
	ShowWindowsDialog();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	//CWnd* pWndFocus = GetFocus();

	//if ((pWndFocus != NULL) &&
	//        (/*(m_wndNavigationBar.IsChild(pWndFocus)) ||*/
	//        (m_wndOutputBar.IsChild(pWndFocus)))) {
	//    // see if the window can handle this message
	//    if (pWndFocus->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo)) {
	//        return TRUE;
	//    }
	//}

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

	if (output_doc_.OnCmdMsg(nID,nCode,pExtra,pHandlerInfo))
		return TRUE;

	//if ((m_wndOutputBar.GetOutputDoc() != NULL) &&
	//        m_wndOutputBar.GetOutputDoc()->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
	//    return TRUE;

	return CMDIFrameWndEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::GetControlBarsEx(CArray<CPane*, CPane*>& arrBars)
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
	if (m_wndMathBar)
		arrBars.Add(&m_wndMathBar);
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
		CPane* pToolBar = GetUserToolBarByIndex(j);

		if (pToolBar != NULL)
			arrBars.Add(pToolBar);
	}
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext)
{
	if (!CMDIFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	for (int i = 0; i < MATHBAR_COUNT; ++i)
		if (!m_awndMathBar[i].IsRestoredFromRegistry())
			ShowPane(m_awndMathBar + i,FALSE,TRUE,FALSE);

	RecalcLayout();

	DisplayDocumentTabs();

	return TRUE;
}

void CMainFrame::OnHelpContents()
{
	HtmlHelp(0,HH_DISPLAY_TOC);
}

void CMainFrame::OnHelpSearch()
{
	HH_FTS_QUERY query = {sizeof(HH_FTS_QUERY)};
	HtmlHelp(reinterpret_cast<DWORD>(&query),HH_DISPLAY_SEARCH);
}

void CMainFrame::OnHelpIndex()
{
	HtmlHelp(0,HH_DISPLAY_INDEX);
}

void CMainFrame::DisplayDocumentTabs()
{
	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_SCROLLED; // other styles available...
	mdiTabParams.m_bActiveTabCloseButton = TRUE; // set to FALSE to place close button at right of tab area
	mdiTabParams.m_bTabIcons = theApp.m_bMDITabIcons; // set to TRUE to enable document icons on MDI taba
	mdiTabParams.m_bAutoColor = FALSE; // set to FALSE to disable auto-coloring of MDI tabs
	mdiTabParams.m_bDocumentMenu = TRUE; // enable the document menu at the right edge of the tab area
	mdiTabParams.m_tabLocation = static_cast<CMFCTabCtrl::Location>(theApp.m_nMDITabLocation);
	mdiTabParams.m_nTabBorderSize = 3;

	EnableMDITabbedGroups(theApp.m_bMDITabs, mdiTabParams);
}

void CMainFrame::OnViewDocTabsBottom()
{
	theApp.m_bMDITabs = true;
	theApp.m_nMDITabLocation = CMFCTabCtrl::LOCATION_BOTTOM;
	DisplayDocumentTabs();
}

void CMainFrame::OnViewDocTabsTop()
{
	theApp.m_bMDITabs = true;
	theApp.m_nMDITabLocation = CMFCTabCtrl::LOCATION_TOP;
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
	if (theApp.m_nMDITabStyle == CMFCTabCtrl::STYLE_3D_SCROLLED)
	{
		theApp.m_nMDITabStyle = CMFCTabCtrl::STYLE_3D_ONENOTE;
	}
	else
	{
		theApp.m_nMDITabStyle = CMFCTabCtrl::STYLE_3D_SCROLLED;
	}

	DisplayDocumentTabs();
}

void CMainFrame::OnUpdateViewDocTabs(CCmdUI* pCmdUI)
{
	switch (pCmdUI->m_nID)
	{
		case ID_VIEW_DOCTAB_BOTTOM:
			pCmdUI->SetRadio(theApp.m_bMDITabs && (theApp.m_nMDITabLocation == CMFCTabCtrl::LOCATION_BOTTOM));
			break;

		case ID_VIEW_DOCTAB_TOP:
			pCmdUI->SetRadio(theApp.m_bMDITabs && (theApp.m_nMDITabLocation == CMFCTabCtrl::LOCATION_TOP));
			break;

		case ID_VIEW_DOCTAB_OFF:
			pCmdUI->SetRadio(!theApp.m_bMDITabs);
			break;

		case ID_VIEW_DOCTAB_ICONS:
			pCmdUI->SetCheck(theApp.m_bMDITabIcons);
			pCmdUI->Enable(theApp.m_bMDITabs);
			break;

		case ID_VIEW_DOCTAB_NOTE:
			pCmdUI->SetCheck(theApp.m_nMDITabStyle == CMFCTabCtrl::STYLE_3D_ONENOTE);
			pCmdUI->Enable(theApp.m_bMDITabs);
			break;

		default:
			ASSERT(false);
			break;
	}
}

void CMainFrame::RebuildToolsMenu()
{
	// we only need to do this, when we do not use the BCG menu bar
	if (!CConfiguration::GetInstance()->m_bOptimizeMenuForVisuallyHandicappedUsers)
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
	const int nToolsMenuIndexOffset = 3;
	CMenu mnuRef;
	VERIFY(mnuRef.LoadMenu(IDR_MAINFRAME));

	CString strToolsMenuTitle;
	mnuRef.GetMenuString(mnuRef.GetMenuItemCount() - nToolsMenuIndexOffset, strToolsMenuTitle, MF_BYPOSITION);

	//
	// Get the tools menu from the current menu
	//
	int nToolsMenuIndex = -1;

	for (UINT nItem = 0; nItem < m_stdMenu.GetMenuItemCount(); ++nItem)
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

	int nLastEntryIndex = pToolsMenu->GetMenuItemCount() - 1;

	while (pToolsMenu->GetMenuItemID(nLastEntryIndex) == ID_TOOLS_ENTRY ||
	        (pToolsMenu->GetMenuItemID(nLastEntryIndex) >= ID_USER_TOOL_FIRST &&
	         pToolsMenu->GetMenuItemID(nLastEntryIndex) <= ID_USER_TOOL_LAST))
	{
		pToolsMenu->DeleteMenu(nLastEntryIndex, MF_BYPOSITION);
		nLastEntryIndex = pToolsMenu->GetMenuItemCount() - 1;
	}

	// add new tool entries
	const CObList &toolsList = theApp.GetUserToolsManager()->GetUserTools();
	POSITION pos = toolsList.GetHeadPosition();

	while (pos)
	{
		CUserTool *pTool = (CUserTool*)toolsList.GetNext(pos);
		pToolsMenu->AppendMenu(MF_STRING, pTool->GetCommandId(), pTool->m_strLabel);
	}
}

int CMainFrame::GetMDIChilds(CArray<CChildFrame*, CChildFrame*>& MDIChildArray, bool /*bSortByTabs*/ /*= true*/)
{
	int index = 0, ActiveChild = -1;

	//Clean up
	MDIChildArray.RemoveAll();

	HWND hwndMDIChild = ::GetWindow(m_hWndMDIClient, GW_CHILD);

	for (; hwndMDIChild; ++index)
	{
		CChildFrame* pMDIChildFrame = DYNAMIC_DOWNCAST(CChildFrame, CWnd::FromHandle(hwndMDIChild));

		if (pMDIChildFrame)
		{
			MDIChildArray.Add(pMDIChildFrame);

			if (GetActiveFrame() == pMDIChildFrame)
				ActiveChild = index;
		}

		hwndMDIChild = ::GetWindow(hwndMDIChild, GW_HWNDNEXT);
	}

	return ActiveChild;
}

void CMainFrame::OnViewFullScreen()
{
	ShowFullScreen();
}

void CMainFrame::OnUpdateViewFullScreen(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(IsFullScreen());
}

void CMainFrame::OnHelpKeyMapping()
{
	CMFCKeyMapDialog dlg(this, TRUE);
	dlg.DoModal();
}

LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// Pass message through file change notifier
	//theApp.m_FileChangeNotifier.PreTranslateMessage(message, wParam, lParam);

	return CMDIFrameWndEx::WindowProc(message, wParam, lParam);
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
	m_pContextMenuTargetWindow = NULL; //Invalidate. So we do not close the wrong file.

	// We have to use the right window to get right coordinates
	::ScreenToClient(pWnd->GetSafeHwnd(), &point);

	//Show two different popups: (a) for the tabs, (b) for the toolsbars
	if (pWnd->m_hWnd == m_hWndMDIClient || pWnd->m_hWnd == m_wndClientArea.GetMDITabs())
	{
		//Show popup for the tabs
		//But first: determine target window for context menu; will be needed to close the right file
		const int nTab = m_wndClientArea.GetMDITabs().GetTabFromPoint(point);

		if (nTab >= 0)
		{
			CChildFrame* wndEdit = dynamic_cast<CChildFrame*>(m_wndClientArea.GetMDITabs().GetTabWnd(nTab));
			// save pointer for later use
			if (wndEdit)
			{
				m_pContextMenuTargetWindow = dynamic_cast<CView*>(wndEdit->GetActiveView());
			}
		}

		// translate menu coordinates and show menu
		::ClientToScreen(pWnd->GetSafeHwnd(), &point);
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_MDICLIENT, point.x, point.y, this);
	}
	else
	{
		//Show popup for the toolbars
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_TOOLBAR, point.x, point.y, this);
	}
}

BOOL CMainFrame::OnHelpInfo(HELPINFO* /*pHelpInfo*/)
{
	return TRUE; //CMDIFrameWndEx::OnHelpInfo(pHelpInfo);
}

LRESULT CMainFrame::OnCustomizationMode(WPARAM wParam, LPARAM /*lParam*/)
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

	if (!pFrame)
		return;

	//Get active view/edit
	LaTeXView* pEdit = NULL;
	CView* pView = pFrame->GetActiveView();

	if (pView)
		pEdit = dynamic_cast<LaTeXView*>(pView);

	if (!pEdit)
		return;

	if (GetFocus() != pEdit)
	{
		//Activate view
		pEdit->SetFocus();
	}
}

void CMainFrame::OnWindowCloseSelectedTab()
{
	//Close the tab where the user pressed the right mouse button, i.e., where the context menu was/is displayed for
	if (m_pContextMenuTargetWindow && m_pContextMenuTargetWindow->GetDocument())
	{
		if (m_pContextMenuTargetWindow->GetDocument()->SaveModified())
		{
			m_pContextMenuTargetWindow->GetDocument()->OnCloseDocument();
		}
	}

	m_pContextMenuTargetWindow = NULL;
}

void CMainFrame::OnUpdateWindowCloseSelectedTab(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pContextMenuTargetWindow != NULL);
}

void CMainFrame::OnWindowCloseAllButActive()
{
	CArray<CChildFrame*, CChildFrame*> MDIChildArray;
	const int nActiveFrame = GetMDIChilds(MDIChildArray);

	for (int i = 0; i < MDIChildArray.GetSize(); i++)
	{
		if (i != nActiveFrame)
		{
			CView* pView = dynamic_cast<CView*>(MDIChildArray.GetAt(i)->GetActiveView());

			if (pView && pView->GetDocument() && pView->GetDocument()->SaveModified())
			{
				pView->GetDocument()->OnCloseDocument();
			}
		}
	}
}

void CMainFrame::OnUpdateWindowCloseAllButActive(CCmdUI* pCmdUI)
{
	CArray<CChildFrame*, CChildFrame*> MDIChildArray;
	GetMDIChilds(MDIChildArray);
	pCmdUI->Enable(MDIChildArray.GetSize() > 1);
}

bool CMainFrame::CreateNavigationViews(void)
{
	const DWORD pane_style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_HIDE_INPLACE;
	const CSize size(250,250);

	file_view_pane_.Create(CString(MAKEINTRESOURCE(STE_TAB_FILES)), this,size,
	                       TRUE, ID_VIEW_FILES_PANE,pane_style);
	bib_view_pane_.Create(CString(MAKEINTRESOURCE(STE_TAB_BIBENTRIES)), this,size,
	                      TRUE, ID_VIEW_BIB_ENTRIES_PANE,pane_style);
	structure_view_.Create(CString(MAKEINTRESOURCE(STE_TAB_STRUCTURE)), this,size,
	                       TRUE, ID_VIEW_STRUCT_PANE,pane_style);
	env_view_pane_.Create(CString(MAKEINTRESOURCE(STE_TAB_ENVIRONMENTS)), this,size,
	                      TRUE, ID_VIEW_ENV_PANE,pane_style);

	// Create views:
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!env_view_.Create(this))
	{
		TRACE0("Failed to create environment view\n");
		return false;
	}

	if (!file_view_.Create(this))
	{
		TRACE0("Failed to create file view\n");
		return false;
	}

	if (!bib_view_.Create(this))
	{
		TRACE0("Failed to create bib view\n");
		return false;
	}

	env_view_pane_.SetClient(&env_view_);
	file_view_pane_.SetClient(&file_view_);
	bib_view_pane_.SetClient(&bib_view_);

	structure_view_.EnableDocking(CBRS_ALIGN_ANY);
	env_view_pane_.EnableDocking(CBRS_ALIGN_ANY);
	file_view_pane_.EnableDocking(CBRS_ALIGN_ANY);
	bib_view_pane_.EnableDocking(CBRS_ALIGN_ANY);

	DockPane(&structure_view_);

	CDockablePane* p = 0;

	env_view_pane_.AttachToTabWnd(&structure_view_,DM_STANDARD,TRUE,&p);
	file_view_pane_.AttachToTabWnd(&structure_view_,DM_STANDARD,FALSE);
	bib_view_pane_.AttachToTabWnd(&structure_view_,DM_STANDARD,FALSE);

	if (CBaseTabbedPane* pane = dynamic_cast<CBaseTabbedPane*>(p))
	{
		HIMAGELIST himl = ::ImageList_LoadImage(AfxGetResourceHandle(),
		                                        MAKEINTRESOURCE(IDB_NAVIGATION_BAR),16,1,RGB(192,192,192),IMAGE_BITMAP,
		                                        LR_CREATEDIBSECTION);

		structure_view_.SetIcon(::ImageList_ExtractIcon(0,himl,0),FALSE);
		env_view_pane_.SetIcon(::ImageList_ExtractIcon(0,himl,1),FALSE);
		file_view_pane_.SetIcon(::ImageList_ExtractIcon(0,himl,2),FALSE);
		bib_view_pane_.SetIcon(::ImageList_ExtractIcon(0,himl,3),FALSE);

		::ImageList_Destroy(himl);

		pane->GetUnderlyingWindow()->SetActiveTab(0);
	}

	return true;
}

void CMainFrame::OnOpenProject(CLaTeXProject* p)
{
	p->AddView(structure_view_.GetProjectView());
	p->AddView(&file_view_);
	p->AddView(&bib_view_);
	p->AddView(&env_view_);
}

void CMainFrame::OnCloseProject(CLaTeXProject* p)
{
	p->RemoveView(structure_view_.GetProjectView());
	p->RemoveView(&file_view_);
	p->RemoveView(&bib_view_);
	p->RemoveView(&env_view_);
}

bool CMainFrame::CreateOutputViews(void)
{
	const DWORD pane_style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_HIDE_INPLACE;
	const CSize size(250,150);

	error_list_view_.Create(CString(MAKEINTRESOURCE(IDS_ERROR_LIST)),this,size,
		TRUE,ID_VIEW_ERROR_LIST_PANE,pane_style);
	build_view_pane_.Create(CString(MAKEINTRESOURCE(STE_TAB_BUILD)), this,size,
	                        TRUE, ID_VIEW_BUILD_PANE,pane_style);
	grep_view_1_pane_.Create(CString(MAKEINTRESOURCE(STE_TAB_GREP_1)), this,size,
	                         TRUE, ID_VIEW_GREP_1_PANE,pane_style);
	grep_view_2_pane_.Create(CString(MAKEINTRESOURCE(STE_TAB_GREP_2)), this,size,
	                         TRUE, ID_VIEW_GREP_2_PANE,pane_style);
	parse_view_pane_.Create(CString(MAKEINTRESOURCE(STE_TAB_PARSE)), this,size,
	                        TRUE, ID_VIEW_PARSE_PANE,pane_style);

	// Create views:
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!build_view_.Create(rectDummy,this))
	{
		TRACE0("Failed to create structure view\n");
		return false;
	}

	if (!grep_view_1_.Create(rectDummy,this))
	{
		TRACE0("Failed to create environment view\n");
		return false;
	}

	if (!grep_view_2_.Create(rectDummy,this))
	{
		TRACE0("Failed to create file view\n");
		return false;
	}

	if (!parse_view_.Create(rectDummy,this))
	{
		TRACE0("Failed to create bib view\n");
		return false;
	}

	grep_view_1_.SetIndex(0);
	grep_view_2_.SetIndex(1);

	output_doc_.SetAllViews(&build_view_,
	                        &grep_view_1_,&grep_view_2_,&parse_view_);

	build_view_.AttachDoc(&output_doc_);
	grep_view_1_.AttachDoc(&output_doc_);
	grep_view_2_.AttachDoc(&output_doc_);
	parse_view_.AttachDoc(&output_doc_);
	error_list_view_.AttachDoc(&output_doc_);

	output_doc_.SetErrorListView(&error_list_view_);

	build_view_pane_.SetClient(&build_view_);
	grep_view_1_pane_.SetClient(&grep_view_1_);
	grep_view_2_pane_.SetClient(&grep_view_2_);
	parse_view_pane_.SetClient(&parse_view_);

	error_list_view_.EnableDocking(CBRS_ALIGN_ANY);
	build_view_pane_.EnableDocking(CBRS_ALIGN_ANY);
	grep_view_1_pane_.EnableDocking(CBRS_ALIGN_ANY);
	grep_view_2_pane_.EnableDocking(CBRS_ALIGN_ANY);
	parse_view_pane_.EnableDocking(CBRS_ALIGN_ANY);

	DockPane(&build_view_pane_);

	CDockablePane* p = 0;

	error_list_view_.AttachToTabWnd(&build_view_pane_,DM_STANDARD,TRUE,&p);
	grep_view_1_pane_.AttachToTabWnd(&build_view_pane_,DM_STANDARD,FALSE);
	grep_view_2_pane_.AttachToTabWnd(&build_view_pane_,DM_STANDARD,FALSE);
	parse_view_pane_.AttachToTabWnd(&build_view_pane_,DM_STANDARD,FALSE);

	if (CBaseTabbedPane* pane = dynamic_cast<CBaseTabbedPane*>(p))
	{
		HIMAGELIST himl = ::ImageList_LoadImage(AfxGetResourceHandle(),
		                                        MAKEINTRESOURCE(IDB_OUTPUT_BAR),16,1,RGB(192,192,192),IMAGE_BITMAP,
		                                        LR_CREATEDIBSECTION);

		build_view_pane_.SetIcon(::ImageList_ExtractIcon(0,himl,0),FALSE);
		grep_view_1_pane_.SetIcon(::ImageList_ExtractIcon(0,himl,1),FALSE);
		grep_view_2_pane_.SetIcon(::ImageList_ExtractIcon(0,himl,2),FALSE);
		parse_view_pane_.SetIcon(::ImageList_ExtractIcon(0,himl,3),FALSE);
		error_list_view_.SetIcon(::ImageList_ExtractIcon(0,himl,4),FALSE);

		::ImageList_Destroy(himl);

		pane->GetUnderlyingWindow()->SetActiveTab(0);
	}

	return true;
}

COutputDoc* CMainFrame::GetOutputDoc(void)
{
	return &output_doc_;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.SetApplicationLook(id);

	switch (theApp.GetApplicationLook())
	{
		case ID_VIEW_APP_LOOK_WIN_2000:
			CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
			break;
		case ID_VIEW_APP_LOOK_OFFICE_XP:
			CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
			break;
		case ID_VIEW_APP_LOOK_WIN_XP:
			CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
			CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
			break;
		case ID_VIEW_APP_LOOK_OFFICE_2003:
			CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
			CDockingManager::SetDockingMode(DT_SMART);
			break;
		case ID_VIEW_APP_LOOK_VS_2005:
			CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
			CDockingManager::SetDockingMode(DT_SMART);
			break;
		default:
			switch (theApp.GetApplicationLook())
			{
				case ID_VIEW_APP_LOOK_OFFICE_2007_BLUE:
					CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
					break;
				case ID_VIEW_APP_LOOK_OFFICE_2007_BLACK:
					CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
					break;
				case ID_VIEW_APP_LOOK_OFFICE_2007_SILVER:
					CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
					break;
				case ID_VIEW_APP_LOOK_OFFICE_2007_AQUA:
					CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
					break;
			}

			CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
			CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	if (pCmdUI->m_nID == ID_VIEW_APP_LOOK_WIN_XP)
		pCmdUI->Enable(RunTimeHelper::IsCommCtrl6());

	pCmdUI->SetRadio(theApp.GetApplicationLook() == pCmdUI->m_nID);
}

void CMainFrame::OnLatexRun()
{
	PostMessage(StartPaneAnimation);
}

void CMainFrame::OnLatexStop()
{
	PostMessage(StopPaneAnimation);
}

void CMainFrame::OnStartPaneAnimation()
{
	m_wndStatusBar.SetPaneAnimation(BuildAnimationPane,build_animation_,250);
}

void CMainFrame::OnStopPaneAnimation()
{
	m_wndStatusBar.SetPaneAnimation(BuildAnimationPane,0);
}