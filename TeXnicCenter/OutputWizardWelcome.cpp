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
#include "OutputWizardWelcome.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(COutputWizardWelcome,OutputWizardPage)

COutputWizardWelcome::COutputWizardWelcome(COutputWizard* w)
		: OutputWizardPage(COutputWizardWelcome::IDD,w)
{
	//{{AFX_DATA_INIT(COutputWizardWelcome)
	// HINWEIS: Der Klassen-Assistent fügt hier Elementinitialisierung ein
	//}}AFX_DATA_INIT
}

COutputWizardWelcome::~COutputWizardWelcome()
{
}

void COutputWizardWelcome::DoDataExchange(CDataExchange* pDX)
{
	OutputWizardPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COutputWizardWelcome)
	// HINWEIS: Der Klassen-Assistent fügt hier DDX- und DDV-Aufrufe ein
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COutputWizardWelcome,OutputWizardPage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten COutputWizardWelcome

#include "RunTimeHelper.h"

BOOL COutputWizardWelcome::OnInitDialog()
{
	OutputWizardPage::OnInitDialog();

	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL COutputWizardWelcome::OnSetActive()
{
	if (RunTimeHelper::IsVista())
	{
		PropSheet_EnableWizButtons(m_hWnd,
		                           PSWIZB_NEXT | PSWIZB_CANCEL,
		                           PSWIZB_BACK | PSWIZB_NEXT | PSWIZB_CANCEL);
		PropSheet_ShowWizButtons(m_hWnd,
		                         PSWIZB_BACK | PSWIZB_NEXT | PSWIZB_CANCEL,
		                         PSWIZB_BACK | PSWIZB_NEXT | PSWIZB_FINISH | PSWIZB_CANCEL);
	}
	else
		GetWizard()->SetWizardButtons(PSWIZB_NEXT);

	return OutputWizardPage::OnSetActive();
}

LRESULT COutputWizardWelcome::OnWizardNext()
{
	UpdateData();
	/*LRESULT result = */OutputWizardPage::OnWizardNext();
	GetWizard()->LookForMikTex();
	return -1;
}

