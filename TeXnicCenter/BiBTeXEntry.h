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

#pragma once

#include "BibItem.h"
#include "BibTeXEntryType.h"
#include "StructureParser.h"

class BibTeXEntry;
class BibTeXFile;

class BibTeXEntry
	: public CObject
	, public StructureItem
{
public:
	/** Constructs a new BibTeX entry
	        @param key Key of the BibTeX entry
	        @param parent Pointer to the BibTeX file containing the entry
	        @param type Entry type (default: @book)

	@see BibTeXEntryType
	 */
	BibTeXEntry(const CString& key, BibTeXFile *parent, BibTeXEntryType type);
	/** Set an entry fields. To set the author of an entry, just call
	        <code>CString author;
	        SetField("author", author);
	        </code>
	        @param forceOverwrite If true, the field value will be overridden in every case. Otherwise
	        SetField returns false, if the entry already exists. Default is false.
	        @return True, if entry was changed successfully.
	 */
	BOOL SetField(const CString& name, const CString& value, BOOL forceOverwrite = FALSE);
	/** Access to entry fields. To get the author of an entry, just call
	        <code>CString author;
	        SetField("author", "Asterix");
	        </code>
	        @return True, if entry was changed
	 */
	BOOL GetField(const CString& name, CString &value) const;

	/* Sets/gets the type of the BibTeX entry */
	BibTeXEntryType GetType() const;

	void SetBibliographyType(BibTeXEntryType type);

	/* Returns the key of the BibTeX entry */
	const CString& GetKey() const;

	/* String representations of CBibTeXEntry */
	const CString ToString() const;
	const CString ToCaption() const;
	void ToCaptionTitleKey(CString& resCaption, CString& resTitle, CString& resKey) const;
	const BibItem ToBibItem() const;

protected:
	const CString ToCaption(const BibItem& item) const;


private:
	/* Removes ugly chars (e. g. whitespace, new lines,...) from a field
	        @param value String to beautify
	        @param bReplaceAnds If true, 'and' (common in author fields) are replaced with a comma.
	 */
	static void BeautifyField(CString &value);
	static void ReplaceAnds( CString &value );
	static void ExtractAuthors(const CString &value, BibItem::AuthorContainerType &authors );
	CMapStringToString m_Fields;
	BibTeXEntryType m_Type;
	CString m_Key;
	BibTeXFile* m_Parent;
};
