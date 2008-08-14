#pragma once
#include "DocumentTokenizer.h"

class LaTeXTokenizer :
	public DocumentTokenizer
{
	TCHAR brace_encountered_;

public:
	LaTeXTokenizer();

	bool NextWord(LPCTSTR text, int length, int& start, int& end);
	void ResetState();

private:
	static bool IsCmdAt(LPCTSTR lpText, LPCTSTR lpPos);
};
