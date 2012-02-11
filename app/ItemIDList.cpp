#include <windows.h>

#include <cstring>
#include <stdexcept>
#include <type_traits>

#include "ItemIDList.h"

namespace {
	template<class T>
	T GetNextItemIDList(T pidl)
	{
		typedef std::conditional<
				std::is_const<typename std::remove_pointer<T>::type>::value,
				LPCBYTE,
				LPBYTE
			>::type Pointer;

		return pidl->mkid.cb == 0 ? NULL : reinterpret_cast<T>
			(reinterpret_cast<Pointer>(pidl) + pidl->mkid.cb);
	}
}

ItemIDList::ItemIDList(const ItemIDList& other) 
	: pidl_(NULL)
{
	if (other.pidl_) {
		std::size_t n = other.GetSize();
		pidl_ = static_cast<LPITEMIDLIST>(::CoTaskMemAlloc(n));
		std::memcpy(pidl_, other.pidl_, n);
	}
}

ItemIDList::ItemIDList(LPCITEMIDLIST pidl)
	: pidl_(NULL)
{
	if (pidl)
		Copy(pidl);
}

#if _MSC_VER >= 1600
ItemIDList::ItemIDList(ItemIDList&& other) 
	: pidl_(other.pidl_)
{
	other.pidl_ = NULL;
}
#endif // _MSC_VER >= 1600

ItemIDList::~ItemIDList()
{
	Free();
}

LPCITEMIDLIST ItemIDList::Next(LPCITEMIDLIST pidl)
{
	return GetNextItemIDList(pidl);
}

LPITEMIDLIST ItemIDList::Next(LPITEMIDLIST pidl)
{
	return GetNextItemIDList(pidl);
}

std::size_t ItemIDList::GetSize() const
{
	std::size_t n = 0;

	if (pidl_)
		n = ILGetSize(pidl_);

	return n;
}

void ItemIDList::Assign(LPCITEMIDLIST pidl)
{
	assert(pidl);

	Free();
	Copy(pidl);
}

ItemIDList ItemIDList::GetParent() const
{
	assert(pidl_);

	ItemIDList result(*this);

	LPITEMIDLIST pidl = result.pidl_;
	LPITEMIDLIST next = pidl;

	while ((next = Next(next)) != NULL)
		pidl = next;

	pidl->mkid.cb = 0;

	return result;
}

ItemIDList ItemIDList::GetLast() const
{
	assert(pidl_);

	LPITEMIDLIST pidl = pidl_;
	LPITEMIDLIST next = pidl;

	while ((next = Next(next)) != NULL)
		pidl = next;

	ItemIDList result;
	result.Assign(pidl);

	return result;
}

void ItemIDList::Swap(ItemIDList& other)
{
	std::swap(pidl_, other.pidl_);
}

ItemIDList& ItemIDList::operator=(const ItemIDList& other)
{
	ItemIDList tmp(other);
	tmp.Swap(*this);

	return *this;
}

ItemIDList& ItemIDList::operator=(ItemIDList&& other)
{
	Swap(other);
	other.Clear();

	return *this;
}

LPITEMIDLIST const* ItemIDList::operator&() const
{
	return &pidl_;
}

LPITEMIDLIST* ItemIDList::operator&()
{
	return &pidl_;
}

ItemIDList& ItemIDList::operator+=(const ItemIDList& other)
{
	Concat(other);
	return *this;
}

ItemIDList::operator LPITEMIDLIST()
{
	return pidl_;
}

ItemIDList::operator LPCITEMIDLIST() const
{
	return pidl_;
}

void ItemIDList::Free()
{
	if (pidl_) {
		::CoTaskMemFree(pidl_);
		pidl_ = NULL;
	}
}

void ItemIDList::Concat(const ItemIDList& other)
{
	assert(pidl_ && other.pidl_);

	std::size_t n1 = GetSize() - sizeof(pidl_->mkid.cb);
	std::size_t n2 = other.GetSize();

	LPITEMIDLIST pidl = static_cast<LPITEMIDLIST>(::CoTaskMemAlloc(n1 + n2));
	assert(pidl);

	std::memcpy(pidl, pidl_, n1);
	std::memcpy(reinterpret_cast<LPBYTE>(pidl) + n1, other.pidl_, n2);

	Assign(pidl);
}

bool ItemIDList::IsEmpty() const
{
	return pidl_ == NULL;
}

void ItemIDList::Copy(LPCITEMIDLIST pidl)
{
	std::size_t n = ILGetSize(pidl);
	pidl_ = static_cast<LPITEMIDLIST>(::CoTaskMemAlloc(n));

	assert(pidl_);
	std::memcpy(pidl_, pidl, n);
}

void ItemIDList::Attach(LPITEMIDLIST pidl)
{
	Free();
	assert(pidl_ == NULL);
	pidl_ = pidl;
}

void ItemIDList::Detach()
{
	pidl_ = NULL;
}

ItemIDList ItemIDList::FromPath(const wchar_t* path)
{
	ItemIDList result;

	if (FAILED(SHILCreateFromPath(path, &result, NULL)))
		throw std::invalid_argument("Failed to convert the specified path");

	return result;
}

void ItemIDList::Clear()
{
	Free();
}

ItemIDList operator+(const ItemIDList& lhs, const ItemIDList& rhs)
{
	ItemIDList result(lhs);
	result += rhs;

	return result;
}

bool operator==(const ItemIDList& lhs, const ItemIDList& rhs)
{
	return ILIsEqual(lhs, rhs) == TRUE;
}

bool operator!=(const ItemIDList& lhs, const ItemIDList& rhs)
{
	return !(lhs == rhs);
}

bool operator<(const ItemIDList& lhs, const ItemIDList& rhs)
{
	return lhs.GetSize() < rhs.GetSize() || lhs.GetSize() == rhs.GetSize() &&
		std::memcmp(lhs, rhs, lhs.GetSize()) < 0;
}

bool operator>(const ItemIDList& lhs, const ItemIDList& rhs)
{
	return lhs.GetSize() > rhs.GetSize() || lhs.GetSize() == rhs.GetSize() &&
		std::memcmp(lhs, rhs, lhs.GetSize()) > 0;
}

bool operator<=(const ItemIDList& lhs, const ItemIDList& rhs)
{
	return lhs < rhs || lhs == rhs;
}

bool operator>=(const ItemIDList& lhs, const ItemIDList& rhs)
{
	return lhs > rhs || lhs == rhs;
}

void swap(ItemIDList& lhs, ItemIDList& rhs)
{
	lhs.Swap(rhs);
}
