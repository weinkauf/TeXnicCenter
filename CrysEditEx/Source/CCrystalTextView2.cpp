////////////////////////////////////////////////////////////////////////////
//	File:		CCrystalTextView2.cpp
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
//	21-Feb-99
//		Paul Selormey, James R. Twine:
//	+	FEATURE: description for Undo/Redo actions
//	+	FEATURE: multiple MSVC-like bookmarks
//	+	FEATURE: 'Disable backspace at beginning of line' option
//	+	FEATURE: 'Disable drag-n-drop editing' option
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
//				 double clicking. (BEGIN_SW(selection))
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
* Revision 1.7  2005/03/08 16:36:08  vachis
* Selection of block between bracket pairs in a hierarchical manner (Ctrl+M)
* Bracket highlighting disabled if a selection is present
* New default colours for bracket highlighting
*
* Revision 1.6  2005/02/23 23:58:19  vachis
* Fixed Matching Bracket Highlighting
* New features for Matching Bracket Highlighting: configure colours,
*     highlight current block and pair, help
*
* Revision 1.5  2005/02/15 09:43:12  vachis
* Implemented feature 536164: Matching Bracket Highlight
*
* Revision 1.4  2002/06/27 14:43:26  svenwiegand
* Instead of the character test functions (isalpha, isdigit, etc.) from the standard library, our own wrappers from the character.h file (myspell directory) are used now, to fix character recognition problems (bug 551033).
*
* Revision 1.3  2002/03/27 19:16:46  cnorris
* Source code clean up
*
* Revision 1.2  2002/03/27 19:06:28  cnorris
* Fix line selection of last line in document
*
* Revision 1.1.1.1  2002/02/26 08:11:59  svenwiegand
* Initial revision
*
* Revision 1.0  2000-05-31 21:55:27+02  sven_wiegand
* Initial revision
*
*
********************************************************************/

#include "stdafx.h"
#include "editcmd.h"
#include "CCrystalTextView.h"
#include "CCrystalTextBuffer.h"
#include <malloc.h>
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

#define	CRYSTAL_TIMER_DRAGSEL	1001


/////////////////////////////////////////////////////////////////////////////
// CCrystalTextView

void CCrystalTextView::MoveLeft(BOOL bSelect)
{
	PrepareSelBounds();
	if (m_ptDrawSelStart != m_ptDrawSelEnd && ! bSelect)
	{
		m_ptCursorPos = m_ptDrawSelStart;
	}
	else
	{
		if (m_ptCursorPos.x == 0)
		{
			if (m_ptCursorPos.y > 0)
			{
				m_ptCursorPos.y --;
				m_ptCursorPos.x = GetLineLength(m_ptCursorPos.y);
			}
		}
		else
			m_ptCursorPos.x --;
	}
	m_nIdealCharPos = CalculateActualOffset(m_ptCursorPos.y, m_ptCursorPos.x);
	EnsureVisible(m_ptCursorPos);
	UpdateCaret();
	if (! bSelect)
		m_ptAnchor = m_ptCursorPos;
	SetSelection(m_ptAnchor, m_ptCursorPos);

	if (!IsSelection()) {
		//highlight brackets
		MarkPairStringTo(GetCursorPos(), FALSE);
		Invalidate();
		UpdateWindow();
	}
	else
		UnmarkPairString();
}

void CCrystalTextView::MoveRight(BOOL bSelect)
{
	PrepareSelBounds();
	if (m_ptDrawSelStart != m_ptDrawSelEnd && ! bSelect)
	{
		m_ptCursorPos = m_ptDrawSelEnd;
	}
	else
	{
		if (m_ptCursorPos.x == GetLineLength(m_ptCursorPos.y))
		{
			if (m_ptCursorPos.y < GetLineCount() - 1)
			{
				m_ptCursorPos.y ++;
				m_ptCursorPos.x = 0;
			}
		}
		else
			m_ptCursorPos.x ++;
	}
	m_nIdealCharPos = CalculateActualOffset(m_ptCursorPos.y, m_ptCursorPos.x);
	EnsureVisible(m_ptCursorPos);
	UpdateCaret();
	if (! bSelect)
		m_ptAnchor = m_ptCursorPos;
	SetSelection(m_ptAnchor, m_ptCursorPos);

	if (!IsSelection()) {
		//highlight brackets
		MarkPairStringTo(GetCursorPos(), FALSE);
		Invalidate();
		UpdateWindow();
	}
	else
		UnmarkPairString();
}

