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
#include "BiBTeXFile.h"
#include "BiBTeXEntry.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern const TCHAR* BibTypeVerbose[];
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBiBTeXFile::CBiBTeXFile(CString file)
:CObject()
{
	m_Filename = file;
}

CBiBTeXFile::~CBiBTeXFile()
{
	DropAllEntries();
}


BOOL CBiBTeXFile::ProcessFile()
{
	CFile f;
	BOOL ok;

	try {
		f.Open(m_Filename, CFile::modeRead);
		DWORD l = f.GetLength();

		TCHAR *buf = new TCHAR[l];

		f.Read(buf, l);

		ok = ParseFile(buf);

		delete [] buf;
	} catch(CFileException &ex) {
		TRACE("Error opening style file: %s\n",ex);
		f.Close();				
		return FALSE;
	} 
	
	f.Close();
	return ok;
}

BOOL CBiBTeXFile::ParseFile(const TCHAR *buf)
{
	const TCHAR *begin, *lastChar = NULL;
	CBiBTeXEntry::BibType type;
	int depth = 0, line = 1, col = 1;
	BOOL inComment = FALSE, inQuote = FALSE;

	begin = buf;	
	type = CBiBTeXEntry::Unknown;

	while (*buf) { // Parser state machine
		switch(*buf) {
		case _T('%'): // Toggle comment
			inComment = TRUE;
			break;
		case _T('\"'): // Toggle quote
			if (inComment) break;
			if (lastChar && lastChar[0] == _T('\\')) break;
			inQuote = !inQuote;
			break;
		case _T('@'): // Start type
			if (inComment || inQuote) break;
			if (0 == depth) { // update pointer if on top level
				begin = buf;			
			}
			break;
		case _T('{'):
			if (inComment) break;
			if (0 == depth) { // process entry type, e. g. @Article
				type = ProcessEntryType(begin, buf-begin, line);
				begin = buf;
			}
			++depth;			
			break;
		case _T(','):
			if (inComment || inQuote) break;
			if (1 == depth) { // process entry field
				ProcessArgument(begin, buf-begin, type, line);
				begin = buf;
			}			
			break;
		case _T('}'):
			if (inComment) break;
			if (1 == depth) { // process entry field and decrease stack depth
				ProcessArgument(begin, buf-begin, type, line);
			} 
			--depth;
			if (depth == 0) {
				FinalizeItem();
			}
			if (depth < 0) {
				HandleParseError(buf, line, col);
				return FALSE;
			}
			break;
		case _T('\n'): // update line number			
			inComment = FALSE;
			col = 0;
			++line;
			break;
		}
		col++;
		lastChar = buf;
		buf++;
	}

	TRACE("Found %d entries\n", m_Entries.GetCount());
	return TRUE;
}

CBiBTeXEntry::BibType CBiBTeXFile::ProcessEntryType(const TCHAR *buf, int len, int line)
{
	TCHAR tmp[MAX_BIBTEX_ARG_LENGTH];
	
	ASSERT(len > 0 && len < MAX_BIBTEX_ARG_LENGTH);
	strncpy(tmp, buf + 1, len - 1);
	tmp[len - 1] = 0;
	//TRACE("Entry: %s\n", tmp);

	for (int i = 0; i < CBiBTeXEntry::Unknown; i++) {
		if (0 == stricmp(tmp, BibTypeVerbose[i])) {
			return (CBiBTeXEntry::BibType)i;
		}
	}

	return CBiBTeXEntry::Unknown;
}

