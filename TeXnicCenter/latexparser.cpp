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
#include "LatexParser.h"
#include "Configuration.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC( CLatexParser, CCrystalParser )

CLatexParser::CLatexParser()
{
}


BOOL CLatexParser::SearchComment( LPCTSTR lpStart, LPCTSTR lpEnd, SUBEXPRESSION what[] )
{
	for( ; lpStart < lpEnd; lpStart++ )
	{
		if( *lpStart == _T('%') )
		{
			what[0].first = lpStart;
			what[0].second = lpStart + 1;
			return TRUE;
		}
	}

	return FALSE;
}


BOOL CLatexParser::SearchInlineMath( LPCTSTR lpStart, LPCTSTR lpEnd, SUBEXPRESSION what[] )
{
	for( ; lpStart < lpEnd; lpStart++ )
	{
		if( *lpStart == _T('$') )
		{
			what[0].first = lpStart;
			what[0].second = lpStart + 1;
			return TRUE;
		}
	}

	return FALSE;
}


BOOL CLatexParser::SearchInlineVerb( LPCTSTR lpStart, LPCTSTR lpEnd, SUBEXPRESSION what[] )
{
	for( ; lpStart < lpEnd; lpStart++ )
	{
		if( *lpStart == _T('\\') && MatchString( lpStart, lpEnd, _T("\\verb") ) )
		{
			what[0].first = lpStart;
			lpStart+= 5;	// point to first character behind match
			if( lpStart >= lpEnd )
				return FALSE;

			if( *lpStart == _T('*') )
				if( ++lpStart >= lpEnd )
					return FALSE;

			TCHAR	c = *lpStart;
			what[1].first = lpStart++;
			what[2].second = what[2].first = what[0].second = what[1].second = lpStart;
			if( lpStart >= lpEnd )
				return TRUE;

			what[2].first = lpStart;
			for( ; lpStart < lpEnd && *lpStart != c; lpStart++ );
			what[2].second = lpStart;
			what[0].second = (lpStart < lpEnd)? lpStart + 1 : lpEnd;

			return TRUE;
		}
	}

	return FALSE;
}


BOOL CLatexParser::SearchOneCharKeyword( LPCTSTR lpStart, LPCTSTR lpEnd, SUBEXPRESSION what[] )
{
	for( ; lpStart < lpEnd; lpStart++ )
	{
		switch( *lpStart )
		{
			case _T('_'):
			case _T('^'):
			case _T('"'):
			case _T('#'):
			case _T('&'):
			case _T('~'):
			case _T('|'):
				what[0].first = lpStart;
				what[0].second = lpStart + 1;
				return TRUE;
		}
	}

	return FALSE;
}


BOOL CLatexParser::SearchKeyword( LPCTSTR lpStart, LPCTSTR lpEnd, SUBEXPRESSION what[] )
{
	for( ; lpStart < lpEnd; lpStart++ )
	{
		if( *lpStart ==	_T('\\') )
		{
			what[0].first = lpStart++;
			if( lpStart >= lpEnd )
				return FALSE;

			switch( *lpStart )
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
				case _T('\\'):
					what[0].second = lpStart + 1;
					return TRUE;

				default:
					for( ; lpStart < lpEnd && (IsAlpha( *lpStart ) || *lpStart == _T('@')); lpStart++ );
					what[0].second = (lpStart < lpEnd)? lpStart : lpEnd;
					return TRUE;
			}
		}
	}

	return FALSE;
}


BOOL CLatexParser::SearchVerbStart( LPCTSTR lpStart, LPCTSTR lpEnd, SUBEXPRESSION what[] )
{
	for( ; lpStart < lpEnd; lpStart++ )
	{
		int	nMatchLen = -1;
		if (*lpStart == _T('\\') && (nMatchLen = MatchStringEx( lpStart, lpEnd, _T("\\begin\a{\averbatim"))) >= 0)
		{
			what[1].first = what[0].first = lpStart;
			what[1].second = lpStart + 6;
			lpStart+= nMatchLen;	// point to first character behind match
			if( lpStart >= lpEnd )
				return FALSE;

			what[2].first = lpStart;
			if( *lpStart == _T('*') )
				if( ++lpStart >= lpEnd )
					return FALSE;

			what[2].second = lpStart;

			if( *lpStart == _T('}') )
			{
				what[0].second = lpStart + 1;
				return TRUE;
			}
		}
	}

	return FALSE;
}


BOOL CLatexParser::SearchVerbEnd( LPCTSTR lpStart, LPCTSTR lpEnd, SUBEXPRESSION what[] )
{
	for( ; lpStart < lpEnd; lpStart++ )
	{
		int	nMatchLen = -1;
		if( *lpStart == _T('\\') && (nMatchLen = MatchStringEx( lpStart, lpEnd, _T("\\end\a{\averbatim"))) >= 0)
		{
			what[1].first = what[0].first = lpStart;
			what[1].second = lpStart + 4;
			lpStart+= nMatchLen;	// point to first character behind match
			if( lpStart >= lpEnd )
				return FALSE;

			what[2].first = lpStart;
			if( *lpStart == _T('*') )
				if( ++lpStart >= lpEnd )
					return FALSE;

			what[2].second = lpStart;

			if( *lpStart == _T('}') )
			{
				what[0].second = lpStart + 1;
				return TRUE;
			}
		}
	}

	return FALSE;
}


