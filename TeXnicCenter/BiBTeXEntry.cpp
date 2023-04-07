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

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BibTeXEntry::BibTeXEntry(const CString& key, BibTeXFile *parent, BibTeXEntryType type)
{
	ASSERT(parent != NULL);
	m_Parent = parent;
	m_Key = key;
	m_Key.Trim(); // Remove excessive whitespace

	if (m_Key.GetLength() == 0)
	{
		m_Key = _T("-");
	}

	m_Type = type;
}

BOOL BibTeXEntry::GetField(const CString& name, CString &value) const
{
	return m_Fields.Lookup(name,value);
}

BOOL BibTeXEntry::SetField( const CString& name, const CString& value, BOOL forceOverwrite /*= FALSE*/ )
{
	CString dummy;

	if (!forceOverwrite && m_Fields.Lookup(name,dummy))
	{
		return FALSE;
	}

	m_Fields.SetAt(name,value);
	return TRUE;
}

const CString BibTeXEntry::ToString() const
{
	return CString(_T("[") + m_Key + _T("] ") + ToCaption());
}

const BibItem BibTeXEntry::ToBibItem() const
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
		int iyear = _ttoi(pos);

		if (iyear != 0) {
			result.year_ = iyear;
		}
	}

	result.publisher_ = publisher;
	result.title_ = title;
	result.label_ = m_Key;
	result.type_text_ = GetString(m_Type);
	result.type_ = m_Type;

	return result;
}

const CString BibTeXEntry::ToCaption() const
{
	return ToCaption(ToBibItem());
}

const CString BibTeXEntry::ToCaption(const BibItem& item) const
{
	CString year;
	
	if (item.HasYear())
		year.Format(_T("%i"), item.GetYear().GetValue());

	return item.GetLabel() + _T(": ") + item.GetAuthor() + _T(": ") +
	       item.GetTitle() + _T(" (") + item.GetTypeString() + _T(", ") + year + _T(")");
}

void BibTeXEntry::ToCaptionTitleKey(CString& resCaption, CString& resTitle, CString& resKey) const
{
	const BibItem item = ToBibItem();

	resCaption = ToCaption(item);
	resTitle = item.GetTitle();
	resKey = GetKey();
}


void BibTeXEntry::BeautifyField( CString &value )
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

void BibTeXEntry::ExtractAuthors(const CString &value, BibItem::AuthorContainerType &authors )
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

void BibTeXEntry::ReplaceAnds( CString &value )
{
	value.Replace(_T(" and"),_T(","));
}

const CString& BibTeXEntry::GetKey() const
{
	return m_Key;
}

void BibTeXEntry::SetBibliographyType( BibTeXEntryType type )
{
	m_Type = type;
}

BibTeXEntryType BibTeXEntry::GetType() const
{
	return m_Type;
}
