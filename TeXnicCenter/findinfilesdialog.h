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

#if !defined(AFX_FINDINFILESDIALOG_H__A010A5E8_1F74_11D4_A221_006097239934__INCLUDED_)
#define AFX_FINDINFILESDIALOG_H__A010A5E8_1F74_11D4_A221_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MRUTrackSelComboBox.h"
#include "BrowseButton.h"

class CFindInFilesDialog : public CDialog
{
// construction/destruction
public:
	CFindInFilesDialog(CWnd* pParent = NULL);

// implementation helper
protected:
	/** enables or disables controls in dependence of the field data. */
	void UpdateControls();

// overridings
protected:
	//{{AFX_VIRTUAL(CFindInFilesDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// message handlers
protected:
	afx_msg void OnInsertRegularExpression( UINT unID );
	//{{AFX_MSG(CFindInFilesDialog)
	afx_msg void OnPopupRegularExpression();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// field data
public:
	//{{AFX_DATA(CFindInFilesDialog)
	enum { IDD = IDD_FILESEARCH };
	CBrowseButton	m_BrowseBtn;
	CButton	m_wndRegExCheck;
	CButton	m_wndOKButton;
	CComboBox	m_wndSearchThroug;
	CMRUTrackSelComboBox	m_wndSearchForCombo;
	CMRUComboBox	m_wndSearchInCombo;
	BOOL	m_bCaseSensitive;
	BOOL	m_bIncludeSubFolders;
	int		m_nOutput;
	BOOL	m_bRegularExpression;
	CString	m_strSearchFor;
	CString	m_strSearchIn;
	CString	m_strSearchThrough;
	BOOL	m_bWholeWords;
	//}}AFX_DATA

// attributes
protected:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_FINDINFILESDIALOG_H__A010A5E8_1F74_11D4_A221_006097239934__INCLUDED_
