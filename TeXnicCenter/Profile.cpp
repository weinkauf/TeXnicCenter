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
#include "Profile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CProfileMap	g_ProfileMap;

//-------------------------------------------------------------------
// class CProfileArray
//-------------------------------------------------------------------

CProfileMap::CProfileMap()
:	CMap<CString, LPCTSTR, CProfile*, CProfile*>()
{}


CProfileMap::~CProfileMap()
{
	RemoveAll();
}


BOOL CProfileMap::Add(LPCTSTR lpszKey, CProfile &profile)
{
	CProfile	*pProfile = new CProfile(profile);
	if (!pProfile)
		return FALSE;

	if (Exists(lpszKey))
		return FALSE;

	SetAt(lpszKey, pProfile);

	// if there is no active profile till now, select the new one
	if (m_strActiveProfile.IsEmpty())
		m_strActiveProfile = lpszKey;

	return TRUE;
}


void CProfileMap::Remove(LPCTSTR lpszKey)
{
	CProfile	*pProfile = NULL;

	if (!Lookup(lpszKey, pProfile))
		return;

	delete pProfile;
	RemoveKey(lpszKey);

	// if deleted profile was the active one, then select the first
	// available profile
	if (m_strActiveProfile == lpszKey)
	{
		POSITION	pos = GetStartPosition();
		if (!pos)
		{
			m_strActiveProfile.Empty();
			return;
		}

		CString		strKey;
		CProfile	*pProfile;
		GetNextAssoc(pos, strKey, pProfile);
		m_strActiveProfile = strKey;
	}
}


void CProfileMap::RemoveAll()
{
	POSITION	pos = GetStartPosition();
	CString		strKey;
	CProfile	*pProfile;

	while (pos)
	{
		GetNextAssoc(pos, strKey, pProfile);
		delete pProfile;
	}

	CMap<CString, LPCTSTR, CProfile*, CProfile*>::RemoveAll();

	m_strActiveProfile.Empty();
}


BOOL CProfileMap::Exists(LPCTSTR lpszKey) const
{
	CProfile	*pTemp;
	return Lookup(lpszKey, pTemp);
}


BOOL CProfileMap::Rename(LPCTSTR lpszOldKey, LPCTSTR lpszNewKey)
{
	if (Exists(lpszNewKey))
		return FALSE;
	if (!Exists(lpszOldKey))
		return FALSE;

	CProfile	*pTemp;
	Lookup(lpszOldKey, pTemp);
	RemoveKey(lpszOldKey);
	SetAt(lpszNewKey, pTemp);

	// hold active profile
	if (m_strActiveProfile == lpszOldKey)
		m_strActiveProfile = lpszNewKey;

	return TRUE;
}


BOOL CProfileMap::SetActiveProfile(LPCTSTR lpszKey, bool bSetModifiedProject/* = true*/)
{
	if (!Exists(lpszKey))
		return FALSE;

	m_strActiveProfile = lpszKey;
	if (bSetModifiedProject)
	{
		//Set the ModifiedFlag of the project, because this change needs to be saved.
		CLatexProject* pLProject = ((CTeXnicCenterApp*)AfxGetApp())->GetProject();
		if (pLProject) pLProject->SetModifiedFlag(true);
	}
	return TRUE;
}


CProfile *CProfileMap::GetActiveProfile()
{
	CProfile	*pProfile;
	if (Lookup(m_strActiveProfile, pProfile))
		return pProfile;
	else
		return NULL;
}


CString CProfileMap::GetActiveProfileKey() const
{
	if (!Exists(m_strActiveProfile))
		m_strActiveProfile.Empty();

	return m_strActiveProfile;
}


void CProfileMap::GetKeyList(CStringArray &astrKeys) const
{
	astrKeys.RemoveAll();

	POSITION	pos = GetStartPosition();
	CString		strKey;
	CProfile	*pProfile;

	while (pos)
	{
		GetNextAssoc(pos, strKey, pProfile);
		astrKeys.Add(strKey);
	}
}


BOOL CProfileMap::SerializeToRegistry() const
{
	CBCGRegistryEx	reg(FALSE, FALSE);
	CString					strKey(CPathTool::Cat(theApp.m_strRegistryRoot, _T("Profiles")));
	reg.DeleteKey(strKey);
	reg.CreateKey(strKey);

	reg.Write(_T("ActiveProfile"), m_strActiveProfile);

	POSITION	pos = GetStartPosition();
	while (pos)
	{
		CString		strTitle;
		CProfile	*pTemp;
		GetNextAssoc(pos, strTitle, pTemp);
		reg.PushKey();
		reg.CreateKey(strTitle);
		pTemp->SerializeToRegistry(reg);
		reg.PopKey();
	}

	return TRUE;
}


