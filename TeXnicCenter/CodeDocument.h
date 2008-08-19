#pragma once

#include "CodeBookmark.h"

int ToScintillaMode(int m);
int FromScintillaMode(int m);

class SpellerBackgroundThread;

// CodeDocument document

class CodeDocument : public CScintillaDoc
{
	DECLARE_DYNAMIC(CodeDocument)

	/// Handle to the error marker or -1 if invalid
	int error_marker_handle_;
	/// Specifies whether a BOM has been found while
	/// opening the document or should be saved with it
	bool use_bom_;

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

	static Encoding DetectEncoding(const BYTE* data, SIZE_T& pos, SIZE_T size);

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
	DWORD SaveFile(LPCTSTR pszFileName, bool bClearModifiedFlag = true);
	virtual DWORD SaveFile(HANDLE file);	
	DWORD SaveFile(HANDLE file, const char* text, std::size_t n);
	Encoding GetEncoding() const;
	void SetEncoding(Encoding e);

	UINT GetCodePage() const;
	void SetSpellerThread(SpellerBackgroundThread* t);
	SpellerBackgroundThread* GetSpellerThread() const;
	bool IsSpellerThreadAttached() const;
	void SetErrorMark(int line);
	bool HasErrorMark() const;

protected:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	afx_msg void OnEditToggleBookmark();

	bool ShowSaveDialog(LPCTSTR file) const;
	DECLARE_MESSAGE_MAP()	
	virtual void OnBookmarkAdded(const ::CodeBookmark& b);
	virtual void OnBookmarkRemoved(const ::CodeBookmark& b);
	virtual void OnRemovedAllBookmarks(void);
	afx_msg void OnEditClearAllBookmarks();

public:
	void RemoveAllBookmarks();
	void ToggleBookmark( int line );

	const BookmarkContainerType GetBookmarks();
	template<class I>
	void SetBookmarks(I first, I last);

	/// Tests data for UTF-8 and UTF-16 encodings
	static Encoding TestForUnicode(const BYTE* data, SIZE_T size);
	/// Reads the whole file by detecting the right encoding
	static bool ReadString(LPCTSTR pszFileName, CStringW& string, UINT codepage = ::GetACP());
	/// Indicates whether this document should be saved with BOM, invalid for ANSI documents
	bool GetUseBOM() const;
	void SetUseBOM(bool use = true);
};

template<class I>
inline void CodeDocument::SetBookmarks(I first, I last)
{
	RemoveAllBookmarks();

	for ( ; first != last; ++first)
		GetView()->GetCtrl().MarkerAdd(first->GetLine(),Bookmark);
}