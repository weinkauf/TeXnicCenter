/********************************************************************
*
* This file is part of the TeXnicCenter-system
*
* Copyright (C) 2002 Chris Norris
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

// OptionPageLanguage.cpp : implementation file
//

#include "stdafx.h"
#include "TeXnicCenter.h"
#include "OptionPageLanguage.h"
#include "Configuration.h"
#include "global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionPageLanguage property page

IMPLEMENT_DYNCREATE(COptionPageLanguage, CPropertyPage)

COptionPageLanguage::COptionPageLanguage() : CPropertyPage(COptionPageLanguage::IDD)
{
	//{{AFX_DATA_INIT(COptionPageLanguage)
	m_bEnableSpell = g_configuration.m_bSpellEnable;
	m_bMainDictOnly = g_configuration.m_bSpellMainDictOnly;
	m_bSkipComments = g_configuration.m_bSpellSkipComments;
	m_bSkipTags = g_configuration.m_bSpellSkipTags;
	m_bSkipCaps = g_configuration.m_bSpellSkipCaps;
	m_bSkipNumbers = g_configuration.m_bSpellSkipNumbers;
	m_strLanguage = g_configuration.m_strLanuage;
	m_strDialect = g_configuration.m_strLanguageDialect;
	m_strPDictionary = g_configuration.m_strSpellPersonalDictionary;
	//}}AFX_DATA_INIT
}


void COptionPageLanguage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionPageLanguage)
	DDX_Check(pDX, IDC_OPTIONS_SPELL_COMMENT, m_bSkipComments);
	DDX_Check(pDX, IDC_OPTIONS_SPELL_IGNORE_NUM, m_bSkipNumbers);
	DDX_Check(pDX, IDC_OPTIONS_SPELL_MAINDICT, m_bMainDictOnly);
	DDX_Check(pDX, IDC_OPTIONS_SPELL_SKIP_TAGS, m_bSkipTags);
	DDX_Check(pDX, IDC_OPTIONS_SPELL_IGNORE_ALLCAPS, m_bSkipCaps);
	DDX_CBString(pDX, IDC_OPTIONS_LANGUAGE, m_strLanguage);
	DDX_CBString(pDX, IDC_OPTIONS_LANGUAGE_DIALECT, m_strDialect);
	DDX_Text(pDX, IDC_OPTIONS_SPELL_PDICT, m_strPDictionary);
	DDX_Check(pDX, IDC_OPTIONS_SPELL_ENABLE, m_bEnableSpell);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionPageLanguage, CPropertyPage)
	//{{AFX_MSG_MAP(COptionPageLanguage)
	ON_CBN_SELCHANGE(IDC_OPTIONS_LANGUAGE, OnSelchangeOptionsLanguage)
	ON_BN_CLICKED(IDC_OPTIONS_SPELL_BROWSE, OnOptionsSpellBrowse)
	ON_BN_CLICKED(IDC_OPTIONS_SPELL_ENABLE, UpdateControlStates)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionPageLanguage message handlers

void COptionPageLanguage::OnOK() 
{
	g_configuration.m_bSpellEnable = m_bEnableSpell;
	g_configuration.m_strLanuage = m_strLanguage;
	g_configuration.m_strLanguageDialect = m_strDialect;
	g_configuration.m_strSpellPersonalDictionary = m_strPDictionary;
	g_configuration.m_bSpellSkipComments = m_bSkipComments;
	g_configuration.m_bSpellSkipNumbers = m_bSkipNumbers;
	g_configuration.m_bSpellSkipTags = m_bSkipTags;
	g_configuration.m_bSpellSkipCaps = m_bSkipCaps;
	g_configuration.m_bSpellMainDictOnly = m_bMainDictOnly;

	if ( m_bEnableSpell  && !m_strPDictionary.IsEmpty() && !::PathFileExists(m_strPDictionary) )
	{
		CString errMsg;
		errMsg.Format( STE_PDICT_OPEN_ERROR, m_strPDictionary );
		AfxMessageBox( errMsg, MB_OK, MB_ICONINFORMATION );
	}

	// Inform the background thread of the new speller state.
	theApp.GetBackgroundThread()->PostThreadMessage(ID_BG_ENABLE_SPELLER, m_bEnableSpell, NULL);
	if ( g_configuration.m_bSpellEnable )
	{
		CSpellerSource *pSource = static_cast<CSpellerSource*>(&theApp);
		theApp.GetBackgroundThread()->PostThreadMessage(ID_BG_RESET_SPELLER, 0, (long) pSource);
	}
	AfxGetMainWnd()->PostMessage( WM_COMMAND, ID_BG_UPDATE_PROJECT ); // clear or set the line attributes

	CPropertyPage::OnOK();
}


void COptionPageLanguage::OnSelchangeOptionsLanguage() 
{
	CComboBox *pDialBox = (CComboBox*) GetDlgItem(IDC_OPTIONS_LANGUAGE_DIALECT);
	CComboBox *pLangBox = (CComboBox*) GetDlgItem(IDC_OPTIONS_LANGUAGE);

	pDialBox->ResetContent();
	if (pLangBox->GetCurSel() == -1)
		return;

	CString lang;
	pLangBox->GetLBText( pLangBox->GetCurSel(), lang );
	for (int i = 0; i < m_aDialect.GetSize(); ++i)
	{
		if ( lang == m_aLanguage[i] )
			pDialBox->AddString( m_aDialect[i] );
	}

	// Select the dialect
	int nSel = pDialBox->FindStringExact( 0, m_strDialect );
	if ( nSel == CB_ERR )
		pDialBox->SetCurSel( 0 );
	else
		pDialBox->SetCurSel( nSel );
}


BOOL COptionPageLanguage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	FindDictionaries();
	
	CComboBox *pLangBox = (CComboBox*)GetDlgItem(IDC_OPTIONS_LANGUAGE);

	// Add all the found languages
	for (int i = 0; i < m_aLanguage.GetSize(); ++i)
	{
		if ( pLangBox->FindString( 0, m_aLanguage[i] ) == CB_ERR )
			pLangBox->AddString (m_aLanguage[i] );
	}

	// Select the language
	int nSel = pLangBox->FindStringExact( 0, m_strLanguage );
	if ( nSel == CB_ERR )
		pLangBox->SetCurSel( 0 );
	else
		pLangBox->SetCurSel( nSel );

	OnSelchangeOptionsLanguage();
	UpdateControlStates();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void COptionPageLanguage::FindDictionaries()
{
	m_aLanguage.RemoveAll();
	m_aDialect.RemoveAll();

	CString dicFileMatch = g_configuration.m_strSpellDictionaryPath+_T("\\*.dic");

	WIN32_FIND_DATA  dirInfo;
	HANDLE hFile;
	boolean bNext = true;

	hFile = FindFirstFile( dicFileMatch, &dirInfo );
	while ( hFile != INVALID_HANDLE_VALUE && bNext )
	{
		// Dictionary file format LANG_DIALACT{-extra}?.dic
		// Example: de_DE.dic en_US-slang.dic 
		// Get the language and dialect of all installed dictionaries.
		TCHAR* dash;
		TCHAR* dot;
		dash = _tcschr( dirInfo.cFileName, _T('_') );
		if (dash != NULL)
			dot = _tcschr( dash, _T('.') );
		if ( (dash != NULL) && (dot != NULL) ) 
		{
			CString lang( dirInfo.cFileName, dash-dirInfo.cFileName );
			CString dialect( dash+1, dot-dash-1);
			m_aDialect.Add( dialect );
			m_aLanguage.Add( lang );
		}
		bNext = FindNextFile( hFile, &dirInfo );
	}
	FindClose( hFile );
}


void COptionPageLanguage::OnOptionsSpellBrowse() 
{
	CWnd *pPersonalDictionary = GetDlgItem(IDC_OPTIONS_SPELL_PDICT);
	CString defaultPath;
	pPersonalDictionary->GetWindowText( defaultPath );
	CFileDialog dlg ( true, NULL, NULL, OFN_PATHMUSTEXIST, 
		AfxLoadString(STE_FILE_DICFILTER) );
	if ( !defaultPath.IsEmpty() )
	{
		TCHAR * pathStr = defaultPath.GetBuffer(0);
		::PathRemoveFileSpec( pathStr );
		dlg.m_ofn.lpstrInitialDir = pathStr;
		defaultPath.ReleaseBuffer();
	}
	if ( dlg.DoModal() == IDOK )
		pPersonalDictionary->SetWindowText( dlg.GetPathName() );
}


void COptionPageLanguage::UpdateControlStates()
{
	UpdateData();
	GetDlgItem(IDC_OPTIONS_SPELL_MAINDICT)->EnableWindow(m_bEnableSpell);
	GetDlgItem(IDC_OPTIONS_SPELL_COMMENT)->EnableWindow(m_bEnableSpell);
	GetDlgItem(IDC_OPTIONS_SPELL_SKIP_TAGS)->EnableWindow(m_bEnableSpell);
	GetDlgItem(IDC_OPTIONS_SPELL_IGNORE_NUM)->EnableWindow(m_bEnableSpell);
	GetDlgItem(IDC_OPTIONS_SPELL_IGNORE_ALLCAPS)->EnableWindow(m_bEnableSpell);
	GetDlgItem(IDC_OPTIONS_SPELL_PDICT)->EnableWindow(m_bEnableSpell);
	GetDlgItem(IDC_OPTIONS_SPELL_BROWSE)->EnableWindow(m_bEnableSpell);
}
