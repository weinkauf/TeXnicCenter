#include "stdafx.h"
#include "TeXnicCenter.h"

#include <string>
#include <algorithm>
#include <cstddef>

#include "LaTeXView.h"
#include "Advice.h"
#include "AutoCompleteDlg.h"
#include "CharType.h"
#include "Configuration.h"
#include "global.h"
#include "LaTeXDocument.h"
#include "TextOutsourceDlg.h"
#include "insertfloatobjectdialog.h"
#include "InsertGraphicDialog.h"
#include "inserttabulardialog.h"
#include "insertheaderdialog.h"
#include "Speller.h"
#include "LaTeXTokenizer.h"
#include "SpellCheckDlg.h"

class SpellerSuggestionMenu
{
	LaTeXView* view_;
	long text_pos_;
	CMenu menu_;

public:
	SpellerSuggestionMenu(LaTeXView* view, long pos)
	: view_(view), text_pos_(pos)
	{
	}

	int TrackPopupMenu(CView* view, const CPoint& pt, CMenu* menu)
	{
		return theApp.GetContextMenuManager()->TrackPopupMenu(menu->m_hMenu,pt.x,pt.y,view);
	}

	BOOL ShowSpellMenu(Speller *pSpell, const CPoint& screenPoint);

protected:
	enum tagBase
	{
		none,
		spellErrorCount = MAXSUGGESTION,
		spellErrorStart = ID_SPELL_ERROR_FIRST,
		spellErrorStop = ID_SPELL_ERROR_LAST
	};
};

BOOL SpellerSuggestionMenu::ShowSpellMenu(Speller *pSpell, const CPoint& screenPoint)
{
	BOOL retValue = FALSE;

	int s = view_->GetCtrl().IndicatorStart(0,text_pos_,FALSE);
	int e = view_->GetCtrl().IndicatorEnd(0,text_pos_,FALSE);

	if (s != e) {
		std::vector<char> buffer;

		TextRange tr;
		tr.chrg.cpMin = s;
		tr.chrg.cpMax = e;
		tr.lpstrText = 0;

		buffer.resize((e - s) * 4 + 1); // Make some room
		tr.lpstrText = &buffer[0];

		view_->GetCtrl().GetTextRange(&tr,FALSE);
		bool clear = false;

		// Test the word again
		if (pSpell->SpellUTF8(&buffer[0])) {
			// Word was found, remove the attribute
			clear = true;
		}
		else {
			// FIX: Menu ID offset; context manager creates
			// a submenu for an item with the ID set to 0
			const UINT start = 1;
			USES_CONVERSION;

			// Get the suggestion list
			CStringArray aSuggestList;
			int nSuggestCount = pSpell->SuggestUTF8(W2CA(A2CW_CP(&buffer[0],CP_UTF8)),aSuggestList); // argh!

			if (!menu_)
				menu_.LoadMenu(IDR_POPUP_ATTRIBUTE);

			// Populate the menu
			CMenu* pPopup = menu_.GetSubMenu(1);

			ASSERT(pPopup);
			pPopup->InsertMenu(0, MF_BYPOSITION | MF_SEPARATOR, 0, _T(""));

			if (nSuggestCount == 0)
			{
				CString noSuggestions;
				noSuggestions.LoadString(IDS_SPELL_NO_SUGGESTIONS);
				pPopup->InsertMenu(0, MF_STRING | MF_GRAYED | MF_BYPOSITION | MF_DISABLED, start, noSuggestions);
			}
			else
			{
				if (nSuggestCount > spellErrorCount)
					nSuggestCount = spellErrorCount;

				for (int i = 0; i < nSuggestCount; ++i)
					pPopup->InsertMenu(i, MF_STRING | MF_ENABLED | MF_BYPOSITION, i + spellErrorStart, aSuggestList[i]);
			}

			// Place the menu under the text
			const int y = view_->GetCtrl().PointYFromPosition(text_pos_) + 
				view_->GetCtrl().TextHeight(view_->GetCtrl().LineFromPosition(text_pos_));

			// Use the y point for text's baseline
			CPoint pt(0,y);
			view_->ClientToScreen(&pt);

			// and the mouse x point
			pt.x = screenPoint.x;

			const int nSel = TrackPopupMenu(view_,pt,pPopup);

			if (nSel >= spellErrorStart && nSel < spellErrorStop)
			{
				// Save the previous selection
				long ptStart = view_->GetCtrl().GetSelectionStart(FALSE);
				long ptEnd = view_->GetCtrl().GetSelectionEnd(FALSE);

				// Replace the word
				view_->GetCtrl().BeginUndoAction();

				view_->GetCtrl().SetSel(s,e,FALSE);
				view_->GetCtrl().ReplaceSel(aSuggestList[nSel - spellErrorStart],FALSE);

				// Restore the old selection
				view_->GetCtrl().SetSel(ptStart, ptEnd,FALSE);

				view_->GetCtrl().EndUndoAction();
			}
			else if (nSel == ID_EDIT_SPELL_ADD)
			{
				pSpell->AddUTF8(&buffer[0]);
				clear = true;
			}
			else if (nSel == ID_EDIT_SPELL_IGNORE_ALL)
			{
				pSpell->IgnoreUTF8(&buffer[0]);
				clear = true;
			}

			if (clear)
				view_->GetCtrl().IndicatorClearRange(s,e - s,FALSE);

			// Trigger the line to be re-checked
			if (view_->GetDocument()->IsSpellerThreadAttached())
				view_->GetDocument()->GetSpellerThread()->
					RecheckSingleLineSpelling(view_->GetCtrl().LineFromPosition(text_pos_),view_);

			retValue = TRUE;
		}
	}

	return retValue;
}

