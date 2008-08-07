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
#include "TextModulesDlg.h"
#include "GotoDialog.h"
//#include "../Speller/Character.h"
#include "Advice.h"
#include "TextOutsourceDlg.h"
#include "RunTimeHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CLaTeXEdit,CCrystalEditViewEx)

BEGIN_MESSAGE_MAP(CLaTeXEdit,CCrystalEditViewEx)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_WM_CHAR()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_DESTROY()

	ON_COMMAND(ID_EDIT_GOTO,OnEditGoto)
	ON_COMMAND(ID_EDIT_TOGGLE_WHITESPACEVIEW,OnEditToggleWhitespaceView)
	ON_UPDATE_COMMAND_UI(ID_EDIT_TOGGLE_WHITESPACEVIEW,OnUpdateEditToggleWhiteSpaceView)

	ON_COMMAND(ID_SPELL_FILE,OnSpellFile)
	ON_COMMAND(ID_TEXTMODULES_DEFINE,OnTextmodulesDefine)
	ON_UPDATE_COMMAND_UI(ID_TEXTMODULES_DEFINE,OnUpdateTextmodulesDefine)
	ON_COMMAND(ID_QUERY_COMPLETION,OnQueryCompletion)

	ON_COMMAND(ID_EDIT_OUTSOURCE,OnTextOutsource)

	ON_COMMAND(ID_FORMAT_TEXT_BACK_COLOR, &CLaTeXEdit::OnFormatTextBackColor)
	ON_COMMAND(ID_FORMAT_TEXT_FORE_COLOR, &CLaTeXEdit::OnFormatTextForeColor)
	ON_COMMAND(ID_EDIT_DELETE_LINE, &CLaTeXEdit::OnEditDeleteLine)

	ON_MESSAGE(WM_COMMANDHELP,OnCommandHelp)

	//Static Commands to insert latex constructs
	ON_COMMAND_EX_RANGE(ID_INSERT_FORMULA_EMBEDDED,ID_INSERT_SPADESUIT,OnInsertLatexConstruct)
	ON_COMMAND_EX_RANGE(ID_INSERT_A,ID_INSERT_NOTIN,OnInsertLatexConstruct)
	ON_COMMAND_EX_RANGE(ID_INSERT_SUPERSCRIPT,ID_INSERT_DDOTS,OnInsertLatexConstruct)

	//TextModules
	ON_UPDATE_COMMAND_UI(ID_TEXTMODULES_FIRST,OnUpdateTextModulesList)
	ON_COMMAND_EX_RANGE(ID_TEXTMODULES_FIRST,ID_TEXTMODULES_LAST,OnInsertLatexConstruct)

	//Commands for Block Comments
	ON_COMMAND_EX(ID_EDIT_BLOCKCOMMENT_INSERT,OnBlockComment)
	ON_COMMAND_EX(ID_EDIT_BLOCKCOMMENT_REMOVE,OnBlockComment)
	ON_COMMAND_EX(ID_EDIT_BLOCKCOMMENT_TOOGLE,OnBlockComment)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLaTeXEdit Konstruktion/Destruktion

CLaTeXEdit::CLaTeXEdit()
{
	SetParser(&m_latexParser);
	SetWordWrapping(WORD_WRAP_WINDOW);
	m_pBackgroundThread = theApp.GetBackgroundThread();
	m_CompletionListBox = NULL;
	m_Proxy = new MyListener(this);
	m_OldFocus = NULL;
	m_InstTip = NULL;
	m_AutoCompleteActive = FALSE;
}

CLaTeXEdit::~CLaTeXEdit()
{
	if (m_CompletionListBox != NULL)
	{
		delete m_CompletionListBox;
	}

	if (m_InstTip != NULL)
	{
		delete m_InstTip;
	}
	delete m_Proxy;
}

void CLaTeXEdit::ResetView()
{
	CCrystalEditViewEx::ResetView();

	ApplyConfiguration();
}

void CLaTeXEdit::ApplyConfiguration()
{
	if (!IsWindow(m_hWnd))
		return;

	// set tab size
	SetTabSize(CConfiguration::GetInstance()->m_nTabWidth);

	// show line numbers?
	SetShowLineNumbers(CConfiguration::GetInstance()->m_bShowLineNumbers);

	// set font for editor
	SetFont(CConfiguration::GetInstance()->m_fontEditor);

	// (de)activate white space view
	SetViewTabs(CConfiguration::GetInstance()->m_bViewWhitespaces);

	//word wrapping
	SetWordWrapping((TCRYSTALWORDWRAP)CConfiguration::GetInstance()->m_WordWrapStyle);
	SetFixedColumn(CConfiguration::GetInstance()->m_nFixedColumnWrap);

	// redraw
	Invalidate();
}

void CLaTeXEdit::InsertText(LPCTSTR lpszText)
{
	CString strText(lpszText);

	for (int i = 0; i < strText.GetLength(); i++)
		CCrystalEditViewEx::OnChar(strText[i],0,0);
	//SendMessage( WM_CHAR, (WPARAM)strText[i] );
}

/////////////////////////////////////////////////////////////////////////////
// CLaTeXEdit virtuals

COLORREF CLaTeXEdit::GetColor(int nColorIndex)
{
	if (nColorIndex >= 0 && nColorIndex < COLORINDEX_ERRORBKGND && CConfiguration::GetInstance()->m_aEditorColors[nColorIndex] != 0xFFFFFFFF)
		return CConfiguration::GetInstance()->m_aEditorColors[nColorIndex];

	// user set automatic color
	return GetAutomaticColor(nColorIndex);
}

