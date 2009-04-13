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
#include "OptionPageFileClean.h"

#include "Configuration.h"
#include "RunTimeHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionPageFileClean property page

IMPLEMENT_DYNCREATE(COptionPageFileClean,CMFCPropertyPage)

COptionPageFileClean::COptionPageFileClean()
		: CMFCPropertyPage(COptionPageFileClean::IDD),
		m_PHInsertBtn(IDR_POPUP_PLACEHOLDEREX_NOOPT),
		m_bItemChangeLock(false),
		m_bUpdatingList(false),
		m_bConfirm(CConfiguration::GetInstance()->m_bFileCleanConfirm)
{
}

COptionPageFileClean::~COptionPageFileClean()
{
}

void COptionPageFileClean::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionPageFileClean)
	DDX_Control(pDX,IDC_OPTIONS_FILECLEAN_DELETE,m_DeleteBtn);
	DDX_Control(pDX,IDC_OPTIONS_FILECLEAN_PHINSERT,m_PHInsertBtn);
	DDX_Control(pDX,IDC_OPTIONS_FILECLEAN_RECURSIVE,m_RecursiveBtn);
	DDX_Control(pDX,IDC_OPTIONS_FILECLEAN_PATTERN,m_PatternEdit);
	DDX_Control(pDX,IDC_OPTIONS_FILECLEAN_NAME,m_NameEdit);
	DDX_Control(pDX,IDC_OPTIONS_FILECLEAN_HANDLING,m_HandleBox);
	DDX_Control(pDX,IDC_OPTIONS_FILECLEAN_LIST,m_wndList);
	DDX_Check(pDX,IDC_OPTIONS_FILECLEAN_CONFIRM,m_bConfirm);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COptionPageFileClean,CMFCPropertyPage)
	//{{AFX_MSG_MAP(COptionPageFileClean)
	ON_EN_CHANGE(IDC_OPTIONS_FILECLEAN_NAME,OnChangeName)
	ON_EN_CHANGE(IDC_OPTIONS_FILECLEAN_PATTERN,OnChangePattern)
	ON_CBN_SELCHANGE(IDC_OPTIONS_FILECLEAN_HANDLING,OnSelchangeHandling)
	ON_BN_CLICKED(IDC_OPTIONS_FILECLEAN_RECURSIVE,OnRecursive)
	ON_BN_CLICKED(IDC_OPTIONS_FILECLEAN_DELETE,OnDelete)
	ON_BN_CLICKED(IDC_OPTIONS_FILECLEAN_NEW,OnNew)
	ON_EN_KILLFOCUS(IDC_OPTIONS_FILECLEAN_PATTERN,OnLeavePattern)
	ON_NOTIFY(LVN_ITEMCHANGED,IDC_OPTIONS_FILECLEAN_LIST,OnListItemchanged)
	//}}AFX_MSG_MAP
	ON_NOTIFY(LVN_DELETEITEM, IDC_OPTIONS_FILECLEAN_LIST, &COptionPageFileClean::OnLvnDeleteitemOptionsFilecleanList)
	ON_NOTIFY(LVN_KEYDOWN, IDC_OPTIONS_FILECLEAN_LIST, &COptionPageFileClean::OnLvnKeydownOptionsFilecleanList)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionPageFileClean message handlers

void COptionPageFileClean::OnOK()
{
	UpdateData();

	//Store settings to configuration
	CConfiguration::GetInstance()->m_bFileCleanConfirm = m_bConfirm != 0;

	//Copy the array
	CConfiguration::GetInstance()->m_aFileCleanItems.RemoveAll();

	const int count = m_wndList.GetItemCount();

	for (int i = 0; i < count; ++i) {
		CFileCleanItem* item = reinterpret_cast<CFileCleanItem*>(m_wndList.GetItemData(i));
		CConfiguration::GetInstance()->m_aFileCleanItems.InsertSorted(*item,true,true);
	}

	//Just to be sure
	CConfiguration::GetInstance()->m_aFileCleanItems.AddDefaultItems();

	CMFCPropertyPage::OnOK();
}

LVITEM COptionPageFileClean::GetLVITEM()
{
	LVITEM lvi;
	lvi.mask = LVIF_TEXT | LVIF_PARAM; //| LVIF_IMAGE

	return lvi;
}

