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

#if !defined(AFX_BCGREGISTRYEX_H__69989084_05C5_11D5_A222_006097239934__INCLUDED_)
#define AFX_BCGREGISTRYEX_H__69989084_05C5_11D5_A222_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Stack.h"

/**
Extends CBCGRegistry.

@author Sven Wiegand
*/
class CBCGRegistryEx : public CBCGRegistry  
{
// construction/destruction
public:
	/** See CBCGRegistry-documentation for parameter description */
	CBCGRegistryEx(BOOL bAdmin, BOOL bReadOnly);

	~CBCGRegistryEx();

// operations
public:
	/**
	Pushes the currently used registry key on a key stack.
	*/
	void PushKey();

	/**
	Pops the top most key that has been pushed using the PushKey()
	method and makes it the current key.
	*/
	void PopKey();

	/**
	Makes the top most key in the key stack the current key.
	*/
	void TopKey();

	/**
	Opens the root key (HKEY_CURRENT_USER or HKEY_LOCAL_MACHINE).
	*/
	void OpenRoot();

	/**
	Opens the specified key, whereby the key is treated as a key 
	relative to the root (HKEY_CURRENT_USER or HKEY_LOCAL_MACHINE).

	@return
		TRUE on success, FALSE otherwise.
	*/
	BOOL OpenFromRoot(LPCTSTR lpszPath);

// attributes
protected:
	BOOL m_bAdmin;
	CStack<HKEY, HKEY> m_KeyStack;
};

#endif // !defined(AFX_BCGREGISTRYEX_H__69989084_05C5_11D5_A222_006097239934__INCLUDED_)
