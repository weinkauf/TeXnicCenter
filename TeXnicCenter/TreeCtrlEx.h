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

#ifndef __CTRLEXT_H_
#define __CTRLEXT_H_

class CCTreeCtrlEx;

class CTreeCursor
{
public:
	CTreeCursor();
	CTreeCursor(HTREEITEM hTreeItem,CCTreeCtrlEx* pTree);
	CTreeCursor(const CTreeCursor& posSrc);
	//	~CTreeCursor();
	const CTreeCursor & operator =(const CTreeCursor &posSrc);
	operator HTREEITEM();

	CTreeCursor InsertAfter(LPCTSTR strItem,HTREEITEM hAfter,
	                        int nImageIndex = -1);
	CTreeCursor AddHead(LPCTSTR strItem,int nImageIndex = -1);
	CTreeCursor AddTail(LPCTSTR strItem,int nImageIndex = -1);

	int GetImageID() const;

	BOOL GetRect(LPRECT lpRect,BOOL bTextOnly);
	CTreeCursor GetNext();
	CTreeCursor GetNext(UINT nCode);
	CTreeCursor GetChild();
	CTreeCursor GetNextSibling();
	CTreeCursor GetPrevSibling();
	CTreeCursor GetParent();
	CTreeCursor GetFirstVisible();
	CTreeCursor GetNextVisible();
	CTreeCursor GetPrevVisible();
	CTreeCursor GetSelected();
	CTreeCursor GetDropHilight();
	CTreeCursor GetRoot();
	CString GetText() const;
	BOOL GetImage(int& nImage,int& nSelectedImage);
	UINT GetState(UINT nStateMask);
	DWORD GetData();
	BOOL Set(UINT nMask,LPCTSTR lpszItem,int nImage,
	         int nSelectedImage,UINT nState,UINT nStateMask,LPARAM lParam);
	BOOL SetText(LPCTSTR lpszItem);
	BOOL SetImage(int nImage,int nSelectedImage);
	BOOL SetState(UINT nState,UINT nStateMask);
	BOOL SetData(DWORD dwData);
	BOOL HasChildren();
	// Operations
	BOOL Delete();

	BOOL Expand(UINT nCode = TVE_EXPAND);
	BOOL Select(UINT nCode);
	BOOL Select();
	BOOL SelectDropTarget();
	BOOL SelectSetFirstVisible();
	CEdit* EditLabel();
	CImageList* CreateDragImage();
	BOOL SortChildren();
	BOOL EnsureVisible();

	// More...
	BOOL IsValid() const;

protected:
	HTREEITEM m_hTreeItem;
	CCTreeCtrlEx *m_pTree;

	CTreeCursor CTreeCursor::_Insert(LPCTSTR strItem,
	                                 int nImageIndex,HTREEITEM hAfter);
};


/////////////////////////////////////////////////////////////////////////////
// CCTreeCtrlEx

class CCTreeCtrlEx : public CTreeCtrl
{
// Attributes
protected:

// Operation
public:
	CCTreeCtrlEx();
	virtual ~CCTreeCtrlEx();
	CImageList* SetImageList(CImageList* pImageList,
	                         int nImageListType = TVSIL_NORMAL);

	CTreeCursor GetNextItem(HTREEITEM hItem,UINT nCode);
	CTreeCursor GetChildItem(HTREEITEM hItem);
	CTreeCursor GetNextSiblingItem(HTREEITEM hItem);
	CTreeCursor GetPrevSiblingItem(HTREEITEM hItem);
	CTreeCursor GetParentItem(HTREEITEM hItem);
	CTreeCursor GetFirstVisibleItem();
	CTreeCursor GetNextVisibleItem(HTREEITEM hItem);
	CTreeCursor GetPrevVisibleItem(HTREEITEM hItem);
	CTreeCursor GetSelectedItem();
	CTreeCursor GetDropHilightItem();
	CTreeCursor GetRootItem();
	CTreeCursor InsertItem(LPTV_INSERTSTRUCT lpInsertStruct);
	CTreeCursor InsertItem(UINT nMask,LPCTSTR lpszItem,int nImage,
	                       int nSelectedImage,UINT nState,UINT nStateMask,LPARAM lParam,
	                       HTREEITEM hParent,HTREEITEM hInsertAfter);
	CTreeCursor InsertItem(LPCTSTR lpszItem,HTREEITEM hParent = TVI_ROOT,
	                       HTREEITEM hInsertAfter = TVI_LAST);
	CTreeCursor InsertItem(LPCTSTR lpszItem,int nImage,int nSelectedImage,
	                       HTREEITEM hParent = TVI_ROOT,HTREEITEM hInsertAfter = TVI_LAST);
	CTreeCursor HitTest(CPoint pt,UINT* pFlags = NULL);
	CTreeCursor HitTest(TV_HITTESTINFO* pHitTestInfo);
};

