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

#include <memory>

#include "LaTeXCommand.h"

typedef CMap<CString, LPCTSTR, std::tr1::shared_ptr<CObject>, std::tr1::shared_ptr<CObject> > SharedObjectMap;

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
const int INVALID_LATEX_ITEM = -5678;

class CStyleFileListener
{
public:
	virtual bool OnQueryCancel() = 0;
	virtual void OnDirectoryFound(const CString &dirname) = 0;
	virtual void OnFileFound(const CString &filename) = 0;
	virtual void OnCommandFound(CLaTeXCommand &command) = 0;
};

/* Represents a LaTeX-Style file or class */
class CStyleFile : public CObject, public std::tr1::enable_shared_from_this<CStyleFile>
{
public:
	const CString ToString() const;

	CStyleFile(const CString &name, const CString &desc,bool isClass = FALSE);
	CStyleFile(CStyleFile &file,bool isClass = FALSE);
	CStyleFile(const CString& m_Filename,bool isClass = FALSE);

	virtual ~CStyleFile();

	virtual void Dump(CDumpContext& dc) const;

	void SetListener(CStyleFileListener* listener);

	void ProcessFile();

	const CString& GetFileName() const;

	const CString& GetName() const;

	const CString& GetDescription() const;

	bool IsDocClass() const;

	const SharedObjectMap* GetCommands() const;

	const CStringArray *GetOptions() const;

	const CStringArray *GetRequiredPackages() const;

	///Adds a list of possible completions of Partial to the Result.
	void GetPossibleItems(const CString& Partial, SharedObjectMap& Result);


	std::tr1::shared_ptr<CNewCommand> AddCommand(const CString &name,int noOfParams = 0, const CString &desc = CString());
	/** Works for both, \newcommand and \newenvironment */
	bool AddCommand(std::tr1::shared_ptr<CLaTeXCommand>& cmd);
	std::tr1::shared_ptr<CNewEnvironment> AddEnvironment(const CString &name,int noOfParams = 0, const CString &desc = CString());
	bool AddOption(std::tr1::shared_ptr<CDeclareOption>& cmd);
	bool AddOption(const CString &name, const CString &desc = CString());

	CStyleFile & operator =(const CStyleFile&);

private:
	int ExtractDescription(const TCHAR *closePar,const TCHAR *openBr,const TCHAR *closeBr, CString &desc);

	void Init();
	CAbstractLaTeXCommand *CreateItem(int type, const CString &name, bool hasStar,int noOfParams);

	CStringArray m_Options;
	CStringArray m_ReqPackages;
	CString m_Name;
	bool m_IsClass;
	CString m_Filename;

	SharedObjectMap m_Commands;
	CStyleFileListener* m_Listener;
	CString m_Desc;

protected:
	void ParseBuffer(const TCHAR *buf);
	bool HasCommands(const TCHAR *buf);
	int ExtractOptionCount(const TCHAR *closePar,const TCHAR *openBr,const TCHAR *closeBr);
};


#endif // !defined(AFX_STYLEFILE_H__E4FDC97C_A031_4E63_890F_3DB0F0DF6950__INCLUDED_)

