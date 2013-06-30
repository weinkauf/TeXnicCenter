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

#include <set>

#include "resource.h"
#include "Global.h"
#include "RunTimeHelper.h"
#include "TemplateDialog.h"
#include "TeXnicCenter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// CTemplateItem
//-------------------------------------------------------------------

CTemplateItem::CTemplateItem()
{
}

CTemplateItem::~CTemplateItem()
{
}

//-------------------------------------------------------------------
// CTemplateDialog
//-------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CTemplateDialog, CDialog)
	//{{AFX_MSG_MAP(CTemplateDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CATEGORIES, &CTemplateDialog::OnSelchangeTabCategories)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TEMPLATES, &CTemplateDialog::OnTemplateItemChanged)
	ON_BN_CLICKED(IDC_BUTTON_CREATE, &CTemplateDialog::OnCreate)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_TEMPLATES, &CTemplateDialog::OnDblClkTemplate)
	ON_BN_CLICKED(IDC_RADIO_VIEW_TYPE, &CTemplateDialog::OnViewTypeSelection)
	ON_BN_CLICKED(IDC_RADIO_VIEW_TYPE1, &CTemplateDialog::OnViewTypeSelection)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CTemplateDialog::CTemplateDialog(UINT unIDTemplate, CWnd* pParent /*=NULL*/)
		: CDialog(unIDTemplate, pParent),
		m_nFirstTab(0), m_nLastTab(0)
{
	m_ImageList32.Create(32, 32, ILC_COLOR32, 1, 1);
	m_ImageList16.Create(16, 16, ILC_COLOR32, 1, 1);

	//{{AFX_DATA_INIT(CTemplateDialog)
	//}}AFX_DATA_INIT

	m_nListViewType = theApp.GetProfileInt(_T("Settings"), _T("TemplateListViewType"), lvtIcons);
}

void CTemplateDialog::AddSearchDir(LPCTSTR lpszSearchDir)
{
	m_astrSearchPaths.Add(lpszSearchDir);
}

void CTemplateDialog::AddTemplateFilter(LPCTSTR lpszFilter, CRuntimeClass *pTemplateItemClass)
{
	m_astrFilters.Add(lpszFilter);
	m_apTemplateItemClass.Add(pTemplateItemClass);
}

void CTemplateDialog::AddTemplateItem(LPCTSTR lpszCategory, std::unique_ptr<CTemplateItem>&& pItem)
{
	if (!pItem->InitItem(NULL, m_ImageList32, m_ImageList16))
		return;

	StringTemplateItemArrayMap::iterator it = m_mapSubdirToTemplates.find(lpszCategory);

	if (it == m_mapSubdirToTemplates.end())
		it = m_mapSubdirToTemplates.insert(it, std::make_pair(lpszCategory, CTemplateItemArray()));

	it->second.push_back(std::move(pItem));
}

