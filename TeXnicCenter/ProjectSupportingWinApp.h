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

#if !defined(AFX_PROJECTSUPPORTINGWINAPP_H__193BB3A0_26A5_11D5_A222_006097239934__INCLUDED_)
#define AFX_PROJECTSUPPORTINGWINAPP_H__193BB3A0_26A5_11D5_A222_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ProjectTemplate.h"

/**
Derive your application application class from this class, if you
want to use project support.

@author Sven Wiegand
 */
class CProjectSupportingWinApp : public CWinAppEx
{
// construction/destruction
public:
	CProjectSupportingWinApp();
	virtual ~CProjectSupportingWinApp();

// operations
public:
	void AddProjectTemplate(CProjectTemplate *pTemplate);
	POSITION GetFirstProjectTemplatePosition() const;
	CProjectTemplate *GetNextProjectTemplate(POSITION &pos) const;
	virtual CProject *OpenProjectFile(LPCTSTR lpszFileName);
	void CloseAllProjects(BOOL bEndSession);
	BOOL DoPromptFileName(CString &fileName, UINT nIDSTitle,
	                      DWORD lFlags, BOOL bOpenFileDialog, CProjectTemplate *pTemplate);

// command handlers
public:
	afx_msg void OnProjectNew();
	afx_msg void OnProjectOpen();
	DECLARE_MESSAGE_MAP()

// overridings
public:
	virtual BOOL SaveAllModified();
	virtual BOOL InitApplication();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void *pExtra, AFX_CMDHANDLERINFO *pHandlerInfo);
	virtual BOOL OnDDECommand(LPTSTR lpszCommand);

// overwritings
public:
	void RegisterShellFileTypes(BOOL bCompat);

// attributes
protected:
	CProjectManager *m_pProjectManager;
};

//{{AFX_INSERT_LOCATION}}

#endif // AFX_PROJECTSUPPORTINGWINAPP_H__193BB3A0_26A5_11D5_A222_006097239934__INCLUDED_
