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

#if !defined(AFX_BCGTOOLBARCUSTOMIZEEX_H__4DDE1561_A8DC_11D6_AF7A_00201855324E__INCLUDED_)
#define AFX_BCGTOOLBARCUSTOMIZEEX_H__4DDE1561_A8DC_11D6_AF7A_00201855324E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PersistPosEdit.h"
#include "EditMenuButtonOpt.h"
#include "UserTool.h"

/**
This class provides a button to display a dialog for
advanced editing of UserTools. The Button will be placed
on the ToolsPage of the Customization Dialog.
We need to implement such a button, because
we do not get the BN_CLICKED, if we would use a normal one.

@see ToolBarsCustomizeDialog
@see CMyUserTool

@ingroup guielements
@ingroup bcgext

@author Tino Weinkauf
 */

class ToolBarCustomizeExAdvBtn : public CButton
{
// Construction
public:
	ToolBarCustomizeExAdvBtn();

// Attributes
public:
	CMyUserTool* pTool;

// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ToolBarCustomizeExAdvBtn)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~ToolBarCustomizeExAdvBtn();

// Generated message map functions
protected:
	//{{AFX_MSG(ToolBarCustomizeExAdvBtn)
	afx_msg void OnClicked();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};



/**
This class overrides some hooks of the original CMFCToolBarsCustomizeDialog
to change the behaviour of the BCG-ToolsPage.
An Edit-Control and a Button are replaced.

@ingroup bcgext

@author Tino Weinkauf
 */
// (AFX_CUSTOMIZE_MENU_SHADOWS | AFX_CUSTOMIZE_TEXT_LABELS | AFX_CUSTOMIZE_MENU_ANIMATIONS | AFX_CUSTOMIZE_NOHELP)

class ToolBarsCustomizeDialog : public CMFCToolBarsCustomizeDialog
{
	DECLARE_DYNAMIC(ToolBarsCustomizeDialog)

//Constructor / Destructor
public:
	ToolBarsCustomizeDialog::ToolBarsCustomizeDialog(CFrameWnd* pWndParentFrame,
	                                       BOOL bAutoSetFromMenus = FALSE,
	                                       UINT uiFlags = (AFX_CUSTOMIZE_MENU_SHADOWS | AFX_CUSTOMIZE_TEXT_LABELS | AFX_CUSTOMIZE_MENU_ANIMATIONS),
	                                       CList <CRuntimeClass*, CRuntimeClass*>* plistCustomPages = NULL);
	virtual ~ToolBarsCustomizeDialog();

// Methods
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ToolBarsCustomizeDialog)
	//}}AFX_VIRTUAL

	virtual void OnInitToolsPage();
	virtual void OnBeforeChangeTool(CUserTool* pSelTool);
	virtual void OnAfterChangeTool(CUserTool* pSelTool);
	virtual BOOL CheckToolsValidity(const CObList& lstTools);


// Attributes
protected:
	/** A Button to replace the original Arguments-Menu-Button.
	                Needed to display an advanced menu with dialog callback.
	 */
	CEditMenuButtonOpt m_wndTXCArgumentsBtn;

	/** An Edit to replace the original Arguments-Edit.
	                Needed for the new Arguments-Menu-Button.
	 */
	CPersistPosEdit m_wndTXCArgumentsEdit;

	///Button for bringing up a dialog to edit advanced features
	ToolBarCustomizeExAdvBtn m_wndAdvBtn;

	///Display area to show summarized advanced features
	CEdit m_wndAdvEdit;

// Generated message map functions
protected:
	//{{AFX_MSG(ToolBarsCustomizeDialog)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void BuildPropPageArray(void);
	static int CALLBACK PropSheetCallback(HWND hWnd, UINT message, LPARAM lParam);
};


/////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_BCGTOOLBARCUSTOMIZEEX_H__4DDE1561_A8DC_11D6_AF7A_00201855324E__INCLUDED_)
