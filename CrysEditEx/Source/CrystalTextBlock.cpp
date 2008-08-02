////////////////////////////////////////////////////////////////////////////
//	File:		CrystalTextBlock.cpp
//
//	Author:		Sven Wiegand
//	E-mail:		sven.wiegand@gmx.de
//
//	Implementation of the CCrystalTextBufferEx class, a part of Crystal Edit -
//	syntax coloring text editor.
//
//	You are free to use or modify this code.
//
////////////////////////////////////////////////////////////////////////////

/********************************************************************
 *
 * $RCSfile: CrystalTextBlock.cpp,v $
 * $Revision$
 * $Date$
 *
 * $Author$
 *
 * $Log: CrystalTextBlock.cpp,v $
 * Revision 1.2  2005/02/15 09:40:49  vachis
 * Implemented feature 536164: Matching Bracket Highlight
 * Fixed bug 912103: Syntax highlighting error with "%\verb|"
 *
 * Revision 1.1.1.1  2002/02/26 08:12:01  svenwiegand
 * Initial revision
 *
 * Revision 1.0  2000-05-31 21:55:35+02  sven_wiegand
 * Initial revision
 *
 *
 ********************************************************************/

#include "stdafx.h"
#include "CrystalTextBlock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CCrystalTextBlock::CCrystalTextBlock(TEXTBLOCK *pTextBlock, int &rnActualItems)
: m_rnActualItems(rnActualItems)
{
    ASSERT(pTextBlock);
    m_pTextBlock = pTextBlock;
}

BOOL CCrystalTextBlock::Add(int nCharPos, int nColorIndex)
{
    // insert new text block sorted by character position
    for (int i = 0; i < m_rnActualItems && m_pTextBlock[i].m_nCharPos < nCharPos; i++);

    // insert text block
    if (i < m_rnActualItems) {
        // if new character position allready exists, the replace old one
        if (m_pTextBlock[i].m_nCharPos == nCharPos) {
            m_pTextBlock[i].m_nCharPos = nCharPos;
            m_pTextBlock[i].m_nColorIndex = nColorIndex;
        }
        else {
            for (int j = ++m_rnActualItems - 1; j > i; j--) {
                m_pTextBlock[j].m_nCharPos = m_pTextBlock[j - 1].m_nCharPos;
                m_pTextBlock[j].m_nColorIndex = m_pTextBlock[j - 1].m_nColorIndex;
            }
            m_pTextBlock[i].m_nCharPos = nCharPos;
            m_pTextBlock[i].m_nColorIndex = nColorIndex;
        }
    }
    else {
        // add text block to end of array
        m_pTextBlock[m_rnActualItems].m_nCharPos = nCharPos;
        m_pTextBlock[m_rnActualItems++].m_nColorIndex = nColorIndex;
    }

    return TRUE;
}

BOOL CCrystalTextBlock::ClearBlocksFrom(int nIndex)
{
    int result = nIndex - m_rnActualItems;
    if (result < 0)
        result = 0;

    m_rnActualItems = (nIndex > m_rnActualItems) ? m_rnActualItems : nIndex;
    return result;
}

void CCrystalTextBlock::GetBlockAt(int nIndex, int &nCharPos, int &nColorIndex) const
{
    ASSERT(nIndex < m_rnActualItems);

    nCharPos = m_pTextBlock[nIndex].m_nCharPos;
    nColorIndex = m_pTextBlock[nIndex].m_nColorIndex;
}

int CCrystalTextBlock::GetBlocksCount() const
{
    return m_rnActualItems;
}

