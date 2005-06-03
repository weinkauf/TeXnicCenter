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
}

CNewCommand::~CNewCommand()
{

}

const CString CNewCommand::ToString() {
	const CStyleFile *sf = GetStyleFile();
	CString x =  (sf != NULL ? sf->GetFilename() : CString("???"));
	
	TCHAR buf[5];
	itoa(m_NoOfParams, buf, 10);
	
	return CString("\\" + GetName() + 
		(m_HasStar ? "*" : "") + 
		(m_NoOfParams>0 ? "#" + CString(buf) : "") + 
		" (" + x + ")");
}

const CString CNewCommand::ToLaTeX()
{
	return CString("\\" + GetName()) + (m_HasStar ? "*" : "");
}

/*
 * $Log$
 * Revision 1.1  2005/06/03 20:29:43  owieland
 * Initial checkin of package and class parser
 *
 */