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

#if !defined(AFX_TEXTMODULESDLG_H__B7379B85_2D99_11D8_B1A8_00095B41B65A__INCLUDED_)
#define AFX_TEXTMODULESDLG_H__B7379B85_2D99_11D8_B1A8_00095B41B65A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TabPersistPosEdit.h"

/** Manages TextModules.

@todo Parts of this need to be reimplemented
when we use more than one group of TextModules.
Especially the ListCtrl needs to be replaced by a TreeCtrl.

@ingroup dialogs
@ingroup textmodules

@author Tino Weinkauf
*/
class CTextModulesDlg : public CDialog
{
// Construction
public:
	CTextModulesDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTextModulesDlg)
	enum { IDD = IDD_TEXTMODULES };
	CTabPersistPosEdit	m_wndBeforeEdit;
	CTabPersistPosEdit	m_wndAfterEdit;
	CEdit	m_wndNameEdit;
	CListCtrl	m_wndModulesList;
	CButton	m_wndDelBtn;
	CButton	m_wndNewBtn;
	CString	m_strRight;
	CString	m_strLeft;
	CString	m_strName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextModulesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTextModulesDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedModules(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnNewTextModule();
	afx_msg void OnDeleteTextModule();
	afx_msg void OnChangeName();
	afx_msg void OnKillFocusName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

//Methods
protected:
	void UpdateTree(int nIDSelect = -1);
	void UpdateControls();
	void ClearControls();
	void UpdateOldSelectedItem();
	CTextModule* GetSelectedModule();
	void UpdateNameInTree(const int Idx, CString NewName);
	int InsertNewModule(CTextModule& tm, const CString& NewName);

//Attributes
public:
	CTextModuleGroup m_tmGroup;

protected:
	int nOldSelectedItem;
	bool bLock;
	bool bNameChanged;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTMODULESDLG_H__B7379B85_2D99_11D8_B1A8_00095B41B65A__INCLUDED_)
