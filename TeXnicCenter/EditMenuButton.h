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

#if !defined(AFX_EDITMENUBUTTON_H__AF589589_0A74_11D5_A222_006097239934__INCLUDED_)
#define AFX_EDITMENUBUTTON_H__AF589589_0A74_11D5_A222_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MenuButton.h"
#include "PersistPosEdit.h"

/**
This class provides a menu button, that can be used inside a dialog
box, to insert text into an edit control, by choosing it from a menu,
displayed, when the users presses this button.

The description text of the menu items in the given menu resource 
has to specify the text to insert into the edit control.

@see CPersistPosEdit

@author Sven Wiegand
*/
class CEditMenuButton : public CMenuButton
{
// construction/destruction
public:
	CEditMenuButton(UINT unMenuId, BOOL bRightArrow = TRUE);

// operation
public:
	/**
	You need to call this method to specify the edit control, the 
	choosen text should be inserted in.
	*/
	void AttachEditCtrl(CPersistPosEdit *pEdit);

// overridings
protected:
	void OnShowMenu();
	//{{AFX_VIRTUAL(CEditMenuButton)
	//}}AFX_VIRTUAL

// message handlers
protected:
	//{{AFX_MSG(CEditMenuButton)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// attributes
protected:
	/** Pointer to the edit control, this menu button belongs to. */
	CPersistPosEdit *m_pEdit;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_EDITMENUBUTTON_H__AF589589_0A74_11D5_A222_006097239934__INCLUDED_
