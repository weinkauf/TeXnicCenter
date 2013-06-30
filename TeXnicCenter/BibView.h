#pragma once

#include <vector>
#include <functional>

#include "LabelDragSource.h"
#include "PrivateToolBar.h"
#include "ProjectView.h"
#include "SortListCtrl.h"
#include "WorkspacePaneBase.h"

class BibItem;
class CLaTeXProject;

/**
 * @brief Bibliography view.
 */
class BibView : 
	public WorkspacePaneBase,
	public CProjectView,
	public LabelDragSource
{
public:
	typedef std::tr1::function<bool (const BibItem&)> PredicateFunctionType;

	DECLARE_DYNAMIC(BibView)

	BibView();
	virtual ~BibView();

	void ShowContextMenu(CPoint &point);
	void Clear(void);
	void AdjustLayout(const CRect& rect);
	void AdjustLayout();
	CLaTeXProject* GetProject();
	void GotoItem();	
	BOOL PreTranslateMessage(MSG* pMsg);

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void OnUpdate(CProjectView* sender, LPARAM l, LPVOID p);
	void OnParsingFinished();
	afx_msg void OnSetFocus(CWnd* pOldWnd);	
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	void OnNMDblClk(NMHDR*, LRESULT*);
	void OnLvnItemChanged(NMHDR*, LRESULT*);
	void OnEnter();

private:
	struct PopulateParam
	{
		PredicateFunctionType predicate;
		BibView* view;
		PopulateParam(const PredicateFunctionType& p, BibView* v);
	};

	struct Item;

	typedef std::vector<Item> BibItemContainerType;

	enum {
		Label		= 1,
		Author		= 1 << 1,
		Title		= 1 << 2,
		Year		= 1 << 3,
		Type		= 1 << 4,
		Publisher	= 1 << 5
	};

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

	void Populate(const PredicateFunctionType& predicate);
	void Populate();

	void DoPopulate( const PredicateFunctionType& predicate );
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
	static UINT PopulateThread(LPVOID p);
	void OnPopulateThread(const PredicateFunctionType& predicate);

	static int TranslateCompareFunction(LPARAM l1, LPARAM l2, BibView* pane, 
		const std::tr1::function<int (const BibItem& a, const BibItem& b)>& f);

	// Actual column compare functions
	static int CompareLabel(const BibItem& a, const BibItem& b);
	static int CompareAuthor(const BibItem& a, const BibItem& b);
	static int CompareYear(const BibItem& a, const BibItem& b);
	static int CompareTitle( const BibItem& a, const BibItem& b );
	static int ComparePublicationType( const BibItem& a, const BibItem& b );
	static int ComparePublisher( const BibItem& a, const BibItem& b );

	unsigned search_flags_;
	const bool can_group_;
	// Synchronization
	volatile LONG stop_search_;
	CSemaphore search_semaphore_;
	// Indicates whether the last populate was a search operation
	bool populate_was_search_;
	BibItemContainerType bib_items_;
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
};