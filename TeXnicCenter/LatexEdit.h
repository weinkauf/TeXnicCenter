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

#if !defined(AFX_LATEXEDIT_H__447410EB_1058_11D3_929E_D75EECD5B417__INCLUDED_)
#define AFX_LATEXEDIT_H__447410EB_1058_11D3_929E_D75EECD5B417__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LatexParser.h"
#include "LatexDoc.h"
#include "CrystalEditViewEx.h"
#include "AutoCompleteListBox.h"

class MyListener;



/** Edit view for LaTeX files.

@author Sven Wiegand
*/
class CLatexEdit : public CCrystalEditViewEx
{
// construction/destruction
protected:
	CLatexEdit();
	DECLARE_DYNCREATE(CLatexEdit)
public:
	virtual ~CLatexEdit();

// operations 
public:
	/**
	Inserts the specified text and the current cursor position.

	@param lpszText
		Text to insert.
	*/
	void InsertText( LPCTSTR lpszText );

	/**
	Should be called, when the configuration has changed. Setzt the
	properties of the editor to that of the configuration and
	redraws the window.
	*/
	void ApplyConfiguration();


// implementation
protected:
	/**
	Returns the text of the current selection. If there is no 
	selection, the LaTeXcommand, the cursor is placed on, is returned.

	@param strKeyword
		Reference to the CString-object, that should receive the
		selected text.
	*/
	void GetSelectedKeyword(CString &strKeyword);

	/**
	Tests if the given character could be a character of a 
	LaTeX-keyword.
	*/
	BOOL IsKeywordCharacter(TCHAR tc) const;


	virtual void QueryComplete();
// static operations
public:
	/**
	Returns the automatic color for the specified colorindex.
	*/
	static COLORREF GetAutomaticColor(int nColorIndex);

// virtuals
protected:
	virtual void ResetView();
	virtual CCrystalTextBuffer *LocateTextBuffer();
	virtual void OnSetStatusMessage(CWnd *pStatusBar, LPCTSTR lpszMessage);

	/** @see CCrystalTextView::GetColor */
	virtual COLORREF GetColor(int nColorIndex);

// overridings
protected:
	//{{AFX_VIRTUAL(CLatexEdit)
	protected:
	//}}AFX_VIRTUAL

// debugging
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// message handler
protected:
	afx_msg BOOL OnInsertLatexConstruct( UINT nID );
	afx_msg LRESULT OnCommandHelp(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSysColorChange();
	//{{AFX_MSG(CLatexEdit)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEditGoto();
	afx_msg void OnEditToggleWhitespaceView();
	afx_msg void OnUpdateEditToggleWhiteSpaceView(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSpellFile();
	afx_msg void OnTextmodulesDefine();
	afx_msg void OnUpdateTextmodulesDefine(CCmdUI* pCmdUI);
	afx_msg void OnQueryCompletion();
	//}}AFX_MSG
	afx_msg void OnUpdateTextModulesList(CCmdUI *pCmdUI);
	afx_msg void OnBlockComment(const UINT nID);
	DECLARE_MESSAGE_MAP()

// attributes
protected:
	CLatexParser	m_latexParser;

public:
	/* Checks if given cursor position is valid */
	BOOL IsValidTextPos(CPoint pos);
	/* Retrieves word before cursor */
	void GetWordBeforeCursor(CString &strKeyword, CPoint &start);
	/* Handlers for events of auto complete listbox */
	virtual void OnACCommandSelect(const CLaTeXCommand *cmd);
	virtual void OnACCommandCancelled();
	virtual void OnACHelp(CString &command);
	virtual void OnACBackspace();
	virtual void OnACChar(UINT nKey, UINT nRepCount, UINT nFlags);
	/* Searches a directory for packages */
	void OnPackageSetup();
	CLatexDoc* GetDocument();

private:
	BOOL RestoreFocus();
	CAutoCompleteListBox *CreateListBox(CString &keywordt, const CPoint topLeft);
	BOOL InvokeContextHelp(const CString keyword);

	CWnd* m_OldFocus;
	CPoint m_oldStart;
	CPoint m_oldEnd;
	CAutoCompleteListBox* m_CompletionListBox;
	MyListener *m_Proxy;
};

#ifndef _DEBUG  // Testversion in LatexEdit.cpp
inline CLatexDoc* CLatexEdit::GetDocument()
   { return (CLatexDoc*)m_pDocument; }
#endif

/**
 This class is needed because CLaTeXEdit will not compile, if CAutoCompleteListener is
 added to the base class list %&$§"*!.
 So we must redirect the listener chain to this class  (sigh).
 */
class MyListener : public CAutoCompleteListener {
public:
	MyListener(CLatexEdit *parent) {ASSERT(parent != NULL); p = parent;}
	/* Command was selected */
	virtual void OnACCommandSelect(const CLaTeXCommand *cmd) {p->OnACCommandSelect(cmd);}
	/* Auto complete was cancelled */
	virtual void OnACCommandCancelled() {p->OnACCommandCancelled();};
	/* User demands context help (F1 key) */
	virtual void OnACHelp(CString &command) {p->OnACHelp(command);}
	/* Backspace was pressed (affects keyword!) */ 
	virtual void OnACBackspace() {p->OnACBackspace();}
	/* A char was typed (affects keyword!) */ 
	virtual void OnACChar(UINT nKey, UINT nRepCount, UINT nFlags) {p->OnACChar(nKey, nRepCount, nFlags);}
private:
	CLatexEdit *p;
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_LATEXEDIT_H__447410EB_1058_11D3_929E_D75EECD5B417__INCLUDED_)
