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

#if !defined(AFX_MAINFRM_H__447410E5_1058_11D3_929E_D75EECD5B417__INCLUDED_)
#define AFX_MAINFRM_H__447410E5_1058_11D3_929E_D75EECD5B417__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WorkspaceBar.h"
#include "OutputBar.h"

class CLatexEdit;

// number of mathematic toolbars
#define MATHBAR_COUNT		16

class CMainFrame : public CBCGMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)

// types
protected:
	enum tagTimers
	{
		timerParse = 1,
		timerSave
	};

// construction/destruction
public:
	CMainFrame();
	virtual ~CMainFrame();

// types
public:
	enum tagOutputTabs
	{
		outputTabBuildResult = 0,
		outputTabGrep1 = 1,
		outputTabGrep2 = 2,
		outputTabParse = 3
	};

	enum tagNavigatorTabs
	{
		navigatorTabStructure = 0,
		navigatorTabEnvironments = 1,
		navigatorTabFiles = 2,
		navigatorTabReferences = 3
	};

// operations
public:

	CWorkspaceBar* GetWorkspaceBar();
	COutputBar* GetOutputBar();

	void GetControlBarsEx(CArray<CControlBar*, CControlBar*>& arrBars);

	/**
	Replaces a toolbar button by another one.

	Allows other classes to modify toolbar buttons without directly
	accessing the toolbar window.

	@param unCmdID
		Command ID of the toolbar button that should be replaced.
	@param button
		Toolbar button the specified button shout be replaced by.

	@return
		TRUE if successfull, FALSE otherwise.
	*/
	BOOL ReplaceToolbarButton( UINT unCmdID, const CBCGToolbarButton &button );

	/**
	Returns a pointer to the toolbar button with the specified
	command ID.

	You can get all instances of a button if you use iteration. Set
	pos to NULL to get the first ocurrence, after the call the 
	referenced pos variable will contain a position information.
	Using this information in the next call, you will get the next
	instance of the button.

	@param unCmdID
		Command ID of the toolbar button to return.
	@param pos
		Position marker for iteration.

	@return
		Pointer to the toolbar button with the specified command ID or
		NULL if there is no such button.
	*/
	CBCGToolbarButton* GetToolbarButton( UINT unCmdID, POSITION &pos ) const;

	/**
	Activates the specified menu.

	@param pMenu
		The Menu to activate.
	*/
	BOOL SetMenu(CMenu* pMenu);

	/**
	Activates the specified tab on the navigation bar and sets the focus on it.

	@param nTab
		Zero based index of the tab on the navigation bar to activate.
	*/
	void ActivateNavigationTab(int nTab);

	/**
	Activates the specified tab on the output bar.

	@param nTab
		Zero based index of the tab on the output bar to activate.

	@param bSetFocus
		Whether to set the focus to the specified tab or not.
	*/
	void ActivateOutputTab(int nTab, bool bSetFocus);

	/**
	Should be called, when the user may have changed the external, user
	defined tools, to update the tools menu.
	*/
	void RebuildToolsMenu();


// implementation helpers
protected:

	/**
	Activates the output bar.

	@param bSetFocus
		Whether to set the focus to the specified tab or not.
	*/
	void ActivateOutputBar(bool bSetFocus);

	/**
	Toggles the specified control bar.

	@param pCtrlBar
		Control bar to toggle.
	*/
	void ToggleControlBar(CControlBar* pCtrlBar);

	/**
	Checks if the specified control bar is visible

	@param pCtrlBar
		Control bar to check.

	@return
		TRUE, if the specified control bar is visible, FALSE otherwise.
	*/
	BOOL IsControlBarVisible(CControlBar* pCtrlBar);

	/**
	Returns the control bar associated with the specified command ID.

	@param unID
		The ID of the command that is used to toggle the control bar.

	@return 
		Pointer to the control bar associated with the specified 
		command ID or NULL, if there is no matching control bar.
	*/
	CControlBar* GetControlBarByCmd(UINT unID);

	/**
	Creates the specified tool bar

	@param pToolbar
		Tool bar to create
	@param unID
		ID of the toolbar resource to use for this toolbar
	@param unTitleID
		String resource to use for the title of the bar
	@param bVisible
		TRUE to make the tool bar initial visible, FALSE to hide it.

	@return
		TRUE if successfull, FALSE otherwise.
	*/
	BOOL CreateToolBar(CBCGToolBar* pToolBar, UINT unID, UINT unTitleID, BOOL bVisible = TRUE);

	/** Hides all control bars */
	void HideControlBars();

	/** Shows all control bars */
	void ShowControlBars();

	/**	Activates the full screen mode.
	*/
	void FullScreenOn();

	/** Deactivates the full screen mode.
	*/
	void FullScreenOff();

	/** Cares (display/hide) about the Document tabs.
	*/
	void DisplayDocumentTabs();

