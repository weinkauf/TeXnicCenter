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
#include "LatexOutputFilter.h"
#include <regex.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//-------------------------------------------------------------------
// class CLatexOutputFilter
//-------------------------------------------------------------------

CLatexOutputFilter::CLatexOutputFilter(BOOL bAutoDelete /*= FALSE*/)
:	COutputFilter(bAutoDelete),
	m_nErrors(0),
	m_nWarnings(0),
	m_nBadBoxes(0)
{}


CString CLatexOutputFilter::GetResultString()
{
	CString	strResult;
	strResult.Format(STE_LATEX_RESULT, m_nErrors, m_nWarnings, m_nBadBoxes);
	return strResult;
}


BOOL CLatexOutputFilter::OnPreCreate()
{
	m_nErrors = 0;
	m_nWarnings = 0;
	m_nBadBoxes = 0;

	return TRUE;
}


void CLatexOutputFilter::UpdateFileStack(const CString &strLine)
{
	for (int i = 0; i < strLine.GetLength(); i++)
	{
		switch (strLine[i])
		{
		case _T('('):
			{
				for (int j = i+1; j < strLine.GetLength() && strLine[j] != _T('[') && strLine[j] != _T(')') && strLine[j] != _T('('); j++);
				if ((j-1) > i)
				{
					CString	strFile(strLine.Mid(i+1, (j-1) - (i+1) + 1));
					strFile.TrimLeft();
					strFile.TrimRight();
					m_stackFile.Push(strFile);
					i = j-1;
				}
			}
			break;

		case _T(')'):
			if (!m_stackFile.IsEmpty())
				m_stackFile.Pop();
			break;
		}
	}
}


void CLatexOutputFilter::FlushCurrentItem()
{
	int	nItemType = m_currentItem.m_nErrorID;
	m_currentItem.m_nErrorID = -1;

	switch (nItemType)
	{
		case itmError:
			AddError(m_currentItem);
			m_nErrors++;
			break;
		case itmWarning:
			AddWarning(m_currentItem);
			m_nWarnings++;
			break;
		case itmBadBox:
			AddBadBox(m_currentItem);
			m_nBadBoxes++;
			break;

		default: //no important item -> do nothing
			;
	}

	m_currentItem.Clear();
}


long CLatexOutputFilter::ExtractLineNumber(const CString &strLine, int nStartChar)
{
	return _ttol(strLine.Mid(nStartChar));
}


DWORD CLatexOutputFilter::ParseLine(CString strLine, DWORD dwCookie)
{
	UpdateFileStack(strLine);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// parse for new error, warning or bad box
	static __JM::RegEx	error1("! LaTeX Error: (.*)$", true);
	static __JM::RegEx	error2("! (.*)$", true);
	static __JM::RegEx	error3("(.*error.*)", true);
	static __JM::RegEx	warning1("LaTeX warning: (.*)", true);
	static __JM::RegEx	warning2("LaTeX .* warning: (.*) on input line ([[:digit:]]+).", true);
	static __JM::RegEx	warning3("(.*warning.*)", true);
	static __JM::RegEx	badBox1("(overfull|underfull \\[hv]box).*in paragraph at lines ([[:digit:]]+)--[[:digit:]]+", true);
	static __JM::RegEx	line1("l.([[:digit:]]+)", true);

	if (error1.Search(strLine))
	{
		FlushCurrentItem();
		m_currentItem = COutputInfo(
			m_stackFile.IsEmpty()? "" : m_stackFile.Top(),
			0,
			GetCurrentOutputLine(),
			strLine.Mid(error1.Position(1), error1.Length(1)),
			itmError);
	}
	else if (error3.Search(strLine))
	{
		FlushCurrentItem();
		m_currentItem = COutputInfo(
			m_stackFile.IsEmpty()? "" : m_stackFile.Top(),
			0,
			GetCurrentOutputLine(),
			strLine.Mid(error3.Position(1), error3.Length(1)),
			itmError);
	}
	else if (warning1.Search(strLine))
	{
		FlushCurrentItem();
		m_currentItem = COutputInfo(
			m_stackFile.IsEmpty()? "" : m_stackFile.Top(),
			0,
			GetCurrentOutputLine(),
			strLine.Mid(warning1.Position(1), warning1.Length(1)),
			itmWarning);
	}
	else if (warning2.Search(strLine))
	{
		FlushCurrentItem();
		m_currentItem = COutputInfo(
			m_stackFile.IsEmpty()? "" : m_stackFile.Top(),
			_ttoi(strLine.Mid(warning2.Position(2), warning2.Length(2))),
			GetCurrentOutputLine(),
			strLine.Mid(warning2.Position(1), warning2.Length(1)),
			itmWarning);
	}
	else if (warning3.Search(strLine))
	{
		FlushCurrentItem();
		m_currentItem = COutputInfo(
			m_stackFile.IsEmpty()? "" : m_stackFile.Top(),
			0,
			GetCurrentOutputLine(),
			strLine.Mid(warning3.Position(1), warning3.Length(1)),
			itmWarning);
	}
	else if (error2.Search(strLine))
	{
		FlushCurrentItem();
		m_currentItem = COutputInfo(
			m_stackFile.IsEmpty()? "" : m_stackFile.Top(),
			0,
			GetCurrentOutputLine(),
			strLine.Mid(error2.Position(1), error2.Length(1)),
			itmError);
	}
	else if (badBox1.Search(strLine))
	{
		FlushCurrentItem();
		m_currentItem = COutputInfo(
			m_stackFile.IsEmpty()? "" : m_stackFile.Top(),
			_ttoi(strLine.Mid(badBox1.Position(2), badBox1.Length(2))),
			GetCurrentOutputLine(),
			strLine.Mid(badBox1.Position(1), badBox1.Length(1)),
			itmBadBox);
	}
	else if (line1.Search(strLine))
	{
		m_currentItem.m_nSrcLine = _ttoi(strLine.Mid(line1.Position(1), line1.Length(1)));
	}

	return dwCookie;
}


BOOL CLatexOutputFilter::OnTerminate()
{
	FlushCurrentItem();

	return m_nErrors == 0;
}