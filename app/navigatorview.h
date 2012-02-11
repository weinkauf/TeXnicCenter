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

#pragma once

#include <cstddef>
#include <memory>

#include "ProjectView.h"
#include "LabelDragSource.h"

class CLaTeXProject;
class StructureItem;

/** Base class for viewing the document structure
        in a tree-like structure.

        @author Sven Wiegand
 */
class NavigatorTreeCtrl :
	public CTreeCtrl,
	public CProjectView,
	public LabelDragSource
{
	DECLARE_DYNAMIC(NavigatorTreeCtrl)
public:
	NavigatorTreeCtrl();
	virtual ~NavigatorTreeCtrl();

// operations
public:
	/**	Creates the view. */
	BOOL Create(CWnd *pwndParent);

	/**
	Returns a pointer to the one and only <var>CLaTeXProject</var>-object of
	the application.
	 */
	CLaTeXProject *GetProject() const;

	/**
	Returns the path of the specified tree item as a text.
	The path contains all the parent items seperated by a newline ('\\n').

	@see #GetItemByPath

	@param hItem
	        Tree item to get the path of
	 */
	CString GetItemPath(HTREEITEM hItem) const;

	/**
	Returns the item with the specified path.

	@see #GetItemPath

	@param lpszPath
	        Path of the item to return.

	@return
	        Handle to the first tree item with the specified path
	        or NULL, if there is no matching item.
	 */
	HTREEITEM GetItemByPath(LPCTSTR lpszPath) const;

	/**
	Finds all expanded items in the tree view.

	@see #GetItemPath
	@see #ExpandItems

	@param astrExpandedItems
	        Array that receives the paths of all expanded items.
	 */
	void GetExpandedItems(CStringArray &astrExpandedItems) const;

	/**
	Expands the specified items.

	@see #GetExpandedItems

	@param astrItems
	        Array with paths of the items to expand.
	 */
	void ExpandItems(const CStringArray &astrItems);

	/** Expands all items until given level.

	        This function expands each item,
	        which has a level smaller or equal to the given one.

	        @param nLevel
	                Items in this level and its parents will be expanded.
	                This is zero-based.
	 */
	void ExpandItemsByLevel(const int nLevel);

// implementation helpers
protected:
	/**
	Returns the next item that is expanded starting the search
	at the specified item.

	@param hItem
	        Tree item to start the search at.
	@param bInclude
	        TRUE to test hItem itself, if it is expanded, FALSE to start
	        with the next one after hItem.

	@return
	        Handle to the first tree item behind hItem that is expanded.
	 */
	HTREEITEM GetNextExpandedItem(HTREEITEM hItem, BOOL bInclude = FALSE) const;

	/** Updates the font settings to the window */
	void UpdateFont();

	virtual std::size_t GetItemIndex(HTREEITEM hItem) const;

// message handlers
protected:
	afx_msg void OnSysColorChange();
	afx_msg void OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	void SetImageLists();

	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTvnBeginDrag(NMHDR *pNMHDR, LRESULT *pResult);
	void OnEnter();

	virtual void GotoItem(HTREEITEM item);
	virtual bool ShouldBeExpanded(HTREEITEM item) const;

	DECLARE_MESSAGE_MAP()

// attributes
private:
	/** image list used by the tree ctrl. */
	CImageList m_images;

	/** Font to be used for the view */
	CFont m_font;

	/** TRUE after construction until the first call of OnInitialUpdate. */
	BOOL m_bFirstTime;

public:
	/// Removes all items in the tree view
	void Clear();
	/// Shows the context menu at the position indicated by the point parameters
	/// \param point
	///		   Position the context menu should be shown at, in screen coordinates
	///		   If both coordinates are set to -1, the position is calculated automatically
	virtual void ShowContextMenu(CPoint &point);
	/// Enables or disables the drag & drop functionality
	void EnableDragDrop(bool enable = true);
	/// Indicates whether the drag & drop functionality has been activated
	bool IsDragDropEnabled() const;
	/// Sends the ID_ITEM_GOTO command
	void GotoItem();
	virtual bool IsFolder(HTREEITEM item) const;
};
