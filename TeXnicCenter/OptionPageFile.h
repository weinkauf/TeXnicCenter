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

#if !defined(AFX_OPTIONPAGEFILE_H__74DCE580_1EB3_11D4_A221_006097239934__INCLUDED_)
#define AFX_OPTIONPAGEFILE_H__74DCE580_1EB3_11D4_A221_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**	Options for File handling.

@ingroup dialogs

@author Sven Wiegand
 */
class COptionPageFile : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionPageFile)

// construction/destruction
public:
	COptionPageFile();
	virtual ~COptionPageFile();

// operations
public:

// implementation helpers
protected:
	/**
	Enables/disables controls of this page.
	 */
	void UpdateCtrlStates();

// overridings
protected:
//{{AFX_VIRTUAL(COptionPageFile)
public:
	virtual void OnOK();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// message handlers
protected:
	//{{AFX_MSG(COptionPageFile)
	virtual BOOL OnInitDialog();
	afx_msg void OnUpdateCtrls();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()


// dialog fields
protected:
	//{{AFX_DATA(COptionPageFile)
	enum
	{
		IDD = IDD_OPTIONS_FILE
	};
	CStatic m_wndSaveIntervalLabel2;
	CStatic m_wndSaveIntervalLabel1;
	CEdit m_wndSaveIntervalEdit;
	CSpinButtonCtrl m_wndSaveIntervalSpin;
	BOOL m_bSaveAutomatic;
	BOOL m_bSaveBeforeCompilation;
	UINT m_unSaveInterval;
	int m_nFileFormat;
	BOOL m_bSaveNewDocuments;
	//}}AFX_DATA
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_OPTIONPAGEFILE_H__74DCE580_1EB3_11D4_A221_006097239934__INCLUDED_
