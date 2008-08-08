#include "stdafx.h"
#include "TeXnicCenter.h"
#include "LaTeXView.h"

#include "Advice.h"
#include "AutoCompleteDlg.h"
#include "../CrysEditEx/Source/CharType.h"
#include "Configuration.h"
#include "gotodialog.h"
#include "LaTeXDocument.h"
#include "TextOutsourceDlg.h"

#pragma region Helper functions and classes

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

class LaTeXViewListener :
	public CAutoCompleteListener
{
public:

	LaTeXViewListener(LaTeXView *parent)
	{
		ASSERT(parent != NULL);
		p = parent;
	}

	/* Command was selected */
	virtual void OnACCommandSelect(const CLaTeXCommand *cmd)
	{
		p->OnACCommandSelect(cmd);
	}

	/* Auto complete was cancelled */
	virtual void OnACCommandCancelled()
	{
		p->OnACCommandCancelled();
	};

	/* User demands context help (F1 key) */
	virtual void OnACHelp(CString &command)
	{
		p->OnACHelp(command);
	}

	/* Backspace was pressed (affects keyword!) */
	virtual void OnACBackspace()
	{
		p->OnACBackspace();
	}

	/* A TCHAR was typed (affects keyword!) */
	virtual void OnACChar(UINT nKey, UINT nRepCount, UINT nFlags)
	{
		p->OnACChar(nKey, nRepCount, nFlags);
	}
private:
	LaTeXView *p;
};

#pragma endregion

// LaTeXView

IMPLEMENT_DYNCREATE(LaTeXView, CScintillaView)

LaTeXView::LaTeXView()
: m_AutoCompleteActive(false)
, m_InstTip(0)
, m_CompletionListBox(0)
, m_Proxy(new LaTeXViewListener(this))
, m_pBackgroundThread(theApp.GetBackgroundThread())
, m_bCreateBackupFile(false)
, encoding_(ASCII)
, code_page_(CP_ACP)
{
}

LaTeXView::~LaTeXView()
{
}

BEGIN_MESSAGE_MAP(LaTeXView, CScintillaView)
	ON_COMMAND(ID_EDIT_GOTO, &LaTeXView::OnEditGoto)
	ON_COMMAND(ID_EDIT_TOGGLE_WHITESPACEVIEW, &LaTeXView::OnEditToggleWhiteSpaceView)
	ON_COMMAND(ID_EDIT_TOGGLE_SHOW_LINE_ENDING, &LaTeXView::OnEditToggleShowLineEnding)
	ON_UPDATE_COMMAND_UI(ID_EDIT_TOGGLE_WHITESPACEVIEW, &LaTeXView::OnUpdateEditToggleWhiteSpaceView)
	ON_UPDATE_COMMAND_UI(ID_EDIT_TOGGLE_SHOW_LINE_ENDING, &LaTeXView::OnUpdateEditToggleShowLineEnding)
	ON_COMMAND(ID_SPELL_FILE, &LaTeXView::OnSpellFile)
	ON_COMMAND(ID_EDIT_DELETE_LINE, &LaTeXView::OnEditDeleteLine)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_OUTSOURCE, &LaTeXView::OnEditOutsource)
	ON_COMMAND(ID_QUERY_COMPLETION, &LaTeXView::OnQueryCompletion)
END_MESSAGE_MAP()


// LaTeXView drawing

// LaTeXView diagnostics

#ifdef _DEBUG
void LaTeXView::AssertValid() const
{
	CScintillaView::AssertValid();
}

#ifndef _WIN32_WCE
void LaTeXView::Dump(CDumpContext& dc) const
{
	CScintillaView::Dump(dc);
}
#endif
#endif //_DEBUG


// LaTeXView message handlers

void LaTeXView::OnInitialUpdate()
{
    CScintillaView::OnInitialUpdate();

    theme_.SubclassWindow(m_hWnd);
    theme_.Initialize();

	old_pos_start_ = old_pos_end_ = -1;
	m_AutoCompleteActive = false;

	CScintillaCtrl& rCtrl = GetCtrl();
	rCtrl.SetLexer(SCLEX_TEX); // TeX Lexer

	rCtrl.SetPasteConvertEndings(TRUE); // Convert line endings

	UpdateSettings();
}

