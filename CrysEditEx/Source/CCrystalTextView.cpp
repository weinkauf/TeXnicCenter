////////////////////////////////////////////////////////////////////////////
//	File:		CCrystalTextView.cpp
//	Version:	1.0.0.0
//	Created:	29-Dec-1998
//
//	Author:		Stcherbatchenko Andrei
//	E-mail:		windfall@gmx.de
//
//	Implementation of the CCrystalTextView class, a part of Crystal Edit -
//	syntax coloring text editor.
//
//	You are free to use or modify this code to the following restrictions:
//	- Acknowledge me somewhere in your about box, simple "Parts of code by.."
//	will be enough. If you can't (or don't want to), contact me personally.
//	- LEAVE THIS HEADER INTACT
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
//	17-Feb-99
//	FIX:	missing UpdateCaret() in CCrystalTextView::SetFont
//	FIX:	missing UpdateCaret() in CCrystalTextView::RecalcVertScrollBar
//	FIX:	mistype in CCrystalTextView::RecalcPageLayouts + instead of +=
//	FIX:	removed condition 'm_nLineHeight < 20' in
//		CCrystalTextView::CalcLineCharDim(). This caused painting defects
//		when using very small fonts.
//
//	FEATURE:	Some experiments with smooth scrolling, controlled by
//		m_bSmoothScroll member variable, by default turned off.
//		See ScrollToLine function for implementation details.
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
//	21-Feb-99
//		Paul Selormey, James R. Twine
//	+	FEATURE: description for Undo/Redo actions
//	+	FEATURE: multiple MSVC-like bookmarks
//	+	FEATURE: 'Disable backspace at beginning of line' option
//	+	FEATURE: 'Disable drag-n-drop editing' option
//
//	+	FIX:  ResetView() now virtual
//	+	FEATURE: Added OnEditOperation() virtual: base for auto-indent,
//		smart indent etc.
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
//	01-Jun-99 to 31-Aug-99
//		Sven Wiegand (search for "//BEGIN SW" to find my changes):
//
//	+ FEATURE: support for language switching on the fly with class 
//			CCrystalParser
//	+	FEATURE: word wrapping
//	+ FIX:	Setting m_nIdealCharPos, when choosing cursor position by mouse
//	+ FIX:	Backward search
//	+ FEATURE: incremental search
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
//	24-Oct-99
//		Sven Wiegand
//
//	+ FIX: Opening large files won't crash anymore and will go very fast
//	       (removed call to RecalcVertScrollBar() in WrapLineCached())
//	+ FIX: Problems with repainting and cursor-position by resizing window 
//	       fixed by adding call to ScrollToSubLine() in OnSize().
//	+ FEATURE: Supporting [Return] to exit incremental-search-mode
//		     (see OnChar())
///////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
//	26-Apr-2000
//		Sven Wiegand
//	+	FIX: Setting selection anchor (m_ptAnchor) when a word is selected by
//				 double clicking. (BEGIN SW(selection))
////////////////////////////////////////////////////////////////////////////

/********************************************************************
*
* $RCSfile$
* $Revision$
* $Date$
*
* $Author$
*
* $Log$
* Revision 1.27  2005/06/17 21:38:44  owieland
* Show line numbers in editor (FR 1178333 )
*
* Revision 1.26  2005/03/16 18:17:58  vachis
* fixed bugs: unmatched brace (after Ctrl+M) disappear when scrolling
*                   disable higlighting when a selection is pressent
*                   memory violation when  Ctrl+M is pressed in an emtpy document
*                   bug in brace block selection
*
* Revision 1.25  2005/03/16 15:20:45  vachis
* fixed redraw bug: unmatched brace is not shown in red if it is in visible window rectangle
*
* Revision 1.24  2005/03/12 11:46:52  niteria
* Enabled bracket matching even if colors are the same as the normal colors.
* Now we have a consistent matching even in formulas.
*
* Revision 1.23  2005/03/08 16:36:08  vachis
* Selection of block between bracket pairs in a hierarchical manner (Ctrl+M)
* Bracket highlighting disabled if a selection is present
* New default colours for bracket highlighting
*
* Revision 1.22  2005/02/23 23:58:19  vachis
* Fixed Matching Bracket Highlighting
* New features for Matching Bracket Highlighting: configure colours,
*     highlight current block and pair, help
*
* Revision 1.21  2005/02/15 09:43:12  vachis
* Implemented feature 536164: Matching Bracket Highlight
*
* Revision 1.20  2005/01/04 07:57:11  niteria
* Implemented function to stop incremental search from the outside.
*
* Revision 1.19  2003/12/16 20:04:38  svenwiegand
* Implemented Feature 726766: "Option for selecting the language for the GUI"
*
* Revision 1.18  2003/12/06 19:59:58  svenwiegand
* - Implemented Feature 601708 + additions: The user can now set the styles for
*   the text cursor independant for the insert and the overwrite mode. The cursor
*   style is defined by its form (line or block) and its mode (blinking or
*   not-blinking). The defaults are 'line' for insert cursor and 'block' for
*   overwrite cursor -- both are blinking.
*
* Revision 1.17  2003/04/30 14:20:33  christianwelzel
* Fixed Bug 602329: File>Print and HP LaserJet 4050
*
* Revision 1.16  2002/06/27 14:43:26  svenwiegand
* Instead of the character test functions (isalpha, isdigit, etc.) from the standard library, our own wrappers from the character.h file (myspell directory) are used now, to fix character recognition problems (bug 551033).
*
* Revision 1.15  2002/06/19 16:04:48  svenwiegand
* Fixed hanging problem that occured on my fast XP machine, when closing a document.
*
* Problem was simply, that the lines Hold() and PostThreadMessage() in OnDestroy() where in the wrong order: On fast systems the background thread already processed the message, before the following Hold() was called and so the hold counter won't become zero which will make block the following Hold(true) call infinite.
*
* Revision 1.14  2002/05/30 02:04:54  niteria
* GetSelectedText behaves now similar to double-click
*
* Revision 1.13  2002/05/06 05:35:42  cnorris
* Protect background thread from document reset
*
* Revision 1.12  2002/04/29 19:45:17  cnorris
* Make text attribute structure thread safe
*
* Revision 1.11  2002/04/27 07:21:59  cnorris
* Avoid ambiguous function under Visual Studio < 5
*
* Revision 1.10  2002/04/25 18:53:53  cnorris
* background spell check support
*
* Revision 1.9  2002/04/24 00:32:39  niteria
* Fixed Bug 84 (old list): incremental search cancels using
* accentuated characters or german umlauts.
*
* Revision 1.8  2002/04/23 21:45:09  cnorris
* realtime spell check
*
* Revision 1.7  2002/04/06 05:28:35  cnorris
* Added SetShowInteractiveSelection
* Added GetParser
* Added default value to nLength of HighlightText
*
* Revision 1.6  2002/04/03 00:42:31  niteria
* Bug 538164:Fixed now.
*
* Revision 1.5  2002/04/02 03:45:21  niteria
* Bug 538164: Workaround / fix in ResetView. Search for BEGIN niteria
*
* Revision 1.4  2002/03/20 00:48:11  cnorris
* Mouse scroll wheel handler
*
* Revision 1.3  2002/03/20 00:15:29  cnorris
* Code clean up and new / delete code inspection
*
* Revision 1.2  2002/03/05 17:23:32  cnorris
* bug id 70 Selecting text immediately after it is inserted from toolbar now works
*
* Revision 1.1.1.1  2002/02/26 08:11:57  svenwiegand
* Initial revision
*
* Revision 1.0  2000-05-31 21:55:25+02  sven_wiegand
* Initial revision
*
*
********************************************************************/

#include "stdafx.h"
#include <malloc.h>
#include <math.h>
#include "editcmd.h"
#include "editreg.h"
#include "CCrystalTextView.h"
#include "CCrystalTextBuffer.h"
#include "CFindTextDlg.h"
#include "../../MySpell/Character.h"

#ifndef __AFXPRIV_H__
#pragma message("Include <afxpriv.h> in your stdafx.h to avoid this message")
#include <afxpriv.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TAB_CHARACTER				_T('\xBB')
#define SPACE_CHARACTER				_T('\x95')
#define DEFAULT_PRINT_MARGIN		1000				//	10 millimeters

#define SMOOTH_SCROLL_FACTOR		6


////////////////////////////////////////////////////////////////////////////
// CCrystalTextView

IMPLEMENT_DYNCREATE(CCrystalTextView, CView)

HINSTANCE CCrystalTextView::s_hResourceInst = NULL;

int CCrystalTextView::s_nCaretInsertForm = CARET_LINE;
int CCrystalTextView::s_nCaretInsertMode = CARET_BLINK;
int CCrystalTextView::s_nCaretOverwriteForm = CARET_BLOCK;
int CCrystalTextView::s_nCaretOverwriteMode = CARET_BLINK;
int CCrystalTextView::m_bShowLineNumbers = FALSE;

BEGIN_MESSAGE_MAP(CCrystalTextView, CView)
	//{{AFX_MSG_MAP(CCrystalTextView)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETFOCUS()
	ON_WM_HSCROLL()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateEditSelectAll)
	ON_WM_RBUTTONDOWN()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_CREATE()
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	ON_COMMAND(ID_EDIT_REPEAT, OnEditRepeat)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPEAT, OnUpdateEditRepeat)
	ON_COMMAND(ID_EDIT_FIND_PREVIOUS, OnEditFindPrevious)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND_PREVIOUS, OnUpdateEditFindPrevious)
	ON_WM_CHAR()
	ON_COMMAND(ID_EDIT_DELETE_BACK, OnEditDeleteBack)
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_EDIT_CHAR_LEFT, OnCharLeft)
	ON_COMMAND(ID_EDIT_EXT_CHAR_LEFT, OnExtCharLeft)
	ON_COMMAND(ID_EDIT_CHAR_RIGHT, OnCharRight)
	ON_COMMAND(ID_EDIT_EXT_CHAR_RIGHT, OnExtCharRight)
	ON_COMMAND(ID_EDIT_WORD_LEFT, OnWordLeft)
	ON_COMMAND(ID_EDIT_EXT_WORD_LEFT, OnExtWordLeft)
	ON_COMMAND(ID_EDIT_WORD_RIGHT, OnWordRight)
	ON_COMMAND(ID_EDIT_EXT_WORD_RIGHT, OnExtWordRight)
	ON_COMMAND(ID_EDIT_LINE_UP, OnLineUp)
	ON_COMMAND(ID_EDIT_EXT_LINE_UP, OnExtLineUp)
	ON_COMMAND(ID_EDIT_LINE_DOWN, OnLineDown)
	ON_COMMAND(ID_EDIT_EXT_LINE_DOWN, OnExtLineDown)
	ON_COMMAND(ID_EDIT_SCROLL_UP, ScrollUp)
	ON_COMMAND(ID_EDIT_SCROLL_DOWN, ScrollDown)
	ON_COMMAND(ID_EDIT_PAGE_UP, OnPageUp)
	ON_COMMAND(ID_EDIT_EXT_PAGE_UP, OnExtPageUp)
	ON_COMMAND(ID_EDIT_PAGE_DOWN, OnPageDown)
	ON_COMMAND(ID_EDIT_EXT_PAGE_DOWN, OnExtPageDown)
	ON_COMMAND(ID_EDIT_LINE_END, OnLineEnd)
	ON_COMMAND(ID_EDIT_EXT_LINE_END, OnExtLineEnd)
	ON_COMMAND(ID_EDIT_HOME, OnHome)
	ON_COMMAND(ID_EDIT_EXT_HOME, OnExtHome)
	ON_COMMAND(ID_EDIT_TEXT_BEGIN, OnTextBegin)
	ON_COMMAND(ID_EDIT_EXT_TEXT_BEGIN, OnExtTextBegin)
	ON_COMMAND(ID_EDIT_TEXT_END, OnTextEnd)
	ON_COMMAND(ID_EDIT_EXT_TEXT_END, OnExtTextEnd)
	ON_COMMAND(ID_EDIT_SEL_BIGGER_BLOCK, OnSelBiggerBlock)
	//	Standard printing commands
	ON_COMMAND(ID_FILE_PAGE_SETUP, OnFilePageSetup)
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	//	Status
	ON_UPDATE_COMMAND_UI(ID_EDIT_INDICATOR_CRLF, OnUpdateIndicatorCRLF)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INDICATOR_POSITION, OnUpdateIndicatorPosition)
	//	Bookmarks
	ON_COMMAND_RANGE(ID_EDIT_TOGGLE_BOOKMARK0, ID_EDIT_TOGGLE_BOOKMARK9, OnToggleBookmark)
	ON_COMMAND_RANGE(ID_EDIT_GO_BOOKMARK0, ID_EDIT_GO_BOOKMARK9, OnGoBookmark)
	ON_COMMAND(ID_EDIT_CLEAR_BOOKMARKS, OnClearBookmarks)
	// More Bookmarks
	ON_COMMAND(ID_EDIT_TOGGLE_BOOKMARK,     OnToggleBookmark)
	ON_COMMAND(ID_EDIT_GOTO_NEXT_BOOKMARK,  OnNextBookmark)
	ON_COMMAND(ID_EDIT_GOTO_PREV_BOOKMARK,  OnPrevBookmark)
	ON_COMMAND(ID_EDIT_CLEAR_ALL_BOOKMARKS, OnClearAllBookmarks)
	ON_UPDATE_COMMAND_UI(ID_EDIT_GOTO_NEXT_BOOKMARK,  OnUpdateNextBookmark)
	ON_UPDATE_COMMAND_UI(ID_EDIT_GOTO_PREV_BOOKMARK,  OnUpdatePrevBookmark)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR_ALL_BOOKMARKS, OnUpdateClearAllBookmarks)
	//BEGIN SW
	// incremental search
	ON_COMMAND(ID_EDIT_FIND_INCREMENTAL_FORWARD, OnEditFindIncrementalForward)
	ON_COMMAND(ID_EDIT_FIND_INCREMENTAL_BACKWARD, OnEditFindIncrementalBackward)
	//END SW
END_MESSAGE_MAP()

#define EXPAND_PRIMITIVE(impl, func)	\
	void CCrystalTextView::On##func() { impl(FALSE); }	\
	void CCrystalTextView::OnExt##func() { impl(TRUE); }
	EXPAND_PRIMITIVE(MoveLeft, CharLeft)
	EXPAND_PRIMITIVE(MoveRight, CharRight)
	EXPAND_PRIMITIVE(MoveWordLeft, WordLeft)
	EXPAND_PRIMITIVE(MoveWordRight, WordRight)
	EXPAND_PRIMITIVE(MoveUp, LineUp)
	EXPAND_PRIMITIVE(MoveDown, LineDown)
	EXPAND_PRIMITIVE(MovePgUp, PageUp)
	EXPAND_PRIMITIVE(MovePgDn, PageDown)
	EXPAND_PRIMITIVE(MoveHome, Home)
	EXPAND_PRIMITIVE(MoveEnd, LineEnd)
	EXPAND_PRIMITIVE(MoveCtrlHome, TextBegin)
	EXPAND_PRIMITIVE(MoveCtrlEnd, TextEnd)
#undef EXPAND_PRIMITIVE

/////////////////////////////////////////////////////////////////////////////
// CCrystalTextView construction/destruction

CCrystalTextView::CCrystalTextView()
{
	AFX_ZERO_INIT_OBJECT(CView);
	::InitializeCriticalSection( &m_csHold );
	m_pevtHoldCountZero = new CEvent(TRUE, FALSE);
	m_bSelMargin = TRUE;
	m_bShowLineNumbers = TRUE;
	//BEGIN SW
	m_panSubLines = new CArray<int, int>();
	m_nSubLineCount = -1;
	ASSERT( m_panSubLines );
	m_panSubLines->SetSize( 0, 4096 );

	m_pstrIncrementalSearchString = new CString;
	ASSERT( m_pstrIncrementalSearchString );
	m_pstrIncrementalSearchStringOld = new CString;
	ASSERT( m_pstrIncrementalSearchStringOld );
	//END SW
	ResetView();
}

CCrystalTextView::~CCrystalTextView()
{
	ASSERT( m_nHoldCount == 0 );
	::DeleteCriticalSection( &m_csHold );
	delete m_pevtHoldCountZero;
	ASSERT(m_hAccel == NULL);
	ASSERT(m_pCacheBitmap == NULL);
	ASSERT(m_pTextBuffer == NULL);		//	Must be correctly detached
	if (m_pszLastFindWhat != NULL)
		free(m_pszLastFindWhat);
	delete [] m_pdwParseCookies;
	delete [] m_pnActualLineLength;
	//BEGIN SW
	delete m_panSubLines;
	delete m_pstrIncrementalSearchString;
	delete m_pstrIncrementalSearchStringOld;
	//END SW
}

BOOL CCrystalTextView::PreCreateWindow(CREATESTRUCT& cs)
{
	CWnd		*pParentWnd = CWnd::FromHandlePermanent(cs.hwndParent);
	if (pParentWnd == NULL || ! pParentWnd->IsKindOf(RUNTIME_CLASS(CSplitterWnd)))
	{
		//	View must always create its own scrollbars,
		//	if only it's not used within splitter
		//BEGIN SW
		if( m_bWordWrap )
			// we do not need a horizontal scroll bar, if we wrap the lines
			cs.style|= WS_VSCROLL;
		else
			cs.style |= (WS_HSCROLL | WS_VSCROLL);
		//END SW
	}
	cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS);
	return CView::PreCreateWindow(cs);
}


/////////////////////////////////////////////////////////////////////////////
// CCrystalTextView drawing

void CCrystalTextView::GetSelection(CPoint &ptStart, CPoint &ptEnd)
{
	PrepareSelBounds();
	ptStart = m_ptDrawSelStart;
	ptEnd = m_ptDrawSelEnd;
}

CCrystalTextBuffer *CCrystalTextView::LocateTextBuffer()
{
	return NULL;
}

int CCrystalTextView::GetLineActualLength(int nLineIndex)
{
	int nLineCount = GetLineCount();
	ASSERT(nLineCount > 0);
	ASSERT(nLineIndex >= 0 && nLineIndex < nLineCount);
	if (m_pnActualLineLength == NULL)
	{
		m_pnActualLineLength = new int[nLineCount];
		memset(m_pnActualLineLength, 0xff, sizeof(int) * nLineCount);
		m_nActualLengthArraySize = nLineCount;
	}

	if (m_pnActualLineLength[nLineIndex] >= 0)
		return m_pnActualLineLength[nLineIndex];

	//	Actual line length is not determined yet, let's calculate a little
	int nActualLength = 0;
	int nLength = GetLineLength(nLineIndex);
	if (nLength > 0)
	{
		LPCTSTR	pszLine = GetLineChars(nLineIndex);
		LPTSTR	pszChars = (LPTSTR) _alloca(sizeof(TCHAR) * (nLength + 1));
		memcpy(pszChars, pszLine, sizeof(TCHAR) * nLength);
		pszChars[nLength] = 0;
		LPTSTR	pszCurrent = pszChars;

		int nTabSize = GetTabSize();
		for (;;)
		{
#ifdef _UNICODE
			LPTSTR	psz = wcschr(pszCurrent, L'\t');
#else
			LPTSTR	psz = strchr(pszCurrent, '\t');
#endif
			if (psz == NULL)
			{
				nActualLength += (pszChars + nLength - pszCurrent);
				break;
			}

			nActualLength += (psz - pszCurrent);
			nActualLength += (nTabSize - nActualLength % nTabSize);
			pszCurrent = psz + 1;
		}
	}

	m_pnActualLineLength[nLineIndex] = nActualLength;
	return nActualLength;
}

void CCrystalTextView::ScrollToChar(int nNewOffsetChar, BOOL bNoSmoothScroll /*= FALSE*/,
									BOOL bTrackScrollBar /*= TRUE*/)
{
	//BEGIN SW
	// no horizontal scrolling, when word wrapping is enabled
	if( m_bWordWrap )
		return;
	//END SW

	//	For now, ignoring bNoSmoothScroll and m_bSmoothScroll
	if (m_nOffsetChar != nNewOffsetChar)
	{
		int nScrollChars = m_nOffsetChar - nNewOffsetChar;
		m_nOffsetChar = nNewOffsetChar;
		CRect rcScroll;
		GetClientRect(&rcScroll);
		rcScroll.left += GetMarginWidth();
		ScrollWindow(nScrollChars * GetCharWidth(), 0, &rcScroll, &rcScroll);
		UpdateWindow();
		if (bTrackScrollBar)
			RecalcHorzScrollBar(TRUE);
	}
}


