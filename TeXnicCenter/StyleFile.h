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

#if !defined(AFX_STYLEFILE_H__E4FDC97C_A031_4E63_890F_3DB0F0DF6950__INCLUDED_)
#define AFX_STYLEFILE_H__E4FDC97C_A031_4E63_890F_3DB0F0DF6950__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LaTeXCommand.h"


class CNewCommand;
class CNewEnvironment;
class CDeclareOption;

/* Token types */
const int LATEX_COMMAND = 0;
const int LATEX_ENVIRONMENT = 1;
const int LATEX_OPTION = 2;
const int LATEX_REQPACKAGE = 3;
const int LATEX_DEF = 4;
const int LATEX_LET = 5;
const int LATEX_CLS_DESC = 6;
const int LATEX_STY_DESC = 7;
const int LATEX_TXT_SYMBOL = 8;
const int LATEX_MATH_SYMBOL = 9;
const int LATEX_TXT_COMMAND = 10;

/* Number of token types */
const int TOKEN_COUNT = 11;

/* Maximum length of desc */
const int MAX_DESC_LEN = 255;

/* Exception ID */
#define INVALID_LATEX_ITEM -5678




class CStyleFileListener
{
public:
	virtual BOOL OnQueryCancel() = 0;
	virtual void OnDirectoryFound(CString &dirname) = 0;
	virtual void OnFileFound(CString &filename) = 0;
	virtual void OnCommandFound(CLaTeXCommand &command) = 0;
};


/* Represents a LaTeX-Style file or class */
class CStyleFile :public CObject
{
public:	
	CString ToString();
	
	CStyleFile(CString &name, CString &desc, BOOL isClass=FALSE);
	CStyleFile(CStyleFile &file, BOOL isClass=FALSE);
	CStyleFile(CString& m_Filename, BOOL isClass=FALSE);
	
	virtual ~CStyleFile();

	virtual void Dump( CDumpContext& dc ) const;

	void SetListener(CStyleFileListener* listener);
	
	void ProcessFile();
	CString GetFilename() const {return m_Filename;};
	CString GetName() const {return m_Name;};
	CString GetDescription() const {return m_Desc;};
	BOOL IsDocClass() const {return m_IsClass;}

	const CMapStringToOb *GetCommands() const {return &m_Commands;}
	const CStringArray *GetOptions() const {return &m_Options;}
	const CStringArray *GetRequiredPackages() const {return &m_ReqPackages;}

	///Adds a list of possible completions of Partial to the Result.
	void GetPossibleItems(const CString& Partial, CMapStringToOb& Result);
	
	
	CNewCommand *AddCommand(CString &name, int noOfParams=0, CString &desc=CString(_T("")));		
	/** Works for both, \newcommand and \newenvironment */
	BOOL AddCommand(CLaTeXCommand *cmd);
	CNewEnvironment *AddEnvironment(CString &name, int noOfParams=0, CString &desc=CString(""));
	BOOL AddOption(CDeclareOption *cmd);
	BOOL AddOption(CString &name, CString &desc=CString(""));

	CStyleFile& operator = (const CStyleFile&);
		
private:
	int ExtractDescription(const TCHAR *closePar, const TCHAR *openBr, const TCHAR *closeBr, CString &desc);
	
	
	void Init();
	CAbstractLaTeXCommand *CreateItem(int type, CString &name, int hasStar, int noOfParams);

	CStringArray			m_Options;
	CStringArray			m_ReqPackages;
	CString					m_Name;
	BOOL					m_IsClass;
	CString					m_Filename;
	CMapStringToOb 			m_Commands;
	CStyleFileListener*		m_Listener;
	CString					m_Desc;
protected:
	

	void ParseBuffer(const TCHAR *buf);
	BOOL HasCommands(const TCHAR *buf);
	int ExtractOptionCount(const TCHAR *closePar, const TCHAR *openBr, const TCHAR *closeBr);
};


#endif // !defined(AFX_STYLEFILE_H__E4FDC97C_A031_4E63_890F_3DB0F0DF6950__INCLUDED_)

