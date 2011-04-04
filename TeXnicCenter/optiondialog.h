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

#if !defined(AFX_OPTIONDIALOG_H__FD645162_2E39_11D3_929E_FAB22A26316D__INCLUDED_)
#define AFX_OPTIONDIALOG_H__FD645162_2E39_11D3_929E_FAB22A26316D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionDialog.h : Header-Datei
//
#include "OptionPagePath.h"
#include "OptionPageGeneric.h"
#include "OptionPageFile.h"
#include "OptionPageLanguage.h"
#include "OptionPageFileClean.h"
#include "OptionPageEditor.h"
#include "OptionPageTextFormat.h"

/**	Property Sheet displaying all Option Pages.

@ingroup dialogs

@see COptionPageGeneric
@see COptionPageFile
@see COptionPagePath
@see COptionPageLanguage
@see COptionPageFileClean

@author Sven Wiegand
 */
class COptionDialog : public CMFCPropertySheet
{
	DECLARE_DYNAMIC(COptionDialog)

// construction/destruction
public:
	COptionDialog(CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~COptionDialog();

// operations
public:

// overridings
protected:
//{{AFX_VIRTUAL(COptionDialog)
public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// message handlers
protected:
	//{{AFX_MSG(COptionDialog)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// attributes
protected:
	COptionPageGeneric m_pageGeneric;
	COptionPageFile m_pageFile;
	COptionPagePath m_pagePath;
	COptionPageLanguage m_pageLanguage;
	COptionPageFileClean m_pageFileClean;
	COptionPageEditor m_pageEditor;
	COptionPageTextFormat m_pageTextFormat;

public:
	INT_PTR DoModal();
protected:
	static int CALLBACK PropSheetCallback(HWND hWnd, UINT message, LPARAM lParam);
public:
	void BuildPropPageArray(void);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_OPTIONDIALOG_H__FD645162_2E39_11D3_929E_FAB22A26316D__INCLUDED_