void LaTeXView::SetAStyle(int style, COLORREF fore, COLORREF back, int size, LPCTSTR face) 
{
    CScintillaCtrl& rCtrl = GetCtrl();

    rCtrl.StyleSetFore(style, fore);
    rCtrl.StyleSetBack(style, back);

    if (size >= 1)
        rCtrl.StyleSetSize(style, size);
    if (face) 
        rCtrl.StyleSetFont(style, face);
}

void LaTeXView::OnCharAdded(SCNotification* n)
{
    if (GetDocument()->IsModified())
        GetDocument()->SetModifiedFlag();

	long pos = GetCtrl().GetCurrentPos();

	if (pos > 0) {
		TCHAR ch = n->ch;

		switch (ch) {
			case _T('"') :
				if (CConfiguration::GetInstance()->m_bReplaceQuotationMarks) {
					long start = pos;
					
					GetCtrl().SetSel(pos - 1,pos);
					GetCtrl().ReplaceSel(_T(""));

					pos = GetCtrl().GetCurrentPos();

					// opening quotation mark, if character is first in line
					if (start < 1) {
						InsertText(CConfiguration::GetInstance()->m_strOpeningQuotationMark);					
					}
					else {
						// opening quotation mark, if character left of selection is whitespace or open brace					
						TCHAR cLeft = GetCtrl().GetCharAt(pos - 1);

						if (!cLeft)
							cLeft = _T(' ');

						CString mark;

						if (CharTraitsT::IsSpace(cLeft) || cLeft == _T('(') || cLeft == _T('{') || cLeft == _T('['))
							mark = CConfiguration::GetInstance()->m_strOpeningQuotationMark;
						else
							mark = CConfiguration::GetInstance()->m_strClosingQuotationMark;

						GetCtrl().InsertText(pos,mark);
						
						// Update position
						pos += mark.GetLength();
						// Set the current position
						GetCtrl().SetCurrentPos(pos);
						// Remove any possible selection
						GetCtrl().SetSel(-1,pos);
					}
				}

				break;
			default:
				if (ch)
					InstantAdvice();
		}
	}
}

void LaTeXView::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
		SaveToFile(ar.GetFile()->m_hFile);
    else
		LoadFromFile(ar.GetFile()->m_hFile);
}

LaTeXView::Encoding LaTeXView::GetEncoding() const
{
	return encoding_;
}

UINT LaTeXView::GetCodePage() const
{
	return code_page_;
}

