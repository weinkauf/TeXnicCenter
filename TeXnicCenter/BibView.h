#pragma once

#include <vector>
#include <functional>

#include "LabelDragSource.h"
#include "PrivateToolBar.h"
#include "SortListCtrl.h"
#include "BiBTeXEntry.h"
#include "WorkspacePaneBase.h"

/// Bibliography view.
class BibView : 
	public WorkspacePaneBase,
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
	const bool can_group_;
	// Synchronization
	volatile LONG stop_search_;
	CSemaphore search_semaphore_;
	// Indicates whether the last populate was a search operation
	bool populate_was_search_;

	struct Item
	{
		int group_id;
		int image;
		StructureItemContainer::size_type structure_item_index;
		BibItem bib;

		Item(int group_id, int image, const BibItem& b);
	};

	typedef std::vector<Item> BibItemContainerType;
	BibItemContainerType bib_items_;

public:
	typedef std::tr1::function<bool (const BibItem&)> PredicateFunctionType;

private:
	struct PopulateParam
	{
		PredicateFunctionType predicate;
		BibView* view;
		PopulateParam(const PredicateFunctionType& p, BibView* v);
	};

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
	/////The window can be a tabbed document
	//virtual BOOL CanBeTabbedDocument() const;
	//virtual BOOL CanBeDocked(CBasePane* pDockBar) const;

protected:
	afx_msg void OnSetFocus(CWnd* pOldWnd);	
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
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

	static int TranslateCompareFunction(LPARAM l1, LPARAM l2, BibView* pane, const std::tr1::function<int (const BibItem& a, const BibItem& b)>& f);
	
	// Actual column compare functions
	static int CompareLabel(const BibItem& a, const BibItem& b);
	static int CompareAuthor(const BibItem& a, const BibItem& b);
	static int CompareYear(const BibItem& a, const BibItem& b);
	static int CompareTitle( const BibItem& a, const BibItem& b );
	static int ComparePublicationType( const BibItem& a, const BibItem& b );
	static int ComparePublisher( const BibItem& a, const BibItem& b );

public:
	void GotoItem();	
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	static UINT PopulateThread(LPVOID p);
	void OnPopulateThread(const PredicateFunctionType& predicate);
};