COLORREF CLaTeXEdit::GetAutomaticColor(int nColorIndex)
{
	switch (nColorIndex)
	{
		case COLORINDEX_WHITESPACE:
		case COLORINDEX_BKGND:
			return ::GetSysColor(COLOR_WINDOW);
		case COLORINDEX_NORMALTEXT:
			return ::GetSysColor(COLOR_WINDOWTEXT);
		case COLORINDEX_SELMARGIN:
			return ::GetSysColor(COLOR_BTNFACE/*COLOR_SCROLLBAR*/);
		case COLORINDEX_PREPROCESSOR:
			return RGB(0x00,0x00,0x80);
		case COLORINDEX_COMMENT:
			return RGB(0x80,0x80,0x80);
		case COLORINDEX_NUMBER:
			return RGB(0x00,0x80,0x00);
		case COLORINDEX_OPERATOR:
			return RGB(0xFF,0x00,0x00);
		case COLORINDEX_STRING:
			return RGB(0xFF,0x00,0xFF);
		case COLORINDEX_KEYWORD:
			return RGB(0x00,0x00,0xFF);
		case COLORINDEX_SELBKGND:
			return ::GetSysColor(COLOR_HIGHLIGHT);
		case COLORINDEX_SELTEXT:
			return ::GetSysColor(COLOR_HIGHLIGHTTEXT);
		case COLORINDEX_PAIRSTRINGBKGND:
			return RGB(0xC0,0xFF,0xC0);
		case COLORINDEX_PAIRSTRINGTEXT:
			return RGB(0x00,0x00,0x00);
		case COLORINDEX_BADPAIRSTRINGBKGND:
			return RGB(0xFF,0xC0,0xC0);
		case COLORINDEX_BADPAIRSTRINGTEXT:
			return RGB(0x00,0x00,0x00);
		case COLORINDEX_CURPAIRSTRINGBKGND:
			return RGB(0xC0,0xFF,0xC0);
		case COLORINDEX_CURPAIRSTRINGTEXT:
			return RGB(0x00,0x00,0x00);
		case COLORINDEX_PAIRBLOCKBKGND:
			return RGB(0xFF,0xFF,0xC0);
		default:
			ASSERT(false); //ColorIndex not found? Shouldn't happen. Please update.
	}

	return RGB(0xFF,0x00,0x00);
}


/////////////////////////////////////////////////////////////////////////////
// CLaTeXEdit Diagnose

#ifdef _DEBUG

void CLaTeXEdit::AssertValid() const
{
	CCrystalEditViewEx::AssertValid();
}

void CLaTeXEdit::Dump(CDumpContext& dc) const
{
	CCrystalEditViewEx::Dump(dc);
}

CLaTeXDoc* CLaTeXEdit::GetDocument() // Die endgültige (nicht zur Fehlersuche kompilierte) Version ist Inline
{
	if (!m_pDocument)
		return NULL;

	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLaTeXDoc)));
	return (CLaTeXDoc*)m_pDocument;
}
#endif //_DEBUG

CCrystalTextBuffer *CLaTeXEdit::LocateTextBuffer()
{
	return GetDocument()->m_pTextBuffer;
}

void CLaTeXEdit::OnSetStatusMessage(CWnd *pStatusBar,LPCTSTR lpszMessage)
{
	((CMFCStatusBar*)pStatusBar)->SetPaneText(0,lpszMessage);
}

int CLaTeXEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CCrystalEditViewEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	ApplyConfiguration();

	return 0;
}

class SpellerSuggestionMenu : public CAttributeMenu
{

public:
	SpellerSuggestionMenu(CCrystalEditView* view,const CPoint& pt)
			: CAttributeMenu(view,pt)
	{
	}

protected:
	int TrackPopupMenu(CView* view, const CPoint& pt, CMenu* menu)
	{
		return theApp.GetContextMenuManager()->TrackPopupMenu(menu->m_hMenu,pt.x,pt.y,view);
	}
};

void CLaTeXEdit::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CPoint ptText = point;
	ScreenToClient(&ptText);
	CCrystalTextBuffer *pBuffer = LocateTextBuffer();
	ASSERT(pBuffer);
	ptText = ClientToText(ptText);
	bool bShowDefault = true;

	CCrystalTextBuffer::CTextAttribute *attr = pBuffer->GetLineAttribute(ptText.y,ptText.x,ptText.x + 1);

	if (attr != NULL)
	{
		SpellerSuggestionMenu menu(this,ptText);

		if (attr->m_Attribute == CCrystalTextBuffer::CTextAttribute::spellError)
			bShowDefault = !menu.ShowSpellMenu(theApp.GetSpeller(),point);
		// else your new attribute here
	}

	if (bShowDefault)
	{
		// Default popup
		theApp.ShowPopupMenu(IDR_POPUP_EDITOR,point,this);
	}
}