void CCrystalTextView::MoveWordLeft(BOOL bSelect)
{
	PrepareSelBounds();
	if (m_ptDrawSelStart != m_ptDrawSelEnd && ! bSelect)
	{
		MoveLeft(bSelect);
		return;
	}

	if (m_ptCursorPos.x == 0)
	{
		if (m_ptCursorPos.y == 0)
			return;
		m_ptCursorPos.y --;
		m_ptCursorPos.x = GetLineLength(m_ptCursorPos.y);
	}

	LPCTSTR pszChars = GetLineChars(m_ptCursorPos.y);
	int nPos = m_ptCursorPos.x;
	while (nPos > 0 && IsSpace(pszChars[nPos - 1]))
		nPos --;

	if (nPos > 0)
	{
		nPos --;
		if (IsAlNum(pszChars[nPos]) || pszChars[nPos] == _T('_'))
		{
			while (nPos > 0 && (IsAlNum(pszChars[nPos - 1]) || pszChars[nPos - 1] == _T('_')))
				nPos --;
		}
		else
		{
			while (nPos > 0 && ! IsAlNum(pszChars[nPos - 1])
						&& pszChars[nPos - 1] != _T('_') && ! IsSpace(pszChars[nPos - 1]))
				nPos --;
		}
	}

	m_ptCursorPos.x = nPos;
	m_nIdealCharPos = CalculateActualOffset(m_ptCursorPos.y, m_ptCursorPos.x);
	EnsureVisible(m_ptCursorPos);
	UpdateCaret();
	if (! bSelect)
		m_ptAnchor = m_ptCursorPos;
	SetSelection(m_ptAnchor, m_ptCursorPos);

	if (!IsSelection()) {
		//highlight brackets
		MarkPairStringTo(GetCursorPos(), FALSE);
		Invalidate();
		UpdateWindow();
	}
	else
		UnmarkPairString();
}

void CCrystalTextView::MoveWordRight(BOOL bSelect)
{
	PrepareSelBounds();
	if (m_ptDrawSelStart != m_ptDrawSelEnd && ! bSelect)
	{
		MoveRight(bSelect);
		return;
	}

	if (m_ptCursorPos.x == GetLineLength(m_ptCursorPos.y))
	{
		if (m_ptCursorPos.y == GetLineCount() - 1)
			return;
		m_ptCursorPos.y ++;
		m_ptCursorPos.x = 0;
	}

	int nLength = GetLineLength(m_ptCursorPos.y);
	if (m_ptCursorPos.x == nLength)
	{
		MoveRight(bSelect);
		return;
	}

	LPCTSTR pszChars = GetLineChars(m_ptCursorPos.y);
	int nPos = m_ptCursorPos.x;
	if (IsAlNum(pszChars[nPos]) || pszChars[nPos] == _T('_'))
	{
		while (nPos < nLength && IsAlNum(pszChars[nPos]) || pszChars[nPos] == _T('_'))
			nPos ++;
	}
	else
	{
		while (nPos < nLength && ! IsAlNum(pszChars[nPos])
						&& pszChars[nPos] != _T('_') && ! IsSpace(pszChars[nPos]))
			nPos ++;
	}

	while (nPos < nLength && IsSpace(pszChars[nPos]))
		nPos ++;

	m_ptCursorPos.x = nPos;
	m_nIdealCharPos = CalculateActualOffset(m_ptCursorPos.y, m_ptCursorPos.x);
	EnsureVisible(m_ptCursorPos);
	UpdateCaret();
	if (! bSelect)
		m_ptAnchor = m_ptCursorPos;
	SetSelection(m_ptAnchor, m_ptCursorPos);

	if (!IsSelection()) {
		//highlight brackets
		MarkPairStringTo(GetCursorPos(), FALSE);
		Invalidate();
		UpdateWindow();
	}
	else
		UnmarkPairString();
}

void CCrystalTextView::MoveUp(BOOL bSelect)
{
	PrepareSelBounds();
	if (m_ptDrawSelStart != m_ptDrawSelEnd && ! bSelect)
		m_ptCursorPos = m_ptDrawSelStart;

	CPoint subLinePos;
	CharPosToPoint( m_ptCursorPos.y, m_ptCursorPos.x, subLinePos );

	int nSubLine = GetSubLineIndex( m_ptCursorPos.y ) + subLinePos.y;

	if( nSubLine > 0 )
	{
		if (m_nIdealCharPos == -1)
			m_nIdealCharPos = CalculateActualOffset(m_ptCursorPos.y, m_ptCursorPos.x);

		//BEGIN SW
		SubLineCursorPosToTextPos( CPoint( m_nIdealCharPos, nSubLine - 1 ), m_ptCursorPos );
		//END SW
		if (m_ptCursorPos.x > GetLineLength(m_ptCursorPos.y))
			m_ptCursorPos.x = GetLineLength(m_ptCursorPos.y);
	}
	EnsureVisible(m_ptCursorPos);
	UpdateCaret();
	if (! bSelect)
		m_ptAnchor = m_ptCursorPos;
	SetSelection(m_ptAnchor, m_ptCursorPos);

	if (!IsSelection()) {
		//highlight brackets
		MarkPairStringTo(GetCursorPos(), FALSE);
		Invalidate();
		UpdateWindow();
	}
	else
		UnmarkPairString();
}

