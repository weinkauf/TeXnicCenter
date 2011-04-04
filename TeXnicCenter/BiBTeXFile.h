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
const int MAX_BIBTEX_ARG_LENGTH = 8192;

/** Represents a BibTeX file. The corresponding file name have to be passed via constructor */
class BibTeXFile : public CObject
{
public:
	/** Constructs a new BibTeXFile object. After the object is created, call ProcessFile to
	        scan a bib file for entries.
	        @param file Name of the BibTeX file (.bib)
	 */
	explicit BibTeXFile(const CString& file);
	/** Destroys the CBiBTeXFile object */
	virtual ~BibTeXFile();

	/** Returns current filename of the BibTeX file */
	const CString& GetFileName() const;

	/** Sets a new filename */
	void SetFileName(const CString& filename);

	/** Parses a BibTeX file @see SetFilename*/
	BOOL ProcessFile();

	/** Returns the number of Bibtex entries. */
	int GetEntriesCount() const;

	/** Returns a map with all BibteX entries. The map key corresponds to the key given
	        in the BibTeX file.
	 */
	const CMapStringToOb *GetEntries() const;
	/** Returns an entry for a given key
	        @return Pointer to corresponding entry or NULL, of no entry was found
	 */
	const BibTeXEntry* GetEntryByKey(const CString& key) const;

	/** Returns all keys defined in the BibTeX file
	        @return Pointer to string array containing the keys.
	 */
	const CStringArray* GetKeys() const;

	/** Returns all error msgs occurred during parse
	        @return Pointer to string array containing the error msgs.
	 */
	const CObArray* GetErrorMessages() const;

	/** Removes all BibTeX entries, including the abbrev strings */
	void DropAllEntries();

	/** Returns the number or parse errors */
	int GetErrorCount() const;

	/** Returns the expanded version of an abbreviation defined via @string command
	        @return Expanded string for given abbreviation or an empty string, if not found
	 */
	const CString GetString(const CString& abbrev) const;

	/** Flag for allowing '@' sign enclosed in braces. Normally you have to use quotes to
	    make BibTeX not using it to indicate an entry. Default is TRUE.

	        m_AllowATSignInBraces = FALSE:<br>
	        email = "hanswurst@nowhere.de" (OK, @ sign enclosed in quotes)<br>
	        email = {hanswurst@nowhere.de} (<b>Error</b>, @ sign enclosed in braces)<br>

	        m_AllowATSignInBraces = TRUE:<br>
	        email = "hanswurst@nowhere.de" (OK, @ sign enclosed in quotes)<br>
	        email = {hanswurst@nowhere.de} (OK, @ sign enclosed in braces)<br>
	 */
	BOOL IsATSignInBracesAllowed() const;

	void SetATSignInBracesAllowed(bool flag);

	/** If true, a warning is issued if @ sign is not found on top level. Default is TRUE.<br>
	Example I<br>
	<code>@Comment{@Book{steward03, [...]}}</code>
	Example II<br>
	<code>{ @Book{steward03, [...]}}</code>
	issues a warning, since on opening brace percedes the book entry. BibTex would NOT consider
	the book entry in this case, since the brace is outside an entry and is treated as comment.
	 */
	BOOL IsWarnWrongLevelAT() const;

	void SetWarnWrongLevelAT(bool flag);

private:
	/* Copies needed part of the parsed file stream for further ops. Otherwise we had to operate on
	a very large buffer which would slow down performance significantly!
	So the maximum buffer size is limited by MAX_BIBTEX_ARG_LENGTH. If the requested buffer is larger,
	it will be cut to this length. Reason: Large buffers often indicate an error (e. g. an unbalanced brace), so we
	had to take care of this. There may be cases, where BibTeX entries contain large abstracts and may exceed
	this buffer. But we just want to show the entry in the navigator, so missing contents of the abstract field
	are not important.
	        @param buffer Pointer to original buffer
	        @param reqSize Size of string to copy from the buffer
	        @return TRUE, if copy was successfully
	 */
	BOOL SaveCopyBuffer(const TCHAR* buffer, int reqSize);
	/* Called, if an item is complete. */
	void FinalizeItem();
	/* Parses a field entry 'name = value' */
	BOOL ParseField(const TCHAR *field, CString &name, CString &val);
	/** Issues a parser warning/error
	        @param msgID ID of error msg (String in STRINGTABLE resource9
	        @param line Line number of warning/error
	        @param col Column of warning/error
	        @param addDesc is an optional string (e. g. for dup keys or invalid BibTeX types)
	 */
	void HandleParseError(UINT msgID, int line, int col, const TCHAR *addDesc = _T(""));
	/** Determines the BibTeX type (Article, Book, ...)
	        @param buf Pointer to buffer containing the tokens
	        @param len Length of the buffer
	        @param line Line number

	        @return BibTeX type or CBiBTeXEntry::Unknown, if invalid entry
	 */
	BibTeXEntryType ProcessEntryType(const TCHAR *buf, int len, int line);
	/** Processes a field of an BibTeX entry
	        @param buf Pointer to buffer containing the tokens
	        @param len Length of the buffer
	        @param type BibTeX type (Article, Book, ...)
	        @param line Line number
	 */
	void ProcessArgument(const TCHAR *buf, int len, BibTeXEntryType type, int line);
	BOOL ParseFile(const TCHAR* buf);

	BOOL m_IsATSignInBracesAllowed;
	BOOL m_WarnWrongLevelAT;
	int m_ErrorCount;
	int m_BufferSize;
	TCHAR* m_Buffer;
	CString m_Filename;
	CMapStringToOb m_Entries;
	CMapStringToString m_Strings;
	CString m_LastKey;
	CStringArray m_Keys;
	CObArray m_ErrorMsgs;

protected:
	static void ReplaceSpecialChars(CString &value);
};

#endif // !defined(AFX_BIBTEXFILE_H__8631BC01_4CE2_4706_995E_5230B32EF3FA__INCLUDED_)
