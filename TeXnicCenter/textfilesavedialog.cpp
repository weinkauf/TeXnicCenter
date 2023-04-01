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

#include <algorithm>
#include <functional>

#include "resource.h"
#include "global.h"
#include "TextFileSaveDialog.h"

LPCTSTR const Format[] = {_T("Windows"),_T("Unix"),_T("Macintosh"),0};
LPCTSTR const Encoding[] = {_T("ANSI"),_T("UTF-8"),_T("UTF-16"),_T("UTF-16 Big Endian"),_T("UTF-32"),_T("UTF-32 Big Endian"),0};

BEGIN_MESSAGE_MAP(CTextFileSaveDialog, CFileDialog)
	ON_CBN_SELCHANGE(IDC_FILE_FORMAT, &CTextFileSaveDialog::OnSelchangeFileFormat)
	ON_CBN_SELCHANGE(IDC_SELECT_ENCODING, &CTextFileSaveDialog::OnSelchangeFileFormat)
END_MESSAGE_MAP()

CTextFileSaveDialog::CTextFileSaveDialog(
    UINT unTitle /*= AFX_IDS_SAVEFILE*/,
    LPCTSTR lpszDefExt /*= NULL*/, LPCTSTR lpszFileName /*= NULL*/,
    DWORD dwFlags /*= OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOREADONLYRETURN*/, int nFileFormat /*= CRLF_STYLE_DOS*/,
    LPCTSTR lpszFilter /*= NULL*/,
    CWnd* pParent /*= NULL*/)

: CFileDialogEx(FALSE, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParent)
, m_strTitle((LPCTSTR)unTitle)
, encoding_index_(0)
, use_bom_(false)
{
	// Vista-style
	IFileDialogCustomize* fdc = NULL;
	if (m_bVistaStyle) fdc = GetIFileDialogCustomize();
	if (fdc != NULL)
	{
#pragma region File Format initialization
		// Add the File Format combo box
		fdc->StartVisualGroup(IDC_STATIC_FORMATTITLE,CStringW(MAKEINTRESOURCE(IDS_FORMAT)));
		fdc->AddComboBox(IDC_FILE_FORMAT);

		USES_CONVERSION;
		DWORD id = 0;

		for (LPCTSTR const* p = Format; *p; ++p)
		{
			fdc->AddControlItem(IDC_FILE_FORMAT,id++,T2CW(*p));
		}

		fdc->EndVisualGroup();

		fdc->SetSelectedControlItem(IDC_FILE_FORMAT,static_cast<DWORD>(nFileFormat));
#pragma endregion

#pragma region Encoding initialization
		id = 0;

		// Add the Encoding combo box
		fdc->StartVisualGroup(IDC_STATIC_ENCODING,CStringW(MAKEINTRESOURCE(IDS_ENCODING)));
		fdc->AddComboBox(IDC_SELECT_ENCODING);

		const std::vector<CString> encodings = GetEncodings();
		typedef std::vector<CString>::const_iterator I;

		for (I it = encodings.begin(); it != encodings.end(); ++it)
		{
			fdc->AddControlItem(IDC_SELECT_ENCODING,id++,T2CW(*it));
		}

		fdc->EndVisualGroup();

		fdc->SetSelectedControlItem(IDC_SELECT_ENCODING,static_cast<DWORD>(encoding_index_));
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

const std::vector<CString> CTextFileSaveDialog::GetEncodings() const
{
	std::size_t size = sizeof(Encoding) / sizeof(*Encoding) - 1;
	std::vector<CString> result(Encoding,Encoding + size);

	CString fmt;
	fmt.Format(IDS_ENCODING_WITH_BOM,Encoding[1]);

	// Insert after UTF-8 which is the 2nd item 
	result.insert(result.begin() + 2,fmt);

	return result;
}

void CTextFileSaveDialog::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	if (!m_bVistaStyle)
	{
		DDX_Control(pDX, IDC_STATIC_FORMATTITLE, m_wndFileFormatTitle);
		DDX_Control(pDX, IDC_FILE_FORMAT, m_wndFileFormatCombo);
		DDX_Control(pDX, IDC_SELECT_ENCODING, encoding_);
		DDX_CBIndex(pDX, IDC_FILE_FORMAT, m_nFileFormat);
		DDX_CBIndex(pDX, IDC_SELECT_ENCODING, encoding_index_);
	}
}

void CTextFileSaveDialog::AlignedResizeBelow(CWnd* wndabove, CWnd* wndlabelabove, CWnd* w, CWnd* label)
{
	CRect rect;
	wndabove->GetWindowRect(&rect);
	ScreenToClient(&rect);

	CRect recttitle;
	wndlabelabove->GetWindowRect(&recttitle);
	ScreenToClient(&recttitle);

	CRect rc;
	w->GetWindowRect(&rc);
	ScreenToClient(&rc);

	CRect rclabel;
	label->GetWindowRect(&rclabel);
	ScreenToClient(&rclabel);

	rc.left = rect.left - 1;
	rc.right = rect.right;

	w->MoveWindow(&rc,FALSE);

	rclabel.left = recttitle.left;
	label->SetWindowPos(0,rclabel.left,rclabel.top,0,0,SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER);
}

BOOL CTextFileSaveDialog::OnInitDialog()
{
	__super::OnInitDialog();

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Modify position, size and tab order of the additional controls

	AlignedResizeBelow(GetParent()->GetDlgItem(0x00000470),GetParent()->GetDlgItem(0x00000441),
		&m_wndFileFormatCombo,&m_wndFileFormatTitle);
	AlignedResizeBelow(&m_wndFileFormatCombo,&m_wndFileFormatTitle,&encoding_,
		GetDlgItem(IDC_STATIC_ENCODING));

	// Add format data
	for (LPCTSTR const* p = Format; *p; ++p)
		m_wndFileFormatCombo.AddString(*p);

	// Add encoding data
	const std::vector<CString> encodings = GetEncodings();
	for (auto e : encodings) encoding_.AddString(e);

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

		if (SUCCEEDED(fdc->GetSelectedControlItem(IDC_FILE_FORMAT,&id)))
			result = static_cast<int>(id);

		fdc->Release();
	}
	else
		result = m_nFileFormat;

	return result;
}

