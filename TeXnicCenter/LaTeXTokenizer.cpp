#include "stdafx.h"
#include "LaTeXTokenizer.h"

#include <algorithm>

#include "configuration.h"
#include "CharType.h"

#define IGNORE_WORDS_IN_BRACES_ 0 // TODO: Doesn't work properly yet: many constructs may span over multiple lines
								  // which isn't supported

LaTeXTokenizer::LaTeXTokenizer()
{
	ResetState();
}

bool LaTeXTokenizer::NextWord(LPCTSTR szLine, int length, int &nStartPos, int &nEndPos)
{
	int nLength = length;
	bool badWord = true;
	TBYTE ch;
	
#if IGNORE_WORDS_IN_BRACES_

	if (nStartPos == 0)
		braces_.clear(); // For now, the processing is done line by line

	const TCHAR opening_braces[] = _T("{[$");
	const std::size_t braces = sizeof(opening_braces) / sizeof(*opening_braces);
#endif // IGNORE_WORDS_IN_BRACES_

	while (nStartPos != -1 && badWord)
	{
		badWord = false;

		while (nStartPos < nLength)
		{
			ch = szLine[nStartPos];

#if IGNORE_WORDS_IN_BRACES_
			
			if (!braces_.empty() && GetClosingBrace(braces_.back()) == ch)
				braces_.pop_back();
			// If we encounter an opening brace, save the state
			else if (std::find_first_of(&ch,&ch + 1,opening_braces,opening_braces + braces) != &ch + 1)
				braces_.push_back(ch);

			// The text is not enclosed by braces, process it
			if (braces_.empty()) {
#endif // IGNORE_WORDS_IN_BRACES_
				// words begin with an alpha character
				if (CharTraitsT::IsAlpha(ch))
					break;

				// if we're skipping words with numbers, words may begin with a number
				if (CConfiguration::GetInstance()->m_bSpellSkipNumbers && CharTraitsT::IsDigit(ch))
				{
					badWord = true;
					break;
				}

				// Skip comments?
				if (CConfiguration::GetInstance()->m_bSpellSkipComments && ch == _T('%') && 
					(nStartPos == 0 || szLine[nStartPos - 1] != _T('\\'))) // Handle \% which is not a comment
				{
					nStartPos = -1;
					return false;
				}
#if IGNORE_WORDS_IN_BRACES_
			}
#endif // IGNORE_WORDS_IN_BRACES_
			// otherwise ignore it

			++nStartPos;
		}

		if (!badWord && nStartPos > 0 && CConfiguration::GetInstance()->m_bSpellSkipTags)
			badWord = IsCmdAt(szLine,&szLine[nStartPos - 1]) != 0;

		nEndPos = nStartPos + 1;

		while (nEndPos < nLength)
		{
			ch = szLine[nEndPos];

			if (!badWord && CConfiguration::GetInstance()->m_bSpellSkipNumbers && CharTraitsT::IsDigit(ch))
				badWord = true;

			// words end on terminating character
			if (!CharTraitsT::IsAlnum(ch) && ch != _T('\'')) //support don't as well as quell'altro sull'esperimento
				break;

			++nEndPos;
		}

		if (!(nStartPos < nLength))
		{
			// We have reached the end of the line
			nStartPos = -1;
		}
		else
		{
			if (!badWord && CConfiguration::GetInstance()->m_bSpellSkipCaps)
			{
				badWord = true;

				for (int i = nStartPos; i < nEndPos; ++i)
				{
					if (CharTraitsT::IsLower(szLine[i]))
					{
						badWord = false;
						break;
					}
				}
			}

			if (badWord)
				nStartPos = nEndPos;
		}
	}

	return nStartPos != nEndPos && nStartPos != -1 && nEndPos != -1;
}

bool LaTeXTokenizer::IsCmdAt(LPCTSTR lpText, LPCTSTR lpPos)
{
	// count number of backslashes before command (including command backslash)
	int nCount = 0;

	for  (; lpPos >= lpText && *lpPos == _T('\\'); nCount++, lpPos--)
		;

	return !(nCount % 2 == 0);
}

void LaTeXTokenizer::ResetState()
{
	braces_.clear();
}

TCHAR LaTeXTokenizer::GetClosingBrace( TCHAR open )
{
	TCHAR result;

	switch (open) {
		case _T('{'): result = _T('}'); break;
		case _T('('): result = _T(')'); break;
		case _T('['): result = _T(']'); break;
		case _T('$'): result = open; break;
		default: result = 0;
	}

	return result;
}