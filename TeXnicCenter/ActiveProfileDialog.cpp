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
#include "ActiveProfileDialog.h"
#include "profile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// class CActiveOutputtypeDialog 
//-------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CActiveProfileDialog, CDialog)
	//{{AFX_MSG_MAP(CActiveProfileDialog)
	ON_LBN_SELCHANGE(IDC_PROFILES, OnSelchangeProfile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CActiveProfileDialog::CActiveProfileDialog(CWnd* pParent /*=NULL*/)
: CDialog(CActiveProfileDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CActiveProfileDialog)
	//}}AFX_DATA_INIT
}


void CActiveProfileDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CActiveProfileDialog)
	DDX_Control(pDX, IDOK, m_wndOkButton);
	DDX_Control(pDX, IDC_PROFILES, m_wndProfileList);
	//}}AFX_DATA_MAP
}


BOOL CActiveProfileDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// insert profiles in list
	POSITION	pos = g_ProfileMap.GetStartPosition();
	CString		strProfile;
	CProfile	*pProfile;

	while (pos)
	{
		g_ProfileMap.GetNextAssoc(pos, strProfile, pProfile);
		m_wndProfileList.AddString(strProfile);
	}

	// select active item
	int	nItem = m_wndProfileList.FindStringExact(-1, g_ProfileMap.GetActiveProfileKey());
	if (nItem != LB_ERR)
		m_wndProfileList.SetCurSel(nItem);

	// disable OK-button
	if (nItem == LB_ERR)
		m_wndOkButton.EnableWindow(FALSE);

	return TRUE;
}


void CActiveProfileDialog::OnSelchangeProfile() 
{
	int	nListIndex = m_wndProfileList.GetCurSel();

	m_wndOkButton.EnableWindow(m_wndProfileList.GetCurSel() != LB_ERR);
}


void CActiveProfileDialog::OnOK() 
{
	int			nListIndex = m_wndProfileList.GetCurSel();
	CString	strKey;
	m_wndProfileList.GetText(nListIndex, strKey);
	g_ProfileMap.SetActiveProfile(strKey);
	
	CDialog::OnOK();
}
