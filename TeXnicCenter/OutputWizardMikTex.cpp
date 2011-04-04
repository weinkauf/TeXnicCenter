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
#include "resource.h"
#include "OutputWizard.h"
#include "OutputWizardMikTex.h"
#include "RunTimeHelper.h"
#include "TeXnicCenter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// class COutputWizardMiKTeX
//-------------------------------------------------------------------

IMPLEMENT_DYNCREATE(COutputWizardMiKTeX,OutputWizardPage)


BEGIN_MESSAGE_MAP(COutputWizardMiKTeX,OutputWizardPage)
	//{{AFX_MSG_MAP(COutputWizardMiKTeX)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


COutputWizardMiKTeX::COutputWizardMiKTeX(COutputWizard* w)
		: OutputWizardPage(COutputWizardMiKTeX::IDD,w)
		, m_nChoice(yes)
{
	m_psp.dwFlags |= PSP_USEHEADERTITLE;
	m_psp.pszHeaderTitle = MAKEINTRESOURCE(IDS_WIZARD_MIKTEX_DETECTED);
	m_psp.hInstance = AfxGetResourceHandle();
}

COutputWizardMiKTeX::~COutputWizardMiKTeX()
{
}

void COutputWizardMiKTeX::DoDataExchange(CDataExchange* pDX)
{
	OutputWizardPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COutputWizardMiKTeX)
	DDX_Radio(pDX,IDC_OW_MIKTEXYES,m_nChoice);
	//}}AFX_DATA_MAP
}

BOOL COutputWizardMiKTeX::OnSetActive()
{
	CString fmt;
	fmt.Format(IDS_WIZARD_MIKTEX_DETECTED,GetWizard()->GetDistributionName());

	PropSheet_SetHeaderTitle(GetWizard()->GetSafeHwnd(),
		PropSheet_HwndToIndex(GetWizard()->GetSafeHwnd(),m_hWnd),static_cast<LPCTSTR>(fmt));

	fmt.Format(IDS_CONFIGURE_FOUND_DISTRIBUTION,GetWizard()->GetDistributionName());

	SetDlgItemText(IDC_OW_MIKTEXYES,fmt);

	if (RunTimeHelper::IsVista())
	{
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

LRESULT COutputWizardMiKTeX::OnWizardNext()
{
	UpdateData();
	OutputWizardPage::OnWizardNext();

	LRESULT result;

	//if (m_nChoice == COutputWizardMiKTeX::yes && GetWizard()->LookForLatex()) {
	//    GetWizard()->LookForDviViewer();
	//    result = 0;
	//}
	//else
	result = -1;

	return result;
}
