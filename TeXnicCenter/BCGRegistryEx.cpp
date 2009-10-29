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
#include "BCGRegistryEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//-------------------------------------------------------------------
// class CBCGRegistryEx
//-------------------------------------------------------------------

RegistryStack::RegistryStack(BOOL bAdmin, BOOL bReadOnly)
		:	CSettingsStore(bAdmin, bReadOnly),
		m_bAdmin(bAdmin)
{
}


RegistryStack::~RegistryStack()
{
	// Your push/pop-calls are not balanced!
	ASSERT(m_KeyStack.empty());
}


void RegistryStack::PushKey()
{
	// Your push/pop-calls are not balanced!
	HANDLE key;
	::DuplicateHandle(::GetCurrentProcess(),m_reg.m_hKey,
	                  ::GetCurrentProcess(),&key,0,FALSE,DUPLICATE_SAME_ACCESS);

	m_KeyStack.push(reinterpret_cast<HKEY>(key));
}

void RegistryStack::PopKey()
{
	// Your push/pop-calls are not balanced!
	ASSERT(!m_KeyStack.empty());

	if (m_reg)
		m_reg.Close();

	m_reg.Attach(m_KeyStack.top());
	m_KeyStack.pop();
}

void RegistryStack::TopKey()
{
	// Stack is empty!
	ASSERT(!m_KeyStack.empty());

	if (m_reg)
		m_reg.Close();

	HANDLE key;
	::DuplicateHandle(::GetCurrentProcess(),m_KeyStack.top(),
	                  ::GetCurrentProcess(),&key,0,FALSE,DUPLICATE_SAME_ACCESS);

	m_reg.Attach(reinterpret_cast<HKEY>(key));
}

void RegistryStack::OpenRoot()
{
	if (m_reg)
		m_reg.Close();

	m_reg.Attach(m_bAdmin ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER);
}

BOOL RegistryStack::OpenFromRoot(LPCTSTR lpszPath)
{
	OpenRoot();
	return Open(lpszPath);
}

bool RegistryStack::VerifyValue(LPCTSTR v)
{
	return m_reg.QueryValue(v,0,0,0) == ERROR_SUCCESS;
}