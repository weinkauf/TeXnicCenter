// BibTeXView.cpp : implementation file
//

#include "stdafx.h"
#include "TeXnicCenter.h"
#include "BibTeXView.h"
#include "LaTeXTokenizer.h"
#include "configuration.h"


// BibTeXView

IMPLEMENT_DYNCREATE(BibTeXView, LaTeXViewBase)

BibTeXView::BibTeXView()
{

}

BibTeXView::~BibTeXView()
{
}

BEGIN_MESSAGE_MAP(BibTeXView, LaTeXViewBase)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// BibTeXView diagnostics

#ifdef _DEBUG
void BibTeXView::AssertValid() const
{
	LaTeXViewBase::AssertValid();
}

#ifndef _WIN32_WCE
void BibTeXView::Dump(CDumpContext& dc) const
{
	LaTeXViewBase::Dump(dc);
}

#endif
#endif //_DEBUG


// BibTeXView message handlers

int BibTeXView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (LaTeXViewBase::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void BibTeXView::UpdateSettings()
{
	CScintillaCtrl& rCtrl = GetCtrl();

	const LOGFONT& editor_font = CConfiguration::GetInstance()->m_fontEditor;
	const int point_size = GetLogFontPointSize(editor_font);

	SetAStyle(STYLE_DEFAULT, GetColor(COLORINDEX_NORMALTEXT),GetColor(COLORINDEX_BKGND),
		point_size,CConfiguration::GetInstance()->m_fontEditor.lfFaceName);

	SetAStyle(SCE_BIBTEX_COMMENT, GetColor(COLORINDEX_COMMENT),GetColor(COLORINDEX_BKGND)); // Includes comments' color
	SetAStyle(SCE_BIBTEX_ENTRY,RGB(125,167,217),GetColor(COLORINDEX_BKGND));

	SetAStyle(SCE_BIBTEX_DEFAULT,GetColor(COLORINDEX_NORMALTEXT),GetColor(COLORINDEX_BKGND));
	SetAStyle(SCE_BIBTEX_KEY, RGB(158,11,15),GetColor(COLORINDEX_BKGND));
	SetAStyle(SCE_BIBTEX_PARAMETER, RGB(145,0,145),GetColor(COLORINDEX_BKGND));

	SetAStyle(SCE_BIBTEX_UNKNOWN_ENTRY, RGB(255,0,0),GetColor(COLORINDEX_BKGND));

	rCtrl.StyleSetFont(SCE_BIBTEX_ENTRY,editor_font.lfFaceName);
	rCtrl.StyleSetSize(SCE_BIBTEX_ENTRY,point_size);
	rCtrl.StyleSetBold(SCE_BIBTEX_ENTRY,TRUE);

	rCtrl.StyleSetFont(SCE_BIBTEX_UNKNOWN_ENTRY,editor_font.lfFaceName);
	rCtrl.StyleSetSize(SCE_BIBTEX_UNKNOWN_ENTRY,point_size);
	rCtrl.StyleSetBold(SCE_BIBTEX_UNKNOWN_ENTRY,TRUE);

#pragma region Comments 

	// Comments displayed in italics
	rCtrl.StyleSetFont(SCE_BIBTEX_COMMENT,editor_font.lfFaceName);
	rCtrl.StyleSetSize(SCE_BIBTEX_COMMENT,point_size);
	rCtrl.StyleSetItalic(SCE_BIBTEX_COMMENT,TRUE);

#pragma endregion

	rCtrl.UsePopUp(FALSE);

	SetAStyle(SCE_BIBTEX_VALUE, GetColor(COLORINDEX_KEYWORD),GetColor(COLORINDEX_BKGND));
	rCtrl.SetSelFore(TRUE,GetColor(COLORINDEX_SELTEXT));
	rCtrl.SetSelBack(TRUE,GetColor(COLORINDEX_SELBKGND));

	rCtrl.StyleSetItalic(STYLE_DEFAULT,editor_font.lfItalic);
	rCtrl.StyleSetItalic(STYLE_DEFAULT,editor_font.lfWeight >= FW_BOLD);

#pragma region Brace highlighting

	rCtrl.StyleSetFont(STYLE_BRACELIGHT,editor_font.lfFaceName);
	rCtrl.StyleSetSize(STYLE_BRACELIGHT,point_size);
	rCtrl.StyleSetBold(STYLE_BRACELIGHT,TRUE);
	rCtrl.StyleSetFore(STYLE_BRACELIGHT,RGB(0,0,0));
	rCtrl.StyleSetBack(STYLE_BRACELIGHT,GetColor(COLORINDEX_PAIRSTRINGBKGND));

	rCtrl.StyleSetFont(STYLE_BRACEBAD,editor_font.lfFaceName);
	rCtrl.StyleSetSize(STYLE_BRACEBAD,point_size);
	rCtrl.StyleSetBold(STYLE_BRACEBAD,TRUE);
	rCtrl.StyleSetFore(STYLE_BRACEBAD,RGB(255,0,0));
	rCtrl.StyleSetBack(STYLE_BRACEBAD,GetColor(COLORINDEX_BADPAIRSTRINGBKGND));

#pragma endregion

#pragma region Caret

	rCtrl.SetCaretStyle(CConfiguration::GetInstance()->IsInsertCaretLine() ? CARETSTYLE_LINE : CARETSTYLE_BLOCK);
	rCtrl.SetCaretPeriod(CConfiguration::GetInstance()->IsBlinkInsertCaret() ? 500 : 0);

#pragma endregion
}

void BibTeXView::OnSettingsChanged()
{
	LaTeXViewBase::OnSettingsChanged();
	UpdateSettings();
}

DocumentTokenizer* BibTeXView::NewDocumentTokenizer() const
{
	return new LaTeXTokenizer();
}

int BibTeXView::GetLexer() const
{
	return SCLEX_BIBTEX;
}
