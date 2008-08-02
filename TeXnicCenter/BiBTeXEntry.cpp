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
#include "BiBTeXFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// NOTE: The string must have an case-insensitive match with the corresponding keyword in bibtex!
const TCHAR* const BibTypeVerbose[] = {
                                 _T("Book"),_T("Article"),_T("Booklet"),_T("Manual"),_T("Inproceedings"),
                                 _T("Conference"),_T("Inbook"),_T("Incollection"),_T("Mastersthesis"),
                                 _T("Misc"),_T("PhDthesis"),_T("Proceedings"),_T("TechReport"),
                                 _T("Unpublished"),_T("String"),_T("Comment"),_T("Preamble"),_T("Parse Error"),_T("Unknown")
}; // must be kept in sync with bibtype!


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBiBTeXEntry::CBiBTeXEntry(const CString& key, CBiBTeXFile *parent, BibType type)
{
    ASSERT(parent != NULL);
    m_Parent = parent;
    m_Key = key;

    if (m_Key.GetLength() == 0) {
        m_Key = _T("-");
    }

    m_Type = type;
}

BOOL CBiBTeXEntry::GetField( const CString& name, CString &value ) const
{
    return m_Fields.Lookup(name,value);
}

BOOL CBiBTeXEntry::SetField(CString name,CString value,BOOL forceOverwrite)
{
    CString dummy;

    if (!forceOverwrite && m_Fields.Lookup(name,dummy)) {
        return FALSE;
    }

    m_Fields.SetAt(name,value);
    return TRUE;
}

const CString CBiBTeXEntry::ToString() const
{
    return CString(_T("[") + m_Key + _T("] ") + ToCaption());
}

const CString CBiBTeXEntry::ToCaption() const
{
    CString author,title,year;
    GetField(_T("author"),author);

    // Get editor instead
    if (author.IsEmpty()) {
        GetField(_T("editor"),author);
    }

    // Last chance: publisher
    if (author.IsEmpty()) {
        GetField(_T("publisher"),author);
    }

    GetField(_T("title"),title);
    GetField(_T("year"),year);

    // Remove trash (slashes, whitespace,...) from string
    BeautifyField(author,true);
    BeautifyField(title,false);
    BeautifyField(year,false);

    // Try to expand abbreviations
    CString abbrev = m_Parent->GetString(author);

    if (!abbrev.IsEmpty()) {
        //TRACE("Expand %s to %s...\n", author, abbrev.Mid(0,5));
        author = abbrev;
        BeautifyField(author,true);
    }

    abbrev = m_Parent->GetString(title);
    if (!abbrev.IsEmpty()) {
        //TRACE("Expand %s to %s...\n", title, abbrev.Mid(0,5));
        title = abbrev;
        BeautifyField(title,false);
    }


    return CString(m_Key + _T(": ") + author + _T(": ") +
            title + _T(" (") +
            BibTypeVerbose[m_Type] + _T(", ") +
            year + _T(")")
            );
}

void CBiBTeXEntry::BeautifyField( CString &value, bool bReplaceAnds )
{

    if (bReplaceAnds) value.Replace(_T(" and"),_T(","));

    //value.Replace(_T("\""), _T(""));
    value.Replace(_T("~"),_T(" "));
    value.Replace(_T("--"),_T("-"));
    value.Replace(_T("{"),_T(""));
    value.Replace(_T("}"),_T(""));
    value.Replace(_T("\n"),_T(""));
    value.Replace(_T("\r"),_T(""));
    value.Replace(_T("\t"),_T(""));
    value.Replace(_T("\\"),_T(""));

    while (value.Find(_T("  ")) != -1) {
        value.Replace(_T("  "),_T(" "));
    }

    value.TrimLeft();
    value.TrimRight();
}