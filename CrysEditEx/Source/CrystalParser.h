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
* $RCSfile$
* $Revision$
* $Date$
*
* $Author$
*
* $Log$
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
	virtual DWORD ParseLine( DWORD dwCookie, int nLineIndex, CCrystalTextBlock *pBlock );

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
	virtual void WrapLine( int nLineIndex, int nMaxLineWidth, int *anBreaks, int &nBreaks );

	/**
	Find the next word within the given line.

    The base implementation defines a word as beginning with an alpha character and ending
	on a space, punctuation, or eol. 

	@param nLineIndex The index of the line to search

	@param nStartPos The index of the character to start the search. If the line contains
	no more words, nStartPos is set to -1.

	@param nEndPos The index of the character one past the end of the word.
	*/
	virtual void NextWord( int nLineIndex, int &nStartPos, int &nEndPos );

// attributes
protected:

	virtual LPCTSTR GetLineChars( int nLineIndex );

	/**	The text view this parser is attached to. */
	CCrystalTextView *m_pTextView;

};

#endif // !defined(AFX_CRYSTALPARSER_H__17435DA0_1F72_11D3_929E_FD90DABF8479__INCLUDED_)