#pragma region Helper functions and classes

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

IMPLEMENT_DYNCREATE(LaTeXView, CodeView)

LaTeXView::LaTeXView()
: autocompletion_active_(false)
, instant_advice_tip_(0)
, autocompletion_list_(0)
, listener_(new LaTeXViewListener(this))
{
}

LaTeXView::~LaTeXView()
{
	delete listener_;
}

BEGIN_MESSAGE_MAP(LaTeXView, CodeView)
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_SPELL_FILE, &LaTeXView::OnSpellFile)
	ON_COMMAND(ID_EDIT_DELETE_LINE, &LaTeXView::OnEditDeleteLine)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_OUTSOURCE, &LaTeXView::OnEditOutsource)
	ON_COMMAND(ID_QUERY_COMPLETION, &LaTeXView::OnQueryCompletion)
	ON_COMMAND_EX_RANGE(ID_INSERT_FORMULA_EMBEDDED, ID_INSERT_SPADESUIT, &LaTeXView::OnInsertLaTeXConstruct)
	ON_COMMAND_EX_RANGE(ID_INSERT_A,ID_INSERT_NOTIN, &LaTeXView::OnInsertLaTeXConstruct)
	ON_COMMAND_EX_RANGE(ID_INSERT_SUPERSCRIPT,ID_INSERT_DDOTS, &LaTeXView::OnInsertLaTeXConstruct)
	ON_COMMAND_EX_RANGE(ID_TEXTMODULES_FIRST,ID_TEXTMODULES_LAST, &LaTeXView::OnInsertLaTeXConstruct)
	ON_WM_CREATE()
	ON_COMMAND_EX(ID_EDIT_BLOCKCOMMENT_INSERT, &LaTeXView::OnBlockComment)
	ON_COMMAND_EX(ID_EDIT_BLOCKCOMMENT_REMOVE, &LaTeXView::OnBlockComment)
	ON_COMMAND_EX(ID_EDIT_BLOCKCOMMENT_TOOGLE, &LaTeXView::OnBlockComment)
END_MESSAGE_MAP()


// LaTeXView diagnostics

#ifdef _DEBUG
void LaTeXView::AssertValid() const
{
	CodeView::AssertValid();
}

#ifndef _WIN32_WCE
void LaTeXView::Dump(CDumpContext& dc) const
{
	CodeView::Dump(dc);
}
#endif
#endif //_DEBUG


// LaTeXView message handlers

int LaTeXView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CodeView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//EnableHardWrap();

	old_pos_start_ = old_pos_end_ = -1;
	autocompletion_active_ = false;

	CScintillaCtrl& rCtrl = GetCtrl();
	rCtrl.SetLexer(SCLEX_TEX); // TeX Lexer

	EnableAutoIndent();

	return 0;
}

