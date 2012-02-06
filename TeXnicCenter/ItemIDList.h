#pragma once

#include <shlobj.h>
#include <shobjidl.h>

#include <cassert>
#include <cstddef>
#include <functional>
#include <memory>
#include <utility>

/**
 * @brief Wrapper around @c LPITEMIDLIST.
 * @author Sergiu Dotenco
 */
class ItemIDList
{
public:
	/**
	 * @brief Initializes a new instance of @ref ItemIDList class and performs
	 *		  a deep-copy of the specified @a pidl.
	 */
	explicit ItemIDList(LPCITEMIDLIST pidl = NULL);

	ItemIDList(const ItemIDList& other);

#if _MSC_VER >= 1600
	ItemIDList(ItemIDList&& other);
#endif // _MSC_VER >= 1600

	~ItemIDList();

	/**
	 * @brief Returns the successor item of the specified @a pidl.
	 * 
	 * @param pidl The item whose successor should be returned.
	 */
	static LPCITEMIDLIST Next(LPCITEMIDLIST pidl);
	static LPITEMIDLIST Next(LPITEMIDLIST pidl);

	/**
	 * @brief Returns the number of bytes required to hold the item structure
	 *		  in a single contiguous memory block.
	 */
	std::size_t GetSize() const;

	/**
	 * @brief Gets value indicating whether the list is empty.
	 */
	bool IsEmpty() const;

	/**
	 * @brief Assigns the specified @a pidl to this instance by copying it.
	 *
	 * @param pidl The item to be assigned.
	 */
	void Assign(LPCITEMIDLIST pidl);

	/**
	 * @brief Returns the parent item.
	 */
	ItemIDList GetParent() const;

	/**
	 * @brief Returns the last item in the list.
	 */
	ItemIDList GetLast() const;

	/**
	 * @brief Swaps the instance with the @a other.
	 */
	void Swap(ItemIDList& other);

	void Attach(LPITEMIDLIST pidl);

	void Clear();
	
	/**
	 * @brief Releases ownership over the hold @c LPITEMIDLIST.
	 */
	void Detach();

	/**
	 * @brief Converts the specified @a path to an @ref ItemIDList instance.
	 * 
	 * @param path The path to be converted.
	 * 
	 * @exception std::invalid_argument Thrown in case the conversion failed.
	 */
	static ItemIDList FromPath(const wchar_t* path);

	LPITEMIDLIST* operator&();
	LPITEMIDLIST const* operator&() const;
	operator LPITEMIDLIST();
	operator LPCITEMIDLIST() const;

	ItemIDList& operator=(const ItemIDList& other);
#if _MSC_VER >= 1600
	ItemIDList& operator=(ItemIDList&& other);
#endif // _MSC_VER >= 1600
	ItemIDList& operator+=(const ItemIDList& other);

private:
	void Concat(const ItemIDList& other);
	void Copy(LPCITEMIDLIST pidl);
	void Free();

	LPITEMIDLIST pidl_;
};

ItemIDList operator+(const ItemIDList& lhs, const ItemIDList& rhs);
bool operator==(const ItemIDList& lhs, const ItemIDList& rhs);
bool operator!=(const ItemIDList& lhs, const ItemIDList& rhs);
bool operator<(const ItemIDList& lhs, const ItemIDList& rhs);
bool operator>(const ItemIDList& lhs, const ItemIDList& rhs);
bool operator<=(const ItemIDList& lhs, const ItemIDList& rhs);
bool operator>=(const ItemIDList& lhs, const ItemIDList& rhs);

namespace std {
	template<>
	inline void swap(ItemIDList& lhs, ItemIDList& rhs)
	{
		lhs.Swap(rhs);
	}

	template<>
	struct hash<ItemIDList>
		: public unary_function<ItemIDList, size_t>
	{
		size_t operator()(const ItemIDList& value) const
		{
			LPCITEMIDLIST pidl = value;

			const unsigned char* s = reinterpret_cast<const unsigned char*>(pidl);
			const unsigned char* e = reinterpret_cast<const unsigned char*>(pidl) + value.GetSize();

#ifdef WIN64
			size_t hash = 14695981039346656037;
			const size_t prime = 1099511628211;
#else
			size_t hash = 16777619;
			const size_t prime = 2166136261;
#endif

			// Fowler-Noll-Vo FNV-1 hash
			for ( ; s != e; ++s)
				hash = prime * *s ^ hash;

			return hash;
		}
	};
}

void swap(ItemIDList& lhs, ItemIDList& rhs);
