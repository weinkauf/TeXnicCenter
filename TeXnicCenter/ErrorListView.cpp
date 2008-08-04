// ErrorListView.cpp : implementation file
//

#include "stdafx.h"
#include "TeXnicCenter.h"
#include "ErrorListView.h"

#include "OutputDoc.h"

// ErrorListView

IMPLEMENT_DYNAMIC(ErrorListView, CDockablePane)

const UINT ListView = 0x1000;

ErrorListView::ErrorListView()
		: doc_(0)
{
}

BEGIN_MESSAGE_MAP(ErrorListView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_UPDATE_COMMAND_UI(ID_SHOW_ERRORS, &ErrorListView::OnUpdateShowErrors)
	ON_UPDATE_COMMAND_UI(ID_SHOW_WARNINGS, &ErrorListView::OnUpdateShowWarnings)
	ON_UPDATE_COMMAND_UI(ID_SHOW_BAD_BOXES, &ErrorListView::OnUpdateShowBadBoxes)
	ON_COMMAND(ID_SHOW_ERRORS, &ErrorListView::OnShowErrors)
	ON_COMMAND(ID_SHOW_WARNINGS, &ErrorListView::OnShowWarnings)
	ON_COMMAND(ID_SHOW_BAD_BOXES, &ErrorListView::OnShowBadBoxes)
	ON_NOTIFY(NM_DBLCLK,ListView,&ErrorListView::OnNMDblClk)
END_MESSAGE_MAP()

// ErrorListView message handlers

int ErrorListView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	show_errors_ = show_warnings_ = show_bad_boxes_ = true;

	toolbar_.Create(this,AFX_DEFAULT_TOOLBAR_STYLE,IDR_ERROR_LIST);
	toolbar_.LoadToolBar(IDR_ERROR_LIST,0,0,TRUE);

	// The style can be changed only after the toolbar has been loaded
	toolbar_.AdjustStyle();

	CMFCToolBarButton* button;

	for (int i = 0; i < toolbar_.GetCount(); ++i)
	{
		button = toolbar_.GetButton(i);

		if (button->m_nID) // Separator's ID is set to 0
			button->m_bText = TRUE;
	}

	toolbar_.SetOwner(this);
	toolbar_.SetRouteCommandsViaFrame(FALSE);

	list_view_.CWnd::CreateEx(WS_EX_CLIENTEDGE,WC_LISTVIEW,0,WS_CHILD|WS_VISIBLE|LVS_REPORT,CRect(),this,ListView);

	const DWORD style = LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT;
	list_view_.SetExtendedStyle(style);

	image_list_.m_hImageList = ::ImageList_LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_PARSE_VIEW),
	                           16,1,RGB(255,0,255),IMAGE_BITMAP,LR_CREATEDIBSECTION);

	list_view_.SetImageList(&image_list_,LVSIL_SMALL);

	list_view_.InsertColumn(0,_T(""),0,24);
	list_view_.InsertColumn(1,_T(""),LVCFMT_RIGHT,24);
	list_view_.InsertColumn(2,_T("Description"),0,500);
	list_view_.InsertColumn(3,_T("Line"),LVCFMT_RIGHT,50);
	list_view_.InsertColumn(4,_T("File"),0,150);

	Clear();

	return 0;
}

void ErrorListView::AdjustLayout(const CRect& rect)
{
	if (toolbar_ && list_view_)
	{
		const CSize size = toolbar_.CalcFixedLayout(TRUE,TRUE);
		toolbar_.SetWindowPos(0,rect.left,rect.top,rect.right - rect.left,rect.top + size.cy,SWP_NOACTIVATE|SWP_NOZORDER);

		const int y = rect.top + size.cy;
		list_view_.SetWindowPos(0,rect.left,y,rect.right,rect.bottom - y,SWP_NOACTIVATE|SWP_NOZORDER);
	}
}

void ErrorListView::AdjustLayout(void)
{
	CRect rect;
	GetClientRect(&rect);
	AdjustLayout(rect);
}

void ErrorListView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	list_view_.SetFocus();
}

void ErrorListView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void ErrorListView::Clear(void)
{
	list_view_.DeleteAllItems();
	items_.clear();

	errors_ = warnings_ = bad_boxes_ = 0;

	UpdateToolBarButton(CBuildView::imageBadBox);
	UpdateToolBarButton(CBuildView::imageWarning);
	UpdateToolBarButton(CBuildView::imageError);
}

