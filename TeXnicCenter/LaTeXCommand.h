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

#if !defined(AFX_LATEXCOMMAND_H__05B66EE9_7F79_43A9_9DA1_9B65013E4233__INCLUDED_)
#define AFX_LATEXCOMMAND_H__05B66EE9_7F79_43A9_9DA1_9B65013E4233__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStyleFile;


/* Base class for all LaTeX entities */
class CLaTeXCommand : public CObject 
{
public:
	DECLARE_DYNAMIC(CLaTeXCommand)		
	CLaTeXCommand(const CStyleFile *parent, CString &name, int m_NoOfParams);
	CLaTeXCommand(CLaTeXCommand &cmd);
	virtual ~CLaTeXCommand();

	CString &GetName() {return m_Name;};
	/* Returns the number of parameters */
	virtual int GetNoOfParams() {return m_NoOfParams;};	
	/* Returns a pointer to the containing style or class file */
	const CStyleFile *GetStyleFile() {return m_Parent;};
	/* Returns the LaTeX string representation */
	virtual const CString ToLaTeX();
	/* Returns a unique identifier of the object (may be used as hash value) */
	virtual const CString ToString();
protected:
	int					m_NoOfParams;

private:	
	CString				m_Name;
	const CStyleFile	*m_Parent;

};

/* Listener interface */
class CLaTeXCommandListener
{
public:
	virtual void OnCommandFound(CLaTeXCommand &command) = 0;
};


#endif // !defined(AFX_LATEXCOMMAND_H__05B66EE9_7F79_43A9_9DA1_9B65013E4233__INCLUDED_)

/*
 * $Log$
 * Revision 1.1  2005/06/03 20:29:43  owieland
 * Initial checkin of package and class parser
 *
 */