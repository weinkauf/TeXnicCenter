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
#include "TeXnicCenter.h"
#include "Placeholder.h"

#include "FileClean.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//////////////////////////////////////////////////////////////////////
// CFileCleanItem Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

///Standard Constructor
CFileCleanItem::CFileCleanItem()
{
	strDescription = "";
	strPattern = "";
	tFileHandling = none;
	bRecursive = true;
}


///Element Constructor
CFileCleanItem::CFileCleanItem(const LPCTSTR argDescription, const LPCTSTR argPattern,
															 const tagFileHandling argFileHandling,
															 const bool argRecursive)
{
	strDescription = argDescription;
	strPattern = argPattern;
	tFileHandling = argFileHandling;
	bRecursive = argRecursive;
}


CFileCleanItem::~CFileCleanItem()
{

}


bool CFileCleanItem::PatternIsValid()
{
	//Pattern is not allowed to be empty
	if (strPattern.IsEmpty()) return false;

	//Check, what does the pattern contain
	bool bContainsWildcards = AfxContainsWildcards(strPattern);
	bool bContainsPlaceholders = AfxContainsPlaceholders(strPattern);
	bool bContainsPlaceholderSets = AfxContainsPlaceholderSets(strPattern);

	//Pattern is not allowed to contain {%} and {*,?}
	//Pattern is not allowed to contain {$} and {*,?}
	//Pattern is not allowed to contain {%} and {$}
	if ( (bContainsWildcards && bContainsPlaceholders)
		|| (bContainsWildcards && bContainsPlaceholderSets)
		|| (bContainsPlaceholders && bContainsPlaceholderSets) )
		return false;

	//Pattern is not allowed to contain Placeholder sets and to be recursive
	if (bContainsPlaceholderSets && bRecursive)
		return false;

	return true;
}


bool CFileCleanItem::Expand(CLatexProject* argpProject, LPCTSTR lpszCurrentPath, CUniqueStringList* pSList)
{
	//Safety
	ASSERT(argpProject);
	if (!argpProject) return false;
	ASSERT(pSList);
	if (!pSList) return false;

	//Valid Pattern?
	if (!PatternIsValid()) return false;

	//Expand the Placeholders
	if (AfxContainsPlaceholders(strPattern))
	{
		CString strToAdd = AfxExpandPlaceholders(strPattern, argpProject->GetMainPath(), lpszCurrentPath, 0, NULL, false);
		if (!strToAdd.IsEmpty())
		{
			//We only want to have absolute paths, because the user must be able to see
			// which file exactly will be deleted.
			//And if the path is relative it is assumed to start in the working dir.
			if ((CPathTool::GetDrive(strToAdd)).IsEmpty())
			{
				strToAdd = argpProject->GetFilePath(strToAdd);
			}

			if (CPathTool::Exists(strToAdd))
				pSList->AddHead(strToAdd);
		}
		return true;
	}

	//Expand the Placeholder sets
	if (AfxContainsPlaceholderSets(strPattern))
	{
		CPlaceholderSets ps(argpProject);
		//To get absolute paths only, we just need to ignore
		// the options of the Set-Placeholder.
		ps.ExpandAllSets(strPattern, pSList, true);
		return true;
	}

	//Expand the Wildcards or just take the file in strPattern
	AfxExpandWildcard(strPattern, bRecursive, argpProject->GetWorkingDir(), pSList);

	return true;
}

bool CFileCleanItem::SerializeToRegistry(CBCGRegistryEx &reg) const
{
	if (tFileHandling == protectbydefault)
	{
		ASSERT(false); //Default Items do not go to the registry
		return false;
	}

	reg.Write(_T("Description"), strDescription);
	reg.Write(_T("Pattern"), strPattern);
	reg.Write(_T("Recursive"), bRecursive);
	reg.Write(_T("Handling"), (int)tFileHandling);

	return true;
}

bool CFileCleanItem::SerializeFromRegistry(CBCGRegistryEx &reg)
{
	reg.Read(_T("Description"), strDescription);
	reg.Read(_T("Pattern"), strPattern);
	reg.Read(_T("Recursive"), (int&)bRecursive);

	//NOTE: Casting an integer to an enum-type is legal, but results are undefined. Thats why:
	int tempFH;
	reg.Read(_T("Handling"), tempFH);
	switch (tempFH)
	{
		case (int)(tagFileHandling::clean):
			{
				tFileHandling = clean;
				break;
			}

		case (int)(tagFileHandling::protect):
			{
				tFileHandling = protect;
				break;
			}

		case (int)(tagFileHandling::protectbydefault):
			{
				ASSERT(false); //Should not happen; Default Items do not go to the registry
				break;
			}

		default:
			tFileHandling = none;
	}


	return true;
}

