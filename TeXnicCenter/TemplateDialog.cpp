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
#include "TemplateDialog.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// globals
//-------------------------------------------------------------------

void AFXAPI DestructElements(CTemplateItemArray* *pElements, int nCount)
{
	for (int i = 0; i < nCount; i++)
		delete pElements[i];
}


void AFXAPI DestructElements(CTemplateItem **pElements, int nCount)
{
	for (int i = 0; i < nCount; i++)
		delete pElements[i];
}

//-------------------------------------------------------------------
// CTemplateItem
//-------------------------------------------------------------------

CTemplateItem::CTemplateItem()
{}


CTemplateItem::~CTemplateItem()
{}


//-------------------------------------------------------------------
// CTemplateDialog 
//-------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CTemplateDialog, CDialog)
	//{{AFX_MSG_MAP(CTemplateDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CATEGORIES, OnSelchangeTabCategories)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TEMPLATES, OnTemplateItemChanged)
	ON_BN_CLICKED(IDC_BUTTON_CREATE, OnCreate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CTemplateDialog::CTemplateDialog(UINT unIDTemplate, CWnd* pParent /*=NULL*/)
: CDialog(unIDTemplate, pParent)
{
	m_ImageList.Create(32, 32, ILC_COLOR | ILC_MASK, 1, 1);

	//{{AFX_DATA_INIT(CTemplateDialog)
	//}}AFX_DATA_INIT
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


void CTemplateDialog::AddTemplateItem(LPCTSTR lpszCategory, CTemplateItem *pItem)
{
	if (!pItem->InitItem(NULL, m_ImageList))
		return;

	CTemplateItemArray	*pTemplateArray;
	if (!m_mapSubdirToTemplates.Lookup(lpszCategory, pTemplateArray))
		m_mapSubdirToTemplates.SetAt(lpszCategory, pTemplateArray = new CTemplateItemArray);

	pTemplateArray->Add(pItem);
}


void CTemplateDialog::CollectTemplates() 
{
	// parse all the search paths
	for (int i = 0; i < m_astrSearchPaths.GetSize(); i++)
	{
		CFileFind		dirs;
		if (!dirs.FindFile( m_astrSearchPaths[i] + "\\*.*", 0 ))
			continue;

		// parse all subdirs in this directory
		BOOL	bDirsContinue  = TRUE;

		while (bDirsContinue)
		{
			bDirsContinue = dirs.FindNextFile();

			if (!dirs.IsDirectory() || dirs.IsDots())
				continue;		// only directories

			CString		strSubdir = dirs.GetFilePath();
			CString		strSubdirName = dirs.GetFileName();

			// check, if name of this subdir is allready existing
			CTemplateItemArray	*pTemplateArray = NULL;
			if (!m_mapSubdirToTemplates.Lookup(strSubdirName, pTemplateArray))
				m_mapSubdirToTemplates.SetAt(strSubdirName, pTemplateArray = new CTemplateItemArray);

			if (!pTemplateArray)
				continue;

			// parse all files of interest in the directory
			for (int nFilter = 0; nFilter < m_astrFilters.GetSize(); nFilter++)
			{
				CFileFind	templates;
				if (!templates.FindFile(strSubdir + "\\" + m_astrFilters[nFilter], 0))
					continue;

				// add all files
				BOOL	bTemplatesContinue = TRUE;

				while (bTemplatesContinue)
				{
					bTemplatesContinue = templates.FindNextFile();
					if (!templates.IsDirectory())
					{
						CTemplateItem	*pItem = dynamic_cast<CTemplateItem*>(m_apTemplateItemClass[nFilter]->CreateObject());
						if (pItem->InitItem(templates.GetFilePath(), m_ImageList))
							pTemplateArray->Add(pItem);
						else
							delete pItem;
					}
				}
				templates.Close();
			}
		}
		dirs.Close();
	}

	// add for each sub dir in the subdir map a tab
	POSITION			pos = m_mapSubdirToTemplates.GetStartPosition();
	CString				strKey;
	int						nItem = 0;
	BOOL					bTemplateFilesFound = FALSE;
	CTemplateItemArray	*pTemplateItemArray;

	while (pos)
	{
		m_mapSubdirToTemplates.GetNextAssoc(pos, strKey, pTemplateItemArray);
		if (pTemplateItemArray->GetSize())
		{
			m_wndCategoriesTab.InsertItem(nItem++, strKey);
			bTemplateFilesFound = TRUE;
		}
	}

	// select first tab
	m_wndCategoriesTab.SetCurSel(0);

	// resize list ctrl
	CRect	tabRect;
	m_wndCategoriesTab.GetWindowRect(tabRect);
	ScreenToClient(tabRect);
	m_wndCategoriesTab.AdjustRect( FALSE, tabRect );
	m_wndTemplateList.MoveWindow( tabRect );

	// initialize list ctrl
	m_wndTemplateList.SetImageList(&m_ImageList, LVSIL_NORMAL);
}


void CTemplateDialog::FillTemplateList()
{
	int			nItem = m_wndCategoriesTab.GetCurSel();
	TCITEM	item;
	TCHAR		lpszTabName[_MAX_PATH];

	item.mask = TCIF_TEXT;
	item.pszText = lpszTabName;
	item.cchTextMax = _MAX_PATH;

	if (!m_wndCategoriesTab.GetItem(nItem, &item))
		return;

	CString							strKey = item.pszText;
	CTemplateItemArray	*papTemplateItems = NULL;

	if (!m_mapSubdirToTemplates.Lookup(strKey, papTemplateItems))
		return;

	if (!papTemplateItems)
		return;

	// fill list ctrl with titles
	m_wndTemplateList.DeleteAllItems();

	int	nListItem = 0;
	for (int i = 0; i < papTemplateItems->GetSize(); i++)
	{
		m_wndTemplateList.InsertItem( 
			LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM,  
			0, 
			papTemplateItems->GetAt(i)->GetTitle(), 0, 0,
			papTemplateItems->GetAt(i)->GetImageIndex(),
			(LPARAM)papTemplateItems->GetAt(i));
	}
}


void CTemplateDialog::OnTemplateSelectionChanged()
{
	UpdateData();
	UpdateControlStates();

	int	nTemplateIndex = GetSelectedItem();

	if (nTemplateIndex < 0 || nTemplateIndex >= m_wndTemplateList.GetItemCount())
	{
		m_strDescription.Empty();
		UpdateData(FALSE);

		return;
	}

	// retrieve template item
	CTemplateItem	*pItem = reinterpret_cast<CTemplateItem*>(m_wndTemplateList.GetItemData(nTemplateIndex));
	ASSERT(pItem);
	if (!pItem)
		return;

	// set description
	m_strDescription = pItem->GetDescription();

	UpdateData(FALSE);
}


int CTemplateDialog::GetSelectedItem() const
{
	POSITION	pos = m_wndTemplateList.GetFirstSelectedItemPosition();
	if (!pos)
		return -1;

	return m_wndTemplateList.GetNextSelectedItem(pos);
}


void CTemplateDialog::UpdateControlStates()
{
	CWnd	*pOkButton = GetDlgItem(IDC_BUTTON_CREATE);
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
	//}}AFX_DATA_MAP
}


BOOL CTemplateDialog::OnInitDialog() 
{
	//
	CDialog::OnInitDialog();

	CollectTemplates();
	FillTemplateList();
	UpdateControlStates();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}


void CTemplateDialog::OnSelchangeTabCategories(NMHDR* pNMHDR, LRESULT* pResult) 
{
	FillTemplateList();	
	OnTemplateSelectionChanged();
	*pResult = 0;
}


void CTemplateDialog::OnTemplateItemChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (pNMListView->uChanged&LVIF_STATE && pNMListView->uNewState&LVIS_SELECTED && !(pNMListView->uOldState&LVIS_SELECTED))
		OnTemplateSelectionChanged();
	else if (pNMListView->uChanged&LVIF_STATE && !(pNMListView->uNewState&LVIS_SELECTED) && pNMListView->uOldState&LVIS_SELECTED)
		OnTemplateSelectionChanged();

	*pResult = 0;
}
