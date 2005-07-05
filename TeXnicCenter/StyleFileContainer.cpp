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

/*
 * $Log$
 * Revision 1.13  2005/06/24 11:41:15  owieland
 * Bugfix: Pass file path to listener instead of directory
 *
 * Revision 1.12  2005/06/23 22:17:13  owieland
 * - Compare words case insensitive
 * - Allow empty insertBefore/After values
 *
 * Revision 1.11  2005/06/11 12:50:57  owieland
 * - Load also expAfter/expBefore attributes from packages.xml
 * - AddCommand/AddEnvironment now return pointers to created object
 *
 * Revision 1.10  2005/06/10 15:31:38  owieland
 * Moved xml constants to .cpp file
 *
 * Revision 1.9  2005/06/09 17:09:59  owieland
 * + Revised architecture (moved autocmpl-handling to listbox)
 * + Hilight commands if they are from a class (unsatisfying yet)
 * + Several bugfixes
 *
 * Revision 1.8  2005/06/09 12:09:59  owieland
 * + Consider ProvidesXXX commands for package/class description
 * + Avoid duplicate option entries
 * + Export description of packages
 * + Consider number of parameters on auto completion
 *
 * Revision 1.7  2005/06/07 23:14:23  owieland
 * + Load commands from packages.xml
 * + Fixed position of the auto complete listbox / beautified content
 * + Fixed some bugs
 *
 * Revision 1.6  2005/06/05 19:45:18  owieland
 * - Beautified auto complete listbox (fonts, position).
 * - Bugfix: Window did not appeared after second invocation
 * - Beep, if no completion was found
 *
 * Revision 1.5  2005/06/05 16:42:42  owieland
 * Extended user interface (prepare for loading the package rep from XML)
 *
 * Revision 1.4  2005/06/04 13:09:35  owieland
 * Added rudimentary auto-complete support (Tools->Complete command).
 *
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

#include "stdafx.h"
#include "TeXnicCenter.h"
#include "StyleFile.h"
#include "StyleFileContainer.h"
#include "LaTeXCommand.h"
#include "NewCommand.h"
#include "NewEnvironment.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define CSF_XML_CONTAINER	_T("lxCollection")
#define CSF_XML_PACKAGE		_T("lxPackage")
#define CSF_XML_COMMAND		_T("lxCommand")
#define CSF_XML_ENVIRONMENT	_T("lxEnvironment")
#define CSF_XML_OPTION		_T("lxOption")
#define CSF_XML_REQPACKAGE	_T("lxReqPackage")
#define CSF_XML_NAME		_T("name")
#define CSF_XML_PATH		_T("path")
#define CSF_XML_PARAMS		_T("parameters")
#define CSF_XML_DESC		_T("desc")
#define CSF_XML_CLASS		_T("class")
#define CSF_XML_EXPAFTER	_T("expafter")
#define CSF_XML_EXPBEFORE	_T("expbefore")


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
	ClearMap();
}

void CStyleFileContainer::FindStyleFiles()
{	
	m_NoOfFiles = 0;
	ClearMap();
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
		if (m_Listener->OnQueryCancel()) {
			TRACE("** Scanning files cancelled by user break\n"); 
			ClearMap();
			return;
		}
	}

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
				CStyleFile *sf = new CStyleFile(p, ext == "cls");
				sf->SetListener(this);				
				
				if (m_Listener != NULL) {
					m_Listener->OnFileFound(p);
				}
				if (!AddStyleFile(sf)) {
					delete sf;
				} else {
					sf->ProcessFile();
				}
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
	return ContainsString(&m_SearchPaths, dir);
}

const CString CStyleFileContainer::GetFileExt(CString &file)
{
	int idx = file.ReverseFind('.');

	if (idx != -1 && file.GetLength() - idx == 4) {
		return CString(file.Mid(idx + 1));
	}
	return CString(""); 
}

BOOL CStyleFileContainer::AddStyleFile(CStyleFile *sf)
{	
	CObject *dummy;
	if (!m_StyleFiles.Lookup(sf->GetName(), dummy)) {
		m_StyleFiles.SetAt(sf->GetName(), sf);				
		m_NoOfFiles++;
		return TRUE;
	}
	return FALSE;
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
		if (lc != NULL && key.GetLength() >= l && stricmp(key.Left(l), cmd) == 0) {
			if (tmp == NULL) {
				tmp = new CStringArray();
			}

			if (!ContainsString(tmp, lc->ToLaTeX())) {
				tmp->Add(lc->ToLaTeX());			 		
			}
			//TRACE("ADD '%s' (now %d items)\n", key, tmp->GetSize());
		}
	}	
	/* TODO: Sort result */
	return tmp;
}