//////////////////////////////////////////////////////////////////////
// CFileCleanItemArray
//
// An Array to hold the Definitions (Items) for file cleaning.
// Adds 'protectedbydefault'-Values at creation.
//
// @author Tino Weinkauf
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileCleanItemArray::CFileCleanItemArray()
{

}

CFileCleanItemArray::~CFileCleanItemArray()
{
}


void CFileCleanItemArray::AddDefaultItems()
{
	CString strDescr;

	//Add protection for project files
	strDescr.LoadString(STE_PLACEHOLDERSETS_APF);
	InsertSorted( CFileCleanItem(strDescr, "$APF", CFileCleanItem::protectbydefault, false), true, false );

	//Add protection for currently opened files
	strDescr.LoadString(STE_PLACEHOLDERSETS_COF);
	InsertSorted( CFileCleanItem(strDescr, "$COF", CFileCleanItem::protectbydefault, false), true, false );
}


bool CFileCleanItemArray::SerializeToRegistry(LPCTSTR strStartSection) const
{
	CBCGRegistryEx reg(false, false);
	CString strKey(CPathTool::Cat(strStartSection, _T("Patterns")));
	reg.DeleteKey(strKey);
	reg.CreateKey(strKey);

	//Iterate and Write
	int nsize = GetSize();
	int i;
	for(i=0;i<nsize;i++)
	{
		CFileCleanItem* pTemp = &GetAt(i);

		//Is it a default item? If yes, continue, because they do not go to the registry
		if (pTemp->tFileHandling == CFileCleanItem::protectbydefault)
			continue;

		//Create a Title
		CString strTitle;
		strTitle.Format("%s_%d", pTemp->strDescription, pTemp->tFileHandling);

		//Write the item
		reg.PushKey();
		reg.CreateKey(strTitle);
		pTemp->SerializeToRegistry(reg);
		reg.PopKey();
	}

	return true;
}


bool CFileCleanItemArray::SerializeFromRegistry(LPCTSTR strStartSection)
{
	//Remove everything and add the defaults
	RemoveAll();
	AddDefaultItems();

	CBCGRegistryEx reg(false, true);
	CString strKey(CPathTool::Cat(strStartSection, _T("Patterns")));

	//Does the key exist?
	if (!reg.Open(strKey))
	{
		//Key does not exist; so I assume, that we need some standards
		InsertSorted(CFileCleanItem("Postscript", "%bm.ps", CFileCleanItem::none, false), true, true);
		InsertSorted(CFileCleanItem("DVI", "%bm.dvi", CFileCleanItem::none, false), true, true);
		InsertSorted(CFileCleanItem("PDF", "%bm.pdf", CFileCleanItem::none, false), true, true);
		InsertSorted(CFileCleanItem("bak", "*.bak", CFileCleanItem::none, true), true, true);

		InsertSorted(CFileCleanItem("aux", "*.aux", CFileCleanItem::clean, true), true, true);
		InsertSorted(CFileCleanItem("bbl", "*.bbl", CFileCleanItem::clean, true), true, true);
		InsertSorted(CFileCleanItem("blg", "*.blg", CFileCleanItem::clean, true), true, true);
		InsertSorted(CFileCleanItem("lof", "*.lof", CFileCleanItem::clean, true), true, true);
		InsertSorted(CFileCleanItem("log", "*.log", CFileCleanItem::clean, true), true, true);
		InsertSorted(CFileCleanItem("lot", "*.lot", CFileCleanItem::clean, true), true, true);
		InsertSorted(CFileCleanItem("mpx", "*.mpx", CFileCleanItem::clean, true), true, true);
		InsertSorted(CFileCleanItem("idx", "*.idx", CFileCleanItem::clean, true), true, true);
		InsertSorted(CFileCleanItem("ilg", "*.ilg", CFileCleanItem::clean, true), true, true);
		InsertSorted(CFileCleanItem("ind", "*.ind", CFileCleanItem::clean, true), true, true);
		InsertSorted(CFileCleanItem("out", "*.out", CFileCleanItem::clean, true), true, true);
		InsertSorted(CFileCleanItem("toc", "*.toc", CFileCleanItem::clean, true), true, true);

		InsertSorted(CFileCleanItem("bmp", "*.bmp", CFileCleanItem::protect, true), true, true);
		InsertSorted(CFileCleanItem("eps", "*.eps", CFileCleanItem::protect, true), true, true);
		InsertSorted(CFileCleanItem("jpeg", "*.jpeg", CFileCleanItem::protect, true), true, true);
		InsertSorted(CFileCleanItem("jpg", "*.jpg", CFileCleanItem::protect, true), true, true);
		InsertSorted(CFileCleanItem("png", "*.png", CFileCleanItem::protect, true), true, true);
		InsertSorted(CFileCleanItem("tex", "*.tex", CFileCleanItem::protect, true), true, true);
		InsertSorted(CFileCleanItem("bib", "*.bib", CFileCleanItem::protect, true), true, true);
		InsertSorted(CFileCleanItem("sty", "*.sty", CFileCleanItem::protect, true), true, true);
		InsertSorted(CFileCleanItem("bst", "*.bst", CFileCleanItem::protect, true), true, true);
		InsertSorted(CFileCleanItem("dtx", "*.dtx", CFileCleanItem::protect, true), true, true);
		InsertSorted(CFileCleanItem("ins", "*.ins", CFileCleanItem::protect, true), true, true);

		//Well, lets say everything was fine
		return true;
	}

	CFileCleanItem tempItem;

	//Read the Titles
	CStringArray astrKeys;
	reg.ReadSubKeys(astrKeys);
	reg.PushKey();

	int i;
	for (i=0;i<astrKeys.GetSize();i++)
	{
		reg.TopKey();
		reg.Open(astrKeys[i]);

		//Get the Infos and Insert
		tempItem.SerializeFromRegistry(reg);
		InsertSorted(tempItem, true, true);
	}

	reg.PopKey();

	return true;
}


