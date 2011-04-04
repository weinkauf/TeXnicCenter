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

#include <algorithm>
#include <functional>
#include <memory>

#include "StructureParser.h"
#include "TextSourceFile.h"
#include "ParseOutputView.h"
#include "resource.h"
#include "bibtexentry.h"
#include "bibtexfile.h"
#include "LatexProject.h"
#include "CharType.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// Statics
const CString CStructureParser::m_sItemNames[StructureItem::typeCount] =
{
	_T("generic"), _T("header"), _T("equation"), _T("quote"), _T("quotation"),
	_T("center"), _T("verse"), _T("itemization"), _T("enumeration"), _T("description"),
	_T("figure"), _T("table"), _T("other environment"),
	_T("texFile"), _T("group"), _T("bibFile"), _T("graphic"), _T("bibitem"),
	_T("missingTexFile"), _T("missingGraphicFile"), _T("missingBibFile")
};

//-------------------------------------------------------------------
// class CStructureParser
//-------------------------------------------------------------------
const CString CStructureParser::m_astrHeader[MAX_DEPTH][CStructureParser::HeaderVariationsCount] = {
	{ _T("appendix") }, 
	{ _T("part"), _T("addpart") }, 
	{ _T("chapter"), _T("addchap") },
	{ _T("section"), _T("addsec") },
	{ _T("subsection"), _T("addsubsec") }, 
	{ _T("subsubsection"), _T("addsubsubsec") },
};

CStructureParser::CStructureParser()
		: m_bCancel(FALSE)
{
	// Only use the public constructor
	ASSERT(FALSE);
}

const CString CStructureParser::CreateHeaderRegularExpression()
{
	CString result;

	for (int i = 0; i < MAX_DEPTH; ++i) {
		for (int j = 0; j < HeaderVariationsCount && !m_astrHeader[i][j].IsEmpty(); ++j)
			result += m_astrHeader[i][j] + _T('|');
	}

	result.Delete(result.GetLength() - 1); // Remove the last |

	return _T("\\\\(") + result + _T(")\\s*\\*?\\s*([\\[\\{].*\\})");
}

CStructureParser::CStructureParser(CStructureParserHandler *pStructureParserHandler,
                                   CParseOutputHandler *pParseOutputHandler)
: m_bCancel(FALSE)
, m_pStructureParserHandler(pStructureParserHandler)
, m_pParseOutputHandler(pParseOutputHandler)
, m_pStructureParserThread(NULL)
, m_evtParsingDone(TRUE, TRUE, NULL, NULL)
, m_regexHeader(CreateHeaderRegularExpression())
, m_regexComment(_T("%"))
, m_regexVerbStart(_T("\\\\begin\\s*\\{verbatim\\*?\\}"))
, m_regexVerbEnd(_T("\\\\end\\{verbatim\\*?\\}"))
, listing_start_(_T("\\\\begin\\s*\\{lstlisting\\*?\\}"))
, listing_end_(_T("\\\\end\\{lstlisting\\*?\\}"))
, m_regexFigureStart(_T("\\\\begin\\s*\\{figure\\*?\\}"))
, m_regexFigureEnd(_T("\\\\end\\s*\\{figure\\*?\\}"))
, m_regexTableStart(_T("\\\\begin\\s*\\{(sideways)?table\\*?\\}"))
, m_regexTableEnd(_T("\\\\end\\s*\\{(sideways)?table\\*?\\}"))
, m_regexEquationStart(_T("\\\\begin\\s*\\{(equation|eqnarray|gather|multline|align|alignat|flalign)\\}"))
, m_regexEquationEnd(_T("\\\\end\\s*\\{(equation|eqnarray|gather|multline|align|alignat|flalign)\\}"))
, m_regexCenterStart(_T("\\\\begin\\s*\\{center\\}"))
, m_regexCenterEnd(_T("\\\\end\\s*\\{center\\}"))
, m_regexUnknownEnvStart(_T("\\\\begin\\s*\\{([^\\}]*)\\}"))
, m_regexUnknownEnvEnd(_T("\\\\end\\s*\\{([^\\}]*)\\}"))
, m_regexCaption(_T("\\\\caption\\s*([\\[\\{].*\\})"))
, m_regexLabel(_T("\\\\label\\s*\\{([^\\}]*)\\}"))
, m_regexInput(_T("\\\\(input|include)\\s*\\{\\s*\"?([^\\}]*)\"?\\s*\\}"))
, m_regexBib(_T("\\\\(no)?bibliography([^(style)][[:alpha:]]+)?\\s*\\{\\s*([^\\}]*)\\s*\\}"))
, m_regexAppendix(_T("\\\\appendix([^[:graph:]]|$)"))
, m_regexGraphic(_T("\\\\includegraphics\\s*\\*?(\\s*\\[[^\\]]*\\]){0,2}\\s*\\{\\s*\"?([^\\}]*)\"?\\s*\\}"))
, m_regexUserCmd(_T("\\\\(re)?newcommand\\s*\\{([^\\}]*)\\}(\\s*\\[[^\\]]*\\])\\s*\\{([^\\}]*)\\}"))
, m_regexUserEnv(_T("\\\\(re)?newenvironment\\s*\\{([^\\}]*)\\}(\\s*\\[[^\\]]*\\])\\s*\\{([^\\}]*)\\}\\s*\\{([^\\}]*)\\}"))
, m_regexInlineVerb(_T("\\\\verb\\*(.)[^$1]*\\1|\\\\verb([^\\*])[^$2]*\\2"))
, index_(_T("\\\\printindex"))
, nomencl_(_T("\\\\print(nomenclature|glossary)"))
{
	// initialize attributes
	ASSERT(pStructureParserHandler);
}

CStructureParser::~CStructureParser()
{
}

BOOL CStructureParser::GetStructureItems(StructureItemContainer *pItemArray)
{
	pItemArray->clear();
    
    CSingleLock lock(&m_csSI);
	*pItemArray = m_paStructureItems;

	return TRUE;
}

