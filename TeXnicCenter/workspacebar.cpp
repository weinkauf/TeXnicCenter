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
#include "WorkspaceBar.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// class CWorkspaceBar
//-------------------------------------------------------------------

CWorkspaceBar::CWorkspaceBar()
{
}

CWorkspaceBar::~CWorkspaceBar()
{
}


BEGIN_MESSAGE_MAP(CWorkspaceBar, CBCGSizingControlBar)
	//{{AFX_MSG_MAP(CWorkspaceBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CWorkspaceBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGSizingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create tabs window:
	if( !m_wndTabs.Create( CBCGTabWnd::STYLE_3D, rectDummy, this, 1 ) )
	{
		TRACE0("Failed to create workspace tab window\n");
		return -1;      // fail to create
	}

	m_wndTabs.SetImageList( IDB_NAVIGATIONTAB, 16, RGB(0, 0x80, 0x80) );
	
	return 0;
}

void CWorkspaceBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGSizingControlBar::OnSize(nType, cx, cy);
	
	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy,
		SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);	
}

BOOL CWorkspaceBar::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN &&
		pMsg->wParam == VK_ESCAPE)
	{
		CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd ();
		ASSERT_VALID (pMainFrame);

		if (pMainFrame->GetActivePopup () == NULL)
		{
			pMainFrame->SetFocus ();
			return TRUE;
		}
	}
	
	return CBCGSizingControlBar::PreTranslateMessage(pMsg);
}


void CWorkspaceBar::SetFlatTabs(BOOL bFlat /*= TRUE*/)
{
	m_wndTabs.SetFlatFrame(bFlat);
}