void CBiBTeXFile::ProcessArgument(const TCHAR *buf, int len, CBiBTeXEntry::BibType type, int line)
{
	TCHAR tmp[MAX_BIBTEX_ARG_LENGTH];
	CBiBTeXEntry *be, *dummy;

	ASSERT(len > 0 && len < MAX_BIBTEX_ARG_LENGTH);
	strncpy(tmp, buf + 1, len - 1);
	tmp[len - 1] = 0;


	if (type == CBiBTeXEntry::String || type == CBiBTeXEntry::Comment) {
		//TRACE("** Skip item at line %d: %s\n", line, tmp);
		return;
	}

	if (type == CBiBTeXEntry::Unknown) {
		TRACE("** Ignore unknown entry at line %d: %s\n", line, tmp);
		HandleParseError(buf, line, 1);
		return;
	}
	

	if (NULL == strstr(tmp, _T("="))) { // argument is key?		
		CString key = tmp;
		key.TrimLeft();
		key.TrimRight();

		if (key.IsEmpty()) {
			return;
		}
		if (!m_Entries.Lookup(key, (CObject*&)dummy)) {
			be = new CBiBTeXEntry(tmp, type);
			/* add entries used by structure parser */
			be->m_nLine = line;			
			
			m_Entries.SetAt(key, be);
			m_LastKey = key;
			m_Keys.Add(key);
		} else {
			TRACE("WARNING: Invalid or duplicate key <%s> (%s)\n", key, BibTypeVerbose[type]);
		}
	} else {
		
		//be = (CBiBTeXEntry *)m_Entries.Top();
		if (m_Entries.Lookup(m_LastKey, (CObject*&)be)) {
			CString name, val;
			ParseField(tmp, name, val);

			//TRACE("Set Value: <%s> = <%s>\n", name, val);
			name.MakeLower();
			name.TrimLeft();
			name.TrimRight();
			val.TrimLeft();
			val.TrimRight();
			be->SetField(name, val);
		} else {
			ASSERT(FALSE);
		}
	}
}

void CBiBTeXFile::HandleParseError(const TCHAR *buf, int line, int col)
{
	TCHAR tmp[21];
	strncpy(tmp, buf + 1, 20);
	tmp[20] = 0;
	TRACE("Parse error at (%d, %d): <%c>%s...\n(Line %d)\n", line, col, buf[0], tmp);
}

BOOL CBiBTeXFile::ParseField(const TCHAR *field, CString &name, CString &val)
{
	TCHAR* eqChar;
	TCHAR tmp[MAX_BIBTEX_ARG_LENGTH];
	eqChar = strstr(field, _T("="));
	int len1, len2, n = strlen(field);

	if (NULL == eqChar) {
		return FALSE;
	}
	
	// Extract name and value
	len1 = eqChar - field - 1;
	len2 = n - (eqChar - field) - 1;
	ASSERT(len1 < MAX_BIBTEX_ARG_LENGTH);
	ASSERT(len2 < MAX_BIBTEX_ARG_LENGTH);

	strncpy(tmp, field, len1);
	tmp[len1] = 0;

	name = tmp;

	strncpy(tmp, eqChar + 1, len2);
	tmp[len2] = 0;

	val = tmp;
	/// remove whitespace
	name.TrimRight();
	name.TrimLeft();
	val.TrimLeft();
	val.TrimRight();

	return TRUE;
}

void CBiBTeXFile::DropAllEntries()
{
	POSITION pos = m_Entries.GetStartPosition();
	while(pos != NULL) {
		CBiBTeXEntry *be;
		CString key;
		m_Entries.GetNextAssoc(pos, key, (CObject*&)be);
		if (be != NULL) {
			//TRACE("Deleting entry %s\n", be->ToString());
			delete be;
		} else {
			TRACE("NP detected in CBiBTeXFile %s", m_Filename);
		}
		m_Entries.RemoveKey(key);
	}

	m_Keys.RemoveAll();
}

void CBiBTeXFile::FinalizeItem()
{
	CBiBTeXEntry *be;
	if (m_Entries.Lookup(m_LastKey, (CObject*&)be)) {
		be->m_nType = CStructureParser::bibItem;
		be->m_strTitle = be->ToCaption();
		be->m_strCaption = be->ToCaption();
		be->m_strLabel = be->GetKey();
		be->m_strComment = BibTypeVerbose[be->GetType()];
		be->m_strPath = m_Filename;
		//TRACE("Finalized %s\n", be->ToString());
	} else {
		//ASSERT(FALSE);
		TRACE("Warning: Did not found key %s\n", m_LastKey);
	}
}