//BEGIN SW
void CCrystalTextView::ScrollToSubLine(int nNewTopSubLine,  BOOL bNoSmoothScroll /*= FALSE*/,
									   BOOL bTrackScrollBar /*= TRUE*/ )
{
	if (m_nTopSubLine != nNewTopSubLine)
	{
		if (bNoSmoothScroll || ! m_bSmoothScroll)
		{
			int nScrollLines = m_nTopSubLine - nNewTopSubLine;
			m_nTopSubLine = nNewTopSubLine;
			ScrollWindow(0, nScrollLines * GetLineHeight());
			UpdateWindow();
			if (bTrackScrollBar)
				RecalcVertScrollBar(TRUE);
		}
		else
		{
			//	Do smooth scrolling
			int nLineHeight = GetLineHeight();
			if (m_nTopSubLine > nNewTopSubLine)
			{
				int nIncrement = (m_nTopSubLine - nNewTopSubLine) / SMOOTH_SCROLL_FACTOR + 1;
				while (m_nTopSubLine != nNewTopSubLine)
				{
					int nTopSubLine = m_nTopSubLine - nIncrement;
					if (nTopSubLine < nNewTopSubLine)
						nTopSubLine = nNewTopSubLine;
					int nScrollLines = nTopSubLine - m_nTopSubLine;
					m_nTopSubLine = nTopSubLine;
					ScrollWindow(0, - nLineHeight * nScrollLines);
					UpdateWindow();
					if (bTrackScrollBar)
						RecalcVertScrollBar(TRUE);
				}
			}
			else
			{
				int nIncrement = (nNewTopSubLine - m_nTopSubLine) / SMOOTH_SCROLL_FACTOR + 1;
				while (m_nTopSubLine != nNewTopSubLine)
				{
					int nTopSubLine = m_nTopSubLine + nIncrement;
					if (nTopSubLine > nNewTopSubLine)
						nTopSubLine = nNewTopSubLine;
					int nScrollLines = nTopSubLine - m_nTopSubLine;
					m_nTopSubLine = nTopSubLine;
					ScrollWindow(0, - nLineHeight * nScrollLines);
					UpdateWindow();
					if (bTrackScrollBar)
						RecalcVertScrollBar(TRUE);
				}
			}
		}
		int nDummy;
		GetLineBySubLine( m_nTopSubLine, m_nTopLine, nDummy );
		InvalidateRect( NULL );	// repaint whole window
	}
}

void CCrystalTextView::GoToLine( int nLine )
{
	if (nLine < 0)
		nLine = 0;
	if (nLine >= GetLineCount())
		nLine = GetLineCount()-1;

	if (nLine == -1)
		return;

	SetCursorPos(CPoint(0, nLine));
	EnsureVisible(GetCursorPos());
}


void CCrystalTextView::EnsureValidTextPos(CPoint &ptTextPos)
{
	if (ptTextPos.y < 0)
		ptTextPos.y = 0;
	else if (ptTextPos.y >= GetLineCount())
		ptTextPos.y = GetLineCount()-1;

	if (ptTextPos.x < 0)
		ptTextPos = 0;
	else if (ptTextPos.x >= GetLineLength(ptTextPos.y))
		ptTextPos = GetLineLength(ptTextPos.y)-1;
}
//END SW


void CCrystalTextView::ScrollToLine(int nNewTopLine, BOOL bNoSmoothScroll /*= FALSE*/, 
									BOOL bTrackScrollBar /*= TRUE*/)
{
	//BEGIN SW
	if( m_nTopLine != nNewTopLine )
		ScrollToSubLine( GetSubLineIndex( nNewTopLine ), bNoSmoothScroll, bTrackScrollBar );
	//END SW
}


int CCrystalTextView::ExpandChars(LPCTSTR pszChars, int &nOffset, int nCount, int *anIndices)
{
	if ( pszChars == NULL )
		return 0;

	int nInputOffset = nOffset; // offset within unexpanded string
	int nScreenOffset = 0; // offset of screen includes tab expansion
	const int nTabSize = GetTabSize();

	while ( pszChars[nInputOffset] != _T('\0') && nInputOffset < nCount )
	{
		if ( pszChars[nInputOffset] == _T('\t') )
		{
			int nSpaces = ( nTabSize - nScreenOffset % nTabSize );
			if ( m_bViewTabs )
			{
				anIndices[nScreenOffset++] = nInputOffset;
				--nSpaces;
			}
			while ( nSpaces > 0 )
			{
				anIndices[nScreenOffset++] = nInputOffset;
				--nSpaces;
			}
			nInputOffset++;
		}
		else
		{
			anIndices[nScreenOffset++] = nInputOffset++;
		}
	}
	anIndices[nScreenOffset] = nInputOffset;
	nOffset = nInputOffset;
	return nScreenOffset;
}


void CCrystalTextView::ExpandChars(LPCTSTR pszChars, int nOffset, int nCount, CString &line)
{
	if (nCount <= 0)
	{
		line = _T("");
		return;
	}

	int nTabSize = GetTabSize();

	int nActualOffset = 0;
	for (int I = 0; I < nOffset; I ++)
	{
		if (pszChars[I] == _T('\t'))
			nActualOffset += (nTabSize - nActualOffset % nTabSize);
		else
			nActualOffset ++;
	}

	pszChars += nOffset;
	int nLength = nCount;

	int nTabCount = 0;
	for (I = 0; I < nLength; I ++)
	{
		if (pszChars[I] == _T('\t'))
			nTabCount ++;
	}

	LPTSTR pszBuf = line.GetBuffer(nLength + nTabCount * (nTabSize - 1) + 1);
	int nCurPos = 0;
	if (nTabCount > 0 || m_bViewTabs)
	{
		for (I = 0; I < nLength; I ++)
		{
			if (pszChars[I] == _T('\t'))
			{
				int nSpaces = nTabSize - (nActualOffset + nCurPos) % nTabSize;
				if (m_bViewTabs)
				{
					pszBuf[nCurPos ++] = TAB_CHARACTER;
					nSpaces --;
				}
				while (nSpaces > 0)
				{
					pszBuf[nCurPos ++] = _T(' ');
					nSpaces --;
				}
			}
			else
			{
				if (pszChars[I] == _T(' ') && m_bViewTabs)
					pszBuf[nCurPos] = SPACE_CHARACTER;
				else
					pszBuf[nCurPos] = pszChars[I];
				nCurPos ++;
			}
		}
	}
	else
	{
		memcpy(pszBuf, pszChars, sizeof(TCHAR) * nLength);
		nCurPos = nLength;
	}
	pszBuf[nCurPos] = 0;
	line.ReleaseBuffer();
}


void CCrystalTextView::DrawLineAttributes(CDC *pdc, CPoint ptOrigin, int nLineIndex, int *anBreaks, int nBreaks)
{
	// Draw line attributes
	LPCTSTR pszLine = GetLineChars(nLineIndex);
	CCrystalTextBuffer *pBuffer = LocateTextBuffer();
	CCrystalTextBuffer::TextAttributeListType *pList = pBuffer->GetLineAttributes(nLineIndex);
	if (pList != NULL)
	{
		IMAGELISTDRAWPARAMS drawing;
		drawing.hdcDst = pdc->m_hDC;
		drawing.rgbBk = CLR_DEFAULT;
		drawing.rgbFg = CLR_DEFAULT;
		drawing.fStyle = ILD_NORMAL;
		drawing.dwRop = SRCCOPY;

		int *anIndices = (int*) _alloca(sizeof(int) * (GetScreenChars()+1));
		int nOffset = 0;
		int nSubline = 0;
		int nLength = ExpandChars(pszLine, nOffset, anBreaks[nSubline], anIndices);
		int nStartLine = 0;
		boolean bSplitAttribute = false;

		CCrystalTextBuffer::CTextAttribute attr;
		POSITION pos = pList->GetHeadPosition();
		while (nLength > 0)
		{
			if ( !bSplitAttribute )
			{
				// Need a new attribute
				if (pos != NULL)
					attr = pList->GetNext(pos);
				else
					break; // done, no more attributes
			}
			else
			{
				// Need a new line
				do 
				{
					nLength = ExpandChars(pszLine, nOffset, anBreaks[++nSubline], anIndices);
					nStartLine = 0;
				} while (nLength && attr.m_nStartPos > anIndices[nLength-1]);
				if (nLength == 0)
					break; // done, no more text
			}
			int nStartChar = __max(attr.m_nStartPos, anIndices[0]);
			int nEndChar = __min(attr.m_nEndPos, anIndices[nLength]);

			for (; anIndices[nStartLine] < nStartChar; ++nStartLine);
			for (int nEndLine = nStartLine; anIndices[nEndLine] < nEndChar; ++nEndLine );

			switch (attr.m_Attribute)
			{
				case CCrystalTextBuffer::CTextAttribute::spellError:
					int nPixelCount = m_nCharWidth * (nEndLine-nStartLine);
					drawing.i = CCrystalTextBuffer::CTextAttribute::spellError;
					drawing.y = ptOrigin.y +(nSubline * GetLineHeight()) - 3;
					drawing.x = ptOrigin.x + (nStartLine * m_nCharWidth);
					drawing.cx = 4;
					drawing.cy = 3;
					drawing.xBitmap = 0;
					drawing.yBitmap = 13;
					for (int x = 0; x < nPixelCount; x += drawing.cx)
					{
						m_TextAttributeImages.DrawIndirect( &drawing );
						drawing.x += drawing.cx;
					}
					break;
				// Add new attribute drawing code here
			}
			bSplitAttribute = (nEndChar < attr.m_nEndPos);
		}
	}
	pBuffer->ReleaseLineAttributes();
}


void CCrystalTextView::DrawLineHelperImpl(CDC *pdc, CPoint &ptOrigin, const CRect &rcClip,
										  LPCTSTR pszChars, int nOffset, int nCount)
{
	ASSERT(nCount >= 0);
	if (nCount > 0)
	{
		CString line;
		ExpandChars(pszChars, nOffset, nCount, line);
		int nWidth = rcClip.right - ptOrigin.x;
		if (nWidth > 0)
		{
			int nCharWidth = GetCharWidth();
			int nCount = line.GetLength();
			int nCountFit = nWidth / nCharWidth + 1;
			if (nCount > nCountFit)
				nCount = nCountFit;
			VERIFY(pdc->ExtTextOut(ptOrigin.x, ptOrigin.y, ETO_CLIPPED, &rcClip, line, nCount, NULL));
		}
		ptOrigin.x += GetCharWidth() * line.GetLength();
	}
}


void CCrystalTextView::DrawLineHelper(CDC *pdc, CPoint &ptOrigin, const CRect &rcClip, 
									  int nColorIndex, LPCTSTR pszChars, int nOffset,
									  int nCount, CPoint ptTextPos)
{
	CCrystalTextView::DrawLineHelperRecur(pdc, ptOrigin, rcClip, nColorIndex, pszChars, 
										nOffset, nCount, ptTextPos, CCrystalTextView::DRAW_SELECTION );
}

void CCrystalTextView::DrawLineHelperRecur(CDC *pdc, CPoint &ptOrigin, const CRect &rcClip, 
									  int nColorIndex, LPCTSTR pszChars, int nOffset,
									  int nCount, CPoint ptTextPos, int nDrawStage)
{
	//prepare
	CPoint rptDrawSelStart,rptDrawSelEnd;
	BOOL bCondition = 0;
	int nNextDrawStage = 0;
	int nDrawIndexBkgnd = 0, nDrawIndexText = 0;

	ASSERT ( m_ptPairStringStart.y < m_ptPairStringEnd.y || (m_ptPairStringStart.y == m_ptPairStringEnd.y && (m_ptPairStringStart.x <= m_ptPairStringEnd.x)) );
	ASSERT ( m_ptCurStringStart.y < m_ptCurStringEnd.y || (m_ptCurStringStart.y == m_ptCurStringEnd.y && (m_ptCurStringStart.x <= m_ptCurStringEnd.x)) );

	if (nDrawStage == CCrystalTextView::DRAW_SELECTION) 
	{
		rptDrawSelStart = m_ptDrawSelStart;
		rptDrawSelEnd = m_ptDrawSelEnd;
		bCondition = m_bFocused || m_bShowInactiveSelection;
		nNextDrawStage = CCrystalTextView::DRAW_PAIR;
		nDrawIndexBkgnd = COLORINDEX_SELBKGND;
		nDrawIndexText = COLORINDEX_SELTEXT;
	}
	else if (nDrawStage == CCrystalTextView::DRAW_PAIR) 
	{
		rptDrawSelStart = m_ptPairStringStart;
		rptDrawSelEnd = m_ptPairStringEnd;
		nNextDrawStage = CCrystalTextView::DRAW_CURPAIR;
		nDrawIndexBkgnd = m_nPairStringColorBkgnd;
		nDrawIndexText = m_nPairStringColorText;
		//Draw only, if no selection is present.
		bCondition = (rptDrawSelEnd != rptDrawSelStart); 
//                        && GetColor(nDrawIndexBkgnd) !=  GetColor(CCrystalTextView::COLORINDEX_BKGND)
//                        && GetColor(nDrawIndexText) !=  GetColor(CCrystalTextView::COLORINDEX_NORMALTEXT);
//			//set same colour as block
//		if ( GetColor(nDrawIndexBkgnd) == GetColor(CCrystalTextView::COLORINDEX_PAIRBLOCKBKGND) ) {
//			nDrawIndexText = CCrystalTextView::COLORINDEX_NORMALTEXT; //TODO to leave text intact
//			bCondition = bCondition && m_bDrawPairBlock;
//		}
	}
	else if (nDrawStage == CCrystalTextView::DRAW_CURPAIR) 
	{
		rptDrawSelStart = m_ptCurStringStart;
		rptDrawSelEnd = m_ptCurStringEnd;
		nNextDrawStage = CCrystalTextView::DRAW_PAIRBLOCK;
		nDrawIndexBkgnd = m_nCurStringColorBkgnd;
		nDrawIndexText = m_nCurStringColorText;
		//Draw only, if no selection is present.
		bCondition = (rptDrawSelEnd != rptDrawSelStart);
//                        && GetColor(nDrawIndexBkgnd) !=  GetColor(CCrystalTextView::COLORINDEX_BKGND)
//                        && GetColor(nDrawIndexText) !=  GetColor(CCrystalTextView::COLORINDEX_NORMALTEXT);
//			//set same colour as block
//		if ( GetColor(nDrawIndexBkgnd) == GetColor(CCrystalTextView::COLORINDEX_PAIRBLOCKBKGND)) {
//			nDrawIndexText = CCrystalTextView::COLORINDEX_NORMALTEXT; //TODO to leave text intact?
//			bCondition = bCondition && m_bDrawPairBlock;
//		}
	}
	else if (nDrawStage == CCrystalTextView::DRAW_PAIRBLOCK) 
	{
		if ( m_ptPairStringEnd.y < m_ptCurStringStart.y 
				 || (m_ptPairStringEnd.y == m_ptCurStringStart.y && (m_ptPairStringStart.x < m_ptCurStringStart.x)) )
		{
			rptDrawSelStart = m_ptPairStringEnd;
			rptDrawSelEnd = m_ptCurStringStart;
		}
		else
		{
			rptDrawSelStart = m_ptCurStringEnd;
			rptDrawSelEnd = m_ptPairStringStart;
		}
		nNextDrawStage = CCrystalTextView::DRAW_IMPL;
		nDrawIndexBkgnd = CCrystalTextView::COLORINDEX_PAIRBLOCKBKGND;
		nDrawIndexText = CCrystalTextView::COLORINDEX_NORMALTEXT; //TODO to leave text intact - or new color for foreground of pair block
		//Draw only, if no selection is present.
		bCondition = (rptDrawSelEnd != rptDrawSelStart) && m_bDrawPairBlock;
//                        && GetColor(nDrawIndexBkgnd) != GetColor(CCrystalTextView::COLORINDEX_BKGND);
	}
	else if (nDrawStage == CCrystalTextView::DRAW_IMPL) 
	{
		DrawLineHelperImpl(pdc, ptOrigin, rcClip, pszChars, nOffset, nCount);
		return;
	}
	else 
	{
		ASSERT(CCrystalTextView::DRAW_IMPL != nDrawStage);
	}

	//draw stage
	if (nCount > 0)
	{
		if (bCondition)
		{
			int nSelBegin = 0, nSelEnd = 0;
			if (rptDrawSelStart.y > ptTextPos.y)
			{
				nSelBegin = nCount;
			}
			else
			if (rptDrawSelStart.y == ptTextPos.y)
			{
				nSelBegin = rptDrawSelStart.x - ptTextPos.x;
				if (nSelBegin < 0)
					nSelBegin = 0;
				if (nSelBegin > nCount)
					nSelBegin = nCount;
			}
			if (rptDrawSelEnd.y > ptTextPos.y)
			{
				nSelEnd = nCount;
			}
			else
			if (rptDrawSelEnd.y == ptTextPos.y)
			{
				nSelEnd = rptDrawSelEnd.x - ptTextPos.x;
				if (nSelEnd < 0)
					nSelEnd = 0;
				if (nSelEnd > nCount)
					nSelEnd = nCount;
			}

			ASSERT(nSelBegin >= 0 && nSelBegin <= nCount);
			ASSERT(nSelEnd >= 0 && nSelEnd <= nCount);
			ASSERT(nSelBegin <= nSelEnd);

			//	Draw part of the text before selection
			if (nSelBegin > 0)
			{
				DrawLineHelperRecur(pdc, ptOrigin, rcClip, nColorIndex, pszChars, 
														 nOffset, nSelBegin, ptTextPos, nNextDrawStage);
			}
			if (nSelBegin < nSelEnd)
			{
				CPoint ptTextPos2 = ptTextPos;
				ptTextPos2.x += nSelBegin;
				COLORREF crOldBk = pdc->SetBkColor(GetColor(nDrawIndexBkgnd));
				COLORREF crOldText = pdc->SetTextColor(GetColor(nDrawIndexText));
				DrawLineHelperRecur(pdc, ptOrigin, rcClip, nColorIndex, pszChars, 
													  nOffset + nSelBegin, nSelEnd - nSelBegin, ptTextPos2, nNextDrawStage);
				pdc->SetBkColor(crOldBk);
				pdc->SetTextColor(crOldText);
			}
			if (nSelEnd < nCount)
			{
				CPoint ptTextPos2 = ptTextPos;
				ptTextPos2.x += nSelEnd;
				DrawLineHelperRecur(pdc, ptOrigin, rcClip, nColorIndex, pszChars, 
														nOffset + nSelEnd, nCount - nSelEnd, ptTextPos2, nNextDrawStage);
			}
		}
		else
		{
			DrawLineHelperRecur(pdc, ptOrigin, rcClip, nColorIndex, pszChars, 
													 nOffset, nCount, ptTextPos, nNextDrawStage);
		}
	}
}

void CCrystalTextView::GetLineColors(int nLineIndex, COLORREF &crBkgnd, COLORREF &crText,
									 BOOL &bDrawWhitespace)
{
	DWORD dwLineFlags = GetLineFlags(nLineIndex);
	bDrawWhitespace = TRUE;
	crText = RGB(255, 255, 255);
	if (dwLineFlags & LF_EXECUTION)
	{
		crBkgnd = RGB(0, 128, 0);
		return;
	}
	if (dwLineFlags & LF_BREAKPOINT)
	{
		crBkgnd = RGB(255, 0, 0);
		return;
	}
	if (dwLineFlags & LF_INVALID_BREAKPOINT)
	{
		crBkgnd = RGB(128, 128, 0);
		return;
	}
	crBkgnd = CLR_NONE;
	crText = CLR_NONE;
	bDrawWhitespace = FALSE;
}

DWORD CCrystalTextView::GetParseCookie(int nLineIndex)
{
	int nLineCount = GetLineCount();
	if (m_pdwParseCookies == NULL)
	{
		m_nParseArraySize = nLineCount;
		m_pdwParseCookies = new DWORD[nLineCount];
		memset(m_pdwParseCookies, 0xff, nLineCount * sizeof(DWORD));
	}

	if (nLineIndex < 0)
		return 0;
	if (m_pdwParseCookies[nLineIndex] != (DWORD) -1)
		return m_pdwParseCookies[nLineIndex];

	int L = nLineIndex;
	while (L >= 0 && m_pdwParseCookies[L] == (DWORD) -1)
		L --;
	L ++;

	int nBlocks;
	while (L <= nLineIndex)
	{
		DWORD dwCookie = 0;
		if (L > 0)
			dwCookie = m_pdwParseCookies[L - 1];
		ASSERT(dwCookie != (DWORD) -1);
		m_pdwParseCookies[L] = ParseLine(dwCookie, L, NULL, nBlocks);
		ASSERT(m_pdwParseCookies[L] != (DWORD) -1);
		L ++;
	}

	return m_pdwParseCookies[nLineIndex];
}

//BEGIN SW
void CCrystalTextView::WrapLine( int nLineIndex, int nMaxLineWidth, int *anBreaks, int &nBreaks )
{
	// There must be a parser attached to this view
	if( !m_pParser )
		return;

	m_pParser->WrapLine( nLineIndex, nMaxLineWidth, anBreaks, nBreaks );
}


void CCrystalTextView::WrapLineCached( int nLineIndex, int nMaxLineWidth, int *anBreaks, int &nBreaks )
{
	// if word wrap is not active, there is not any break in the line
	if( !m_bWordWrap )
	{
		nBreaks = 0;
		return;
	}

	// word wrap is active
	if( nLineIndex < m_panSubLines->GetSize() && !anBreaks && (*m_panSubLines)[nLineIndex] > -1 )
		// return cached data
		nBreaks = (*m_panSubLines)[nLineIndex] - 1;
	else
	{
		// recompute line wrap
		nBreaks = 0;
		WrapLine( nLineIndex, nMaxLineWidth, anBreaks, nBreaks );

		// cache data
		ASSERT( nBreaks > -1 );
		m_panSubLines->SetAtGrow( nLineIndex, nBreaks + 1 );

		//RecalcVertScrollBar();
	}
}


void CCrystalTextView::InvalidateLineCache( int nLineIndex1, int nLineIndex2 /*= -1*/ )
{
	// invalidate cached sub line count
	m_nSubLineCount = -1;
	if( nLineIndex2 == -1 && nLineIndex1 < m_panSubLines->GetSize() )
		for( int i = nLineIndex1; i < m_panSubLines->GetSize(); i++ )
			(*m_panSubLines)[i] = -1;
	else
	{
		if( nLineIndex1 > nLineIndex2 )
		{
			int nStorage = nLineIndex1;
			nLineIndex1 = nLineIndex2;
			nLineIndex2 = nStorage;
		}

		if( nLineIndex1 >= m_panSubLines->GetSize() )
			return;

		if( nLineIndex2 >= m_panSubLines->GetSize() )
			nLineIndex2 = m_panSubLines->GetUpperBound();

		for( int i = nLineIndex1; i <= nLineIndex2; i++ )
			if( i >= 0 && i < m_panSubLines->GetSize() )
				(*m_panSubLines)[i] = -1;
	}
}


