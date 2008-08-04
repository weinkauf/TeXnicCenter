////////////////////////////////////////////////////////////////////////////
//	File:		CrystalParser.h
//
//	Author:		Sven Wiegand
//	E-mail:		sven.wiegand@gmx.de
//
//	Interface of the CCrystalParser class, a part of Crystal Edit -
//	syntax coloring text editor.
//
//	You are free to use or modify this code.
//
////////////////////////////////////////////////////////////////////////////

/********************************************************************
 *
 * $RCSfile: CrystalParser.h,v $
 * $Revision$
 * $Date$
 *
 * $Author$
 *
 * $Log: CrystalParser.h,v $
 * Revision 1.5  2005/03/16 18:18:00  vachis
 * fixed bugs: unmatched brace (after Ctrl+M) disappear when scrolling
 *                   disable higlighting when a selection is pressent
 *                   memory violation when  Ctrl+M is pressed in an emtpy document
 *                   bug in brace block selection
 *
 * Revision 1.4  2005/03/08 16:36:08  vachis
 * Selection of block between bracket pairs in a hierarchical manner (Ctrl+M)
 * Bracket highlighting disabled if a selection is present
 * New default colours for bracket highlighting
 *
 * Revision 1.3  2005/02/15 09:43:12  vachis
 * Implemented feature 536164: Matching Bracket Highlight
 *
 * Revision 1.2  2002/04/06 05:33:57  cnorris
 * Added NextWord method required for spell checker
 *
 * Revision 1.1.1.1  2002/02/26 08:12:01  svenwiegand
 * Initial revision
 *
 * Revision 1.0  2000-05-31 21:55:35+02  sven_wiegand
 * Initial revision
 *
 *
 ********************************************************************/

#if !defined(AFX_CRYSTALPARSER_H__17435DA0_1F72_11D3_929E_FD90DABF8479__INCLUDED_)
#define AFX_CRYSTALPARSER_H__17435DA0_1F72_11D3_929E_FD90DABF8479__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>


#include "CrystalTextBlock.h"

/**
Objects of this class are used to describe language properties. At this time
language properties are:
<ul>
        <li> Syntax highlighting (method ParseLine())
        <li> Word wrapping (method WrapLine())
        <li> Word extraction (method NextWord())
</ul>

Derive your own class from CCrystalParser and override the method ParseLine()
to modify syntax highlighting and/or WrapLine() to modify line wrapping.

Use the CCrystalTextView::SetParser() function to attach a parser to a text
view.

@author Sven Wiegand
 */
class CRYSEDIT_CLASS_DECL CCrystalParser : public CObject
{
	DECLARE_DYNAMIC(CCrystalParser)
	friend class CCrystalTextView;

// construction/destruction
public:
	CCrystalParser();
	virtual ~CCrystalParser();

//structures
public:

	typedef struct tagPairStackItm
	{
		int nPairIdx;
		int nPairDir;
		CPoint ptStart, ptEnd;
	} PAIR_STACK_ITM;

	typedef std::vector <PAIR_STACK_ITM> CPairStack;

	enum
	{
		DIRECTION_LEFT = 0,
		DIRECTION_RIGHT = 1
	};

	enum
	{
		RESULT_ERROR = 0,
		RESULT_OK = 1
	};

public:
	/** Changes direction to the its opposite.
	@param nDirection
	        DIRECTION_LEFT ot DIRECTION_RIGHT
	@return opposite direction
	 */
	static int OppositeDir(int nDirection);

// overridables
public:
	/**
	Is called by the view, when there is a Line to parse.

	@param dwCookie The result of parsing the previous line. This parameter contains flags,
	        you set while parsing the previous line. Normaly this will be flags that
	        indicate a region that was still open at the end of the last line. You
	        set those flags for a line by returning them ored (|).
	@param nLineIndex The zero-based index of the line to parse.
	@param pBlock You have to split the lines in to parts which are specified by there
	        beginning (zero-based index of the character in this line) and there text
	        color. You have to specifie each one of this blocks by adding them to
	        this block array, using CTextBlock::Add(). You do not have to care about
	        the sort to add the elements.

	        When the function ist called with this parameter set zu NULL, you only
	        have to calculate the cookies which can be (hoply) done much more faster.

	@return The calculated flags for this line (see dwCookie).
	 */
	virtual DWORD ParseLine(DWORD dwCookie, int nLineIndex, CCrystalTextBlock *pBlock);

