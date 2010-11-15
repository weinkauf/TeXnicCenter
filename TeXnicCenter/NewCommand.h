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

#if !defined(AFX_NEWCOMMAND_H__B50E5B1F_5ABA_4F1D_83E8_800A811FC0FC__INCLUDED_)
#define AFX_NEWCOMMAND_H__B50E5B1F_5ABA_4F1D_83E8_800A811FC0FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LaTeXCommand.h"

/** Represents a LaTeX command. See class CLaTeXCommand for further details */
class CNewCommand : public CLaTeXCommand
{
public:
	DECLARE_DYNAMIC(CNewCommand)

	CNewCommand(const WeakStyleFilePtr& parent, const CString &name, int NoOfParam = 0, bool hasStar = false);
	virtual ~CNewCommand();

	virtual const CString ToLaTeX(BOOL showParString = TRUE) const;
	virtual const CString ToString() const;

	/** Returns true, if command ends with a star, like <tt>\section*</tt> */
	bool HasStar() const;

private:
	bool m_HasStar;
};

#endif // !defined(AFX_NEWCOMMAND_H__B50E5B1F_5ABA_4F1D_83E8_800A811FC0FC__INCLUDED_)

/*
 * $Log: NewCommand.h,v $
 * Revision 1.6  2005/06/14 21:12:19  owieland
 * Added doxygen comments
 *
 * Revision 1.5  2005/06/10 14:52:54  owieland
 * Bugfix ToLaTeX (was 'out of call hierachy', now declared as const)
 *
 * Revision 1.4  2005/06/09 22:58:47  owieland
 * Introduced ExpandBefore/After members to allow insertion of additional constructs
 * (useful for environments to insert \\begin-\\end pairs}
 *
 * Revision 1.3  2005/06/09 12:09:59  owieland
 * + Consider ProvidesXXX commands for package/class description
 * + Avoid duplicate option entries
 * + Export description of packages
 * + Consider number of parameters on auto completion
 *
 * Revision 1.2  2005/06/03 22:24:56  owieland
 * + IMPLEMENT/DECLARE_DYNAMIC
 *
 * Revision 1.1  2005/06/03 20:29:43  owieland
 * Initial checkin of package and class parser
 *
 */