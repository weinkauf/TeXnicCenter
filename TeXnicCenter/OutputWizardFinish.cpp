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
#include "OutputWizardFinish.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// class COutputWizardFinish 
//-------------------------------------------------------------------

IMPLEMENT_DYNCREATE(COutputWizardFinish, CPropertyPage)


BEGIN_MESSAGE_MAP(COutputWizardFinish, CPropertyPage)
	//{{AFX_MSG_MAP(COutputWizardFinish)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


COutputWizardFinish::COutputWizardFinish() 
: CPropertyPage(COutputWizardFinish::IDD)
{
	//{{AFX_DATA_INIT(COutputWizardFinish)
	m_strList = _T("");
	//}}AFX_DATA_INIT
}


COutputWizardFinish::~COutputWizardFinish()
{
}

void COutputWizardFinish::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COutputWizardFinish)
	DDX_Text(pDX, IDC_OW_OPTLIST, m_strList);
	//}}AFX_DATA_MAP
}
