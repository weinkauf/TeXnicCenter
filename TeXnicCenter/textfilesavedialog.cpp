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

#include "global.h"
#include "TextFileSaveDialog.h"
#include "RunTimeHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// class CTextFileSaveDialog
//-------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CTextFileSaveDialog, CFileDialog)
	//{{AFX_MSG_MAP(CTextFileSaveDialog)
	ON_CBN_SELCHANGE(IDC_SELECT_FILEFORMAT, OnSelchangeFileFormat)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

LPCTSTR const Format[] = {_T("Windows"),_T("Unix"),_T("Macintosh"),0};
LPCTSTR const Encoding[] = {_T("ANSI"),_T("Unicode"),_T("Unicode Big Endian"),_T("UTF-8"),0};

CTextFileSaveDialog::CTextFileSaveDialog(
    UINT unTitle /*= AFX_IDS_SAVEFILE*/,
    LPCTSTR lpszDefExt /*= NULL*/, LPCTSTR lpszFileName /*= NULL*/,
    DWORD dwFlags /*= OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOREADONLYRETURN*/, int nFileFormat /*= CRLF_STYLE_DOS*/,
    LPCTSTR lpszFilter /*= NULL*/,
    CWnd* pParent /*= NULL*/)

		: CFileDialogEx(FALSE, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParent)
		, m_strTitle((LPCTSTR)unTitle)
{
	// Vista-style
	if (IFileDialogCustomize* fdc = GetIFileDialogCustomize())
	{
#pragma region File Format initialization
		// Add the File Format combo box
		fdc->StartVisualGroup(IDC_STATIC_FORMATTITLE,CStringW(MAKEINTRESOURCE(IDS_FORMAT)));
		fdc->AddComboBox(IDC_SELECT_FILEFORMAT);

		USES_CONVERSION;
		DWORD id = 0;

		for (LPCTSTR const* p = Format; *p; ++p)
			fdc->AddControlItem(IDC_SELECT_FILEFORMAT,id++,T2CW(*p));

		fdc->EndVisualGroup();

		fdc->SetSelectedControlItem(IDC_SELECT_FILEFORMAT,static_cast<DWORD>(nFileFormat));
#pragma endregion

#pragma region Encoding initialization
		id = 0;

		// Add the Encoding combo box
		fdc->StartVisualGroup(IDC_STATIC_ENCODING,CStringW(MAKEINTRESOURCE(IDS_ENCODING)));
		fdc->AddComboBox(IDC_SELECT_ENCODING);		

		for (LPCTSTR const* p = Encoding; *p; ++p)
			fdc->AddControlItem(IDC_SELECT_ENCODING,id++,T2CW(*p));

		fdc->EndVisualGroup();

		fdc->SetSelectedControlItem(IDC_SELECT_ENCODING,static_cast<DWORD>(nFileFormat));
#pragma endregion		

		fdc->Release();
	}
	else
	{
		m_ofn.Flags |= OFN_ENABLETEMPLATE;
		m_ofn.lpTemplateName = MAKEINTRESOURCE(IDD_FILE_SAVE);
	}

	m_ofn.lpstrTitle = m_strTitle;

	m_nFileFormat = nFileFormat;
}

void CTextFileSaveDialog::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	if (!RunTimeHelper::IsVista())
	{
		DDX_Control(pDX, IDC_STATIC_FORMATTITLE, m_wndFileFormatTitle);
		DDX_Control(pDX, IDC_SELECT_FILEFORMAT, m_wndFileFormatCombo);
		DDX_CBIndex(pDX, IDC_SELECT_FILEFORMAT, m_nFileFormat);
	}
}

BOOL CTextFileSaveDialog::OnInitDialog()
{
	__super::OnInitDialog();

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Modify position, size and tab order of the additional controls

	// Get position and size of file type title (ID retrieved with Spy++)
	CRect rectFileTypeTitle;
	GetParent()->GetDlgItem(0x00000441)->GetWindowRect(rectFileTypeTitle);
	ScreenToClient(rectFileTypeTitle);

	// Get position and size of file type combo (ID retrieved with Spy++)
	CRect rectFileTypeCombo;
	GetParent()->GetDlgItem(0x00000470)->GetWindowRect(rectFileTypeCombo);
	ScreenToClient(rectFileTypeCombo);

	// Get position and size of file format combo
	CRect rectFileFormatCombo;
	m_wndFileFormatCombo.GetWindowRect(rectFileFormatCombo);
	ScreenToClient(rectFileFormatCombo);

	// have to specify -1 here, otherwise box will jump behind the right border -- don't ask me why...
	rectFileFormatCombo.left = rectFileTypeCombo.left - 1;
	rectFileFormatCombo.right = rectFileFormatCombo.left + rectFileTypeCombo.Width();

	// Get position and size of file format title
	CRect rectFileFormatTitle;
	m_wndFileFormatTitle.GetWindowRect(rectFileFormatTitle);
	ScreenToClient(rectFileFormatTitle);

	rectFileFormatTitle.left = rectFileTypeTitle.left;
	rectFileFormatTitle.right = rectFileFormatTitle.right + rectFileTypeTitle.Width();

	// adjust position
	m_wndFileFormatTitle.MoveWindow(rectFileFormatTitle);
	m_wndFileFormatCombo.MoveWindow(rectFileFormatCombo);

	// Add data
	for (LPCTSTR const* p = Format; *p; ++p)
		m_wndFileFormatCombo.AddString(*p);

	//
	UpdateData(FALSE);

	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void CTextFileSaveDialog::OnSelchangeFileFormat()
{
	UpdateData();
}

int CTextFileSaveDialog::GetFileFormat()
{
	int result = -1;

	if (IFileDialogCustomize* fdc = GetIFileDialogCustomize())
	{
		DWORD id;

		if (SUCCEEDED(fdc->GetSelectedControlItem(IDC_SELECT_FILEFORMAT,&id)))
			result = static_cast<int>(id);

		fdc->Release();
	}
	else
		result = m_nFileFormat;

	return result;
}