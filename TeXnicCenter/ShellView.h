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

#if !defined(AFX_SHELLVIEW_H__10EC4EE4_E283_11D2_9B23_004005649FB5__INCLUDED_)
#define AFX_SHELLVIEW_H__10EC4EE4_E283_11D2_9B23_004005649FB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShellView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShellView view

class CShellView : public CEditView
{
public:
	CShellView();		   // protected constructor used by dynamic creation
	virtual ~CShellView();

protected:
//	CShellView();		   // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CShellView)

private:
	HANDLE m_hChildStdinRd, m_hChildStdinWr, m_hChildStdinWrDup, 
		m_hChildStdoutRd, m_hChildStdoutWr, m_hChildStdoutRdDup; 
	CWinThread*	m_pReadThread;
	DWORD m_dwProcessId;

	LOGFONT m_lf;
	CFont m_defFont;

	CStringArray m_History;
	int m_nHistoryCounter;
// Attributes
public:

// Operations
public:
	void GetUserInput(CString& input);
	void AddTexts(LPCTSTR string, BOOL bReplace = FALSE);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShellView)
	protected:
	virtual void OnDraw(CDC* pDC);	  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
//	virtual ~CShellView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif 

	// Generated message map functions
protected:
	BOOL CreateShellRedirect();
	//{{AFX_MSG(CShellView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEditPaste();
	//}}AFX_MSG
	afx_msg BOOL OnEditChange();
	DECLARE_MESSAGE_MAP()
private:
	void AddTexts(TCHAR ch);
	void WriteToPipe(LPCTSTR line);
	BOOL CreateChildProcess();

	static UINT ReadPipeThreadProc(LPVOID pParam);
private:
	int GetSelLength();
	int GetCurrentPosition();
	void MoveToEnd();
	void MoveToPrompt();
	int m_nLength;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHELLVIEW_H__10EC4EE4_E283_11D2_9B23_004005649FB5__INCLUDED_)