void CCrystalTextView::DrawScreenLine( CDC *pdc, CPoint &ptOrigin, const CRect &rcClip,
									  TEXTBLOCK *pBuf, int nBlocks, int &nActualItem, 
									  COLORREF crText, COLORREF crBkgnd, BOOL bDrawWhitespace,
									  LPCTSTR pszChars, int nOffset, int nCount, CPoint ptTextPos )
{
	CPoint	originalOrigin = ptOrigin;
	CRect	frect = rcClip;
	int		nLength = GetLineLength( ptTextPos.y );

	frect.top = ptOrigin.y;
	frect.bottom = frect.top + GetLineHeight();

	ASSERT( nActualItem < nBlocks );

	if (crText == CLR_NONE)
		pdc->SetTextColor(GetColor(pBuf[nActualItem].m_nColorIndex));
	pdc->SelectObject(
		GetFont(GetItalic(pBuf[nActualItem].m_nColorIndex), 
		GetBold(pBuf[nActualItem].m_nColorIndex)));

	if( nBlocks > 0 && nActualItem < nBlocks - 1 && 
		pBuf[nActualItem + 1].m_nCharPos >= nOffset && 
		pBuf[nActualItem + 1].m_nCharPos <= nOffset + nCount )
	{
		ASSERT(pBuf[nActualItem].m_nCharPos >= 0 && pBuf[nActualItem].m_nCharPos <= nLength);

		for (int I = nActualItem; I < nBlocks - 1 && pBuf[I + 1].m_nCharPos <= nOffset + nCount; I ++)
		{
			ASSERT(pBuf[I].m_nCharPos >= 0 && pBuf[I].m_nCharPos <= nLength);
			if (crText == CLR_NONE)
				pdc->SetTextColor(GetColor(pBuf[I].m_nColorIndex));

			pdc->SelectObject(GetFont(GetItalic(pBuf[I].m_nColorIndex), GetBold(pBuf[I].m_nColorIndex)));

			int nOffsetToUse = (nOffset > pBuf[I].m_nCharPos)? nOffset : pBuf[I].m_nCharPos;
			DrawLineHelper(pdc, ptOrigin, rcClip, pBuf[I].m_nColorIndex, pszChars,
				(nOffset > pBuf[I].m_nCharPos)? nOffset : pBuf[I].m_nCharPos, 
				pBuf[I + 1].m_nCharPos - nOffsetToUse,
				CPoint( nOffsetToUse, ptTextPos.y ));
		}

		nActualItem = I;

		ASSERT(pBuf[nActualItem].m_nCharPos >= 0 && pBuf[nActualItem].m_nCharPos <= nLength);
		if (crText == CLR_NONE)
			pdc->SetTextColor(GetColor(pBuf[nActualItem].m_nColorIndex));

		pdc->SelectObject(GetFont(GetItalic(pBuf[nActualItem].m_nColorIndex),
			GetBold(pBuf[nActualItem].m_nColorIndex)));
		DrawLineHelper(pdc, ptOrigin, rcClip, pBuf[nActualItem].m_nColorIndex, pszChars,
			pBuf[nActualItem].m_nCharPos, nOffset + nCount - pBuf[nActualItem].m_nCharPos,
			CPoint(pBuf[nActualItem].m_nCharPos, ptTextPos.y));
	}
	else
	{
		DrawLineHelper(
			pdc, ptOrigin, rcClip, pBuf[nActualItem].m_nColorIndex, 
			pszChars, nOffset, nCount, ptTextPos);
	}

	// Draw space on the right of the text
	if (ptOrigin.x > frect.left)
		frect.left = ptOrigin.x;
	if (frect.right > frect.left)
	{
		if ((m_bFocused || m_bShowInactiveSelection) 
			&& IsInsideSelBlock(CPoint(GetLineLength( ptTextPos.y ), ptTextPos.y)) 
			&& (nOffset + nCount) == GetLineLength( ptTextPos.y ) )
		{
			pdc->FillSolidRect(frect.left, frect.top, GetCharWidth(), frect.Height(),
				GetColor(COLORINDEX_SELBKGND));
			frect.left += GetCharWidth();
		}
		if (frect.right > frect.left)
			pdc->FillSolidRect(frect, bDrawWhitespace ? crBkgnd : GetColor(COLORINDEX_WHITESPACE));
	}

	// set origin to beginning of next screen line
	ptOrigin.x = originalOrigin.x;
	ptOrigin.y+= GetLineHeight();
}
//END SW

void CCrystalTextView::DrawSingleLine(CDC *pdc, const CRect &rc, int nLineIndex)
{
	ASSERT(nLineIndex >= -1 && nLineIndex < GetLineCount());

	if (nLineIndex == -1)
	{
		//	Draw line beyond the text
		pdc->FillSolidRect(rc, GetColor(COLORINDEX_WHITESPACE));
		return;
	}

	//	Acquire the background color for the current line
	BOOL bDrawWhitespace = FALSE;
	COLORREF crBkgnd, crText;
	GetLineColors(nLineIndex, crBkgnd, crText, bDrawWhitespace);
	if (crBkgnd == CLR_NONE)
		crBkgnd = GetColor(COLORINDEX_BKGND);

	int nLength = GetLineLength(nLineIndex);
	if (nLength == 0)
	{
		//	Draw the empty line
		CRect rect = rc;
		if ((m_bFocused || m_bShowInactiveSelection) && IsInsideSelBlock(CPoint(0, nLineIndex)))
		{
			pdc->FillSolidRect(rect.left, rect.top, GetCharWidth(), rect.Height(), GetColor(COLORINDEX_SELBKGND));
			rect.left += GetCharWidth();
		}
		pdc->FillSolidRect(rect, bDrawWhitespace ? crBkgnd : GetColor(COLORINDEX_WHITESPACE));
		return;
	}

	//	Parse the line
	LPCTSTR pszChars = GetLineChars(nLineIndex);
	DWORD dwCookie = GetParseCookie(nLineIndex - 1);
	TEXTBLOCK *pBuf = (TEXTBLOCK *) _alloca(sizeof(TEXTBLOCK) * nLength * 3);
	int nBlocks = 0;
	//BEGIN SW
	// insert at least one textblock of normal color at the beginning
	pBuf[0].m_nCharPos = 0;
	pBuf[0].m_nColorIndex = COLORINDEX_NORMALTEXT;
	nBlocks++;
	//END SW
	m_pdwParseCookies[nLineIndex] = ParseLine(dwCookie, nLineIndex, pBuf, nBlocks);
	ASSERT(m_pdwParseCookies[nLineIndex] != (DWORD) -1);

	//BEGIN SW
	int nActualItem = 0;

	// Wrap the line
	int *anBreaks = (int*)_alloca( sizeof( int ) * nLength );
	int nBreaks = 0;

	WrapLineCached( nLineIndex, GetScreenChars(), anBreaks, nBreaks );
	//END SW

	//	Draw the line text
	CPoint origin(rc.left - m_nOffsetChar * GetCharWidth(), rc.top);
	pdc->SetBkColor(crBkgnd);
	if (crText != CLR_NONE)
		pdc->SetTextColor(crText);
	BOOL bColorSet = FALSE;

	//BEGIN SW
	if( nBreaks > 0 )
	{
		// Draw all the screen lines of the wrapped line
		ASSERT( anBreaks[0] < nLength );
		
		// draw start of line to first break
		DrawScreenLine(
			pdc, origin, rc,
			pBuf, nBlocks, nActualItem,
			crText, crBkgnd, bDrawWhitespace,
			pszChars, 0, anBreaks[0], CPoint( 0, nLineIndex ) );
		
		// draw from first break to last break
		for( int i = 0; i < nBreaks - 1; i++ )
		{
			ASSERT( anBreaks[i] >= 0 && anBreaks[i] < nLength );
			DrawScreenLine(
				pdc, origin, rc,
				pBuf, nBlocks, nActualItem,
				crText, crBkgnd, bDrawWhitespace,
				pszChars, anBreaks[i], anBreaks[i + 1] - anBreaks[i],
				CPoint( anBreaks[i], nLineIndex ) );
		}
		
		// draw from last break till end of line
		DrawScreenLine(
			pdc, origin, rc,
			pBuf, nBlocks, nActualItem,
			crText, crBkgnd, bDrawWhitespace,
			pszChars, anBreaks[i], nLength - anBreaks[i],
			CPoint( anBreaks[i], nLineIndex ) );
	}
	else
		DrawScreenLine(
		pdc, origin, rc,
		pBuf, nBlocks, nActualItem,
		crText, crBkgnd, bDrawWhitespace,
		pszChars, 0, nLength, CPoint(0, nLineIndex));

	CPoint attrOrig(rc.left, rc.top+GetLineHeight());
	DrawLineAttributes(pdc, attrOrig, nLineIndex, anBreaks, nBreaks);

	//	Draw whitespaces to the left of the text
	//BEGIN SW
	// Drawing is now realized by DrawScreenLine()
	//END SW
}

COLORREF CCrystalTextView::GetColor(int nColorIndex)
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
		return RGB(0, 128, 192);
	case COLORINDEX_COMMENT:
		return RGB(128, 128, 128);
	//	[JRT]: Enabled Support For Numbers...
	case COLORINDEX_NUMBER:
		return RGB(0x80, 0x00, 0x00);
	//	[JRT]: Support For C/C++ Operators
	case COLORINDEX_OPERATOR:
		return RGB(0x00, 0x00, 0x00);
	case COLORINDEX_KEYWORD:
		return RGB(0, 0, 255);
	case COLORINDEX_SELBKGND:
		return RGB(0, 0, 0);
	case COLORINDEX_SELTEXT:
		return RGB(255, 255, 255);
	case COLORINDEX_PAIRSTRINGBKGND:
		return RGB(0, 192, 0);
	case COLORINDEX_PAIRSTRINGTEXT:
		return RGB(255, 255, 255);
	case COLORINDEX_BADPAIRSTRINGBKGND:
		return RGB(192, 0, 0);
	case COLORINDEX_BADPAIRSTRINGTEXT:
		return RGB(255, 255, 255);
	case COLORINDEX_CURPAIRSTRINGBKGND:
		return RGB(0, 192, 0);
	case COLORINDEX_CURPAIRSTRINGTEXT:
		return RGB(255, 255, 255);
	case COLORINDEX_PAIRBLOCKBKGND:
		return RGB(255, 255, 192);
	}
	return RGB(255, 0, 0);
}

DWORD CCrystalTextView::GetLineFlags(int nLineIndex)
{
	if (m_pTextBuffer == NULL)
		return 0;
	return m_pTextBuffer->GetLineFlags(nLineIndex);
}

void CCrystalTextView::DrawMargin(CDC *pdc, const CRect &rect, int nLineIndex)
{

	if (! m_bSelMargin)
	{
		pdc->FillSolidRect(rect, GetColor(COLORINDEX_BKGND));
		return;
	}

	pdc->FillSolidRect(rect, GetColor(COLORINDEX_SELMARGIN));

	if (m_bShowLineNumbers && nLineIndex != -1) { // draw line number
		CPoint pt(rect.left + 2, rect.top + (GetLineHeight() - 16) / 2);
		CString s;
		s.Format(_T("%d"), nLineIndex + 1);
		/* Select color and font */
		CFont *pOldFont = pdc->SelectObject(GetFont());
		COLORREF oldColor = pdc->SetTextColor(GetColor(COLORINDEX_NORMALTEXT));
		/* Draw it */
		CRect r = CRect(pt.x, pt.y, pt.x + GetMarginWidth()-4, pt.y + GetLineHeight());
		pdc->DrawText(s, r, DT_RIGHT|DT_SINGLELINE);
		/* Restore old values */
		pdc->SelectObject(pOldFont);
		pdc->SetTextColor(oldColor);
	}

	int nImageIndex = -1;
	if (nLineIndex >= 0)
	{
		DWORD dwLineFlags = GetLineFlags(nLineIndex);
		static const DWORD adwFlags[] =
		{
			LF_EXECUTION,
			LF_BREAKPOINT,
			LF_COMPILATION_ERROR,
			LF_BOOKMARK(1),
			LF_BOOKMARK(2),
			LF_BOOKMARK(3),
			LF_BOOKMARK(4),
			LF_BOOKMARK(5),
			LF_BOOKMARK(6),
			LF_BOOKMARK(7),
			LF_BOOKMARK(8),
			LF_BOOKMARK(9),
			LF_BOOKMARK(0),
			LF_BOOKMARKS,
			LF_INVALID_BREAKPOINT
		};
		for (int I = 0; I <= sizeof(adwFlags) / sizeof(adwFlags[0]); I ++)
		{
			if ((dwLineFlags & adwFlags[I]) != 0)
			{
				nImageIndex = I;
				break;
			}
		}
	}

	if (nImageIndex >= 0)
	{
		if (m_pIcons == NULL)
		{ 
			m_pIcons = new CImageList;
			VERIFY(m_pIcons->Create(IDR_MARGIN_ICONS, 16, 16, RGB(255, 255, 255)));
		}
		//BEGIN SW
		CPoint pt(rect.left + 2, rect.top + (GetLineHeight() - 16) / 2);
		//END SW
		VERIFY(m_pIcons->Draw(pdc, nImageIndex, pt, ILD_TRANSPARENT));
	}
}

BOOL CCrystalTextView::IsInsideSelBlock(CPoint ptTextPos)
{
	ASSERT_VALIDTEXTPOS(ptTextPos);
	if (ptTextPos.y < m_ptDrawSelStart.y)
		return FALSE;
	if (ptTextPos.y > m_ptDrawSelEnd.y)
		return FALSE;
	if (ptTextPos.y < m_ptDrawSelEnd.y && ptTextPos.y > m_ptDrawSelStart.y)
		return TRUE;
	if (m_ptDrawSelStart.y < m_ptDrawSelEnd.y)
	{
		if (ptTextPos.y == m_ptDrawSelEnd.y)
			return ptTextPos.x < m_ptDrawSelEnd.x;
		ASSERT(ptTextPos.y == m_ptDrawSelStart.y);
		return ptTextPos.x >= m_ptDrawSelStart.x;
	}
	ASSERT(m_ptDrawSelStart.y == m_ptDrawSelEnd.y);
	return ptTextPos.x >= m_ptDrawSelStart.x && ptTextPos.x < m_ptDrawSelEnd.x;
}

BOOL CCrystalTextView::IsInsideSelection(const CPoint &ptTextPos)
{
	PrepareSelBounds();
	return IsInsideSelBlock(ptTextPos);
}

void CCrystalTextView::PrepareSelBounds()
{
	if (m_ptSelStart.y < m_ptSelEnd.y ||
			(m_ptSelStart.y == m_ptSelEnd.y && m_ptSelStart.x < m_ptSelEnd.x))
	{
		m_ptDrawSelStart = m_ptSelStart;
		m_ptDrawSelEnd = m_ptSelEnd;
	}
	else
	{
		m_ptDrawSelStart = m_ptSelEnd;
		m_ptDrawSelEnd = m_ptSelStart;
	}
}

void CCrystalTextView::OnDraw(CDC *pdc)
{
	CRect rcClient;
	GetClientRect(rcClient);

	int nLineCount = GetLineCount();
	int nLineHeight = GetLineHeight();
	PrepareSelBounds();

	CDC cacheDC;
	VERIFY(cacheDC.CreateCompatibleDC(pdc));
	if (m_pCacheBitmap == NULL)
	{
		m_pCacheBitmap = new CBitmap;
		//BEGIN SW
		VERIFY(m_pCacheBitmap->CreateCompatibleBitmap(pdc, rcClient.Width(), rcClient.Height()));
		//END SW
	}
	CBitmap *pOldBitmap = cacheDC.SelectObject(m_pCacheBitmap);

	CRect rcLine;
	rcLine = rcClient;
	rcLine.bottom = rcLine.top + nLineHeight;
	CRect rcCacheMargin(0, 0, GetMarginWidth(), nLineHeight);
	CRect rcCacheLine(GetMarginWidth(), 0, rcLine.Width(), nLineHeight);
	//BEGIN SW
	// initialize rects
	int nSubLineOffset = GetSubLineIndex( m_nTopLine ) - m_nTopSubLine;
	if( nSubLineOffset < 0 )
	{
		rcCacheMargin.OffsetRect( 0, nSubLineOffset * nLineHeight );
		rcCacheLine.OffsetRect( 0, nSubLineOffset * nLineHeight );
	}

	int nBreaks;
	int nMaxLineChars = GetScreenChars();
	//END SW

	int nCurrentLine = m_nTopLine;
	while (rcLine.top < rcClient.bottom)
	{
		//BEGIN SW
		nBreaks = 0;
		if( nCurrentLine < nLineCount /*&& GetLineLength( nCurrentLine ) > nMaxLineChars*/ )
			WrapLineCached( nCurrentLine, nMaxLineChars, NULL, nBreaks );

		rcLine.bottom = rcLine.top + (nBreaks + 1) * nLineHeight;
		rcCacheLine.bottom = rcCacheLine.top + rcLine.Height();
		rcCacheMargin.bottom = rcCacheMargin.top + rcLine.Height();

		if( rcCacheLine.top < 0 )
			rcLine.bottom+= rcCacheLine.top;
		//END SW

		if (nCurrentLine < nLineCount)
		{
			DrawMargin(&cacheDC, rcCacheMargin, nCurrentLine);
			DrawSingleLine(&cacheDC, rcCacheLine, nCurrentLine);
		}
		else
		{
			DrawMargin(&cacheDC, rcCacheMargin, -1);
			DrawSingleLine(&cacheDC, rcCacheLine, -1);
		}

		VERIFY(pdc->BitBlt(rcLine.left, rcLine.top, rcLine.Width(), rcLine.Height(), &cacheDC, 0, 0, SRCCOPY));

		nCurrentLine ++;
		//BEGIN SW
		rcLine.top = rcLine.bottom;
		rcCacheLine.top = 0;
		rcCacheMargin.top = 0;
		//END SW
	}

	cacheDC.SelectObject(pOldBitmap);
	cacheDC.DeleteDC();
}

void CCrystalTextView::ResetView()
{
	m_nTopLine = 0;
	m_nOffsetChar = 0;
	m_nLineHeight = -1;
	m_nCharWidth = -1;
	m_nTabSize = 4;
	m_nMaxLineLength = -1;
	m_nScreenLines = -1;
	m_nSubLineCount = -1; //Change of View affects Number of Sublines as well
	m_nScreenChars = -1;
	m_nIdealCharPos = -1;
	m_ptAnchor.x = 0;
	m_ptAnchor.y = 0;
	delete m_pIcons;
	m_pIcons = NULL;
	for (int I = 0; I < 4; I ++)
	{
		if (m_apFonts[I] != NULL)
		{
			m_apFonts[I]->DeleteObject();
			delete m_apFonts[I];
			m_apFonts[I] = NULL;
		}
	}
	delete [] m_pdwParseCookies;
	m_pdwParseCookies = NULL;
	delete [] m_pnActualLineLength;
	m_pnActualLineLength = NULL;
	m_nParseArraySize = 0;
	m_nActualLengthArraySize = 0;
	m_ptCursorPos.x = 0;
	m_ptCursorPos.y = 0;
	m_ptSelStart = m_ptSelEnd = m_ptCursorPos;
	m_bDragSelection = FALSE;
	m_bVertScrollBarLocked = FALSE;
	m_bHorzScrollBarLocked = FALSE;
	if (::IsWindow(m_hWnd))
		UpdateCaret();
	m_bLastSearch = FALSE;
	m_bShowInactiveSelection = FALSE;
	m_bPrintHeader = FALSE;
	m_bPrintFooter = TRUE;

	m_bBookmarkExist  = FALSE;	// More bookmarks
	m_bMultipleSearch = FALSE;	// More search

	if ( m_pBackgroundThread )
	{
		if ( m_pTextBuffer )
			// Inform the background thread we've been reset.
			m_pBackgroundThread->PostThreadMessage(ID_BG_UPDATE_BUFFER, 0, (long)this);
	}

	m_ptPairStringStart.x = 0;
	m_ptPairStringStart.y = 0;
	m_ptPairStringEnd.x = 0;
	m_ptPairStringEnd.y = 0;
	m_nPairStringColorBkgnd = 0;
	m_nPairStringColorText = 0;
}

void CCrystalTextView::UpdateCaret()
{
	ASSERT_VALIDTEXTPOS(m_ptCursorPos);
	if (m_bFocused && ! m_bCursorHidden &&
		CalculateActualOffset(m_ptCursorPos.y, m_ptCursorPos.x) >= m_nOffsetChar)
	{
		if (GetCaretForm()==CARET_BLOCK)
			CreateSolidCaret(GetCharWidth(), GetLineHeight());
		else
			CreateSolidCaret(GetSystemMetrics(SM_CXBORDER)+1, GetLineHeight());
		SetCaretPos(TextToClient(m_ptCursorPos));
		ShowCaret();
	}
	else
	{
		HideCaret();
	}
}

int CCrystalTextView::GetTabSize()
{
	ASSERT(m_nTabSize >= 0 && m_nTabSize <= 64);
	return m_nTabSize;
}

