/********************************************************************
*
* This file is part of the TeXnicCenter-system
*
* Copyright (C) 2002-$CurrentYear$ Chris Norris
* Copyright (C) 2002-$CurrentYear$ ToolsCenter
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

#ifndef __CHARACTER_H__
#define __CHARACTER_H__

inline BOOL IsAlpha(TCHAR tc)
{
	return IsCharAlpha(tc);
}


inline BOOL IsUpper(TCHAR tc)
{
	return IsCharUpper(tc);
}


inline BOOL IsLower(TCHAR tc)
{
	return IsCharLower(tc);
}


inline BOOL IsDigit(TCHAR tc)
{
	return _istdigit(tc);
}


inline BOOL IsSpace(TCHAR tc)
{
	return _istspace(tc);
}


inline BOOL IsPunct(TCHAR tc)
{
	return _istpunct(tc);
}


inline BOOL IsAlNum(TCHAR tc)
{
	return IsCharAlphaNumeric(tc);
}


inline BOOL IsPrint(TCHAR tc)
{
	return _istprint(tc);
}


inline BOOL IsGraph(TCHAR tc)
{
	return _istgraph(tc);
}


inline BOOL IsCntrl(TCHAR tc)
{
	return _istcntrl(tc);
}


#endif __CHARACTER_H__