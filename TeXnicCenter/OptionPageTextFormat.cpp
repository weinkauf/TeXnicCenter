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
	ON_CBN_SELCHANGE(IDC_EDITOR_SCHEME,OnCbnSelchangeEditorSchemeElement)
	ON_BN_CLICKED(IDC_EDITOR_EDITSCHEMECOLOR,OnBnClickedEditorEditSchemeColor)
	ON_BN_CLICKED(IDC_EDITOR_SELECTFONT,OnEditorSelectfont)
	ON_BN_CLICKED(IDC_EDITOR_ELEMENTCOLOR,OnElementColor)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_WINDOW,OnSelchangeWindow)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_INSERTCURSOR_BLINK, &COptionPageTextFormat::UpdateDataAndControlStates)
END_MESSAGE_MAP()


IMPLEMENT_DYNCREATE(COptionPageTextFormat,CMFCPropertyPage)


COptionPageTextFormat::COptionPageTextFormat()
		: CMFCPropertyPage(COptionPageTextFormat::IDD),
		m_bApplyChanges(FALSE)
{
	// copy color array
	for (int i = 0; i < LaTeXView::COLORINDEX_COUNT; i++)
	{
		m_aColors[i] = CConfiguration::GetInstance()->m_aEditorColors[i];
	}

	m_nEditorElement = 0;
	m_nEditorColorScheme = CConfiguration::GetInstance()->m_nEditorColorScheme;

	// initialize
	m_font.CreateFontIndirect(&CConfiguration::GetInstance()->m_fontEditor);

	//{{AFX_DATA_INIT(COptionPageTextFormat)
	//}}AFX_DATA_INIT

	// preselect a window type
	m_nWindowElement = wndEditor;

	//Get current caret settings
	m_nInsertCaretStyle = CConfiguration::GetInstance()->m_nInsertCaretStyle - 1;
	m_nInsertCaretBlinkPeriod = CConfiguration::GetInstance()->m_nInsertCaretBlinkPeriod;
	m_bInsertCaretBlink = (m_nInsertCaretBlinkPeriod > 0);
	// some nice UI stuff
	if (!m_bInsertCaretBlink)
	{
		m_nInsertCaretBlinkPeriod = 500;
	}
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
	m_wndEditorSchemeElement.EnableWindow(m_nWindowElement == wndEditor);
	m_wndEditorSchemeColorEdit.EnableWindow(m_nWindowElement == wndEditor && m_nEditorColorScheme != LaTeXViewBase::COLORSCHEME_CUSTOM);
	m_wndEditorElement.EnableWindow(m_nWindowElement == wndEditor && m_nEditorColorScheme == LaTeXViewBase::COLORSCHEME_CUSTOM);
	m_wndElementColorPicker.EnableWindow(m_nWindowElement == wndEditor && m_nEditorColorScheme == LaTeXViewBase::COLORSCHEME_CUSTOM);

	m_wndInsertCaretStyleLine.EnableWindow(m_nWindowElement == wndEditor);
	m_wndInsertCaretStyleBlock.EnableWindow(m_nWindowElement == wndEditor);
	m_btnInsertCaretBlink.EnableWindow(m_nWindowElement == wndEditor);
	m_wndInsertCaretBlinkPeriod.EnableWindow(m_nWindowElement == wndEditor && m_bInsertCaretBlink);
}


void COptionPageTextFormat::UpdateDataAndControlStates()
{
	UpdateData();
	UpdateControlStates();
}


void COptionPageTextFormat::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_EDITOR_FONT,m_wndFontExample);
	DDX_Control(pDX,IDC_EDITOR_ELEMENT,m_wndEditorElement);
	DDX_Control(pDX,IDC_EDITOR_ELEMENTCOLOR,m_wndElementColorPicker);
	DDX_Control(pDX, IDC_EDITOR_SCHEME, m_wndEditorSchemeElement);
	DDX_Control(pDX, IDC_EDITOR_EDITSCHEMECOLOR, m_wndEditorSchemeColorEdit);
	DDX_CBIndex(pDX,IDC_EDITOR_ELEMENT,m_nEditorElement);
	DDX_CBIndex(pDX,IDC_EDITOR_SCHEME,m_nEditorColorScheme);
	DDX_CBIndex(pDX,IDC_WINDOW,m_nWindowElement);
	DDX_Text(pDX, IDC_INSERTCURSOR_BLINK_SPEED, m_nInsertCaretBlinkPeriod);
	DDV_MinMaxInt(pDX, m_nInsertCaretBlinkPeriod, 10, 2000);
	DDX_Check(pDX, IDC_INSERTCURSOR_BLINK, m_bInsertCaretBlink);
	DDX_Radio(pDX, IDC_INSERTCURSOR_STYLE_LINE, m_nInsertCaretStyle);
	DDX_Control(pDX, IDC_INSERTCURSOR_BLINK, m_btnInsertCaretBlink);
	DDX_Control(pDX, IDC_INSERTCURSOR_BLINK_SPEED, m_wndInsertCaretBlinkPeriod);
	DDX_Control(pDX, IDC_INSERTCURSOR_STYLE_LINE, m_wndInsertCaretStyleLine);
	DDX_Control(pDX, IDC_INSERTCURSOR_STYLE_BLOCK, m_wndInsertCaretStyleBlock);
}