BOOL CStructureParser::StartParsing(LPCTSTR lpszMainPath, LPCTSTR lpszWorkingDir, int nPriority)
{
	// initialization
	for (int i = 0; i < MAX_DEPTH; i++)
		m_anItem[i] = -1;

	m_strMainPath = lpszMainPath;
	m_strWorkingDir = lpszWorkingDir;
	m_nDepth = 0;
	m_nLinesParsed = 0;
	m_nFilesParsed = 0;
	m_nCharsParsed = 0;

	// Signal that parsing has started.
	m_evtParsingDone.ResetEvent();
	BOOL bFailed = m_bCancel;

	if (!bFailed && m_pParseOutputHandler)
		m_pParseOutputHandler->OnParseBegin(m_bCancel != 0);

	// start parsing thread
	if (!bFailed)
	{
		m_pStructureParserThread = AfxBeginThread(StructureParserThread, this, nPriority);
		bFailed = !m_pStructureParserThread;
	}

	if (bFailed)
	{
		m_evtParsingDone.SetEvent();
		m_bCancel = FALSE;
		return FALSE;
	}

	return TRUE;
}

UINT StructureParserThread(LPVOID pStructureParser)
{
	CStructureParser::CCookieStack cookies;
	BOOL bParsingResult;

	CStructureParser* pParser = (CStructureParser*)pStructureParser;

	StructureItemContainer paSI;
	bParsingResult = pParser->Parse(pParser->m_strMainPath, cookies, 0, paSI);

	pParser->EmptyCookieStack(cookies, paSI);
	
    {
        // Save the results
        CSingleLock lock(&pParser->m_csSI);
        pParser->m_paStructureItems.swap(paSI);
    }

	pParser->Done(bParsingResult != 0);
	pParser->m_bCancel = FALSE;

	pParser->m_evtParsingDone.SetEvent();

	return 0;
}

bool CStructureParser::IsCmdAt(LPCTSTR lpText, int nPos)
{
	// count number of backslashes before command (including command backslash)
	int nCount = 0;
	for (; nPos >= 0 && *(lpText + nPos) == _T('\\'); nCount++, nPos--);
	return !(nCount % 2 == 0);
}

CString CStructureParser::GetArgument(const CString &strText, TCHAR tcOpeningDelimiter,
                                      TCHAR tcClosingDelimiter)
{
	int nLen = strText.GetLength();
	int nDepth = 0;
	int nStart = -1;
	int nEnd = -1;

	// find argument
	for (int i = 0; i < nLen; i++)
	{
		if (strText[i] == tcOpeningDelimiter)
		{
			if (nStart < 0)
				nStart = i + 1;
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
				--nDepth;
		}
	}

	if (nEnd - nStart <= 0)
		return _T("");

	CString strResult = strText.Mid(nStart, nEnd - nStart);
	strResult.TrimLeft();
	strResult.TrimRight();
	return strResult;
}


#define INITIALIZE_SI( si )\
	si.m_nLine = nActualLine;\
	si.m_nParent = m_anItem[m_nDepth];\
	si.m_strComment.Empty();\
	si.m_strCaption.Empty();\
	si.RemoveLabels();\
	si.m_strPath = strActualFile;\
	si.m_strTitle.Empty()

#define INITIALIZE_OI( oi )\
	oi.SetSourceFile(strActualFile);\
	oi.SetSourceLine(nActualLine);

