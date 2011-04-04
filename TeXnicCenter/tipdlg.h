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

#if !defined(TIPDLG_H_INCLUDED_)
#define TIPDLG_H_INCLUDED_

/**	Shows some usefull hints (tips) after startup.

@ingroup dialogs

@author Sven Wiegand
 */
class CTipDlg : public CDialog
{
	CFont title_font_;
	COLORREF title_color_;
// Construction
public:
	CTipDlg(CWnd* pParent = NULL); // standard constructor

	// Dialog Data
	//{{AFX_DATA(CTipDlg)
	// enum { IDD = IDD_TIP };
	BOOL m_bStartup;
	CString m_strTip;
	//}}AFX_DATA

	FILE* m_pStream;

	// Overrides
	// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CTipDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTipDlg();
	virtual INT_PTR DoModal();

protected:

	afx_msg void OnNextTip();
	afx_msg HBRUSH OnCtlColor(CDC* pDC,CWnd* pWnd,UINT nCtlColor);
	virtual void OnOK();
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	void GetNextTipString(CString& strNext);
};

#endif // !defined(TIPDLG_H_INCLUDED_)
