#include "stdafx.h"
#include "BookmarkEventArgs.h"

BookmarkEventArgs::BookmarkEventArgs( const CString& filename, const CodeBookmark& b ) : b_(b), filename_(filename)
{

}

const CodeBookmark& BookmarkEventArgs::GetBookmark() const
{
	return b_;
}

const CString& BookmarkEventArgs::GetFileName() const
{
	return filename_;
}