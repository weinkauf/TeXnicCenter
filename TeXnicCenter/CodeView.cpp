#include "stdafx.h"
#include "TeXnicCenter.h"

#include <sstream>
#include <string>
#include <functional>
#include <iterator>
#include <cstring>
#include <cstddef>

#include "CodeDocument.h"
#include "CodeView.h"
#include "gotodialog.h"
#include "configuration.h"
#include "CharType.h"
#include "FindReplaceDlg.h"
#include "EncodingConverter.h"

#pragma region Helper functions

int GetLogFontPointSize(const LOGFONT& lf)
{
	CWindowDC dc(0);

	const LOGFONT& editor_font = lf;

	CFont font;
	font.CreateFontIndirect(&editor_font);

	CFont* oldfont = dc.SelectObject(&font);

	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);

	dc.SelectObject(&oldfont);

	// Convert logical units to points
	return ::MulDiv(tm.tmHeight - tm.tmInternalLeading,72,dc.GetDeviceCaps(LOGPIXELSY));
}

#pragma endregion

// CodeView

IMPLEMENT_DYNAMIC(CodeView, CScintillaView)

CodeView::CodeView()
: hold_count_(0)
, hard_wrap_enabled_(false)
, remove_leading_spaces_(true)
, suppress_speller_(false)
, shadow_(this)
, last_change_pos_(-1)
{
}

CodeView::~CodeView()
{
	ASSERT(hold_count_ == 0);
}

BEGIN_MESSAGE_MAP(CodeView, CScintillaView)
	ON_COMMAND(ID_EDIT_GOTO, &CodeView::OnEditGoto)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CodeView::OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INDICATOR_ENCODING, &CodeView::OnUpdateEncodingIndicator)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INDICATOR_POSITION, &CodeView::OnUpdatePositionIndicator)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INDICATOR_CRLF, &CodeView::OnUpdateEOLModeIndicator)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WHITE_SPACE, &CodeView::OnUpdateViewWhiteSpace)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LINE_ENDING, &CodeView::OnUpdateViewShowLineEnding)
	ON_COMMAND(ID_VIEW_WORD_WRAP, &CodeView::OnViewWordWrap)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WORD_WRAP, &CodeView::OnUpdateViewWordWrap)
	ON_COMMAND(ID_VIEW_WHITE_SPACE, &CodeView::OnViewWhiteSpace)
	ON_COMMAND(ID_VIEW_LINE_ENDING, &CodeView::OnViewLineEnding)	
	ON_WM_SYSCOLORCHANGE()
	ON_WM_DESTROY()	
	ON_COMMAND(ID_VIEW_LINE_NUMBERS, &CodeView::OnViewLineNumbers)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LINE_NUMBERS, &CodeView::OnUpdateViewLineNumbers)
	ON_COMMAND(ID_EDIT_GOTO_NEXT_BOOKMARK, &CodeView::OnEditGotoNextBookmark)
	ON_COMMAND(ID_EDIT_GOTO_PREV_BOOKMARK, &CodeView::OnEditGotoPrevBookmark)
	ON_UPDATE_COMMAND_UI(ID_EDIT_GOTO_PREV_BOOKMARK, &CodeView::OnUpdateEditGotoPrevBookmark)
	ON_UPDATE_COMMAND_UI(ID_EDIT_GOTO_NEXT_BOOKMARK, &CodeView::OnUpdateEditGotoNextBookmark)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR_ALL_BOOKMARKS, &CodeView::OnUpdateEditClearAllBookmarks)
	ON_WM_CREATE()
	ON_COMMAND(ID_EDIT_FIND_INCREMENTAL_FORWARD, &CodeView::OnEditFindIncrementalForward)
	ON_COMMAND(ID_EDIT_FIND_INCREMENTAL_BACKWARD, &CodeView::OnEditFindIncrementalBackward)
	ON_COMMAND(ID_EDIT_GOTO_LAST_CHANGE, &CodeView::OnEditGotoLastChange)
	ON_UPDATE_COMMAND_UI(ID_EDIT_GOTO_LAST_CHANGE, &CodeView::OnUpdateEditGotoLastChange)
	ON_COMMAND(ID_VIEW_INDENTATION_GUIDES, &CodeView::OnViewIndentationGuides)
	ON_UPDATE_COMMAND_UI(ID_VIEW_INDENTATION_GUIDES, &CodeView::OnUpdateViewIndentationGuides)
