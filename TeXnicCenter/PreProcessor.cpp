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
#include "PreProcessor.h"
#include "PlaceHolder.h"
#include "Process.h"
#include "RegistryStack.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//-------------------------------------------------------------------
// class CPreProcessorArray
//-------------------------------------------------------------------

int CPreProcessorArray::ExecuteAll(LPCTSTR lpszMainPath,LPCTSTR lpszWorkingDir,HANDLE hOutput)
{
	for (int i = 0; i < GetSize(); i++)
	{
		if (!GetAt(i).Execute(lpszMainPath,lpszWorkingDir,hOutput))
			return i;
	}

	// all processors have been executed successfully
	return -1;
}

void CPreProcessorArray::RemoveDirectorySpecifications()
{
	for (int i = 0; i < GetSize(); ++i)
		ElementAt(i).RemoveDirectorySpecifications();
}

BOOL CPreProcessorArray::SerializeToRegistry(RegistryStack &reg) const
{
	CString strValue;

	for (int i = 0; i < GetSize(); i++)
	{
		strValue.Format(_T("PreProcessor%d"),i);
		reg.Write(strValue,GetAt(i).SerializeToString());
	}

	return TRUE;
}

BOOL CPreProcessorArray::SerializeFromRegistry(RegistryStack &reg)
{
	RemoveAll();

	int i = 0;
	CString strValue;

	while (true)
	{
		CString strPackedInformation;
		CPreProcessor pp;

		strValue.Format(_T("PreProcessor%d"),i++);

		if (!reg.Read(strValue,strPackedInformation))
			break;

		pp.SerializeFromString(strPackedInformation);
		Add(pp);
	}

	return TRUE;
}

void CPreProcessorArray::SaveXml(MsXml::CXMLDOMElement xmlPreProcessors) const
{
	for (int i = 0; i < GetSize(); ++i)
	{
		MsXml::CXMLDOMElement xmlPreProcessor(xmlPreProcessors.GetOwnerDocument().CreateElement(_T("processor")));
		GetAt(i).SaveXml(xmlPreProcessor);
		xmlPreProcessors.AppendChild(xmlPreProcessor);
	}
}

void CPreProcessorArray::LoadXml(MsXml::CXMLDOMElement xmlPreProcessors)
{
	RemoveAll();

	MsXml::CXMLDOMNodeList xmlPreProcessorList(xmlPreProcessors.SelectNodes(_T("processor")));
	const long lProcessors = xmlPreProcessorList.GetLength();
	for (long lProcessor = 0; lProcessor < lProcessors; ++lProcessor)
	{
		MsXml::CXMLDOMElement xmlPreProcessor(xmlPreProcessorList.GetItem(lProcessor).QueryInterface(IID_IXMLDOMElement));
		CPreProcessor pp;
		pp.LoadXml(xmlPreProcessor);
		Add(pp);
	}
}


//-------------------------------------------------------------------
// class CPreProcessor
//-------------------------------------------------------------------

CPreProcessor::CPreProcessor(LPCTSTR lpszTitle /*= _T("")*/,LPCTSTR lpszPath /*= _T("")*/,
                               LPCTSTR lpszArguments /*= _T("")*/,
                               LPCTSTR lpszInputFile /*= _T("")*/,LPCTSTR lpszOutputFile /*= _T("")*/)
{
	ASSERT(lpszTitle && lpszPath && lpszArguments && lpszInputFile && lpszOutputFile);

	m_strTitle = lpszTitle;
	m_strPath = lpszPath;
	m_strArguments = lpszArguments;
	m_strInputFile = lpszInputFile;
	m_strOutputFile = lpszOutputFile;
}

CPreProcessor::~CPreProcessor()
{
}

void CPreProcessor::SetTitle(LPCTSTR lpszTitle)
{
	m_strTitle = lpszTitle;
}

void CPreProcessor::SetPath(LPCTSTR lpszPath)
{
	m_strPath = lpszPath;
}

void CPreProcessor::SetArguments(LPCTSTR lpszArguments)
{
	m_strArguments = lpszArguments;
}

void CPreProcessor::SetInputFile(LPCTSTR lpszInputFile)
{
	m_strInputFile = lpszInputFile;
}

void CPreProcessor::SetOutputFile(LPCTSTR lpszOutputFile)
{
	m_strOutputFile = lpszOutputFile;
}