/////////////////////////////////////////////////////////////////////////////

inline CTreeCursor::CTreeCursor() : m_hTreeItem(NULL),m_pTree(NULL)
{
	m_hTreeItem = NULL;
	m_pTree = NULL;
}

inline CTreeCursor::CTreeCursor(HTREEITEM hTreeItem,CCTreeCtrlEx* pTree)
{
	m_hTreeItem = hTreeItem;
	m_pTree = pTree;
}

inline CTreeCursor::CTreeCursor(const CTreeCursor& posSrc)
{
	*this = posSrc;
}
//inline CTreeCursor::~CTreeCursor()
//	{ }

inline CTreeCursor::operator HTREEITEM()
{
	return m_hTreeItem;
}

inline CTreeCursor CTreeCursor::InsertAfter(LPCTSTR strItem,
        HTREEITEM hAfter,int nImageIndex)
{
	return _Insert(strItem,nImageIndex,hAfter);
}

inline CTreeCursor CTreeCursor::AddHead(LPCTSTR strItem,int nImageIndex)
{
	return _Insert(strItem,nImageIndex,TVI_FIRST);
}

inline CTreeCursor CTreeCursor::AddTail(LPCTSTR strItem,int nImageIndex)
{
	return _Insert(strItem,nImageIndex,TVI_LAST);
}

inline BOOL CTreeCursor::GetRect(LPRECT lpRect,BOOL bTextOnly)
{
	return m_pTree->GetItemRect(m_hTreeItem,lpRect,bTextOnly);
}

inline CTreeCursor CTreeCursor::GetNext(UINT nCode)
{
	return m_pTree->GetNextItem(m_hTreeItem,nCode);
}

inline CTreeCursor CTreeCursor::GetChild()
{
	return m_pTree->GetChildItem(m_hTreeItem);
}

inline CTreeCursor CTreeCursor::GetNextSibling()
{
	return m_pTree->GetNextSiblingItem(m_hTreeItem);
}

inline CTreeCursor CTreeCursor::GetPrevSibling()
{
	return m_pTree->GetPrevSiblingItem(m_hTreeItem);
}

inline CTreeCursor CTreeCursor::GetParent()
{
	return m_pTree->GetParentItem(m_hTreeItem);
}

inline CTreeCursor CTreeCursor::GetFirstVisible()
{
	return m_pTree->GetFirstVisibleItem();
}

inline CTreeCursor CTreeCursor::GetNextVisible()
{
	return m_pTree->GetNextVisibleItem(m_hTreeItem);
}

inline CTreeCursor CTreeCursor::GetPrevVisible()
{
	return m_pTree->GetPrevVisibleItem(m_hTreeItem);
}

inline CTreeCursor CTreeCursor::GetSelected()
{
	return m_pTree->GetSelectedItem();
}

inline CTreeCursor CTreeCursor::GetDropHilight()
{
	return m_pTree->GetDropHilightItem();
}

inline CTreeCursor CTreeCursor::GetRoot()
{
	return m_pTree->GetRootItem();
}

CString CTreeCursor::GetText() const
{
	return m_pTree->GetItemText(m_hTreeItem);
}

inline BOOL CTreeCursor::GetImage(int& nImage,int& nSelectedImage)
{
	return m_pTree->GetItemImage(m_hTreeItem,nImage,nSelectedImage);
}

inline UINT CTreeCursor::GetState(UINT nStateMask)
{
	return m_pTree->GetItemState(m_hTreeItem,nStateMask);
}

inline DWORD CTreeCursor::GetData()
{
	return m_pTree->GetItemData(m_hTreeItem);
}
//inline BOOL SetItem(UINT nMask, LPCTSTR lpszItem, int nImage,
//	int nSelectedImage,	UINT nState,nStateMask,lParam);