void CStructureParser::ParseString(LPCTSTR lpText, int nLength, CCookieStack &cookies,
                                   const CString &strActualFile,
                                   int nActualLine, int nFileDepth, StructureItemContainer &aSI)
{
	LPCTSTR lpTextEnd = lpText;
	StructureItem si;
	std::tr1::match_results<LPCTSTR> what;
	std::tr1::regex_constants::match_flag_type nFlags = std::tr1::regex_constants::match_default;
	int nTypeStart, nTypeCount, nTitleStart, nTitleCount;
	CString strHeaderType;
	COOKIE cookie;

	lpTextEnd = lpText + nLength;

	/* OW 060419: Disabled for upcoming release, will be used later to extend autocomplete function. The regexES have been
	   tested and seem to work properly - as far as *no comments* are part of the expression

	        Tino: Add re-commands as well!

	  Searching for user commands
	if( regex_search( lpText, lpTextEnd, what, m_regexUserCmd, nFlags ) && IsCmdAt( lpText, what[0].first - lpText ) )
	{
	        CString strCmd( what[1].first, what[1].second - what[1].first );
	        CString strPar( what[2].first + 1, what[2].second - what[2].first - 2 );

	        TRACE("User cmd: '%s', parameters: %s\n", strCmd, strPar);
	        return;
	}

	 Searching for user commands
	if( regex_search( lpText, lpTextEnd, what, m_regexUserEnv, nFlags ) && IsCmdAt( lpText, what[0].first - lpText ) )
	{
	        CString strCmd( what[1].first, what[1].second - what[1].first );
	        CString strPar( what[2].first + 1, what[2].second - what[2].first - 2 );

	        TRACE("User env: '%s', parameters: %s\n", strCmd, strPar);
	        return;
	}*/

	// look for input command
	if (regex_search(lpText, lpTextEnd, what, m_regexInput, nFlags) && IsCmdAt(lpText, what[0].first - lpText))
	{
		// parse string before occurrence
		ParseString(lpText, what[0].first - lpText, cookies,
		            strActualFile, nActualLine, nFileDepth, aSI);

		// parse input file
		CString strPath(what[2].first, what[2].second - what[2].first);
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
		if (CPathTool::GetFileExtension(strPath).IsEmpty())
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
		INITIALIZE_OI(info);

		if (::PathFileExists(strPath))
		{
			CString path;

			if (CPathTool::IsRelativePath(strPath))
			{
				CString dir;
				dir.ReleaseBuffer(::GetCurrentDirectory(MAX_PATH,dir.GetBuffer(MAX_PATH)));
				
				path = CPathTool::Cat(dir,strPath);
			}

			const CString relative_path = CPathTool::GetRelativePath(path,strActualFile,FALSE,FALSE);

			// Make sure it's not a recursive inclusion
			// otherwise it will result in a stack overflow
			if (relative_path.CompareNoCase(strPath) != 0)
			{
				if (m_pParseOutputHandler && !m_bCancel)
				{
					CString message;
					message.Format(STE_PARSE_PARSING, strPath);
					
					info.SetErrorMessage(message);

					m_pParseOutputHandler->OnParseLineInfo(info, nFileDepth, CParseOutputHandler::information);
				}

				Parse(strPath, cookies, nFileDepth + 1, aSI);
			}
			else // otherwise notify the user about the error
			{
				if (m_pParseOutputHandler && !m_bCancel)
				{
					info.SetErrorMessage(CString(MAKEINTRESOURCE(IDS_RECURSIVE_INCLUSION)));
					m_pParseOutputHandler->OnParseLineInfo(info, nFileDepth, CParseOutputHandler::error);
				}
			}
		}
		else if (m_pParseOutputHandler && !m_bCancel)
		{
			AddFileItem(strPath, StructureItem::missingTexFile, strActualFile, nActualLine, aSI);

			CString message;
			message.Format(STE_FILE_EXIST, strPath);

			info.SetErrorMessage(message);
			m_pParseOutputHandler->OnParseLineInfo(info, nFileDepth, CParseOutputHandler::warning);
		}

		// parse string behind occurrence
		ParseString(what[0].second, lpTextEnd - what[0].second, cookies,
		            strActualFile, nActualLine, nFileDepth, aSI);

		return;
	}

	// look for graphic file
	if (regex_search(lpText, lpTextEnd, what, m_regexGraphic, nFlags) && IsCmdAt(lpText, what[0].first - lpText))
	{
		// This file extension list should be user configurable because the rules for including
		// graphics are configurable. I think this can wait until the config files are converted
		// to XML.
		static const CString strGraphicTypes[] =
		{
			_T(""), _T(".eps"), _T(".pdf"), _T(".png"),
			_T(".jpg"), _T(".jpeg"), _T(".bmp")
		};
		static const int strGraphicLength = 7;

		// parse string before occurrence
		ParseString(lpText, what[0].first - lpText, cookies,
		            strActualFile, nActualLine, nFileDepth, aSI);

		// parse input file
		CString strPath(what[2].first, what[2].second - what[2].first);
		strPath.Trim();
		strPath.Trim(_T('"'));
		bool GraphicFileFound = false;
		CString strCompletePath;

		for (int i = 0; i < strGraphicLength; ++i)
		{
			strCompletePath = strPath;
			const CString& extension = strGraphicTypes[i];
			strCompletePath += extension;

			if (::PathFileExists(strCompletePath))
			{
				//File exists
				AddFileItem(ResolveFileName(strCompletePath), StructureItem::graphicFile,
				            strActualFile, nActualLine, aSI);
				GraphicFileFound = true;
				break;
			}
		}

		//File does not exist? Add as missing.
		if (!GraphicFileFound)
		{
			AddFileItem(strPath, StructureItem::missingGraphicFile, strActualFile, nActualLine, aSI);
		}

		//Give information
		if (m_pParseOutputHandler && !m_bCancel)
		{
			COutputInfo info;
			INITIALIZE_OI(info);

			if (GraphicFileFound)
			{
				CString message;
				message.Format(STE_PARSE_FOUND, strCompletePath);
				
				info.SetErrorMessage(message);
				m_pParseOutputHandler->OnParseLineInfo(info, nFileDepth, CParseOutputHandler::information);
			}
			else
			{
				CString concat;

				for (int i = 0; i < strGraphicLength; i++)
				{
					const CString& extension = strGraphicTypes[i];

					if (!extension.IsEmpty()) 
					{
						if (!concat.IsEmpty())
							concat += _T("|");

						concat += extension;
					}
				}

				strCompletePath = strPath + _T(" [") + concat + _T("]");
				
				CString message;
				message.Format(STE_FILE_EXIST, strCompletePath);
				
				info.SetErrorMessage(message);
				m_pParseOutputHandler->OnParseLineInfo(info, nFileDepth, CParseOutputHandler::warning);
			}
		}

		// parse string behind occurrence
		ParseString(what[0].second, lpTextEnd - what[0].second,
		            cookies, strActualFile, nActualLine, nFileDepth, aSI);

		return;
	}

	// look for StructureItem::figure start
	if (regex_search(lpText, lpTextEnd, what, m_regexFigureStart, nFlags) && IsCmdAt(lpText, what[0].first - lpText))
	{
		// parse string before occurrence
		ParseString(lpText, what[0].first - lpText, cookies,
		            strActualFile, nActualLine, nFileDepth, aSI);

		// add StructureItem::figure to collection
		INITIALIZE_SI(si);
		cookie.nCookieType = si.m_nType = StructureItem::figure;
		cookie.nItemIndex = aSI.size();
		aSI.push_back(si);
		cookies.push(cookie);

		// parse string behind occurrence
		ParseString(what[0].second, lpTextEnd - what[0].second, cookies,
		            strActualFile, nActualLine, nFileDepth, aSI);

		return;
	}

	// look for table start
	if (regex_search(lpText, lpTextEnd, what, m_regexTableStart, nFlags) && IsCmdAt(lpText, what[0].first - lpText))
	{
		// parse string before occurrence
		ParseString(lpText, what[0].first - lpText, cookies,
		            strActualFile, nActualLine, nFileDepth, aSI);

		// add table to collection
		INITIALIZE_SI(si);
		cookie.nCookieType = si.m_nType = StructureItem::table;
		cookie.nItemIndex = aSI.size();
		aSI.push_back(si);
		cookies.push(cookie);

		// parse string behind occurrence
		ParseString(what[0].second, lpTextEnd - what[0].second, cookies,
		            strActualFile, nActualLine, nFileDepth, aSI);

		return;
	}
	// look for equation start
	if (regex_search(lpText, lpTextEnd, what, m_regexEquationStart, nFlags) && IsCmdAt(lpText, what[0].first - lpText))
	{
		// parse string before occurrence
		ParseString(lpText, what[0].first - lpText, cookies,
		            strActualFile, nActualLine, nFileDepth, aSI);

		// add equation to collection
		INITIALIZE_SI(si);
		cookie.nCookieType = si.m_nType = StructureItem::equation;
		cookie.nItemIndex = aSI.size();
		aSI.push_back(si);
		cookies.push(cookie);

		// parse string behind occurrence
		ParseString(what[0].second, lpTextEnd - what[0].second, cookies,
		            strActualFile, nActualLine, nFileDepth, aSI);

		return;
	}

	// look for center start
	if (regex_search(lpText, lpTextEnd, what, m_regexCenterStart, nFlags) && IsCmdAt(lpText, what[0].first - lpText))
	{
		// parse string before occurrence
		ParseString(lpText, what[0].first - lpText, cookies,
		            strActualFile, nActualLine, nFileDepth, aSI);

		// parse string behind occurrence
		ParseString(what[0].second, lpTextEnd - what[0].second, cookies,
		            strActualFile, nActualLine, nFileDepth, aSI);

		return;
	}

	//ATTENTION: Insert the start of other (known) environments before this!
	// look for unknown environment start
	if (regex_search(lpText, lpTextEnd, what, m_regexUnknownEnvStart, nFlags) && IsCmdAt(lpText, what[0].first - lpText))
	{
		//Parse string before occurrence
		ParseString(lpText, what[0].first - lpText, cookies,
		            strActualFile, nActualLine, nFileDepth, aSI);

		//Get the name of the environment
		CString strEnvName(what[1].first, what[1].second - what[1].first);

		//Add unknown environment to collection
		INITIALIZE_SI(si);
		cookie.nCookieType = si.m_nType = StructureItem::unknownEnv;
		si.m_strTitle = strEnvName; //Misuse the title for saving the environment name
		cookie.nItemIndex = aSI.size();
		aSI.push_back(si);
		cookies.push(cookie);

		//Parse string behind occurrence
		ParseString(what[0].second, lpTextEnd - what[0].second,
		            cookies, strActualFile, nActualLine, nFileDepth, aSI);

		return;
	}
	// look for headers
	if (regex_search(lpText, lpTextEnd, what, m_regexHeader, nFlags) && IsCmdAt(lpText, what[0].first - lpText))
	{
		// parse string before occurrence
		ParseString(lpText, what[0].first - lpText, cookies,
		            strActualFile, nActualLine, nFileDepth, aSI);

		// if the top of the stack is a header, then remove it
		if (!cookies.empty() && cookies.top().nCookieType == StructureItem::header)
			cookies.pop();

		// initialize structure
		INITIALIZE_SI(si);

		// get header depth (to get parent item)
		nTypeStart = what[1].first - lpText;
		nTypeCount = what[1].second - what[1].first;
		strHeaderType = lpText;
		strHeaderType = strHeaderType.Mid(nTypeStart, nTypeCount);

		bool stop = false;

		for (m_nDepth = 0; m_nDepth < MAX_DEPTH && !stop; ) {
			for (int i = 0; i < HeaderVariationsCount && !m_astrHeader[m_nDepth][i].IsEmpty() && !stop ; ++i)
				if (m_astrHeader[m_nDepth][i] == strHeaderType)
					stop = true;

			if (!stop)
				++m_nDepth;
		}

		// get parent
		if (m_nDepth < 1 || m_nDepth >= MAX_DEPTH)
			si.m_nParent = -1;
		else {
			StructureItemContainer::difference_type parent = m_anItem[m_nDepth - 1];

			if (parent == -1 && !aSI.empty()) {
				using namespace std::tr1::placeholders;
				// Current header doesn't have a parent: Find previously inserted header item
				// and check whether is its depth
				StructureItemContainer::reverse_iterator it = std::find_if(aSI.rbegin(), aSI.rend(),
					std::tr1::bind(std::tr1::bind(std::equal_to<int>(),std::tr1::bind(&StructureItem::GetType,_1),
						static_cast<int>(StructureItem::header)), _1));

				if (it != aSI.rend()) 
				{
					const StructureItem& lastSi = *it;
					int currentDepth = m_nDepth - 1; // Depth of the current structure item
					
					if (lastSi.GetType() == StructureItem::header && lastSi.GetDepth() + 1 < currentDepth) 
					{
						// Last inserted structure item is a header and a gap is in the structure
						StructureItem ghostSi = si;
						ghostSi.RemoveLabels();
						ghostSi.SetType(StructureItem::header);
						ghostSi.SetMissing(); // All following sections are missing
						ghostSi.SetParent(std::distance(aSI.begin(), it.base()) - 1);

						int ghostDepth = lastSi.GetDepth();
						const CString title(MAKEINTRESOURCE(IDS_MISSING_SECTION));

						while (++ghostDepth < currentDepth) 
						{
							// Insert missing sections between last inserted structure item
							// and the current one. Use a copy of the current structure
							// item so the user can jump to the location the missing headers
							// need to be inserted at.
							ghostSi.SetDepth(ghostDepth);							
							ghostSi.SetTitle(title);
							aSI.push_back(ghostSi);

							ghostSi.SetParent(aSI.size() - 1);

							COutputInfo info;
							INITIALIZE_OI(info);
							info.SetErrorMessage(title);
							m_pParseOutputHandler->OnParseLineInfo(info, nFileDepth, CParseOutputHandler::warning);
						}

						parent = aSI.size() - 1;
					}
				}
			}
			
			si.m_nParent = parent;
		}

		// get title
		nTitleStart = what[2].first - lpText;
		nTitleCount = what[2].second - what[2].first;

		CString strFullMatch(what[2].first, nTitleCount);

		if (strFullMatch[0] == _T('['))
			si.m_strTitle = GetArgument(strFullMatch, _T('['), _T(']'));
		else
			si.m_strTitle = GetArgument(strFullMatch, _T('{'), _T('}'));

		si.m_nType = StructureItem::header;
		si.SetDepth(m_nDepth - 1);
		cookie.nItemIndex = m_anItem[m_nDepth] = cookie.nItemIndex = aSI.size();
		aSI.push_back(si);
		cookie.nCookieType = StructureItem::header;
		// TODO: Causes mismatched \begin{document}
		//cookies.push(cookie);

		// parse string behind occurrence
		int nEnd = nTitleStart + nTitleCount;
		nEnd = nEnd - (strFullMatch.GetLength() - si.m_strTitle.GetLength()) + 2;
		ParseString(lpText + nEnd, lpTextEnd - (lpText + nEnd),
		            cookies, strActualFile, nActualLine, nFileDepth, aSI);

		return;
	}

	// find label
	if (regex_search(lpText, lpTextEnd, what, m_regexLabel, nFlags) && IsCmdAt(lpText, what[0].first - lpText))
	{
		// parse string before occurrence
		ParseString(lpText, what[0].first - lpText, cookies,
		            strActualFile, nActualLine, nFileDepth, aSI);

		StructureItem* item;

		if (!cookies.empty() && (aSI.empty() || !aSI.back().IsHeader()))
			item = &aSI[cookies.top().nItemIndex];
		else if (!aSI.empty())
			item = &aSI.back();
		else item = 0;
		
		if (item)
			item->AddLabel(CString(what[1].first, what[1].second - what[1].first));

		// parse string behind occurrence
		ParseString(what[0].second, lpTextEnd - what[0].second, cookies,
		            strActualFile, nActualLine, nFileDepth, aSI);

		return;
	}

	// Find \printindex
	if (regex_search(lpText, lpTextEnd, what, index_, nFlags) && IsCmdAt(lpText, what[0].first - lpText))
	{
		// parse string before occurrence
		ParseString(lpText, what[0].first - lpText, cookies,
			strActualFile, nActualLine, nFileDepth, aSI);

		StructureItem si;
		INITIALIZE_SI(si);

		si.SetParent(-1);
		si.SetType(StructureItem::header);
		si.SetTitle(CString(MAKEINTRESOURCE(IDS_INDEX)));

		aSI.push_back(si);

		// parse string behind occurrence
		ParseString(what[0].second, lpTextEnd - what[0].second, cookies,
			strActualFile, nActualLine, nFileDepth, aSI);

		return;
	}

	// Find print nomencl
	if (regex_search(lpText, lpTextEnd, what, nomencl_, nFlags) && IsCmdAt(lpText, what[0].first - lpText))
	{
		// parse string before occurrence
		ParseString(lpText, what[0].first - lpText, cookies,
			strActualFile, nActualLine, nFileDepth, aSI);

		StructureItem si;
		INITIALIZE_SI(si);

		si.SetParent(-1);
		si.SetType(StructureItem::header);
		si.SetTitle(CString(MAKEINTRESOURCE(IDS_NOMENCLATURE)));

		aSI.push_back(si);

		// parse string behind occurrence
		ParseString(what[0].second, lpTextEnd - what[0].second, cookies,
			strActualFile, nActualLine, nFileDepth, aSI);

		return;
	}

	// find caption
	if (regex_search(lpText, lpTextEnd, what, m_regexCaption, nFlags) && IsCmdAt(lpText, what[0].first - lpText))
	{
		// parse string before occurrence
		ParseString(lpText, what[0].first - lpText, cookies, strActualFile,
		            nActualLine, nFileDepth, aSI);

		if (!cookies.empty())
		{
			CString strFullMatch(what[1].first, what[1].second - what[1].first);
			if (strFullMatch[0] == _T('['))
				aSI[cookies.top().nItemIndex].m_strCaption = GetArgument(strFullMatch, _T('['), _T(']'));
			else
				aSI[cookies.top().nItemIndex].m_strCaption = GetArgument(strFullMatch, _T('{'), _T('}'));
		}

		// parse string behind occurrence
		ParseString(what[0].second, lpTextEnd - what[0].second, cookies,
		            strActualFile, nActualLine, nFileDepth, aSI);

		return;
	}

	// find end of StructureItem::figure
	if (regex_search(lpText, lpTextEnd, what, m_regexFigureEnd, nFlags) && IsCmdAt(lpText, what[0].first - lpText))
	{
		// parse string before occurrence
		ParseString(lpText, what[0].first - lpText, cookies, strActualFile,
		            nActualLine, nFileDepth, aSI);

		// pop StructureItem::figure from stack
		if (!cookies.empty() && cookies.top().nCookieType == StructureItem::figure)
		{
			StructureItem &si = aSI[cookies.top().nItemIndex];
			cookies.pop();
			CreateDefaultTitle(si);
		}
		else if (m_pParseOutputHandler && !m_bCancel)
		{
			COutputInfo info;
			INITIALIZE_OI(info);

			CString message;
			message.Format(STE_PARSE_FOUND_UNMATCHED, m_sItemNames[StructureItem::figure]);

			info.SetErrorMessage(message);
			m_pParseOutputHandler->OnParseLineInfo(info, nFileDepth, CParseOutputHandler::warning);
		}

		// parse string behind occurrence
		ParseString(what[0].second, lpTextEnd - what[0].second, cookies,
		            strActualFile, nActualLine, nFileDepth, aSI);

		return;
	}
	// find end of table
	if (regex_search(lpText, lpTextEnd, what, m_regexTableEnd, nFlags) && IsCmdAt(lpText, what[0].first - lpText))
	{
		// parse string before occurrence
		ParseString(lpText, what[0].first - lpText, cookies, strActualFile,
		            nActualLine, nFileDepth, aSI);

		// pop StructureItem::figure from stack
		if (!cookies.empty() && cookies.top().nCookieType == StructureItem::table)
		{
			StructureItem &si = aSI[cookies.top().nItemIndex];
			cookies.pop();
			CreateDefaultTitle(si);
		}
		else if (m_pParseOutputHandler && !m_bCancel)
		{
			COutputInfo info;
			INITIALIZE_OI(info);

			CString message;
			message.Format(STE_PARSE_FOUND_UNMATCHED, m_sItemNames[StructureItem::table]);

			info.SetErrorMessage(message);
			m_pParseOutputHandler->OnParseLineInfo(info, nFileDepth, CParseOutputHandler::warning);
		}

		// parse string behind occurrence
		ParseString(what[0].second, lpTextEnd - what[0].second, cookies,
		            strActualFile, nActualLine, nFileDepth, aSI);

		return;
	}
	// find end of equation
	if (regex_search(lpText, lpTextEnd, what, m_regexEquationEnd, nFlags) && IsCmdAt(lpText, what[0].first - lpText))
	{
		// parse string before occurrence
		ParseString(lpText, what[0].first - lpText, cookies, strActualFile,
		            nActualLine, nFileDepth, aSI);

		// pop equation from stack
		if (!cookies.empty() && cookies.top().nCookieType == StructureItem::equation)
		{
			StructureItem &si = aSI[cookies.top().nItemIndex];
			cookies.pop();
			CreateDefaultTitle(si);
		}
		else if (m_pParseOutputHandler && !m_bCancel)
		{
			COutputInfo info;
			INITIALIZE_OI(info);

			CString message;
			message.Format(STE_PARSE_FOUND_UNMATCHED, m_sItemNames[StructureItem::equation]);

			info.SetErrorMessage(message);
			m_pParseOutputHandler->OnParseLineInfo(info, nFileDepth, CParseOutputHandler::warning);
		}

		// parse string behind occurrence
		ParseString(what[0].second, lpTextEnd - what[0].second,
		            cookies, strActualFile, nActualLine, nFileDepth, aSI);

		return;
	}

	// find end of center
	if (regex_search(lpText, lpTextEnd, what, m_regexCenterEnd, nFlags) && IsCmdAt(lpText, what[0].first - lpText))
	{
		// parse string before occurrence
		ParseString(lpText, what[0].first - lpText, cookies,
		            strActualFile, nActualLine, nFileDepth, aSI);
		// parse string behind occurrence
		ParseString(what[0].second, lpTextEnd - what[0].second, cookies,
		            strActualFile, nActualLine, nFileDepth, aSI);

		return;
	}

	//ATTENTION: Insert the end of other (known) environments before this!
	// find end of unknown environment
	if (regex_search(lpText, lpTextEnd, what, m_regexUnknownEnvEnd, nFlags) && IsCmdAt(lpText, what[0].first - lpText))
	{
		//Parse string before occurrence
		ParseString(lpText, what[0].first - lpText, cookies, strActualFile,
		            nActualLine, nFileDepth, aSI);

		//Get the name of the environment
		CString strEnvName(what[1].first, what[1].second - what[1].first);

		//Pop unknown environment from stack
		if (!cookies.empty()
			&& (cookies.top().nCookieType == StructureItem::unknownEnv)
		        && (aSI[cookies.top().nItemIndex].m_strTitle == strEnvName))
		{
			StructureItem &si = aSI[cookies.top().nItemIndex];
			cookies.pop();

			if (si.m_strCaption.IsEmpty() && !si.HasLabels())
			{
				si.m_strTitle.Format(_T("%s: %s (%d)"), strEnvName,
				                     ResolveFileName(si.m_strPath), si.m_nLine);
			}
			else
			{
				if (si.m_strCaption.IsEmpty())
					si.SetTitle(strEnvName + _T(": ") + si.GetLabel());
				else
					si.SetTitle(strEnvName + _T(": ") + si.GetLabel());
			}
		}
		else if (m_pParseOutputHandler && !m_bCancel)
		{
			COutputInfo info;
			INITIALIZE_OI(info);

			CString message;
			message.Format(STE_PARSE_FOUND_UNMATCHED, strEnvName);

			info.SetErrorMessage(message);
			m_pParseOutputHandler->OnParseLineInfo(info, nFileDepth, CParseOutputHandler::warning);
		}

		// parse string behind occurrence
		ParseString(what[0].second, lpTextEnd - what[0].second, cookies,
		            strActualFile, nActualLine, nFileDepth, aSI);

		return;
	}

	// Find appendix
	if (regex_search(lpText, lpTextEnd, what, m_regexAppendix, nFlags) && IsCmdAt(lpText, what[0].first - lpText))
	{
		// parse string before occurrence
		ParseString(lpText, what[0].first - lpText, cookies, strActualFile,
		            nActualLine, nFileDepth, aSI);

		// Reset the headers
		//EmptyCookieStack(cookies, aSI);
		m_nDepth = 1;

		// add appendix to collection
		INITIALIZE_SI(si);
		si.m_strTitle.LoadString(STE_APPENDIX);
		si.m_nParent = -1;
		cookie.nCookieType = si.m_nType = StructureItem::header;
		cookie.nItemIndex = m_anItem[m_nDepth] = aSI.size();
		aSI.push_back(si);
		// cookies.push(cookie);

		// parse string behind occurrence
		ParseString(what[0].second, lpTextEnd - what[0].second, cookies,
		            strActualFile, nActualLine, nFileDepth, aSI);
		return;
	}

	// Find bibliography
	if (regex_search(lpText, lpTextEnd, what, m_regexBib, nFlags) && IsCmdAt(lpText, what[0].first - lpText))
	{
		// parse string before occurrence
		ParseString(lpText, what[0].first - lpText, cookies, strActualFile,
		            nActualLine, nFileDepth, aSI);

		const std::size_t prefixIndex = 2;

		const CString prefix(what.str(prefixIndex).c_str());

		const std::size_t pathIndex = 3;
		// parse input file
		CString bibPath(what.str(pathIndex).c_str());

		int nStart = 0;
		int nFound;

		while (true)
		{
			nFound = bibPath.Find(_T(','), nStart);
			if (nFound == -1)
				if (nStart >= bibPath.GetLength())
					// Done
					break;
				else
					// Extract last comma separated element from list
					nFound = bibPath.GetLength();

			CString strPath = Unescape(bibPath.Mid(nStart, nFound - nStart));

			strPath.Trim();
			strPath.Trim(_T('"'));

			//BibTeX takes only the .bib-Extension!
			// And in \bibliography{...} the user has to omit the extension!
			// We could warn the user, if he uses an extension in \bibliography{...}
			strPath += _T(".bib");
			strPath = ResolveFileName(strPath);

			COutputInfo info;
			INITIALIZE_OI(info);

			if (::PathFileExists(strPath))
			{
				StructureItemContainer::iterator it = FindItemByPath(aSI.begin(), aSI.end(), strPath);

				if (it != aSI.end())
				{
					StructureItemContainer::difference_type original = std::distance(aSI.begin(), it);

					StructureItem copy = *it;
					copy.SetCopy();
					copy.SetOriginal(original);
					copy.SetPrefix(prefix);
					copy.SetLine(nActualLine);

					aSI.push_back(copy);
				}
				else
				{
					//Give information that we found the bibfile and that we are going to parse it now.
					if (m_pParseOutputHandler && !m_bCancel)
					{
						CString message;
						message.Format(STE_PARSE_PARSING, strPath);

						info.SetErrorMessage(message);
						m_pParseOutputHandler->OnParseLineInfo(info, nFileDepth,
															   CParseOutputHandler::information);
					}

					//Now we parse the bibtex file with the bibtex parser.
					//The bibtex parser collects all items and warnings
					//and will add it to the structure and parse-output after its work.
					BibTeXFile aBibFile(strPath);
					aBibFile.DropAllEntries(); // clean up previous result
					aBibFile.ProcessFile(); // parse it ...
					const CMapStringToOb *items = aBibFile.GetEntries(); // ...and fetch the entries.

					//Add the bibfile itself to the structure
					CString strAnnotation;
					strAnnotation.Format(STE_ENTRIES_COUNT, aBibFile.GetEntriesCount());

					if (aBibFile.GetErrorCount())
					{
						CString strWarnings;
						strWarnings.Format(STE_WARNING_COUNT, aBibFile.GetErrorCount());
						strAnnotation += _T(", ");
						strAnnotation += strWarnings;
					}

					StructureItemContainer::size_type parentPos =
						AddFileItem(strPath, StructureItem::bibFile, strActualFile, nActualLine, aSI, strAnnotation, 
							prefix.IsEmpty() ? NULL : prefix);

					//Add the parser warnings, if any.
					if (aBibFile.GetErrorCount())
					{
						//Write msgs to output window, if we detected some errors
						const CObArray *msgs = aBibFile.GetErrorMessages();

						for (int i = 0; i < msgs->GetSize(); i++)
						{
							BibTeXEntry *be = dynamic_cast<BibTeXEntry*>(msgs->GetAt(i));
							if (be == NULL)
							{
								TRACE("NP found in error msgs of BibTex file\n");
								continue;
							}

							info.SetErrorMessage(be->m_strTitle);
							info.SetSourceLine(be->m_nLine);
							info.SetSourceFile(be->m_strPath);
							m_pParseOutputHandler->OnParseLineInfo(info, nFileDepth + 1,
																   CParseOutputHandler::warning);
						}
					}

					//Iterate over entry map and add the collected bibitems
					POSITION pos = items->GetStartPosition();

					while (pos != NULL)
					{
						BibTeXEntry *be;
						CString key;
						items->GetNextAssoc(pos, key, (CObject*&)be);

						if (be != NULL)   // setup entry
						{
							be->SetParent(parentPos);
							cookie.nCookieType = be->GetType();
							//TRACE("Added si: %s, %s, %s\n", be->m_strPath, be->m_strLabel, be->m_strCaption);
							cookie.nItemIndex = aSI.size();

							StructureItem item(*be);
							item.SetItemInfo(new BibItem(be->ToBibItem()));

							aSI.push_back(item);
							//cookies.push(cookie);
						}
						else
						{
							TRACE("NP detected in CBiBTeXFile %s", strPath);
						}
					}
				}
			}
			else
			{
				if (m_pParseOutputHandler && !m_bCancel)
				{
					AddFileItem(strPath, StructureItem::missingBibFile, strActualFile, nActualLine, aSI, NULL, 
						prefix.IsEmpty() ? NULL : prefix);

					CString message;
					message.Format(STE_FILE_EXIST, strPath);

					info.SetErrorMessage(message);

					m_pParseOutputHandler->OnParseLineInfo(info, nFileDepth,
					                                       CParseOutputHandler::warning);
				}
			}

			nStart = nFound + 1;
		}

		// parse string behind occurrence
		ParseString(what[0].second, lpTextEnd - what[0].second, cookies,
		            strActualFile, nActualLine, nFileDepth, aSI);

		return;
	}

}