//////////////////////////////////////////////////////////////////////
// CFileClean
//
// A Class to delete (clean) Files defined by Wildcards,
// Placeholders and Placeholder sets.
// Supports Protection of Files.
//
// @author Tino Weinkauf
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileClean::CFileClean()
{

}

CFileClean::~CFileClean()
{

}

bool CFileClean::Initialize(const CFileCleanItemArray& ItemArray)
{
	//Retrieve Information
	CLatexProject* pProject = theApp.GetProject();
	if (!pProject) return false; //No Project = nothing to clean

	CString	strCurrentPath = "";
	CLatexEdit* pEdit = theApp.GetActiveEditView();
	if (pEdit)
	{
		strCurrentPath = pEdit->GetDocument()->GetPathName();
	}


	//Init the lists
	m_FilesToClean.RemoveAll();
	m_FilesToProtect.RemoveAll();


	//Iterate through the pattern array
	int i;
	int nsize = ItemArray.GetSize();
	for(i=0;i<nsize;i++)
	{
		//Filling the List of files to be deleted
		if (ItemArray[i].tFileHandling == CFileCleanItem::clean)
		{
			ItemArray[i].Expand(pProject,
													strCurrentPath.IsEmpty() ? (LPCTSTR)NULL : strCurrentPath,
													&m_FilesToClean);
		}

		//Filling the List of files to be protected
		if ( (ItemArray[i].tFileHandling == CFileCleanItem::protect)
			|| (ItemArray[i].tFileHandling == CFileCleanItem::protectbydefault) )
		{
			ItemArray[i].Expand(pProject,
													strCurrentPath.IsEmpty() ? (LPCTSTR)NULL : strCurrentPath,
													&m_FilesToProtect);
		}
	}

	return true;
}

bool CFileClean::CleanFiles()
{
	//Protect!!!
	m_FilesToClean.SubtractList(m_FilesToProtect);

	try
	{
		POSITION pos = m_FilesToClean.GetHeadPosition();
		while(pos)
		{
			//If this fails, there is something wrong in the code
			//The List shall only contain files, which are present
			//And they should be built up fast enough, so that
			// another app is not able to delete some file meanwhile.
			ASSERT( CPathTool::Exists(m_FilesToClean.GetAt(pos)) );

			if ( CPathTool::Exists(m_FilesToClean.GetAt(pos)) )
				CFile::Remove(m_FilesToClean.GetAt(pos));

			m_FilesToClean.GetNext(pos);
		}
	}
	catch	(CFileException* e)
	{
		e->Delete();
		return false;
	}

	return true;
}

CString CFileClean::GetFilesToClean(bool bProtectionOn) const
{
	if (bProtectionOn)
	{
		CUniqueStringList strList;

		POSITION pos = m_FilesToClean.GetHeadPosition();
		while(pos)
		{
			strList.AddHead(m_FilesToClean.GetNext(pos));
		}

		strList.SubtractList(m_FilesToProtect);
		return strList.FlattenList(NULL, NULL, "\r\n", true);
	}
	else
	{
		return m_FilesToClean.FlattenList(NULL, NULL, "\r\n", true);
	}
}

CString CFileClean::GetFilesToProtect() const
{
	return m_FilesToProtect.FlattenList(NULL, NULL, "\r\n", true);
}