#define COLORINDEX_NORMAL	((dwCookie & inlineMath)? CCrystalTextView::COLORINDEX_NUMBER : CCrystalTextView::COLORINDEX_NORMALTEXT )
#define COLORINDEX_KEYWORD ((dwCookie & inlineMath)? CCrystalTextView::COLORINDEX_PREPROCESSOR : CCrystalTextView::COLORINDEX_KEYWORD )

DWORD CLatexParser::ParseString( LPCTSTR lpText, LPCTSTR lpTextEnd, DWORD dwCookie, CCrystalTextBlock *pTextBlock, LPCTSTR lpLine )
{
	SUBEXPRESSION				what[3];


	///////////////////////////////////////////////////////////////////
	// look for end of verb
	if( dwCookie & verb || dwCookie & verbStar )
	{
		if( SearchVerbEnd( lpText, lpTextEnd, what ) )
		{
			if( ((what[2].second - what[2].first) && (dwCookie & verbStar)) ||
					(!(what[2].second - what[2].first) && (dwCookie & verb )) )
			{
				dwCookie&= ~(verb | verbStar);	// delete verbatim flags

				if( pTextBlock )
				{
					pTextBlock->Add( lpText - lpLine, CCrystalTextView::COLORINDEX_STRING );
					pTextBlock->Add( what[1].first - lpLine, COLORINDEX_KEYWORD );
					pTextBlock->Add( what[1].second - lpLine, COLORINDEX_NORMAL );
				}

				// Parse behind end command
				dwCookie = ParseString( what[0].second, lpTextEnd, dwCookie, pTextBlock, lpLine );
			}
			else if( pTextBlock )
				pTextBlock->Add( lpText - lpLine, CCrystalTextView::COLORINDEX_STRING );
		}
		else if( pTextBlock )
			pTextBlock->Add( lpText - lpLine, CCrystalTextView::COLORINDEX_STRING );

		return dwCookie;
	}

	///////////////////////////////////////////////////////////////////
	// look for inline verb
	if( SearchInlineVerb( lpText, lpTextEnd, what ) && IsCmdAt( lpText, what[0].first ) )
	{
		// parse string before occurence
		dwCookie = ParseString( lpText, what[0].first, dwCookie, pTextBlock, lpLine );

		// was there a "\begin{verbatim}" before this \verb?
		if( dwCookie & verb || dwCookie & verbStar )
			return dwCookie;

		// highlight occurence
		if( pTextBlock )
		{
			pTextBlock->Add( what[0].first - lpLine, COLORINDEX_KEYWORD );
			pTextBlock->Add( what[1].first - lpLine, CCrystalTextView::COLORINDEX_OPERATOR );
			pTextBlock->Add( what[2].first - lpLine, CCrystalTextView::COLORINDEX_STRING );
			pTextBlock->Add( what[2].second - lpLine, CCrystalTextView::COLORINDEX_OPERATOR );
			if( what[0].second < lpTextEnd )
				pTextBlock->Add( what[0].second - lpLine, COLORINDEX_NORMAL );
		}

		// parse string behind occurence
		dwCookie = ParseString( what[0].second, lpTextEnd, dwCookie, pTextBlock, lpLine );

		return dwCookie;
	}

	///////////////////////////////////////////////////////////////////
	// look for comments
	if( SearchComment( lpText, lpTextEnd, what ) && (what[0].first == lpLine || !IsCmdAt( lpText, what[0].first - 1 )) )
	{
		// parse string before occurence
		dwCookie = ParseString( lpText, what[0].first, dwCookie, pTextBlock, lpLine );

		// highlicht occurence
		if( pTextBlock )
			pTextBlock->Add( what[0].first - lpLine, CCrystalTextView::COLORINDEX_COMMENT );

		return dwCookie;
	}

	///////////////////////////////////////////////////////////////////
	// look for beginning of verb
	if( SearchVerbStart( lpText, lpTextEnd, what ) && IsCmdAt( lpText, what[0].first ) )
	{
		// parse string before occurence
		dwCookie = ParseString( lpText, what[0].first, dwCookie, pTextBlock, lpLine );

		// highlight occurence
		if( pTextBlock )
		{
			pTextBlock->Add( what[1].first - lpLine, COLORINDEX_KEYWORD );
			pTextBlock->Add( what[1].second - lpLine, COLORINDEX_NORMAL );
			pTextBlock->Add( what[0].second - lpLine, CCrystalTextView::COLORINDEX_STRING );
		}

		if( what[2].second - what[2].first )
			dwCookie|= verbStar;
		else
			dwCookie|= verb;

		// parse string behind occurence
		dwCookie = ParseString( what[0].second, lpTextEnd, dwCookie, pTextBlock, lpLine );

		return dwCookie;
	}

	///////////////////////////////////////////////////////////////////
	// look for inline math
	if( SearchInlineMath( lpText, lpTextEnd, what ) && (what[0].first == lpLine || !IsCmdAt( lpText, what[0].first - 1)) )
	{
		// parse string before occurence
		dwCookie = ParseString( lpText, what[0].first, dwCookie, pTextBlock, lpLine );

		// highlight
		if( pTextBlock && !(dwCookie & inlineMath) )
			pTextBlock->Add( what[0].first - lpLine, COLORINDEX_KEYWORD );

		// set cookies
		if( dwCookie & inlineMath )
			dwCookie&= ~inlineMath;
		else
			dwCookie|= inlineMath;

		if( pTextBlock && !(dwCookie & inlineMath) )
			pTextBlock->Add( what[0].first - lpLine, COLORINDEX_KEYWORD );
		if( pTextBlock )
			pTextBlock->Add( what[0].second - lpLine, COLORINDEX_NORMAL );

		// parse string behind occurence
		dwCookie = ParseString( what[0].second, lpTextEnd, dwCookie, pTextBlock, lpLine );

		return dwCookie;
	}

	///////////////////////////////////////////////////////////////////
	// look for keywords
	if( pTextBlock && 
			((SearchKeyword( lpText, lpTextEnd, what ) && IsCmdAt( lpText, what[0].first )) ||
			 (SearchOneCharKeyword( lpText, lpTextEnd, what) && !IsCmdAt( lpText, what[0].first ))) )
	{
		// parse string before occurence
		dwCookie = ParseString( lpText, what[0].first, dwCookie, pTextBlock, lpLine );

		// highlicht occurence
		if( dwCookie & inlineMath )
			pTextBlock->Add( what[0].first - lpLine, CCrystalTextView::COLORINDEX_PREPROCESSOR );
		else
			pTextBlock->Add( what[0].first - lpLine, COLORINDEX_KEYWORD );

		pTextBlock->Add( what[0].second - lpLine, COLORINDEX_NORMAL );

		// parse string behind occurence
		dwCookie = ParseString( what[0].second, lpTextEnd, dwCookie, pTextBlock, lpLine );

		return dwCookie;
	}

	///////////////////////////////////////////////////////////////////
	// mark text as normal
	if( pTextBlock )
		pTextBlock->Add( lpText - lpLine, COLORINDEX_NORMAL );

	return dwCookie;
}


