#pragma once

#include "CodeView.h"

class LaTeXDocumentBase;

// LaTeXViewBase view

class LaTeXViewBase : public CodeView
{
	DECLARE_DYNAMIC(LaTeXViewBase)

public:
	enum
	{
		// Base colors
		COLORINDEX_BKGND,
		COLORINDEX_NORMALTEXT,
		COLORINDEX_CARET,
		COLORINDEX_CARETLINE,
		COLORINDEX_SELBKGND,
		COLORINDEX_SELTEXT,

		// Syntax colors
		COLORINDEX_KEYWORD,
		COLORINDEX_COMMENT,
		COLORINDEX_SPECIAL, //> Characters triggering something special.
		COLORINDEX_SYMBOLS, //> The '+' and '-' and similar signs.

		COLORINDEX_GROUP, //> The '{' and '}' in \begin{figure} and the '$' signs.
		COLORINDEX_GROUP_NAME, //> The 'figure' in \begin{figure}

		COLORINDEX_STYLE_INCLUSION, //> The \documentclass command, or \bibliographystyle
		COLORINDEX_FILE_INCLUSION, //> Everything from \usepackage to \input

		COLORINDEX_INLINE_MATH_TEXT,
		COLORINDEX_INLINE_MATH_COMMAND,

		COLORINDEX_DIGIT,
		COLORINDEX_UNIT,

		// Brackets highlighting
		COLORINDEX_PAIRSTRINGBKGND,
		COLORINDEX_PAIRSTRINGTEXT,
		COLORINDEX_BADPAIRSTRINGBKGND,
		COLORINDEX_BADPAIRSTRINGTEXT,

		//COLORINDEX_VERBATIM_TEXT, >>>> MISSING IN THE PARSER <<<<<

		//Margins and markers
		COLORINDEX_FOLDMARGIN,
		COLORINDEX_FOLDMARGIN_HIGHLIGHT,
		COLORINDEX_FOLDMARK_FORE,
		COLORINDEX_FOLDMARK_BACK,
		COLORINDEX_ERRORMARK_FORE,
		COLORINDEX_ERRORMARK_BACK,
		COLORINDEX_BOOKMARK_FORE,
		COLORINDEX_BOOKMARK_BACK,
		COLORINDEX_LINENUMBERS_FORE,
		COLORINDEX_LINENUMBERS_BACK,

		//Indicators
		COLORINDEX_INDICATOR_SPELLING,

		// How many colors do we have? ==> This must be the last line.
		COLORINDEX_COUNT
	};

	enum
	{
		// Custom colors
		COLORSCHEME_CUSTOM,

		// Default color schemes
		COLORSCHEME_DEFAULT,
		COLORSCHEME_INVERSE,

		COLORSCHEME_COUNT
	};

protected:
	LaTeXViewBase();           // protected constructor used by dynamic creation
	virtual ~LaTeXViewBase();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

	void SetAStyle(int style, COLORREF fore, COLORREF back = RGB(255,255,255), int size = -1, LPCTSTR face = 0, BOOL bold = false, BOOL italic = false);
	void OnUpdateUI(SCNotification* pSCNotification);

public:
	static COLORREF GetAutomaticColor(int nColorIndex, const int idScheme = COLORSCHEME_DEFAULT);
	static COLORREF GetColor(int nColorIndex);
	virtual void OnSettingsChanged();

	LaTeXDocumentBase* GetDocument() const;

protected:
	afx_msg void OnSpellFile();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
//public:
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
