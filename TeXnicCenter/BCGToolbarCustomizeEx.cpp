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
#include "BCGToolbarCustomizeEx.h"
#include "UserTool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

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

void CBCGToolbarCustomizeEx::OnInitToolsPage ()
{
	ASSERT_VALID (m_pToolsPage);

	CRect rectTemp;

//	m_pToolsPage->m_wndToolsList.GetClientRect(rectTemp);
//	m_pToolsPage->m_wndToolsList.SetWindowPos(NULL, 0, 0,
//							rectTemp.right, rectTemp.bottom - 20,
//							SWP_NOMOVE | SWP_FRAMECHANGED);

	//Create and set up the Edit for the arguments
	m_pToolsPage->m_wndArgumentsEdit.GetWindowRect(rectTemp);
	m_pToolsPage->ScreenToClient(rectTemp);
	char ClassName[100];
	GetClassName(m_pToolsPage->m_wndArgumentsEdit.GetSafeHwnd(), ClassName, 99);
	m_wndTXCArgumentsEdit.CreateEx(
					m_pToolsPage->m_wndArgumentsEdit.GetExStyle(),
					ClassName,
					"TXCArgumentsEdit",
					m_pToolsPage->m_wndArgumentsEdit.GetStyle(),
					rectTemp,
					m_pToolsPage,
					(UINT)102,
					NULL);
	m_wndTXCArgumentsEdit.SetFont(m_pToolsPage->GetFont());

//	m_wndTXCArgumentsEdit.Create(
//					m_pToolsPage->m_wndArgumentsEdit.GetStyle(),
//					rectTemp,
//					m_pToolsPage,
//					(UINT)102);
//	m_wndTXCArgumentsEdit.ModifyStyleEx(0xFFFF, m_pToolsPage->m_wndArgumentsEdit.GetExStyle(),
//					SWP_FRAMECHANGED | SWP_SHOWWINDOW);

	//Create and set up the Button for the arguments
	m_pToolsPage->m_wndArgumentsBtn.GetWindowRect(rectTemp);
	m_pToolsPage->ScreenToClient(rectTemp);
	m_wndTXCArgumentsBtn.Create("",
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
					rectTemp,
					m_pToolsPage,
					101);
	m_wndTXCArgumentsBtn.SetFont(m_pToolsPage->GetFont());


	m_wndTXCArgumentsBtn.AttachEditCtrl(&m_wndTXCArgumentsEdit);
	m_wndTXCArgumentsBtn.ShowWindow(SW_SHOW);
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

	}
	else
	{
		m_wndTXCArgumentsEdit.SetWindowText(_T(""));
	}

	m_wndTXCArgumentsBtn.EnableWindow(pSelTool != NULL);
	m_wndTXCArgumentsEdit.EnableWindow(pSelTool != NULL);
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
