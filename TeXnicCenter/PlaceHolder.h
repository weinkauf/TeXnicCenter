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

#if !defined(AFX_PLACEHOLDER_H__65CF1821_0593_11D5_A222_006097239934__INCLUDED_)
#define AFX_PLACEHOLDER_H__65CF1821_0593_11D5_A222_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/**
Expands the placeholders in the given string and returns the result.

@param lpszStringWithPlaceholders
	String that contains the placeholders to expand.
@param lpszMainPath
	Path of the project's main file.
@param lpszCurrentPath
	Path of the current file in the editor. Set to NULL to prevent the
	function from replacing	the current file specific placeholders.
@param lCurrentLine
	Line index of the cursor in the current file in the editor. Set to 
	-1 to prevent the function from replacing the line specific place
	holders
@param lpszCurrentSelection
	Selected text in the current document.

@author Sven Wiegand
*/
CString AfxExpandPlaceholders(LPCTSTR lpszStringWithPlaceholders, 
															LPCTSTR lpszMainPath = NULL, 
															LPCTSTR lpszCurrentPath = NULL, 
															long lCurrentLine = -1,
															LPCTSTR lpszCurrentSelection = NULL);


#endif // !defined(AFX_PLACEHOLDER_H__65CF1821_0593_11D5_A222_006097239934__INCLUDED_)