LaTeXView::Encoding LaTeXView::DetectEncoding(const BYTE* data, SIZE_T& pos, SIZE_T size)
{
	// By default we assume ASCII text
	Encoding encoding = ASCII;
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

COLORREF LaTeXView::GetAutomaticColor(int nColorIndex)
{
	switch (nColorIndex) {
		case COLORINDEX_WHITESPACE:
		case COLORINDEX_BKGND:
			return ::GetSysColor(COLOR_WINDOW);
		case COLORINDEX_NORMALTEXT:
			return ::GetSysColor(COLOR_WINDOWTEXT);
		case COLORINDEX_SELMARGIN:
			return ::GetSysColor(COLOR_BTNFACE/*COLOR_SCROLLBAR*/);
		case COLORINDEX_PREPROCESSOR:
			return RGB(0x00,0x00,0x80);
		case COLORINDEX_COMMENT:
			return RGB(0x80,0x80,0x80);
		case COLORINDEX_NUMBER:
			return RGB(0x00,0x80,0x00);
		case COLORINDEX_OPERATOR:
			return RGB(0xFF,0x00,0x00);
		case COLORINDEX_STRING:
			return RGB(0xFF,0x00,0xFF);
		case COLORINDEX_KEYWORD:
			return RGB(0x00,0x00,0xFF);
		case COLORINDEX_SELBKGND:
			return ::GetSysColor(COLOR_HIGHLIGHT);
		case COLORINDEX_SELTEXT:
			return ::GetSysColor(COLOR_HIGHLIGHTTEXT);
		case COLORINDEX_PAIRSTRINGBKGND:
			return RGB(0xC0,0xFF,0xC0);
		case COLORINDEX_PAIRSTRINGTEXT:
			return RGB(0x00,0x00,0x00);
		case COLORINDEX_BADPAIRSTRINGBKGND:
			return RGB(0xFF,0xC0,0xC0);
		case COLORINDEX_BADPAIRSTRINGTEXT:
			return RGB(0x00,0x00,0x00);
		case COLORINDEX_CURPAIRSTRINGBKGND:
			return RGB(0xC0,0xFF,0xC0);
		case COLORINDEX_CURPAIRSTRINGTEXT:
			return RGB(0x00,0x00,0x00);
		case COLORINDEX_PAIRBLOCKBKGND:
			return RGB(0xFF,0xFF,0xC0);
		default:
			ASSERT(false); //ColorIndex not found? Shouldn't happen. Please update.
	}

	return RGB(0xFF,0x00,0x00);
}

COLORREF LaTeXView::GetColor(int nColorIndex)
{
	if (nColorIndex >= 0 && nColorIndex < COLORINDEX_ERRORBKGND && CConfiguration::GetInstance()->m_aEditorColors[nColorIndex] != 0xFFFFFFFF)
		return CConfiguration::GetInstance()->m_aEditorColors[nColorIndex];

	// user set automatic color
	return GetAutomaticColor(nColorIndex);
}

void LaTeXView::UpdateSettings()
{
	CScintillaCtrl& rCtrl = GetCtrl();

	CWindowDC dc(this);

	const LOGFONT& editor_font = CConfiguration::GetInstance()->m_fontEditor;

	CFont font;
	font.CreateFontIndirect(&editor_font);

	CFont* oldfont = dc.SelectObject(&font);

	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);

	dc.SelectObject(&oldfont);

	const int style_number = (tm.tmHeight - tm.tmInternalLeading) * 72 / dc.GetDeviceCaps(LOGPIXELSY);
	style_number_ = style_number;

	SetAStyle(STYLE_DEFAULT, GetColor(COLORINDEX_NORMALTEXT),GetColor(COLORINDEX_BKGND),
		style_number,CConfiguration::GetInstance()->m_fontEditor.lfFaceName);

	SetAStyle(SCE_TEX_DEFAULT, GetColor(COLORINDEX_COMMENT)); // Includes the comment
	SetAStyle(SCE_TEX_GROUP, RGB(0,0,0));
	SetAStyle(SCE_TEX_TEXT,GetColor(COLORINDEX_NORMALTEXT));
	SetAStyle(SCE_TEX_SPECIAL, RGB(255,0,255));
	SetAStyle(SCE_TEX_SYMBOL, RGB(0,165,0));
	rCtrl.StyleSetBold(SCE_TEX_COMMAND,TRUE);

	rCtrl.UsePopUp(FALSE);
	rCtrl.SetTabWidth(CConfiguration::GetInstance()->m_nTabWidth);
	rCtrl.SetUseTabs(TRUE); // TODO: Let the user choose
	rCtrl.SetIndent(rCtrl.GetTabWidth());

	SetAStyle(SCE_TEX_COMMAND, GetColor(COLORINDEX_KEYWORD));
	rCtrl.SetSelFore(TRUE,GetColor(COLORINDEX_SELTEXT));
	rCtrl.SetSelBack(TRUE,GetColor(COLORINDEX_SELBKGND));

	rCtrl.StyleSetItalic(style_number,editor_font.lfItalic);
	rCtrl.StyleSetItalic(style_number,editor_font.lfWeight >= FW_BOLD);
	//rCtrl.StyleSetSize(style_number,font_height);
		
}

void LaTeXView::OnUpdateUI(SCNotification* n)
{
	
}

void LaTeXView::InstantAdvice()
{
	CString keyw,key;
	long ptStart = GetCtrl().GetCurrentPos();
	CPoint ptClient;

	GetWordBeforeCursor(keyw,ptStart,false);

	if (keyw.GetLength() > MINIMUM_KEYWORD_LENGTH) {
		CMapStringToOb map;

		theApp.m_AvailableCommands.GetAllPossibleItems(keyw,_T(""),map);

		if (map.GetCount() == 1) { //keyword is unique -> OK
			CLaTeXCommand *lc;
			POSITION pos = map.GetStartPosition();
			map.GetNextAssoc(pos,key,(CObject*&)lc);

			if (lc != NULL) {
				if (m_InstTip == NULL) {
					m_InstTip = new CAdvice();
					m_InstTip->Create(lc->ToLaTeX(),WS_POPUP | WS_BORDER/*SS_SUNKEN*/,CRect(),CWnd::FromHandle(GetCtrl()));
				}

				if (!m_InstTip->IsWindowVisible()) {
					// Compute window size
					CPoint ptClient(GetCtrl().PointXFromPosition(ptStart),GetCtrl().PointYFromPosition(ptStart));
					::ClientToScreen(GetSafeHwnd(),&ptClient);
					// Determine if there is space enough to show the window below the text
					ptClient.y += GetCtrl().TextHeight(GetCtrl().LineFromPosition(ptStart));
					// Place and show the window
					m_InstTip->SetWindowPos(NULL,ptClient.x,ptClient.y,0,0,SWP_NOSIZE | SWP_NOZORDER);
					m_InstTip->SetWindowText(lc->GetExpandBefore() + lc->ToLaTeX() + lc->GetExpandAfter());
					m_InstTip->ShowWindow(SW_SHOWNA);
					m_InstTip->SetTimer(1,5000,0);
					SetFocus();
				}
			}
			else
				HideAdvice();
		}
		else {
			// Nothing found: Hide window
			HideAdvice();
		}
	}
}