void CTextFileSaveDialog::SetEncoding( CodeDocument::Encoding e )
{
	encoding_index_ = e;
	ASSERT(use_bom_ && e != CodeDocument::ANSI || !use_bom_ && 
		(e == CodeDocument::ANSI || e == CodeDocument::UTF8));

	if (use_bom_)
		++encoding_index_;

	if (m_bVistaStyle) 
	{
		IFileDialogCustomize* fdc = GetIFileDialogCustomize();
		fdc->SetSelectedControlItem(IDC_SELECT_ENCODING,static_cast<DWORD>(encoding_index_));
		fdc->Release();
	}
}

CodeDocument::Encoding CTextFileSaveDialog::GetEncoding()
{
	int index = GetEncodingIndex();

	if (index >= 2)
		--index;

	return static_cast<CodeDocument::Encoding>(index);
}

bool CTextFileSaveDialog::GetUseBOM()
{
	return GetEncodingIndex() >= 2;
}

void CTextFileSaveDialog::SetUseBOM( bool use /*= true*/ )
{
	bool previous = use_bom_;
	use_bom_ = use;

	if (use_bom_ != previous) {
		if (use_bom_) {
			ASSERT(encoding_index_ == 0 || encoding_index_ == 1);
			++encoding_index_;
		}
		else {
			ASSERT(encoding_index_ >= 2);
			--encoding_index_;
		}
	}
}

int CTextFileSaveDialog::GetEncodingIndex()
{
	int index = 0;

	if (IFileDialogCustomize* fdc = GetIFileDialogCustomize())
	{
		DWORD id;

		if (SUCCEEDED(fdc->GetSelectedControlItem(IDC_SELECT_ENCODING,&id)))
			index = static_cast<int>(id);

		fdc->Release();
	}
	else
		index = static_cast<CodeDocument::Encoding>(encoding_index_);
	
	return index;
}