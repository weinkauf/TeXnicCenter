#pragma once

#include <iostream>
#include <vector>

/// Represents a bookmark within a \ref CodeDocument.
class CodeBookmark
{
	int line_;
	CString name_;

public:
	explicit CodeBookmark(int line = -1);

	/// Returns the line number of the bookmark.
	///
	/// \return Line number.
	int GetLine() const;

	/// Sets the line number of the bookmark.
	void SetLine(int line);

	/// Sets bookmark's name.
	void SetName(LPCTSTR name);

	/// Gets bookmark's name.
	///
	/// \return Name of the bookmark.
	const CString& GetName() const;

	/// Indicates whether the bookmark has a name.
	///
	/// \return \c true if the bookmark has been named, \c false otherwise.
	bool HasName() const;
};


typedef std::vector<CodeBookmark> BookmarkContainerType;

template<class Ch, class Tr>
inline std::basic_ostream<Ch,Tr>& operator<<(std::basic_ostream<Ch,Tr>& out, const CodeBookmark& b)
{
	ASSERT(b.GetLine() != -1);
	out << b.GetLine();

	if (b.HasName()) {
		const Ch space = out.widen(' ');
		out << space << out.widen('N') << b.GetName().GetLength() << space << static_cast<const Ch*>(b.GetName());
	}
	
	return out;
}

template<class Ch, class Tr>
inline std::basic_istream<Ch,Tr>& operator>>(std::basic_istream<Ch,Tr>& in, CodeBookmark& b)
{
	b.SetName(0); // Clear the previous bookmark name

	int line;
	in >> line;

	if (in) {
		b.SetLine(line);

		Ch ch;

		if (in >> ch) {
			const Ch n = in.widen('N');

			if (ch == n) {
				int length;
				
				if (in >> length) {
					if (length <= 0)
						in.setstate(std::ios_base::failbit);
					else {
						in.get();

						std::vector<Ch> buffer(length + 1);
						in.read(&buffer[0],buffer.size());

						buffer[length] = 0;
						b.SetName(&buffer[0]);
					}
				}
			}
			else
				in.putback(ch);
		}
		else {
			in.clear(); // Clear the state flags, otherwise the last bookmark will be ignored
		}
	}

	return in;
}

/// Represents a folding point in a \ref CodeDocument.
class FoldingPoint
{
	CodeBookmark bookmark_;
	bool contracted_;

public:
	explicit FoldingPoint(int line = -1, bool contracted = false);

	int GetLine() const;
	void SetLine(int line);

	bool IsContracted() const;
	void SetContracted(bool contracted);

	bool operator==(const FoldingPoint& o) const;

	template<class Ch, class Tr>
	friend std::basic_ostream<Ch,Tr>& operator<<(std::basic_ostream<Ch,Tr>&, const FoldingPoint&);

	template<class Ch, class Tr>
	friend std::basic_istream<Ch,Tr>& operator>>(std::basic_istream<Ch,Tr>&, FoldingPoint&);
};


typedef std::vector<FoldingPoint> FoldingPointContainerType;

template<class Ch, class Tr>
inline std::basic_ostream<Ch,Tr>& operator<<(std::basic_ostream<Ch,Tr>& out, const FoldingPoint& b)
{
	return out << b.bookmark_/* << out.widen(' ') << b.IsContracted()*/;
}

template<class Ch, class Tr>
inline std::basic_istream<Ch,Tr>& operator>>(std::basic_istream<Ch,Tr>& in, FoldingPoint& b)
{
	in >> b.bookmark_;
	
	//bool contracted;
	//in >> contracted;

	//if (in)
	//	b.SetContracted(contracted);

	return in;
}

bool operator==(const CodeBookmark& a, const CodeBookmark& b);
bool operator!=(const CodeBookmark& a, const CodeBookmark& b);
bool operator!=(const FoldingPoint& a, const FoldingPoint& b);
bool operator<(const CodeBookmark& a, const CodeBookmark& b);
bool operator>(const CodeBookmark& a, const CodeBookmark& b);
bool operator<=(const CodeBookmark& a, const CodeBookmark& b);
bool operator>=(const CodeBookmark& a, const CodeBookmark& b);