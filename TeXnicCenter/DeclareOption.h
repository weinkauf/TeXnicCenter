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

#if !defined(AFX_DECLAREOPTION_H__251FEC0F_A498_4D2A_817B_5FF0FD3EC37E__INCLUDED_)
#define AFX_DECLAREOPTION_H__251FEC0F_A498_4D2A_817B_5FF0FD3EC37E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LaTeXCommand.h"

/** 
	Represents an option package of a LaTeX package or class.

  */
class CDeclareOption : public CAbstractLaTeXCommand  
{
public:
	CDeclareOption(const CStyleFile *parent, CString &name);
	virtual ~CDeclareOption();

};

#endif // !defined(AFX_DECLAREOPTION_H__251FEC0F_A498_4D2A_817B_5FF0FD3EC37E__INCLUDED_)
