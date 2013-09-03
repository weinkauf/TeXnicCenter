// Scintilla source code edit control

// File: LexTeX.cxx - general context conformant tex coloring scheme
// Author: Hans Hagen - PRAGMA ADE - Hasselt NL - www.pragma-ade.com
// Version: September 28, 2003

// Copyright: 1998-2003 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

// This lexer is derived from the one written for the texwork environment (1999++) which in
// turn is inspired on texedit (1991++) which finds its roots in wdt (1986).

// If you run into strange boundary cases, just tell me and I'll look into it.


// TeX Folding code added by instanton (soft_share@126.com) with borrowed code from VisualTeX source by Alex Romanenko.
// Version: June 22, 2007

// Modifications for the TeXnicCenter Project by Sergiu Dotenco
// Version: May 4, 2009

#include <algorithm>
#include <functional>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <cctype>

#include "ILexer.h"
#include "Scintilla.h"
#include "SciLexer.h"

#include "PropSetSimple.h"
#include "WordList.h"
#include "LexAccessor.h"
#include "Accessor.h"
#include "StyleContext.h"
#include "CharacterSet.h"
#include "LexerModule.h"

#ifdef SCI_NAMESPACE
using namespace Scintilla;
#endif

// val SCE_TEX_DEFAULT = 0
// val SCE_TEX_SPECIAL = 1
// val SCE_TEX_GROUP   = 2
// val SCE_TEX_SYMBOL  = 3
// val SCE_TEX_COMMAND = 4
// val SCE_TEX_TEXT    = 5

// Definitions in SciTEGlobal.properties:
//
// TeX Highlighting
//
// # Default
// style.tex.0=fore:#7F7F00
// # Special
// style.tex.1=fore:#007F7F
// # Group
// style.tex.2=fore:#880000
// # Symbol
// style.tex.3=fore:#7F7F00
// # Command
// style.tex.4=fore:#008800
// # Text
// style.tex.5=fore:#000000

// lexer.tex.interface.default=0
// lexer.tex.comment.process=0

// todo: lexer.tex.auto.if

// Auxiliary functions:
namespace {
#pragma region Auxiliary functions

	template<class Predicate>
	bool SkipWhileCurrent( StyleContext &sc, Predicate pred)
	{
		while (sc.More() && pred(sc.ch))
			sc.Forward();

		return pred(sc.ch);
	}

	template<class Predicate>
	bool SkipWhileNext( StyleContext &sc, Predicate pred)
	{
		while (sc.More() && pred(sc.chNext))
			sc.Forward();

		return pred(sc.chNext);
	}

	template<class Predicate>
	bool SkipWhileContext( StyleContext &sc, Predicate pred)
	{
		while (sc.More() && pred(sc))
			sc.Forward();

		return pred(sc);
	}

	bool SkipSpaces( StyleContext& sc )
	{
		return SkipWhileCurrent(sc,std::bind2nd(std::equal_to<char>(),' '));
	}

	bool SkipToOpenBrace( StyleContext& sc )
	{
		return SkipWhileCurrent(sc,std::bind2nd(std::not2(std::equal_to<char>()),'{'));
	}

	bool EndOfLine(Accessor &styler, unsigned int i)
	{
		return (styler[i] == '\n') || ((styler[i] == '\r') && (styler.SafeGetCharAt(i + 1) != '\n'));
	}

	bool IsComment(int ch)
	{
		return ch == '%';
	}

	bool IsFileInclusion(const char* key)
	{
		return (std::strcmp(key, "usepackage") == 0
			||  std::strcmp(key, "input") == 0
			||  std::strcmp(key, "input*") == 0
			||  std::strcmp(key, "include") == 0
			||  std::strcmp(key, "include*") == 0
			||  std::strcmp(key, "bibliography") == 0
			||  std::strcmp(key, "includegraphics") == 0
			);
	}

	bool IsStyleInclusion(const char* key)
	{
		return (std::strcmp(key, "documentclass") == 0
			||  std::strcmp(key, "bibliographystyle") == 0
			);
	}

	bool isTeXone(int ch)
	{
		return  (ch == '[') || (ch == ']') || (ch == '=') || (ch == '#') ||
			(ch == '(') || (ch == ')') || (ch == '<') || (ch == '>') ||
			(ch == '"');
	}

