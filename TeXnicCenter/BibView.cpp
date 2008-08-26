#include "stdafx.h"
#include "TeXnicCenter.h"

#include "BibView.h"
#include "BiBTeXEntry.h"
#include "RunTimeHelper.h"
#include "navigatorview.h"
#include "OleDrop.h"

enum {
	ListID = 100,
	SearchTimerID,
	SearchTimerTimeout = 1000 // 1 second
};

// Generic string tokenizer
template<class Ch, class Container>
void Tokenize(const Ch* first, const Ch* last, const Ch* sep_first, const Ch* sep_last, Container& c)
{
	c.clear();

	typedef typename Container::value_type StringType;

	const Ch* pos1 = first; const Ch* pos2;
	bool stop = false;

	while (!stop) {
		// find first not of
		pos1 = std::find_first_of(pos1,last,sep_first,sep_last,std::not2(std::equal_to<Ch>()));
		pos2 = std::find_first_of(pos1,last,sep_first,sep_last);

		if (pos1 != last) {
			c.push_back(StringType(pos1,pos2 - pos1));
			pos1 = pos2;
		}
		else
			stop = true;
	}
}

/// Splits a string into tokens.
/// \param text 
///		   String to split
/// \param sep 
///		   Characters used to split the string
/// \param words
///		   A container that will contain the resulting tokens
void Tokenize(const CString& text, const CString& sep, std::vector<CString>& words)
{
	LPCTSTR first = text;
	LPCTSTR last = first + text.GetLength();

	LPCTSTR sep_first = sep;
	LPCTSTR sep_last = sep_first + sep.GetLength();

	Tokenize(first,last,sep_first,sep_last,words);
}

// BibView

IMPLEMENT_DYNAMIC(BibView, CDockablePane)

BibView::BibView()
: search_timer_enabled_(false)
, dragged_item_(-1)
, search_flags_(0)
{
}

BibView::~BibView()
{
}


