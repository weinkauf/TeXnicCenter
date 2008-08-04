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

#if !defined(AFX_MENUBUTTON_H__AF58958B_0A74_11D5_A222_006097239934__INCLUDED_)
#define AFX_MENUBUTTON_H__AF58958B_0A74_11D5_A222_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
Simplifies the use of CMFCMenuButton. You can specify the resource-ID
of the menu-resource on construction.

@ingroup guielements

@author	Sven Wiegand
 */
class CMenuButton : public CMFCMenuButton
{
// construction/destruction
public:
	/**
	Constructs a CMenuButton-object.

	When the button is pressed after creation, it will show the menu
	with the given resource ID.

	@param unMenuId
	        Resource-Id of the menu to popup when the button has been
	        pressed. The first sub menu of the given menu will be shown.
	@param bRightArrow
	        TRUE to get an arrow pointing to the right, FALSE to get one
	        pointing down.
	 */
	CMenuButton(UINT unMenuId, BOOL bRightArrow = TRUE);
	virtual ~CMenuButton();

// overridings
protected:
	//{{AFX_VIRTUAL(CMenuButton)
	//}}AFX_VIRTUAL

// message handlers
protected:
	//{{AFX_MSG(CMenuButton)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

// attributes
protected:
	/** The menu to popup */
	CMenu m_menu;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_MENUBUTTON_H__AF58958B_0A74_11D5_A222_006097239934__INCLUDED_
