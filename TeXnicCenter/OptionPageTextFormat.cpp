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
#include "configuration.h"
#include "MainFrm.h"
#include "OptionPageTextFormat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// class COptionPageTextFormat
//-------------------------------------------------------------------

BEGIN_MESSAGE_MAP(COptionPageTextFormat,CMFCPropertyPage)
	//{{AFX_MSG_MAP(COptionPageTextFormat)
	ON_CBN_SELCHANGE(IDC_EDITOR_ELEMENT,OnSelchangeEditorElement)
	ON_BN_CLICKED(IDC_EDITOR_SELECTFONT,OnEditorSelectfont)
	ON_BN_CLICKED(IDC_EDITOR_ELEMENTCOLOR,OnElementColor)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_WINDOW,OnSelchangeWindow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


IMPLEMENT_DYNCREATE(COptionPageTextFormat,CMFCPropertyPage)


COptionPageTextFormat::COptionPageTextFormat()
		: CMFCPropertyPage(COptionPageTextFormat::IDD),
		m_bApplyChanges(FALSE)
{
	// copy color array
	for (int i = 0; i < LaTeXView::COLORINDEX_COUNT; i++)
		m_aColors[i] = CConfiguration::GetInstance()->m_aEditorColors[i];

	m_nEditorElement = 0;

	// initialize
	m_font.CreateFontIndirect(&CConfiguration::GetInstance()->m_fontEditor);

	//{{AFX_DATA_INIT(COptionPageTextFormat)
	//}}AFX_DATA_INIT

	// preselect a window type
	m_nWindowElement = wndEditor;

	// TODO: get current caret settings
	//m_nInsertCursorForm = LaTeXView::GetCaretInsertForm();
	//m_nInsertCursorMode = LaTeXView::GetCaretInsertMode();
	//m_nOverwriteCursorForm = LaTeXView::GetCaretOverwriteForm();
	//m_nOverwriteCursorMode = LaTeXView::GetCaretOverwriteMode();
}

COptionPageTextFormat::~COptionPageTextFormat()
{
	m_font.DeleteObject();
}

void COptionPageTextFormat::UpdateFontExample()
{
	m_font.DeleteObject();
	m_font.CreateFontIndirect(&m_logfont);
	m_wndFontExample.SetFont(&m_font);
	m_wndFontExample.SetWindowText(m_logfont.lfFaceName);
}

void COptionPageTextFormat::UseWindowTypeSettings()
{
	switch (m_nWindowElement)
	{
		case wndEditor:
			memcpy(&m_logfont,&CConfiguration::GetInstance()->m_fontEditor,sizeof(m_logfont));
			break;

		case wndOutput:
			memcpy(&m_logfont,&CConfiguration::GetInstance()->m_fontOutput,sizeof(m_logfont));
			break;

		case wndNavigator:
			memcpy(&m_logfont,&CConfiguration::GetInstance()->m_fontNavigator,sizeof(m_logfont));
			break;

		default:
			ASSERT(FALSE);
	}

	UpdateControlStates();
	UpdateFontExample();
}

void COptionPageTextFormat::StoreWindowTypeSettings()
{
	switch (m_nWindowElement)
	{
		case wndEditor:
			memcpy(&CConfiguration::GetInstance()->m_fontEditor,&m_logfont,sizeof(m_logfont));
			// TODO: Cursor mode
			//LaTeXView::SetCaretInsertStyle(m_nInsertCursorForm,m_nInsertCursorMode);
			//LaTeXView::SetCaretOverwriteStyle(m_nOverwriteCursorForm,m_nOverwriteCursorMode);
			break;

		case wndOutput:
			memcpy(&CConfiguration::GetInstance()->m_fontOutput,&m_logfont,sizeof(m_logfont));
			break;

		case wndNavigator:
			memcpy(&CConfiguration::GetInstance()->m_fontNavigator,&m_logfont,sizeof(m_logfont));
			break;

		default:
			ASSERT(FALSE);
	}
}