BEGIN_MESSAGE_MAP(BibView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_CONTEXTMENU()
	ON_WM_TIMER()
	ON_EN_CHANGE(ID_SEARCH_EDIT, &BibView::OnEnChangeSearch)
	ON_COMMAND(ID_SEARCH_EDIT, &BibView::OnEnChangeSearch)
	ON_COMMAND(ID_CLEAR_SEARCH, &BibView::OnClearSearch)
	ON_COMMAND(ID_START_SEARCH, &BibView::OnStartSearch)
	ON_NOTIFY(LVN_BEGINDRAG, ListID, &BibView::OnLvnBeginDrag)
	ON_COMMAND_RANGE(ID_AUTHOR,ID_PUBLISHER, &BibView::OnSearchOptions)
	ON_UPDATE_COMMAND_UI_RANGE(ID_AUTHOR,ID_PUBLISHER, &BibView::OnUpdateSearchOptions)
	ON_NOTIFY(NM_DBLCLK, ListID, &BibView::OnNMDblClk)
	ON_NOTIFY(LVN_ITEMCHANGED, ListID, &BibView::OnLvnItemChanged)
	ON_COMMAND(IDOK, &BibView::OnEnter)
END_MESSAGE_MAP()

// BibView message handlers

int BibView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Default search options
	search_flags_ = Author|Title|Year;

	toolbar_.Create(this,AFX_DEFAULT_TOOLBAR_STYLE,IDR_BIB_SEARCH);
	toolbar_.LoadToolBar(IDR_BIB_SEARCH,0,0,TRUE);
	toolbar_.AdjustStyle();

	toolbar_.SetRouteCommandsViaFrame(FALSE);

	CMFCToolBarEditBoxButton edit(ID_SEARCH_EDIT,-1,ES_WANTRETURN|ES_AUTOHSCROLL|ES_LEFT|ES_NOHIDESEL,250);

	search_button_ = static_cast<CMFCToolBarEditBoxButton*>(toolbar_.GetButton(toolbar_.ReplaceButton(ID_SEARCH_EDIT,edit)));

	CMenu menu;
	menu.LoadMenu(IDR_BIB_SEARCH_OPTIONS);
	
	CMFCToolBarMenuButton m(~0U,menu.GetSubMenu(0)->GetSafeHmenu(),0);
	toolbar_.ReplaceButton(ID_SEARCH_OPTIONS,m);

	if (RunTimeHelper::IsCommCtrl6())
		search_button_->GetEditBox()->SetCueBanner(CString(MAKEINTRESOURCE(IDS_SEARCH)));

	list_view_.CreateEx(WS_EX_CLIENTEDGE,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|LVS_REPORT,
		CRect(0,0,0,0),this,ListID);

	if (RunTimeHelper::IsVista())
		::SetWindowTheme(list_view_,L"explorer",0);

	list_view_.SetExtendedStyle(LVS_EX_DOUBLEBUFFER|LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP);
	list_view_.EnableGroupView(TRUE);

	list_view_.InsertColumn(0,CString(MAKEINTRESOURCE(IDS_LABEL)),0,150);
	list_view_.InsertColumn(1,CString(MAKEINTRESOURCE(IDS_AUTHOR)),0,250);
	list_view_.InsertColumn(2,CString(MAKEINTRESOURCE(IDS_TITLE)),0,300);
	list_view_.InsertColumn(3,CString(MAKEINTRESOURCE(IDS_YEAR)),LVCFMT_RIGHT,50);
	list_view_.InsertColumn(4,CString(MAKEINTRESOURCE(IDS_PUBLICATION_TYPE)),0,100);
	list_view_.InsertColumn(5,CString(MAKEINTRESOURCE(IDS_PUBLISHER)),0,150);

	CImageList il;
	il.Attach(::ImageList_LoadImage(AfxGetResourceHandle(),MAKEINTRESOURCE(IDB_ITEM_TYPES),16,1,
		RGB(255,0,255),IMAGE_BITMAP,LR_CREATEDIBSECTION));
	ASSERT(il);

	list_view_.SetImageList(&il,LVSIL_SMALL);
	il.Detach(); // Will be destroyed by the list view

	using namespace std::tr1;
	using namespace placeholders;

	MapKeyStateToFormat(0,CLaTeXProject::FormatRef); // No addition key pressed while dragging, output \cite{key}
	MapKeyStateToFormat(MK_CONTROL,bind(&CStructureItem::GetLabel,_1)); // Just the label
	
	MapKeyStateToMessage(0,ID_ITEM_INSERT_REF);
	MapKeyStateToMessage(MK_CONTROL,ID_ITEM_INSERT_LABEL);

	list_view_.SetColumnCompareFunction(0,bind(TranslateCompareFunction,_1,_2,this,CompareLabel));
	list_view_.SetColumnCompareFunction(1,bind(TranslateCompareFunction,_1,_2,this,CompareAuthor));
	list_view_.SetColumnCompareFunction(2,bind(TranslateCompareFunction,_1,_2,this,CompareTitle));
	list_view_.SetColumnCompareFunction(3,bind(TranslateCompareFunction,_1,_2,this,CompareYear));
	list_view_.SetColumnCompareFunction(4,bind(TranslateCompareFunction,_1,_2,this,ComparePublicationType));
	list_view_.SetColumnCompareFunction(5,bind(TranslateCompareFunction,_1,_2,this,ComparePublisher));

	AdjustLayout();

	return 0;
}

void BibView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout(CRect(0,0,cx,cy));
}

CLaTeXProject* BibView::GetProject()
{
	ASSERT_KINDOF(CLaTeXProject,CProjectView::GetProject());
	return static_cast<CLaTeXProject*>(CProjectView::GetProject());
}

void BibView::OnUpdate(CProjectView* /*sender*/, LPARAM lHint, LPVOID /*p*/)
{
	switch (lHint) {
		case COutputDoc::hintParsingFinished:
			OnParsingFinished();
			break;
	}
}