void LaTeXView::HideAdvice()
{
	if (m_InstTip)
		m_InstTip->ShowWindow(SW_HIDE);
}


#pragma region Message handlers for auto complete listbox

void LaTeXView::OnACCommandSelect(const CLaTeXCommand* cmd)
{
	long ptCaret;

	m_AutoCompleteActive = false;
	old_pos_start_ = old_pos_end_ = -1;

	ASSERT(cmd != NULL);

	//Generate string to be inserted
	CString InsertString = cmd->GetExpandBefore(); // collect completion...
	InsertString += cmd->ToLaTeX();
	InsertString += cmd->GetExpandAfter();

	long s = GetCtrl().GetSelectionStart();
	long e = GetCtrl().GetSelectionEnd();

	const long origs = s;

	GetCtrl().ReplaceSel(InsertString);
	s = GetCtrl().GetSelectionStart();
	e = GetCtrl().GetSelectionEnd();
	GetCtrl().SetSel(-1,s); // drop selection	

	bool isEnv = cmd->GetExpandAfter().GetLength() && cmd->GetExpandBefore().GetLength();

	if (!isEnv) {
		bool bSetPosition = false;

		//If a command was inserted and it contains a brace, we might want to go there
		if (InsertString.FindOneOf(_T("{[(")) != -1) {
			long ls = GetCtrl().LineFromPosition(s);
			long le = GetCtrl().LineFromPosition(e);

			//Search for the first brace
			for (long y = ls; y <= le; y++) {
				CString text = GetLineText(y);
				//Is there an offset? Only in first line.
				int Offset = 0;

				if (y == s) {
					Offset = s - GetCtrl().PositionFromLine(GetCtrl().LineFromPosition(s));
					text = text.Mid(Offset);
				}

				//Find a brace and place the cursor
				int BracePos = text.FindOneOf(_T("{[("));

				if (BracePos != -1) {
					ptCaret = GetCtrl().PositionFromLine(y) + Offset + BracePos + 1;

					if (ptCaret < GetCtrl().GetLength()) {
						//Place caret after the first opening brace
						GetCtrl().GotoPos(ptCaret);
						bSetPosition = true;
					}

					break;
				}
			}
		}

		// no brace found; drop selection and place cursor after inserted word
		if (!bSetPosition) 
			GetCtrl().GotoPos(e);
	}
	else {
		// env was inserted -> place cursor at end of next row
		s = origs;
		long l = GetCtrl().LineFromPosition(s);
		ptCaret = GetCtrl().GetLineEndPosition(l + 1);

		GetCtrl().GotoPos(ptCaret);
	}

	ptCaret = GetCtrl().GetCurrentPos();
	GetCtrl().SetSel(-1,ptCaret); // sync selection with C'pos
	RestoreFocus(); // set focus back to edit view
}

void LaTeXView::OnACCommandCancelled()
{
	/* This issue is a little bit weird: The AC window sends a OnACCommandCancelled in every case the window is close
	in order to react on KillFocus msgs. If the completion was finished OnACCommandSelect, we must prohibit to execute
	OnACCommandCancelled in this case. So we use the m_AutoCompleteActive flag to determine, if the autocomplete
	has been finished or not */

	if (m_AutoCompleteActive) {
		/* try to restore old cursor pos */
		if (old_pos_start_ != -1 && old_pos_end_ != -1)
			GetCtrl().SetSel(old_pos_start_,old_pos_end_);
		else {  /* suggest new position */
			long pos = GetCtrl().GetSelectionEnd();
			GetCtrl().SetSel(-1,pos);
		}

		old_pos_start_ = old_pos_end_ = -1;
		m_AutoCompleteActive = false;
		RestoreFocus();
	}
}