void COptionPageTextFormat::UpdateControlStates()
{
	m_wndEditorElement.EnableWindow(m_nWindowElement == wndEditor);
	m_wndElementColorPicker.EnableWindow(m_nWindowElement == wndEditor);

	m_wndInsertCursorForm.EnableWindow(m_nWindowElement == wndEditor);
	m_wndInsertCursorMode.EnableWindow(m_nWindowElement == wndEditor);
	m_wndOverwriteCursorForm.EnableWindow(m_nWindowElement == wndEditor);
	m_wndOverwriteCursorMode.EnableWindow(m_nWindowElement == wndEditor);
}

void COptionPageTextFormat::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionPageTextFormat)
	DDX_Control(pDX,IDC_INSERTCURSOR_BLINK,m_wndInsertCursorMode);
	DDX_Control(pDX,IDC_INSERCURSOR_STYLE,m_wndInsertCursorForm);
	DDX_Control(pDX,IDC_OVERWRITECURSOR_BLINK,m_wndOverwriteCursorMode);
	DDX_Control(pDX,IDC_OVERWRITECURSOR_STYLE,m_wndOverwriteCursorForm);
	DDX_Control(pDX,IDC_EDITOR_FONT,m_wndFontExample);
	DDX_Control(pDX,IDC_EDITOR_ELEMENT,m_wndEditorElement);
	DDX_Control(pDX,IDC_EDITOR_ELEMENTCOLOR,m_wndElementColorPicker);
	DDX_CBIndex(pDX,IDC_EDITOR_ELEMENT,m_nEditorElement);
	DDX_CBIndex(pDX,IDC_WINDOW,m_nWindowElement);
	DDX_CBIndex(pDX,IDC_INSERCURSOR_STYLE,m_nInsertCursorForm);
	DDX_CBIndex(pDX,IDC_INSERTCURSOR_BLINK,m_nInsertCursorMode);
	DDX_CBIndex(pDX,IDC_OVERWRITECURSOR_STYLE,m_nOverwriteCursorForm);
	DDX_CBIndex(pDX,IDC_OVERWRITECURSOR_BLINK,m_nOverwriteCursorMode);
	//}}AFX_DATA_MAP
}

BOOL COptionPageTextFormat::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// set item data for element combo
	// - Use this to sort the entries of the combobox. The rest of the code takes care.
	m_wndEditorElement.SetItemData(0,LaTeXView::COLORINDEX_SELMARGIN);
	m_wndEditorElement.SetItemData(1,LaTeXView::COLORINDEX_WHITESPACE);
	m_wndEditorElement.SetItemData(2,LaTeXView::COLORINDEX_BKGND);
	m_wndEditorElement.SetItemData(3,LaTeXView::COLORINDEX_NORMALTEXT);
	m_wndEditorElement.SetItemData(4,LaTeXView::COLORINDEX_SELBKGND);
	m_wndEditorElement.SetItemData(5,LaTeXView::COLORINDEX_SELTEXT);
	m_wndEditorElement.SetItemData(6,LaTeXView::COLORINDEX_KEYWORD);
	m_wndEditorElement.SetItemData(7,LaTeXView::COLORINDEX_COMMENT);
	m_wndEditorElement.SetItemData(8,LaTeXView::COLORINDEX_VERBATIM_TEXT);
	m_wndEditorElement.SetItemData(9,LaTeXView::COLORINDEX_INLINE_MATH_TEXT);
	m_wndEditorElement.SetItemData(10,LaTeXView::COLORINDEX_OPERATOR);
	m_wndEditorElement.SetItemData(11,LaTeXView::COLORINDEX_INLINE_MATH_COMMAND);
	m_wndEditorElement.SetItemData(12,LaTeXView::COLORINDEX_PAIRSTRINGBKGND);
	m_wndEditorElement.SetItemData(13,LaTeXView::COLORINDEX_PAIRSTRINGTEXT);
	m_wndEditorElement.SetItemData(14,LaTeXView::COLORINDEX_CURPAIRSTRINGBKGND);
	m_wndEditorElement.SetItemData(15,LaTeXView::COLORINDEX_CURPAIRSTRINGTEXT);
	m_wndEditorElement.SetItemData(16,LaTeXView::COLORINDEX_PAIRBLOCKBKGND);
	m_wndEditorElement.SetItemData(17,LaTeXView::COLORINDEX_BADPAIRSTRINGBKGND);
	m_wndEditorElement.SetItemData(18,LaTeXView::COLORINDEX_BADPAIRSTRINGTEXT);
	m_wndEditorElement.SetItemData(19,LaTeXView::COLORINDEX_GROUP_NAME);
	m_wndEditorElement.SetItemData(20,LaTeXView::COLORINDEX_DIGIT);
	m_wndEditorElement.SetItemData(21,LaTeXView::COLORINDEX_UNIT);
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// select color
	m_wndElementColorPicker.m_bEnabledInCustomizeMode = TRUE;

	CPalette palette;
	int colors, columns;

	CreateColorButtonPalette(palette,colors,columns);

	m_wndElementColorPicker.SetPalette(&palette);
	m_wndElementColorPicker.SetColumnsNumber(columns);

	m_wndElementColorPicker.EnableOtherButton(CString((LPCTSTR)STE_COLOR_OTHERS));
	OnSelchangeEditorElement();

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// use the right settings
	UseWindowTypeSettings();

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// use data
	UpdateData(FALSE);

	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zur�ckgeben
}