void BibView::Clear(void)
{
	list_view_.DeleteAllItems();
	list_view_.RemoveAllGroups();
}

void BibView::AdjustLayout(const CRect& rect)
{
	if (toolbar_ && list_view_)
	{
		CSize size(0,0);

		if (toolbar_.IsVisible())
		{
			size = toolbar_.CalcFixedLayout(TRUE,TRUE);
			toolbar_.SetWindowPos(0,rect.left,rect.top,
				rect.right - rect.left,rect.top + size.cy,SWP_NOACTIVATE|SWP_NOZORDER);
		}

		const int y = rect.top + size.cy;
		list_view_.SetWindowPos(0,rect.left,y,rect.right,rect.bottom - y,
			SWP_NOACTIVATE|SWP_NOZORDER);
	}
}

void BibView::AdjustLayout()
{
	CRect rect;
	GetClientRect(&rect);
	AdjustLayout(rect);
}

void BibView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	list_view_.SetFocus();
}

void BibView::OnParsingFinished()
{
	Populate();
}

void BibView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	ShowContextMenu(point);
}

void BibView::ShowContextMenu(CPoint& point)
{
	// handle only if generated by key (context menu key on win95 keyboard)
	// (mouse is handled by OnRButtonDown() -- problems with messages from CCJTabCtrlBar!)
	if (point.x == -1 && point.y == -1) {
		
		int item = list_view_.GetNextItem(-1,LVNI_ALL|LVNI_SELECTED);
		CRect rect;

		if (item != -1 && list_view_.GetItemRect(item,rect,LVIR_LABEL)) {
			list_view_.ClientToScreen(rect);
			theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_ITEM,rect.right,rect.bottom,&list_view_);
		}
	}
	else {
		CPoint pt = point;
		list_view_.ScreenToClient(&pt);
		UINT flags = LVHT_ONITEM;
		int item = list_view_.HitTest(pt,&flags);	

		if (item != -1) {
			list_view_.SetRedraw(FALSE);
			list_view_.SetItemState(-1,LVIS_SELECTED,0);
			list_view_.SetItemState(item,LVIS_SELECTED,LVIS_SELECTED);
			list_view_.SetRedraw();
			list_view_.Invalidate();

			theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_ITEM,point.x,point.y,&list_view_);
		}
		else
			theApp.m_pMainWnd->SendMessage(AFX_WM_TOOLBARMENU, 0,MAKELPARAM(point.x,point.y));
	}
}

void BibView::OnSearchTimerElapsed()
{
	StartSearch();
}

void BibView::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent) {
		case SearchTimerID:
			KillTimer(nIDEvent);
			search_timer_enabled_ = false;
			OnSearchTimerElapsed();
			break;
	}

	CDockablePane::OnTimer(nIDEvent);
}

void BibView::OnEnChangeSearch()
{
	if (search_timer_enabled_) {
		KillTimer(SearchTimerID);
		search_timer_enabled_ = false;
	}

	if (IsAttached()) { // A project has to be attached to this view
		SetTimer(SearchTimerID,SearchTimerTimeout,0);
		search_timer_enabled_ = true;
	}		
}

void BibView::OnClearSearch()
{
	search_button_->GetEditBox()->SetWindowText(0);
	Populate();
}

bool BibView::EveryItem( const BibItem& /*item*/ )
{
	return true;
}

bool BibView::Contains(const CString& text, const CString& search)
{
	temp_ = text;
	temp_.MakeLower();
	return temp_.Find(search) != -1;
}

bool BibView::ContainsAny( const CString& text, std::vector<CString>& tokens )
{
	std::vector<CString>::iterator it = tokens.begin();
	pos_buffer_.clear();

	// We need to find all matches. If we stop after the first match
	// then e.g. a search for an item with more than one author will fail
	// in case the input string contains partial names for a couple of authors
	for ( ; it != tokens.end() && tokens.size() != pos_buffer_.size(); ++it) {
		if (Contains(text,*it))
			pos_buffer_.push_back(it);
	}

	bool found = !pos_buffer_.empty();

	// and remove these
	for (std::vector<std::vector<CString>::iterator>::reverse_iterator it = pos_buffer_.rbegin();
		it != pos_buffer_.rend(); ++it)
		tokens.erase(*it);

	return found;
}

