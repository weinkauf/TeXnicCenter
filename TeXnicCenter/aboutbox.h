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

#ifndef __ABOUTBOX_H__
#define __ABOUTBOX_H__

//#include <hyperlink.h>

/**	About Box of TeXnicCenter.

@ingroup dialogs

@author Sven Wiegand
*/
class CAboutDlg : public CDialog
{
// construction/destruction
public:
	CAboutDlg();

// overwritings
protected:
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// message handlers
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// attributes
public:
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CBCGURLLinkButton m_wndURLToolsCenter;
	CString	m_strVersion;
	//}}AFX_DATA

// scrolling credits attributes
protected:
	#define     DISPLAY_TIMER_ID		150		// timer id

 	RECT        m_ScrollRect,r;		   // rect of Static Text frame
	int         nArrIndex,nCounter;		   // work ints
	CString     m_szWork;			   // holds display line
	BOOL        m_bFirstTime;
	BOOL        m_bDrawText;
	int         nClip;
	int         nCurrentFontHeight;

	CWnd*       m_pDisplayFrame;

	CBitmap     m_bmpWork;                  // bitmap holder
	CBitmap* 	pBmpOld;                    // other bitmap work members
	CBitmap*    m_bmpCurrent;
	HBITMAP 	m_hBmpOld;

	CSize 		m_size;                     // drawing helpers
	CPoint 		m_pt;
	BITMAP 		m_bmpInfo;
	CDC 		m_dcMem;
	BOOL 		m_bProcessingBitmap;
};


#endif
