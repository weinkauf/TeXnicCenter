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

#include "stdafx.h"
#include "resource.h"
#include "ItemPropertyDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CItemPropertyDialog

CItemPropertyDialog::CItemPropertyDialog(CWnd* pParent /*=NULL*/)
		: CDialog(CItemPropertyDialog::IDD,pParent)
{
	//{{AFX_DATA_INIT(CItemPropertyDialog)
	//m_strCaption = _T("");
	//m_strFile = _T("");
	//m_strLabel = _T("");
	//m_strLine = _T("");
	//m_strTitle = _T("");
	//m_strComment = _T("");
	//}}AFX_DATA_INIT
}

CItemPropertyDialog::CItemPropertyDialog(StructureItem &si,CWnd* pParent /*=NULL*/)
		: CDialog(CItemPropertyDialog::IDD,pParent)
{
	//{{AFX_DATA_INIT(CItemPropertyDialog)
	m_strCaption = si.m_strCaption;
	m_strFile = si.m_strPath;
	m_strLabel = si.GetLabel();
	m_strLine.Format(_T("%d"),si.m_nLine);
	m_strTitle = si.GetTitle();
	m_strComment = si.GetComment();
	//}}AFX_DATA_INIT
}

void CItemPropertyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CItemPropertyDialog)
	DDX_Text(pDX,IDC_CAPTION,m_strCaption);
	DDX_Text(pDX,IDC_FILE,m_strFile);
	DDX_Text(pDX,IDC_LABEL,m_strLabel);
	DDX_Text(pDX,IDC_LINE,m_strLine);
	DDX_Text(pDX,IDC_TITLE,m_strTitle);
	DDX_Text(pDX,IDC_COMMENT,m_strComment);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CItemPropertyDialog,CDialog)
	//{{AFX_MSG_MAP(CItemPropertyDialog)
	// HINWEIS: Der Klassen-Assistent fügt hier Zuordnungsmakros für Nachrichten ein
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CItemPropertyDialog
