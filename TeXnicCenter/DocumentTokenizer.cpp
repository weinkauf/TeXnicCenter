#include "stdafx.h"
#include "DocumentTokenizer.h"

DocumentTokenizer::~DocumentTokenizer()
{
}

bool DocumentTokenizer::NextWord( const CString& text, int& start, int& end )
{
	return NextWord(text,text.GetLength(),start,end);
}

void DocumentTokenizer::ResetState()
{
}