END_MESSAGE_MAP()


// CodeView diagnostics

#ifdef _DEBUG
void CodeView::AssertValid() const
{
	CScintillaView::AssertValid();
}

#ifndef _WIN32_WCE
void CodeView::Dump(CDumpContext& dc) const
{
	CScintillaView::Dump(dc);
}
#endif
#endif //_DEBUG


// CodeView message handlers

int CodeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScintillaView::OnCreate(lpCreateStruct) == -1)
		return -1;

	shadow_.SubclassWindow(GetCtrl());

	theme_.SubclassWindow(m_hWnd);
	theme_.Initialize();

	EnableFolding(); // CodeView::OnSettingsChanged depends on whether folding is enabled or not
	OnSettingsChanged(); // Make sure derived class setup the settings such as fonts, lexers etc. first

	GetCtrl().SetViewWS(CConfiguration::GetInstance()->m_bViewWhitespaces ? SCWS_VISIBLEALWAYS : SCWS_INVISIBLE);
	GetCtrl().SetViewEOL(CConfiguration::GetInstance()->GetShowLineEnding());
	GetCtrl().SetWrapMode(CConfiguration::GetInstance()->IsWordWrapEnabled() ? SC_WRAP_WORD : SC_WRAP_NONE);

	UpdateLineNumberMargin();

#pragma region Markers

	GetCtrl().SetMarginSensitiveN(1,TRUE); // React on clicks

	GetCtrl().MarkerDefine(CodeDocument::Errormark,SC_MARK_ARROW);

	GetCtrl().MarkerSetFore(CodeDocument::Errormark,RGB(158,0,57));
	GetCtrl().MarkerSetBack(CodeDocument::Errormark,RGB(237,28,36));


	GetCtrl().MarkerDefine(CodeDocument::Bookmark,SC_MARK_SMALLRECT);

	GetCtrl().MarkerSetFore(CodeDocument::Bookmark,RGB(70,105,175));
	GetCtrl().MarkerSetBack(CodeDocument::Bookmark,RGB(232,241,255));	

#pragma endregion

	CScintillaCtrl& rCtrl = GetCtrl();

	rCtrl.SetPasteConvertEndings(TRUE); // Convert line endings

#pragma region Folding

	const int folding_margin_num = GetFoldingMargin();

	rCtrl.SetMarginSensitiveN(folding_margin_num,TRUE); // Receive clicks
	rCtrl.SetMarginTypeN(folding_margin_num,SC_MARGIN_SYMBOL);
	rCtrl.SetMarginMaskN(folding_margin_num,SC_MASK_FOLDERS);

	// Setup markers for VS style folding
	COLORREF clr = ::GetSysColor(COLOR_3DSHADOW);

	DefineMarker(SC_MARKNUM_FOLDEROPEN, SC_MARK_BOXMINUS, RGB(0xff, 0xff, 0xff), clr);
	DefineMarker(SC_MARKNUM_FOLDER, SC_MARK_BOXPLUS, RGB(0xff, 0xff, 0xff), clr);
	DefineMarker(SC_MARKNUM_FOLDERSUB, SC_MARK_VLINE, RGB(0xff, 0xff, 0xff), clr);
	DefineMarker(SC_MARKNUM_FOLDERTAIL, SC_MARK_LCORNER, RGB(0xff, 0xff, 0xff), clr);
	DefineMarker(SC_MARKNUM_FOLDEREND, SC_MARK_BOXPLUSCONNECTED, RGB(0xff, 0xff, 0xff), clr);
	DefineMarker(SC_MARKNUM_FOLDEROPENMID, SC_MARK_BOXMINUSCONNECTED, RGB(0xff, 0xff, 0xff),clr);
	DefineMarker(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_TCORNER, RGB(0xff, 0xff, 0xff), clr);