void COptionPageFileClean::Populate()
{
	//Not valid?
	if (!IsWindow(m_wndList.m_hWnd))
		return;

	//Remember the selected entry
	int nLastSelected = m_wndList.GetNextItem(-1,LVNI_SELECTED);

	if (nLastSelected < 0) 
		nLastSelected = 0;

	//////////////////////////
	//Clear List
	m_wndList.SetRedraw(FALSE);
	m_wndList.DeleteAllItems();

	const CFileCleanItemArray& items = CConfiguration::GetInstance()->m_aFileCleanItems;

	if (items.GetSize() > 0) {
		//////////////////////////
		//Init Structure

		LVITEM lvi = GetLVITEM();
		
		//////////////////////////
		//Fill List
		m_bUpdatingList = true; //Lock
		int size = items.GetSize();

		for (int i = 0; i < size; i++)
			InsertItem(i,lvi,items[i]);

		m_bUpdatingList = false; //Unlock

		//Restore the last selected Item
		if (!m_wndList.SetItemState(nLastSelected,LVNI_SELECTED,LVNI_SELECTED))
		{
			//Happens, if the entry could not be found (last entry deleted for example)
			m_wndList.SetItemState(0,LVNI_SELECTED,LVNI_SELECTED);
		}

		UpdateControls();

		//Get the focus!
		m_wndList.SetFocus();
	}

	m_wndList.SetRedraw(TRUE);
}

namespace {
	template<class T>
	int LexicographicalCompare(const T& a, const T& b)
	{
		return a < b ? -1 : a > b ? 1 : 0;
	}
}

int COptionPageFileClean::CompareDescription(LPARAM l1, LPARAM l2)
{
	const CFileCleanItem* i1 = reinterpret_cast<const CFileCleanItem*>(l1);
	const CFileCleanItem* i2 = reinterpret_cast<const CFileCleanItem*>(l2);

	return i1->GetDescription().CompareNoCase(i2->GetDescription());
}

int COptionPageFileClean::ComparePattern(LPARAM l1, LPARAM l2)
{
	const CFileCleanItem* i1 = reinterpret_cast<const CFileCleanItem*>(l1);
	const CFileCleanItem* i2 = reinterpret_cast<const CFileCleanItem*>(l2);

	return i1->GetPattern().CompareNoCase(i2->GetPattern());
}

int COptionPageFileClean::CompareRecursive(LPARAM l1, LPARAM l2)
{
	const CFileCleanItem* i1 = reinterpret_cast<const CFileCleanItem*>(l1);
	const CFileCleanItem* i2 = reinterpret_cast<const CFileCleanItem*>(l2);

	return LexicographicalCompare(i1->IsRecursive(),i2->IsRecursive());
}

int COptionPageFileClean::CompareAction(LPARAM l1, LPARAM l2)
{
	const CFileCleanItem* i1 = reinterpret_cast<const CFileCleanItem*>(l1);
	const CFileCleanItem* i2 = reinterpret_cast<const CFileCleanItem*>(l2);

	return LexicographicalCompare(i1->GetFileHandling(),i2->GetFileHandling());
}

