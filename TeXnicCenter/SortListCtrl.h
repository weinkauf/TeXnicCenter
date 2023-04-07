#pragma once

#include <map>
#include <functional>

// SortListCtrl

class SortListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(SortListCtrl)

private:
	int sort_column_;
	bool ascending_;

public:
	SortListCtrl();
	virtual ~SortListCtrl();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLvnColumnClick(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void PreSubclassWindow();

private:
	void Initialize();
	void SetColumnSort(int column, bool ascending);
	void RemoveColumnSort(int column);

public:
	typedef std::function<int (LPARAM, LPARAM)> CompareFunctionType;
	typedef std::map<int,CompareFunctionType> CompareFunctionContainerType;

private:
	CompareFunctionContainerType compare_functions_;

public:
	void SetColumnCompareFunction(int column, const CompareFunctionType& func);
	void RemoveColumnCompareFunction(int column);

private:
	struct BinaryFunctionResultNegate :
		public std::binary_function<LPARAM,LPARAM,int>
	{
		const CompareFunctionType& func;
		
		explicit BinaryFunctionResultNegate(const CompareFunctionType& f)
		: func(f)
		{
		}
		
		result_type operator()(first_argument_type a, second_argument_type b) const
		{
			return -func(a,b);
		}
	};

	static int CALLBACK DoCompare(LPARAM l1, LPARAM l2, LPARAM p);

public:
	int GetSortColumn() const;
	bool IsSortAscending() const;
	bool IsSortDescending() const;
	void Sort(int column, bool ascending);
	void Sort();
	bool IsSorted() const;
};


