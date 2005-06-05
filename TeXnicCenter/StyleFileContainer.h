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
#if !defined(AFX_STYLEFILECONTAINER_H__20CE8791_F3F3_4CA7_9FA6_373EAD4AAABF__INCLUDED_)
#define AFX_STYLEFILECONTAINER_H__20CE8791_F3F3_4CA7_9FA6_373EAD4AAABF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stylefile.h"

#define CSF_XML_CONTAINER	_T("lxCollection")
#define CSF_XML_PACKAGE		_T("lxPackage")
#define CSF_XML_COMMAND		_T("lxCommand")
#define CSF_XML_ENVIRONMENT	_T("lxEnvironment")
#define CSF_XML_OPTION		_T("lxOption")
#define CSF_XML_REQPACKAGE	_T("lxReqPackage")
#define CSF_XML_NAME		_T("name")
#define CSF_XML_PATH		_T("path")
#define CSF_XML_PARAMS		_T("parameters")


/* Container class for all available .sty and .cls files.
 *
 */
class CStyleFileContainer : public CObject, virtual public CLaTeXCommandListener
{
public:	
	void LoadFromXML(const CString &file, BOOL addToExisting);
	static BOOL ContainsString(const CStringArray *list, const CString &string);
	BOOL SaveAsXML(CString &path);
	CStyleFileContainer(CString &basePath=CString("."));
	virtual ~CStyleFileContainer();

	/* Returns true, if path <dir> is already in the search path, otherwise FALSE */
	BOOL IsDirInSearchPath(CString &dir);
	/* Adds the search path <dir> to the search path list */
	void AddSearchPath(CString &dir);
	/* Removes a path entry from the search path. Returns true, if path was found and removed, otherwise false */
	BOOL RemoveSearchPath(CString &dir);
	/* Removes all entries from search path */
	void ClearSearchPath();	
	/* Searches the given path list for files */
	void FindStyleFiles();

	/* Returns a list of possible completions to a given string */
	const CStringArray* GetPossibleCompletions(const CString &cmd);

	/* Sets an event listener to CStyleFileContainer events */
	void SetEventListener(CStyleFileListener *listener);
	
	static const CString GetFileExt(CString &file);
	CString GetLastDir() {return m_LastDir;};
	CString GetLastFile() {return m_LastFile;};
	CString GetLastItem() {return m_LastItem;};
	int GetNoOfFiles() {return m_NoOfFiles;};

	virtual void OnCommandFound(CLaTeXCommand &command);

private:
	
	void FindStyleFilesRecursive(CString dir);

	CString			m_LastItem;		/* Contains name of last item found */
	CString			m_LastFile;		/* Contains name of last file found */
	CString			m_LastDir;		/* Contains name of last dir found */
	
	CMapStringToOb 	m_StyleFiles;	/* Hash map of available style files */
	CMapStringToOb 	m_Commands;		/* Hash map of all available commands */
	int				m_NoOfFiles;	/* Number of scanned files */

	CStyleFileListener *m_Listener;	/* Pointer to event listener (maybe NULL!) */
	CStringArray	m_SearchPaths;	/* List of paths where to search for .sty and .cls files */
protected:
	void ClearMap();
};

#endif // !defined(AFX_STYLEFILECONTAINER_H__20CE8791_F3F3_4CA7_9FA6_373EAD4AAABF__INCLUDED_)

/*
 * $Log$
 * Revision 1.3  2005/06/04 10:39:12  owieland
 * Added option and required package support
 *
 * Revision 1.2  2005/06/03 22:29:20  owieland
 * XML Export
 *
 * Revision 1.1  2005/06/03 20:29:43  owieland
 * Initial checkin of package and class parser
 *
 */