#pragma endregion

	// Indicator for spell checker errors
	GetCtrl().SetIndicatorCurrent(0); // Default is a squiggly line, which we want to use
	GetCtrl().IndicSetFore(0,RGB(255,0,0)); // Red color instead of dark green
	GetCtrl().SetIndent(0);

	return 0;
}

void CodeView::GoToLine( int line, bool direct /*= true*/ )
{
	GetCtrl().GotoLine(line,direct);
	// Make sure that lines that have been hidden due to folding
	// will be visible as well
	GetCtrl().EnsureVisible(line,direct); 
}

int CodeView::GetLineLength( int line, bool direct /*= true*/ )
{
	return GetCtrl().GetLineEndPosition(line,direct) - GetCtrl().PositionFromLine(line,direct);
}

int CodeView::GetLineCount( bool direct /*= true*/ )
{
	return GetCtrl().GetLineCount(direct);
}

int CodeView::GetCurrentLine( bool direct /*= true*/ )
{
	return GetCtrl().LineFromPosition(GetCtrl().GetCurrentPos(direct),direct);
}

const CString CodeView::GetLineText( int line, bool direct /*= true*/ )
{
	const int length = GetLineLength(line,direct);
	CString strLine;

	if (length > 0) {
		CStringA temp;

		GetCtrl().GetLine(line,temp.GetBuffer(length + 1),direct);
		temp.ReleaseBuffer(length);

#ifdef UNICODE
		std::vector<wchar_t> buffer;
		UTF8toUTF16(temp,temp.GetLength(),buffer);
#else
		std::vector<char> buffer;
		UTF8toANSI(temp,temp.GetLength(),buffer);
#endif

		if (!buffer.empty())
			strLine.SetString(&buffer[0],buffer.size());
	}

	return strLine;
}

void CodeView::InsertText( const CString& text )
{
	long pos = GetCtrl().GetCurrentPos();
	GetCtrl().InsertText(pos,text);
	GetCtrl().GotoPos(pos + text.GetLength());
}


