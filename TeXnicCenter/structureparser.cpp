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

#include "stdafx.h"
#include "stdafx.h"
#include "StructureParser.h"
#include "TextSourceFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//-------------------------------------------------------------------
// class CStructureItem
//-------------------------------------------------------------------
CStructureItem::CStructureItem()
{
	// initialize members
	m_nLine = 0;
	m_nParent = -1;
	m_nType = 0;
}


//-------------------------------------------------------------------
// class CStructureParser
//-------------------------------------------------------------------
CString CStructureParser::m_astrHeader[MAX_DEPTH] = 
{ _T("part"), _T("chapter"), _T("section"), _T("subsection"), _T("subsubsection") };

CStructureParser::CStructureParser()
:	m_bCancel( FALSE )
{
	// Only use the public constructor
	ASSERT( FALSE );
}

CStructureParser::CStructureParser( CStructureParserHandler *pStructureParserHandler/*, CTextSourceManager *pTextSourceManager */)
:	m_bCancel( FALSE ),
	m_pStructureParserHandler( pStructureParserHandler ),
//	m_pTextSourceManager( pTextSourceManager ),
	m_pStructureParserThread( NULL )
{
	// initialize attributes
//	ASSERT( pTextSourceManager ); 
	ASSERT( pStructureParserHandler );

	m_structureItemsAccess.Unlock();

	// initialize regular expressions
	int	nResult;

	nResult = m_regexHeader.set_expression( _T(
		"\\\\(part|chapter|section|subsection|subsubsection)\\s*\\*?\\s*([\\[\\{].*\\})"
	) );
	TRACE( "m_regexHeader returned %d\n", nResult );

	nResult = m_regexComment.set_expression( _T(
		"%"
	) );
	TRACE( "m_regexComment returned %d\n", nResult );

	nResult = m_regexInlineVerb.set_expression( _T(
		"\\\\verb\\*(.)[^\\1]*\\1|\\\\verb([^\\*])[^\\2]*\\2"
	) );
	TRACE( "m_regexInlineVerb returned %d\n", nResult );

	nResult = m_regexVerbStart.set_expression( _T(
		"\\\\begin\\s*\\{verbatim\\*?\\}"
	) );
	TRACE( "m_regexVerbStart returned %d\n", nResult );

	nResult = m_regexVerbEnd.set_expression( _T(
		"\\\\end\\{verbatim\\*?\\}"
	) );
	TRACE( "m_regexVerbEnd returned %d\n", nResult );

	nResult = m_regexFigureStart.set_expression( _T(
		"\\\\begin\\s*\\{figure\\}"
	) );
	TRACE( "m_regexFiguresStart returned %d\n", nResult );

	nResult = m_regexFigureEnd.set_expression( _T(
		"\\\\end\\s*\\{figure\\}"
	) );
	TRACE( "m_regexFigureEnd returned %d\n", nResult );

	nResult = m_regexTableStart.set_expression( _T(
		"\\\\begin\\s*\\{(sideways)?table\\}"
	) );
	TRACE( "m_regexTableStart returned %d\n", nResult );

	nResult = m_regexTableEnd.set_expression( _T(
		"\\\\end\\s*\\{(sideways)?table\\}"
	) );
	TRACE( "m_regexTableEnd returned %d\n", nResult );

	nResult = m_regexCaption.set_expression( _T(
		"\\\\caption\\s*([\\[\\{].*\\})"
	) );
	TRACE( "m_regexCaption returned %d\n", nResult );

	nResult = m_regexLabel.set_expression( _T(
		"\\\\label\\s*\\{([^\\}]*)\\}"
	) );
	TRACE( "m_regexLabel returned %d\n", nResult );

	nResult = m_regexInput.set_expression( _T(
		"\\\\(input|include)\\s*\\{\\s*\"?([^\\}]*)\"?\\s*\\}"
	) );
	TRACE( "m_regexInput returned %d\n", nResult );

 	nResult = m_regexBib.set_expression( _T(
 		"\\\\(bibliography)\\s*\\{\\s*\"?([^\\}]*)\"?\\s*\\}"
 	) );
 	TRACE( "m_regexBib returned %d\n", nResult );
}

