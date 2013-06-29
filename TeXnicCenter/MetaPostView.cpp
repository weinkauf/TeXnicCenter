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
	const LOGFONT& editor_font = CConfiguration::GetInstance()->m_fontEditor;
	const int point_size = GetLogFontPointSize(editor_font);

	SetAStyle(SCE_METAPOST_DEFAULT, GetColor(COLORINDEX_COMMENT),GetColor(COLORINDEX_BKGND), point_size, editor_font.lfFaceName, false, true);
	SetAStyle(SCE_TEX_COMMAND, GetColor(COLORINDEX_KEYWORD),GetColor(COLORINDEX_BKGND));
	SetAStyle(SCE_TEX_GROUP, GetColor(COLORINDEX_GROUP), GetColor(COLORINDEX_BKGND));
	SetAStyle(SCE_METAPOST_TEXT, GetColor(COLORINDEX_NORMALTEXT), GetColor(COLORINDEX_BKGND));
	SetAStyle(SCE_METAPOST_SPECIAL, GetColor(COLORINDEX_SPECIAL), GetColor(COLORINDEX_BKGND));
	SetAStyle(SCE_METAPOST_SYMBOL, GetColor(COLORINDEX_SYMBOLS), GetColor(COLORINDEX_BKGND));
}

DocumentTokenizer* MetaPostView::NewDocumentTokenizer() const
{
	return new LaTeXTokenizer();
}

int MetaPostView::GetLexer() const
{
	return SCLEX_METAPOST;
}
