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

#ifndef __CUSERTOOL_H__
#define __CUSERTOOL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BCGUserTool.h"


class CUserTool : public CBCGUserTool
{
	DECLARE_SERIAL(CUserTool)

// Construction/Destruction
public:
	CUserTool();
	virtual ~CUserTool();

// Operations
public:

// Overrides
public:
	virtual void Serialize(CArchive& ar);
	virtual BOOL Invoke();

// Implementation helpers
public:
	///Returns a description of the advanced options
	CString GetAdvDescription();


// Attributes
public:
	///Whether to use TXC's output window or not
	bool m_bUseOutputWindow;

	///Whether to prompt for arguments before invoking or not
	bool m_bPromptForArguments;

	///Whether to close the console window after execution or not
	bool m_bCloseConsoleWindow;

	/** 
	Path of the file to use as standard input for the postprocessor
	or an empty string, if no input redirection should be used.
	*/
	CString m_strInputFile;

	/** 
	Path of the file to write the output of the processor to
	or an empty string, if no output redirection should be used.
	*/
	CString m_strOutputFile;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //__CUSERTOOL_H__
