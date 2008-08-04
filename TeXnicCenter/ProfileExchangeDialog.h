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

#if !defined(AFX_PROFILEEXCHANGEDIALOG_H__21068D46_1554_4A62_B621_A86AE4A5D1BF__INCLUDED_)
#define AFX_PROFILEEXCHANGEDIALOG_H__21068D46_1554_4A62_B621_A86AE4A5D1BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Profile.h"

/**
Provides a dialog, that allows the user to select the porfiles to
export/import and set the required options.

On construction a map of profiles is given to the dialog. When the
user presses the button with the id IDOK, this class adjusts the
given map, so that it represents the settings made by the user.

@author Sven Wiegand
 */
class CProfileExchangeDialog : public CDialog
{
// construction/destruction
public:
	/**
	@param bExportMode
	        Specifies whether the dialog is used to export (TRUE) or import
	        (FALSE) output profiles.
	 */
	CProfileExchangeDialog(CProfileMap &profiles, BOOL bExportMode, CWnd* pParent = NULL);

// implementation helpers
public:
	/**
	Refills the profile list.

	By default all items are checked.
	 */
	void RefillProfileList();

// overridings
protected:
//{{AFX_VIRTUAL(CProfileExchangeDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// message handlers
protected:
	//{{AFX_MSG(CProfileExchangeDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelectAll();
	afx_msg void OnSelectNone();
	afx_msg void OnSelectInvert();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

// dialog data
private:
	//{{AFX_DATA(CProfileExchangeDialog)
	enum
	{
		IDD = IDD_PROFILE_EXCHANGE
	};
	CButton m_wndExecuteButton;
	CListCtrl m_wndProfileList;
	BOOL m_bRemoveDirectorySpecifications;
	//}}AFX_DATA

// attributes
private:
	/** TRUE if we are in export mode, FALSE if we are in import mode. */
	BOOL m_bExportMode;

	/** The profile map to export from or to import to. */
	CProfileMap &m_Profiles;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_PROFILEEXCHANGEDIALOG_H__21068D46_1554_4A62_B621_A86AE4A5D1BF__INCLUDED_
