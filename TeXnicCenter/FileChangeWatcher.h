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

#if !defined(AFX_FILECHANGEWATCHER_H__431478E2_B4F5_11D5_A222_006097239934__INCLUDED_)
#define AFX_FILECHANGEWATCHER_H__431478E2_B4F5_11D5_A222_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
Class for checking for changes of a file.

@author Sven Wiegand
*/
class CFileChangeWatcher  
{
// construction/destruction
public:
	CFileChangeWatcher();
	virtual ~CFileChangeWatcher();

// constants
public:
	enum changeType
	{
		chNone = 0,
		chReadOnly = 1,
		chModTime = chReadOnly<<1,
		chLength = chModTime<<1,
		chAll = chReadOnly|chModTime|chLength
	};

// operations
public:
	/**
	Sets the path of the file to watch changes for and snaps the 
	file's current attributes.

	@param lpszPath
		The path of the file to watch the changes for or NULL to 
		deactivate the watcher.

	@return
		FALSE if the file cannot be accessed.
	*/
	BOOL SetFilePath(LPCTSTR lpszPath);

	/** Returns the path of the file, the watcher works on. */
	CString GetFilePath() const;

	/**
	Specifies which of the file's attributes should be checked for
	changes.

	@param wFlags
		A combination of the values of the changeType enumeration.
	*/
	void SetFileChangeFlags(WORD wFlags = chAll);

	/**
	Returns the file's attributes, the watcher checks for changes.

	@return
		A combination of the values of the changeType enumeration.
	*/
	WORD GetFileChangeFlags() const;

	/**
	Checks, if the file's state has changed. Only the file's 
	attributes,	that have been specified by SetChangeFlags() will be
	checked for changes.

	@param bSnapState
		TRUE if the watcher should also snap the file's current 
		attributes, FALSE if they should not be updated.

	@return
		A combination of the flags of the changeType enumeration, 
		specifieng the file's attributes, that have change or 0 if no
		changes have been detected.
	*/
	WORD GetFileChanges(BOOL bSnapState = TRUE);

	/**
	Snaps the current file attributes and stores them for later
	comparison by GetChanges().

	@retrun
		FALSE if the file cannot be accessed.
	*/
	BOOL SnapFileState();

	/**
	Returns TRUE if the file is writeprotected at the moment and FALSE
	otherwise.

	The result mirrors the values, acquired by the last SnapState() 
	call.
	*/
	BOOL IsFileReadOnly() const;

	/**
	Returns the file's current modification time.

	The result mirrors the values, acquired by the last SnapState() 
	call.
	*/
	CTime GetFileModificationTime() const;

	/**
	Returns the file's current length.

	The result mirrors the values, acquired by the last SnapState() 
	call.
	*/
	DWORD GetFileLength() const;

// attributes
private:
	/** The path of the file to watch */
	CString m_strFilePath;

	/** Attributes to check for changes */
	WORD m_wChangeMask;

	/** Latest files read only state */
	BOOL m_bReadOnly;

	/** Latest files modification time */
	CTime m_timeModification;

	/** Latest files length */
	DWORD m_dwFileLength;
};


// inlines
inline
CString CFileChangeWatcher::GetFilePath() const
{
	return m_strFilePath;
}


inline
WORD CFileChangeWatcher::GetFileChangeFlags() const
{
	return m_wChangeMask;
}


inline
BOOL CFileChangeWatcher::IsFileReadOnly() const
{
	return m_bReadOnly;
}


inline
CTime CFileChangeWatcher::GetFileModificationTime() const
{
	return m_timeModification;
}


inline
DWORD CFileChangeWatcher::GetFileLength() const
{
	return m_dwFileLength;
}

#endif // !defined(AFX_FILECHANGEWATCHER_H__431478E2_B4F5_11D5_A222_006097239934__INCLUDED_)
