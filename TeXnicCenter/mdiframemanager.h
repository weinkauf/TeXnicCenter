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

#if !defined(AFX_MDIFRAMEMANAGER_H__11A01CA1_45D6_11D3_929E_A8AA058B8412__INCLUDED_)
#define AFX_MDIFRAMEMANAGER_H__11A01CA1_45D6_11D3_929E_A8AA058B8412__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KeyboardHook.h"

class CMDIFrameManager : public CCmdTarget, public CKeyboardHook
{
	DECLARE_DYNCREATE(CMDIFrameManager)

// construction/destruction
public:
	CMDIFrameManager();

// operations
public:
	/**
	Installs the manager-object for the specified CMDIFrameWnd-object.

	@param pMDIFrameWnd
		Pointer to the CMDIFrameWnd-object to install the manager for.

	@return
		TRUE if successfull, FALSE otherwise.
	*/
	BOOL Install( CMDIFrameWnd *pMDIFrameWnd );

	/**
	Adds the specified frame window to the window list.
	*/
	void AddChildFrame( CFrameWnd *pFrameWnd );

	/**
	Removes the specified frame window from the window list.
	*/
	void RemoveChildFrame( CFrameWnd *pFrameWnd );

	/**
	Activates the specified frame window from the window list.
	*/
	void ActivateChildFrame( CFrameWnd *pFrameWnd );

	/**
	Get the number of child frames in the window list.
	*/
	int GetChildFrameCount() const;

// overwritings
protected:
	virtual BOOL OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags );
	virtual BOOL OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );

// implementation helpers
protected:
	/**
	Looksup the index in the window array matching the specified frame window.

	@pFrameWnd
		Frame window to look up the index for.

	@return
		Index of the specified frame window in the window array or -1, if no
		window matches.
	*/
	int LookupFrameWnd( CFrameWnd *pFrameWnd );

// message handler
protected:
	//{{AFX_MSG(CMDIFrameManager)
	afx_msg void OnWindowList();
	afx_msg void OnUpdateWindowMenu(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnWindow( UINT nID );
	DECLARE_MESSAGE_MAP()

// attributes
protected:
	/** pointer to the CMDIFrameWnd-object, the manager is installed for. */
	CMDIFrameWnd *m_pMDIFrameWnd;

	/** Array of all frame windows to manage.*/
	CArray<CFrameWnd*, CFrameWnd*> m_aFrameWnd;

	/** Index of actual shown window in m_aFrameWnd. */
	int m_nActualWndIndex;

private:
	/** TRUE, if the Ctrl-Key is pressed, FALSE otherwise. */
	BOOL m_bCtrl;
};

#endif // !defined(AFX_MDIFRAMEMANAGER_H__11A01CA1_45D6_11D3_929E_A8AA058B8412__INCLUDED_)