void LaTeXView::RestoreFocus()
{
	SetFocus();
}

void LaTeXView::OnACChar(UINT nKey, UINT nRepCount, UINT nFlags)
{
	if (m_CompletionListBox && m_CompletionListBox->IsWindowVisible()) {
		long pos = GetCtrl().GetCurrentPos();
		const TCHAR text[] = {static_cast<TCHAR>(nKey),0};

		GetCtrl().InsertText(pos,text);
		GetCtrl().GotoPos(pos + 1);
		//CPoint ps,pe,dummy;
		//GetSelection(ps,pe);
		//SetSelection(pe,pe);
		//CCrystalEditViewEx::OnChar(nKey,nRepCount,nFlags);
		//GetSelection(dummy,pe); /* retrieve new cursor pos */
		//SetSelection(ps,pe); /* restore selection */
	}
}

void LaTeXView::OnACBackspace()
{
	if (m_CompletionListBox && m_CompletionListBox->IsWindowVisible())
	{
		//CPoint ps,pe,dummy;
		//GetSelection(ps,pe);
		long s = GetCtrl().GetCurrentPos();
		
		if (s > 0) {
			GetCtrl().SetSel(s - 1,s);
			GetCtrl().ReplaceSel(_T(""));
			GetCtrl().SetSel(-1,GetCtrl().GetCurrentPos());
		}

		//dummy.x = pe.x - 1;
		//dummy.y = pe.y;
		//SetSelection(dummy,pe);
		////CCrystalEditViewEx::OnChar(VK_BACK, 1, 0);
		//if (ReplaceSelection(_T("")))   /* WÜRG !!!! */
		//{
		//	GetSelection(dummy,pe); /* retrieve new cursor pos */
		//	SetSelection(ps,pe); /* restore selection */
		//}
		//else
		//{
		//	TRACE("Could not delete selection!\n");
		//}
	}
}

void LaTeXView::OnACHelp(CString &cmd)
{
	//InvokeContextHelp(cmd);
}

#pragma endregion

CAutoCompleteDlg* LaTeXView::CreateListBox( CString &keyword, long pos )
{
	//TRACE("==> CreateListBox\n");

	CPoint ptStart(GetCtrl().PointXFromPosition(pos),GetCtrl().PointYFromPosition(pos));
	ptStart.y += GetCtrl().TextHeight(GetCtrl().LineFromPosition(pos)); // Goto next row
	ptStart.y += ::GetSystemMetrics(SM_CYDLGFRAME);

	// setup listbox
	int nWords = GetNumberOfMatches(keyword); // find number of matches
	//TRACE("==> CreateListBox matches = %d\n", nWords);

	//Found one or more matches?
	if (nWords >= 1)
	{
		//Create window, if needed
		if (m_CompletionListBox == NULL)
		{
			m_CompletionListBox = new CAutoCompleteDlg(&theApp.m_AvailableCommands,this /*theApp.GetMainWnd()*/);
			m_CompletionListBox->SetListener(m_Proxy);
		}

		//InitWithKeyword will notify the listener immediately, if only one exact match exists
		if (m_CompletionListBox->InitWithKeyword(keyword) && nWords > 1)
		{
			//TRACE("==> CreateListBox: Show listbox \n");
			m_AutoCompleteActive = TRUE;

			//Move box by (16 IconPixels + 2 Pixels between Icon and text + 6/2 BorderPixels + 1 Pixel Magick) = 22
			//This way, the text matches with the editor
			ClientToScreen(&ptStart); //yes, this works only with screen coords. do not ask me why.
			m_CompletionListBox->SetWindowPos(NULL,ptStart.x - 22,ptStart.y,0,0,SWP_NOSIZE | SWP_NOZORDER
				| SWP_NOACTIVATE);

			//Adjust size of the box to see all content and we do not want to be over the border
			m_CompletionListBox->AdjustSizeAndPosition(GetCtrl().TextHeight(GetCtrl().LineFromPosition(pos)));

			m_CompletionListBox->ShowWindow(SW_SHOW);
			m_CompletionListBox->SetCurSel(0);
		}
		else
		{
			//TRACE("==> CreateListBox: NOT Show listbox \n");
		}
	}

	//TRACE("<== CreateListBox\n");
	return m_CompletionListBox;
}