int CodeView::Lock(bool exclusive /*= false */)
{
	ASSERT(hold_count_ >= 0);

	if (exclusive) {
		MSG msg;
		
		// Adjust the min/max range if new message are added to Scintilla
		const UINT min = SCI_START;
		const UINT max = SCI_SHOWCURSOR;

		while (hold_count_ > 0) { // atomic operation, no lock needed
			// Let the thread process remaining Scintilla messages 
			// sent by the speller otherwise a dead lock will occur.
			while (::PeekMessage(&msg,GetCtrl(),min,max,PM_REMOVE) > 0) {
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}

		::InterlockedExchange(&hold_count_,1);
	}
	else
		::InterlockedIncrement(&hold_count_);

	ASSERT(hold_count_ > 0);
	return hold_count_;
}

int CodeView::Unlock(bool exclusive /*= false */)
{
	ASSERT(hold_count_ > 0);
	int hold_count;

	if (exclusive) {
		// Lock( true ) was not called
		ASSERT(hold_count_ == 1);

		::InterlockedExchange(&hold_count_,0);
		hold_count = 0;
	}
	else {
		::InterlockedDecrement(&hold_count_);
		hold_count = hold_count_;
	}

	return hold_count;
}

void CodeView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

void CodeView::OnUpdateEncodingIndicator( CCmdUI* pCmdUI )
{
	LPCTSTR text = 0;

	switch (GetDocument()->GetEncoding()) {
		case CodeDocument::UTF8: text = _T("UTF-8"); break;
		case CodeDocument::UTF16LE: text = _T("UTF-16"); break;
		case CodeDocument::UTF16BE: text = _T("UTF-16BE"); break;
		case CodeDocument::UTF32LE: text = _T("UTF-32"); break;
		case CodeDocument::UTF32BE: text = _T("UTF-32BE"); break;
		default: text = _T("ANSI");
	}

	pCmdUI->SetText(text);
	pCmdUI->Enable();
}

void CodeView::OnUpdateEOLModeIndicator( CCmdUI* pCmdUI )
{
	LPCTSTR text = 0;

	switch (GetCtrl().GetEOLMode()) {
		case SC_EOL_CRLF: text = _T("CR+LF"); break;
		case SC_EOL_LF: text = _T("LF"); break;
		case SC_EOL_CR: text = _T("CR"); break;
	}

	pCmdUI->SetText(text);
	pCmdUI->Enable();
}

void CodeView::OnUpdatePositionIndicator( CCmdUI* pCmdUI )
{
	CScintillaCtrl& rCtrl = GetCtrl();
	int pos = rCtrl.GetCurrentPos();
	int line = rCtrl.LineFromPosition(pos);
	int col = rCtrl.GetColumn(pos);

	CString text;
	text.Format(_T("Ln %d, Col %d, Ch %d"),line + 1,col + 1,pos + 1);

	pCmdUI->SetText(text);
	pCmdUI->Enable();
}

void CodeView::TextNotFound(LPCTSTR /*lpszFind*/, BOOL /*bNext*/, BOOL /*bCase*/, BOOL /*bWord*/, BOOL /*bRegularExpression*/, BOOL /*bReplaced*/)
{
}

DocumentTokenizer* CodeView::GetTokenizer() const
{
	// Create the tokenizer only on demand
	if (!tokenizer_.get())
		tokenizer_.reset(NewDocumentTokenizer());

	return tokenizer_.get();
}

void CodeView::OnDestroy()
{
	if (GetDocument()->IsSpellerThreadAttached()) {
		CWaitCursor wc;
		// Inform the background thread we're being destroyed
		GetDocument()->GetSpellerThread()->InvalidateView(this);
		// Wait for the background thread to stop
		Lock(true);	
		Unlock(true);
	}

	CScintillaView::OnDestroy();
}

void CodeView::OnViewWhiteSpace()
{
	bool visible = GetCtrl().GetViewWS() != SCWS_INVISIBLE;
	int mode = visible ? SCWS_INVISIBLE : SCWS_VISIBLEALWAYS;

	CConfiguration::GetInstance()->m_bViewWhitespaces = !visible;

	using namespace std::tr1;
	using namespace placeholders;

	// Chained update: call for every view GetCtrl().SetViewWS(!visible,TRUE)
	ForEveryView(bind(bind(&CScintillaCtrl::SetViewWS,_1,mode,TRUE),
		bind(&CodeView::GetCtrl,_1)));
}

void CodeView::OnViewLineEnding()
{
	bool visible = !GetCtrl().GetViewEOL();
	CConfiguration::GetInstance()->SetShowLineEnding(visible);

	using namespace std::tr1;
	using namespace placeholders;

	// Chained update: call for every view GetCtrl().SetViewEOL(visible,TRUE)
	ForEveryView(bind(bind(&CScintillaCtrl::SetViewEOL,_1,visible,TRUE),
		bind(&CodeView::GetCtrl,_1)));
}

void CodeView::OnViewWordWrap()
{
#if 0 // Hard wrap test code
	long s = GetCtrl().GetSelectionStart();
	long e = GetCtrl().GetSelectionEnd();

	if (s != e)
		HardWrapRange(s,e);
#endif // 0

	bool wrap = GetCtrl().GetWrapMode() != SC_WRAP_WORD;
	CConfiguration::GetInstance()->EnableWordWrap(wrap);

	int mode = wrap ? SC_WRAP_WORD : SC_WRAP_NONE;

	using namespace std::tr1;
	using namespace placeholders;

	// Chained update: call for every view GetCtrl().SetWrapMode(!visible,TRUE)
	ForEveryView(bind(bind(&CScintillaCtrl::SetWrapMode,_1,mode,TRUE),
		bind(&CodeView::GetCtrl,_1)));
}

void CodeView::OnViewLineNumbers()
{
	CConfiguration::GetInstance()->m_bShowLineNumbers = 
		!CConfiguration::GetInstance()->m_bShowLineNumbers;

	// Chained update
	ForEveryView(std::mem_fun(&CodeView::UpdateLineNumberMargin));
}

void CodeView::OnUpdateViewWhiteSpace(CCmdUI *pCmdUI)
{
	bool visible = GetCtrl().GetViewWS() != SCWS_INVISIBLE;
	pCmdUI->SetCheck(visible);
}

void CodeView::OnUpdateViewShowLineEnding(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(GetCtrl().GetViewEOL());
}

void CodeView::OnEditDeleteLine()
{
	GetCtrl().LineDelete();
}

void CodeView::OnInitialUpdate()
{
	CScintillaView::OnInitialUpdate();
}

void CodeView::OnSettingsChanged()
{
	// User changed application's settings: react here

	// Folding margin width: 2px + 1.5ex + 2px
	const int width = IsFoldingEnabled() ? GetCtrl().TextWidth(STYLE_DEFAULT,"9") + 2 * 2 : 0;
	GetCtrl().SetMarginWidthN(GetFoldingMargin(),width);
	GetCtrl().SetFoldMarginColour(TRUE,GetCtrl().StyleGetBack(STYLE_DEFAULT));
	
	int flags = 0;

	if (CConfiguration::GetInstance()->GetShowLineBelowFold())
		flags |= 16;

	if (CConfiguration::GetInstance()->GetShowLineBelowNoFold())
		flags |= 8;

	if (CConfiguration::GetInstance()->GetShowLineAboveFold())
		flags |= 4;

	if (CConfiguration::GetInstance()->GetShowLineAboveNoFold())
		flags |= 2;

	GetCtrl().SetFoldFlags(flags);

	GetCtrl().SetUseTabs(!CConfiguration::GetInstance()->GetUseSpaces());
	GetCtrl().SetTabWidth(CConfiguration::GetInstance()->m_nTabWidth);
	GetCtrl().SetIndent(GetCtrl().GetTabWidth());
}

void CodeView::OnSysColorChange()
{
	CScintillaView::OnSysColorChange();
	OnSettingsChanged();
}

CodeDocument* CodeView::GetDocument() const
{
	return static_cast<CodeDocument*>(CScintillaView::GetDocument());
}

void CodeView::OnUpdateViewWordWrap(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(GetCtrl().GetWrapMode() == SC_WRAP_WORD);
}

void CodeView::OnEditGoto()
{
	CGotoDialog dlg(GetLineCount(),this);
	dlg.m_nLine = GetCurrentLine() + 1;

	if (dlg.DoModal() == IDOK)
		GoToLine(dlg.m_nLine - 1);
}

void CodeView::OnUpdateViewLineNumbers(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(IsLineMarginVisible());
}

void CodeView::UpdateLineNumberMargin()
{
	CScintillaCtrl& rCtrl = GetCtrl();
	int line_count = rCtrl.GetLineCount();

	// Calculate the number of digits
	// used to display the line_count value
	int digits = 0;

	while (line_count) {
		line_count /= 10;
		++digits;
	}

	std::string number(std::max(digits,2),'9'); // Put 9 line_count times to measure the width
	number.insert(number.begin(),'_'); // Some padding

	if (CConfiguration::GetInstance()->m_bShowLineNumbers) {
		const int width = rCtrl.TextWidth(STYLE_LINENUMBER,number.c_str());
		rCtrl.SetMarginWidthN(0,width);
	}
	else
		rCtrl.SetMarginWidthN(0,0); // Margin invisible
}

bool CodeView::IsLineMarginVisible()
{
	return GetCtrl().GetMarginWidthN(0) > 0;
}

void CodeView::OnCharAdded(SCNotification* n)
{
	if (GetDocument()->IsModified())
		GetDocument()->SetModifiedFlag();

	if (!suppress_speller_) {
		if (GetDocument()->IsSpellerThreadAttached())
			GetDocument()->GetSpellerThread()->RecheckSingleLineSpelling(GetCurrentLine(),this);
	}

	// Hard wrapping
	if (hard_wrap_enabled_) {
		if (n->ch != _T('\n') && n->ch != _T('\r')) {
			GetCtrl().BeginUndoAction();

			long pos = GetCtrl().GetCurrentPos();
			int line = GetCtrl().LineFromPosition(pos);
			long s = GetCtrl().PositionFromLine(line);
			
			if (GetCtrl().GetCurrentPos() - s >= CConfiguration::GetInstance()->m_nFixedColumnWrap) {
				long e = GetCtrl().GetLineEndPosition(line);
				bool stop = false;

				while (e >= s && !stop) {
					if (CharTraitsT::IsSpace(GetCtrl().GetCharAt(e))) {
						GetCtrl().GotoPos(e);
						// Advance to the next line
						GetCtrl().NewLine(); ++line;

						long new_line_end = GetCtrl().GetLineEndPosition(line);
						long new_line_start = GetCtrl().PositionFromLine(line);
						long new_line_pos = new_line_start;

						if (remove_leading_spaces_) {
							// Count the number of leading spaces
							while (new_line_pos < new_line_end && 
								GetCtrl().GetCharAt(new_line_pos) == _T(' ')) {
								++new_line_pos;
							}

							// Remove if non zero
							if (new_line_pos - new_line_start > 0) {
								GetCtrl().SetSel(new_line_start,new_line_pos);
								GetCtrl().ReplaceSel("");
							}
						}

						new_line_end = GetCtrl().GetLineEndPosition(line);
						GetCtrl().GotoPos(new_line_end);

						stop = true;
					}

					--e;
				}
			}

			GetCtrl().EndUndoAction();
		}		
	}
}

void CodeView::EnableHardWrap( bool e /*= true*/ )
{
	hard_wrap_enabled_ = e;
}

bool CodeView::IsHardWrapEnabled() const
{
	return hard_wrap_enabled_;
}

void CodeView::SetRemoveLeadingSpaces( bool e /*= true*/ )
{
	remove_leading_spaces_ = e;
}

bool CodeView::GetRemoveLeadingSpaces() const
{
	return remove_leading_spaces_;
}

void CodeView::HardWrapRange( long s, long e )
{
	const int col = CConfiguration::GetInstance()->m_nFixedColumnWrap;

	if (e - s > col) {
		GetCtrl().BeginUndoAction();

		long pos = s; bool move, new_line_started = false;
		int oldpos, newpos, lastcrlfpos = -1, count;

		while (pos <= e) {
			move = false;
			int ch = GetCtrl().GetCharAt(pos);

			// Remove CR+LF and leading spaces for newly started bookmarks
			if (ch == '\r' || ch == '\n' || ch == ' ' && new_line_started) {
				if (ch == '\r' || ch == '\n')
					lastcrlfpos = pos;

				count = 1;

				if (pos + 1 <= e && GetCtrl().GetCharAt(pos + 1) == '\n')
					++count;

				GetCtrl().SetSel(pos,pos + count);
				GetCtrl().ReplaceSel("");
				e -= count;
			}
			else if (ch != ' ' && new_line_started)
				new_line_started = false;

			if ((pos - GetCtrl().PositionFromLine(GetCtrl().LineFromPosition(pos))) >= col) {
				oldpos = GetCtrl().WordStartPosition(pos,TRUE);

				if (oldpos > 0)
					--oldpos;

				if (GetCtrl().GetCharAt(oldpos) == ' ') {
					new_line_started = true;
					GetCtrl().GotoPos(oldpos); GetCtrl().NewLine();

					newpos = GetCtrl().PositionFromLine(GetCtrl().LineFromPosition(oldpos) + 1);
					e += newpos - oldpos; pos = newpos;					

					move = false;
				}
				else
					move = true;
			}
			else
				move = true;
			
			if (move)
				++pos;
		}

		GetCtrl().EndUndoAction();
	}
}

int CodeView::GetPreviousBookmark()
{
	return GetCtrl().MarkerPrevious(GetCurrentLine() - 1,1 << CodeDocument::Bookmark);
}

int CodeView::GetNextBookmark()
{
	return GetCtrl().MarkerNext(GetCurrentLine() + 1,1 << CodeDocument::Bookmark);
}

void CodeView::OnEditGotoNextBookmark()
{
	int line = GetNextBookmark();

	if (line >= 0)
		GetCtrl().GotoLine(line);
}

void CodeView::OnEditGotoPrevBookmark()
{
	int line = GetPreviousBookmark();

	if (line >= 0)
		GetCtrl().GotoLine(line);
}

void CodeView::OnUpdateEditGotoPrevBookmark(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetPreviousBookmark() >= 0);
}