BOOL COptionPageFileClean::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();

	//Initialize controls
	m_PHInsertBtn.AttachEditCtrl(&m_PatternEdit);
	//	m_images.Create( IDB_ITEM_TYPES, 16, 0, RGB( 0, 128, 128 ) );
	//	m_wndList.SetImageList(&m_images, TVSIL_NORMAL);
	//	m_wndList.SetImageList(&m_images, TVSIL_STATE);

	//Change Style
	m_wndList.SetExtendedStyle(m_wndList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER); //| LVS_EX_GRIDLINES

	if (RunTimeHelper::IsVista())
		::SetWindowTheme(m_wndList,L"explorer",0);

	//Create the Columns
	CString strAdd;
	strAdd.LoadString(STE_FILECLEAN_HEAD1);
	m_wndList.InsertColumn(0,strAdd,LVCFMT_LEFT,95,-1);

	strAdd.LoadString(STE_FILECLEAN_HEAD2);
	m_wndList.InsertColumn(1,strAdd,LVCFMT_LEFT,55,1);

	strAdd.LoadString(STE_FILECLEAN_HEAD3);
	m_wndList.InsertColumn(2,strAdd,LVCFMT_LEFT,75,2);

	strAdd.LoadString(STE_FILECLEAN_HEAD4);
	m_wndList.InsertColumn(3,strAdd,LVCFMT_LEFT,85,3);

	//Add some text to the combobox
	m_HandleBox.ResetContent(); //Just to be sure...
	strAdd.LoadString(STE_FILECLEAN_HNONE);
	m_HandleBox.AddString(strAdd);
	strAdd.LoadString(STE_FILECLEAN_HCLEAN);
	m_HandleBox.AddString(strAdd);
	strAdd.LoadString(STE_FILECLEAN_HPROTECT);
	m_HandleBox.AddString(strAdd);

	m_wndList.SetColumnCompareFunction(0,&COptionPageFileClean::CompareDescription);
	m_wndList.SetColumnCompareFunction(1,&COptionPageFileClean::ComparePattern);
	m_wndList.SetColumnCompareFunction(2,&COptionPageFileClean::CompareRecursive);
	m_wndList.SetColumnCompareFunction(3,&COptionPageFileClean::CompareAction);

	//Add items to the list
	Populate();

	m_wndList.Sort(0,true);

	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void COptionPageFileClean::UpdateControls()
{
	//Get the selected item
	CFileCleanItem* pItem = GetSelectedFCItem();
	if (!pItem)
	{
		UpdateControlsState(false);
		return; //Nothing selected or whatever
	}

	//Is it a default Item?
	if (ShouldBeDisabled(*pItem))
	{
		//Read it and set the Controls
		m_NameEdit.SetWindowText(pItem->GetDescription());
		m_PatternEdit.SetWindowText(pItem->GetPattern());
		m_RecursiveBtn.SetCheck(pItem->IsRecursive());
		m_HandleBox.SetCurSel(-1);

		//Deactivate
		UpdateControlsState(false);
	}
	else
	{
		//Read it and set the Controls
		m_NameEdit.SetWindowText(pItem->GetDescription());
		m_PatternEdit.SetWindowText(pItem->GetPattern());
		m_RecursiveBtn.SetCheck(pItem->IsRecursive());
		m_HandleBox.SetCurSel(pItem->GetFileHandling());

		//Activate
		UpdateControlsState(true);
	}

	return;
}

void COptionPageFileClean::UpdateControlsState(bool bEnable /*= true*/)
{
	//(De)Activate
	m_NameEdit.EnableWindow(bEnable);
	m_PatternEdit.EnableWindow(bEnable);
	m_RecursiveBtn.EnableWindow(bEnable);
	m_HandleBox.EnableWindow(bEnable);
	m_PHInsertBtn.EnableWindow(bEnable);
	m_DeleteBtn.EnableWindow(bEnable);
}

void COptionPageFileClean::OnChangeName()
{
	//	//Locked?
	//	if (m_bUpdatingList) return;

	//Get the selected item
	CFileCleanItem* pItem = GetSelectedFCItem();
	if (!pItem) return; //Nothing selected or whatever

	CString text;
	m_NameEdit.GetWindowText(text);

	pItem->SetDescription(text);

	UpdateSelectedListItem();
}

void COptionPageFileClean::OnChangePattern()
{
	//Locked?
	if (m_bUpdatingList) return;

	//Get the selected item
	CFileCleanItem* pItem = GetSelectedFCItem();
	if (!pItem) return; //Nothing selected or whatever

	CString text;
	m_PatternEdit.GetWindowText(text);
	pItem->SetPattern(text);

	UpdateSelectedListItem();
}

void COptionPageFileClean::OnSelchangeHandling()
{
	//Locked?
	if (m_bUpdatingList) return;

	//Get the selected item
	CFileCleanItem* pItem = GetSelectedFCItem();
	if (!pItem) return; //Nothing selected or whatever

	//NOTE: Casting an integer to an enum-type is legal, but results are undefined. Thats why:
	int tempFH;
	tempFH = m_HandleBox.GetCurSel();
	//if (tempFH < 0) return; //Shit happened
	switch (tempFH)
	{

		case(int)(CFileCleanItem::clean) :
		{
			pItem->SetFileHandling(CFileCleanItem::clean);
			break;
		}

		case(int)(CFileCleanItem::protect) :
		{
			pItem->SetFileHandling(CFileCleanItem::protect);
			break;
		}

		case(int)(CFileCleanItem::protectbydefault) :
		{
			ASSERT(false); //Should not happen; Default Items do not go to the registry
			break;
		}

		default:
			pItem->SetFileHandling(CFileCleanItem::none);
	}

	UpdateSelectedListItem();
}

