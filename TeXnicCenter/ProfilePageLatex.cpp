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
#include "ProfilePageLatex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// class CProfilePageLatex 
//-------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CProfilePageLatex, CProfilePage)
	//{{AFX_MSG_MAP(CProfilePageLatex)
	ON_BN_CLICKED(IDC_USELATEX, OnUpdateControlStates)
	ON_BN_CLICKED(IDC_DONTUSEBIBTEX, OnUpdateControlStates)
	ON_BN_CLICKED(IDC_DONTUSEMAKEINDEX, OnUpdateControlStates)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CProfilePageLatex::CProfilePageLatex()
: CProfilePage(CProfilePageLatex::IDD),

	m_wndBrowseLatex(IDC_EDIT_LATEXPATH, TRUE, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, CString((LPCTSTR)STE_COMPILER_FILTER)),
	m_wndLatexPh(IDR_POPUP_PLACEHOLDER),

	m_wndBrowseBibTex(IDC_EDIT_BIBTEXPATH, TRUE, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, CString((LPCTSTR)STE_COMPILER_FILTER)),
	m_wndBibTexPh(IDR_POPUP_PLACEHOLDER),

	m_wndBrowseMakeIndex(IDC_EDIT_MAKEINDEXPATH, TRUE, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, CString((LPCTSTR)STE_COMPILER_FILTER)),
	m_wndMakeIndexPh(IDR_POPUP_PLACEHOLDER),

	m_pProfile(NULL)
{
	//{{AFX_DATA_INIT(CProfilePageLatex)
	m_bUseLatex = FALSE;
	m_strLatexPath = _T("");
	m_strLatexArguments = _T("");
	m_bDontUseBibTex = FALSE;
	m_bDontUseMakeIndex = FALSE;
	m_strBibTexArguments = _T("");
	m_strMakeIndexArguments = _T("");
	m_strBibTexPath = _T("");
	m_strMakeIndexPath = _T("");
	//}}AFX_DATA_INIT
}


