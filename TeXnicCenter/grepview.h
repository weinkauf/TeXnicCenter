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

#if !defined(AFX_GREPVIEW_H__0EE022A7_20D1_11D3_929E_DA6C20A3421F__INCLUDED_)
#define AFX_GREPVIEW_H__0EE022A7_20D1_11D3_929E_DA6C20A3421F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GrepView.h : Header-Datei
//
#include "OutputView.h"


class CGrepView : public COutputView
{
	DECLARE_DYNCREATE(CGrepView)

// construction/destruction
public:
	CGrepView();
	virtual ~CGrepView();

// operations
public:
	/** 
	Sets the index of this grep view. Used to distinguish between more 
	grep views.

	@param nIndex
		Index of the grep view.
	*/
	void SetIndex( int nIndex );

	/**
	Returns the index of this grep view. If -1, no index has been 
	set for this view.
	*/
	int GetIndex() const;

// overridings
protected:
	virtual void OnUpdate(COutputView* pSender, LPARAM lHint, CObject* pHint);

	//{{AFX_VIRTUAL(CGrepView)
	protected:
	//}}AFX_VIRTUAL

// debugging
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// message handlers
protected:
	//{{AFX_MSG(CGrepView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// attributes
protected:
	/** index of this grep view */
	int m_nIndex;
};


// inlines
inline
void CGrepView::SetIndex( int nIndex )
{
	m_nIndex = nIndex;
}


inline
int CGrepView::GetIndex() const
{
	return m_nIndex;
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_GREPVIEW_H__0EE022A7_20D1_11D3_929E_DA6C20A3421F__INCLUDED_
