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

#include "stdafx.h"
#include "TeXnicCenter.h"
#include "OptionPageFile.h"
#include "Configuration.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// class COptionPageFiles
//-------------------------------------------------------------------


IMPLEMENT_DYNCREATE(COptionPageFile, CPropertyPage)


BEGIN_MESSAGE_MAP(COptionPageFile, CPropertyPage)
	//{{AFX_MSG_MAP(COptionPageFile)
	ON_BN_CLICKED(IDC_OPTIONS_SAVE_AUTOMATIC, OnUpdateCtrls)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


COptionPageFile::COptionPageFile() : CPropertyPage(COptionPageFile::IDD)
{
	//{{AFX_DATA_INIT(COptionPageFile)
	//}}AFX_DATA_INIT
	m_nFileFormat = g_configuration.m_nStandardFileFormat;
	m_bSaveAutomatic = g_configuration.m_bSaveAutomatic;
	m_bSaveBeforeCompilation = g_configuration.m_bSaveBeforeCompilation;
	m_unSaveInterval = g_configuration.m_unSaveInterval;
}


COptionPageFile::~COptionPageFile()
{
}


void COptionPageFile::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionPageFile)
	DDX_Control(pDX, IDC_OPTIONS_SAVE_LABEL2, m_wndSaveIntervalLabel2);
	DDX_Control(pDX, IDC_OPTIONS_SAVE_LABEL1, m_wndSaveIntervalLabel1);
	DDX_Control(pDX, IDC_OPTIONS_SAVE_INTERVALL_EDIT, m_wndSaveIntervalEdit);
	DDX_Control(pDX, IDC_OPTIONS_SAVE_INTERVALL_SPIN, m_wndSaveIntervalSpin);
	DDX_Check(pDX, IDC_OPTIONS_SAVE_AUTOMATIC, m_bSaveAutomatic);
	DDX_Check(pDX, IDC_OPTIONS_SAVE_BEFORE_COMPILATION, m_bSaveBeforeCompilation);
	DDX_Text(pDX, IDC_OPTIONS_SAVE_INTERVALL_EDIT, m_unSaveInterval);
	DDX_CBIndex(pDX, IDC_OPTIONS_FILETYPE, m_nFileFormat);
	//}}AFX_DATA_MAP
}


BOOL COptionPageFile::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_wndSaveIntervalSpin.SetRange( 0, 120 );

	UpdateCtrlStates();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}


void COptionPageFile::UpdateCtrlStates()
{
	// enable/disable controls
	m_wndSaveIntervalEdit.EnableWindow( m_bSaveAutomatic );
	m_wndSaveIntervalSpin.EnableWindow( m_bSaveAutomatic );
	m_wndSaveIntervalLabel1.EnableWindow( m_bSaveAutomatic );
	m_wndSaveIntervalLabel2.EnableWindow( m_bSaveAutomatic );
}


void COptionPageFile::OnUpdateCtrls() 
{
	UpdateData();
	UpdateCtrlStates();
}


void COptionPageFile::OnOK() 
{
	UpdateData();

	// Store settings to configuration
	g_configuration.m_bSaveAutomatic = m_bSaveAutomatic;
	g_configuration.m_bSaveBeforeCompilation = m_bSaveBeforeCompilation;
	g_configuration.m_unSaveInterval = m_unSaveInterval;
	g_configuration.m_nStandardFileFormat = m_nFileFormat;
	
	CPropertyPage::OnOK();
}
