#pragma once

class SearchToolBarEditButton :
	public CMFCToolBarEditBoxButton
{
	DECLARE_DYNCREATE(SearchToolBarEditButton)
public:
	SearchToolBarEditButton(UINT id = 0, UINT width = 250);
	~SearchToolBarEditButton();

	CEdit* CreateEdit(CWnd* parent, const CRect& rect);
};
