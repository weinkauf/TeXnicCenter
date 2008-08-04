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
#include "BrowseButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// CBrowseButton
//-------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CBrowseButton,CButton)
	//{{AFX_MSG_MAP(CBrowseButton)
	ON_WM_LBUTTONUP()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CBrowseButton::CBrowseButton(UINT unCtrlId,
                             LPCTSTR lpszTitle,bool bUseEditForInitFolder /*= true*/,
                             LPCTSTR lpszInitFolder /*= NULL*/,UINT unFlags /*= 0*/,
                             BFFCALLBACK lpfn /*= NULL*/,LPARAM lParam /*= 0*/)
		: m_unCtrlId(unCtrlId),
		m_bBrowseForFile(FALSE),
		m_strInitFolder(lpszInitFolder),
		m_bUseEditForInitFolder(bUseEditForInitFolder)
{
	m_dataFolderDialog.strTitle = lpszTitle;
	m_dataFolderDialog.unFlags = unFlags;
	//We need to change the next 2 items, if the caller wants us to do a callback.
	// But we will do this just before we open the BFF-Dialog.
	m_dataFolderDialog.lpfn = lpfn;
	m_dataFolderDialog.lParam = lParam;
}

CBrowseButton::CBrowseButton(UINT unCtrlId,
                             BOOL bOpenFileDialog /*= TRUE*/,
                             LPCTSTR lpszDefExt /*= NULL*/,
                             DWORD dwFlags /*= OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT*/,
                             LPCTSTR lpszFilter /*= NULL*/)
		: m_unCtrlId(unCtrlId),
		m_bBrowseForFile(TRUE)
{
	m_dataFileDialog.bOpenFileDialog = bOpenFileDialog;
	m_dataFileDialog.strDefExt = lpszDefExt;
	m_dataFileDialog.dwFlags = dwFlags;
	m_dataFileDialog.strFilter = lpszFilter;
}

void CBrowseButton::Browse()
{
	// get parent window
	CWnd *pParent = GetParent();
	ASSERT(pParent && IsWindow(pParent->m_hWnd));
	if (!pParent || !IsWindow(pParent->m_hWnd))
		return;

	// get control
	CWnd *pCtrl = pParent->GetDlgItem(m_unCtrlId);
	ASSERT(pCtrl && IsWindow(pCtrl->m_hWnd));
	if (!pCtrl || !IsWindow(pCtrl->m_hWnd))
		return;

	//Get the String from the edit control (hopefully a dirpath)
	CString strPath;
	pCtrl->GetWindowText(strPath);

	// Browse
	if (m_bBrowseForFile)
	{
		CFileDialog dlg(
		    m_dataFileDialog.bOpenFileDialog,
		    m_dataFileDialog.strDefExt,strPath,
		    m_dataFileDialog.dwFlags,
		    m_dataFileDialog.strFilter,pParent);

		if (dlg.DoModal() == IDCANCEL)
			return;

		strPath = dlg.GetPathName();
	}
	else
	{
		bool bUsedCallback = false;

		//Use Callback from the caller or does he want us to do the callback?
		if (m_dataFolderDialog.lpfn == NULL)
		{
			//At least he did not gave us a callback of his own.
			//So if he gave us the permission to use the edit control
			// or the given Initfolder, we will do the callback.
			if (m_bUseEditForInitFolder)
			{
				m_strInitFolder = strPath;
			}

			if (!m_strInitFolder.IsEmpty())
			{
				//Now we establish the callback
				m_dataFolderDialog.lpfn = (BFFCALLBACK) BrowseDlgCallback;
				m_dataFolderDialog.lParam = (LONG)(LPCTSTR) m_strInitFolder;
				bUsedCallback = true;
			}
		}

		CFolderSelect dlg(
		    m_dataFolderDialog.strTitle,
		    m_dataFolderDialog.unFlags,
		    pParent,
		    m_dataFolderDialog.lpfn,
		    m_dataFolderDialog.lParam);

		if (dlg.DoModal() == IDCANCEL)
		{
			//We need to reset the callback. If not, we will always jump to the same dir.
			if (bUsedCallback)
				m_dataFolderDialog.lpfn = NULL;

			return;
		}

		strPath = dlg.GetPath();

		//We need to reset the callback. If not, we will always jump to the same dir.
		if (bUsedCallback)
			m_dataFolderDialog.lpfn = NULL;
	}

	// Set text

	pCtrl->SetWindowText(strPath);
	pCtrl->SetFocus();
}

void CBrowseButton::OnLButtonUp(UINT nFlags,CPoint point)
{
	CRect rect;
	GetWindowRect(rect);
	ClientToScreen(&point);
	if (rect.PtInRect(point) && (GetState() & 0x0004))
		Browse();

	CButton::OnLButtonUp(nFlags,point);
}

void CBrowseButton::OnKeyUp(UINT nChar,UINT nRepCnt,UINT nFlags)
{
	if (nChar == VK_SPACE && (GetState() & 0x0004))
		Browse();

	CButton::OnKeyUp(nChar,nRepCnt,nFlags);
}

//---------------------------------------------------------------------

int CALLBACK BrowseDlgCallback(HWND hwnd,UINT uMsg,
                               LPARAM /*lParam*/,LPARAM lpData)
{
	if (uMsg == BFFM_INITIALIZED)
	{
		//Select the folder
		::SendMessage(hwnd,BFFM_SETSELECTION,true,lpData);
	}


	return 0;
}