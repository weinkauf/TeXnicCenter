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

#if !defined(AFX_OUTPUTINFO_H__2F2F3500_25A9_11D3_929E_DBAEDA762522__INCLUDED_)
#define AFX_OUTPUTINFO_H__2F2F3500_25A9_11D3_929E_DBAEDA762522__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
Class for information outputted by another program
(i.e. error messages of a compiler).

@author Sven Wiegand
 */
class COutputInfo
{
// construction
public:
	/** Standard constructor */
	COutputInfo();

	/** This constructor initializes the attributes of the class with the given values. */
	COutputInfo(const CString& strSrcFile, int nSrcLine, int nOutputLine, const CString& strError = _T(""), int nErrorID = -1);

// operations
public:
	/** Sets all attributes to standard values */
	void Clear();

// attributes
public:
	/** Source file, the error position is in */
	CString m_strSrcFile;

	const CString& GetSourceFile() const { return m_strSrcFile; }
	void SetSourceFile(const CString& val) { m_strSrcFile = val; }

	/** Line number in the source file, the message referrs to */
	int m_nSrcLine;

	int GetSourceLine() const { return m_nSrcLine; }
	void SetSourceLine(int val) { m_nSrcLine = val; }
	/** The error message */
	CString m_strError;

	const CString& GetErrorMessage() const { return m_strError; }
	void SetErrorMessage(const CString& val) { m_strError = val; }
	/** An error code */
	int m_nErrorID;

	int GetErrorID() const { return m_nErrorID; }
	void SetErrorID(int val) { m_nErrorID = val; }
	/** The line number in the output, where the error was reported */
	int m_nOutputLine;
	int GetOutputLine() const { return m_nOutputLine; }
	void SetOutputLine(int val) { m_nOutputLine = val; }
};


/**
Array of COutputInfo

@author Sven Wiegand
 */
typedef CArray<COutputInfo, COutputInfo&> COutputInfoArray;


#endif // !defined(AFX_OUTPUTINFO_H__2F2F3500_25A9_11D3_929E_DBAEDA762522__INCLUDED_)