CStructureParser::~CStructureParser()
{

}

const CStructureItemArray *CStructureParser::LockStructureItems()
{
	//m_structureItemsAccess.Lock();

	return &m_aStructureItems;
}

void CStructureParser::UnlockStructureItems()
{
	//m_structureItemsAccess.Unlock();
}

BOOL CStructureParser::StartParsing( LPCTSTR lpszMainPath, LPCTSTR lpszWorkingDir, int nPriority )
{
	// initialization
	for( int i = 0; i < MAX_DEPTH; i++ )
		m_anItem[i] = -1;
	m_strMainPath = lpszMainPath;
	m_strWorkingDir = lpszWorkingDir;
	m_aStructureItems.RemoveAll();
	m_nDepth = 0;

	// start parsing thread
	if( m_pStructureParserThread = AfxBeginThread( StructureParserThread, this, nPriority ) )
		return TRUE;
	else
		return FALSE;
}

UINT StructureParserThread( LPVOID pStructureParser )
{
//	ASSERT( ((CStructureParser*)pStructureParser)->m_pTextSourceManager );
	CStructureParser* pParser = (CStructureParser*)pStructureParser;
//	ASSERT(pParser->m_pTextSourceManager);

	CStructureParser::CCookieStack cookies;
	BOOL bParsingResult;

//	bParsingResult =  ((CStructureParser*)pStructureParser)->Parse( ((CStructureParser*)pStructureParser)->m_strMainPath, cookies );
	bParsingResult = pParser->Parse(pParser->m_strMainPath, cookies);
	pParser->m_pStructureParserHandler->OnParsingFinished( bParsingResult );

	return 0;
}


BOOL CStructureParser::IsCmdAt( LPCTSTR lpText, int nPos )
{
	// count number of backslashes before command (including command backslash)
	for( int nCount = 0; nPos >= 0 && *(lpText + nPos) == _T('\\'); nCount++, nPos-- );
	return !(nCount % 2 == 0);
}


CString CStructureParser::GetArgument(const CString &strText, TCHAR tcOpeningDelimiter, TCHAR tcClosingDelimiter)
{
	int		nLen = strText.GetLength();
	int		nDepth = 0;
	int		nStart = -1;
	int		nEnd = -1;

	// find argument
	for (int i = 0; i < nLen; i++)
	{
		if (strText[i] == tcOpeningDelimiter)
		{
			if (nStart < 0)
				nStart = i+1;
			nDepth++;
		}
		else if (strText[i] == tcClosingDelimiter)
		{
			if (nDepth == 1)
			{
				// found the end
				nEnd = i;
				break;
			}
			else if (nDepth > 1)
				nDepth--;
		}
	}

	if (nEnd-nStart <= 0)
		return _T("");

	CString	strResult = strText.Mid(nStart, nEnd-nStart);
	strResult.TrimLeft();
	strResult.TrimRight();
	return strResult;
}



#define INITIALIZE_SI( si )\
	si.m_nLine = nActualLine;\
	si.m_nParent = m_anItem[m_nDepth];\
	si.m_strComment = "";\
	si.m_strCaption = "";\
	si.m_strLabel = "";\
	si.m_strPath = strActualFile;\
	si.m_strTitle = ""

