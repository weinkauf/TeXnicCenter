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
#include "stdafx.h"
#include "FileChangeWatcher.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//-------------------------------------------------------------------
// class CFileChangeWatcher
//-------------------------------------------------------------------

CFileChangeWatcher::CFileChangeWatcher()
:	m_wChangeMask(chAll),
	m_bReadOnly(FALSE),
	m_dwFileLength(0)
{}


CFileChangeWatcher::~CFileChangeWatcher()
{}


BOOL CFileChangeWatcher::SetFilePath(LPCTSTR lpszPath)
{
	if (!lpszPath)
	{
		m_strFilePath.Empty();
		return TRUE;
	}

	m_strFilePath = lpszPath;
	if (SnapFileState())
		return TRUE;

	// SnapState() failed
	m_strFilePath.Empty();
	return FALSE;
}


void CFileChangeWatcher::SetFileChangeFlags(WORD wFlags /*= chAll*/)
{
	ASSERT(wFlags);

	m_wChangeMask = wFlags;
}


BOOL CFileChangeWatcher::SnapFileState()
{
	CFileFind	ff;
	if (!ff.FindFile(m_strFilePath))
		return FALSE;

	ff.FindNextFile();
	m_bReadOnly = ff.IsReadOnly();
	ff.GetLastWriteTime(m_timeModification);
	m_dwFileLength = ff.GetLength();

	return TRUE;
}


WORD CFileChangeWatcher::GetFileChanges(BOOL bSnapState /*= TRUE*/)
{
	BOOL	bOldReadOnly = m_bReadOnly;
	CTime	timeOldModification(m_timeModification);
	DWORD	dwOldFileLength = m_dwFileLength;

	if (!SnapFileState())
		return chNone;

	WORD	wChanges = chNone;

	if (bOldReadOnly != m_bReadOnly)
		wChanges|= chReadOnly;
	if (timeOldModification != m_timeModification)
		wChanges|= chModTime;
	if (dwOldFileLength != m_dwFileLength)
		wChanges|= chLength;

	if (!bSnapState)
	{
		m_bReadOnly = bOldReadOnly;
		m_timeModification = timeOldModification;
		m_dwFileLength = dwOldFileLength;
	}

	return wChanges&m_wChangeMask;
}


