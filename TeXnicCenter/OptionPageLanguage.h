/********************************************************************
*
* This file is part of the TeXnicCenter-system
*
* Copyright (C) 2002 Chris Norris
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

#if !defined(AFX_OPTIONPAGELANGUAGE_H__2082FD53_4ABF_4A14_8B8B_6254FF4F040D__INCLUDED_)
#define AFX_OPTIONPAGELANGUAGE_H__2082FD53_4ABF_4A14_8B8B_6254FF4F040D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**	Options for spell checking.

@ingroup dialogs

@author Chris Norris
*/
class COptionPageLanguage : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionPageLanguage)

// Construction
public:
	COptionPageLanguage();

// Dialog Data
	//{{AFX_DATA(COptionPageLanguage)
	enum { IDD = IDD_OPTIONS_SPELLING };
	CComboBox	c_Locale;
	BOOL	m_bEnableSpell;
	BOOL	m_bMainDictOnly;
	BOOL	m_bSkipComments;
	BOOL	m_bSkipNumbers;
	BOOL	m_bSkipTags;
	BOOL	m_bSkipCaps;
	CString	m_strLanguageDefault;
	CString	m_strDialectDefault;
	CString	m_strPDictionary;
	CString	m_strLocale;
	CBCGURLLinkButton m_wndURLDownloadDicts;
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionPageLanguage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateControlStates();
	void FindDictionaries();
	CArray<CString, CString&> m_aLanguage;
	CArray<CString, CString&> m_aDialect;

	// Generated message map functions
	//{{AFX_MSG(COptionPageLanguage)
	afx_msg void OnSelchangeOptionsLanguage();
	virtual BOOL OnInitDialog();
	afx_msg void OnOptionsSpellBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONPAGELANGUAGE_H__2082FD53_4ABF_4A14_8B8B_6254FF4F040D__INCLUDED_)
