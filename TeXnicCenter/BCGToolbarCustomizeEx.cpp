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
#include "texniccenter.h"
#include "UserToolAdvDlg.h"


#include "BCGToolbarCustomizeEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



/////////////////////////////////////////////////////////////////////////////
// CBCGToolbarCustomizeExAdvBtn

CBCGToolbarCustomizeExAdvBtn::CBCGToolbarCustomizeExAdvBtn()
:pTool(NULL)
{
}

CBCGToolbarCustomizeExAdvBtn::~CBCGToolbarCustomizeExAdvBtn()
{
}


BEGIN_MESSAGE_MAP(CBCGToolbarCustomizeExAdvBtn, CButton)
	//{{AFX_MSG_MAP(CBCGToolbarCustomizeExAdvBtn)
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGToolbarCustomizeExAdvBtn message handlers

void CBCGToolbarCustomizeExAdvBtn::OnClicked() 
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




/////////////////////////////////////////////////////////////////////////////
// CBCGToolbarCustomizeEx

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CBCGToolbarCustomizeEx, CBCGToolbarCustomize)


CBCGToolbarCustomizeEx::CBCGToolbarCustomizeEx(CFrameWnd* pWndParentFrame, 
							BOOL bAutoSetFromMenus /*= FALSE*/,
							UINT uiFlags /*= (BCGCUSTOMIZE_MENU_SHADOWS | BCGCUSTOMIZE_TEXT_LABELS | BCGCUSTOMIZE_LOOK_2000 | BCGCUSTOMIZE_MENU_ANIMATIONS)*/,
							CList <CRuntimeClass*, CRuntimeClass*>* plistCustomPages /*= NULL*/)
	: CBCGToolbarCustomize(pWndParentFrame, bAutoSetFromMenus, uiFlags, plistCustomPages),
	m_wndTXCArgumentsBtn(IDR_POPUP_PLACEHOLDEREX_OPT)
{
}

CBCGToolbarCustomizeEx::~CBCGToolbarCustomizeEx()
{
}

BEGIN_MESSAGE_MAP(CBCGToolbarCustomizeEx, CBCGToolbarCustomize)
	//{{AFX_MSG_MAP(CBCGToolbarCustomizeEx)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBCGToolbarCustomizeEx message handlers

void CBCGToolbarCustomizeEx::OnInitToolsPage()
{
	ASSERT_VALID (m_pToolsPage);

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
	CSize sizeViewRect = pListCtrl->ApproximateViewRect(rectTemp.Size(), 6);
	//Difference
	int nListSizeDiff = rectTemp.bottom - sizeViewRect.cy;
	//Resize the listbox
	m_pToolsPage->m_wndToolsList.SetWindowPos(NULL, 0, 0,
							rectTemp.right, sizeViewRect.cy,
							SWP_NOMOVE | SWP_FRAMECHANGED);


	////////////////////////////////////////////
	// Repositioning other controls
	// - to get space for new controls
	CWnd* pWnd = (CWnd*)m_pToolsPage->m_wndToolsList.GetNextWindow();
	while(pWnd != NULL)
	{
		//Reposition Window
		pWnd->GetWindowRect(rectTemp);
		m_pToolsPage->ScreenToClient(rectTemp);
		rectTemp.OffsetRect(0, -nListSizeDiff);
		pWnd->MoveWindow(rectTemp, true);

		//Next Window
		pWnd = pWnd->GetNextWindow();
	}


	////////////////////////////////////////////
	// Replacing unwanted controls by own controls

	//Create and set up the Edit for the arguments
	m_pToolsPage->m_wndArgumentsEdit.GetWindowRect(rectTemp);
	m_pToolsPage->ScreenToClient(rectTemp);
	TCHAR ClassName[100];
	GetClassName(m_pToolsPage->m_wndArgumentsEdit.GetSafeHwnd(), (LPTSTR)ClassName, 99);
	m_wndTXCArgumentsEdit.CreateEx(
					m_pToolsPage->m_wndArgumentsEdit.GetExStyle(),
					ClassName,
					_T("TXCArgumentsEdit"),
					m_pToolsPage->m_wndArgumentsEdit.GetStyle(),
					rectTemp,
					m_pToolsPage,
					(UINT)101,
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
	rectTemp.OffsetRect(0, rectTemp.bottom - rectTemp.top + 7);
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

void CBCGToolbarCustomizeEx::OnBeforeChangeTool(CBCGUserTool* pSelTool)
{
	if (pSelTool != NULL)
	{
		//--------------------------------------------------
		// Update selected tool properties (screen -> data):
		//--------------------------------------------------
		CUserTool* pTool = DYNAMIC_DOWNCAST(CUserTool, pSelTool);
		ASSERT_VALID(pTool);

		m_wndTXCArgumentsEdit.GetWindowText(pTool->m_strArguments);
		m_pToolsPage->m_wndArgumentsEdit.SetWindowText(pTool->m_strArguments);
	}

	m_pToolsPage->m_wndArgumentsBtn.ShowWindow(SW_HIDE);
	m_pToolsPage->m_wndArgumentsEdit.ShowWindow(SW_HIDE);
}

void CBCGToolbarCustomizeEx::OnAfterChangeTool(CBCGUserTool* pSelTool)
{
	if (pSelTool != NULL)
	{
		//--------------------------------------------------
		// Update selected tool properties (data -> screen):
		//--------------------------------------------------
		CUserTool* pTool = DYNAMIC_DOWNCAST(CUserTool, pSelTool);
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

void CBCGToolbarCustomizeEx::OnDestroy() 
{
	ASSERT_VALID (m_pToolsPage);

	if (m_pToolsPage->GetSafeHwnd () != NULL)
	{
		//--------------------------------------------------
		// Update selected tool properties (screen -> data):
		//--------------------------------------------------
		int iCurSel = m_pToolsPage->m_wndToolsList.GetSelItem ();
		if (iCurSel >= 0)
		{
			CUserTool* pTool = DYNAMIC_DOWNCAST(CUserTool, 
				(CBCGUserTool*) m_pToolsPage->m_wndToolsList.GetItemData(iCurSel));
			ASSERT_VALID(pTool);

			m_wndTXCArgumentsEdit.GetWindowText(pTool->m_strArguments);
		}
	}

	CBCGToolbarCustomize::OnDestroy();
}

BOOL CBCGToolbarCustomizeEx::CheckToolsValidity (const CObList& lstTools)
{
//	for (POSITION pos = lstTools.GetHeadPosition (); pos != NULL;)
//	{
//		CUserTool* pTool = (CUserTool*) lstTools.GetNext (pos);
//		ASSERT_VALID (pTool);
//
//		if (pTool->GetCommand ().IsEmpty ())
//		{
//			MessageBox (_T("Command is required"));
//			return FALSE;
//		}
//	}

	return 	CBCGToolbarCustomize::CheckToolsValidity(lstTools);
//	return TRUE;
}


