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
#include "NotifyingEditListBox.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//-------------------------------------------------------------------
// class NotifyingEditListBox
//-------------------------------------------------------------------

BEGIN_MESSAGE_MAP(NotifyingEditListBox,CVSListBox)
	ON_WM_ENABLE()
	ON_NOTIFY(LVN_ITEMCHANGING,1,OnItemChanging)
END_MESSAGE_MAP()

typedef struct tagEncapsulatedNMHDR
{
	NMHDR hdr;
} ENCAPSULATEDNMHDR,*LPENCAPSULATEDNMHDR;

NotifyingEditListBox::NotifyingEditListBox()
{
}

void NotifyingEditListBox::OnAfterAddItem(int iItem)
{
	CVSListBox::OnAfterAddItem(iItem);
	NMBCGCEDITLISTBOX hdr;
	hdr.iItem = iItem;
	SendNotification(BCGCELN_AFTERADDITEM,(LPARAM) & hdr);
}

void NotifyingEditListBox::OnAfterMoveItemDown(int iItem)
{
	CVSListBox::OnAfterMoveItemDown(iItem);
	NMBCGCEDITLISTBOX hdr;
	hdr.iItem = iItem;
	SendNotification(BCGCELN_AFTERMOVEITEMDOWN,(LPARAM) & hdr);
}

void NotifyingEditListBox::OnAfterMoveItemUp(int iItem)
{
	CVSListBox::OnAfterMoveItemUp(iItem);
	NMBCGCEDITLISTBOX hdr;
	hdr.iItem = iItem;
	SendNotification(BCGCELN_AFTERMOVEITEMUP,(LPARAM) & hdr);
}

void NotifyingEditListBox::OnAfterRenameItem(int iItem)
{
	CVSListBox::OnAfterRenameItem(iItem);
	NMBCGCEDITLISTBOX hdr;
	hdr.iItem = iItem;
	SendNotification(BCGCELN_AFTERRENAMEITEM,(LPARAM) & hdr);
}

BOOL NotifyingEditListBox::OnBeforeRemoveItem(int iItem)
{
	NMBCGCEDITLISTBOX hdr;
	hdr.iItem = iItem;
	return !(BOOL) SendNotification(BCGCELN_BEFOREREMOVEITEM,(LPARAM) & hdr);
}

void NotifyingEditListBox::OnClickButton(int iButton)
{
	CVSListBox::OnClickButton(iButton);
	NMBCGCELBUTTON hdr;
	hdr.iButton = iButton;
	SendNotification(BCGCELN_CLICKBUTTON,(LPARAM) & hdr);
}

void NotifyingEditListBox::OnEndEditLabel(LPCTSTR lpszLabel)
{
	// bug fix for BCGC-library
	CString strLabel;

	if (lpszLabel)
		strLabel = lpszLabel;
	else
		strLabel = GetItemText(GetSelItem());

	// call base implementation
	CVSListBox::OnEndEditLabel(strLabel);

	// notification
	NMBCGCELEDIT hdr;
	hdr.lpszLabel = lpszLabel;
	SendNotification(BCGCELN_ENDEDITLABEL, (LPARAM)&hdr);
}

int NotifyingEditListBox::OnGetImage(LV_ITEM *pItem)
{
	NMBCGCELGETIMAGE hdr;
	hdr.pItem = pItem;
	return (int)SendNotification(BCGCELN_GETIMAGE, (LPARAM)&hdr);
}

void NotifyingEditListBox::OnKey(WORD wKey,BYTE fFlags)
{
	CVSListBox::OnKey(wKey,fFlags);
	NMBCGCELKEY hdr;
	hdr.wKey = wKey;
	hdr.fFlags = fFlags;
	SendNotification(BCGCELN_KEY, (LPARAM)&hdr);
}

void NotifyingEditListBox::OnSelectionChanged()
{
	CVSListBox::OnSelectionChanged();
	SendNotification(BCGCELN_SELECTIONCHANGED);
}

BOOL NotifyingEditListBox::OnSelectionChanging()
{
	return !(BOOL)SendNotification(BCGCELN_SELECTIONCHANGING);
}

LRESULT NotifyingEditListBox::SendNotification(UINT unCode,LPARAM lParam)
{
	CWnd *pParent = GetParent();
	if (!pParent || !IsWindow(pParent->m_hWnd))
		return 0L;

	int nControlId = GetDlgCtrlID();

	if (lParam)
	{
		LPENCAPSULATEDNMHDR pnmh = reinterpret_cast<LPENCAPSULATEDNMHDR>(lParam);
		pnmh->hdr.hwndFrom = m_hWnd;
		pnmh->hdr.idFrom = (UINT) nControlId;
		pnmh->hdr.code = unCode;
		lParam = reinterpret_cast<LPARAM>(pnmh);
		return ::SendMessage(pParent->m_hWnd,WM_NOTIFY,(WPARAM) nControlId,lParam);
	}
	else
	{
		NMHDR hdr;
		hdr.hwndFrom = m_hWnd;
		hdr.idFrom = (UINT) nControlId;
		hdr.code = unCode;
		lParam = reinterpret_cast<LPARAM>(&hdr);
		return ::SendMessage(pParent->m_hWnd,WM_NOTIFY,(WPARAM) nControlId,lParam);
	}
}

void NotifyingEditListBox::OnEnable(BOOL bEnable)
{
	m_pWndList->EnableWindow(bEnable);
}

void NotifyingEditListBox::OnItemChanging(NMHDR* pNMHDR,LRESULT* pResult)
{
	ASSERT(pNMHDR != NULL);

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*) pNMHDR;
	ASSERT(pNMListView != NULL);

	*pResult = 0;

	if (pNMListView->uChanged == LVIF_STATE &&
	        (pNMListView->uOldState & LVIS_SELECTED) != (pNMListView->uNewState & LVIS_SELECTED))
	{
		*pResult = !OnSelectionChanging();
	}
}