BOOL CLaTeXEdit::OnInsertLatexConstruct(UINT nID)
{
	BOOL bReplaceBell = FALSE;
	CString strInsert; // text to insert
	CPoint ptSelStart,ptSelEnd;
	CInsertFloatObjectDialog *pDlg = NULL;
	GetSelection(ptSelStart,ptSelEnd);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// ID specific handling
	switch (nID)
	{
		case ID_INSERT_FIGURE:
			pDlg = new CInsertFloatObjectDialog(CInsertFloatObjectDialog::figure,this);
			break;
		case ID_INSERT_TABLE:
			pDlg = new CInsertFloatObjectDialog(CInsertFloatObjectDialog::table,this);
			break;
		case ID_INSERT_INCLUDEGRAPHICS:
			pDlg = new CInsertGraphicDialog(this);
			break;
		case ID_INSERT_TABULAR:
			pDlg = new CInsertTabularDialog(this);
			break;
	}

	switch (nID)
	{
		case ID_INSERT_HEADER:
		{
			CInsertHeaderDialog dlg(this);

			if (ptSelStart != ptSelEnd)
			{
				CString strSel;
				LocateTextBuffer()->GetText(ptSelStart.y,ptSelStart.x,ptSelEnd.y,ptSelEnd.x,strSel,_T("\r"));
				dlg.SetProperties(strSel);
			}
			if (dlg.DoModal() != IDOK)
				return TRUE;

			LocateTextBuffer()->BeginUndoGroup();

			if (ptSelStart != ptSelEnd)
				LocateTextBuffer()->DeleteText(this,ptSelStart.y,ptSelStart.x,ptSelEnd.y,ptSelEnd.x,CE_ACTION_TYPING);

			strInsert = dlg.GetProperties();

			if (dlg.m_strTitle.IsEmpty())
				bReplaceBell = TRUE;
		}
		break;
		case ID_INSERT_FIGURE:
		case ID_INSERT_TABLE:
		case ID_INSERT_INCLUDEGRAPHICS:
		case ID_INSERT_TABULAR:
		{
			BOOL bIsSelectionEnvironment = FALSE;

			if (ptSelStart != ptSelEnd)
			{
				CString strSel;
				LocateTextBuffer()->GetText(ptSelStart.y,ptSelStart.x,ptSelEnd.y,ptSelEnd.x,strSel,_T("\r"));
				bIsSelectionEnvironment = pDlg->SetProperties(strSel);
			}

			if (pDlg->DoModal() != IDOK)
			{
				delete pDlg;
				return TRUE;
			}

			LocateTextBuffer()->BeginUndoGroup();

			if (bIsSelectionEnvironment && ptSelStart != ptSelEnd)
				LocateTextBuffer()->DeleteText(this,ptSelStart.y,ptSelStart.x,ptSelEnd.y,ptSelEnd.x,CE_ACTION_TYPING);

			strInsert = pDlg->GetProperties();

			if (!bIsSelectionEnvironment || pDlg->m_strContents.IsEmpty())
				bReplaceBell = TRUE;
		}
		break;
		default:
			if ((nID >= ID_TEXTMODULES_FIRST) && (nID <= ID_TEXTMODULES_LAST))
			{
				ASSERT(CConfiguration::GetInstance()->m_aTextModules.GetSize() > nID - ID_TEXTMODULES_FIRST);

				if (CConfiguration::GetInstance()->m_aTextModules.GetSize() > nID - ID_TEXTMODULES_FIRST)
					strInsert = CConfiguration::GetInstance()->m_aTextModules[nID - ID_TEXTMODULES_FIRST].GetText();
				else
					return false;
			}
			else
				strInsert = AfxLoadString(nID);

			bReplaceBell = true;

			LocateTextBuffer()->BeginUndoGroup();
	}

	if (pDlg)
		delete pDlg;

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// evaluate how to insert the text
	CString strBeforeCursor,strBehindCursor;
	strInsert = strInsert.Right(strInsert.GetLength() - strInsert.ReverseFind(_T('\n')) - 1);
	strInsert.Replace(_T("\r"),_T("\r\n"));

	int nIndentation = 0;

	int nSplitPos = strInsert.Find(_T('\001'));

	if (bReplaceBell && nSplitPos > -1)
	{
		int nPos = nSplitPos - 1;
		for (; nPos >= 0 && strInsert[nPos] == _T('\t'); nPos--);
		nIndentation = nSplitPos - nPos - 1;
		strBeforeCursor = strInsert.Left(nPos + 1);
		strBehindCursor = strInsert.Right(strInsert.GetLength() - nSplitPos - 1);
	}
	else
		strBeforeCursor = strInsert;

	// get selection
	GetSelection(ptSelStart,ptSelEnd);

	// test, if selection anchor is at the beginning or at the end of
	// selection.
	BOOL bAnchorAtEndOfSelection = ptSelEnd != GetCursorPos();
	CPoint ptNewSelStart = ptSelStart;
	CPoint ptDummy;

	// adapt ptSelEnd intuitive
	if (ptSelEnd.y > ptSelStart.y && ptSelEnd.x == 0)
		// don't take an 'empty' line feed with the selection
		ptSelEnd.x = GetLineLength(--ptSelEnd.y);

	// insert text before selection
	if (!strBeforeCursor.IsEmpty())
		LocateTextBuffer()->InsertText(
		    this,ptSelStart.y,ptSelStart.x,
		    (ptSelStart.x == 0 && strBeforeCursor[0] == _T('\r')) ? strBeforeCursor.Right(strBeforeCursor.GetLength() - 2) : strBeforeCursor,
		    (int&)ptNewSelStart.y,(int&)ptNewSelStart.x,CE_ACTION_TYPING);

	// calculate new position of the selected text
	if (ptSelStart.y < ptSelEnd.y)
		// ignore horizontal moving of the selected text,
		// if the selected text contains line feeds
		ptSelEnd.Offset(0,ptNewSelStart.y - ptSelStart.y);
	else
		ptSelEnd.Offset(ptNewSelStart - ptSelStart);

	// insert text behind selection
	if (!strBehindCursor.IsEmpty())
		LocateTextBuffer()->InsertText(
		    this,ptSelEnd.y,ptSelEnd.x,
		    (ptSelEnd.x == GetLineLength(ptSelEnd.y) && strBehindCursor[strBehindCursor.GetLength() - 1] == _T('\n')) ? strBehindCursor.Left(strBehindCursor.GetLength() - 2) : strBehindCursor,
		    (int&)ptDummy.y,(int&)ptDummy.x,CE_ACTION_TYPING);

	// indent selection if wanted
	CPoint ptStart = ptNewSelStart;
	CPoint ptEnd = ptSelEnd;

	if (nIndentation)
	{
		for (int l = ptStart.y; l <= ptEnd.y; l++)
			for (int n = 0; n < nIndentation; n++)
				LocateTextBuffer()->InsertText(
				    this,l,0,_T("\t"),
				    (int&)ptDummy.y,(int&)ptDummy.x,CE_ACTION_TYPING);

		if (ptStart.y == ptEnd.y)
		{
			ptStart.x += nIndentation;
			ptEnd.x += nIndentation;
		}
		else if (ptEnd.x > 0)
			ptEnd.x += nIndentation;
	}

	// Set Selection
	SetSelection(ptStart,ptEnd);

	// Set Anchor and cursor for selection
	if (ptStart != ptEnd)
	{
		SetAnchor(bAnchorAtEndOfSelection ? ptEnd : ptStart);
		SetCursorPos(bAnchorAtEndOfSelection ? ptStart : ptEnd);
	}
	else
		// set cursor
		SetCursorPos(ptStart);

	///////////////////////////////////////////////////////////////////
	// end undo group
	LocateTextBuffer()->FlushUndoGroup(this);

	SetFocus();

	return TRUE;
}

