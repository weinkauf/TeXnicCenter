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
#include "global.h"
#include "resource.h"
#include "BiBTeXFile.h"
#include "BiBTeXEntry.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern const TCHAR* BibTypeVerbose[];

CBiBTeXFile::CBiBTeXFile(CString file)
:CObject()
{
	m_Filename = file;
	m_ErrorCount = 0;
	m_IsATSignInBracesAllowed = TRUE;
	m_WarnWrongLevelAT = TRUE;
	m_BufferSize = MAX_BIBTEX_ARG_LENGTH;
	m_Buffer = new TCHAR[m_BufferSize];
}

CBiBTeXFile::~CBiBTeXFile()
{
	DropAllEntries();
	delete [] m_Buffer;
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
		m_ErrorCount = 0;
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
	BOOL inComment = FALSE, inQuote = FALSE, insideEntry = FALSE, openBrace = FALSE;

	begin = buf;	
	type = CBiBTeXEntry::Unknown;

	while (*buf) { // Parser state machine
		switch(*buf) {
		case _T('%'): // Toggle comment. Please note, that this is no official comment sign!
			if (depth != 0) break;
			inComment = TRUE;
			break;
		case _T('\"'): // Toggle quote
			if (inComment || !insideEntry) break;
			/* Quotes must be included in arbitry braces, if the fields are delimited by quotes */
			if (depth ==1 && inQuote && (lastChar && lastChar[0] == _T('\\'))) {
				HandleParseError(STE_BIBTEX_ERR_QOUTEWITHINQUOTE, line, col);
			}			
			/* Quotes are taken "as is", if included in arbitrary braces */
			if (depth >= 2 || (lastChar && lastChar[0] == _T('\\'))) break;
			
			inQuote = !inQuote;
			break;
		case _T('@'): // Start type
			/* The @ sign has a very strong meaning in BibTeX. It is indicating the beginning of an
			   entry, no matter which text precedes. So the entry "steward03" is considered by BibTeX!

				@Comment{
				  @Book{steward03, [...]
				  }
				}
			   You can have a @ inside a quoted values but not inside a braced value. 
			 */
			if (inComment || inQuote) break;			
			/* More relaxed handling of @? */
			if (m_IsATSignInBracesAllowed && depth>= 2) break;
			if (/*0 != depth*/ insideEntry && m_WarnWrongLevelAT) { 
				HandleParseError(STE_BIBTEX_ERR_WRONGLEVELAT, line, col);
			}
			depth = 0;
			insideEntry = TRUE;
			begin = buf; // update pointer if on top level
			break;
		case _T('('): // alternative beginning of an entry
			if (inComment || !insideEntry) break;
			if (0 == depth) { // process entry type, e. g. @Article
				type = ProcessEntryType(begin, buf-begin, line);
				begin = buf;
				++depth; // only on level 0!
			}			
			break;
		case _T('{'): // beginning of an entry, field or special chars within a field
			if (inComment || !insideEntry) break;
			openBrace = TRUE;
			if (0 == depth) { // process entry type, e. g. @Article
				type = ProcessEntryType(begin, buf-begin, line);
				begin = buf;
			} 
			++depth;			
			break;
		case _T(','): // field delimiter
			if (inComment || inQuote || !insideEntry) break;
			if (1 == depth) { // process entry field
				ProcessArgument(begin, buf-begin, type, line);
				begin = buf;
			}			
			break;
		case _T(')'): // alternative end of an entry
			if (inComment || inQuote || !insideEntry) break;
			if (1 == depth) { // process entry field and decrease stack depth
				ProcessArgument(begin, buf-begin, type, line);

				--depth;
				if (depth == 0) {
					insideEntry = FALSE;
					FinalizeItem();
				}
			} 			
			break;
		case _T('}'): // ending of an entry, field or special chars within a field
			if (inComment || !insideEntry) break;
			openBrace = FALSE;
			if (1 == depth) { // process entry field and decrease stack depth
				ProcessArgument(begin, buf-begin, type, line);
			} 
			--depth;
			if (depth == 0) {
				insideEntry = FALSE;
				FinalizeItem();
			}
			break;		
		} // end switch

		if (*buf == _T('\n')) { // update line number?
			inComment = FALSE; // quit comment
			col = 0;
			++line;
		}
		col++;
		lastChar = buf;
		buf++;
	}

	// Check if parser finished correctly
	if (depth > 0) {
		HandleParseError(STE_BIBTEX_ERR_INVALID_EOF, line, col);
	}

	TRACE("\n%s: Found %d entries\n", m_Filename, m_Entries.GetCount());
	return TRUE;
}

