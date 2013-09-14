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

#include "CodeDocument.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/**
 * Initialize object with a given BibTeX file
 */
BibTeXFile::BibTeXFile(const CString& fileName)
{
	m_Filename = fileName;
	m_ErrorCount = 0;
	m_IsATSignInBracesAllowed = TRUE;
	m_WarnWrongLevelAT = TRUE;
	m_BufferSize = MAX_BIBTEX_ARG_LENGTH;
}

/**
 * Clean up
 */
BibTeXFile::~BibTeXFile()
{
	DropAllEntries();
}

/**
 * Opens the given BibTeX file, store the contents in a buffer and passes it to the parser.
 */
BOOL BibTeXFile::ProcessFile()
{
	CStringW text;
	TextDocument doc;

	bool result = doc.Read(m_Filename,text);

	if (result) {
		const CString s(text); text.Empty();
		result = ParseFile(s) != 0;
	}
	else {
		TRACE0("Error opening BibTeX file\n");
	}

	return result;
}

/**
 * Parses the BibTex file.
 */
BOOL BibTeXFile::ParseFile(const TCHAR *buf)
{
	const TCHAR *begin,*lastChar = NULL;
	BibTeXEntryType type;
	int depth = 0,line = 1,col = 1;
	BOOL inComment = FALSE,inQuote = FALSE,insideEntry = FALSE,openBrace = FALSE; // internal states
	bool inKey = false;

	begin = buf;
	type = BIBTEX_ENTRY_TYPE_UNKNOWN;

	while (*buf)   // Parser state machine
	{
		switch (*buf)
		{
			case _T('%') : // Toggle comment. Please note, that this is no official comment sign!
				if (depth != 0) break;
				inComment = TRUE;
				break;
			case _T('\"') : // Toggle quote
				if (inComment || !insideEntry) break;
				/* Quotes must be included in arbitrary braces, if the fields are delimited by quotes */
				if (depth == 1 && inQuote && (lastChar && lastChar[0] == _T('\\')))
				{
					HandleParseError(STE_BIBTEX_ERR_QOUTEWITHINQUOTE,line,col);
				}
				/* Quotes are taken "as is", if included in arbitrary braces */
				if (depth >= 2 || (lastChar && lastChar[0] == _T('\\'))) break;

				inQuote = !inQuote;
				break;
			case _T('@') : // Start type
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
				if (m_IsATSignInBracesAllowed && depth >= 2) break;
				if (/*0 != depth*/ insideEntry && m_WarnWrongLevelAT)
				{
					HandleParseError(STE_BIBTEX_ERR_WRONGLEVELAT,line,col);
				}
				depth = 0;
				insideEntry = TRUE;
				begin = buf; // update pointer if on top level
				break;
			case _T('(') : // alternative beginning of an entry
				if (inComment || !insideEntry) break;
				if (0 == depth)   // process entry type, e. g. @Article
				{
					type = ProcessEntryType(begin,buf - begin,line);
					begin = buf;
					++depth; // only on level 0!
				}
				break;
			case _T('{') : // beginning of an entry, field or special chars within a field
				if (inComment || !insideEntry) break;
				openBrace = TRUE;
				inKey = true;

				if (0 == depth)   // process entry type, e. g. @Article
				{
					type = ProcessEntryType(begin,buf - begin,line);
					begin = buf;
				}
				++depth;
				break;
			case _T(',') : // field delimiter
				inKey = false;

				if (inComment || inQuote || !insideEntry) break;
				if (1 == depth)   // process entry field
				{
					ProcessArgument(begin,buf - begin,type,line);
					begin = buf;
				}
				break;
			case _T(')') : // alternative end of an entry
				if (inComment || inQuote || !insideEntry || inKey) break;
				if (1 == depth)   // process entry field and decrease stack depth
				{
					ProcessArgument(begin,buf - begin,type,line);

					--depth;
					if (depth == 0)
					{
						insideEntry = FALSE;
						FinalizeItem();
					}
				}
				break;
			case _T('}') : // ending of an entry, field or special chars within a field
				if (inComment || !insideEntry) break;
				openBrace = FALSE;
				if (1 == depth)   // process entry field and decrease stack depth
				{
					ProcessArgument(begin,buf - begin,type,line);
				}
				--depth;
				if (depth == 0)   // we're back on earth -> reset parser state and complete item
				{
					insideEntry = FALSE;
					FinalizeItem();
				}
				break;
		} // end switch

		if (*buf == _T('\n'))   // found a new line
		{
			inComment = FALSE; // quit comment
			col = 0;
			++line; // update line number
		}

		// move to next TCHAR in buffer
		++col;
		lastChar = buf;
		++buf;
	}

	// Check if parser finished correctly
	if (depth > 0)
	{
		HandleParseError(STE_BIBTEX_ERR_INVALID_EOF,line,col);
	}

	TRACE2("\n%s: Found %d entries\n",m_Filename,m_Entries.GetCount());
	return TRUE;
}