int LaTeXView::GetNumberOfMatches( const CString& keyword )
{
	if (keyword.GetLength() < CAutoCompleteDlg::GetMinimumKeywordLength()) return 0;

	CMapStringToOb map;
	theApp.m_AvailableCommands.GetAllPossibleItems(keyword,_T(""),map);
	return map.GetCount();
}

void LaTeXView::GoToLine(int line)
{
	GetCtrl().GotoLine(line);
}

int LaTeXView::GetLineLength(int line)
{
	return GetCtrl().GetLineEndPosition(line) - GetCtrl().PositionFromLine(line);
}

void LaTeXView::GetWordBeforeCursor(CString& strKeyword, long& a, bool bSelect /*=true*/)
{
	long pos = a;
	int line = GetCtrl().LineFromPosition(pos);
	int length = GetLineLength(line);
	
	if (length > 0) {
		//Get the line, the cursor is placed on
		const CString strLine = GetLineText(line);
		long start = GetCtrl().PositionFromLine(line);

		// Get the position from where we start the backward search
		long l = pos - start;

		if (l <= strLine.GetLength()) {
			long EndX = (l <= strLine.GetLength()) ? l - 1 : l + strLine.GetLength() - 1;
			long CurrentX = EndX;

			//Backward search: go to first character of the current word
			for (; CurrentX >= 0; CurrentX--) {
				if (!IsAutoCompletionCharacter(strLine[CurrentX])) {
					++CurrentX; //This is the last valid TCHAR
					break;
				}
			}

			if (CurrentX < 0) 
				CurrentX = 0;

			if (CurrentX <= EndX) {
				ASSERT(CurrentX >= 0);
				ASSERT(EndX - CurrentX + 1 > 0);

				strKeyword = strLine.Mid(CurrentX,EndX - CurrentX + 1);

				if (bSelect)
					GetCtrl().SetSel(start + CurrentX,pos);

				a = start + CurrentX;
			}
			else
				strKeyword.Empty();
		}
	}
}

bool LaTeXView::IsAutoCompletionCharacter(TCHAR tc)
{
	switch (tc) {
			//All the following chars are allowed in labels.
			// - but we do not allow all since some of them are rather strange
		case _T('&') :
		case _T('_') :
		case _T('-') :
		case _T('+') :
		case _T(':') :
			//case _T('='):
			//case _T('^'):
			//case _T('.'):
			//case _T(';'):
			//case _T(','): //A comma can be used in \cite{weinkauf04a,weinkauf05b} and we want to see this as two bibkeys
			//case _T('!'):
			//case _T('`'):
			//case _T('´'):
			//case _T('('): //Especially braces should not be part of a label; this would kill most editor stuff
			//case _T(')'):
			//case _T('['):
			//case _T(']'):
			//case _T('<'):
			//case _T('>'):

			//All the following chars are allowed in keywords.
		case _T('\\') :
		case _T('@') :
			return TRUE;

			//Be default, labels can consist of numbers and letters; keywords only of letters
		default:
			return CharTraitsT::IsAlnum(tc);
	}
}
void LaTeXView::OnEditGoto()
{
	CGotoDialog dlg(GetLineCount(),this);
	dlg.m_nLine = GetCurrentLine() + 1;

	if (dlg.DoModal() == IDOK)
		GoToLine(dlg.m_nLine - 1);
}

int LaTeXView::GetLineCount(void)
{
	return GetCtrl().GetLineCount();
}

int LaTeXView::GetCurrentLine(void)
{
	return GetCtrl().LineFromPosition(GetCtrl().GetCurrentPos());
}

const CString LaTeXView::GetLineText( int line )
{
	const int length = GetLineLength(line);

	CString strLine;
	GetCtrl().GetLineEx(line,strLine.GetBuffer(length),length);
	strLine.ReleaseBuffer(length);

	return strLine;
}

void LaTeXView::InsertText( const CString& text )
{
	long pos = GetCtrl().GetCurrentPos();
	GetCtrl().InsertText(pos,text);
	GetCtrl().GotoPos(pos + text.GetLength());
}

void LaTeXView::OnEditToggleWhiteSpaceView()
{
	bool visible = GetCtrl().GetViewWS() != SCWS_INVISIBLE;
	GetCtrl().SetViewWS(visible ? SCWS_INVISIBLE : SCWS_VISIBLEALWAYS);
}

