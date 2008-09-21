#pragma once

#include <memory>

#include "OutputFilter.h"

class BibTeXOutputFilter : 
	public COutputFilter
{
	std::auto_ptr<COutputInfo> last_entry_;
	const tregex warning_line_;
	const tregex error_line_;

public:
	BibTeXOutputFilter();
	~BibTeXOutputFilter();

protected:
	DWORD ParseLine(const CString& line, DWORD cookie);
	BOOL OnTerminate();
};
