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
#include "InsertHeaderDialog.h"
#include "global.h"
//#include "../Speller/Character.h"

#include <utility>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace {
#pragma push_macro("DEFINE_SECTION")
#define DEFINE_SECTION(name) std::make_pair(STE_HEADER_##name, STE_HEADERPREFIX_##name)

	const std::pair<int, int> SectionIDs[] = { 
		DEFINE_SECTION(PART),
		DEFINE_SECTION(CHAPTER),
		DEFINE_SECTION(SECTION),
		DEFINE_SECTION(SUBSECTION),
		DEFINE_SECTION(SUBSUBSECTION),
		DEFINE_SECTION(PARAGRAPH),
		DEFINE_SECTION(SUBPARAGRAPH)
	};
#pragma pop_macro("DEFINE_SECTION")
}

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CInsertHeaderDialog

CInsertHeaderDialog::CInsertHeaderDialog(CWnd* pParent /*=NULL*/)
		: CDialog(CInsertHeaderDialog::IDD,pParent)
{
	//{{AFX_DATA_INIT(CInsertHeaderDialog)
	m_strLabel = _T("sec:");
	//m_strTitle = _T("");
	m_nDepth = 2;
	//}}AFX_DATA_INIT
}

BOOL CInsertHeaderDialog::SetProperties(const CString &strProperties)
{
	const int count = sizeof(SectionIDs) / sizeof(*SectionIDs);

	for (int i = 0; i < count; i++)
	{
		CString strCommand = AfxLoadString(SectionIDs[i].first) + _T('{');
		int nPos;

		if ((nPos = strProperties.Find(strCommand)) == -1)
			continue;

		// set depth
		m_nDepth = i;

		// set title
		m_strTitle.Empty();

		for (nPos += strCommand.GetLength(); nPos < strProperties.GetLength(); nPos++)
		{
			if (strProperties[nPos] == _T('}'))
				break;
			else
				m_strTitle += strProperties[nPos];
		}

		// set label
		m_strLabel = AfxLoadString(SectionIDs[m_nDepth].second);
		strCommand = _T("\\label{");
		if ((nPos = strProperties.Find(strCommand)) == -1)
			return TRUE;

		m_strLabel.Empty();
		for (nPos += strCommand.GetLength(); nPos < strProperties.GetLength(); nPos++)
		{
			if (strProperties[nPos] == _T('}'))
				break;
			else
				m_strLabel += strProperties[nPos];
		}

		return TRUE;
	}

	return FALSE;
}

CString CInsertHeaderDialog::GetProperties()
{
	CString strResult = _T('\r') + AfxLoadString(SectionIDs[m_nDepth].first) + _T('{');

	if (!m_strTitle.IsEmpty())
	{
		strResult += m_strTitle + _T("}\r");
		if (!m_strLabel.IsEmpty())
			strResult += _T("\\label{") + m_strLabel + _T("}\r");
	}
	else
		strResult += _T("\001}\r");

	return strResult;
}

void CInsertHeaderDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInsertHeaderDialog)
	DDX_Control(pDX,IDC_LABEL,m_wndLabelEdit);
	DDX_Text(pDX,IDC_LABEL,m_strLabel);
	DDX_Text(pDX,IDC_TITLE,m_strTitle);
	DDX_CBIndex(pDX,IDC_DEPTH,m_nDepth);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInsertHeaderDialog,CDialog)
	//{{AFX_MSG_MAP(CInsertHeaderDialog)
	ON_EN_CHANGE(IDC_TITLE,OnChangeTitle)
	ON_CBN_SELCHANGE(IDC_DEPTH,OnSelchangeDepth)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CInsertHeaderDialog

BOOL CInsertHeaderDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// enable/disable window
	m_wndLabelEdit.EnableWindow(!m_strTitle.IsEmpty());

	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void CInsertHeaderDialog::OnChangeTitle()
{
	UpdateData();

	// disable label-window?
	m_wndLabelEdit.EnableWindow(!m_strTitle.IsEmpty());

	// auto generate label
	BOOL bWhiteSpace = FALSE;
	TCHAR strC[2] = _T(" ");

	m_strLabel = AfxLoadString(SectionIDs[m_nDepth].second);

	for (int i = 0; i < m_strTitle.GetLength(); i++)
	{
		if (!_istalnum(m_strTitle[i]))
			bWhiteSpace = TRUE;
		else
		{
			strC[0] = m_strTitle[i];
			m_strLabel += (bWhiteSpace ? _tcsupr(strC) : strC);
			bWhiteSpace = FALSE;
		}
	}

	UpdateData(FALSE);
}

void CInsertHeaderDialog::OnSelchangeDepth()
{
	// auto generate label
	OnChangeTitle();
}
