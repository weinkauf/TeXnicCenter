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

#if !defined(AFX_ABSTRACTLATEXCOMMAND_H__F6CB60C7_B5EA_4FFC_A1A1_DEAEDB79A29C__INCLUDED_)
#define AFX_ABSTRACTLATEXCOMMAND_H__F6CB60C7_B5EA_4FFC_A1A1_DEAEDB79A29C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStyleFile;

/**
 CAbstractLaTeXCommand is the base class for all LaTeX constructs handled by
 CStylefile(Container).
 */
class CAbstractLaTeXCommand : public CObject  
{
public:
	
	DECLARE_DYNAMIC(CAbstractLaTeXCommand)
	CAbstractLaTeXCommand(const CStyleFile *parent, CString &name);
	CAbstractLaTeXCommand(CAbstractLaTeXCommand &cmd);

	virtual ~CAbstractLaTeXCommand();
	/* Returns a pointer to the containing style or class file */
	const CStyleFile *GetStyleFile() const {return m_Parent;};
	/* Returns the LaTeX string representation */
	virtual CString ToLaTeX(BOOL showParString=TRUE) const {return m_Name;}
	/* Returns a unique identifier of the object (may be used as hash value) */
	virtual CString ToString();
	/* Maintain description */
	void SetDescription(CString &desc);
	CString GetDescription() const {return m_Description;};
	/* Name of the command (without any decorations )*/
	CString GetName() const {return m_Name;}
	/* Returns a pointer to the style file which contains this command */
	const CStyleFile *GetParent() const {return m_Parent;}

	virtual void Dump( CDumpContext& dc ) const;
private:	
	CString				m_Description;
	CString				m_Name;
	const CStyleFile	*m_Parent;
protected:
	void SetName(CString &name);
};

#endif // !defined(AFX_ABSTRACTLATEXCOMMAND_H__F6CB60C7_B5EA_4FFC_A1A1_DEAEDB79A29C__INCLUDED_)
