// BiBTeXFile.cpp: implementation of the CBiBTeXFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BiBTeXFile.h"
#include "BiBTeXEntry.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

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
	const TCHAR *begin;
	CBiBTeXEntry::BibType type;
	int depth = 0, line = 1, col = 1;
	BOOL inComment = FALSE, inAccent = FALSE;

	begin = buf;	
	type = CBiBTeXEntry::Unknown;

	while (*buf) { // Parser state machine
		switch(*buf) {
		case _T('%'): // Toggle comment
			inComment = TRUE;
			break;
		case _T('\"'): // Toggle quote
			if (inComment) break;
			inAccent = !inAccent;
			break;
		case _T('@'): // Start type
			if (inComment || inAccent) break;
			if (0 == depth) { // update pointer if on top level
				begin = buf;			
			}
			break;
		case _T('{'):
			if (inComment) break;
			if (0 == depth) { // process entry type, e. g. @Article
				type = ProcessEntryType(begin, buf-begin);
				begin = buf;
			}
			++depth;			
			break;
		case _T(','):
			if (inComment || inAccent) break;
			if (1 == depth) { // process entry field
				ProcessArgument(begin, buf-begin, type);
				begin = buf;
			}			
			break;
		case _T('}'):
			if (inComment) break;
			if (1 == depth) { // process entry field and decrease stack depth
				ProcessArgument(begin, buf-begin, type);				
			} 
			--depth;
			if (depth < 0) {
				HandleParseError(buf, line, col);
				return FALSE;
			}
			break;
		case _T('\n'): // update line number
			if (inAccent) { // Quotes may not exceed a line
				HandleParseError(buf, line, col);
				return FALSE;
			}
			inComment = FALSE;
			col = 0;
			++line;
			break;
		}
		col++;
		buf++;
	}

	TRACE("Found %d entries\n", m_Entries.GetCount());
	return TRUE;
}

CBiBTeXEntry::BibType CBiBTeXFile::ProcessEntryType(const TCHAR *buf, int len)
{
	TCHAR tmp[MAX_BIBTEX_ARG_LENGTH];
	
	ASSERT(len > 0 && len < MAX_BIBTEX_ARG_LENGTH);
	strncpy(tmp, buf + 1, len - 1);
	tmp[len - 1] = 0;
	//TRACE("Entry: %s\n", tmp);

	for (int i = 0; i < BIBTEX_ENTRY_SIZE; i++) {
		if (0 == stricmp(tmp, BibTypeVerbose[i])) {
			return (CBiBTeXEntry::BibType)i;
		}
	}
	return CBiBTeXEntry::Unknown;
}

void CBiBTeXFile::ProcessArgument(const TCHAR *buf, int len, CBiBTeXEntry::BibType type)
{
	TCHAR tmp[MAX_BIBTEX_ARG_LENGTH];
	CBiBTeXEntry *be, *dummy;

	ASSERT(len > 0 && len < MAX_BIBTEX_ARG_LENGTH);
	strncpy(tmp, buf + 1, len - 1);
	tmp[len - 1] = 0;


	if (type == CBiBTeXEntry::Unknown) {
		TRACE("** Ignore entry: %s\n", tmp);
		return;
	}
	//

	if (NULL == strstr(tmp, _T("="))) { // argument is key?		
		if (!m_Entries.Lookup(tmp, (CObject*&)dummy)) {
			be = new CBiBTeXEntry(tmp, type);
			m_Entries.SetAt(tmp, be);
			m_LastKey = tmp;
			m_Keys.Add(tmp);
		} else {
			TRACE("WARNING: Invalid or duplicate key <%s> (%s)\n", tmp, BibTypeVerbose[type]);
		}
	} else {
		
		//be = (CBiBTeXEntry *)m_Entries.Top();
		if (m_Entries.Lookup(m_LastKey, (CObject*&)be)) {
			CString name, val;
			ParseField(tmp, name, val);

			//TRACE("Set Value: <%s> = <%s>\n", name, val);
			name.MakeLower();
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
