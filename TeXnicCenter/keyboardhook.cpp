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

#include "stdafx.h"
#include "stdafx.h"
#include "KeyboardHook.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// pointer to the only one allowed hook-object
static CKeyboardHook *g_pKeyboardHook = NULL;

//////////////////////////////////////////////////////////////////////
// keyboard hook
LRESULT CALLBACK KeyboardHook( int nCode, WPARAM wParam, LPARAM lParam )
{
	if( nCode == HC_ACTION && g_pKeyboardHook )
	{
		// key pressed or released?
		if( lParam & 0x80000000 )
		{
			if( g_pKeyboardHook->OnKeyUp( wParam, lParam & 0x0000FFFF, (lParam & 0xFFFF0000) >> 16 ) )
				return 1L;
		}
		else
		{
			if( g_pKeyboardHook->OnKeyDown( wParam, lParam & 0x0000FFFF, (lParam & 0xFFFF0000) >> 16 ) )
				return 1L;
		}
	}
		
	return CallNextHookEx( g_pKeyboardHook->m_hKeyboardHook, nCode, wParam, lParam );
}


//////////////////////////////////////////////////////////////////////
// class CKeyboardHook

CKeyboardHook::CKeyboardHook()
: m_hKeyboardHook( NULL )
{}


CKeyboardHook::~CKeyboardHook()
{
	if( m_hKeyboardHook && g_pKeyboardHook == this )
		Uninstall();
}


BOOL CKeyboardHook::Install()
{
	ASSERT( !g_pKeyboardHook );
	ASSERT( !m_hKeyboardHook );

	if( g_pKeyboardHook || m_hKeyboardHook )
		return FALSE;

	// install hook
	if( 
			(m_hKeyboardHook = SetWindowsHookEx( 
				WH_KEYBOARD, 
				KeyboardHook, 
				AfxGetInstanceHandle(), 
				AfxGetThread()->m_nThreadID )
			) == NULL )
			return FALSE;

	// the one and only...
	g_pKeyboardHook = this;
	return TRUE;
}


BOOL CKeyboardHook::Uninstall()
{
	ASSERT( g_pKeyboardHook == this );
	ASSERT( m_hKeyboardHook );

	if( g_pKeyboardHook != this || !m_hKeyboardHook )
		return FALSE;

	// uninstall hook
	if( UnhookWindowsHookEx( m_hKeyboardHook ) )
	{
		g_pKeyboardHook = NULL;
		m_hKeyboardHook = NULL;
		return TRUE;
	}

	return FALSE;
}
