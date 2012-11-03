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
#include "resource.h"
#include "UserToolAdvDlg.h"
#include "ToolBarsCustomizeDialog.h"
#include "FontOccManager.h"
#include "DialogTemplate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

int CALLBACK AfxPropSheetCallback(HWND,UINT message,LPARAM lParam);
const TCHAR*  PROP_CLOSEPENDING_NAME = _T("AfxClosePending");
BOOL AFXAPI AfxEndDeferRegisterClass(LONG fToRegister);

#define AfxDeferRegisterClass(fClass) AfxEndDeferRegisterClass(fClass)
#define AFX_WNDCOMMCTLS_REG             0x00010 // means all original Win95

#ifndef _UNICODE
#define AFX_WNDCOMMCTLSALL_REG          0x3C010 // COMMCTLS|INTERNET|COOL|USEREX|DATE
#define AFX_WNDCOMMCTLSNEW_REG          0x3C000 // INTERNET|COOL|USEREX|DATE
#else
#define AFX_WNDCOMMCTLSALL_REG          0xFC010 // COMMCTLS|INTERNET|COOL|USEREX|DATE|LINK|PAGER
#define AFX_WNDCOMMCTLSNEW_REG          0xFC000 // INTERNET|COOL|USEREX|DATE|LINK|PAGER
#endif

BOOL AFXAPI AfxInitNetworkAddressControl();

ToolBarCustomizeExAdvBtn::ToolBarCustomizeExAdvBtn()
		: pTool(NULL)
{
}

ToolBarCustomizeExAdvBtn::~ToolBarCustomizeExAdvBtn()
{
}