CBiBTeXEntry::BibType CBiBTeXFile::ProcessEntryType(const TCHAR *buf, int len, int line)
{
	if (!SaveCopyBuffer(buf + 1, len - 1)) {
		HandleParseError(STE_BIBTEX_ERR_SUSPICOUS_LINE, line, 1, m_Buffer);
	}

	for (int i = 0; i < CBiBTeXEntry::Unknown; i++) {
		if (0 == stricmp(m_Buffer, BibTypeVerbose[i])) {
			return (CBiBTeXEntry::BibType)i;
		}
	}
	HandleParseError(STE_BIBTEX_ERR_INVALID_TYPE, line, 1, m_Buffer);
	return CBiBTeXEntry::Unknown;
}

void CBiBTeXFile::ProcessArgument(const TCHAR *buf, int len, CBiBTeXEntry::BibType type, int line)
{
	CBiBTeXEntry *be, *dummy;
	
	
	if (!SaveCopyBuffer(buf + 1, len - 1)) {
		HandleParseError(STE_BIBTEX_ERR_SUSPICOUS_LINE, line, 1, m_Buffer);
	}	

	/* Skip comments and preamble */
	if (type == CBiBTeXEntry::Preamble || type == CBiBTeXEntry::Comment) {
		return;
	}

	if (type == CBiBTeXEntry::Unknown) 
	{
		if (strlen(m_Buffer) > 100) 
		{
			m_Buffer[100] = 0;
		}
		TRACE("** Ignore unknown entry at line %d: %s\n", line, m_Buffer);
		return;
	}

	//TRACE("Processing argument: %s, type %d at line %d\n", m_Buffer, type, line);

	// strings have no explicit key, instead we are using the first field name as key
	if (type == CBiBTeXEntry::String && NULL != strstr(buf, _T("="))) 
	{ 
		CString name, val;
		ParseField(m_Buffer, name, val);

		name.TrimLeft();
		name.TrimRight();
		val.TrimLeft();
		val.TrimRight();

		m_Strings.SetAt(name, val);
		return;
	}

	if (NULL == strstr(m_Buffer, _T("="))) 
	{ // argument is key?		
		CString key = m_Buffer;
		
		key.TrimLeft();
		key.TrimRight();

		if (key.IsEmpty()) 
		{ // invalid key?
			return;
		}

		if (!m_Entries.Lookup(key, (CObject*&)dummy)) { // key already exists?
			be = new CBiBTeXEntry(m_Buffer, this, type);
			/* add entries used by structure parser */
			be->m_nLine = line;			
			//TRACE("Adding key %s\n", key);
			m_Entries.SetAt(key, be);
			m_LastKey = key;
			m_Keys.Add(key);
		} else {			
			HandleParseError(STE_BIBTEX_ERR_DUP_KEY, line, 1, key);
			TRACE("WARNING: Invalid or duplicate key <%s> (%s)\n", key, BibTypeVerbose[type]);
		}
	} else { // extract name-value pair and add it to the entry
		if (m_Entries.Lookup(m_LastKey, (CObject*&)be)) {
			CString name, val;
			if (!ParseField(m_Buffer, name, val)) {
				HandleParseError(STE_BIBTEX_ERR_SUSPICOUS_LINE, line, 1);
				return;
			}

			//TRACE("Set Value: <%s> = <%s>\n", name, val);
			name.MakeLower();
			name.TrimLeft();
			name.TrimRight();
			ReplaceSpecialChars(val);
			val.TrimLeft();
			val.TrimRight();
			be->SetField(name, val);
		} else { // error: key not found -> likely an error in the bibtex file			
			HandleParseError(STE_BIBTEX_ERR_MISSING_KEY, line, 1);
		}
	}
}