void CodeView::OnUpdateEditGotoNextBookmark(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetNextBookmark() >= 0);
}

void CodeView::OnUpdateEditClearAllBookmarks(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetCtrl().MarkerNext(0,1 << CodeDocument::Bookmark) >= 0);
}

void CodeView::DefineMarker(int marker, int markerType, COLORREF fore, COLORREF back) 
{
	CScintillaCtrl& rCtrl = GetCtrl();

	rCtrl.MarkerDefine(marker, markerType);
	rCtrl.MarkerSetFore(marker, fore);
	rCtrl.MarkerSetBack(marker, back);
}

bool CodeView::Serialize(CIniFile &ini, LPCTSTR lpszKey, bool write)
{
	LPCTSTR const VAL_VIEWINFO_CURSOR = _T("Cursor");
	LPCTSTR const TopLine = _T("TopLine");

	if (write) {
		ini.SetValue(lpszKey,TopLine,GetCtrl().GetFirstVisibleLine());
		ini.SetValue(lpszKey,VAL_VIEWINFO_CURSOR,GetCtrl().GetCurrentPos());
	}
	else {
		// TODO: Figure out how to set the top line
		//GetCtrl().GotoLine(ini.GetValue(lpszKey,TopLine,0));

		// Go to the cursor position first
		GetCtrl().GotoPos(ini.GetValue(lpszKey,VAL_VIEWINFO_CURSOR,0));		
	}

	return true;
}

