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

#if !defined(AFX_INSERTGRAPHICDIALOG_H__F884E221_4371_11D3_929E_A6DAFD7685C8__INCLUDED_)
#define AFX_INSERTGRAPHICDIALOG_H__F884E221_4371_11D3_929E_A6DAFD7685C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "InsertFloatObjectDialog.h"

/** Lets the user insert a graphic file.

@ingroup dialogs

@author Sven Wiegand
 */
class CInsertGraphicDialog : public CInsertFloatObjectDialog
{
// construction/destruction
public:
	CInsertGraphicDialog(CWnd* pParent = NULL);

// operations
public:
	virtual BOOL SetProperties(const CString &strProperties);
	virtual CString GetProperties();

// overrides
protected:
//{{AFX_VIRTUAL(CInsertGraphicDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// message handlers
protected:
	//{{AFX_MSG(CInsertGraphicDialog)
	afx_msg void OnGraphicBrowse();
	afx_msg void OnGraphicBscale();
	afx_msg void OnGraphicFloat();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnChangePath();
	afx_msg void OnChangeFloatCaption();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

// dialog data
public:
	//{{AFX_DATA(CInsertGraphicDialog)
	enum
	{
		IDD = IDD_GRAPHIC
	};
	CButton m_wndScaleToText;
	CStatic m_wndScaleLabel;
	CSpinButtonCtrl m_wndScaleSpin;
	CEdit m_wndScale;
	BOOL m_bScale;
	BOOL m_bHCenter;
	CString m_strFile;
	BOOL m_bFloat;
	UINT m_unScale;
	BOOL m_bScaleToText;
	//}}AFX_DATA
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f�gt unmittelbar vor der vorhergehenden Zeile zus�tzliche Deklarationen ein.

#endif // AFX_INSERTGRAPHICDIALOG_H__F884E221_4371_11D3_929E_A6DAFD7685C8__INCLUDED_
