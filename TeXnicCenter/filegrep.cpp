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
#include "FileGrep.h"
#include "TextSourceFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


//-------------------------------------------------------------------
// global GrepThread
//-------------------------------------------------------------------

UINT GrepThread(LPVOID lpFileGrepObject)
{
	ASSERT(lpFileGrepObject);

	if (!lpFileGrepObject)
		return ~0U;

	// get grep object
	CFileGrep *pThis = (CFileGrep*) lpFileGrepObject;
	return !pThis->GrepHelper();
}


//-------------------------------------------------------------------
// class CFileGrep
//-------------------------------------------------------------------

CFileGrep::CFileGrep()
		: m_pGrepThread(FALSE),
		m_canRun(1) // make available
{
}

CFileGrep::~CFileGrep()
{
}

BOOL CFileGrep::Grep(
    CFileGrepHandler *pFileGrepHandler,/*CTextSourceManager *pTextSourceManager,*/
    LPCTSTR lpszSearchText,LPCTSTR lpszFiles,LPCTSTR lpszFolder,DWORD dwFlags /*= flagIncludeSubFolders*/)
{
	// parameter testing
	ASSERT(pFileGrepHandler);
	//	ASSERT( pTextSourceManager );
	ASSERT(lpszSearchText);
	ASSERT(lpszFiles);
	ASSERT(lpszFolder);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// make thread save
	if (!m_canRun.Lock(0))
		return FALSE; // search is already running

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// parameter preparation
	m_pFileGrepHandler = pFileGrepHandler;
	//	m_pTextSourceManager = pTextSourceManager;
	m_strFiles = lpszFiles;
	m_strFolder = lpszFolder;
	m_bCaseSensitive = dwFlags & flagCaseSensitive;
	m_bIncludeSubFolders = dwFlags & flagIncludeSubFolders;
	m_bRegularExpression = dwFlags & flagRegularExpression;
	m_bWholeWords = dwFlags & flagWholeWords;
	m_nHits = 0;

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// build regular expression to search for
	CString strSearch(lpszSearchText);

	if (!m_bRegularExpression)
	{
		// build regular expression for normal text search.
		// represent each character by its hexdecimal code
		CString strNewSearch,strTemp;
		unsigned char cTemp;

		for (int i = 0; i < strSearch.GetLength(); i++)
		{
			cTemp = strSearch[i];
			strTemp.Format(_T("%s\\x%2X"),(LPCTSTR) strNewSearch,cTemp);
			strNewSearch = strTemp;
		}

		strSearch = strNewSearch;
	}
	if (m_bWholeWords)
	{
		strSearch = _T("\\b") + strSearch; // start of a word
		strSearch += _T("\\b"); // end of a word
	}

	try
	{
		if (m_bCaseSensitive)
			m_regEx.assign(strSearch);
		else
			m_regEx.assign(strSearch,std::regex_constants::icase);
	}
	catch (...)
	{
		return FALSE; // invalid expression
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// start search
	if (dwFlags & flagOwnThread)
	{
		// create thread
		if (AfxBeginThread(GrepThread,(LPVOID)this))
			return TRUE;
		else
		{
			m_canRun.Unlock();
			return FALSE;
		}
	}
	else
		// run in this thread
		return GrepHelper();
}

void CFileGrep::SplitFileList(LPCTSTR lpszFiles,CArray<CString,const CString&> &astrFiles)
{
	// split file list into array
	CString strFiles(lpszFiles);
	CString strPart;

	astrFiles.RemoveAll();

	strFiles.TrimLeft(_T(";, \t"));
	strFiles.TrimRight(_T(";, \t"));

	while (!strFiles.IsEmpty())
	{
		// get next element
		strPart = strFiles.SpanExcluding(_T(";, \t"));
		strPart.TrimRight(_T(";, \t"));

		// remove element from list
		strFiles = strFiles.Right(strFiles.GetLength() - strPart.GetLength());

		// remove separator and leading whitespaces from list
		strFiles.TrimLeft(_T(";, \t"));

		if (strPart.IsEmpty())
			continue;
		if (strPart.GetLength() == 1)
			continue;

		// add element to array
		astrFiles.Add(strPart);
	}
}

void CFileGrep::FindFiles(const CArray<CString, const CString&> &astrPatterns,LPCTSTR lpszFolder,CSortArray<CString,const CString&> &astrFilePaths)
{
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// find files
	CFileFind ff;
	for (int i = 0; i < astrPatterns.GetSize(); i++)
	{
		BOOL bMoreFiles = ff.FindFile(CPathTool::Cat(lpszFolder,astrPatterns[i]));

		while (bMoreFiles)
		{
			bMoreFiles = ff.FindNextFile();
			astrFilePaths.InsertSorted(ff.GetFilePath());
		}

		ff.Close();
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// recursive through sub folders
	if (m_bIncludeSubFolders)
	{
		BOOL bMoreFiles = ff.FindFile(CPathTool::Cat(lpszFolder,_T("*.*")));

		while (bMoreFiles)
		{
			bMoreFiles = ff.FindNextFile();
			if (ff.IsDirectory() && !ff.IsDots())
				FindFiles(astrPatterns,ff.GetFilePath(),astrFilePaths);
		}

		ff.Close();
	}
}

void CFileGrep::GrepFile(LPCTSTR lpszPath)
{
	//	CTextSource	*pTextSource = m_pTextSourceManager->GetTextSource( lpszPath );
	CTextSourceFile *pTextSource = NULL;

	pTextSource = new CTextSourceFile();
	ASSERT(pTextSource != NULL);
	if (pTextSource && !pTextSource->Create(lpszPath))
	{
		delete pTextSource;
		pTextSource = NULL;
	}

	if (!pTextSource)
	{
		m_pFileGrepHandler->OnFileGrepError(this,lpszPath);
		return;
	}

	// grep whole file
	LPCTSTR lpszLine = NULL;
	int nLength;

	for (int nLine = 1; pTextSource->GetNextLine(lpszLine,nLength); nLine++)
	{
		if (regex_search(lpszLine,m_regEx))
		{
			m_pFileGrepHandler->OnFileGrepHit(this,lpszPath,nLine,lpszLine);
			++m_nHits;
		}
	}

	pTextSource->Delete();
}

BOOL CFileGrep::GrepHelper()
{
	ASSERT(m_pFileGrepHandler);
	//	ASSERT( m_pTextSourceManager );

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// get list of files to search
	CSortArray<CString, const CString&> astrFilePaths;
	CArray<CString,const CString&> astrFiles;

	SplitFileList(m_strFiles,astrFiles);
	astrFilePaths.RemoveAll();

	FindFiles(astrFiles,m_strFolder,astrFilePaths);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// search all files
	for (int i = 0; i < astrFilePaths.GetSize(); i++)
		GrepFile(astrFilePaths[i]);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// search finished -- inform handler
	m_pFileGrepHandler->OnFileGrepFinished(this,m_nHits);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// release semaphore and return
	m_canRun.Unlock();
	return TRUE;
}