void CCrystalTextView::MoveDown(BOOL bSelect)
{
	PrepareSelBounds();
	if (m_ptDrawSelStart != m_ptDrawSelEnd && ! bSelect)
		m_ptCursorPos = m_ptDrawSelEnd;

	//BEGIN SW
	CPoint subLinePos;
	CharPosToPoint( m_ptCursorPos.y, m_ptCursorPos.x, subLinePos );

	int nSubLine = GetSubLineIndex( m_ptCursorPos.y ) + subLinePos.y;

	if( nSubLine < GetSubLineCount() - 1 )
	{
		if (m_nIdealCharPos == -1)
			m_nIdealCharPos = CalculateActualOffset(m_ptCursorPos.y, m_ptCursorPos.x);
		//BEGIN SW
		SubLineCursorPosToTextPos( CPoint( m_nIdealCharPos, nSubLine + 1 ), m_ptCursorPos );
		//END SW
		if (m_ptCursorPos.x > GetLineLength(m_ptCursorPos.y))
			m_ptCursorPos.x = GetLineLength(m_ptCursorPos.y);
	}
	EnsureVisible(m_ptCursorPos);
	UpdateCaret();
	if (! bSelect)
		m_ptAnchor = m_ptCursorPos;
	SetSelection(m_ptAnchor, m_ptCursorPos);

	if (!IsSelection()) {
		//highlight brackets
		MarkPairStringTo(GetCursorPos(), FALSE);
		Invalidate();
		UpdateWindow();
	}
	else
		UnmarkPairString();
}

void CCrystalTextView::MoveHome(BOOL bSelect)
{
	int nLength = GetLineLength(m_ptCursorPos.y);
	LPCTSTR pszChars = GetLineChars(m_ptCursorPos.y);
	//BEGIN SW
	CPoint pos;
	CharPosToPoint( m_ptCursorPos.y, m_ptCursorPos.x, pos );
	int nHomePos = SubLineHomeToCharPos( m_ptCursorPos.y, pos.y );
	int nOriginalHomePos = nHomePos;
	//END SW

	while (nHomePos < nLength && IsSpace(pszChars[nHomePos]))
		nHomePos ++;
	if (nHomePos == nLength || m_ptCursorPos.x == nHomePos)
		//BEGIN SW
		m_ptCursorPos.x = nOriginalHomePos;
		//END SW
	else
		m_ptCursorPos.x = nHomePos;
	m_nIdealCharPos = CalculateActualOffset(m_ptCursorPos.y, m_ptCursorPos.x);
	EnsureVisible(m_ptCursorPos);
	UpdateCaret();
	if (! bSelect)
		m_ptAnchor = m_ptCursorPos;
	SetSelection(m_ptAnchor, m_ptCursorPos);

	if (!IsSelection()) {
		//highlight brackets
		MarkPairStringTo(GetCursorPos(), FALSE);
		Invalidate();
		UpdateWindow();
	}
	else
		UnmarkPairString();
}

void CCrystalTextView::MoveEnd(BOOL bSelect)
{
	//BEGIN SW
	CPoint pos;
	CharPosToPoint( m_ptCursorPos.y, m_ptCursorPos.x, pos );
	m_ptCursorPos.x = SubLineEndToCharPos( m_ptCursorPos.y, pos.y );
	//END SW
	m_nIdealCharPos = CalculateActualOffset(m_ptCursorPos.y, m_ptCursorPos.x);
	EnsureVisible(m_ptCursorPos);
	UpdateCaret();
	if (! bSelect)
		m_ptAnchor = m_ptCursorPos;
	SetSelection(m_ptAnchor, m_ptCursorPos);

	if (!IsSelection()) {
		//highlight brackets
		MarkPairStringTo(GetCursorPos(), FALSE);
		Invalidate();
		UpdateWindow();
	}
	else
		UnmarkPairString();
}

void CCrystalTextView::MovePgUp(BOOL bSelect)
{
	//BEGIN SW
	// scrolling windows
	int nNewTopSubLine = m_nTopSubLine - GetScreenLines() + 1;
	if (nNewTopSubLine < 0)
		nNewTopSubLine = 0;
	if (m_nTopSubLine != nNewTopSubLine)
	{
		ScrollToSubLine(nNewTopSubLine);
		UpdateSiblingScrollPos(TRUE);
	}

	// setting cursor
	CPoint subLinePos;
	CharPosToPoint( m_ptCursorPos.y, m_ptCursorPos.x, subLinePos );

	int nSubLine = GetSubLineIndex( m_ptCursorPos.y ) + subLinePos.y - GetScreenLines() + 1;

	if( nSubLine < 0 )
		nSubLine = 0;

	SubLineCursorPosToTextPos( 
		CPoint( m_nIdealCharPos, nSubLine ), m_ptCursorPos );
	//END SW

	m_nIdealCharPos = CalculateActualOffset(m_ptCursorPos.y, m_ptCursorPos.x);
	EnsureVisible(m_ptCursorPos);	//todo: no vertical scroll
	UpdateCaret();
	if (! bSelect)
		m_ptAnchor = m_ptCursorPos;
	SetSelection(m_ptAnchor, m_ptCursorPos);

	if (!IsSelection()) {
		//highlight brackets
		MarkPairStringTo(GetCursorPos(), FALSE);
		Invalidate();
		UpdateWindow();
	}
	else
		UnmarkPairString();
}