CString CStructureParser::ResolveFileName(LPCTSTR lpszPath) const
{
	// format file name (remove path, if identical with working dir
	CString strActualFile(lpszPath);

	if (!m_strWorkingDir.CompareNoCase(CPathTool::Format(_T("%d"), strActualFile)))
		strActualFile = CPathTool::Format(_T("%n"), strActualFile);
	return (strActualFile);
}

StructureItemContainer::size_type CStructureParser::AddFileItem(LPCTSTR lpszPath, int nType, LPCTSTR lpszIncludeFromFile,
                                  int nIncludedFileLineNumber, StructureItemContainer &aSI,
                                  LPCTSTR lpszAnnotation /*= NULL*/, LPCTSTR prefix)
{
	// insert file into item-array
	StructureItem si;
	si.m_nLine = nIncludedFileLineNumber;
	si.m_nParent = m_anItem[m_nDepth];
	si.m_nType = nType;
	si.m_strComment = lpszIncludeFromFile;
	si.m_strPath = lpszPath;
	
	if (prefix)
		si.SetPrefix(prefix);

	if (lpszAnnotation == NULL)
	{
		//Displayed title will be the path
		si.m_strTitle = lpszPath;
	}
	else
	{
		//Displayed title with path and annotation
		si.m_strTitle.Format(_T("%s (%s)"), lpszPath, lpszAnnotation);
	}

	// Look for item with the same file path and prefix

	StructureItemContainer::iterator it = 
		std::find_if(aSI.begin(),aSI.end(), ItemMatch(si.GetPath(), si.GetPrefix()));

	StructureItemContainer::size_type pos;

	if (it != aSI.end()) // Found it, return the position
		pos = std::distance(aSI.begin(),it);
	else   // Nope, we don't have it yet; insert it
	{
		pos = aSI.size();
		aSI.push_back(si);
	}

	return pos;
}

