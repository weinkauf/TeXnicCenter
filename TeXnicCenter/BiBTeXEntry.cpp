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

#include "stdafx.h"
#include "BiBTeXEntry.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// NOTE: The string must have an case-insentive match with the corresponding keyword in bibtex!
const TCHAR* BibTypeVerbose[] = {
"Book", "Article", "Booklet", "Manual", "Inproceedings", 
"Conference", "Inbook", "Incollection", "Mastersthesis",
"Misc", "PhDthesis", "Proceedings", "TechReport",
"Unpublished", "String", "Comment", "Preamble", "Unknown"
}; // must be kept in sync with bibtype!


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBiBTeXEntry::CBiBTeXEntry(CString key, BibType type)
{
	m_Key = key;
	m_Type = type;
}

CBiBTeXEntry::~CBiBTeXEntry()
{
	/*
	POSITION pos = m_Fields.GetStartPosition();
	while(pos != NULL) {
		CString sf;
		CString key;
		m_Fields.GetNextAssoc(pos, key, sf);
		if (sf != NULL) {
			//TRACE("Deleting cmd %s\n", sf->ToString());
			delete sf;
		} else {
			TRACE("NP detected in CBiBTeXEntry %s", m_Key);
		}
		m_Commands.RemoveKey(key);
	}*/
}

BOOL CBiBTeXEntry::GetField(CString name, CString &value)
{
	return m_Fields.Lookup(name, value);
}

BOOL CBiBTeXEntry::SetField(CString name, CString value, BOOL forceOverwrite)
{
	CString dummy;
	if (!forceOverwrite && m_Fields.Lookup(name, dummy)) {
		return FALSE;
	}
	m_Fields.SetAt(name, value);
	return TRUE;
}

CString CBiBTeXEntry::ToString()
{
	return CString(_T("[") + m_Key + _T("] ") + ToCaption());
}

CString CBiBTeXEntry::ToCaption()
{
	CString author, title, year;
	GetField(_T("author"), author);

	if (author.IsEmpty()) {
		GetField(_T("editor"), author);
	}
	GetField(_T("title"), title);
	GetField(_T("year"), year);

	// beautify output (looks cruel, I know)
	author.Replace(_T("{"), _T(""));
	author.Replace(_T("}"), _T(""));
	author.Replace(_T("\r"), _T(""));
	author.Replace(_T("\n"), _T(""));
	author.Replace(_T("\t"), _T(""));	
	author.Replace(_T("\""), _T(""));
	author.Replace(_T(" and"), _T(","));
	while (author.Find(_T("  ")) != -1) {
		author.Replace(_T("  "), _T(" "));
	}
	
	title.Replace(_T("\""), _T(""));
	title.Replace(_T("{"), _T(""));
	title.Replace(_T("}"), _T(""));
	title.Replace(_T("\n"), _T(""));
	title.Replace(_T("\r"), _T(""));
	title.Replace(_T("\t"), _T(""));
	title.Replace(_T("\\"), _T(""));
	while (title.Find(_T("  ")) != -1) {
		title.Replace(_T("  "), _T(" "));
	}
	year.Replace(_T("{"), _T(""));
	year.Replace(_T("}"), _T(""));
	year.Replace(_T("\""), _T(""));

	author.TrimLeft();


	return CString(author + _T(": ") + 
		title + _T(" (") + 
		BibTypeVerbose[m_Type] + _T(", ") +
		year + _T(")")
		);
}
