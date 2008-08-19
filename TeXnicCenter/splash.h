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

#pragma once

#include <memory>

/** Splash Screen of TeXnicCenter displayed during startup.

@ingroup dialogs

@author Sven Wiegand
 */
class CSplashWnd : public CWnd
{
	CProgressCtrl progress_bar_;
	CStatic message_;
	CFont font_;
// Konstruktion
protected:
	CSplashWnd();

// Attribute:
public:
	CBitmap m_bitmap;

// Operationen
public:
	static void EnableSplashScreen(BOOL bEnable = TRUE);
	static void ShowSplashScreen(CWnd* pParentWnd = NULL);
	static BOOL PreTranslateAppMessage(MSG* pMsg);

public:
	virtual ~CSplashWnd();
	virtual void PostNcDestroy();

public:
	/** Hides the Splash Window. Do not use the public pointer after calling this function! Will be NULL.*/
	void HideSplashScreen();

protected:
	BOOL Create(CWnd* pParentWnd = NULL);

private:
	static BOOL c_bShowSplashWnd;
	static std::auto_ptr<CSplashWnd> c_pSplashWnd;

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

public:
	static CSplashWnd* GetInstance();
	static bool IsActive();
};