/**
 * Find the corresponding BibTeX entry type and issues an error, if entry type is unknown
 */
BibTeXEntryType BibTeXFile::ProcessEntryType(const TCHAR *buf,int len,int line)
{
	CString entry;

	if (!SaveCopyBuffer(buf + 1,len - 1))   // buffer sanity check
	{
		HandleParseError(STE_BIBTEX_ERR_SUSPICOUS_LINE,line,1,m_Buffer);
	}

	entry = m_Buffer; // remove leading WS
	entry.Trim();

	try {
		return GetBibTeXEntryTypeFromString(entry);
	}
	catch (std::invalid_argument&) {
		// type not found -> raise error msg
		HandleParseError(STE_BIBTEX_ERR_INVALID_TYPE,line,1,entry);
	}

	return BIBTEX_ENTRY_TYPE_UNKNOWN;
}

void BibTeXFile::ProcessArgument(const TCHAR *buf,int len, BibTeXEntryType type,int line)
{
	BibTeXEntry *be,*dummy;

	if (!SaveCopyBuffer(buf + 1,len - 1))
	{
		HandleParseError(STE_BIBTEX_ERR_SUSPICOUS_LINE,line,1,m_Buffer);
	}

	/* Skip comments and preamble */
	if (type == BIBTEX_ENTRY_TYPE_PREAMBLE || type == BIBTEX_ENTRY_TYPE_COMMENT)
	{
		return;
	}

	if (type == BIBTEX_ENTRY_TYPE_UNKNOWN)
	{
		TRACE2("** Ignore unknown entry at line %d: %s\n",line,m_Buffer);
		return;
	}

	//TRACE("Processing argument: %s, type %d at line %d\n", m_Buffer, type, line);

	// strings have no explicit key, instead we are using the first field name as key
	if (type == BIBTEX_ENTRY_TYPE_STRING && NULL != _tcsstr(buf,_T("=")))
	{
		CString name,val;
		ParseField(m_Buffer,name,val);

		name.Trim();
		val.Trim();

		m_Strings.SetAt(name,val);
		return;
	}

	if (NULL == _tcsstr(m_Buffer,_T("=")))   // argument is key?
	{
		CString key = m_Buffer;
		key.Trim();

		if (key.IsEmpty())   // invalid key?
		{
			return;
		}

		if (!m_Entries.Lookup(key,(CObject*&) dummy))   // key already exists?
		{
			be = new BibTeXEntry(m_Buffer,this,type);
			/* add entries used by structure parser */
			be->m_nLine = line;
			//TRACE("Adding key %s\n", key);
			m_Entries.SetAt(key,be);
			m_LastKey = key;
			m_Keys.Add(key);
		}
		else
		{
			HandleParseError(STE_BIBTEX_ERR_DUP_KEY,line,1,key);
			TRACE2("WARNING: Invalid or duplicate key <%s> (%s)\n",key,
				::GetString(type));
		}
	}
	else   // extract name-value pair and add it to the entry
	{
		if (m_Entries.Lookup(m_LastKey,(CObject*&) be))
		{
			CString name,val;
			if (!ParseField(m_Buffer,name,val))
			{
				HandleParseError(STE_BIBTEX_ERR_SUSPICOUS_LINE,line,1);
				return;
			}

			//TRACE("Set Value: <%s> = <%s>\n", name, val);

			// Clean up contents
			name.MakeLower();
			name.Trim();

			ReplaceSpecialChars(val);
			
			val.Trim();
			be->SetField(name,val);
		}
		else   // error: key not found -> likely an error in the bibtex file
		{
			HandleParseError(STE_BIBTEX_ERR_MISSING_KEY,line,1);
		}
	}
}

