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

#if !defined(AFX_WORKSPACEBAR_H__6CC59864_DC90_11D3_946E_000000000000__INCLUDED_)
#define AFX_WORKSPACEBAR_H__6CC59864_DC90_11D3_946E_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
Controlbar which allows the user to navigate through his text.

@author Sven Wiegand
*/
class CWorkspaceBar : public CBCGSizingControlBar
{
// construction/destruction
public:
	CWorkspaceBar();
	virtual ~CWorkspaceBar();

// operations
public:
	/**
	Returns a reference to the tab window.
	*/
	CBCGTabWnd &GetTabWnd();

	void SetFlatTabs (BOOL bFlat = TRUE);

// overridings
protected:
	//{{AFX_VIRTUAL(CWorkspaceBar)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// implementation helpers
public:

// message handlers
protected:
	//{{AFX_MSG(CWorkspaceBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// attributes
protected:
	/** Tab-Ctrl */
	CBCGTabWnd m_wndTabs;
};


// inlines
inline
CBCGTabWnd &CWorkspaceBar::GetTabWnd()
{
	return m_wndTabs;
}


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_WORKSPACEBAR_H__6CC59864_DC90_11D3_946E_000000000000__INCLUDED_
