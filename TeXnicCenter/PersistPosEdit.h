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

#if !defined(AFX_PERSISTPOSEDIT_H__AF589588_0A74_11D5_A222_006097239934__INCLUDED_)
#define AFX_PERSISTPOSEDIT_H__AF589588_0A74_11D5_A222_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
This class implements a normal CEdit-control, which remembers the
caret position each time it looses the focus. After it has lost the
focus, you can call the InsertText()-method to insert text at the
remembered caret position and give the focus back to the control.

This class is usefull inside a dialog, in combination with 
menu-buttons, used to insert words into an edit control by choosing 
them from a menu that pops-up, when clicking on the menu button.

@ingroup guielements

@author Sven Wiegand
*/
class CPersistPosEdit : public CEdit
{
// construction/destruction
public:
	CPersistPosEdit();

// operations
public:
	/**
	Sets the input focus to this control, replaces the selection,
	stored as the control looses the focus, and places the carret
	behind the inserted text.
	*/
	void InsertText(LPCTSTR lpszText);

// overridings
protected:
	//{{AFX_VIRTUAL(CPersistPosEdit)
	//}}AFX_VIRTUAL

// message handlers
protected:
	//{{AFX_MSG(CPersistPosEdit)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// attributes
protected:
	/**
	The starting character of the selection is stored here, when 
	loosing	input focus.
	*/
	int m_nSelStart;

	/**
	The ending character of the selection is stored here, when loosing
	input focus.
	*/
	int m_nSelEnd;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_PERSISTPOSEDIT_H__AF589588_0A74_11D5_A222_006097239934__INCLUDED_