void CBiBTeXFile::HandleParseError(UINT msgID, int line, int col, const TCHAR *addDesc)
{
	CString s, key;	

	m_ErrorCount++;
	key.Format("Parse_Error%d", m_ErrorCount);

	CString errMsgFmt = AfxLoadString(msgID);

	switch (msgID) {
	case STE_BIBTEX_ERR_MISSING_KEY:	
	case STE_BIBTEX_ERR_INVALID_EOF:
	case STE_BIBTEX_ERR_QOUTEWITHINQUOTE:
	case STE_BIBTEX_ERR_WRONGLEVELAT:
	case STE_BIBTEX_ERR_SUSPICOUS_LINE:
		s.Format(errMsgFmt, m_Filename, line, col);
		break;
	case STE_BIBTEX_ERR_INVALID_TYPE:
	case STE_BIBTEX_ERR_DUP_KEY:
		s.Format(errMsgFmt, m_Filename, line, col, addDesc);
		break;
	default:
		TRACE("BibTeX: Warning: No handler for msgID %d\n", msgID);
	}

	TRACE(s + "\n");
	
	
	CBiBTeXEntry *be = new CBiBTeXEntry(s, this, CBiBTeXEntry::Error);			
	be->m_nLine = line;						
	be->m_nType = CStructureParser::bibItem;
	be->m_strTitle = s;
	be->m_strComment = _T("");
	be->m_strCaption = s;
	be->m_strLabel = key;
	be->m_strPath = m_Filename;
	m_ErrorMsgs.Add(be);
	//m_Entries.SetAt(key, be);	
	
}

BOOL CBiBTeXFile::ParseField(const TCHAR *field, CString &name, CString &val)
{
	TCHAR* eqChar;

	eqChar = strstr(field, _T("="));
	int len1, len2, n = strlen(field);

	if (NULL == eqChar) {
		return FALSE;
	}
	
	// Extract name and value
	len1 = eqChar - field;
	len2 = n - (eqChar - field) - 1;

	if (len1 > MAX_BIBTEX_ARG_LENGTH || len2 > MAX_BIBTEX_ARG_LENGTH) {		
		return FALSE;
	}

	name = field;
	val = field;

	name.Delete(len1, n-len1);
	val.Delete(0, len1 + 1);
/*
	strncpy(tmp, field, len1);
	tmp[len1] = 0;

	name = tmp;

	strncpy(tmp, eqChar + 1, len2);
	tmp[len2] = 0;

	val = tmp;
	*/
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
	m_Strings.RemoveAll();
	for (int i=0;i < m_ErrorMsgs.GetSize(); i++) {
		CBiBTeXEntry *be = dynamic_cast<CBiBTeXEntry*>(m_ErrorMsgs.GetAt(i));
		if (be != NULL) {
			delete be;
		}
	}
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
		// TRACE("Warning: Did not found key %s\n", m_LastKey);
	}
}

const CBiBTeXEntry* CBiBTeXFile::GetEntryByKey(CString key)
{
	CBiBTeXEntry *be;
	if (m_Entries.Lookup(m_LastKey, (CObject*&)be)) {
		return (const CBiBTeXEntry*)be;
	}
	return NULL;
}

CString CBiBTeXFile::GetString(CString abbrev)
{
	CString expanded;
	if (m_Strings.Lookup(abbrev, expanded)) {
		return CString(expanded);
	}
	return CString();
}

void CBiBTeXFile::ReplaceSpecialChars(CString &value)
{
	// Strip off surrounding quotes
	if (!value.IsEmpty() && value[0] == '\"' && value[value.GetLength()-1] == '\"') {
		value.Delete(0);
		value.Delete(value.GetLength()-1);
	}
	// Strip off surrounding braces
	if (!value.IsEmpty() && value[0] == '{' && value[value.GetLength()-1] == '}') {
		value.Delete(0);
		value.Delete(value.GetLength()-1);
	}

	// Replace umlauts (incomplete!)
	value.Replace(_T("\"o"), _T("ö"));
	value.Replace(_T("\"a"), _T("ä"));
	value.Replace(_T("\"u"), _T("ü"));
	value.Replace(_T("\"O"), _T("Ö"));
	value.Replace(_T("\"A"), _T("Ä"));
	value.Replace(_T("\"U"), _T("Ü"));
	value.Replace(_T("\"ss"), _T("ß"));
}

BOOL CBiBTeXFile::SaveCopyBuffer(const TCHAR *buffer, int reqSize)
{
	BOOL ret = TRUE;
	ASSERT(reqSize >= 0);

	if (reqSize == 0) return TRUE;

	/* NOTE: We cut the buffer here instead of reallocate it in order 
	   to avoid heap corruptions. Large buffers may caused by invalid braces
	   */
	if (reqSize > m_BufferSize) { // cut buffer, if needed
		reqSize = m_BufferSize - 1;
		ret = FALSE;
	}
	strncpy(m_Buffer, buffer, reqSize);
	m_Buffer[reqSize] = 0;

	return ret;
}