void COptionPageFileClean::OnRecursive()
{
	//Locked?
	if (m_bUpdatingList) return;

	//Get the selected item
	CFileCleanItem* pItem = GetSelectedFCItem();
	if (!pItem) return; //Nothing selected or whatever

	pItem->SetRecursive(m_RecursiveBtn.GetCheck() != 0);

	UpdateSelectedListItem();
}

CFileCleanItem* COptionPageFileClean::GetSelectedFCItem(int* index)
{
	//Get the selected item
	int nItemIndex = m_wndList.GetNextItem(-1,LVNI_SELECTED);

	if (index)
		*index = nItemIndex;

	//Something selected at all?
	if (nItemIndex < 0) return NULL;

	//Index correct?
	return reinterpret_cast<CFileCleanItem*>(m_wndList.GetItemData(nItemIndex));
}

void COptionPageFileClean::UpdateSelectedListItem()
{
	int index;
	CFileCleanItem* pItem = GetSelectedFCItem(&index);

	if (!pItem) return;

	//Lock
	m_bUpdatingList = true;

	//First Column
	m_wndList.SetItemText(index,0,pItem->GetDescription());

	//Second Column
	m_wndList.SetItemText(index,1,pItem->GetPattern());

	//Third Column
	CString strRecursive;
	strRecursive.LoadString(pItem->IsRecursive() ? STE_FILECLEAN_RECURSIVE : STE_FILECLEAN_NOTRECURSIVE);

	m_wndList.SetItemText(index,2,strRecursive);

	//Fourth Column
	CString strHandling = GetFileHandlingName(pItem);
	m_wndList.SetItemText(index,3,strHandling);

	//Unlock
	m_bUpdatingList = false;
}

CString COptionPageFileClean::GetFileHandlingName(CFileCleanItem* pItem)
{
	//Get the FileHandling type
	CString strHandling;
	switch (pItem->GetFileHandling())
	{
		case CFileCleanItem::none :
		{
			strHandling.LoadString(STE_FILECLEAN_HNONE);
			//lvi.iImage = 1;
			break;
		}

		case CFileCleanItem::clean :
		{
			strHandling.LoadString(STE_FILECLEAN_HCLEAN);
			//lvi.iImage = 2;
			break;
		}

		case CFileCleanItem::protect :
		{
			strHandling.LoadString(STE_FILECLEAN_HPROTECT);
			//lvi.iImage = 3;
			break;
		}

		case CFileCleanItem::protectbydefault :
		{
			strHandling.LoadString(STE_FILECLEAN_HPROTECTBYDEFAULT);
			//lvi.iImage = 4;
			break;
		}

		default:
			ASSERT(false); //Should never happen
			strHandling.LoadString(STE_FILECLEAN_HNONE);
	}

	return strHandling;
}

void COptionPageFileClean::OnListItemchanged(NMHDR* pNMHDR,LRESULT* pResult)
{
	//Locked?
	if (m_bItemChangeLock)
	{
		return;
	}

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	//Did the state change?
	if (pNMListView->uChanged & LVIF_STATE)
	{
		//Was this Item selected or de-selected
		// - We're considering only selected Items
		if (pNMListView->uNewState & LVIS_SELECTED)
		{
			//Lock this event. Should not be considered, while executing UpdateControls
			m_bItemChangeLock = true;
			UpdateControls();
			m_bItemChangeLock = false;
		}
	}

	*pResult = 0;
}

void COptionPageFileClean::OnDelete()
{
	//Get the selected item
	int nItemIndex = m_wndList.GetNextItem(-1,LVNI_SELECTED);

	//Something selected at all?
	if (nItemIndex < 0)
	{
		ASSERT(false); //Should not happen; Btn should be disabled
		return; //Nothing selected or whatever
	}

	//Default Item? Not to be deleted
	if (reinterpret_cast<CFileCleanItem*>(m_wndList.GetItemData(nItemIndex))->GetFileHandling() == CFileCleanItem::protectbydefault)
	{
		ASSERT(false); //Should not happen; Btn should be disabled
		return;
	}

	m_wndList.DeleteItem(nItemIndex);
}