void CCrystalTextView::SetTabSize(int nTabSize)
{
	ASSERT(nTabSize >= 0 && nTabSize <= 64);
	if (m_nTabSize != nTabSize)
	{
		m_nTabSize = nTabSize;
		delete [] m_pnActualLineLength;
		m_pnActualLineLength = NULL;
		m_nActualLengthArraySize = 0;
		m_nMaxLineLength = -1;
		RecalcHorzScrollBar();
		Invalidate();
		UpdateCaret();
	}
}

CFont *CCrystalTextView::GetFont(BOOL bItalic /*= FALSE*/, BOOL bBold /*= FALSE*/)
{
	int nIndex = 0;
	if (bBold)
		nIndex |= 1;
	if (bItalic)
		nIndex |= 2;

	if (m_apFonts[nIndex] == NULL)
	{
		m_apFonts[nIndex] = new CFont;
		m_lfBaseFont.lfWeight = bBold ? FW_BOLD : FW_NORMAL;
		m_lfBaseFont.lfItalic = (BYTE) bItalic;
		if (! m_apFonts[nIndex]->CreateFontIndirect(&m_lfBaseFont))
		{
			delete m_apFonts[nIndex];
			m_apFonts[nIndex] = NULL;
			return CView::GetFont();
		}

	}
	return m_apFonts[nIndex];
}

void CCrystalTextView::CalcLineCharDim()
{
	CDC *pdc = GetDC();
	CFont *pOldFont = pdc->SelectObject(GetFont());
	CSize szCharExt = pdc->GetTextExtent(_T("X"));
	m_nLineHeight = szCharExt.cy;
	if (m_nLineHeight < 1)
		m_nLineHeight = 1;
	m_nCharWidth = szCharExt.cx;
	pdc->SelectObject(pOldFont);
	ReleaseDC(pdc);
}

int CCrystalTextView::GetLineHeight()
{
	if (m_nLineHeight == -1)
		CalcLineCharDim();
	return m_nLineHeight;
}

//BEGIN SW
int CCrystalTextView::GetSubLines( int nLineIndex )
{
	// get number of wrapped lines, this line contains of
	int nBreaks = 0;
	WrapLineCached( nLineIndex, GetScreenChars(), NULL, nBreaks );

	return nBreaks + 1;
}

int CCrystalTextView::CharPosToPoint( int nLineIndex, int nCharPos, CPoint &charPoint )
{
	// if we do not wrap lines, y is always 0 and x is equl to nCharPos
	if( !m_bWordWrap )
	{
		charPoint.x = nCharPos;
		charPoint.y = 0;
	}

	// calculate point out of char pos

	// line is wrapped
	int *anBreaks = (int*)_alloca( sizeof( int ) * GetLineLength( nLineIndex ) );
	int nBreaks = 0;

	WrapLineCached( nLineIndex, GetScreenChars(), anBreaks, nBreaks );

	for( int i = nBreaks - 1; i >= 0 && nCharPos < anBreaks[i]; i-- );

	charPoint.x = (i >= 0)? nCharPos - anBreaks[i] : nCharPos;
	charPoint.y = i + 1;

	return (i >= 0)? anBreaks[i] : 0;
}

int CCrystalTextView::CursorPointToCharPos( int nLineIndex, const CPoint &curPoint )
{
	// calculate char pos out of point
	int		nLength = GetLineLength( nLineIndex );
	int		nScreenChars = GetScreenChars();
	LPCTSTR	szLine = GetLineChars( nLineIndex );

	// wrap line
	int		*anBreaks = (int*)_alloca( sizeof( int ) * nLength );
	int		nBreaks = 0;

	WrapLineCached( nLineIndex, nScreenChars, anBreaks, nBreaks );

	// find char pos that matches cursor position
	int nXPos = 0;
	int nYPos = 0;
	int nCurPos = 0;
	int nTabSize = GetTabSize();

	for( int nIndex = 0; nIndex < nLength; nIndex++ )
	{
		if( nBreaks && nIndex == anBreaks[nYPos] )
		{
			nXPos = 0;
			nYPos++;
		}

		if (szLine[nIndex] == _T('\t'))
		{
			nXPos+= (nTabSize - nCurPos % nTabSize);
			nCurPos+= (nTabSize - nCurPos % nTabSize);
		}
		else
		{
			nXPos++;
			nCurPos++;
		}

		if( nXPos > curPoint.x && nYPos == curPoint.y )
			break;
		else if( nYPos > curPoint.y )
		{
			nIndex--;
			break;
		}
	}

	return nIndex;	
}

void CCrystalTextView::SubLineCursorPosToTextPos( const CPoint &subLineCurPos, CPoint &textPos )
{
	// Get line breaks
	int nSubLineOffset, nLine;

	GetLineBySubLine( subLineCurPos.y, nLine, nSubLineOffset );

	// compute cursor-position
	textPos.x = CursorPointToCharPos( nLine, CPoint( subLineCurPos.x, nSubLineOffset ) );
	textPos.y = nLine;
}

int CCrystalTextView::SubLineEndToCharPos( int nLineIndex, int nSubLineOffset )
{
	int nLength = GetLineLength( nLineIndex );

	// if word wrapping is disabled, the end is equal to the length of the line -1
	if( !m_bWordWrap /*|| nLength <= GetScreenChars()*/ )
		return nLength;

	// wrap line
	int *anBreaks = (int*)_alloca( sizeof( int ) * nLength );
	int nBreaks = 0;

	WrapLineCached( nLineIndex, GetScreenChars(), anBreaks, nBreaks );

	// if there is no break inside the line or the given subline is the last
	// one in this line...
	if( !nBreaks || nSubLineOffset == nBreaks )
		return nLength;

	// compute character position for end of subline
	ASSERT( nSubLineOffset >= 0 && nSubLineOffset <= nBreaks );
	
	return anBreaks[nSubLineOffset] - 1;
}

int CCrystalTextView::SubLineHomeToCharPos( int nLineIndex, int nSubLineOffset )
{
	int nLength = GetLineLength( nLineIndex );

	// if word wrapping is disabled, the start is 0
	if( !m_bWordWrap || nSubLineOffset == 0 )
		return 0;

	// wrap line
	int *anBreaks = (int*)_alloca( sizeof( int ) * nLength );
	int nBreaks = 0;

	WrapLineCached( nLineIndex, GetScreenChars(), anBreaks, nBreaks );

	// if there is no break inside the line...
	if( !nBreaks )
		return 0;

	// compute character position for end of subline
	ASSERT( nSubLineOffset > 0 && nSubLineOffset <= nBreaks );
	
	return anBreaks[nSubLineOffset - 1];
}
//END SW

int CCrystalTextView::GetCharWidth()
{
	if (m_nCharWidth == -1)
		CalcLineCharDim();
	return m_nCharWidth;
}

int CCrystalTextView::GetMaxLineLength()
{
	if (m_nMaxLineLength == -1)
	{
		m_nMaxLineLength = 0;
		int nLineCount = GetLineCount();
		for (int I = 0; I < nLineCount; I ++)
		{
			int nActualLength = GetLineActualLength(I);
			if (m_nMaxLineLength < nActualLength)
				m_nMaxLineLength = nActualLength;
		}
	}
	return m_nMaxLineLength;
}

CCrystalTextView *CCrystalTextView::GetSiblingView(int nRow, int nCol)
{
	CSplitterWnd *pSplitter = GetParentSplitter(this, FALSE);
	if (pSplitter == NULL)
		return NULL;
	CWnd *pWnd = CWnd::FromHandlePermanent(
		::GetDlgItem(pSplitter->m_hWnd, pSplitter->IdFromRowCol(nRow, nCol)));
	if (pWnd == NULL || ! pWnd->IsKindOf(RUNTIME_CLASS(CCrystalTextView)))
		return NULL;
	return (CCrystalTextView *) pWnd;
}

void CCrystalTextView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	AttachToBuffer(NULL);

	CSplitterWnd *pSplitter = GetParentSplitter(this, FALSE);
	if (pSplitter != NULL)
	{
		//	See CSplitterWnd::IdFromRowCol() implementation
		int nRow = (GetDlgCtrlID() - AFX_IDW_PANE_FIRST) / 16;
		int nCol = (GetDlgCtrlID() - AFX_IDW_PANE_FIRST) % 16;
		ASSERT(nRow >= 0 && nRow < pSplitter->GetRowCount());
		ASSERT(nCol >= 0 && nCol < pSplitter->GetColumnCount());

		if (nRow > 0)
		{
			CCrystalTextView *pSiblingView = GetSiblingView(0, nCol);
			if (pSiblingView != NULL && pSiblingView != this)
			{
				m_nOffsetChar = pSiblingView->m_nOffsetChar;
				ASSERT(m_nOffsetChar >= 0 && m_nOffsetChar <= GetMaxLineLength());
			}
		}

		if (nCol > 0)
		{
			CCrystalTextView *pSiblingView = GetSiblingView(nRow, 0);
			if (pSiblingView != NULL && pSiblingView != this)
			{
				m_nTopLine = pSiblingView->m_nTopLine;
				ASSERT(m_nTopLine >= 0 && m_nTopLine < GetLineCount());
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CCrystalTextView printing

void CCrystalTextView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	CView::OnPrepareDC(pDC, pInfo);

	if (pInfo != NULL)
	{
		pInfo->m_bContinuePrinting = TRUE;
		if (m_pnPages != NULL && (int) pInfo->m_nCurPage > m_nPrintPages)
			pInfo->m_bContinuePrinting = FALSE;
	}
}

BOOL CCrystalTextView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

int CCrystalTextView::PrintLineHeight(CDC *pdc, int nLine)
{
	ASSERT(nLine >= 0 && nLine < GetLineCount());
	ASSERT(m_nPrintLineHeight > 0);
	int nLength = GetLineLength(nLine);
	if (nLength == 0)
		return m_nPrintLineHeight;

	CString	line;
	LPCTSTR	pszChars = GetLineChars(nLine);
	ExpandChars(pszChars, 0, nLength, line);
	CRect rcPrintArea = m_rcPrintArea;
	pdc->DrawText(line, &rcPrintArea, DT_LEFT | DT_NOPREFIX | DT_TOP | DT_WORDBREAK | DT_CALCRECT);
	return rcPrintArea.Height();
}

void CCrystalTextView::GetPrintHeaderText(int nPageNum, CString &text)
{
	ASSERT(m_bPrintHeader);
	text = _T("");
}

void CCrystalTextView::GetPrintFooterText(int nPageNum, CString &text)
{
	ASSERT(m_bPrintFooter);
	text.Format(_T("Page %d/%d"), nPageNum, m_nPrintPages);
}

void CCrystalTextView::PrintHeader(CDC *pdc, int nPageNum)
{
	CRect rcHeader = m_rcPrintArea;
	rcHeader.bottom = rcHeader.top;
	rcHeader.top -= (m_nPrintLineHeight + m_nPrintLineHeight / 2);

	CString	text;
	GetPrintHeaderText(nPageNum, text);
	if (! text.IsEmpty())
		pdc->DrawText(text, &rcHeader, DT_CENTER | DT_NOPREFIX | DT_TOP | DT_SINGLELINE);
}

void CCrystalTextView::PrintFooter(CDC *pdc, int nPageNum)
{
	CRect rcFooter = m_rcPrintArea;
	rcFooter.top = rcFooter.bottom;
	rcFooter.bottom += (m_nPrintLineHeight + m_nPrintLineHeight / 2);

	CString	text;
	GetPrintFooterText(nPageNum, text);
	if (! text.IsEmpty())
		pdc->DrawText(text, &rcFooter, DT_CENTER | DT_NOPREFIX | DT_BOTTOM | DT_SINGLELINE);
}

void CCrystalTextView::RecalcPageLayouts(CDC *pdc, CPrintInfo *pInfo)
{
	m_ptPageArea = pInfo->m_rectDraw;
	m_ptPageArea.NormalizeRect();

	m_nPrintLineHeight = pdc->GetTextExtent(_T("X")).cy;

	m_rcPrintArea = m_ptPageArea;
	CSize szTopLeft, szBottomRight;
	CWinApp *pApp = AfxGetApp();
	ASSERT(pApp != NULL);
	szTopLeft.cx = pApp->GetProfileInt(REG_PAGE_SUBKEY, REG_MARGIN_LEFT, DEFAULT_PRINT_MARGIN);
	szBottomRight.cx = pApp->GetProfileInt(REG_PAGE_SUBKEY, REG_MARGIN_RIGHT, DEFAULT_PRINT_MARGIN);
	szTopLeft.cy = pApp->GetProfileInt(REG_PAGE_SUBKEY, REG_MARGIN_TOP, DEFAULT_PRINT_MARGIN);
	szBottomRight.cy = pApp->GetProfileInt(REG_PAGE_SUBKEY, REG_MARGIN_BOTTOM, DEFAULT_PRINT_MARGIN);
	pdc->HIMETRICtoLP(&szTopLeft);
	pdc->HIMETRICtoLP(&szBottomRight);
	m_rcPrintArea.left += szTopLeft.cx;
	m_rcPrintArea.right -= szBottomRight.cx;
	m_rcPrintArea.top += szTopLeft.cy;
	m_rcPrintArea.bottom -= szBottomRight.cy;
	if (m_bPrintHeader)
		m_rcPrintArea.top += m_nPrintLineHeight + m_nPrintLineHeight / 2;
	if (m_bPrintFooter)
		m_rcPrintArea.bottom += m_nPrintLineHeight + m_nPrintLineHeight / 2;

	int nLimit = 32;
	m_nPrintPages = 1;
	m_pnPages = new int[nLimit];
	m_pnPages[0] = 0;

	int nLineCount = GetLineCount();
	int nLine = 1;
	int y = m_rcPrintArea.top + PrintLineHeight(pdc, 0);
	while (nLine < nLineCount)
	{
		int nHeight = PrintLineHeight(pdc, nLine);
		if (y + nHeight <= m_rcPrintArea.bottom)
		{
			y += nHeight;
		}
		else
		{
			ASSERT(nLimit >= m_nPrintPages);
			if (nLimit <= m_nPrintPages)
			{
				nLimit += 32;
				int *pnNewPages = new int[nLimit];
				memcpy(pnNewPages, m_pnPages, sizeof(int) * m_nPrintPages);
				delete [] m_pnPages;
				m_pnPages = pnNewPages;
			}
			ASSERT(nLimit > m_nPrintPages);
			m_pnPages[m_nPrintPages ++] = nLine;
			y = m_rcPrintArea.top + nHeight;
		}
		nLine ++;
	}
}

void CCrystalTextView::OnBeginPrinting(CDC *pdc, CPrintInfo *pInfo)
{
	ASSERT(m_pnPages == NULL);
	ASSERT(m_pPrintFont == NULL);
	CFont *pDisplayFont = GetFont();

	LOGFONT lf;
	pDisplayFont->GetLogFont(&lf);

	CDC *pDisplayDC = GetDC();
	lf.lfHeight = MulDiv(lf.lfHeight, pdc->GetDeviceCaps(LOGPIXELSY), pDisplayDC->GetDeviceCaps(LOGPIXELSY));
	lf.lfWidth = MulDiv(lf.lfWidth, pdc->GetDeviceCaps(LOGPIXELSX), pDisplayDC->GetDeviceCaps(LOGPIXELSX));
	ReleaseDC(pDisplayDC);

	m_pPrintFont = new CFont;
	if (! m_pPrintFont->CreateFontIndirect(&lf))
	{
		delete m_pPrintFont;
		m_pPrintFont = NULL;
		return;
	}

	pdc->SelectObject(m_pPrintFont);
}

void CCrystalTextView::OnEndPrinting(CDC *pdc, CPrintInfo *pInfo)
{
	delete m_pPrintFont;
	m_pPrintFont = NULL;
	delete m_pnPages;
	m_pnPages = NULL;
	m_nPrintPages = 0;
	m_nPrintLineHeight = 0;
}

void CCrystalTextView::OnPrint(CDC* pdc, CPrintInfo* pInfo) 
{
	if (m_pnPages == NULL)
	{
		RecalcPageLayouts(pdc, pInfo);
		ASSERT(m_pnPages != NULL);
	}

	ASSERT(pInfo->m_nCurPage >= 1 && (int) pInfo->m_nCurPage <= m_nPrintPages);
	int nLine = m_pnPages[pInfo->m_nCurPage - 1];
	int nEndLine = GetLineCount();
	if ((int) pInfo->m_nCurPage < m_nPrintPages)
		nEndLine = m_pnPages[pInfo->m_nCurPage];
	TRACE(_T("Printing page %d of %d, lines %d - %d\n"), pInfo->m_nCurPage, m_nPrintPages,
						nLine, nEndLine - 1);

	if (m_bPrintHeader)
		PrintHeader(pdc, pInfo->m_nCurPage);
	if (m_bPrintFooter)
		PrintFooter(pdc, pInfo->m_nCurPage);

	int y = m_rcPrintArea.top;
	for (; nLine < nEndLine; nLine ++)
	{
		int nLineLength = GetLineLength(nLine);
		if (nLineLength == 0)
		{
			y += m_nPrintLineHeight;
			continue;
		}

		CRect rcPrintRect = m_rcPrintArea;
		rcPrintRect.top = y;
		LPCTSTR	pszChars = GetLineChars(nLine);
		CString	line;
		ExpandChars(pszChars, 0, nLineLength, line);
		y += pdc->DrawText(line, &rcPrintRect, DT_LEFT | DT_NOPREFIX | DT_TOP | DT_WORDBREAK);
	}
}


/////////////////////////////////////////////////////////////////////////////
// CCrystalTextView message handlers

int CCrystalTextView::GetLineCount()
{
	if (m_pTextBuffer == NULL)
		return 1;		//	Single empty line
	int nLineCount = m_pTextBuffer->GetLineCount();
	ASSERT(nLineCount > 0);
	return nLineCount;
}

//BEGIN SW
int CCrystalTextView::GetSubLineCount()
{
	// if we do not wrap words, number of sub lines is equal to number of lines
	if( !m_bWordWrap )
		return GetLineCount();

	if ( m_nSubLineCount == -1 )
	{
		// calculate number of sub lines
		int nLineCount = GetLineCount();
		int nSubLineCount = 0;

		for( int i = 0; i < nLineCount; i++ )
			nSubLineCount+= GetSubLines( i );

		m_nSubLineCount = nSubLineCount;
	}
	return m_nSubLineCount;
}

int CCrystalTextView::GetSubLineIndex( int nLineIndex )
{
	// if we do not wrap words, subline index of this line is equal to its index
	if( !m_bWordWrap )
		return nLineIndex;

	// calculate subline index of the line
	int nSubLineCount = 0;

	if( nLineIndex >= GetLineCount() )
		nLineIndex = GetLineCount() - 1;

	for( int i = 0; i < nLineIndex; i++ )
		nSubLineCount+= GetSubLines( i );

	return nSubLineCount;
}

void CCrystalTextView::GetLineBySubLine( int nSubLineIndex, int &nLine, int &nSubLine )
{
	ASSERT( nSubLineIndex < GetSubLineCount() );

	// if we do not wrap words, nLine is equal to nSubLineIndex and nSubLine is always 0
	if( !m_bWordWrap )
	{
		nLine = nSubLineIndex;
		nSubLine = 0;
	}

	// compute result
	int nSubLineCount = 0;
	int nLineCount = GetLineCount();

	for( int i = 0; i < nLineCount; i++ )
	{
		nSubLineCount+= GetSubLines( i );
		if( !(nSubLineCount <= nSubLineIndex) )
			break;
	}

	ASSERT( i < nLineCount );
	nLine = i;
	nSubLine = nSubLineIndex - (nSubLineCount - GetSubLines( i ));
}
//END SW

int CCrystalTextView::GetLineLength(int nLineIndex)
{
	if (m_pTextBuffer == NULL)
		return 0;
	return m_pTextBuffer->GetLineLength(nLineIndex);
}

LPCTSTR CCrystalTextView::GetLineChars(int nLineIndex)
{
	if (m_pTextBuffer == NULL)
		return NULL;
	return m_pTextBuffer->GetLineChars(nLineIndex);
}

void CCrystalTextView::AttachToBuffer(CCrystalTextBuffer *pBuf /*= NULL*/)
{
	if (m_pTextBuffer != NULL)
		m_pTextBuffer->RemoveView(this);
	if (pBuf == NULL)
	{
		pBuf = LocateTextBuffer();
	}
	m_pTextBuffer = pBuf;
	if (m_pTextBuffer != NULL)
		m_pTextBuffer->AddView(this);
	Hold( true );
	ResetView();
	Release( true );

	//	Init scrollbars
	CScrollBar *pVertScrollBarCtrl = GetScrollBarCtrl(SB_VERT);
	if (pVertScrollBarCtrl != NULL)
		pVertScrollBarCtrl->EnableScrollBar(GetScreenLines() >= GetLineCount() ?
			ESB_DISABLE_BOTH : ESB_ENABLE_BOTH);
	CScrollBar *pHorzScrollBarCtrl = GetScrollBarCtrl(SB_HORZ);
	if (pHorzScrollBarCtrl != NULL)
		pHorzScrollBarCtrl->EnableScrollBar(GetScreenChars() >= GetMaxLineLength() ?
			ESB_DISABLE_BOTH : ESB_ENABLE_BOTH);

	//	Update scrollbars
	RecalcVertScrollBar();
	RecalcHorzScrollBar();
}

void CCrystalTextView::DetachFromBuffer()
{
	if (m_pTextBuffer != NULL)
	{
		m_pTextBuffer->RemoveView(this);
		m_pTextBuffer = NULL;
		ResetView();
	}
}

int CCrystalTextView::GetScreenLines()
{
	if (m_nScreenLines == -1)
	{
		CRect rect;
		GetClientRect(&rect);
		m_nScreenLines = rect.Height() / GetLineHeight();
	}
	return m_nScreenLines;
}

BOOL CCrystalTextView::GetItalic(int nColorIndex)
{
	return FALSE;
}

BOOL CCrystalTextView::GetBold(int nColorIndex)
{
	return FALSE;
}

int CCrystalTextView::GetScreenChars()
{
	if (m_nScreenChars == -1)
	{
		CRect rect;
		GetClientRect(&rect);
		m_nScreenChars = (rect.Width() - GetMarginWidth()) / GetCharWidth();
	}
	return m_nScreenChars;
}

void CCrystalTextView::OnDestroy() 
{
	if ( m_pBackgroundThread )
	{
		// Increase the hold count to delay destruction until the background thread 
		// processes the ID_BG_INVALIDATE_VIEW message and releases this view.
		Hold();
		// Inform the background thread we're being destroyed
		m_pBackgroundThread->PostThreadMessage(ID_BG_INVALIDATE_VIEW, 0, (long)this);
	}
	// This hold will wait until background thread has processed the 
	// ID_BG_INVALIDATE_VIEW message
	Hold( true );
	DetachFromBuffer();
	m_hAccel = NULL;

	CView::OnDestroy();

	for (int I = 0; I < 4; I ++)
	{
		if (m_apFonts[I] != NULL)
		{
			m_apFonts[I]->DeleteObject();
			delete m_apFonts[I];
			m_apFonts[I] = NULL;
		}
	}
	delete m_pCacheBitmap;
	m_pCacheBitmap = NULL;
	Release( true );
}

BOOL CCrystalTextView::OnEraseBkgnd(CDC *pdc) 
{
	return TRUE;
}


void CCrystalTextView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	//BEGIN SW
	// get char position of top left visible character with old cached word wrap
	CPoint topPos;
	SubLineCursorPosToTextPos( CPoint( 0, m_nTopSubLine ), topPos );
	//END SW

	if (m_pCacheBitmap != NULL)
	{
		m_pCacheBitmap->DeleteObject();
		delete m_pCacheBitmap;
		m_pCacheBitmap = NULL;
	}
	m_nScreenLines = -1;
	m_nScreenChars = -1;

	//BEGIN SW
	// we have to recompute the line wrapping
	InvalidateLineCache( 0, -1 );

	// compute new top sub line
	CPoint topSubLine;
	CharPosToPoint( topPos.y, topPos.x, topSubLine );
	m_nTopSubLine = GetSubLineIndex(topPos.y) + topSubLine.y;

	ScrollToSubLine( m_nTopSubLine );

	// set caret to right position
	CPoint pt = GetCursorPos();
	if( pt.y >= 0 && pt.y < GetLineCount() && pt.x >= 0 && (pt.x < GetLineLength( pt.y ) || pt.x == 0) )
	{
		EnsureVisible( pt );
		UpdateCaret();
	}
	//END SW

	RecalcVertScrollBar();
	RecalcHorzScrollBar();
}

void CCrystalTextView::UpdateSiblingScrollPos(BOOL bHorz)
{
	CSplitterWnd *pSplitterWnd = GetParentSplitter(this, FALSE);
	if (pSplitterWnd != NULL)
	{
		//	See CSplitterWnd::IdFromRowCol() implementation for details
		int nCurrentRow = (GetDlgCtrlID() - AFX_IDW_PANE_FIRST) / 16;
		int nCurrentCol = (GetDlgCtrlID() - AFX_IDW_PANE_FIRST) % 16;
		ASSERT(nCurrentRow >= 0 && nCurrentRow < pSplitterWnd->GetRowCount());
		ASSERT(nCurrentCol >= 0 && nCurrentCol < pSplitterWnd->GetColumnCount());

		if (bHorz)
		{
			int nCols = pSplitterWnd->GetColumnCount();
			for (int nCol = 0; nCol < nCols; nCol ++)
			{
				if (nCol != nCurrentCol)	//	We don't need to update ourselves
				{
					CCrystalTextView *pSiblingView = GetSiblingView(nCurrentRow, nCol);
					if (pSiblingView != NULL)
						pSiblingView->OnUpdateSibling(this, TRUE);
				}
			}
		}
		else
		{
			int nRows = pSplitterWnd->GetRowCount();
			for (int nRow = 0; nRow < nRows; nRow ++)
			{
				if (nRow != nCurrentRow)	//	We don't need to update ourselves
				{
					CCrystalTextView *pSiblingView = GetSiblingView(nRow, nCurrentCol);
					if (pSiblingView != NULL)
						pSiblingView->OnUpdateSibling(this, FALSE);
				}
			}
		}
	}
}

void CCrystalTextView::OnUpdateSibling(CCrystalTextView *pUpdateSource, BOOL bHorz)
{
	if (pUpdateSource != this)
	{
		ASSERT(pUpdateSource != NULL);
		ASSERT_KINDOF(CCrystalTextView, pUpdateSource);
		if (bHorz)
		{
			ASSERT(pUpdateSource->m_nTopLine >= 0);
			ASSERT(pUpdateSource->m_nTopLine < GetLineCount());
			if (pUpdateSource->m_nTopLine != m_nTopLine)
			{
				ScrollToLine(pUpdateSource->m_nTopLine, TRUE, FALSE);
				UpdateCaret();
			}
		}
		else
		{
			ASSERT(pUpdateSource->m_nOffsetChar >= 0);
			ASSERT(pUpdateSource->m_nOffsetChar < GetMaxLineLength());
			if (pUpdateSource->m_nOffsetChar != m_nOffsetChar)
			{
				ScrollToChar(pUpdateSource->m_nOffsetChar, TRUE, FALSE);
				UpdateCaret();
			}
		}
	}
}

void CCrystalTextView::RecalcVertScrollBar(BOOL bPositionOnly /*= FALSE*/)
{
	SCROLLINFO si;
	si.cbSize = sizeof(si);
	if (bPositionOnly)
	{
		si.fMask = SIF_POS;
		//BEGIN SW
		si.nPos = m_nTopSubLine;
		//END SW
	}
	else
	{
		//BEGIN SW
		if( GetScreenLines() >= GetSubLineCount() && m_nTopSubLine > 0 )
		//END SW
		{
			m_nTopLine = 0;
			m_nTopSubLine = 0;
			Invalidate();
			UpdateCaret();
		}
		si.fMask = SIF_DISABLENOSCROLL | SIF_PAGE | SIF_POS | SIF_RANGE;
		si.nMin = 0;
		//BEGIN SW
		si.nMax = GetSubLineCount() - 1;
		//END SW
		si.nPage = GetScreenLines();
		//BEGIN SW
		si.nPos = m_nTopSubLine;
		//END SW
	}
	VERIFY(SetScrollInfo(SB_VERT, &si));
}

void CCrystalTextView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CView::OnVScroll(nSBCode, nPos, pScrollBar);

	//	Note we cannot use nPos because of its 16-bit nature
	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL;
	VERIFY(GetScrollInfo(SB_VERT, &si));

	//BEGIN SW
	int nPageLines = GetScreenLines();
	int nSubLineCount = GetSubLineCount();

	int nNewTopSubLine;
	BOOL bDisableSmooth = TRUE;
	switch (nSBCode)
	{
	case SB_TOP:
		nNewTopSubLine = 0;
		bDisableSmooth = FALSE;
		break;
	case SB_BOTTOM:
		nNewTopSubLine = nSubLineCount - nPageLines + 1;
		bDisableSmooth = FALSE;
		break;
	case SB_LINEUP:
		nNewTopSubLine = m_nTopSubLine - 1;
		break;
	case SB_LINEDOWN:
		nNewTopSubLine = m_nTopSubLine + 1;
		break;
	case SB_PAGEUP:
		nNewTopSubLine = m_nTopSubLine - si.nPage + 1;
		bDisableSmooth = FALSE;
		break;
	case SB_PAGEDOWN:
		nNewTopSubLine = m_nTopSubLine + si.nPage - 1;
		bDisableSmooth = FALSE;
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		nNewTopSubLine = si.nTrackPos;
		break;
	default:
		return;
	}

	if (nNewTopSubLine < 0)
		nNewTopSubLine = 0;
	if (nNewTopSubLine >= nSubLineCount)
		nNewTopSubLine = nSubLineCount - 1;
	ScrollToSubLine(nNewTopSubLine, bDisableSmooth);
	//END SW
}

