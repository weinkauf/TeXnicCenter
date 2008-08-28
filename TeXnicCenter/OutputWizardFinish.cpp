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
#include "OutputWizardFinish.h"
#include "RunTimeHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// class COutputWizardFinish
//-------------------------------------------------------------------

IMPLEMENT_DYNCREATE(COutputWizardFinish,OutputWizardPage)

BEGIN_MESSAGE_MAP(COutputWizardFinish,OutputWizardPage)
END_MESSAGE_MAP()

COutputWizardFinish::COutputWizardFinish(COutputWizard* w)
		: OutputWizardPage(COutputWizardFinish::IDD,w)
{
	m_psp.dwFlags |= PSP_USEHEADERTITLE;
	m_psp.pszHeaderTitle = MAKEINTRESOURCE(IDS_WIZARD_FINALIZE);
	m_psp.hInstance = AfxGetResourceHandle();
}

COutputWizardFinish::~COutputWizardFinish()
{
}

void COutputWizardFinish::DoDataExchange(CDataExchange* pDX)
{
	OutputWizardPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COutputWizardFinish)
	DDX_Text(pDX,IDC_OW_OPTLIST,m_strList);
	//}}AFX_DATA_MAP
}

BOOL COutputWizardFinish::OnSetActive()
{
	if (RunTimeHelper::IsVista())
	{
		PropSheet_EnableWizButtons(m_hWnd,
		                           PSWIZB_BACK | PSWIZB_NEXT | PSWIZB_FINISH,
		                           PSWIZB_BACK | PSWIZB_NEXT | PSWIZB_FINISH);
		PropSheet_ShowWizButtons(m_hWnd,
		                         PSWIZB_BACK | PSWIZB_NEXT | PSWIZB_CANCEL | PSWIZB_FINISH,
		                         PSWIZB_BACK | PSWIZB_NEXT | PSWIZB_FINISH | PSWIZB_FINISH);
	}
	else
		GetWizard()->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH);

	return OutputWizardPage::OnSetActive();
}

BOOL COutputWizardFinish::OnWizardFinish()
{
	UpdateData();
	LRESULT result = OutputWizardPage::OnWizardFinish();
	GetWizard()->GenerateOutputProfiles();
	return result;
}