void COptionPageTextFormat::OnOK()
{
	UpdateData();

	// get font size
	// CString	strFontSize;
	// m_wndEditorFontSize.GetLBText( m_nEditorFontSize, strFontSize );
	// CConfiguration::GetInstance()->m_nFontSize = (int)atol( strFontSize );

	// store settings
	StoreWindowTypeSettings();

	// copy color array
	for (int i = 0; i < LaTeXView::COLORINDEX_COUNT; i++)
		CConfiguration::GetInstance()->m_aEditorColors[i] = m_aColors[i];

	// OnOK should update all windows
	m_bApplyChanges = TRUE;

	CMFCPropertyPage::OnOK();
}

void COptionPageTextFormat::OnDestroy()
{
	// this message will be send to all windows of the application
	if (m_bApplyChanges)
	{
		AfxGetMainWnd()->SendMessage(WM_SYSCOLORCHANGE);
		m_bApplyChanges = FALSE;
	}

	CMFCPropertyPage::OnDestroy();
}

void COptionPageTextFormat::OnSelchangeEditorElement()
{
	UpdateData();

	// select element color
	m_wndElementColorPicker.EnableAutomaticButton(
	    CString((LPCTSTR)STE_COLOR_AUTOMATIC),
	    LaTeXView::GetAutomaticColor(m_wndEditorElement.GetItemData(m_nEditorElement)));
	m_wndElementColorPicker.SetColor(m_aColors[m_wndEditorElement.GetItemData(m_nEditorElement)]);
}

void COptionPageTextFormat::OnElementColor()
{
	UpdateData();
	m_aColors[m_wndEditorElement.GetItemData(m_nEditorElement)] = m_wndElementColorPicker.GetColor();

}

void COptionPageTextFormat::OnEditorSelectfont()
{
	// select font for editor
	CFontDialog fontdlg;
	LOGFONT tmpFont = m_logfont;

	fontdlg.m_cf.lpLogFont = &tmpFont;
	fontdlg.m_cf.Flags |= CF_INITTOLOGFONTSTRUCT;
	fontdlg.m_cf.Flags &= ~CF_EFFECTS;

	if (fontdlg.DoModal() == IDOK)
		fontdlg.GetCurrentFont(&m_logfont);

	UpdateFontExample();
}

void COptionPageTextFormat::OnSelchangeWindow()
{
	StoreWindowTypeSettings();
	UpdateData();
	UseWindowTypeSettings();
	UpdateControlStates();
}

