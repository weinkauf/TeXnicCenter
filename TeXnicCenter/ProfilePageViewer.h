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

#if !defined(AFX_PROFILEPAGEVIEWER_H__5C552767_082E_11D5_A222_006097239934__INCLUDED_)
#define AFX_PROFILEPAGEVIEWER_H__5C552767_082E_11D5_A222_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "ProfilePage.h"
#include "BrowseButton.h"
#include "EditMenuButton.h"
#include "EditMenuButtonOpt.h"


/**	Edits the Viewer parts of output profiles.

@ingroup dialogs
@ingroup profiles

@author Sven Wiegand
*/
class CProfilePageViewer : public CProfilePage
{
// types
protected:
	enum tagCommand
	{
		cmdCommandLine = 0,
		cmdDde,
		cmdNone
	};

// construction/destruction
public:
	CProfilePageViewer();

// implementation
protected:
	void UpdateCmdTypes(BOOL bSaveAndValidate = TRUE);
	void UpdateControlStates();

// overridings
public:
	void OnUpdateDataSet(CProfile *pProfile);

	//{{AFX_VIRTUAL(CProfilePageViewer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// message handlers
protected:
	//{{AFX_MSG(CProfilePageViewer)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdTypeChanged();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// dialog field data
protected:
	//{{AFX_DATA(CProfilePageViewer)
	enum { IDD = IDD_PROFILE_VIEWER };
	CStatic	m_wndViewExServerTitle;
	CStatic	m_wndViewServerTitle;
	CStatic	m_wndCloseCmdServerTitle;
	CEdit	m_wndViewExDdeServer;
	CEdit	m_wndViewDdeServer;
	CEdit	m_wndCloseCmdDdeServer;
	CEdit	m_wndCloseCmdDdeTopic;
	CEditMenuButtonOpt	m_wndCloseCmdPh;
	CPersistPosEdit	m_wndCloseCmd;
	CStatic	m_wndCloseCmdTitle;
	CEditMenuButtonOpt	m_wndViewExCmdPh;
	CEdit	m_wndViewExDdeTopic;
	CStatic	m_wndViewExTopicTitle;
	CPersistPosEdit	m_wndViewExCmd;
	CStatic	m_wndViewExCmdTitle;
	CEdit	m_wndViewDdeTopic;
	CStatic	m_wndViewTopicTitle;
	CStatic	m_wndViewCmdTitle;
	CEditMenuButtonOpt	m_wndViewCmdPh;
	CPersistPosEdit	m_wndViewCmd;
	CBrowseButton	m_wndBrowseViewerPath;
	CStatic	m_wndCloseCmdTopicTitle;
	//}}AFX_DATA

// attributes
protected:
	CProfile *m_pProfile;
	int m_nViewCmdType;
	int m_nViewExCmdType;
	int m_nCloseCmdType;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_PROFILEPAGEVIEWER_H__5C552767_082E_11D5_A222_006097239934__INCLUDED_
