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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Eigenschaftenseite COptionPageGeneric 

IMPLEMENT_DYNCREATE(COptionPageGeneric, CPropertyPage)

COptionPageGeneric::COptionPageGeneric() : CPropertyPage(COptionPageGeneric::IDD)
{
	//{{AFX_DATA_INIT(COptionPageGeneric)
	m_bReplaceQm = g_configuration.m_bReplaceQuotationMarks;
	m_strClosingQm = g_configuration.m_strClosingQuotationMark;
	m_strOpeningQm = g_configuration.m_strOpeningQuotationMark;
	m_bRestoreSession = g_configuration.m_bLoadLastProject;
	m_bOptimizeGuiForVisuallyHandicappedUsers = g_configuration.m_bOptimizeMenuForVisuallyHandicappedUsersOnNextStart;
	m_strGuiLanguage = g_configuration.m_strGuiLanguageOnNextStart;
	m_strLookAndFeel = g_configuration.m_strLookAndFeelOnNextStart;
	//}}AFX_DATA_INIT

	//Init the QMSets
	QMSets.RemoveAll();
	// - English
	QMSets.Add( CQMSet(STE_LANGUAGE_ENGLISH, "``", "''") );
	// - German
	QMSets.Add( CQMSet(STE_LANGUAGE_GERMAN, "\"`", "\"'") );
	// - French
	QMSets.Add( CQMSet(STE_LANGUAGE_FRENCH, "\"<", "\">") );
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
	m_wndGuiLanguageList.AddString(_T("English"));

	// parse language directory to determine additional languages
	CFileFind	ff;
	BOOL			bMoreFiles = ff.FindFile(CPathTool::Cat(theApp.GetWorkingDir(), _T("language\\TxcRes*.dll")));
	while (bMoreFiles)
	{
		bMoreFiles = ff.FindNextFile();

		CString	strLanguage = ff.GetFileTitle();
		strLanguage = strLanguage.Right(strLanguage.GetLength() - _tcslen(_T("TxcRes")));

		if (m_wndGuiLanguageList.FindStringExact(0, strLanguage)==CB_ERR)
			m_wndGuiLanguageList.AddString(strLanguage);
	}
}

void COptionPageGeneric::RefillPredefinedQMSets()
{
	m_wndQMSetsCombo.ResetContent();

	//Add the sets
	CString	strLanguage;
	for(int i=0;i<QMSets.GetSize();i++)
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
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionPageGeneric)
	DDX_Control(pDX, IDC_OPTIONS_QM_SET_LABEL, m_QMSetLabel);
	DDX_Control(pDX, IDC_OPTIONS_REPLACE_QM_PREDEFINED_COMBO, m_wndQMSetsCombo);
	DDX_Control(pDX, IDC_LOOKNFEEL_COMBO, m_wndLookAndFeelList);
	DDX_Control(pDX, IDC_GUI_LANGUAGE_COMBO, m_wndGuiLanguageList);
	DDX_Control(pDX, IDC_OPTIONS_REPLACE_QM_OPEN_EDIT, m_wndOpeningQm);
	DDX_Control(pDX, IDC_OPTIONS_REPLACE_QM_CLOSE_EDIT, m_wndClosingQm);
	DDX_Control(pDX, IDC_OPTIONS_QM_LABEL2, m_wndQmLabel2);
	DDX_Control(pDX, IDC_OPTIONS_QM_LABEL1, m_wndQmLabel1);
	DDX_Check(pDX, IDC_OPTIONS_REPLACE_QM, m_bReplaceQm);
	DDX_Text(pDX, IDC_OPTIONS_REPLACE_QM_CLOSE_EDIT, m_strClosingQm);
	DDX_Text(pDX, IDC_OPTIONS_REPLACE_QM_OPEN_EDIT, m_strOpeningQm);
	DDX_Check(pDX, IDC_OPTIONS_RESTORE_SESSION, m_bRestoreSession);
	DDX_Check(pDX, IDC_OPTIONS_OPTIMIZE_FOR_VISUALLY_HANDICAPPED_USERS, m_bOptimizeGuiForVisuallyHandicappedUsers);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate)
	{
		// Read selected information into variables
		int	nSelection = -1;

		// Look and feel
		nSelection = m_wndLookAndFeelList.GetCurSel();
		if (nSelection==CB_ERR)
			nSelection = m_wndLookAndFeelList.SetCurSel(0);
		m_wndLookAndFeelList.GetLBText(nSelection, m_strLookAndFeel);

		// GUI language
		nSelection = m_wndGuiLanguageList.GetCurSel();
		if (nSelection==CB_ERR)
			nSelection = m_wndGuiLanguageList.SetCurSel(0);
		m_wndGuiLanguageList.GetLBText(nSelection, m_strGuiLanguage);
	}
	else
	{
		// update liste selection based on variables
		int	nMatch = -1;
		
		// Look and feel
		nMatch = m_wndLookAndFeelList.FindStringExact(0, m_strLookAndFeel);
		if (nMatch==CB_ERR)
			nMatch = 0;
		m_wndLookAndFeelList.SetCurSel(nMatch);
		
		// GUI language
		nMatch = m_wndGuiLanguageList.FindStringExact(0, m_strGuiLanguage);
		if (nMatch==CB_ERR)
			nMatch = 0;
		m_wndGuiLanguageList.SetCurSel(nMatch);
	}
}