void CStructureParser::ParseString( LPCTSTR lpText, int nLength, CCookieStack &cookies, const CString &strActualFile, int nActualLine )
{
	LPCTSTR							lpTextEnd = lpText;
	CStructureItem			si;
	reg_match<LPCTSTR>	what;
	int									nFlags = match_default;
	int									nTypeStart, nTypeCount, nTitleStart, nTitleCount;
	CString							strHeaderType;
	COOKIE							cookie;

	// wait until we can make changes to m_aStructureItems
	//m_structureItemsAccess.Lock();

	lpTextEnd = lpText + nLength;

	// look for input command
	if( reg_search( lpText, lpTextEnd, what, m_regexInput, nFlags ) && IsCmdAt( lpText, what[0].first - lpText ) )
	{
		// parse string before occurence
		ParseString( lpText, what[0].first - lpText, cookies, strActualFile, nActualLine );

		// parse input file
		CString	strPath( what[2].first, what[2].second - what[2].first );
		strPath.TrimLeft();
		strPath.TrimRight();
		strPath.TrimLeft(_T('"'));
		strPath.TrimRight(_T('"'));
		if( strPath.Find( _T('.') ) < 0 )
			strPath+= _T(".tex");
		Parse( strPath, cookies );

		// parse string behind occurence
		ParseString( what[0].second, lpTextEnd - what[0].second, cookies, strActualFile, nActualLine );

		return;
	}
	// look for figure start
	if( reg_search( lpText, lpTextEnd, what, m_regexFigureStart, nFlags ) && IsCmdAt( lpText, what[0].first - lpText ) )
	{
		// parse string before occurence
		ParseString( lpText, what[0].first - lpText, cookies, strActualFile, nActualLine );

		// add figure to collection
		INITIALIZE_SI( si );
		cookie.nCookieType = si.m_nType = figure;
		cookie.nItemIndex = m_aStructureItems.Add( si );
		cookies.Push( cookie );

		// parse string behind occurence
		ParseString( what[0].second, lpTextEnd - what[0].second, cookies, strActualFile, nActualLine );

		return;
	}
	// look for table start
	if( reg_search( lpText, lpTextEnd, what, m_regexTableStart, nFlags ) && IsCmdAt( lpText, what[0].first - lpText ) )
	{
		// parse string before occurence
		ParseString( lpText, what[0].first - lpText, cookies, strActualFile, nActualLine );

		// add table to collection
		INITIALIZE_SI( si );
		cookie.nCookieType = si.m_nType = table;
		cookie.nItemIndex = m_aStructureItems.Add( si );
		cookies.Push( cookie );

		// parse string behind occurence
		ParseString( what[0].second, lpTextEnd - what[0].second, cookies, strActualFile, nActualLine );

		return;
	}
	// look for headers
	if( reg_search( lpText, lpTextEnd, what, m_regexHeader, nFlags ) && IsCmdAt( lpText, what[0].first - lpText ) )
	{
		// parse string before occurence
		ParseString( lpText, what[0].first - lpText, cookies, strActualFile, nActualLine );

		// if the top of the stack is a header, then remove it
		if( !cookies.IsEmpty() && cookies.Top().nCookieType == header )
			cookies.Pop();

		// initialize structure
		INITIALIZE_SI( si );

		// get header depth (to get parent item)
		nTypeStart = what[1].first - lpText;
		nTypeCount = what[1].second - what[1].first;
		strHeaderType = lpText;
		strHeaderType = strHeaderType.Mid( nTypeStart, nTypeCount );

		for( m_nDepth = 0; m_nDepth < MAX_DEPTH && m_astrHeader[m_nDepth] != strHeaderType; m_nDepth++ );

		// get parent
		if( m_nDepth < 1 || m_nDepth >= MAX_DEPTH )
			si.m_nParent = -1;
		else
			si.m_nParent = m_anItem[m_nDepth - 1];

		// get title
		nTitleStart = what[2].first - lpText;
		nTitleCount = what[2].second - what[2].first;

		CString	strFullMatch(what[2].first, nTitleCount);
		if (strFullMatch[0] == _T('['))
			si.m_strTitle = GetArgument(strFullMatch, _T('['), _T(']'));
		else
			si.m_strTitle = GetArgument(strFullMatch, _T('{'), _T('}'));
		si.m_nType = header;
		cookie.nItemIndex = m_anItem[m_nDepth] = m_aStructureItems.Add( si );
		cookie.nCookieType = header;
		cookies.Push( cookie );

		// parse string behind occurence
		int	nEnd = nTitleStart+nTitleCount;
		nEnd = nEnd - (strFullMatch.GetLength() - si.m_strTitle.GetLength()) + 2;
		ParseString( lpText+nEnd, lpTextEnd - (lpText+nEnd), cookies, strActualFile, nActualLine );

		return;
	}
	
	// find label
	if( reg_search( lpText, lpTextEnd, what, m_regexLabel, nFlags ) && IsCmdAt( lpText, what[0].first - lpText ) )
	{
		// parse string before occurence
		ParseString( lpText, what[0].first - lpText, cookies, strActualFile, nActualLine );

		if( !cookies.IsEmpty() )
			if( cookies.Top().nCookieType == header )
				m_aStructureItems[cookies.Pop().nItemIndex].m_strLabel = CString( what[1].first, what[1].second - what[1].first );
			else
				m_aStructureItems[cookies.Top().nItemIndex].m_strLabel = CString( what[1].first, what[1].second - what[1].first );

		// parse string behind occurence
		ParseString( what[0].second, lpTextEnd - what[0].second, cookies, strActualFile, nActualLine );

		return;
	}

	// find caption
	if( reg_search( lpText, lpTextEnd, what, m_regexCaption, nFlags ) && IsCmdAt( lpText, what[0].first - lpText ) )
	{
		// parse string before occurence
		ParseString( lpText, what[0].first - lpText, cookies, strActualFile, nActualLine );

		if( !cookies.IsEmpty() )
		{
			CString	strFullMatch(what[1].first, what[1].second - what[1].first);
			if (strFullMatch[0] == _T('['))
				m_aStructureItems[cookies.Top().nItemIndex].m_strCaption = GetArgument(strFullMatch, _T('['), _T(']'));
			else
				m_aStructureItems[cookies.Top().nItemIndex].m_strCaption = GetArgument(strFullMatch, _T('{'), _T('}'));
		}

		// parse string behind occurence
		ParseString( what[0].second, lpTextEnd - what[0].second, cookies, strActualFile, nActualLine );

		return;
	}

	// find end of figure
	if( reg_search( lpText, lpTextEnd, what, m_regexFigureEnd, nFlags ) && IsCmdAt( lpText, what[0].first - lpText ) )
	{
		// parse string before occurence
		ParseString( lpText, what[0].first - lpText, cookies, strActualFile, nActualLine );

		// pop figure from stack
		if( !cookies.IsEmpty() && cookies.Top().nCookieType == figure )
		{
			CStructureItem	&si = m_aStructureItems[cookies.Pop().nItemIndex];
			if( si.m_strTitle.IsEmpty() )
			{
				if( si.m_strCaption.IsEmpty() )
					si.m_strTitle = si.m_strLabel;
				else
					si.m_strTitle = si.m_strCaption;
			}
		}

		// parse string behind occurence
		ParseString( what[0].second, lpTextEnd - what[0].second, cookies, strActualFile, nActualLine );

		return;
	}
	// find end of table
	if( reg_search( lpText, lpTextEnd, what, m_regexTableEnd, nFlags ) && IsCmdAt( lpText, what[0].first - lpText ) )
	{
		// parse string before occurence
		ParseString( lpText, what[0].first - lpText, cookies, strActualFile, nActualLine );

		// pop figure from stack
		if( !cookies.IsEmpty() && cookies.Top().nCookieType == table )
		{
			CStructureItem	&si = m_aStructureItems[cookies.Pop().nItemIndex];
			if( si.m_strTitle.IsEmpty() )
			{
				if( si.m_strCaption.IsEmpty() )
					si.m_strTitle = si.m_strLabel;
				else
					si.m_strTitle = si.m_strCaption;
			}
		}

		// parse string behind occurence
		ParseString( what[0].second, lpTextEnd - what[0].second, cookies, strActualFile, nActualLine );

		return;
	}
	// Find bibliography
	if( reg_search( lpText, lpTextEnd, what, m_regexBib, nFlags ) && IsCmdAt( lpText, what[0].first - lpText ) )
	{
		// parse string before occurence
		ParseString( lpText, what[0].first - lpText, cookies, strActualFile, nActualLine );

		// parse input file
		CString	strPath( what[2].first, what[2].second - what[2].first );
		strPath.TrimLeft();
		strPath.TrimRight();
		strPath.TrimLeft(_T('"'));
		strPath.TrimRight(_T('"'));
		strPath+= _T(".bib");
		AddFileItem( strPath );

		// parse string behind occurence
		ParseString( what[0].second, lpTextEnd - what[0].second, cookies, strActualFile, nActualLine );

		return;
	}

	// allow others to make changes to m_aStructureItems
	//m_structureItemsAccess.Unlock();
}


