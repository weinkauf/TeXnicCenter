#include "stdafx.h"
#include "TeXnicCenter.h"

#include <algorithm>

#include "CodeDocument.h"
#include "global.h"
#include "SpellerBackgroundThread.h"
#include "CodeBookmark.h"
#include "EncodingConverter.h"
#include "RunTimeHelper.h"

int DetectScintillaEOLMode(const char* text, std::size_t size)
{
	int mode = SC_EOL_CRLF;

	const char* const le = "\r\n";
	const char* end = text + size;

	const char* pos = std::find_first_of(text,end,le,le + std::strlen(le));

	if (pos != end) {
		if (*pos == '\r' && pos[1] != '\n')
			mode = SC_EOL_CR;
		else if (*pos == '\n')
			mode = SC_EOL_LF;
	}

	return mode;
}

namespace {
	namespace BOM {
		const BYTE utf8[] = {0xef,0xbb,0xbf};
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
, code_page_(::GetACP())
, speller_thread_(theApp.GetSpellerThread())
, error_marker_handle_(-1)
, use_bom_(false)
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

CodeDocument::Encoding CodeDocument::TestForUnicode(const BYTE* data, SIZE_T size)
{
	Encoding encoding = ANSI;

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
		std::size_t consecutive_bytes = 0;
		bool ascii = size && data[0] < 128;
		std::size_t i = 0;

		for ( ; i < size && utf8; ++i) {
			// Might be pure ASCII
			ascii &= data[i] < 128;

			if (data[i] == 0xfe || data[i] == 0xff)
				utf8 = false;

			if (consecutive_bytes == 0)
				GetUTF8CharBytes(data[i],consecutive_bytes);
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

	return encoding;
}

bool CodeDocument::ReadString(LPCTSTR pszFileName, CStringW& string, UINT codepage)
{
	DWORD result = ERROR_SUCCESS;
	ATL::CAtlFile file;

	if (SUCCEEDED(file.Create(pszFileName,GENERIC_READ,FILE_SHARE_READ,OPEN_EXISTING))) {
		ATL::CAtlFileMapping<BYTE> mapping;

		if (SUCCEEDED(mapping.MapFile(file))) {
			const BYTE* data = mapping;
			SIZE_T size = mapping.GetMappingSize();
			SIZE_T pos = 0;

			Encoding encoding = DetectEncoding(data,pos,size);

			if (encoding == ANSI) { // ANSI? Still might by Unicode without BOM
				encoding = TestForUnicode(data,size);
			}

			UINT code_page = codepage; // Default user code page for ANSI text

			std::vector<wchar_t> buffer;
			const wchar_t* text = 0;
			std::size_t n = 0;

			switch (encoding) {
				case ANSI:
					ANSItoUTF16(reinterpret_cast<const char*>(data + pos),size - pos,buffer,code_page);
					break;
				case UTF8:
					UTF8toUTF16(reinterpret_cast<const char*>(data + pos),size - pos,buffer);					
					break;
				case UTF16LE:
				case UTF16BE:
					// No conversion needed
					text = reinterpret_cast<const wchar_t*>(data + pos);
					n = size - pos;

					if (encoding == UTF16BE) {
						buffer.assign(text,text + n);
						std::for_each(buffer.begin(),buffer.end(),SwapCodePoint);

						text = 0; n = 0; // Will be reassigned
					}
					break;
				case UTF32LE:
				case UTF32BE:
					UTF32toUTF16(reinterpret_cast<const char*>(data + pos),size - pos,
						buffer,encoding == UTF32LE);
					break;
			}

			if (!text && !buffer.empty()) {
				text = &buffer[0];
				n = buffer.size();

				string = CStringW(text,n);
			}
		}
		else
			result = ::GetLastError();
	}
	else
		result = ::GetLastError();

	return result == ERROR_SUCCESS;
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
			encoding = TestForUnicode(data,size);
		}

		UINT code_page = ::GetACP(); // Default user code page for ANSI text
		
		std::vector<char> buffer;
		const char* text = 0;
		std::size_t n = 0;

		switch (encoding) {
			case ANSI:
				ANSItoUTF8(reinterpret_cast<const char*>(data + pos),size - pos,buffer,code_page);
				break;
			case UTF8:
				// No conversion needed
				text = reinterpret_cast<const char*>(data + pos);
				n = size - pos;
				break;
			case UTF16LE:
			case UTF16BE:
				UTF16toUTF8(reinterpret_cast<const char*>(data + pos),size - pos,
					buffer,encoding == UTF16LE);
				break;
			case UTF32LE:
			case UTF32BE:
				UTF32toUTF8(reinterpret_cast<const char*>(data + pos),size - pos,
					buffer,encoding == UTF32LE);
				break;
		}

		if (!text && !buffer.empty()) {
			text = &buffer[0];
			n = buffer.size();
		}

		GetView()->GetCtrl().ClearAll();
		GetView()->GetCtrl().SetCodePage(SC_CP_UTF8);

		if (n) {
			GetView()->GetCtrl().AppendText(n,text);
			// Detected EOL type
			GetView()->GetCtrl().SetEOLMode(DetectScintillaEOLMode(text,n));
		}

		use_bom_ = pos > 0;
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

DWORD CodeDocument::SaveFile( HANDLE file, const char* text, std::size_t n)
{
	CAtlFile f(file);
	DWORD result = ERROR_SUCCESS;

	if (encoding_ != ANSI && use_bom_) {
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

	if (result == ERROR_SUCCESS && n > 0) {
		if (FAILED(f.Write(text,n)))
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
		std::vector<char> buffer(length + 1);
		GetView()->GetCtrl().GetText(length + 1,&buffer[0]);

		buffer.pop_back(); // Strip the terminating zero

		const char* text = 0;
		std::size_t n = 0;

		std::vector<char> convbuffer;

		switch (encoding_) {
			case ANSI:
				UTF8toANSI(&buffer[0],buffer.size(),convbuffer,code_page_);
				break;
			case UTF8:
				text = &buffer[0];
				n = buffer.size();
				break;
			case UTF16LE:
			case UTF16BE:
				UTF8toUTF16(&buffer[0],buffer.size(),convbuffer,encoding_ == UTF16LE);
				break;
			case UTF32LE:
			case UTF32BE:
				UTF8toUTF32(&buffer[0],buffer.size(),convbuffer,encoding_ == UTF32LE);
				break;
		}

		if (!text && !convbuffer.empty()) {
			text = &convbuffer[0];
			n = convbuffer.size();
		}

		result = SaveFile(file,text,n);
	}

	return result;
}

CodeDocument::Encoding CodeDocument::DetectEncoding(const BYTE* data, SIZE_T& pos, SIZE_T size)
{
	// By default we assume ANSI text
	Encoding encoding = ANSI;
	pos = 0;

	if (size >= 2) {// minimal BOM size
		// Try to detect Unicode using the byte order mark
		if (size >= sizeof(BOM::utf8) && std::memcmp(data,BOM::utf8,sizeof(BOM::utf8)) == 0) {
			encoding = UTF8;
			pos += sizeof(BOM::utf8);
		} 
		// Try UTF-32LE instead of UTF-16 since BOM's first 2 bytes are for both encodings the same
		else if (size >= sizeof(BOM::utf32le) && std::memcmp(data,BOM::utf32le,sizeof(BOM::utf32le)) == 0) {
			encoding = UTF32LE;
			pos += sizeof(BOM::utf32le);
		}
		else if (size >= sizeof(BOM::utf32be) && std::memcmp(data,BOM::utf32be,sizeof(BOM::utf32be)) == 0) {
			encoding = UTF32BE;
			pos += sizeof(BOM::utf32be);
		}
		else if (std::memcmp(data,BOM::utf16le,sizeof(BOM::utf16le)) == 0) {
			encoding = UTF16LE;
			pos += sizeof(BOM::utf16le);
		}
		else if (std::memcmp(data,BOM::utf16be,sizeof(BOM::utf16be)) == 0) {
			encoding = UTF16BE;
			pos += sizeof(BOM::utf16be);
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

void CodeDocument::SetEncoding(Encoding e) 
{
	encoding_ = e; 
}

bool CodeDocument::GetUseBOM() const
{
	return use_bom_;
}

void CodeDocument::SetUseBOM( bool use /*= true*/ )
{
	use_bom_ = use;
}

BOOL CodeDocument::SaveModified()
{
	BOOL result;

	if (!RunTimeHelper::IsVista())
		result = CScintillaDoc::SaveModified();
	else
		result = DoSaveModified();

	return result;
}

BOOL CodeDocument::DoSaveModified()
{
	BOOL result;

	if (!IsModified())
		result = TRUE;        // ok to continue
	else {
		// get name/title of document
		CString name;

		if (m_strPathName.IsEmpty()) {
			// get name based on caption
			name = m_strTitle;

			if (name.IsEmpty())
				ENSURE(name.LoadString(AFX_IDS_UNTITLED));
		}
		else {
			extern UINT AFXAPI AfxGetFileTitle(LPCTSTR, LPTSTR, UINT);
			// get name based on file title of path name
			name = m_strPathName;
			AfxGetFileTitle(m_strPathName, name.GetBuffer(_MAX_PATH), _MAX_PATH);
			name.ReleaseBuffer();
		}

		CString prompt;
		AfxFormatString1(prompt,AFX_IDP_ASK_TO_SAVE,name);

		TASKDIALOGCONFIG tdc = {sizeof(TASKDIALOGCONFIG)};
		tdc.dwCommonButtons = TDCBF_CANCEL_BUTTON;

		USES_CONVERSION;
		tdc.pszMainInstruction = T2CW(prompt);

		const CStringW save(MAKEINTRESOURCE(IDS_DO_SAVE));
		const CStringW dont_save(MAKEINTRESOURCE(IDS_DO_NOT_SAVE));

		const TASKDIALOG_BUTTON btns[] = {
			IDYES,save,
			IDNO,dont_save
		};

		tdc.cButtons = sizeof(btns) / sizeof(*btns);
		tdc.pButtons = btns;

		// disable windows for modal dialog
		theApp.DoEnableModeless(FALSE);

		HWND hWndTop;
		HWND hWnd = CWnd::GetSafeOwner_(NULL, &hWndTop);

		// re-enable the parent window, so that focus is restored 
		// correctly when the dialog is dismissed.
		if (hWnd != hWndTop)
			EnableWindow(hWnd, TRUE);

		tdc.hwndParent = hWnd;

		const CStringW title(MAKEINTRESOURCE(IDR_MAINFRAME));
		tdc.pszWindowTitle = title;

		int button = 0;
		::TaskDialogIndirect(&tdc,&button,0,0); // AfxMessageBox(prompt, MB_YESNOCANCEL, AFX_IDP_ASK_TO_SAVE)

		// re-enable windows
		if (hWndTop != NULL)
			::EnableWindow(hWndTop, TRUE);

		theApp.DoEnableModeless(TRUE);

		switch (button) {
			case IDCANCEL:
				result = FALSE;       // don't continue

			case IDYES:
				// If so, either Save or Update, as appropriate
				if (!DoFileSave())
					result = FALSE;       // don't continue
				break;

			case IDNO:
				// If not saving changes, revert the document
				break;

			case IDOK:
				result = TRUE;    // keep going
				break;

			default:
				ASSERT(FALSE);
				break;
		}
	}

	return result;
}