/**
 * Setup error message and adds it to the message list
 */
void BibTeXFile::HandleParseError(UINT msgID,int line,int col,const TCHAR *addDesc)
{
	CString s,key;

	++m_ErrorCount;
	key.Format(_T("Parse_Error%d"),m_ErrorCount);

	CString errMsgFmt = AfxLoadString(msgID);

	switch (msgID)
	{
		case STE_BIBTEX_ERR_MISSING_KEY:
		case STE_BIBTEX_ERR_INVALID_EOF:
		case STE_BIBTEX_ERR_QOUTEWITHINQUOTE:
		case STE_BIBTEX_ERR_WRONGLEVELAT:
		case STE_BIBTEX_ERR_SUSPICOUS_LINE:
			s.Format(errMsgFmt,m_Filename,line,col);
			break;
		case STE_BIBTEX_ERR_INVALID_TYPE:
		case STE_BIBTEX_ERR_DUP_KEY:
			s.Format(errMsgFmt,m_Filename,line,col,addDesc);
			break;
		default:
			TRACE1("BibTeX: Warning: No handler for msgID %d\n",msgID);
	}

	//TRACE(s + "\n");

	BibTeXEntry *be = new BibTeXEntry(s,this, BIBTEX_ENTRY_TYPE_UNKNOWN);
	be->SetLine(line);
	be->SetType(StructureItem::bibItem);
	be->SetTitle(s);
	//be->m_strComment = _T("");
	be->SetCaption(s);
	be->SetLabel(key);
	be->SetPath(m_Filename);

	m_ErrorMsgs.Add(be);
	//m_Entries.SetAt(key, be);

}

/**
 * Parses a BibTeX field and returns a corresponding key-value pair.
 */
BOOL BibTeXFile::ParseField(const TCHAR *field,CString &name,CString &val)
{
	CONST TCHAR* eqChar = _tcsstr(field,_T("="));
	
    if (NULL == eqChar)
	{
		return FALSE;
	}

    size_t n = _tcslen(field);

	// Extract name and value
	size_t len1 = eqChar - field;
	size_t len2 = n - (eqChar - field) - 1;

	if (len1 > MAX_BIBTEX_ARG_LENGTH || len2 > MAX_BIBTEX_ARG_LENGTH)
	{
		return FALSE;
	}

	name = field;
	val = field;

	name.Delete(static_cast<int>(len1), static_cast<int>(n - len1));
	val.Delete(0, static_cast<int>(len1 + 1));

	/// remove whitespace
	name.Trim();
	val.Trim();
	val.Replace(_T("\n"), _T(" "));

	return TRUE;
}

/**
 * Drops all entries including key and string hash table
 */
void BibTeXFile::DropAllEntries()
{
	POSITION pos = m_Entries.GetStartPosition();
	while (pos != NULL)   // delete key-entry hashtable
	{
		BibTeXEntry *be;
		CString key;
		m_Entries.GetNextAssoc(pos,key,(CObject*&) be);
		if (be != NULL)
		{
			//TRACE("Deleting entry %s\n", be->ToString());
			delete be;
		}
		else
		{
			TRACE("NP detected in CBiBTeXFile %s",m_Filename);
		}
		m_Entries.RemoveKey(key);
	}

	m_Keys.RemoveAll(); // drop all keys
	m_Strings.RemoveAll(); // drop all abbreviations
	for (int i = 0; i < m_ErrorMsgs.GetSize(); i++)   // cleanup memory
	{
		BibTeXEntry *be = dynamic_cast<BibTeXEntry*>(m_ErrorMsgs.GetAt(i));
		if (be != NULL)
		{
			delete be;
		}
	}
}

/**
 * Invoked by the parser if an BibTeX entry is complete. The BibTeXEntry object is filled up with the
 * missing data needed for the structure view.
 */
