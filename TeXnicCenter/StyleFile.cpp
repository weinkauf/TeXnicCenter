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

/*
 * $Log$
 * Revision 1.12  2005/07/04 23:01:11  owieland
 * Alloc file buffer correctly
 *
 * Revision 1.11  2005/06/22 20:19:50  owieland
 * Added \\let and \\DeclareMathSymbol to parser tokens
 *
 * Revision 1.10  2005/06/11 12:50:57  owieland
 * - Load also expAfter/expBefore attributes from packages.xml
 * - AddCommand/AddEnvironment now return pointers to created object
 *
 * Revision 1.9  2005/06/10 15:34:15  owieland
 * Consider \\DeclareTextCommand and \\DeclareTextSymbol
 *
 * Revision 1.8  2005/06/09 17:09:59  owieland
 * + Revised architecture (moved autocmpl-handling to listbox)
 * + Hilight commands if they are from a class (unsatisfying yet)
 * + Several bugfixes
 *
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


#include "stdafx.h"

#include "StyleFile.h"
#include "LaTeXCommand.h"
#include "NewCommand.h"
#include "NewEnvironment.h"
#include "TeXnicCenter.h"
#include "DeclareOption.h"

#ifdef _DEBUG
#undef THIS_FILE
static TCHAR THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const TCHAR* TOKENS[]={
	"\\newcommand", 
	"\\newenvironment", 
	"\\DeclareOption", 
	"\\RequirePackage", 
	"\\def",
	"\\let",
	"\\ProvidesClass",
	"\\ProvidesPackage",
	"\\DeclareTextSymbol",
	"\\DeclareMathSymbol",
	"\\DeclareTextCommand"
};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStyleFile::CStyleFile(CString &name, CString &desc, BOOL isClass)
{
	m_Filename = _T("");
	m_Name = name;
	m_Desc = desc;
	m_IsClass = isClass;
	Init();
}

CStyleFile::CStyleFile(CStyleFile &file, BOOL isClass)
{
	m_Filename = file.m_Filename;
	m_Name = file.GetName();
	m_Desc = _T("");
	m_IsClass = isClass;
	Init();
}

CStyleFile::CStyleFile(CString& filename, BOOL isClass)
{
	m_Filename = filename;
	m_Name = CPathTool::GetFileTitle(filename);
	m_Desc = _T("");
	m_IsClass = isClass;
	Init();
}

void CStyleFile::Init()
{	
	m_Listener = NULL;
}

CStyleFile::~CStyleFile()
{
	POSITION pos = m_Commands.GetStartPosition();
	while(pos != NULL) {
		CLaTeXCommand *sf;
		CString key;
		m_Commands.GetNextAssoc(pos, key, (CObject*&)sf);
		if (sf != NULL) {
			//TRACE("Deleting cmd %s\n", sf->ToString());
			delete sf;
		} else {
			TRACE("NP detected in file %s", m_Filename);
		}
		m_Commands.RemoveKey(key);
	}
}

CStyleFile& CStyleFile::operator = (const CStyleFile& sf) 
{
	if (this != &sf) {
		m_Filename = sf.m_Filename;
	}
	return *this;
}

/**
 Processes a file, if it contains valuable commands. The file is loaded into a buffer
 a is passed to <it>ParseBuffer</it>, where all commands will be extracted. 
 */
void CStyleFile::ProcessFile()
{
	CFile f;

	try {
		f.Open(m_Filename, CFile::modeRead);
		DWORD l = f.GetLength();

		TCHAR *buf = new TCHAR[l + 1];
		f.Read(buf, l);

		if (HasCommands(buf)) {
			ParseBuffer(buf);
		}

		delete [] buf;
	} catch(CFileException &ex) {
		TRACE("Error opening style file: %s\n",ex);
		f.Close();				
	} 
	
	f.Close();	
}

/**
 Checks if a buffer contains given keywords.
 */
BOOL CStyleFile::HasCommands(const TCHAR *buf)
{
	for (int i = 0; i < TOKEN_COUNT; i++) {
		if (strstr(buf, TOKENS[i]) != NULL) {
			return TRUE;
		}
	}
	return FALSE;
}

/**
 Parses a buffer and creates appropriate instances. It works on a very low level.
 Ugly, but fast :-)
 */
