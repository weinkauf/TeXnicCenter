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
const TCHAR* const BibTypeVerbose[] =
{
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

	if (m_Key.GetLength() == 0)
	{
		m_Key = _T("-");
	}

	m_Type = type;
}

BOOL CBiBTeXEntry::GetField(const CString& name, CString &value) const
{
	return m_Fields.Lookup(name,value);
}

BOOL CBiBTeXEntry::SetField( const CString& name, const CString& value, BOOL forceOverwrite /*= FALSE*/ )
{
	CString dummy;

	if (!forceOverwrite && m_Fields.Lookup(name,dummy))
	{
		return FALSE;
	}

	m_Fields.SetAt(name,value);
	return TRUE;
}

const CString CBiBTeXEntry::ToString() const
{
	return CString(_T("[") + m_Key + _T("] ") + ToCaption());
}

const BibItem CBiBTeXEntry::ToBibItem() const
{
	CString author, title, year, editor, publisher;
	BibItem result;
	
	GetField(_T("author"),author);

	GetField(_T("editor"),editor);

	// Get editor instead
	if (author.IsEmpty())
	{		
		author = editor;
	}

	GetField(_T("publisher"),publisher);

	// Last chance: publisher
	if (author.IsEmpty())
	{		
		author = publisher;
	}

	GetField(_T("title"),title);
	GetField(_T("year"),year);

	// Remove trash (slashes, whitespace,...) from string
	BeautifyField(author);
	ExtractAuthors(author,result.authors_);
	ReplaceAnds(author);

	BeautifyField(title);
	BeautifyField(year);

	// Try to expand abbreviations
	CString abbrev = m_Parent->GetString(author);

	if (!abbrev.IsEmpty())
	{
		//TRACE("Expand %s to %s...\n", author, abbrev.Mid(0,5));
		author = abbrev;
		BeautifyField(author);
		ExtractAuthors(author,result.authors_);
		ReplaceAnds(author);
	}

	abbrev = m_Parent->GetString(title);

	if (!abbrev.IsEmpty())
	{
		//TRACE("Expand %s to %s...\n", title, abbrev.Mid(0,5));
		title = abbrev;
		BeautifyField(title);
	}

	result.author_ = author;

	LPCTSTR first = year;
	LPCTSTR last =  first + year.GetLength();

	// Search for the first digit, in case the year field contains e.g. a month name
	LPCTSTR pos = std::find_if(first,last,_istdigit);

	if (pos != last) {
		result.year_ = _ttoi(pos);
		result.has_year_ = true;
	}

	result.publisher_ = publisher;
	result.title_ = title;
	result.label_ = m_Key;
	result.type_text_ = BibTypeVerbose[m_Type];

	if (m_Type >= Booklet && m_Type <= Unpublished)
		result.type_ = static_cast<BibItem::Type>(m_Type); // Remap
	else {
		// Following BibType values are incompatible with BibItem::Type
		switch (m_Type)
		{
			case Article:
				result.type_ = BibItem::Article;
				break;
			case Book:
				result.type_ = BibItem::Book;
				break;
			case String:
				result.type_ = BibItem::String;
				break;
			case Comment:
				result.type_ = BibItem::Comment;
				break;
			case Preamble:
				result.type_ = BibItem::Preamble;
				break;
			// otherwise unknown
		}
	}

	return result;
}

const CString CBiBTeXEntry::ToCaption() const
{
	const BibItem item = ToBibItem();
	CString year;
	year.Format(_T("%i"),item.year_);

	return CString(item.GetLabel() + _T(": ") + item.GetAuthor() + _T(": ") +
	               item.GetTitle() + _T(" (") + item.GetTypeString() + _T(", ") + year + _T(")"));
}

void CBiBTeXEntry::BeautifyField( CString &value )
{
	//value.Replace(_T("\""), _T(""));
	value.Replace(_T("~"),_T(" "));
	value.Replace(_T("--"),_T("-"));
	value.Replace(_T("{"),_T(""));
	value.Replace(_T("}"),_T(""));
	value.Replace(_T("\n"),_T(""));
	value.Replace(_T("\r"),_T(""));
	value.Replace(_T("\t"),_T(""));
	value.Replace(_T("\\"),_T(""));

	while (value.Find(_T("  ")) != -1)
	{
		value.Replace(_T("  "),_T(" "));
	}

	value.TrimLeft();
	value.TrimRight();
}

void CBiBTeXEntry::ExtractAuthors(const CString &value, BibItem::AuthorContainerType &authors )
{
	int index1 = 0, index2;
	bool stop = false;

	const CString delimiter(_T(" and "));

	while (!stop) {
		while (index1 < value.GetLength() && value[index1] == _T(' '))
			++index1;

		index2 = value.Find(delimiter,index1);

		if (index1 < value.GetLength()) {
			if (index2 == -1) {
				index2 = value.GetLength();

				while (--index2 > index1 && value[index2] == _T(' '))
					;

				stop = true;
			}

			authors.push_back(value.Mid(index1,index2 - index1));
			index1 = index2 + delimiter.GetLength();
		}
		else
			stop = true;
	}
}

void CBiBTeXEntry::ReplaceAnds( CString &value )
{
	value.Replace(_T(" and"),_T(","));
}