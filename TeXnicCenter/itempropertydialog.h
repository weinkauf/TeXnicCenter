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

#if !defined(AFX_ITEMPROPERTYDIALOG_H__C6715540_3C76_11D3_929E_B8DAAFC2017D__INCLUDED_)
#define AFX_ITEMPROPERTYDIALOG_H__C6715540_3C76_11D3_929E_B8DAAFC2017D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StructureParser.h"

/**	Displays information about a structure item.

@ingroup dialogs

@see CStructureItem

@author Sven Wiegand
 */
class CItemPropertyDialog : public CDialog
{
// Konstruktion
public:
	CItemPropertyDialog(CWnd* pParent = NULL); // Standardkonstruktor
	CItemPropertyDialog(CStructureItem &si, CWnd* pParent = NULL);

	// Dialogfelddaten
	//{{AFX_DATA(CItemPropertyDialog)

	enum
	{
		IDD = IDD_ITEM_PROPERTIES
	};
	CString m_strCaption;
	CString m_strFile;
	CString m_strLabel;
	CString m_strLine;
	CString m_strTitle;
	CString m_strComment;
	//}}AFX_DATA


	// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
//{{AFX_VIRTUAL(CItemPropertyDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CItemPropertyDialog)
	// HINWEIS: Der Klassen-Assistent fügt hier Member-Funktionen ein
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_ITEMPROPERTYDIALOG_H__C6715540_3C76_11D3_929E_B8DAAFC2017D__INCLUDED_
