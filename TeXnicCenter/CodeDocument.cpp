#include "stdafx.h"
#include "TeXnicCenter.h"

#include <algorithm>

#include "CodeDocument.h"
#include "global.h"
#include "SpellerBackgroundThread.h"
#include "CodeBookmark.h"

int DetectScintillaEOLMode(LPCTSTR text, std::size_t size)
{
	int mode = SC_EOL_CRLF;

	LPCTSTR const le = _T("\r\n");
	LPCTSTR end = text + size;

	LPCTSTR pos = std::find_first_of(text,end,le,le + _tcslen(le));

	if (pos != end) {
		if (*pos == _T('\r') && pos[1] != _T('\n'))
			mode = SC_EOL_CR;
		else if (*pos == _T('\n'))
			mode = SC_EOL_LF;
	}

	return mode;
}

namespace {
	namespace BOM {
		const BYTE utf8[] = {0xEF,0xBB,0xBF};
		const BYTE utf16le[] = {0xff,0xfe};
		const BYTE utf16be[] = {0xfe,0xff};
		const BYTE utf32le[] = {0xff,0xfe,0x00,0x00};
		const BYTE utf32be[] = {0x00,0x00,0xfe,0xff};
	}

	void SwapCodePoint(WCHAR& ch)
	{
		ch = (ch & 0xff) << 8 | ch >> 8 & 0xff;
	}
}

const TCHAR ModifiedMarker = _T('*');

#pragma region Line ending mode conversion functions

int ToScintillaMode(int m)
{
	int mode = SC_EOL_CRLF;

	switch (m) {
		case UnixStyleEOLMode: mode = SC_EOL_LF; break;
		case MacStyleEOLMode: mode = SC_EOL_CR; break;
	}

	return mode;
}

int FromScintillaMode(int m)
{
	int mode = DOSStyleEOLMode;

	switch (m) {
		case SC_EOL_CR: mode = MacStyleEOLMode; break;
		case SC_EOL_LF: mode = UnixStyleEOLMode; break;
	}

	return mode;
}

#pragma endregion

// CodeDocument

IMPLEMENT_DYNAMIC(CodeDocument, CScintillaDoc)

CodeDocument::CodeDocument()
: create_backup_file_(false)
, encoding_(ANSI) // Current user code page, see code_page_ data member
, code_page_(CP_ACP)
, speller_thread_(theApp.GetSpellerThread())
, error_marker_handle_(-1)
{
}

BOOL CodeDocument::OnNewDocument()
{
	if (!CScintillaDoc::OnNewDocument())
		return FALSE;

	return TRUE;
}

CodeDocument::~CodeDocument()
{
}


BEGIN_MESSAGE_MAP(CodeDocument, CScintillaDoc)
	ON_COMMAND(ID_EDIT_TOGGLE_BOOKMARK, &CodeDocument::OnEditToggleBookmark)
	ON_COMMAND(ID_EDIT_CLEAR_ALL_BOOKMARKS, &CodeDocument::OnEditClearAllBookmarks)
END_MESSAGE_MAP()


// CodeDocument diagnostics

#ifdef _DEBUG
void CodeDocument::AssertValid() const
{
	CScintillaDoc::AssertValid();
}

#ifndef _WIN32_WCE
void CodeDocument::Dump(CDumpContext& dc) const
{
	CScintillaDoc::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CodeDocument serialization

void CodeDocument::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
		SaveFile(ar.GetFile()->m_hFile);
	else
		LoadFile(ar.GetFile()->m_hFile);
}
#endif

// CodeDocument commands

void CodeDocument::SetModifiedFlag(BOOL modified)
{
	if (m_bModified != modified) {
		const TCHAR ch = ModifiedMarker;
		CString title = GetTitle();

		if (modified)
			title += ch;
		else
			title.TrimRight(ch);

		SetTitle(title);
	}

	__super::SetModifiedFlag(modified);
}

CodeDocument::Encoding CodeDocument::GetEncoding() const
{
	return encoding_;
}

UINT CodeDocument::GetCodePage() const
{
	return code_page_;
}

#pragma region Reading & writing to/from files

DWORD CodeDocument::LoadFile(LPCTSTR pszFileName)
{
	DWORD result = ERROR_SUCCESS;
	CAtlFile file;

	if (SUCCEEDED(file.Create(pszFileName,GENERIC_READ,FILE_SHARE_READ,OPEN_EXISTING)))
		result = LoadFile(file);
	else
		result = ::GetLastError();

	return result;
}

