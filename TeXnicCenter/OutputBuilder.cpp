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
#include "TeXnicCenter.h"
#include "OutputBuilder.h"
#include "OutputFilter.h"
#include "LatexOutputFilter.h"
#include "OutputView.h"
#include "OutputDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//-------------------------------------------------------------------
// class COutputBuilder
//-------------------------------------------------------------------

COutputBuilder::COutputBuilder()
:	CWorkerThread(FALSE),
	m_pView(NULL),
	m_pProfile(NULL)
{}


BOOL COutputBuilder::Create(int nMode,
														COutputDoc *pDoc, COutputView *pView, 
														LPCTSTR lpszWorkingDir, LPCTSTR lpszMainPath, 
														BOOL bRunBibTex, BOOL bRunMakeIndex,
														int nPriority /*= THREAD_PRIORITY_BELOW_NORMAL*/)
{
	m_nMode = nMode;

	m_pDoc = pDoc;
	m_pView = pView;
	m_strWorkingDir = lpszWorkingDir;
	m_strMainPath = lpszMainPath;
	m_bRunBibTex = bRunBibTex;
	m_bRunMakeIndex = bRunMakeIndex;

	// get current build profile
	m_pProfile = g_ProfileMap.GetActiveProfile();	
	if (!m_pProfile)
		return FALSE;

	// initialize output view
	if (m_pDoc)
		m_pDoc->ResetBuildInformation();
	if (m_pView)
	{
		CString	strMsg;
		strMsg.Format(STE_LATEX_START, g_ProfileMap.GetActiveProfileKey());
		m_pView->AddLine(strMsg);
	}

	// initialize attributes
	m_strLatexResult.Empty();

	return CWorkerThread::Create(TRUE, nPriority);
}


BOOL COutputBuilder::BuildAll(COutputDoc *pDoc, COutputView *pView, 
															LPCTSTR lpszWorkingDir, LPCTSTR lpszMainPath, 
															BOOL bRunBibTex, BOOL bRunMakeIndex,
															int nPriority /*= THREA_PRIORITY_BELOW_NORMAL*/)
{
	return Create(modeBuildAll, pDoc, pView, lpszWorkingDir, lpszMainPath, bRunBibTex, bRunMakeIndex, nPriority);
}


BOOL COutputBuilder::RunBibTex(COutputDoc *pDoc, COutputView *pView,
															 LPCTSTR lpszWorkingDir, LPCTSTR lpszMainPath,
															 int nPriority /*= THREAD_PRIORITY_BELOW_NORMAL*/)
{
	return Create(modeRunBibTexOnly, pDoc, pView, lpszWorkingDir, lpszMainPath, TRUE, FALSE, nPriority);
}


BOOL COutputBuilder::RunMakeIndex(COutputDoc *pDoc, COutputView *pView,
																	LPCTSTR lpszWorkingDir, LPCTSTR lpszMainPath,
																	int nPriority /*= THREAD_PRIORITY_BELOW_NORMAL*/)
{
	return Create(modeRunMakeIndexOnly, pDoc, pView, lpszWorkingDir, lpszMainPath, FALSE, TRUE, nPriority);
}


BOOL COutputBuilder::CancelExecution()
{
	m_bCancel = TRUE;
	return ::TerminateProcess(m_hCurrentProcess, -1);
}


UINT COutputBuilder::Run()
{
	m_bCancel = FALSE;

	// valid profile available?
	if (!m_pProfile)
		return -1;

	// run (La)TeX-compiler
	if (m_nMode == modeBuildAll)
	{
		if (!RunLatex() || m_bCancel)
			return -1;
	}

	// run BibTeX
	if (m_nMode == modeBuildAll || m_nMode == modeRunBibTexOnly)
	{
		if (!RunBibTex() || m_bCancel)
			return -1;
	}

	// run MakeIndex
	if (m_nMode == modeBuildAll || m_nMode == modeRunMakeIndexOnly)
	{
		if (!RunMakeIndex() || m_bCancel)
			return -1;
	}

	// run postprocessors
	if (m_nMode == modeBuildAll)
	{
		if (!RunPostProcessors() || m_bCancel)
			return -1;
	}
	
	return 0;
}


UINT COutputBuilder::OnTerminate(UINT unExitCode)
{
	if (m_bCancel )
	{
		if (m_pView)
		{
			m_pView->AddLine(_T(""));
			m_pView->AddLine(CString((LPCTSTR)STE_LATEX_CANCELED));
		}
		return -1;
	}
	else
	{
		if (m_pView && !m_strLatexResult.IsEmpty())
		{
			m_pView->AddLine(_T(""));
			m_pView->AddLine(m_strLatexResult);
		}

		return unExitCode;
	}
}


