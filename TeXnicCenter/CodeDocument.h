#pragma once
#include <utility>
#include <cstddef>

#include "CodeBookmark.h"

int ShowSaveTaskDialog(LPCTSTR prompt);
int ToScintillaMode(int m);
int FromScintillaMode(int m);

class SpellerBackgroundThread;
class TextDocument;

#pragma region Line ending detection helpers

template<class Ch>
struct LineEndingTraits;

template<>
struct LineEndingTraits<char>
{
	static const char* Endings() { return "\r\n"; }
	static const char* LineFeed() { return "\n"; }
	static const char* CarriageReturn() { return "\r"; }
};

template<>
struct LineEndingTraits<wchar_t>
{
	static const wchar_t* Endings() { return L"\r\n"; }
	static const wchar_t* LineFeed() { return L"\n"; }
	static const wchar_t* CarriageReturn() { return L"\r"; }
};

template<class Ch>
inline const Ch* GetLineEnding(const Ch* text, std::size_t size)
{
	const Ch* const le = LineEndingTraits<Ch>::Endings();
	const Ch* mode = le;
	const Ch* end = text + size;

	const Ch* pos = std::find_first_of(text,end,le,le + std::char_traits<Ch>::length(le));

	if (pos != end) {
		if (*pos == *LineEndingTraits<Ch>::CarriageReturn() && (pos + 1 == end || pos[1] != *LineEndingTraits<Ch>::LineFeed()))
			mode = LineEndingTraits<Ch>::CarriageReturn();
		else if (std::char_traits<Ch>::compare(pos,le,std::min<std::size_t>(size - (pos - text),
				std::char_traits<Ch>::length(le))) != 0 && 
			*pos == *LineEndingTraits<Ch>::LineFeed())
			mode = LineEndingTraits<Ch>::LineFeed();
	}

	return mode;
}

#pragma endregion

class CodeDocument : public CScintillaDoc
{
	DECLARE_DYNAMIC(CodeDocument)

	/// Handle to the error marker or -1 if invalid
	int error_marker_handle_;
	/// Specifies whether a BOM has been found while
	/// opening the document or should be saved with it
	bool use_bom_;

	CSemaphore semaphore_;

public:
	enum Encoding
	{
		ANSI,
		UTF8,
		UTF16LE,
		UTF16BE,
		UTF32LE,
		UTF32BE
	};

	enum Marker
	{
		Errormark,
		Bookmark
	};

private:
	Encoding encoding_;

	UINT code_page_;
	bool create_backup_file_;

	SpellerBackgroundThread* speller_thread_;

protected:
	CodeDocument();
	virtual ~CodeDocument();

public:
#ifndef _WIN32_WCE
	void Serialize(CArchive& ar);   // overridden for document i/o
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	void SetModifiedFlag(BOOL modified = TRUE);

	virtual DWORD LoadFile(HANDLE file);
	DWORD LoadFile(LPCTSTR pszFileName);

	/// Writes the document to the specified file
	/// \param throw_on_invalid_sequence
	///		   Throws an InvalidSequenceEncountered exception
	///		   if any of document's characters cannot be represented
	///		   in the specified encoding
	virtual DWORD SaveFile(HANDLE file, bool throw_on_invalid_sequence);	
	DWORD SaveFile(HANDLE file, const char* text, std::size_t n);
	DWORD SaveFile(LPCTSTR pszFileName, bool bClearModifiedFlag = true);

	Encoding GetEncoding() const;
	void SetEncoding(Encoding e);

