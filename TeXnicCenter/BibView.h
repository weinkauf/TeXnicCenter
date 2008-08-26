#pragma once

#include <vector>
#include <functional>

#include "LabelDragSource.h"
#include "PrivateToolBar.h"
#include "SortListCtrl.h"

class BibItem;

// BibView

class BibView : 
	public CDockablePane,
	public CProjectView,
	public LabelDragSource
{
	enum {
		Author		= 1,
		Title		= 1 << 1,
		Year		= 1 << 2,
		Type		= 1 << 3,
		Publisher	= 1 << 4
	};

	unsigned search_flags_;

	DECLARE_DYNAMIC(BibView)

private:
	bool search_timer_enabled_;
	SortListCtrl list_view_;
	PrivateToolBar toolbar_;
	CMFCToolBarEditBoxButton* search_button_;
	CString search_string_;
	CString temp_;
	int dragged_item_;
	// Used in ItemContains to reduce stack (re)allocations while 
	// performing the search operation
	std::vector<CString> token_buffer_;
	std::vector<std::vector<CString>::iterator> pos_buffer_;
	bool context_menu_active_;

public:
	BibView();
	virtual ~BibView();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void OnUpdate(CProjectView* sender, LPARAM l, LPVOID p);
	void OnParsingFinished();

public:
	void ShowContextMenu(CPoint &point);
	void Clear(void);
	void AdjustLayout(const CRect& rect);
	void AdjustLayout();
	CLaTeXProject* GetProject();
	
protected:
	afx_msg void OnSetFocus(CWnd* pOldWnd);	
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void OnNMDblClk(NMHDR*, LRESULT*);
	void OnLvnItemChanged(NMHDR*, LRESULT*);
	void OnEnter();

private:
	static bool EveryItem(const BibItem& /*item*/);

	/// Indicates whether the text parameter contains the search 
	/// string. The comparison is case insensitive.
	bool Contains(const CString& text, const CString& search);
	/// Indicates whether the text string contains any of the tokens
	/// and removes the token that has been found from the container 
	bool ContainsAny(const CString& text, std::vector<CString>& tokens);
	/// Predicate function which examines the item for search_tokens 
	/// using the current search options.
	bool ItemContains(const BibItem& item, const std::vector<CString>& search_tokens);

	void Populate(const std::tr1::function<bool (const BibItem&)>& predicate);
	void Populate();
	void OnSearchTimerElapsed();

	void StartSearch();
	void OnClearSearch();
	void OnLvnBeginDrag(NMHDR*, LRESULT*);

	void OnEnChangeSearch();
	void OnStartSearch();
	void OnSearchOptions(UINT id);
	void ToggleSearchOption(unsigned opt);
	bool IsSearchOptionSet(unsigned opt) const;
	void OnUpdateSearchOptions(CCmdUI* pCmdUI);

	static int TranslateCompareFunction(LPARAM l1, LPARAM l2, BibView* pane, 
		const std::tr1::function<int (const CStructureItem& a, const CStructureItem& b)>& f);
	
	// Actual column compare functions
	static int CompareLabel(const CStructureItem& a, const CStructureItem& b);
	static int CompareAuthor(const CStructureItem& a, const CStructureItem& b);
	static int CompareYear(const CStructureItem& a, const CStructureItem& b);
	static int CompareTitle( const CStructureItem& a, const CStructureItem& b );
	static int ComparePublicationType( const CStructureItem& a, const CStructureItem& b );
	static int ComparePublisher( const CStructureItem& a, const CStructureItem& b );

public:
	void GotoItem();	
};