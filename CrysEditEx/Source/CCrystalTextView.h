////////////////////////////////////////////////////////////////////////////
//	File:		CCrystalTextView.h
//	Version:	1.0.0.0
//	Created:	29-Dec-1998
//
//	Author:		Stcherbatchenko Andrei
//	E-mail:		windfall@gmx.de
//
//	Interface of the CCrystalTextView class, a part of Crystal Edit -
//	syntax coloring text editor.
//
//	You are free to use or modify this code to the following restrictions:
//	- Acknowledge me somewhere in your about box, simple "Parts of code by.."
//	will be enough. If you can't (or don't want to), contact me personally.
//	- LEAVE THIS HEADER INTACT
////////////////////////////////////////////////////////////////////////////

/********************************************************************
 *
 * $RCSfile: CCrystalTextView.h,v $
 * $Revision$
 * $Date$
 *
 * $Author$
 *
 * $Log: CCrystalTextView.h,v $
 * Revision 1.18  2007/01/23 14:41:28  niteria
 * Removed option to disable word wrap due to crashes caused by the spell checker.
 *
 * Revision 1.17  2007/01/15 18:50:40  niteria
 * New property page for the editor - includes now the new feature for word wrapping.
 * Renamed internal class and file name of property page for text format.
 *
 * Revision 1.16  2006/07/27 15:47:17  sschrade
 * Enhanced word wrapping style. The lines can be either wrapped at the window border or at a fixed amount of columns.
 *
 * Revision 1.15  2005/06/17 21:38:44  owieland
 * Show line numbers in editor (FR 1178333 )
 *
 * Revision 1.14  2005/03/08 16:36:08  vachis
 * Selection of block between bracket pairs in a hierarchical manner (Ctrl+M)
 * Bracket highlighting disabled if a selection is present
 * New default colours for bracket highlighting
 *
 * Revision 1.13  2005/02/23 23:58:19  vachis
 * Fixed Matching Bracket Highlighting
 * New features for Matching Bracket Highlighting: configure colours,
 *     highlight current block and pair, help
 *
 * Revision 1.12  2005/02/15 09:43:12  vachis
 * Implemented feature 536164: Matching Bracket Highlight
 *
 * Revision 1.11  2005/01/04 07:57:11  niteria
 * Implemented function to stop incremental search from the outside.
 *
 * Revision 1.10  2003/12/23 14:50:40  svenwiegand
 * Again modified some resource and language switching related stuff. Hoping this will work now for SmartTranslator...
 *
 * Revision 1.9  2003/12/16 20:04:38  svenwiegand
 * Implemented Feature 726766: "Option for selecting the language for the GUI"
 *
 * Revision 1.8  2003/12/06 19:59:58  svenwiegand
 * - Implemented Feature 601708 + additions: The user can now set the styles for
 *   the text cursor independant for the insert and the overwrite mode. The cursor
 *   style is defined by its form (line or block) and its mode (blinking or
 *   not-blinking). The defaults are 'line' for insert cursor and 'block' for
 *   overwrite cursor -- both are blinking.
 *
 * Revision 1.7  2002/04/27 07:21:59  cnorris
 * Avoid ambiguous function under Visual Studio < 5
 *
 * Revision 1.6  2002/04/23 21:45:09  cnorris
 * realtime spell check
 *
 * Revision 1.5  2002/04/09 23:22:41  cnorris
 * Added explicit virtual destructor
 *
 * Revision 1.4  2002/04/06 05:28:35  cnorris
 * Added SetShowInteractiveSelection
 * Added GetParser
 * Added default value to nLength of HighlightText
 *
 * Revision 1.3  2002/03/20 00:48:11  cnorris
 * Mouse scroll wheel handler
 *
 * Revision 1.2  2002/03/20 00:15:29  cnorris
 * Code clean up and new / delete code inspection
 *
 * Revision 1.1.1.1  2002/02/26 08:11:58  svenwiegand
 * Initial revision
 *
 * Revision 1.0  2000-05-31 21:55:26+02  sven_wiegand
 * Initial revision
 *
 *
 ********************************************************************/

#if !defined(AFX_CCRYSTALTEXTVIEW_H__AD7F2F41_6CB3_11D2_8C32_0080ADB86836__INCLUDED_)
#define AFX_CCRYSTALTEXTVIEW_H__AD7F2F41_6CB3_11D2_8C32_0080ADB86836__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "cedefs.h"
#include "CrystalParser.h"
#include <afxmt.h>


////////////////////////////////////////////////////////////////////////////
// Forward class declarations

class CCrystalTextBuffer;
class CUpdateContext;



////////////////////////////////////////////////////////////////////////////
// CCrystalTextView class declaration

//	CCrystalTextView::FindText() flags

enum
{
	FIND_MATCH_CASE = 0x0001,
	FIND_WHOLE_WORD = 0x0002,
	FIND_DIRECTION_UP = 0x0010,
	REPLACE_SELECTION = 0x0100
};

//	CCrystalTextView::UpdateView() flags

enum
{
	UPDATE_HORZRANGE = 0x0001, //	update horz scrollbar
	UPDATE_VERTRANGE = 0x0002, //	update vert scrollbar
	UPDATE_SINGLELINE = 0x0100, //	single line has changed
	UPDATE_FLAGSONLY = 0x0200, //	only line-flags were changed
	UPDATE_LINEATTR = 0x0400, //	only line-attributes were changed

	UPDATE_RESET = 0x1000 //	document was reloaded, update all!
};

/** Word wrapping flags

        PLEASE NOTE:

        Disabling the line wrapping is currently not possible since the edit control
        crashes due to a problem with styling. This problem occurs only in combination
        with online spell checking.

        If this problem has been fixed, please edit

                OptionPageEditor.cpp (m_nWordWrapStyle +/- 1)
                        and its dialog resource
 */
