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
#include "OptionPageGeneric.h"
#include "configuration.h"
#include "CharType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Eigenschaftenseite COptionPageGeneric

IMPLEMENT_DYNCREATE(COptionPageGeneric,CMFCPropertyPage)

COptionPageGeneric::COptionPageGeneric() : CMFCPropertyPage(COptionPageGeneric::IDD)
{
	//{{AFX_DATA_INIT(COptionPageGeneric)
	m_bReplaceQm = CConfiguration::GetInstance()->m_bReplaceQuotationMarks;
	m_strClosingQm = CConfiguration::GetInstance()->m_strClosingQuotationMark;
	m_strOpeningQm = CConfiguration::GetInstance()->m_strOpeningQuotationMark;
	m_bRestoreSession = CConfiguration::GetInstance()->m_bLoadLastProject;
	m_bOptimizeGuiForVisuallyHandicappedUsers = CConfiguration::GetInstance()->m_bOptimizeMenuForVisuallyHandicappedUsersOnNextStart;
	m_strGuiLanguage = CConfiguration::GetInstance()->m_strGuiLanguageOnNextStart;
	//}}AFX_DATA_INIT

	//Init the QMSets
	QMSets.RemoveAll();
	// - English
	QMSets.Add(CQMSet(STE_LANGUAGE_ENGLISH,_T("``"),_T("''")));
	// - German
	QMSets.Add(CQMSet(STE_LANGUAGE_GERMAN,_T("\"`"),_T("\"'")));
	// - French
	QMSets.Add(CQMSet(STE_LANGUAGE_FRENCH,_T("\"<"),_T("\">")));
}

COptionPageGeneric::~COptionPageGeneric()
{
}

void COptionPageGeneric::UpdateCtrlStates()
{
	// enable/disable controls

	bool bCustomQM = false;
	if (m_bReplaceQm && (m_wndQMSetsCombo.GetCurSel() != CB_ERR))
	{
		bCustomQM = (m_wndQMSetsCombo.GetCurSel() == m_wndQMSetsCombo.GetCount() - 1);
	}

	m_QMSetLabel.EnableWindow(m_bReplaceQm);
	m_wndQMSetsCombo.EnableWindow(m_bReplaceQm);
	m_wndOpeningQm.EnableWindow(m_bReplaceQm && bCustomQM);
	m_wndClosingQm.EnableWindow(m_bReplaceQm && bCustomQM);
	m_wndQmLabel1.EnableWindow(m_bReplaceQm && bCustomQM);
	m_wndQmLabel2.EnableWindow(m_bReplaceQm && bCustomQM);
}

void COptionPageGeneric::RefillLanguageList()
{
	m_wndGuiLanguageList.ResetContent();

	// add build in language 'English'
	const Win32Locale eng(MAKELCID(MAKELANGID(LANG_ENGLISH,SUBLANG_DEFAULT),SORT_DEFAULT));

	//m_wndGuiLanguageList.AddString(eng.GetSNativeLangName());
	m_wndGuiLanguageList.AddString(_T("English"));

	// parse language directory to determine additional languages
	CFileFind ff;

	if (ff.FindFile(CPathTool::Cat(theApp.GetWorkingDir(),_T("language\\TxcRes*.dll"))))
	{
		const size_t n = _tcslen(_T("TxcRes"));
		CString lang;

		while (ff.FindNextFile())
		{
			//HMODULE module = ::LoadLibrary(ff.GetFileName());
			//CFileVersionInfo fv(module);
			//lang = Win32Locale(MAKELCID(fv.GetLanguageId(),SORT_DEFAULT)).GetSNativeLangName();

			lang = ff.GetFileTitle();
			lang = lang.Right(lang.GetLength() - n);

			if (m_wndGuiLanguageList.FindStringExact(0,lang) == CB_ERR)
				m_wndGuiLanguageList.AddString(lang);
		}
	}
}

void COptionPageGeneric::RefillPredefinedQMSets()
{
	m_wndQMSetsCombo.ResetContent();

	//Add the sets
	CString strLanguage;
	for (int i = 0; i < QMSets.GetSize(); i++)
	{
		const CQMSet& QMSet = QMSets.ElementAt(i);
		strLanguage.LoadString(QMSet.NameID);
		m_wndQMSetsCombo.AddString(strLanguage);
	}

	//Add custom possibility
	strLanguage.LoadString(STE_CUSTOM);
	m_wndQMSetsCombo.AddString(strLanguage);
}

