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

#if !defined(AFX_OUTPUTWIZARDVIEWER_H__627C2BE6_5B16_11D4_A222_006097239934__INCLUDED_)
#define AFX_OUTPUTWIZARDVIEWER_H__627C2BE6_5B16_11D4_A222_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EditMenuButtonOpt.h"
#include "OutputWizardPage.h"

/**
This property page is part of the output wizard (COutputWizard) and
requests the user to specify the properties of a file-type-specific
viewer.

@ingroup dialogs
@ingroup profiles

@author Sven Wiegand
 */
class COutputWizardViewer : public OutputWizardPage
{
	DECLARE_DYNCREATE(COutputWizardViewer)

public:
	/**
	Constructs a page for the specified dialog template.
	 */
	COutputWizardViewer(COutputWizard* w = 0, UINT unIDTemplate = -1, UINT title_id = 0);
	virtual ~COutputWizardViewer();


// Overridings
protected:
//{{AFX_VIRTUAL(COutputWizardViewer)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Message Handlers
protected:
	//{{AFX_MSG(COutputWizardViewer)
	afx_msg void OnViewerBrowse();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

// Dialog Data
public:
	//{{AFX_DATA(COutputWizardViewer)
	enum
	{
		IDD = IDD_OUTPUTWIZARD_DVIVIEWER
	};
	CPersistPosEdit m_wndFWSearch;
	CEditMenuButtonOpt m_wndViewerPHBtn;
	CString m_strForwardSearchOption;
	CString m_strPath;
	CString m_strSingleInstanceOption;
	//}}AFX_DATA

// Attributes
protected:

	/**
	Stores the current cursor position of the
	m_wndForwardSearchOptionEdit.
	 */
	struct tagSelection
	{
		int nStart, nEnd;
	} m_selForwardSearchOptionEdit;

public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();

	/// Gets the path of the selected viewer.
	const CString& GetViewerPath() const { return m_strPath; }
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_OUTPUTWIZARDVIEWER_H__627C2BE6_5B16_11D4_A222_006097239934__INCLUDED_