BEGIN_MESSAGE_MAP(ToolBarCustomizeExAdvBtn,CButton)
	//{{AFX_MSG_MAP(ToolBarCustomizeExAdvBtn)
	ON_CONTROL_REFLECT(BN_CLICKED,OnClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void ToolBarCustomizeExAdvBtn::OnClicked()
{
	ASSERT_VALID(pTool);

	//Open Dialog
	CUserToolAdvDlg AdvDlg;
	AdvDlg.m_bUseOutputWindow = pTool->m_bUseOutputWindow;
	AdvDlg.m_bPromptForArguments = pTool->m_bPromptForArguments;
	AdvDlg.m_bCloseConsoleWindow = pTool->m_bCloseConsoleWindow;
	AdvDlg.m_strInputFile = pTool->m_strInputFile;
	AdvDlg.m_strOutputFile = pTool->m_strOutputFile;

	if (AdvDlg.DoModal() == IDOK)
	{
		pTool->m_bUseOutputWindow = AdvDlg.m_bUseOutputWindow;
		pTool->m_bPromptForArguments = AdvDlg.m_bPromptForArguments;
		pTool->m_bCloseConsoleWindow = AdvDlg.m_bCloseConsoleWindow;
		pTool->m_strInputFile = AdvDlg.m_strInputFile;
		pTool->m_strOutputFile = AdvDlg.m_strOutputFile;
	}
}

IMPLEMENT_DYNAMIC(ToolBarsCustomizeDialog,CMFCToolBarsCustomizeDialog)


ToolBarsCustomizeDialog::ToolBarsCustomizeDialog(CFrameWnd* pWndParentFrame,
                                       BOOL bAutoSetFromMenus /*= FALSE*/,
                                       UINT uiFlags /*= (BCGCUSTOMIZE_MENU_SHADOWS | BCGCUSTOMIZE_TEXT_LABELS | BCGCUSTOMIZE_LOOK_2000 | BCGCUSTOMIZE_MENU_ANIMATIONS)*/,
                                       CList <CRuntimeClass*,CRuntimeClass*>* plistCustomPages /*= NULL*/)
		: CMFCToolBarsCustomizeDialog(pWndParentFrame,bAutoSetFromMenus,uiFlags,plistCustomPages),
		m_wndTXCArgumentsBtn(IDR_POPUP_PLACEHOLDEREX_OPT)
{
}

ToolBarsCustomizeDialog::~ToolBarsCustomizeDialog()
{
}

BEGIN_MESSAGE_MAP(ToolBarsCustomizeDialog,CMFCToolBarsCustomizeDialog)
	//{{AFX_MSG_MAP(ToolBarsCustomizeDialog)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// ToolBarsCustomizeDialog message handlers

void ToolBarsCustomizeDialog::OnInitToolsPage()
{
	ASSERT_VALID(m_pToolsPage);

	///////////////////////////////////////////////////////
	// PAINTING THE DIALOG...
	///////////////////////////////////////////////////////

	CRect rectTemp;

	////////////////////////////////////////////
	// Resizing the list
	// - to get space for new controls
	m_pToolsPage->m_wndToolsList.GetClientRect(rectTemp);

	CListCtrl* pListCtrl = m_pToolsPage->m_wndToolsList.m_pWndList;
	ASSERT_VALID(pListCtrl);
	//Calc the size of the list for 5 items and 1 header
	CSize sizeViewRect = pListCtrl->ApproximateViewRect(rectTemp.Size(),6);
	//Difference
	int nListSizeDiff = rectTemp.bottom - sizeViewRect.cy;
	//Resize the listbox
	m_pToolsPage->m_wndToolsList.SetWindowPos(NULL,0,0,
	        rectTemp.right,sizeViewRect.cy,
	        SWP_NOMOVE | SWP_FRAMECHANGED);


	////////////////////////////////////////////
	// Repositioning other controls
	// - to get space for new controls
	CWnd* pWnd = (CWnd*) m_pToolsPage->m_wndToolsList.GetNextWindow();
	while (pWnd != NULL)
	{
		//Reposition Window
		pWnd->GetWindowRect(rectTemp);
		m_pToolsPage->ScreenToClient(rectTemp);
		rectTemp.OffsetRect(0,-nListSizeDiff);
		pWnd->MoveWindow(rectTemp,true);

		//Next Window
		pWnd = pWnd->GetNextWindow();
	}


	////////////////////////////////////////////
	// Replacing unwanted controls by own controls

	//Create and set up the Edit for the arguments
	m_pToolsPage->m_wndArgumentsEdit.GetWindowRect(rectTemp);
	m_pToolsPage->ScreenToClient(rectTemp);
	TCHAR ClassName[100];
	GetClassName(m_pToolsPage->m_wndArgumentsEdit.GetSafeHwnd(),(LPTSTR) ClassName,99);
	m_wndTXCArgumentsEdit.CreateEx(
	    m_pToolsPage->m_wndArgumentsEdit.GetExStyle(),
	    ClassName,
	    _T("TXCArgumentsEdit"),
	    m_pToolsPage->m_wndArgumentsEdit.GetStyle(),
	    rectTemp,
	    m_pToolsPage,
	    (UINT) 101,
	    NULL);
	m_wndTXCArgumentsEdit.SetFont(m_pToolsPage->GetFont());

	//Create and set up the Button for the arguments
	m_pToolsPage->m_wndArgumentsBtn.GetWindowRect(rectTemp);
	m_pToolsPage->ScreenToClient(rectTemp);
	m_wndTXCArgumentsBtn.Create(_T(""),
	                            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
	                            rectTemp,
	                            m_pToolsPage,
	                            102);
	m_wndTXCArgumentsBtn.SetFont(m_pToolsPage->GetFont());

	//Attaching the Edit to the Button
	m_wndTXCArgumentsBtn.AttachEditCtrl(&m_wndTXCArgumentsEdit);
	m_wndTXCArgumentsBtn.ShowWindow(SW_SHOW);


	////////////////////////////////////////////
	// Creating new controls

	//Getting the font
	CClientDC dc(m_pToolsPage);
	CFont* pOldFont = dc.SelectObject(m_pToolsPage->GetFont());

	// The Advanced Button
	// - getting the position
	m_pToolsPage->m_wndInitialDirBtn.GetWindowRect(rectTemp);
	m_pToolsPage->ScreenToClient(rectTemp);
	rectTemp.OffsetRect(0,rectTemp.bottom - rectTemp.top + 7);
	CString strAdvBtnText;
	strAdvBtnText.LoadString(STE_CUSTOMIZE_TOOLS_ADVBTN);
	CSize sizeAdvBtnLabel = dc.GetTextExtent(strAdvBtnText);
	rectTemp.left = rectTemp.right - sizeAdvBtnLabel.cx - 10;
	// - creating it
	m_wndAdvBtn.Create(strAdvBtnText,
	                   WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
	                   rectTemp,
	                   m_pToolsPage,
	                   103);
	// - set it up
	m_wndAdvBtn.SetFont(m_pToolsPage->GetFont());
	//	m_wndAdvBtn.ShowWindow(SW_SHOW);
	m_wndAdvBtn.ShowWindow(SW_HIDE);
	m_wndAdvBtn.EnableWindow(false);

	// The Advanced Summarize Edit
	// - getting the position
	CRect rectListCtrl;
	pListCtrl->GetWindowRect(rectListCtrl);
	m_pToolsPage->ScreenToClient(rectListCtrl);
	rectTemp.right = rectTemp.left - 10;
	rectTemp.left = rectListCtrl.left;
	m_wndAdvEdit.Create(ES_READONLY | WS_CHILD | WS_VISIBLE | WS_BORDER,
	                    rectTemp,
	                    m_pToolsPage,
	                    104);
	m_wndAdvEdit.SetFont(m_pToolsPage->GetFont());
	//	m_wndAdvEdit.ShowWindow(SW_SHOW);
	m_wndAdvEdit.ShowWindow(SW_HIDE);
	m_wndAdvEdit.EnableWindow(false);

	//Restoring the font
	dc.SelectObject(pOldFont);
}

void ToolBarsCustomizeDialog::OnBeforeChangeTool(CUserTool* pSelTool)
{
	if (pSelTool != NULL)
	{
		//--------------------------------------------------
		// Update selected tool properties (screen -> data):
		//--------------------------------------------------
		UserTool* pTool = DYNAMIC_DOWNCAST(UserTool,pSelTool);
		ASSERT_VALID(pTool);

		m_wndTXCArgumentsEdit.GetWindowText(pTool->m_strArguments);
		m_pToolsPage->m_wndArgumentsEdit.SetWindowText(pTool->m_strArguments);
	}

	m_pToolsPage->m_wndArgumentsBtn.ShowWindow(SW_HIDE);
	m_pToolsPage->m_wndArgumentsEdit.ShowWindow(SW_HIDE);
}

void ToolBarsCustomizeDialog::OnAfterChangeTool(CUserTool* pSelTool)
{
	if (pSelTool != NULL)
	{
		//--------------------------------------------------
		// Update selected tool properties (data -> screen):
		//--------------------------------------------------
		UserTool* pTool = DYNAMIC_DOWNCAST(UserTool,pSelTool);
		ASSERT_VALID(pTool);

		m_wndTXCArgumentsEdit.SetWindowText(pTool->m_strArguments);
		m_wndAdvBtn.pTool = pTool;

		//Set advanced description
		m_wndAdvEdit.SetWindowText(pTool->GetAdvDescription());
	}
	else
	{
		m_wndTXCArgumentsEdit.SetWindowText(_T(""));
		m_wndAdvBtn.pTool = NULL;
	}

	m_wndTXCArgumentsBtn.EnableWindow(pSelTool != NULL);
	m_wndTXCArgumentsEdit.EnableWindow(pSelTool != NULL);
	//	m_wndAdvBtn.EnableWindow(pSelTool != NULL);
	//	m_wndAdvEdit.EnableWindow(pSelTool != NULL);
}

void ToolBarsCustomizeDialog::OnDestroy()
{
	ASSERT_VALID(m_pToolsPage);

	if (m_pToolsPage->GetSafeHwnd() != NULL)
	{
		//--------------------------------------------------
		// Update selected tool properties (screen -> data):
		//--------------------------------------------------
		int iCurSel = m_pToolsPage->m_wndToolsList.GetSelItem();

		if (iCurSel >= 0)
		{
			UserTool* pTool = DYNAMIC_DOWNCAST(UserTool,
			                                      (CUserTool*) m_pToolsPage->m_wndToolsList.GetItemData(iCurSel));
			ASSERT_VALID(pTool);

			m_wndTXCArgumentsEdit.GetWindowText(pTool->m_strArguments);
		}
	}

	CMFCToolBarsCustomizeDialog::OnDestroy();
}

BOOL ToolBarsCustomizeDialog::CheckToolsValidity(const CObList& lstTools)
{
	return CMFCToolBarsCustomizeDialog::CheckToolsValidity(lstTools);
}

void ToolBarsCustomizeDialog::BuildPropPageArray(void)
{
	__super::BuildPropPageArray();
	FixPropSheetFont(m_psh,m_pages);

	m_psh.pfnCallback = PropSheetCallback;
}

int ToolBarsCustomizeDialog::PropSheetCallback(HWND hWnd, UINT message, LPARAM lParam)
{
	int result;

	switch (message)
	{
		case PSCB_PRECREATE:
			OnPropSheetCreate(lParam);
			result = 1;
			break;
		default:
			result = AfxPropSheetCallback(hWnd,message,lParam);
	}

	return result;
}

// This function has been extracted from dlgprop.cpp and modified to allow 
// hooking the property sheet callback. Unfortunately, the original function
// always overwrites m_psh.pfnCallback which makes it impossible to set up a
// hook. This version doesn't overwrite m_psh.pfnCallback if it's non-NULL.
BOOL ToolBarsCustomizeDialog::Create(CWnd* pParentWnd, DWORD dwStyle, DWORD dwExStyle)
{
	ENSURE( 0 == ( m_psh.dwFlags & PSH_AEROWIZARD ) );

	_AFX_THREAD_STATE* pState = AfxGetThreadState();

	// Calculate the default window style.
	if (dwStyle == (DWORD)-1)
	{
		pState->m_dwPropStyle = DS_MODALFRAME | DS_3DLOOK | DS_CONTEXTHELP |
			DS_SETFONT | WS_POPUP | WS_VISIBLE | WS_CAPTION;

		// Wizards don't have WS_SYSMENU.
		if (!IsWizard())
			pState->m_dwPropStyle |= WS_SYSMENU;
	}
	else
	{
		pState->m_dwPropStyle = dwStyle;
	}

	pState->m_dwPropExStyle = dwExStyle;

	ASSERT_VALID(this);
	ASSERT(m_hWnd == NULL);

	VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTLS_REG));
	AfxDeferRegisterClass(AFX_WNDCOMMCTLSNEW_REG);