BOOL CPreProcessor::Execute(LPCTSTR lpszMainPath,LPCTSTR lpszWorkingDir,HANDLE hOutput,PHANDLE phProcess /*= NULL*/)
{
	HANDLE hStdInput = INVALID_HANDLE_VALUE;
	HANDLE hStdOutput = INVALID_HANDLE_VALUE;
	bool bUsedGivenOutputHandle = false;
	DWORD dwExitCode = 0;

	try
	{
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// prepare handles for input and output redirection
		SECURITY_ATTRIBUTES sa =
		{
			sizeof(SECURITY_ATTRIBUTES),
			NULL,
			TRUE
		};

		// Opening file, if input redirection should be used
		if (!m_strInputFile.IsEmpty())
		{
			hStdInput = CreateFile(
			                GetExpandedInputFile(lpszMainPath),GENERIC_READ,FILE_SHARE_READ,
			                &sa,OPEN_EXISTING,0,NULL);
			if (hStdInput == INVALID_HANDLE_VALUE)
				throw FALSE;
		}

		// Creating new file, if output redirection should be used
		if (!m_strOutputFile.IsEmpty())
		{
			hStdOutput = CreateFile(
			                 GetExpandedOutputFile(lpszMainPath),GENERIC_WRITE,0,
			                 &sa,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
			if (hStdOutput == INVALID_HANDLE_VALUE)
				throw FALSE;
		}

		if (hStdOutput == INVALID_HANDLE_VALUE)
		{
			hStdOutput = hOutput;
			//Do not close the handle here;
			// Caller created it and is responsible for closing it
			bUsedGivenOutputHandle = true;
		}

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// create process
		CProcess p;
		BOOL bResult;

		bResult = p.CreateHiddenConsole(
		              m_strPath + _T(' ') + GetExpandedArguments(lpszMainPath),
		              hStdInput,hStdOutput,hOutput,
		              CREATE_NO_WINDOW,lpszWorkingDir);
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
		dwExitCode = ~0U;
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// clean up
	if (hStdInput != INVALID_HANDLE_VALUE)
		CloseHandle(hStdInput);
	if ((hStdOutput != INVALID_HANDLE_VALUE) && (!bUsedGivenOutputHandle))
		CloseHandle(hStdOutput);

	return dwExitCode ? FALSE : TRUE;
}

BOOL CPreProcessor::CancelExecution()
{
	return GenerateConsoleCtrlEvent(CTRL_BREAK_EVENT,GetProcessGroupID());
}

void CPreProcessor::RemoveDirectorySpecifications()
{
	m_strPath = CPathTool::GetFile(m_strPath);
	m_strInputFile = CPathTool::GetFile(m_strInputFile);
	m_strOutputFile = CPathTool::GetFile(m_strOutputFile);
}

CString CPreProcessor::GetExpandedArguments(LPCTSTR lpszPath) const
{
	return AfxExpandPlaceholders(m_strArguments,lpszPath);
}

CString CPreProcessor::GetExpandedInputFile(LPCTSTR lpszPath) const
{
	return AfxExpandPlaceholders(m_strInputFile,lpszPath);
}

CString CPreProcessor::GetExpandedOutputFile(LPCTSTR lpszPath) const
{
	return AfxExpandPlaceholders(m_strOutputFile,lpszPath);
}

CString CPreProcessor::SerializeToString() const
{
	return
	    m_strTitle + _T('\n') +
	    m_strPath + _T('\n') +
	    m_strArguments + _T('\n') +
	    m_strInputFile + _T('\n') +
	    m_strOutputFile;
}

BOOL CPreProcessor::SerializeFromString(LPCTSTR lpszPackedInformation)
{
	CString strTitle,strPath,strArguments,strInputFile,strOutputFile;

	if (!AfxExtractSubString(strTitle,lpszPackedInformation,0,_T('\n')))
		return FALSE;
	if (!AfxExtractSubString(strPath,lpszPackedInformation,1,_T('\n')))
		return FALSE;
	if (!AfxExtractSubString(strArguments,lpszPackedInformation,2,_T('\n')))
		return FALSE;
	if (!AfxExtractSubString(strInputFile,lpszPackedInformation,3,_T('\n')))
		return FALSE;
	if (!AfxExtractSubString(strOutputFile,lpszPackedInformation,4,_T('\n')))
		return FALSE;

	SetTitle(strTitle);
	SetPath(strPath);
	SetArguments(strArguments);
	SetInputFile(strInputFile);
	SetOutputFile(strOutputFile);

	return TRUE;
}

void CPreProcessor::SaveXml(MsXml::CXMLDOMElement xmlPreProcessor) const
{
	xmlPreProcessor.SetAttribute(_T("name"),(LPCTSTR)m_strTitle);
	xmlPreProcessor.SetAttribute(_T("path"),(LPCTSTR)m_strPath);
	xmlPreProcessor.SetAttribute(_T("arguments"),(LPCTSTR)m_strArguments);
	xmlPreProcessor.SetAttribute(_T("inputFile"),(LPCTSTR)m_strInputFile);
	xmlPreProcessor.SetAttribute(_T("outputFile"),(LPCTSTR)m_strOutputFile);
}

void CPreProcessor::LoadXml(MsXml::CXMLDOMElement xmlPreProcessor)
{
	m_strTitle = (LPCTSTR)(_bstr_t)xmlPreProcessor.GetAttribute(_T("name"));
	m_strPath = (LPCTSTR)(_bstr_t)xmlPreProcessor.GetAttribute(_T("path"));
	m_strArguments = (LPCTSTR)(_bstr_t)xmlPreProcessor.GetAttribute(_T("arguments"));
	m_strInputFile = (LPCTSTR)(_bstr_t)xmlPreProcessor.GetAttribute(_T("inputFile"));
	m_strOutputFile = (LPCTSTR)(_bstr_t)xmlPreProcessor.GetAttribute(_T("outputFile"));
}

const CString& CPreProcessor::GetTitle() const
{
	return m_strTitle;
}

const CString& CPreProcessor::GetPath() const
{
	return m_strPath;
}

const CString& CPreProcessor::GetArguments() const
{
	return m_strArguments;
}

const CString& CPreProcessor::GetInputFile() const
{
	return m_strInputFile;
}

const CString& CPreProcessor::GetOutputFile() const
{
	return m_strOutputFile;
}

DWORD CPreProcessor::GetProcessGroupID() const
{
	return m_dwProcessGroupId;
}