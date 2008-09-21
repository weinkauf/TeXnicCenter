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

/********************************************************************
 *
 * $Id$
 *
 ********************************************************************/

#include "stdafx.h"
#include "TeXnicCenter.h"
#include "OptionPageLanguage.h"
#include "Configuration.h"
#include "global.h"
#include <locale.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionPageLanguage property page

IMPLEMENT_DYNCREATE(COptionPageLanguage,CMFCPropertyPage)

COptionPageLanguage::COptionPageLanguage() : CMFCPropertyPage(COptionPageLanguage::IDD)
{
	m_bEnableSpell = CConfiguration::GetInstance()->m_bSpellEnable;
	m_bMainDictOnly = CConfiguration::GetInstance()->m_bSpellMainDictOnly;
	m_bSkipComments = CConfiguration::GetInstance()->m_bSpellSkipComments;
	m_bSkipNumbers = CConfiguration::GetInstance()->m_bSpellSkipNumbers;
	m_bSkipTags = CConfiguration::GetInstance()->m_bSpellSkipTags;
	m_bSkipCaps = CConfiguration::GetInstance()->m_bSpellSkipCaps;
	m_strLanguageDefault = CConfiguration::GetInstance()->m_strLanguageDefault;
	m_strDialectDefault = CConfiguration::GetInstance()->m_strLanguageDialectDefault;
	m_strPDictionary = CConfiguration::GetInstance()->m_strSpellPersonalDictionary;
}

void COptionPageLanguage::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionPageLanguage)
	DDX_Check(pDX,IDC_OPTIONS_SPELL_ENABLE,m_bEnableSpell);
	DDX_Check(pDX,IDC_OPTIONS_SPELL_MAINDICT,m_bMainDictOnly);
	DDX_Check(pDX,IDC_OPTIONS_SPELL_COMMENT,m_bSkipComments);
	DDX_Check(pDX,IDC_OPTIONS_SPELL_IGNORE_NUM,m_bSkipNumbers);
	DDX_Check(pDX,IDC_OPTIONS_SPELL_SKIP_TAGS,m_bSkipTags);
	DDX_Check(pDX,IDC_OPTIONS_SPELL_IGNORE_ALLCAPS,m_bSkipCaps);
	DDX_CBString(pDX,IDC_OPTIONS_LANGUAGE,m_strLanguageDefault);
	DDX_CBString(pDX,IDC_OPTIONS_LANGUAGE_DIALECT,m_strDialectDefault);
	DDX_Text(pDX,IDC_OPTIONS_SPELL_PDICT,m_strPDictionary);
	DDX_Control(pDX,IDC_OPTIONS_LANGUAGE_DOWNLOAD,m_wndURLDownloadDicts);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COptionPageLanguage,CMFCPropertyPage)
	//{{AFX_MSG_MAP(COptionPageLanguage)
	ON_CBN_SELCHANGE(IDC_OPTIONS_LANGUAGE,OnSelchangeOptionsLanguage)
	ON_BN_CLICKED(IDC_OPTIONS_SPELL_BROWSE,OnOptionsSpellBrowse)
	ON_BN_CLICKED(IDC_OPTIONS_SPELL_ENABLE,UpdateControlStates)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionPageLanguage message handlers

