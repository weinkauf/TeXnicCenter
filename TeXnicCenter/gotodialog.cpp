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
#include "TeXnicCenter.h"
#include "GotoDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CGotoDialog 


CGotoDialog::CGotoDialog( long nMaxLine /*= 0*/, CWnd* pParent /*=NULL*/)
: CDialog(CGotoDialog::IDD, pParent),
	m_nMaxLine( nMaxLine )
{
	//{{AFX_DATA_INIT(CGotoDialog)
	m_nLine = 1;
	//}}AFX_DATA_INIT
}


void CGotoDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGotoDialog)
	DDX_Control(pDX, IDC_GOTO_LINE_SPIN, m_wndLineSpin);
	DDX_Control(pDX, IDOK, m_wndOkButton);
	DDX_Text(pDX, IDC_GOTO_LINE_EDIT, m_nLine);
	//}}AFX_DATA_MAP
	DDV_MinMaxLong(pDX, m_nLine, 1, m_nMaxLine);
}


BEGIN_MESSAGE_MAP(CGotoDialog, CDialog)
	//{{AFX_MSG_MAP(CGotoDialog)
	ON_EN_CHANGE(IDC_GOTO_LINE_EDIT, OnChangeGotoLineEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CGotoDialog 

void CGotoDialog::OnChangeGotoLineEdit() 
{
	// check if controls are allready created
	if( !IsWindow( m_wndLineSpin.m_hWnd ) )
		return;

	UpdateData();
	m_wndOkButton.EnableWindow( m_nLine >= 0 && m_nLine <= m_nMaxLine );
}

BOOL CGotoDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// set range of line spin
	m_wndLineSpin.SetRange( 1, m_nMaxLine );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}
