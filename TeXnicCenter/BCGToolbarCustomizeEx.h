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

/**
This class overrides some hooks of the original CBCGToolbarCustomize
to change the behaviour of the BCG-ToolsPage.
An Edit-Control and a Button are replaced.

@author Tino Weinkauf
*/
class CBCGToolbarCustomizeEx : public CBCGToolbarCustomize  
{
	DECLARE_DYNAMIC(CBCGToolbarCustomizeEx)

//Constructor / Destructor
public:
CBCGToolbarCustomizeEx::CBCGToolbarCustomizeEx(CFrameWnd* pWndParentFrame, 
							BOOL bAutoSetFromMenus = FALSE,
							UINT uiFlags = (BCGCUSTOMIZE_MENU_SHADOWS | BCGCUSTOMIZE_TEXT_LABELS | BCGCUSTOMIZE_LOOK_2000 | BCGCUSTOMIZE_MENU_ANIMATIONS),
							CList <CRuntimeClass*, CRuntimeClass*>* plistCustomPages = NULL);
	virtual ~CBCGToolbarCustomizeEx();

// Methods
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGToolbarCustomizeEx)
	//}}AFX_VIRTUAL

	virtual void OnInitToolsPage ();
	virtual void OnBeforeChangeTool (CBCGUserTool* pSelTool);
	virtual void OnAfterChangeTool (CBCGUserTool* pSelTool);
	virtual BOOL CheckToolsValidity (const CObList& lstTools);


// Attributes
protected:
	CEditMenuButtonOpt m_wndTXCArgumentsBtn;
	CPersistPosEdit m_wndTXCArgumentsEdit;


	// Generated message map functions
protected:
	//{{AFX_MSG(CBCGToolbarCustomizeEx)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_BCGTOOLBARCUSTOMIZEEX_H__4DDE1561_A8DC_11D6_AF7A_00201855324E__INCLUDED_)
