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
#include "StyleFile.h"
#include "StyleFileContainer.h"
#include "LaTeXCommand.h"
#include "NewCommand.h"
#include "NewEnvironment.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

LPCTSTR const CSF_XML_CONTAINER = _T("lxCollection");
LPCTSTR const CSF_XML_PACKAGE = _T("lxPackage");
LPCTSTR const CSF_XML_COMMAND = _T("lxCommand");
LPCTSTR const CSF_XML_ENVIRONMENT = _T("lxEnvironment");
LPCTSTR const CSF_XML_OPTION = _T("lxOption");
LPCTSTR const CSF_XML_REQPACKAGE = _T("lxReqPackage");
LPCTSTR const CSF_XML_NAME = _T("name");
LPCTSTR const CSF_XML_PATH = _T("path");
LPCTSTR const CSF_XML_PARAMS = _T("parameters");
LPCTSTR const CSF_XML_DESC = _T("desc");
LPCTSTR const CSF_XML_CLASS = _T("class");
LPCTSTR const CSF_XML_EXPAFTER = _T("expafter");
LPCTSTR const CSF_XML_EXPBEFORE = _T("expbefore");
LPCTSTR const CSF_XML_ICONFILE = _T("icon");
LPCTSTR const CSF_XML_ICONINDEX = _T("index");
LPCTSTR const CSF_XML_HIGHCOLORICONFILE = _T("hicoloricon");
LPCTSTR const CSF_XML_ICON_MASK_COLOR = _T("maskcolor");


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStyleFileContainer::CStyleFileContainer(const CString &basePath)
{
	AddSearchPath(basePath);
	m_NoOfFiles = 0;
	m_Listener = NULL;
}

CStyleFileContainer::~CStyleFileContainer()
{
	ClearMap();
}

bool CStyleFileContainer::FindStyleFiles()
{
	m_NoOfFiles = 0;
	ClearMap();
	bool bResult(true);
	for (int i = 0; bResult && i <= m_SearchPaths.GetUpperBound(); i++)
	{
		bResult = FindStyleFilesRecursive(m_SearchPaths.GetAt(i));
	}

	return bResult;
}

bool CStyleFileContainer::FindStyleFilesRecursive(CString dir)
{
	CFileFind finder;
	BOOL bWorking = finder.FindFile(dir + "\\*");

	m_LastDir = dir;

	if (m_Listener) m_Listener->OnDirectoryFound(dir);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		CPathTool Path(finder.GetFilePath());

		//Progress
		if (m_Listener && m_Listener->OnQueryCancel())
		{
			TRACE("** Scanning files cancelled by user break\n");
			ClearMap();
			return false;
		}

		if (finder.IsDirectory() && !finder.IsDots())
		{
			if (!FindStyleFilesRecursive(dir + "\\" + Path.GetFile())) return false;
		}
		else
		{
			CString ext = Path.GetFileExtension();

			if (ext == "sty" || ext == "cls")
			{
				m_LastFile = Path.GetFile();
				std::tr1::shared_ptr<CStyleFile> sf(new CStyleFile(Path.GetPath(), ext == "cls"));
				sf->SetListener(m_Listener);

				if (m_Listener) m_Listener->OnFileFound(Path.GetPath());

				if (!AddStyleFile(sf))
				{
					continue;
				}

				sf->ProcessFile();
			}
		}
	}

	return true;
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
void CStyleFileContainer::AddSearchPath(const CString &dir)
{
	if (!IsDirInSearchPath(dir))
	{
		m_SearchPaths.Add(dir);
		TRACE("Added %s to search path\n", dir);
	}
}

/* Returns true, if path <dir> is already in the search path, otherwise FALSE */
BOOL CStyleFileContainer::IsDirInSearchPath(const CString &dir)
{
	return ContainsString(&m_SearchPaths, dir);
}

BOOL CStyleFileContainer::AddStyleFile(const std::tr1::shared_ptr<CStyleFile>& sf)
{
	StyleMapArg dummy;
	if (!m_StyleFiles.Lookup(sf->GetName(), dummy))
	{
		m_StyleFiles.SetAt(sf->GetName(), sf);
		++m_NoOfFiles;
		return TRUE;
	}
	return FALSE;
}

void CStyleFileContainer::Merge(CStyleFileContainer& /*other*/)
{
	ASSERT(false); //Needs to be implemented
	//NOTE: Be carefull when merging, since CStyleFile really holds the CLaTeXCommand objects.
	//If we simply copy the pointers, then we will crash.
}

