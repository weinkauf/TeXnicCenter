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

#if !defined(AFX_OUTPUTBAR_H__6CC59864_DC90_11D3_946E_000000000000__INCLUDED_)
#define AFX_OUTPUTBAR_H__6CC59864_DC90_11D3_946E_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BuildView.h"
#include "GrepView.h"
#include "ParseOutputView.h"
#include "OutputDoc.h" //UPDATE
#include "ShellView.h" //UPDATE

/**
Controlbar which allows the user to navigate through his text.

@author Sven Wiegand
*/
class COutputBar : public CBCGSizingControlBar
{
// construction/destruction
public:
	COutputBar();
	virtual ~COutputBar();

// operations
public:
	void SetFlatTabs (BOOL bFlat = TRUE)
	{
		m_wndTabs.SetFlatFrame (bFlat);
	}

	/**
	Creates the views.

	@param pDocument
		Pointer to the document the view should be attached to.
	*/
	void CreateViews( COutputDoc *pDocument );

	/**
	Removes all views.
	*/
	void DeleteViews();

	/**
	Returns a reference to the tab window.
	*/
	CBCGTabWnd &GetTabWnd();

	// Returns the pointer to the output document! object
	COutputDoc* GetOutputDoc();

// overridings
protected:
	//{{AFX_VIRTUAL(COutputBar)
	//}}AFX_VIRTUAL

// implementation helpers
public:

// message handlers
protected:
	//{{AFX_MSG(COutputBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// attributes
protected:
	/** Tab-Ctrl */
	CBCGTabWnd m_wndTabs;

	/** View containing the results of the build proceses. */
	CBuildView *m_pwndBuildView;

	/** View containing the results of file grep 1. */
	CGrepView* m_pwndGrepView1;

	/** View containing the results of file grep 2. */
	CGrepView* m_pwndGrepView2;

	/** Structure parser output view. */
	CParseOutputView *m_pwndParseView;

	/** DOS Shell view to direct command entering*/
	CShellView* m_pwndShellView;

	COutputDoc* m_pOutputDoc;  //UPDATE
};


// inlines
inline
CBCGTabWnd &COutputBar::GetTabWnd()
{
	return m_wndTabs;
}

inline COutputDoc* COutputBar::GetOutputDoc() 
{ 
	return m_pOutputDoc; 
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_OUTPUTBAR_H__6CC59864_DC90_11D3_946E_000000000000__INCLUDED_
