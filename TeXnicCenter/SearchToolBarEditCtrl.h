#pragma once

class SearchToolBarEditCtrl :
	public CMFCToolBarEditCtrl
{
	DECLARE_DYNAMIC(SearchToolBarEditCtrl)
public:
	SearchToolBarEditCtrl(CMFCToolBarEditBoxButton& button);
	~SearchToolBarEditCtrl();

	BOOL PreTranslateMessage(MSG* pMsg);
};