// overloadings
public:
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// debug
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// message handlers
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnExtrasCustomize();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnOptionsChanged();
	afx_msg void OnWindowEditor();
	afx_msg void OnWindowNavigator();
	afx_msg void OnWindowEnvironment();
	afx_msg void OnWindowFiles();
	afx_msg void OnWindowStructure();
	afx_msg void OnWindowOutput();
	afx_msg void OnWindowBuildResult();
	afx_msg void OnWindowGrep1();
	afx_msg void OnWindowGrep2();
	afx_msg void OnWindowList();
	afx_msg void OnHelpSearch();
	afx_msg void OnHelpIndex();
	afx_msg void OnViewFullScreen();
	afx_msg void OnUpdateViewFullScreen(CCmdUI* pCmdUI);
	afx_msg void OnHelpKeyMapping();
	afx_msg void OnUpdateWindowNext(CCmdUI* pCmdUI);
	afx_msg void OnWindowNext();
	afx_msg void OnWindowPrevious();
	afx_msg void OnUpdateWindowPrevious(CCmdUI* pCmdUI);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnHelpContents();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnViewDocTabsBottom();
	afx_msg void OnViewDocTabsOff();
	afx_msg void OnViewDocTabsTop();
	afx_msg void OnViewDocTabsIcons();
	afx_msg void OnViewDocTabsNote();
	afx_msg void OnUpdateViewDocTabs(CCmdUI* pCmdUI);
	afx_msg void OnToolsCancel();
	afx_msg void OnFileClose();
	afx_msg void OnWindowReferences();
	afx_msg void OnWindowParse();
	//}}AFX_MSG
	afx_msg void OnToggleCtrlBar( UINT nIDEvent );
	afx_msg void OnToggleMathBar( UINT nIDEvent );
	afx_msg void OnCheckCtrlBarVisible( CCmdUI *pCmdUI );
	afx_msg LRESULT OnToolbarContextMenu( WPARAM wp, LPARAM lp );
	afx_msg LRESULT OnResetToolbar( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnCustomizationMode( WPARAM wParam, LPARAM lParam );
	afx_msg void OnUpdateUserTool(CCmdUI *pCmdUI);
	afx_msg void OnExecuteUserTool(UINT nIDEvent);

	//BOOL InitializeDockingWindows();

	DECLARE_MESSAGE_MAP()

// attributes
protected:
	CBCGStatusBar					m_wndStatusBar;
	CBCGToolBar						m_wndToolBar;
	CBCGToolBar						m_wndFormatBar;
	CBCGToolBar						m_wndFormatRUBar;
	CBCGToolBar						m_wndInsertBar;
	CBCGToolBar						m_wndLatexBar;
	CBCGToolBar						m_wndMathBar;
	CBCGToolBar						m_wndFindBar;
	CBCGToolBar						m_awndMathBar[MATHBAR_COUNT];
	CBCGMenuBar						m_wndMenuBar;
	CWorkspaceBar					m_wndNavigationBar;
	COutputBar						m_wndOutputBar;
	CImageList						m_navigatorImages;
	CImageList						m_outputImages;
	CMenu									m_stdMenu;
	CMenu									m_projectMenu;
	CBCGToolBarImages			m_userImages;

	CLatexEdit*						m_pTargetWindow; // Target window from context menu

	// Full Screen mode support
	BOOL          m_bFSModeShowMenuBar;
	BOOL          m_bFSModeShowDocTabs;
	CRect         m_rcMainFrame;
	BOOL          m_bMaxChild;
	CToolBar*     m_pwndFullScrnToolBar;
	BOOL          m_bFullScreen;        // TRUE for "On" mode, FALSE for "Off"

	struct FSControlBars
	{
		BOOL         bWasVisible;
		CControlBar* pControlBar;
	};

	CArray<FSControlBars, FSControlBars&>* m_pwndControlBars;

private:
	/** Identifier of the timer used to parse the project in given intervals. */
	UINT m_unParseTimer;
};

inline CWorkspaceBar* CMainFrame::GetWorkspaceBar()
{
	return &m_wndNavigationBar;
}

inline COutputBar* CMainFrame::GetOutputBar()
{
	return &m_wndOutputBar;
}


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_MAINFRM_H__447410E5_1058_11D3_929E_D75EECD5B417__INCLUDED_)
