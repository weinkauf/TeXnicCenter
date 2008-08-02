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
#include "OutputWizard.h"
#include "OutputWizardDistributionPath.h"
#include "RunTimeHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// class COutputWizardDistributionPath 
//-------------------------------------------------------------------

IMPLEMENT_DYNCREATE(COutputWizardDistributionPath,OutputWizardPage)


BEGIN_MESSAGE_MAP(COutputWizardDistributionPath,OutputWizardPage)
//{{AFX_MSG_MAP(COutputWizardDistributionPath)
ON_BN_CLICKED(IDC_OW_BROWSEPATH,OnOwBrowsepath)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


COutputWizardDistributionPath::COutputWizardDistributionPath(COutputWizard* w)
: OutputWizardPage(COutputWizardDistributionPath::IDD,w)
{
    //{{AFX_DATA_INIT(COutputWizardDistributionPath)
    //}}AFX_DATA_INIT
}

COutputWizardDistributionPath::~COutputWizardDistributionPath()
{
}

void COutputWizardDistributionPath::DoDataExchange(CDataExchange* pDX)
{
    OutputWizardPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(COutputWizardDistributionPath)
    DDX_Text(pDX,IDC_OW_PATH,m_strPath);
    //}}AFX_DATA_MAP
}

void COutputWizardDistributionPath::OnOwBrowsepath()
{
    CFolderSelect dlg(CString((LPCTSTR)STE_GET_PATH));

    if (dlg.DoModal() != IDOK)
        return;

    m_strPath = dlg.GetPath();

    UpdateData(FALSE);
}

BOOL COutputWizardDistributionPath::OnInitDialog()
{
    OutputWizardPage::OnInitDialog();

    //browse_.SubclassWindow(GetDlgItem(IDC_OW_PATH)->m_hWnd);
    //browse_.EnableBrowseButton();

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL COutputWizardDistributionPath::OnSetActive()
{
    if (RunTimeHelper::IsVista()) {
        PropSheet_EnableWizButtons(m_hWnd,
            PSWIZB_BACK | PSWIZB_NEXT | PSWIZB_CANCEL,
            PSWIZB_BACK | PSWIZB_NEXT | PSWIZB_CANCEL);
        PropSheet_ShowWizButtons(m_hWnd,
            PSWIZB_BACK | PSWIZB_NEXT | PSWIZB_CANCEL,
            PSWIZB_BACK | PSWIZB_NEXT | PSWIZB_FINISH | PSWIZB_CANCEL);
    }
    else
        GetWizard()->SetWizardButtons(PSWIZB_NEXT | PSWIZB_BACK);

    return OutputWizardPage::OnSetActive();
}

LRESULT COutputWizardDistributionPath::OnWizardNext()
{
    UpdateData();
    OutputWizardPage::OnWizardNext();

    //LRESULT result;

    //if (GetWizard()->LookForLatex()) {
    //    GetWizard()->LookForDviViewer();
    //    result = 0;
    //}
    //else {
    //    GetWizard()->m_stackPageHistory.Pop();
    //    result = -1;
    //}

    return -1/*result*/;
}