void CLaTeXEdit::OnChar(UINT nChar,UINT nRepCnt,UINT nFlags)
{
	//If we are in incremental search mode, then we do not act here
	if (m_bIncrementalSearchForward || m_bIncrementalSearchBackward)
	{
		CCrystalEditViewEx::OnChar(nChar,nRepCnt,nFlags);
		return;
	}

	if (!GetDocument()->IsModified())
		GetDocument()->SetModifiedFlag();

	switch (nChar)
	{
		case _T('"') :
			if (!CConfiguration::GetInstance()->m_bReplaceQuotationMarks)
				CCrystalEditViewEx::OnChar(nChar,nRepCnt,nFlags);
			else
			{
				CPoint ptSelStart,ptSelEnd;
				GetSelection(ptSelStart,ptSelEnd);

				// opening quotation mark, if character is first in line
				if (ptSelStart.x < 1)
					InsertText(CConfiguration::GetInstance()->m_strOpeningQuotationMark);
				else
				{
					// opening quotation mark, if character left of selection is whitespace or open brace
					TCHAR cLeft = GetLineChars(ptSelStart.y)[ptSelStart.x - 1];

					if (_istspace(cLeft) || cLeft == _T('(') || cLeft == _T('{') || cLeft == _T('['))
						InsertText(CConfiguration::GetInstance()->m_strOpeningQuotationMark);
					else
						InsertText(CConfiguration::GetInstance()->m_strClosingQuotationMark);
				}
			}

			break;
		default:
			CCrystalEditViewEx::OnChar(nChar,nRepCnt,nFlags);
			InstantAdvice();
	}
}

void CLaTeXEdit::OnEditGoto()
{
	CGotoDialog dlg(GetLineCount(),this);
	dlg.m_nLine = GetCursorPos().y + 1;
	if (dlg.DoModal() != IDOK)
		return;

	// goto selected line
	SetCursorPos(CPoint(0,dlg.m_nLine - 1));
	SetSelection(GetCursorPos(),GetCursorPos());
	EnsureVisible(GetCursorPos());
}

void CLaTeXEdit::OnEditToggleWhitespaceView()
{
	SetViewTabs(CConfiguration::GetInstance()->m_bViewWhitespaces = !GetViewTabs());
}

void CLaTeXEdit::OnUpdateEditToggleWhiteSpaceView(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(CConfiguration::GetInstance()->m_bViewWhitespaces);
}

void CLaTeXEdit::GetSelectedKeyword(CString &strKeyword)
{
	CPoint ptStart,ptEnd;
	GetSelection(ptStart,ptEnd);

	if (ptStart != ptEnd)
		GetText(ptStart,ptEnd,strKeyword);
	else if (GetLineLength(ptStart.y))
	{
		// retrieve the keyword, the cursor is placed on
		CString strLine(GetLineChars(ptStart.y));

		// check for one TCHAR keyword
		if (CString(_T("_^\"#&~|")).Find(strLine[(int)ptStart.x]) >= 0)
		{
			strKeyword = strLine[(int)ptStart.x];
			return;
		}

		// retrieve position of first character of the current word
		int nStartChar = ptStart.x;

		for (; nStartChar >= 0; nStartChar--)
		{
			if (!IsKeywordCharacter(strLine[nStartChar]))
			{
				++nStartChar;
				break;
			}
			if (strLine[nStartChar] == _T('\\'))
				break;
		}
		if (nStartChar < 0)
			nStartChar = 0;

		// retrieve position of first character not belonging to the current word
		int nEndChar = ptStart.x;

		for (; nEndChar < strLine.GetLength(); nEndChar++)
		{
			if (!IsKeywordCharacter(strLine[nEndChar]))
				break;
		}

		if (nEndChar <= nStartChar)
			strKeyword.Empty();
		else
			strKeyword = strLine.Mid(nStartChar,nEndChar - nStartChar);
	}
}

BOOL CLaTeXEdit::IsKeywordCharacter(TCHAR tc) const
{
	switch (tc)
	{
		case _T('#') :
		case _T('$') :
		case _T('%') :
		case _T('&') :
		case _T('~') :
		case _T('_') :
		case _T('^') :
		case _T('{') :
		case _T('}') :
		case _T('.') :
		case _T(':') :
		case _T(';') :
		case _T(',') :
		case _T('!') :
		case _T('`') :
		case _T('´') :
		case _T('\'') :
		case _T('(') :
		case _T(')') :
		case _T('[') :
		case _T(']') :
		case _T('-') :
		case _T('+') :
		case _T('=') :
		case _T('<') :
		case _T('>') :
		case _T('"') :
		case _T(' ') :
		case _T('\t') :
			return FALSE;

		case _T('\\') :
		case _T('@') :
			return TRUE;

		default:
			return _istalpha(tc);
	}
}

BOOL CLaTeXEdit::IsAutoCompletionCharacter(TCHAR tc) const
{
	switch (tc)
	{
			//All the following chars are allowed in labels.
			// - but we do not allow all since some of them are rather strange
		case _T('&') :
		case _T('_') :
		case _T('-') :
		case _T('+') :
		case _T(':') :
			//case _T('='):
			//case _T('^'):
			//case _T('.'):
			//case _T(';'):
			//case _T(','): //A comma can be used in \cite{weinkauf04a,weinkauf05b} and we want to see this as two bibkeys
			//case _T('!'):
			//case _T('`'):
			//case _T('´'):
			//case _T('('): //Especially braces should not be part of a label; this would kill most editor stuff
			//case _T(')'):
			//case _T('['):
			//case _T(']'):
			//case _T('<'):
			//case _T('>'):

			//All the following chars are allowed in keywords.
		case _T('\\') :
		case _T('@') :
			return TRUE;

			//Be default, labels can consist of numbers and letters; keywords only of letters
		default:
			return _istalnum(tc);
	}
}

LRESULT CLaTeXEdit::OnCommandHelp(WPARAM wParam, LPARAM lParam)
{
	UNUSED_ALWAYS(wParam);
	UNUSED_ALWAYS(lParam);

	CString strKeyword;
	GetSelectedKeyword(strKeyword);
	return InvokeContextHelp(strKeyword);
}

void CLaTeXEdit::OnSysColorChange()
{
	CCrystalEditViewEx::OnSysColorChange();
	ApplyConfiguration();
}

void CLaTeXEdit::OnSetFocus(CWnd* pOldWnd)
{
	CCrystalEditViewEx::OnSetFocus(pOldWnd);

	CLaTeXDoc *pDoc = GetDocument();
	if (pDoc)
		pDoc->CheckForFileChanges();
	/*
	if (m_CompletionListBox != NULL) {
	DestroyListBox();
	}*/
}