void CStructureParser::EmptyCookieStack(CCookieStack &cookies, StructureItemContainer &aSI)
{
	if (m_pParseOutputHandler && !m_bCancel)
	{
		while (!cookies.empty())
		{
			COOKIE item = cookies.top();
			cookies.pop();
			COutputInfo info;
			StructureItem &si = aSI[item.nItemIndex];

			if (StructureItem::IsEnvironment(static_cast<StructureItem::Type>(item.nCookieType)))
			{
				info.SetSourceLine(si.m_nLine);
				info.SetSourceFile(si.m_strPath);

				CString message;
				message.Format(STE_PARSE_FOUND_UNMATCHED, aSI[item.nItemIndex].GetTitle());

				info.SetErrorMessage(message);

				m_pParseOutputHandler->OnParseLineInfo(info, 0, CParseOutputHandler::warning);
			}
		}
	}
	else
	{
		while (!cookies.empty())
			cookies.pop();
	}
}

void CStructureParser::Done(bool bParsingResult)
{
	m_pStructureParserHandler->OnParsingFinished(bParsingResult);

	if (m_pParseOutputHandler && !m_bCancel)
		m_pParseOutputHandler->OnParseEnd(bParsingResult, m_nFilesParsed, m_nLinesParsed);
}

BOOL CStructureParser::Parse(LPCTSTR lpszPath, CCookieStack &cookies,
                             int nFileDepth, StructureItemContainer &aSI)
{
	std::auto_ptr<CTextSourceFile> pTs(new CTextSourceFile);

	if (!pTs->Create(lpszPath))
	{
		pTs.reset();
	}

	if (!pTs.get())
	{
		if (m_pParseOutputHandler && !m_bCancel)
		{
			COutputInfo info;
			info.SetSourceFile(lpszPath);
			info.SetSourceLine();

			CString message;
			message.Format(STE_GREP_ERROR, lpszPath);
			
			info.SetErrorMessage(message);

			m_pParseOutputHandler->OnParseLineInfo(info, nFileDepth, CParseOutputHandler::warning);
		}
		return FALSE;
	}

	m_nFilesParsed++;

	CString strActualFile(lpszPath);
	AddFileItem(strActualFile, StructureItem::texFile, _T(""), -1, aSI);

	// parse text source
	LPCTSTR lpLine, lpLineEnd, lpOffset;
	int nLength;
	std::tr1::regex_constants::match_flag_type nFlags = std::tr1::regex_constants::match_default;
	std::tr1::match_results<LPCTSTR> what;
	BOOL bVerbatim = FALSE;
	int nActualLine = 0;
	const tregex* current_verb_end = 0;

	while (!m_bCancel && pTs->GetNextLine(lpLine, nLength))
	{
		m_nLinesParsed++;
		m_nCharsParsed += nLength;
		nActualLine++;
		lpOffset = lpLine;
		lpLineEnd = lpLine + nLength;

		if (!bVerbatim)
		{
			// find comments
			lpOffset = lpLine;

			while (lpOffset < lpLineEnd && regex_search(lpOffset, lpLineEnd, what, m_regexComment, nFlags))
			{
				if (IsCmdAt(lpLine, what[0].first - lpLine - 1))
					lpOffset = what[0].second;
				else
				{
					lpLineEnd = what[0].first;
					break;
				}
			}

			// Verbatim environments: verbatim, lstlisting
			const tregex* verb_start[] = {&m_regexVerbStart,&listing_start_};
			const int count = sizeof(verb_start) / sizeof(*verb_start);
			bool stop = false;

			for (int i = 0; i < count && !stop; ++i) {
				// find start of verbatim
				if (regex_search(lpLine, lpLineEnd, what, *verb_start[i]))
				{
					stop = true;

					// Set the appropriate environment end regex
					if (verb_start[i] == &m_regexVerbStart)
						current_verb_end = &m_regexVerbEnd;
					else if (verb_start[i] == &listing_start_)
						current_verb_end = &listing_end_;
					else
						ASSERT(FALSE);

					if (IsCmdAt(lpLine, what[0].first - lpLine))
					{
						// parse line to beginning of \begin{verbatim}-command
						ParseString(lpLine, what[0].first - lpLine, cookies,
									strActualFile, nActualLine, nFileDepth, aSI);
						bVerbatim = TRUE;

						// set line start to end of \begin{verbatim}-command
						lpLine = what[0].second;
					}
				}
			}
		}

		// find end of verbatim
		if (bVerbatim)
		{
			ASSERT(current_verb_end);

			if (!regex_search(lpLine, lpLineEnd, what,*current_verb_end))
				continue; // continue with next line

			// we found a verbatim end, set line start to end of \end{verbatim}-command
			lpLine = what[0].second;
			bVerbatim = FALSE;
			current_verb_end = 0;
		}

		// find inline verbatim
		lpOffset = lpLine;

		while (lpOffset < lpLineEnd && regex_search(lpOffset, lpLineEnd, what, m_regexInlineVerb, nFlags))
		{
			if (!IsCmdAt(lpLine, what[0].first - lpLine))
			{
				lpOffset = what[0].first + 1;
				continue;
			}

			// parse before verbatim
			ParseString(lpOffset, what[0].first - lpOffset, cookies, strActualFile,
			            nActualLine, nFileDepth, aSI);
			lpOffset = what[0].second;
		}

		// parse rest of line
		if (lpOffset < lpLineEnd)
			ParseString(lpOffset, lpLineEnd - lpOffset, cookies, strActualFile,
			            nActualLine, nFileDepth, aSI);
	}

	// delete text source object
	pTs.release()->Delete();

	return !m_bCancel;
}

