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

#if !defined(AFX_TEXTMODULES_H__B7379B81_2D99_11D8_B1A8_00095B41B65A__INCLUDED_)
#define AFX_TEXTMODULES_H__B7379B81_2D99_11D8_B1A8_00095B41B65A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/** @addtogroup textmodules

        @{
 */

/** Represents a user definable piece of text to be inserted.

A TextModule is a piece of text, that is often used in documents
and therefore shall be defined once and recalled often.
Examples of such are LaTeX commands like <em>"\emph{}"</em> or <em>"\textbf{}"</em>,
often used phrases like <em>", i.e."</em> or <em>"An illustration is given in Figure \ref{}."</em>
and even longer stuff like addresses.

TextModules can also be called "user definable insert commands".

@todo TextModules can be customized using parameters.

@todo A shortcut can be given to a TextModule.
        This makes it available while typing without using the mouse.

@todo Export to and Import from XML.

@see CTextModuleGroup

@author Tino Weinkauf
 */
class CTextModule
{
//Constructor / Destructor
public:
	///Constructor
	CTextModule();

	//Element Constructor
	//CTextModule(LPCTSTR lpszText, bool bEnableParams,...) //TODO

	///Destructor
	virtual ~CTextModule();


	//Operators
	/** @{*/
	/** @name Operators*/
public:
	///Assignment - assigns new values to this (left) item

	inline void operator=(const CTextModule &arg)
	{
		Name = arg.Name;
		strText = arg.strText;
		bEnableParams = arg.bEnableParams;
	};

	///Comparison of Equality - true, if equal

	inline bool operator==(const CTextModule &arg) const
	{
		return (Name == arg.Name);
	};

	///Comparison: Less

	inline bool operator<(const CTextModule &arg) const
	{
		return (Name < arg.Name);
	};

	///Comparison: Greater

	inline bool operator>(const CTextModule &arg) const
	{
		return (Name > arg.Name);
	};

	///Comparison: LessEqual

	inline bool operator<=(const CTextModule &arg) const
	{
		return (Name <= arg.Name);
	};

	///Comparison: GreaterEqual

	inline bool operator>=(const CTextModule &arg) const
	{
		return (Name >= arg.Name);
	};

	/* @} */

//Methods
public:
	/** Sets the text template of the module.
	                Do \b not provide a "\001" TCHAR!
	                Translates any "\n" into a "\r".
	 */
	bool SetText(const CString& strLeft,const CString& strRight);

	/** Returns the left part of the text.
	                This is everything before the "\001" TCHAR.
	                Translates any "\r" into a "\n".
	 */
	CString GetLeftText();

	/** Returns the right part of the text.
	                This is everything after the "\001" TCHAR.
	                Translates any "\r" into a "\n".
	 */
	CString GetRightText();

	/** Returns the full text - including the "\001" TCHAR.
	                Does \b not translate the parameters.
	 */
	CString GetText()
	{
		return strText;
	};

	///Saves the module to the registry
	bool SerializeToRegistry(CBCGRegistryEx &reg) const;
	///Loads the settings of this module from the registry
	bool SerializeFromRegistry(CBCGRegistryEx &reg);

//Attributes
public:
	///Name of the TextModule
	CString Name;

	///Whether the parameters of this module are active or not.
	bool bEnableParams;

protected:
	/** The text to be inserted.
	                May contain placeholders for parameters and files and sets of files.
	                The String is splitted into two parts by a "\001" character.
	                This represents the position of the cursor.
	 */
	CString strText;
};

/** Sorted Collection of uniquely named TextModules.

Does not allow duplicates of TextModules.
Two TextModules are meant to be the same (duplicates),
if they have the same name.
The Text to be inserted is not considered for this.

@todo Implement a manager of groups.

@todo Export to and Import from XML.

@see CTextModule

@author Tino Weinkauf
 */
class CTextModuleGroup : public CSortArray<CTextModule,CTextModule&>
{
//Constructor / Destructor
public:
	///Constructor
	CTextModuleGroup();

	///Destructor
	virtual ~CTextModuleGroup();

//Methods
public:
	///Overridden to prevent duplicates. bAllowDuplicate is ignored and always set to false.
	virtual int InsertSorted(CTextModule& newElement,bool bSortAscending = true,bool bAllowDuplicate = false);

	///Adds a number of default TextModules.
	void AddDefaultModules();

	///Serializes the array to the registry.
	bool SerializeToRegistry(LPCTSTR strStartSection) const;
	///Loads the array from the registry.
	bool SerializeFromRegistry(LPCTSTR strStartSection);

//Attributes
public:
	///Name of the Group
	CString Name;
};

/** @} */
//end of documentation group

#endif // !defined(AFX_TEXTMODULES_H__B7379B81_2D99_11D8_B1A8_00095B41B65A__INCLUDED_)
