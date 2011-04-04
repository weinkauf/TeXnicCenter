// SortListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "SortListCtrl.h"

// SortListCtrl

IMPLEMENT_DYNAMIC(SortListCtrl, CListCtrl)

SortListCtrl::SortListCtrl()
{

}

SortListCtrl::~SortListCtrl()
{
}


BEGIN_MESSAGE_MAP(SortListCtrl, CListCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, &SortListCtrl::OnLvnColumnClick)
END_MESSAGE_MAP()


// SortListCtrl message handlers

int SortListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	Initialize();
	return 0;
}

void SortListCtrl::OnLvnColumnClick(NMHDR *pNMHDR, LRESULT* /*pResult*/)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	int column = pNMLV->iSubItem;
	
	if (sort_column_ == column)
		ascending_ = !ascending_;
	else {
		// Sort column has been changed, reset the sort order
		ascending_ = true;
	}

	SetRedraw(FALSE);
	Sort(column,ascending_);
	SetRedraw();
	Invalidate();
}

void SortListCtrl::PreSubclassWindow()
{
	Initialize();
	CListCtrl::PreSubclassWindow();
}

void SortListCtrl::Initialize()
{
	sort_column_ = -1;
	ascending_ = true;
	compare_functions_.clear();
}

void SortListCtrl::SetColumnSort( int column, bool ascending )
{
	if (CHeaderCtrl* h = GetHeaderCtrl()) {
		HDITEM hdi = {HDI_FORMAT};
		VERIFY(h->GetItem(column,&hdi));

		hdi.fmt &= ~(!ascending ? HDF_SORTUP : HDF_SORTDOWN);
		hdi.fmt |= ascending ? HDF_SORTUP : HDF_SORTDOWN;

		VERIFY(h->SetItem(column,&hdi));
	}
}

void SortListCtrl::RemoveColumnSort( int column )
{
	if (CHeaderCtrl* h = GetHeaderCtrl()) {
		HDITEM hdi = {HDI_FORMAT};
		VERIFY(h->GetItem(column,&hdi));

		hdi.fmt &= ~(HDF_SORTUP | HDF_SORTDOWN);
		VERIFY(h->SetItem(column,&hdi));
	}
}

void SortListCtrl::SetColumnCompareFunction( int column, const CompareFunctionType& func )
{
	ASSERT(column >= 0 && column < GetHeaderCtrl()->GetItemCount());
	compare_functions_.insert(std::make_pair(column,func));
}

void SortListCtrl::RemoveColumnCompareFunction( int column )
{
	CompareFunctionContainerType::iterator it = compare_functions_.find(column);
	ASSERT(it != compare_functions_.end());

	if (it != compare_functions_.end())
		compare_functions_.erase(it);
}

int SortListCtrl::DoCompare( LPARAM l1, LPARAM l2, LPARAM p )
{
	return (*reinterpret_cast<CompareFunctionType*>(p))(l1,l2);
}

int SortListCtrl::GetSortColumn() const
{
	return sort_column_;
}

bool SortListCtrl::IsSortAscending() const
{
	return ascending_;
}

bool SortListCtrl::IsSortDescending() const
{
	return !IsSortAscending();
}

void SortListCtrl::Sort( int column, bool ascending )
{
	ascending_ = ascending;

	if (sort_column_ == column)
		SetColumnSort(sort_column_,ascending_);
	else {
		// Previous column was valid but the new one is not
		if (sort_column_ != -1 || sort_column_ != -1 && column == -1)
			RemoveColumnSort(sort_column_);

		if (column != -1)
			SetColumnSort(column,ascending_);

		sort_column_ = column;
	}

	if (sort_column_ != -1) {
		CompareFunctionContainerType::iterator it = compare_functions_.find(sort_column_);

		if (it != compare_functions_.end()) {
			CompareFunctionType func = it->second;

			if (!ascending_)
				func = BinaryFunctionResultNegate(it->second);
			
			SortItems(DoCompare,reinterpret_cast<LPARAM>(&func));
		}
	}
}

void SortListCtrl::Sort()
{
	ASSERT(IsSorted());
	Sort(GetSortColumn(),IsSortAscending());
}
bool SortListCtrl::IsSorted() const
{
	return GetSortColumn() != -1;
}