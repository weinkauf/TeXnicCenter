////////////////////////////////////////////////////////////////////////////
//	File:		CrystalTextBlock.h
//
//	Author:		Sven Wiegand
//	E-mail:		sven.wiegand@gmx.de
//
//	Interface of the CCrystalTextBlock class, a part of Crystal Edit -
//	syntax coloring text editor.
//
//	You are free to use or modify this code.
//
////////////////////////////////////////////////////////////////////////////

/********************************************************************
 *
 * $RCSfile: CrystalTextBlock.h,v $
 * $Revision$
 * $Date$
 *
 * $Author$
 *
 * $Log: CrystalTextBlock.h,v $
 * Revision 1.2  2005/02/15 09:40:49  vachis
 * Implemented feature 536164: Matching Bracket Highlight
 * Fixed bug 912103: Syntax highlighting error with "%\verb|"
 *
 * Revision 1.1.1.1  2002/02/26 08:12:01  svenwiegand
 * Initial revision
 *
 * Revision 1.0  2000-05-31 21:55:36+02  sven_wiegand
 * Initial revision
 *
 *
 ********************************************************************/

#if !defined(AFX_CRYSTALTEXTBLOCK_H__37ECF0C1_145F_11D3_929E_444553540000__INCLUDED_)
#define AFX_CRYSTALTEXTBLOCK_H__37ECF0C1_145F_11D3_929E_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cedefs.h"

/**
Objects of this class are passed to CCrystalParser::ParseLine() to get the
positions of color changes in a text line.

You can simply add a color change using the Add()-Method. You do add the
color changes sorted by position -- the Add()-Method will do this for you.

@author Sven Wiegand
 */
class CRYSEDIT_CLASS_DECL CCrystalTextBlock
{
	friend class CCrystalTextView;

protected:

	struct TEXTBLOCK
	{
		int m_nCharPos;
		int m_nColorIndex;
	};


// construction/destruction
protected:
	CCrystalTextBlock(TEXTBLOCK *pTextBlock, int &rnActualItems);


// operations
public:
	/**
	Adds a text block to the array.

	You do not need to add the text blocks in the order of character position.

	@param nCharPos
	        Character position of the line, where the text block starts.
	@param nColorIndex
	        Index of the color to use to colorize the text block.
	 */
	BOOL Add(int nCharPos, int nColorIndex);

	/** Removes all blocks after given index, including block on this index.

	@param nIndex
	        Block index.

	@return number of removed blocks
	 */
	int ClearBlocksFrom(int nIndex);

	/**Returns values of text block on given index.

	@param nIndex
	        Text block index.
	@param nCharPos
	        Character position of the line, where the text block starts.
	@param nColorIndex
	        Index of the color to use to colorize the text block.
	 */
	void GetBlockAt(int nIndex, int &nCharPos, int &nColorIndex) const;


	/**Returns number of textblock items
	@return number of textblock items
	 */
	int GetBlocksCount() const;

// attributes
protected:
	/** text block to work on */
	TEXTBLOCK *m_pTextBlock;

	/** Index */
	int &m_rnActualItems;
};


#endif // !defined(AFX_CRYSTALTEXTBLOCK_H__37ECF0C1_145F_11D3_929E_444553540000__INCLUDED_)