bool BibView::ItemContains( const BibItem& item, const std::vector<CString>& search_tokens )
{
	bool result = false;

	std::vector<CString>& tokens = token_buffer_;
	tokens = search_tokens;

	if (search_flags_ & Title)
		result = ContainsAny(item.GetTitle(),tokens);

	if (!tokens.empty() && search_flags_ & Author)
		result = ContainsAny(item.GetAuthor(),tokens);

	if (!tokens.empty() && search_flags_ & Year) {
		if (item.HasYear()) {
			CString year;
			year.Format(_T("%i"),item.GetYear());

			result = ContainsAny(year,tokens);
		}
	}

	if (!tokens.empty() && search_flags_ & Type)
		result = ContainsAny(item.GetType(),tokens);

	if (!tokens.empty() && search_flags_ & Publisher)
		result = ContainsAny(item.GetPublisher(),tokens);

	if (!tokens.empty())
		result = false; // Not all tokens have been matched
	
	// AND search
	//if (search_flags_ & Title)
	//	result = ContainsAny(item.GetTitle(),search_tokens);

	//if (search_flags_ & Author) {
	//	bool contains = ContainsAny(item.GetAuthor(),search_tokens);

	//	if (!(search_flags_ & Title)) // No other options than Author set
	//		result = contains;
	//	else
	//		result &= contains;
	//}

	//if (search_flags_ & Year) {
	//	bool contains;

	//	if (item.HasYear()) {
	//		CString year;
	//		year.Format(_T("%i"),item.GetYear());

	//		contains = ContainsAny(year,search_tokens);
	//	}
	//	else
	//		contains = false; // The year field doesn't exist

	//	if (!(search_flags_ & (Title|Author))) // // No other options than Year set
	//		result = contains;
	//	else
	//		result &= contains;
	//}

	return result;
}

void BibView::Populate( const std::tr1::function<bool (const BibItem&)>& predicate )
{
	const StructureItemContainer &a = GetProject()->m_aStructureItems;

	list_view_.SetRedraw(FALSE);
	list_view_.DeleteAllItems();
	//list_view_.RemoveAllGroups();

	std::tr1::hash<std::basic_string<TCHAR> > hasher;
	LVITEM lvi = {LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM|LVIF_GROUPID};

	LVGROUP lvg = {};
	lvg.mask = LVGF_HEADER|LVGF_GROUPID;

	if (RunTimeHelper::IsVista()) {
		lvg.cbSize = sizeof(LVGROUP);
		lvg.mask |= LVGF_STATE;
		lvg.state = LVGS_COLLAPSIBLE;
	}
	else // Windows XP compatibility
		lvg.cbSize = LVGROUP_V5_SIZE;

	for (StructureItemContainer::const_iterator it = a.begin(); it != a.end(); ++it) {
		const CStructureItem &si = *it;

		switch (si.GetType()) {
			case CStructureParser::bibFile :
			case CStructureParser::missingBibFile :
				{
					const CString& title = si.GetTitle();
					const_cast<LPCTSTR&>(lvg.pszHeader) = title;
					lvg.cchHeader = title.GetLength();
					lvg.iGroupId = static_cast<int>(hasher(std::basic_string<TCHAR>(title)));

					VERIFY(list_view_.InsertGroup(list_view_.GetGroupCount(),&lvg) != -1);
				}
				break;
			case CStructureParser::bibItem :
				if (const BibItem* info = dynamic_cast<const BibItem*>(it->GetItemInfo())) {
					if (predicate(*info)) {
						const CString& text = info->GetLabel();
						const_cast<LPCTSTR&>(lvi.pszText) = text;
						lvi.cchTextMax = text.GetLength();
						lvi.iGroupId = lvg.iGroupId;
						lvi.iItem = std::distance(a.begin(),it);
						lvi.iImage = si.GetType();
						lvi.lParam = lvi.iItem;

						const int index = list_view_.InsertItem(&lvi);
						ASSERT(index != -1);

						list_view_.SetItemText(index,1,info->GetAuthor());
						list_view_.SetItemText(index,2,info->GetTitle());

						if (info->HasYear()) {
							CString year;
							year.Format(_T("%i"),info->GetYear());

							list_view_.SetItemText(index,3,year);
						}
						list_view_.SetItemText(index,4,info->GetType());
						list_view_.SetItemText(index,5,info->GetPublisher());
					}
				}
				break;
		}
	}

	if (list_view_.IsSorted())
		list_view_.Sort();

	list_view_.SetRedraw();
	list_view_.Invalidate();
}