void CStyleFile::ParseBuffer(TCHAR *buf)
{
	
	for (int i = 0; i < TOKEN_COUNT; i++) {
		TCHAR *t, *token;
		TCHAR nameBuf[255];
		int lTok = strlen(TOKENS[i]);		

		token = strstr(buf, TOKENS[i]);
		while( token != NULL ) {
			t = token;
			token += lTok;		  			
			int hasStar = 0;
			int isPrivate = 0;
			int nOptions = 0;

			TCHAR *first = strstr(token, "\\");
			TCHAR *second = strstr(token, "{");
			TCHAR *close = strstr(token, "}");
			TCHAR *openBr = strstr(token, "[");
			TCHAR *closeBr = strstr(token, "]");
			
			TCHAR *p;
			
			if (first == NULL && second != NULL) {
				p = second;
			} else if (first != NULL && second == NULL) {
				p = first;
			} else if (first != NULL && second != NULL) {
				p = (first < second ? first : second);
			} else {
				token = strstr(token + 1, TOKENS[i]);
				continue;
			}

			if (*token == '*') {
				hasStar = 1;
			}

			if (i == LATEX_COMMAND) { /* Commands have a leading '\' */
				token++;				
			}

			/* Get number of options */
			nOptions = ExtractOptionCount(close, openBr, closeBr);			
			
			while (!(isalpha(*p) || isdigit(*p) || *p == '@')) p++; /* skip comments, white spaces, etc... */

			TCHAR *pStart = p;
			while (isalpha(*p) || isdigit(*p) || *p == '@') {
				if (*p == '@') {
					isPrivate = 1; /* mark as internal command */ 
				}
				p++;
			}

			int l = p - pStart;
			if (l < 255 && l > 1 && !isPrivate) { /* Command valid? */
				strncpy((TCHAR*)&nameBuf, pStart, l);
				nameBuf[l] = 0;

				/* Create appropriate instance */
				CAbstractLaTeXCommand *lc = CreateItem(i, CString(nameBuf), hasStar, nOptions);

				if (lc != NULL) { /* Instance valid -> Notify container and add to list */
					
					switch (i) {
					case LATEX_COMMAND:
					case LATEX_ENVIRONMENT:
					case LATEX_DEF:
					case LATEX_LET:
					case LATEX_TXT_SYMBOL:
					case LATEX_MATH_SYMBOL:
					case LATEX_TXT_COMMAND:
						/* Some commands may be duplicate due to conditional definitions in the
						   style file. */
						if (!AddCommand((CLaTeXCommand*)lc)) {
							delete lc;
						}
						break;
					case LATEX_OPTION:
						if (!CStyleFileContainer::ContainsString(&m_Options, lc->GetName())) {
							m_Options.Add(lc->GetName());
						}
						delete lc; /* currently not needed */
						break;
					case LATEX_REQPACKAGE:						
						m_ReqPackages.Add(lc->GetName());
						delete lc; /* currently not needed */
						break;
					case LATEX_CLS_DESC:
					case LATEX_STY_DESC:						
						ExtractDescription(close, openBr, closeBr, m_Desc);
						TRACE("Desc: %s: <%s>\n", m_Name, m_Desc);
						delete lc;
						break;
					}
				}
			} else { /* For debug purposes only */
				if (l > 1 && !isPrivate) {
					TRACE("!! Buffer to small: Needs %d bytes\n", l);
				} else if (!isPrivate) {
					strncpy(nameBuf, t, 50);
					strncpy(((TCHAR*)(&nameBuf)+50), "...", 3);
					nameBuf[53] = 0;

					TRACE("** Invalid name: %s\n", nameBuf);

					strncpy(nameBuf, pStart, 10);
					strncpy(((TCHAR*)(&nameBuf)+10), "...", 3);
					nameBuf[13] = 0;

					TRACE("** pStart = %s\n", nameBuf);
					
					strncpy(nameBuf, p, 10);
					strncpy(((TCHAR*)(&nameBuf)+10), "...", 3);
					nameBuf[13] = 0;

					TRACE("** p = %s\n", nameBuf);
				}
			}

			/* Get next token: */
			token = strstr(token + 1, TOKENS[i]);	  
	   }
	}
}

/**
 Retrieves the number of options of a command or environment. Does not work for commands
 defined via '\def' (TeX-Style).
 */
int CStyleFile::ExtractOptionCount(const TCHAR *closePar, const TCHAR *openBr, const TCHAR *closeBr)
{
	if (closePar == NULL || openBr == NULL || closeBr == NULL) return -2; /* No option avalable */

	int d = closeBr - openBr;
	if (d < 0 || d > 2) return -1;
	TCHAR *p = (TCHAR*)closePar;
	TCHAR buf[10];

	p++;
	while (p < openBr) {
		if (isdigit(*p) || isalpha(*p)) {
			return -3;
		}
		p++;
	}

	strncpy(buf, openBr + 1, d);
	buf[d] = 0;

	return atoi(buf);
}

/** 
 Creates an instance of a LaTeX command (factory pattern) 
 */
CAbstractLaTeXCommand *CStyleFile::CreateItem(int type, CString &name, int hasStar, int noOfParams)
{
	switch(type) {
	case LATEX_COMMAND:
	case LATEX_DEF:
	case LATEX_LET:
		return new CNewCommand(this, name, noOfParams, hasStar);
		break;
	case LATEX_TXT_SYMBOL:
	case LATEX_MATH_SYMBOL:
		return new CNewCommand(this, name, 0, hasStar);
		break;
	case LATEX_TXT_COMMAND:
		return new CNewCommand(this, name, noOfParams, hasStar);
		break;
	case LATEX_ENVIRONMENT:
		return new CNewEnvironment(this, name, noOfParams);
		break;
	case LATEX_OPTION:		
		return new CDeclareOption(this, name);
		break;
	case LATEX_REQPACKAGE:
		return new CDeclareOption(this, name); /* murx */
		break;
	case LATEX_CLS_DESC:
	case LATEX_STY_DESC:
		return new CDeclareOption(this, name); /* murx */
		break;
	}
	throw INVALID_LATEX_ITEM;
}

