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
#include "PostProcessor.h"
#include "PlaceHolder.h"
#include "Process.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//-------------------------------------------------------------------
// class CPostProcessorArray
//-------------------------------------------------------------------

int CPostProcessorArray::ExecuteAll(LPCTSTR lpszMainPath, LPCTSTR lpszWorkingDir, HANDLE hOutput) const
{
	for (int i = 0; i < GetSize(); i++)
	{
		if (!GetAt(i).Execute(lpszMainPath, lpszWorkingDir, hOutput))
			return i;
	}

	// all processors have been executed successfully
	return -1;
}


BOOL CPostProcessorArray::SerializeToRegistry(CBCGRegistryEx &reg) const
{
	CString	strValue;
	for (int i = 0; i < GetSize(); i++)
	{
		strValue.Format(_T("PostProcessor%d"), i);
		reg.Write(strValue, GetAt(i).SerializeToString());
	}

	return TRUE;
}


BOOL CPostProcessorArray::SerializeFromRegistry(CBCGRegistryEx &reg)
{
	RemoveAll();

	int			i = 0;
	CString	strValue;
	while (TRUE)
	{
		CString					strPackedInformation;
		CPostProcessor	pp;

		strValue.Format(_T("PostProcessor%d"), i++);
		if (!reg.Read(strValue, strPackedInformation))
			break;
		pp.SerializeFromString(strPackedInformation);
		Add(pp);
	}

	return TRUE;
}


//-------------------------------------------------------------------
// class CPostProcessor
//-------------------------------------------------------------------

CPostProcessor::CPostProcessor(LPCTSTR lpszTitle /*= _T("")*/, LPCTSTR lpszPath /*= _T("")*/,
															 LPCTSTR lpszArguments /*= _T("")*/, 
															 LPCTSTR lpszInputFile /*= _T("")*/, LPCTSTR lpszOutputFile /*= _T("")*/)
{
	ASSERT(lpszTitle && lpszPath && lpszArguments && lpszInputFile && lpszOutputFile);

	m_strTitle = lpszTitle;
	m_strPath = lpszPath;
	m_strArguments = lpszArguments;
	m_strInputFile = lpszInputFile;
	m_strOutputFile = lpszOutputFile;
}


CPostProcessor::~CPostProcessor()
{}


void CPostProcessor::SetTitle(LPCTSTR lpszTitle)
{
	m_strTitle = lpszTitle;
}


void CPostProcessor::SetPath(LPCTSTR lpszPath)
{
	m_strPath = lpszPath;
}


void CPostProcessor::SetArguments(LPCTSTR lpszArguments)
{
	m_strArguments = lpszArguments;
}


void CPostProcessor::SetInputFile(LPCTSTR lpszInputFile)
{
	m_strInputFile = lpszInputFile;
}


void CPostProcessor::SetOutputFile(LPCTSTR lpszOutputFile)
{
	m_strOutputFile = lpszOutputFile;
}


BOOL CPostProcessor::Execute(LPCTSTR lpszMainPath, LPCTSTR lpszWorkingDir, HANDLE hOutput, PHANDLE phProcess /*= NULL*/)
{
	HANDLE	hStdInput = INVALID_HANDLE_VALUE;
	HANDLE	hStdOutput = INVALID_HANDLE_VALUE;
	DWORD		dwExitCode = 0;

	try
	{
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// prepare handles for input and output redirection
		SECURITY_ATTRIBUTES	sa = {
			sizeof(SECURITY_ATTRIBUTES),
			NULL,
			TRUE
		};

		// Opening file, if input redirection should be used
		if (!m_strInputFile.IsEmpty())
		{
			hStdInput = CreateFile(
				GetExpandedInputFile(lpszMainPath), GENERIC_READ, FILE_SHARE_READ,
				&sa, OPEN_EXISTING, 0, NULL);
			if (hStdInput == INVALID_HANDLE_VALUE)
				throw FALSE;
		}

		// Creating new file, if output redirection should be used
		if (!m_strOutputFile.IsEmpty())
		{
			hStdOutput = CreateFile(
				GetExpandedOutputFile(lpszMainPath), GENERIC_WRITE, 0,
				&sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hStdOutput == INVALID_HANDLE_VALUE)
				throw FALSE;
		}

		if (hStdOutput == INVALID_HANDLE_VALUE)
			hStdOutput = hOutput;

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// create process
		CProcess	p;
		BOOL			bResult;

		bResult = p.CreateHiddenConsole(
			m_strPath + _T(' ') + GetExpandedArguments(lpszMainPath), 
			hStdInput, hStdOutput, hOutput, 
			CREATE_NEW_PROCESS_GROUP, lpszWorkingDir);
		if (!bResult)
			throw FALSE;

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// wait for process
		m_dwProcessGroupId = p.GetProcessID();
		if (phProcess)
			*phProcess = p.GetProcessHandle();
		dwExitCode = p.WaitForProcess();
	}
	catch (...)
	{
		dwExitCode = -1;
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// clean up
	if (hStdInput != INVALID_HANDLE_VALUE)
		CloseHandle(hStdInput);
	if (hStdOutput != INVALID_HANDLE_VALUE)
		CloseHandle(hStdOutput);

	return dwExitCode? FALSE : TRUE;
}


BOOL CPostProcessor::CancelExecution()
{
	return GenerateConsoleCtrlEvent(CTRL_BREAK_EVENT, GetProcessGroupId());
}


CString CPostProcessor::GetExpandedArguments(LPCTSTR lpszPath) const
{
	return AfxExpandPlaceholders(m_strArguments, lpszPath);
}


CString CPostProcessor::GetExpandedInputFile(LPCTSTR lpszPath) const
{
	return AfxExpandPlaceholders(m_strInputFile, lpszPath);
}


CString CPostProcessor::GetExpandedOutputFile(LPCTSTR lpszPath) const
{
	return AfxExpandPlaceholders(m_strOutputFile, lpszPath);
}


CString CPostProcessor::SerializeToString() const
{
	return
		m_strTitle + _T('\n') +
		m_strPath + _T('\n') +
		m_strArguments + _T('\n') +
		m_strInputFile + _T('\n') +
		m_strOutputFile;
}


BOOL CPostProcessor::SerializeFromString(LPCTSTR lpszPackedInformation)
{
	CString	strTitle, strPath, strArguments, strInputFile, strOutputFile;

	if (!AfxExtractSubString(strTitle, lpszPackedInformation, 0, _T('\n')))
		return FALSE;
	if (!AfxExtractSubString(strPath, lpszPackedInformation, 1, _T('\n')))
		return FALSE;
	if (!AfxExtractSubString(strArguments, lpszPackedInformation, 2, _T('\n')))
		return FALSE;
	if (!AfxExtractSubString(strInputFile, lpszPackedInformation, 3, _T('\n')))
		return FALSE;
	if (!AfxExtractSubString(strOutputFile, lpszPackedInformation, 4, _T('\n')))
		return FALSE;

	SetTitle(strTitle);
	SetPath(strPath);
	SetArguments(strArguments);
	SetInputFile(strInputFile);
	SetOutputFile(strOutputFile);

	return TRUE;
}