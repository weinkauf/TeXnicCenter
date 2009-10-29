#include "stdafx.h"
#include "BibItem.h"

BibItem::BibItem()
: has_year_(false)
, type_(Unknown)
{
}

BibItem::~BibItem()
{
}

BibItem::Type BibItem::GetType() const
{
	return type_;
}

const CString& BibItem::GetTypeString() const
{
	return type_text_;
}

const CString& BibItem::GetAuthor() const
{
	return author_;
}

const CString& BibItem::GetEditor() const
{
	return editor_;
}

int BibItem::GetYear() const
{
	ASSERT(HasYear()); return year_;
}

const CString& BibItem::GetLabel() const
{
	return label_;
}

const CString& BibItem::GetTitle() const
{
	return title_;
}

const CString& BibItem::GetPublisher() const
{
	return publisher_;
}

const BibItem::AuthorContainerType& BibItem::GetAuthors() const
{
	return authors_;
}

bool BibItem::HasYear() const
{
	return has_year_;
}

bool BibItem::HasEditor() const
{
	return !editor_.IsEmpty();
}