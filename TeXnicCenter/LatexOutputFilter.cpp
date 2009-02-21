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
#include "LatexOutputFilter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//-------------------------------------------------------------------
// class CLaTeXOutputFilter
//-------------------------------------------------------------------

CLaTeXOutputFilter::CLaTeXOutputFilter(BOOL bAutoDelete /*= FALSE*/)
		: COutputFilter(bAutoDelete),
		m_nErrors(0),
		m_nWarnings(0),
		m_nBadBoxes(0),
		m_nOutputPages(0),
		m_bFileNameOverLines(false)
{
}

/**
Returns the number of output pages, that have been created by LaTeX.
 */
int CLaTeXOutputFilter::GetNumberOfOutputPages() const
{
	return m_nOutputPages;
}

CString CLaTeXOutputFilter::GetResultString()
{
	CString strResult;
	strResult.Format(STE_LATEX_RESULT,m_nErrors,m_nWarnings,m_nBadBoxes,m_nOutputPages);
	return strResult;
}

BOOL CLaTeXOutputFilter::OnPreCreate()
{
	m_nErrors = 0;
	m_nWarnings = 0;
	m_nBadBoxes = 0;
	m_nOutputPages = 0;

	return TRUE;
}

void CLaTeXOutputFilter::UpdateFileStack(const CString &strLine)
{
	/*
	With the code in this function we catch non-file-related stuff as well.
	Consider the following output from TeX:

	        Underfull \hbox (badness 3323) in paragraph at lines 87--88

	This pushes the wrong filename "badness 3323" on our filestack.
	But that does not matter. We will pop it before detecting the bad box
	in ParseLine - so we have the correct filename again (reported by TeX
	somewhat earlier).
	Lets call all braces "(...)", which are not related to files, "non-file-braces".

	For this function, we need the assume, that TeX closes all non-file-braces
	before reporting an error; at least before we detect it in ParseLine.
	I did not see any other behaviour of TeX and I can not imagine it.
	So I think, this assumption is correct. The assertion	"ASSERT(!m_stackFile.IsEmpty());"
	below will help us to find out, whether I am right or not.
	 */

	int i = 0;
	int nStrMax = strLine.GetLength();

	//If the last run of this func discovered a filename, that
	// was printed out over more than one line, we will directly
	// continue to read it in.
	if (m_bFileNameOverLines)
		goto ScanNextLine;

	for (; i < nStrMax; i++)
	{
		switch (strLine[i])
		{
			case _T('(') :
			{
ScanNextLine:
				int j = i + 1;

				for (;
				        (j < nStrMax)
				        && (strLine[j] != _T('['))
				        && (strLine[j] != _T(')'))
				        && (strLine[j] != _T('('))
				        && (strLine[j] != _T('{'));
				        j++);

				/*
				We need to push even empty strings on the stack, because only this
				assures, that we get every starting and closing brace.
				Consider the following output from TeX (it is ONE! line of output):

				        \OT1/cmr/m/n/8 Punk-ten. $\OML/cmm/m/it/8 K[] \OT1/cmr/m/n/8 = ([]\OML/cmm/m/it
				        /8 ; []; []\OT1/cmr/m/n/8 )$. $\OML/cmm/m/it/8 K[] \OT1/cmr/m/n/8 =

				The first line has a starting brace '(' directly followed by a '['.
				This will produce an empty string (i.e. i == j-1) due to the code
				in the for-next-loop above. But we need the code above - no chance to change it.
				Certainly, adding empty strings, will somehow corrupt our file stack, but as mentioned
				above, we catch other non-file-related stuff as well.
				And the hope/knowledge is, that TeX closes the brace before reporting an error.
				 */

				CString strFile(strLine.Mid(i + 1,(j - 1) - (i + 1) + 1));

				if (m_bFileNameOverLines)
				{
					strFile = m_strPartialFileName + strLine[0] + strFile;
					m_bFileNameOverLines = false;
				}

				//Has the filename been broken up over two or even more lines?
				if ((j == nStrMax) && (nStrMax >= 79) && (CPathTool::GetFileExtension(strFile).GetLength() < 3))
				{
					//Yes - save it and wait for the next line to read
					m_strPartialFileName = strFile;
					m_bFileNameOverLines = true;
					i = j - 1;
				}
				else
				{
					//No - push it
					strFile.TrimLeft();
					strFile.TrimRight();
					strFile.Trim(_T('"'));

					m_stackFile.push(strFile);
					i = j - 1;
				}
			}
			break;

			case _T(')') :
				//If we are good in parsing, this ASSERT should not happen.
				//Comment this out, if it always asserts or try to find the error,
				//where we pop something from the stackFile, what should not be poped.
				//
				// Yeah, well, I found a case:
				// Mostly TeX prints out some parts of the processed tex-file after reporting a bad box.
				// And sometimes not! But if this part contains a closing brace ')', we will get
				// an assertion here somewhat after a wrong poping caused by this ')'. Any Idea?

				ASSERT(!m_stackFile.empty());

				if (!m_stackFile.empty())
					m_stackFile.pop();

				break;
		}
	}
}

