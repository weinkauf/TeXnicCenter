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

#if !defined(AFX_GOTODIALOG_H__B72DAA01_5401_11D3_929E_9164A4E94415__INCLUDED_)
#define AFX_GOTODIALOG_H__B72DAA01_5401_11D3_929E_9164A4E94415__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/** Allows the user to jump directly to a line in the active document.

@ingroup dialogs

@author Sven Wiegand
*/
class CGotoDialog : public CDialog
{
// construction/destruction
public:
	CGotoDialog( long nMaxLine = 0, CWnd* pParent = NULL );   // Standardkonstruktor

// overwritings
protected:
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CGotoDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// message handlers
protected:
	//{{AFX_MSG(CGotoDialog)
	afx_msg void OnChangeGotoLineEdit();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// attributes
public:
	/** highes valid line number. */
	long m_nMaxLine;

	//{{AFX_DATA(CGotoDialog)
	enum { IDD = IDD_GOTO };
	CSpinButtonCtrl	m_wndLineSpin;
	CButton	m_wndOkButton;
	long	m_nLine;
	//}}AFX_DATA
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_GOTODIALOG_H__B72DAA01_5401_11D3_929E_9164A4E94415__INCLUDED_