/**
 Registers a listener for style file events. An existing listener will be dropped. 
 */
void CStyleFile::SetListener(CStyleFileListener* listener)
{
	m_Listener = listener;
}


/** 
 Adds a command to the style file. Returns true, if command was inserted successfully or FALSE,
 if not (e. g. command already exists)
 */
BOOL CStyleFile::AddCommand(CLaTeXCommand *cmd)
{
	CObject *dummy;
	if (!m_Commands.Lookup(cmd->ToString(), dummy)) {
		m_Commands.SetAt(cmd->ToString(), cmd);
		if (m_Listener != NULL) {
			m_Listener->OnCommandFound(*cmd);
		}
		return TRUE;
	} else {						
		TRACE("** Duplicate key: %s\n", cmd->ToString());		
	}	
	return FALSE;
}

/** 
 Adds an option to the style file. Returns true, if command was inserted successfully or FALSE,
 if not (e. g. command already exists)
 */
BOOL CStyleFile::AddOption(CDeclareOption *cmd)
{
	if (!CStyleFileContainer::ContainsString(&m_Options, cmd->GetName())) {
		m_Options.Add(cmd->GetName());
		return TRUE;
	}
	return FALSE;
}


/**
 Creates a LaTeX command (\newcommand) with the given parameters and sets
 current style file as parent. The parameter desc is an optional description.
 */
CNewCommand *CStyleFile::AddCommand(CString &name, int noOfParams, CString &desc)
{
	CNewCommand *nc = new CNewCommand(this, name, noOfParams);
	nc->SetDescription(desc);

	if (AddCommand(nc)) {
		return nc;
	} else {
		delete nc;
	}
	return NULL;
}

/**
 Adds an option to this style file.
 */
BOOL CStyleFile::AddOption(CString &name, CString &desc)
{
	CDeclareOption *d = new CDeclareOption(this, name);
	if (AddOption(d)) {
		return TRUE;
	} else {
		delete d;
	}
	return FALSE;
}

/**
 Creates a LaTeX environment (\newenvironment) with the given parameters and sets
 current style file as parent. The parameter desc is an optional description.
 */

CNewEnvironment *CStyleFile::AddEnvironment(CString &name, int noOfParams, CString &desc)
{
	CNewEnvironment *ne = new CNewEnvironment(this, name, noOfParams);
	ne->SetDescription(desc);

	if (AddCommand(ne)) {
		return ne;
	} else {
		delete ne;
	}
	return NULL;
}

/**
 Extracts a dscription string from a \ProvidesXXX command. Works similar as ExtractOptionCount
 */
int CStyleFile::ExtractDescription(const TCHAR *closePar, const TCHAR *openBr, const TCHAR *closeBr, CString &desc)
{
	if (closePar == NULL || openBr == NULL || closeBr == NULL) {
		desc.Empty();
		return -2; /* No desc avalable */
	}

	int d = closeBr - openBr - 1;
	if (d <= 0) {
		desc.Empty();
		return -4;
	}

	if (d >= MAX_DESC_LEN) {
		d = MAX_DESC_LEN - 1;
	}
	TCHAR *p = (TCHAR*)closePar;

	p++;
	while (p < openBr) {
		if (isdigit(*p) || isalpha(*p)) {
			desc.Empty();
			return -3;
		}
		p++;
	}

	TCHAR buf[MAX_DESC_LEN];
	strncpy(buf, openBr + 1, d);
	buf[d] = 0;
	desc = buf;

	return 0;
}

CString CStyleFile::ToString()
{
	return CString((m_IsClass ? _T("Class ") : _T("File ")) + m_Name);
}

void CStyleFile::Dump( CDumpContext& dc ) const {
	CObject::Dump(dc);
	dc << m_Name;	
}


void CStyleFile::GetPossibleItems(const CString& Partial, CMapStringToOb& Result)
{
	const int SearchLength = Partial.GetLength();
	ASSERT(SearchLength != 0);

	POSITION pos = m_Commands.GetStartPosition();
	while(pos != NULL)
	{
		CString key;
		CObject* pObj = NULL;
		m_Commands.GetNextAssoc(pos, key, pObj);

		CLaTeXCommand* pLatexCmd = dynamic_cast<CLaTeXCommand*>(pObj);
		if (!pLatexCmd) continue;

		if ((key.GetLength() >= SearchLength) && (key.Left(SearchLength).CompareNoCase(Partial) == 0))
		{
			Result[key] = pLatexCmd;

			/*
			if (!tmp->Lookup(key, (CObject*&)LatexCmd) || LatexCmd->GetParent()->IsDocClass()) {
				tmp->SetAt(LatexCmd->ToLaTeX(), LatexCmd);			 		
			}
			*/
		}
	}	
}


