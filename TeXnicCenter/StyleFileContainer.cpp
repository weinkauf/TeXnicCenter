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
				xmlCmd.SetAttribute(CSF_XML_NAME, (LPCTSTR)lc->ToLaTeX());

				if (lc->GetNoOfParams() > 0) {
					TCHAR buf[5];
					itoa(lc->GetNoOfParams(), buf, 10);
					xmlCmd.SetAttribute(CSF_XML_PARAMS, buf);
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
			TRACE("Desc: %s", pE->GetDescription());
			
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

/*
 * $Log$
 * Revision 1.2  2005/06/03 22:29:20  owieland
 * XML Export
 *
 * Revision 1.1  2005/06/03 20:29:43  owieland
 * Initial checkin of package and class parser
 *
 */

