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

#if !defined(AFX_PROFILEPAGELATEX_H__5C552765_082E_11D5_A222_006097239934__INCLUDED_)
#define AFX_PROFILEPAGELATEX_H__5C552765_082E_11D5_A222_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ProfilePage.h"
#include "BrowseButton.h"
#include "PersistPosEdit.h"
#include "EditMenuButton.h"

class CProfilePageLatex : public CProfilePage
{
// construction/destruction
public:
	CProfilePageLatex();

// implementation
protected:
	void UpdateControlStates();

// overridings
public:
	void OnUpdateDataSet(CProfile *pProfile);
	BOOL ApplyChanges();
	
	//{{AFX_VIRTUAL(CProfilePageLatex)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// message handlers
protected:
	//{{AFX_MSG(CProfilePageLatex)
	afx_msg void OnUpdateControlStates();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// dialog field data
protected:
	//{{AFX_DATA(CProfilePageLatex)
	enum { IDD = IDD_PROFILE_LATEX };
	CButton	m_wndStopOnLatexError;
	CStatic	m_wndTitleMakeIndexPath;
	CStatic	m_wndTitleMakeIndexArguments;
	CStatic	m_wndTitleBibTexPath;
	CStatic	m_wndTitleBibTexArguments;
	CEdit	m_wndMakeIndexPath;
	CEdit	m_wndBibTexPath;
	CPersistPosEdit	m_wndMakeIndexArguments;
	CPersistPosEdit	m_wndBibTexArguments;
	CEditMenuButton	m_wndMakeIndexPh;
	CEditMenuButton	m_wndBibTexPh;
	CBrowseButton	m_wndBrowseMakeIndex;
	CBrowseButton	m_wndBrowseBibTex;
	CEditMenuButton	m_wndLatexPh;
	CPersistPosEdit	m_wndLatexArguments;
	CStatic	m_wndTitleLatexArguments;
	CBrowseButton	m_wndBrowseLatex;
	CEdit	m_wndLatexPath;
	CStatic	m_wndTitleLatexPath;
	BOOL	m_bUseLatex;
	CString	m_strLatexPath;
	CString	m_strLatexArguments;
	BOOL	m_bDontUseBibTex;
	BOOL	m_bDontUseMakeIndex;
	CString	m_strBibTexArguments;
	CString	m_strMakeIndexArguments;
	CString	m_strBibTexPath;
	CString	m_strMakeIndexPath;
	BOOL	m_bStopOnLatexError;
	//}}AFX_DATA

// attributes
protected:
	CProfile *m_pProfile;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_PROFILEPAGELATEX_H__5C552765_082E_11D5_A222_006097239934__INCLUDED_