void CTemplateDialog::CollectTemplates()
{
	// parse all the search paths
	for (int i = 0; i < m_astrSearchPaths.GetSize(); i++)
	{
		CFileFind dirs;
		if (!dirs.FindFile(m_astrSearchPaths[i] + _T("\\*.*"), 0)) continue;

		//Parse the files in this parent directory - they will appear on a tab with the name of this parent dir
		CollectTemplateFilesInDirectory(m_astrSearchPaths[i], CPathTool::GetFile(m_astrSearchPaths[i]));

		//Parse all subdirs in this directory
		BOOL bDirsContinue = TRUE;
		while (bDirsContinue)
		{
			bDirsContinue = dirs.FindNextFile();
			if (!dirs.IsDirectory() || dirs.IsDots()) continue; // only directories

			//Parse all files of interest in this subdirectory
			CollectTemplateFilesInDirectory(dirs.GetFilePath(), dirs.GetFileName());
		}
		dirs.Close();
	}// end of "parse all the search paths"


	//Create a sorted array, so we can add the tabs in alphabetical order
	bool bTemplateFilesFound = false;

	typedef std::set<CString> StringSet;
	StringSet astrSubDirsNonEmpty;

	for (StringTemplateItemArrayMap::const_iterator it = m_mapSubdirToTemplates.begin();
		it != m_mapSubdirToTemplates.end(); ++it)
	{
		//Any templates in this subdir? ==> If yes, then insert it sorted
		if (!it->second.empty())
		{
			astrSubDirsNonEmpty.insert(it->first);
			bTemplateFilesFound = true;
		}
	}

	//Add a category tab for each non-empty SubDir in the map
	if (bTemplateFilesFound)
	{
		int index = 0;

		for (StringSet::const_iterator it = astrSubDirsNonEmpty.begin(); 
			it != astrSubDirsNonEmpty.end(); ++it)
		{
			m_wndCategoriesTab.InsertItem(index++, *it);
		}
	}


	// select first tab
	if (m_wndCategoriesTab.SetCurSel(m_nFirstTab) < 0)
		m_wndCategoriesTab.SetCurSel(0);
	m_nLastTab = m_wndCategoriesTab.GetCurSel();

	// resize list ctrl
	CRect tabRect;
	m_wndCategoriesTab.GetWindowRect(tabRect);
	ScreenToClient(tabRect);
	m_wndCategoriesTab.AdjustRect(FALSE, tabRect);
	m_wndTemplateList.MoveWindow(tabRect);

	// initialize list ctrl
	m_wndTemplateList.SetImageList(&m_ImageList32, LVSIL_NORMAL);
	m_wndTemplateList.SetImageList(&m_ImageList16, LVSIL_SMALL);
}


void CTemplateDialog::CollectTemplateFilesInDirectory(const CString& strDirectory, const CString& strCategory)
{
	//Check, if the name of this subdir exists
	StringTemplateItemArrayMap::iterator it = m_mapSubdirToTemplates.find(strCategory);
	if (it == m_mapSubdirToTemplates.end())
	{
		//Create new entry in map
		it = m_mapSubdirToTemplates.insert(it, std::make_pair(strCategory, CTemplateItemArray()));
	}

	//Parse all files of interest in the directory
	for (int nFilter = 0; nFilter < m_astrFilters.GetSize(); nFilter++)
	{
		CFileFind templates;
		if (!templates.FindFile(strDirectory + "\\" + m_astrFilters[nFilter], 0))
			continue;

		// add all files
		BOOL bTemplatesContinue = TRUE;

		while (bTemplatesContinue)
		{
			bTemplatesContinue = templates.FindNextFile();
			if (!templates.IsDirectory())
			{
				std::unique_ptr<CTemplateItem> pItem
					(dynamic_cast<CTemplateItem*>(m_apTemplateItemClass[nFilter]->CreateObject()));

				if (pItem->InitItem(templates.GetFilePath(), m_ImageList16, m_ImageList32))
					it->second.push_back(std::move(pItem));
			}
		}
		templates.Close();
	}
}


void CTemplateDialog::FillTemplateList()
{
	int nItem = m_wndCategoriesTab.GetCurSel();
	TCITEM item;
	TCHAR lpszTabName[MAX_PATH];

	item.mask = TCIF_TEXT;
	item.pszText = lpszTabName;
	item.cchTextMax = MAX_PATH;

	if (!m_wndCategoriesTab.GetItem(nItem, &item))
		return;

	CString strKey = item.pszText;

	StringTemplateItemArrayMap::iterator it = m_mapSubdirToTemplates.find(strKey);

	if (it == m_mapSubdirToTemplates.end())
		return;

	// fill list ctrl with titles
	m_wndTemplateList.DeleteAllItems();

	//int nListItem = 0;
	for (CTemplateItemArray::const_iterator it1 = it->second.begin(); 
		it1 != it->second.end(); ++it1)
	{
		m_wndTemplateList.InsertItem(
		    LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM,
		    0,
		    (*it1)->GetTitle(), 0, 0,
		    (*it1)->GetImageIndex(),
		    (LPARAM)it1->get());
	}

	//Select the first entry in that template list
	m_wndTemplateList.SetItemState(0, LVNI_SELECTED, LVNI_SELECTED);
}

