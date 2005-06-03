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

#include "StyleFile.h"
#include "LaTeXCommand.h"
#include "NewCommand.h"
#include "NewEnvironment.h"

#ifdef _DEBUG
#undef THIS_FILE
static TCHAR THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const TCHAR* TOKENS[]={"\\newcommand", "\\newenvironment", "\\DeclareOption"};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStyleFile::CStyleFile(CStyleFile &file)
{
	m_Filename = file.m_Filename;
	Init();
}

CStyleFile::CStyleFile(CString& filename)
{
	m_Filename = filename;
	Init();
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

void CStyleFile::ProcessFile()
{
	CFile f;

	try {
		f.Open(m_Filename, CFile::modeRead);
		DWORD l = f.GetLength();

		TCHAR *buf = new TCHAR[l];

		f.Read(buf, l);

		if (HasCommands(buf)) {
			//TRACE("File %s contains commands\n", m_Filename);
			ParseBuffer(buf);
		}

		delete [] buf;
	} catch(CFileException &ex) {
		TRACE("Error opening style file: %s\n",ex);
		f.Close();				
	} 
	
	f.Close();	
}

BOOL CStyleFile::HasCommands(const TCHAR *buf)
{
	return strstr(buf, "\\newcommand") != NULL || 
		strstr(buf, "\\newenvironment") != NULL || 
		strstr(buf, "\\DeclareOption") != NULL;
}

void CStyleFile::Init()
{	
	m_Listener = NULL;
}

void CStyleFile::ParseBuffer(TCHAR *buf)
{
	
	for (int i = 0; i < 3; i++) {
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

			if (i == 0) { /* Commands have a leading '\' */
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
			if (l < 255 && l > 0 && !isPrivate) { // Command valid?
				strncpy((TCHAR*)&nameBuf, pStart, l);
				nameBuf[l] = 0;

				/* Create appropriate instance */
				CLaTeXCommand *lc = CreateItem(i, CString(nameBuf), hasStar, nOptions);

				if (lc != NULL) { /* Instance valid -> Notify container and add to list */
					if (m_Listener != NULL) {
						m_Listener->OnCommandFound(*lc);
					}
					CObject *dummy;
					if (!m_Commands.Lookup(lc->ToString(), dummy)) {
						m_Commands.SetAt(lc->ToString(), lc);
					} else {						
						TRACE("** Duplicate key: %s\n", lc->ToString());
						delete lc;
					}
				}

				//TRACE("--> %s '%s'\n", TOKENS[i], nameBuf);
				if (nOptions > 0) {
					//TRACE("--> Options: %d\n", nOptions);
				}
				if (hasStar > 0) {
					//TRACE("--> has star\n");
				}
			} else { /* For debug purposes only */
				if (l > 0 && !isPrivate) {
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

/* Creates an instance of a LaTeX command (factory pattern) */
CLaTeXCommand *CStyleFile::CreateItem(int type, CString &name, int hasStar, int noOfParams)
{
	switch(type) {
	case LATEX_COMMAND:
		return new CNewCommand(this, name, noOfParams, hasStar);
		break;
	case LATEX_ENVIRONMENT:
		return new CNewEnvironment(this, name, noOfParams);
		break;
	case LATEX_OPTION:
		return NULL;
		break;
	}
	throw INVALID_LATEX_ITEM;
}

void CStyleFile::SetListener(CLaTeXCommandListener *listener)
{
	m_Listener = listener;
}

/*
 * $Log$
 */