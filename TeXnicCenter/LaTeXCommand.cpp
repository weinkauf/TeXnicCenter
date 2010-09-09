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
 * $Log: LaTeXCommand.cpp,v $
 * Revision 1.6  2005/06/10 14:52:54  owieland
 * Bugfix ToLaTeX (was 'out of call hierachy', now declared as const)
 *
 * Revision 1.5  2005/06/09 22:58:47  owieland
 * Introduced ExpandBefore/After members to allow insertion of additional constructs
 * (useful for environments to insert \\begin-\\end pairs}
 *
 * Revision 1.4  2005/06/09 12:07:44  owieland
 * Addtional parameter 'showParams' in ToLaTeX()
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

#include "stdafx.h"
#include "StyleFile.h"
#include "LaTeXCommand.h"
#include "AbstractLaTeXCommand.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

const TCHAR* const PAR_STRINGS[] = {_T(""),_T("{}"),_T("{}{}"),_T("{}{}{}"),_T("{}{}{}{}"),_T("{}{}{}{}{}")};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CLaTeXCommand,CAbstractLaTeXCommand)

CLaTeXCommand::CLaTeXCommand(const WeakStyleFilePtr& parent, const CString &name, int noOfParams)
		: CAbstractLaTeXCommand(parent,name)
{
	m_NoOfParams = noOfParams > 0 ? noOfParams : 0;
	//m_ExpandAfter = _T("");
	//m_ExpandBefore = _T("");
}

//CLaTeXCommand::CLaTeXCommand(CLaTeXCommand &cmd)
//: CAbstractLaTeXCommand(cmd)
//{
//    m_NoOfParams = cmd.m_NoOfParams;
//    m_ExpandAfter = cmd.m_ExpandAfter;
//    m_ExpandBefore = cmd.m_ExpandBefore;
//}

CLaTeXCommand::~CLaTeXCommand()
{
}

const CString CLaTeXCommand::GetParameterString() const
{
	if (m_NoOfParams >= 0 && m_NoOfParams < 6)
	{
		return PAR_STRINGS[m_NoOfParams];
	}

	CString tmp(PAR_STRINGS[5]);

	for (int i = 0; i < m_NoOfParams - 5; i++)
	{
		tmp += _T("{}");
	}

	return tmp;
}

void CLaTeXCommand::SetExpandAfter(const CString &txt)
{
	m_ExpandAfter = txt;
}

void CLaTeXCommand::SetExpandBefore(const CString &txt)
{
	m_ExpandBefore = txt;
}

int CLaTeXCommand::GetNoOfParams() const
{
	return m_NoOfParams;
}

const CString CLaTeXCommand::GetExpandBefore() const
{
	return m_ExpandBefore;
}

const CString CLaTeXCommand::GetExpandAfter() const
{
	return m_ExpandAfter;
}