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
#include "OutputWizardViewer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// class COutputWizardViewer 
//-------------------------------------------------------------------

IMPLEMENT_DYNCREATE(COutputWizardViewer, CPropertyPage)


BEGIN_MESSAGE_MAP(COutputWizardViewer, CPropertyPage)
	//{{AFX_MSG_MAP(COutputWizardViewer)
	ON_BN_CLICKED(IDC_OW_VIEWERBROWSE, OnViewerBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


COutputWizardViewer::COutputWizardViewer(UINT unIDTemplate /*= -1*/) 
: CPropertyPage(unIDTemplate),
m_wndViewerPHBtn(IDR_POPUP_PLACEHOLDER_CURRENT_OPT)
{
	// you have to specify a valid dialog template
	ASSERT((int)unIDTemplate != -1);

	//{{AFX_DATA_INIT(COutputWizardViewer)
	m_strForwardSearchOption = _T("");
	m_strPath = _T("");
	m_strSingleInstanceOption = _T("");
	//}}AFX_DATA_INIT
}


COutputWizardViewer::~COutputWizardViewer()
{
}


void COutputWizardViewer::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COutputWizardViewer)
	DDX_Control(pDX, IDC_OW_VIEWERFWSEARCH, m_wndFWSearch);
	DDX_Control(pDX, IDC_OW_VIEWERPOPUP, m_wndViewerPHBtn);
	DDX_Text(pDX, IDC_OW_VIEWERFWSEARCH, m_strForwardSearchOption);
	DDX_Text(pDX, IDC_OW_VIEWERPATH, m_strPath);
	DDX_Text(pDX, IDC_OW_VIEWERSI, m_strSingleInstanceOption);
	//}}AFX_DATA_MAP
}


void COutputWizardViewer::OnViewerBrowse() 
{
	CFileDialogEx	dlg( 
		TRUE, NULL, m_strPath, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, 
		CString((LPCTSTR)STE_COMPILER_FILTER), this);

	if (dlg.DoModal() != IDOK)
		return;

	m_strPath = dlg.GetPathName();
	UpdateData(FALSE);
}

BOOL COutputWizardViewer::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	//Attach edit control
	m_wndViewerPHBtn.AttachEditCtrl(&m_wndFWSearch);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
