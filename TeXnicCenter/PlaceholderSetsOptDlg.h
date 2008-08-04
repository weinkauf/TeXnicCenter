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

#if !defined(AFX_PLACEHOLDERSETSOPTDLG_H__334DF790_8DD2_11D6_AF57_00201855324E__INCLUDED_)
#define AFX_PLACEHOLDERSETSOPTDLG_H__334DF790_8DD2_11D6_AF57_00201855324E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Placeholder.h"

/**
This dialog lets the user define all possible placeholders for sets of files
together with their options (forward slash, 8.3-Format, quoting, relative Path).

<b>Usage:</b>

First call SetAllowOptions() to enable/disable Options-Support.
Then call SetShowOneLineExample() to configure the type of example shown.

Then call DoModal().

After the user finished the dialog with the Ok-Button,
you will find the defined placeholder in strPlaceholder.
If the dialog was canceled, strPlaceholder is empty.

@ingroup dialogs

@author Tino Weinkauf
 */
class CPlaceholderSetsOptDlg : public CDialog
{
// Construction
public:
	CPlaceholderSetsOptDlg(CWnd* pParent = NULL); // standard constructor

	// Dialog Data
	//{{AFX_DATA(CPlaceholderSetsOptDlg)

	enum
	{
		IDD = IDD_PLACEHOLDER_SET
	};
	CButton m_ShortBtn;
	CStatic m_ResultStatic;
	CButton m_RelativeBtn;
	CButton m_QuotedBtn;
	CComboBox m_PHTypeCombo;
	CButton m_ForwardBtn;
	CEdit m_ExampleEdit;
	BOOL m_bForwardSlash;
	BOOL m_bQuoted;
	BOOL m_bRelativePath;
	BOOL m_bShortFormat;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CPlaceholderSetsOptDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPlaceholderSetsOptDlg)
	virtual BOOL OnInitDialog();
	afx_msg void ShowExample();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Implementation
public:
	/** Enables/Disables Support of Options.
	Call this before calling DoModal().
	 */
	void SetAllowOptions(bool bEnable = true);

	/**
	To configure the type of example shown.
	Call this before calling DoModal().
	 */
	void SetShowOneLineExample(bool bShowOneLineExample = true);

protected:
	///Enables/Disables controls
	void UpdateControls();
	void ConstructPlaceholder();

//Attributes
public:
	/**Use this to retrieve the Placeholder String with all options
	defined by the user - after DoModal().
	 */
	CString strPlaceholder;

private:
	///true, if Options (forward slash, 8.3-Format, quoting, relative Path) are allowed to be configured
	bool m_bAllowOptions;

	///true, if Example shall be shown in one line
	bool m_bShowOneLineExample;

	///Helps constructing an example
	CPlaceholderSets m_ExamplePHSets;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLACEHOLDERSETSOPTDLG_H__334DF790_8DD2_11D6_AF57_00201855324E__INCLUDED_)