BOOL CProfileMap::SerializeFromRegistry()
{
	RemoveAll();

	CBCGRegistryEx	reg(FALSE, TRUE);
	CString					strKey(CPathTool::Cat(theApp.m_strRegistryRoot, _T("Profiles")));
	if (!reg.Open(strKey))
		return FALSE;

	CStringArray	astrKeys;
	CProfile			*pTemp;
	reg.ReadSubKeys(astrKeys);
	reg.PushKey();

	reg.Read(_T("ActiveProfile"), m_strActiveProfile);
	if (m_strActiveProfile.IsEmpty() && astrKeys.GetSize())
		m_strActiveProfile = astrKeys[0];

	for (int i = 0; i < astrKeys.GetSize(); i++)
	{
		if (Exists(astrKeys[i]))
			// key already exists -> continue with next element
			continue;

		reg.TopKey();
		reg.Open(astrKeys[i]);
		pTemp = new CProfile;
		pTemp->SerializeFromRegistry(reg);

		SetAt(astrKeys[i], pTemp);
	}

	reg.PopKey();

	return TRUE;
}


//-------------------------------------------------------------------
// class CProfile
//-------------------------------------------------------------------

CProfile::CProfile()
:	m_bRunLatex(FALSE),
	m_bStopOnLatexError(FALSE),
	m_bRunBibTex(FALSE),
	m_bRunMakeIndex(FALSE),
	m_bCloseBeforeCompilation(FALSE)
{}


CProfile::CProfile(const CProfile &p)
{
	*this = p;
}


CProfile::~CProfile()
{}


CProfile &CProfile::operator=(const CProfile &p)
{
	m_aPostProcessors.RemoveAll();
	m_aPostProcessors.Append(p.m_aPostProcessors);

	m_bCloseBeforeCompilation = p.m_bCloseBeforeCompilation;
	m_bRunLatex = p.m_bRunLatex;
	m_bStopOnLatexError = p.m_bStopOnLatexError;
	m_bRunBibTex = p.m_bRunBibTex;
	m_bRunMakeIndex = p.m_bRunMakeIndex;
	m_cmdCloseView = p.m_cmdCloseView;
	m_cmdViewCurrent = p.m_cmdViewCurrent;
	m_cmdViewProject = p.m_cmdViewProject;
	m_strLatexPath = p.m_strLatexPath;
	m_strLatexArguments = p.m_strLatexArguments;
	m_strBibTexPath = p.m_strBibTexPath;
	m_strBibTexArguments = p.m_strBibTexArguments;
	m_strMakeIndexPath = p.m_strMakeIndexPath;
	m_strMakeIndexArguments = p.m_strMakeIndexArguments;
	m_strViewerPath = p.m_strViewerPath;

	return *this;
}


void CProfile::SetLatexPath(LPCTSTR lpszPath, LPCTSTR lpszArguments, BOOL bUseLatex /*= TRUE*/, BOOL bStopOnLatexError /*= FALSE*/)
{
	m_strLatexPath = lpszPath;
	m_strLatexArguments = lpszArguments;
	m_bRunLatex = bUseLatex;
	m_bStopOnLatexError = bStopOnLatexError;
}


void CProfile::SetBibTexPath(LPCTSTR lpszPath, LPCTSTR lpszArguments, BOOL bUseBibTex /*= TRUE*/)
{
	m_strBibTexPath = lpszPath;
	m_strBibTexArguments = lpszArguments;
	m_bRunBibTex = bUseBibTex;
}


void CProfile::SetMakeIndexPath(LPCTSTR lpszPath, LPCTSTR lpszArguments, BOOL bUseMakeIndex /*= TRUE*/)
{
	m_strMakeIndexPath = lpszPath;
	m_strMakeIndexArguments = lpszArguments;
	m_bRunMakeIndex = bUseMakeIndex;
}


void CProfile::SetViewerPath(LPCTSTR lpszViewerPath)
{
	m_strViewerPath = lpszViewerPath;
}


void CProfile::SetViewProjectCmd(const CProfile::CCommand &cmd)
{
	m_cmdViewProject = cmd;
}


void CProfile::SetViewCurrentCmd(const CProfile::CCommand &cmd)
{
	m_cmdViewCurrent = cmd;
}


void CProfile::SetCloseView(BOOL bClose)
{
	m_bCloseBeforeCompilation = bClose;
}


void CProfile::SetViewCloseCmd(const CProfile::CCommand &cmd)
{
	m_cmdCloseView = cmd;
}


