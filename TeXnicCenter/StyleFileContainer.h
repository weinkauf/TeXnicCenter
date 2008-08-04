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

#include "StyleFile.h"

/* Container class for all available .sty and .cls files.
 *
 */
class CStyleFileContainer : public CObject
{
public:
	CStyleFileContainer(const CString &basePath = CString(_T(".")));
	virtual ~CStyleFileContainer();

	BOOL AddStyleFile(CStyleFile *sf);
	BOOL LoadFromXML(const CString &file,BOOL addToExisting = FALSE);
	static BOOL ContainsString(const CStringArray *list,const CString &string);
	BOOL SaveAsXML(const CString &path);

	///Merges given commands into this container.
	void Merge(CStyleFileContainer& other);

	/* Returns true, if path <dir> is already in the search path, otherwise FALSE */
	BOOL IsDirInSearchPath(const CString &dir);
	/* Adds the search path <dir> to the search path list */
	void AddSearchPath(const CString &dir);
	/* Removes a path entry from the search path. Returns true, if path was found and removed, otherwise false */
	BOOL RemoveSearchPath(const CString &dir);
	/* Removes all entries from search path */
	void ClearSearchPath();
	/* Searches the given path list for files */
	bool FindStyleFiles();

	/* Returns a list of possible completions to a given string */
	void GetAllPossibleCompletions(const CString& Partial,const CString& docClassName,CUniqueStringList& Result);

	/*	Returns a list of possible completions to a given string. Here the function
	        returns a map with objects instead of string, so that the receiver has more
	        options to display the result.  */
	void GetAllPossibleItems(const CString& Partial,const CString& docClassName,CMapStringToOb& Result);

	/* Sets an event listener to CStyleFileContainer events */
	void SetEventListener(CStyleFileListener *listener);

	static const CString GetFileExt(CString &file);

	const CString& GetLastDir() const;

	const CString& GetLastFile() const;

	const CString& GetLastItem() const;

	int GetNoOfFiles() const;


private:
	void SetupCR(CString &s);
	void ProcessEntityNodes(MsXml::CXMLDOMNode &element,CStyleFile *parent);
	void ProcessPackageNode(MsXml::CXMLDOMNode &element);


	bool FindStyleFilesRecursive(CString dir);

	CString m_LastItem; /* Contains name of last item found */
	CString m_LastFile; /* Contains name of last file found */
	CString m_LastDir; /* Contains name of last dir found */

	CMapStringToOb m_StyleFiles; /* Hash map of available style files */
	int m_NoOfFiles; /* Number of scanned files */

	CStyleFileListener *m_Listener; /* Pointer to event listener (maybe NULL!) */
	CStringArray m_SearchPaths; /* List of paths where to search for .sty and .cls files */
protected:
	void ClearMap();
};


#endif // !defined(AFX_STYLEFILECONTAINER_H__20CE8791_F3F3_4CA7_9FA6_373EAD4AAABF__INCLUDED_)

