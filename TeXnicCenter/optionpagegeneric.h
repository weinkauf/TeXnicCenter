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

#if !defined(AFX_OPTIONPAGEGENERIC_H__B6F7CFC1_4515_11D3_929E_B4D2F36FA64C__INCLUDED_)
#define AFX_OPTIONPAGEGENERIC_H__B6F7CFC1_4515_11D3_929E_B4D2F36FA64C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**	General Options.

@ingroup dialogs

@author Sven Wiegand
 */
class COptionPageGeneric :
			public CMFCPropertyPage
{
	DECLARE_DYNCREATE(COptionPageGeneric)

//types
protected:
	///Defines a set of quotation marks for a certain language
	struct CQMSet
	{

		CQMSet()
				: NameID(STE_LANGUAGE_ENGLISH)
				, OpeningQM(_T("``"))
				, ClosingQM(_T("''"))
		{
		}

		CQMSet(const UINT nStringID, LPCTSTR argOpeningQM, LPCTSTR argClosingQM)
				: NameID(nStringID),
				OpeningQM(argOpeningQM),
				ClosingQM(argClosingQM)
		{
		}


		UINT NameID;
		CString OpeningQM;
		CString ClosingQM;
	};

	typedef CArray< CQMSet, const CQMSet& > CQMSetARray;

// construction/destruction
public:
	COptionPageGeneric();
	~COptionPageGeneric();

	// operations

// implementation helpers
protected:
	/**
	Enables/disables controls of this page.
	 */
	void UpdateCtrlStates();

	/**
	Fills the language list with the languages available on this
	system.
	 */
	void RefillLanguageList();

	/**
	Fills the list of predefined quotations mark replacement sets.
	 */
	void RefillPredefinedQMSets();

// overwritings
public:
//{{AFX_VIRTUAL(COptionPageGeneric)
public:
	virtual void OnOK();
protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// message handlers
protected:
	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(COptionPageGeneric)
	virtual BOOL OnInitDialog();
	afx_msg void OnUpdateCtrls();
	afx_msg void OnSelchangePredefinedQMSets();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

// attributes
public:
	//{{AFX_DATA(COptionPageGeneric)
	enum
	{
		IDD = IDD_OPTIONS_GENERIC
	};
	CStatic m_QMSetLabel;
	CComboBox m_wndQMSetsCombo;
	//CComboBox m_wndLookAndFeelList;
	CComboBox m_wndGuiLanguageList;
	CEdit m_wndOpeningQm;
	CEdit m_wndClosingQm;
	CStatic m_wndQmLabel2;
	CStatic m_wndQmLabel1;
	BOOL m_bReplaceQm;
	CString m_strClosingQm;
	CString m_strOpeningQm;
	BOOL m_bRestoreSession;
	BOOL m_bOptimizeGuiForVisuallyHandicappedUsers;
	//}}AFX_DATA
	CString m_strLookAndFeel;
	CString m_strGuiLanguage;
	CQMSetARray QMSets;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_OPTIONPAGEGENERIC_H__B6F7CFC1_4515_11D3_929E_B4D2F36FA64C__INCLUDED_