void CCrystalTextView::MovePgDn(BOOL bSelect)
{
	//BEGIN SW
	// scrolling windows
	int nNewTopSubLine = m_nTopSubLine + GetScreenLines() - 1;
	int nSubLineCount = GetSubLineCount();

	if (nNewTopSubLine > nSubLineCount)
		nNewTopSubLine = nSubLineCount - 1;
	if (m_nTopSubLine != nNewTopSubLine)
	{
		ScrollToSubLine(nNewTopSubLine);
		UpdateSiblingScrollPos(TRUE);
	}

	// setting cursor
	CPoint subLinePos;
	CharPosToPoint( m_ptCursorPos.y, m_ptCursorPos.x, subLinePos );

	int nSubLine = GetSubLineIndex( m_ptCursorPos.y ) + subLinePos.y + GetScreenLines() - 1;

	if( nSubLine > nSubLineCount - 1 )
		nSubLine = nSubLineCount - 1;

	SubLineCursorPosToTextPos( 
		CPoint( m_nIdealCharPos, nSubLine ), m_ptCursorPos );
	//END SW
	m_nIdealCharPos = CalculateActualOffset(m_ptCursorPos.y, m_ptCursorPos.x);
	EnsureVisible(m_ptCursorPos);	//todo: no vertical scroll
	UpdateCaret();
	if (! bSelect)
		m_ptAnchor = m_ptCursorPos;
	SetSelection(m_ptAnchor, m_ptCursorPos);

	if (!IsSelection()) {
		//highlight brackets
		MarkPairStringTo(GetCursorPos(), FALSE);
		Invalidate();
		UpdateWindow();
	}
	else
		UnmarkPairString();
}

void CCrystalTextView::MoveCtrlHome(BOOL bSelect)
{
	m_ptCursorPos.x = 0;
	m_ptCursorPos.y = 0;
	m_nIdealCharPos = CalculateActualOffset(m_ptCursorPos.y, m_ptCursorPos.x);
	EnsureVisible(m_ptCursorPos);
	UpdateCaret();
	if (! bSelect)
		m_ptAnchor = m_ptCursorPos;
	SetSelection(m_ptAnchor, m_ptCursorPos);

	if (!IsSelection()) {
		//highlight brackets
		MarkPairStringTo(GetCursorPos(), FALSE);
		Invalidate();
		UpdateWindow();
	}
	else
		UnmarkPairString();
}

void CCrystalTextView::MoveCtrlEnd(BOOL bSelect)
{
	m_ptCursorPos.y = GetLineCount() - 1;
	m_ptCursorPos.x = GetLineLength(m_ptCursorPos.y);
	m_nIdealCharPos = CalculateActualOffset(m_ptCursorPos.y, m_ptCursorPos.x);
	EnsureVisible(m_ptCursorPos);
	UpdateCaret();
	if (! bSelect)
		m_ptAnchor = m_ptCursorPos;
	SetSelection(m_ptAnchor, m_ptCursorPos);	

	if (!IsSelection()) {
		//highlight brackets
		MarkPairStringTo(GetCursorPos(), FALSE);
		Invalidate();
		UpdateWindow();
	}
	else
		UnmarkPairString();
}

void CCrystalTextView::ScrollUp()
{
	if (m_nTopSubLine > 0)
	{
		ScrollToSubLine(m_nTopSubLine - 1);
		UpdateSiblingScrollPos(TRUE);
	}
}

void CCrystalTextView::ScrollDown()
{
	if (m_nTopSubLine < GetSubLineCount() - 1)
	{
		ScrollToSubLine(m_nTopSubLine + 1);
		UpdateSiblingScrollPos(TRUE);
	}
}

void CCrystalTextView::ScrollLeft()
{
	if (m_nOffsetChar > 0)
	{
		ScrollToChar(m_nOffsetChar - 1);
		UpdateCaret();
	}
}

void CCrystalTextView::ScrollRight()
{
	if (m_nOffsetChar < GetMaxLineLength() - 1)
	{
		ScrollToChar(m_nOffsetChar + 1);
		UpdateCaret();
	}
}

CPoint CCrystalTextView::WordToRight(CPoint pt)
{
	ASSERT_VALIDTEXTPOS(pt);
	int nLength = GetLineLength(pt.y);
	LPCTSTR pszChars = GetLineChars(pt.y);
	while (pt.x < nLength)
	{
		if (! IsAlNum(pszChars[pt.x]) && pszChars[pt.x] != _T('_'))
			break;
		pt.x ++;
	}
	ASSERT_VALIDTEXTPOS(pt);
	return pt;
}

CPoint CCrystalTextView::WordToLeft(CPoint pt)
{
	ASSERT_VALIDTEXTPOS(pt);
	LPCTSTR pszChars = GetLineChars(pt.y);
	while (pt.x > 0)
	{
		if (! IsAlNum(pszChars[pt.x - 1]) && pszChars[pt.x - 1] != _T('_'))
			break;
		pt.x --;
	}
	ASSERT_VALIDTEXTPOS(pt);
	return pt;
}

void CCrystalTextView::SelectAll()
{
	int nLineCount = GetLineCount();
	m_ptCursorPos.x = GetLineLength(nLineCount - 1);
	m_ptCursorPos.y = nLineCount - 1;
	SetSelection(CPoint(0, 0), m_ptCursorPos);
	UpdateCaret();
}