void COptionPageGeneric::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionPageGeneric)
	DDX_Control(pDX,IDC_OPTIONS_QM_SET_LABEL,m_QMSetLabel);
	DDX_Control(pDX,IDC_OPTIONS_REPLACE_QM_PREDEFINED_COMBO,m_wndQMSetsCombo);
	//DDX_Control(pDX,IDC_LOOKNFEEL_COMBO,m_wndLookAndFeelList);
	DDX_Control(pDX,IDC_GUI_LANGUAGE_COMBO,m_wndGuiLanguageList);
	DDX_Control(pDX,IDC_OPTIONS_REPLACE_QM_OPEN_EDIT,m_wndOpeningQm);
	DDX_Control(pDX,IDC_OPTIONS_REPLACE_QM_CLOSE_EDIT,m_wndClosingQm);
	DDX_Control(pDX,IDC_OPTIONS_QM_LABEL2,m_wndQmLabel2);
	DDX_Control(pDX,IDC_OPTIONS_QM_LABEL1,m_wndQmLabel1);
	DDX_Check(pDX,IDC_OPTIONS_REPLACE_QM,m_bReplaceQm);
	DDX_Text(pDX,IDC_OPTIONS_REPLACE_QM_CLOSE_EDIT,m_strClosingQm);
	DDX_Text(pDX,IDC_OPTIONS_REPLACE_QM_OPEN_EDIT,m_strOpeningQm);
	DDX_Check(pDX,IDC_OPTIONS_RESTORE_SESSION,m_bRestoreSession);
	DDX_Check(pDX,IDC_OPTIONS_OPTIMIZE_FOR_VISUALLY_HANDICAPPED_USERS,m_bOptimizeGuiForVisuallyHandicappedUsers);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate)
	{
		// GUI language
		int nSelection = m_wndGuiLanguageList.GetCurSel();

		if (nSelection == CB_ERR)
			nSelection = m_wndGuiLanguageList.SetCurSel(0);

		m_wndGuiLanguageList.GetLBText(nSelection,m_strGuiLanguage);
	}
	else
	{
		// GUI language
		int nMatch = m_wndGuiLanguageList.FindStringExact(0,m_strGuiLanguage);

		if (nMatch == CB_ERR)
			nMatch = 0;

		m_wndGuiLanguageList.SetCurSel(nMatch);
	}
}

BEGIN_MESSAGE_MAP(COptionPageGeneric,CMFCPropertyPage)
	//{{AFX_MSG_MAP(COptionPageGeneric)
	ON_BN_CLICKED(IDC_OPTIONS_REPLACE_QM,OnUpdateCtrls)
	ON_CBN_SELCHANGE(IDC_OPTIONS_REPLACE_QM_PREDEFINED_COMBO,OnSelchangePredefinedQMSets)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL COptionPageGeneric::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();

	RefillLanguageList();
	RefillPredefinedQMSets();
	UpdateData(FALSE);

	int i = 0;

	//Find the QM Setting
	for (; i < QMSets.GetSize(); i++)
	{
		const CQMSet& QMSet = QMSets.ElementAt(i);

		if ((QMSet.OpeningQM == m_strOpeningQm) && (QMSet.ClosingQM == m_strClosingQm))
			break;
	}

	m_wndQMSetsCombo.SetCurSel(i);

	UpdateCtrlStates();

	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void COptionPageGeneric::OnOK()
{
	UpdateData();

	// Store settings to configuration
	CConfiguration::GetInstance()->m_bReplaceQuotationMarks = m_bReplaceQm;
	CConfiguration::GetInstance()->m_strClosingQuotationMark = m_strClosingQm;
	CConfiguration::GetInstance()->m_strOpeningQuotationMark = m_strOpeningQm;
	CConfiguration::GetInstance()->m_bLoadLastProject = m_bRestoreSession;

	// generate message, that tells the user, that his settings will be
	// activated on next start of TXC if those settings have changed.
	bool bShowNextStartInfo = (CConfiguration::GetInstance()->m_bOptimizeMenuForVisuallyHandicappedUsers != m_bOptimizeGuiForVisuallyHandicappedUsers);
	bShowNextStartInfo |= (CConfiguration::GetInstance()->m_strGuiLanguage != m_strGuiLanguage);

	//bool update_look = CConfiguration::GetInstance()->m_strLookAndFeel != m_strLookAndFeel;

	if (bShowNextStartInfo) AfxMessageBox(STE_OPTIONS_REQUIRES_RESTART,MB_ICONINFORMATION | MB_OK);

	CConfiguration::GetInstance()->m_bOptimizeMenuForVisuallyHandicappedUsersOnNextStart = m_bOptimizeGuiForVisuallyHandicappedUsers;
	CConfiguration::GetInstance()->m_strGuiLanguageOnNextStart = m_strGuiLanguage;

	CMFCPropertyPage::OnOK();
}

void COptionPageGeneric::OnUpdateCtrls()
{
	UpdateData();
	UpdateCtrlStates();
}

void COptionPageGeneric::OnSelchangePredefinedQMSets()
{
	if (m_wndQMSetsCombo.GetCurSel() != CB_ERR)
	{
		int idx = m_wndQMSetsCombo.GetCurSel();
		ASSERT((idx >= 0) && (idx <= QMSets.GetSize()));
		if ((idx >= 0) && (idx < QMSets.GetSize()))
		{
			const CQMSet& QMSet = QMSets.ElementAt(idx);
			m_wndOpeningQm.SetWindowText(QMSet.OpeningQM);
			m_wndClosingQm.SetWindowText(QMSet.ClosingQM);
		}

		UpdateCtrlStates();
	}
}