void CCrystalTextView::RecalcHorzScrollBar(BOOL bPositionOnly /*= FALSE*/)
{
	// only recalc, if word wrapping is disabled
	if( m_bWordWrap )
		return;

	//	Again, we cannot use nPos because it's 16-bit
	SCROLLINFO si;
	si.cbSize = sizeof(si);
	if (bPositionOnly)
	{
		si.fMask = SIF_POS;
		si.nPos = m_nOffsetChar;
	}
	else
	{
		if (GetScreenChars() >= GetMaxLineLength() && m_nOffsetChar > 0)
		{
			m_nOffsetChar = 0;
			Invalidate();
			UpdateCaret();
		}
		si.fMask = SIF_DISABLENOSCROLL | SIF_PAGE | SIF_POS | SIF_RANGE;
		si.nMin = 0;
		si.nMax = GetMaxLineLength() - 1;
		si.nPage = GetScreenChars();
		si.nPos = m_nOffsetChar;
	}
	VERIFY(SetScrollInfo(SB_HORZ, &si));
}

void CCrystalTextView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CView::OnHScroll(nSBCode, nPos, pScrollBar);

	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL;
	VERIFY(GetScrollInfo(SB_HORZ, &si));

	int nPageChars = GetScreenChars();
	int nMaxLineLength = GetMaxLineLength();
	int nNewOffset;

	switch (nSBCode)
	{
	case SB_LEFT:
		nNewOffset = 0;
		break;
	case SB_BOTTOM:
		nNewOffset = nMaxLineLength - nPageChars + 1;
		break;
	case SB_LINEUP:
		nNewOffset = m_nOffsetChar - 1;
		break;
	case SB_LINEDOWN:
		nNewOffset = m_nOffsetChar + 1;
		break;
	case SB_PAGEUP:
		nNewOffset = m_nOffsetChar - si.nPage + 1;
		break;
	case SB_PAGEDOWN:
		nNewOffset = m_nOffsetChar + si.nPage - 1;
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		nNewOffset = si.nTrackPos;
		break;
	default:
		return;
	}

	if (nNewOffset >= nMaxLineLength)
		nNewOffset = nMaxLineLength - 1;
	if (nNewOffset < 0)
		nNewOffset = 0;
	ScrollToChar(nNewOffset, TRUE);
	UpdateCaret();
}

BOOL CCrystalTextView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (nHitTest == HTCLIENT)
	{
		CPoint pt;
		::GetCursorPos(&pt);
		ScreenToClient(&pt);
		if (pt.x < GetMarginWidth())
		{
			::SetCursor(::LoadCursor(GetResourceHandle(), MAKEINTRESOURCE(IDR_MARGIN_CURSOR)));
		}
		else
		{
			CPoint ptText = ClientToText(pt);
			PrepareSelBounds();
			if (IsInsideSelBlock(ptText))
			{
				//	[JRT]:	Support For Disabling Drag and Drop...
				if (!m_bDisableDragAndDrop)				// If Drag And Drop Not Disabled
					::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));	// Set To Arrow Cursor
			}
			else
				::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));
		}
		return TRUE;
	}

	BOOL result = CView::OnSetCursor(pWnd, nHitTest, message);

	return result;
}

CPoint CCrystalTextView::ClientToText(const CPoint &point)
{
	//BEGIN SW
	int nSubLineCount = GetSubLineCount();
	int nLineCount = GetLineCount();

	CPoint pt;
	pt.y = m_nTopSubLine + point.y / GetLineHeight();
	if (pt.y >= nSubLineCount)
		pt.y = nSubLineCount - 1;
	if (pt.y < 0)
		pt.y = 0;

	int nLine;
	int nSubLineOffset;
	int nOffsetChar = m_nOffsetChar;

	GetLineBySubLine( pt.y, nLine, nSubLineOffset );
	pt.y = nLine;

	LPCTSTR pszLine = NULL;
	int nLength = 0;
	int *anBreaks = NULL;
	int nBreaks = 0;

	if (pt.y >= 0 && pt.y < nLineCount)
	{
		nLength = GetLineLength( pt.y );
		anBreaks = (int*)_alloca( sizeof( int ) * nLength );
		pszLine = GetLineChars(pt.y);
		WrapLineCached( pt.y, GetScreenChars(), anBreaks, nBreaks );

		if( nSubLineOffset > 0 )
			nOffsetChar = anBreaks[nSubLineOffset - 1];
		if( nBreaks > nSubLineOffset )
			nLength = anBreaks[nSubLineOffset] - 1;
	}

	int nPos = nOffsetChar + (point.x - GetMarginWidth()) / GetCharWidth();
	if (nPos < 0)
		nPos = 0;

	int nIndex = 0, nCurPos = 0, n = 0, i = 0;
	int nTabSize = GetTabSize();

	while (nIndex < nLength)
	{
		if( nBreaks && nIndex == anBreaks[i] )
		{
			n = nIndex;
			i++;
		}

		if (pszLine[nIndex] == _T('\t'))
		{
			n+= (nTabSize - nCurPos % nTabSize);
			nCurPos += (nTabSize - nCurPos % nTabSize);
		}
		else
		{
			n++;
			nCurPos ++;
		}

		if (n > nPos && i == nSubLineOffset)
			break;

		nIndex ++;
	}

	ASSERT(nIndex >= 0 && nIndex <= nLength);
	pt.x = nIndex;
	return pt;
	//END SW
}

#ifdef _DEBUG
void CCrystalTextView::AssertValidTextPos(const CPoint &point)
{
	if (GetLineCount() > 0)
	{
		ASSERT(m_nTopLine >= 0 && m_nOffsetChar >= 0);
		ASSERT(point.y >= 0 && point.y < GetLineCount());
		ASSERT(point.x >= 0 && point.x <= GetLineLength(point.y));
	}
}
#endif

CPoint CCrystalTextView::TextToClient(const CPoint &point)
{
	ASSERT_VALIDTEXTPOS(point);
	int		nLength = GetLineLength(point.y);
	LPCTSTR	pszLine = GetLineChars(point.y);
	CPoint	pt;

	//BEGIN SW
	CPoint	charPoint;
	int		nSubLineStart = CharPosToPoint( point.y, point.x, charPoint );
	charPoint.y+= GetSubLineIndex( point.y );

	// compute y-position
	pt.y = (charPoint.y - m_nTopSubLine) * GetLineHeight();

	// if pt.x is null, we know the result
	if( charPoint.x == 0 )
	{
		pt.x = GetMarginWidth();
		return pt;
	}

	// we have to calculate x-position
	int nPreOffset = 0;
	//END SW

	pt.x = 0;
	int nTabSize = GetTabSize();

	for (int nIndex = 0; nIndex < point.x; nIndex ++)
	{
		//BEGIN SW
		if( nIndex == nSubLineStart )
			nPreOffset = pt.x;
		//END SW

		if (pszLine[nIndex] == _T('\t'))
			pt.x += (nTabSize - pt.x % nTabSize);
		else
			pt.x ++;
	}

	//BEGIN SW
	pt.x-= nPreOffset;
	//END SW

	pt.x = (pt.x - m_nOffsetChar) * GetCharWidth() + GetMarginWidth();
	return pt;
}

void CCrystalTextView::InvalidateLines(int nLine1, int nLine2, BOOL bInvalidateMargin /*= FALSE*/)
{
	m_nSubLineCount = -1;
	bInvalidateMargin = TRUE;
	if (nLine2 == -1)
	{
		CRect rcInvalid;
		GetClientRect(&rcInvalid);
		if (! bInvalidateMargin)
			rcInvalid.left += GetMarginWidth();

		//BEGIN SW
		rcInvalid.top = (GetSubLineIndex( nLine1 ) - m_nTopSubLine) * GetLineHeight();
		//END SW
		InvalidateRect(&rcInvalid, FALSE);
	}
	else
	{
		if (nLine2 < nLine1)
		{
			int nTemp = nLine1;
			nLine1 = nLine2;
			nLine2 = nTemp;
		}
		CRect rcInvalid;
		GetClientRect(&rcInvalid);
		if (! bInvalidateMargin)
			rcInvalid.left += GetMarginWidth();

		//BEGIN SW
		rcInvalid.top = (GetSubLineIndex( nLine1 ) - m_nTopSubLine) * GetLineHeight();
		rcInvalid.bottom = (GetSubLineIndex( nLine2 ) - m_nTopSubLine + GetSubLines( nLine2 )) * GetLineHeight();
		//END SW
		InvalidateRect(&rcInvalid, FALSE);
	}
}

void CCrystalTextView::SetSelection(const CPoint &ptStart, const CPoint &ptEnd)
{
	ASSERT_VALIDTEXTPOS(ptStart);
	ASSERT_VALIDTEXTPOS(ptEnd);
	if (m_ptSelStart == ptStart)
	{
		if (m_ptSelEnd != ptEnd)
			InvalidateLines(ptEnd.y, m_ptSelEnd.y);
	}
	else
	{
		InvalidateLines(ptStart.y, ptEnd.y);
		InvalidateLines(m_ptSelStart.y, m_ptSelEnd.y);
	}
	m_ptSelStart = ptStart;
	m_ptSelEnd = ptEnd;

	//no selection 
	if (m_ptSelStart == m_ptSelEnd) {
		ResetSelBlockLevel();
	}
	else {
		UnmarkPairString();
	}
}

void CCrystalTextView::AdjustTextPoint(CPoint &point)
{
	point.x += GetCharWidth() / 2;	//todo
}

void CCrystalTextView::OnSetFocus(CWnd* pOldWnd) 
{
	CView::OnSetFocus(pOldWnd);

	m_bFocused = TRUE;
	if (m_ptSelStart != m_ptSelEnd)
		InvalidateLines(m_ptSelStart.y, m_ptSelEnd.y);
	UpdateCaret();
}

DWORD CCrystalTextView::ParseLine(DWORD dwCookie, int nLineIndex, TEXTBLOCK *pBuf, int &nActualItems)
{
	if( !m_pParser )
		return 0;

	if (pBuf)
	{
		CCrystalTextBlock	textBlock = CCrystalTextBlock( (CCrystalTextBlock::TEXTBLOCK*)pBuf, nActualItems );
		dwCookie = m_pParser->ParseLine( dwCookie, nLineIndex, &textBlock );
	}
	
	return dwCookie;
}

int CCrystalTextView::CalculateActualOffset(int nLineIndex, int nCharIndex)
{
	int nLength = GetLineLength(nLineIndex);
	ASSERT(nCharIndex >= 0 && nCharIndex <= nLength);
	LPCTSTR	pszChars = GetLineChars(nLineIndex);
	int nOffset = 0;
	int nTabSize = GetTabSize();

	//BEGIN SW
	int *anBreaks = (int*)_alloca( sizeof( int ) * nLength );
	int nBreaks = 0;

	WrapLineCached( nLineIndex, GetScreenChars(), anBreaks, nBreaks );

	int nPreOffset = 0;
	int nPreBreak = 0;

	if( nBreaks )
	{
		for( int J = nBreaks - 1; J >= 0 && nCharIndex < anBreaks[J]; J-- );
		nPreBreak = anBreaks[J];
	}
	//END SW

	for (int I = 0; I < nCharIndex; I ++)
	{
		//BEGIN SW
		if( nPreBreak == I && nBreaks )
			nPreOffset = nOffset;
		//END SW

		if (pszChars[I] == _T('\t'))
			nOffset += (nTabSize - nOffset % nTabSize);
		else
			nOffset ++;
	}

	//BEGIN SW
	if( nPreBreak == I && nBreaks )
		return 0;
	else
		return nOffset - nPreOffset;
	//END SW
}

/* // Unused
int CCrystalTextView::ApproxActualOffset(int nLineIndex, int nOffset)
{
	if (nOffset == 0)
		return 0;

	int		nLength = GetLineLength(nLineIndex);
	LPCTSTR	pszChars = GetLineChars(nLineIndex);
	int		nCurrentOffset = 0;
	int		nTabSize = GetTabSize();
	for (int I = 0; I < nLength; I ++)
	{
		if (pszChars[I] == _T('\t'))
			nCurrentOffset += (nTabSize - nCurrentOffset % nTabSize);
		else
			nCurrentOffset ++;
		if (nCurrentOffset >= nOffset)
		{
			if (nOffset <= nCurrentOffset - nTabSize / 2)
				return I;
			return I + 1;
		}
	}
	return nLength;
}
*/