void CLaTeXEdit::OnSpellFile()
{
	// Save selection
	CPoint ptStart,ptEnd;
	GetSelection(ptStart,ptEnd);

	SetShowInteractiveSelection(TRUE);
	CCrystalResources cr;
	CSpellCheckDlg dlg(this,theApp.GetSpeller());
	dlg.DoModal();
	SetShowInteractiveSelection(FALSE);

	// Restore selection
	SetSelection(ptStart,ptEnd);
}

afx_msg BOOL CLaTeXEdit::OnBlockComment(const UINT nID)
{
	//Get the current selection
	CPoint ptStartSel,ptEndSel;
	GetSelection(ptStartSel,ptEndSel);
	//Get the line numbers to act on
	const int nStartLine = ptStartSel.y;
	const int nEndLine = ptEndSel.y;
	ASSERT(nStartLine <= nEndLine);

	//Get the text buffer
	CCrystalTextBuffer* pText = LocateTextBuffer();

	//Start Undo Group
	pText->BeginUndoGroup();


	//We want to issue a comment even if nothing or just one line is selected ==> (nStartLine == nEndLine)
	//And we want the last line to be commented even if it is not fully selected
	int nEndLineOffset = ((nStartLine == nEndLine) || (ptEndSel.x > 0)) ? 1 : 0;

	//Commenting the First line is different, because
	// comment could be inserted/removed/toogled in the middle of the line, too.
	// - Where to start with acting
	//NOTE: Before and including TXC 1 Beta 7.01 we inserted the comment where the cursor was placed.
	// Now we do it for the whole line, always.
	//int nStartChar = ptStartSel.x; ==> old behavior
	int nStartChar = 0;


	//Go through all lines
	for (int i = nStartLine; i < nEndLine + nEndLineOffset; i++)
	{
		//Is this line commented?
		bool bHasComment = false;
		CString strCommentTest;
		if (pText->GetLineLength(i) > nStartChar)
		{
			pText->GetText(i,nStartChar,i,nStartChar + 1,strCommentTest);
			bHasComment = ((strCommentTest.GetLength() > 0) && (strCommentTest[0] == '%'));
		}

		//What to do for the Toogle Command?
		UINT NewID = nID;
		if (nID == ID_EDIT_BLOCKCOMMENT_TOOGLE)
		{
			NewID = (bHasComment) ? ID_EDIT_BLOCKCOMMENT_REMOVE : ID_EDIT_BLOCKCOMMENT_INSERT;
		}

		//Insert or Remove Comment for the current line
		switch (NewID)
		{
			case ID_EDIT_BLOCKCOMMENT_INSERT:
			{
				CPoint ptNewStartSel;
				//Insert Comment
				pText->InsertText(this,i,nStartChar,_T("%"),
				                  (int&)ptNewStartSel.y,(int&)ptNewStartSel.x,
				                  CE_ACTION_TYPING);
				break;
			}

			case ID_EDIT_BLOCKCOMMENT_REMOVE:
			{
				if (bHasComment)
					pText->DeleteText(this,i,nStartChar,i,nStartChar + 1,CE_ACTION_TYPING);

				break;
			}
		}

		nStartChar = 0;
	}//end of "Go through all lines"


	//End Undo Group
	pText->FlushUndoGroup(this);

	SetFocus();
	return TRUE;
}

void CLaTeXEdit::OnUpdateTextModulesList(CCmdUI *pCmdUI)
{
	CTextModuleGroup* ptmGroup = &CConfiguration::GetInstance()->m_aTextModules;

	if (CConfiguration::GetInstance()->m_bOptimizeMenuForVisuallyHandicappedUsers)
	{
		/////////////////////////////////////////////////////////////////
		//
		// handling for normal windows menus
		// (visually handicapped user mode)
		//

		// update message from menu bar?
		//CMenu	*pMenu = pCmdUI->m_pSubMenu;
		CMenu* pMenu = pCmdUI->m_pMenu;
		if (!pMenu || !IsMenu(pMenu->GetSafeHmenu()))
			return;

		int i;

		//Delete old entries
		for (i = pMenu->GetMenuItemCount(); i >= 3; --i)
			pMenu->DeleteMenu(i,MF_BYPOSITION);

		//Anything defined at all?
		if (!ptmGroup->GetSize())
		{
			pCmdUI->SetText(AfxLoadString(IDS_TEXTMODULES_NONEDEFINED));
			pCmdUI->Enable(false);
			return;
		}

		//There is something to insert. So lets delete the last entry.
		ASSERT(pMenu->GetMenuItemCount() == 3);
		if (pMenu->GetMenuItemCount() > 2)
			pMenu->DeleteMenu(2,MF_BYPOSITION);

		for (i = 0; i < ptmGroup->GetSize(); i++)
		{
			//Get Name to display
			CString strDisplayName(ptmGroup->GetAt(i).Name);

			//Add number for the first ten entries
			if (i < 10)
			{
				CString strFormat;
				if (i == 9)
					strFormat.Format(_T("1&0 %s"),strDisplayName);
				else
					strFormat.Format(_T("&%d %s"),i + 1,strDisplayName);

				strDisplayName = strFormat;
			}

			pMenu->AppendMenu(MF_STRING,ID_TEXTMODULES_FIRST + i,strDisplayName);
		}
	}
	else
	{
		/////////////////////////////////////////////////////////////////
		//
		// handling for BCG menus
		//

		// update message from menu bar?
		if (!pCmdUI->m_pOther || !pCmdUI->m_pOther->IsKindOf(RUNTIME_CLASS(CMFCPopupMenuBar)))
			return;

		CMFCPopupMenuBar* pMenu = (CMFCPopupMenuBar*)pCmdUI->m_pOther;
		bool bChange = false;

		//Anything defined at all?
		if (!ptmGroup->GetSize())
		{
			pCmdUI->SetText(AfxLoadString(IDS_TEXTMODULES_NONEDEFINED));
			pCmdUI->Enable(false);
			//Delete the rest
			//TODO: We have an assertion in the BCG later on,
			// if there is something to delete here.
			// Anyway, it works perfectly after this.
			// Someone with the full sources needs to have a look at it.
			while (pMenu->GetCount() > 3)
			{
				pMenu->RemoveButton(pMenu->GetCount() - 1);
				bChange = true;
			}

			//Repaint menu
			//if (bChange) pMenu->AdjustLayout();
			return;
		}

		//Insert all entries
		int i,j;
		for (i = 0,j = 2; i < ptmGroup->GetSize(); i++,j++)
		{
			//Get Name to display
			CString strDisplayName(ptmGroup->GetAt(i).Name);

			//Add number for the first ten entries
			if (i < 10)
			{
				CString strFormat;
				if (i == 9)
					strFormat.Format(_T("1&0 %s"),strDisplayName);
				else
					strFormat.Format(_T("&%d %s"),i + 1,strDisplayName);

				strDisplayName = strFormat;
			}

			if (pMenu->GetCount() > j && pMenu->GetButtonText(j) != strDisplayName)
			{
				pMenu->SetButtonText(j,strDisplayName);
				bChange = true;
			}
			else if (pMenu->GetCount() <= j)
			{
				int nIndex = pMenu->InsertButton(CMFCToolBarMenuButton(ID_TEXTMODULES_FIRST + i,0,-1,strDisplayName),j);
				ASSERT(nIndex > -1);

				bChange = true;
			}
		}
		ASSERT(j == ptmGroup->GetSize() + 2);

		//Remove additional entries
		while (j < pMenu->GetCount())
		{
			pMenu->RemoveButton(pMenu->GetCount() - 1);
			bChange = true;
		}

		//Repaint menu
		//if (bChange) pMenu->AdjustLayout();
	}
}