enum TCRYSTALWORDWRAP
{
	WORD_WRAP_NONE = 0,
	WORD_WRAP_WINDOW = 1,
	WORD_WRAP_FIXEDCOLUMN = 2
};

class CRYSEDIT_CLASS_DECL CCrystalTextView : public CView
{
	DECLARE_DYNCREATE(CCrystalTextView)

	int mouse_wheel_delta_;

	//BEGIN SW
	friend CCrystalParser;
	//END SW

private:
	int m_nHoldCount; // Number of holds on view. View cannot be destroyed with outstanding holds.
	CEvent *m_pevtHoldCountZero; // Signaled when hold count is zero.
	CRITICAL_SECTION m_csHold; // Protect hold count operations.

	//	Search parameters
	BOOL m_bLastSearch;
	DWORD m_dwLastSearchFlags;
	LPTSTR m_pszLastFindWhat;
	BOOL m_bMultipleSearch; // More search

	BOOL m_bCursorHidden;

	//	Painting caching bitmap
	CBitmap *m_pCacheBitmap;

	// Text attribute images
	CImageList m_TextAttributeImages;

	//	Line/character dimensions
	int m_nLineHeight, m_nCharWidth;
	void CalcLineCharDim();

	//	Text attributes
	int m_nTabSize;
	BOOL m_bViewTabs;
	BOOL m_bSelMargin;

	//	Amount of lines/characters that completely fits the client area
	int m_nScreenLines, m_nScreenChars;

	// the current wrapping style
	TCRYSTALWORDWRAP m_WordWrapType;

	// number of chars in a line when WORD_WRAP_FIXEDCOLUMN
	int m_nFixedColumn;

	//BEGIN SW
	/**
	Contains for each line the number of sublines. If the line is not
	wrapped, the value for this line is 1. The value of a line is invalid,
	if it is -1.

	Must create pointer, because contructor uses AFX_ZERO_INIT_OBJECT to
	initialize the member objects. This would destroy a CArray object.
	 */
	CArray<int, int> *m_panSubLines;
	/**
	Cache the number of sublines
	 */
	int m_nSubLineCount;
	//END SW

	int m_nMaxLineLength;
	int m_nIdealCharPos;

	BOOL m_bFocused;
	CPoint m_ptAnchor;
	LOGFONT m_lfBaseFont;
	CFont *m_apFonts[4];

	//	Parsing stuff
	DWORD *m_pdwParseCookies;
	int m_nParseArraySize;
	DWORD GetParseCookie(int nLineIndex);

	//Pair String
	CPoint m_ptPairStringStart;
	CPoint m_ptPairStringEnd;
	int m_nPairStringColorText;
	int m_nPairStringColorBkgnd;
	CPoint m_ptCurStringStart;
	CPoint m_ptCurStringEnd;
	int m_nCurStringColorText;
	int m_nCurStringColorBkgnd;
	BOOL m_bDrawPairBlock;

	//block selection
	int m_nSelBlockLevel;

	//	Pre-calculated line lengths (in characters)
	int m_nActualLengthArraySize;
	int *m_pnActualLineLength;

	BOOL m_bPreparingToDrag;
	BOOL m_bDraggingText;
	BOOL m_bDragSelection, m_bWordSelection, m_bLineSelection;
	UINT m_nDragSelTimer;
	CPoint WordToRight(CPoint pt);
	CPoint WordToLeft(CPoint pt);

	CPoint m_ptDrawSelStart, m_ptDrawSelEnd;
	CPoint m_ptCursorPos;
	CPoint m_ptSelStart, m_ptSelEnd;
	void PrepareSelBounds();

	//	Helper functions
	void ExpandChars(LPCTSTR pszChars, int nOffset, int nCount, CString &line);
	/*
	Populate an array that shadows the expanded string. Each element in the array corresponds
	to a character in the expanded string. Each element contains the index of character of the
	unexpanded string. If the string has no expansion characters, the array contains the numbers 0
	to n, where n is the length of the unexpanded string.
	@param pszChars Unexpanded string
	@param nOffset The index to begin the expansion. The expansion begins at the beginning of
	        a screen line.
	@param nCount The length of the line until break.
	@param anIndices The array where the expanded indices are stored.
	@return Length of the screen line.
	 */
	int ExpandChars(LPCTSTR pszChars, int &nOffset, int nCount, int *anIndices);

	// int ApproxActualOffset(int nLineIndex, int nOffset); // Unused
	void AdjustTextPoint(CPoint &point);
	void DrawLineHelperImpl(CDC *pdc, CPoint &ptOrigin, const CRect &rcClip,
	                        LPCTSTR pszChars, int nOffset, int nCount);
	void DrawLineHelperRecur(CDC *pdc, CPoint &ptOrigin, const CRect &rcClip,
	                         int nColorIndex, LPCTSTR pszChars, int nOffset,
	                         int nCount, CPoint ptTextPos, int nDrawStage);
	BOOL IsInsideSelBlock(CPoint ptTextPos);

	BOOL m_bBookmarkExist; // More bookmarks