void COptionPageFileClean::OnNew()
{
	CFileCleanItem newItem;

	//Get the selected item
	CFileCleanItem* pItem = GetSelectedFCItem();

	if (pItem)
	{
		//We try to make the new Item similar to the selected one
		newItem.SetDescription(CString(MAKEINTRESOURCE(STE_FILECLEAN_HEAD1))); //= pItem->GetDescription();
		newItem.SetPattern(pItem->GetPattern());
		newItem.SetFileHandling((pItem->GetFileHandling() == CFileCleanItem::protectbydefault) ? CFileCleanItem::protect : pItem->GetFileHandling());
		newItem.SetRecursive(pItem->IsRecursive());
	}
	else
	{
		//We create a standard
		newItem.SetDescription(CString(MAKEINTRESOURCE(STE_FILECLEAN_HEAD1))); //= pItem->GetDescription();
		newItem.SetFileHandling(CFileCleanItem::none);
		newItem.SetRecursive(false);
	}

	const int index = 0;

	//Add it at the top of the array
	InsertItem(index,newItem);

	//Select the first entry
	m_wndList.SetItemState(index,LVNI_SELECTED,LVNI_SELECTED);
	UpdateControls();

	//Set the Focus to the description field
	m_NameEdit.SetFocus();
	m_NameEdit.SetSel(0,-1,true);
}

void COptionPageFileClean::OnLeavePattern()
{
	CFileCleanItem TestItem;

	CString text;
	m_PatternEdit.GetWindowText(text);
	TestItem.SetPattern(text);

	//Test if valid
	if (!TestItem.PatternIsValid())
	{
		//Not valid: Show Message to the user
		CString strMsg;
		strMsg.Format(STE_FILECLEAN_INVALIDPATTERN,TestItem.GetPattern());
		AfxMessageBox(strMsg,MB_OK | MB_ICONSTOP,0);

		//Get focus back
		//m_PatternEdit.SetFocus();

		//Mostly we are just informing the user here.
		//I do not want to cancel actions like pressing the Ok-Button etc., which
		// will happen, if we take the focus back. Now it happens only for the
		// first try (to close the dialog).
		//The validity of a pattern will be checked before deleting files anyway.
	}
}


void COptionPageFileClean::OnLvnDeleteitemOptionsFilecleanList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	delete reinterpret_cast<CFileCleanItem*>(m_wndList.GetItemData(pNMLV->iItem));

	*pResult = 0;
}


void COptionPageFileClean::InsertItem(int index, const CFileCleanItem& item)
{
	LVITEM lvi = GetLVITEM();
	InsertItem(index,lvi,item);
}

void COptionPageFileClean::InsertItem( int index, LVITEM &lvi, const CFileCleanItem& a )
{
	CFileCleanItem* item = new CFileCleanItem(a);

	//Item specific stuff
	lvi.lParam = reinterpret_cast<LPARAM>(item);
	lvi.iItem = index;

	//First Column
	const_cast<LPCTSTR&>(lvi.pszText) = item->GetDescription();
	lvi.iSubItem = 0;
	m_wndList.InsertItem(&lvi);

	//Second Column
	m_wndList.SetItemText(index,1,item->GetPattern());

	//Third Column
	CString strRecursive;
	strRecursive.LoadString(item->IsRecursive() ? STE_FILECLEAN_RECURSIVE : STE_FILECLEAN_NOTRECURSIVE);
	m_wndList.SetItemText(index,2,strRecursive);

	//Fourth Column
	m_wndList.SetItemText(index,3,GetFileHandlingName(item));
}

void COptionPageFileClean::OnLvnKeydownOptionsFilecleanList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	
	switch (pLVKeyDow->wVKey) {
		case _T('A'):
			if (::GetKeyState(VK_CONTROL) >> 15 & 1)
				m_wndList.SetItemState(-1,LVIS_SELECTED,LVIS_SELECTED);
			break;
		case VK_DELETE:
			{
				m_wndList.SetRedraw(FALSE);

				int index;

				while ((index = m_wndList.GetNextItem(-1,LVNI_SELECTED|LVNI_ALL)) != -1) 
				{
					if (!ShouldBeDisabled(*reinterpret_cast<CFileCleanItem*>(m_wndList.GetItemData(index))))
						m_wndList.DeleteItem(index);
				}

				m_wndList.SetRedraw(TRUE);
			}
			break;
	}

	*pResult = 0;
}

bool COptionPageFileClean::ShouldBeDisabled(const CFileCleanItem& item)
{
	return item.GetFileHandling() == CFileCleanItem::protectbydefault;
}