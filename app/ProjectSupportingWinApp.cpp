/********************************************************************
 *
 * This file is part of the TeXnicCenter-system
 *
 * Copyright (C) 1999-2000 Sven Wiegand
 * Copyright (C) 2000-$CurrentYear$ ToolsCenter
 *
 * This program is free software you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * If you have further questions or if you want to support
 * further TeXnicCenter development, visit the TeXnicCenter-homepage
 *
 *    http://www.ToolsCenter.org
 *
 *********************************************************************/

#include "stdafx.h"
#include "resource.h"
#include "Project.hpp"
#include "TeXnicCenter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//-------------------------------------------------------------------
// class CProjectSupportingWinApp implementation
//-------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CProjectSupportingWinApp, CWinApp)
	ON_COMMAND(ID_PROJECT_NEW, OnProjectNew)
	ON_COMMAND(ID_PROJECT_OPEN, OnProjectOpen)
END_MESSAGE_MAP()


CProjectSupportingWinApp::CProjectSupportingWinApp()
{
	// initialize
	m_pProjectManager = NULL;
}

CProjectSupportingWinApp::~CProjectSupportingWinApp()
{
	if (m_pProjectManager)
		delete m_pProjectManager;
}

void CProjectSupportingWinApp::AddProjectTemplate(CProjectTemplate *pTemplate)
{
	if (m_pProjectManager == NULL)
		m_pProjectManager = new CProjectManager;
	m_pProjectManager->AddProjectTemplate(pTemplate);
}

POSITION CProjectSupportingWinApp::GetFirstProjectTemplatePosition() const
{
	if (m_pProjectManager == NULL)
		return NULL;
	return m_pProjectManager->GetFirstProjectTemplatePosition();
}

CProjectTemplate *CProjectSupportingWinApp::GetNextProjectTemplate(POSITION &pos) const
{
	ASSERT(m_pProjectManager != NULL);
	return m_pProjectManager->GetNextProjectTemplate(pos);
}

CProject *CProjectSupportingWinApp::OpenProjectFile(LPCTSTR lpszFileName)
{
	ASSERT(m_pProjectManager != NULL);
	return m_pProjectManager->OpenProjectFile(lpszFileName);
}

void CProjectSupportingWinApp::CloseAllProjects(BOOL bEndSession)
{
	if (m_pProjectManager != NULL)
		m_pProjectManager->CloseAllProjects(bEndSession);
}

BOOL CProjectSupportingWinApp::DoPromptFileName(CString &fileName,
        UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog, CProjectTemplate *pTemplate)
{
	ASSERT(m_pProjectManager != NULL);
	return m_pProjectManager->DoPromptFileName(fileName, nIDSTitle, lFlags, bOpenFileDialog, pTemplate);
}

void CProjectSupportingWinApp::OnProjectNew()
{
	if (m_pProjectManager != NULL)
		m_pProjectManager->OnProjectNew();
}

void CProjectSupportingWinApp::OnProjectOpen()
{
	ASSERT(m_pProjectManager != NULL);
	m_pProjectManager->OnProjectOpen();
}

BOOL CProjectSupportingWinApp::SaveAllModified()
{
	if (!CWinApp::SaveAllModified())
		return FALSE;

	if (m_pProjectManager != NULL)
		return m_pProjectManager->SaveAllModified();

	return TRUE;
}

BOOL CProjectSupportingWinApp::InitApplication()
{
	if (!CWinApp::InitApplication())
		return FALSE;

	if (CProjectManager::pStaticProjectManager != NULL)
	{
		if (m_pProjectManager == NULL)
			m_pProjectManager = CProjectManager::pStaticProjectManager;
		CProjectManager::pStaticProjectManager = NULL;
	}

	if (m_pProjectManager != NULL)
		m_pProjectManager->AddProjectTemplate(NULL);
	else
		CProjectManager::bStaticInit = FALSE;

	return TRUE;

}

BOOL CProjectSupportingWinApp::OnCmdMsg(UINT nID, int nCode, void *pExtra, AFX_CMDHANDLERINFO *pHandlerInfo)
{
	if (CWinApp::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	if (!m_pProjectManager)
		return FALSE;

	// route throug projects
	POSITION posTemplate = m_pProjectManager->GetFirstProjectTemplatePosition();

	while (posTemplate)
	{
		CProjectTemplate *pTemplate = m_pProjectManager->GetNextProjectTemplate(posTemplate);
		POSITION posProject = pTemplate->GetFirstProjectPosition();
		while (posProject)
		{
			CProject *pProject = pTemplate->GetNextProject(posProject);
			if (pProject->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
				return TRUE;
		}
	}

	return FALSE;
}

BOOL CProjectSupportingWinApp::OnDDECommand(LPTSTR lpszCommand)
{
	if (m_pProjectManager != NULL && m_pProjectManager->OnDDECommand(lpszCommand))
		return TRUE;

	return CWinApp::OnDDECommand(lpszCommand);
}

void CProjectSupportingWinApp::RegisterShellFileTypes(BOOL bCompat)
{
	CWinApp::RegisterShellFileTypes(bCompat);
	ASSERT(m_pProjectManager != NULL);
	m_pProjectManager->RegisterShellFileTypes(bCompat);
}