	/** Finds pair string or nth open pair string. It checks if pair strings match each other
	(not to allow this '({)}').

	@param ptTextPost
	        Position in the text document. It can be before the beginning of line or after the end of line then the
	        previous or next line is immediately processed.
	@param nNthOpenPair
	        Find nth open pair
	@param nDirection
	        The direction of search (CLatexParser::DIRECTION_LEFT or CLatexParser::DIRECTION_RIGHT)
	@param aPairStack
	        Stack for search. It can be filled with starting bracket, or some required sequence of brackets that have
	        to be matched with their pairs. In case of error you can find from the stack when the error did occure.
	@param bClearToEnd
	        When the function is about exit according to normal run, it will additionally check the rest of text document
	        whether it conatins any open pair. If do so, an error is returned.

	@param ptFoundStrStart
	        The start of found pair or start of string that caused an error.
	@param ptFoundStrEnd
	        The end of found pair or end of string that caused an error. The end is the position just after the string.

	@param openPairStack
	        Output, the stack where open pairs are stored.
	@param bClearEndReached
	        <code>TRUE</code> if the parameter <code>bClearToEnd<code> was set and the end/start of file
	        was reached without an error.

	@return FALSE if the pair was not found or if some pair strings didn't match ("{(})"case ).
	        The start and the end of error are returned in <code>ptFoundStrStart</code>, <code>ptFoundStrEnd</code>,
	        and <code>aPairStack</code> stores the state of stack when the error did occure. If the stack is empty then
	        the beginning or end of file was reached (it depends on direction).
	        If an unknown error occures both <code>ptPairStrStart</code> and <code>ptPairStrEnd</code> are set to <code>CPoin(0,0)</code>
	 */
	BOOL FindPairHelper(const CPoint &ptTextPos, int nNthOpenPair, int nDirection, CCrystalParser::CPairStack &aPairStack,
	                    BOOL bClearToEnd, const CPoint &ptCurStrStart, const CPoint &ptCurStrEnd,
	                    CPoint &ptFoundStrStart, CPoint &ptFoundStrEnd, CCrystalParser::CPairStack &openPairStack,
	                    BOOL &bClearEndReached);

	/**Returns TRUE if some string from <code>m_lpszPairs</code> ends at ptTextPos and this string
	is not escaped by sequence of '\', it is not in comment, and it is not in verbatim

	@param ptPairStrStart
	  Start of found string
	@param ptPairStrEnd
	  End of found string, the end is the position just after the string.
	@param nPairIdx
	        Found string index
	@param nPairDir
	        Found string direction: DIRECTION_LEFT or DIRECITON_RIGHT
	 */
	BOOL IsEndOfPairAtHelper(const CPoint &ptTextPos, CPoint &ptFoundStrStart,
	                         int &nPairIdx, int &nPairDir);

	//	CCrystalTextView::DrawLineHelperRecur() flags

	enum
	{
		DRAW_SELECTION = 0x0001,
		DRAW_PAIR = 0x0002,
		DRAW_CURPAIR = 0x0003,
		DRAW_PAIRBLOCK = 0x0004,
		DRAW_IMPL = 0x0005
	};

	//	CCrystalTextView::FindPairHelper() contants

	enum
	{
		PAIR_NONE = -1
	};

protected:
	CImageList *m_pIcons;
	CCrystalTextBuffer *m_pTextBuffer;
	HACCEL m_hAccel;
	BOOL m_bVertScrollBarLocked, m_bHorzScrollBarLocked;
	CPoint m_ptDraggedTextBegin, m_ptDraggedTextEnd;
	virtual void ResetView();
	void UpdateCaret();
	void SetAnchor(const CPoint &ptNewAnchor);
	int GetMarginWidth();

	CPoint GetPairStringStart() const;
	CPoint GetPairStringEnd() const;
	void SetPairStringStart(const CPoint &ptStart);
	void SetPairStringEnd(const CPoint &ptEnd);
	int GetPairStringColorBkgnd() const;
	int GetPairStringColorText() const;
	void SetPairStringColorBkgnd(int nColorIndex);
	void SetPairStringColorText(int nColorIndex);

	CPoint GetCurStringStart() const;
	CPoint GetCurStringEnd() const;
	void SetCurStringStart(const CPoint &ptStart);
	void SetCurStringEnd(const CPoint &ptEnd);
	int GetCurStringColorBkgnd() const;
	int GetCurStringColorText() const;
	void SetCurStringColorBkgnd(int nColorIndex);
	void SetCurStringColorText(int nColorIndex);

	BOOL GetDrawPairBlock() const;
	void SetDrawPairBlock(BOOL bDrawBlock);

	void IncSelBlockLevel();
	void ResetSelBlockLevel();

	BOOL m_bShowInactiveSelection;
	//	[JRT]
	BOOL m_bDisableDragAndDrop;
	//BEGIN SW
	//BOOL m_bWordWrap;
	CCrystalParser *m_pParser;
	//END SW
	CWinThread *m_pBackgroundThread;

	CPoint ClientToText(const CPoint &point);
	CPoint TextToClient(const CPoint &point);
	void InvalidateLines(int nLine1, int nLine2, BOOL bInvalidateMargin = FALSE);
	int CalculateActualOffset(int nLineIndex, int nCharIndex);

	//	Printing
	int m_nPrintPages;
	int *m_pnPages;
	CFont *m_pPrintFont;
	int m_nPrintLineHeight;
	BOOL m_bPrintHeader, m_bPrintFooter;
	CRect m_ptPageArea, m_rcPrintArea;
	int PrintLineHeight(CDC *pdc, int nLine);
	void RecalcPageLayouts(CDC *pdc, CPrintInfo *pInfo);
	virtual void PrintHeader(CDC *pdc, int nPageNum);
	virtual void PrintFooter(CDC *pdc, int nPageNum);
	virtual void GetPrintHeaderText(int nPageNum, CString &text);
	virtual void GetPrintFooterText(int nPageNum, CString &text);

	//	Keyboard handlers
	void MoveLeft(BOOL bSelect);
	void MoveRight(BOOL bSelect);
	void MoveWordLeft(BOOL bSelect);
	void MoveWordRight(BOOL bSelect);
	void MoveUp(BOOL bSelect);
	void MoveDown(BOOL bSelect);
	void MoveHome(BOOL bSelect);
	void MoveEnd(BOOL bSelect);
	void MovePgUp(BOOL bSelect);
	void MovePgDn(BOOL bSelect);
	void MoveCtrlHome(BOOL bSelect);
	void MoveCtrlEnd(BOOL bSelect);

