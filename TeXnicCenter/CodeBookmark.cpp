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

FoldingPoint::FoldingPoint( int line /*= -1*/, bool contracted /*= false*/ ) 
: bookmark_(line), contracted_(contracted)
{
}

int FoldingPoint::GetLine() const
{
	return bookmark_.GetLine();
}

void FoldingPoint::SetLine( int line )
{
	bookmark_.SetLine(line);
}

bool FoldingPoint::IsContracted() const
{
	return contracted_;
}

void FoldingPoint::SetContracted( bool contracted )
{
	contracted_ = contracted;
}

bool FoldingPoint::operator==( const FoldingPoint& o ) const
{
	return bookmark_ == o.bookmark_;
}

