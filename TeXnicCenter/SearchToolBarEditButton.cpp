#include "stdafx.h"
#include "SearchToolBarEditButton.h"
#include "SearchToolBarEditCtrl.h"

IMPLEMENT_DYNCREATE(SearchToolBarEditButton, CMFCToolBarEditBoxButton)

SearchToolBarEditButton::SearchToolBarEditButton(UINT id, UINT width /*= 250*/)
	: CMFCToolBarEditBoxButton(id, -1 , ES_WANTRETURN | ES_AUTOHSCROLL |
		ES_LEFT | ES_NOHIDESEL | WS_TABSTOP, width)
{
}

SearchToolBarEditButton::~SearchToolBarEditButton()
{
}

CEdit* SearchToolBarEditButton::CreateEdit(CWnd* parent, const CRect& rect)
{
	SearchToolBarEditCtrl* e = new SearchToolBarEditCtrl(*this);
	e->Create(m_dwStyle, rect, parent, m_nID);

	return e;
}