void BibTeXFile::FinalizeItem()
{
	BibTeXEntry *be;
	if (m_Entries.Lookup(m_LastKey,(CObject*&) be))
	{
		be->SetType(StructureItem::bibItem);

		CString BeCaption, BeTitle, BeKey;
		be->ToCaptionTitleKey(BeCaption, BeTitle, BeKey);

		be->SetTitle(BeCaption);
		be->SetCaption(BeCaption);
		be->SetLabel(BeKey);
		be->SetComment(BeTitle);
		be->m_strPath = m_Filename;
		//TRACE("Finalized %s\n", be->ToString());
	}
	else
	{
		//ASSERT(FALSE);
		// TRACE("Warning: Did not found key %s\n", m_LastKey);
	}
}

/**
 * Returns an BibTeX entry for a given key or NULL, if key not exists
 */
const BibTeXEntry* BibTeXFile::GetEntryByKey(const CString& key) const
{
	UNUSED_ALWAYS(key);
	BibTeXEntry *be;

	if (!m_Entries.Lookup(m_LastKey,(CObject*&) be))
	{
		be = 0;
	}

	return be;
}

/**
 * Returns a string (an abbreviation) of the BibTeX file or an empty string, if not exist.
 */
const CString BibTeXFile::GetString(const CString& abbrev) const
{
	CString expanded;
	m_Strings.Lookup(abbrev,expanded);

	return expanded;
}

/**
 * Removes unwanted whitespace from a string
 */
void BibTeXFile::ReplaceSpecialChars(CString &value)
{
	// Strip off surrounding quotes
	if (!value.IsEmpty() && value[0] == _T('\"') && value[value.GetLength() - 1] == _T('\"'))
	{
		value.Delete(0);
		value.Delete(value.GetLength() - 1);
	}
	// Strip off surrounding braces
	if (!value.IsEmpty() && value[0] == _T('{') && value[value.GetLength() - 1] == _T('}'))
	{
		value.Delete(0);
		value.Delete(value.GetLength() - 1);
	}

	// Replace umlauts (incomplete!)
	value.Replace(_T("\"o"),_T("ö"));
	value.Replace(_T("\"a"),_T("ä"));
	value.Replace(_T("\"u"),_T("ü"));
	value.Replace(_T("\"O"),_T("Ö"));
	value.Replace(_T("\"A"),_T("Ä"));
	value.Replace(_T("\"U"),_T("Ü"));
	value.Replace(_T("\"ss"),_T("ß"));
}

/**
 * Copies a string buffer to a local buffer with checking the requested length.
 */
BOOL BibTeXFile::SaveCopyBuffer(LPCTSTR buffer, int reqSize)
{
	BOOL ret = TRUE;
	ASSERT(reqSize >= 0);

	if (reqSize == 0) return TRUE;

	/* NOTE: We cut the buffer here instead of reallocate it in order
	   to avoid heap corruptions. Large buffers may caused by invalid braces
	 */
	if (reqSize > m_BufferSize)   // cut buffer, if needed
	{
		reqSize = m_BufferSize - 1;
		ret = FALSE;
	}

	m_Buffer.SetString(buffer, static_cast<int>(reqSize));

	return ret;
}

const CString& BibTeXFile::GetFileName() const
{
	return m_Filename;
}

void BibTeXFile::SetFileName(const CString& filename)
{
	m_Filename = filename;
}

int BibTeXFile::GetEntriesCount() const
{
	return m_Entries.GetCount();
}

int BibTeXFile::GetErrorCount() const
{
	return m_ErrorCount;
}

BOOL BibTeXFile::IsATSignInBracesAllowed() const
{
	return m_IsATSignInBracesAllowed;
}

void BibTeXFile::SetATSignInBracesAllowed( bool flag )
{
	m_IsATSignInBracesAllowed = flag;
}

BOOL BibTeXFile::IsWarnWrongLevelAT() const
{
	return m_WarnWrongLevelAT;
}

void BibTeXFile::SetWarnWrongLevelAT( bool flag )
{
	m_WarnWrongLevelAT = flag;
}

const CMapStringToOb * BibTeXFile::GetEntries() const
{
	return (const CMapStringToOb *)&m_Entries;
}

const CObArray* BibTeXFile::GetErrorMessages() const
{
	return (const CObArray*)&m_ErrorMsgs;
}

const CStringArray* BibTeXFile::GetKeys() const
{
	return (const CStringArray*)&m_Keys;
}