DWORD CodeDocument::LoadFile(HANDLE file)
{
	DWORD result = ERROR_SUCCESS;
	ATL::CAtlFileMapping<BYTE> mapping;

	if (SUCCEEDED(mapping.MapFile(file))) {
		const BYTE* data = mapping;
		SIZE_T size = mapping.GetMappingSize();
		SIZE_T pos = 0;

		Encoding encoding = DetectEncoding(data,pos,size);

		if (encoding == ANSI) { // ANSI? Still might by Unicode without BOM
			INT flags = IS_TEXT_UNICODE_UNICODE_MASK;

			if (::IsTextUnicode(data,size,&flags)) {
				if (flags & (IS_TEXT_UNICODE_REVERSE_ASCII16|IS_TEXT_UNICODE_REVERSE_STATISTICS))
					encoding = UTF16BE;
				else
					encoding = UTF16LE;
			}
			else {
				// From Wikipedia:
				// As a consequence of the design of UTF-8, the following properties of multi-byte sequences hold:

				//  * The most significant bit of a single-byte character is always 0.
				//	* The most significant bits of the first byte of a multi-byte sequence determine the length of 
				//	  the sequence. These most significant bits are 110 for two-byte sequences; 1110 for three-byte 
				//	  sequences, and so on.
				//	* The remaining bytes in a multi-byte sequence have 10 as their two most significant bits.
				//	* A UTF-8 stream contains neither the byte FE nor FF. This makes sure that a UTF-8 stream never 
				//	  looks like a UTF-16 stream starting with U+FEFF (Byte-order mark)

				bool utf8 = true;
				int consecutive_bytes = 0;
				bool ascii = size && data[0] < 128;
				std::size_t i = 0;

				for ( ; i < size && utf8; ++i) {
					// Might be pure ASCII
					ascii &= data[i] < 128;

					if (data[i] == 0xfe || data[i] == 0xff)
						utf8 = false;

					if (consecutive_bytes == 0) {
						if (data[i] >> 5 == 6)
							consecutive_bytes = 2;
						else if (data[i] >> 4 == 0xe)
							consecutive_bytes = 3;
					}
					else if (consecutive_bytes == 0 && data[i] >> 7 & 1 ||
						consecutive_bytes > 0 && data[i] >> 6 != 2)
						utf8 = false; // Either MSB is not 0 or MSB of a byte in a 
					// multi-part sequence is not 10

					if (consecutive_bytes > 0)
						--consecutive_bytes;
				}

				if (utf8 && !(ascii && i == size))
					encoding = UTF8;
			}
		}

		UINT code_page = CP_ACP; // Default user code page for ANSI text

		switch (encoding) {
			case UTF8: code_page = CP_UTF8; break;
			case UTF16LE: code_page = 1200; break; // Available only for managed applications
			case UTF16BE: code_page = 1201; break;
			case UTF32LE: code_page = 12000; break;
			case UTF32BE: code_page = 12001; break;
		}

		LPTSTR text;
		std::size_t required_size;

		if (encoding != UTF16LE && encoding != UTF16BE) {
			const UINT flags = 0;
			required_size = ::MultiByteToWideChar(code_page,flags,
				reinterpret_cast<LPCSTR>(data + pos),size - pos,0,0);

			LPWSTR temp = new WCHAR[required_size + 1];

			::MultiByteToWideChar(code_page,flags,
				reinterpret_cast<LPCSTR>(data + pos),size - pos,temp,required_size);

#ifdef UNICODE
			text = temp;
#else
			int asize = ::WideCharToMultiByte(CP_ACP,0,temp,required_size,0,0,0,0);

			LPSTR temp1 = new CHAR[asize + 1];

			::WideCharToMultiByte(CP_ACP,0,temp,required_size,temp1,asize,0,0);
			text = temp1;

			delete[] temp;
#endif
		}
		else {
			ASSERT((size - pos) % 2 == 0); // Size must be even
			required_size = (size - pos) / 2;
			LPWSTR temp = new WCHAR[required_size + 1];

			std::memcpy(temp,data + pos,required_size * 2);

			if (encoding == UTF16BE) // Swap bytes
				std::for_each(temp,temp + required_size,SwapCodePoint);

#ifndef UNICODE
			USES_CONVERSION_EX;
			text = new char[required_size + 1];

			std::memcpy(text,W2CA_EX(temp,required_size),required_size);

			delete[] temp;
#else
			text = temp;
#endif
		}

		text[required_size] = 0;

		//GetCtrl().SetCodePage(SC_CP_UTF8);
		GetView()->GetCtrl().SetText(text);

		// Detected EOL type
		GetView()->GetCtrl().SetEOLMode(DetectScintillaEOLMode(text,required_size));

		delete[] text;

		code_page_ = code_page;
		encoding_ = encoding;

		// No undo operation needed so far, we have just opened the document
		GetView()->GetCtrl().EmptyUndoBuffer();
	}
	else
		result = ::GetLastError();

	return result;
}

