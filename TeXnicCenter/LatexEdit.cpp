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

#include "stdafx.h"
#include "TeXnicCenter.h"

#include "LatexDoc.h"
#include "LatexEdit.h"

#include "configuration.h"
#include "global.h"
#include "InsertFloatObjectDialog.h"
#include "InsertGraphicDialog.h"
#include "InsertTabularDialog.h"
#include "InsertHeaderDialog.h"
#include "GotoDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLatexEdit

IMPLEMENT_DYNCREATE(CLatexEdit, CCrystalEditViewEx)

BEGIN_MESSAGE_MAP(CLatexEdit, CCrystalEditViewEx)
	//{{AFX_MSG_MAP(CLatexEdit)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_WM_CHAR()
	ON_COMMAND(ID_EDIT_GOTO, OnEditGoto)
	ON_COMMAND(ID_EDIT_TOGGLE_WHITESPACEVIEW, OnEditToggleWhitespaceView)
	ON_UPDATE_COMMAND_UI(ID_EDIT_TOGGLE_WHITESPACEVIEW, OnUpdateEditToggleWhiteSpaceView)
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	ON_WM_SYSCOLORCHANGE()

	ON_MESSAGE(WM_COMMANDHELP, OnCommandHelp)

	// commands to insert latex constructs
	ON_COMMAND_EX_RANGE(ID_INSERT_FORMULA_EMBEDDED, ID_INSERT_SPADESUIT, OnInsertLatexConstruct)
	ON_COMMAND_EX_RANGE(ID_INSERT_A, ID_INSERT_NOTIN, OnInsertLatexConstruct)
	ON_COMMAND_EX_RANGE(ID_INSERT_SUPERSCRIPT, ID_INSERT_DDOTS, OnInsertLatexConstruct)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLatexEdit Konstruktion/Destruktion

CLatexEdit::CLatexEdit()
{
	SetParser( &m_latexParser );
	SetWordWrapping( TRUE );
}


CLatexEdit::~CLatexEdit()
{
}


void CLatexEdit::ResetView()
{
	CCrystalEditViewEx::ResetView();

	ApplyConfiguration();
}


void CLatexEdit::ApplyConfiguration()
{
	if( !IsWindow( m_hWnd ) )
		return;

	// set tab size
	SetTabSize( g_configuration.m_nTabWidth );
	
	// set font for editor
	SetFont(g_configuration.m_fontEditor);

	// (de)activate white space view
	SetViewTabs( g_configuration.m_bViewWhitespaces );

	// redraw
	Invalidate();
}


void CLatexEdit::InsertText( LPCTSTR lpszText )
{
	CString	strText( lpszText );

	for( int i = 0; i < strText.GetLength(); i++ )
		CCrystalEditViewEx::OnChar( strText[i], 0, 0 );
		//SendMessage( WM_CHAR, (WPARAM)strText[i] );
}

/////////////////////////////////////////////////////////////////////////////
// CLatexEdit virtuals
COLORREF CLatexEdit::GetColor(int nColorIndex)
{
	if( nColorIndex >= 0 && nColorIndex < COLORINDEX_ERRORBKGND && g_configuration.m_aEditorColors[nColorIndex] != 0xFFFFFFFF )
		return g_configuration.m_aEditorColors[nColorIndex];

	// user set automatic color
	return GetAutomaticColor(nColorIndex);
}


COLORREF CLatexEdit::GetAutomaticColor(int nColorIndex)
{
	switch (nColorIndex)
	{
	case COLORINDEX_WHITESPACE:
	case COLORINDEX_BKGND:
		return ::GetSysColor(COLOR_WINDOW);
	case COLORINDEX_NORMALTEXT:
		return ::GetSysColor(COLOR_WINDOWTEXT);
	case COLORINDEX_SELMARGIN:
		return ::GetSysColor(COLOR_SCROLLBAR);
	case COLORINDEX_PREPROCESSOR:
		return RGB(0x00, 0x00, 0x80);
	case COLORINDEX_COMMENT:
		return RGB(0x80, 0x80, 0x80);
	case COLORINDEX_NUMBER:
		return RGB(0x00, 0x80, 0x00);
	case COLORINDEX_OPERATOR:
		return RGB(0xFF, 0x00, 0x00);
	case COLORINDEX_STRING:
		return RGB(0xFF, 0x00, 0xFF);
	case COLORINDEX_KEYWORD:
		return RGB(0x00, 0x00, 0xFF);
	case COLORINDEX_SELBKGND:
		return RGB(0x00, 0x00, 0x00);
	case COLORINDEX_SELTEXT:
		return RGB(0xFF, 0xFF, 0xFF);
	}

	return RGB(0xFF, 0x00, 0x00);
}


