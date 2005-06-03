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

const int LATEX_COMMAND = 0;
const int LATEX_ENVIRONMENT = 1;
const int LATEX_OPTION = 2;

#define INVALID_LATEX_ITEM -5678

/* Represents a LaTeX-Style file or class */
class CStyleFile :public CObject
{
public:	
	
	CStyleFile(CStyleFile &file);
	CStyleFile(CString& m_Filename);
	
	virtual ~CStyleFile();

	void SetListener(CLaTeXCommandListener *listener);
	CLaTeXCommand *CreateItem(int type, CString &name, int hasStar, int noOfParams);
	void ProcessFile();
	CString GetFilename() const {return m_Filename;};

	CStyleFile& operator = (const CStyleFile&);
		
private:
	CString m_Name;
	BOOL m_IsClass;
	void Init();


	CString m_Filename;
	CMapStringToOb 	m_Commands;
	CLaTeXCommandListener *m_Listener;
protected:
	void ParseBuffer(TCHAR *buf);
	BOOL HasCommands(const TCHAR *buf);
	int ExtractOptionCount(const TCHAR *closePar, const TCHAR *openBr, const TCHAR *closeBr);
};

class CStyleFileListener
{
public:
	virtual void OnDirectoryFound(CString &dirname) = 0;
	virtual void OnFileFound(CString &filename) = 0;
	virtual void OnCommandFound(CLaTeXCommand &command) = 0;
};

#endif // !defined(AFX_STYLEFILE_H__E4FDC97C_A031_4E63_890F_3DB0F0DF6950__INCLUDED_)

/*
 * $Log$
 */