void CCrystalTextView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CView::OnLButtonDown(nFlags, point);

	BOOL bShift = GetKeyState(VK_SHIFT) & 0x8000;
	BOOL bControl = GetKeyState(VK_CONTROL) & 0x8000;

	if (point.x < GetMarginWidth())
	{
		AdjustTextPoint(point);
		if (bControl)
		{
			SelectAll();
		}
		else
		{
			m_ptCursorPos = ClientToText(point);
			//BEGIN SW
			// Find char pos that is the beginning of the subline clicked on
			CPoint pos;
			CharPosToPoint( m_ptCursorPos.y, m_ptCursorPos.x, pos );
			m_ptCursorPos.x = SubLineHomeToCharPos( m_ptCursorPos.y, pos.y );
			//END SW
			if (! bShift)
				m_ptAnchor = m_ptCursorPos;

			CPoint ptStart, ptEnd;
			//BEGIN SW
			CharPosToPoint( m_ptAnchor.y, m_ptAnchor.x, pos );
			ptStart.y = m_ptAnchor.y;
			if( GetSubLineIndex( ptStart.y ) + pos.y == GetSubLineCount() - 1 )
			{
				// select to end of subline
				ptStart.y = GetSubLineCount() - 1;
				ptStart.x = SubLineEndToCharPos( ptStart.y, pos.y );
			}
			else
			{
				int nLine, nSubLine;
				GetLineBySubLine( GetSubLineIndex( ptStart.y ) + pos.y + 1, nLine, nSubLine );
				ptStart.y = nLine;
				ptStart.x = SubLineHomeToCharPos( nLine, nSubLine );
			}
			ptEnd = m_ptCursorPos;
			//END SW

			m_ptCursorPos = ptEnd;
			UpdateCaret();
			EnsureVisible(m_ptCursorPos);
			SetSelection(ptStart, ptEnd);

			SetCapture();
			m_nDragSelTimer = SetTimer(CRYSTAL_TIMER_DRAGSEL, 100, NULL);
			ASSERT(m_nDragSelTimer != 0);
			m_bWordSelection = FALSE;
			m_bLineSelection = TRUE;
			m_bDragSelection = TRUE;
		}
	}
	else
	{
		CPoint ptText = ClientToText(point);
		PrepareSelBounds();
		//	[JRT]:	Support For Disabling Drag and Drop...
		if ((IsInsideSelBlock(ptText)) &&				// If Inside Selection Area
				(!m_bDisableDragAndDrop))				// And D&D Not Disabled
		{
			m_bPreparingToDrag = TRUE;
		}
		else
		{
			AdjustTextPoint(point);
			m_ptCursorPos = ClientToText(point);
			if (! bShift)
				m_ptAnchor = m_ptCursorPos;

			CPoint ptStart, ptEnd;
			if (bControl)
			{
				if (m_ptCursorPos.y < m_ptAnchor.y ||
					m_ptCursorPos.y == m_ptAnchor.y && m_ptCursorPos.x < m_ptAnchor.x)
				{
					ptStart = WordToLeft(m_ptCursorPos);
					ptEnd = WordToRight(m_ptAnchor);
				}
				else
				{
					ptStart = WordToLeft(m_ptAnchor);
					ptEnd = WordToRight(m_ptCursorPos);
				}
			}
			else
			{
				ptStart = m_ptAnchor;
				ptEnd = m_ptCursorPos;
			}

			m_ptCursorPos = ptEnd;
			UpdateCaret();
			EnsureVisible(m_ptCursorPos);
			SetSelection(ptStart, ptEnd);

			SetCapture();
			m_nDragSelTimer = SetTimer(CRYSTAL_TIMER_DRAGSEL, 100, NULL);
			ASSERT(m_nDragSelTimer != 0);
			m_bWordSelection = bControl;
			m_bLineSelection = FALSE;
			m_bDragSelection = TRUE;
		}
	}

	ASSERT_VALIDTEXTPOS(m_ptCursorPos);

	//BEGIN SW
	// we must set the ideal character position here!
	m_nIdealCharPos = CalculateActualOffset( m_ptCursorPos.y, m_ptCursorPos.x );
	//END SW

	if (!IsSelection()) {
		//highlight brackets
		MarkPairStringTo(GetCursorPos(), FALSE);
		Invalidate();
		UpdateWindow();
	}
	else
		UnmarkPairString();
}

