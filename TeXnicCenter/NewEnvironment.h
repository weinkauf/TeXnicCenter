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
#if !defined(AFX_NEWENVIRONMENT_H__CA02E4AE_6D38_4F53_9CB6_3FE9C7D4E068__INCLUDED_)
#define AFX_NEWENVIRONMENT_H__CA02E4AE_6D38_4F53_9CB6_3FE9C7D4E068__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LaTeXCommand.h"

/* Represents a LaTeX environment. See class CLaTeXCommand for further details */
class CNewEnvironment : public CLaTeXCommand  
{
public:
	DECLARE_DYNAMIC(CNewEnvironment)
	CNewEnvironment(const CStyleFile *parent, CString &name, int noOfParams);
	virtual ~CNewEnvironment();

	virtual CString CNewEnvironment::ToString();
};

#endif // !defined(AFX_NEWENVIRONMENT_H__CA02E4AE_6D38_4F53_9CB6_3FE9C7D4E068__INCLUDED_)

/*
 * $Log$
 * Revision 1.2  2005/06/03 22:24:56  owieland
 * + IMPLEMENT/DECLARE_DYNAMIC
 *
 * Revision 1.1  2005/06/03 20:29:43  owieland
 * Initial checkin of package and class parser
 *
 */