void BibView::Populate()
{
	Populate(EveryItem);
}

void BibView::OnLvnBeginDrag(NMHDR* nm, LRESULT*)
{
	NMLISTVIEW* p = reinterpret_cast<NMLISTVIEW*>(nm);
	ASSERT(p->iItem != -1);

	StructureItemContainer::size_type pos = list_view_.GetItemData(p->iItem);
	const CStructureItem& item = GetProject()->GetStructureItems()[pos];

	SetDraggedItem(new CStructureItem(item));
	DWORD result;
	DoDragDrop(DROPEFFECT_COPY,&result);
}

void BibView::ToggleSearchOption(unsigned opt)
{
	if (IsSearchOptionSet(opt))
		search_flags_ &= ~opt;
	else
		search_flags_ |= opt;
}

bool BibView::IsSearchOptionSet(unsigned opt) const
{
	return (search_flags_ & opt) == opt;
}

void BibView::OnSearchOptions(UINT id)
{
	switch (id) {
		case ID_AUTHOR:
			ToggleSearchOption(Author);
			break;
		case ID_TITLE:
			ToggleSearchOption(Title);
			break;
		case ID_YEAR:
			ToggleSearchOption(Year);
			break;
		case ID_PUBLICATION_TYPE:
			ToggleSearchOption(Type);
			break;
		case ID_PUBLISHER:
			ToggleSearchOption(Publisher);
			break;
	}
}

void BibView::OnUpdateSearchOptions(CCmdUI* pCmdUI)
{
	unsigned opt;

	switch (pCmdUI->m_nID) {
		case ID_AUTHOR:
			opt = Author;
			break;
		case ID_TITLE:
			opt = Title;
			break;
		case ID_YEAR:
			opt = Year;
			break;
		case ID_PUBLICATION_TYPE:
			opt = Type;
			break;
		case ID_PUBLISHER:
			opt = Publisher;
			break;
		default:
			ASSERT(FALSE);
			opt = 0;
	}

	pCmdUI->SetCheck(IsSearchOptionSet(opt));
}

int BibView::TranslateCompareFunction(LPARAM l1, LPARAM l2, BibView* pane,
									  const std::tr1::function<int (const CStructureItem& a, const CStructureItem& b)>& f)
{
	const StructureItemContainer& c = pane->GetProject()->GetStructureItems();
	ASSERT(l1 < c.size() && l2 < c.size());

	return f(c[l1],c[l2]);
}

int BibView::CompareLabel( const CStructureItem& a, const CStructureItem& b )
{
	return a.GetLabel().CompareNoCase(b.GetLabel());
}

int BibView::CompareAuthor( const CStructureItem& a, const CStructureItem& b )
{
	ASSERT(dynamic_cast<const BibItem*>(a.GetItemInfo()) &&
		dynamic_cast<const BibItem*>(b.GetItemInfo()));

	return static_cast<const BibItem*>(a.GetItemInfo())->GetAuthor().
		CompareNoCase(static_cast<const BibItem*>(b.GetItemInfo())->GetAuthor());
}