void CLaTeXEdit::OnTextmodulesDefine()
{
	CTextModulesDlg dlg;

	//Give a copy of the TextModules
	dlg.m_tmGroup.RemoveAll();
	for (int i = 0; i < CConfiguration::GetInstance()->m_aTextModules.GetSize(); i++)
	{
		dlg.m_tmGroup.InsertSorted(CConfiguration::GetInstance()->m_aTextModules[i]);
	}

	if (dlg.DoModal() == IDOK)
	{
		CConfiguration::GetInstance()->m_aTextModules.RemoveAll();
		for (int i = 0; i < dlg.m_tmGroup.GetSize(); i++)
		{
			CConfiguration::GetInstance()->m_aTextModules.InsertSorted(dlg.m_tmGroup[i]);
		}
	};
}

void CLaTeXEdit::OnUpdateTextmodulesDefine(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(true);
}

void CLaTeXEdit::OnQueryCompletion()
{
	CString keyword;
	CPoint topLeft;

	if (m_InstTip != NULL)   // Drop advice
	{
		m_InstTip->ShowWindow(SW_HIDE);
	}

	// Don't allow a second window
	if (m_AutoCompleteActive)
	{
		TRACE("==> autocomplete is active, quitting...\n");
		return;
	}

	GetSelection(m_oldStart,m_oldEnd); /* store old position */

	GetWordBeforeCursor(keyword,topLeft); /* retrieve word to be completed */

	if (!keyword.IsEmpty())
	{
		m_CompletionListBox = CreateListBox(keyword,topLeft); /* setup (and show) list box */
	}
	else
	{
		SetSelection(m_oldStart,m_oldEnd); /* restore old position */
	}
}

CAutoCompleteDlg* CLaTeXEdit::CreateListBox(CString &keyword, const CPoint topLeft)
{
	CPoint ptStart,ptText;
	if (!IsValidTextPos(topLeft))
	{
		TRACE("Invalid text pos %d, %d\n",topLeft.x,topLeft.y);
		return 0;
	}
	//TRACE("==> CreateListBox\n");
	ptStart = TextToClient(topLeft);
	ptStart.y += GetLineHeight(); // Goto next row

	// setup listbox
	int nWords = GetNumberOfMatches(keyword); // find number of matches
	//TRACE("==> CreateListBox matches = %d\n", nWords);

	//Found one or more matches?
	if (nWords >= 1)
	{
		//Create window, if needed
		if (m_CompletionListBox == NULL)
		{
			m_CompletionListBox = new CAutoCompleteDlg(&theApp.m_AvailableCommands,this /*theApp.GetMainWnd()*/);
			m_CompletionListBox->SetListener(m_Proxy);
		}

		//InitWithKeyword will notify the listener immediately, if only one exact match exists
		if (m_CompletionListBox->InitWithKeyword(keyword) && nWords > 1)
		{
			//TRACE("==> CreateListBox: Show listbox \n");
			m_AutoCompleteActive = TRUE;

			//Move box by (16 IconPixels + 2 Pixels between Icon and text + 6/2 BorderPixels + 1 Pixel Magick) = 22
			//This way, the text matches with the editor
			ClientToScreen(&ptStart); //yes, this works only with screen coords. do not ask me why.
			m_CompletionListBox->SetWindowPos(NULL,ptStart.x - 22,ptStart.y,0,0,SWP_NOSIZE | SWP_NOZORDER
			                                  | SWP_NOACTIVATE);

			//Adjust size of the box to see all content and we do not want to be over the border
			m_CompletionListBox->AdjustSizeAndPosition(GetLineHeight());

			m_CompletionListBox->ShowWindow(SW_SHOW);
			m_CompletionListBox->SetCurSel(0);
		}
		else
		{
			//TRACE("==> CreateListBox: NOT Show listbox \n");
		}
	}

	//TRACE("<== CreateListBox\n");
	return m_CompletionListBox;
}

void CLaTeXEdit::QueryComplete()
{
	OnQueryCompletion();
}

void CLaTeXEdit::GetWordBeforeCursor(CString& strKeyword,CPoint& start,bool bSelect /*=true*/)
{
	const CPoint ptCursor = GetCursorPos();

	if (GetLineLength(ptCursor.y) > 0)
	{
		//Get the line, the cursor is placed on
		CString strLine(GetLineChars(ptCursor.y));

		//Get the position from where we start the backward search
		ASSERT(ptCursor.x <= strLine.GetLength());
		int EndX = (ptCursor.x <= strLine.GetLength()) ? ptCursor.x - 1 : (strLine.GetLength() - 1);
		int CurrentX = EndX;

		//Backward search: go to first character of the current word
		for (; CurrentX >= 0; CurrentX--)
		{
			if (!IsAutoCompletionCharacter(strLine[CurrentX]))
			{
				++CurrentX; //This is the last valid TCHAR
				break;
			}
		}
		if (CurrentX < 0) CurrentX = 0;

		start.x = CurrentX;
		start.y = ptCursor.y;

		if (CurrentX <= EndX)
		{
			ASSERT(CurrentX >= 0);
			ASSERT(EndX - CurrentX + 1 > 0);

			strKeyword = strLine.Mid(CurrentX,EndX - CurrentX + 1);
			if (bSelect)
			{
				SetSelection(start,ptCursor);
			}
		}
		else
		{
			strKeyword.Empty();
		}
	}
}