CString CStructureParser::AddFileItem( LPCTSTR lpszPath )
{
	// format file name (remove path, if identical with working dir
	CString	strActualFile( lpszPath );
	if( !m_strWorkingDir.CompareNoCase( CPathTool::Format( _T("%d"), strActualFile ) ) )
		strActualFile = CPathTool::Format( _T("%n"), strActualFile ); 

	// insert file into item-array
	CStructureItem	si;
	si.m_nLine = 0;
	si.m_nParent = m_anItem[m_nDepth];
	si.m_nType = file;
	si.m_strCaption = "";
	si.m_strComment = "";
	si.m_strLabel = "";
	si.m_strPath = strActualFile;
	si.m_strTitle = "";
	m_aStructureItems.Add( si );

	return ( strActualFile );
}


BOOL CStructureParser::Parse(  LPCTSTR lpszPath, CCookieStack &cookies  )
{
	CTextSourceFile	*pTs = NULL;

	pTs = new CTextSourceFile();
	ASSERT(pTs != NULL);
	if( pTs && !pTs->Create( lpszPath ) )
	{
		delete pTs;
		pTs = NULL;
	}

//	CTextSource	*pTs = m_pTextSourceManager->GetTextSource( lpszPath );
	
	if( !pTs )
		return FALSE;

	CString strActualFile( AddFileItem( lpszPath ) );

	// parse text source
	LPCTSTR							lpLine, lpLineEnd, lpOffset;
	int									nLength;
	int									nFlags = match_default;
	reg_match<LPCTSTR>	what;
	BOOL								bVerbatim = FALSE;
	int									nActualLine = 0;

	while( !m_bCancel && pTs->GetNextLine( lpLine, nLength ) )
	{
		nActualLine++;
		lpOffset = lpLine;
		lpLineEnd = lpLine + nLength;

		if( !bVerbatim )
		{
			// find comments
			lpOffset = lpLine;

			while( lpOffset < lpLineEnd && reg_search( lpOffset, lpLineEnd, what, m_regexComment, nFlags ) )
			{
				if( IsCmdAt( lpLine, what[0].first - lpLine - 1 ) )
					lpOffset = what[0].second;
				else
				{
					lpLineEnd = what[0].first;
					break;
				}
			}	

			// find start of verbatim
			if( reg_search( lpLine, lpLineEnd, what, m_regexVerbStart ) )
			{
				if( IsCmdAt( lpLine, what[0].first - lpLine ) )
				{
					// parse line to beginning of \begin{verbatim}-command
					ParseString( lpLine, what[0].first - lpLine, cookies, strActualFile, nActualLine );
					bVerbatim = TRUE;

					// set line start to end of \begin{verbatim}-command
					lpLine = what[0].second;
				}
			}
		}

		// find end of verbatim
		if( bVerbatim )
		{
			if( !reg_search( lpLine, lpLineEnd, what, m_regexVerbEnd ) )
				continue; // continue with next line

			// we found a verbatim end, set line start to end of \end{verbatim}-command
			lpLine = what[0].second;
			bVerbatim = FALSE;
		}

		// find inline verbatim
		lpOffset = lpLine;

		while( lpOffset < lpLineEnd && reg_search( lpOffset, lpLineEnd, what, m_regexInlineVerb, nFlags ) )
		{
			if( !IsCmdAt( lpLine, what[0].first - lpLine ) )
			{
				lpOffset = what[0].first + 1;
				continue;
			}

			// parse before verbatim
			ParseString( lpOffset, what[0].first - lpOffset, cookies, strActualFile, nActualLine );
			lpOffset = what[0].second;
		}

		// parse rest of line
		if( lpOffset < lpLineEnd )
			ParseString( lpOffset, lpLineEnd - lpOffset, cookies, strActualFile, nActualLine );
	}

	// delete text source object
	pTs->Delete();

	return !m_bCancel;
}


void CStructureParser::CancelParsing()
{
	m_bCancel = TRUE;
}
