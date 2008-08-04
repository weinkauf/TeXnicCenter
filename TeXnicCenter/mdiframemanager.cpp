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
#include "MDIFrameManager.h"
#include "configuration.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CMDIFrameManager,CCmdTarget)

CMDIFrameManager::CMDIFrameManager()
		: CKeyboardHook(),
		m_bCtrl(FALSE),
		m_pMDIFrameWnd(NULL),
		m_nActualWndIndex(-1)
{
}

BEGIN_MESSAGE_MAP(CMDIFrameManager,CCmdTarget)
	//{{AFX_MSG_MAP(CMDIFrameManager)
	ON_COMMAND(ID_WINDOW_LIST,OnWindowList)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_LIST,OnUpdateWindowMenu)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_WINDOW_FIRST,ID_WINDOW_LAST,OnWindow)
END_MESSAGE_MAP()

BOOL CMDIFrameManager::Install(CMDIFrameWnd *pMDIFrameWnd)
{
	ASSERT(pMDIFrameWnd && IsWindow(pMDIFrameWnd->m_hWnd));
	m_pMDIFrameWnd = pMDIFrameWnd;
	return CKeyboardHook::Install();
}

BOOL CMDIFrameManager::OnKeyDown(UINT nChar,UINT nRepCnt,UINT nFlags)
{
	switch (nChar)
	{
		case VK_CONTROL:
			m_bCtrl = TRUE;
			m_nActualWndIndex = (m_aFrameWnd.GetSize() > 0) ? 0 : -1;
			return FALSE;

		case VK_F6:
		case VK_TAB:
			if (!m_bCtrl)
				return FALSE;
			else
			{
				// valid window index
				if (m_nActualWndIndex < 0)
					return FALSE;

				// show next window
				m_pMDIFrameWnd->MDIActivate(m_aFrameWnd[(m_nActualWndIndex == m_aFrameWnd.GetUpperBound()) ? m_nActualWndIndex = 0 : ++m_nActualWndIndex]);
			}
			return TRUE;

		case VK_F4:
			// Ctrl pressed?
			if (!m_bCtrl)
				return FALSE;

			// valide window index
			if (m_nActualWndIndex < 0)
				return FALSE;

			// close window
			{
				CFrameWnd *pActiveWindow = m_pMDIFrameWnd->MDIGetActive();

				pActiveWindow->SendMessage(WM_CLOSE);
				RemoveChildFrame(pActiveWindow);
			}

			return TRUE;
	}

	return FALSE;
}

BOOL CMDIFrameManager::OnKeyUp(UINT nChar,UINT nRepCnt,UINT nFlags)
{
	switch (nChar)
	{
		case VK_CONTROL:
			// move active window in array to first position
			if (m_aFrameWnd.GetSize() > 0 && m_nActualWndIndex > 0)
			{
				ASSERT(m_nActualWndIndex >= 0 && m_nActualWndIndex < m_aFrameWnd.GetSize());

				CFrameWnd *pFrmWnd = m_aFrameWnd[m_nActualWndIndex];
				m_aFrameWnd.RemoveAt(m_nActualWndIndex);
				m_aFrameWnd.InsertAt(0,pFrmWnd);
				m_nActualWndIndex = 0;
			}

			m_bCtrl = FALSE;
			return FALSE;
	}

	return FALSE;
}

void CMDIFrameManager::AddChildFrame(CFrameWnd *pFrameWnd)
{
	ASSERT(pFrameWnd);
	ASSERT(IsWindow(pFrameWnd->m_hWnd));
	m_aFrameWnd.InsertAt(0,pFrameWnd);
}

void CMDIFrameManager::RemoveChildFrame(CFrameWnd *pFrameWnd)
{
	int nIndex = LookupFrameWnd(pFrameWnd);
	if (nIndex == -1)
		return;

	if (m_nActualWndIndex >= nIndex)
		--m_nActualWndIndex;

	m_aFrameWnd.RemoveAt(nIndex);
}

void CMDIFrameManager::ActivateChildFrame(CFrameWnd *pFrameWnd)
{
	int nIndex = LookupFrameWnd(pFrameWnd);

	if (nIndex < 0)
		return;

	// move window to first position in array
	if (nIndex == m_nActualWndIndex)
		return;

	CFrameWnd *pFrmWnd = m_aFrameWnd[nIndex];
	m_aFrameWnd.RemoveAt(nIndex);
	m_aFrameWnd.InsertAt(0,pFrmWnd);
	m_nActualWndIndex = nIndex;
}

int CMDIFrameManager::LookupFrameWnd(CFrameWnd *pFrameWnd)
{
	if (!m_aFrameWnd.GetSize())
		return -1;

	for (int i = 0; i < m_aFrameWnd.GetSize(); i++)
		if (m_aFrameWnd[i] == pFrameWnd)
			return i;

	return -1;
}

void CMDIFrameManager::OnWindowList()
{
	CMFCWindowsManagerDialog dlg((CMDIFrameWndEx*)m_pMDIFrameWnd);
	dlg.DoModal();
}

void CMDIFrameManager::OnUpdateWindowMenu(CCmdUI* pCmdUI)
{
	if (!CConfiguration::GetInstance()->m_bOptimizeMenuForVisuallyHandicappedUsers || !pCmdUI->m_pMenu)
		return;

	// add the most recent and current window to window menu
	CMenu *pMenu = pCmdUI->m_pMenu;
	int nInsertIndex;
	for (nInsertIndex = pCmdUI->m_nIndex; nInsertIndex > 0; --nInsertIndex)
	{
		if (pMenu->GetMenuItemID(nInsertIndex - 1) >= ID_WINDOW_FIRST && pMenu->GetMenuItemID(nInsertIndex - 1) <= ID_WINDOW_LAST)
			pMenu->DeleteMenu(nInsertIndex - 1,MF_BYPOSITION);
		else
			break;
	}

	for (int i = 0; i < m_aFrameWnd.GetSize() && i < CConfiguration::GetInstance()->m_nWndMenuMaxEntries; ++i)
	{
		// get window title
		CString strWindowText;
		m_aFrameWnd[i]->GetWindowText(strWindowText);

		// add number for the first ten windows
		if (i < 10)
		{
			CString strFormat;
			if (i == 9)
				strFormat.Format(_T("1&0 %s"),strWindowText);
			else
				strFormat.Format(_T("&%d %s"),i + 1,strWindowText);
			strWindowText = strFormat;
		}

		pMenu->InsertMenu(nInsertIndex + i,(i == 0) ? MF_BYPOSITION | MF_CHECKED : MF_BYPOSITION,ID_WINDOW_FIRST + i,strWindowText);
	}
}

void CMDIFrameManager::OnWindow(UINT nID)
{
	int nIndex = nID - ID_WINDOW_FIRST;
	ASSERT(nIndex >= 0 && nIndex < m_aFrameWnd.GetSize());
	ASSERT(m_aFrameWnd[nIndex] && IsWindow(m_aFrameWnd[nIndex]->m_hWnd));

	m_pMDIFrameWnd->MDIActivate(m_aFrameWnd[nIndex]);
}

int CMDIFrameManager::GetChildFrameCount() const
{
	return m_aFrameWnd.GetSize();
}