DWORD CLatexParser::ParseLine( DWORD dwCookie, int nLineIndex, CCrystalTextBlock *pTextBlock )
{
	IFNOT( m_pTextView )
		return dwCookie;

	// get length of line
	CCrystalTextBuffer	*pTextBuffer = m_pTextView->LocateTextBuffer();
	IFNOT( pTextBuffer )
		return dwCookie;

	int				nLength = pTextBuffer->GetLineLength( nLineIndex );
	LPCTSTR		lpLine = pTextBuffer->GetLineChars( nLineIndex );

	if( !nLength || !lpLine )
		return dwCookie;

	// parse
	return ParseString( lpLine, lpLine + nLength, dwCookie, pTextBlock, lpLine );
}


void CLatexParser::NextWord( int nLineIndex, int &nStartPos, int &nEndPos )
{
	LPCTSTR szLine = GetLineChars( nLineIndex );
	if ( szLine == NULL )
	{
		nStartPos = -1;
		return;
	}
	int nLength = _tcsclen(szLine);
	boolean badWord = true;
	TBYTE ch;

	while ( nStartPos != -1 && badWord )
	{
		badWord = false;
		while ( nStartPos < nLength )
		{
			ch = szLine[nStartPos];
			// words begin with an alpha character
			if ( IsAlpha(ch) )
				break;
			// if we're skipping words with numbers, words may begin with a number
			if ( g_configuration.m_bSpellSkipNumbers && IsDigit(ch) )
			{
				badWord = true;
				break;
			}
			// Skip comments?
			if ( g_configuration.m_bSpellSkipComments && ch == _T('%') )
			{
				nStartPos = -1;
				return;
			}
			++nStartPos;
		}
		if ( !badWord && nStartPos > 0 && g_configuration.m_bSpellSkipTags )
			badWord = IsCmdAt(szLine, &szLine[nStartPos-1]);

		nEndPos = nStartPos + 1;

		while ( nEndPos < nLength )
		{
			ch = szLine[nEndPos];
			if ( !badWord && g_configuration.m_bSpellSkipNumbers && IsDigit(ch) )
				badWord = true;

			// words end on terminating character 
			//if ( IsSpace(ch) || (IsPunct(ch) &&  ch != _T('\'')) )
			if (!IsAlNum(ch))
				break;
			++nEndPos;
		}
		if ( !(nStartPos < nLength) )
		{
			// We have reached the end of the line
			nStartPos = -1;
		}
		else
		{
			if ( !badWord && g_configuration.m_bSpellSkipCaps )
			{
				badWord = true;
				for (int i = nStartPos; i < nEndPos; ++i)
				{
					if ( IsLower(szLine[i]) )
					{
						badWord = false;
						break;
					}
				}
			}

			if ( badWord )
				nStartPos = nEndPos;
		}
	}
}
