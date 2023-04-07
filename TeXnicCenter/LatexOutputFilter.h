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

#if !defined(AFX_LATEXOUTPUTFILTER_H__10B312A4_21EB_11D5_A222_006097239934__INCLUDED_)
#define AFX_LATEXOUTPUTFILTER_H__10B312A4_21EB_11D5_A222_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stack>
#include <cstddef>

#include "OutputFilter.h"
#include "OutputInfo.h"

/**
An object of this class is used to parse the output messages
generated by a TeX/LaTeX-compiler.

@author Sven Wiegand
 */
class CLaTeXOutputFilter : public COutputFilter
{
	/// Name of the log file.
	CString transcriptFileName_;

// construction/destruction
public:
	CLaTeXOutputFilter(BOOL bAutoDelete = FALSE);

// implementation
public:
	/**
	Returns the number of output pages, that have been created by LaTeX.
	 */
	int GetNumberOfOutputPages() const;

	/// Gets the file name of the transcript file.
	const CString& GetTranscriptFileName() const;

// implementation helpers
protected:
	/**
	Parses the given line for the start of new files or the end of
	old files.
	 */
	void UpdateFileStack(const CString &strLine);

	void PushFile(const CString& fileName);
	/**
	Forwards the currently parsed item to the item list.
	 */
	void FlushCurrentItem();

// overridings
public:
	CString GetResultString() const;

private:
	std::match_results<LPCTSTR> results;

protected:
	virtual BOOL OnPreCreate();
	virtual DWORD ParseLine(const CString& strLine, DWORD dwCookie);

	const CString GetCurrentFilePath() const;
	virtual BOOL OnTerminate();
	void SetCurrentItem(const COutputInfo& item);
// types
protected:

	/**
	These constants are describing, which item types is currently
	parsed.
	 */
	enum tagCookies
	{
		itmNone = 0,
		itmError,
		itmWarning,
		itmBadBox
	};

	const COutputInfo CreateItem(const CString &strLine, tagCookies tag, int line = -1) const;
	void UpdateCurrentItem( const CString& strLine, tagCookies tag, int line = -1 );

// attributes
private:
	/** number or errors detected */
	int m_nErrors;

	/** number of warning detected */
	int m_nWarnings;

	/** number of bad boxes detected */
	int m_nBadBoxes;

	/** number of output pages detected */
	int m_nOutputPages;

	/**
	Stack containing the files parsed by the compiler. The top-most
	element is the currently parsed file.
	 */
	typedef std::stack<CString> StringStack;
	StringStack m_stackFile;

	/** The item currently parsed. */
	COutputInfo m_currentItem;

	/** True, if a filename in the TeX-Output spreads over more than one line */
	bool m_bFileNameOverLines;

	/**
	The first part of a filename, if the whole filename
	spreads over more than one line.
	 */
	CString m_strPartialFileName;
};

#endif // !defined(AFX_LATEXOUTPUTFILTER_H__10B312A4_21EB_11D5_A222_006097239934__INCLUDED_)