template<class T, std::size_t S>
const T* AssignByteOrderMark(const T (&p)[S], SIZE_T& s)
{
	s = S;
	return p;
}

void CodeDocument::ConvertToMultiByte(LPCWSTR input, int cch, std::vector<BYTE>& buffer, Encoding encoding, UINT cp)
{
	if (encoding != UTF16LE && encoding != UTF16BE) {
		int required_size = ::WideCharToMultiByte(cp,0,input,cch,0,0,0,0);

		if (required_size > 0) {
			buffer.resize(required_size);
			::WideCharToMultiByte(cp,0,input,cch,reinterpret_cast<LPSTR>(&buffer[0]),buffer.size(),0,0);
		}
	}
	else {
		const BYTE* begin = reinterpret_cast<const BYTE*>(input);
		const BYTE* end = reinterpret_cast<const BYTE*>(input + cch);

		buffer.assign(begin,end);

		if (encoding == UTF16BE) {
			LPWSTR begin = reinterpret_cast<LPWSTR>(&buffer[0]);
			LPWSTR end = begin + buffer.size() / 2;

			std::for_each(begin,end,SwapCodePoint);
		}
	}
}

DWORD CodeDocument::SaveFile(LPCTSTR pszFileName, bool bClearModifiedFlag)
{
	HANDLE hSearch = INVALID_HANDLE_VALUE;
	TCHAR szTempFileDir[_MAX_PATH + 1];
	TCHAR szTempFileName[_MAX_PATH + 1];
	TCHAR szBackupFileName[_MAX_PATH + 1];
	DWORD result = 1;

	CAtlFile temp_file;

	TCHAR drive[_MAX_PATH], dir[_MAX_PATH], name[_MAX_PATH], ext[_MAX_PATH];
	_tsplitpath(pszFileName, drive, dir, name, ext);

	lstrcpy(szTempFileDir, drive);
	lstrcat(szTempFileDir, dir);
	lstrcpy(szBackupFileName, pszFileName);
	lstrcat(szBackupFileName, _T(".bak"));

	bool error_occured = false;

	if (::GetTempFileName(szTempFileDir, _T("CRE"), 0, szTempFileName) != 0) {
		if (SUCCEEDED(temp_file.Create(szTempFileName, GENERIC_WRITE, 0, CREATE_ALWAYS))) {
			SaveFile(temp_file);

			if (create_backup_file_) {
				WIN32_FIND_DATA wfd;
				hSearch = ::FindFirstFile(pszFileName, &wfd);

				if (hSearch != INVALID_HANDLE_VALUE) {
					//	File exist - create backup file
					::DeleteFile(szBackupFileName);

					if (!::MoveFile(pszFileName, szBackupFileName))
						error_occured = true;
					else {
						::FindClose(hSearch);
						hSearch = INVALID_HANDLE_VALUE;
					}
				}
			}
			else
				::DeleteFile(pszFileName);

			temp_file.Close();

			//	Move temporary file to target name
			if (!error_occured && ::MoveFile(szTempFileName, pszFileName)) {
				if (bClearModifiedFlag)
					SetModifiedFlag(FALSE);

				result = ERROR_SUCCESS;
			}
		}
	}

	if (result)
		result = ::GetLastError();

	if (hSearch != INVALID_HANDLE_VALUE)
		::FindClose(hSearch);

	::DeleteFile(szTempFileName);

	return result;
}

DWORD CodeDocument::SaveFile( HANDLE file, LPCWSTR text, int length)
{
	CAtlFile f(file);
	DWORD result = ERROR_SUCCESS;

	if (encoding_ != ANSI) {
		const BYTE* bom = 0;
		SIZE_T size = 0;

		switch(encoding_) {
			case UTF8: bom = AssignByteOrderMark(BOM::utf8,size); break;
			case UTF16LE: bom = AssignByteOrderMark(BOM::utf16le,size); break;
			case UTF16BE: bom = AssignByteOrderMark(BOM::utf16be,size); break;
			case UTF32LE: bom = AssignByteOrderMark(BOM::utf32le,size); break;
			case UTF32BE: bom = AssignByteOrderMark(BOM::utf32be,size); break;
		}

		if (FAILED(f.Write(bom,size)))
			result = ::GetLastError();
	}

	if (result == ERROR_SUCCESS && length > 0) {
		std::vector<BYTE> buffer;
		ConvertToMultiByte(text,length,buffer,encoding_,code_page_);

		if (FAILED(f.Write(&buffer[0],buffer.size())))
			result = ::GetLastError();
	}

	f.Detach();

	return result;
}