/////////////////////////////////////////////////////////////////////////////
// CLatexEdit Diagnose

#ifdef _DEBUG
void CLatexEdit::AssertValid() const
{
	CCrystalEditViewEx::AssertValid();
}

void CLatexEdit::Dump(CDumpContext& dc) const
{
	CCrystalEditViewEx::Dump(dc);
}

CLatexDoc* CLatexEdit::GetDocument() // Die endgültige (nicht zur Fehlersuche kompilierte) Version ist Inline
{
	if (!m_pDocument)
		return NULL;

	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLatexDoc)));
	return (CLatexDoc*)m_pDocument;
}
#endif //_DEBUG


CCrystalTextBuffer *CLatexEdit::LocateTextBuffer()
{
	return GetDocument()->m_pTextBuffer;
}


void CLatexEdit::OnSetStatusMessage(CWnd *pStatusBar, LPCTSTR lpszMessage)
{
	((CBCGStatusBar*)pStatusBar)->SetPaneText(0, lpszMessage);
}

/////////////////////////////////////////////////////////////////////////////
// CLatexEdit Nachrichten-Handler

int CLatexEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CCrystalEditViewEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	ApplyConfiguration();

	return 0;
}

void CLatexEdit::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	theApp.ShowPopupMenu( IDR_POPUP_EDITOR, point, this );	//UPDATE
}