	/**
	Called by CCrystalTextView-object to wrap the line with the given index into sublines.

	The standard implementation wraps the line behind the first whitespace in front
	of the word that exceeds the visible line width (nMaxLineWidth). Override
	this function to provide your own word wrapping.

	<b>Attention:</b> Never call this function directly in other parts of the code
	call WrapLineCached() instead, which will work much faster in most cases.

	@param nLineIndex The index of the line to wrap

	@param nMaxLineWidth The number of characters a subline of this line should
	not exceed

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
	Find the next word within the given line.

	The base implementation defines a word as beginning with an alpha character and ending
	on a space, punctuation, or eol.

	@param nLineIndex The index of the line to search

	@param nStartPos The index of the character to start the search. If the line contains
	no more words, nStartPos is set to -1.

	@param nEndPos The index of the character one past the end of the word.
	 */
	virtual void NextWord(int nLineIndex, int &nStartPos, int &nEndPos);

	/**Searches for pair string in given line. The search stops if <code>nNthOpenPais</code> decreses to zero
	or <code>nNthOpenPais</code> equals to <code>PAIR_NONE</code> and aStackPair stack becomes empty.
	Descendants are supposed to override this empty implementation!!

	@param lpszLine
	Line string
	@param lpszLineEnd
	        Pointer to end of line
	@param pTextBlock,
	        Parsed line
	@param nFoundStrStart
	        Start of string if found, or start of string that caused an error.
	@param nFoundStrEnd
	        End of string if found, or end of string that caused an error.
	@param nDirection
	        search direction: DIRECTION_LEFT or DIRECITON_RIGHT
	@param lpszTextPos
	        Current position in the line
	@param aPairStack
	        Stack of found and opened pairs
	@param nNthOpenPair
	        Number of open pairs that have to be skiped before stopping.
	        Only pairs with opposite direction than <code>nDireciton</code> does matter.
	@param bClearToEnd
	        When the function is about exit according to normal run, it will continue to check the rest of line
	        whether it conatins any open pair. If do so, an error is returned.
	@param openPairStack
	        Output, the stack where open pairs are stored.
	@param result
	        CParser::RESULT_ERROR if an error occures, the string that din't have pair is at the top of the stack
	        CParser::RESULT_OK otherwise


	@return
	FALSE if the search should continue on the next/previous line
	 */
	virtual BOOL FindPairInLine(LPCTSTR lpszLine, LPCTSTR lpszLineEnd, CCrystalTextBlock *pTextBlock, long nLineIndex,
	                            int nDirection, LPCTSTR lpszTextPos, CPairStack &aPairStack, int &nNthOpenPair, BOOL bClearToEnd,
	                            long &nFoundStrStart, long &nFoundStrEnd, CPairStack &openPairStack, BOOL &result);

	/**Returns TRUE if some pair ends at ptTextPos. Descendants are supposed to override this empty implementation!!

	@param lpszLine
	        Line string
	@param lpszTextPos
	        Pointer to end of line
	@param pBlock
	        Parsed lined
	@param nPairStrStart
	        Start of string if found
	@param nPairIdx
	        Found string index
	@param nPairDir
	        Found string direction: DIRECTION_LEFT or DIRECITON_RIGHT

	@return TRUE if some pair ends at ptTextPos.
	 */
	virtual BOOL IsEndOfPairAt(LPCTSTR lpszLine, LPCTSTR lpszTextPos, const CCrystalTextBlock *pTextBlock,
	                           long &nPairStrStart, int &nPairIdx, int &nPairDir);



// attributes
protected:

	virtual LPCTSTR GetLineChars(int nLineIndex);

	/**	The text view this parser is attached to. */
	CCrystalTextView *m_pTextView;

};

inline int CCrystalParser::OppositeDir(int nDirection)
{
	ASSERT(nDirection == DIRECTION_LEFT || nDirection == DIRECTION_RIGHT);

	if (nDirection == DIRECTION_LEFT)
	{
		return DIRECTION_RIGHT;
	}
	else
	{
		return DIRECTION_LEFT;
	}
}


#endif // !defined(AFX_CRYSTALPARSER_H__17435DA0_1F72_11D3_929E_FD90DABF8479__INCLUDED_)