void LaTeXView::OnEditToggleShowLineEnding()
{
	bool visible = GetCtrl().GetViewEOL() != 0;
	GetCtrl().SetViewEOL(!visible);
}

void LaTeXView::OnUpdateEditToggleWhiteSpaceView(CCmdUI *pCmdUI)
{
	bool visible = GetCtrl().GetViewWS() != SCWS_INVISIBLE;
	pCmdUI->SetCheck(visible);
}

void LaTeXView::OnUpdateEditToggleShowLineEnding(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(GetCtrl().GetViewEOL());
}

void LaTeXView::OnSpellFile()
{
	//// Save selection
	//long s = GetCtrl().GetSelectionStart();
	//long e = GetCtrl().GetSelectionEnd();

	////SetShowInteractiveSelection(TRUE);
	//
	//CCrystalResources cr;
	//CSpellCheckDlg dlg(this,theApp.GetSpeller());

	//dlg.DoModal();
	////SetShowInteractiveSelection(FALSE);

	//// Restore selection
	//GetCtrl().SetSel(s,e);
}

#pragma region Reading & writing to/from files

DWORD LaTeXView::LoadFromFile(LPCTSTR pszFileName)
{
	DWORD result = ERROR_SUCCESS;
	CAtlFile file;

	if (SUCCEEDED(file.Create(pszFileName,GENERIC_READ,FILE_SHARE_READ,OPEN_EXISTING)))
		result = LoadFromFile(file);
	else
		result = ::GetLastError();

	return result;
}

