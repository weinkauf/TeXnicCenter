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

#if !defined(AFX_ACTIVEPROFILEDIALOG_H__C38490A0_3225_11D3_929E_B53A62B086A5__INCLUDED_)
#define AFX_ACTIVEPROFILEDIALOG_H__C38490A0_3225_11D3_929E_B53A62B086A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**	Allows to switch between different output profiles.

@ingroup dialogs
@ingroup profiles

@author Sven Wiegand
 */
class CActiveProfileDialog : public CDialog
{
// construction/destruction
public:
	CActiveProfileDialog(CWnd* pParent = NULL);

// overridings
protected:
//{{AFX_VIRTUAL(CActiveProfileDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV-Unterst�tzung
	//}}AFX_VIRTUAL

// message handlers
protected:
	//{{AFX_MSG(CActiveProfileDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeProfile();
	virtual void OnOK();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

// dialog field data
protected:
	//{{AFX_DATA(CActiveProfileDialog)
	enum
	{
		IDD = IDD_ACTIVE_PROFILE
	};
	CButton m_wndOkButton;
	CListBox m_wndProfileList;
	//}}AFX_DATA
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f�gt unmittelbar vor der vorhergehenden Zeile zus�tzliche Deklarationen ein.

#endif // AFX_ACTIVEPROFILEDIALOG_H__C38490A0_3225_11D3_929E_B53A62B086A5__INCLUDED_