BOOL CLatexEdit::OnInsertLatexConstruct( UINT nID )
{
	BOOL											bReplaceBell = FALSE;
	CString										strInsert;	// text to insert
	CPoint										ptSelStart, ptSelEnd;
	CInsertFloatObjectDialog	*pDlg = NULL;
	GetSelection( ptSelStart, ptSelEnd );

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// ID specific handling
	switch( nID )
	{
		case ID_INSERT_FIGURE:
			pDlg = new CInsertFloatObjectDialog( CInsertFloatObjectDialog::figure, this );
			break;

		case ID_INSERT_TABLE:
			pDlg = new CInsertFloatObjectDialog( CInsertFloatObjectDialog::table, this );
			break;

		case ID_INSERT_INCLUDEGRAPHICS:
			pDlg = new CInsertGraphicDialog( this );
			break;

		case ID_INSERT_TABULAR:
			pDlg = new CInsertTabularDialog( this );
			break;
	}
	switch( nID )
	{
		case ID_INSERT_HEADER:
		{
			CInsertHeaderDialog	dlg( this );

			if( ptSelStart != ptSelEnd )
			{
				CString	strSel;
				LocateTextBuffer()->GetText( ptSelStart.y, ptSelStart.x, ptSelEnd.y, ptSelEnd.x, strSel, _T("\r") );
				dlg.SetProperties( strSel );
			}
			if( dlg.DoModal() != IDOK )
				return TRUE;

			LocateTextBuffer()->BeginUndoGroup();

			if( ptSelStart != ptSelEnd )
				LocateTextBuffer()->DeleteText( this, ptSelStart.y, ptSelStart.x, ptSelEnd.y, ptSelEnd.x, CE_ACTION_TYPING );

			strInsert = dlg.GetProperties();
			if( dlg.m_strTitle.IsEmpty() )
				bReplaceBell = TRUE;
		}
		break;

		case ID_INSERT_FIGURE:
		case ID_INSERT_TABLE:
		case ID_INSERT_INCLUDEGRAPHICS:
		case ID_INSERT_TABULAR:
		{
			BOOL	bIsSelectionEnvironment = FALSE;
			if( ptSelStart != ptSelEnd )
			{
				CString	strSel;
				LocateTextBuffer()->GetText( ptSelStart.y, ptSelStart.x, ptSelEnd.y, ptSelEnd.x, strSel, _T("\r") );
				bIsSelectionEnvironment = pDlg->SetProperties( strSel );
			}
			if( pDlg->DoModal() != IDOK )
			{
				delete pDlg;
				return TRUE;
			}

			LocateTextBuffer()->BeginUndoGroup();

			if( bIsSelectionEnvironment && ptSelStart != ptSelEnd )
				LocateTextBuffer()->DeleteText( this, ptSelStart.y, ptSelStart.x, ptSelEnd.y, ptSelEnd.x, CE_ACTION_TYPING );

			strInsert = pDlg->GetProperties();
			if( !bIsSelectionEnvironment || pDlg->m_strContents.IsEmpty() )
				bReplaceBell = TRUE;
		}
		break;

		default:
			LocateTextBuffer()->BeginUndoGroup();

			strInsert = AfxLoadString( nID );
			bReplaceBell = TRUE;
	}

	if (pDlg)
		delete pDlg;

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// evaluate how to insert the text
	CString	strBeforeCursor, strBehindCursor;
	strInsert = strInsert.Right( strInsert.GetLength() - strInsert.ReverseFind( _T('\n') ) - 1 );
	strInsert.Replace( _T("\r"), _T("\r\n") );
	
	int nIndentation = 0;

	int	nSplitPos	= strInsert.Find( _T('\001') );
	if( bReplaceBell && nSplitPos > -1 )
	{
		for( int nPos = nSplitPos - 1; nPos >= 0 && strInsert[nPos] == _T('\t'); nPos-- );
		nIndentation = nSplitPos - nPos - 1;
		strBeforeCursor = strInsert.Left( nPos + 1 );
		strBehindCursor = strInsert.Right( strInsert.GetLength() - nSplitPos - 1);
	}
	else
		strBeforeCursor = strInsert;

	// get selection
	GetSelection( ptSelStart, ptSelEnd );

	// test, if selection anchor is at the beginning of at the end of
	// selection.
	BOOL		bAnchorAtEndOfSelection = ptSelEnd != GetCursorPos();
	CPoint	ptNewSelStart = ptSelStart;
	CPoint	ptDummy;

	// adapt ptSelEnd intuitive
	if( ptSelEnd.y > ptSelStart.y && ptSelEnd.x == 0 )
		// don't take an 'empty' line feed with the selection
		ptSelEnd.x = GetLineLength( --ptSelEnd.y );

	// insert text before selection
	if( !strBeforeCursor.IsEmpty() )
		LocateTextBuffer()->InsertText( 
			this, ptSelStart.y, ptSelStart.x, 
			(ptSelStart.x == 0 && strBeforeCursor[0] == _T('\r'))? strBeforeCursor.Right( strBeforeCursor.GetLength() - 2 ) : strBeforeCursor, 
			(int&)ptNewSelStart.y, (int&)ptNewSelStart.x, CE_ACTION_TYPING );

	// calculate new position of the selected text
	if( ptSelStart.y < ptSelEnd.y )
		// ignore horizontal moving of the selected text,
		// if the selected text contains line feeds
		ptSelEnd.Offset( 0, ptNewSelStart.y - ptSelStart.y );
	else
		ptSelEnd.Offset( ptNewSelStart - ptSelStart );

	// insert text behind selection
	if( !strBehindCursor.IsEmpty() )
		LocateTextBuffer()->InsertText( 
			this, ptSelEnd.y, ptSelEnd.x, 
			(ptSelEnd.x == GetLineLength( ptSelEnd.y ) && strBehindCursor[strBehindCursor.GetLength() - 1] == _T('\n'))? strBehindCursor.Left( strBehindCursor.GetLength() - 2 ) : strBehindCursor,
			(int&)ptDummy.y, (int&)ptDummy.x, CE_ACTION_TYPING );

	// indent selection if wanted
	CPoint	ptStart = ptNewSelStart;
	CPoint	ptEnd = ptSelEnd;

	if( nIndentation )
	{
		for( int l = ptStart.y; l <= ptEnd.y; l++ )
			for( int n = 0; n < nIndentation; n++ )
				LocateTextBuffer()->InsertText( 
					this, l, 0, _T("\t"), 
					(int&)ptDummy.y, (int&)ptDummy.x, CE_ACTION_TYPING );

		if( ptStart.y == ptEnd.y )
		{
			ptStart.x+= nIndentation;
			ptEnd.x+= nIndentation;
		}
		else if( ptEnd.x > 0 )
			ptEnd.x+= nIndentation;
	}

	// Set Selection
	SetSelection( ptStart, ptEnd );

	// Set Anchor and cursor for selection
	if( ptStart != ptEnd )
	{
		SetAnchor( bAnchorAtEndOfSelection? ptEnd : ptStart );
		SetCursorPos( bAnchorAtEndOfSelection? ptStart : ptEnd );
	}
	else
		// set cursor
		SetCursorPos( ptStart );

	///////////////////////////////////////////////////////////////////
	// end undo group
	LocateTextBuffer()->FlushUndoGroup( this );

	SetFocus();

	return TRUE;
}


void CLatexEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch( nChar )
	{
		case _T('"'):
			if( !g_configuration.m_bReplaceQuotationMarks )
				CCrystalEditViewEx::OnChar(nChar, nRepCnt, nFlags);
			else				
			{
				CPoint	ptSelStart, ptSelEnd;
				GetSelection( ptSelStart, ptSelEnd );

				// opening quotation mark, if character left to selection is whitespace.
				if( ptSelStart.x < 1 || _istspace( GetLineChars( ptSelStart.y )[ptSelStart.x - 1] ) )
					InsertText( g_configuration.m_strOpeningQuotationMark );
				else
					InsertText( g_configuration.m_strClosingQuotationMark );
			}

			break;

		default:	
			CCrystalEditViewEx::OnChar(nChar, nRepCnt, nFlags);
	}
}