/* Invokes context help for a given keyword */
BOOL CLaTeXEdit::InvokeContextHelp(const CString keyword)
{
	if (keyword.IsEmpty())
	{
		::HtmlHelp(NULL,AfxGetApp()->m_pszHelpFilePath,HH_DISPLAY_TOC,0L);
		return TRUE;
	}

	::HtmlHelp(NULL,theApp.m_pszHelpFilePath,HH_DISPLAY_TOC,0L);

	HH_AKLINK link;
	link.cbStruct = sizeof(link);
	link.fReserved = FALSE;
	link.pszKeywords = (LPCTSTR)keyword;
	link.pszUrl = NULL;
	link.pszMsgText = NULL;
	link.pszWindow = _T(">$global_TxcHelpWindow");
	link.fIndexOnFail = TRUE;
	::HtmlHelp(NULL,theApp.m_pszHelpFilePath,HH_KEYWORD_LOOKUP,(DWORD) & link);

	return TRUE;
}

/* ------------ Message handlers for auto complete listbox ------------*/

void CLaTeXEdit::OnACCommandSelect(const CLaTeXCommand* cmd)
{
	CPoint ptStart,ptEnd;
	CPoint ptCaret;
	BOOL isEnv = FALSE;

	m_AutoCompleteActive = FALSE;
	ASSERT(cmd != NULL);

	//Generate string to be inserted
	CString InsertString = cmd->GetExpandBefore(); // collect completion...
	InsertString += cmd->ToLaTeX();
	InsertString += cmd->GetExpandAfter();

	GetSelection(ptStart,ptEnd);

	//Get the text buffer
	CCrystalTextBuffer* pText = LocateTextBuffer();

	//Insert the text
	pText->BeginUndoGroup();
	ReplaceSelection(InsertString);
	pText->FlushUndoGroup(this);

	GetSelection(ptStart,ptEnd); // retrieve new selection pos
	SetSelection(ptStart,ptStart); // drop selection

	isEnv = cmd->GetExpandAfter().GetLength() && cmd->GetExpandBefore().GetLength();

	if (!isEnv)
	{
		bool bSetPosition(false);

		//If a command was inserted and it contains a brace, we might want to go there
		if (InsertString.FindOneOf(_T("{[(")) != -1)
		{
			//Search for the first brace
			for (int y = ptStart.y; y <= ptEnd.y; y++)
			{
				CString s = GetLineChars(y);

				//Is there an offset? Only in first line.
				int Offset = 0;
				if (y == ptStart.y)
				{
					s = s.Mid(ptStart.x);
					Offset = ptStart.x;
				}

				//Find a brace and place the cursor
				int BracePos = s.FindOneOf(_T("{[("));
				if (BracePos != -1)
				{
					ptCaret = CPoint(Offset + BracePos + 1,y);

					if (IsValidTextPos(ptCaret))
					{
						//Place caret after the first opening brace
						SetCursorPos(ptCaret);
						bSetPosition = true;
					}

					break;
				}
			}
		}

		// no brace found; drop selection and place cursor after inserted word
		if (!bSetPosition) SetCursorPos(ptEnd);
	}
	else
	{
		// env was inserted -> place cursor at end of next row
		ptCaret.y = ptStart.y + 1;
		ptCaret.x = GetLineLength(ptCaret.y);

		SetCursorPos(ptCaret);
	}
	ptCaret = GetCursorPos();
	SetSelection(ptCaret,ptCaret); // sync selection with C'pos
	RestoreFocus(); // set focus back to edit view
}

void CLaTeXEdit::OnACCommandCancelled()
{
	/* This issue is a little bit weird: The AC window sends a OnACCommandCancelled in every case the window is close
	in order to react on KillFocus msgs. If the completion was finished OnACCommandSelect, we must prohibit to execute
	OnACCommandCancelled in this case. So we use the m_AutoCompleteActive flag to determine, if the autocomplete
	has been finished or not */
	if (!m_AutoCompleteActive) return;

	/* try to restore old cursor pos */
	if (IsValidTextPos(m_oldStart) && IsValidTextPos(m_oldEnd))
	{
		SetSelection(m_oldStart,m_oldEnd);
	}
	else   /* suggest new position */
	{
		GetSelection(m_oldStart,m_oldEnd);
		SetSelection(m_oldEnd,m_oldEnd);
	}
	m_AutoCompleteActive = FALSE;
	RestoreFocus();
}

void CLaTeXEdit::OnACHelp(CString &cmd)
{
	InvokeContextHelp(cmd);
}

void CLaTeXEdit::OnACBackspace()
{
	if (m_CompletionListBox != NULL && m_CompletionListBox->IsWindowVisible())
	{
		CPoint ps,pe,dummy;
		GetSelection(ps,pe);
		dummy.x = pe.x - 1;
		dummy.y = pe.y;
		SetSelection(dummy,pe);
		//CCrystalEditViewEx::OnChar(VK_BACK, 1, 0);
		if (ReplaceSelection(_T("")))   /* WÜRG !!!! */
		{
			GetSelection(dummy,pe); /* retrieve new cursor pos */
			SetSelection(ps,pe); /* restore selection */
		}
		else
		{
			TRACE("Could not delete selection!\n");
		}
		return;
	}
}

void CLaTeXEdit::OnACChar(UINT nKey,UINT nRepCount,UINT nFlags)
{
	if (m_CompletionListBox != NULL && m_CompletionListBox->IsWindowVisible())
	{
		CPoint ps,pe,dummy;
		GetSelection(ps,pe);
		SetSelection(pe,pe);
		CCrystalEditViewEx::OnChar(nKey,nRepCount,nFlags);
		GetSelection(dummy,pe); /* retrieve new cursor pos */
		SetSelection(ps,pe); /* restore selection */
		return;
	}
}

