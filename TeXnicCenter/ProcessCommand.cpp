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
#include "ProcessCommand.h"
#include "PlaceHolder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//-------------------------------------------------------------------
// class CProcessCommand
//-------------------------------------------------------------------

CProcessCommand::CProcessCommand() : m_nLastError(0)
{}


CProcessCommand::~CProcessCommand()
{}


void CProcessCommand::Set(LPCTSTR lpszExecutable, LPCTSTR lpszArguments)
{
	m_strExecutable = lpszExecutable;
	m_strArguments = lpszArguments;
}


CProcess *CProcessCommand::Execute(LPCTSTR lpszWorkingDir, LPCTSTR lpszMainPath, LPCTSTR lpszCurrentPath /*= NULL*/, long lCurrentLine /*= -1*/) const
{
	CString		strArguments = AfxExpandPlaceholders(m_strArguments, lpszMainPath, lpszCurrentPath, lCurrentLine);
	CProcessCommand * const localThis = (CProcessCommand * const)this;
	CProcess	*p = new CProcess;
	
	if (p->Create(m_strExecutable + _T(' ') + strArguments, NULL, NULL, FALSE, CREATE_NEW_PROCESS_GROUP, NULL, lpszWorkingDir))
	{
		localThis->m_nLastError = 0;
		return p;
	}

	localThis->m_nLastError = ::GetLastError();

	delete p;
	return NULL;
}


CProcess *CProcessCommand::Execute(HANDLE hOutput, LPCTSTR lpszWorkingDir, LPCTSTR lpszMainPath, LPCTSTR lpszCurrentPath /*= NULL*/, long lCurrentLine /*= -1*/) const
{
	CString		strArguments = AfxExpandPlaceholders(m_strArguments, lpszMainPath, lpszCurrentPath, lCurrentLine);
	CProcessCommand * const localThis = (CProcessCommand * const)this;
	CProcess	*p = new CProcess;

	if (p->CreateHiddenConsole(m_strExecutable + _T(' ') + strArguments, INVALID_HANDLE_VALUE, hOutput, hOutput, CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP, lpszWorkingDir))
	{
		localThis->m_nLastError = 0;
		return p;
	}

	localThis->m_nLastError = ::GetLastError();

	delete p;
	return NULL;
}


CString CProcessCommand::SerializeToString() const
{
	return m_strExecutable + _T('\n') + m_strArguments;
}


BOOL CProcessCommand::SerializeFromString(LPCTSTR lpszPackedInformation)
{
	CString	strExecutable;
	CString	strArguments;
	if (!AfxExtractSubString(strExecutable, lpszPackedInformation, 0, _T('\n')))
		return FALSE;
	if (!AfxExtractSubString(strArguments, lpszPackedInformation, 1, _T('\n')))
		return FALSE;

	Set(strExecutable, strArguments);
	return TRUE;
}