BOOL COptionPageTextFormat::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// set item data for element combo
	// - Use this to sort the entries of the combobox. The rest of the code takes care.
	m_wndEditorElement.SetItemData( 1, LaTeXViewBase::COLORINDEX_BKGND);
	m_wndEditorElement.SetItemData( 2, LaTeXViewBase::COLORINDEX_NORMALTEXT);
	m_wndEditorElement.SetItemData( 3, LaTeXViewBase::COLORINDEX_CARET);
	m_wndEditorElement.SetItemData( 4, LaTeXViewBase::COLORINDEX_CARETLINE);
	m_wndEditorElement.SetItemData( 5, LaTeXViewBase::COLORINDEX_SELBKGND);
	m_wndEditorElement.SetItemData( 6, LaTeXViewBase::COLORINDEX_SELTEXT);
	m_wndEditorElement.SetItemData( 7, LaTeXViewBase::COLORINDEX_KEYWORD);
	m_wndEditorElement.SetItemData( 8, LaTeXViewBase::COLORINDEX_COMMENT);
	m_wndEditorElement.SetItemData( 9, LaTeXViewBase::COLORINDEX_SPECIAL);
	m_wndEditorElement.SetItemData(10, LaTeXViewBase::COLORINDEX_SYMBOLS);
	m_wndEditorElement.SetItemData(11, LaTeXViewBase::COLORINDEX_GROUP);
	m_wndEditorElement.SetItemData(12, LaTeXViewBase::COLORINDEX_GROUP_NAME);
	m_wndEditorElement.SetItemData(13, LaTeXViewBase::COLORINDEX_STYLE_INCLUSION);
	m_wndEditorElement.SetItemData(14, LaTeXViewBase::COLORINDEX_FILE_INCLUSION);
	m_wndEditorElement.SetItemData(15, LaTeXViewBase::COLORINDEX_INLINE_MATH_TEXT);
	m_wndEditorElement.SetItemData(16, LaTeXViewBase::COLORINDEX_INLINE_MATH_COMMAND);
	m_wndEditorElement.SetItemData(17, LaTeXViewBase::COLORINDEX_DIGIT);
	m_wndEditorElement.SetItemData(18, LaTeXViewBase::COLORINDEX_UNIT);
	m_wndEditorElement.SetItemData(19, LaTeXViewBase::COLORINDEX_PAIRSTRINGBKGND);
	m_wndEditorElement.SetItemData(20, LaTeXViewBase::COLORINDEX_PAIRSTRINGTEXT);
	m_wndEditorElement.SetItemData(21, LaTeXViewBase::COLORINDEX_BADPAIRSTRINGBKGND);
	m_wndEditorElement.SetItemData(22, LaTeXViewBase::COLORINDEX_BADPAIRSTRINGTEXT);
	m_wndEditorElement.SetItemData(23, LaTeXViewBase::COLORINDEX_FOLDMARGIN);
	m_wndEditorElement.SetItemData(24, LaTeXViewBase::COLORINDEX_FOLDMARGIN_HIGHLIGHT);
	m_wndEditorElement.SetItemData(25, LaTeXViewBase::COLORINDEX_FOLDMARK_FORE);
	m_wndEditorElement.SetItemData(26, LaTeXViewBase::COLORINDEX_FOLDMARK_BACK);
	m_wndEditorElement.SetItemData(27, LaTeXViewBase::COLORINDEX_ERRORMARK_FORE);
	m_wndEditorElement.SetItemData(28, LaTeXViewBase::COLORINDEX_ERRORMARK_BACK);
	m_wndEditorElement.SetItemData(29, LaTeXViewBase::COLORINDEX_BOOKMARK_FORE);
	m_wndEditorElement.SetItemData(30, LaTeXViewBase::COLORINDEX_BOOKMARK_BACK);
	m_wndEditorElement.SetItemData(31, LaTeXViewBase::COLORINDEX_LINENUMBERS_FORE);
	m_wndEditorElement.SetItemData(32, LaTeXViewBase::COLORINDEX_LINENUMBERS_BACK);
	m_wndEditorElement.SetItemData(33, LaTeXViewBase::COLORINDEX_INDICATOR_SPELLING);

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

	m_wndEditorElement.EnableWindow(m_nEditorColorScheme == LaTeXViewBase::COLORSCHEME_CUSTOM);
	m_wndElementColorPicker.EnableWindow(m_nEditorColorScheme == LaTeXViewBase::COLORSCHEME_CUSTOM);

	m_wndEditorSchemeColorEdit.EnableWindow(m_nEditorColorScheme != LaTeXViewBase::COLORSCHEME_CUSTOM);

	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
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
	{
		CConfiguration::GetInstance()->m_aEditorColors[i] = m_aColors[i];
	}

	//Copy Color scheme
	CConfiguration::GetInstance()->m_nEditorColorScheme = m_nEditorColorScheme;

	//Copy caret settings
	CConfiguration::GetInstance()->m_nInsertCaretStyle = m_nInsertCaretStyle + 1;
	CConfiguration::GetInstance()->m_nInsertCaretBlinkPeriod = (m_bInsertCaretBlink ? m_nInsertCaretBlinkPeriod : 0);

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

void COptionPageTextFormat::OnCbnSelchangeEditorSchemeElement()
{
	UpdateData();
	UpdateControlStates();
	//Set the right color in the color picker
	OnSelchangeEditorElement();
}

void COptionPageTextFormat::OnBnClickedEditorEditSchemeColor()
{
	if (AfxMessageBox(IDS_CUSTOMIZE_COLORSCHEME, MB_YESNO | MB_ICONQUESTION, 0) == IDNO)
	{
		return;
	}

	//Copy current scheme to the custom scheme, then switch to custom mode
	UpdateData();

	for (int i = 0; i < LaTeXView::COLORINDEX_COUNT; i++)
	{
		m_aColors[i] = LaTeXViewBase::GetAutomaticColor(i, m_nEditorColorScheme);
	}

	m_wndEditorSchemeElement.SetCurSel(LaTeXViewBase::COLORSCHEME_CUSTOM);
	OnCbnSelchangeEditorSchemeElement();
}

