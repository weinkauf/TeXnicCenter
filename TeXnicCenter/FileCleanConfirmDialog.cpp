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
#include "texniccenter.h"
#include "FileCleanConfirmDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileCleanConfirmDialog dialog


CFileCleanConfirmDialog::CFileCleanConfirmDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CFileCleanConfirmDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileCleanConfirmDialog)
	m_CleanedFiles = _T("");
	m_ProtectedFiles = _T("");
	m_bShowDialog = FALSE;
	//}}AFX_DATA_INIT
}


void CFileCleanConfirmDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileCleanConfirmDialog)
	DDX_Text(pDX, IDC_FCCONFIRM_CLEANEDFILES, m_CleanedFiles);
	DDX_Text(pDX, IDC_FCCONFIRM_PROTECTEDFILES, m_ProtectedFiles);
	DDX_Check(pDX, IDC_FCCONFIRM_SHOWDIALOG, m_bShowDialog);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileCleanConfirmDialog, CDialog)
	//{{AFX_MSG_MAP(CFileCleanConfirmDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileCleanConfirmDialog message handlers
