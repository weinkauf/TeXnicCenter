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

#if !defined(AFX_LATEXSYNTAXPARSER_H__31735C81_110C_11D3_929E_444553540000__INCLUDED_)
#define AFX_LATEXSYNTAXPARSER_H__31735C81_110C_11D3_929E_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../MySpell/Character.h"



class CLatexParser : public CCrystalParser  
{
	DECLARE_DYNAMIC( CLatexParser )

// construction/destruction
public:
	CLatexParser();


    
// operations
protected:
	/**
	Parses the specified string and fills the m_anItem-array.

	@param lpText
		Pointer to first character of string to parse 
		(not guaranteed to be terminated by null-character).
	@param lpTextEnd
		Pointer to the last character of lpText to parse.
	@param dwCookie
		Cookies.
	@param pTextBlock
		@see #ParseLine
	@param lpLine
		Pointer to the first character of the line this string belongs to.

	@return 
		New cookies.
	*/
	virtual DWORD ParseString( LPCTSTR lpText, LPCTSTR lpTextEnd, DWORD dwCookie, CCrystalTextBlock *pTextBlock, LPCTSTR lpLine );

	/**
	Checks, if there is a LaTeX-command beginning with at the specified position.

	The method simply counts the number of backslashes from the
	given position to the beginning of the string. If the number
	modular 2 is zero, then there is no command, otherwise there
	is one.

	@param lpText
		String that contains the possible command
		(not guaranteed to be terminated by null-character).
	@param lpPos
		Pointer to the character that starts the possible command (the backslash).

	@return
		TRUE if there is a command at nPos, FALSE otherwise.
	*/
	virtual BOOL IsCmdAt( LPCTSTR lpText, LPCTSTR lpPos );


// implementation helpers
protected:
	/**
	Compares the two strings.

	@param lpStart1
		Pointer to the first character of the first string.
	@param lpEnd1
		Pointer to the first character behind the first string
	@param lpszStr2
		The second string (null-terminated)

	@return
		TRUE if the strings match exactly, FALSE otherwise.
	*/
	static BOOL MatchString( LPCTSTR lpStart1, LPCTSTR lpEnd1, LPCTSTR lpszStr2 );

	/**
	Compares the two strings. whereby the second string can contain
	bell characters ('\a') at positions where the first string can 
	contain optional whitespaces.

	@param lpStart1
		Pointer to the first character of the first string.
	@param lpEnd1
		Pointer to the first character behind the first string
	@param lpszStr2
		The second string (null-terminated)

	@return
		-1 if the strings does not match, otherwise the strings match and
		the returned value specifies the offset of the first character in
		lpStart1, not belonging to the match.
	*/
	static int MatchStringEx( LPCTSTR lpStart1, LPCTSTR lpEnd1, LPCTSTR lpszStr2 );

	/** stores information about a found subexpression */
	typedef struct tagSubExpression
	{
		/** pointer to the first charcter matching the subexpression */
		LPCTSTR	first;

		/** pointer to the first character <b>not</b> matching the subexpression */
		LPCTSTR	second;
	} SUBEXPRESSION;

	/** 
	Searches a comment. 
	what[0]: whole expression
	*/
	static BOOL SearchComment( LPCTSTR lpStart, LPCTSTR lpEnd, SUBEXPRESSION what[] );

	/** 
	Searches a inline Math. 
	what[0]: whole expression
	*/
	static BOOL SearchInlineMath( LPCTSTR lpStart, LPCTSTR lpEnd, SUBEXPRESSION what[] );

	/** 
	Searches a inline verb. 
	what[0]: whole expression
	what[1]: opening delimiter
	what[2]: contents
	*/
	static BOOL SearchInlineVerb( LPCTSTR lpStart, LPCTSTR lpEnd, SUBEXPRESSION what[] );

	/** 
	Searches a Keyword. 
	what[0]: whole expression
	*/
	static BOOL SearchKeyword( LPCTSTR lpStart, LPCTSTR lpEnd, SUBEXPRESSION what[] );