BOOL CProfile::SerializeToRegistry(CBCGRegistryEx &reg) const
{
	reg.Write(_T("RunLatex"), m_bRunLatex);
	reg.Write(_T("LatexPath"), m_strLatexPath);
	reg.Write(_T("LatexArgs"), m_strLatexArguments);
	reg.Write(_T("LatexStopOnError"), m_bStopOnLatexError);

	reg.Write(_T("RunBibTex"), m_bRunBibTex);
	reg.Write(_T("BibTexPath"), m_strBibTexPath);
	reg.Write(_T("BibTexArgs"), m_strBibTexArguments);

	reg.Write(_T("RunMakeIndex"), m_bRunMakeIndex);
	reg.Write(_T("MakeIndexPath"), m_strMakeIndexPath);
	reg.Write(_T("MakeIndexArgs"), m_strMakeIndexArguments);

	reg.Write(_T("ViewerPath"), m_strViewerPath);
	reg.Write(_T("CloseViewBeforeCompilation"), m_bCloseBeforeCompilation);

	reg.PushKey();
	reg.CreateKey(_T("PostProcessors"));
	m_aPostProcessors.SerializeToRegistry(reg);

	reg.TopKey();
	reg.CreateKey(_T("ViewProjectCmd"));
	m_cmdViewProject.SerializeToRegistry(reg);

	reg.TopKey();
	reg.CreateKey(_T("ViewCurrentCmd"));
	m_cmdViewCurrent.SerializeToRegistry(reg);

	reg.TopKey();
	reg.CreateKey(_T("ViewCloseCmd"));
	m_cmdCloseView.SerializeToRegistry(reg);

	reg.PopKey();

	return TRUE;
}


BOOL CProfile::SerializeFromRegistry(CBCGRegistryEx &reg)
{
	reg.Read(_T("RunLatex"), m_bRunLatex);
	reg.Read(_T("LatexPath"), m_strLatexPath);
	reg.Read(_T("LatexArgs"), m_strLatexArguments);
	reg.Read(_T("LatexStopOnError"), m_bStopOnLatexError);

	reg.Read(_T("RunBibTex"), m_bRunBibTex);
	reg.Read(_T("BibTexPath"), m_strBibTexPath);
	reg.Read(_T("BibTexArgs"), m_strBibTexArguments);

	reg.Read(_T("RunMakeIndex"), m_bRunMakeIndex);
	reg.Read(_T("MakeIndexPath"), m_strMakeIndexPath);
	reg.Read(_T("MakeIndexArgs"), m_strMakeIndexArguments);
	
	reg.Read(_T("ViewerPath"), m_strViewerPath);
	reg.Read(_T("CloseViewBeforeCompilation"), m_bCloseBeforeCompilation);

	reg.PushKey();
	reg.Open(_T("PostProcessors"));
	m_aPostProcessors.SerializeFromRegistry(reg);

	reg.TopKey();
	reg.Open(_T("ViewProjectCmd"));
	m_cmdViewProject.SerializeFromRegistry(reg);

	reg.TopKey();
	reg.Open(_T("ViewCurrentCmd"));
	m_cmdViewCurrent.SerializeFromRegistry(reg);

	reg.TopKey();
	reg.Open(_T("ViewCloseCmd"));
	m_cmdCloseView.SerializeFromRegistry(reg);

	reg.PopKey();

	return TRUE;
}


//-------------------------------------------------------------------
// class CProfile::CCommand
//-------------------------------------------------------------------

CProfile::CCommand::CCommand()
:	m_nActiveCommand(typeProcess)
{}


CProfile::CCommand::~CCommand()
{}


void CProfile::CCommand::SetActiveCommand(int nActiveCommand)
{
	ASSERT(nActiveCommand == typeProcess || nActiveCommand == typeDde);

	m_nActiveCommand = nActiveCommand;
}


void CProfile::CCommand::SetProcessCommand(const CProcessCommand &cmd)
{
	m_cmdProcess = cmd;
}


void CProfile::CCommand::SetDdeCommand(const CDdeCommand &cmd)
{
	m_cmdDde = cmd;
}


BOOL CProfile::CCommand::SerializeToRegistry(CBCGRegistryEx &reg) const
{
	reg.Write(_T("ActiveType"), m_nActiveCommand);
	reg.Write(_T("ProcessCmd"), m_cmdProcess.SerializeToString());
	reg.Write(_T("DDECmd"), m_cmdDde.SerializeToString());

	return TRUE;
}


BOOL CProfile::CCommand::SerializeFromRegistry(CBCGRegistryEx &reg)
{
	CString	strPackedInformation;
	reg.Read(_T("ActiveType"), m_nActiveCommand);
	reg.Read(_T("ProcessCmd"), strPackedInformation);
	m_cmdProcess.SerializeFromString(strPackedInformation);
	reg.Read(_T("DDECmd"), strPackedInformation);
	m_cmdDde.SerializeFromString(strPackedInformation);

	return TRUE;
}