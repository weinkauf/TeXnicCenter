#pragma once
#include "PrivateToolBar.h"
#include "WorkspacePaneBase.h"

class CodeBookmark;
class BookmarkEventArgs;
class CLaTeXProject;

class BookmarkView : 
	public WorkspacePaneBase, 
	public CProjectView
{
	DECLARE_DYNAMIC(BookmarkView)
	
private:
	SortListCtrl list_view_;
	PrivateToolBar toolbar_;

public:
	BookmarkView();
	virtual ~BookmarkView();

protected:
	DECLARE_MESSAGE_MAP()

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void AdjustLayout(const CRect& rect);

private:
	void Populate();

public:
	void Clear();

protected:
	void OnOpeningProject();
	void OnNMDblClk(NMHDR* hdr, LRESULT*);
	void OnLvnEndEditLabel(NMHDR* /*hdr*/, LRESULT*);
	void OnClosingProject();

private:
	void OnBookmarkAdded(CLaTeXProject* sender, const BookmarkEventArgs& args);
	void OnBookmarkRemoved(CLaTeXProject* sender, const BookmarkEventArgs& args);
	void InsertBookmark(int index, const CString& filename, const CodeBookmark& b);
	void OnLvnDeleteItem(NMHDR* /*hdr*/, LRESULT*);
	int FindBookmark(const CodeBookmark b, CodeBookmark** p = 0);
	static int CompareName(LPARAM l1, LPARAM l2);
	static int CompareLineNumber(LPARAM l1, LPARAM l2);
	void OnLvnKeyDown(NMHDR* hdr, LRESULT*);

public:
	BOOL PreTranslateMessage(MSG* msg);

protected:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};