	void SelectAll();
	void Copy();


public:
	BOOL IsSelection();
	BOOL IsInsideSelection(const CPoint &ptTextPos);
	void GetSelection(CPoint &ptStart, CPoint &ptEnd);
	void SetSelection(const CPoint &ptStart, const CPoint &ptEnd);

	void SetShowInteractiveSelection(BOOL bShowInactiveSelection)
	{
		m_bShowInactiveSelection = bShowInactiveSelection;
	}

	/**Selects block around the cursor, it uses last block level
	@param ptCursorPos position in the text
	@return FALSE if an error occures
	 */
	BOOL SelectBlockAround(const CPoint &ptCursorPos);

	/**Selects block around the cursor.

	@param ptCursorPos position in the text

	@param nBlockLevel
	        block level. Let us have text "[{()}]" and
	        let the cursor is in the middle then 1st level block is (), 2nd level block is {()},
	        and 3th level block is [{()}]
	        level 0 clears selected block

	@return FALSE if an error occures
	 */
	BOOL SelectBlockAround(const CPoint &ptCursorPos, int nBlockLevel);

	static HINSTANCE GetResourceHandle();

	/** Stops incremental search.

	        @param bKeepSelection
	                Whether to stay at the found place and keep the selection,
	                or return to where the search started.

	        @return true, if incremental search was active before calling this func.
	        @return false, otherwise.
	 */
	bool OnEditFindIncrementalStop(bool bKeepSelection);

	/**
	Sets the word wrapping style.

	@param wrap
	the style can be one of:
	        WORD_WRAP_NONE,
	        WORD_WRAP_WINDOW,
	        WORD_WRAP_FIXEDCOLUMN
	 */
	void SetWordWrapping(TCRYSTALWORDWRAP wrap);

	/**
	Returns TRUE when word wrap style is WORD_WRAP_FIXEDCOLUMN
	 */
	BOOL IsFixedColumnWrap();

	/**
	Set the amount of chars per line.
	@param column
	        chars
	 */
	void SetFixedColumn(int column);

	/**
	Returns the amount of chars per line.
	 */
	int GetFixedColumn();

protected:
	int m_nTopLine, m_nOffsetChar;
	//BEGIN SW
	/**
	The index of the subline that is the first visible line on the screen.
	 */
	int m_nTopSubLine;
	//END SW
	BOOL m_bSmoothScroll;

	int GetLineHeight();
	//BEGIN SW
	/**
	Returns the number of sublines the given line contains of.
	Allway "1", if word wrapping is disabled.

	@param nLineIndex Index of the line to get the subline count of.

	@return Number of sublines the given line contains of
	 */
	int GetSubLines(int nLineIndex);

	/**
	Converts the given character position for the given line into a point.

	After the call the x-member of the returned point contains the
	character position relative to the beginning of the subline. The y-member
	contains the zero based index of the subline relative to the line, the
	character position was given for.

	@param nLineIndex Zero based index of the line, nCharPos refers to.
	@param nCharPos The character position, the point shoult be calculated for.
	@param charPoint Reference to a point, which should receive the result.

	@return The character position of the beginning of the subline charPoint.y.
	 */
	int CharPosToPoint(int nLineIndex, int nCharPos, CPoint &charPoint);

	/**
	Converts the given cursor point for the given line to the character position
	for the given line.

	The y-member of the cursor position specifies the subline inside the given
	line, the cursor is placed on and the x-member specifies the cursor position
	(in character widths) relative to the beginning of that subline.

	@param nLineIndex Zero based index of the line the cursor position refers to.
	@param curPoint Position of the cursor relative to the line in sub lines and
	        TCHAR widths.

	@return The character position the best matches the cursor position.
	 */
	int CursorPointToCharPos(int nLineIndex, const CPoint &curPoint);

	/**
	Converts the given cursor position to a text position.

	The x-member of the subLinePos parameter describes the cursor position in
	TCHAR widths relative to the beginning of the subline described by the
	y-member. The subline is the zero based number of the subline relative to
	the beginning of the text buffer.

	<p>
	The returned point contains a valid text position, where the y-member is
	the zero based index of the textline and the x-member is the character
	position inside this line.

	@param subLinePos The sublinebased cursor position
	        (see text above for detailed description).
	@param textPos The calculated line and character position that best matches
	        the cursor position (see text above for detailed descritpion).
	 */
	void SubLineCursorPosToTextPos(const CPoint &subLinePos, CPoint &textPos);

	/**
	Returns the character position relative to the given line, that matches
	the end of the given sub line.

	@param nLineIndex Zero based index of the line to work on.
	@param nSubLineOffset Zero based index of the subline inside the given line.

	@return Character position that matches the end of the given subline, relative
	        to the given line.
	 */
	int SubLineEndToCharPos(int nLineIndex, int nSubLineOffset);

	/**
	Returns the character position relative to the given line, that matches
	the start of the given sub line.

	@param nLineIndex Zero based index of the line to work on.
	@param nSubLineOffset Zero based index of the subline inside the given line.

	@return Character position that matches the start of the given subline, relative
	        to the given line.
	 */
	int SubLineHomeToCharPos(int nLineIndex, int nSubLineOffset);
	//END SW

	int GetCharWidth();
	int GetMaxLineLength();
	int GetScreenLines();
	int GetScreenChars();
	CFont *GetFont(BOOL bItalic = FALSE, BOOL bBold = FALSE);

	void RecalcVertScrollBar(BOOL bPositionOnly = FALSE);
	void RecalcHorzScrollBar(BOOL bPositionOnly = FALSE);

