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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionPageFileClean property page

IMPLEMENT_DYNCREATE(COptionPageFileClean, CPropertyPage)

COptionPageFileClean::COptionPageFileClean()
: CPropertyPage(COptionPageFileClean::IDD),
	m_PHInsertBtn(IDR_POPUP_PLACEHOLDEREX_NOOPT)
{
	//{{AFX_DATA_INIT(COptionPageFileClean)
	//}}AFX_DATA_INIT

	//Read from the config
	m_bConfirm = g_configuration.m_bFileCleanConfirm;

	m_bUpdatingList = false;
//	m_bUserListEvent = false;
}

COptionPageFileClean::~COptionPageFileClean()
{
}

void COptionPageFileClean::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionPageFileClean)
	DDX_Control(pDX, IDC_OPTIONS_FILECLEAN_DELETE, m_DeleteBtn);
	DDX_Control(pDX, IDC_OPTIONS_FILECLEAN_PHINSERT, m_PHInsertBtn);
	DDX_Control(pDX, IDC_OPTIONS_FILECLEAN_RECURSIVE, m_RecursiveBtn);
	DDX_Control(pDX, IDC_OPTIONS_FILECLEAN_PATTERN, m_PatternEdit);
	DDX_Control(pDX, IDC_OPTIONS_FILECLEAN_NAME, m_NameEdit);
	DDX_Control(pDX, IDC_OPTIONS_FILECLEAN_HANDLING, m_HandleBox);
	DDX_Control(pDX, IDC_OPTIONS_FILECLEAN_LIST, m_wndList);
	DDX_Check(pDX, IDC_OPTIONS_FILECLEAN_CONFIRM, m_bConfirm);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionPageFileClean, CPropertyPage)
	//{{AFX_MSG_MAP(COptionPageFileClean)
	ON_EN_CHANGE(IDC_OPTIONS_FILECLEAN_NAME, OnChangeName)
	ON_EN_CHANGE(IDC_OPTIONS_FILECLEAN_PATTERN, OnChangePattern)
	ON_CBN_SELCHANGE(IDC_OPTIONS_FILECLEAN_HANDLING, OnSelchangeHandling)
	ON_BN_CLICKED(IDC_OPTIONS_FILECLEAN_RECURSIVE, OnRecursive)
	ON_NOTIFY(NM_CLICK, IDC_OPTIONS_FILECLEAN_LIST, OnListEvent)
	ON_NOTIFY(NM_DBLCLK, IDC_OPTIONS_FILECLEAN_LIST, OnListEvent)
	ON_NOTIFY(NM_RCLICK, IDC_OPTIONS_FILECLEAN_LIST, OnListEvent)
	ON_NOTIFY(NM_RDBLCLK, IDC_OPTIONS_FILECLEAN_LIST, OnListEvent)
	ON_NOTIFY(NM_RETURN, IDC_OPTIONS_FILECLEAN_LIST, OnListEvent)
	ON_NOTIFY(NM_SETFOCUS, IDC_OPTIONS_FILECLEAN_LIST, OnListEvent)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_OPTIONS_FILECLEAN_LIST, OnListEvent)
	ON_BN_CLICKED(IDC_OPTIONS_FILECLEAN_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_OPTIONS_FILECLEAN_NEW, OnNew)
	ON_BN_CLICKED(IDC_OPTIONS_FILECLEAN_SORT, OnSort)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionPageFileClean message handlers

void COptionPageFileClean::OnOK() 
{
	UpdateData();

	//Store settings to configuration
	g_configuration.m_bFileCleanConfirm = m_bConfirm;

	//Copy the array
	g_configuration.m_aFileCleanItems.RemoveAll();
	int i;
	for(i=0;i<m_ItemArray.GetSize();i++)
	{
		g_configuration.m_aFileCleanItems.InsertSorted(m_ItemArray.GetAt(i), true, true);
	}
	//Just to be sure
	g_configuration.m_aFileCleanItems.AddDefaultItems();

	CPropertyPage::OnOK();
}

