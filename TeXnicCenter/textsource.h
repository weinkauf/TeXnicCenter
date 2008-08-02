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

#ifndef __TEXTSOURCE_H__
#define __TEXTSOURCE_H__

/**
Objects of this class are used by other classes to 
get the text of a file for parsing or searching, etc.

CTextSource is an abstract base class. You have to derive
your own class.

@see CStructureParser
@see CFileGrep
@see CTextSourceManager

@author Sven Wiegand
 */
class CTextSource {
    // operations
public:
    /**
    Returns the next line of text of this object.

    @param lpLine
            Pointer to a variable, that will get the pointer to the line text.
            The text pointed to is not guaranteed to be terminated by a Null-character.
    @param nLength
            The length of the line text.

    @return
            FALSE, if there are no more lines, TRUE otherwise.
     */
    virtual BOOL GetNextLine(LPCTSTR &lpLine,int &nLength) = 0;

    /**
    Deletes this text source (use this instead of delete-operator).
     */
    virtual void Delete() = 0;
};

/**
An object of this class is used by other classes to
get an CTextSource object for a specified path-name.

CTextSourceManager is an abstract base class. You have to derive
your own manager class.

@see CTextSource
@see CFileGrep
@see CStructureParser

@author Sven Wiegand
 */
class CTextSourceManager {
    // operations
public:
    /**
    Returns a pointer to a CTextSource-object that contains
    the text of the specified file or NULL, if the text source
    has not been created.

    <b>Remark:</b> The caller is responsible for deleting the
    CTextSource-object by calling its Delete()-method.

    @see CTextSource::Delete

    @param lpszPath
            Path of the file to generate a CTextSource-object for.
     */
    virtual CTextSource *GetTextSource(LPCTSTR lpszPath) = 0;
};

#endif //__TEXTSOURCE_H__