DWORD CodeDocument::SaveFile( HANDLE file )
{
	const int length = GetView()->GetCtrl().GetLength();
	DWORD result = ERROR_SUCCESS;

	if (length > 0) {
		LPWSTR text = new WCHAR[length + 1];
		GetView()->GetCtrl().GetText(length + 1,text);

		result = SaveFile(file,text,length);

		delete[] text;
	}

	return result;
}

CodeDocument::Encoding CodeDocument::DetectEncoding(const BYTE* data, SIZE_T& pos, SIZE_T size)
{
	// By default we assume ANSI text
	Encoding encoding = ANSI;
	pos = 0;

	if (size >= 2) {// minimal BOM size
		// Try to detect Unicode using byte order mark
		if (size >= sizeof(BOM::utf8) && std::memcmp(data,BOM::utf8,sizeof(BOM::utf8)) == 0) {
			encoding = UTF8;
			pos += sizeof(BOM::utf8);
		}
		else if (std::memcmp(data,BOM::utf16le,sizeof(BOM::utf16le)) == 0) {
			encoding = UTF16LE;
			pos += sizeof(BOM::utf16le);
		}
		else if (std::memcmp(data,BOM::utf16be,sizeof(BOM::utf16be)) == 0) {
			encoding = UTF16BE;
			pos += sizeof(BOM::utf16be);
		}
		else if (size >= sizeof(BOM::utf32le) && std::memcmp(data,BOM::utf32le,sizeof(BOM::utf32le)) == 0) {
			encoding = UTF32LE;
			pos += sizeof(BOM::utf32le);
		}
		else if (size >= sizeof(BOM::utf32be) && std::memcmp(data,BOM::utf32be,sizeof(BOM::utf32be)) == 0) {
			encoding = UTF32BE;
			pos += sizeof(BOM::utf32be);
		}
	}

	return encoding;
}

#pragma endregion

void CodeDocument::SetSpellerThread( SpellerBackgroundThread* t )
{
	speller_thread_ = t;
}

SpellerBackgroundThread* CodeDocument::GetSpellerThread() const
{
	return speller_thread_;
}

bool CodeDocument::IsSpellerThreadAttached() const
{
	return GetSpellerThread() != 0;
}

bool CodeDocument::ShowSaveDialog(LPCTSTR file) const
{
	CString title(GetTitle());
	title.TrimRight(ModifiedMarker);

	return !file || title == file;
}

void CodeDocument::SetErrorMark(int line)
{
	if (error_marker_handle_ != -1) {
		GetView()->GetCtrl().MarkerDeleteHandle(error_marker_handle_);
		error_marker_handle_ = -1;
	}

	if (line > 0) {
		--line;
		error_marker_handle_ = GetView()->GetCtrl().
			MarkerAdd(line,CodeDocument::Errormark);
		GetView()->GetCtrl().GotoLine(line);
	}
}

bool CodeDocument::HasErrorMark() const
{
	return error_marker_handle_ != -1;
}

BOOL CodeDocument::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CScintillaDoc::OnOpenDocument(lpszPathName))
		return FALSE;

	error_marker_handle_ = -1;

	return TRUE;
}

void CodeDocument::OnEditToggleBookmark()
{
	int line = GetView()->GetCtrl().LineFromPosition(GetView()->GetCtrl().GetCurrentPos());
	ToggleBookmark(line);
}

void CodeDocument::OnBookmarkAdded(const ::CodeBookmark& /*b*/)
{
}

void CodeDocument::OnBookmarkRemoved(const ::CodeBookmark& /*b*/)
{
}

void CodeDocument::OnEditClearAllBookmarks()
{
	RemoveAllBookmarks();
	OnRemovedAllBookmarks();
}

void CodeDocument::OnRemovedAllBookmarks(void)
{
}

void CodeDocument::RemoveAllBookmarks()
{
	GetView()->GetCtrl().MarkerDeleteAll(Bookmark);
}

const BookmarkContainerType CodeDocument::GetBookmarks()
{
	BookmarkContainerType bookmarks;
	int line = 0;

	while ((line = GetView()->GetCtrl().MarkerNext(line,1 << CodeDocument::Bookmark)) >= 0) {
		bookmarks.push_back(CodeBookmark(line));
		++line;
	}

	return bookmarks;
}

void CodeDocument::ToggleBookmark( int line )
{
	int marker = GetView()->GetCtrl().MarkerGet(line);

	const ::CodeBookmark b(line);

	if (marker & 1 << Bookmark) { // present
		GetView()->GetCtrl().MarkerDelete(line,Bookmark);
		OnBookmarkRemoved(b);
	}
	else {
		GetView()->GetCtrl().MarkerAdd(line,Bookmark);
		OnBookmarkAdded(b);
	}
}