void COptionPageFileClean::UpdateItems(const CFileCleanItemArray& copyItems)
{
	//Copy the array
	m_ItemArray.RemoveAll();
	int i;
	for(i=0;i<copyItems.GetSize();i++)
	{
		m_ItemArray.InsertSorted(copyItems.GetAt(i), true, true);
	}
}

void COptionPageFileClean::RefillList()
{
	//Not valid?
	if (!IsWindow(m_wndList.m_hWnd))
		return;

	//Remember the selected entry
	int nLastSelected = m_wndList.GetNextItem(-1, LVNI_SELECTED);
	if (nLastSelected < 0) nLastSelected = 0;

	//////////////////////////
	//Clear List
	m_wndList.DeleteAllItems();

	if (m_ItemArray.GetSize() == 0)
		return;

	//////////////////////////
	//Init Structure
	LVITEM lvi;
	lvi.mask = LVIF_TEXT | LVIF_PARAM; //| LVIF_IMAGE

	//////////////////////////
	//Fill List
	m_bUpdatingList = true; //Lock
	int i;
	int nsize = m_ItemArray.GetSize();
	for(i=0;i<nsize;i++)
	{
		//Item specific stuff
		lvi.lParam = (LPARAM)i;
		lvi.iItem = i;

		//First Column
		lvi.pszText = m_ItemArray[i].strDescription.GetBuffer(1);
		lvi.iSubItem = 0;
		m_wndList.InsertItem(&lvi);

		//Second Column
		m_wndList.SetItemText(i, 1, m_ItemArray[i].strPattern.GetBuffer(1));

		//Third Column
		CString strRecursive;
		strRecursive.LoadString(m_ItemArray[i].bRecursive ? STE_FILECLEAN_RECURSIVE : STE_FILECLEAN_NOTRECURSIVE);
		m_wndList.SetItemText(i, 2, strRecursive.GetBuffer(1));

		//Fourth Column
		CString strHandling = GetFileHandlingName(&m_ItemArray[i]);
		m_wndList.SetItemText(i, 3, strHandling.GetBuffer(1));
	}

	m_bUpdatingList = false; //Unlock

	//Restore the last selected Item
	if (!m_wndList.SetItemState(nLastSelected, LVNI_SELECTED, LVNI_SELECTED))
	{
		//Happens, if the entry could not be found (last entry deleted for example)
		m_wndList.SetItemState(0, LVNI_SELECTED, LVNI_SELECTED);
	}

	UpdateControls();

	//Get the focus!
	m_wndList.SetFocus();
}


BOOL COptionPageFileClean::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	//Initialize controls
	m_PHInsertBtn.AttachEditCtrl(&m_PatternEdit);
//	m_images.Create( IDB_ITEM_TYPES, 16, 0, RGB( 0, 128, 128 ) );
//	m_wndList.SetImageList(&m_images, TVSIL_NORMAL);
//	m_wndList.SetImageList(&m_images, TVSIL_STATE);


	//Change Style
	m_wndList.SetExtendedStyle(m_wndList.GetExtendedStyle() | LVS_EX_FULLROWSELECT); //| LVS_EX_GRIDLINES

	//Create the Columns
	CString strAdd;
	strAdd.LoadString(STE_FILECLEAN_HEAD1);
	m_wndList.InsertColumn(0, strAdd, LVCFMT_LEFT, 95, -1);

	strAdd.LoadString(STE_FILECLEAN_HEAD2);
	m_wndList.InsertColumn(1, strAdd, LVCFMT_LEFT, 55, 1);
	
	strAdd.LoadString(STE_FILECLEAN_HEAD3);
	m_wndList.InsertColumn(2, strAdd, LVCFMT_LEFT, 75, 2);
	
	strAdd.LoadString(STE_FILECLEAN_HEAD4);
	m_wndList.InsertColumn(3, strAdd, LVCFMT_LEFT, 85, 3);

	//Add some text to the combobox
	m_HandleBox.ResetContent(); //Just to be sure...
	strAdd.LoadString(STE_FILECLEAN_HNONE);
	m_HandleBox.AddString(strAdd);
	strAdd.LoadString(STE_FILECLEAN_HCLEAN);
	m_HandleBox.AddString(strAdd);
	strAdd.LoadString(STE_FILECLEAN_HPROTECT);
	m_HandleBox.AddString(strAdd);

	//Copy Data	from config
	UpdateItems(g_configuration.m_aFileCleanItems);

	//Add items to the list
	RefillList();

	return TRUE;  // return TRUE unless you set the focus to a control
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
	if (pItem->tFileHandling == CFileCleanItem::protectbydefault)
	{
		//Read it and set the Controls
		m_NameEdit.SetWindowText(pItem->strDescription);
		m_PatternEdit.SetWindowText(pItem->strPattern);
		m_RecursiveBtn.SetCheck(pItem->bRecursive);
		m_HandleBox.SetCurSel(-1);

		//Deactivate
		UpdateControlsState(false);
	}
	else
	{
		//Read it and set the Controls
		m_NameEdit.SetWindowText(pItem->strDescription);
		m_PatternEdit.SetWindowText(pItem->strPattern);
		m_RecursiveBtn.SetCheck(pItem->bRecursive);
		m_HandleBox.SetCurSel(pItem->tFileHandling);

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

	m_NameEdit.GetWindowText(pItem->strDescription);

//	//User ListEvent?
//	if (!m_bUserListEvent)
		UpdateSelectedListItem();
}