inline BOOL CTreeCursor::SetText(LPCTSTR lpszItem)
{
	return m_pTree->SetItemText(m_hTreeItem,lpszItem);
}

inline BOOL CTreeCursor::SetImage(int nImage,int nSelectedImage)
{
	return m_pTree->SetItemImage(m_hTreeItem,nImage,nSelectedImage);
}

inline BOOL CTreeCursor::SetState(UINT nState,UINT nStateMask)
{
	return m_pTree->SetItemState(m_hTreeItem,nState,nStateMask);
}

inline BOOL CTreeCursor::SetData(DWORD dwData)
{
	return m_pTree->SetItemData(m_hTreeItem,dwData);
}

inline BOOL CTreeCursor::HasChildren()
{
	return m_pTree->ItemHasChildren(m_hTreeItem);
}
// Operations
//inline BOOL CTreeCursor::Delete()
//{
//	return m_pTree->DeleteItem(m_hTreeItem);
//}

inline BOOL CTreeCursor::Delete()
{
	BOOL bReturn = m_pTree->DeleteItem(m_hTreeItem);
	if (bReturn)
		m_hTreeItem = NULL;
	return bReturn;
}

inline BOOL CTreeCursor::Expand(UINT nCode)
{
	return m_pTree->Expand(m_hTreeItem,nCode);
}

inline BOOL CTreeCursor::Select(UINT nCode)
{
	return m_pTree->Select(m_hTreeItem,nCode);
}

inline BOOL CTreeCursor::Select()
{
	return m_pTree->SelectItem(m_hTreeItem);
}

inline BOOL CTreeCursor::SelectDropTarget()
{
	return m_pTree->SelectDropTarget(m_hTreeItem);
}

inline BOOL CTreeCursor::SelectSetFirstVisible()
{
	return m_pTree->SelectSetFirstVisible(m_hTreeItem);
}

inline CEdit* CTreeCursor::EditLabel()
{
	return m_pTree->EditLabel(m_hTreeItem);
}

inline CImageList* CTreeCursor::CreateDragImage()
{
	return m_pTree->CreateDragImage(m_hTreeItem);
}

inline BOOL CTreeCursor::SortChildren()
{
	return m_pTree->SortChildren(m_hTreeItem);
}

inline BOOL CTreeCursor::EnsureVisible()
{
	return m_pTree->EnsureVisible(m_hTreeItem);
}

