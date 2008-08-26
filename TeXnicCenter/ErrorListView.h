#pragma once

#include <vector>

#include "BuildView.h"
#include "PrivateToolBar.h"
#include "SortListCtrl.h"

class COutputDoc;

// ErrorListView

class ErrorListView : public CDockablePane
{
	DECLARE_DYNAMIC(ErrorListView)

private:
	COutputDoc* doc_;
	CCriticalSection item_monitor_;

	struct Item
	{
		COutputInfo info;
		CBuildView::tagImage type;
		int ordinal;

		Item(const COutputInfo& info, CBuildView::tagImage type)
				: info(info), type(type)
		{
		}
	};

	typedef std::vector<Item> ItemContainer;
	ItemContainer items_;

	PrivateToolBar toolbar_;
	SortListCtrl list_view_;
	CImageList image_list_;
	int errors_, warnings_, bad_boxes_;
	bool show_errors_, show_warnings_, show_bad_boxes_;

	static const int UpdateToolBarButtonMessageID = WM_USER + 1;

public:
	ErrorListView();

protected:
	DECLARE_MESSAGE_MAP()

public:	
	void AdjustLayout(const CRect& rect);
	void AdjustLayout(void);
	void Clear(void);
	void AddMessage(const COutputInfo& info, CBuildView::tagImage t);

private:
	void UpdateToolBarButton(CBuildView::tagImage t);

protected:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnUpdateShowErrors(CCmdUI *pCmdUI);
	afx_msg void OnUpdateShowWarnings(CCmdUI *pCmdUI);
	afx_msg void OnUpdateShowBadBoxes(CCmdUI *pCmdUI);
	afx_msg void OnShowErrors();
	afx_msg void OnShowWarnings();
	afx_msg void OnShowBadBoxes();
	afx_msg void OnNMDblClk(NMHDR*, LRESULT*);

private:
	int InsertMessage(const COutputInfo& info, CBuildView::tagImage t);
	void Populate(unsigned set);
	void Populate(void);

public:
	void AttachDoc(COutputDoc* doc);

private:
	int CompareType(LPARAM l1, LPARAM l2);
	int CompareOrdinal(LPARAM l1, LPARAM l2);
	int CompareErrorMessage(LPARAM l1, LPARAM l2);
	int CompareSourceLine(LPARAM l1, LPARAM l2);
	int CompareSourceFile(LPARAM l1, LPARAM l2);
};
