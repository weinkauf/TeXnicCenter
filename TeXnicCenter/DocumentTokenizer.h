#pragma once

class DocumentTokenizer
{
public:
	virtual ~DocumentTokenizer();

	// Base NextWord function
	virtual bool NextWord(LPCTSTR text, int length, int& start, int& end) = 0;
	// Calls the base NextWord
	virtual bool NextWord(const CString& text, int& start, int& end);

	virtual void ResetState();
};
