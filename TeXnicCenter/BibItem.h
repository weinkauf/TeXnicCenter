#pragma once

#include <vector>

#include "StructureItemInfo.h"
#include "Nullable.h"

/// Represents a bibliography item used in a \ref BibView.
class BibItem :
	public StructureItemInfo
{
	friend class BibTeXEntry;

	CString type_text_;
	CString author_;
	CString editor_;
	Nullable<int> year_;
	CString label_;
	CString title_;
	CString publisher_;

public:
	// Known BibTeX types in alphabetical order
	enum Type {
		Unknown = -1,
		Article,
		Book,
		Booklet,
		Comment,
		Conference,
		Inbook,
		Incollection,
		Inproceedings,
		Manual,
		MastersThesis,
		Misc,
		PhDThesis,
		Preamble,
		Proceedings,
		String,
		Techreport,
		Unpublished
	};

	typedef std::vector<CString> AuthorContainerType;
	// Extracted author names, useful for grouping
	AuthorContainerType authors_;

private:
	Type type_;

public:
	BibItem();
	~BibItem();

	Type GetType() const;
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
};