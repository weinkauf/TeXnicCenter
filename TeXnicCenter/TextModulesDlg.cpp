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
#include "TextModules.h"
#include "TextModulesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextModulesDlg dialog


CTextModulesDlg::CTextModulesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTextModulesDlg::IDD, pParent),
	nOldSelectedItem(-1),
	bLock(false),
	bNameChanged(false)
{
	//{{AFX_DATA_INIT(CTextModulesDlg)
	m_strRight = _T("");
	m_strLeft = _T("");
	m_strName = _T("");
	//}}AFX_DATA_INIT
}


void CTextModulesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTextModulesDlg)
	DDX_Control(pDX, IDC_TEXTMODULES_BEFORECURSOR, m_wndBeforeEdit);
	DDX_Control(pDX, IDC_TEXTMODULES_AFTERCURSOR, m_wndAfterEdit);
	DDX_Control(pDX, IDC_TEXTMODULES_NAME, m_wndNameEdit);
	DDX_Control(pDX, IDC_TEXTMODULES_MODULES, m_wndModulesList);
	DDX_Control(pDX, IDC_TEXTMODULES_DELETEMODULE, m_wndDelBtn);
	DDX_Control(pDX, IDC_TEXTMODULES_NEWMODULE, m_wndNewBtn);
	DDX_Text(pDX, IDC_TEXTMODULES_AFTERCURSOR, m_strRight);
	DDX_Text(pDX, IDC_TEXTMODULES_BEFORECURSOR, m_strLeft);
	DDX_Text(pDX, IDC_TEXTMODULES_NAME, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTextModulesDlg, CDialog)
	//{{AFX_MSG_MAP(CTextModulesDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TEXTMODULES_MODULES, OnItemchangedModules)
	ON_BN_CLICKED(IDC_TEXTMODULES_NEWMODULE, OnNewTextModule)
	ON_BN_CLICKED(IDC_TEXTMODULES_DELETEMODULE, OnDeleteTextModule)
	ON_EN_CHANGE(IDC_TEXTMODULES_NAME, OnChangeName)
	ON_EN_KILLFOCUS(IDC_TEXTMODULES_NAME, OnKillFocusName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextModulesDlg message handlers

BOOL CTextModulesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//Change Style
	m_wndModulesList.SetExtendedStyle(m_wndModulesList.GetExtendedStyle() | LVS_EX_FULLROWSELECT); //| LVS_EX_GRIDLINES

	//Create the Column
	CRect rect;
	m_wndModulesList.GetWindowRect(rect);
	m_wndModulesList.InsertColumn(0, _T(""), LVCFMT_LEFT, rect.Width() - 20, -1);

	//Fill Tree
	UpdateTree(0);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTextModulesDlg::UpdateTree(int nIDSelect /*= -1*/)
{
	//Not valid?
	if (!IsWindow(m_wndModulesList.m_hWnd))
		return;

//	//Remember the selected entry
//	int nLastSelected = m_wndModulesList.GetNextItem(-1, LVNI_SELECTED);
//	if (nLastSelected < 0) nLastSelected = 0;

	bLock = true;

	//////////////////////////
	//Clear List
	m_wndModulesList.DeleteAllItems();

	if (m_tmGroup.GetSize() == 0)
	{
		bLock = false;
		return;
	}

	//////////////////////////
	//Init Structure
	LVITEM lvi;
	lvi.mask = LVIF_TEXT | LVIF_PARAM; //| LVIF_IMAGE

	//////////////////////////
	//Fill List
	int i;
	int nsize = m_tmGroup.GetSize();
	for(i=0;i<nsize;i++)
	{
		//Item specific stuff
		lvi.lParam = (LPARAM)i;
		lvi.iItem = i;

		//First Column
		lvi.pszText = m_tmGroup[i].Name.GetBuffer(1);
		lvi.iSubItem = 0;
		m_wndModulesList.InsertItem(&lvi);
	}

	bLock = false;

	//Restore the last selected Item
	if (nIDSelect >= 0)
	{
		if (!m_wndModulesList.SetItemState(nIDSelect, LVNI_SELECTED, LVNI_SELECTED))
		{
			//Happens, if the entry could not be found (last entry deleted for example)
			m_wndModulesList.SetItemState(0, LVNI_SELECTED, LVNI_SELECTED);
		}
	}

//	//Get the Focus!
//	m_wndModulesList.SetFocus();
}


void CTextModulesDlg::OnItemchangedModules(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (bLock) return;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	//Did the state change?
	if (pNMListView->uChanged & LVIF_STATE)
	{
		//Was this Item selected or de-selected
		// - We're considering only selected Items
		if (pNMListView->uNewState & LVIS_SELECTED)
		{
			UpdateControls();
		}
	}

	*pResult = 0;
}

void CTextModulesDlg::UpdateControls()
{
	//Retrieve data
	UpdateOldSelectedItem();

	//Set data
	CTextModule* ptm = GetSelectedModule();
	if (ptm)
	{
		m_strLeft = ptm->GetLeftText();
		m_strRight = ptm->GetRightText();
		m_strName = ptm->Name;
		UpdateData(false);
	}
	else
	{
		ClearControls();
	}
}


CTextModule* CTextModulesDlg::GetSelectedModule()
{
	//Get the selected item
	int nItemIndex = m_wndModulesList.GetNextItem(-1, LVNI_SELECTED);

	//Something selected at all?
	//Index correct?
	if ( (nItemIndex < 0) || (nItemIndex >= m_tmGroup.GetSize()) )
	{
		nOldSelectedItem = -1;
		return NULL;
	}

	//Set old selected
	nOldSelectedItem = nItemIndex;

	//Get it from the array
	return &m_tmGroup[nItemIndex];
}


void CTextModulesDlg::UpdateNameInTree(const int Idx, CString NewName)
{
	ASSERT(Idx < m_wndModulesList.GetItemCount());

	//////////////////////////
	//Init Structure
	LVITEM lvi;
	lvi.mask = LVIF_TEXT | LVIF_PARAM; //| LVIF_IMAGE

	//Item specific stuff
	lvi.lParam = (LPARAM)Idx;
	lvi.iItem = Idx;

	//First Column
	lvi.pszText = NewName.GetBuffer(1);
	lvi.iSubItem = 0;
	m_wndModulesList.SetItem(&lvi);
}

void CTextModulesDlg::UpdateOldSelectedItem()
{
	//Retrieve data
	if ( (nOldSelectedItem > -1) && (nOldSelectedItem < m_tmGroup.GetSize()) )
	{
		UpdateData(true);

		m_tmGroup[nOldSelectedItem].SetText(m_strLeft, m_strRight);
		if (m_tmGroup[nOldSelectedItem].Name != m_strName)
		{
			CTextModule tm = m_tmGroup[nOldSelectedItem];

			tm.Name = m_strName;
			m_tmGroup.RemoveAt(nOldSelectedItem);
			int NewIdx = InsertNewModule(tm, m_strName);
			nOldSelectedItem = -1;
			UpdateTree(NewIdx);

			//UpdateNameInTree(nOldSelectedItem, m_tmGroup[nOldSelectedItem].Name);
		}
	}
}

void CTextModulesDlg::OnOK() 
{
	UpdateOldSelectedItem();

	CDialog::OnOK();
}

int CTextModulesDlg::InsertNewModule(CTextModule& tm, const CString& NewName)
{
	int i = 0;
	int NewIdx = m_tmGroup.InsertSorted(tm);
	while ( (NewIdx == -1) && (i<1000) )
	{
		i++;
		tm.Name.Format("%s (%d)", NewName, i);
		NewIdx = m_tmGroup.InsertSorted(tm);
	}

	return NewIdx;
}


void CTextModulesDlg::OnNewTextModule() 
{
	UpdateOldSelectedItem();

	//Create new Module
	CTextModule tm;

	CString NewName;
	NewName.LoadString(IDS_NEWTEXTMODULE);
	tm.Name = NewName;

	//Insert new module
	int NewIdx = InsertNewModule(tm, NewName);

	nOldSelectedItem = -1;

	UpdateTree(NewIdx);

	//Focus to Name Control
	m_wndNameEdit.SetFocus();
	m_wndNameEdit.SetSel(0, -1); //Select All
}

void CTextModulesDlg::OnDeleteTextModule() 
{
	GetSelectedModule();

	if ( (nOldSelectedItem >=0) && (nOldSelectedItem < m_tmGroup.GetSize()) )
	{
		int nOld = nOldSelectedItem;
		m_tmGroup.RemoveAt(nOldSelectedItem);
		nOldSelectedItem = -1;
		UpdateTree(nOld);
	}
}

void CTextModulesDlg::ClearControls()
{
	m_strLeft = "";
	m_strRight = "";
	m_strName = "";

	UpdateData(false);
}

void CTextModulesDlg::OnChangeName() 
{
	CString newName;
	LPTSTR buffer = newName.GetBuffer(200);
	buffer[199] = _T('\0'); //To be on the safe side
	int nCopied = m_wndNameEdit.GetLine(0, buffer, 199);
	newName.ReleaseBuffer(nCopied);
	if ( (nCopied == 0) || (nCopied > 199) ) return;

	UpdateNameInTree(nOldSelectedItem, newName);
	bNameChanged = true;
}

void CTextModulesDlg::OnKillFocusName() 
{
	//Did the name change?
	if (bNameChanged)
	{
		bNameChanged = false;
		UpdateOldSelectedItem();
	}
}


