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
 * Revision 1.2  2005/06/09 12:07:44  owieland
 * Addtional parameter 'showParams' in ToLaTeX()
 *
 * Revision 1.1  2005/06/05 16:41:57  owieland
 * Introduced new base class CAbstractLaTeXCommand for a better
 * understandable class hierarchy and better seperation of options and commands.
 *
 */
#include "stdafx.h"
#include "texniccenter.h"
#include "AbstractLaTeXCommand.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CAbstractLaTeXCommand, CObject) 
CAbstractLaTeXCommand::CAbstractLaTeXCommand(const CStyleFile *parent, CString &name)
{
	ASSERT(parent != NULL);
	m_Parent = parent;
	m_Name = name;	
}

CAbstractLaTeXCommand::CAbstractLaTeXCommand(CAbstractLaTeXCommand &cmd) {
	m_Parent = cmd.m_Parent;
	m_Name = cmd.m_Name;
}


CAbstractLaTeXCommand::~CAbstractLaTeXCommand()
{

}

const CString CAbstractLaTeXCommand::ToString()
{
	return m_Name;
}


const CString CAbstractLaTeXCommand::ToLaTeX(BOOL showParString)
{
	return m_Name;
}

void CAbstractLaTeXCommand::SetDescription(CString &desc)
{
	m_Description = desc;
}

void CAbstractLaTeXCommand::SetName(CString &name)
{
	m_Name = name;
}
