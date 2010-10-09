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
#include "TeXnicCenter.h"
#include "OutputFilter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//-------------------------------------------------------------------
// class COutputFilter
//-------------------------------------------------------------------

COutputFilter::COutputFilter(BOOL bAutoDelete /*= FALSE*/)
		: CWorkerThread(bAutoDelete)
		, m_pView(NULL)
{
}

BOOL COutputFilter::Create(PHANDLE phFilterInput,
                           COutputDoc *pDoc,COutputView *pView,
                           BOOL bAutoCloseHandle /*= TRUE*/,
                           int nPriority /*= THREAD_PRIORITY_BELOW_NORMAL*/,
                           DWORD dwCreationFlags /*= 0*/)
{
	ASSERT(phFilterInput);
	ASSERT(pView);

	m_pDoc = pDoc;
	m_pView = pView;

	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	if (!::CreatePipe(&m_hOutput,phFilterInput,&sa,0))
		return FALSE;

	return CWorkerThread::Create(bAutoCloseHandle,nPriority,dwCreationFlags);
}

void COutputFilter::AddLine(LPCTSTR lpszLine)
{
	if (m_pView)
	{
		if (m_pDoc)
			m_pView->AddLine(m_pDoc->GetCurrentProcessName() + _T("> ") + lpszLine);
		else
			m_pView->AddLine(lpszLine);
	}
}

int COutputFilter::GetCurrentOutputLine() const
{
	return m_pView->GetLineCount() - 1;
}

void COutputFilter::AddError(COutputInfo &error)
{
	++errors_;

	if (m_pDoc)
		m_pDoc->AddError(error);
}

void COutputFilter::AddWarning(COutputInfo &warning)
{
	++warnings_;

	if (m_pDoc)
		m_pDoc->AddWarning(warning);
}

void COutputFilter::AddBadBox(COutputInfo &badbox)
{
	++bad_boxes_;

	if (m_pDoc)
		m_pDoc->AddBadBox(badbox);
}

CString COutputFilter::GetResultString()
{
	return CString();
}

UINT COutputFilter::Run()
{
	errors_ = warnings_ = bad_boxes_ = 0;
	cancel_ = false;

	DWORD dwBytesRead;

	char c;
	std::vector<char> line;
    line.reserve(128);

	DWORD dwCookie = 0;
	BOOL bLastWasNewLine = FALSE;

	while (::ReadFile(m_hOutput,&c,sizeof(c),&dwBytesRead,NULL) && dwBytesRead && !cancel_)
	{
		switch (c)
		{
			case '\n' :
			case '\r' :
				if (!bLastWasNewLine)
				{
					bLastWasNewLine = TRUE;
					
					if (!line.empty()) 
					{
						const CString text(&line[0],line.size());

						AddLine(text);
						dwCookie = ParseLine(text,dwCookie);
						line.clear();
					}
				}
				break;

			default:
				bLastWasNewLine = FALSE;
				line.push_back(c);
				break;
		}
	}

	if (!line.empty())
	{
		const CString text(&line[0],line.size());

		AddLine(text);
		ParseLine(text,dwCookie);
	}

	return !OnTerminate();
}

DWORD COutputFilter::ParseLine(const CString& /*strLine*/,DWORD /*dwCookie*/)
{
	return 0;
}

BOOL COutputFilter::OnTerminate()
{
	return TRUE;
}

int COutputFilter::GetErrorCount() const
{
	return errors_;
}

int COutputFilter::GetWarningCount() const
{
	return warnings_;
}

int COutputFilter::GetBadBoxCount() const
{
	return bad_boxes_;
}

void COutputFilter::Cancel()
{
	cancel_ = true;
}