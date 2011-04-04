#pragma once

#include "EventArgs.h"
#include "CodeBookmark.h"

class BookmarkEventArgs : public EventArgs
{
	const CodeBookmark b_;
	const CString filename_;

public:
	BookmarkEventArgs(const CString& filename, const CodeBookmark& b);

	const CodeBookmark& GetBookmark() const;
	const CString& GetFileName() const;
};