void COptionPageFileClean::OnChangePattern() 
{
	//Locked?
	if (m_bUpdatingList) return;

	//Get the selected item
	CFileCleanItem* pItem = GetSelectedFCItem();
	if (!pItem) return; //Nothing selected or whatever

	//TODO: Validate Pattern! using PatternIsValid()

	m_PatternEdit.GetWindowText(pItem->strPattern);

//	//User ListEvent?
//	if (!m_bUserListEvent)
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
		case (int)(CFileCleanItem::clean):
			{
				pItem->tFileHandling = CFileCleanItem::clean;
				break;
			}

		case (int)(CFileCleanItem::protect):
			{
				pItem->tFileHandling = CFileCleanItem::protect;
				break;
			}

		case (int)(CFileCleanItem::protectbydefault):
			{
				ASSERT(false); //Should not happen; Default Items do not go to the registry
				break;
			}

		default:
			pItem->tFileHandling = CFileCleanItem::none;
	}

//	//User ListEvent?
//	if (!m_bUserListEvent)
		UpdateSelectedListItem();
}

void COptionPageFileClean::OnRecursive()
{
	//Locked?
	if (m_bUpdatingList) return;

	//Get the selected item
	CFileCleanItem* pItem = GetSelectedFCItem();
	if (!pItem) return; //Nothing selected or whatever

	pItem->bRecursive = (bool)m_RecursiveBtn.GetCheck();

//	//User ListEvent?
//	if (!m_bUserListEvent)
		UpdateSelectedListItem();
}

CFileCleanItem* COptionPageFileClean::GetSelectedFCItem()
{
	//Get the selected item
	int nItemIndex = m_wndList.GetNextItem(-1, LVNI_SELECTED);

	//Something selected at all?
	if (nItemIndex < 0) return NULL;
	//Index correct?
	if (nItemIndex >= m_ItemArray.GetSize()) return NULL;

	//Get it from the array
	return &m_ItemArray[nItemIndex];
}

void COptionPageFileClean::UpdateSelectedListItem()
{
	//Get the selected item
	int nItemIndex = m_wndList.GetNextItem(-1, LVNI_SELECTED);

	//Something selected at all?
	if (nItemIndex < 0) return;
	//Index correct?
	if (nItemIndex >= m_ItemArray.GetSize()) return;

	//Get it from the array
	CFileCleanItem* pItem = &m_ItemArray[nItemIndex];
	if (!pItem) return;

	//Lock
	m_bUpdatingList = true;

	//First Column
	m_wndList.SetItemText(nItemIndex, 0, pItem->strDescription.GetBuffer(1));

	//Second Column
	m_wndList.SetItemText(nItemIndex, 1, pItem->strPattern.GetBuffer(1));

	//Third Column
	CString strRecursive;
	strRecursive.LoadString(pItem->bRecursive ? STE_FILECLEAN_RECURSIVE : STE_FILECLEAN_NOTRECURSIVE);
	m_wndList.SetItemText(nItemIndex, 2, strRecursive.GetBuffer(1));

	//Fourth Column
	CString strHandling = GetFileHandlingName(pItem);
	m_wndList.SetItemText(nItemIndex, 3, strHandling.GetBuffer(1));

	//Unlock
	m_bUpdatingList = false;
}