void CCrystalTextView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CView::OnMouseMove(nFlags, point);

	//unhighlight brackets
	UnmarkPairString();

	if (m_bDragSelection)
	{
		BOOL bOnMargin = point.x < GetMarginWidth();

		AdjustTextPoint(point);
		CPoint ptNewCursorPos = ClientToText(point);

		CPoint ptStart, ptEnd;
		if (m_bLineSelection)
		{
			if (bOnMargin)
			{
				if (ptNewCursorPos.y < m_ptAnchor.y ||
					ptNewCursorPos.y == m_ptAnchor.y && ptNewCursorPos.x < m_ptAnchor.x)
				{
					//BEGIN SW
					CPoint pos;
					ptEnd = m_ptAnchor;
					CharPosToPoint( ptEnd.y, ptEnd.x, pos );
					if( GetSubLineIndex( ptEnd.y ) + pos.y == GetSubLineCount() - 1 )
						ptEnd = SubLineEndToCharPos( ptEnd.y, pos.y );
					else
					{
						int nLine, nSubLine;
						GetLineBySubLine( GetSubLineIndex( ptEnd.y ) + pos.y + 1, nLine, nSubLine );
						ptEnd.y = nLine;
						ptEnd.x = SubLineHomeToCharPos( nLine, nSubLine );
					}
					CharPosToPoint( ptNewCursorPos.y, ptNewCursorPos.x, pos );
					ptNewCursorPos.x = SubLineHomeToCharPos( ptNewCursorPos.y, pos.y );
					//END SW
					m_ptCursorPos = ptNewCursorPos;
				}
				else
				{
					ptEnd = m_ptAnchor;
					//BEGIN SW

					CPoint pos;
					CharPosToPoint( ptEnd.y, ptEnd.x, pos );
					ptEnd.x = SubLineHomeToCharPos( ptEnd.y, pos.y );

					m_ptCursorPos = ptNewCursorPos;
					CharPosToPoint( ptNewCursorPos.y, ptNewCursorPos.x, pos );
					if( GetSubLineIndex( ptNewCursorPos.y ) + pos.y == GetSubLineCount() - 1 )
						ptNewCursorPos.x = SubLineEndToCharPos( ptNewCursorPos.y, pos.y );
					else
					{
						int nLine, nSubLine;
						GetLineBySubLine( GetSubLineIndex( ptNewCursorPos.y ) + pos.y + 1, nLine, nSubLine );
						ptNewCursorPos.y = nLine;
						ptNewCursorPos.x = SubLineHomeToCharPos( nLine, nSubLine );
					}

					int nLine, nSubLine;
					GetLineBySubLine( GetSubLineIndex( m_ptCursorPos.y ) + pos.y, nLine, nSubLine );
					m_ptCursorPos.y = nLine;
					m_ptCursorPos.x = SubLineHomeToCharPos( nLine, nSubLine );
					//END SW
				}
				UpdateCaret();
				SetSelection(ptNewCursorPos, ptEnd);

				//if (!isSelection()) {
				//highlight brackets
				//MarkPairStringTo(GetCursorPos());
				//Invalidate();
				//UpdateWindow();
				//}
				//else
				//UnmarkPairString();
				return;
			}

			//	Moving to normal selection mode
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));
			m_bLineSelection = m_bWordSelection = FALSE;
		}

		if (m_bWordSelection)
		{
			if (ptNewCursorPos.y < m_ptAnchor.y ||
				ptNewCursorPos.y == m_ptAnchor.y && ptNewCursorPos.x < m_ptAnchor.x)
			{
				ptStart = WordToLeft(ptNewCursorPos);
				ptEnd = WordToRight(m_ptAnchor);
			}
			else
			{
				ptStart = WordToLeft(m_ptAnchor);
				ptEnd = WordToRight(ptNewCursorPos);
			}
		}
		else
		{
			ptStart = m_ptAnchor;
			ptEnd = ptNewCursorPos;
		}

		m_ptCursorPos = ptEnd;
		UpdateCaret();
		SetSelection(ptStart, ptEnd);

		//if (!isSelection()) {
		//highlight brackets
		//MarkPairStringTo(GetCursorPos());
		//Invalidate();
		//UpdateWindow();
		//}
		//else
		//UnmarkPairString();
	}

	if (m_bPreparingToDrag)
	{
		m_bPreparingToDrag = FALSE;
		HGLOBAL hData = PrepareDragData();
		if (hData != NULL)
		{
			if (m_pTextBuffer != NULL)
				m_pTextBuffer->BeginUndoGroup();

			COleDataSource ds;
			ds.CacheGlobalData(CF_TEXT, hData);
			m_bDraggingText = TRUE;
			DROPEFFECT de = ds.DoDragDrop(GetDropEffect());
			if (de != DROPEFFECT_NONE)
				OnDropSource(de);
			m_bDraggingText = FALSE;

			if (m_pTextBuffer != NULL)
				m_pTextBuffer->FlushUndoGroup(this);
		}
	}

	ASSERT_VALIDTEXTPOS(m_ptCursorPos);
}

