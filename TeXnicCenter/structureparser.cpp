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
*	 http://www.ToolsCenter.org
*
*********************************************************************/

/********************************************************************
*
* $Id$
*
********************************************************************/

#include "stdafx.h"
#include "stdafx.h"
#include "StructureParser.h"
#include "TextSourceFile.h"
#include "ParseOutputView.h"
#include "resource.h"
#include "bibtexentry.h"
#include "bibtexfile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// Statics
const CString CStructureParser::m_sItemNames[typeCount]= {
	"generic","header","equation","quote","quotation",
	"center","verse","itemization","enumeration","description",
	"figure","table","other environment",
	"texFile","group","bibliography","graphic", "bibitem", "missingFile",
	"missingPicFile", "missingBibFile"};

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
{ _T("appendix"), _T("part"), _T("chapter"), _T("section"), _T("subsection"), _T("subsubsection") };

CStructureParser::CStructureParser()
:	m_bCancel( FALSE )
{
	// Only use the public constructor
	ASSERT( FALSE );
}

CStructureParser::CStructureParser(CStructureParserHandler *pStructureParserHandler, 
								   CParseOutputHandler *pParseOutputHandler)
:	m_bCancel( FALSE ),
	m_pStructureParserHandler( pStructureParserHandler ),
	m_pParseOutputHandler( pParseOutputHandler ),
	m_pStructureParserThread( NULL ),
	m_evtParsingDone(TRUE, TRUE, NULL, NULL)
{
	// initialize attributes
	ASSERT( pStructureParserHandler );

	// initialize regular expressions
	int nResult;

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
		"\\\\begin\\s*\\{figure\\*?\\}"
	) );
	TRACE( "m_regexFiguresStart returned %d\n", nResult );

	nResult = m_regexFigureEnd.set_expression( _T(
		"\\\\end\\s*\\{figure\\*?\\}"
	) );
	TRACE( "m_regexFigureEnd returned %d\n", nResult );

	nResult = m_regexTableStart.set_expression( _T(
		"\\\\begin\\s*\\{(sideways)?table\\*?\\}"
	) );
	TRACE( "m_regexTableStart returned %d\n", nResult );

	nResult = m_regexTableEnd.set_expression( _T(
		"\\\\end\\s*\\{(sideways)?table\\*?\\}"
	) );
	TRACE( "m_regexTableEnd returned %d\n", nResult );

	nResult = m_regexEquationStart.set_expression( _T(
		"\\\\begin\\s*\\{(equation|eqnarray|gather|multline|align|alignat)\\}"
	) );
	TRACE( "m_regexEquationStart returned %d\n", nResult );

	nResult = m_regexEquationEnd.set_expression( _T(
		"\\\\end\\s*\\{(equation|eqnarray|gather|multline|align|alignat)\\}"
	) );
	TRACE( "m_regexEquationEnd returned %d\n", nResult );

	nResult = m_regexCenterStart.set_expression( _T(
		"\\\\begin\\s*\\{center\\}"
	) );
	TRACE( "m_regexCenterStart returned %d\n", nResult );

	nResult = m_regexCenterEnd.set_expression( _T(
		"\\\\end\\s*\\{center\\}"
	) );
	TRACE( "m_regexCenterEnd returned %d\n", nResult );
	
	nResult = m_regexUnknownEnvStart.set_expression( _T(
		"\\\\begin\\s*\\{([^\\}]*)\\}"
	) );
	TRACE( "m_regexUnknownEnvStart returned %d\n", nResult );

	nResult = m_regexUnknownEnvEnd.set_expression( _T(
		"\\\\end\\s*\\{([^\\}]*)\\}"
	) );
	TRACE( "m_regexUnknownEnvEnd returned %d\n", nResult );

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
		"\\\\bibliography\\s*\\{\\s*([^\\}]*)\\s*\\}"
	) );
	TRACE( "m_regexBib returned %d\n", nResult );

	nResult = m_regexAppendix.set_expression( _T("\\\\appendix([^[:graph:]]|$)") );
	TRACE( "m_regexAppendix returned %d\n", nResult );

	// \includegraphics * [ ] [ ] { file }	only {file} is required
	nResult = m_regexGraphic.set_expression( _T(
		"\\\\includegraphics\\s*\\*?(\\s*\\[[^\\]]*\\]){0,2}\\s*\\{\\s*\"?([^\\}]*)\"?\\s*\\}"
	) );
	TRACE( "m_regexGraphic returned %d\n", nResult );

	::InitializeCriticalSection( &m_csSI );
	m_paStructureItems = new CStructureItemArray;
}