#ifdef _UNICODE
	AfxInitNetworkAddressControl();
#endif

	// finish building PROPSHEETHEADER structure
	BuildPropPageArray();
	m_bModeless = TRUE;
	m_psh.dwFlags |= (PSH_MODELESS|PSH_USECALLBACK);

	// Modification: Don't overwrite the callback if it has been set to allow hooks
	if (!m_psh.pfnCallback)
		m_psh.pfnCallback = AfxPropSheetCallback;

	m_psh.hwndParent = pParentWnd->GetSafeHwnd();

	// hook the window creation process
	AfxHookWindowCreate(this);
#if _MSC_VER >= 1700
	HWND hWnd = (HWND)PropertySheet(&m_psh);
#else
	HWND hWnd = (HWND)AfxPropertySheet(&m_psh);
#endif
#ifdef _DEBUG
	DWORD dwError = ::GetLastError();
#endif

	// cleanup on failure, otherwise return TRUE
	if (!AfxUnhookWindowCreate())
		PostNcDestroy();    // cleanup if Create fails

	// setting a custom property in our window
	HGLOBAL hMem = GlobalAlloc(GPTR, sizeof(BOOL)); 
	BOOL* pBool = static_cast<BOOL*>(GlobalLock(hMem));
	if (pBool != NULL)
	{
		*pBool = TRUE;
		GlobalUnlock(hMem); 
		if (SetProp(this->m_hWnd, PROP_CLOSEPENDING_NAME, hMem) == 0)
		{
			GlobalFree(hMem);
			this->DestroyWindow();
			return FALSE;
		}
	}
	else
	{
		this->DestroyWindow();
		return FALSE;
	}

	if (hWnd == NULL || hWnd == (HWND)-1)
	{
#ifdef _DEBUG
		TRACE(traceAppMsg, 0, "PropertySheet() failed: GetLastError returned %d\n", dwError);
#endif
		return FALSE;
	}

	ASSERT(hWnd == m_hWnd);
	return TRUE;
}

// Original function, slightly modified
BOOL ToolBarsCustomizeDialog::Create()
{
	DWORD dwExStyle = 0;

	if ((m_pParentFrame != NULL) &&(m_pParentFrame->GetExStyle() & WS_EX_LAYOUTRTL))
	{
		dwExStyle |= WS_EX_LAYOUTRTL;
	}

	if (!Create(m_pParentFrame, (DWORD)-1, dwExStyle)) // Modified
	{
		return FALSE;
	}

	SetFrameCustMode(TRUE);
	return TRUE;
}

void ToolBarsCustomizeDialog::OnPropSheetCreate(LPARAM lParam)
{
	LPDLGTEMPLATE const lpTemplate = reinterpret_cast<LPDLGTEMPLATE>(lParam);
	CDialogTemplate t;
	t.m_hTemplate = reinterpret_cast<HGLOBAL>(lpTemplate);
	t.m_dwTemplateSize = DialogTemplate::GetTemplateSize(lpTemplate);

	LOGFONT lf;
	WORD size;
	GetDisplayFont(lf,size);

	t.SetFont(lf.lfFaceName,size);
	t.Detach();
}