	//	Scrolling helpers
	void ScrollToChar(int nNewOffsetChar, BOOL bNoSmoothScroll = FALSE, BOOL bTrackScrollBar = TRUE);
	void ScrollToLine(int nNewTopLine, BOOL bNoSmoothScroll = FALSE, BOOL bTrackScrollBar = TRUE);
	//BEGIN SW
	/**
	Scrolls to the given sub line, so that the sub line is the first visible
	line on the screen.

	@param nNewTopSubLine Index of the sub line to scroll to.
	@param bNoSmoothScroll TRUE to disable smooth scrolling, else FALSE.
	@param bTrackScrollBar TRUE to recalculate the scroll bar after scrolling,
	        else FALSE.
	 */
	void ScrollToSubLine(int nNewTopSubLine, BOOL bNoSmoothScroll = FALSE, BOOL bTrackScrollBar = TRUE);
	//END SW

	//	Splitter support
	virtual void UpdateSiblingScrollPos(BOOL bHorz);
	virtual void OnUpdateSibling(CCrystalTextView *pUpdateSource, BOOL bHorz);
	CCrystalTextView *GetSiblingView(int nRow, int nCol);

	virtual int GetLineCount();
	//BEGIN SW
	/**
	Returns the number of sublines in the whole text buffer.

	The number of sublines is the sum of all sublines of all lines.

	@return Number of sublines in the whole text buffer.
	 */
	virtual int GetSubLineCount();

	/**
	Returns the zero-based subline index of the given line.

	@param nLineIndex Index of the line to calculate the subline index of

	@return The zero-based subline index of the given line.
	 */
	virtual int GetSubLineIndex(int nLineIndex);

	/**
	Splits the given subline index into line and sub line of this line.

	@param nSubLineIndex The zero based index of the subline to get info about
	@param nLine Gets the line number the give subline is included in
	@param nSubLine Get the subline of the given subline relative to nLine
	 */
	virtual void GetLineBySubLine(int nSubLineIndex, int &nLine, int &nSubLine);
	//END SW

	virtual int GetLineLength(int nLineIndex);
	virtual int GetLineActualLength(int nLineIndex);
	virtual LPCTSTR GetLineChars(int nLineIndex);
	virtual DWORD GetLineFlags(int nLineIndex);
	virtual void GetText(const CPoint &ptStart, const CPoint &ptEnd, CString &text);

	//	Clipboard overridable
	virtual BOOL TextInClipboard();
	virtual BOOL PutToClipboard(LPCTSTR pszText);
	virtual BOOL GetFromClipboard(CString &text);

	//	Drag-n-drop overrideable
	virtual HGLOBAL PrepareDragData();
	virtual DROPEFFECT GetDropEffect();
	virtual void OnDropSource(DROPEFFECT de);
	BOOL IsDraggingText() const;

	virtual COLORREF GetColor(int nColorIndex);
	virtual void GetLineColors(int nLineIndex, COLORREF &crBkgnd,
	                           COLORREF &crText, BOOL &bDrawWhitespace);
	virtual BOOL GetItalic(int nColorIndex);
	virtual BOOL GetBold(int nColorIndex);

	void DrawLineHelper(CDC *pdc, CPoint &ptOrigin, const CRect &rcClip, int nColorIndex,
	                    LPCTSTR pszChars, int nOffset, int nCount, CPoint ptTextPos);

	virtual void DrawSingleLine(CDC *pdc, const CRect &rect, int nLineIndex);
	virtual void DrawMargin(CDC *pdc, const CRect &rect, int nLineIndex);
	virtual void DrawLineAttributes(CDC *pdc, CPoint ptOrigin, int nLineIndex, int *anBreaks, int nBreaks);

	//BEGIN SW
	// word wrapping

	/**
	Called to wrap the line with the given index into sublines.

	The standard implementation wraps the line at the first non-whitespace after
	an whitespace that exceeds the visible line width (nMaxLineWidth). Override
	this function to provide your own word wrapping.

	<b>Attention:</b> Never call this function directly,
	call WrapLineCached() instead, which calls this method.

	@param nLineIndex The index of the line to wrap

	@param nMaxLineWidth The number of characters a subline of this line should
	not exceed (except whitespaces)

	@param anBreaks An array of integers. Put the positions where to wrap the line
	in that array (its allready allocated). If this pointer is NULL, the function
	has only to compute the number of breaks (the parameter nBreaks).

	@param nBreaks The number of breaks this line has (number of sublines - 1). When
	the function is called, this variable is 0. If the line is not wrapped, this value
	should be 0 after the call.

	@see WrapLineCached()
	 */
	virtual void WrapLine(int nLineIndex, int nMaxLineWidth, int *anBreaks, int &nBreaks);

	/**
	Called to wrap the line with the given index into sublines.

	Call this method instead of WrapLine() (which is called internal by this
	method). This function uses an internal cache which contains the number
	of sublines for each line, so it has only to call WrapLine(), if the
	cache for the given line is invalid or if the caller wants to get the
	wrap postions (anBreaks != NULL).

	This functions also tests m_bWordWrap -- you can call it even if
	word wrapping is disabled and you will retrieve a valid value.

	@param nLineIndex The index of the line to wrap

	@param nMaxLineWidth The number of characters a subline of this line should
	not exceed (except whitespaces)

	@param anBreaks An array of integers. Put the positions where to wrap the line
	in that array (its allready allocated). If this pointer is NULL, the function
	has only to compute the number of breaks (the parameter nBreaks).

	@param nBreaks The number of breaks this line has (number of sublines - 1). When
	the function is called, this variable is 0. If the line is not wrapped, this value
	should be 0 after the call.

	@see WrapLine()
	@see m_anSubLines
	 */
	void WrapLineCached(int nLineIndex, int nMaxLineWidth, int *anBreaks, int &nBreaks);

	/**
	Invalidates the cached data for the given lines.

	<b>Remarks:</b> Override this method, if your derived class caches other
	view specific line info, which becomes invalid, when this line changes.
	Call this standard implementation in your overriding.

	@param nLineIndex1 The index of the first line to invalidate.

	@param nLineIndex2 The index of the last line to invalidate. If this value is
	-1 (default) all lines from nLineIndex1 to the end are invalidated.
	 */
	virtual void InvalidateLineCache(int nLineIndex1, int nLineIndex2);
	//END SW

