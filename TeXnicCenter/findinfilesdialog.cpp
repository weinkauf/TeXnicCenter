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
#include "FindInFilesDialog.h"
#include "Configuration.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// class CFindInFilesDialog 
//-------------------------------------------------------------------


BEGIN_MESSAGE_MAP(CFindInFilesDialog, CDialog)
	//{{AFX_MSG_MAP(CFindInFilesDialog)
	ON_BN_CLICKED(IDC_POPUP_REGULAREXPRESSION, OnPopupRegularExpression)
	ON_CBN_SELCHANGE(IDC_SEARCH_SEARCHTHROUGH, OnSelChange)
	ON_CBN_SELCHANGE(IDC_SEARCH_SEARCHIN, OnSelChange)
	ON_CBN_SELCHANGE(IDC_SEARCH_SEARCHFOR, OnSelChange)
	ON_CBN_EDITCHANGE(IDC_SEARCH_SEARCHFOR, UpdateControls)
	ON_CBN_EDITCHANGE(IDC_SEARCH_SEARCHIN, UpdateControls)
	ON_CBN_EDITCHANGE(IDC_SEARCH_SEARCHTHROUGH, UpdateControls)
	ON_CBN_SETFOCUS(IDC_SEARCH_SEARCHIN, UpdateControls)
	//}}AFX_MSG_MAP
	ON_COMMAND_EX_RANGE(ID_REGEX_FIRST, ID_REGEX_LAST, OnInsertRegularExpression)
END_MESSAGE_MAP()


CFindInFilesDialog::CFindInFilesDialog(CWnd* pParent /*=NULL*/)
: CDialog(CFindInFilesDialog::IDD, pParent),
	m_wndSearchForCombo( _T("Settings\\FindInFiles\\MRUQueries"), _T("Query%d") ),
	m_wndSearchInCombo( _T("Settings\\FindInFiles\\MRUDirectories"), _T("Directory%d") ),
	m_BrowseBtn(IDC_SEARCH_SEARCHIN, CString((LPCTSTR)STE_FILEFIND_BROWSEDIR))
{
	m_bCaseSensitive = g_configuration.m_bFileFindCaseSensitive;
	m_bIncludeSubFolders = g_configuration.m_bFileFindSubFolders;
	m_nOutput = g_configuration.m_nFileFindOutput;
	m_bRegularExpression = g_configuration.m_bFileFindRegularExpression;
	m_strSearchThrough = g_configuration.m_strFileFindFileTypes;
	m_bWholeWords = g_configuration.m_bFileFindWholeWords;
	m_strSearchFor = _T("");
	m_strSearchIn = _T("");
	//{{AFX_DATA_INIT(CFindInFilesDialog)
	//}}AFX_DATA_INIT
}


void CFindInFilesDialog::DoDataExchange(CDataExchange* pDX)
{

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindInFilesDialog)
	DDX_Control(pDX, IDC_SEARCH_BROWSE_SEARCHIN, m_BrowseBtn);
	DDX_Control(pDX, IDC_SEARCH_REGULAREXPRESSION, m_wndRegExCheck);
	DDX_Control(pDX, IDOK, m_wndOKButton);
	DDX_Control(pDX, IDC_SEARCH_SEARCHTHROUGH, m_wndSearchThroug);
	DDX_Control(pDX, IDC_SEARCH_SEARCHFOR, m_wndSearchForCombo);
	DDX_Control(pDX, IDC_SEARCH_SEARCHIN, m_wndSearchInCombo);
	DDX_Check(pDX, IDC_SEARCH_CASESENSITIVE, m_bCaseSensitive);
	DDX_Check(pDX, IDC_SEARCH_INCLUDESUBFOLDERS, m_bIncludeSubFolders);
	DDX_Radio(pDX, IDC_SEARCH_OUTPUT1, m_nOutput);
	DDX_Check(pDX, IDC_SEARCH_REGULAREXPRESSION, m_bRegularExpression);
	DDX_CBString(pDX, IDC_SEARCH_SEARCHFOR, m_strSearchFor);
	DDX_CBString(pDX, IDC_SEARCH_SEARCHIN, m_strSearchIn);
	DDX_CBString(pDX, IDC_SEARCH_SEARCHTHROUGH, m_strSearchThrough);
	DDX_Check(pDX, IDC_SEARCH_WHOLEWORDS, m_bWholeWords);
	//}}AFX_DATA_MAP
}