inline BOOL CTreeCursor::IsValid() const
{
	return (m_hTreeItem != NULL && m_pTree != NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CCTreeCtrlEx inline functions

inline CCTreeCtrlEx::CCTreeCtrlEx() : CTreeCtrl()
{
}

inline CCTreeCtrlEx::~CCTreeCtrlEx()
{
}

inline CImageList* CCTreeCtrlEx::SetImageList(CImageList* pImageList,int nImageListType)
{
	return CTreeCtrl::SetImageList(pImageList,nImageListType);
}

inline CTreeCursor CCTreeCtrlEx::GetNextItem(HTREEITEM hItem,UINT nCode)
{
	return CTreeCursor(CTreeCtrl::GetNextItem(hItem,nCode),this);
}

inline CTreeCursor CCTreeCtrlEx::GetChildItem(HTREEITEM hItem)
{
	return CTreeCursor(CTreeCtrl::GetChildItem(hItem),this);
}

inline CTreeCursor CCTreeCtrlEx::GetNextSiblingItem(HTREEITEM hItem)
{
	return CTreeCursor(CTreeCtrl::GetNextSiblingItem(hItem),this);
}

inline CTreeCursor CCTreeCtrlEx::GetPrevSiblingItem(HTREEITEM hItem)
{
	return CTreeCursor(CTreeCtrl::GetPrevSiblingItem(hItem),this);
}

inline CTreeCursor CCTreeCtrlEx::GetParentItem(HTREEITEM hItem)
{
	return CTreeCursor(CTreeCtrl::GetParentItem(hItem),this);
}

inline CTreeCursor CCTreeCtrlEx::GetFirstVisibleItem()
{
	return CTreeCursor(CTreeCtrl::GetFirstVisibleItem(),this);
}

inline CTreeCursor CCTreeCtrlEx::GetNextVisibleItem(HTREEITEM hItem)
{
	return CTreeCursor(CTreeCtrl::GetNextVisibleItem(hItem),this);
}

inline CTreeCursor CCTreeCtrlEx::GetPrevVisibleItem(HTREEITEM hItem)
{
	return CTreeCursor(CTreeCtrl::GetPrevVisibleItem(hItem),this);
}

inline CTreeCursor CCTreeCtrlEx::GetSelectedItem()
{
	return CTreeCursor(CTreeCtrl::GetSelectedItem(),this);
}

inline CTreeCursor CCTreeCtrlEx::GetDropHilightItem()
{
	return CTreeCursor(CTreeCtrl::GetDropHilightItem(),this);
}

inline CTreeCursor CCTreeCtrlEx::GetRootItem()
{
	return CTreeCursor(CTreeCtrl::GetRootItem(),this);
}

inline CTreeCursor CCTreeCtrlEx::InsertItem(LPTV_INSERTSTRUCT lpInsertStruct)
{
	return CTreeCursor(CTreeCtrl::InsertItem(lpInsertStruct),this);
}

inline CTreeCursor CCTreeCtrlEx::InsertItem(UINT nMask,LPCTSTR lpszItem,int nImage,
        int nSelectedImage,UINT nState,UINT nStateMask,LPARAM lParam,
        HTREEITEM hParent,HTREEITEM hInsertAfter)
{
	return CTreeCursor(CTreeCtrl::InsertItem(nMask,lpszItem,nImage,
	                   nSelectedImage,nState,nStateMask,lParam,hParent,hInsertAfter),this);
}

inline CTreeCursor CCTreeCtrlEx::InsertItem(LPCTSTR lpszItem,HTREEITEM hParent,
        HTREEITEM hInsertAfter)
{
	return CTreeCursor(CTreeCtrl::InsertItem(lpszItem,hParent,hInsertAfter),this);
}

inline CTreeCursor CCTreeCtrlEx::InsertItem(LPCTSTR lpszItem,
        int nImage,int nSelectedImage,
        HTREEITEM hParent,HTREEITEM hInsertAfter)
{
	return CTreeCursor(CTreeCtrl::InsertItem(lpszItem,nImage,nSelectedImage,
	                   hParent,hInsertAfter),this);
}

inline CTreeCursor CCTreeCtrlEx::HitTest(CPoint pt,UINT* pFlags)
{
	return CTreeCursor(CTreeCtrl::HitTest(pt,pFlags),this);
}

inline CTreeCursor CCTreeCtrlEx::HitTest(TV_HITTESTINFO* pHitTestInfo)
{
	return CTreeCursor(CTreeCtrl::HitTest(pHitTestInfo),this);
}

inline const CTreeCursor& CTreeCursor::operator =(const CTreeCursor& posSrc)
{
	if (&posSrc != this)
	{
		m_hTreeItem = posSrc.m_hTreeItem;
		m_pTree = posSrc.m_pTree;
	}
	return *this;
}

inline CTreeCursor CTreeCursor::_Insert(LPCTSTR strItem,
                                        int nImageIndex,HTREEITEM hAfter)
{
	TV_INSERTSTRUCT ins;
	ins.hParent = m_hTreeItem;
	ins.hInsertAfter = hAfter;
	ins.item.mask = TVIF_TEXT;
	ins.item.pszText = (LPTSTR) strItem;
	if (nImageIndex != -1)
	{
		ins.item.mask |= TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		ins.item.iImage = nImageIndex;
		ins.item.iSelectedImage = nImageIndex;
	}
	return CTreeCursor(m_pTree->InsertItem(&ins),m_pTree);
}

int CTreeCursor::GetImageID() const
{
	TV_ITEM item;
	item.mask = TVIF_HANDLE | TVIF_IMAGE;
	item.hItem = m_hTreeItem;
	m_pTree->GetItem(&item);
	return item.iImage;
}


// GetNextItem  - Get next item as if outline was completely expanded
// Returns              - The item immediately below the reference item
// hItem                - The reference item

inline CTreeCursor CTreeCursor::GetNext()
{
	if (HasChildren())
	{
		return GetChild();
	}
	else
	{
		CTreeCursor newCursor;
		// return next sibling item
		// Go up the tree to find a parent's sibling if needed.
		while (TRUE)
		{
			newCursor = GetNextSibling();
			if (newCursor != NULL)
				break;
			*this = GetParent();
			if (*this == NULL)
				return *this;
		}

		return newCursor;
	}
}

#endif //__CTRLEXT_H_
