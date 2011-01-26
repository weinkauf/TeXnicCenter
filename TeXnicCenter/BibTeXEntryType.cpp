#include "stdafx.h"

#include <algorithm>
#include <stdexcept>

#include "BibTeXEntryType.h"

LPCTSTR const names[] = {
	_T("Article"),
	_T("Artwork"),
	_T("Audio"),
	_T("Book"),
	_T("Bookinbook"),
	_T("Booklet"),
	_T("Collection"),
	_T("Comment"),
	_T("Commentary"),
	_T("Conference"),
	_T("Custom A"),
	_T("Custom B"),
	_T("Custom C"),
	_T("Custom D"),
	_T("Custom E"),
	_T("Custom F"),
	_T("Electronic"),
	_T("Image"),
	_T("Inbook"),
	_T("Incollection"),
	_T("Inproceedings"),
	_T("Inreference"),
	_T("Jurisdiction"),
	_T("Legal"),
	_T("Legislation"),
	_T("Letter"),
	_T("Manual"),
	_T("Masterthesis"),
	_T("Misc"),
	_T("Movie"),
	_T("Music"),
	_T("Online"),
	_T("Patent"),
	_T("Performance"),
	_T("Periodical"),
	_T("PhdThesis"),
	_T("Preamble"),
	_T("Proceedings"),
	_T("Reference"),
	_T("Report"),
	_T("Review"),
	_T("Set"),
	_T("Software"),
	_T("Standard"),
	_T("String"),
	_T("Suppbook"),
	_T("Suppcollection"),
	_T("Suppperiodical"),
	_T("Techreport"),
	_T("Thesis"),
	_T("Unpublished"),
	_T("Video"),
	_T("WWW")
};

const int nameCount = sizeof(names) / sizeof(*names);

CString GetString(BibTeXEntryType type)
{
	int index = type - 1;

	return index >= 0 && index < nameCount ? names[index]  : CString();
}

BibTeXEntryType GetBibTeXEntryTypeFromString(const CString& text)
{
	// Perform binary search
	const LPCTSTR* end = names + nameCount;
	const LPCTSTR* pos = std::lower_bound(names, end, text,
		[] (LPCTSTR val1, LPCTSTR val2) { return _tcsicmp(val1, val2) < 0; }
	);

	if (pos == end)
		throw std::invalid_argument("Invalid BibTeX entry type text");

	return static_cast<BibTeXEntryType>(std::distance(names, pos) + 1);
}
