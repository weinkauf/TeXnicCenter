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

#if !defined(AFX_CUSTOMIZEPAGEEDITOR_H__4CDEE221_243C_11D4_A221_006097239934__INCLUDED_)
#define AFX_CUSTOMIZEPAGEEDITOR_H__4CDEE221_243C_11D4_A221_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LaTeXView.h"

/**	Options for the editor window.

@ingroup dialogs

@author Sven Wiegand
 */
class COptionPageTextFormat : public CMFCPropertyPage
{
	DECLARE_DYNCREATE(COptionPageTextFormat)

// construction/destruction
public:
	COptionPageTextFormat();
	virtual ~COptionPageTextFormat();

// constants
public:

	enum tagWindowType
	{
		wndEditor = 0,
		wndNavigator,
		wndOutput
	};

// Implementation helpers
protected:
	/** Updates the control, that displays the font example */
	void UpdateFontExample();

	/** Updates the dialog, based on the selected window type */
	void UseWindowTypeSettings();

	/**
	Stores the settings of the selected window type based to the
	configuration.
	 */
	void StoreWindowTypeSettings();

	/**
	Updates the states of the dialog's controls based on the current
	settings.
	 */
	void UpdateControlStates();

// Overrides
protected:
//{{AFX_VIRTUAL(COptionPageTextFormat)
public:
	virtual void OnOK();
protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV-Unterst�tzung
	//}}AFX_VIRTUAL

// message handlers
protected:
	//{{AFX_MSG(COptionPageTextFormat)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeEditorElement();
	afx_msg void OnEditorSelectfont();
	afx_msg void OnElementColor();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeWindow();
	afx_msg void OnCbnSelchangeEditorSchemeElement();
	afx_msg void OnBnClickedEditorEditSchemeColor();
	afx_msg void UpdateDataAndControlStates();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

// dialog data
protected:
	//{{AFX_DATA(COptionPageTextFormat)
	enum
	{
		IDD = IDD_OPTIONS_TEXTFORMAT
	};
	CStatic m_wndFontExample;
	CComboBox m_wndEditorElement;
	CComboBox m_wndEditorSchemeElement;
	CMFCColorButton m_wndElementColorPicker;
	CButton m_wndEditorSchemeColorEdit;
	int m_nEditorElement;
	int m_nWindowElement;
	int m_nInsertCaretStyle;
	BOOL m_bInsertCaretBlink;
	int m_nInsertCaretBlinkPeriod;
	CButton m_wndInsertCaretStyleLine;
	CButton m_wndInsertCaretStyleBlock;
	CButton m_btnInsertCaretBlink;
	CEdit m_wndInsertCaretBlinkPeriod;
	//}}AFX_DATA

// Attributes
protected:
	/** array for storing color assignment */
	COLORREF m_aColors[LaTeXView::COLORINDEX_COUNT];

	/** color scheme for editor components */
	int m_nEditorColorScheme;

	/** font definitions for editor */
	LOGFONT m_logfont;

	/** font to display in the example */
	CFont m_font;

	/** TRUE if the OnOK function has been called. */
	BOOL m_bApplyChanges;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f�gt unmittelbar vor der vorhergehenden Zeile zus�tzliche Deklarationen ein.

#endif // AFX_CUSTOMIZEPAGEEDITOR_H__4CDEE221_243C_11D4_A221_006097239934__INCLUDED_