void CProfilePageLatex::UpdateControlStates()
{
	if (m_pProfile)
		EnableAllControls(TRUE);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// (La)TeX
	if (m_bUseLatex)
	{
		m_wndTitleLatexPath.ShowWindow(SW_SHOW);
		m_wndLatexPath.ShowWindow(SW_SHOW);
		m_wndBrowseLatex.ShowWindow(SW_SHOW);
		m_wndTitleLatexArguments.ShowWindow(SW_SHOW);
		m_wndLatexArguments.ShowWindow(SW_SHOW);
		m_wndLatexPh.ShowWindow(SW_SHOW);
	}
	else
	{
		m_wndTitleLatexPath.ShowWindow(SW_HIDE);
		m_wndLatexPath.ShowWindow(SW_HIDE);
		m_wndBrowseLatex.ShowWindow(SW_HIDE);
		m_wndTitleLatexArguments.ShowWindow(SW_HIDE);
		m_wndLatexArguments.ShowWindow(SW_HIDE);
		m_wndLatexPh.ShowWindow(SW_HIDE);
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// BibTex
	if (!m_bDontUseBibTex)
	{
		m_wndTitleBibTexPath.ShowWindow(SW_SHOW);
		m_wndBibTexPath.ShowWindow(SW_SHOW);
		m_wndBrowseBibTex.ShowWindow(SW_SHOW);
		m_wndTitleBibTexArguments.ShowWindow(SW_SHOW);
		m_wndBibTexArguments.ShowWindow(SW_SHOW);
		m_wndBibTexPh.ShowWindow(SW_SHOW);
	}
	else
	{
		m_wndTitleBibTexPath.ShowWindow(SW_HIDE);
		m_wndBibTexPath.ShowWindow(SW_HIDE);
		m_wndBrowseBibTex.ShowWindow(SW_HIDE);
		m_wndTitleBibTexArguments.ShowWindow(SW_HIDE);
		m_wndBibTexArguments.ShowWindow(SW_HIDE);
		m_wndBibTexPh.ShowWindow(SW_HIDE);
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// MakeIndex
	if (!m_bDontUseMakeIndex)
	{
		m_wndTitleMakeIndexPath.ShowWindow(SW_SHOW);
		m_wndMakeIndexPath.ShowWindow(SW_SHOW);
		m_wndBrowseMakeIndex.ShowWindow(SW_SHOW);
		m_wndTitleMakeIndexArguments.ShowWindow(SW_SHOW);
		m_wndMakeIndexArguments.ShowWindow(SW_SHOW);
		m_wndMakeIndexPh.ShowWindow(SW_SHOW);
	}
	else
	{
		m_wndTitleMakeIndexPath.ShowWindow(SW_HIDE);
		m_wndMakeIndexPath.ShowWindow(SW_HIDE);
		m_wndBrowseMakeIndex.ShowWindow(SW_HIDE);
		m_wndTitleMakeIndexArguments.ShowWindow(SW_HIDE);
		m_wndMakeIndexArguments.ShowWindow(SW_HIDE);
		m_wndMakeIndexPh.ShowWindow(SW_HIDE);
	}
	
	if (!m_pProfile)
		EnableAllControls(FALSE);
}


void CProfilePageLatex::OnUpdateDataSet(CProfile *pProfile)
{
	m_pProfile = pProfile;

	if (m_pProfile)
	{
		// (La)TeX
		m_bUseLatex = m_pProfile->GetRunLatex();
		m_strLatexPath = m_pProfile->GetLatexPath();
		m_strLatexArguments = m_pProfile->GetLatexArguments();

		// BibTeX
		m_bDontUseBibTex = !m_pProfile->GetRunBibTex();
		m_strBibTexPath = m_pProfile->GetBibTexPath();
		m_strBibTexArguments = m_pProfile->GetBibTexArguments();

		// MakeIndex
		m_bDontUseMakeIndex = !m_pProfile->GetRunMakeIndex();
		m_strMakeIndexPath = m_pProfile->GetMakeIndexPath();
		m_strMakeIndexArguments = m_pProfile->GetMakeIndexArguments();
	}

	if (IsWindow(m_hWnd))
	{
		UpdateControlStates();
		UpdateData(FALSE);
	}
}


BOOL CProfilePageLatex::ApplyChanges()
{
	if (!m_pProfile)
		return TRUE;

	UpdateData();

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// (La)TeX
	if (m_bUseLatex && m_strLatexPath.IsEmpty())
	{
		m_wndLatexPath.SetFocus();
		return FALSE;
	}
	else if (m_bUseLatex && m_strLatexArguments.IsEmpty())
	{
		m_wndLatexArguments.SetFocus();
		return FALSE;
	}
	m_pProfile->SetLatexPath(m_strLatexPath, m_strLatexArguments, m_bUseLatex);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// BibTeX
	if (!m_bDontUseBibTex && m_strBibTexPath.IsEmpty())
	{
		m_wndBibTexPath.SetFocus();
		return FALSE;
	}
	else if (!m_bDontUseBibTex && m_strBibTexArguments.IsEmpty())
	{
		m_wndBibTexArguments.SetFocus();
		return FALSE;
	}
	m_pProfile->SetBibTexPath(m_strBibTexPath, m_strBibTexArguments, !m_bDontUseBibTex);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// MakeIndex
	if (!m_bDontUseMakeIndex && m_strMakeIndexPath.IsEmpty())
	{
		m_wndMakeIndexPath.SetFocus();
		return FALSE;
	}
	else if (!m_bDontUseMakeIndex && m_strMakeIndexArguments.IsEmpty())
	{
		m_wndMakeIndexArguments.SetFocus();
		return FALSE;
	}
	m_pProfile->SetMakeIndexPath(m_strMakeIndexPath, m_strMakeIndexArguments, !m_bDontUseMakeIndex);

	return TRUE;
}


void CProfilePageLatex::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProfilePageLatex)
	DDX_Control(pDX, IDC_TITLE_MAKEINDEXPATH, m_wndTitleMakeIndexPath);
	DDX_Control(pDX, IDC_TITLE_MAKEINDEXARGUMENTS, m_wndTitleMakeIndexArguments);
	DDX_Control(pDX, IDC_TITLE_BIBTEXPATH, m_wndTitleBibTexPath);
	DDX_Control(pDX, IDC_TITLE_BIBTEXARGUMENTS, m_wndTitleBibTexArguments);
	DDX_Control(pDX, IDC_EDIT_MAKEINDEXPATH, m_wndMakeIndexPath);
	DDX_Control(pDX, IDC_EDIT_BIBTEXPATH, m_wndBibTexPath);
	DDX_Control(pDX, IDC_EDIT_MAKEINDEXARGUMENTS, m_wndMakeIndexArguments);
	DDX_Control(pDX, IDC_EDIT_BIBTEXARGUMENTS, m_wndBibTexArguments);
	DDX_Control(pDX, IDC_MAKEINDEXPH, m_wndMakeIndexPh);
	DDX_Control(pDX, IDC_BIBTEXPH, m_wndBibTexPh);
	DDX_Control(pDX, IDC_BROWSEMAKEINDEX, m_wndBrowseMakeIndex);
	DDX_Control(pDX, IDC_BROWSEBIBTEX, m_wndBrowseBibTex);
	DDX_Control(pDX, IDC_LATEXPH, m_wndLatexPh);
	DDX_Control(pDX, IDC_EDIT_LATEXARGUMENTS, m_wndLatexArguments);
	DDX_Control(pDX, IDC_TITLE_LATEXARGUMENTS, m_wndTitleLatexArguments);
	DDX_Control(pDX, IDC_BROWSELATEX, m_wndBrowseLatex);
	DDX_Control(pDX, IDC_EDIT_LATEXPATH, m_wndLatexPath);
	DDX_Control(pDX, IDC_TITLE_LATEXPATH, m_wndTitleLatexPath);
	DDX_Check(pDX, IDC_USELATEX, m_bUseLatex);
	DDX_Text(pDX, IDC_EDIT_LATEXPATH, m_strLatexPath);
	DDX_Text(pDX, IDC_EDIT_LATEXARGUMENTS, m_strLatexArguments);
	DDX_Check(pDX, IDC_DONTUSEBIBTEX, m_bDontUseBibTex);
	DDX_Check(pDX, IDC_DONTUSEMAKEINDEX, m_bDontUseMakeIndex);
	DDX_Text(pDX, IDC_EDIT_BIBTEXARGUMENTS, m_strBibTexArguments);
	DDX_Text(pDX, IDC_EDIT_MAKEINDEXARGUMENTS, m_strMakeIndexArguments);
	DDX_Text(pDX, IDC_EDIT_BIBTEXPATH, m_strBibTexPath);
	DDX_Text(pDX, IDC_EDIT_MAKEINDEXPATH, m_strMakeIndexPath);
	//}}AFX_DATA_MAP
}


BOOL CProfilePageLatex::OnInitDialog() 
{
	CProfilePage::OnInitDialog();
	
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// initialize control
	m_wndLatexPh.AttachEditCtrl(&m_wndLatexArguments);
	m_wndBibTexPh.AttachEditCtrl(&m_wndBibTexArguments);
	m_wndMakeIndexPh.AttachEditCtrl(&m_wndMakeIndexArguments);

	UpdateControlStates();
	
	return TRUE;
}


void CProfilePageLatex::OnUpdateControlStates() 
{
	UpdateData();
	UpdateControlStates();
}
