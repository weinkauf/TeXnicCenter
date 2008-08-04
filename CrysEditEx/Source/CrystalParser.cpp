////////////////////////////////////////////////////////////////////////////
//	File:		CrystalParser.cpp
//
//	Author:		Sven Wiegand
//	E-mail:		sven.wiegand@gmx.de
//
//	Implementation of the CCrystalParser class, a part of Crystal Edit -
//	syntax coloring text editor.
//
//	You are free to use or modify this code.
//
////////////////////////////////////////////////////////////////////////////

/********************************************************************
 *
 * $RCSfile: CrystalParser.cpp,v $
 * $Revision$
 * $Date$
 *
 * $Author$
 *
 * $Log: CrystalParser.cpp,v $
 * Revision 1.9  2005/03/08 16:36:08  vachis
 * Selection of block between bracket pairs in a hierarchical manner (Ctrl+M)
 * Bracket highlighting disabled if a selection is present
 * New default colours for bracket highlighting
 *
 * Revision 1.8  2005/02/15 09:43:12  vachis
 * Implemented feature 536164: Matching Bracket Highlight
 *
 * Revision 1.7  2002/06/27 14:43:26  svenwiegand
 * Instead of the character test functions (CharTraitsT::IsAlpha, CharTraitsT::IsDigit, etc.) from the standard library, our own wrappers from the character.h file (myspell directory) are used now, to fix character recognition problems (bug 551033).
 *
 * Revision 1.6  2002/04/27 07:12:33  cnorris
 * Release build crash
 *
 * Revision 1.5  2002/04/23 21:31:26  cnorris
 * Include line length in WrapLine
 *
 * Revision 1.4  2002/04/06 05:33:57  cnorris
 * Added NextWord method required for spell checker
 *
 * Revision 1.3  2002/04/01 23:07:56  cnorris
 * Implement secondary line break characters.
 *
 * Revision 1.2  2002/03/25 19:12:57  cnorris
 * Fix line wrap to break lines without white space which affected screen draw
 *
 * Revision 1.1.1.1  2002/02/26 08:12:00  svenwiegand
 * Initial revision
 *
 * Revision 1.0  2000-05-31 21:55:34+02  sven_wiegand
 * Initial revision
 *
 *
 ********************************************************************/

#include "stdafx.h"
#include "CrystalParser.h"
#include "CCrystalTextView.h"
#include "CharType.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CCrystalParser, CObject)

CCrystalParser::CCrystalParser()
{
	m_pTextView = NULL;
}

CCrystalParser::~CCrystalParser()
{
}

DWORD CCrystalParser::ParseLine(DWORD dwCookie, int nLineIndex, CCrystalTextBlock *pBlock)
{
	return 0;
}

void CCrystalParser::WrapLine(int nLineIndex, int nMaxLineWidth, int *anBreaks, int &nBreaks)
{
	// The parser must be attached to a view!
	ASSERT(m_pTextView);

	int nLineLength = m_pTextView->GetLineLength(nLineIndex);
	int nTabWidth = m_pTextView->GetTabSize();
	int nLineCharCount = 0;
	int nCharCount = 0;
	LPCTSTR szLine = m_pTextView->GetLineChars(nLineIndex);
	int nLastBreakPos = 0;
	int nLastCharBreakPos = 0;
	BOOL bWhitespace = FALSE;
	BOOL bFoundWhiteSpace = FALSE;
	BOOL bStructureBreak = FALSE;

	for (int i = 0; i < nLineLength; i++)
	{
		// remember position of whitespace for wrap
		if (bWhitespace)
		{
			nLastBreakPos = i;
			nLastCharBreakPos = nCharCount;
			bWhitespace = FALSE;
			bFoundWhiteSpace = TRUE;
		}
		else if (bStructureBreak && !bFoundWhiteSpace)
		{
			// We'll use the structure break if there isn't a whitespace break
			nLastBreakPos = i;
			nLastCharBreakPos = nCharCount;
			bStructureBreak = FALSE;
		}

		// increment TCHAR counter (evtl. expand tab)
		if (szLine[i] == _T('\t'))
		{
			int nSpaceCount = nTabWidth ? (nTabWidth - nCharCount % nTabWidth) : 0;
			nLineCharCount += nSpaceCount;
			nCharCount += nSpaceCount;
		}
		else
		{
			++nLineCharCount;
			++nCharCount;
		}

		// remember possible break points
		if (szLine[i] == _T('\t') || szLine[i] == _T(' '))
			bWhitespace = TRUE;
		else if (!bFoundWhiteSpace && (szLine[i] == _T(')') || szLine[i] == _T('}')))
			bStructureBreak = TRUE;

		// wrap line
		if (nLineCharCount >= nMaxLineWidth)
		{
			// if no wrap position found, but line is too wide, wrap at current position
			if (nLastBreakPos == 0)
			{
				nLastBreakPos = i;
				nLastCharBreakPos = nCharCount;
			}
			if (anBreaks)
				anBreaks[nBreaks++] = nLastBreakPos;
			else
				++nBreaks;

			nLineCharCount = nCharCount - nLastCharBreakPos;
			nLastBreakPos = 0;
			bWhitespace = FALSE;
			bFoundWhiteSpace = FALSE;
		}
	}
	// Sneak the total length of the line into the anBreaks array
	if (anBreaks && nLineLength)
		anBreaks[nBreaks] = nCharCount;
}

void CCrystalParser::NextWord(int nLineIndex, int &nStartPos, int &nEndPos)
{
	LPCTSTR szLine = GetLineChars(nLineIndex);

	if (szLine == NULL)
	{
		nStartPos = -1;
		return;
	}

	int nLength = _tcsclen(szLine);

	while (nStartPos < nLength)
	{
		// words begin with an alpha character
		if (CharTraitsT::IsAlpha(szLine[nStartPos]))
			break;

		++nStartPos;
	}

	nEndPos = nStartPos;

	while (nEndPos < nLength)
	{
		// words end on terminating character
		if (CharTraitsT::IsSpace(szLine[nEndPos]) ||
		        (CharTraitsT::IsPunct(szLine[nEndPos]) && szLine[nEndPos] != _T('\'')))
			break;
		else
			++nEndPos;
	}
	if (nStartPos == nEndPos)
		nStartPos = -1;
}

LPCTSTR CCrystalParser::GetLineChars(int nLineIndex)
{
	ASSERT(m_pTextView);
	return m_pTextView->GetLineChars(nLineIndex);
}

BOOL CCrystalParser::FindPairInLine(LPCTSTR lpszLine, LPCTSTR lpszLineEnd, CCrystalTextBlock *pTextBlock, long nLineIndex,
                                    int nDirection, LPCTSTR lpszTextPos, CPairStack &aPairStack, int &nNthOpenPair, BOOL bClearToEnd,
                                    long &nFoundStrStart, long &nFoundStrEnd, CPairStack &openPairStack, BOOL &result)
{
	nFoundStrStart = 0;
	nFoundStrEnd = 0;
	result = FALSE;
	return TRUE;
}

BOOL CCrystalParser::IsEndOfPairAt(LPCTSTR lpszLine, LPCTSTR lpszTextPos, const CCrystalTextBlock * pTextBlock,
                                   long &nPairStrStart, int &nPairIdx, int &nPairDir)
{
	nPairIdx = -1;
	nPairDir = -1;

	return FALSE;
}