	//	Syntax coloring overrides

	struct TEXTBLOCK
	{
		int m_nCharPos;
		int m_nColorIndex;
	};

	virtual DWORD ParseLine(DWORD dwCookie, int nLineIndex, TEXTBLOCK *pBuf, int &nActualItems);

	//BEGIN SW
	// function to draw a single screen line
	// (a wrapped line can consist of many screen lines
	virtual void DrawScreenLine(CDC *pdc, CPoint &ptOrigin, const CRect &rcClip,
	                            TEXTBLOCK *pBuf, int nBlocks, int &nActualItem,
	                            COLORREF crText, COLORREF crBkgnd, BOOL bDrawWhitespace,
	                            LPCTSTR pszChars,
	                            int nOffset, int nCount, CPoint ptTextPos);
	//END SW

	//brackets higlighting
	/**Finds pair string (e.g. bracket) to the string that ends at <code>ptCursorPos</code>.
	        The marked string is drawn during next redraw. The calling discards previously
	        marked string.

	@param ptCursorPos
	        Typically it's cursor position,
	@param bMarkBlock
	        Mark all block between start pair and end pair (brackets)

	@return FALSE if the string that ends at given position have its pair string (e.g. bracket)
	        but this pair have not been found
	        TRUE otherwise
	 */
	virtual BOOL MarkPairStringTo(const CPoint &ptCursorPos, BOOL bMarkBlock);

	/**Unmark string previoslly marked by <code>MarkPairStringTo</code>.
	 */
	virtual void UnmarkPairString();


	/**Finds a pair to the string that ends at <code>ptCursorPos</code> (e.g. pair to a bracket)
	@param ptCursorPos
	        Cursor position.

	@param ptCurStrStart
	        The start of the pair string that ends <code>ptCursorPos</code>,
	        (0,0) if not found. (It depends on the value of <code>bPairFound</code>.)

	@param ptCurStrEnd
	        The end of the pair string that ends <code>ptCursorPos</code>,
	        (0,0) if not found. (It depends on the value of <code>bPairFound</code>.). The end is the position just after the string.

	@param ptFoundStrStar
	        The start of found pair or the start of the string that caused an error,
	        (0,0) if not found. (It depends on return value.)

	@param ptFoundStrEnd
	        The end of found pair or the end of the string that caused an error,
	        (0,0) if not found. (It depends on return value.). The end is the position just after the string.

	@param bPairFound
	        <code>TRUE</code> if the pair string was found

	@return <code>TRUE</code> if the pair was found or no pair string ends at the <code>ptCursorPos</code>
	(If <code>bPairFound == FALSE</code> all points parameter are set to zero point,
	if <code>bPairFound == TRUE</code> all parameter are set).
	FALSE otherwise (<code>FoundStrStar</code>, <code>FoundStrEnd</code> contain
	the position of the string that caused an error  (e.g. lost bracket))
	 */
	virtual BOOL FindPairTo(const CPoint &ptCursorPos, CPoint &ptCurStrStart, CPoint &ptCurStrEnd,
	                        CPoint &ptFoundStrStart, CPoint &ptFoundStrEnd, BOOL &bPairFound);


	//BEGIN SW
	// helpers for incremental search

	/**
	Called each time the position-information in the status bar
	is updated. Use this to change the text of the message field
	in the status bar.

	@param pStatusBar
	        Pointer to the status bar.
	 */
	void OnUpdateStatusMessage(CWnd *pStatusBar);

	/**
	Called to display the given message in the status bar.

	This base implementation tests if pStatusBar is of the class
	CStatusBar (or derived) and sets the pane text if this is the
	case.

	Override this method if your application does not use CStatusBar
	or derived classes.

	@param pStatusBar
	        Pointer to the status bar.
	@param lpszMessage
	        Message to display in the status bar.
	 */
	virtual void OnSetStatusMessage(CWnd *pStatusBar, LPCTSTR lpszMessage);

	/**	Finds a term incrementally.

	@param bFindNextOccurence
	        TRUE, if the method should look for the next occurence of the
	        search string in search direction.

	@see OnEditFindIncrementalForward
	@see OnEditFindIncrementalBackward
	@see OnEditFindIncrementalStop
	 */
	void OnEditFindIncremental(BOOL bFindNextOccurence = FALSE);

	/** TRUE if incremental forward search is active, FALSE otherwise */
	BOOL m_bIncrementalSearchForward;

	/** TRUE if incremental backward search is active, FALSE otherwise */
	BOOL m_bIncrementalSearchBackward;


private:
	/** TRUE if we found the string to search for */
	BOOL m_bIncrementalFound;

	/** String we are looking for.*/
	CString *m_pstrIncrementalSearchString;

	/** String we looked for last time.*/
	CString *m_pstrIncrementalSearchStringOld;

	/** Start of selection at the time the incremental search started */
	CPoint m_selStartBeforeIncrementalSearch;

	/** Start of selection at the time the incremental search started */
	CPoint m_selEndBeforeIncrementalSearch;

	/** Cursor position at the time the incremental search started */
	CPoint m_cursorPosBeforeIncrementalSearch;

	/** position to start the incremental search at */
	CPoint m_incrementalSearchStartPos;

	/** The current form of the caret when in insert mode */
	static int s_nCaretInsertForm;

	/** The current mode of the caret when in insert mode */
	static int s_nCaretInsertMode;

	/** The current form of the caret when in overwrite mode */
	static int s_nCaretOverwriteForm;

	/** The current mode of the caret when in overwrite mode */
	static int s_nCaretOverwriteMode;
	//END SW

