#include "stdafx.h"
#include "CodeBookmark.h"

CodeBookmark::CodeBookmark(int line)
: line_(line)
{
}

int CodeBookmark::GetLine() const
{
	return line_;
}

void CodeBookmark::SetLine( int line )
{
	ASSERT(line >= -1);
	line_ = line;
}