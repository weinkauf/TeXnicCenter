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
#include "Process.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CProcessException,CException)

CProcessException::CProcessException()
		: CException(TRUE)
{
}

BOOL CProcessException::GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext /*= NULL*/)
{
	UNUSED_ALWAYS(lpszError);
	UNUSED_ALWAYS(nMaxError);
	UNUSED_ALWAYS(pnHelpContext);

	return FALSE;
}

void AfxThrowProcessException(int nCause)
{
	CProcessException *pE = new CProcessException();
	pE->m_cause = (CProcessException::cause)nCause;
	throw pE;
}

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CProcess::CProcess()
: m_processInfo()
, m_bProcess(FALSE)
{
}

CProcess::~CProcess()
{
	if (m_bProcess)
	{
		CloseHandle(m_processInfo.hThread);
		CloseHandle(m_processInfo.hProcess);
	}
}

BOOL CProcess::Create(
    LPCTSTR lpszCmdLine,
    LPSECURITY_ATTRIBUTES lpsaProcess /*= NULL*/,
    LPSECURITY_ATTRIBUTES lpsaThread /*= NULL*/,
    BOOL bInheritHandles /*= FALSE*/,
    DWORD dwCreate /*= 0*/,
    LPVOID lpvEnvironment /*= NULL*/,
    LPCTSTR lpszCurDir /*= NULL*/,
    LPSTARTUPINFO lpsiStartInfo /*= NULL*/)
{
	ASSERT(lpszCmdLine);
	if (!lpszCmdLine)
		return FALSE;

#ifdef UNICODE
	dwCreate |= CREATE_UNICODE_ENVIRONMENT;
#endif

	// provide standard STARTUPINFO
	STARTUPINFO si;
	if (!lpsiStartInfo)
	{
		ZeroMemory(&si,sizeof(si));
		si.cb = sizeof(si);
		lpsiStartInfo = &si;
	}

	return m_bProcess = ::CreateProcess(
	                        NULL,(LPTSTR)lpszCmdLine,
	                        lpsaProcess,lpsaThread,bInheritHandles,
	                        dwCreate,lpvEnvironment,lpszCurDir,
	                        lpsiStartInfo,
	                        &m_processInfo);
}

BOOL CProcess::CreateHiddenConsole(
    LPCTSTR lpszCmdLine,
    HANDLE hStdInput,HANDLE hStdOutput,HANDLE hStdError,
    DWORD dwCreate /*= 0*/,
    LPCTSTR lpszCurDir /*= NULL*/,
    LPSTARTUPINFO lpsiStartInfo /*= NULL*/)
{
	STARTUPINFO si;

	if (!lpsiStartInfo)
	{
		ZeroMemory(&si,sizeof(si));
		si.cb = sizeof(si);
		lpsiStartInfo = &si;
	}

	lpsiStartInfo->dwFlags |= STARTF_USESTDHANDLES;
	lpsiStartInfo->hStdInput = hStdInput;
	lpsiStartInfo->hStdOutput = hStdOutput;
	lpsiStartInfo->hStdError = hStdError;

	return Create(lpszCmdLine,NULL,NULL,TRUE,dwCreate,NULL,lpszCurDir,lpsiStartInfo);
}

BOOL CProcess::CreateHiddenConsole(
    LPCTSTR lpszCmdLine,
    PHANDLE phStdOut,PHANDLE phStdError,
    DWORD dwCreate /*= 0*/,
    LPCTSTR lpszCurDir /*= NULL*/,
    LPSTARTUPINFO lpsiStartInfo /*= NULL*/)
{
	HANDLE hOutPipeIn = NULL, hOutPipeOut = NULL, hErrorPipeIn = NULL, hErrorPipeOut = NULL;
	SECURITY_ATTRIBUTES sa;

	// creating pipe
	try
	{
		sa.nLength = sizeof(sa);
		sa.bInheritHandle = TRUE;
		sa.lpSecurityDescriptor = NULL;

		if (!CreatePipe(&hOutPipeOut,&hOutPipeIn,&sa,0))
			throw FALSE;

		if (!CreatePipe(&hErrorPipeOut,&hErrorPipeIn,&sa,0))
			throw FALSE;

		if (!CreateHiddenConsole(lpszCmdLine,NULL,hOutPipeIn,hErrorPipeIn,dwCreate,lpszCurDir,lpsiStartInfo))
			throw FALSE;
	}
	catch (...)
	{
		if (hOutPipeIn)
			CloseHandle(hOutPipeIn);

		if (hOutPipeOut)
			CloseHandle(hOutPipeOut);

		if (hErrorPipeIn)
			CloseHandle(hErrorPipeIn);

		if (hErrorPipeOut)
			CloseHandle(hErrorPipeOut);

		return FALSE;
	}

	// close write handles to pipe
	CloseHandle(hOutPipeIn);
	CloseHandle(hErrorPipeIn);

	*phStdOut = hOutPipeOut;
	*phStdError = hErrorPipeOut;

	return TRUE;
}

DWORD CProcess::GetExitCode() const
{
	DWORD dwExitCode;

	if (!GetExitCodeProcess(m_processInfo.hProcess,&dwExitCode))
	{
		AfxThrowProcessException(CProcessException::generic);
		//return ~0U;
	}

	return dwExitCode;
}

DWORD CProcess::WaitForProcess(DWORD dwTimeOut /*= INFINITE*/) const
{
	DWORD dwResult = WaitForSingleObject(m_processInfo.hProcess,dwTimeOut);

	switch (dwResult)
	{
		case WAIT_FAILED:
			AfxThrowProcessException(CProcessException::generic);
			//return WAIT_FAILED;
		case WAIT_TIMEOUT:
			AfxThrowProcessException(CProcessException::generic);
			//return WAIT_TIMEOUT;
		case WAIT_OBJECT_0:
			return GetExitCode();
		default:
			AfxThrowProcessException(CProcessException::generic);
			//return ~0U;
	}
}