BEGIN_MESSAGE_MAP(COptionPageGeneric, CPropertyPage)
	//{{AFX_MSG_MAP(COptionPageGeneric)
	ON_BN_CLICKED(IDC_OPTIONS_REPLACE_QM, OnUpdateCtrls)
	ON_CBN_SELCHANGE(IDC_OPTIONS_REPLACE_QM_PREDEFINED_COMBO, OnSelchangePredefinedQMSets)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL COptionPageGeneric::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	RefillLanguageList();
	RefillPredefinedQMSets();
	UpdateData(FALSE);

	//Find the QM Setting
	int i(0);
	for(i=0;i<QMSets.GetSize();i++)
	{
		const CQMSet& QMSet = QMSets.ElementAt(i);
		if ( (QMSet.OpeningQM == m_strOpeningQm) && (QMSet.ClosingQM == m_strClosingQm) )
			break;
	}
	m_wndQMSetsCombo.SetCurSel(i);

	UpdateCtrlStates();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void COptionPageGeneric::OnOK() 
{
	UpdateData();

	// Store settings to configuration
	g_configuration.m_bReplaceQuotationMarks = m_bReplaceQm;
	g_configuration.m_strClosingQuotationMark = m_strClosingQm;
	g_configuration.m_strOpeningQuotationMark = m_strOpeningQm;
	g_configuration.m_bLoadLastProject = m_bRestoreSession;

	// generate message, that tells the user, that his settings will be
	// activated on next start of TXC if those settings have changed.
	BOOL	bShowNextStartInfo = FALSE;
	bShowNextStartInfo = (bool)g_configuration.m_bOptimizeMenuForVisuallyHandicappedUsers != (bool)m_bOptimizeGuiForVisuallyHandicappedUsers;
	if (!bShowNextStartInfo)
		bShowNextStartInfo = g_configuration.m_strGuiLanguage != m_strGuiLanguage;
	if (!bShowNextStartInfo)
		bShowNextStartInfo = g_configuration.m_strLookAndFeel != m_strLookAndFeel;

	if (bShowNextStartInfo)
		AfxMessageBox(STE_OPTIONS_REQUIRES_RESTART, MB_ICONINFORMATION|MB_OK);

	g_configuration.m_bOptimizeMenuForVisuallyHandicappedUsersOnNextStart = m_bOptimizeGuiForVisuallyHandicappedUsers;
	g_configuration.m_strGuiLanguageOnNextStart = m_strGuiLanguage;
	g_configuration.m_strLookAndFeelOnNextStart = m_strLookAndFeel;
	
	CPropertyPage::OnOK();
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
