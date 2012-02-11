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
#pragma once

#include <sstream>

#include "StructureParser.h"

/**
Objects of this class can be used by CStructureParser-objects, to
parse the contents of files. They get there text out of a file.

@see CStructureParser

@author Sven Wiegand
 */
class CTextSourceFile : public CTextSource
{
// construction/destruction
public:
	/**
	Creates a CTextSourceFile-object, that gets its text out
	of the specified file.

	@param lpszFile
	        Path of the file, the object should get its text from.

	@return
	        TRUE if successful, FALSE otherwise.
	 */
	BOOL Create(LPCTSTR lpszFile);

// operations
public:
	virtual BOOL GetNextLine(LPCTSTR &lpLine,int &nLength);
	virtual void Delete();

// attributes
protected:

	/** CString-object that stores the current line. */
	CString m_strCurrentLine;

private:
	std::wstringstream is_;
};
