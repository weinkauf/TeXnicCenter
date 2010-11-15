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

#if !defined(AFX_BCGNOTIFYINGEDITLISTBOX_H__ED259B04_0B33_11D5_A222_006097239934__INCLUDED_)
#define AFX_BCGNOTIFYINGEDITLISTBOX_H__ED259B04_0B33_11D5_A222_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum
{
	BCGCELN_AFTERADDITEM = 1,
	BCGCELN_AFTERMOVEITEMDOWN,
	BCGCELN_AFTERMOVEITEMUP,
	BCGCELN_AFTERRENAMEITEM,
	BCGCELN_BEFOREREMOVEITEM,
	BCGCELN_CLICKBUTTON,
	BCGCELN_ENDEDITLABEL,
	BCGCELN_GETIMAGE,
	BCGCELN_KEY,
	BCGCELN_SELECTIONCHANGED,
	BCGCELN_SELECTIONCHANGING
};

typedef struct tagNMBCGCEDITLISTBOX
{
	NMHDR hdr;
	int iItem;
} NMBCGCEDITLISTBOX, *LPNMBCGCEDITLISTBOX;

typedef struct tagNMBCGCELBUTTON
{
	NMHDR hdr;
	int iButton;
} NMBCGCELBUTTON, *LPNMBCGCELBUTTON;

typedef struct tagNMBCGCELEDIT
{
	NMHDR hdr;
	LPCTSTR lpszLabel;
} NMBCGCELEDIT, *LPNMBCGCELEDIT;

typedef struct tagNMBCGCELGETIMAGE
{
	NMHDR hdr;
	LV_ITEM *pItem;
} NMBCGCELGETIMAGE, *LPNMBCGCELGETIMAGE;

typedef struct tagNMBCGCELKEY
{
	NMHDR hdr;
	WORD wKey;
	BYTE fFlags;
} NMBCGCELKEY, *LPNMBCGCELKEY;

/**
Overloads the CBCGEditListBox::On...()-virtuals to send notification
messages to the box's parent window.

Should simplify the use of CBCGEditListBox, because you don't have to
derive your own classes any more.

@ingroup guielements
@ingroup bcgext

@author Sven Wiegand
 */
class NotifyingEditListBox : public CVSListBox
{
// construction/destruction
public:
	NotifyingEditListBox();

// overridings
protected:
	void OnAfterAddItem(int iItem);
	void OnAfterMoveItemDown(int iItem);
	void OnAfterMoveItemUp(int iItem);
	void OnAfterRenameItem(int iItem);
	BOOL OnBeforeRemoveItem(int iItem);
	void OnClickButton(int iButton);
	void OnEndEditLabel(LPCTSTR lpszLabel);
	int OnGetImage(LV_ITEM *pItem);
	void OnKey(WORD wKey, BYTE fFlags);
	void OnSelectionChanged();
	BOOL OnSelectionChanging();

// implementation
protected:
	/**
	Fills the hdr-element of the given structure and sends the
	notification message.

	@param unCode
	        Notification code to include into the hdr-element.
	@param lParam
	        Pointer to the structure to send or NULL to send a simple NMHDR.

	@return
	        The result of the SendMessage() call.
	 */
	LRESULT SendNotification(UINT unCode, LPARAM lParam = NULL);

// message handlers
protected:
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnItemChanging(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_BCGNOTIFYINGEDITLISTBOX_H__ED259B04_0B33_11D5_A222_006097239934__INCLUDED_)
