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
#include "PlaceholderSetsOptDlg.h"
#include "TeXnicCenter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlaceholderSetsOptDlg dialog

CPlaceholderSetsOptDlg::CPlaceholderSetsOptDlg(CWnd* pParent /*=NULL*/)
		: CDialog(CPlaceholderSetsOptDlg::IDD,pParent)
{
	//{{AFX_DATA_INIT(CPlaceholderSetsOptDlg)
	m_bForwardSlash = FALSE;
	m_bQuoted = TRUE;
	m_bRelativePath = FALSE;
	m_bShortFormat = FALSE;
	//}}AFX_DATA_INIT

	m_bAllowOptions = true;
	m_bShowOneLineExample = true;
	m_ExamplePHSets.SetProject(((CTeXnicCenterApp*)AfxGetApp())->GetProject());
}

void CPlaceholderSetsOptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlaceholderSetsOptDlg)
	DDX_Control(pDX,IDC_PLACEHOLDER_SET_SHORT,m_ShortBtn);
	DDX_Control(pDX,IDC_PLACEHOLDER_SET_RESULT,m_ResultStatic);
	DDX_Control(pDX,IDC_PLACEHOLDER_SET_RELATIVE,m_RelativeBtn);
	DDX_Control(pDX,IDC_PLACEHOLDER_SET_QUOTED,m_QuotedBtn);
	DDX_Control(pDX,IDC_PLACEHOLDER_SET_PHTYPE,m_PHTypeCombo);
	DDX_Control(pDX,IDC_PLACEHOLDER_SET_FORWARD,m_ForwardBtn);
	DDX_Control(pDX,IDC_PLACEHOLDER_SET_EXAMPLE,m_ExampleEdit);
	DDX_Check(pDX,IDC_PLACEHOLDER_SET_FORWARD,m_bForwardSlash);
	DDX_Check(pDX,IDC_PLACEHOLDER_SET_QUOTED,m_bQuoted);
	DDX_Check(pDX,IDC_PLACEHOLDER_SET_RELATIVE,m_bRelativePath);
	DDX_Check(pDX,IDC_PLACEHOLDER_SET_SHORT,m_bShortFormat);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPlaceholderSetsOptDlg,CDialog)
	//{{AFX_MSG_MAP(CPlaceholderSetsOptDlg)
	ON_BN_CLICKED(IDC_PLACEHOLDER_SET_FORWARD,ShowExample)
	ON_BN_CLICKED(IDC_PLACEHOLDER_SET_QUOTED,ShowExample)
	ON_BN_CLICKED(IDC_PLACEHOLDER_SET_RELATIVE,ShowExample)
	ON_BN_CLICKED(IDC_PLACEHOLDER_SET_SHORT,ShowExample)
	ON_CBN_SELCHANGE(IDC_PLACEHOLDER_SET_PHTYPE,ShowExample)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlaceholderSetsOptDlg message handlers



void CPlaceholderSetsOptDlg::SetAllowOptions(bool bEnable /*= true*/)
{
	m_bAllowOptions = bEnable;
}

void CPlaceholderSetsOptDlg::SetShowOneLineExample(bool bShowOneLineExample /*= true*/)
{
	m_bShowOneLineExample = bShowOneLineExample;
}

void CPlaceholderSetsOptDlg::UpdateControls()
{
	m_ForwardBtn.EnableWindow(m_bAllowOptions);
	m_ShortBtn.EnableWindow(m_bAllowOptions);
	m_RelativeBtn.EnableWindow(m_bAllowOptions);
	m_QuotedBtn.EnableWindow(m_bAllowOptions);
}

BOOL CPlaceholderSetsOptDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	strPlaceholder.Empty();

	//Activate the first entry
	m_PHTypeCombo.SetCurSel(0);

	//Update Options
	UpdateControls();

	//Init the example
	ShowExample();

	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPlaceholderSetsOptDlg::ConstructPlaceholder()
{
	UpdateData(true);

	//Get type of reference

	LPCTSTR const strPHTypes[] =
	{
		TXC_PLACEHOLDERSETNAME_TEXFILES,
		TXC_PLACEHOLDERSETNAME_BIBTEXFILES,
		TXC_PLACEHOLDERSETNAME_GRAPHICFILES,
		TXC_PLACEHOLDERSETNAME_TXCFILES,
		TXC_PLACEHOLDERSETNAME_CURRENTLYOPENEDFILES,
		TXC_PLACEHOLDERSETNAME_ALLPROJECTFILES,
		TXC_PLACEHOLDERSETNAME_ALLFILESETS
	};

	int nPHType = m_PHTypeCombo.GetCurSel();

	if ((nPHType < 0) || (nPHType > 6))
	{
		strPlaceholder.Empty();
		return;
	}

	//Construct
	strPlaceholder.Format(_T("$%s%s%s%s%s"),
	                      (m_bAllowOptions && m_bQuoted) ? _T("q") : _T(""),
	                      (m_bAllowOptions && m_bShortFormat) ? _T("s") : _T(""),
	                      (m_bAllowOptions && m_bForwardSlash) ? _T("f") : _T(""),
	                      (m_bAllowOptions && m_bRelativePath) ? _T("r") : _T(""),
	                      strPHTypes[nPHType]
	                     );
}

void CPlaceholderSetsOptDlg::ShowExample()
{
	//Get the placeholder
	ConstructPlaceholder();
	//Show it to the user
	m_ResultStatic.SetWindowText(strPlaceholder);

	/////////////////////////////////
	//Construct a real world example
	//NOTE: Works only, if a project is opened and/or files are opened in editor
	if (m_bShowOneLineExample)
	{
		m_ExampleEdit.SetWindowText(m_ExamplePHSets.ExpandAllSets(strPlaceholder));
	}
	else
	{
		CUniqueStringList strList;
		m_ExamplePHSets.ExpandAllSets(strPlaceholder,&strList);
		m_ExampleEdit.SetWindowText(strList.FlattenList(
		                                (m_bAllowOptions && m_bQuoted) ? _T("\")") : NULL,
		                                (m_bAllowOptions && m_bQuoted) ? _T("\")") : NULL,
		                                _T("\r\n"),
		                                true));
	}

	//Is something shown as an example?
	if (m_ExampleEdit.GetWindowTextLength() == 0)
	{
		//No example could be given; lets inform the user
		CString strInfo;
		strInfo.LoadString(STE_PHSDLG_NOEXAMPLE);
		m_ExampleEdit.SetWindowText(strInfo);
	}
}
