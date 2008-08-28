#pragma once

#include <vector>

#include "DocumentTokenizer.h"

class LaTeXTokenizer :
	public DocumentTokenizer
{
	typedef std::vector<TCHAR> BraceContainer;
	BraceContainer braces_;

	static TCHAR GetClosingBrace(TCHAR open);

public:
	LaTeXTokenizer();

	bool NextWord(LPCTSTR text, int length, int& start, int& end);
	void ResetState();

private:
	static bool IsCmdAt(LPCTSTR lpText, LPCTSTR lpPos);
};
