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

class CChildFrame : public CMDIChildWndEx
{
	//CSplitterWndEx m_wndSplitter;

	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// operations
public:
	/**
	Stores frame information to the specified ini-file or restores it
	from the specified file.

	@param ini
	        The ini-file to read the frame information from or to write the
	        information to.
	@param lpszKey
	        Key in the ini file, under which the information should be
	        stored.
	@param bWrite
	        FALSE if the information should be read from the file, TRUE if it
	        should be written to the file.

	@return
	        TRUE on success, FALSE if an error occured. If an error occurs,
	        the window is closed and the object can be destroyed.
	 */
	bool Serialize(CIniFile &ini, LPCTSTR lpszKey, bool write);

	/**
	Returns the FileName / PathName of the document associated with this ChildFrame.
	 */
	CString GetPathNameOfDocument();

// overridings
protected:
//{{AFX_VIRTUAL(CChildFrame)
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void ActivateFrame(int nCmdShow = -1);
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// message handlers
protected:
	//{{AFX_MSG(CChildFrame)
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// attributes
protected:
//CSplitterWnd m_wndSplitter;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
