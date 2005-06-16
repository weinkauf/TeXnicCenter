// BiBTeXFile.h: interface for the CBiBTeXFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BIBTEXFILE_H__8631BC01_4CE2_4706_995E_5230B32EF3FA__INCLUDED_)
#define AFX_BIBTEXFILE_H__8631BC01_4CE2_4706_995E_5230B32EF3FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BiBTeXEntry.h"

/* Must have this length, because some authors include large abstracts */
#define MAX_BIBTEX_ARG_LENGTH 4096

/** Represents a BibTeX file. The corresponding file name have to be passed via constructor */
class CBiBTeXFile : public CObject  
{
public:	
	
	CBiBTeXFile(CString file);
	virtual ~CBiBTeXFile();

	/** Returns current filename of the BibTeX file */
	CString GetFilename() const {return m_Filename;}
	/** Sets a new filename
	void SetFilename(CString filename) {m_Filename = filename;}

	/** Parses a BibTeX file @see SetFilename*/
	BOOL ProcessFile();

	/** Returns a map with all BibteX entries. The map key corresponds to the key given
		in the BibTeX file.
	*/
	const CMapStringToOb *GetEntries() const {return (const CMapStringToOb *)&m_Entries;}
	/** Returns all keys defined in the BibTeX file */
	const CStringArray* GetKeys() const {return (const CStringArray*)&m_Keys;}

	/** Removes all BibTeX entries */
	void DropAllEntries();
private:
	/* Parses a field entry 'name = value' */
	BOOL ParseField(const TCHAR *field, CString &name, CString &val);
	void HandleParseError(const TCHAR *buf, int line, int col);
	CBiBTeXEntry::BibType ProcessEntryType(const TCHAR *buf, int len);
	void ProcessArgument(const TCHAR *buf, int len, CBiBTeXEntry::BibType);
	BOOL ParseFile(const TCHAR* buf);
	

	CString			m_Filename;
	CMapStringToOb	m_Entries;
	CString			m_LastKey;
	CStringArray	m_Keys;
};

#endif // !defined(AFX_BIBTEXFILE_H__8631BC01_4CE2_4706_995E_5230B32EF3FA__INCLUDED_)