void ErrorListView::AddMessage(const COutputInfo& info, CBuildView::tagImage t)
{
	bool insert = false;

	switch (t)
	{
		case CBuildView::imageError:
			++errors_;

			if (show_errors_)
				insert = true;

			break;
		case CBuildView::imageWarning:
			++warnings_;

			if (show_warnings_)
				insert = true;

			break;
		case CBuildView::imageBadBox:
			++bad_boxes_;

			if (show_bad_boxes_)
				insert = true;

			break;
	}

	item_monitor_.Lock();
	items_.push_back(Item(info,t));
	item_monitor_.Unlock();

	if (insert)
		list_view_.SetItemData(InsertMessage(info,t),items_.size() - 1);

	UpdateToolBarButton(t);
}

void ErrorListView::UpdateToolBarButton(CBuildView::tagImage t)
{
	// AddMessage and consequently UpdateToolBarButton can be
	// called from a different thread causing an assertion
	::PostMessage(m_hWnd,UpdateToolBarButtonMessageID,t,0);
}

BOOL ErrorListView::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL result;

	if (message == UpdateToolBarButtonMessageID)
	{
		CString fmt;
		int index;

		switch (wParam)
		{
			case CBuildView::imageError:
				fmt.Format(_T("%u Errors"),errors_);
				index = 0;
				break;
			case CBuildView::imageWarning:
				fmt.Format(_T("%u Warnings"),warnings_);
				index = 2;
				break;
			case CBuildView::imageBadBox:
				fmt.Format(_T("%u Bad boxes"),bad_boxes_);
				index = 4;
				break;
			default:
				index = -1;
		}

		ASSERT(index >= 0);
		toolbar_.SetButtonText(index,fmt);
		toolbar_.AdjustLayout();

		result = TRUE;
	}
	else
		result = CDockablePane::OnWndMsg(message, wParam, lParam, pResult);

	return result;
}

void ErrorListView::OnUpdateShowErrors(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(show_errors_);
}

void ErrorListView::OnUpdateShowWarnings(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(show_warnings_);
}

void ErrorListView::OnUpdateShowBadBoxes(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(show_bad_boxes_);
}

void ErrorListView::OnShowErrors()
{
	show_errors_ = !show_errors_;
	Populate();
}

void ErrorListView::OnShowWarnings()
{
	show_warnings_ = !show_warnings_;
	Populate();
}

void ErrorListView::OnShowBadBoxes()
{
	show_bad_boxes_ = !show_bad_boxes_;
	Populate();
}

int ErrorListView::InsertMessage(const COutputInfo& info, CBuildView::tagImage t)
{
	const int index = list_view_.InsertItem(0,0,t);

	CString line;
	line.Format(_T("%d"),list_view_.GetItemCount());

	list_view_.SetItemText(index,1,line);
	list_view_.SetItemText(index,2,info.GetErrorMessage());

	line.Format(_T("%d"),info.m_nSrcLine);

	list_view_.SetItemText(index,3,line);

	list_view_.SetItemText(index,4,CPathTool::GetFile(info.GetSourceFile()));

	return index;
}

void ErrorListView::Populate(unsigned set)
{
	list_view_.SetRedraw(FALSE);
	list_view_.DeleteAllItems();

	item_monitor_.Lock();

	for (ItemContainer::iterator it = items_.begin(); it != items_.end(); ++it)
		if (set & 1 << it->type)
			list_view_.SetItemData(InsertMessage(it->info,it->type),std::distance(items_.begin(),it));

	item_monitor_.Unlock();

	list_view_.SetRedraw();
	list_view_.Invalidate();
}

void ErrorListView::Populate(void)
{
	Populate(show_errors_ << CBuildView::imageError | show_warnings_ << CBuildView::imageWarning |
	         show_bad_boxes_ << CBuildView::imageBadBox);
}

void ErrorListView::OnNMDblClk(NMHDR*, LRESULT* result)
{
	if (doc_)
	{
		if (POSITION pos = list_view_.GetFirstSelectedItemPosition())
		{
			item_monitor_.Lock();
			const int line = items_[list_view_.GetItemData(list_view_.GetNextSelectedItem(pos))].info.GetOutputLine();
			item_monitor_.Unlock();

			doc_->ActivateBuildMessageByOutputLine(line);
			*result = 1;
		}
	}
}

void ErrorListView::AttachDoc(COutputDoc* doc)
{
	doc_ = doc;
}