CStructureParser::~CStructureParser()
{
	::DeleteCriticalSection( &m_csSI );
	delete m_paStructureItems;
}


BOOL CStructureParser::GetStructureItems(CStructureItemArray *pItemArray)
{
	pItemArray->RemoveAll();
	Lock();
	pItemArray->Copy( *m_paStructureItems );
	Unlock();
	return TRUE;
}


BOOL CStructureParser::StartParsing( LPCTSTR lpszMainPath, LPCTSTR lpszWorkingDir, int nPriority )
{
	// initialization
	for( int i = 0; i < MAX_DEPTH; i++ )
		m_anItem[i] = -1;
	m_strMainPath = lpszMainPath;
	m_strWorkingDir = lpszWorkingDir;
	m_nDepth = 0;
	m_nLinesParsed = 0;
	m_nFilesParsed = 0;
	m_nCharsParsed = 0;

	// Signal that parsing has started.
	m_evtParsingDone.ResetEvent();
	bool bFailed = m_bCancel; 
	if ( !bFailed && m_pParseOutputHandler )
		m_pParseOutputHandler->OnParseBegin( m_bCancel );

	// start parsing thread
	if ( !bFailed )
		bFailed = !( m_pStructureParserThread = AfxBeginThread( StructureParserThread, this, nPriority ) );

	if ( bFailed )
	{
		m_evtParsingDone.SetEvent();
		m_bCancel = FALSE;
		return FALSE;
	}
	return TRUE;
}