BOOL COutputBuilder::RunLatex()
{
	if (!m_pProfile->GetRunLatex())
		return TRUE;

	CLatexOutputFilter	filter;
	HANDLE	hOutput;
	if (!filter.Create(&hOutput, m_pDoc, m_pView, FALSE))
		return FALSE;

	CProcessCommand	pc;
	pc.Set(m_pProfile->GetLatexPath(), m_pProfile->GetLatexArguments());
	CProcess	*p = pc.Execute(hOutput, m_strWorkingDir, m_strMainPath, NULL, -1);
	if (!p)
	{
		::CloseHandle(hOutput);
		filter.Terminate(0);
		filter.CloseHandle();
		TCHAR systemError[100];
		::FormatMessage(
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK,
			NULL,
			pc.GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			systemError,
			100,
			NULL 
		);

		m_strLatexResult.Format( STE_LATEX_START_FAILED_EXT, pc.GetExecutable(), systemError );
		return FALSE; 
	}

//	m_dwProcessGroupId = p->GetProcessID();
	m_hCurrentProcess = p->GetProcessHandle();
	p->WaitForProcess();
	delete p;
	::CloseHandle(hOutput);
	filter.WaitForThread();

	DWORD	dwExitCode;
	if (!filter.GetExitCode(&dwExitCode))
	{
		filter.CloseHandle();
		return TRUE;
	}

	filter.CloseHandle();
	m_strLatexResult = filter.GetResultString();

	//Error handling
	if (m_pProfile->GetStopOnLatexError())
		//Stop on error code != 0
		return !dwExitCode;
	else
		//Stop only, if no pages of output and error code != 0
		return ( (filter.GetNumberOfOutputPages() > 0) || !dwExitCode);
}


BOOL COutputBuilder::RunBibTex()
{
	if (!m_bRunBibTex || !m_pProfile->GetRunBibTex())
		return TRUE;

	COutputFilter	filter;
	HANDLE	hOutput;
	if (!filter.Create(&hOutput, m_pDoc, m_pView, FALSE))
		return FALSE;

	CProcessCommand	pc;
	pc.Set(m_pProfile->GetBibTexPath(), m_pProfile->GetBibTexArguments());
	CProcess	*p = pc.Execute(hOutput, m_strWorkingDir, m_strMainPath, NULL, -1);
	if (!p)
	{
		::CloseHandle(hOutput);
		filter.Terminate(0);
		filter.CloseHandle();
		TCHAR systemError[100];
		::FormatMessage(
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK,
			NULL,
			pc.GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			systemError,
			100,
			NULL 
		);

		m_strLatexResult.Format( STE_LATEX_START_FAILED_EXT, pc.GetExecutable(), systemError );
		return FALSE; 
	}

	m_hCurrentProcess = p->GetProcessHandle();
	p->WaitForProcess();
	delete p;
	::CloseHandle(hOutput);
	filter.WaitForThread();

	DWORD	dwExitCode;
	if (!filter.GetExitCode(&dwExitCode))
	{
		filter.CloseHandle();
		return TRUE;
	}

	filter.CloseHandle();
	//m_strLatexResult = filter.GetResultString();

	return !dwExitCode;
}


BOOL COutputBuilder::RunMakeIndex()
{
	if (!m_bRunMakeIndex || !m_pProfile->GetRunMakeIndex())
		return TRUE;

	COutputFilter	filter;
	HANDLE	hOutput;
	if (!filter.Create(&hOutput, m_pDoc, m_pView, FALSE))
		return FALSE;

	CProcessCommand	pc;
	pc.Set(m_pProfile->GetMakeIndexPath(), m_pProfile->GetMakeIndexArguments());
	CProcess	*p = pc.Execute(hOutput, m_strWorkingDir, m_strMainPath, NULL, -1);
	if (!p)
	{
		::CloseHandle(hOutput);
		filter.Terminate(0);
		filter.CloseHandle();
		TCHAR systemError[100];
		::FormatMessage(
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK,
			NULL,
			pc.GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			systemError,
			100,
			NULL 
		);

		m_strLatexResult.Format( STE_LATEX_START_FAILED_EXT, pc.GetExecutable(), systemError );
		return FALSE; 
	}

	m_hCurrentProcess = p->GetProcessHandle();
	p->WaitForProcess();
	delete p;
	::CloseHandle(hOutput);
	filter.WaitForThread();

	DWORD	dwExitCode;
	if (!filter.GetExitCode(&dwExitCode))
	{
		filter.CloseHandle();
		return TRUE;
	}

	filter.CloseHandle();
	//m_strLatexResult = filter.GetResultString();

	return !dwExitCode;
}


BOOL COutputBuilder::RunPostProcessors()
{
	COutputFilter	filter;
	HANDLE	hOutput;
	if (!filter.Create(&hOutput, m_pDoc, m_pView, FALSE))
		return FALSE;

	CPostProcessorArray	&a = m_pProfile->GetPostProcessorArray();
	BOOL								bResult = TRUE;

	for (int i=0; ( (i < a.GetSize()) && (!m_bCancel) ); i++)
	{
		if (!a[i].Execute(m_strMainPath, m_strWorkingDir, hOutput, &m_hCurrentProcess) || m_bCancel)
		{
			bResult = FALSE;
			break;
		}
	}

	::CloseHandle(hOutput);
	filter.WaitForThread();
	filter.CloseHandle();

	return bResult;
}