void CLatexEdit::OnEditGoto() 
{
	CGotoDialog	dlg( GetLineCount(), this );
	dlg.m_nLine = GetCursorPos().y + 1;
	if( dlg.DoModal() != IDOK )
		return;

	// goto selected line
	SetCursorPos( CPoint( 0, dlg.m_nLine - 1 ) );
	SetSelection( GetCursorPos(), GetCursorPos() );
	EnsureVisible( GetCursorPos() );
}


void CLatexEdit::OnEditToggleWhitespaceView() 
{
	SetViewTabs( g_configuration.m_bViewWhitespaces = !GetViewTabs() );
}


void CLatexEdit::OnUpdateEditToggleWhiteSpaceView(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( g_configuration.m_bViewWhitespaces );
}


void CLatexEdit::GetSelectedKeyword(CString &strKeyword)
{
	CPoint	ptStart, ptEnd;
	GetSelection(ptStart, ptEnd);

	if (ptStart != ptEnd)
		GetText(ptStart, ptEnd, strKeyword);
	else
	{
		// retrieve the keyword, the cursor is placed on
		CString	strLine(GetLineChars(ptStart.y));

		// check for one char keyword
		if (CString(_T("_^\"#&~|")).Find(strLine[(int)ptStart.x]) >= 0)
		{
			strKeyword = strLine[(int)ptStart.x];
			return;
		}

		// retrieve position of first character of the current word
		for (int nStartChar = ptStart.x; nStartChar >= 0; nStartChar--)
		{
			if (!IsKeywordCharacter(strLine[nStartChar]))
			{
				nStartChar++;
				break;
			}
			if (strLine[nStartChar] == _T('\\'))
				break;
		}
		if (nStartChar < 0)
			nStartChar = 0;
		
		// retrieve position of first character not belonging to the current word
		for (int nEndChar = ptStart.x; nEndChar < strLine.GetLength(); nEndChar++)
		{
			if (!IsKeywordCharacter(strLine[nEndChar]))
				break;
		}

		if (nEndChar <= nStartChar)
			strKeyword.Empty();
		else
			strKeyword = strLine.Mid(nStartChar, nEndChar-nStartChar);
	}
}


BOOL CLatexEdit::IsKeywordCharacter(TCHAR tc) const
{
	switch (tc)
	{
		case _T('#'):
		case _T('$'):
		case _T('%'):
		case _T('&'):
		case _T('~'):
		case _T('_'):
		case _T('^'):
		case _T('{'):
		case _T('}'):
		case _T('.'):
		case _T(':'):
		case _T(';'):
		case _T(','):
		case _T('!'):
		case _T('`'):
		case _T('´'):
		case _T('\''):
		case _T('('):
		case _T(')'):
		case _T('['):
		case _T(']'):
		case _T('-'):
		case _T('+'):
		case _T('='):
		case _T('<'):
		case _T('>'):
		case _T('"'):
		case _T(' '):
		case _T('\t'):
			return FALSE;

		case _T('\\'):
		case _T('@'):
			return TRUE;

		default:
			return _istalpha(tc);
	}
}


LRESULT CLatexEdit::OnCommandHelp(WPARAM wParam, LPARAM lParam)
{
	CString	strKeyword;
	GetSelectedKeyword(strKeyword);
	if (strKeyword.IsEmpty())
	{
		HtmlHelp(NULL, AfxGetApp()->m_pszHelpFilePath, HH_DISPLAY_TOC, 0L);	
		return TRUE;
	}

	HtmlHelp(NULL, theApp.m_pszHelpFilePath, HH_DISPLAY_TOC, 0L);

	HH_AKLINK	link;
	link.cbStruct = sizeof(link);
	link.fReserved = FALSE;
	link.pszKeywords = (LPCTSTR)strKeyword;
	link.pszUrl = NULL;
	link.pszMsgText = NULL;
	link.pszWindow = NULL;
	link.fIndexOnFail = TRUE;
	HtmlHelp(NULL, theApp.m_pszHelpFilePath, HH_KEYWORD_LOOKUP, (DWORD)&link);

	return TRUE;
}


void CLatexEdit::OnSysColorChange()
{
	CCrystalEditViewEx::OnSysColorChange();
	ApplyConfiguration();
}


void CLatexEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CCrystalEditViewEx::OnSetFocus(pOldWnd);

	CLatexDoc	*pDoc = GetDocument();
	if (pDoc)
		pDoc->CheckForFileChanges();
}
