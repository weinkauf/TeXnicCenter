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

#if !defined(AFX_USERTOOLADVDLG_H__F97DBD05_AB35_11D6_AF7D_00201855324E__INCLUDED_)
#define AFX_USERTOOLADVDLG_H__F97DBD05_AB35_11D6_AF7D_00201855324E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserToolAdvDlg.h : header file
//

#include "PersistPosEdit.h"
#include "EditMenuButtonOpt.h"

/////////////////////////////////////////////////////////////////////////////
// CUserToolAdvDlg dialog

class CUserToolAdvDlg : public CDialog
{
// Construction
public:
	CUserToolAdvDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUserToolAdvDlg)
	enum { IDD = IDD_CUSTOMIZE_TOOLS_ADV };
	CButton	m_wndCloseWindowBtn;
	CPersistPosEdit	m_wndInputFile;
	CPersistPosEdit	m_wndOutputFile;
	CEditMenuButtonOpt	m_wndOutputBtn;
	CEditMenuButtonOpt	m_wndInputBtn;
	CButton	m_wndUseOutputBtn;
	BOOL	m_bCloseConsoleWindow;
	BOOL	m_bPromptForArguments;
	BOOL	m_bUseOutputWindow;
	CString	m_strInputFile;
	CString	m_strOutputFile;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserToolAdvDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUserToolAdvDlg)
	afx_msg void OnUseOutput();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERTOOLADVDLG_H__F97DBD05_AB35_11D6_AF7D_00201855324E__INCLUDED_)
