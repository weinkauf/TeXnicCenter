/********************************************************************
*
* This file is part of the TeXnicCenter-system
*
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

#if !defined(AFX_TABPERSISTPOSEDIT_H__F5ACFC14_325B_11D8_B1AD_00095B41B65A__INCLUDED_)
#define AFX_TABPERSISTPOSEDIT_H__F5ACFC14_325B_11D8_B1AD_00095B41B65A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PersistPosEdit.h"


/** A CEdit-control, which lets the user insert tabs
		and remembers the caret position each time it looses the focus.

Tabs will be inserted at the current caret position,
if no text is selected. If some chars are selected,
they will be overwritten.
But if one or more full lines are selected, they will be intended.
So this CEdit behaves like many text editors.

See the base class CPersistPosEdit for a description
of how it remembers the caret position after loosing the focus.

@note Using this control in a dialog means, that the user can
not step through the dialog using the tab key. Instead, the user
needs to use the mouse to get away from this control.

@note If the lines get intended, then it is not possible
to undo that action. At least not on NT.

@ingroup guielements

@author Tino Weinkauf
*/
class CTabPersistPosEdit : public CPersistPosEdit
{
// Construction
public:
	CTabPersistPosEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabPersistPosEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTabPersistPosEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTabPersistPosEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABPERSISTPOSEDIT_H__F5ACFC14_325B_11D8_B1AD_00095B41B65A__INCLUDED_)
