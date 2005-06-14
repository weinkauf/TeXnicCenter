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

#include "AbstractLaTeXCommand.h"

class CStyleFile;


/** 
	Base class for all LaTeX entities provided in a style file, usually commands end environments.

	They may have optional parameters and have configurable text blocks, which are inserted before 
	resp. behind the inserted command. E. g. in environments like
<code>
	\begin{myEnv}
	\end{myEnv}
</code>	
	a common value for the <tt>ExpandBefore</tt> attribute is '\begin{' resp. 
	'}\end{myEnv}' for the <tt>ExpandAfter</tt> attribute.

	The class CNewEnvironment use these values as default.
	
@see CNewCommand, CNewEnvironment
*/
class CLaTeXCommand : public CAbstractLaTeXCommand 
{
public:
	
	DECLARE_DYNAMIC(CLaTeXCommand)		
	CLaTeXCommand(const CStyleFile *parent, CString &name, int m_NoOfParams);
	CLaTeXCommand(CLaTeXCommand &cmd);
	virtual ~CLaTeXCommand();

	/** Returns the number of parameters */
	virtual int GetNoOfParams() {return m_NoOfParams;};		
	/** Returns the text block which is inserted before the command/environment */
	virtual CString GetExpandBefore() const {return m_ExpandBefore;}
	/** Returns the text block which is inserted after the command/environment */
	virtual CString GetExpandAfter() const {return m_ExpandAfter;}
	/** Sets the text block which is inserted before the command/environment */
	virtual void SetExpandBefore(CString &txt);
	/** Sets the text block which is inserted after the command/environment */
	virtual void SetExpandAfter(CString &txt);

private:
	CString m_ExpandAfter;
	CString m_ExpandBefore;
	int m_NoOfParams;
protected:
	const CString GetParameterString() const;
};

/** Abstract interface for parsing events of CStylefile */
class CLaTeXCommandListener
{
public:
	/** Called by the framework, if the parser recognized a command or an environment 
		@param command Reference to the parsed item.	
	*/
	virtual void OnCommandFound(CLaTeXCommand &command) = 0;
};


#endif // !defined(AFX_LATEXCOMMAND_H__05B66EE9_7F79_43A9_9DA1_9B65013E4233__INCLUDED_)

/*
 * $Log$
 * Revision 1.7  2005/06/10 14:52:54  owieland
 * Bugfix ToLaTeX (was 'out of call hierachy', now declared as const)
 *
 * Revision 1.6  2005/06/09 22:58:47  owieland
 * Introduced ExpandBefore/After members to allow insertion of additional constructs
 * (useful for environments to insert \\begin-\\end pairs}
 *
 * Revision 1.5  2005/06/09 12:07:44  owieland
 * Addtional parameter 'showParams' in ToLaTeX()
 *
 * Revision 1.4  2005/06/07 23:14:23  owieland
 * + Load commands from packages.xml
 * + Fixed position of the auto complete listbox / beautified content
 * + Fixed some bugs
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