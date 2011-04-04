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

#ifndef __EDITMENUBUTTONOPT_H__
#define __EDITMENUBUTTONOPT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EditMenuButton.h"

/**
This class provides a menu button, that can be used inside a dialog
box, to insert a placeholder into an edit control, by choosing it from a menu,
displayed, when the users presses this button. After choosing the placeholder
from the menu, the user is able to adjust some options of the placeholder.

It behaves like a normal CEditMenuButton, if the description text
of the menu items in the given menu resource do not start with a '\\n'.

But if the description starts with '\\n', a dialog will come up, where the user
will be able to adjust options related to Placeholders and/or Placeholder sets.

@see CEditMenuButton

@ingroup guielements

@author Tino Weinkauf
 */

class CEditMenuButtonOpt : public CEditMenuButton
{
// construction/destruction
public:
	CEditMenuButtonOpt(UINT unMenuId, BOOL bRightArrow = TRUE);

// operation
public:

// overridings
protected:
	void OnShowMenu();
	//{{AFX_VIRTUAL(CEditMenuButtonOpt)
	//}}AFX_VIRTUAL

// message handlers
protected:
	//{{AFX_MSG(CEditMenuButtonOpt)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// attributes
protected:
};


#endif //__EDITMENUBUTTONOPT_H__