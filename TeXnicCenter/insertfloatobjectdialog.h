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

#if !defined(AFX_INSERTFLOATOBJECTDIALOG_H__F884E220_4371_11D3_929E_A6DAFD7685C8__INCLUDED_)
#define AFX_INSERTFLOATOBJECTDIALOG_H__F884E220_4371_11D3_929E_A6DAFD7685C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InsertFloatObjectDialog.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CInsertFloatObjectDialog 

class CInsertFloatObjectDialog : public CDialog
{
// types
public:
	enum tagFloatObjectType
	{
		figure = 0,
		table
	};

// construction/desctruction
public:
	/**
	Constructs a CInsertFloatObjectDialog-object.

	@param nType
		Type of the object to insert. Can be CInsertFloatDialog::figure or CInsertFloatDialog::table.
	@param pParent
		Pointer to the parent window or NULL if there is none.
	*/
	CInsertFloatObjectDialog( int nType = figure,	CWnd* pParent = NULL);

protected:
	/**
	Used by derived classes.

	@param nIDTemplate
		ID of the dialog-template to use.
	@param nType
		Type of the object to insert (figure/table)
	@param bAutoLabel
		<var>TRUE</var> if the label should be generated automatically out of the
		caption, <var>FALSE</var> if the derived class generates the label.
	@param pParent
		Parent window or <var>NULL</var> if there is none.
	*/
	CInsertFloatObjectDialog( UINT nIDTemplate, int nType, BOOL bAutoLabel = TRUE, CWnd *pParent = NULL );


// overridables
protected:
	//{{AFX_VIRTUAL(CInsertFloatObjectDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

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

protected:
	/**
	Enables or disables all the float-object related windows.

	Can be called by derived classes.

	@param bEnable
		TRUE to enable, FALSE to disable the windows.
	*/
	virtual void EnableFloatOptions( BOOL bEnable = TRUE );

// message handlers
protected:
	//{{AFX_MSG(CInsertFloatObjectDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeFloatCaption();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// attributes
public:
	//{{AFX_DATA(CInsertFloatObjectDialog)
	enum { IDD = IDD_FLOATOBJECT };
	CButton m_wndPosTwoCol;
	CButton	m_wndPosOneCol;
	CStatic	m_wndLabelLabel;
	CStatic	m_wndLabelCaption;
	CButton	m_wndGroupWidth;
	CButton	m_wndGroupPos;
	CButton	m_wndPosTop;
	CButton	m_wndPosPage;
	CButton	m_wndPosHere;
	CButton	m_wndPosBottom;
	CEdit	m_wndCaptionEdit;
	CEdit	m_wndLabelEdit;
	CString	m_strCaption;
	CString	m_strLabel;
	BOOL	m_bPosBottom;
	BOOL	m_bPosHere;
	BOOL	m_bPosPage;
	BOOL	m_bPosTop;
	int		m_nWidth;
	//}}AFX_DATA

	/** The contents of the environment. */
	CString m_strContents;

	/** <var>TRUE</var> if this class generates the label, <var>FALSE</var> otherwise. */
	BOOL m_bAutoLabel;

protected:
	/** Type the dialog works on. */
	int m_nType;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_INSERTFLOATOBJECTDIALOG_H__F884E220_4371_11D3_929E_A6DAFD7685C8__INCLUDED_
