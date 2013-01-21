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
		//	Base colors
		COLORINDEX_WHITESPACE,
		COLORINDEX_BKGND,
		COLORINDEX_NORMALTEXT,
		COLORINDEX_SELMARGIN,
		COLORINDEX_SELBKGND,
		COLORINDEX_SELTEXT,
		//	Syntax colors
		COLORINDEX_KEYWORD,
		COLORINDEX_COMMENT,
		COLORINDEX_OPERATOR,
		COLORINDEX_VERBATIM_TEXT, // [JRT]:
		COLORINDEX_INLINE_MATH_TEXT,
		COLORINDEX_INLINE_MATH_COMMAND,

		// Brackets highlighting
		COLORINDEX_PAIRSTRINGBKGND,
		COLORINDEX_PAIRSTRINGTEXT,
		COLORINDEX_BADPAIRSTRINGBKGND,
		COLORINDEX_BADPAIRSTRINGTEXT,
		COLORINDEX_CURPAIRSTRINGBKGND,
		COLORINDEX_CURPAIRSTRINGTEXT,
		COLORINDEX_PAIRBLOCKBKGND,
		COLORINDEX_GROUP_NAME,
		COLORINDEX_DIGIT,
		COLORINDEX_UNIT,

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

	void SetAStyle(int style, COLORREF fore, COLORREF back = RGB(255,255,255), int size = -1, LPCTSTR face = 0);
	void OnUpdateUI(SCNotification* pSCNotification);

public:
	static COLORREF GetAutomaticColor(int nColorIndex, const int idScheme = COLORSCHEME_DEFAULT);
	static COLORREF GetColor(int nColorIndex);

	LaTeXDocumentBase* GetDocument() const;

protected:
	afx_msg void OnSpellFile();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};