	static BOOL m_bShowLineNumbers;


// Attributes
public:
	BOOL GetViewTabs();
	void SetViewTabs(BOOL bViewTabs);
	int GetTabSize();
	void SetTabSize(int nTabSize);
	BOOL GetSelectionMargin();
	void SetSelectionMargin(BOOL bSelMargin);
	void GetFont(LOGFONT &lf);
	void SetFont(const LOGFONT &lf);
	BOOL GetSmoothScroll() const;
	void SetSmoothScroll(BOOL bSmoothScroll);
	//	[JRT]:
	BOOL GetDisableDragAndDrop() const;
	void SetDisableDragAndDrop(BOOL bDDAD);
	//BEGIN SW
	TCRYSTALWORDWRAP GetWordWrapping() const;
	//void SetWordWrapping( BOOL bWordWrap );

	/**
	Increment the hold view count. The view cannot be destroyed with outstanding
	holds. Every call must be matched with a call to Release()
	@param bExclusive Hold exclusively. Wait until the hold count reaches zero,
	then make an exclusive hold of view. All future holds block until the
	exlcusive hold is released. Must be matched by call to Release(bExclusive).
	@return Approximate number of outstanding holds.
	 */
	int Hold(bool bExclusive = false);

	/**
	Decrement the hold view count. The view cannot be destroyed with outstanding
	holds. Every call must be matched with a previous call to Hold()
	@param bExclusive Release exclusively. Must match call to Hold(bExclusive).
	@return Approximate number of outstanding holds.
	 */
	int Release(bool bExclusive = false);

	/**
	Sets the Parser to use to parse the file.

	@param pParser Pointer to parser to use. Set to NULL to use no parser.

	@return Pointer to parser used before or NULL, if no parser has been used before.
	 */
	CCrystalParser *SetParser(CCrystalParser *pParser);
	//END SW

	CCrystalParser *GetParser() const;

	//	Default handle to resources
	static HINSTANCE s_hResourceInst;

	enum
	{
		//	Base colors
		COLORINDEX_WHITESPACE,
		COLORINDEX_BKGND,
		COLORINDEX_NORMALTEXT,
		COLORINDEX_SELMARGIN,
		COLORINDEX_SELBKGND,
		COLORINDEX_SELTEXT,
		//	Syntax colors
		COLORINDEX_KEYWORD,
		COLORINDEX_COMMENT,
		COLORINDEX_NUMBER,
		COLORINDEX_OPERATOR, // [JRT]:
		COLORINDEX_STRING,
		COLORINDEX_PREPROCESSOR,

		// Brackets higlighting
		COLORINDEX_PAIRSTRINGBKGND,
		COLORINDEX_PAIRSTRINGTEXT,
		COLORINDEX_BADPAIRSTRINGBKGND,
		COLORINDEX_BADPAIRSTRINGTEXT,
		COLORINDEX_CURPAIRSTRINGBKGND,
		COLORINDEX_CURPAIRSTRINGTEXT,
		COLORINDEX_PAIRBLOCKBKGND,

		//	Compiler/debugger colors
		COLORINDEX_ERRORBKGND,
		COLORINDEX_ERRORTEXT,
		COLORINDEX_EXECUTIONBKGND,
		COLORINDEX_EXECUTIONTEXT,
		COLORINDEX_BREAKPOINTBKGND,
		COLORINDEX_BREAKPOINTTEXT
		//	...
		//	Expandable: custom elements are allowed.
	};

	//BEGIN SW

	/**
	Defines constants to be used when calling SetInsertCaretStyle(),
	SetOverwriteCaretStyle().

	@see tagCaretMode
	 */
	enum tagCaretForm
	{
		/** Caret is a vertical line */
		CARET_LINE,

		/** Caret is a block */
		CARET_BLOCK
	};

	/**
	Defines constants to be used when calling SetInsertCaretStyle(),
	SetOverwriteCaretStyle().

	@see tagCaretForm
	 */
	enum tagCaretMode
	{
		/** Caret is blinking */
		CARET_BLINK,

		/** Caret is not blinking */
		CARET_STATIC
	};
	//END SW

// Operations
public:
	void AttachToBuffer(CCrystalTextBuffer *pBuf = NULL);
	void DetachFromBuffer();

	//	Buffer-view interaction, multiple views
	virtual CCrystalTextBuffer *LocateTextBuffer();
	virtual void UpdateView(CCrystalTextView *pSource, CUpdateContext *pContext, DWORD dwFlags, int nLineIndex = -1);

	//	Attributes
	CPoint GetCursorPos();
	void SetCursorPos(const CPoint &ptCursorPos);
	void ShowCursor();
	void HideCursor();

	//	Operations
	void EnsureVisible(CPoint pt);

	/**
	Sets the caret on the first column of the given line and ensures, that the
	caret is visible.

	@param nLine Line to go to.
	 */
	void GoToLine(int nLine);

	//	Text search helpers
	BOOL FindText(LPCTSTR pszText, const CPoint &ptStartPos, DWORD dwFlags, BOOL bWrapSearch, CPoint *pptFoundPos);
	BOOL FindTextInBlock(LPCTSTR pszText, const CPoint &ptStartPos, const CPoint &ptBlockBegin, const CPoint &ptBlockEnd,
	                     DWORD dwFlags, BOOL bWrapSearch, CPoint *pptFoundPos);
	BOOL HighlightText(const CPoint &ptStartPos, int nLength = -1);

	//	Overridable: an opportunity for Auto-Indent, Smart-Indent etc.
	virtual void OnEditOperation(int nAction, LPCTSTR pszText);

	/**
	Ensures, that the given text position is a valid one. Adjusts the
	position if necessary.
	 */
	void EnsureValidTextPos(CPoint &ptTextPos);
	//BEGIN SW

