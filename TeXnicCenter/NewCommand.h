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



/* Represents a LaTeX command. See class CLaTeXCommand for further details */
class CNewCommand : public CLaTeXCommand  
{
public:
	DECLARE_DYNAMIC(CNewCommand)

	CNewCommand(const CStyleFile *parent, CString &name, int NoOfParam = 0, BOOL hasStar=FALSE);
	virtual ~CNewCommand();

	virtual const CString ToLaTeX(BOOL showParString=TRUE);
	virtual const CString ToString();

	BOOL HasStar() const {return m_HasStar;}
private:
	BOOL m_HasStar;
};

#endif // !defined(AFX_NEWCOMMAND_H__B50E5B1F_5ABA_4F1D_83E8_800A811FC0FC__INCLUDED_)

/*
 * $Log$
 * Revision 1.2  2005/06/03 22:24:56  owieland
 * + IMPLEMENT/DECLARE_DYNAMIC
 *
 * Revision 1.1  2005/06/03 20:29:43  owieland
 * Initial checkin of package and class parser
 *
 */