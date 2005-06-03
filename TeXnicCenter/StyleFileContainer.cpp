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
#include "StyleFile.h"
#include "StyleFileContainer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStyleFileContainer::CStyleFileContainer(CString &basePath)
{
	AddSearchPath(basePath);
	m_NoOfFiles = 0;
	m_Listener = NULL;
}

CStyleFileContainer::~CStyleFileContainer()
{
	POSITION pos = m_StyleFiles.GetStartPosition();
	while(pos != NULL) {
		CStyleFile *sf;
		CString key;
		m_StyleFiles.GetNextAssoc(pos, key, (CObject*&)sf);
		if (sf != NULL) {
			delete sf;
		}
		m_StyleFiles.RemoveKey(key);
	}
	/* Note: Memory clean up in CStyleFile, we only drop the references */
	m_Commands.RemoveAll();
}

void CStyleFileContainer::FindStyleFiles()
{	
	m_NoOfFiles = 0;
	for (int i = 0; i <= m_SearchPaths.GetUpperBound(); i++) {
		FindStyleFilesRecursive(m_SearchPaths.GetAt(i));
	}
	TRACE("==\nScanned %d files\n", m_NoOfFiles);
}

void CStyleFileContainer::FindStyleFilesRecursive(CString dir) 
{
	CFileFind finder;
	BOOL bWorking = finder.FindFile(dir + "\\*");

	m_LastDir = dir;

	if (m_Listener != NULL) {
		m_Listener->OnDirectoryFound(dir);
	}

	while (bWorking) {
		bWorking = finder.FindNextFile();
		CString name(finder.GetFileName());

		if (finder.IsDirectory() && !finder.IsDots()) {
			FindStyleFilesRecursive(dir + "\\" + name);
			
		} else {
			CString p(finder.GetFilePath());
			//UpdateAllViews(NULL);
			
			CString ext = GetFileExt(name);

			if (ext == "sty" || ext == "cls") {
				m_LastFile = name;
				CStyleFile *sf = new CStyleFile(p);
				sf->SetListener(this);
				sf->ProcessFile();
				if (m_Listener != NULL) {
					m_Listener->OnFileFound(dir);
				}
				m_StyleFiles.SetAt(p, sf);				
				m_NoOfFiles++;
			}
		}
	}

}


void CStyleFileContainer::SetEventListener(CStyleFileListener *listener)
{
	m_Listener = listener;
}

void CStyleFileContainer::ClearSearchPath()
{
	m_SearchPaths.RemoveAll();
	TRACE("Clear search path\n");
}

/* Adds the search path <dir> to the search path list */
void CStyleFileContainer::AddSearchPath(CString &dir)
{
	if (!IsDirInSearchPath(dir)) {
		m_SearchPaths.Add(dir);
		TRACE("Added %s to search path\n", dir);
	}
}

/* Returns true, if path <dir> is already in the search path, otherwise FALSE */
BOOL CStyleFileContainer::IsDirInSearchPath(CString &dir)
{
	for (int i = 0; i <= m_SearchPaths.GetUpperBound(); i++) {
		if (dir == m_SearchPaths.GetAt(i)) {
			return TRUE;
		}
	}
	return FALSE;
}

const CString CStyleFileContainer::GetFileExt(CString &file)
{
	int idx = file.ReverseFind('.');

	if (idx != -1 && file.GetLength() - idx == 4) {
		return CString(file.Mid(idx + 1));
	}
	return CString(""); 
}

void CStyleFileContainer::OnCommandFound(CLaTeXCommand &command) {
	if (m_Listener != NULL) {
		m_LastItem = command.ToString();
		m_Listener->OnCommandFound(command);		
	}
	//TRACE("Adding %s\n", command.ToString());
	m_Commands.SetAt(command.ToString(), &command);
}


/* Returns a list of possible completions to a given string */
const CStringArray* CStyleFileContainer::GetPossibleCompletions(const CString &cmd) {
	CStringArray *tmp = NULL;
	int l;

	l = cmd.GetLength();
	if (0 == l) { /* Nothing to do */
		return NULL;
	}


	POSITION pos = m_Commands.GetStartPosition();
	while(pos != NULL) {
		CLaTeXCommand *lc;
		CString key;
		m_Commands.GetNextAssoc(pos, key, (CObject*&)lc);
		//TRACE("test '%s' = '%s' (%d)\n", key.Left(l), cmd, key.Left(l) == cmd);
		if (lc != NULL && key.GetLength() >= l && key.Left(l) == cmd) {
			if (tmp == NULL) {
				tmp = new CStringArray();
			}
			tmp->Add(key);			 		
			//TRACE("ADD '%s' (now %d items)\n", key, tmp->GetSize());
		}
	}	
	/* TODO: Sort result */
	return tmp;
}

/* Removes a path entry from the search path. Returns true, if path was found and removed, otherwise false */
BOOL CStyleFileContainer::RemoveSearchPath(CString &dir) {
	for (int i = 0; i <= m_SearchPaths.GetUpperBound(); i++) {
		CString s;
		if( ( s = m_SearchPaths.GetAt(i) ) != NULL ) {
		   m_SearchPaths.RemoveAt(i);
		   return TRUE;
		}
	}
	return FALSE;
}

/*
 * $Log$
 */