CString COptionPageFileClean::GetFileHandlingName(CFileCleanItem* pItem)
{
	//Get the FileHandling type
	CString strHandling;
	switch (pItem->tFileHandling)
	{
	case CFileCleanItem::none:
		{
			strHandling.LoadString(STE_FILECLEAN_HNONE);
			//lvi.iImage = 1;
			break;
		}

	case CFileCleanItem::clean:
		{
			strHandling.LoadString(STE_FILECLEAN_HCLEAN);
			//lvi.iImage = 2;
			break;
		}

	case CFileCleanItem::protect:
		{
			strHandling.LoadString(STE_FILECLEAN_HPROTECT);
			//lvi.iImage = 3;
			break;
		}

	case CFileCleanItem::protectbydefault:
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

void COptionPageFileClean::OnListEvent(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//TODO: I can't figure out how to get LVN_ITEMACTIVATE.
	//I tried OnNotify(), but it didn't work cause the NMHDR was not vaild.
	//At least it did not contain, what I wanted. I tried NMITEMACTIVATE as well.
	//... I am somewhat better with the pure SDK...

//	m_bUserListEvent = false;
	UpdateControls();
//	m_bUserListEvent = false;
	
	*pResult = 0;
}

void COptionPageFileClean::OnDelete() 
{
	//Get the selected item
	int nItemIndex = m_wndList.GetNextItem(-1, LVNI_SELECTED);

	//Something selected at all?
	if (nItemIndex < 0)
	{
		ASSERT(false); //Should not happen; Btn should be disabled
		return; //Nothing selected or whatever
	}

	//Index correct?
	if (nItemIndex >= m_ItemArray.GetSize())
	{
		ASSERT(false); //Should not happen
		return; //Nothing selected or whatever
	}

	//Default Item? Not to be deleted
	if (m_ItemArray.GetAt(nItemIndex).tFileHandling == CFileCleanItem::protectbydefault)
	{
		ASSERT(false); //Should not happen; Btn should be disabled
		return;
	}

	//Remove and Refill
	m_ItemArray.RemoveAt(nItemIndex, 1);
	RefillList();
}

void COptionPageFileClean::OnNew() 
{
	CFileCleanItem newItem;

	//Get the selected item
	CFileCleanItem* pItem = GetSelectedFCItem();
	if (pItem)
	{
		//We try to make the new Item similar to the selected one
		newItem.strDescription.LoadString(STE_FILECLEAN_HEAD1); //= pItem->strDescription;
		newItem.strPattern = pItem->strPattern;
		newItem.tFileHandling = (pItem->tFileHandling == CFileCleanItem::protectbydefault) ? CFileCleanItem::protect : pItem->tFileHandling;
		newItem.bRecursive = pItem->bRecursive;
	}
	else
	{
		//We create a standard
		newItem.strDescription.LoadString(STE_FILECLEAN_HEAD1); //= pItem->strDescription;
		newItem.strPattern = "";
		newItem.tFileHandling = CFileCleanItem::none;
		newItem.bRecursive = false;
	}

	//Add it at the top of the array
	m_ItemArray.InsertAt(0, newItem);
	//Update list
	RefillList();
	//Select the first entry
	m_wndList.SetItemState(0, LVNI_SELECTED, LVNI_SELECTED);
	UpdateControls();
	//Set the Focus to the description field
	m_NameEdit.SetFocus();
	m_NameEdit.SetSel(0, -1, true);
}

void COptionPageFileClean::OnSort() 
{
	//We need to build up a new array and assign it back to our member array
	CFileCleanItemArray tempArray;

	//Copy the array
	tempArray.RemoveAll();
	int i;
	for(i=0;i<m_ItemArray.GetSize();i++)
	{
		tempArray.InsertSorted(m_ItemArray.GetAt(i), true, true);
	}

	//Copy Back
	UpdateItems(tempArray);

	//User Interface updaten
	RefillList();
}