	bool isWordChar(int ch)
	{
		return ((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z'));
	}

	bool isTeXtwo(int ch)
	{
		return ch == '{' || ch == '}' || ch == '$';
	}

	bool isTeXthree(int ch)
	{
		return (ch == '~') || (ch == '^') || (ch == '_') || (ch == '&') ||
			(ch == '-') || (ch == '+') || (ch == '\"') || (ch == '`') ||
			(ch == '/') || (ch == '|') || (ch == '%');
	}

	bool IsEscape(int ch)
	{
		return ch == '\\';
	}

	bool isTeXfive(int ch)
	{
		return ((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z')) ||
			(ch == '@') || (ch == '!') || (ch == '?');
	}

	bool IsSpace(int ch)
	{
		return ch == ' ';
	}

	bool isTeXseven(int ch)
	{
		return ch == '^';
	}

	bool IsDigit(int ch)
	{
		return ch >= '0' && ch <= '9';
	}

	bool equal_strings(const char* s1, const char* s2)
	{
		return std::strcmp(s1, s2) == 0;
	}

	bool IsCommentLine(int line, Accessor &styler)
	{
		int pos = styler.LineStart(line);
		int eol_pos = styler.LineStart(line + 1) - 1;

		int startpos = pos;

		while (startpos < eol_pos) {
			char ch = styler[startpos];

			if (ch != '%' && ch != ' ') 
				return false;
			else if (ch == '%')
				return true;

			++startpos;
		}

		return false;
	}

	const char* const verbatim[] = {
		"verbatim",
		"listings"
	};

	bool IsVerbatimEnvironment(const char* s)
	{
		const char* const* end = verbatim + sizeof(verbatim) / sizeof(*verbatim);
		return std::find_if(verbatim,end,std::bind2nd(std::ptr_fun(equal_strings),s)) != end;
	}

	bool IsTeXSecond(int ch)
	{
		return std::isprint(ch) != 0;
	}

	const char* const units[] = {
		"em",
		"ex",
		"pt",
		"pc",
		"in",
		"bp",
		"cm",
		"mm",
		"dd",
		"cc",
		"sp"
	};

#pragma endregion

	// Interface determination
	int CheckTeXInterface(unsigned startPos, int length, Accessor &styler, int defaultInterface)
	{
		char lineBuffer[1024];
		unsigned int linePos = 0;

		// some day we can make something lexer.tex.mapping=(all,0)(nl,1)(en,2)...

		if (styler.SafeGetCharAt(0) == '%') {
			for (unsigned int i = 0; i < startPos + length; i++) {
				lineBuffer[linePos++] = styler.SafeGetCharAt(i);

				if (EndOfLine(styler, i) || (linePos >= sizeof (lineBuffer) - 1)) {
					lineBuffer[linePos] = '\0';

					if (std::strstr(lineBuffer, "interface=all")) {
						return 0;
					}
					else if (std::strstr(lineBuffer, "interface=tex")) {
						return 1;
					}
					else if (std::strstr(lineBuffer, "interface=nl")) {
						return 2;
					}
					else if (std::strstr(lineBuffer, "interface=en")) {
						return 3;
					}
					else if (std::strstr(lineBuffer, "interface=de")) {
						return 4;
					}
					else if (std::strstr(lineBuffer, "interface=cz")) {
						return 5;
					}
					else if (std::strstr(lineBuffer, "interface=it")) {
						return 6;
					}
					else if (std::strstr(lineBuffer, "interface=ro")) {
						return 7;
					}
					else if (std::strstr(lineBuffer, "interface=latex")) {
						// we will move latex cum suis up to 91+ when more keyword lists are supported
						return 8;
					}
					else if (styler.SafeGetCharAt(1) == 'D' && std::strstr(lineBuffer, "%D \\module")) {
						// better would be to limit the search to just one line
						return 3;
					}
					else {
						return defaultInterface;
					}
				}
			}
		}

		return defaultInterface;
	}

	void ColouriseTeXDoc(unsigned startPos, int length, int, WordList** keywordlists, Accessor &styler)
	{
		styler.StartAt(startPos);
		styler.StartSegment(startPos);

		bool processComment = styler.GetPropertyInt("lexer.tex.comment.process", 0) == 1;
		bool useKeywords = styler.GetPropertyInt("lexer.tex.use.keywords", 1) == 1;
		bool autoIf = styler.GetPropertyInt("lexer.tex.auto.if", 1) == 1;
		int defaultInterface = styler.GetPropertyInt("lexer.tex.interface.default", 1);

		char key[100];
		const int key_length = sizeof(key) - 1;

		std::size_t k;
		bool newifDone = false;
		bool inComment = false;

		int currentInterface = CheckTeXInterface(startPos, length, styler, defaultInterface);

		if (currentInterface == 0) {
			useKeywords = false;
			currentInterface = 1;
		}

		WordList &keywords = *keywordlists[currentInterface - 1];

		int group_depth = 0;
		bool next_group_name = false;
		int text_style = SCE_TEX_TEXT;
		int line = styler.GetLine(startPos);

		if (line > 0)
			text_style = styler.StyleAt(styler.LineStart(line) - 1);

		int command_style = text_style == SCE_TEX_INLINE_MATH ? SCE_TEX_INLINE_MATH_COMMAND : SCE_TEX_COMMAND;

		StyleContext sc(startPos, length, text_style, styler);

		bool going = sc.More(); // needed because of a fuzzy end of file state

		for (; going; sc.Forward()) {

			if (!sc.More()) {
				going = false;
			} // we need to go one behind the end of text

			if (inComment) {
				if (sc.atLineEnd) {
					sc.SetState(text_style);
					newifDone = false;
					inComment = false;
				}
				else
					sc.SetState(SCE_TEX_COMMENT);
			}
			else {
				if (text_style == SCE_TEX_INLINE_MATH && sc.currentPos > 0 &&
						sc.ch == '$' && (sc.chPrev != '\\' || styler.SafeGetCharAt(sc.currentPos - 2) == '\\')) {
					sc.SetState(SCE_TEX_GROUP);
					text_style = SCE_TEX_TEXT;
					command_style = SCE_TEX_COMMAND;
					sc.ForwardSetState(text_style);
				}

				bool done = false;
				bool reset = false;
				bool cnt = true;

				if (sc.state == SCE_TEX_DIGIT) {
					const int count = sizeof(units) / sizeof(*units);
					bool stop = false;

					for (int i = 0; i < count && !stop; ++i)
						if (sc.Match(units[i]))
							stop = true;

					if (stop) {
						sc.SetState(SCE_TEX_UNIT);
						sc.ForwardSetState(sc.state);
						done = true;
						cnt = false;
					}
				}

				if (cnt) {
					if (!isTeXfive(sc.ch)) {
						if (sc.state == command_style) {
							if (sc.LengthCurrent() == 1) { // \<noncstoken>
								if (isTeXseven(sc.ch) && isTeXseven(sc.chNext)) {
									sc.Forward(2); // \^^ and \^^<token>
								}

								if (!(text_style == SCE_TEX_INLINE_MATH && sc.chNext == '$')) {
									if (!IsDigit(sc.ch))
										sc.ForwardSetState(text_style);									
								}
								else
									done = true;
							}
							else {
								sc.GetCurrent(key, key_length);
								k = std::strlen(key);
								std::memmove(key, key + 1, k); // shift left over escape token
								key[k--] = '\0';

								if (IsFileInclusion(key)) {
									// \usepackage, \include, \input, and so on
									sc.ChangeState(SCE_TEX_FILE_INCLUSION);
									newifDone = false;
								}
								else if (IsStyleInclusion(key)) {
									// \documentclass \bibliographystyle
									sc.ChangeState(SCE_TEX_STYLE_INCLUSION);
									newifDone = false;
								}
								else if (!keywords || !useKeywords) {
									sc.SetState(command_style);
									newifDone = false;
								}
								else if (k == 1) { //\<cstoken>
									sc.SetState(command_style);
									newifDone = false;
								}
								else if (keywords.InList(key)) {
									sc.SetState(command_style);
									newifDone = autoIf && (std::strcmp(key, "newif") == 0);
								}
								else if (autoIf && !newifDone && (key[0] == 'i') && (key[1] == 'f') && keywords.InList("if")) {
									sc.SetState(command_style);
								}
								else {
									sc.ChangeState(text_style);
									sc.SetState(text_style);
									newifDone = false;
								}

								if (std::strcmp(key, "begin") == 0) {
									next_group_name = true;
									++group_depth;
								}
								else if (std::strcmp(key, "end") == 0) {
									next_group_name = true;

									if (group_depth > 0)
										--group_depth;
								}
							}
						}

						if (!done) {
							if (IsComment(sc.ch)) {
								sc.SetState(SCE_TEX_COMMENT);

								inComment = !processComment;
								newifDone = false;
							}
							else if (isTeXseven(sc.ch) && isTeXseven(sc.chNext)) {
								sc.SetState(text_style);
								sc.ForwardSetState(SCE_TEX_TEXT);
							}
							else if (isTeXone(sc.ch)) {
								sc.SetState(SCE_TEX_SPECIAL);
								newifDone = false;
							}
							else if (sc.ch == '$' && text_style != SCE_TEX_INLINE_MATH) {
								sc.SetState(SCE_TEX_GROUP);
								text_style = SCE_TEX_INLINE_MATH;
								command_style = SCE_TEX_INLINE_MATH_COMMAND;

								newifDone = false;
							}
							else if (isTeXtwo(sc.ch)) {
								sc.SetState(SCE_TEX_GROUP);
								newifDone = false;

								if (sc.ch == '{' && next_group_name) {
									sc.ForwardSetState(SCE_TEX_GROUP_NAME);

									SkipWhileNext(sc,std::bind2nd(std::equal_to<char>(),' ')); // while sc.chNext == ' '
									SkipWhileNext(sc,std::bind2nd(std::not2(std::equal_to<char>()),'}')); // while sc.chNext != '}'

									next_group_name = false;
								}
							}
							else if (isTeXthree(sc.ch)) {
								sc.SetState(SCE_TEX_SYMBOL);
								newifDone = false;
							}
							else if (IsEscape(sc.ch)) {
								sc.SetState(command_style);

								if (!IsDigit(sc.chNext) && !IsTeXSecond(sc.chNext))
									sc.ForwardSetState(text_style);
							}
							else if (IsSpace(sc.ch)) {
								reset = true;
							}
							else if (IsDigit(sc.ch)) {
								sc.SetState(SCE_TEX_DIGIT);
							}
							else if (sc.atLineEnd) {
								reset = true;
								newifDone = inComment = false;
							}
							else {
								reset = true;
							}
						}
						else if (sc.state != command_style) {
							reset = true;
						}
					}
					else if (sc.state != command_style) {
						reset = true;
						sc.SetState(text_style);
					}
				}

				if (reset)
					sc.SetState(text_style);
			}
		}

		sc.ChangeState(text_style);
		sc.Complete();
	}

	int ParseTeXCommand(unsigned int pos, Accessor &styler, char *command)
	{
		int length = 0;
		char ch = styler.SafeGetCharAt(pos + 1);

		if (ch == ',' || ch == ':' || ch == ';' || ch == '%') {
			command[0] = ch;
			command[1] = 0;
			return 1;
		}

		// find end
		while (isWordChar(ch) && !IsDigit(ch) && ch != '_' && ch != '.' && length < 100) {
			command[length] = ch;
			++length;
			ch = styler.SafeGetCharAt(pos + length + 1);
		}

		command[length] = '\0';

		if (!length)
			return 0;

		return length + 1;
	}

	int classifyFoldPointTeXPaired(const char* s)
	{
		int lev = 0;

		if (!(std::isdigit(s[0]) || (s[0] == '.'))) {
			if (std::strcmp(s, "begin") == 0 || std::strcmp(s, "FoldStart") == 0 ||
					std::strcmp(s, "begingroup") == 0 ||
					std::strcmp(s, "abstract") == 0 || std::strcmp(s, "unprotect") == 0 ||
					std::strcmp(s, "makeatletter") == 0 ||
					std::strcmp(s, "title") == 0 || std::strncmp(s, "start", 5) == 0 || std::strncmp(s, "Start", 5) == 0 ||
					std::strcmp(s, "documentclass") == 0 || std::strncmp(s, "if", 2) == 0
					)
				lev = 1;
			if (std::strcmp(s, "end") == 0 || std::strcmp(s, "FoldStop") == 0 ||
					std::strcmp(s, "endgroup") == 0 ||
					std::strcmp(s, "maketitle") == 0 || std::strcmp(s, "protect") == 0 ||
					std::strcmp(s, "makeatother") == 0 ||
					std::strncmp(s, "stop", 4) == 0 || std::strncmp(s, "Stop", 4) == 0 ||
					std::strcmp(s, "fi") == 0
					)
				lev = -1;
		}

		return lev;
	}

	int classifyFoldPointTeXUnpaired(const char* s)
	{
		int lev = 0;

		if (!(std::isdigit(s[0]) || (s[0] == '.'))) {
			static const char* const sections[] = {
			   "part",
			   "chapter",
			   "section",
			   "subsection",
			   "subsubsection",
			   "paragraph",
			   "subparagraph",
			   "addpart",
			   "addchap",
			   "addsec",
			   "addsubsec",
			   "minisec",
			   //"newcommand",
			   //"renewcommand",
			   "newenvironment",
			   "renewenvironment",
			   "CJKfamily",
			   "appendix",
			   "Topic",
			   "topic",
			   "subject",
			   "subsubject",
			   "def",
			   "gdef",
			   "edef",
			   "xdef",
			   "framed",
			   "frame",
			   "foilhead",
			   "overlays",
			   "slide"
			};

			const std::size_t count = sizeof (sections) / sizeof (*sections);

			if (std::find_if(sections, sections + count, std::bind2nd(std::ptr_fun(equal_strings), s)) != sections + count)
				lev = 1;
		}

		return lev;
	}

	// FoldTeXDoc: borrowed from VisualTeX with modifications
	void FoldTexDoc(unsigned startPos, int length, int, WordList**, Accessor &styler)
	{
		bool foldCompact = styler.GetPropertyInt("fold.compact", 1) != 0;
		unsigned endPos = startPos + length;
		int visibleChars = 0;
		int lineCurrent = styler.GetLine(startPos);
		int levelPrev = styler.LevelAt(lineCurrent) & SC_FOLDLEVELNUMBERMASK;
		int levelCurrent = levelPrev;
		char chNext = styler[startPos];
		char buffer[100] = "";

		for (unsigned i = startPos; i < endPos; ++i) {
			char ch = chNext;
			chNext = styler.SafeGetCharAt(i + 1);
			bool atEOL = (ch == '\r' && chNext != '\n') || (ch == '\n');

			if (ch == '\\') {
				ParseTeXCommand(i, styler, buffer);
				levelCurrent += classifyFoldPointTeXPaired(buffer) + classifyFoldPointTeXUnpaired(buffer);
			}

			if (levelCurrent > SC_FOLDLEVELBASE && ((ch == '\r' || ch == '\n') && (chNext == '\\'))) {
				ParseTeXCommand(i + 1, styler, buffer);
				levelCurrent -= classifyFoldPointTeXUnpaired(buffer);
			}

			char chNext2 = styler.SafeGetCharAt(i + 2);
			char chNext3 = styler.SafeGetCharAt(i + 3);
			char chNext4 = styler.SafeGetCharAt(i + 4);
			char chNext5 = styler.SafeGetCharAt(i + 5);

			bool atEOfold = (ch == '%') &&
					(chNext == '%') && (chNext2 == '}') &&
					(chNext3 == '}') && (chNext4 == '-') && (chNext5 == '-');

			bool atBOfold = (ch == '%') &&
					(chNext == '%') && (chNext2 == '-') &&
					(chNext3 == '-') && (chNext4 == '{') && (chNext5 == '{');

			if (atBOfold) {
				++levelCurrent;
			}

			if (atEOfold) {
				--levelCurrent;
			}

			if (ch == '\\' && chNext == '[') {
				++levelCurrent;
			}

			if (ch == '\\' && chNext == ']') {
				--levelCurrent;
			}

			bool foldComment = styler.GetPropertyInt("fold.comment") != 0;

			if (foldComment && atEOL && IsCommentLine(lineCurrent, styler)) {
				if (lineCurrent == 0 && IsCommentLine(lineCurrent + 1, styler)
						)
					++levelCurrent;
				else if (lineCurrent != 0 && !IsCommentLine(lineCurrent - 1, styler)
						&& IsCommentLine(lineCurrent + 1, styler)
						)
					++levelCurrent;
				else if (lineCurrent != 0 && IsCommentLine(lineCurrent - 1, styler) &&
						!IsCommentLine(lineCurrent + 1, styler))
					--levelCurrent;
			}

			//---------------------------------------------------------------------------------------------

			if (atEOL) {
				int lev = levelPrev;
				if (visibleChars == 0 && foldCompact)
					lev |= SC_FOLDLEVELWHITEFLAG;
				if ((levelCurrent > levelPrev) && (visibleChars > 0))
					lev |= SC_FOLDLEVELHEADERFLAG;
				if (lev != styler.LevelAt(lineCurrent)) {
					styler.SetLevel(lineCurrent, lev);
				}

				++lineCurrent;
				levelPrev = levelCurrent;
				visibleChars = 0;
			}

			if (!isspacechar(ch))
				++visibleChars;
		}

		// Fill in the real level of the next line, keeping the current flags as they will be filled in later
		int flagsNext = styler.LevelAt(lineCurrent) & ~SC_FOLDLEVELNUMBERMASK;
		styler.SetLevel(lineCurrent, levelPrev | flagsNext);
	}

	const char * const texWordListDesc[] = {
	   "TeX, eTeX, pdfTeX, Omega",
	   "ConTeXt Dutch",
	   "ConTeXt English",
	   "ConTeXt German",
	   "ConTeXt Czech",
	   "ConTeXt Italian",
	   "ConTeXt Romanian",
	   0,
	};
}

LexerModule lmTeX(SCLEX_TEX, ColouriseTeXDoc, "tex", FoldTexDoc, texWordListDesc);
