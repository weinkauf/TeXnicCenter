#include "stdafx.h"
#include "MetaPostView.h"

#include "configuration.h"
#include "LaTeXTokenizer.h"

IMPLEMENT_DYNCREATE(MetaPostView, LaTeXViewBase)

MetaPostView::MetaPostView(void)
{
}

MetaPostView::~MetaPostView(void)
{
}

BEGIN_MESSAGE_MAP(MetaPostView, LaTeXViewBase)
	ON_WM_CREATE()
END_MESSAGE_MAP()

// MetaPostView diagnostics

#ifdef _DEBUG
void MetaPostView::AssertValid() const
{
	LaTeXViewBase::AssertValid();
}

#ifndef _WIN32_WCE
void MetaPostView::Dump(CDumpContext& dc) const
{
	LaTeXViewBase::Dump(dc);
}
#endif
#endif //_DEBUG


int MetaPostView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (LaTeXViewBase::OnCreate(lpCreateStruct) == -1)
		return -1;

	UpdateSettings();

	return 0;
}

void MetaPostView::UpdateSettings()
{
	CScintillaCtrl& rCtrl = GetCtrl();

	const LOGFONT& editor_font = CConfiguration::GetInstance()->m_fontEditor;
	const int point_size = GetLogFontPointSize(editor_font);

	SetAStyle(STYLE_DEFAULT, GetColor(COLORINDEX_NORMALTEXT),GetColor(COLORINDEX_BKGND),
		point_size,CConfiguration::GetInstance()->m_fontEditor.lfFaceName);

	SetAStyle(SCE_METAPOST_DEFAULT, GetColor(COLORINDEX_COMMENT),GetColor(COLORINDEX_BKGND)); // Includes comments' color
	SetAStyle(SCE_TEX_GROUP,RGB(125,167,217),GetColor(COLORINDEX_BKGND));

	SetAStyle(SCE_METAPOST_TEXT,GetColor(COLORINDEX_NORMALTEXT),GetColor(COLORINDEX_BKGND));
	SetAStyle(SCE_METAPOST_SPECIAL, RGB(158,11,15),GetColor(COLORINDEX_BKGND));
	SetAStyle(SCE_METAPOST_SYMBOL, RGB(145,0,145),GetColor(COLORINDEX_BKGND));

#pragma region Comments 

	// Comments displayed in italics
	rCtrl.StyleSetFont(SCE_METAPOST_DEFAULT,editor_font.lfFaceName);
	rCtrl.StyleSetSize(SCE_METAPOST_DEFAULT,point_size);
	rCtrl.StyleSetItalic(SCE_METAPOST_DEFAULT,TRUE);

#pragma endregion

	rCtrl.UsePopUp(FALSE);

	SetAStyle(SCE_TEX_COMMAND, GetColor(COLORINDEX_KEYWORD),GetColor(COLORINDEX_BKGND));
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
}

DocumentTokenizer* MetaPostView::NewDocumentTokenizer() const
{
	return new LaTeXTokenizer();
}

int MetaPostView::GetLexer() const
{
	return SCLEX_METAPOST;
}