DWORD LaTeXView::LoadFromFile(HANDLE file)
{
	DWORD result = ERROR_SUCCESS;
	ATL::CAtlFileMapping<BYTE> mapping;

	if (SUCCEEDED(mapping.MapFile(file))) {
		const BYTE* data = mapping;
		SIZE_T size = mapping.GetMappingSize();
		SIZE_T pos = 0;

		Encoding encoding = DetectEncoding(data,pos,size);

		if (encoding == ASCII) { // ASCII? Still might by Unicode without BOM
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

				for (std::size_t i = 0; i < size && utf8; ++i) {
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

				if (utf8)
					encoding = UTF8;
			}
		}

		UINT code_page;

		switch (encoding) {
			case ASCII: code_page = CP_ACP; break;
			case UTF8: code_page = CP_UTF8; break;
			case UTF16LE: code_page = 1200; break; // Available only for managed applications
			case UTF16BE: code_page = 1201; break;
			case UTF32LE: code_page = 12000; break;
			case UTF32BE: code_page = 12001; break;
			default: ASSERT(FALSE);
		}

		LPTSTR text;
		int required_size;

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

		GetCtrl().SetCodePage(SC_CP_UTF8);
		GetCtrl().SetText(text);

		// Detected EOL type
		int mode = SC_EOL_CRLF;

		if (!_tcsstr(text,_T("\r\n"))) {
			if (_tcsstr(text,_T("\r")))
				mode = SC_EOL_CR;
			else if (_tcsstr(text,_T("\n")))
				mode = SC_EOL_LF;
		}

		GetCtrl().SetEOLMode(mode);

		delete[] text;

		code_page_ = code_page;
		encoding_ = encoding;
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

void LaTeXView::ConvertToMultiByte(LPCWSTR input, int cch, std::vector<BYTE>& buffer, Encoding encoding, UINT cp)
{
	if (encoding != UTF16LE && encoding != UTF16BE) {
		int required_size = ::WideCharToMultiByte(cp,0,input,cch,0,0,0,0);

		if (required_size > 0) {
			buffer.resize(--required_size); // We don't need the terminating null, strip it
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

DWORD LaTeXView::SaveToFile(LPCTSTR pszFileName, bool bClearModifiedFlag)
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
			SaveToFile(temp_file);

			if (m_bCreateBackupFile) {
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
					GetDocument()->SetModifiedFlag(FALSE);

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

DWORD LaTeXView::SaveToFile( HANDLE file, LPCWSTR text, int length)
{
	CAtlFile f(file);
	DWORD result = ERROR_SUCCESS;

	if (encoding_ != ASCII) {
		const BYTE* bom;
		SIZE_T size;

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

DWORD LaTeXView::SaveToFile( HANDLE file )
{
	const int length = GetCtrl().GetLength();
	DWORD result = ERROR_SUCCESS;

	if (length > 0) {
		LPWSTR text = new WCHAR[length + 1];
		GetCtrl().GetText(length + 1,text);
		
		CAtlFile f(file);
		result = SaveToFile(file,text,length);
		
		delete[] text;
	}

	return result;
}

#pragma endregion
void LaTeXView::OnEditDeleteLine()
{
	GetCtrl().LineDelete();
}

void LaTeXView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CPoint ptText = point;
	ScreenToClient(&ptText);

	long pos = GetCtrl().PositionFromPoint(ptText.x,ptText.y);
	bool bShowDefault = true;

	// TODO :

	//CCrystalTextBuffer::CTextAttribute *attr = pBuffer->GetLineAttribute(ptText.y,ptText.x,ptText.x + 1);

	//if (attr) {
	//	SpellerSuggestionMenu menu(this,ptText);

	//	if (attr->m_Attribute == CCrystalTextBuffer::CTextAttribute::spellError)
	//		bShowDefault = !menu.ShowSpellMenu(theApp.GetSpeller(),point);
	//	// else your new attribute here
	//}

	if (bShowDefault) {
		// Default popup
		theApp.ShowPopupMenu(IDR_POPUP_EDITOR,point,this);
	}
}

void LaTeXView::OnEditOutsource()
{
	//Create the dialog
	CTextOutsourceDlg OutsourceDlg;

	//Get the file name of ourselves
	LaTeXDocument* pDoc = GetDocument();

	if (!pDoc) return;

	CPathTool OldPath = pDoc->GetPathName();

	//Suggest a directory for the new file
	OutsourceDlg.m_Directory = OldPath.GetDirectory();

	//Show the dialog
	if (OutsourceDlg.DoModal() == IDOK) {
		//Get the new file name and write the selected lines to it
		CFile NewFile;

		if (NewFile.Open(OutsourceDlg.NewPath.GetPath(),CFile::modeWrite | CFile::modeCreate)) {
			//Get selected text - may be empty
			long s = GetCtrl().GetSelectionStart();
			long e = GetCtrl().GetSelectionEnd();

			CString strSelectedText;
			
			if (s != e) {
				GetCtrl().GetSelText(strSelectedText.GetBuffer(e - s));
				strSelectedText.ReleaseBuffer();
			}

			//Write it to the file
			SaveToFile(NewFile,strSelectedText,strSelectedText.GetLength());
			NewFile.Close();

			//Get relative path - omit tex-extension
			CString RelativeFilePath = CPathTool::GetRelativePath(OldPath.GetDirectory(),
				(OutsourceDlg.NewPath.GetFileExtension() == _T("tex"))
				? CPathTool(OutsourceDlg.NewPath.GetBase())
				: OutsourceDlg.NewPath);

			//Insert the text into this document
			GetCtrl().ReplaceSel(OutsourceDlg.CmdLeft + RelativeFilePath + OutsourceDlg.CmdRight);

			//Open the new file
			if (OutsourceDlg.m_bOpenNewFile) {
				//Open it
				theApp.OpenLatexDocument(OutsourceDlg.NewPath,FALSE,-1,FALSE,false);

				//In background? Foreground is automatically done by the line above.
				if (OutsourceDlg.m_nOpenNewFileType == 0 /*background*/) {
					//Re-activate this view (I tried to open the new doc in background, but it did not work out)
					CFrameWnd* pFrame = GetParentFrame();

					if (pFrame) 
						pFrame->ActivateFrame();
				}
			}
		}
	}
}

LaTeXDocument* LaTeXView::GetDocument() const
{
	return static_cast<LaTeXDocument*>(__super::GetDocument());
}

void LaTeXView::OnQueryCompletion()
{
	HideAdvice();

	// Don't allow a second window
	if (m_AutoCompleteActive) {
		TRACE("==> autocomplete is active, quitting...\n");
		return;
	}

	/* store old position */
	old_pos_start_ = GetCtrl().GetSelectionStart();
	old_pos_end_ = GetCtrl().GetSelectionEnd();

	CString keyword;
	long topLeft = GetCtrl().GetCurrentPos();

	GetWordBeforeCursor(keyword,topLeft); /* retrieve word to be completed */

	if (!keyword.IsEmpty())
		m_CompletionListBox = CreateListBox(keyword,topLeft); /* setup (and show) list box */
	else
		GetCtrl().SetSel(old_pos_start_,old_pos_end_); /* restore old position */
}