	/** 
	Searches a Keyword that consits of a one character. 
	what[0]: whole expression
	*/
	static BOOL SearchOneCharKeyword( LPCTSTR lpStart, LPCTSTR lpEnd, SUBEXPRESSION what[] );

	/** 
	Searches a verb start. 
	what[0]: whole expression
	what[1]: "\begin"
	what[2]: "*" or ""

	@bug {verbatim } not recognized, i.e. whitespace before the closing brace.
		We should better use regexp here.
	*/
	static BOOL SearchVerbStart( LPCTSTR lpStart, LPCTSTR lpEnd, SUBEXPRESSION what[] );

	/** 
	Searches a verb end.
	what[0]: whole expression
	what[1]: "\end"
	what[2]: "*" or ""

	@bug {verbatim } not recognized, i.e. whitespace before the closing brace.
		We should better use regexp here.
	*/
	static BOOL SearchVerbEnd( LPCTSTR lpStart, LPCTSTR lpEnd, SUBEXPRESSION what[] );

	/** Clears all blocks after first comment and also returns correspoding modification of <code>dwCookie</code>.

	@param dwCookie 
		The result of parsing the line returned by <code>ParseString</code>.
	@param pBlock 
		Parsed line.	

	@return Modified dwCookie.
	*/
	static DWORD ClearBlocksAfterComment( DWORD dwCookie, CCrystalTextBlock *pTextBlock );

	/** Return the index of found pair string that starts at <code>lpszTextPost</code> position in the string
	<code>lpszStart/code>.

	@param lpszStart
		Beginning of the string
	@param lpszTextPos
		Position in the string
	@param lpszEnd
		End of string
	@param lpszPairStrEnd
		End of found string, NULL if not found.
	@param nPairIdx    
		Found string index
	@param nPairDir
		Found string direction: DIRECTION_LEFT or DIRECITON_RIGHT    

	@return TRUE if some pair string have been found.
  */
  virtual BOOL WhatPairStartsAt( LPCTSTR lpszStart, LPCTSTR lpszTextPost, LPCTSTR lpszEnd, 
																	LPCTSTR &lpszPairStrEnd, int &nPairIdx, int &nPairDir );
 
	/** Return the index of found pair string that ends at <code>lpszTextPost</code> position in the string
	<code>lpszStart</code>

	@param lpszStart
		Beginning of the string
	@param lpszTextPos
		Position in the string
	@param lpszEnd
		End of string
	@param lpszPairStrStart
		Start of found string, NULL if not found.
	@param nPairIdx    
		Found string index
	@param nPairDir
		Found string direction: DIRECTION_LEFT or DIRECITON_RIGHT   

	@return TRUE if some pair string have been found.
  */
	virtual BOOL WhatPairEndsAt( LPCTSTR lpszStart, LPCTSTR lpszTextPos, 
															 LPCTSTR &lpszPairStrStart, int &nPairIdx, int &nPairDir );

	/**Searches for pair string in given block. The search stops if <code>nNthOpenPais</code> decreses to zero 
	or <code>nNthOpenPais</code> equals to <code>PAIR_NONE</code> and aStackPair stack becomes empty.  

	@param lpszLine
		Line string, only for computing position in line
	@param lpszFrom
		Begining of the block (inclusive). It can be grater than lpszTo, it depends on <code>nDirection<c/ode>
	@param lpszTo
		End of the block (inclusive)
	@param nFoundStrStart 
		Start of string if found, or start of string that caused an error. 
	@param nFoundStrEnd 
		End of string if found, or end of string that caused an error.  
	@param nDirection
		Search direction: DIRECTION_LEFT or DIRECITON_RIGHT    
	@param aPairStack
		Stack of found and opened pairs
	@param nNthOpenPair    
		Number of open pairs that have to be skiped before stopping. 
		Only pairs with opposite direction than <code>nDireciton</code> does matter.
	@param bClearToEnd  
		When the function is about exit according to normal run, it will continue to check the rest of text document
		whether it conatins any open pair. If do so, an error is returned. 
	@param openPairStack
		Output, the stack where open pairs are stored.
	@param result
		CParser::RESULT_ERROR if an error occures, the string that din't have pair is at the top of the stack
		CParser::RESULT_OK otherwise
        
	@return     
		FALSE if the search should continue on the next/previous block
  */
	virtual BOOL FindPairInBlock( LPCTSTR lpszLine, LPCTSTR lpszFrom, LPCTSTR lpszTo, 
																long nLineIndex, int nDirection, CPairStack &aPairStack, int &nNthOpenPair, BOOL bClearToEnd, 
																long &nFoundStrStart, long &nFoundStrEnd, CPairStack &openPairStack, int &result );


// overridables
public:
	/**
	Is called by tht view, when there is a Line to parse.

	See base class declaration for parameter description.

	@return 
		The calculated flags for this line (see dwCookie).
	*/
	virtual DWORD ParseLine( DWORD dwCookie, int nLineIndex, CCrystalTextBlock *pTextBlock );

