#pragma once

#include "PrivateToolBar.h"
#include "SortListCtrl.h"
#include "WorkspacePaneBase.h"

class CodeBookmark;
class BookmarkEventArgs;
class CLaTeXProject;

/**
 * @brief Represents a view that displays all the bookmarks within a project.
 */
class BookmarkView : 
	public WorkspacePaneBase, 
	public CProjectView
{
	DECLARE_DYNAMIC(BookmarkView)
public:
	BookmarkView();
	virtual ~BookmarkView();

	void Clear();
	BOOL PreTranslateMessage(MSG* msg);

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void AdjustLayout(const CRect& rect);
	void OnOpeningProject();
	void OnNMDblClk(NMHDR* hdr, LRESULT*);
	void OnLvnEndEditLabel(NMHDR* /*hdr*/, LRESULT*);
	void OnClosingProject();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

private:
	void Populate();
	void OnBookmarkAdded(CLaTeXProject* sender, const BookmarkEventArgs& args);
	void OnBookmarkRemoved(CLaTeXProject* sender, const BookmarkEventArgs& args);
	void InsertBookmark(int index, const CString& filename, const CodeBookmark& b);
	void OnLvnDeleteItem(NMHDR* /*hdr*/, LRESULT*);
	int FindBookmark(const CodeBookmark b, CodeBookmark** p = 0);
	static int CompareName(LPARAM l1, LPARAM l2);
	static int CompareLineNumber(LPARAM l1, LPARAM l2);
	void OnLvnKeyDown(NMHDR* hdr, LRESULT*);

	SortListCtrl list_view_;
	PrivateToolBar toolbar_;
};