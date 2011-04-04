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

#if !defined(AFX_FILEGREP_H__A010A5E0_1F74_11D4_A221_006097239934__INCLUDED_)
#define AFX_FILEGREP_H__A010A5E0_1F74_11D4_A221_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TextSource.h"

/** forward declaration of class CFileGrep */
class CFileGrep;

/**
An object of a CFileGrepHandler-derived class retrieves the results
of a text search about more files performed by a CFileGrep-object.

<b>Remarks:</b> If the search of CFileGrep is performed in an own
thread, the handler functions of this class a called out of this
thread.

@author Sven Wiegand
 */
class CFileGrepHandler
{
public:
	/**
	Called, when the CFileGrep-object hits.

	@param pFileGrep
	        Pointer to the CFileGrep-object that got a hit.
	@param lpszPath
	        Path of the file in which the object hit.
	@param nLine
	        One-based line of the hit in lpszPath.
	@param lpszLine
	        The text of the line that hit.
	 */
	virtual void OnFileGrepHit(CFileGrep *pFileGrep, LPCTSTR lpszPath, int nLine, LPCTSTR lpszLine) = 0;

	/**
	Called, when a text file cannot be accessed.

	@param pFileGrep
	        Pointer to the CFileGrep-object, for which the error occured.
	@param lpszPath
	        Path of the file, that cannot be accessed.
	 */
	virtual void OnFileGrepError(CFileGrep *pFileGrep, LPCTSTR lpszPath) = 0;

	/**
	Called, when the search of a CFileGrep-object is over.

	@param nHits
	        Total number of hits.
	 */
	virtual void OnFileGrepFinished(CFileGrep *pFileGrep, int nHits) = 0;
};

/**
An object of the class CFileGrep can be used to search for a text or
a regular expression in one ore more files, which names matches a
wildcard in one directory (and maybe its subdirectories)

An object that implements CFileGrepHandler is needed to get the
results of the search.

An object that implements CTextSourceManager is needed to get the
text to search.

One ore more objects that implement CTextSource are needed to get
the text to search.

@author Sven Wiegand
 */
class CFileGrep
{
// construction/destruction
public:
	CFileGrep();
	virtual ~CFileGrep();

// definitions
public:

	enum tagFlags
	{
		flagOwnThread = 1,
		flagWholeWords = flagOwnThread << 1,
		flagCaseSensitive = flagWholeWords << 1,
		flagRegularExpression = flagCaseSensitive << 1,
		flagIncludeSubFolders = flagRegularExpression << 1
	};

// operations
public:
	/**
	Starts the text search.

	@param pFileGrepHandler
	        CFileGrepHandler-derived object that gets informed about search events.
	@param pTextSourceManager
	        CTextSourceManager-derived object that is responsible for providing
	        CTextSource-derived objects that contain the text to search.
	@param lpszSearchText
	        Text to search for. If the Flag flagRegularExpression is set in dwFlags,
	        this text is treated as a regular expression.
	@param lpszFiles
	        A semicolon separated list of files to search lpszSearchString in.
	        The file names may contain wildcards (* or ?).
	@param lpszFolder
	        The folder to find the files to search in. If the flag
	        flagIncludeSubFolders is set in dwFlags, all sub folders of this
	        folder are also searched.
	@param dwFlags
	        A combination of the following flags:
	        <UL>
	                <li><b>flagOwnThread</b> Set this flag, if the search should be
	                        performed in an own thread. All methods of pFileGrepHandler
	                        and pTextSourceManager and the CTextSource-objects will be
	                        called from this thread. The thread terminates, when the
	                        search has finished.
	                <li><b>flagWholeWords</b> Set this flag, if only whole words
	                        matches.
	                <li><b>flagCaseSensitive</b> Set this flag, if the search should
	                        be case sensitive.
	                <li><b>flagRegularExpression</b> Set this flag, if lpszSearchString
	                        is a regular expression.
	                <li><b>flagIncludeSubFolders</b> Set this flag, if sub folders of
	                        lpszFolder should also be searched.
	        </UL>

	@return
	        If the flag flagOwnThread is set, the return parameter is TRUE, if the
	        search thread has been startet successfully and FALSE, if the
	        thread has not been started (maybe there is already a running grep
	        thread. If the flag flagOwnThread is not set,
	        the the parameter is TRUE when the search finished or FALSE if
	        an error occured.
	 */
	BOOL Grep(
	    CFileGrepHandler *pFileGrepHandler, /*CTextSourceManager *pTextSourceManager,*/
	    LPCTSTR lpszSearchText, LPCTSTR lpszFiles, LPCTSTR lpszFolder, DWORD dwFlags = flagIncludeSubFolders);

// implementation helpers
protected:
	static void SplitFileList(LPCTSTR lpszFiles, CArray<CString, const CString&> &astrFiles);
	void FindFiles(const CArray<CString, const CString&> &astrPatterns, LPCTSTR lpszFolder, CSortArray<CString, const CString&> &astrFilePaths);
	BOOL GrepLine(LPCTSTR lpszLine);
	void GrepFile(LPCTSTR lpszPath);
	BOOL GrepHelper();

// friends
private:
	friend UINT GrepThread(LPVOID lpFileGrepObject);

// attributes
protected:
	/** The event handler. */
	CFileGrepHandler *m_pFileGrepHandler;

	/** The text source manager. */
	//	CTextSourceManager *m_pTextSourceManager;

	/**
	The regular expression we are searching for

	Search is always performed as regular expression
	search. If normal text search is wanted (m_bRegularExpression is
	FALSE), a regular expression is build to find the normal text.
	 */
	tregex m_regEx;

	/** Semicolon separated list of file to search */
	CString m_strFiles;

	/** Folder to search. */
	CString m_strFolder;

	/** TRUE if the flagWholeWords-Flag was set. */
	BOOL m_bWholeWords;

	/** TRUE if the flagCaseSensitive-Flag was set. */
	BOOL m_bCaseSensitive;

	/** TRUE if the flagRegularExpression-Flag was set. */
	BOOL m_bRegularExpression;

	/** TRUE if the flagIncludeSubFolders-flag was set. */
	BOOL m_bIncludeSubFolders;

	/** Number of hits. */
	int m_nHits;

	/** The grep-thread. */
	CWinThread *m_pGrepThread;

	/**
	Semaphore that is signaled (available), if no grep thread is
	running and	unsignaled otherwise.
	 */
	CSemaphore m_canRun;
};

#endif // !defined(AFX_FILEGREP_H__A010A5E0_1F74_11D4_A221_006097239934__INCLUDED_)
