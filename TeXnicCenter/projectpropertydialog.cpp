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

#include "stdafx.h"
#include "TeXnicCenter.h"
#include "ProjectPropertyDialog.h"
#include "global.h"
#include "Configuration.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// class CProjectPropertyDialog 
//-------------------------------------------------------------------

CProjectPropertyDialog::CProjectPropertyDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CProjectPropertyDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProjectPropertyDialog)
	m_strMainFile = _T("");
	m_bUseMakeIndex = FALSE;
	m_bUseBibTex = FALSE;
	m_strLanguageCurrent	= g_configuration.m_strLanguageDefault;
	m_strDialectCurrent		= g_configuration.m_strLanguageDialectDefault;
	//}}AFX_DATA_INIT
}


void CProjectPropertyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProjectPropertyDialog)
	DDX_Text(pDX, IDC_MAIN_FILE, m_strMainFile);
	DDX_Check(pDX, IDC_CHECK_MAKEINDEX, m_bUseMakeIndex);
	DDX_Check(pDX, IDC_CHECK_BIBTEX, m_bUseBibTex);
	DDX_CBString(pDX, IDC_OPTIONS_LANGUAGE, m_strLanguageCurrent);
	DDX_CBString(pDX, IDC_OPTIONS_LANGUAGE_DIALECT, m_strDialectCurrent);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProjectPropertyDialog, CDialog)
	//{{AFX_MSG_MAP(CProjectPropertyDialog)
	ON_BN_CLICKED(IDC_BROWSE_MAIN_FILE, OnBrowseMainFile)
	ON_CBN_SELCHANGE(IDC_OPTIONS_LANGUAGE, OnSelchangeOptionsLanguage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CProjectPropertyDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// make relative path out of main file's path
	m_strMainFile = CPathTool::GetRelativePath(m_strProjectDir, m_strMainFile);

	FindDictionaries();
	CComboBox *pLangBox = (CComboBox*)GetDlgItem(IDC_OPTIONS_LANGUAGE);

	// Add all the found languages
	int i;
	for (i = 0; i < m_aLanguage.GetSize(); ++i)
	{
		if (pLangBox->FindString( 0, m_aLanguage[i] ) == CB_ERR )
			pLangBox->AddString (m_aLanguage[i] );
	}

	// Select the language
	int nSel = pLangBox->FindStringExact( 0, m_strLanguageCurrent );
	if ( nSel == CB_ERR )
		pLangBox->SetCurSel( 0 );
	else
		pLangBox->SetCurSel( nSel );

	OnSelchangeOptionsLanguage();

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}


void CProjectPropertyDialog::OnBrowseMainFile() 
{
	UpdateData();

	SetCurrentDirectory(m_strProjectDir);

	CFileDialogEx	dlg( 
		TRUE, NULL, 
		m_strMainFile.IsEmpty()? NULL : (LPCTSTR)CPathTool::Cat(m_strProjectDir, m_strMainFile), 
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, 
		theApp.GetLatexFileFilter(), theApp.m_pMainWnd );

	if( dlg.DoModal() != IDOK )
		return;

	m_strMainFile = CPathTool::GetRelativePath(m_strProjectDir, dlg.GetPathName());
	UpdateData(FALSE);
}


void CProjectPropertyDialog::OnOK() 
{
	UpdateData();

	m_strMainFile = CPathTool::Cat(m_strProjectDir, m_strMainFile);
	m_strMainFile = CPathTool::GetBackslashPath(m_strMainFile);

	if (!CPathTool::Exists(m_strMainFile))
	{
		AfxMessageBox(STE_PROJECT_MAINFILENOTFOUND, MB_ICONSTOP|MB_OK);
		return;
	}

	UpdateData(FALSE);
	
	CDialog::OnOK();
}


void CProjectPropertyDialog::FindDictionaries()
{
	m_aLanguage.RemoveAll();
	m_aDialect.RemoveAll();
	AfxFindDictionnaries(m_aLanguage,m_aDialect);
}


void CProjectPropertyDialog::OnSelchangeOptionsLanguage() 
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
	int nSel = pDialBox->FindStringExact( 0, this->m_strDialectCurrent);
	if ( nSel == CB_ERR )
		pDialBox->SetCurSel( 0 );
	else
		pDialBox->SetCurSel( nSel );
	
}