void CFindInFilesDialog::UpdateControls()
{
	if( !IsWindow( m_wndOKButton.m_hWnd ) )
		return;

	UpdateData();

	// enable/disable OK Button
	m_wndOKButton.EnableWindow( 
		!m_strSearchFor.IsEmpty() && 
		!m_strSearchIn.IsEmpty() && 
		!m_strSearchThrough.IsEmpty() );
}


BOOL CFindInFilesDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// fill controls
	m_wndSearchForCombo.RefreshCtrl();
	m_wndSearchInCombo.RefreshCtrl();

	// set standard strings
	m_wndSearchInCombo.SetCurSel( 0 );
	if( m_strSearchThrough.IsEmpty() )
		m_wndSearchThroug.SetCurSel( 0 );

	UpdateControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}


void CFindInFilesDialog::OnPopupRegularExpression() 
{
	// get position of button
	CWnd	*pButton = GetDlgItem( IDC_POPUP_REGULAREXPRESSION );
	if( !pButton )
		return;

	CRect	buttonRect;
	pButton->GetWindowRect( buttonRect );

	// Create and display menu
	CMenu	popupMenu;
	if( !popupMenu.LoadMenu( IDR_POPUP_REGULAREXPRESSION ) )
		return;

	popupMenu.GetSubMenu( 0 )->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON, buttonRect.right, buttonRect.top, this );
}


//void CFindInFilesDialog::OnBrowseSearchIn() 
//{
//	CFolderSelect	fs( CString( (LPCTSTR)STE_FILEFIND_BROWSEDIR ) );
//
//	if( fs.DoModal() != IDOK )
//		return;
//
//	m_strSearchIn = fs.GetPath();
//
//	UpdateData( FALSE );
//
//	UpdateControls();
//}


void CFindInFilesDialog::OnInsertRegularExpression( UINT unID )
{
	// set cursor
	m_wndSearchForCombo.SetEditSel( 
		m_wndSearchForCombo.GetRecentEditSelStart(),
		m_wndSearchForCombo.GetRecentEditSelEnd() );

	//
	CString	strRegEx( (LPCTSTR)unID );
	int			nCursorOffset = 0;

	for( int i = 0; i < strRegEx.GetLength(); i++ )
	{
		if( strRegEx[i] == _T('\001') )
		{
			nCursorOffset = strRegEx.GetLength() - i - 1;
			if( nCursorOffset < 0 )
				nCursorOffset = 0;

			continue;
		}

		m_wndSearchForCombo.SendMessage( WM_CHAR, strRegEx[i] );
	}

	// set cursor
	int		nPosition = LOWORD(m_wndSearchForCombo.GetEditSel()) - nCursorOffset;
	m_wndSearchForCombo.SetFocus();
	m_wndSearchForCombo.SetEditSel( nPosition, nPosition );

	// check regular expression
	m_wndRegExCheck.SetCheck( 1 );
}


void CFindInFilesDialog::OnOK() 
{
	UpdateData();

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// check data

	// is directory existing?
	CFileFind	ff;
	if( !ff.FindFile( m_strSearchIn ) )
	{
		AfxMessageBox( STE_FILEFIND_NOFOLDER, MB_ICONEXCLAMATION | MB_OK );
		return;
	}

	// is expression valid?
	try
	{
		RegEx	re;

		if( m_bRegularExpression )
			re.SetExpression( m_strSearchFor );
		else if( m_strSearchFor.IsEmpty() )
			throw FALSE;
	}
	catch( ... )
	{
		AfxMessageBox( STE_FILEFIND_INVALIDEXPRESSION, MB_ICONEXCLAMATION | MB_OK );
		return;
	}
			
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// store settings
	m_wndSearchForCombo.AddToMRU( m_strSearchFor );
	m_wndSearchInCombo.AddToMRU( m_strSearchIn );
	
	g_configuration.m_bFileFindCaseSensitive = m_bCaseSensitive;
	g_configuration.m_bFileFindSubFolders = m_bIncludeSubFolders;
	g_configuration.m_nFileFindOutput = m_nOutput;
	g_configuration.m_bFileFindRegularExpression = m_bRegularExpression;
	g_configuration.m_strFileFindFileTypes = m_strSearchThrough;
	g_configuration.m_bFileFindWholeWords = m_bWholeWords;

	// standard implementation
	CDialog::OnOK();
}


void CFindInFilesDialog::OnSelChange()
{
	// inform, the content of edit control has changed
	PostMessage( WM_COMMAND, (WPARAM)(CBN_EDITCHANGE << 16 | IDC_SEARCH_SEARCHFOR), (LPARAM)m_wndSearchForCombo.m_hWnd );
}