/* Returns a list of possible completions to a given string. Here the function retuns objects
   instead of string, so that the receiver has more options to display the result.  */
const CMapStringToOb* CStyleFileContainer::GetPossibleItems(const CString &cmd, const CString &docClass) {
	CMapStringToOb *tmp = NULL;
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

		if (!docClass.IsEmpty() && 
			lc->GetParent()->IsDocClass() &&  
			docClass != lc->GetParent()->GetName()) {

			continue; /* skip commands which are not available in this class */
		}

		if (lc != NULL && key.GetLength() >= l && stricmp(key.Left(l), cmd) == 0) {
			if (tmp == NULL) {
				tmp = new CMapStringToOb();
			}

			if (!tmp->Lookup(key, (CObject*&)lc) || lc->GetParent()->IsDocClass()) {
				tmp->SetAt(lc->ToLaTeX(), lc);			 		
			}
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

/* Saves package tree to XML (currently incomplete!) */
BOOL CStyleFileContainer::SaveAsXML(CString &path)
{
	MsXml::CXMLDOMDocumentEx	xmlDoc;
	try
	{
		xmlDoc.Create();
		/*
		xmlDoc.Create(
			_T("xmlns:txclxp='http://schemas.ToolsCenter.org/TeXnicCenter/LaTeXCommands.xsd'"), 
			CPathTool::Cat(theApp.GetWorkingDir(), _T("LaTeXCommands.xsd")),
			_T("http://schemas.ToolsCenter.org/TeXnicCenter/LaTeXCommands.xsd"));
		*/
		
		MsXml::CXMLDOMElement	xmlRoot(xmlDoc.CreateElement(CSF_XML_CONTAINER));
		xmlRoot.SetAttribute(_T("xmlns:txclxp"), _T("http://schemas.ToolsCenter.org/TeXnicCenter/LaTeXCommands.xsd"));
		xmlRoot.SetAttribute(_T("version"), 1.0);
		xmlDoc.AppendChild(xmlRoot);
		
		POSITION pos = m_StyleFiles.GetStartPosition();
		while(pos != NULL) {
			CStyleFile *sf;
			CString key;
			m_StyleFiles.GetNextAssoc(pos, key, (CObject*&)sf);
			
			MsXml::CXMLDOMElement	xmlPackage(xmlDoc.CreateElement(CSF_XML_PACKAGE));
			xmlPackage.SetAttribute(CSF_XML_NAME, (LPCTSTR)sf->GetName());
			xmlPackage.SetAttribute(CSF_XML_PATH, (LPCTSTR)sf->GetFilename());
			if (sf->GetDescription().GetLength() > 0) {
				xmlPackage.SetAttribute(CSF_XML_DESC, (LPCTSTR)sf->GetDescription());
			}

			if (sf->IsDocClass()) {
				xmlPackage.SetAttribute(CSF_XML_CLASS, "TRUE");
			}
			
			/* Export commands and environments */
			const CMapStringToOb *cmds = sf->GetCommands();
			POSITION posC = cmds->GetStartPosition();
			while(posC != NULL) {
				CLaTeXCommand *lc;
				CString keyC;
				CString type;

				cmds->GetNextAssoc(posC, keyC, (CObject*&)lc);
								
				if (lc->IsKindOf( RUNTIME_CLASS( CNewCommand))) {
					type = CSF_XML_COMMAND;
				} else {
					type = CSF_XML_ENVIRONMENT;
				}
				MsXml::CXMLDOMElement	xmlCmd(xmlDoc.CreateElement(type));
				xmlCmd.SetAttribute(CSF_XML_NAME, (LPCTSTR)lc->ToLaTeX(FALSE));

				if (lc->GetNoOfParams() > 0) {
					TCHAR buf[5];
					itoa(lc->GetNoOfParams(), buf, 10);
					xmlCmd.SetAttribute(CSF_XML_PARAMS, buf);
				}

				/* just for testing */
				if (lc->GetExpandAfter().GetLength() > 0) {
					xmlCmd.SetAttribute(CSF_XML_EXPAFTER, (LPCTSTR)lc->GetExpandAfter());
				}

				if (lc->GetExpandBefore().GetLength() > 0) {
					xmlCmd.SetAttribute(CSF_XML_EXPBEFORE, (LPCTSTR)lc->GetExpandBefore());
				}
				
				xmlPackage.AppendChild(xmlCmd);
			}

			const CStringArray *opt = sf->GetOptions();
			for(int i=0; i < opt->GetUpperBound(); i++) {
				MsXml::CXMLDOMElement	xmlCmd(xmlDoc.CreateElement(CSF_XML_OPTION));
				xmlCmd.SetAttribute(CSF_XML_NAME, (LPCTSTR)opt->GetAt(i));
				xmlPackage.AppendChild(xmlCmd);
			}

			opt = sf->GetRequiredPackages();
			for(i=0; i < opt->GetUpperBound(); i++) {
				MsXml::CXMLDOMElement	xmlCmd(xmlDoc.CreateElement(CSF_XML_REQPACKAGE));
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
		if (pE->GetDescription() == _T(""))
		{
			//A generic COM error.
			//For example, MsXML not installed.

			//Inform the user, that he needs to install MsXML.
			AfxMessageBox(STE_XML_INSTALLNEEDED, MB_ICONINFORMATION|MB_OK);
		}
		else
		{
			//Some other error. For example, schema file is missing (*.xsd)
			pE->ReportError(MB_ICONEXCLAMATION|MB_OK);
		}

		pE->Delete();
		return FALSE;
	}

}

void CStyleFileContainer::ClearMap()
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

/** Helper function for StringArrays. */
BOOL CStyleFileContainer::ContainsString(const CStringArray *list, const CString &string)
{
	ASSERT(list != NULL);
	for(int i=0; i < list->GetSize(); i++) {
		if (string == list->GetAt(i)) {
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
	MsXml::CXMLDOMDocumentEx	xmlDoc;
	try
	{
		xmlDoc.Create(/*
			_T("xmlns:txcop='http://schemas.ToolsCenter.org/TeXnicCenter/OutputProfiles.xsd'"), 
			CPathTool::Cat(theApp.GetWorkingDir(), _T("OutputProfiles.xsd")),
			_T("http://schemas.ToolsCenter.org/TeXnicCenter/OutputProfiles.xsd")*/);

		if (!CPathTool::Exists(file)) {
			return FALSE;
		}

		if (!xmlDoc.Load((LPCTSTR)file)) AfxThrowComException(S_FALSE);

				
		MsXml::CXMLDOMElement xmlPackages(xmlDoc.GetDocumentElement());

		MsXml::CXMLDOMNodeList nodes = xmlPackages.GetChildNodes();
		const long	lPackages = nodes.GetLength();
		//TRACE("Found %d package nodes in %s\n", lPackages, file);

		if (lPackages > 0 && !addToExisting) {
			ClearMap(); /* drop existing commands */
		}

		for (long i = 0; i < lPackages; ++i) {	/* walk through child nodes */		 
			ProcessPackageNode(nodes.GetItem(i));
		}
		TRACE("** Loaded %d commands\n", m_Commands.GetCount());
		return TRUE;
	}
	catch (CComException *pE)
	{
		bool bReportedError(false);

		//Ask MsXML for error description
		try
		{
			if (xmlDoc && xmlDoc.GetParseError() && xmlDoc.GetParseError().GetErrorCode()!=0)
			{
				MsXml::CXMLDOMParseError xmlError(xmlDoc.GetParseError());
				CString	strErrorMsg;
				strErrorMsg.Format(STE_XML_PARSE_ERROR, 
					xmlError.GetErrorCode(), xmlError.GetReason(),
					xmlError.GetUrl(), xmlError.GetLine(), xmlError.GetLinepos(),
					xmlError.GetSrcText());

				AfxMessageBox(strErrorMsg, MB_ICONEXCLAMATION|MB_OK);
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
			_asm int 3; // force debug
			if (pE->GetDescription() == _T(""))
			{
				//A generic COM error.
				//For example, MsXML not installed.

				//Inform the user, that he needs to install MsXML.
				AfxMessageBox(STE_XML_INSTALLNEEDED, MB_ICONINFORMATION|MB_OK);
			}
			else
			{
				//Some other error. For example, schema file is missing (*.xsd)
				pE->ReportError(MB_ICONEXCLAMATION|MB_OK);
			}
		}

		pE->Delete();
		return FALSE;
	}	
}



void CStyleFileContainer::ProcessPackageNode(MsXml::CXMLDOMNode &element)
{
	/* fetch attributes */
	MsXml::CXMLDOMNamedNodeMap attr = element.GetAttributes();
	CString nameVal, descVal;
	BOOL isClass = FALSE;

	for(int j = 0; j < attr.GetLength(); j++) {
		MsXml::CXMLDOMNode a = attr.GetItem(j);
		CString s = a.GetNodeName();
		if (s == CSF_XML_NAME) {
			nameVal = a.GetText();
		} else if (s == CSF_XML_DESC) {
			descVal = a.GetText();
		} else if (s == CSF_XML_CLASS) {			
			isClass = TRUE; /* existing node is sufficient */
		}
	}
	
	/* Create style file instance and insert it into the hash map */
	CStyleFile *sf = new CStyleFile(nameVal, descVal, isClass);
	sf->SetListener(this);
	if (!AddStyleFile(sf)) {
		TRACE("WARNING: Unable to add file %s, seems to be duplicate\n", sf->GetName());
		delete sf;
		return;
	}
	
	/* Process children */
	MsXml::CXMLDOMNodeList nodes = element.GetChildNodes();		
	long n = nodes.GetLength();
	
	for (long i = 0; i < n; ++i) {	/* walk through child nodes */
		ProcessEntityNodes(nodes.GetItem(i), sf);
	}
}


void CStyleFileContainer::ProcessEntityNodes(MsXml::CXMLDOMNode &element, CStyleFile *parent)
{
	/* fetch attributes */
	MsXml::CXMLDOMNamedNodeMap attr = element.GetAttributes();
	CString nameVal, descVal, beforeVal, afterVal;
	BOOL hasAfterVal = FALSE, hasBeforeVal = FALSE; 
	int nOfParams = 0;		

	for(int j = 0; j < attr.GetLength(); j++) {
		MsXml::CXMLDOMNode a = attr.GetItem(j);
		CString s = a.GetNodeName();
		if (s == CSF_XML_NAME) {
			nameVal = a.GetText();
		} else if (s == CSF_XML_DESC) {
			descVal = a.GetText();
		} else if (s == CSF_XML_PARAMS) {
			nOfParams = atoi(a.GetText());			
		} else if (s == CSF_XML_EXPAFTER) {
			hasAfterVal = TRUE;
			afterVal = a.GetText();
			SetupCR(afterVal);
		} else if (s == CSF_XML_EXPBEFORE) {
			hasBeforeVal = TRUE;
			beforeVal = a.GetText();
			SetupCR(beforeVal);
		}
	}

	/* Add element to style file */
	if (element.GetNodeName() == CSF_XML_COMMAND) {
		CNewCommand *nc = parent->AddCommand(nameVal, nOfParams, descVal);

		if (nc != NULL && hasAfterVal) {
			//TRACE("Add expA = %s to %s\n", afterVal, nc->GetName());
			nc->SetExpandAfter(afterVal);
		}
		if (nc != NULL && hasBeforeVal) {
			//TRACE("Add expB = %s to %s\n", beforeVal, nc->GetName());
			nc->SetExpandBefore(beforeVal);
		}
	} else if (element.GetNodeName() == CSF_XML_ENVIRONMENT) {
		CNewEnvironment *ne = parent->AddEnvironment(nameVal, nOfParams, descVal);
		//TRACE("** Loaded env %s (%s)\n", nameVal, descVal);

		if (ne != NULL && hasAfterVal) {
			//TRACE("Add expA = %s to %s\n", afterVal, ne->GetName());
			ne->SetExpandAfter(afterVal);
		}
		if (ne != NULL && hasBeforeVal) {
			//TRACE("Add expB = %s to %s\n", beforeVal, ne->GetName());
			ne->SetExpandBefore(beforeVal);
		}
	}
}

/* 
	This is needed, because CrystalEdit needs a \r\n to create a line feed, but XML parsers
	turns every &#xD;&#xA; sequence into an single '&#xA;'.
	(see http://www.sql-und-xml.de/xml-lernen/xml-version-1.1-unicode.html for details)
 */

void CStyleFileContainer::SetupCR(CString &s)
{
	s.Replace("\n", "\r\n");
}
