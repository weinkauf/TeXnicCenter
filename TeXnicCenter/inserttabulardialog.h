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

#if !defined(AFX_INSERTTABULARDIALOG_H__F884E222_4371_11D3_929E_A6DAFD7685C8__INCLUDED_)
#define AFX_INSERTTABULARDIALOG_H__F884E222_4371_11D3_929E_A6DAFD7685C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InsertTabularDialog.h : Header-Datei
//
#include "InsertFloatObjectDialog.h"

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CInsertTabularDialog 

class CInsertTabularDialog : public CInsertFloatObjectDialog
{
// types
public:
	//{{AFX_DATA(CInsertTabularDialog)
	enum { IDD = IDD_TABULAR };
	BOOL	m_bHCenter;
	BOOL	m_bFloat;
	int		m_nPosition;
	//}}AFX_DATA

// construction/destruction
public:
	CInsertTabularDialog(CWnd* pParent = NULL);   // Standardkonstruktor


// operations
public:
	virtual BOOL SetProperties( const CString &strProperties );
	virtual CString GetProperties();


// implementation helpers
protected:
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CInsertTabularDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CInsertTabularDialog)
	afx_msg void OnTabularFloat();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_INSERTTABULARDIALOG_H__F884E222_4371_11D3_929E_A6DAFD7685C8__INCLUDED_
