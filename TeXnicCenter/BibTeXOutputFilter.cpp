#include "stdafx.h"
#include "resource.h"

#include "BibTeXOutputFilter.h"

BibTeXOutputFilter::BibTeXOutputFilter()
: warning_line_(_T("--line (\\d+) of file (.+)"))
, error_line_(_T("([^-]*)---line (\\d+) of file (.+)"))
, database_file_(_T("Database file #\\d+:\\s+(.*)"))
{
}

BibTeXOutputFilter::~BibTeXOutputFilter()
{
}

DWORD BibTeXOutputFilter::ParseLine(const CString& line, DWORD cookie)
{
	const CString warning(_T("Warning--"));
	std::tr1::match_results<LPCTSTR> matches;

	if (line.GetLength() >= warning.GetLength() && line.Left(warning.GetLength()) == warning) {
		COutputInfo oi;
		oi.SetErrorMessage(line.Mid(warning.GetLength()));
		oi.SetOutputLine(GetCurrentOutputLine());

		if (fileNames_.size() == 1)
			oi.SetSourceFile(fileNames_.front());

		if (last_entry_.get())
			AddWarning(*last_entry_);

		last_entry_.reset(new COutputInfo(oi));
	}
	else if (last_entry_.get() && regex_match(static_cast<LPCTSTR>(line),matches,warning_line_)) {
		last_entry_->SetSourceLine(_ttoi(matches.str(1).c_str()));
		last_entry_->SetSourceFile(matches.str(2).c_str());

		AddWarning(*last_entry_);
		last_entry_.reset();
	}
	else if (regex_match(static_cast<LPCTSTR>(line),matches,error_line_)) {
		COutputInfo oi;
		oi.SetErrorMessage(matches.str(1).c_str());
		oi.SetSourceLine(_ttoi(matches.str(2).c_str()));
		oi.SetSourceFile(matches.str(3).c_str());
		oi.SetOutputLine(GetCurrentOutputLine());

		AddError(oi);
	}
	else if (regex_match(static_cast<LPCTSTR>(line), matches, database_file_)) {
		CString fileName(matches.str(1).c_str());
		fileNames_.push_back(fileName);
	}

	return cookie;
}

BOOL BibTeXOutputFilter::OnTerminate()
{
	if (last_entry_.get()) {
		AddWarning(*last_entry_);
		last_entry_.reset();
	}

	return TRUE;
}

CString BibTeXOutputFilter::GetResultString()
{
	CString result;
	result.Format(IDS_BIBTEX_RESULT,GetErrorCount(),GetWarningCount());

	return result;
}