#pragma once

#include <vector>

#include "BibTeXEntryType.h"
#include "Nullable.h"
#include "StructureItemInfo.h"

/**
 * @brief Represents a bibliography item used in a @ref BibView.
 */
class BibItem :
	public StructureItemInfo
{
public:
	/**
	 * @brief Container type that holds author names.
	 */
	typedef std::vector<CString> AuthorContainerType;

	BibItem();
	~BibItem();

	BibTeXEntryType GetType() const;
	const CString& GetTypeString() const;
	const CString& GetAuthor() const;
	const CString& GetEditor() const;
	const Nullable<int>& GetYear() const;
	const CString& GetLabel() const;
	const CString& GetTitle() const;
	const CString& GetPublisher() const;
	const AuthorContainerType& GetAuthors() const;

	bool HasYear() const;
	bool HasEditor() const;

private:
	friend class BibTeXEntry;

	/**
	 * @brief Extracted author names, useful for grouping.
	 */
	AuthorContainerType authors_;
	BibTeXEntryType type_;

	CString type_text_;
	CString author_;
	CString editor_;
	Nullable<int> year_;
	CString label_;
	CString title_;
	CString publisher_;
};
