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

#if !defined(AFX_INSERTHEADERDIALOG_H__F884E223_4371_11D3_929E_A6DAFD7685C8__INCLUDED_)
#define AFX_INSERTHEADERDIALOG_H__F884E223_4371_11D3_929E_A6DAFD7685C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InsertHeaderDialog.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CInsertHeaderDialog 

class CInsertHeaderDialog : public CDialog
{
// types
public:

// construction/destruction
public:
	CInsertHeaderDialog(CWnd* pParent = NULL);   // Standardkonstruktor

// operations
public:
	/**
	Sets the properties of the dialog.

	@param strProperties
		The string to interpret. Linebreaks must be represented by '\r'.
	*/
	virtual BOOL SetProperties( const CString &strProperties );

	/**
	Gets a string that describes the LaTeX-environment with the
	properties specified by the dialog. 
	
	Linebreaks are represented by '\r'.
	(i.e. "\r\\begin{table*}[htb]\r\t\001\r\\end{table*}\r")
	*/
	virtual CString GetProperties();

// implementation
protected:
	//{{AFX_VIRTUAL(CInsertHeaderDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CInsertHeaderDialog)
	afx_msg void OnChangeTitle();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeDepth();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// attributes
public:
	//{{AFX_DATA(CInsertHeaderDialog)
	enum { IDD = IDD_HEADER };
	CEdit	m_wndLabelEdit;
	CString	m_strLabel;
	CString	m_strTitle;
	int		m_nDepth;
	//}}AFX_DATA
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_INSERTHEADERDIALOG_H__F884E223_4371_11D3_929E_A6DAFD7685C8__INCLUDED_
