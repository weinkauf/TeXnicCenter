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
#include "OptionPageGeneric.h"
#include "configuration.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Eigenschaftenseite COptionPageGeneric 

IMPLEMENT_DYNCREATE(COptionPageGeneric, CPropertyPage)

COptionPageGeneric::COptionPageGeneric() : CPropertyPage(COptionPageGeneric::IDD)
{
	//{{AFX_DATA_INIT(COptionPageGeneric)
	m_bReplaceQm = g_configuration.m_bReplaceQuotationMarks;
	m_strClosingQm = g_configuration.m_strClosingQuotationMark;
	m_strOpeningQm = g_configuration.m_strOpeningQuotationMark;
	m_bRestoreSession = g_configuration.m_bLoadLastProject;
	m_bOptimizeGuiForVisuallyHandicappedUsers = g_configuration.m_bOptimizeMenuForVisuallyHandycappedUsersOnNextStart;
	//}}AFX_DATA_INIT
}

COptionPageGeneric::~COptionPageGeneric()
{
}


void COptionPageGeneric::UpdateCtrlStates()
{
	// enable/disable controls
	m_wndOpeningQm.EnableWindow( m_bReplaceQm );
	m_wndClosingQm.EnableWindow( m_bReplaceQm );
	m_wndQmLabel1.EnableWindow( m_bReplaceQm );
	m_wndQmLabel2.EnableWindow( m_bReplaceQm );
}


void COptionPageGeneric::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionPageGeneric)
	DDX_Control(pDX, IDC_OPTIONS_REPLACE_QM_OPEN_EDIT, m_wndOpeningQm);
	DDX_Control(pDX, IDC_OPTIONS_REPLACE_QM_CLOSE_EDIT, m_wndClosingQm);
	DDX_Control(pDX, IDC_OPTIONS_QM_LABEL2, m_wndQmLabel2);
	DDX_Control(pDX, IDC_OPTIONS_QM_LABEL1, m_wndQmLabel1);
	DDX_Check(pDX, IDC_OPTIONS_REPLACE_QM, m_bReplaceQm);
	DDX_Text(pDX, IDC_OPTIONS_REPLACE_QM_CLOSE_EDIT, m_strClosingQm);
	DDX_Text(pDX, IDC_OPTIONS_REPLACE_QM_OPEN_EDIT, m_strOpeningQm);
	DDX_Check(pDX, IDC_OPTIONS_RESTORE_SESSION, m_bRestoreSession);
	DDX_Check(pDX, IDC_OPTIONS_OPTIMIZE_FOR_VISUALLY_HANDICAPPED_USERS, m_bOptimizeGuiForVisuallyHandicappedUsers);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionPageGeneric, CPropertyPage)
	//{{AFX_MSG_MAP(COptionPageGeneric)
	ON_BN_CLICKED(IDC_OPTIONS_REPLACE_QM, OnUpdateCtrls)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL COptionPageGeneric::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	UpdateCtrlStates();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void COptionPageGeneric::OnOK() 
{
	UpdateData();

	// Store settings to configuration
	g_configuration.m_bReplaceQuotationMarks = m_bReplaceQm;
	g_configuration.m_strClosingQuotationMark = m_strClosingQm;
	g_configuration.m_strOpeningQuotationMark = m_strOpeningQm;
	g_configuration.m_bLoadLastProject = m_bRestoreSession ;

	if ((bool)g_configuration.m_bOptimizeMenuForVisuallyHandycappedUsers != (bool)m_bOptimizeGuiForVisuallyHandicappedUsers)
		AfxMessageBox(STE_OPTIONS_REQUIRES_RESTART, MB_ICONINFORMATION|MB_OK);
	g_configuration.m_bOptimizeMenuForVisuallyHandycappedUsersOnNextStart = m_bOptimizeGuiForVisuallyHandicappedUsers;
	
	CPropertyPage::OnOK();
}


void COptionPageGeneric::OnUpdateCtrls() 
{
	UpdateData();
	UpdateCtrlStates();
}