int BibView::CompareYear( const CStructureItem& a, const CStructureItem& b )
{
	ASSERT(dynamic_cast<const BibItem*>(a.GetItemInfo()) &&
		dynamic_cast<const BibItem*>(b.GetItemInfo()));

	const BibItem* b1 = static_cast<const BibItem*>(a.GetItemInfo());
	const BibItem* b2 = static_cast<const BibItem*>(b.GetItemInfo());

	int result;

	if (!b1->HasYear() && b2->HasYear())
		result = 0;
	else if (!b1->HasYear())
		result = 1;
	else if (!b2->HasYear())
		result = -1;
	else {
		int year1 = static_cast<const BibItem*>(a.GetItemInfo())->GetYear();
		int year2 = static_cast<const BibItem*>(b.GetItemInfo())->GetYear();
		
		result = year1 < year2 ? -1 : year1 > year2 ? 1 : 0;
	}

	return result;
}

int BibView::CompareTitle( const CStructureItem& a, const CStructureItem& b )
{
	ASSERT(dynamic_cast<const BibItem*>(a.GetItemInfo()) &&
		dynamic_cast<const BibItem*>(b.GetItemInfo()));

	return static_cast<const BibItem*>(a.GetItemInfo())->GetTitle().
		CompareNoCase(static_cast<const BibItem*>(b.GetItemInfo())->GetTitle());
}

int BibView::ComparePublicationType( const CStructureItem& a, const CStructureItem& b )
{
	ASSERT(dynamic_cast<const BibItem*>(a.GetItemInfo()) &&
		dynamic_cast<const BibItem*>(b.GetItemInfo()));

	return static_cast<const BibItem*>(a.GetItemInfo())->GetType().
		CompareNoCase(static_cast<const BibItem*>(b.GetItemInfo())->GetType());
}

int BibView::ComparePublisher( const CStructureItem& a, const CStructureItem& b )
{
	ASSERT(dynamic_cast<const BibItem*>(a.GetItemInfo()) &&
		dynamic_cast<const BibItem*>(b.GetItemInfo()));

	return static_cast<const BibItem*>(a.GetItemInfo())->GetPublisher().
		CompareNoCase(static_cast<const BibItem*>(b.GetItemInfo())->GetPublisher());
}

void BibView::StartSearch()
{
	if (CEdit* edit = search_button_->GetEditBox()) {
		CString text;
		edit->GetWindowText(text);

		text.Trim();

		if (text.IsEmpty())
			Populate(EveryItem);
		else {
			text.MakeLower();
			using namespace std::tr1;
			using namespace placeholders;

			std::vector<CString> words;
			Tokenize(text,_T(" "),words);

			Populate(bind(&BibView::ItemContains,this,_1,std::tr1::ref(words)));
		}
	}
}

void BibView::OnStartSearch()
{
	StartSearch();
}

void BibView::OnNMDblClk(NMHDR* /*nm*/, LRESULT*)
{
	GotoItem();
}

void BibView::OnEnter()
{
	if (GetFocus() == search_button_->GetEditBox()) {
		if (search_timer_enabled_) {
			KillTimer(SearchTimerID);
			search_timer_enabled_ = false;
		}

		StartSearch();
	}
	else {
		if (::GetKeyState(VK_CONTROL) >> 15 & 1) // Ctrl+enter pressed
			AfxGetMainWnd()->SendMessage(WM_COMMAND,ID_ITEM_INSERT_REF);
		else
			GotoItem();
	}
}

void BibView::GotoItem()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND,ID_ITEM_GOTO);
}

void BibView::OnLvnItemChanged(NMHDR* nm, LRESULT*)
{
	LPNMLISTVIEW p = reinterpret_cast<LPNMLISTVIEW>(nm);

	if (p->uChanged & LVIF_STATE && p->iItem != -1 && p->uNewState & LVIS_SELECTED)
		GetProject()->SetCurrentStructureItem(list_view_.GetItemData(p->iItem));
}
