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

#if !defined(AFX_NAVIGATORVIEW_H__75A37CC2_20BE_11D3_929E_D32D9B17C664__INCLUDED_)
#define AFX_NAVIGATORVIEW_H__75A37CC2_20BE_11D3_929E_D32D9B17C664__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ProjectView.h"

class CLatexProject;


/** Base class for viewing the document structure
	in a tree-like structure.

	@author Sven Wiegand
*/
class CNavigatorView : 
	public CTreeCtrl,
	public CProjectView
{
// construction/destruction
public:
	CNavigatorView();
	virtual ~CNavigatorView();

// operations
public:
	/**	Creates the view. */
	BOOL Create(CWnd *pwndParent);

	/**
	Returns a pointer to the one and only <var>CLatexProject</var>-object of
	the application.
	*/
	CLatexProject *GetProject() const;

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
	HTREEITEM GetItemByPath( LPCTSTR lpszPath ) const;

	/**
	Finds all expanded items in the tree view.

	@see #GetItemPath
	@see #ExpandItems

	@param astrExpandedItems
		Array that receives the paths of all expanded items.
	*/
	void GetExpandedItems( CStringArray &astrExpandedItems ) const;

	/**
	Expands the specified items.

	@see #GetExpandedItems

	@param astrItems
		Array with paths of the items to expand.
	*/
	void ExpandItems( const CStringArray &astrItems );

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
	HTREEITEM GetNextExpandedItem( HTREEITEM hItem, BOOL bInclude = FALSE ) const;

	/** Applies the font settings to the window */
	void ApplyFont();

// overwritings
public:
	//{{AFX_VIRTUAL(CNavigatorView)
	public:
	//}}AFX_VIRTUAL

// message handlers
protected:
	afx_msg void OnSysColorChange();

	//{{AFX_MSG(CNavigatorView)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// attributes
protected:
	/** image list used by the tree ctrl. */
	CImageList m_images;

	/** Font to be used for the view */
	CFont m_font;

private:
	/** TRUE after construction until the first call of OnInitialUpdate. */
	BOOL m_bFirstTime;
};


inline CLatexProject *CNavigatorView::GetProject() const
{
	return (CLatexProject*)CProjectView::GetProject();
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f�gt unmittelbar vor der vorhergehenden Zeile zus�tzliche Deklarationen ein.

#endif // AFX_NAVIGATORVIEW_H__75A37CC2_20BE_11D3_929E_D32D9B17C664__INCLUDED_
