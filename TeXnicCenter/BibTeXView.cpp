// BibTeXView.cpp : implementation file
//

#include "stdafx.h"
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
	const LOGFONT& editor_font = CConfiguration::GetInstance()->m_fontEditor;
	const int point_size = GetLogFontPointSize(editor_font);

	//Default text.
	SetAStyle(SCE_BIBTEX_DEFAULT, GetColor(COLORINDEX_NORMALTEXT), GetColor(COLORINDEX_BKGND));

	//Syntax
	// - comments displayed in italics
	SetAStyle(SCE_BIBTEX_COMMENT, GetColor(COLORINDEX_COMMENT), GetColor(COLORINDEX_BKGND), point_size, editor_font.lfFaceName, false, true);
	SetAStyle(SCE_BIBTEX_ENTRY, GetColor(COLORINDEX_FILE_INCLUSION),GetColor(COLORINDEX_BKGND), point_size, editor_font.lfFaceName, true, false);
	SetAStyle(SCE_BIBTEX_KEY, GetColor(COLORINDEX_SPECIAL), GetColor(COLORINDEX_BKGND));
	SetAStyle(SCE_BIBTEX_VALUE, GetColor(COLORINDEX_KEYWORD), GetColor(COLORINDEX_BKGND));
	SetAStyle(SCE_BIBTEX_PARAMETER, GetColor(COLORINDEX_SYMBOLS), GetColor(COLORINDEX_BKGND));

	SetAStyle(SCE_BIBTEX_UNKNOWN_ENTRY, GetColor(COLORINDEX_INDICATOR_SPELLING),GetColor(COLORINDEX_BKGND), point_size, editor_font.lfFaceName, true, false);
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


void BibTeXView::AddEscapeChars(CString& EscapeChars)
{
	EscapeChars.Append(_T("@"));
}
