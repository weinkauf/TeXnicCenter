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

// KeyboardHook.h: Schnittstelle für die Klasse CKeyboardHook.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEYBOARDHOOK_H__11A01CA1_45D6_11D3_929E_A8AA058B8412__INCLUDED_)
#define AFX_KEYBOARDHOOK_H__11A01CA1_45D6_11D3_929E_A8AA058B8412__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CKeyboardHook  
{
	friend LRESULT CALLBACK KeyboardHook( int nCode, WPARAM wParam, LPARAM lParam );

// construction/destruction
public:
	CKeyboardHook();
	virtual ~CKeyboardHook();

// operations
public:
	/**
	Installs this CKeyboardHook-object.

	@return
		TRUE if successfull, FALSE otherwise.
	*/
	BOOL Install();

	/**
	Uninstalls this CKeyboardHook-object.

	@return
		TRUE if successfull, FALSE otherwise.
	*/
	BOOL Uninstall();

// implementation helpers
protected:
	/**
	This function is called, when a key has been pressed down.

	The parameters are the same, as by CWnd::OnKeyDown().

	@return
		Return FALSE to send on the message or TRUE to keep it.
	*/
	virtual BOOL OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags ) = 0;

	/**
	This function is called, when a key has been released.

	The parameters are the same, as by CWnd::OnKeyUp().

	@return
		Return FALSE to send on the message or TRUE to keep it.
	*/
	virtual BOOL OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags ) = 0;

// attributes
private:
	/** The handle to the installed hook. */
	HHOOK m_hKeyboardHook;
};

#endif // !defined(AFX_KEYBOARDHOOK_H__11A01CA1_45D6_11D3_929E_A8AA058B8412__INCLUDED_)