void CodeView::OnMarginClick(SCNotification* n)
{
	if (n->margin == 1) // Bookmark/Errormark margin
		GetDocument()->ToggleBookmark(GetCtrl().LineFromPosition(n->position));
	else
		CScintillaView::OnMarginClick(n); // Toggle folding
}

void CodeView::OnUpdateUI(SCNotification* n)
{
}

void CodeView::OnEditFindIncrementalForward()
{
	shadow_.SetSearchForward();
	shadow_.EnableIncrementalSearch(true);
}

void CodeView::OnEditFindIncrementalBackward()
{
	shadow_.SetSearchForward(false);
	shadow_.EnableIncrementalSearch(true);
}

void CodeView::GetReplaceAllTarget(long& s, long& e)
{
	CScintillaCtrl& c = GetCtrl();
	bool do_default = true;

	if (FindReplaceDlg* d = dynamic_cast<FindReplaceDlg*>(CScintillaFindReplaceDlg::GetFindReplaceDlg())) {
		d->UpdateData();

		if (d->ReplaceSelection()) {
			s = c.GetSelectionStart();
			e = c.GetSelectionEnd();
			do_default = false;
		}
	}

	if (do_default)
		CScintillaView::GetReplaceAllTarget(s,s);
}

CScintillaFindReplaceDlg* CodeView::CreateFindReplaceDialog(void)
{
	return new FindReplaceDlg;
}

