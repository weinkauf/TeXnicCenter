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
#include "OutputWizardMikTex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// class COutputWizardMikTex 
//-------------------------------------------------------------------

IMPLEMENT_DYNCREATE(COutputWizardMikTex, CPropertyPage)


BEGIN_MESSAGE_MAP(COutputWizardMikTex, CPropertyPage)
	//{{AFX_MSG_MAP(COutputWizardMikTex)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


COutputWizardMikTex::COutputWizardMikTex() 
: CPropertyPage(COutputWizardMikTex::IDD),
	m_nChoice(yes)
{
	//{{AFX_DATA_INIT(COutputWizardMikTex)
	//}}AFX_DATA_INIT
}


COutputWizardMikTex::~COutputWizardMikTex()
{
}


void COutputWizardMikTex::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COutputWizardMikTex)
	DDX_Radio(pDX, IDC_OW_MIKTEXYES, m_nChoice);
	//}}AFX_DATA_MAP
}
