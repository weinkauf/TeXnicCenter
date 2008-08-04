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
 * $Log: AbstractLaTeXCommand.cpp,v $
 * Revision 1.6  2006/03/24 22:56:23  owieland
 * Added opportunity to decorate each command/environment with an icon
 *
 * Revision 1.5  2005/06/11 12:48:34  owieland
 * Added Dump support
 *
 * Revision 1.4  2005/06/09 22:58:47  owieland
 * Introduced ExpandBefore/After members to allow insertion of additional constructs
 * (useful for environments to insert \\begin-\\end pairs}
 *
 * Revision 1.3  2005/06/09 17:09:59  owieland
 * + Revised architecture (moved autocmpl-handling to listbox)
 * + Hilight commands if they are from a class (unsatisfying yet)
 * + Several bugfixes
 *
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
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CAbstractLaTeXCommand,CObject)

CAbstractLaTeXCommand::CAbstractLaTeXCommand(const CStyleFile *parent, const CString &name)
		: m_Parent(parent)
		, m_Name(name)
		, m_IconIndex(-1)
{
	ASSERT(parent != NULL);
}

CAbstractLaTeXCommand::~CAbstractLaTeXCommand()
{
}

const CString CAbstractLaTeXCommand::ToString() const
{
	return m_Name;
}

void CAbstractLaTeXCommand::SetDescription(const CString &desc)
{
	m_Description = desc;
}

void CAbstractLaTeXCommand::SetName(const CString &name)
{
	m_Name = name;
}

void CAbstractLaTeXCommand::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
	dc << m_Name;

	if (m_Parent != NULL)
	{
		dc << "/";
		dc << m_Parent->GetName();
	}
}

const CString& CAbstractLaTeXCommand::GetIconFileName() const
{
	return m_IconFile;
}

void CAbstractLaTeXCommand::SetIconFileName(const CString& file)
{
	m_IconFile = file;
}

int CAbstractLaTeXCommand::GetIconIndex() const
{
	return m_IconIndex;
}

void CAbstractLaTeXCommand::SetIconIndex(int idx)
{
	m_IconIndex = idx;
}

const CStyleFile * CAbstractLaTeXCommand::GetStyleFile() const
{
	return m_Parent;
}

const CString CAbstractLaTeXCommand::ToLaTeX(BOOL /*showParString*/ /*= TRUE*/) const
{
	return m_Name;
}

const CString& CAbstractLaTeXCommand::GetDescription() const
{
	return m_Description;
}

const CString& CAbstractLaTeXCommand::GetName() const
{
	return m_Name;
}

const CStyleFile * CAbstractLaTeXCommand::GetParent() const
{
	return m_Parent;
}

COLORREF CAbstractLaTeXCommand::GetIconMaskColor() const
{
	return icon_mask_color_;
}

const CString& CAbstractLaTeXCommand::GetHighColorIconFileName() const
{
	return high_color_icon_fn_;
}

void CAbstractLaTeXCommand::SetHighColorIconFileName(const CString& file)
{
	high_color_icon_fn_ = file;
}