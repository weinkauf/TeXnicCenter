#pragma once

#include <vector>

#include "BuildView.h"
#include "PrivateToolBar.h"

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

        Item(const COutputInfo& info, CBuildView::tagImage type)
        : info(info), type(type)
        {
        }
    };

    typedef std::vector<Item> ItemContainer;
    ItemContainer items_;

    PrivateToolBar toolbar_;
    CListCtrl list_view_;
    CImageList image_list_;
    int errors_, warnings_, bad_boxes_;
    bool show_errors_, show_warnings_, show_bad_boxes_;

    static const int UpdateToolBarButtonMessageID = WM_USER + 1;

public:
	ErrorListView();

protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    void AdjustLayout(const CRect& rect);
    void AdjustLayout(void);
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    void Clear(void);
    void AddMessage(const COutputInfo& info, CBuildView::tagImage t);
private:
    void UpdateToolBarButton(CBuildView::tagImage t);
protected:
    virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
    afx_msg void OnUpdateShowErrors(CCmdUI *pCmdUI);
    afx_msg void OnUpdateShowWarnings(CCmdUI *pCmdUI);
    afx_msg void OnUpdateShowBadBoxes(CCmdUI *pCmdUI);
    afx_msg void OnShowErrors();
    afx_msg void OnShowWarnings();
    afx_msg void OnShowBadBoxes();
private:
    int InsertMessage(const COutputInfo& info, CBuildView::tagImage t);
    void Populate(unsigned set);
    void Populate(void);

    afx_msg void OnNMDblClk(NMHDR*, LRESULT*);
public:
    void AttachDoc(COutputDoc* doc);
};
