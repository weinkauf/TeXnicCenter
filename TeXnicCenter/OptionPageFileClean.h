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

#if !defined(AFX_OPTIONPAGEFILECLEAN_H__C556B114_5207_11D6_AF06_00201855324E__INCLUDED_)
#define AFX_OPTIONPAGEFILECLEAN_H__C556B114_5207_11D6_AF06_00201855324E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "FileClean.h"
#include "PersistPosEdit.h"
#include "EditMenuButton.h"

/////////////////////////////////////////////////////////////////////////////
// COptionPageFileClean dialog

class COptionPageFileClean : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionPageFileClean)

// Construction
public:
	COptionPageFileClean();
	~COptionPageFileClean();

// Dialog Data
	//{{AFX_DATA(COptionPageFileClean)
	enum { IDD = IDD_OPTIONS_FILECLEAN };
	CButton	m_DeleteBtn;
	CEditMenuButton	m_PHInsertBtn;
	CButton	m_RecursiveBtn;
	CPersistPosEdit	m_PatternEdit;
	CEdit	m_NameEdit;
	CComboBox	m_HandleBox;
	CListCtrl	m_wndList;
	BOOL	m_bConfirm;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionPageFileClean)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionPageFileClean)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeName();
	afx_msg void OnChangePattern();
	afx_msg void OnSelchangeHandling();
	afx_msg void OnRecursive();
	afx_msg void OnListEvent(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDelete();
	afx_msg void OnNew();
	afx_msg void OnSort();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

//Methods
protected:
	void UpdateItems(const CFileCleanItemArray& copyItems);
	void RefillList();
	void UpdateControls();
	void UpdateControlsState(bool bEnable = true);
	CFileCleanItem* GetSelectedFCItem();
	void UpdateSelectedListItem();
	CString GetFileHandlingName(CFileCleanItem* pItem);

//Attributes
private:
	CFileCleanItemArray m_ItemArray;
	//CImageList m_images;
	bool m_bUpdatingList;
	bool m_bUserListEvent;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONPAGEFILECLEAN_H__C556B114_5207_11D6_AF06_00201855324E__INCLUDED_)