void CCrystalTextView::EnsureVisible(CPoint pt)
{
	//	Scroll vertically
	//BEGIN SW
	int		nSubLineCount = GetSubLineCount();
	int		nNewTopSubLine = m_nTopSubLine;
	CPoint	subLinePos;

	CharPosToPoint( pt.y, pt.x, subLinePos );
	subLinePos.y+= GetSubLineIndex( pt.y );

	if( subLinePos.y >= nNewTopSubLine + GetScreenLines() )
		nNewTopSubLine = subLinePos.y - GetScreenLines() + 1;
	if( subLinePos.y < nNewTopSubLine )
		nNewTopSubLine = subLinePos.y;

	if( nNewTopSubLine < 0 )
		nNewTopSubLine = 0;
	if( nNewTopSubLine >= nSubLineCount )
		nNewTopSubLine = nSubLineCount - 1;
	
	if( nNewTopSubLine != m_nTopSubLine )
	{
		ScrollToSubLine( nNewTopSubLine );
		UpdateCaret();
		UpdateSiblingScrollPos( TRUE );
	}
	//END SW

	//	Scroll horizontally
	//BEGIN SW
	// we do not need horizontally scrolling, if we wrap the words
	if( m_bWordWrap )
		return;
	//END SW

	int nActualPos = CalculateActualOffset(pt.y, pt.x);
	int nNewOffset = m_nOffsetChar;
	if (nActualPos > nNewOffset + GetScreenChars())
	{
		nNewOffset = nActualPos - GetScreenChars();
	}
	if (nActualPos < nNewOffset)
	{
		nNewOffset = nActualPos;
	}

	if (nNewOffset >= GetMaxLineLength())
		nNewOffset = GetMaxLineLength() - 1;
	if (nNewOffset < 0)
		nNewOffset = 0;

	if (m_nOffsetChar != nNewOffset)
	{
		ScrollToChar(nNewOffset);
		UpdateCaret();
		UpdateSiblingScrollPos(FALSE);
	}
}

void CCrystalTextView::OnKillFocus(CWnd* pNewWnd) 
{
	CView::OnKillFocus(pNewWnd);

	//Stop incremental search
	OnEditFindIncrementalStop(true);

	m_bFocused = FALSE;
	UpdateCaret();
	if (m_ptSelStart != m_ptSelEnd)
		InvalidateLines(m_ptSelStart.y, m_ptSelEnd.y);
	if (m_bDragSelection)
	{
		ReleaseCapture();
		KillTimer(m_nDragSelTimer);
		m_bDragSelection = FALSE;
	}
}

void CCrystalTextView::OnSysColorChange() 
{
	CView::OnSysColorChange();
	Invalidate();
}

void CCrystalTextView::GetText(const CPoint &ptStart, const CPoint &ptEnd, CString &text)
{
	if (m_pTextBuffer != NULL)
		m_pTextBuffer->GetText(ptStart.y, ptStart.x, ptEnd.y, ptEnd.x, text);
	else
		text = _T("");
}

void CCrystalTextView::UpdateView(CCrystalTextView *pSource, CUpdateContext *pContext,
								  DWORD dwFlags, int nLineIndex /*= -1*/)
{
	if (dwFlags & UPDATE_RESET)
	{
		ResetView();
		RecalcVertScrollBar();
		RecalcHorzScrollBar();
		return;
	}

	int nLineCount = GetLineCount();
	ASSERT(nLineCount > 0);
	ASSERT(nLineIndex >= -1 && nLineIndex < nLineCount);
	if ((dwFlags & UPDATE_LINEATTR) != 0)
	{
		// Just this line needs painting
		ASSERT( (dwFlags & UPDATE_SINGLELINE) == 0 ); // Mutally exclusive update commands
		InvalidateLines(nLineIndex, nLineIndex, FALSE);
	}
	if ((dwFlags & UPDATE_SINGLELINE) != 0)
	{
		ASSERT(nLineIndex != -1);
		//	All text below this line should be reparsed
		if (m_pdwParseCookies != NULL)
		{
			ASSERT(m_nParseArraySize == nLineCount);
			memset(m_pdwParseCookies + nLineIndex, 0xff, sizeof(DWORD) * (m_nParseArraySize - nLineIndex));
		}
		//	This line'th actual length must be recalculated
		if (m_pnActualLineLength != NULL)
		{
			ASSERT(m_nActualLengthArraySize == nLineCount);
			m_pnActualLineLength[nLineIndex] = -1;
			//BEGIN SW
			InvalidateLineCache( nLineIndex, nLineIndex );
			//END SW
		}
		//	Repaint the lines
		InvalidateLines(nLineIndex, -1, TRUE);
	}
	else
	{
		if (nLineIndex == -1)
			nLineIndex = 0;		//	Refresh all text
		//	All text below this line should be reparsed
		if (m_pdwParseCookies != NULL)
		{
			if (m_nParseArraySize != nLineCount)
			{
				//	Reallocate cookies array
				DWORD *pdwNewArray = new DWORD[nLineCount];
				if (nLineIndex > 0)
					memcpy(pdwNewArray, m_pdwParseCookies, sizeof(DWORD) * nLineIndex);
				delete [] m_pdwParseCookies;
				m_nParseArraySize = nLineCount;
				m_pdwParseCookies = pdwNewArray;
			}
			memset(m_pdwParseCookies + nLineIndex, 0xff, sizeof(DWORD) * (m_nParseArraySize - nLineIndex));
		}
		//	Recalculate actual length for all lines below this
		if (m_pnActualLineLength != NULL)
		{
			if (m_nActualLengthArraySize != nLineCount)
			{
				//	Reallocate actual length array
				int *pnNewArray = new int[nLineCount];
				if (nLineIndex > 0)
					memcpy(pnNewArray, m_pnActualLineLength, sizeof(int) * nLineIndex);
				delete [] m_pnActualLineLength;
				m_nActualLengthArraySize = nLineCount;
				m_pnActualLineLength = pnNewArray;
			}
			memset(m_pnActualLineLength + nLineIndex, 0xff, sizeof(DWORD) * (m_nActualLengthArraySize - nLineIndex));
		}
		//BEGIN SW
		InvalidateLineCache( nLineIndex, -1 );
		//END SW
		//	Repaint the lines
		InvalidateLines(nLineIndex, -1, TRUE);
	}

	//	All those points must be recalculated and validated
	if (pContext != NULL)
	{
		pContext->RecalcPoint(m_ptCursorPos);
		pContext->RecalcPoint(m_ptSelStart);
		pContext->RecalcPoint(m_ptSelEnd);
		pContext->RecalcPoint(m_ptAnchor);
		ASSERT_VALIDTEXTPOS(m_ptCursorPos);
		ASSERT_VALIDTEXTPOS(m_ptSelStart);
		ASSERT_VALIDTEXTPOS(m_ptSelEnd);
		ASSERT_VALIDTEXTPOS(m_ptAnchor);
		if (m_bDraggingText)
		{
			pContext->RecalcPoint(m_ptDraggedTextBegin);
			pContext->RecalcPoint(m_ptDraggedTextEnd);
			ASSERT_VALIDTEXTPOS(m_ptDraggedTextBegin);
			ASSERT_VALIDTEXTPOS(m_ptDraggedTextEnd);
		}
		CPoint ptTopLine(0, m_nTopLine);
		pContext->RecalcPoint(ptTopLine);
		ASSERT_VALIDTEXTPOS(ptTopLine);
		m_nTopLine = ptTopLine.y;
		UpdateCaret();
	}

	//	Recalculate vertical scrollbar, if needed
	if ((dwFlags & UPDATE_VERTRANGE) != 0)
	{
		if (! m_bVertScrollBarLocked)
			RecalcVertScrollBar();
	}

	//	Recalculate horizontal scrollbar, if needed
	if ((dwFlags & UPDATE_HORZRANGE) != 0)
	{
		m_nMaxLineLength = -1;
		if (! m_bHorzScrollBarLocked)
			RecalcHorzScrollBar();
	}
}

HINSTANCE CCrystalTextView::GetResourceHandle()
{
#ifdef CRYSEDIT_RES_HANDLE
	return CRYSEDIT_RES_HANDLE;
#else
	if (s_hResourceInst != NULL)
		return s_hResourceInst;
	return AfxGetResourceHandle();
#endif
}

int CCrystalTextView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	memset(&m_lfBaseFont, 0, sizeof(m_lfBaseFont));
	lstrcpy(m_lfBaseFont.lfFaceName, _T("FixedSys"));
	m_lfBaseFont.lfHeight = 0;
	m_lfBaseFont.lfWeight = FW_NORMAL;
	m_lfBaseFont.lfItalic = FALSE;
	m_lfBaseFont.lfCharSet = DEFAULT_CHARSET;
	m_lfBaseFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	m_lfBaseFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	m_lfBaseFont.lfQuality = DEFAULT_QUALITY;
	m_lfBaseFont.lfPitchAndFamily = DEFAULT_PITCH;

	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	VERIFY(m_TextAttributeImages.Create(IDB_TEXT_ATTRIBUTE, 16, 0, RGB(255, 255, 255)));

	ASSERT(m_hAccel == NULL);
	m_hAccel = ::LoadAccelerators(GetResourceHandle(), MAKEINTRESOURCE(IDR_DEFAULT_ACCEL));
	ASSERT(m_hAccel != NULL);
	return 0;
}

void CCrystalTextView::SetAnchor(const CPoint &ptNewAnchor)
{
	ASSERT_VALIDTEXTPOS(ptNewAnchor);
	m_ptAnchor = ptNewAnchor;
}

void CCrystalTextView::OnEditOperation(int nAction, LPCTSTR pszText)
{
}

BOOL CCrystalTextView::PreTranslateMessage(MSG *pMsg) 
{
	// catch caret blink message and hide it if we should not blink
	if (pMsg->message==0x0118 && GetCaretMode()==CARET_STATIC)
		return TRUE;
	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	{
		if (m_hAccel != NULL)
		{
			if (::TranslateAccelerator(m_hWnd, m_hAccel, pMsg))
				return TRUE;
		}
	}

	return CView::PreTranslateMessage(pMsg);
}

CPoint CCrystalTextView::GetCursorPos()
{
	return m_ptCursorPos;
}

void CCrystalTextView::SetCursorPos(const CPoint &ptCursorPos)
{
	ASSERT_VALIDTEXTPOS(ptCursorPos);
	m_ptCursorPos = ptCursorPos;
	m_ptAnchor = ptCursorPos;
	m_nIdealCharPos = CalculateActualOffset(m_ptCursorPos.y, m_ptCursorPos.x);
	UpdateCaret();

	if (!IsSelection()) {
		MarkPairStringTo(ptCursorPos, TRUE);
		Invalidate();
		UpdateWindow();
	}
	else
		UnmarkPairString();
}

void CCrystalTextView::SetSelectionMargin(BOOL bSelMargin)
{
	if (m_bSelMargin != bSelMargin)
	{
		m_bSelMargin = bSelMargin;
		if (::IsWindow(m_hWnd))
		{
			m_nScreenChars = -1;
			Invalidate();
			RecalcHorzScrollBar();
		}
	}
}

void CCrystalTextView::GetFont(LOGFONT &lf)
{
	lf = m_lfBaseFont;
}

void CCrystalTextView::SetFont(const LOGFONT &lf)
{
	m_lfBaseFont = lf;
	m_nScreenLines = -1;
	m_nScreenChars = -1;
	m_nCharWidth = -1;
	m_nLineHeight = -1;
	if (m_pCacheBitmap != NULL)
	{
		m_pCacheBitmap->DeleteObject();
		delete m_pCacheBitmap;
		m_pCacheBitmap = NULL;
	}
	for (int I = 0; I < 4; I ++)
	{
		if (m_apFonts[I] != NULL)
		{
			m_apFonts[I]->DeleteObject();
			delete m_apFonts[I];
			m_apFonts[I] = NULL;
		}
	}
	if (::IsWindow(m_hWnd))
	{
		RecalcVertScrollBar();
		RecalcHorzScrollBar();
		UpdateCaret();
		Invalidate();
	}
}

void CCrystalTextView::OnUpdateIndicatorPosition(CCmdUI* pCmdUI)
{
	ASSERT_VALIDTEXTPOS(m_ptCursorPos);
	CString	stat;
	stat.Format(_T("Ln %d, Col %d"), m_ptCursorPos.y + 1, m_ptCursorPos.x + 1);
	pCmdUI->SetText(stat);

	//BEGIN SW
	pCmdUI->Enable(TRUE);
	if (pCmdUI->m_pOther && IsWindow(pCmdUI->m_pOther->m_hWnd))
		OnUpdateStatusMessage(pCmdUI->m_pOther);
	//END SW
}

void CCrystalTextView::OnUpdateIndicatorCRLF(CCmdUI* pCmdUI)
{
	if (m_pTextBuffer != NULL)
	{
		int crlfMode = m_pTextBuffer->GetCRLFMode();
		switch (crlfMode)
		{
		case CRLF_STYLE_DOS:
			pCmdUI->SetText(_T("DOS"));
			pCmdUI->Enable(TRUE);
			break;
		case CRLF_STYLE_UNIX:
			pCmdUI->SetText(_T("UNIX"));
			pCmdUI->Enable(TRUE);
			break;
		case CRLF_STYLE_MAC:
			pCmdUI->SetText(_T("MAC"));
			pCmdUI->Enable(TRUE);
			break;
		default:
			pCmdUI->SetText(NULL);
			pCmdUI->Enable(FALSE);
		}
	}
	else
	{
		pCmdUI->SetText(NULL);
		pCmdUI->Enable(FALSE);
	}
}

void CCrystalTextView::OnToggleBookmark(UINT nCmdID)
{
	int nBookmarkID = nCmdID - ID_EDIT_TOGGLE_BOOKMARK0;
	ASSERT(nBookmarkID >= 0 && nBookmarkID <= 9);
	if (m_pTextBuffer != NULL)
	{
		DWORD dwFlags = GetLineFlags(m_ptCursorPos.y);
		DWORD dwMask = LF_BOOKMARK(nBookmarkID);
		m_pTextBuffer->SetLineFlag(m_ptCursorPos.y, dwMask, (dwFlags & dwMask) == 0);
	}
}

void CCrystalTextView::OnGoBookmark(UINT nCmdID)
{
	int nBookmarkID = nCmdID - ID_EDIT_GO_BOOKMARK0;
	ASSERT(nBookmarkID >= 0 && nBookmarkID <= 9);
	if (m_pTextBuffer != NULL)
	{
		int nLine = m_pTextBuffer->GetLineWithFlag(LF_BOOKMARK(nBookmarkID));
		if (nLine >= 0)
		{
			CPoint pt(0, nLine);
			ASSERT_VALIDTEXTPOS(pt);
			SetCursorPos(pt);
			SetSelection(pt, pt);
			SetAnchor(pt);
			EnsureVisible(pt);
		}
	}
}

void CCrystalTextView::OnClearBookmarks()
{
	if (m_pTextBuffer != NULL)
	{
		for (int nBookmarkID = 0; nBookmarkID <= 9; nBookmarkID++)
		{
			int nLine = m_pTextBuffer->GetLineWithFlag(LF_BOOKMARK(nBookmarkID));
			if (nLine >= 0)
			{
				m_pTextBuffer->SetLineFlag(nLine, LF_BOOKMARK(nBookmarkID), FALSE);
			}
		}
		
	}
}

void CCrystalTextView::ShowCursor()
{
	m_bCursorHidden = FALSE;
	UpdateCaret();
}

void CCrystalTextView::HideCursor()
{
	m_bCursorHidden = TRUE;
	UpdateCaret();
}

DROPEFFECT CCrystalTextView::GetDropEffect()
{
	return DROPEFFECT_COPY;
}

void CCrystalTextView::OnDropSource(DROPEFFECT de)
{
	ASSERT(de == DROPEFFECT_COPY);
}

HGLOBAL CCrystalTextView::PrepareDragData()
{
	PrepareSelBounds();
	if (m_ptDrawSelStart == m_ptDrawSelEnd)
		return NULL;

	CString	text;
	GetText(m_ptDrawSelStart, m_ptDrawSelEnd, text);
	HGLOBAL hData = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, lstrlen(text) + 1);
	if (hData == NULL)
		return NULL;

	LPSTR	pszData = (LPSTR) ::GlobalLock(hData);
	USES_CONVERSION;
	strcpy(pszData, T2A(text.GetBuffer(0)));
	text.ReleaseBuffer();
	::GlobalUnlock(hData);

	m_ptDraggedTextBegin = m_ptDrawSelStart;
	m_ptDraggedTextEnd = m_ptDrawSelEnd;
	return hData;
}

static int FindStringHelper(LPCTSTR pszFindWhere, LPCTSTR pszFindWhat, BOOL bWholeWord)
{
	ASSERT(pszFindWhere != NULL);
	ASSERT(pszFindWhat != NULL);
	int nCur = 0;
	int nLength = lstrlen(pszFindWhat);
	for (;;)
	{
#ifdef _UNICODE
		LPCTSTR	pszPos = wcsstr(pszFindWhere, pszFindWhat);
#else
		LPCTSTR	pszPos = strstr(pszFindWhere, pszFindWhat);
#endif
		if (pszPos == NULL)
			return -1;
		if (! bWholeWord)
			return nCur + (pszPos - pszFindWhere);
		if (pszPos > pszFindWhere && (IsAlNum(pszPos[-1]) || pszPos[-1] == _T('_')))
		{
			nCur += (pszPos - pszFindWhere);
			pszFindWhere = pszPos + 1;
			continue;
		}
		if (IsAlNum(pszPos[nLength]) || pszPos[nLength] == _T('_'))
		{
			nCur += (pszPos - pszFindWhere + 1);
			pszFindWhere = pszPos + 1;
			continue;
		}
		return nCur + (pszPos - pszFindWhere);
	}
	ASSERT(FALSE);		// Unreachable
	return -1;
}

BOOL CCrystalTextView::HighlightText(const CPoint &ptStartPos, int nLength /*= -1*/)
{
	ASSERT_VALIDTEXTPOS(ptStartPos);
	m_ptCursorPos = ptStartPos;
	if (nLength == -1)
		m_ptCursorPos.x = GetLineLength(ptStartPos.y); // Highlight to end of line
	else
	m_ptCursorPos.x += nLength;
	
	ASSERT_VALIDTEXTPOS(m_ptCursorPos);		//	Probably 'nLength' is bigger than expected...
	m_ptAnchor = m_ptCursorPos;
	SetSelection(ptStartPos, m_ptCursorPos);
	UpdateCaret();
	EnsureVisible(m_ptCursorPos);

	if (!IsSelection()) {
		MarkPairStringTo(m_ptCursorPos, FALSE);
		Invalidate();
		UpdateWindow();		
	}
	else
		UnmarkPairString();
	return TRUE;
}

BOOL CCrystalTextView::FindText(LPCTSTR pszText, const CPoint &ptStartPos, DWORD dwFlags, 
								BOOL bWrapSearch, CPoint *pptFoundPos)
{
	int nLineCount = GetLineCount();
	return FindTextInBlock(pszText, ptStartPos, CPoint(0, 0),
			CPoint(GetLineLength(nLineCount - 1), nLineCount - 1),
			dwFlags, bWrapSearch, pptFoundPos);
}

BOOL CCrystalTextView::FindTextInBlock(LPCTSTR pszText, const CPoint &ptStartPosition, const CPoint
									   &ptBlockBegin, const CPoint &ptBlockEnd, DWORD dwFlags, 
									   BOOL bWrapSearch, CPoint *pptFoundPos)
{
	CPoint ptCurrentPos = ptStartPosition;

	ASSERT(pszText != NULL && lstrlen(pszText) > 0);
	ASSERT_VALIDTEXTPOS(ptCurrentPos);
	ASSERT_VALIDTEXTPOS(ptBlockBegin);
	ASSERT_VALIDTEXTPOS(ptBlockEnd);
	ASSERT(ptBlockBegin.y < ptBlockEnd.y || ptBlockBegin.y == ptBlockEnd.y && 
		ptBlockBegin.x <= ptBlockEnd.x);
	if (ptBlockBegin == ptBlockEnd)
		return FALSE;

	if (ptCurrentPos.y < ptBlockBegin.y || ptCurrentPos.y == ptBlockBegin.y && 
		ptCurrentPos.x < ptBlockBegin.x)
		ptCurrentPos = ptBlockBegin;

	CString	what = pszText;
	if ((dwFlags & FIND_MATCH_CASE) == 0)
		what.MakeUpper();

	if (dwFlags & FIND_DIRECTION_UP)
	{
		//	Let's check if we deal with whole text.
		//	At this point, we cannot search *up* in selection
		ASSERT(ptBlockBegin.x == 0 && ptBlockBegin.y == 0);
		ASSERT(ptBlockEnd.x == GetLineLength(GetLineCount() - 1) &&
				ptBlockEnd.y == GetLineCount() - 1);

		//	Proceed as if we have whole text search.
		for (;;)
		{
			while (ptCurrentPos.y >= 0)
			{
				int nLineLength = GetLineLength(ptCurrentPos.y);
				//BEGIN SW
				if( ptCurrentPos.x >= nLineLength )
					ptCurrentPos.x = nLineLength - 1;
				//END SW

				LPCTSTR	pszChars = GetLineChars(ptCurrentPos.y);

				//BEGIN SW
				CString	line;
				lstrcpyn(line.GetBuffer(ptCurrentPos.x + 2), pszChars, ptCurrentPos.x + 1);
				//END SW
				line.ReleaseBuffer();
				if ((dwFlags & FIND_MATCH_CASE) == 0)
					line.MakeUpper();

				//BEGIN SW
				int nFoundPos = -1;
				int nMatchLen = what.GetLength();
				int nLineLen = line.GetLength();
				int nPos;
				do
				{
					nPos = ::FindStringHelper(line, what, (dwFlags & FIND_WHOLE_WORD) != 0);
					if( nPos >= 0 )
					{
						nFoundPos = (nFoundPos == -1)? nPos : nFoundPos + nPos;
						nFoundPos+= nMatchLen;
						line = line.Right( nLineLen - (nMatchLen + nPos) );
						nLineLen = line.GetLength();
					}
				}
				while( nPos >= 0 );

				if( nFoundPos >= 0 )	// Found text!
				{
					ptCurrentPos.x = nFoundPos - nMatchLen;
					*pptFoundPos = ptCurrentPos;
					return TRUE;
				}

				ptCurrentPos.y--;
				if( ptCurrentPos.y >= 0 )
					ptCurrentPos.x = GetLineLength( ptCurrentPos.y );
				//END SW
			}

			//	Beginning of text reached
			if (! bWrapSearch)
				return FALSE;

			//	Start again from the end of text
			bWrapSearch = FALSE;
			ptCurrentPos = CPoint(0, GetLineCount() - 1);
		}
	}
	else
	{
		for (;;)
		{
			while (ptCurrentPos.y <= ptBlockEnd.y)
			{
				int nLineLength = GetLineLength(ptCurrentPos.y);
				nLineLength -= ptCurrentPos.x;
				if (nLineLength <= 0)
				{
					ptCurrentPos.x = 0;
					ptCurrentPos.y ++;
					continue;
				}

				LPCTSTR	pszChars = GetLineChars(ptCurrentPos.y);
				pszChars += ptCurrentPos.x;

				//	Prepare necessary part of line
				CString	line;
				lstrcpyn(line.GetBuffer(nLineLength + 1), pszChars, nLineLength + 1);
				line.ReleaseBuffer();
				if ((dwFlags & FIND_MATCH_CASE) == 0)
					line.MakeUpper();

				//	Perform search in the line
				int nPos = ::FindStringHelper(line, what, (dwFlags & FIND_WHOLE_WORD) != 0);
				if (nPos >= 0)
				{
					ptCurrentPos.x += nPos;
					//	Check of the text found is outside the block.
					if (ptCurrentPos.y == ptBlockEnd.y && ptCurrentPos.x >= ptBlockEnd.x)
						break;

					*pptFoundPos = ptCurrentPos;
					return TRUE;
				}

				//	Go further, text was not found
				ptCurrentPos.x = 0;
				ptCurrentPos.y ++;
			}

			//	End of text reached
			if (! bWrapSearch)
				return FALSE;

			//	Start from the beginning
			bWrapSearch = FALSE;
			ptCurrentPos = ptBlockBegin;
		}
	}

	ASSERT(FALSE);		// Unreachable
	return FALSE;
}

