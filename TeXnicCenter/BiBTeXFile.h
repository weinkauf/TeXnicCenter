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

#if !defined(AFX_BIBTEXFILE_H__8631BC01_4CE2_4706_995E_5230B32EF3FA__INCLUDED_)
#define AFX_BIBTEXFILE_H__8631BC01_4CE2_4706_995E_5230B32EF3FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BiBTeXEntry.h"

/* Must have this length, because some authors include large abstracts */
#define MAX_BIBTEX_ARG_LENGTH 8192

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

	/** Returns the number of Bibtex entries. */
	int GetEntriesCount() const
	{
		return m_Entries.GetCount();
	}

	/** Returns a map with all BibteX entries. The map key corresponds to the key given
		in the BibTeX file.
	*/
	const CMapStringToOb *GetEntries() const {return (const CMapStringToOb *)&m_Entries;}
	/** Returns an entry for a given key 
		@return Pointer to corresponding entry or NULL, of no entry was found
	*/
	const CBiBTeXEntry* GetEntryByKey(CString key);
	/** Returns all keys defined in the BibTeX file 
		@return Pointer to string array containing the keys.
	*/
	const CStringArray* GetKeys() const {return (const CStringArray*)&m_Keys;}

	/** Returns all error msgs ocurred during parse
		@return Pointer to string array containing the error msgs.
	*/
	const CObArray* GetErrorMsgs() const {return (const CObArray*)&m_ErrorMsgs;}

	/** Removes all BibTeX entries, including the abbrev strings */
	void DropAllEntries();

	/** Returns the number or parse errors */
	int GetErrorCount() const {return m_ErrorCount;}

	/** Returns the expanded version of an abbreviation defined via @string command
		@return Expanded string for given abbreviation or an empty string, if not found
	*/
	CString GetString(CString abbrev);

	/** Flag for allowing '@' sign enclosed in braces. Normally you have to use quotes to
	    make BibTeX not using it to indicate an entry. Default is TRUE.
		
		m_AllowATSignInBraces = FALSE:<br>
		email = "hanswurst@nowhere.de" (OK, @ sign enclosed in quotes)<br>
		email = {hanswurst@nowhere.de} (<b>Error</b>, @ sign enclosed in braces)<br>

		m_AllowATSignInBraces = TRUE:<br>
		email = "hanswurst@nowhere.de" (OK, @ sign enclosed in quotes)<br>
		email = {hanswurst@nowhere.de} (OK, @ sign enclosed in braces)<br>
		*/
	BOOL IsATSignInBracesAllowed() const {return m_IsATSignInBracesAllowed;}
	void SetATSignInBracesAllowed(BOOL flag) {m_IsATSignInBracesAllowed = flag;}

	/** If true, a warning is issued if @ sign is not found on top level. Default is TRUE.<br>
	Example I<br>
	<code>@Comment{@Book{steward03, [...]}}</code>
	Example II<br>
	<code>{ @Book{steward03, [...]}}</code>
	issues a warning, since on opening brace percedes the book entry. BibTex would NOT consider
	the book entry in this case, since the brace is outside an entry and is treated as comment.
	*/
	BOOL IsWarnWrongLevelAT() const {return m_WarnWrongLevelAT;}
	void SetWarnWrongLevelAT(BOOL flag) {m_WarnWrongLevelAT = flag;}

private:
	/* Called, if an item is complete */
	void FinalizeItem();
	/* Parses a field entry 'name = value' */
	BOOL ParseField(const TCHAR *field, CString &name, CString &val);
	/** Issues a parser warning/error 
		@param msgID ID of error msg (String in STRINGTABLE resource9
		@param line Line number of warning/error
		@param col Column of warning/error
		@param addDesc is an optional string (e. g. for dup keys or invalied BibTeX types)
	*/
	void HandleParseError(UINT msgID, int line, int col, const TCHAR *addDesc = _T(""));
	CBiBTeXEntry::BibType ProcessEntryType(const TCHAR *buf, int len, int line);
	void ProcessArgument(const TCHAR *buf, int len, CBiBTeXEntry::BibType, int line);
	BOOL ParseFile(const TCHAR* buf);

	BOOL m_IsATSignInBracesAllowed;
	BOOL m_WarnWrongLevelAT;
	int m_ErrorCount;
	CString			m_Filename;
	CMapStringToOb	m_Entries;
	CMapStringToString	m_Strings;
	CString			m_LastKey;
	CStringArray	m_Keys;
	CObArray		m_ErrorMsgs;
protected:
	void ReplaceSpecialChars(CString &value);
};

#endif // !defined(AFX_BIBTEXFILE_H__8631BC01_4CE2_4706_995E_5230B32EF3FA__INCLUDED_)
