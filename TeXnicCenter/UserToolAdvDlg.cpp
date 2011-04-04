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
#include "resource.h"
#include "UserToolAdvDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserToolAdvDlg dialog


CUserToolAdvDlg::CUserToolAdvDlg(CWnd* pParent /*=NULL*/)
		: CDialog(CUserToolAdvDlg::IDD, pParent),
		m_wndInputBtn(IDR_POPUP_PLACEHOLDER_OPT),
		m_wndOutputBtn(IDR_POPUP_PLACEHOLDER_OPT)
{
	//{{AFX_DATA_INIT(CUserToolAdvDlg)
	m_bCloseConsoleWindow = FALSE;
	m_bPromptForArguments = FALSE;
	m_bUseOutputWindow = FALSE;
	//}}AFX_DATA_INIT
}


void CUserToolAdvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserToolAdvDlg)
	DDX_Control(pDX, IDC_ADVTOOLS_CLOSEWIN, m_wndCloseWindowBtn);
	DDX_Control(pDX, IDC_EDIT_PPINPUT, m_wndInputFile);
	DDX_Control(pDX, IDC_EDIT_PPOUTPUT, m_wndOutputFile);
	DDX_Control(pDX, IDC_INSERTPH_PPOUTPUT, m_wndOutputBtn);
	DDX_Control(pDX, IDC_INSERTPH_PPINPUT, m_wndInputBtn);
	DDX_Control(pDX, IDC_ADVTOOLS_USEOUTPUT, m_wndUseOutputBtn);
	DDX_Check(pDX, IDC_ADVTOOLS_CLOSEWIN, m_bCloseConsoleWindow);
	DDX_Check(pDX, IDC_ADVTOOLS_PROMPTARG, m_bPromptForArguments);
	DDX_Check(pDX, IDC_ADVTOOLS_USEOUTPUT, m_bUseOutputWindow);
	DDX_Text(pDX, IDC_EDIT_PPINPUT, m_strInputFile);
	DDX_Text(pDX, IDC_EDIT_PPOUTPUT, m_strOutputFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserToolAdvDlg, CDialog)
	//{{AFX_MSG_MAP(CUserToolAdvDlg)
	ON_BN_CLICKED(IDC_ADVTOOLS_USEOUTPUT, OnUseOutput)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserToolAdvDlg message handlers

void CUserToolAdvDlg::OnUseOutput()
{
	UpdateData();

	m_wndOutputFile.EnableWindow(!m_bUseOutputWindow);
	m_wndOutputBtn.EnableWindow(!m_bUseOutputWindow);
	m_wndCloseWindowBtn.EnableWindow(!m_bUseOutputWindow);
}

BOOL CUserToolAdvDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//Attach Edits to Buttons
	m_wndInputBtn.AttachEditCtrl(&m_wndInputFile);
	m_wndOutputBtn.AttachEditCtrl(&m_wndOutputFile);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