void CCrystalTextView::OnEditFind() 
{
	CCrystalResources	cr;

	CWinApp *pApp = AfxGetApp();
	ASSERT(pApp != NULL);

	CFindTextDlg dlg(this);
	if (m_bLastSearch)
	{
		//	Get the latest search parameters
		dlg.m_bMatchCase = (m_dwLastSearchFlags & FIND_MATCH_CASE) != 0;
		dlg.m_bWholeWord = (m_dwLastSearchFlags & FIND_WHOLE_WORD) != 0;
		dlg.m_nDirection = (m_dwLastSearchFlags & FIND_DIRECTION_UP) != 0 ? 0 : 1;
		if (m_pszLastFindWhat != NULL)
			dlg.m_sText = m_pszLastFindWhat;
	}
	else
	{
		//	Take search parameters from registry
		dlg.m_bMatchCase = pApp->GetProfileInt(REG_FIND_SUBKEY, REG_MATCH_CASE, FALSE);
		dlg.m_bWholeWord = pApp->GetProfileInt(REG_FIND_SUBKEY, REG_WHOLE_WORD, FALSE);
		dlg.m_nDirection = 1;		//	Search down
		dlg.m_sText = pApp->GetProfileString(REG_FIND_SUBKEY, REG_FIND_WHAT, _T(""));
	}

	//	Take the current selection, if any
	if (IsSelection())
	{
		CPoint ptSelStart, ptSelEnd;
		GetSelection(ptSelStart, ptSelEnd);		if (ptSelStart.y == ptSelEnd.y)
		{
			LPCTSTR pszChars = GetLineChars(ptSelStart.y);
			int nChars = ptSelEnd.x - ptSelStart.x;
			lstrcpyn(dlg.m_sText.GetBuffer(nChars + 1), pszChars + ptSelStart.x, nChars + 1);
			dlg.m_sText.ReleaseBuffer();
		}
	}

	//	Execute Find dialog
	dlg.m_ptCurrentPos = m_ptCursorPos;		//	Search from cursor position
	m_bShowInactiveSelection = TRUE;
	dlg.DoModal();
	m_bShowInactiveSelection = FALSE;

	//	Save search parameters for 'F3' command
	m_bLastSearch = TRUE;
	if (m_pszLastFindWhat != NULL)
		free(m_pszLastFindWhat);
#ifdef _UNICODE
	m_pszLastFindWhat = _wcsdup(dlg.m_sText);
#else
	m_pszLastFindWhat = strdup(dlg.m_sText);
#endif

	m_dwLastSearchFlags = 0;
	if (dlg.m_bMatchCase)
		m_dwLastSearchFlags |= FIND_MATCH_CASE;
	if (dlg.m_bWholeWord)
		m_dwLastSearchFlags |= FIND_WHOLE_WORD;
	if (dlg.m_nDirection == 0)
		m_dwLastSearchFlags |= FIND_DIRECTION_UP;

	//	Save search parameters to registry
	pApp->WriteProfileInt(REG_FIND_SUBKEY, REG_MATCH_CASE, dlg.m_bMatchCase);
	pApp->WriteProfileInt(REG_FIND_SUBKEY, REG_WHOLE_WORD, dlg.m_bWholeWord);
	pApp->WriteProfileString(REG_FIND_SUBKEY, REG_FIND_WHAT, dlg.m_sText);
}

void CCrystalTextView::OnEditRepeat() 
{
	if (m_bLastSearch)
	{
		CPoint ptFoundPos;
		//BEGIN SW
		// for correct backward search we need some changes:
		CPoint ptSearchPos = m_ptCursorPos;
		if( m_dwLastSearchFlags & FIND_DIRECTION_UP && IsSelection() )
		{
			CPoint ptDummy;
			GetSelection( ptSearchPos, ptDummy );
		}

		if (! FindText(m_pszLastFindWhat, ptSearchPos, m_dwLastSearchFlags, TRUE, &ptFoundPos))
		///END SW
		{
			CCrystalResources	cr;

			CString	prompt;
			prompt.Format(IDS_EDIT_TEXT_NOT_FOUND, m_pszLastFindWhat);
			AfxMessageBox(prompt);
			return;
		}
		HighlightText(ptFoundPos, lstrlen(m_pszLastFindWhat));
		m_bMultipleSearch = TRUE;       // More search       
	}
}

void CCrystalTextView::OnUpdateEditRepeat(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bLastSearch);
}

void CCrystalTextView::OnEditFindPrevious() 
{
	DWORD dwSaveSearchFlags = m_dwLastSearchFlags;
	if ((m_dwLastSearchFlags & FIND_DIRECTION_UP) != 0)
		m_dwLastSearchFlags &= ~FIND_DIRECTION_UP;
	else
		m_dwLastSearchFlags |= FIND_DIRECTION_UP;
	OnEditRepeat();
	m_dwLastSearchFlags = dwSaveSearchFlags;
}

void CCrystalTextView::OnUpdateEditFindPrevious(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bLastSearch);
}

void CCrystalTextView::OnFilePageSetup()
{
	CWinApp *pApp = AfxGetApp();
	ASSERT(pApp != NULL);

	CPageSetupDialog dlg;
	dlg.m_psd.Flags &= ~PSD_INTHOUSANDTHSOFINCHES;
	dlg.m_psd.Flags |= PSD_INHUNDREDTHSOFMILLIMETERS | PSD_DISABLEORIENTATION | PSD_DISABLEPAPER;
	dlg.m_psd.rtMargin.left = pApp->GetProfileInt(REG_PAGE_SUBKEY, REG_MARGIN_LEFT, DEFAULT_PRINT_MARGIN);
	dlg.m_psd.rtMargin.right = pApp->GetProfileInt(REG_PAGE_SUBKEY, REG_MARGIN_RIGHT, DEFAULT_PRINT_MARGIN);
	dlg.m_psd.rtMargin.top = pApp->GetProfileInt(REG_PAGE_SUBKEY, REG_MARGIN_TOP, DEFAULT_PRINT_MARGIN);
	dlg.m_psd.rtMargin.bottom = pApp->GetProfileInt(REG_PAGE_SUBKEY, REG_MARGIN_BOTTOM, DEFAULT_PRINT_MARGIN);
	if (dlg.DoModal() == IDOK)
	{
		pApp->WriteProfileInt(REG_PAGE_SUBKEY, REG_MARGIN_LEFT, dlg.m_psd.rtMargin.left);
		pApp->WriteProfileInt(REG_PAGE_SUBKEY, REG_MARGIN_RIGHT, dlg.m_psd.rtMargin.right);
		pApp->WriteProfileInt(REG_PAGE_SUBKEY, REG_MARGIN_TOP, dlg.m_psd.rtMargin.top);
		pApp->WriteProfileInt(REG_PAGE_SUBKEY, REG_MARGIN_BOTTOM, dlg.m_psd.rtMargin.bottom);
	}
}

void CCrystalTextView::OnToggleBookmark()
{
	if (m_pTextBuffer != NULL)
	{
		DWORD dwFlags = GetLineFlags(m_ptCursorPos.y);
		DWORD dwMask  = LF_BOOKMARKS;
		m_pTextBuffer->SetLineFlag(m_ptCursorPos.y, dwMask, (dwFlags & dwMask) == 0, FALSE);
	}
	int nLine = m_pTextBuffer->GetLineWithFlag(LF_BOOKMARKS);
	if (nLine >= 0)
		m_bBookmarkExist = TRUE;
	else
		m_bBookmarkExist = FALSE;
}

void CCrystalTextView::OnNextBookmark()
{
	if (m_pTextBuffer != NULL)
	{
		int nLine = m_pTextBuffer->FindNextBookmarkLine(m_ptCursorPos.y);
		if (nLine >= 0)
		{
			CPoint pt(0, nLine);
			ASSERT_VALIDTEXTPOS(pt);
			SetCursorPos(pt);
			SetSelection(pt, pt);
			SetAnchor(pt);
			EnsureVisible(pt);
		}
	}
}

void CCrystalTextView::OnPrevBookmark()
{
	if (m_pTextBuffer != NULL)
	{
		int nLine = m_pTextBuffer->FindPrevBookmarkLine(m_ptCursorPos.y);
		if (nLine >= 0)
		{
			CPoint pt(0, nLine);
			ASSERT_VALIDTEXTPOS(pt);
			SetCursorPos(pt);
			SetSelection(pt, pt);
			SetAnchor(pt);
			EnsureVisible(pt);
		}
	}
}

void CCrystalTextView::OnClearAllBookmarks()
{
	if (m_pTextBuffer != NULL)
	{
		int nLineCount = GetLineCount();
		for (int I = 0; I < nLineCount; I ++)
		{
			if (m_pTextBuffer->GetLineFlags(I) & LF_BOOKMARKS)
				m_pTextBuffer->SetLineFlag(I, LF_BOOKMARKS, FALSE);
		}
		m_bBookmarkExist = FALSE;
	}
}

void CCrystalTextView::OnUpdateNextBookmark(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bBookmarkExist);
}

void CCrystalTextView::OnUpdatePrevBookmark(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bBookmarkExist);
}

void CCrystalTextView::OnUpdateClearAllBookmarks(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bBookmarkExist);
}

BOOL CCrystalTextView::GetViewTabs()
{
	return m_bViewTabs;
}

void CCrystalTextView::SetViewTabs(BOOL bViewTabs)
{
	if (bViewTabs != m_bViewTabs)
	{
		m_bViewTabs = bViewTabs;
		if (::IsWindow(m_hWnd))
			Invalidate();
	}
}

BOOL CCrystalTextView::GetSelectionMargin()
{
	return m_bSelMargin;
}

int CCrystalTextView::GetMarginWidth()
{
	int w = 16, cw; // default

	if (m_bShowLineNumbers) {
		int nLines = GetLineCount();		
		int nMax = ceil(log10(nLines < 10 ? 10 : nLines));

		CDC *pDC = GetDC();
		CFont *pOldFont = pDC->SelectObject(GetFont());
		CSize s = pDC->GetTextExtent("9");		
		pDC->SelectObject(pOldFont);
		cw = s.cx + 1;	
		w =  nMax * cw;

		if (w < 20) w = 20; // ensure minimum width
	}

	return m_bSelMargin ? w + 4 : 1;
}

BOOL CCrystalTextView::GetSmoothScroll() const
{
	return m_bSmoothScroll;
}

void CCrystalTextView::SetSmoothScroll(BOOL bSmoothScroll)
{
	m_bSmoothScroll = bSmoothScroll;
}

//	[JRT]
BOOL CCrystalTextView::GetDisableDragAndDrop() const
{
	return m_bDisableDragAndDrop;
}

//	[JRT]
void CCrystalTextView::SetDisableDragAndDrop(BOOL bDDAD)
{
	m_bDisableDragAndDrop = bDDAD;
}


//BEGIN SW
BOOL CCrystalTextView::GetWordWrapping() const
{
	return m_bWordWrap;
}

void CCrystalTextView::SetWordWrapping( BOOL bWordWrap )
{
	m_bWordWrap = bWordWrap;

	if( IsWindow( m_hWnd ) )
		InvalidateLines( 0, -1, TRUE );
}

CCrystalParser *CCrystalTextView::SetParser( CCrystalParser *pParser )
{
	CCrystalParser	*pOldParser = m_pParser;

	m_pParser = pParser;

	if( pParser )
		pParser->m_pTextView = this;

	return pOldParser;
}
//END SW


CCrystalParser *CCrystalTextView::GetParser() const
{
	return m_pParser;
}


//BEGIN SW
// incremental search imlementation
BOOL CCrystalTextView::OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO *pHandlerInfo )
{
	// just look for commands
	if( nCode != CN_COMMAND || pExtra )
		return CView::OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );

	// handle code:
	// each command that is not related to incremental search
	// ends the incremental search
	if( nID == ID_EDIT_FIND_INCREMENTAL_FORWARD || 
		nID == ID_EDIT_FIND_INCREMENTAL_BACKWARD || 
		nID == ID_EDIT_DELETE_BACK )
		return CView::OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );

	if ( nID >= ID_EDIT_FIRST && nID <= ID_EDIT_LAST ) OnEditFindIncrementalStop(true);

	return CView::OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
}

void CCrystalTextView::OnChar( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	CView::OnChar( nChar, nRepCnt, nFlags );

	// we only have to handle character-input, if we are in incremental search mode
	if( !m_bIncrementalSearchForward && !m_bIncrementalSearchBackward )
		return;

	// exit incremental search, if Escape is pressed
	if( nChar == VK_ESCAPE )
	{
		OnEditFindIncrementalStop(false);
		return;
	}

	// exit incremental search without destroying selection, if Return was pressed
	if( nChar == VK_RETURN )
	{
		OnEditFindIncrementalStop(true);
		return;
	}

	//Is the character valid for incremental search?
	//
	// - Although I think, that _istgraph() should return true for german umlauts
	// - and french accentuated characters it does NOT.
	// - Therefore IsAlNum() is used here as well - it will return true for them.
	// - Both funcs overlap somehow, but that shouldn't matter that much.
	//
	// - I found one character, where the incremental search still cancels:
	// - The Euro-Symbol
	//
	// - The Tabulator key is not supported as well,
	// - because pressing this key does not bring up the OnChar-Event.
	if( !( IsGraph( nChar ) || IsAlNum(nChar) ) && !(nChar == _T(' ')) /*&& !(nChar == _T('\t'))*/ )
	{
		//If not valid, then end incremental search
		OnEditFindIncrementalStop(true);
		return;
	}

	// if last search was not successfull do not add a new character
	if( !m_bIncrementalFound )
	{
		MessageBeep( MB_OK );
		return;
	}

	// add character to incremental search string and search
	*m_pstrIncrementalSearchString+= nChar;
	OnEditFindIncremental();
	
}


void CCrystalTextView::OnEditDeleteBack() 
{
	if( !m_bIncrementalSearchForward && !m_bIncrementalSearchBackward )
		return;

	// remove last character from search string
	if( m_pstrIncrementalSearchString->IsEmpty() )
		return;

	*m_pstrIncrementalSearchString = m_pstrIncrementalSearchString->Left( m_pstrIncrementalSearchString->GetLength() - 1 );
	OnEditFindIncremental();
}


void CCrystalTextView::OnEditFindIncremental( BOOL bFindNextOccurence /*= FALSE*/ )
{
	// when string is empty, then goto position where the search starts
	if( m_pstrIncrementalSearchString->IsEmpty() )
	{
		SetSelection( m_incrementalSearchStartPos, m_incrementalSearchStartPos );
		SetCursorPos( m_incrementalSearchStartPos );
		EnsureVisible( m_incrementalSearchStartPos );
		return;
	}

	// otherwise search next occurence of search string, 
	// starting at current cursor position
	CPoint matchStart, matchEnd;

	// calculate start point for search
	if( bFindNextOccurence )
	{
		CPoint selStart, selEnd;
		GetSelection( selStart, selEnd );
		m_incrementalSearchStartPos = (m_bIncrementalSearchBackward)? selStart : selEnd;
	}

	m_bIncrementalFound = FindText( 
		*m_pstrIncrementalSearchString,
		m_incrementalSearchStartPos,
		m_bIncrementalSearchBackward? FIND_DIRECTION_UP : 0,
		TRUE,
		&matchStart );

	if( !m_bIncrementalFound )
	{
		MessageBeep( MB_OK );
		return;
	}

	// select found text and set cursor to end of match
	matchEnd = matchStart;
	matchEnd.x+= m_pstrIncrementalSearchString->GetLength();
	SetSelection( matchStart, matchEnd );
	SetCursorPos( matchEnd );
	EnsureVisible( matchEnd );
}


bool CCrystalTextView::OnEditFindIncrementalStop(bool bKeepSelection)
{
	if (!m_bIncrementalSearchForward && !m_bIncrementalSearchBackward)
		return false; //Not in search mode.

	//Stop search.
	m_bIncrementalSearchForward = m_bIncrementalSearchBackward = FALSE;

	//Cancel it? I.e., return to original place?
	if (!bKeepSelection)
	{
		SetSelection(m_selStartBeforeIncrementalSearch, m_selEndBeforeIncrementalSearch);
		SetCursorPos(m_cursorPosBeforeIncrementalSearch);
		EnsureVisible(m_cursorPosBeforeIncrementalSearch);
	}

	return true;
}


void CCrystalTextView::OnEditFindIncrementalForward()
{
	if( !m_bIncrementalSearchForward && !m_bIncrementalSearchBackward )
	{
		// initialize
		if( !m_pstrIncrementalSearchString->IsEmpty() )
			*m_pstrIncrementalSearchStringOld = *m_pstrIncrementalSearchString;
		m_pstrIncrementalSearchString->Empty();
		m_incrementalSearchStartPos = m_cursorPosBeforeIncrementalSearch = m_ptCursorPos;
		GetSelection( m_selStartBeforeIncrementalSearch, m_selEndBeforeIncrementalSearch );
	}
	else if( m_bIncrementalSearchForward )
	{
		if( m_pstrIncrementalSearchString->IsEmpty() )
		{
			*m_pstrIncrementalSearchString = *m_pstrIncrementalSearchStringOld;
			m_pstrIncrementalSearchStringOld->Empty();
			OnEditFindIncremental();
		}
		else
			OnEditFindIncremental( TRUE );

		return;
	}

	m_bIncrementalSearchForward = TRUE;
	m_bIncrementalSearchBackward = FALSE;
	m_bIncrementalFound = TRUE;
	OnEditFindIncremental();
}

void CCrystalTextView::OnEditFindIncrementalBackward()
{
	if( !m_bIncrementalSearchForward && !m_bIncrementalSearchBackward )
	{
		// initialize
		if( !m_pstrIncrementalSearchString->IsEmpty() )
			*m_pstrIncrementalSearchStringOld = *m_pstrIncrementalSearchString;
		m_pstrIncrementalSearchString->Empty();
		GetSelection( m_selStartBeforeIncrementalSearch, m_selEndBeforeIncrementalSearch );
		m_incrementalSearchStartPos = m_cursorPosBeforeIncrementalSearch = m_ptCursorPos;
	}
	else if( m_bIncrementalSearchBackward )
	{
		if( m_pstrIncrementalSearchString->IsEmpty() )
		{
			*m_pstrIncrementalSearchString = *m_pstrIncrementalSearchStringOld;
			m_pstrIncrementalSearchStringOld->Empty();
			OnEditFindIncremental();
		}
		else
			OnEditFindIncremental( TRUE );

		return;
	}

	m_bIncrementalSearchForward = FALSE;
	m_bIncrementalSearchBackward = TRUE;
	m_bIncrementalFound = TRUE;
	OnEditFindIncremental();
}

void CCrystalTextView::OnUpdateStatusMessage(CWnd *pStatusBar)
{
	CCrystalResources	cr;

	static BOOL	bUpdatedAtLastCall = FALSE;

	if (!m_bIncrementalSearchForward && !m_bIncrementalSearchBackward)
	{
		if (bUpdatedAtLastCall)
			OnSetStatusMessage(pStatusBar, CString((LPCTSTR)AFX_IDS_IDLEMESSAGE));

		bUpdatedAtLastCall = FALSE;

		return;
	}

	CString	strFormat;
	if(!m_bIncrementalFound)
		strFormat.Format(IDS_FIND_INCREMENTAL_FAILED, *m_pstrIncrementalSearchString);
	else if (m_bIncrementalSearchForward)
		strFormat.Format(IDS_FIND_INCREMENTAL_FORWARD, *m_pstrIncrementalSearchString);
	else if (m_bIncrementalSearchBackward)
		strFormat.Format(IDS_FIND_INCREMENTAL_BACKWARD, *m_pstrIncrementalSearchString);

	if (strFormat.IsEmpty())
		return;

	OnSetStatusMessage(pStatusBar, strFormat);
	bUpdatedAtLastCall = TRUE;
}