void CLaTeXOutputFilter::FlushCurrentItem()
{
	int nItemType = m_currentItem.m_nErrorID;
	m_currentItem.m_nErrorID = -1;

	switch (nItemType)
	{
		case itmError:
			AddError(m_currentItem);
			++m_nErrors;
			break;
		case itmWarning:
		{
			AddWarning(m_currentItem);
			++m_nWarnings;
			break;
		}
		case itmBadBox:
			AddBadBox(m_currentItem);
			++m_nBadBoxes;
			break;

			//default: //no important item -> do nothing
			//    ;
	}

	m_currentItem.Clear();
}

DWORD CLaTeXOutputFilter::ParseLine(const CString& strLine, DWORD dwCookie)
{
	UpdateFileStack(strLine);

	typedef std::tr1::basic_regex<TCHAR> regex_type;
	static const regex_type::flag_type flags = regex_type::ECMAScript | regex_type::icase;

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// parse for new error, warning or bad box

	//Catching Errors
	static const regex_type error1(_T("^! LaTeX Error: (.*)$"),flags);
	static regex_type error2(_T("^! (.*)$"),flags); // This could catch warnings, so run it last

	//Catching Warnings
	static const regex_type warning1(_T(".+warning.*: (.*) on input line ([[:digit:]]+)"),flags); //Catches Latex and package warnings
	static const regex_type warning2(_T(".+warning.*: (.*)"),flags); //Catches LaTeX and package warnings that split over several lines

	//Catching Bad Boxes
	static const regex_type badBox1(_T("^(Over|Under)full \\\\[hv]box .* at lines ([[:digit:]]+)--([[:digit:]]+)"),flags);
	//Use the following only, if you know how to get the source line for it.
	// This is not simple, as TeX is not reporting it.
	//static regex_type	badBox2(_T("^(Over|Under)full \\\\[hv]box .* has occurred while \\output is active"), true);

	//Catching the source line numbers of error/warning types
	// which are reported over more than one line in the output
	static const regex_type line1(_T("l\\.([[:digit:]]+)"),flags);
	static const regex_type line2(_T("line ([[:digit:]]+)"),flags);

	//Catching the number of output pages
	static const regex_type output1(_T("Output written on .* \\((\\d*) page.*\\)"),flags);
	static const regex_type output2(_T("No pages of output"),flags);

	/* ABOUT THE ORDER OF SEARCHING THE OUTPUT

	                Every TeX-User needs to correct an error, if he wants to have a valid document.
	                Therefore, errors are quite rare.
	                For some warnings, but not all, there is also a need to correct them - to get a better document.
	                Therefore, warnings may occure more often than errors.
	                Most users are not going to correct a bad box. Sometimes it is not possible at all (at least
	                for the average TeX-user).
	                Therefore, bad boxes are the most common type of catched entities.

	                To speed up parsing the output, it is preferable to look for the common stuff first.
	                ==> 1. bad boxes; 2. warnings; 3. errors; 4. srcline-numbers; 5. other stuff

	 */
	LPCTSTR const text = strLine;

	if (regex_search(text,results,badBox1))
	{
		FlushCurrentItem();

		//Get the srcline for it.
		// - Mostly TeX reports something like "100--103" ==> so it is the first one.
		// - Sometimes TeX reports "167--47". This comes up, if an input
		// - file was just closed. The first number (n1) means the last line of the input file,
		// - whereas the second number (n2) means the line after the \input-command in the
		// - "master"-file. Certainly there is no need, that we always have (n1 > n2),
		// - but we will not catch the case (n1 < n2) correctly anyway, because we do not
		// - have enough information about "closing files by TeX" here.
		// -
		// - So I make the assumption, that the last line of the input file is very likely
		// - to be greater than the line of the \input-command.
		// - ==> Take min(n1,n2) as the srcline. This will do a good job for more cases than
		// - just taking the first number. But the problem still remains for some (rare) cases.
		// -
		int n1 = _ttoi(strLine.Mid(results.position(2),results.length(2)));
		int n2 = _ttoi(strLine.Mid(results.position(3),results.length(3)));

		m_currentItem = COutputInfo(
		                    m_stackFile.empty() ? _T("") : m_stackFile.top(),
		                    (n1 < n2) ? n1 : n2,
		                    GetCurrentOutputLine(),
		                    strLine.Mid(results.position(1),results.length(1)),
		                    itmBadBox);
	}
	else if (regex_search(text,results,warning1))
	{
		FlushCurrentItem();
		m_currentItem = COutputInfo(
		                    m_stackFile.empty() ? _T("") : m_stackFile.top(),
		                    _ttoi(strLine.Mid(results.position(2),results.length(2))),
		                    GetCurrentOutputLine(),
		                    strLine.Mid(results.position(1),results.length(1)),
		                    itmWarning);
	}
	else if (regex_search(text,results,warning2))
	{
		FlushCurrentItem();
		m_currentItem = COutputInfo(
		                    m_stackFile.empty() ? _T("") : m_stackFile.top(),
		                    0,
		                    GetCurrentOutputLine(),
		                    strLine.Mid(results.position(1),results.length(1)),
		                    itmWarning);
	}
	else if (regex_search(static_cast<LPCTSTR>(strLine),results,error1))
	{
		FlushCurrentItem();
		m_currentItem = COutputInfo(
		                    m_stackFile.empty() ? _T("") : m_stackFile.top(),
		                    0,
		                    GetCurrentOutputLine(),
		                    strLine.Mid(results.position(1),results.length(1)),
		                    itmError);
	}
	else if (regex_search(text,results,error2))
	{
		FlushCurrentItem();
		m_currentItem = COutputInfo(
		                    m_stackFile.empty() ? _T("") : m_stackFile.top(),
		                    0,
		                    GetCurrentOutputLine(),
		                    strLine.Mid(results.position(1),results.length(1)),
		                    itmError);
	}
	else if (regex_search(text,results,line1))
	{
		//Catching the line number of some errors/warnings types, which do not
		// report their place of occurence in their first line. Consider the
		// following example (catched by line1):
		//
		//! Undefined control sequence.
		//<argument> \pa
		//               rskip
		//l.2 \setlength{\pa rskip}{0.5ex}
		if (m_currentItem.m_nSrcLine == 0)
			m_currentItem.m_nSrcLine = _ttoi(strLine.Mid(results.position(1),results.length(1)));
	}
	else if (regex_search(text,results,line2))
	{
		//Only change the SrcLine, if it could not been assigned yet (== 0), !!!
		// because TeX or a TeX-Package is reporting the errorneus input line
		// one line after reporting the error itself. Consider the following
		// example (catched by line2):
		//
		//Package amsmath Warning: Cannot use `split' here;
		//(amsmath)                trying to recover with `aligned' on input line 65.
		//
		//But: Do not change an Item, if the SrcLine could be retrieved from the output!!!
		if (m_currentItem.m_nSrcLine == 0)
			m_currentItem.m_nSrcLine = _ttoi(strLine.Mid(results.position(1),results.length(1)));
	}
	else if (regex_search(text,results,output1))
	{
		//LaTeX said 'Output written on _file_ (%m_nOutputPages% pages, _number_ bytes)'
		m_nOutputPages = _ttoi(strLine.Mid(results.position(1),results.length(1)));
	}
	else if (regex_search(text,results,output2))
	{
		//LaTeX said 'No pages of output'
		m_nOutputPages = 0;
	}

	return dwCookie;
}

BOOL CLaTeXOutputFilter::OnTerminate()
{
	FlushCurrentItem();

	return m_nErrors == 0;
}
