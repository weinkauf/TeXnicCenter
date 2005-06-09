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
#include "NewCommand.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "StyleFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CNewCommand, CLaTeXCommand) 

CNewCommand::CNewCommand(const CStyleFile *parent, CString &name, int noOfParam, BOOL hasStar)
:CLaTeXCommand(parent, name, noOfParam)
{	
	m_HasStar = hasStar;
	if (name[0] == '\\') { /* remove trailing '\', if neccessary  */
		name.Delete(0);
	}
	if (name[name.GetLength()-1] == '*') { // correct name, if neccessary
		m_HasStar = TRUE;
		name.Delete(name.GetLength()-1);		
	}
	SetName(name);
}

CNewCommand::~CNewCommand()
{

}

const CString CNewCommand::ToString() {
	const CStyleFile *sf = GetStyleFile();
	CString x =  (sf != NULL ? sf->GetName() : CString("???"));
	
	TCHAR buf[5];
	itoa(GetNoOfParams(), buf, 10);
	
	return CString(_T("\\") + GetName() + 
		(m_HasStar ? _T("*") : _T("")) + 
		(GetNoOfParams()>0 ? _T("#") + CString(buf) : _T("")) + 
		" (" + x + ")");
}

const CString CNewCommand::ToLaTeX(BOOL showParString)
{	
	return CString(_T("\\") + 
		GetName() + 
		(m_HasStar ? _T("*") : _T("")) + 
		(showParString ? GetParameterString() : _T("")));
}

/*
 * $Log$
 * Revision 1.4  2005/06/07 23:14:23  owieland
 * + Load commands from packages.xml
 * + Fixed position of the auto complete listbox / beautified content
 * + Fixed some bugs
 *
 * Revision 1.3  2005/06/05 16:41:59  owieland
 * Introduced new base class CAbstractLaTeXCommand for a better
 * understandable class hierarchy and better seperation of options and commands.
 *
 * Revision 1.2  2005/06/03 22:24:56  owieland
 * + IMPLEMENT/DECLARE_DYNAMIC
 *
 * Revision 1.1  2005/06/03 20:29:43  owieland
 * Initial checkin of package and class parser
 *
 */