void CCrystalTextView::OnSetStatusMessage(CWnd *pStatusBar, LPCTSTR lpszMessage)
{
	if (pStatusBar->IsKindOf(RUNTIME_CLASS(CStatusBar)))
		((CStatusBar*)(pStatusBar))->SetPaneText(0, lpszMessage);
}


void CCrystalTextView::GetSelectedText(CString &strSelection)
{
	CPoint ptStart, ptEnd;
	GetSelection(ptStart, ptEnd);

	if (ptStart != ptEnd)
		GetText(ptStart, ptEnd, strSelection);
	else
	{
		// retrieve the word, the cursor is placed on
		CString	strLine(GetLineChars(ptStart.y));

		//If the cursor is placed at the end of the line ==> step one char back.
		// (If this EOL is not catched here, it will assert in the
		//  first run of the next loop - strLine[nStartChar])
		//If the cursor is placed at the end of a word, step one char back.
		int nStartChar = ptStart.x;
		if ( (nStartChar >= strLine.GetLength()) || (!IsAlNum(strLine[nStartChar])) )
		{
			nStartChar--;
		}

		// retrieve position of first character of the current word
		for (; nStartChar >= 0; nStartChar--)
		{
			if (!IsAlNum(strLine[nStartChar]))
			{
				nStartChar++;
				break;
			}
		}
		if (nStartChar < 0)
			nStartChar = 0;
		
		// retrieve position of first character not belonging to the current word
		for (int nEndChar = ptStart.x; nEndChar < strLine.GetLength(); nEndChar++)
		{
			if (!IsAlNum(strLine[nEndChar]))
				break;
		}

		if (nEndChar <= nStartChar)
			strSelection.Empty();
		else
			strSelection = strLine.Mid(nStartChar, nEndChar-nStartChar);
	}
}


BOOL CCrystalTextView::GetOverwriteMode() const
{
	return FALSE;
}


void CCrystalTextView::SetCaretInsertStyle(int nForm, int nMode)
{
	s_nCaretInsertForm = nForm;
	s_nCaretInsertMode = nMode;
}


void CCrystalTextView::SetCaretOverwriteStyle(int nForm, int nMode)
{
	s_nCaretOverwriteForm = nForm;
	s_nCaretOverwriteMode = nMode;
}


int CCrystalTextView::GetCaretInsertForm()
{
	return s_nCaretInsertForm;
}


int CCrystalTextView::GetCaretInsertMode()
{
	return s_nCaretInsertMode;
}


int CCrystalTextView::GetCaretOverwriteForm()
{
	return s_nCaretOverwriteForm;
}


int CCrystalTextView::GetCaretOverwriteMode()
{
	return s_nCaretOverwriteMode;
}


int CCrystalTextView::GetCaretForm()
{
	if (GetOverwriteMode())
		return GetCaretOverwriteForm();
	else
		return GetCaretInsertForm();
}


int CCrystalTextView::GetCaretMode()
{
	if (GetOverwriteMode())
		return GetCaretOverwriteMode();
	else
		return GetCaretInsertMode();
}

//END SW

BOOL CCrystalTextView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	/* zDelta is 120 per unit of wheel movement. Future devices may use smaller
	increments, in which case CCrystalTextView will have to accumilate zDelta until
	a full line is reached. 
	*/

	int nScrollTo = m_nTopSubLine;

	nScrollTo -= zDelta / 40; // three lines

	if ( nScrollTo < 0 )
		nScrollTo = 0;
	else if ( nScrollTo >= GetSubLineCount() )
		nScrollTo = GetSubLineCount() - 1;

	ScrollToSubLine(nScrollTo);
	return TRUE;
}


int CCrystalTextView::Hold( bool bExclusive /*= false */ )
{
	ASSERT( m_nHoldCount >= 0 );
	if ( bExclusive )
	{
		while ( true )
		{
			::WaitForSingleObject( *m_pevtHoldCountZero, INFINITE );
			::EnterCriticalSection( &m_csHold );
			if ( m_nHoldCount == 0)
				break;
			::LeaveCriticalSection( &m_csHold );
		}
		m_nHoldCount = 1;
	}
	else
	{
		::EnterCriticalSection( &m_csHold );
		m_pevtHoldCountZero->ResetEvent();
		++m_nHoldCount;
		::LeaveCriticalSection( &m_csHold );
	}
	ASSERT( m_nHoldCount > 0 );
	return m_nHoldCount;
}


int CCrystalTextView::Release( bool bExclusive /*= false */ )
{
	ASSERT( m_nHoldCount > 0 );
	int nHoldCount;
	if ( bExclusive )
	{
		// Hold( true ) was not called
		ASSERT( m_nHoldCount == 1 );
		nHoldCount = m_nHoldCount = 0;
		m_pevtHoldCountZero->SetEvent();
		::LeaveCriticalSection( &m_csHold );
	}
	else
	{
		::EnterCriticalSection( &m_csHold );
		nHoldCount = --m_nHoldCount;
		if ( m_nHoldCount == 0 )
			m_pevtHoldCountZero->SetEvent();
		::LeaveCriticalSection( &m_csHold );
	}
	return nHoldCount;
}


BOOL CCrystalTextView::MarkPairStringTo( const CPoint &ptCursorPos, BOOL bMarkBlock )
{
	CPoint ptCurStrStart, ptCurStrEnd;
	CPoint ptFoundStrStart, ptFoundStrEnd;
	BOOL bPairFound;

	if( m_pParser == NULL )
		return FALSE;

	if ( !FindPairTo(ptCursorPos, ptCurStrStart, ptCurStrEnd,
															ptFoundStrStart, ptFoundStrEnd, bPairFound ) )
	{
		UnmarkPairString();
		//SetPairStringStart( ptFoundStrStart );
		//SetPairStringEnd( ptFoundStrEnd );
		SetCurStringStart( ptCurStrStart );
		SetCurStringEnd( ptCurStrEnd );
		SetCurStringColorBkgnd( CCrystalTextView::COLORINDEX_BADPAIRSTRINGBKGND );
		SetCurStringColorText( CCrystalTextView::COLORINDEX_BADPAIRSTRINGTEXT );
		return FALSE;
	}

	if ( bPairFound ) 
	{
		SetPairStringStart( ptFoundStrStart );
		SetPairStringEnd( ptFoundStrEnd );
		SetPairStringColorBkgnd( CCrystalTextView::COLORINDEX_PAIRSTRINGBKGND );
		SetPairStringColorText( CCrystalTextView::COLORINDEX_PAIRSTRINGTEXT );

		SetCurStringStart( ptCurStrStart );
		SetCurStringEnd( ptCurStrEnd );
		SetCurStringColorBkgnd( CCrystalTextView::COLORINDEX_CURPAIRSTRINGBKGND );
		SetCurStringColorText( CCrystalTextView::COLORINDEX_CURPAIRSTRINGTEXT );

		if ( bMarkBlock ) 
			SetDrawPairBlock( TRUE );
		else
			SetDrawPairBlock( FALSE );

	}
	else {
		UnmarkPairString();	
	}
		
	return TRUE;
}


void CCrystalTextView::UnmarkPairString()
{
	SetPairStringStart(CPoint(0,0));
	SetPairStringEnd(CPoint(0,0));
	SetPairStringColorText(0);
	SetPairStringColorBkgnd(0);

	SetCurStringStart(CPoint(0,0));
	SetCurStringEnd(CPoint(0,0));
	SetCurStringColorText(0);
	SetCurStringColorBkgnd(0);

	SetDrawPairBlock( FALSE );
}



CPoint CCrystalTextView::GetPairStringStart() const
{
	return m_ptPairStringStart;
}

CPoint CCrystalTextView::GetPairStringEnd() const
{
	return m_ptPairStringStart;
}

void CCrystalTextView::SetPairStringStart(const CPoint &ptStart)
{
	ASSERT_VALIDTEXTPOS(ptStart);
	m_ptPairStringStart = ptStart;
}

void CCrystalTextView::SetPairStringEnd(const CPoint &ptEnd)
{
	ASSERT_VALIDTEXTPOS(ptEnd);
	m_ptPairStringEnd = ptEnd;
}

int CCrystalTextView::GetPairStringColorBkgnd() const
{
	return m_nPairStringColorBkgnd;
}

void CCrystalTextView::SetPairStringColorBkgnd(int nColorIndex)
{
	m_nPairStringColorBkgnd = nColorIndex;
}

int CCrystalTextView::GetPairStringColorText() const
{
	return m_nPairStringColorText;
}

void CCrystalTextView::SetPairStringColorText(int nColorIndex)
{
	m_nPairStringColorText = nColorIndex;
}

CPoint CCrystalTextView::GetCurStringStart() const
{
	return m_ptCurStringStart;
}

CPoint CCrystalTextView::GetCurStringEnd() const
{
	return m_ptCurStringStart;
}

void CCrystalTextView::SetCurStringStart(const CPoint &ptStart)
{
	ASSERT_VALIDTEXTPOS(ptStart);
	m_ptCurStringStart = ptStart;
}

void CCrystalTextView::SetCurStringEnd(const CPoint &ptEnd)
{
	ASSERT_VALIDTEXTPOS(ptEnd);
	m_ptCurStringEnd = ptEnd;
}

int CCrystalTextView::GetCurStringColorBkgnd() const
{
	return m_nCurStringColorBkgnd;
}

void CCrystalTextView::SetCurStringColorBkgnd(int nColorIndex)
{
	m_nCurStringColorBkgnd = nColorIndex;
}

int CCrystalTextView::GetCurStringColorText() const
{
	return m_nCurStringColorText;
}

void CCrystalTextView::SetCurStringColorText(int nColorIndex)
{
	m_nCurStringColorText = nColorIndex;
}

void CCrystalTextView::SetDrawPairBlock( BOOL bDrawBlock )
{
	m_bDrawPairBlock = bDrawBlock;
}

BOOL CCrystalTextView::GetDrawPairBlock() const
{
	return m_bDrawPairBlock;
}

BOOL CCrystalTextView::FindPairTo(const CPoint &ptCursorPos, CPoint &ptCurStrStart, CPoint &ptCurStrEnd,
													CPoint &ptFoundStrStart, CPoint &ptFoundStrEnd, BOOL &bPairFound )

{
	BOOL result;
	BOOL bClearEndReached = FALSE;

	bPairFound = FALSE;

	int nCurPairIdx = -1, nCurPairDir = -1;
	if ( m_pParser == NULL 
			 || !IsEndOfPairAtHelper( ptCursorPos, ptCurStrStart, nCurPairIdx, nCurPairDir) )
	{
			ptFoundStrStart = CPoint(0,0);
			ptFoundStrEnd = CPoint(0,0);
			return TRUE;
	}

	ptCurStrEnd = ptCursorPos;

	//fill stack
	CCrystalParser::CPairStack aPairStack;
	CCrystalParser::CPairStack openPairStack;
    
	struct CCrystalParser::tagPairStackItm newBracket;
	newBracket.nPairIdx = nCurPairIdx;
	newBracket.nPairDir = nCurPairDir;
	newBracket.ptStart = ptCurStrStart;
	newBracket.ptEnd = ptCurStrEnd;

	aPairStack.push_back(newBracket);
    
	int nOpDir = m_pParser->OppositeDir(nCurPairDir);
	if (nOpDir == CCrystalParser::DIRECTION_LEFT)
	{
		CPoint ptTemp = ptCurStrStart;
		ptTemp.x--; //move to first position that doesn't belong to CurStr
		result = FindPairHelper( ptTemp, 0, nOpDir, aPairStack, false, ptCurStrStart, ptCurStrEnd,  
										 ptFoundStrStart, ptFoundStrEnd, openPairStack, bClearEndReached );
	}
	else
	{
		result = FindPairHelper( ptCurStrEnd, 0, nOpDir, aPairStack, false, ptCurStrStart, ptCurStrEnd,
										 ptFoundStrStart, ptFoundStrEnd, openPairStack, bClearEndReached );
	}

    if (result == FALSE ) {
		if (aPairStack.empty())
		{
			ptFoundStrStart = ptFoundStrEnd = CPoint(0,0);
		}
		else
		{
			const struct CCrystalParser::tagPairStackItm &rPairItm = aPairStack.back();
			ptFoundStrStart = rPairItm.ptStart;
			ptFoundStrEnd = rPairItm.ptEnd;
		}
	}
	
	aPairStack.clear();
	bPairFound = result;
	return result;
}



BOOL CCrystalTextView::FindPairHelper( const CPoint &ptTextPos, int nNthOpenPair, int nDirection, CCrystalParser::CPairStack &aPairStack,
														 BOOL bClearToEnd, const CPoint &ptCurStrStart, const CPoint &ptCurStrEnd,
														 CPoint &ptFoundStrStart, CPoint &ptFoundStrEnd, CCrystalParser::CPairStack &openPairStack,
														 BOOL &bClearEndReached )
{
	int result = CCrystalParser::RESULT_OK;
  
	ASSERT( nDirection == CCrystalParser::DIRECTION_LEFT 
    		  || nDirection == CCrystalParser::DIRECTION_RIGHT );
	
	int nLineCount = GetLineCount();
	int nLineIndex = ptTextPos.y;
	LPCTSTR	lpszLine = GetLineChars( nLineIndex );
	LPCTSTR lpszLineEnd = lpszLine + GetLineLength( nLineIndex );
	LPCTSTR lpszTextPos = lpszLine + ptTextPos.x;
	
	BOOL bSkipLine = ptTextPos.x < 0 || lpszTextPos >= lpszLineEnd;

	//search through the lines
	while ( true ) 
	{
		long nLength = GetLineLength( nLineIndex );

		if ( !bSkipLine && lpszTextPos >= lpszLine && nLength != 0 )
		{
			DWORD dwCookie = 0;
			DWORD dwCookie2 = 0;

			//	Parse the string
			dwCookie = GetParseCookie( nLineIndex - 1 );
			TEXTBLOCK *pBuf = (TEXTBLOCK *) malloc(sizeof(TEXTBLOCK) * nLength * 3);
			int nBlocks = 0;
			CCrystalTextBlock	textBlock = CCrystalTextBlock( (CCrystalTextBlock::TEXTBLOCK*) pBuf, nBlocks );

			dwCookie2 = ParseLine( dwCookie, nLineIndex, pBuf, nBlocks);

			if ( m_pParser->FindPairInLine( lpszLine, lpszLineEnd, &textBlock, nLineIndex, nDirection, 
																lpszTextPos, aPairStack, nNthOpenPair, bClearToEnd, 
																ptFoundStrStart.x, ptFoundStrEnd.x, openPairStack, result ) ) 
			{
				//pair found or error
				break;
			}
			free( pBuf );
		}
		bSkipLine = FALSE;
	
		nLineIndex += (nDirection == CCrystalParser::DIRECTION_LEFT) ? -1 : 1;
		//end of file, pair not found
		if ( nLineIndex < 0 || nLineIndex >= nLineCount ) 
		{
			if ( bClearToEnd && aPairStack.empty() )  {
				bClearEndReached = TRUE;
				result = TRUE;
				break;
			}
			result = FALSE;
			break;
		}
		
		lpszLine = GetLineChars( nLineIndex );
		lpszLineEnd = lpszLine + GetLineLength( nLineIndex );
		lpszTextPos = lpszLine;
		if ( nDirection == CCrystalParser::DIRECTION_LEFT && GetLineLength( nLineIndex ) != 0 ) 
		{
			lpszTextPos +=  GetLineLength(nLineIndex) - 1; 
		}
	} 

//	if  (result == TRUE )
//	{
		ptFoundStrEnd.y = ptFoundStrStart.y = nLineIndex;
//	}
//	else 
//	{
//   		ptPairStrEnd = ptPairStrStart = CPoint(0,0);
//	}
	
	return result;  
}


BOOL CCrystalTextView::IsEndOfPairAtHelper( const CPoint &ptTextPos, CPoint &ptFoundStrStart,
																			 int &nPairIdx, int &nPairDir )
{
	nPairIdx = -1;
	nPairDir = -1;
	DWORD dwCookie = 0;
	DWORD dwCookie2 = 0;
	int nLineIndex = ptTextPos.y;

	LPCTSTR lpszLine = GetLineChars( nLineIndex );
	long nLength = GetLineLength( nLineIndex );

	//	Parse the string
	dwCookie = GetParseCookie( nLineIndex - 1);
	TEXTBLOCK *pBuf = (TEXTBLOCK *) _alloca(sizeof(TEXTBLOCK) * nLength * 3);
	int nBlocks = 0;
	CCrystalTextBlock	textBlock = CCrystalTextBlock( (CCrystalTextBlock::TEXTBLOCK*) pBuf, nBlocks );

	dwCookie2 = ParseLine( dwCookie, nLineIndex, pBuf, nBlocks);

	if ( m_pParser->IsEndOfPairAt( lpszLine, lpszLine + ptTextPos.x, &textBlock, ptFoundStrStart.x, nPairIdx, nPairDir) )
	{
		ptFoundStrStart.y = ptTextPos.y;
		return TRUE;
	}

	return FALSE;
}


void CCrystalTextView::IncSelBlockLevel()
{
	m_nSelBlockLevel++;
}


void CCrystalTextView::ResetSelBlockLevel()
{
	m_nSelBlockLevel = 0;
}


BOOL CCrystalTextView::SelectBlockAround( const CPoint &ptTextPos )
{
	return SelectBlockAround( ptTextPos, m_nSelBlockLevel );
}


BOOL CCrystalTextView::SelectBlockAround( const CPoint &ptCursorPos, int nBlockLevel )
{
	if (nBlockLevel == 0) {
		SetSelection( CPoint(0,0), CPoint(0,0) );
		return TRUE;
	}
	BOOL bClearEndReached = FALSE;

	m_nSelBlockLevel = nBlockLevel;
	CPoint ptSelStart, ptSelEnd;
	CPoint ptNull(0,0);
	CPoint ptTemp(0,0);
	CPoint ptCurStrStart;
	CPoint ptErrStrStart(0,0);
	CPoint ptErrStrEnd(0,0);
	int nPairIdx = -1, nPairDir = -1;
	if ( m_pParser == NULL )
		return FALSE;

	//move to first position that doesn't belong to CurStr
	CPoint ptSearchStartL = ptCursorPos;
	CPoint ptSearchStartR = ptCursorPos;
	if ( IsEndOfPairAtHelper( ptCursorPos, ptCurStrStart, nPairIdx, nPairDir) )
	{
		if ( nPairDir == CCrystalParser::DIRECTION_RIGHT ) {
			ptSearchStartL = ptCurStrStart;
			ptSearchStartR = ptCurStrStart;
		}
	}
	ptSearchStartL.x--; 
	

	//create stack
	CCrystalParser::CPairStack aPairStack;
	CCrystalParser::CPairStack openPairStack;
	BOOL		bClearToEnd = false;
	
	//fills stack with nBlockLevel of open pairs
	if (!FindPairHelper( ptSearchStartL, nBlockLevel, CCrystalParser::DIRECTION_LEFT, aPairStack,
				bClearToEnd, ptNull, ptNull, ptSelStart, ptTemp, openPairStack, bClearEndReached ))
	{
		if (aPairStack.empty())
		{
			ptSelStart = CPoint(0,0);	
			bClearToEnd = true;
		}
		else
		{
			const struct CCrystalParser::tagPairStackItm &rPairItm = aPairStack.back();
			ptErrStrStart = rPairItm.ptStart;
			ptErrStrEnd = rPairItm.ptEnd;
			goto error;
		}
	}
	aPairStack.clear();
	//consumes stack with nBlockLevel of open pairs
	if (!FindPairHelper( ptSearchStartR, 0, CCrystalParser::DIRECTION_RIGHT, openPairStack,
				bClearToEnd, ptNull, ptNull, ptTemp, ptSelEnd, aPairStack, bClearEndReached ))
	{
		if (openPairStack.empty()) {
			ptErrStrStart = ptErrStrEnd = CPoint(0,0);
		}
		else {
			const struct CCrystalParser::tagPairStackItm &rPairItm = openPairStack.back();
			ptErrStrStart = rPairItm.ptStart;
			ptErrStrEnd = rPairItm.ptEnd;
		}
		goto error;
	}
	else 
	{
		if ( bClearToEnd && bClearEndReached )
		{
 			ptSelEnd.y = GetLineCount()-1;
   		ptSelEnd.x = GetLineLength(ptSelEnd.y);
		}
	}


	SetSelection( ptSelStart, ptSelEnd );
	return TRUE;

error:
	SetSelection( CPoint(0,0), CPoint(0,0) ); //deselect
	ResetSelBlockLevel();

	SetCurStringStart( ptErrStrStart );
	SetCurStringEnd( ptErrStrEnd );
	SetCurStringColorBkgnd( CCrystalTextView::COLORINDEX_BADPAIRSTRINGBKGND );
	SetCurStringColorText( CCrystalTextView::COLORINDEX_BADPAIRSTRINGTEXT );

	//jump to error
	//SetCursorPos( ptErrStrEnd );
	EnsureVisible( ptErrStrEnd );
	Invalidate();
	//UpdateWindow();
	return FALSE;
}


void CCrystalTextView::OnSelBiggerBlock()
{
	IncSelBlockLevel();
	SelectBlockAround( GetCursorPos() );
}


void CCrystalTextView::SetShowLineNumbers(BOOL show) {
	if (show != m_bShowLineNumbers) {
		m_bShowLineNumbers = show;
	}
}
 
BOOL CCrystalTextView::GetShowLineNumbers() {
	return m_bShowLineNumbers;
}