void COptionPageLanguage::OnOK()
{
	bool bShowNextStartInfo = (CConfiguration::GetInstance()->m_strLanguageDefault != m_strLanguageDefault);
	bShowNextStartInfo |= (CConfiguration::GetInstance()->m_strLanguageDialectDefault != m_strDialectDefault);
	if (bShowNextStartInfo) AfxMessageBox(STE_OPTIONS_REQUIRES_RESTART,MB_ICONINFORMATION | MB_OK);

	CConfiguration::GetInstance()->m_strLanguageDefault = m_strLanguageDefault;
	CConfiguration::GetInstance()->m_strLanguageDialectDefault = m_strDialectDefault;
	CConfiguration::GetInstance()->m_strSpellPersonalDictionary = m_strPDictionary;
	CConfiguration::GetInstance()->m_bSpellSkipComments = m_bSkipComments;
	CConfiguration::GetInstance()->m_bSpellSkipNumbers = m_bSkipNumbers;
	CConfiguration::GetInstance()->m_bSpellSkipTags = m_bSkipTags;
	CConfiguration::GetInstance()->m_bSpellSkipCaps = m_bSkipCaps;
	CConfiguration::GetInstance()->m_bSpellMainDictOnly = m_bMainDictOnly;
	CConfiguration::GetInstance()->m_bSpellEnable = m_bEnableSpell;

	if (m_bEnableSpell && !m_strPDictionary.IsEmpty() && !::PathFileExists(m_strPDictionary))
	{
		CString errMsg;
		errMsg.Format(STE_PDICT_OPEN_ERROR,m_strPDictionary);
		AfxMessageBox(errMsg,MB_OK,MB_ICONINFORMATION);
	}

	// Inform the background thread of the new speller state.
	theApp.GetSpellerThread()->EnableSpeller(m_bEnableSpell);

	if (m_bEnableSpell) 
	{
		SpellerSource* pSource = &theApp;
		theApp.GetSpellerThread()->ResetSpeller(pSource);
	}

	AfxGetMainWnd()->PostMessage(WM_COMMAND,ID_BG_UPDATE_PROJECT); // clear or set the line attributes

	CMFCPropertyPage::OnOK();
}

void COptionPageLanguage::OnSelchangeOptionsLanguage()
{
	CComboBox *pDialBox = (CComboBox*)GetDlgItem(IDC_OPTIONS_LANGUAGE_DIALECT);
	CComboBox *pLangBox = (CComboBox*)GetDlgItem(IDC_OPTIONS_LANGUAGE);

	pDialBox->ResetContent();
	if (pLangBox->GetCurSel() == -1)
		return;

	CString lang;
	pLangBox->GetLBText(pLangBox->GetCurSel(),lang);
	for (int i = 0; i < m_aDialect.GetSize(); ++i)
	{
		if (lang == m_aLanguage[i])
			pDialBox->AddString(m_aDialect[i]);
	}

	// Select the dialect
	int nSel = pDialBox->FindStringExact(0,m_strDialectDefault);
	if (nSel == CB_ERR)
		pDialBox->SetCurSel(0);
	else
		pDialBox->SetCurSel(nSel);
}

BOOL COptionPageLanguage::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();

	//Set URLs
	CString DictURL;
	DictURL.LoadString(ID_URL_TCDICTDOWNLOAD);
	int idLastLineBreak = DictURL.ReverseFind(_T('\n'));
	ASSERT(idLastLineBreak > 0);
	m_wndURLDownloadDicts.SetURL(DictURL.Mid(idLastLineBreak + 1));
	m_wndURLDownloadDicts.SizeToContent(true,true);

	FindDictionaries();
	CComboBox *pLangBox = (CComboBox*)GetDlgItem(IDC_OPTIONS_LANGUAGE);

	// Add all the found languages
	for (int i = 0; i < m_aLanguage.GetSize(); ++i)
	{
		if (pLangBox->FindString(0,m_aLanguage[i]) == CB_ERR)
			pLangBox->AddString(m_aLanguage[i]);
	}

	// Select the language
	int nSel = pLangBox->FindStringExact(0,m_strLanguageDefault);

	if (nSel == CB_ERR)
		pLangBox->SetCurSel(0);
	else
		pLangBox->SetCurSel(nSel);

	OnSelchangeOptionsLanguage();
	UpdateControlStates();

	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void COptionPageLanguage::FindDictionaries()
{
	m_aLanguage.RemoveAll();
	m_aDialect.RemoveAll();
	AfxFindDictionaries(m_aLanguage,m_aDialect);
}

void COptionPageLanguage::OnOptionsSpellBrowse()
{
	CWnd *pPersonalDictionary = GetDlgItem(IDC_OPTIONS_SPELL_PDICT);
	CString defaultPath;
	pPersonalDictionary->GetWindowText(defaultPath);

	CFileDialog dlg(true,NULL,defaultPath,OFN_PATHMUSTEXIST,
	                AfxLoadString(STE_FILE_DICFILTER));

	if (!defaultPath.IsEmpty())
	{
		LPTSTR pathStr = defaultPath.GetBuffer(0);
		::PathRemoveFileSpec(pathStr);
		dlg.m_ofn.lpstrInitialDir = pathStr;
		defaultPath.ReleaseBuffer();
	}

	if (dlg.DoModal() == IDOK)
		pPersonalDictionary->SetWindowText(dlg.GetPathName());
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
