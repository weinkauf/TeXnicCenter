// BiBTeXEntry.cpp: implementation of the CBiBTeXEntry class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BiBTeXEntry.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


const TCHAR* BibTypeVerbose[] = {
"Book", "Article", "Booklet", "Manual", "Inproceedings", 
"Conference", "Inbook", "Incollection", "Masterthesis",
"Misc", "PhDthesis", "Proceedings", "TechReport",
"Unpublished", "Unknown"
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
	CString author, title, year;
	GetField(_T("author"), author);
	GetField(_T("title"), title);
	GetField(_T("year"), year);

	// beautify output
	author.Replace(_T("{"), _T(""));
	author.Replace(_T("}"), _T(""));
	author.Replace(_T(" and"), _T(","));
	title.Replace(_T("{"), _T(""));
	title.Replace(_T("}"), _T(""));
	year.Replace(_T("{"), _T(""));
	year.Replace(_T("}"), _T(""));

	return CString(_T("[") + m_Key + _T("] ") + 
		author + _T(": ") + 
		title + _T(" (") + 
		BibTypeVerbose[m_Type] + _T(", ") +
		year + _T(")")
		);
}