bool CodeView::IsAutoIndentEnabled() const
{
	return shadow_.autoindent_enabled;
}

void CodeView::EnableAutoIndent( bool enable /*= true*/ )
{
	shadow_.autoindent_enabled = enable;
}

void CodeView::OnModified(SCNotification* n)
{
	CScintillaView::OnModified(n);

	last_change_pos_ = GetCtrl().GetCurrentPos();
}

void CodeView::OnEditGotoLastChange()
{
	GetCtrl().GotoPos(last_change_pos_);
}

void CodeView::OnUpdateEditGotoLastChange(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(last_change_pos_ != -1);
}

void CodeView::EnableFolding( bool enable /*= true*/ )
{
	GetCtrl().SetProperty("fold",enable ? "1" : "0");
}

bool CodeView::IsFoldingEnabled()
{
	return GetCtrl().GetPropertyInt("fold") == 1;
}

void CodeView::OnViewIndentationGuides()
{
	bool enable = !CConfiguration::GetInstance()->GetShowIndentationGuides();
	GetCtrl().SetIndentationGuides(enable ? SC_IV_LOOKBOTH : SC_IV_NONE);
	CConfiguration::GetInstance()->SetShowIndentationGuides(enable);
}

void CodeView::OnUpdateViewIndentationGuides(CCmdUI *pCmdUI)
{
	bool check = CConfiguration::GetInstance()->GetShowIndentationGuides();
	pCmdUI->SetCheck(check);
}