void LaTeXView::OnInitialUpdate()
{
    CodeView::OnInitialUpdate();	
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
	CodeView::OnCharAdded(n);

	long pos = GetCtrl().GetCurrentPos();

	if (pos > 0) {
		char ch = static_cast<char>(n->ch);

		switch (ch) {
			case '"':
				if (CConfiguration::GetInstance()->m_bReplaceQuotationMarks) {
					const int prev_pos = GetCtrl().PositionBefore(GetCtrl().PositionBefore(pos));

					if (static_cast<char>(GetCtrl().GetCharAt(prev_pos)) != '\\') { // Skip the case \"
						long start = pos;
					
						GetCtrl().SetSel(GetCtrl().PositionBefore(pos),pos);
						GetCtrl().ReplaceSel("");

						pos = GetCtrl().GetCurrentPos();

						// opening quotation mark, if character is first in line
						if (start < 1) {
							InsertText(CConfiguration::GetInstance()->m_strOpeningQuotationMark);					
						}
						else {
							// opening quotation mark, if character left of selection is whitespace or open brace					
							char cLeft = static_cast<char>(GetCtrl().GetCharAt(GetCtrl().PositionBefore(pos)));

							if (!cLeft)
								cLeft = ' ';
							
							CString mark;

							if (CharTraitsA::IsSpace(cLeft) || cLeft == '(' || cLeft == '{' || cLeft == '[')
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
				}

				break;
			default:
				if (ch)
					InstantAdvice();
		}
	}
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

	const LOGFONT& editor_font = CConfiguration::GetInstance()->m_fontEditor;
	const int point_size = GetLogFontPointSize(editor_font);

	SetAStyle(STYLE_DEFAULT, GetColor(COLORINDEX_NORMALTEXT),GetColor(COLORINDEX_BKGND),
		point_size,CConfiguration::GetInstance()->m_fontEditor.lfFaceName);
	
	SetAStyle(SCE_TEX_DEFAULT, GetColor(COLORINDEX_COMMENT)); // Includes comments' color
	SetAStyle(SCE_TEX_GROUP,RGB(125,167,217));
	
	SetAStyle(SCE_TEX_TEXT,GetColor(COLORINDEX_NORMALTEXT));
	SetAStyle(SCE_TEX_SPECIAL, RGB(158,11,15));
	SetAStyle(SCE_TEX_SYMBOL, RGB(145,0,145));

#pragma region Comments 

	// Comments displayed in italics
	rCtrl.StyleSetFont(SCE_TEX_DEFAULT,editor_font.lfFaceName);
	rCtrl.StyleSetSize(SCE_TEX_DEFAULT,point_size);
	rCtrl.StyleSetItalic(SCE_TEX_DEFAULT,TRUE);

#pragma endregion

	rCtrl.UsePopUp(FALSE);
	rCtrl.SetTabWidth(CConfiguration::GetInstance()->m_nTabWidth);
	rCtrl.SetUseTabs(TRUE); // TODO: Let the user choose
	rCtrl.SetIndent(rCtrl.GetTabWidth());

	SetAStyle(SCE_TEX_COMMAND, GetColor(COLORINDEX_KEYWORD));
	rCtrl.SetSelFore(TRUE,GetColor(COLORINDEX_SELTEXT));
	rCtrl.SetSelBack(TRUE,GetColor(COLORINDEX_SELBKGND));

	rCtrl.StyleSetItalic(STYLE_DEFAULT,editor_font.lfItalic);
	rCtrl.StyleSetItalic(STYLE_DEFAULT,editor_font.lfWeight >= FW_BOLD);

#pragma region Brace highlighting

	rCtrl.StyleSetFont(STYLE_BRACELIGHT,editor_font.lfFaceName);
	rCtrl.StyleSetSize(STYLE_BRACELIGHT,point_size);
	rCtrl.StyleSetBold(STYLE_BRACELIGHT,TRUE);
	rCtrl.StyleSetFore(STYLE_BRACELIGHT,RGB(0,0,0));

	rCtrl.StyleSetFont(STYLE_BRACEBAD,editor_font.lfFaceName);
	rCtrl.StyleSetSize(STYLE_BRACEBAD,point_size);
	rCtrl.StyleSetBold(STYLE_BRACEBAD,TRUE);
	rCtrl.StyleSetFore(STYLE_BRACEBAD,RGB(255,0,0));

#pragma endregion

#pragma region Caret

	rCtrl.SetCaretStyle(CConfiguration::GetInstance()->IsInsertCaretLine() ? CARETSTYLE_LINE : CARETSTYLE_BLOCK);
	rCtrl.SetCaretPeriod(CConfiguration::GetInstance()->IsBlinkInsertCaret() ? 500 : 0);

#pragma endregion
}

#pragma region Scintilla notifications

void LaTeXView::OnUpdateUI(SCNotification* n)
{
	CodeView::OnUpdateUI(n);

	long pos = GetCtrl().GetCurrentPos();

	int line = GetCtrl().LineFromPosition(pos);
	long line_start_pos = GetCtrl().PositionFromLine(line);
	long line_end_pos = GetCtrl().GetLineEndPosition(line);

	bool comment = false;

	for ( ; line_start_pos < line_end_pos && !comment; 
		line_start_pos = GetCtrl().PositionAfter(line_start_pos))
		if (static_cast<char>(GetCtrl().GetCharAt(line_start_pos)) == '%')
			comment = true;

	// Do brace matching only if the line is not commented
	if (!comment) {
		if (pos > 0)
			pos = GetCtrl().PositionBefore(pos); // Look at the previous character first

		char ch = static_cast<char>(GetCtrl().GetCharAt(pos));
		const CStringA braces("{}[]()");

		if (!ch || braces.Find(ch) == -1)
			pos = GetCtrl().PositionAfter(pos);
			ch = GetCtrl().GetCharAt(pos); // Now go back

		if (braces.Find(ch) != -1) {	
			long brace_pos = GetCtrl().BraceMatch(pos);

			if (brace_pos != -1)
				GetCtrl().BraceHighlight(pos,brace_pos);
			else
				GetCtrl().BraceBadLight(pos);
		}
		else
			GetCtrl().BraceHighlight(-1,-1);
	}
}

void LaTeXView::OnModified(SCNotification* n)
{
	CodeView::OnModified(n);

	// Text has been inserted or deleted but it's not a part of a
	// multi step undo/redo action or it is the last action in this undo/redo chain
	if (n->modificationType & (SC_MOD_INSERTTEXT|SC_MOD_DELETETEXT) && 
		(!(n->modificationType & SC_MULTISTEPUNDOREDO) || n->modificationType & SC_LASTSTEPINUNDOREDO)) {
		GetDocument()->SetModifiedFlag(GetCtrl().CanUndo());

		if (GetDocument()->IsSpellerThreadAttached()) {
			int line = GetCtrl().LineFromPosition(n->position);

			// Only some lines have been changed, recheck only those
			if (n->length != GetCtrl().GetLength()) {
				int endline = line + n->linesAdded;

				do {
					GetDocument()->GetSpellerThread()->RecheckSingleLineSpelling(line,this);
				}
				while (++line < endline);
			}
			else // Document has been completely changed, recheck the whole buffer
				GetDocument()->GetSpellerThread()->RecheckSpelling(this);
		}
	}

	if (n->linesAdded) {
		// Number of lines changed, update margin's width
		UpdateLineNumberMargin();
	}
}

#pragma endregion

#pragma region Advice tips

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
				if (instant_advice_tip_ == NULL) {
					instant_advice_tip_ = new CAdvice();
					instant_advice_tip_->Create(lc->ToLaTeX(),WS_POPUP | WS_BORDER/*SS_SUNKEN*/,CRect(),CWnd::FromHandle(GetCtrl()));
				}

				if (!instant_advice_tip_->IsWindowVisible()) {
					// Compute window size
					CPoint ptClient(GetCtrl().PointXFromPosition(ptStart),GetCtrl().PointYFromPosition(ptStart));
					::ClientToScreen(GetSafeHwnd(),&ptClient);
					// Determine if there is space enough to show the window below the text
					ptClient.y += GetCtrl().TextHeight(GetCtrl().LineFromPosition(ptStart));
					// Place and show the window
					instant_advice_tip_->SetWindowPos(NULL,ptClient.x,ptClient.y,0,0,SWP_NOSIZE | SWP_NOZORDER);
					instant_advice_tip_->SetWindowText(lc->GetExpandBefore() + lc->ToLaTeX() + lc->GetExpandAfter());
					instant_advice_tip_->ShowWindow(SW_SHOWNA);
					instant_advice_tip_->SetTimer(1,5000,0);
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
	if (instant_advice_tip_)
		instant_advice_tip_->ShowWindow(SW_HIDE);
}

#pragma endregion

#pragma region Message handlers for auto complete listbox

void LaTeXView::OnACCommandSelect(const CLaTeXCommand* cmd)
{
	long ptCaret;

	autocompletion_active_ = false;
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
	OnACCommandCancelled in this case. So we use the autocompletion_active_ flag to determine, if the autocomplete
	has been finished or not */

	if (autocompletion_active_) {
		/* try to restore old cursor pos */
		if (old_pos_start_ != -1 && old_pos_end_ != -1)
			GetCtrl().SetSel(old_pos_start_,old_pos_end_);
		else {  /* suggest new position */
			long pos = GetCtrl().GetSelectionEnd();
			GetCtrl().SetSel(-1,pos);
		}

		old_pos_start_ = old_pos_end_ = -1;
		autocompletion_active_ = false;
		RestoreFocus();
	}
}

void LaTeXView::RestoreFocus()
{
	SetFocus();
}

void LaTeXView::OnACChar(UINT nKey, UINT /*nRepCount*/, UINT /*nFlags*/)
{
	if (autocompletion_list_ && autocompletion_list_->IsWindowVisible()) {
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
	if (autocompletion_list_ && autocompletion_list_->IsWindowVisible())
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

void LaTeXView::OnACHelp( const CString &cmd )
{
	InvokeContextHelp(cmd);
}

#pragma endregion

#pragma region Auto completion listbox handling

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
		if (autocompletion_list_ == NULL) {
			autocompletion_list_ = new CAutoCompleteDlg(&theApp.m_AvailableCommands,this /*theApp.GetMainWnd()*/);
			autocompletion_list_->SetListener(listener_);
		}

		//InitWithKeyword will notify the listener immediately, if only one exact match exists
		if (autocompletion_list_->InitWithKeyword(keyword) && nWords > 1) {
			//TRACE("==> CreateListBox: Show listbox \n");
			autocompletion_active_ = TRUE;

			//Move box by (16 IconPixels + 2 Pixels between Icon and text + 6/2 BorderPixels + 1 Pixel Magick) = 22
			//This way, the text matches with the editor
			ClientToScreen(&ptStart); //yes, this works only with screen coords. do not ask me why.
			autocompletion_list_->SetWindowPos(NULL,ptStart.x - 22,ptStart.y,0,0,SWP_NOSIZE | SWP_NOZORDER
				| SWP_NOACTIVATE);

			//Adjust size of the box to see all content and we do not want to be over the border
			autocompletion_list_->AdjustSizeAndPosition(GetCtrl().TextHeight(GetCtrl().LineFromPosition(pos)));

			autocompletion_list_->ShowWindow(SW_SHOW);
			autocompletion_list_->SetCurSel(0);
		}
	}

	//TRACE("<== CreateListBox\n");
	return autocompletion_list_;
}

int LaTeXView::GetNumberOfMatches( const CString& keyword )
{
	if (keyword.GetLength() < CAutoCompleteDlg::GetMinimumKeywordLength()) return 0;

	CMapStringToOb map;
	theApp.m_AvailableCommands.GetAllPossibleItems(keyword,_T(""),map);
	return map.GetCount();
}

void LaTeXView::GetWordBeforeCursor(CString& strKeyword, long& a, bool bSelect /*=true*/)
{
	long pos = a;
	int line = GetCtrl().LineFromPosition(pos);
	int length = GetLineLength(line);
	
	if (length > 0) {
		// Get the line, the cursor is placed on
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

#pragma endregion

void LaTeXView::OnSpellFile()
{
	// Save selection
	long s = GetCtrl().GetSelectionStart();
	long e = GetCtrl().GetSelectionEnd();
	
	SpellCheckDlg dlg(this,theApp.GetSpeller());
	dlg.DoModal();

	// Restore selection
	GetCtrl().SetSel(s,e);
}

void LaTeXView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CPoint ptText = point;
	ScreenToClient(&ptText);

	long pos = GetCtrl().PositionFromPoint(ptText.x,ptText.y);
	bool bShowDefault = true;

	// TODO :

	if (GetCtrl().IndicatorValueAt(0,pos) == INDIC_SQUIGGLE) {  	
		SpellerSuggestionMenu menu(this,pos);
		bShowDefault = !menu.ShowSpellMenu(theApp.GetSpeller(),point);
		// else your new attribute here
	}

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
			GetDocument()->SaveFile(NewFile,strSelectedText,strSelectedText.GetLength());
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
	if (autocompletion_active_) {
		TRACE0("==> autocomplete is active, quitting...\n");
		return;
	}

	/* store old position */
	old_pos_start_ = GetCtrl().GetSelectionStart();
	old_pos_end_ = GetCtrl().GetSelectionEnd();

	CString keyword;
	long topLeft = GetCtrl().GetCurrentPos();

	GetWordBeforeCursor(keyword,topLeft); /* retrieve word to be completed */

	if (!keyword.IsEmpty())
		autocompletion_list_ = CreateListBox(keyword,topLeft); /* setup (and show) list box */
	else
		GetCtrl().SetSel(old_pos_start_,old_pos_end_); /* restore old position */
}


/* Invokes context help for a given keyword */
BOOL LaTeXView::InvokeContextHelp( const CString& keyword )
{
	if (keyword.IsEmpty())
		::HtmlHelp(NULL,AfxGetApp()->m_pszHelpFilePath,HH_DISPLAY_TOC,0L);
	else {
		::HtmlHelp(NULL,theApp.m_pszHelpFilePath,HH_DISPLAY_TOC,0L);

		HH_AKLINK link;
		link.cbStruct = sizeof(link);
		link.fReserved = FALSE;
		link.pszKeywords = (LPCTSTR)keyword;
		link.pszUrl = NULL;
		link.pszMsgText = NULL;
		link.pszWindow = _T(">$global_TxcHelpWindow");
		link.fIndexOnFail = TRUE;
		::HtmlHelp(NULL,theApp.m_pszHelpFilePath,HH_KEYWORD_LOOKUP,(DWORD) & link);
	}

	return TRUE;
}

BOOL LaTeXView::OnInsertLaTeXConstruct( UINT nID )
{
	bool bReplaceBell = false;
	CString strInsert; // text to insert

	CInsertFloatObjectDialog *pDlg = NULL;

	long ptSelStart = GetCtrl().GetSelectionStart();
	long ptSelEnd = GetCtrl().GetSelectionEnd();

#pragma region ID specific handling

	switch (nID) {
		case ID_INSERT_FIGURE:
			pDlg = new CInsertFloatObjectDialog(CInsertFloatObjectDialog::figure,this);
			break;
		case ID_INSERT_TABLE:
			pDlg = new CInsertFloatObjectDialog(CInsertFloatObjectDialog::table,this);
			break;
		case ID_INSERT_INCLUDEGRAPHICS:
			pDlg = new CInsertGraphicDialog(this);
			break;
		case ID_INSERT_TABULAR:
			pDlg = new CInsertTabularDialog(this);
			break;
	}

	switch (nID) {
	case ID_INSERT_HEADER:
		{
			CInsertHeaderDialog dlg(this);

			if (ptSelStart != ptSelEnd) {
				CString strSel;
				
				GetCtrl().GetSelText(strSel.GetBuffer(ptSelEnd - ptSelStart));
				strSel.ReleaseBuffer();

				dlg.SetProperties(strSel);
			}

			if (dlg.DoModal() != IDOK)
				return TRUE;

			if (ptSelStart != ptSelEnd)
				GetCtrl().ReplaceSel(_T(""));

			strInsert = dlg.GetProperties();

			if (dlg.m_strTitle.IsEmpty())
				bReplaceBell = TRUE;
		}
		break;
	case ID_INSERT_FIGURE:
	case ID_INSERT_TABLE:
	case ID_INSERT_INCLUDEGRAPHICS:
	case ID_INSERT_TABULAR:
		{
			BOOL bIsSelectionEnvironment = FALSE;

			if (ptSelStart != ptSelEnd) {
				CString strSel;

				GetCtrl().GetSelText(strSel.GetBuffer(ptSelEnd - ptSelStart));
				strSel.ReleaseBuffer();

				bIsSelectionEnvironment = pDlg->SetProperties(strSel);
			}

			if (pDlg->DoModal() != IDOK) {
				delete pDlg;
				return TRUE;
			}

			if (bIsSelectionEnvironment && ptSelStart != ptSelEnd)
				GetCtrl().ReplaceSel(_T(""));

			strInsert = pDlg->GetProperties();

			if (!bIsSelectionEnvironment || pDlg->m_strContents.IsEmpty())
				bReplaceBell = TRUE;
		}
		break;
	default:
		if ((nID >= ID_TEXTMODULES_FIRST) && (nID <= ID_TEXTMODULES_LAST)) {
			ASSERT(CConfiguration::GetInstance()->m_aTextModules.GetSize() > nID - ID_TEXTMODULES_FIRST);

			if (CConfiguration::GetInstance()->m_aTextModules.GetSize() > nID - ID_TEXTMODULES_FIRST)
				strInsert = CConfiguration::GetInstance()->m_aTextModules[nID - ID_TEXTMODULES_FIRST].GetText();
			else
				return false;
		}
		else
			strInsert = AfxLoadString(nID);

		bReplaceBell = true;
	}

#pragma endregion

	if (pDlg)
		delete pDlg;

	// evaluate how to insert the text
	CString strBeforeCursor,strBehindCursor;
	strInsert = strInsert.Right(strInsert.GetLength() - strInsert.ReverseFind(_T('\n')) - 1);
	strInsert.Replace(_T("\r"),_T("\r\n"));

	int nIndentation = 0;
	int nSplitPos = strInsert.Find(_T('\001'));

	if (bReplaceBell && nSplitPos > -1) {
		int nPos = nSplitPos - 1;

		for (; nPos >= 0 && strInsert[nPos] == _T('\t'); nPos--)
			;

		nIndentation = nSplitPos - nPos - 1;
		strBeforeCursor = strInsert.Left(nPos + 1);
		strBehindCursor = strInsert.Right(strInsert.GetLength() - nSplitPos - 1);
	}
	else
		strBeforeCursor = strInsert;

	// get selection
	ptSelStart = GetCtrl().GetSelectionStart();
	ptSelEnd = GetCtrl().GetSelectionEnd();

	// test, if selection anchor is at the beginning or at the end of
	// selection.
	bool bAnchorAtEndOfSelection = ptSelEnd != GetCtrl().GetCurrentPos();
	long ptNewSelStart = ptSelStart;

	// adapt ptSelEnd intuitive
	if (GetCtrl().LineFromPosition(ptSelEnd) > GetCtrl().LineFromPosition(ptSelStart) && 
		GetCtrl().GetLineEndPosition(ptSelEnd) - ptSelEnd) {
		// don't take an 'empty' line feed with the selection
		ptSelEnd = GetCtrl().GetLineEndPosition(GetCtrl().LineFromPosition(ptSelEnd) - 1);
	}

	// insert text before selection
	if (!strBeforeCursor.IsEmpty()) {
		InsertText(GetCtrl().GetLineEndPosition(ptSelEnd) - ptSelEnd == 0 && strBeforeCursor[0] == _T('\r') 
			? strBeforeCursor.Right(strBeforeCursor.GetLength() - 2) : strBeforeCursor);
		ptNewSelStart = GetCtrl().GetSelectionStart();
	}

	// calculate new position of the selected text
	if (GetCtrl().LineFromPosition(ptSelStart) < GetCtrl().LineFromPosition(ptSelEnd)) {
		// ignore horizontal moving of the selected text,
		// if the selected text contains line feeds
		ptSelEnd += GetCtrl().LineFromPosition(ptNewSelStart) - GetCtrl().LineFromPosition(ptSelStart);
	}
	else
		ptSelEnd += ptNewSelStart - ptSelStart;

	// insert text behind selection
	if (!strBehindCursor.IsEmpty()) {
		GetCtrl().InsertText(ptSelEnd,
		(ptSelEnd == GetCtrl().GetLineEndPosition(GetCtrl().LineFromPosition(ptSelEnd)) && 
			strBehindCursor[strBehindCursor.GetLength() - 1] == _T('\n')) ? 
			strBehindCursor.Left(strBehindCursor.GetLength() - 2) : strBehindCursor);
	}

	// indent selection if wanted
	long ptStart = ptNewSelStart;
	long ptEnd = ptSelEnd;

	if (nIndentation) {
		for (int l = GetCtrl().LineFromPosition(ptStart); l <= GetCtrl().LineFromPosition(ptEnd); l++)
			for (int n = 0; n < nIndentation; n++)
				GetCtrl().InsertText(GetCtrl().PositionFromLine(l),_T("\t"));

		if (GetCtrl().LineFromPosition(ptStart) == GetCtrl().LineFromPosition(ptEnd)) {
			ptStart += nIndentation;
			ptEnd += nIndentation;
		}
		else if (GetCtrl().GetLineEndPosition(GetCtrl().LineFromPosition(ptEnd)) - ptEnd > 0)
			ptEnd += nIndentation;
	}

	// Set Selection
	GetCtrl().SetSel(ptStart,ptEnd);

	// Set Anchor and cursor for selection
	if (ptStart != ptEnd) {
		//SetAnchor(bAnchorAtEndOfSelection ? ptEnd : ptStart);
		GetCtrl().GotoPos(bAnchorAtEndOfSelection ? ptStart : ptEnd);
	}
	else // set cursor
		GetCtrl().GotoPos(ptStart);
		//SetCursorPos(ptStart);

	SetFocus();

	return TRUE;
}

void LaTeXView::OnSetFocus(CWnd* pOldWnd)
{
	CodeView::OnSetFocus(pOldWnd);

	LaTeXDocument* doc = GetDocument();

	if (doc)
		doc->CheckForFileChanges();
}

DocumentTokenizer* LaTeXView::NewDocumentTokenizer() const
{
	return new LaTeXTokenizer;
}

void LaTeXView::OnSettingsChanged()
{
	UpdateSettings();
}

BOOL LaTeXView::OnBlockComment( UINT nID )
{
	// Get the current selection
	long ptStartSel = GetCtrl().GetSelectionStart();
	long ptEndSel = GetCtrl().GetSelectionEnd();

	// Get the line numbers to act on
	const int start_line = GetCtrl().LineFromPosition(ptStartSel);
	const int end_line = GetCtrl().LineFromPosition(ptEndSel);

	// Start Undo Group
	GetCtrl().BeginUndoAction();

	//We want to issue a comment even if nothing or just one line is selected ==> (start_line == end_line)
	//And we want the last line to be commented even if it is not fully selected
	int end_line_offset = ((start_line == end_line) || (ptEndSel - GetCtrl().PositionFromLine(end_line) > 0)) ? 1 : 0;

	// Commenting the First line is different, because
	// comment could be inserted/removed/toggled in the middle of the line, too.
	// - Where to start with acting
	// NOTE: Before and including TXC 1 Beta 7.01 we inserted the comment where the cursor was placed.
	// Now we do it for the whole line, always.
	// int start_pos = ptStartSel.x; ==> old behavior
	
	int start_pos = 0;
	int count = 0;

	// Go through all lines
	for (int i = start_line; i < end_line + end_line_offset; i++) {
		// Is this line commented?
		bool has_comment = false;
		char comment_test;

		if (GetLineLength(i) > start_pos) {
			comment_test = static_cast<char>(GetCtrl().GetCharAt(GetCtrl().PositionFromLine(i) + start_pos));
			has_comment = comment_test == '%';
		}

		// What to do for the Toggle Command?
		UINT NewID = nID;

		if (nID == ID_EDIT_BLOCKCOMMENT_TOOGLE)
			NewID = has_comment ? ID_EDIT_BLOCKCOMMENT_REMOVE : ID_EDIT_BLOCKCOMMENT_INSERT;

		// Insert or Remove Comment for the current line
		switch (NewID) {
			case ID_EDIT_BLOCKCOMMENT_INSERT:
				// Insert Comment
				GetCtrl().InsertText(GetCtrl().PositionFromLine(i) + start_pos,"%");
				++count;
				break;
			case ID_EDIT_BLOCKCOMMENT_REMOVE:
				if (has_comment) {
					long pos = GetCtrl().PositionFromLine(i) + start_pos;
					GetCtrl().SetSel(pos,pos + 1);
					GetCtrl().ReplaceSel("");
					--count;
				}
				break;
		}

		// Speller should skip comments
		if (CConfiguration::GetInstance()->m_bSpellSkipComments && GetDocument()->IsSpellerThreadAttached())
			GetDocument()->GetSpellerThread()->RecheckSingleLineSpelling(i,this);

		start_pos = 0;
	} // end of "Go through all lines"

	// Restore the original selection
	const int sgn = count > 0 ? 1 : -1;
	GetCtrl().SetSel(ptStartSel + sgn,ptEndSel + count);

	// End Undo Group
	GetCtrl().EndUndoAction();

	return TRUE;

}