void CCrystalTextView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CView::OnLButtonUp(nFlags, point);

	if (m_bDragSelection)
	{
		AdjustTextPoint(point);
		CPoint ptNewCursorPos = ClientToText(point);

		CPoint ptStart, ptEnd;
		if (m_bLineSelection)
		{
			CPoint ptEnd;
			if (ptNewCursorPos.y < m_ptAnchor.y ||
				ptNewCursorPos.y == m_ptAnchor.y && ptNewCursorPos.x < m_ptAnchor.x)
			{
				//BEGIN SW
				CPoint pos;
				ptEnd = m_ptAnchor;
				CharPosToPoint( ptEnd.y, ptEnd.x, pos );
				if( GetSubLineIndex( ptEnd.y ) + pos.y == GetSubLineCount() - 1 )
					ptEnd = SubLineEndToCharPos( ptEnd.y, pos.y );
				else
				{
					int nLine, nSubLine;
					GetLineBySubLine( GetSubLineIndex( ptEnd.y ) + pos.y + 1, nLine, nSubLine );
					ptEnd.y = nLine;
					ptEnd.x = SubLineHomeToCharPos( nLine, nSubLine );
				}
				CharPosToPoint( ptNewCursorPos.y, ptNewCursorPos.x, pos );
				ptNewCursorPos.x = SubLineHomeToCharPos( ptNewCursorPos.y, pos.y );
				//END SW
				m_ptCursorPos = ptNewCursorPos;
			}
			else
			{
				ptEnd = m_ptAnchor;
				//BEGIN SW

				CPoint pos;
				CharPosToPoint( ptEnd.y, ptEnd.x, pos );
				ptEnd.x = SubLineHomeToCharPos( ptEnd.y, pos.y );

				m_ptCursorPos = ptNewCursorPos;
				CharPosToPoint( ptNewCursorPos.y, ptNewCursorPos.x, pos );
				if( GetSubLineIndex( ptNewCursorPos.y ) + pos.y == GetSubLineCount() - 1 )
					ptNewCursorPos.x = SubLineEndToCharPos( ptNewCursorPos.y, pos.y );
				else
				{
					int nLine, nSubLine;
					GetLineBySubLine( GetSubLineIndex( ptNewCursorPos.y ) + pos.y + 1, nLine, nSubLine );
					ptNewCursorPos.y = nLine;
					ptNewCursorPos.x = SubLineHomeToCharPos( nLine, nSubLine );
				}
				m_ptCursorPos = ptNewCursorPos;
				//END SW
			}
			EnsureVisible(m_ptCursorPos);
			UpdateCaret();
			SetSelection(ptNewCursorPos, ptEnd);
		}
		else
		{
			if (m_bWordSelection)
			{
				if (ptNewCursorPos.y < m_ptAnchor.y ||
					ptNewCursorPos.y == m_ptAnchor.y && ptNewCursorPos.x < m_ptAnchor.x)
				{
					ptStart = WordToLeft(ptNewCursorPos);
					ptEnd = WordToRight(m_ptAnchor);
				}
				else
				{
					ptStart = WordToLeft(m_ptAnchor);
					ptEnd = WordToRight(ptNewCursorPos);
				}
			}
			else
			{
				ptStart = m_ptAnchor;
				ptEnd = m_ptCursorPos;
			}

			m_ptCursorPos = ptEnd;
			EnsureVisible(m_ptCursorPos);
			UpdateCaret();
			SetSelection(ptStart, ptEnd);
		}

		ReleaseCapture();
		KillTimer(m_nDragSelTimer);
		m_bDragSelection = FALSE;
	}

	if (m_bPreparingToDrag)
	{
		m_bPreparingToDrag = FALSE;

		AdjustTextPoint(point);
		m_ptCursorPos = ClientToText(point);
		EnsureVisible(m_ptCursorPos);
		UpdateCaret();
		SetSelection(m_ptCursorPos, m_ptCursorPos);
	}

	ASSERT_VALIDTEXTPOS(m_ptCursorPos);
	if (!IsSelection()) 
	{
		//highlight brackets
		MarkPairStringTo(GetCursorPos(), FALSE);
		Invalidate();
		UpdateWindow();
	}
	else
		UnmarkPairString();

}

void CCrystalTextView::OnTimer(UINT nIDEvent) 
{
	CView::OnTimer(nIDEvent);

	if (nIDEvent == CRYSTAL_TIMER_DRAGSEL)
	{
		ASSERT(m_bDragSelection);
		CPoint pt;
		::GetCursorPos(&pt);
		ScreenToClient(&pt);
		CRect rcClient;
		GetClientRect(&rcClient);

		BOOL bChanged = FALSE;

		//	Scroll vertically, if necessary
		int nNewTopLine = m_nTopLine;
		int nLineCount = GetLineCount();
		if (pt.y < rcClient.top)
		{
			nNewTopLine --;
			if (pt.y < rcClient.top - GetLineHeight())
				nNewTopLine -= 2;
		}
		else
		if (pt.y >= rcClient.bottom)
		{
			nNewTopLine ++;
			if (pt.y >= rcClient.bottom + GetLineHeight())
				nNewTopLine += 2;
		}

		if (nNewTopLine < 0)
			nNewTopLine = 0;
		if (nNewTopLine >= nLineCount)
			nNewTopLine = nLineCount - 1;

		if (m_nTopLine != nNewTopLine)
		{
			ScrollToLine(nNewTopLine);
			UpdateSiblingScrollPos(TRUE);
			bChanged = TRUE;
		}

		//	Scroll horizontally, if necessary
		int nNewOffsetChar = m_nOffsetChar;
		int nMaxLineLength = GetMaxLineLength();
		if (pt.x < rcClient.left)
			nNewOffsetChar --;
		else
		if (pt.x >= rcClient.right)
			nNewOffsetChar ++;

		if (nNewOffsetChar >= nMaxLineLength)
			nNewOffsetChar = nMaxLineLength - 1;
		if (nNewOffsetChar < 0)
			nNewOffsetChar = 0;

		if (m_nOffsetChar != nNewOffsetChar)
		{
			ScrollToChar(nNewOffsetChar);
			UpdateCaret();
			UpdateSiblingScrollPos(FALSE);
			bChanged = TRUE;
		}

		//	Fix changes
		if (bChanged)
		{
			AdjustTextPoint(pt);
			CPoint ptNewCursorPos = ClientToText(pt);
			if (ptNewCursorPos != m_ptCursorPos)
			{
				m_ptCursorPos = ptNewCursorPos;
				UpdateCaret();
			}
			SetSelection(m_ptAnchor, m_ptCursorPos);
		}
	}
}