/* Checks, if pos is a valid cursor position (to avoid assertions due to invalid pos) */
BOOL CLaTeXEdit::IsValidTextPos(CPoint point)
{
	if (GetLineCount() > 0)
	{
		return (m_nTopLine >= 0 && m_nOffsetChar >= 0) &&
		       (point.y >= 0 && point.y < GetLineCount()) &&
		       (point.x >= 0 && point.x <= GetLineLength(point.y));
	}
	return FALSE;
}

BOOL CLaTeXEdit::RestoreFocus()
{
	SetFocus();
	return TRUE;
}

void CLaTeXEdit::InstantAdvice()
{
	CString keyw,key;
	CPoint ptStart(GetCursorPos());
	CPoint ptClient;

	GetWordBeforeCursor(keyw,ptStart,FALSE);

	if (keyw.GetLength() > MINIMUM_KEYWORD_LENGTH)
	{
		CMapStringToOb map;

		theApp.m_AvailableCommands.GetAllPossibleItems(keyw,_T(""),map);
		if (map.GetCount() == 1) //keyword is unique -> OK
		{
			CLaTeXCommand *lc;
			POSITION pos = map.GetStartPosition();
			map.GetNextAssoc(pos,key,(CObject*&)lc);

			if (lc != NULL)
			{
				if (m_InstTip == NULL)
				{
					m_InstTip = new CAdvice();
					m_InstTip->Create(lc->ToLaTeX(),WS_POPUP | WS_BORDER/*SS_SUNKEN*/,CRect(),this);
				}

				if (!m_InstTip->IsWindowVisible())
				{
					// Compute window size
					ptClient = TextToClient(ptStart);
					::ClientToScreen(GetSafeHwnd(),&ptClient);
					// Determine if there is space enough to show the window below the text
					ptClient.y += GetLineHeight();
					// Place and show the window
					m_InstTip->SetWindowPos(NULL,ptClient.x,ptClient.y,0,0,SWP_NOSIZE | SWP_NOZORDER);
					m_InstTip->SetWindowText(lc->GetExpandBefore() + lc->ToLaTeX() + lc->GetExpandAfter());
					m_InstTip->ShowWindow(SW_SHOWNA);
					m_InstTip->SetTimer(1,5000,0);
					SetFocus();
				}
			}
			else
			{
				HideAdvice();
			}
		}
		else
		{
			// Nothing found: Hide window
			HideAdvice();
		}
	}
}

void CLaTeXEdit::HideAdvice()
{
	if (m_InstTip != NULL)
	{
		m_InstTip->ShowWindow(SW_HIDE);
	}
}

void CLaTeXEdit::OnKillFocus(CWnd* pNewWnd)
{
	CCrystalEditViewEx::OnKillFocus(pNewWnd);
}

void CLaTeXEdit::DestroyListBox()
{
	if (m_CompletionListBox != NULL && ::IsWindow(m_CompletionListBox->GetSafeHwnd()))
	{
		m_CompletionListBox->ShowWindow(SW_HIDE);
	}
}

/* Returns the number of possible matches */
int CLaTeXEdit::GetNumberOfMatches(CString keyword)
{
	if (keyword.GetLength() < CAutoCompleteDlg::GetMinimumKeywordLength()) return 0;

	CMapStringToOb map;
	theApp.m_AvailableCommands.GetAllPossibleItems(keyword,_T(""),map);
	return map.GetCount();
}

void CLaTeXEdit::OnTextOutsource()
{
	//Create the dialog
	CTextOutsourceDlg OutsourceDlg;

	//Get the file name of ourselves
	CLaTeXDoc* pDoc = GetDocument();

	if (!pDoc) return;

	CPathTool OldPath = pDoc->GetPathName();

	//Suggest a directory for the new file
	OutsourceDlg.m_Directory = OldPath.GetDirectory();

	//Show the dialog
	SetShowInteractiveSelection(true);

	if (OutsourceDlg.DoModal() == IDCANCEL)
	{
		SetShowInteractiveSelection(false);
		return;
	}

	SetShowInteractiveSelection(false);

	//Get the new file name and write the selected lines to it
	CFile NewFile;
	if (NewFile.Open(OutsourceDlg.NewPath.GetPath(),CFile::modeWrite | CFile::modeCreate))
	{
		//Get selected text - may be empty
		CPoint ptStart,ptEnd;
		GetSelection(ptStart,ptEnd);
		CString strSelectedText("");
		if (ptStart != ptEnd) GetText(ptStart,ptEnd,strSelectedText);

		//Write it to the file
		NewFile.Write((LPCTSTR)strSelectedText,strSelectedText.GetLength());
		NewFile.Close();

		//Get relative path - omit tex-extension
		CString RelativeFilePath = CPathTool::GetRelativePath(OldPath.GetDirectory(),
		                           (OutsourceDlg.NewPath.GetFileExtension() == "tex")
		                           ? CPathTool(OutsourceDlg.NewPath.GetBase())
		                           : OutsourceDlg.NewPath);

		//Insert the text into this document
		LocateTextBuffer()->BeginUndoGroup();
		ReplaceSelection(OutsourceDlg.CmdLeft + RelativeFilePath + OutsourceDlg.CmdRight);
		LocateTextBuffer()->FlushUndoGroup(this);

		//Open the new file
		if (OutsourceDlg.m_bOpenNewFile)
		{
			//Open it
			theApp.OpenLatexDocument(OutsourceDlg.NewPath,FALSE,-1,FALSE,false);

			//In background? Foreground is automatically done by the line above.
			if (OutsourceDlg.m_nOpenNewFileType == 0 /*background*/)
			{
				//Re-activate this view (I tried to open the new doc in background, but it did not work out)
				CFrameWnd* pFrame = GetParentFrame();
				if (pFrame) pFrame->ActivateFrame();
			}
		}
	}
}

void CLaTeXEdit::OnNcPaint()
{
}

void CLaTeXEdit::OnDestroy()
{
	CCrystalEditViewEx::OnDestroy();
}

void CLaTeXEdit::OnFormatTextBackColor()
{
	// TODO: Add your command handler code here
}

void CLaTeXEdit::OnFormatTextForeColor()
{
	// TODO: Add your command handler code here
}

void CLaTeXEdit::OnEditDeleteLine()
{
	if (m_pTextBuffer)
		m_pTextBuffer->DeleteLine(this,GetCursorPos().y);
}