	/**
	Find the next word in a line.

	See base class declaration for parameter description.
	*/
	virtual void NextWord( int nLineIndex, int &nStartPos, int &nEndPos );

	/**Searches for pair string at given line. The search stops if <code>nNthOpenPais</code> decreses to zero 
	or <code>nNthOpenPais</code> equals to <code>PAIR_NONE</code> and aStackPair stack becomes empty.  

	See base class declaration for parameter description.
        
	@return     
		FALSE if the search should continue on the next/previous line
  */
	virtual BOOL FindPairInLine( LPCTSTR lpszLine, LPCTSTR lpszLineEnd, CCrystalTextBlock *pTextBlock, long nLineIndex, 
												int nDirection, LPCTSTR lpszTextPos, CPairStack &aPairStack, int &nNthOpenPair, BOOL bClearToEnd, 
												long &nFoundStrStart, long &nFoundStrEnd, CPairStack &openPairStack, int &result );

	/**Returns TRUE if some pair ends at ptTextPos.
    
	See base class declaration for parameter description.

	@return TRUE if some pair ends at ptTextPos.
  */
	virtual BOOL IsEndOfPairAt( LPCTSTR lpszLine, LPCTSTR lpszTextPos, CCrystalTextBlock const*const pTextBlock, 
															long &nPairStrStart, int &nPairIdx, int &nPairDir );

	
// attributes
protected:
	enum tagCookies {
		verb = 1,
		verbStar = verb << 1,
		inlineMath = verbStar << 1
	};

private:
	//Array of pair strings (e.g. various brackets).
	static const int s_nPairs;
	static LPCTSTR const s_pptszPairs[][2];     //initialization written in cpp file
};


  
// Inline-Methods
inline 
BOOL CLatexParser::IsCmdAt( LPCTSTR lpText, LPCTSTR lpPos )
{
	// count number of backslashes before command (including command backslash)
	for( int nCount = 0; lpPos >= lpText && *lpPos == _T('\\'); nCount++, lpPos-- );
	return !(nCount % 2 == 0);
}


inline 
BOOL CLatexParser::MatchString( LPCTSTR lpStart1, LPCTSTR lpEnd1, LPCTSTR lpszStr2 )
{
	for( ; *lpszStr2 && lpStart1 < lpEnd1; lpStart1++, lpszStr2++ )
		if( *lpStart1 != *lpszStr2 )
			return FALSE;

	return TRUE;
}


inline 
int CLatexParser::MatchStringEx( LPCTSTR lpStart1, LPCTSTR lpEnd1, LPCTSTR lpszStr2 )
{
	LPCTSTR	lpStr1 = lpStart1;
	LPCTSTR	lpStr2 = lpszStr2;

	while (TRUE)
	{
		if (*lpStr2 == _T('\0'))
			break;
		if (*lpStr1 == *lpStr2)
		{
			lpStr1++;
			lpStr2++;
			continue;
		}
		else if (*lpStr2 == _T('\a'))
		{
			if (IsSpace(*lpStr1))
			{
				lpStr1++;
				continue;
			}
			else
			{
				lpStr2++;
				continue;
			}
		}
		else
			return -1;
	}

	return (lpStr1-lpStart1);
}



#endif // !defined(AFX_LATEXSYNTAXPARSER_H__31735C81_110C_11D3_929E_444553540000__INCLUDED_)