void CCrystalTextView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CView::OnLButtonDblClk(nFlags, point);

	if (! m_bDragSelection)
	{
		AdjustTextPoint(point);

		m_ptCursorPos = ClientToText(point);
		m_ptAnchor = m_ptCursorPos;

		CPoint ptStart, ptEnd;
		if (m_ptCursorPos.y < m_ptAnchor.y ||
			m_ptCursorPos.y == m_ptAnchor.y && m_ptCursorPos.x < m_ptAnchor.x)
		{
			ptStart = WordToLeft(m_ptCursorPos);
			ptEnd = WordToRight(m_ptAnchor);
		}
		else
		{
			ptStart = WordToLeft(m_ptAnchor);
			ptEnd = WordToRight(m_ptCursorPos);
		}

		m_ptCursorPos = ptEnd;
		UpdateCaret();
		EnsureVisible(m_ptCursorPos);
		SetSelection(ptStart, ptEnd);
		//BEGIN SW(Selection)
		m_ptAnchor = ptStart;
		//END SW(Selection)

		SetCapture();
		m_nDragSelTimer = SetTimer(CRYSTAL_TIMER_DRAGSEL, 100, NULL);
		ASSERT(m_nDragSelTimer != 0);
		m_bWordSelection = TRUE;
		m_bLineSelection = FALSE;
		m_bDragSelection = TRUE;

		if (!IsSelection()) //highlight brackets
		{
			MarkPairStringTo(GetCursorPos(), FALSE);
			Invalidate();
			UpdateWindow();
		}
		else
			UnmarkPairString();
	}
}

void CCrystalTextView::OnEditCopy() 
{
	Copy();
}

void CCrystalTextView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_ptSelStart != m_ptSelEnd);
}

void CCrystalTextView::OnEditSelectAll() 
{
	SelectAll();
}

void CCrystalTextView::OnUpdateEditSelectAll(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void CCrystalTextView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CPoint pt = point;
	AdjustTextPoint(pt);
	pt = ClientToText(pt);
	if (! IsInsideSelBlock(pt))
	{
		m_ptAnchor = m_ptCursorPos = pt;
		SetSelection(m_ptCursorPos, m_ptCursorPos);
		EnsureVisible(m_ptCursorPos);
		UpdateCaret();
	}

	if ( !IsSelection() ) {
		//highlight brackets
		MarkPairStringTo(GetCursorPos(), FALSE);
		Invalidate();
		UpdateWindow();
	}
	else
		UnmarkPairString();

	CView::OnRButtonDown(nFlags, point);
}

BOOL CCrystalTextView::IsSelection()
{
	return m_ptSelStart != m_ptSelEnd;
}

void CCrystalTextView::Copy()
{
	if (m_ptSelStart == m_ptSelEnd)
		return;

	PrepareSelBounds();
	CString text;
	GetText(m_ptDrawSelStart, m_ptDrawSelEnd, text);
	PutToClipboard(text);
}

BOOL CCrystalTextView::TextInClipboard()
{
	return IsClipboardFormatAvailable(CF_TEXT);
}

BOOL CCrystalTextView::PutToClipboard(LPCTSTR pszText)
{
	if (pszText == NULL || lstrlen(pszText) == 0)
		return FALSE;

	CWaitCursor wc;
	BOOL bOK = FALSE;
	if (OpenClipboard())
	{
		EmptyClipboard();
		HGLOBAL hData = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, lstrlen(pszText) + 1);
		if (hData != NULL)
		{
			LPSTR pszData = (LPSTR) ::GlobalLock(hData);
			USES_CONVERSION;
			strcpy(pszData, T2A((LPTSTR) pszText));
			GlobalUnlock(hData);
			bOK = SetClipboardData(CF_TEXT, hData) != NULL;
		}
		CloseClipboard();
	}
	return bOK;
}

BOOL CCrystalTextView::GetFromClipboard(CString &text)
{
	BOOL bSuccess = FALSE;
	if (OpenClipboard())
	{
		HGLOBAL hData = GetClipboardData(CF_TEXT);
		if (hData != NULL)
		{
			LPSTR pszData = (LPSTR) GlobalLock(hData);
			if (pszData != NULL)
			{
				text = pszData;
				GlobalUnlock(hData);
				bSuccess = TRUE;
			}
		}
		CloseClipboard();
	}
	return bSuccess;
}