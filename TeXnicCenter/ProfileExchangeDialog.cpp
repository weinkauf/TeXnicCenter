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
#include "texniccenter.h"
#include "ProfileExchangeDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// class CProfileExchangeDialog 
//-------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CProfileExchangeDialog, CDialog)
	//{{AFX_MSG_MAP(CProfileExchangeDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CProfileExchangeDialog::CProfileExchangeDialog(CProfileMap &profiles, BOOL bExportMode, CWnd* pParent /* = NULL */)
: CDialog(CProfileExchangeDialog::IDD, pParent),
	m_bExportMode(bExportMode),
	m_Profiles(profiles)
{
	//{{AFX_DATA_INIT(CProfileExchangeDialog)
	m_bRemoveDirectorySpecifications = TRUE;
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////
// implementation helpers

void CProfileExchangeDialog::RefillProfileList()
{
	CStringArray	astrProfiles;
	m_Profiles.GetKeyList(astrProfiles);

	m_wndProfileList.LockWindowUpdate();
	m_wndProfileList.DeleteAllItems();
	for (int i = 0; i < astrProfiles.GetSize(); ++i)
	{
		int	nIndex = m_wndProfileList.InsertItem(i, astrProfiles[i]);
		if (nIndex > -1)
			m_wndProfileList.SetCheck(nIndex);
	}

	m_wndProfileList.UnlockWindowUpdate();
}


/////////////////////////////////////////////////////////////////////
// overridings

void CProfileExchangeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProfileExchangeDialog)
	DDX_Control(pDX, IDOK, m_wndExecuteButton);
	DDX_Control(pDX, IDC_PROFILE_LIST, m_wndProfileList);
	DDX_Check(pDX, IDC_REMOVE_DIRECTORY_SPECIFICATIONS, m_bRemoveDirectorySpecifications);
	//}}AFX_DATA_MAP
}


/////////////////////////////////////////////////////////////////////
// message handlers

BOOL CProfileExchangeDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString	strButtonText;
	if (m_bExportMode)
		GetDlgItem(IDC_STATIC_EXPORT)->GetWindowText(strButtonText);
	else
		GetDlgItem(IDC_STATIC_IMPORT)->GetWindowText(strButtonText);
	m_wndExecuteButton.SetWindowText(strButtonText);

	m_wndProfileList.SetExtendedStyle(m_wndProfileList.GetExtendedStyle()|LVS_EX_CHECKBOXES);
	RefillProfileList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}


void CProfileExchangeDialog::OnOK() 
{
	UpdateData();

	//
	// modify the profile map to reflect the user's selections
	//

	// remove unchecked items from the map
	for (int nItem = 0; nItem < m_wndProfileList.GetItemCount(); ++nItem)
	{
		if (!m_wndProfileList.GetCheck(nItem))
			m_Profiles.Remove(m_wndProfileList.GetItemText(nItem, 0));
	}

	// remove directory specifications if required
	if (m_bRemoveDirectorySpecifications)
		m_Profiles.RemoveDirectorySpecifications();
	
	CDialog::OnOK();
}
