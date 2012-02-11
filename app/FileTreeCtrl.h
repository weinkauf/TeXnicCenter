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

#pragma once

#include <cstddef>
#include <map>

#include "NavigatorView.h"

class ItemIDList;

/**
 * @brief Project file tree control.
 * @author Sergiu Dotenco
 */
class FileTreeCtrl
	: public NavigatorTreeCtrl
{
	DECLARE_DYNAMIC(FileTreeCtrl)
public:
	FileTreeCtrl();
	~FileTreeCtrl();

	void ShowContextMenu(CPoint& pt);

	/**
	 * @brief Notifies the shell that all files under the root item should be
	 *		  refreshed.
	 */
	void Refresh();

	void Clear();

	void SetMissingItemColor(COLORREF value, bool redraw = true);
	COLORREF GetMissingItemColor() const;

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	void OnUpdate(CProjectView* pSender, LPARAM lHint, LPVOID pHint);
	std::size_t GetItemIndex(HTREEITEM hItem) const;
	bool IsFolder(HTREEITEM item) const;
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
	struct Entry;
	struct FileEntry;
	struct DirectoryEntry;

	/**
	 * @brief Binary less predicate for @ref ItemIDList.
	 */
	struct FolderItemIDListLessPredicate
		: std::binary_function<ItemIDList, ItemIDList, bool>
	{
		explicit FolderItemIDListLessPredicate(CComPtr<IShellFolder> shellFolder);
		FolderItemIDListLessPredicate();
		bool operator()(const ItemIDList& lhs, const ItemIDList& rhs) const;

		CComPtr<IShellFolder> shellFolder;
	};

	typedef std::map<ItemIDList, HTREEITEM, FolderItemIDListLessPredicate>
		ItemIDListTreeItemMap;

	afx_msg void OnTvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnGetInfoTip(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);

	void OnParsingFinished();

	int GetItemImageIndex(std::size_t index, int flags = 0);
	int GetImageIndex(LPCWSTR path, int flags = 0, DWORD fileAttributes = 0);
	int GetImageIndex(LPCITEMIDLIST pidl, int flags = 0);

	void Initialize();
	void Populate();

	void Register(const CString& path, bool recursive = true);
	void Unregister();

	LRESULT OnShellChange(WPARAM wParam, LPARAM lParam);
	void OnShellCreateItem(PIDLIST_ABSOLUTE* pidls);
	void OnShellUpdateItem(PIDLIST_ABSOLUTE* pidls);
	void OnShellRenameItem(PIDLIST_ABSOLUTE* pidls);
	void OnShellDeleteItem(PIDLIST_ABSOLUTE* pidls);

	void MarkItemAsMissing(HTREEITEM hItem, bool missing = true);
	void MarkItemAsMissing(const ItemIDList& key, bool missing = true);

	HTREEITEM InsertEntry(HTREEITEM parent, Entry* entry, const StructureItem* item = NULL, bool bold = false);
	void UpdateEntryTree(HTREEITEM hItem);
	void UpdateEntryImages(HTREEITEM hItem);
	ItemIDList GetPathItemIDList(const CString& path) const;
	void ForceClear();
	void OnShellUpdateImage();
	void SetupImageLists();
	static int CALLBACK SortComparer(LPARAM l1, LPARAM l2, LPARAM p);
	int SortComparer(LPARAM l1, LPARAM l2);

	ItemIDListTreeItemMap itemIdTreeItem_;
	ULONG shellNotifyId_;
	CComPtr<IShellFolder> shellFolder_;
	CComPtr<IShellFolder> projectShellFolder_;
	CComPtr<IContextMenu2> contextMenu2_;
	CComPtr<IContextMenu3> contextMenu3_;
	COLORREF missingItemColor_;
};
