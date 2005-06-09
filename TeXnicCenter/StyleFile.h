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

class CDeclareOption;

/* Token types */
const int LATEX_COMMAND = 0;
const int LATEX_ENVIRONMENT = 1;
const int LATEX_OPTION = 2;
const int LATEX_REQPACKAGE = 3;
const int LATEX_DEF = 4;
const int LATEX_CLS_DESC = 5;
const int LATEX_STY_DESC = 6;
/* Number of token types */
const int TOKEN_COUNT = 7;
/* Maximum length of desc */
const int MAX_DESC_LEN = 255;

/* Exception ID */
#define INVALID_LATEX_ITEM -5678

/* Represents a LaTeX-Style file or class */
class CStyleFile :public CObject
{
public:	
	CString ToString();
	
	CStyleFile(CString &name, CString &desc, BOOL isClass=FALSE);
	CStyleFile(CStyleFile &file, BOOL isClass=FALSE);
	CStyleFile(CString& m_Filename, BOOL isClass=FALSE);
	
	virtual ~CStyleFile();

	void SetListener(CLaTeXCommandListener *listener);
	
	void ProcessFile();
	CString GetFilename() const {return m_Filename;};
	CString GetName() const {return m_Name;};
	CString GetDescription() const {return m_Desc;};
	BOOL IsDocClass() const {return m_IsClass;}

	const CMapStringToOb *GetCommands() const {return &m_Commands;}
	const CStringArray *GetOptions() const {return &m_Options;}
	const CStringArray *GetRequiredPackages() const {return &m_ReqPackages;}
	
	
	BOOL AddCommand(CString &name, int noOfParams=0, CString &desc=CString(""));
	/** Works for both, \newcommand and \newenvironment */
	BOOL AddCommand(CLaTeXCommand *cmd);
	BOOL AddEnvironment(CString &name, int noOfParams=0, CString &desc=CString(""));
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
	CLaTeXCommandListener	*m_Listener;
	CString					m_Desc;
protected:
	

	void ParseBuffer(TCHAR *buf);
	BOOL HasCommands(const TCHAR *buf);
	int ExtractOptionCount(const TCHAR *closePar, const TCHAR *openBr, const TCHAR *closeBr);
};

class CStyleFileListener
{
public:
	virtual BOOL OnQueryCancel() = 0;
	virtual void OnDirectoryFound(CString &dirname) = 0;
	virtual void OnFileFound(CString &filename) = 0;
	virtual void OnCommandFound(CLaTeXCommand &command) = 0;
};

#endif // !defined(AFX_STYLEFILE_H__E4FDC97C_A031_4E63_890F_3DB0F0DF6950__INCLUDED_)

/*
 * $Log$
 * Revision 1.7  2005/06/09 12:09:59  owieland
 * + Consider ProvidesXXX commands for package/class description
 * + Avoid duplicate option entries
 * + Export description of packages
 * + Consider number of parameters on auto completion
 *
 * Revision 1.6  2005/06/07 23:14:23  owieland
 * + Load commands from packages.xml
 * + Fixed position of the auto complete listbox / beautified content
 * + Fixed some bugs
 *
 * Revision 1.5  2005/06/07 19:48:21  owieland
 * + Parse commands declared via \\def
 * + Revised HasCommands
 * + Added comments
 *
 * Revision 1.4  2005/06/05 16:42:42  owieland
 * Extended user interface (prepare for loading the package rep from XML)
 *
 * Revision 1.3  2005/06/04 10:39:12  owieland
 * Added option and required package support
 *
 * Revision 1.2  2005/06/03 22:28:43  owieland
 * Impl. GetName()
 *
 * Revision 1.1  2005/06/03 20:29:43  owieland
 * Initial checkin of package and class parser
 *
 */