/* Returns a list of possible completions to a given string */
void CStyleFileContainer::GetAllPossibleCompletions(const CString& Partial, const CString& docClassName, CUniqueStringList& Result)
{
	SharedObjectMap AllPossibleItems;
	GetAllPossibleItems(Partial, docClassName, AllPossibleItems);

	POSITION pos = AllPossibleItems.GetStartPosition();
	while (pos)
	{
		std::tr1::shared_ptr<CObject> pObj;
		CString key;
		AllPossibleItems.GetNextAssoc(pos, key, pObj);
		SharedLaTeXCommandPtr pLatexCmd = std::tr1::dynamic_pointer_cast<CLaTeXCommand>(pObj);
		if (!pLatexCmd) continue;

		Result.AddTail(pLatexCmd->ToLaTeX());
	}
}

/* Returns a list of possible completions to a given string. Here the function returns objects
   instead of string, so that the receiver has more options to display the result.  */
void CStyleFileContainer::GetAllPossibleItems(const CString& Partial, const CString& docClassName, SharedObjectMap& Result)
{
	//Clear result
	Result.RemoveAll();

	const int CmdLength = Partial.GetLength();
	if (CmdLength == 0) return; //Nothing to do.

	//Go through all style files and get their commands which correspond to the given Partial
	POSITION filepos = m_StyleFiles.GetStartPosition();

	while (filepos)
	{
		//Get style file
		StyleMapArg pSFile;
		CString key;
		m_StyleFiles.GetNextAssoc(filepos, key, pSFile);

		//TODO: If not in usepackage, then skip this one

		//Skip this file, if it does not fit to the given document class
		if (!docClassName.IsEmpty() && pSFile->IsDocClass() && docClassName != pSFile->GetName())
		{
			continue; /* skip commands which are not available in this class */
		}

		//Add the commands
		pSFile->GetPossibleItems(Partial, Result);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// adding all labels from the latex document
	CString str;
	CLaTeXProject* proj = theApp.GetProject();

	if (proj)
	{
		int SearchLength = Partial.GetLength();

		std::tr1::shared_ptr<CStyleFile> dummy
			(new CStyleFile(CString(_T("Labels")), CString(_T("All Labels from document."))));

		//for (int i = 0; i < size; i++) {
		for (StructureItemContainer::const_iterator it = proj->m_aStructureItems.begin(); it != proj->m_aStructureItems.end(); ++it)
		{
			str = it->GetLabel();//proj->m_aStructureItems.ElementAt(i).m_strLabel;

			if (!str.IsEmpty())
			{
				if ((str.GetLength() >= SearchLength) && (str.Left(SearchLength).CompareNoCase(Partial) == 0))
				{
					//Result[str] = CLaTeXCommand(NULL, str, 0);
					Result.SetAt(str, std::tr1::shared_ptr<CObject>(new CLaTeXCommand(dummy, str, 0)));
				}
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//TODO: Sort result
}

/* Removes a path entry from the search path. Returns true, if path was found and removed, otherwise false */
BOOL CStyleFileContainer::RemoveSearchPath(const CString &/*dir*/)
{
	for (int i = 0; i <= m_SearchPaths.GetUpperBound(); i++)
	{
		CString s;
		if ((s = m_SearchPaths.GetAt(i)) != (LPCTSTR)NULL)
		{
			m_SearchPaths.RemoveAt(i);
			return TRUE;
		}
	}
	return FALSE;
}

/* Saves package tree to XML (currently incomplete!) */
BOOL CStyleFileContainer::SaveAsXML(const CString &path)
{
	MsXml::CXMLDOMDocumentEx xmlDoc;
	try
	{
		xmlDoc.Create();
		/*
		xmlDoc.Create(
		        _T("xmlns:txclxp='http://schemas.ToolsCenter.org/TeXnicCenter/LaTeXCommands.xsd'"),
		        CPathTool::Cat(theApp.GetWorkingDir(), _T("LaTeXCommands.xsd")),
		        _T("http://schemas.ToolsCenter.org/TeXnicCenter/LaTeXCommands.xsd"));
		 */

		MsXml::CXMLDOMElement xmlRoot(xmlDoc.CreateElement(CSF_XML_CONTAINER));
		xmlRoot.SetAttribute(_T("xmlns:txclxp"), _T("http://schemas.ToolsCenter.org/TeXnicCenter/LaTeXCommands.xsd"));
		xmlRoot.SetAttribute(_T("version"), 1.0);
		xmlDoc.AppendChild(xmlRoot);

		POSITION pos = m_StyleFiles.GetStartPosition();
		while (pos != NULL)
		{
			StyleMapArg sf;
			CString key;
			m_StyleFiles.GetNextAssoc(pos, key, sf);

			MsXml::CXMLDOMElement xmlPackage(xmlDoc.CreateElement(CSF_XML_PACKAGE));
			xmlPackage.SetAttribute(CSF_XML_NAME, (LPCTSTR)sf->GetName());
			xmlPackage.SetAttribute(CSF_XML_PATH, (LPCTSTR)sf->GetFileName());
			if (sf->GetDescription().GetLength() > 0)
			{
				xmlPackage.SetAttribute(CSF_XML_DESC, (LPCTSTR)sf->GetDescription());
			}

			if (sf->IsDocClass())
			{
				xmlPackage.SetAttribute(CSF_XML_CLASS, "TRUE");
			}

			/* Export commands and environments */
			const SharedObjectMap *cmds = sf->GetCommands();
			POSITION posC = cmds->GetStartPosition();
			while (posC != NULL)
			{
				SharedLaTeXCommandPtr lc;
				std::tr1::shared_ptr<CObject> c;
				CString keyC;
				CString type;

				cmds->GetNextAssoc(posC, keyC, c);
				lc = std::tr1::dynamic_pointer_cast<CLaTeXCommand>(c);

				if (lc->IsKindOf(RUNTIME_CLASS(CNewCommand)))
				{
					type = CSF_XML_COMMAND;
				}
				else
				{
					type = CSF_XML_ENVIRONMENT;
				}
				MsXml::CXMLDOMElement xmlCmd(xmlDoc.CreateElement(type));
				xmlCmd.SetAttribute(CSF_XML_NAME, (LPCTSTR)lc->ToLaTeX(FALSE));

				if (lc->GetNoOfParams() > 0)
				{
					TCHAR buf[5];
					_itot(lc->GetNoOfParams(), buf, 10);
					xmlCmd.SetAttribute(CSF_XML_PARAMS, buf);
				}

				/* just for testing */
				if (lc->GetExpandAfter().GetLength() > 0)
				{
					xmlCmd.SetAttribute(CSF_XML_EXPAFTER, (LPCTSTR)lc->GetExpandAfter());
				}

				if (lc->GetExpandBefore().GetLength() > 0)
				{
					xmlCmd.SetAttribute(CSF_XML_EXPBEFORE, (LPCTSTR)lc->GetExpandBefore());
				}

				xmlPackage.AppendChild(xmlCmd);
			}

			const CStringArray *opt = sf->GetOptions();
			for (int i = 0; i < opt->GetUpperBound(); i++)
			{
				MsXml::CXMLDOMElement xmlCmd(xmlDoc.CreateElement(CSF_XML_OPTION));
				xmlCmd.SetAttribute(CSF_XML_NAME, (LPCTSTR)opt->GetAt(i));
				xmlPackage.AppendChild(xmlCmd);
			}

			opt = sf->GetRequiredPackages();

			for (int i = 0; i < opt->GetUpperBound(); i++)
			{
				MsXml::CXMLDOMElement xmlCmd(xmlDoc.CreateElement(CSF_XML_REQPACKAGE));
				xmlCmd.SetAttribute(CSF_XML_NAME, (LPCTSTR)opt->GetAt(i));
				xmlPackage.AppendChild(xmlCmd);
			}

			xmlRoot.AppendChild(xmlPackage);
		}

		TRACE("Saving XML tree...");

		// write to file
		xmlDoc.SavePretty(path);
		TRACE("done\n");
		return TRUE;
	}
	catch (CComException* pE)
	{
		if (pE->GetDescription().IsEmpty())
		{
			//A generic COM error.
			//For example, MsXML not installed.

			//Inform the user, that he needs to install MsXML.
			AfxMessageBox(STE_XML_INSTALLNEEDED, MB_ICONINFORMATION | MB_OK);
		}
		else
		{
			//Some other error. For example, schema file is missing (*.xsd)
			pE->ReportError(MB_ICONEXCLAMATION | MB_OK);
		}

		pE->Delete();
		return FALSE;
	}

}

void CStyleFileContainer::ClearMap()
{
	Clear();
}

/** Helper function for StringArrays. */
BOOL CStyleFileContainer::ContainsString(const CStringArray *list, const CString &string)
{
	ASSERT(list != NULL);
	for (int i = 0; i < list->GetSize(); i++)
	{
		if (string == list->GetAt(i))
		{
			return TRUE;
		}
	}
	return FALSE;
}

/**
 Loads commands from XML. Took code from profile.cpp
 */
BOOL CStyleFileContainer::LoadFromXML(const CString &file, BOOL addToExisting)
{
	MsXml::CXMLDOMDocumentEx xmlDoc;
	try
	{
		xmlDoc.Create(/*
			_T("xmlns:txcop='http://schemas.ToolsCenter.org/TeXnicCenter/OutputProfiles.xsd'"),
			CPathTool::Cat(theApp.GetWorkingDir(), _T("OutputProfiles.xsd")),
			_T("http://schemas.ToolsCenter.org/TeXnicCenter/OutputProfiles.xsd")*/);

		if (!CPathTool::Exists(file))
		{
			return FALSE;
		}

		if (!xmlDoc.Load((LPCTSTR)file)) AfxThrowComException(S_FALSE);


		MsXml::CXMLDOMElement xmlPackages(xmlDoc.GetDocumentElement());

		MsXml::CXMLDOMNodeList nodes = xmlPackages.GetChildNodes();
		const long lPackages = nodes.GetLength();
		//TRACE("Found %d package nodes in %s\n", lPackages, file);

		if (lPackages > 0 && !addToExisting)
		{
			ClearMap(); /* drop existing commands */
		}

		for (long i = 0; i < lPackages; ++i)   /* walk through child nodes */
		{
			ProcessPackageNode(nodes.GetItem(i));
		}
		return TRUE;
	}
	catch (CComException *pE)
	{
		// TODO: The following error is reported multiple times which
		// is very annoying. The catch handler should be set by the most outer caller.
		bool bReportedError(false);

		//Ask MsXML for error description
		try
		{
			if (xmlDoc && xmlDoc.GetParseError() && xmlDoc.GetParseError().GetErrorCode() != 0)
			{
				MsXml::CXMLDOMParseError xmlError(xmlDoc.GetParseError());
				CString strErrorMsg;
				strErrorMsg.Format(STE_XML_PARSE_ERROR,
				                   xmlError.GetErrorCode(), xmlError.GetReason(),
				                   xmlError.GetUrl(), xmlError.GetLine(), xmlError.GetLinepos(),
				                   xmlError.GetSrcText());

				AfxMessageBox(strErrorMsg, MB_ICONEXCLAMATION | MB_OK);
				bReportedError = true;
			}
		}
		catch (CComException* pENew)
		{
			pENew->Delete();
		}


		//Report Error, if MsXML did not respond
		if (!bReportedError)
		{
			if (pE->GetDescription().IsEmpty())
			{
				//A generic COM error.
				//For example, MsXML not installed.

				//Inform the user, that he needs to install MsXML.
				AfxMessageBox(STE_XML_INSTALLNEEDED, MB_ICONINFORMATION | MB_OK);
			}
			else
			{
				//Some other error. For example, schema file is missing (*.xsd)
				pE->ReportError(MB_ICONEXCLAMATION | MB_OK);
			}
		}

		pE->Delete();
		return FALSE;
	}
}

void CStyleFileContainer::ProcessPackageNode( const MsXml::CXMLDOMNode &element )
{
	/* fetch attributes */
	MsXml::CXMLDOMNamedNodeMap attr = element.GetAttributes();
	CString nameVal, descVal;
	bool isClass = false;

	for (int j = 0; j < attr.GetLength(); j++)
	{
		MsXml::CXMLDOMNode a = attr.GetItem(j);
		CString s = a.GetNodeName();
		if (s == CSF_XML_NAME)
		{
			nameVal = a.GetText();
		}
		else if (s == CSF_XML_DESC)
		{
			descVal = a.GetText();
		}
		else if (s == CSF_XML_CLASS)
		{
			isClass = TRUE; /* existing node is sufficient */
		}
	}

	/* Create style file instance and insert it into the hash map */
	std::tr1::shared_ptr<CStyleFile> sf(new CStyleFile(nameVal, descVal, isClass));

	sf->SetListener(m_Listener);

	if (!AddStyleFile(sf))
	{
		TRACE("WARNING: Unable to add file %s, seems to be duplicate\n", sf->GetName());
		return;
	}

	/* Process children */
	MsXml::CXMLDOMNodeList nodes = element.GetChildNodes();
	long n = nodes.GetLength();

	for (long i = 0; i < n; ++i)   /* walk through child nodes */
	{
		ProcessEntityNodes(nodes.GetItem(i), sf);
	}
}

void CStyleFileContainer::ProcessEntityNodes( const MsXml::CXMLDOMNode &element, std::tr1::shared_ptr<CStyleFile>& parent )
{
	/* fetch attributes */
	MsXml::CXMLDOMNamedNodeMap attr = element.GetAttributes();
	CString nameVal, descVal, beforeVal, afterVal, iconFile, high_color_icon_file;

	BOOL hasAfterVal = FALSE, hasBeforeVal = FALSE, hasFile = FALSE, hasIndex = FALSE;
	int nOfParams = 0, iconIndex = 0;
	COLORREF mask; bool has_mask = false;
	
	UNUSED_ALWAYS(mask);

	for (int j = 0; j < attr.GetLength(); j++)
	{
		MsXml::CXMLDOMNode a = attr.GetItem(j);
		CString s = a.GetNodeName();

		if (s == CSF_XML_NAME)
		{
			nameVal = a.GetText();
		}
		else if (s == CSF_XML_DESC)
		{
			descVal = a.GetText();
		}
		else if (s == CSF_XML_PARAMS)
		{
			nOfParams = _ttoi(a.GetText());
		}
		else if (s == CSF_XML_EXPAFTER)
		{
			hasAfterVal = TRUE;
			afterVal = a.GetText();
			SetupCR(afterVal);
		}
		else if (s == CSF_XML_EXPBEFORE)
		{
			hasBeforeVal = TRUE;
			beforeVal = a.GetText();
			SetupCR(beforeVal);
		}
		else if (s == CSF_XML_ICONINDEX)
		{
			hasIndex = TRUE;
			iconIndex = _ttoi(a.GetText());
		}
		else if (s == CSF_XML_ICONFILE)
		{
			hasFile = TRUE;
			iconFile = theApp.GetWorkingDir() + _T("\\packages\\") + a.GetText();
		}
		else if (s == CSF_XML_HIGHCOLORICONFILE)
			high_color_icon_file = theApp.GetWorkingDir() + _T("\\packages\\") + a.GetText();
		else if (s == CSF_XML_ICON_MASK_COLOR)
		{
			has_mask = true;
			//mask =
		}
	}

	/* Add element to style file */
	SharedLaTeXCommandPtr lc;

	if (element.GetNodeName() == CSF_XML_COMMAND)
	{
		lc = parent->AddCommand(nameVal, nOfParams, descVal);
	}
	else if (element.GetNodeName() == CSF_XML_ENVIRONMENT)
	{
		lc = parent->AddEnvironment(nameVal, nOfParams, descVal);
	}

	// assign missing attributes
	if (lc != NULL)
	{
		if (hasAfterVal)
		{
			lc->SetExpandAfter(afterVal);
		}

		if (hasBeforeVal)
		{
			lc->SetExpandBefore(beforeVal);
		}

		if (hasFile)
		{
			lc->SetIconFileName(iconFile);
			lc->SetIconIndex(0); // set default
		}

		if (hasIndex)
		{
			lc->SetIconIndex(iconIndex);
		}

		if (!high_color_icon_file.IsEmpty())
			lc->SetHighColorIconFileName(high_color_icon_file);
	}
}

/*
        This is needed, because CrystalEdit needs a \r\n to create a line feed, but XML parsers
        turns every &#xD;&#xA; sequence into an single '&#xA;'.
        (see http://www.sql-und-xml.de/xml-lernen/xml-version-1.1-unicode.html for details)
 */

void CStyleFileContainer::SetupCR(CString &s)
{
	s.Replace(_T("\n"), _T("\r\n"));
}

const CString& CStyleFileContainer::GetLastDir() const
{
	return m_LastDir;
}

const CString& CStyleFileContainer::GetLastFile() const
{
	return m_LastFile;
}

const CString& CStyleFileContainer::GetLastItem() const
{
	return m_LastItem;
}

int CStyleFileContainer::GetNoOfFiles() const
{
	return m_NoOfFiles;
}