void CStructureParser::CancelParsing()
{
	m_bCancel = TRUE;
}

const CString CStructureParser::Unescape(const CString &tmp )
{
	CString strPath;

	int pos = 0;

	while (pos < tmp.GetLength()) 
	{
		if (tmp[pos] == _T('\\')) {
			++pos;

			while (pos < tmp.GetLength() && CharTraitsT::IsAlnum(tmp[pos]))
				++pos;
		}

		if (pos < tmp.GetLength())
			strPath.AppendChar(tmp[pos]);

		++pos;
	}

	return strPath;
}

void CStructureParser::CreateDefaultTitle( StructureItem& si,bool bForce /*= false*/ )
{
	if (si.m_strTitle.IsEmpty() || bForce)
	{
		if (si.m_strCaption.IsEmpty() && !si.HasLabels())
		{
			//Label and Caption empty ==> Generate a title from the filename
			si.m_strTitle.Format(_T("%s (%d)"),ResolveFileName(si.m_strPath),si.m_nLine);
		}
		else
		{
			if (si.GetCaption().IsEmpty())
				si.SetTitle(si.GetLabel());
			else
				si.SetTitle(si.GetCaption());
		}
	}
}
void CParseOutputHandler::OnParseLineInfo( COutputInfo &/*line*/, int /*nLevel*/, int /*nSeverity*/ )
{

}

void CParseOutputHandler::OnParseBegin( bool /*bCancelState*/ )
{

}

void CParseOutputHandler::OnParseEnd( bool /*bResult*/,int /*nFiles*/,int /*nLines*/ )
{

}