UINT StructureParserThread( LPVOID pStructureParser )
{
	CStructureParser::CCookieStack cookies;
	BOOL bParsingResult;

	CStructureParser* pParser = (CStructureParser*)pStructureParser;

	CStructureItemArray *paSI = new CStructureItemArray;
	bParsingResult = pParser->Parse( pParser->m_strMainPath, cookies, 0, *paSI );

	pParser->EmptyCookieStack( cookies, *paSI );

	// Save the results
	pParser->Lock();
	delete pParser->m_paStructureItems;
	pParser->m_paStructureItems = paSI;
	pParser->Unlock();

	pParser->Done( bParsingResult );
	pParser->m_bCancel = FALSE;

	pParser->m_evtParsingDone.SetEvent();

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
	int 	nLen = strText.GetLength();
	int 	nDepth = 0;
	int 	nStart = -1;
	int 	nEnd = -1;

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

	CString strResult = strText.Mid(nStart, nEnd-nStart);
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

#define INITIALIZE_OI( oi )\
	oi.m_strSrcFile = strActualFile;\
	oi.m_nSrcLine = nActualLine;

void CStructureParser::ParseString( LPCTSTR lpText, int nLength, CCookieStack &cookies, const CString &strActualFile, 
								   int nActualLine, int nFileDepth, CStructureItemArray &aSI )
{
	LPCTSTR 						lpTextEnd = lpText;
	CStructureItem			si;
	reg_match<LPCTSTR>	what;
	int 								nFlags = match_default;
	int 								nTypeStart, nTypeCount, nTitleStart, nTitleCount;
	CString 						strHeaderType;
	COOKIE							cookie;

	lpTextEnd = lpText + nLength;

	// look for input command
	if( reg_search( lpText, lpTextEnd, what, m_regexInput, nFlags ) && IsCmdAt( lpText, what[0].first - lpText ) )
	{
		// parse string before occurence
		ParseString( lpText, what[0].first - lpText, cookies, strActualFile, nActualLine, nFileDepth, aSI );

		// parse input file
		CString strPath( what[2].first, what[2].second - what[2].first );
		strPath.TrimLeft();
		strPath.TrimRight();
		strPath.TrimLeft(_T('"'));
		strPath.TrimRight(_T('"'));

		/* Which file does LaTeX include? (tested with MikTeX 2.3)

			You say ==> existing files on disk ==> result / used file

			\input foo ==> foo and foo.tex ==> foo.tex
			\input foo ==> foo ==> error
			\input foo ==> foo.tex.tex ==> error
			\input foo.text ==> foo.text and foo.text.tex ==> foo.text
			\input foo.text ==> foo.text.tex ==> foo.text.tex
			\input foo.tex ==> foo.tex and foo.tex.tex ==> foo.tex
			\input foo.tex ==> foo.tex.tex ==> foo.tex.tex

			TXC shall have the same behaviour. So first we ask for the file extension.
			If it is empty, then we add ".tex" by default.
			If it is non-empty, then we scan for this file first and then for the file with added ".tex".
		*/

		//Check if the file exists and try some extensions
		if ( CPathTool::GetFileExtension(strPath).IsEmpty() )
		{
			strPath += _T(".tex"); //add ".tex" by default.
		}
		else
		{
			//Extension is non-empty. Scan for this file first.
			if (!::PathFileExists(strPath))
			{
				//File does not exist in its original spelling. We add a ".tex".
				CString strNewPath(strPath);
				strNewPath += _T(".tex");
				if (::PathFileExists(strNewPath))
				{
					strPath = strNewPath;
				}
			}
		}

		COutputInfo info;
		INITIALIZE_OI ( info );
		if ( ::PathFileExists(strPath) )
		{
			if ( m_pParseOutputHandler && !m_bCancel )
			{
				info.m_strError.Format( STE_PARSE_PARSING, strPath );
				m_pParseOutputHandler->OnParseLineInfo( info, nFileDepth, CParseOutputHandler::information );
			}
			Parse( strPath, cookies, nFileDepth+1, aSI );
		}
		else if ( m_pParseOutputHandler && !m_bCancel )
		{
			AddFileItem(strPath, missingFile, aSI);
			info.m_strError.Format( STE_FILE_EXIST, strPath );
			m_pParseOutputHandler->OnParseLineInfo( info, nFileDepth, CParseOutputHandler::warning );			
		}

		// parse string behind occurence
		ParseString( what[0].second, lpTextEnd - what[0].second, cookies, strActualFile, nActualLine, nFileDepth, aSI );

		return;
	}
	// look for graphic file
	if( reg_search( lpText, lpTextEnd, what, m_regexGraphic, nFlags ) && IsCmdAt( lpText, what[0].first - lpText ) )
	{
		// This file extension list should be user configurable because the rules for including 
		// graphics are configurable. I think this can wait until the config files are converted 
		// to XML.
		static const CString strGraphicTypes[] = {"", ".eps", ".pdf", ".png", ".jpg", ".jpeg", ".bmp"};
		static const int strGraphicLength = 7;

		// parse string before occurence
		ParseString( lpText, what[0].first - lpText, cookies, strActualFile, nActualLine, nFileDepth, aSI );

		// parse input file
		CString strPath( what[2].first, what[2].second - what[2].first );
		strPath.TrimLeft();
		strPath.TrimRight();
		strPath.TrimLeft(_T('"'));
		strPath.TrimRight(_T('"'));
		bool GraphicFileFound = false;
		CString strCompletePath;
		for (int i = 0; i < strGraphicLength; ++i)
		{
			strCompletePath = strPath;
			strCompletePath += strGraphicTypes[i];
			if ( ::PathFileExists(strCompletePath) )
			{
				AddFileItem( ResolveFileName(strCompletePath), graphicFile, aSI );
				GraphicFileFound = true;
				break;
			} else {
				COutputInfo info;
				INITIALIZE_OI ( info );

				AddFileItem(strCompletePath, missingPicFile, aSI);
				info.m_strError.Format( STE_FILE_EXIST, strPath );				
				m_pParseOutputHandler->OnParseLineInfo( info, nFileDepth, CParseOutputHandler::warning );
				break;
			}
		}

		//Give information
		if (m_pParseOutputHandler && !m_bCancel)
		{
			COutputInfo info;
			INITIALIZE_OI ( info );

			if (GraphicFileFound)
			{
				info.m_strError.Format(STE_PARSE_FOUND, strCompletePath);
				m_pParseOutputHandler->OnParseLineInfo( info, nFileDepth, CParseOutputHandler::information );
			}
			else
			{
				strCompletePath = strPath + " [";
				for (int i = 0; i < strGraphicLength; i++)
				{
					strCompletePath += "|" + strGraphicTypes[i];
				}
				strCompletePath += "]";
				info.m_strError.Format(STE_FILE_EXIST, strCompletePath);
				m_pParseOutputHandler->OnParseLineInfo( info, nFileDepth, CParseOutputHandler::warning );
			}
		}

		// parse string behind occurence
		ParseString( what[0].second, lpTextEnd - what[0].second, cookies, strActualFile, nActualLine, nFileDepth, aSI );

		return;
	}
	// look for figure start
	if( reg_search( lpText, lpTextEnd, what, m_regexFigureStart, nFlags ) && IsCmdAt( lpText, what[0].first - lpText ) )
	{
		// parse string before occurence
		ParseString( lpText, what[0].first - lpText, cookies, strActualFile, nActualLine, nFileDepth, aSI );

		// add figure to collection
		INITIALIZE_SI( si );
		cookie.nCookieType = si.m_nType = figure;
		cookie.nItemIndex = aSI.Add( si );
		cookies.Push( cookie );

		// parse string behind occurence
		ParseString( what[0].second, lpTextEnd - what[0].second, cookies, strActualFile, nActualLine, nFileDepth, aSI );

		return;
	}
	// look for table start
	if( reg_search( lpText, lpTextEnd, what, m_regexTableStart, nFlags ) && IsCmdAt( lpText, what[0].first - lpText ) )
	{
		// parse string before occurence
		ParseString( lpText, what[0].first - lpText, cookies, strActualFile, nActualLine, nFileDepth, aSI );

		// add table to collection
		INITIALIZE_SI( si );
		cookie.nCookieType = si.m_nType = table;
		cookie.nItemIndex = aSI.Add( si );
		cookies.Push( cookie );

		// parse string behind occurence
		ParseString( what[0].second, lpTextEnd - what[0].second, cookies, strActualFile, nActualLine, nFileDepth, aSI );

		return;
	}
	// look for equation start
	if( reg_search( lpText, lpTextEnd, what, m_regexEquationStart, nFlags ) && IsCmdAt( lpText, what[0].first - lpText ) )
	{
		// parse string before occurence
		ParseString( lpText, what[0].first - lpText, cookies, strActualFile, nActualLine, nFileDepth, aSI );

		// add equation to collection
		INITIALIZE_SI( si );
		cookie.nCookieType = si.m_nType = equation;
		cookie.nItemIndex = aSI.Add( si );
		cookies.Push( cookie );

		// parse string behind occurence
		ParseString( what[0].second, lpTextEnd - what[0].second, cookies, strActualFile, nActualLine, nFileDepth, aSI );

		return;
	}
	
	// look for center start
	if( reg_search( lpText, lpTextEnd, what, m_regexCenterStart, nFlags ) && IsCmdAt( lpText, what[0].first - lpText ) )
	{
		// parse string before occurence
		ParseString( lpText, what[0].first - lpText, cookies, strActualFile, nActualLine, nFileDepth, aSI );

		// parse string behind occurence
		ParseString( what[0].second, lpTextEnd - what[0].second, cookies, strActualFile, nActualLine, nFileDepth, aSI );

		return;
	}

	//ATTENTION: Insert the start of other (known) environments before this!
	// look for unknown environment start
	if( reg_search( lpText, lpTextEnd, what, m_regexUnknownEnvStart, nFlags ) && IsCmdAt( lpText, what[0].first - lpText ) )
	{
		//Parse string before occurence
		ParseString( lpText, what[0].first - lpText, cookies, strActualFile, nActualLine, nFileDepth, aSI );

		//Get the name of the environment
		CString strEnvName(what[1].first, what[1].second - what[1].first);

		//Add unknown environment to collection
		INITIALIZE_SI( si );
		cookie.nCookieType = si.m_nType = unknownEnv;
		si.m_strTitle = strEnvName; //Misuse the title for saving the environment name
		cookie.nItemIndex = aSI.Add( si );
		cookies.Push( cookie );

		//Parse string behind occurence
		ParseString( what[0].second, lpTextEnd - what[0].second, cookies, strActualFile, nActualLine, nFileDepth, aSI );

		return;
	}
	// look for headers
	if( reg_search( lpText, lpTextEnd, what, m_regexHeader, nFlags ) && IsCmdAt( lpText, what[0].first - lpText ) )
	{
		// parse string before occurence
		ParseString( lpText, what[0].first - lpText, cookies, strActualFile, nActualLine, nFileDepth, aSI );

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

		CString strFullMatch(what[2].first, nTitleCount);
		if (strFullMatch[0] == _T('['))
			si.m_strTitle = GetArgument(strFullMatch, _T('['), _T(']'));
		else
			si.m_strTitle = GetArgument(strFullMatch, _T('{'), _T('}'));
		si.m_nType = header;
		cookie.nItemIndex = m_anItem[m_nDepth] = aSI.Add( si );
		cookie.nCookieType = header;
		cookies.Push( cookie );

		// parse string behind occurence
		int nEnd = nTitleStart+nTitleCount;
		nEnd = nEnd - (strFullMatch.GetLength() - si.m_strTitle.GetLength()) + 2;
		ParseString( lpText+nEnd, lpTextEnd - (lpText+nEnd), cookies, strActualFile, nActualLine, nFileDepth, aSI );

		return;
	}

	// find label
	if( reg_search( lpText, lpTextEnd, what, m_regexLabel, nFlags ) && IsCmdAt( lpText, what[0].first - lpText ) )
	{
		// parse string before occurence
		ParseString( lpText, what[0].first - lpText, cookies, strActualFile, nActualLine, nFileDepth, aSI );

		if( !cookies.IsEmpty() )
			if( cookies.Top().nCookieType == header )
				aSI[cookies.Pop().nItemIndex].m_strLabel = CString( what[1].first, what[1].second - what[1].first );
			else
				aSI[cookies.Top().nItemIndex].m_strLabel = CString( what[1].first, what[1].second - what[1].first );

		// parse string behind occurence
		ParseString( what[0].second, lpTextEnd - what[0].second, cookies, strActualFile, nActualLine, nFileDepth, aSI );

		return;
	}

	// find caption
	if( reg_search( lpText, lpTextEnd, what, m_regexCaption, nFlags ) && IsCmdAt( lpText, what[0].first - lpText ) )
	{
		// parse string before occurence
		ParseString( lpText, what[0].first - lpText, cookies, strActualFile, nActualLine, nFileDepth, aSI );

		if( !cookies.IsEmpty() )
		{
			CString strFullMatch(what[1].first, what[1].second - what[1].first);
			if (strFullMatch[0] == _T('['))
				aSI[cookies.Top().nItemIndex].m_strCaption = GetArgument(strFullMatch, _T('['), _T(']'));
			else
				aSI[cookies.Top().nItemIndex].m_strCaption = GetArgument(strFullMatch, _T('{'), _T('}'));
		}

		// parse string behind occurence
		ParseString( what[0].second, lpTextEnd - what[0].second, cookies, strActualFile, nActualLine, nFileDepth, aSI );

		return;
	}

	// find end of figure
	if( reg_search( lpText, lpTextEnd, what, m_regexFigureEnd, nFlags ) && IsCmdAt( lpText, what[0].first - lpText ) )
	{
		// parse string before occurence
		ParseString( lpText, what[0].first - lpText, cookies, strActualFile, nActualLine, nFileDepth, aSI );

		// pop figure from stack
		if( !cookies.IsEmpty() && cookies.Top().nCookieType == figure )
		{
			CStructureItem	&si = aSI[cookies.Pop().nItemIndex];
			CreateDefaultTitle(si);
//			if( si.m_strTitle.IsEmpty() )
//			{
//				if( si.m_strCaption.IsEmpty() )
//					si.m_strTitle = si.m_strLabel;
//				else
//					si.m_strTitle = si.m_strCaption;
//			}
		}
		else if ( m_pParseOutputHandler && !m_bCancel )
		{
			COutputInfo info;
			INITIALIZE_OI( info );
			info.m_strError.Format( STE_PARSE_FOUND_UNMATCHED, m_sItemNames[figure] );
			m_pParseOutputHandler->OnParseLineInfo( info, nFileDepth, CParseOutputHandler::warning );
		}

		// parse string behind occurence
		ParseString( what[0].second, lpTextEnd - what[0].second, cookies, strActualFile, nActualLine, nFileDepth, aSI );

		return;
	}
	// find end of table
	if( reg_search( lpText, lpTextEnd, what, m_regexTableEnd, nFlags ) && IsCmdAt( lpText, what[0].first - lpText ) )
	{
		// parse string before occurence
		ParseString( lpText, what[0].first - lpText, cookies, strActualFile, nActualLine, nFileDepth, aSI );

		// pop figure from stack
		if( !cookies.IsEmpty() && cookies.Top().nCookieType == table )
		{
			CStructureItem	&si = aSI[cookies.Pop().nItemIndex];
			CreateDefaultTitle(si);
//			if( si.m_strTitle.IsEmpty() )
//			{
//				if( si.m_strCaption.IsEmpty() )
//					si.m_strTitle = si.m_strLabel;
//				else
//					si.m_strTitle = si.m_strCaption;
//			}
		}
		else if ( m_pParseOutputHandler && !m_bCancel )
		{
			COutputInfo info;
			INITIALIZE_OI( info );
			info.m_strError.Format( STE_PARSE_FOUND_UNMATCHED, m_sItemNames[table] );
			m_pParseOutputHandler->OnParseLineInfo( info, nFileDepth, CParseOutputHandler::warning );
		}

		// parse string behind occurence
		ParseString( what[0].second, lpTextEnd - what[0].second, cookies, strActualFile, nActualLine, nFileDepth, aSI );

		return;
	}
	// find end of equation
	if( reg_search( lpText, lpTextEnd, what, m_regexEquationEnd, nFlags ) && IsCmdAt( lpText, what[0].first - lpText ) )
	{
		// parse string before occurence
		ParseString( lpText, what[0].first - lpText, cookies, strActualFile, nActualLine, nFileDepth, aSI );

		// pop equation from stack
		if( !cookies.IsEmpty() && cookies.Top().nCookieType == equation )
		{
			CStructureItem	&si = aSI[cookies.Pop().nItemIndex];
			CreateDefaultTitle(si);
//			if( si.m_strTitle.IsEmpty() )
//			{
//				if( si.m_strCaption.IsEmpty() )
//					si.m_strTitle = si.m_strLabel;
//				else
//					si.m_strTitle = si.m_strCaption;
//			}
		}
		else if ( m_pParseOutputHandler && !m_bCancel )
		{
			COutputInfo info;
			INITIALIZE_OI( info );
			info.m_strError.Format( STE_PARSE_FOUND_UNMATCHED, m_sItemNames[equation] );
			m_pParseOutputHandler->OnParseLineInfo( info, nFileDepth, CParseOutputHandler::warning );
		}

		// parse string behind occurence
		ParseString( what[0].second, lpTextEnd - what[0].second, cookies, strActualFile, nActualLine, nFileDepth, aSI );

		return;
	}

	// find end of center
	if( reg_search( lpText, lpTextEnd, what, m_regexCenterEnd, nFlags ) && IsCmdAt( lpText, what[0].first - lpText ) )
	{
		// parse string before occurence
		ParseString( lpText, what[0].first - lpText, cookies, strActualFile, nActualLine, nFileDepth, aSI );

		// parse string behind occurence
		ParseString( what[0].second, lpTextEnd - what[0].second, cookies, strActualFile, nActualLine, nFileDepth, aSI );

		return;
	}

	//ATTENTION: Insert the end of other (known) environments before this!
	// find end of unknown environment
	if( reg_search( lpText, lpTextEnd, what, m_regexUnknownEnvEnd, nFlags ) && IsCmdAt( lpText, what[0].first - lpText ) )
	{
		//Parse string before occurence
		ParseString( lpText, what[0].first - lpText, cookies, strActualFile, nActualLine, nFileDepth, aSI );

		//Get the name of the environment
		CString strEnvName(what[1].first, what[1].second - what[1].first);

		//Pop unknown environment from stack
		if ( !cookies.IsEmpty()
				&& (cookies.Top().nCookieType == unknownEnv)
				&& (aSI[cookies.Top().nItemIndex].m_strTitle == strEnvName) )
		{
			CStructureItem &si = aSI[cookies.Pop().nItemIndex];
			if (si.m_strCaption.IsEmpty() && si.m_strLabel.IsEmpty())
			{
				si.m_strTitle.Format("%s: %s(%d)", strEnvName, ResolveFileName(si.m_strPath), si.m_nLine);
			}
			else
			{
				if (si.m_strCaption.IsEmpty())
					si.m_strTitle = strEnvName + _T(": ") + si.m_strLabel;
				else
					si.m_strTitle = strEnvName + _T(": ") + si.m_strCaption;
			}
		}
		else if ( m_pParseOutputHandler && !m_bCancel )
		{
			COutputInfo info;
			INITIALIZE_OI(info);
			info.m_strError.Format(STE_PARSE_FOUND_UNMATCHED, strEnvName);
			m_pParseOutputHandler->OnParseLineInfo( info, nFileDepth, CParseOutputHandler::warning );
		}

		// parse string behind occurence
		ParseString( what[0].second, lpTextEnd - what[0].second, cookies, strActualFile, nActualLine, nFileDepth, aSI );

		return;
	}
	// Find appendix
	if( reg_search( lpText, lpTextEnd, what, m_regexAppendix, nFlags ) && IsCmdAt( lpText, what[0].first - lpText ) )
	{
		// parse string before occurence
		ParseString( lpText, what[0].first - lpText, cookies, strActualFile, nActualLine, nFileDepth, aSI );

		// Reset the headers
		EmptyCookieStack( cookies, aSI );
		m_nDepth = 1;

		// add appendix to collection
		INITIALIZE_SI( si );
		si.m_strTitle.LoadString(STE_APPENDIX);
		si.m_nParent = -1;
		cookie.nCookieType = si.m_nType = header;
		cookie.nItemIndex = m_anItem[m_nDepth] = aSI.Add( si );
		cookies.Push( cookie );

		// parse string behind occurence
		ParseString( what[0].second, lpTextEnd - what[0].second, cookies, strActualFile, nActualLine, nFileDepth, aSI );
		return;
	}
	// Find bibliography
	if( reg_search( lpText, lpTextEnd, what, m_regexBib, nFlags ) && IsCmdAt( lpText, what[0].first - lpText ) )
	{
		// parse string before occurence
		ParseString( lpText, what[0].first - lpText, cookies, strActualFile, nActualLine, nFileDepth, aSI );

		// parse input file
		CString bibPath( what[1].first, what[1].second - what[1].first );
		int nStart = 0;
		int nFound;
		while (true)
		{
			nFound = bibPath.Find( _T(','), nStart );
			if ( nFound == -1 )
				if ( nStart >= bibPath.GetLength() )
					// Done
					break;
				else
					// Extract last comma separated element from list
					nFound = bibPath.GetLength();

			CString strPath( bibPath.Mid(nStart, nFound-nStart) );
			strPath.TrimLeft();
			strPath.TrimRight();
			strPath.TrimLeft( _T('"') );
			strPath.TrimRight( _T('"') );
			//BibTeX takes only the .bib-Extension!
			// And in \bibliography{...} the user has to omitt the extension!
			// We could warn the user, if he uses an extension in \bibliography{...}
			strPath += _T(".bib");
			strPath = ResolveFileName(strPath);

			COutputInfo info;
			INITIALIZE_OI ( info );

			if ( ::PathFileExists(strPath) )
			{
				
				/* begin ow */
				CBiBTeXFile aBibFile(strPath);
				aBibFile.DropAllEntries();	// clean up previous result
				aBibFile.ProcessFile();		// parse it ...
				const CMapStringToOb *items = aBibFile.GetEntries(); // ...and fetch the entries.

				if (aBibFile.GetErrorCount()) {
					CString strCap;
					strCap.Format(_T("%s (%d errors)"), strPath, aBibFile.GetErrorCount());
					AddFileItem( strCap, bibFile, aSI );
				} else {
					AddFileItem( strPath, bibFile, aSI );
				}
				POSITION pos = items->GetStartPosition(); // iterate over entry map
				while(pos != NULL) {
					CBiBTeXEntry *be;
					CString key;
					items->GetNextAssoc(pos, key, (CObject*&)be);
					if (be != NULL) { // setup entry
						be->m_nParent = m_anItem[m_nDepth];
						cookie.nCookieType = be->m_nType;
						//TRACE("Added si: %s, %s, %s\n", be->m_strPath, be->m_strLabel, be->m_strCaption);
						cookie.nItemIndex = aSI.Add( *be );
						cookies.Push( cookie );
					} else {
						TRACE("NP detected in CBiBTeXFile %s", strPath);
					}					
				}
				/* end ow */
				if ( m_pParseOutputHandler && !m_bCancel )
				{
					info.m_strError.Format( STE_PARSE_FOUND, m_sItemNames[bibFile] );
					m_pParseOutputHandler->OnParseLineInfo( info, nFileDepth, CParseOutputHandler::information );
				}
			}
			else 
			{
				if ( m_pParseOutputHandler && !m_bCancel )
				{
					AddFileItem(strPath, missingBibFile, aSI);				
					info.m_strError.Format( STE_FILE_EXIST, strPath );
					m_pParseOutputHandler->OnParseLineInfo( info, nFileDepth, CParseOutputHandler::warning );
				}
			}
			nStart = nFound + 1;
		}

		// parse string behind occurence
		ParseString( what[0].second, lpTextEnd - what[0].second, cookies, strActualFile, nActualLine, nFileDepth, aSI );

		return;
	}
}


CString CStructureParser::ResolveFileName( LPCTSTR lpszPath ) const
{
	// format file name (remove path, if identical with working dir
	CString strActualFile( lpszPath );
	if( !m_strWorkingDir.CompareNoCase( CPathTool::Format( _T("%d"), strActualFile ) ) )
		strActualFile = CPathTool::Format( _T("%n"), strActualFile ); 
	return ( strActualFile );
}


int CStructureParser::AddFileItem( LPCTSTR lpszPath, int nType, CStructureItemArray &aSI )
{
	// insert file into item-array
	CStructureItem	si;
	si.m_nLine = 0;
	si.m_nParent = m_anItem[m_nDepth];
	si.m_nType = nType;
	si.m_strCaption = "";
	si.m_strComment = "";
	si.m_strLabel = "";
	si.m_strPath = lpszPath;
	si.m_strTitle = "";
	return aSI.Add( si );
}

void CStructureParser::EmptyCookieStack( CCookieStack &cookies, CStructureItemArray &aSI )
{
	if ( m_pParseOutputHandler && !m_bCancel )
	{
		while ( !cookies.IsEmpty() )
		{
			COOKIE item = cookies.Pop();
			COutputInfo info;
			CStructureItem	&si = aSI[item.nItemIndex];

			if (item.nCookieType == table || item.nCookieType == figure) 
			{
				info.m_nSrcLine = si.m_nLine;
				info.m_strSrcFile = si.m_strPath;
				info.m_strError.Format( STE_PARSE_FOUND_UNMATCHED, m_sItemNames[item.nCookieType] );
				m_pParseOutputHandler->OnParseLineInfo( info, 0, CParseOutputHandler::warning );
			}
		}
	}
	else
	{
		cookies.RemoveAll();
	}
}

void CStructureParser::Done( boolean bParsingResult )
{
	m_pStructureParserHandler->OnParsingFinished( bParsingResult );
	if ( m_pParseOutputHandler && !m_bCancel )
		m_pParseOutputHandler->OnParseEnd( bParsingResult, m_nFilesParsed, m_nLinesParsed );
}


BOOL CStructureParser::Parse(  LPCTSTR lpszPath, CCookieStack &cookies, int nFileDepth, CStructureItemArray &aSI )
{
	CTextSourceFile *pTs = NULL;

	pTs = new CTextSourceFile();
	ASSERT(pTs != NULL);
	if( pTs && !pTs->Create( lpszPath ) )
	{
		delete pTs;
		pTs = NULL;
	}

	if( !pTs )
	{
		if ( m_pParseOutputHandler && !m_bCancel )
		{
			COutputInfo info;
			info.m_strSrcFile = lpszPath;
			info.m_nSrcLine = 0;
			info.m_strError.Format( STE_GREP_ERROR, lpszPath);
			m_pParseOutputHandler->OnParseLineInfo( info, nFileDepth, CParseOutputHandler::warning );
		}
		return FALSE;
	}
	m_nFilesParsed++;

	CString strActualFile( lpszPath );
	AddFileItem( strActualFile, texFile, aSI );

	// parse text source
	LPCTSTR 						lpLine, lpLineEnd, lpOffset;
	int 								nLength;
	int 								nFlags = match_default;
	reg_match<LPCTSTR>	what;
	BOOL								bVerbatim = FALSE;
	int 								nActualLine = 0;

	while( !m_bCancel && pTs->GetNextLine( lpLine, nLength ) )
	{
		m_nLinesParsed++;
		m_nCharsParsed += nLength;
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
					ParseString( lpLine, what[0].first - lpLine, cookies, strActualFile, nActualLine, nFileDepth, aSI );
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
			ParseString( lpOffset, what[0].first - lpOffset, cookies, strActualFile, nActualLine, nFileDepth, aSI );
			lpOffset = what[0].second;
		}

		// parse rest of line
		if( lpOffset < lpLineEnd )
			ParseString( lpOffset, lpLineEnd - lpOffset, cookies, strActualFile, nActualLine, nFileDepth, aSI );
	}

	// delete text source object
	pTs->Delete();

	return !m_bCancel;
}


void CStructureParser::CancelParsing()
{
	m_bCancel = TRUE;
}
