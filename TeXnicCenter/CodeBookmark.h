#pragma once

#include <iostream>
#include <vector>

class CodeBookmark
{
	int line_;

public:
	explicit CodeBookmark(int line = -1);

	int GetLine() const;
	void SetLine(int line);
};


typedef std::vector<CodeBookmark> BookmarkContainerType;

template<class Ch, class Tr>
inline std::basic_ostream<Ch,Tr>& operator<<(std::basic_ostream<Ch,Tr>& out, const CodeBookmark& b)
{
	ASSERT(b.GetLine() != -1);
	return out << b.GetLine();
}

template<class Ch, class Tr>
inline std::basic_istream<Ch,Tr>& operator>>(std::basic_istream<Ch,Tr>& in, CodeBookmark& b)
{
	int line;
	in >> line;

	if (in)
		b.SetLine(line);

	return in;
}

inline bool operator==(const CodeBookmark& a, const CodeBookmark& b)
{
	return a.GetLine() == b.GetLine();
}