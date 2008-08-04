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

#if !defined(AFX_TEXTFILESAVEDIALOG_H__74DCE581_1EB3_11D4_A221_006097239934__INCLUDED_)
#define AFX_TEXTFILESAVEDIALOG_H__74DCE581_1EB3_11D4_A221_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LatexDoc.h"

/** A File Dialog with a combo box, where the user can choose
the text format (Unix, Windows, Mac) to save in.

@ingroup dialogs

@author Sven Wiegand
 */
class CTextFileSaveDialog : public CFileDialogEx
{
// construction/destruction
public:
	CTextFileSaveDialog(UINT unTitleID = AFX_IDS_SAVEFILE,
	                    LPCTSTR lpszDefExt = NULL,LPCTSTR lpszFileName = NULL,
	                    DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOREADONLYRETURN,int nFileFormat = CRLF_STYLE_DOS,
	                    LPCTSTR lpszFilter = NULL,
	                    CWnd* pParent = NULL);

// operations
public:
	int GetFileFormat();

// overridings
protected:
//{{AFX_VIRTUAL(CTextFileSaveDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// message handlers
protected:
	//{{AFX_MSG(CTextFileSaveDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeFileFormat();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

// data fields
protected:
	//{{AFX_DATA(CTextFileSaveDialog)
	enum
	{
		IDD = IDD_FILE_SAVE
	};
	CStatic m_wndFileFormatTitle;
	CComboBox m_wndFileFormatCombo;
	int m_nFileFormat;
	//}}AFX_DATA

// attributes
protected:
	/** Title to display */
	CString m_strTitle;
};


// inlines

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_TEXTFILESAVEDIALOG_H__74DCE581_1EB3_11D4_A221_006097239934__INCLUDED_