	UINT GetCodePage() const;
	void SetSpellerThread(SpellerBackgroundThread* t);
	SpellerBackgroundThread* GetSpellerThread() const;
	bool IsSpellerThreadAttached() const;
	void SetErrorMark(int line);
	bool HasErrorMark() const;
	BOOL SaveModified();

private:
	// Vista Task Dialog implementation
	BOOL DoSaveModified();

protected:
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);

	afx_msg void OnEditToggleBookmark();
	afx_msg void OnEditClearAllBookmarks();

	bool ShowSaveDialog(LPCTSTR file) const;
	
	virtual void OnBookmarkAdded(const ::CodeBookmark& b);
	virtual void OnBookmarkRemoved(const ::CodeBookmark& b);
	virtual void OnRemovedAllBookmarks(void);

	afx_msg void OnEditSplitParagraph();
	afx_msg void OnEditJoinParagraph();

public:
	/// Removes all bookmarks that have been set
	void RemoveAllBookmarks();
	/// Sets or removes a bookmark on the specified line
	void ToggleBookmark( int line );
	/// Returns a container of CodeBookmarks that have been set for this document
	const BookmarkContainerType GetBookmarks();
	/// Clears all the bookmarks and sets the ones from the range specified by first last
	template<class I>
	void SetBookmarks(I first, I last);	
	/// Indicates whether this document should be saved with BOM, invalid for ANSI documents
	bool GetUseBOM() const;
	/// Enables or disabled the usage of a BOM for Unicode text documents
	void SetUseBOM(bool use = true);
	/// Returns a tuple containing the first and last line
	/// of the paragraph the start_line parameter points to
	const std::pair<int,int> GetParagraphRange(int start_line);
	/// Returns a container for FoldingPoints
	const FoldingPointContainerType GetFoldingPoints(bool contracted = false);
	const FoldingPointContainerType GetContractedFoldingPoints();

	template<class I>
	void SetFoldingPoints(I first, I last);
	virtual void OnCloseDocument();
};

class TextDocument
{
	bool use_bom_;
	CodeDocument::Encoding encoding_;
	UINT code_page_;

	friend class CodeDocument;

private:
	TextDocument(bool use_bom, CodeDocument::Encoding e);

	static CodeDocument::Encoding DetectEncoding(const BYTE* data, SIZE_T& pos, SIZE_T size);
	/// Tests data for UTF-8 and UTF-16 encodings
	static CodeDocument::Encoding TestForUnicode(const BYTE* data, SIZE_T size);

public:
	TextDocument();
	TextDocument(CodeDocument::Encoding e, UINT codepage);
	
	explicit TextDocument(CodeDocument* document);
	
	CodeDocument::Encoding GetEncoding() const;
	void SetEncoding(CodeDocument::Encoding e);
	UINT GetCodePage() const;
	void SetCodePage(UINT codepage);
	bool GetUseBOM() const;
	void SetUseBOM(bool use);

	DWORD Write(ATL::CAtlFile& file, const char* text, std::size_t);
	/// Reads the whole file by detecting the right encoding.
	bool Read(LPCTSTR pszFileName, CStringW& string);
	/// Writes the whole file by detecting the right encoding.
	bool Write(LPCTSTR pszFileName, const CStringW& string);
	/// Writes UTF-8 encoded text to the file by performing any required code page conversion.
	DWORD WriteUTF8( ATL::CAtlFile& file, const char* utf8, std::size_t size);
};

template<class I>
inline void CodeDocument::SetBookmarks(I first, I last)
{
	RemoveAllBookmarks();

	for ( ; first != last; ++first)
		GetView()->GetCtrl().MarkerAdd(first->GetLine(),Bookmark);
}

template<class I>
inline void CodeDocument::SetFoldingPoints(I first, I last)
{
	CScintillaCtrl& c = GetView()->GetCtrl();
	c.Colourise(0,-1); // Let the lexer set the folding levels

	for ( ; first != last; ++first) {
		if (first->IsContracted()) {
			const int level = c.GetFoldLevel(first->GetLine());

			if (level & SC_FOLDLEVELHEADERFLAG) {
				const int max = c.GetLastChild(first->GetLine(),-1);
				c.SetFoldExpanded(first->GetLine(),FALSE);

				if (max > first->GetLine())
					c.HideLines(first->GetLine() + 1,max);
			}
		}
	}
}