	/**
	Returns the text of the current selection. If there is no
	selection, the word, the cursor is placed on, is returned.

	@param strSelection
	        Reference to the CString-object, that should receive the
	        selected text.
	 */
	void GetSelectedText(CString &strSelection);

	/**
	Returns TRUE if the view is in overwrite mode or FALSE if it is in
	insert mode.

	This base implementation always returns FALSE, but the derived
	CCrystalEditView class may return TRUE.
	 */
	virtual BOOL GetOverwriteMode() const;

	//@{
	/**
	Defines the style of the caret.

	The name of the method defines whether to set the given style for
	the caret in insert or in overwrite mode.

	The style is always set for all view instances.

	@param nForm
	        The form of the caret -- a value of the tagCaretForm enumeration.
	@param nMode
	        The mode of the caret -- a value of the tagCaretMode enumeration.
	 */
	static void SetCaretInsertStyle(int nForm, int nMode);
	static void SetCaretOverwriteStyle(int nForm, int nMode);
	//@}

	//@{
	/**
	Returns the specified caret setting which is either a value of the
	tagCaretForm or tagCaretMode enumeration.
	 */
	static int GetCaretInsertForm();
	static int GetCaretInsertMode();
	static int GetCaretOverwriteForm();
	static int GetCaretOverwriteMode();
	//@}

	//@{
	/**
	Returns the current caret settings depending on the insert/overwrite
	caret settings and the current mode (overwrite/insert).

	Depending on the called method the result is either a value of the
	tagCaretForm or tagCaretMode enumeration.
	 */
	int GetCaretForm();
	int GetCaretMode();
	//@}

	static void SetShowLineNumbers(BOOL show);
	static BOOL GetShowLineNumbers();

	//END SW

	// Overrides
	// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CCrystalTextView)
public:
	virtual void OnDraw(CDC* pDC); // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO *pHandlerInfo);
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	CCrystalTextView();
	virtual ~CCrystalTextView();

protected:

// Generated message map functions
protected:
#ifdef _DEBUG
	void AssertValidTextPos(const CPoint &pt);
#endif

	//{{AFX_MSG(CCrystalTextView)
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditSelectAll();
	afx_msg void OnUpdateEditSelectAll(CCmdUI* pCmdUI);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSysColorChange();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnEditFind();
	afx_msg void OnEditRepeat();
	afx_msg void OnUpdateEditRepeat(CCmdUI* pCmdUI);
	afx_msg void OnEditFindPrevious(); // More search
	afx_msg void OnUpdateEditFindPrevious(CCmdUI* pCmdUI);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEditDeleteBack();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	afx_msg void OnFilePageSetup();

	afx_msg void OnCharLeft();
	afx_msg void OnExtCharLeft();
	afx_msg void OnCharRight();
	afx_msg void OnExtCharRight();
	afx_msg void OnWordLeft();
	afx_msg void OnExtWordLeft();
	afx_msg void OnWordRight();
	afx_msg void OnExtWordRight();
	afx_msg void OnLineUp();
	afx_msg void OnExtLineUp();
	afx_msg void OnLineDown();
	afx_msg void OnExtLineDown();
	afx_msg void OnPageUp();
	afx_msg void OnExtPageUp();
	afx_msg void OnPageDown();
	afx_msg void OnExtPageDown();
	afx_msg void OnLineEnd();
	afx_msg void OnExtLineEnd();
	afx_msg void OnHome();
	afx_msg void OnExtHome();
	afx_msg void OnTextBegin();
	afx_msg void OnExtTextBegin();
	afx_msg void OnTextEnd();
	afx_msg void OnExtTextEnd();
	afx_msg void OnSelBiggerBlock();
	afx_msg void OnUpdateIndicatorCRLF(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIndicatorPosition(CCmdUI* pCmdUI);
	afx_msg void OnToggleBookmark(UINT nCmdID);
	afx_msg void OnGoBookmark(UINT nCmdID);
	afx_msg void OnClearBookmarks();

	afx_msg void OnToggleBookmark(); // More bookmarks
	afx_msg void OnClearAllBookmarks();
	afx_msg void OnNextBookmark();
	afx_msg void OnPrevBookmark();
	afx_msg void OnUpdateClearAllBookmarks(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNextBookmark(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePrevBookmark(CCmdUI* pCmdUI);

	afx_msg void ScrollUp();
	afx_msg void ScrollDown();
	afx_msg void ScrollLeft();
	afx_msg void ScrollRight();


	//BEGIN SW
	// incremental search
	afx_msg void OnEditFindIncrementalForward();
	afx_msg void OnEditFindIncrementalBackward();
	//END SW

	DECLARE_MESSAGE_MAP()
public:
	void OnFilePrintPreview(void);
};

/**
Create an object of this class in every method, that accesses the
local resources.

@author Sven Wiegand
 */
class CCrystalResources
{
// construction/destruction
public:

	CCrystalResources(HINSTANCE hResources = CCrystalTextView::GetResourceHandle())
	{
		m_hPrevResources = AfxGetResourceHandle();
		m_hResources = hResources;
		AfxSetResourceHandle(m_hResources);
	}

	virtual ~CCrystalResources()
	{
		RestorePrevResources();
	}

	void RestorePrevResources()
	{
		if (m_hPrevResources)
			AfxSetResourceHandle(m_hPrevResources);
		m_hResources = NULL;
	}

// Attribute
public:
	HINSTANCE m_hPrevResources;
	HINSTANCE m_hResources;
};


#ifdef _DEBUG
#define ASSERT_VALIDTEXTPOS(pt)		AssertValidTextPos(pt);
#else
#define ASSERT_VALIDTEXTPOS(pt)
#endif

#if ! (defined(CE_FROM_DLL) || defined(CE_DLL_BUILD))
#include "CCrystalTextView.inl"
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CCRYSTALTEXTVIEW_H__AD7F2F41_6CB3_11D2_8C32_0080ADB86836__INCLUDED_)