void CTemplateDialog::OnTemplateSelectionChanged()
{
	UpdateData();
	UpdateControlStates();

	int nTemplateIndex = GetSelectedItem();

	if (nTemplateIndex < 0 || nTemplateIndex >= m_wndTemplateList.GetItemCount())
	{
		m_strDescription.Empty();
		UpdateData(FALSE);

		return;
	}

	// retrieve template item
	CTemplateItem *pItem = reinterpret_cast<CTemplateItem*>(m_wndTemplateList.GetItemData(nTemplateIndex));
	ASSERT(pItem);
	if (!pItem)
		return;

	// set description
	m_strDescription = pItem->GetDescription();

	UpdateData(FALSE);
}

int CTemplateDialog::GetSelectedItem() const
{
	POSITION pos = m_wndTemplateList.GetFirstSelectedItemPosition();
	if (!pos)
		return -1;

	return m_wndTemplateList.GetNextSelectedItem(pos);
}

void CTemplateDialog::UpdateTemplateListViewType()
{
	if (!IsWindow(m_wndTemplateList.m_hWnd))
		return;

	switch (m_nListViewType)
	{
		case lvtIcons:
			m_wndTemplateList.ModifyStyle(LVS_LIST | LVS_ICON | LVS_SMALLICON | LVS_REPORT, LVS_ICON);
			break;
		case lvtList:
			m_wndTemplateList.ModifyStyle(LVS_LIST | LVS_ICON | LVS_SMALLICON | LVS_REPORT, LVS_LIST);
	}
}

void CTemplateDialog::UpdateControlStates()
{
	CWnd *pOkButton = GetDlgItem(IDC_BUTTON_CREATE);
	if (pOkButton && IsWindow(pOkButton->m_hWnd))
		pOkButton->EnableWindow(GetSelectedItem() > -1);
}

void CTemplateDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTemplateDialog)
	DDX_Control(pDX, IDC_LIST_TEMPLATES, m_wndTemplateList);
	DDX_Control(pDX, IDC_TAB_CATEGORIES, m_wndCategoriesTab);
	DDX_Text(pDX, IDC_STATIC_DESCRIPTION, m_strDescription);
	DDX_Radio(pDX, IDC_RADIO_VIEW_TYPE, m_nListViewType);
	//}}AFX_DATA_MAP
}

BOOL CTemplateDialog::OnInitDialog()
{
	//
	CDialog::OnInitDialog();

	UpdateTemplateListViewType();
	CollectTemplates();
	FillTemplateList();
	UpdateControlStates();

	if (RunTimeHelper::IsVista()) {
		::SetWindowTheme(m_wndTemplateList, L"explorer", 0);
		m_wndTemplateList.SetExtendedStyle(LVS_EX_DOUBLEBUFFER);
	}

	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void CTemplateDialog::OnSelchangeTabCategories(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	FillTemplateList();
	OnTemplateSelectionChanged();

	m_nLastTab = m_wndCategoriesTab.GetCurSel();

	*pResult = 0;
}

void CTemplateDialog::OnTemplateItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (pNMListView->uChanged & LVIF_STATE && pNMListView->uNewState & LVIS_SELECTED && !(pNMListView->uOldState & LVIS_SELECTED))
		OnTemplateSelectionChanged();
	else if (pNMListView->uChanged & LVIF_STATE && !(pNMListView->uNewState & LVIS_SELECTED) && pNMListView->uOldState & LVIS_SELECTED)
		OnTemplateSelectionChanged();

	*pResult = 0;
}

void CTemplateDialog::OnCreate()
{
	Create();
}

void CTemplateDialog::OnDblClkTemplate(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	CWnd *pOkButton = GetDlgItem(IDC_BUTTON_CREATE);
	if (!pOkButton || !pOkButton->IsWindowEnabled())
		return;

	// simulate click on button
	OnCreate();

	*pResult = 0;
}

void CTemplateDialog::OnViewTypeSelection()
{
	UpdateData();
	UpdateTemplateListViewType();

	theApp.WriteProfileInt(_T("Settings"), _T("TemplateListViewType"), m_nListViewType);
}
