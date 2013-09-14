// LaTeXViewBase.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "CodeView.h"
#include "CodeDocument.h"
#include "configuration.h"
#include "LaTeXDocumentBase.h"
#include "LaTeXViewBase.h"
#include "SpellCheckDlg.h"
#include "SpellerBackgroundThread.h"
#include "Speller.h"
#include "TeXnicCenter.h"

class SpellerSuggestionMenu
{
	CodeView* view_;
	long text_pos_;
	CMenu menu_;

public:
	SpellerSuggestionMenu(CodeView* view, long pos)
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

		buffer.resize((e - s) + 1); // Make some room
		tr.lpstrText = &buffer[0];

		view_->GetCtrl().GetTextRange(&tr,FALSE);
		bool clear = false;

		// Test the word again
		if (pSpell->SpellUTF8(&buffer[0])) {
			// Word was found, remove the attribute
			clear = true;
		}
		else {
			// Get the suggestion list
			CStringArray aSuggestList;
			int nSuggestCount = pSpell->SuggestUTF8(&buffer[0],aSuggestList);

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
				pPopup->InsertMenu(0, MF_STRING | MF_GRAYED | MF_BYPOSITION | MF_DISABLED, ID_SPELL_ERROR_LAST, noSuggestions);
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

			if (nSel != 0) {
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

					if (view_->GetDocument()->IsSpellerThreadAttached())
						view_->GetDocument()->GetSpellerThread()->
						RecheckSingleLineSpelling(view_->GetCtrl().LineFromPosition(text_pos_),view_);
				}
				else if (nSel == ID_EDIT_SPELL_IGNORE_ALL)
				{
					pSpell->IgnoreUTF8(&buffer[0]);
					clear = true;

					if (view_->GetDocument()->IsSpellerThreadAttached())
						view_->GetDocument()->GetSpellerThread()->
						RecheckSpelling(view_);
				}

				if (clear)
					view_->GetCtrl().IndicatorClearRange(s,e - s,FALSE);				
			}

			retValue = TRUE;
		}
	}

	return retValue;
}


// LaTeXViewBase

IMPLEMENT_DYNAMIC(LaTeXViewBase, CodeView)

LaTeXViewBase::LaTeXViewBase()
{

}

LaTeXViewBase::~LaTeXViewBase()
{
}

BEGIN_MESSAGE_MAP(LaTeXViewBase, CodeView)
	ON_WM_CONTEXTMENU()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// LaTeXViewBase diagnostics

#ifdef _DEBUG
void LaTeXViewBase::AssertValid() const
{
	CodeView::AssertValid();
}

#ifndef _WIN32_WCE
void LaTeXViewBase::Dump(CDumpContext& dc) const
{
	CodeView::Dump(dc);
}
#endif
#endif //_DEBUG


// LaTeXViewBase message handlers

COLORREF LaTeXViewBase::GetAutomaticColor(int nColorIndex, const int idScheme)
{
	switch (idScheme)
	{
		//Default color scheme
		case COLORSCHEME_CUSTOM:
		case COLORSCHEME_DEFAULT:
			switch (nColorIndex)
			{
				case COLORINDEX_BKGND:
					return ::GetSysColor(COLOR_WINDOW);
				case COLORINDEX_NORMALTEXT:
					return ::GetSysColor(COLOR_WINDOWTEXT);
				case COLORINDEX_CARET:
					return ::GetSysColor(COLOR_WINDOWTEXT);
				case COLORINDEX_CARETLINE:
					return RGB(240, 240, 240);
				case COLORINDEX_SELBKGND:
					return ::GetSysColor(COLOR_HIGHLIGHT);
				case COLORINDEX_SELTEXT:
					return ::GetSysColor(COLOR_HIGHLIGHTTEXT);

				case COLORINDEX_KEYWORD:
					return RGB(0x00,0x00,0xFF);
				case COLORINDEX_COMMENT:
					return RGB(0x80,0x80,0x80);
				case COLORINDEX_SPECIAL:
					return RGB(158,11,15);
				case COLORINDEX_SYMBOLS:
					return RGB(145,0,145);

				case COLORINDEX_GROUP:
					return RGB(125,167,217);
				case COLORINDEX_GROUP_NAME:
					return RGB(0,110,150);

				case COLORINDEX_STYLE_INCLUSION:
					return RGB(0x8b, 0, 0);
				case COLORINDEX_FILE_INCLUSION:
					return RGB(0, 0x80, 0x80);

				case COLORINDEX_INLINE_MATH_TEXT:
					return RGB(0x00,0x80,0x00);
				case COLORINDEX_INLINE_MATH_COMMAND:
					return RGB(0x00,0x00,0x80);

				case COLORINDEX_DIGIT:
					return RGB(75,30,200);
				case COLORINDEX_UNIT:
					return RGB(100,125,0);

				case COLORINDEX_PAIRSTRINGBKGND:
					return RGB(0xC0,0xFF,0xC0);
				case COLORINDEX_PAIRSTRINGTEXT:
					return RGB(0x00,0x00,0x00);
				case COLORINDEX_BADPAIRSTRINGBKGND:
					return RGB(0xFF,0xC0,0xC0);
				case COLORINDEX_BADPAIRSTRINGTEXT:
					return RGB(0xFF,0x00,0x00);

				//case COLORINDEX_VERBATIM_TEXT:
				//	return RGB(0xFF,0x00,0xFF);

				case COLORINDEX_FOLDMARGIN:
					return ::GetSysColor(COLOR_WINDOW);
				case COLORINDEX_FOLDMARGIN_HIGHLIGHT:
					return ::GetSysColor(COLOR_WINDOW);
				case COLORINDEX_FOLDMARK_FORE:
					return RGB(255,255,255);
				case COLORINDEX_FOLDMARK_BACK:
					return ::GetSysColor(COLOR_3DSHADOW);
				case COLORINDEX_ERRORMARK_FORE:
					return RGB(158,0,57);
				case COLORINDEX_ERRORMARK_BACK:
					return RGB(237,28,36);
				case COLORINDEX_BOOKMARK_FORE:
					return RGB(70,105,175);
				case COLORINDEX_BOOKMARK_BACK:
					return RGB(232,241,255);
				case COLORINDEX_LINENUMBERS_FORE:
					return ::GetSysColor(COLOR_WINDOWTEXT);
				case COLORINDEX_LINENUMBERS_BACK:
					return ::GetSysColor(COLOR_BTNFACE);

				case COLORINDEX_INDICATOR_SPELLING:
					return RGB(255,0,0);

				default:
					ASSERT(false); //ColorIndex not found? Shouldn't happen. Please update.
			}


		//Dark color scheme
		case COLORSCHEME_DARK:
			switch (nColorIndex)
			{
				case COLORINDEX_BKGND:
					return RGB(0, 0, 0);
				case COLORINDEX_NORMALTEXT:
					return RGB(220, 220, 220);
				case COLORINDEX_CARET:
					return RGB(255, 255, 255);
				case COLORINDEX_CARETLINE:
					return RGB(41, 36, 17);
				case COLORINDEX_SELBKGND:
					return RGB(130, 102, 5);
				case COLORINDEX_SELTEXT:
					return RGB(0, 0, 0);

				case COLORINDEX_KEYWORD:
					return RGB(255, 162, 37);
				case COLORINDEX_COMMENT:
					return RGB(110, 110, 110);
				case COLORINDEX_SPECIAL:
					return RGB(99, 170, 178);
				case COLORINDEX_SYMBOLS:
					return RGB(11, 255, 213);

				case COLORINDEX_GROUP:
					return RGB(219, 139, 31);
				case COLORINDEX_GROUP_NAME:
					return RGB(99, 170, 178);

				case COLORINDEX_STYLE_INCLUSION:
					return RGB(37, 162, 255);
				case COLORINDEX_FILE_INCLUSION:
					return RGB(255, 81, 18);

				case COLORINDEX_INLINE_MATH_TEXT:
					return RGB(11, 255, 213);
				case COLORINDEX_INLINE_MATH_COMMAND:
					return RGB(219, 139, 31);

				case COLORINDEX_DIGIT:
					return RGB(11, 255, 213);
				case COLORINDEX_UNIT:
					return RGB(99, 170, 178);

				case COLORINDEX_PAIRSTRINGBKGND:
					return RGB(32, 92, 32);
				case COLORINDEX_PAIRSTRINGTEXT:
					return RGB(255, 255, 255);
				case COLORINDEX_BADPAIRSTRINGBKGND:
					return RGB(168, 32, 32);
				case COLORINDEX_BADPAIRSTRINGTEXT:
					return RGB(255, 220, 220);

				//case COLORINDEX_VERBATIM_TEXT:
				//	return RGB(0xFF,0x00,0xFF);

				case COLORINDEX_FOLDMARGIN:
					return RGB(48, 48, 48);
				case COLORINDEX_FOLDMARGIN_HIGHLIGHT:
					return RGB(48, 48, 48);
				case COLORINDEX_FOLDMARK_FORE:
					return RGB(48, 48, 48);
				case COLORINDEX_FOLDMARK_BACK:
					return RGB(128, 128, 128);
				case COLORINDEX_ERRORMARK_FORE:
					return RGB(255, 64, 64);
				case COLORINDEX_ERRORMARK_BACK:
					return RGB(255, 64, 64);
				case COLORINDEX_BOOKMARK_FORE:
					return RGB(220, 220, 220);
				case COLORINDEX_BOOKMARK_BACK:
					return RGB(110, 110, 110);
				case COLORINDEX_LINENUMBERS_FORE:
					return RGB(110, 110, 110);
				case COLORINDEX_LINENUMBERS_BACK:
					return RGB(0, 0, 0);

				case COLORINDEX_INDICATOR_SPELLING:
					return RGB(255, 192, 192);

				default:
					ASSERT(false); //ColorIndex not found? Shouldn't happen. Please update.
			}


		//Dark color scheme Kayleigh
		case COLORSCHEME_DARK_HC:
			switch (nColorIndex)
			{
				case COLORINDEX_BKGND:
					return RGB(0, 0, 0);
				case COLORINDEX_NORMALTEXT:
					return RGB(255, 255, 255);
				case COLORINDEX_CARET:
					return RGB(255, 255, 255);
				case COLORINDEX_CARETLINE:
					return RGB(32, 32, 32);
				case COLORINDEX_SELBKGND:
					return RGB(128, 0, 0);
				case COLORINDEX_SELTEXT:
					return RGB(0, 0, 0);

				case COLORINDEX_KEYWORD:
					return RGB(128, 128, 255);
				case COLORINDEX_COMMENT:
					return RGB(128, 128, 128);
				case COLORINDEX_SPECIAL:
					return RGB(128, 255, 128);
				case COLORINDEX_SYMBOLS:
					return RGB(128, 255, 128);

				case COLORINDEX_GROUP:
					return RGB(128, 128, 255);
				case COLORINDEX_GROUP_NAME:
					return RGB(128, 128, 255);

				case COLORINDEX_STYLE_INCLUSION:
					return RGB(128, 128, 255);
				case COLORINDEX_FILE_INCLUSION:
					return RGB(128, 128, 255);

				case COLORINDEX_INLINE_MATH_TEXT:
					return RGB(128, 255, 128);
				case COLORINDEX_INLINE_MATH_COMMAND:
					return RGB(128, 128, 255);

				case COLORINDEX_DIGIT:
					return RGB(128, 255, 128);
				case COLORINDEX_UNIT:
					return RGB(128, 128, 255);

				case COLORINDEX_PAIRSTRINGBKGND:
					return RGB(0, 0, 0);
				case COLORINDEX_PAIRSTRINGTEXT:
					return RGB(0, 255, 0);
				case COLORINDEX_BADPAIRSTRINGBKGND:
					return RGB(0, 0, 0);
				case COLORINDEX_BADPAIRSTRINGTEXT:
					return RGB(255, 0, 0);

				//case COLORINDEX_VERBATIM_TEXT:
				//	return RGB(0xFF,0x00,0xFF);

				case COLORINDEX_FOLDMARGIN:
					return RGB(48, 48, 48);
				case COLORINDEX_FOLDMARGIN_HIGHLIGHT:
					return RGB(48, 48, 48);
				case COLORINDEX_FOLDMARK_FORE:
					return RGB(48, 48, 48);
				case COLORINDEX_FOLDMARK_BACK:
					return RGB(128, 128, 128);
				case COLORINDEX_ERRORMARK_FORE:
					return RGB(255, 64, 64);
				case COLORINDEX_ERRORMARK_BACK:
					return RGB(255, 64, 64);
				case COLORINDEX_BOOKMARK_FORE:
					return RGB(220, 220, 220);
				case COLORINDEX_BOOKMARK_BACK:
					return RGB(110, 110, 110);
				case COLORINDEX_LINENUMBERS_FORE:
					return RGB(110, 110, 110);
				case COLORINDEX_LINENUMBERS_BACK:
					return RGB(0, 0, 0);

				case COLORINDEX_INDICATOR_SPELLING:
					return RGB(255, 0, 0);

				default:
					ASSERT(false); //ColorIndex not found? Shouldn't happen. Please update.
			}


		case COLORSCHEME_SYSTEM:
			switch (nColorIndex)
			{
				case COLORINDEX_BKGND:
					return ::GetSysColor(COLOR_WINDOW);
				case COLORINDEX_NORMALTEXT:
					return ::GetSysColor(COLOR_WINDOWTEXT);
				case COLORINDEX_CARET:
					return ::GetSysColor(COLOR_WINDOWTEXT);
				case COLORINDEX_CARETLINE:
					return ::GetSysColor(COLOR_INFOBK);
				case COLORINDEX_SELBKGND:
					return ::GetSysColor(COLOR_HIGHLIGHT);
				case COLORINDEX_SELTEXT:
					return ::GetSysColor(COLOR_HIGHLIGHTTEXT);

				case COLORINDEX_KEYWORD:
					return ::GetSysColor(COLOR_HOTLIGHT);
				case COLORINDEX_COMMENT:
					return ::GetSysColor(COLOR_GRAYTEXT);
				case COLORINDEX_SPECIAL:
					return ::GetSysColor(COLOR_WINDOWTEXT);
				case COLORINDEX_SYMBOLS:
					return ::GetSysColor(COLOR_WINDOWTEXT);

				case COLORINDEX_GROUP:
					return ::GetSysColor(COLOR_WINDOWTEXT);
				case COLORINDEX_GROUP_NAME:
					return ::GetSysColor(COLOR_WINDOWTEXT);

				case COLORINDEX_STYLE_INCLUSION:
					return ::GetSysColor(COLOR_HOTLIGHT);
				case COLORINDEX_FILE_INCLUSION:
					return ::GetSysColor(COLOR_HOTLIGHT);

				case COLORINDEX_INLINE_MATH_TEXT:
					return ::GetSysColor(COLOR_MENUHILIGHT);
				case COLORINDEX_INLINE_MATH_COMMAND:
					return ::GetSysColor(COLOR_HOTLIGHT);

				case COLORINDEX_DIGIT:
					return ::GetSysColor(COLOR_WINDOWTEXT);
				case COLORINDEX_UNIT:
					return ::GetSysColor(COLOR_WINDOWTEXT);

				case COLORINDEX_PAIRSTRINGBKGND:
					return ::GetSysColor(COLOR_BTNFACE);
				case COLORINDEX_PAIRSTRINGTEXT:
					return ::GetSysColor(COLOR_WINDOWTEXT);
				case COLORINDEX_BADPAIRSTRINGBKGND:
					return ::GetSysColor(COLOR_3DSHADOW);
				case COLORINDEX_BADPAIRSTRINGTEXT:
					return ::GetSysColor(COLOR_3DHILIGHT);

				//case COLORINDEX_VERBATIM_TEXT:
				//	return ::GetSysColor()

				case COLORINDEX_FOLDMARGIN:
					return ::GetSysColor(COLOR_WINDOW);
				case COLORINDEX_FOLDMARGIN_HIGHLIGHT:
					return ::GetSysColor(COLOR_WINDOW);
				case COLORINDEX_FOLDMARK_FORE:
					return ::GetSysColor(COLOR_WINDOW);
				case COLORINDEX_FOLDMARK_BACK:
					return ::GetSysColor(COLOR_3DSHADOW);
				case COLORINDEX_ERRORMARK_FORE:
					return ::GetSysColor(COLOR_HOTLIGHT);
				case COLORINDEX_ERRORMARK_BACK:
					return ::GetSysColor(COLOR_3DSHADOW);
				case COLORINDEX_BOOKMARK_FORE:
					return ::GetSysColor(COLOR_3DHIGHLIGHT);
				case COLORINDEX_BOOKMARK_BACK:
					return ::GetSysColor(COLOR_3DSHADOW);
				case COLORINDEX_LINENUMBERS_FORE:
					return ::GetSysColor(COLOR_WINDOWTEXT);
				case COLORINDEX_LINENUMBERS_BACK:
					return ::GetSysColor(COLOR_BTNFACE);

				case COLORINDEX_INDICATOR_SPELLING:
					return ::GetSysColor(COLOR_HIGHLIGHT);

				default:
					ASSERT(false); //ColorIndex not found? Shouldn't happen. Please update.
			}

		case COLORSCHEME_SYSTEM_INVERSE:
			return RGB(255, 255, 255) - GetAutomaticColor(nColorIndex, COLORSCHEME_SYSTEM);

			default:
				ASSERT(false); //idScheme not found? Shouldn't happen. Please update.
	}

	return RGB(0xFF,0x00,0x00);
}

void LaTeXViewBase::SetAStyle(int style, COLORREF fore, COLORREF back, int size, LPCTSTR face, BOOL bold, BOOL italic) 
{
	CScintillaCtrl& rCtrl = GetCtrl();

	rCtrl.StyleSetFore(style, fore);
	rCtrl.StyleSetBack(style, back);

	if (size >= 1)
		rCtrl.StyleSetSize(style, size);

	if (face) 
		rCtrl.StyleSetFont(style, face);

	rCtrl.StyleSetBold(style, bold);
	rCtrl.StyleSetItalic(style, italic);
}


COLORREF LaTeXViewBase::GetColor(int nColorIndex)
{
	if (CConfiguration::GetInstance()->m_nEditorColorScheme == COLORSCHEME_CUSTOM &&
		nColorIndex >= 0 && nColorIndex < COLORINDEX_COUNT &&
		CConfiguration::GetInstance()->m_aEditorColors[nColorIndex] != 0xFFFFFFFF)
		return CConfiguration::GetInstance()->m_aEditorColors[nColorIndex];

	// user set automatic color
	return GetAutomaticColor(nColorIndex, CConfiguration::GetInstance()->m_nEditorColorScheme);
}

void LaTeXViewBase::OnSettingsChanged()
{
	// User changed application's settings: react here
	CScintillaCtrl& rCtrl = GetCtrl();

	const LOGFONT& editor_font = CConfiguration::GetInstance()->m_fontEditor;
	const int point_size = GetLogFontPointSize(editor_font);

	//Foreground, Background, Selection.
	SetAStyle(STYLE_DEFAULT, GetColor(COLORINDEX_NORMALTEXT), GetColor(COLORINDEX_BKGND), point_size, editor_font.lfFaceName, false, false);
	rCtrl.StyleSetItalic(STYLE_DEFAULT, editor_font.lfItalic);
	rCtrl.StyleSetBold(STYLE_DEFAULT, editor_font.lfWeight >= FW_BOLD);
	rCtrl.SetSelFore(TRUE, GetColor(COLORINDEX_SELTEXT));
	rCtrl.SetSelBack(TRUE, GetColor(COLORINDEX_SELBKGND));

	//Brace highlighting
	SetAStyle(STYLE_BRACELIGHT, GetColor(COLORINDEX_PAIRSTRINGTEXT), GetColor(COLORINDEX_PAIRSTRINGBKGND),
				point_size, editor_font.lfFaceName, true, false);
	SetAStyle(STYLE_BRACEBAD, GetColor(COLORINDEX_BADPAIRSTRINGTEXT), GetColor(COLORINDEX_BADPAIRSTRINGBKGND),
				point_size, editor_font.lfFaceName, true, false);

	rCtrl.SetFoldMarginColour(TRUE, GetColor(COLORINDEX_FOLDMARGIN));
	rCtrl.SetFoldMarginHiColour(TRUE, GetColor(COLORINDEX_FOLDMARGIN_HIGHLIGHT));
	// Setup markers for VS style folding
	COLORREF fore = GetColor(COLORINDEX_FOLDMARK_FORE);
	COLORREF back = GetColor(COLORINDEX_FOLDMARK_BACK);
	DefineMarker(SC_MARKNUM_FOLDEROPEN, SC_MARK_BOXMINUS, fore, back);
	DefineMarker(SC_MARKNUM_FOLDER, SC_MARK_BOXPLUS, fore, back);
	DefineMarker(SC_MARKNUM_FOLDERSUB, SC_MARK_VLINE, fore, back);
	DefineMarker(SC_MARKNUM_FOLDERTAIL, SC_MARK_LCORNER, fore, back);
	DefineMarker(SC_MARKNUM_FOLDEREND, SC_MARK_BOXPLUSCONNECTED, fore, back);
	DefineMarker(SC_MARKNUM_FOLDEROPENMID, SC_MARK_BOXMINUSCONNECTED, fore, back);
	DefineMarker(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_TCORNER, fore, back);

	rCtrl.MarkerSetFore(CodeDocument::Errormark, GetColor(COLORINDEX_ERRORMARK_FORE));
	rCtrl.MarkerSetBack(CodeDocument::Errormark, GetColor(COLORINDEX_ERRORMARK_BACK));

	rCtrl.MarkerSetFore(CodeDocument::Bookmark, GetColor(COLORINDEX_BOOKMARK_FORE));
	rCtrl.MarkerSetBack(CodeDocument::Bookmark, GetColor(COLORINDEX_BOOKMARK_BACK));

	SetAStyle(STYLE_LINENUMBER, GetColor(COLORINDEX_LINENUMBERS_FORE), GetColor(COLORINDEX_LINENUMBERS_BACK));

	rCtrl.IndicSetFore(0, GetColor(COLORINDEX_INDICATOR_SPELLING));

	rCtrl.SetCaretFore(GetColor(COLORINDEX_CARET));
	rCtrl.SetCaretLineBack(GetColor(COLORINDEX_CARETLINE));
	//Not so nice: rCtrl.SetCaretLineBackAlpha(90);

	CodeView::OnSettingsChanged();
}

void LaTeXViewBase::OnUpdateUI(SCNotification* n)
{
	CodeView::OnUpdateUI(n);

	long pos = GetCtrl().GetCurrentPos();

	int line = GetCtrl().LineFromPosition(pos);
	long line_start_pos = GetCtrl().PositionFromLine(line);
	//long line_end_pos = GetCtrl().GetLineEndPosition(line);

	bool comment = false;

	for ( ; line_start_pos < pos && !comment; 
		line_start_pos = GetCtrl().PositionAfter(line_start_pos))
		if (static_cast<char>(GetCtrl().GetCharAt(line_start_pos)) == '%')
			comment = true;

	bool remove_highlight = false;

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
			remove_highlight = true;
	}

	if (comment || remove_highlight)
		GetCtrl().BraceHighlight(-1,-1); // (0,0) leads to display corruption, why?
}

void LaTeXViewBase::OnSpellFile()
{
	// Save selection
	long s = GetCtrl().GetSelectionStart();
	long e = GetCtrl().GetSelectionEnd();

	SpellCheckDlg dlg(this,theApp.GetSpeller());
	dlg.DoModal();

	// Restore selection
	GetCtrl().SetSel(s,e);
}

void LaTeXViewBase::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CPoint ptText = point;

	CRect rect;
	GetWindowRect(&rect);

	long pos;

	if (rect.PtInRect(point)) {
		ScreenToClient(&ptText);
		pos = GetCtrl().PositionFromPoint(ptText.x,ptText.y);
	}
	else {
		// The cursor is outside this window because the
		// user pressed the app key. Use caret's coordinates
		pos = GetCtrl().GetCurrentPos();

		// We use the baseline
		point.SetPoint(GetCtrl().PointXFromPosition(pos),
			GetCtrl().PointYFromPosition(pos) + GetCtrl().TextHeight(GetCtrl().LineFromPosition(pos)));

		ptText = point;
		ClientToScreen(&point); // Scintilla returns client relative positions
	}

	bool bShowDefault = true;

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

LaTeXDocumentBase* LaTeXViewBase::GetDocument() const
{
	return dynamic_cast<LaTeXDocumentBase*>(CodeView::GetDocument());
}


//int LaTeXViewBase::OnCreate(LPCREATESTRUCT lpCreateStruct)
//{
//	if (CodeView::OnCreate(lpCreateStruct) == -1)
//		return -1;
//
//	return 0;
//}
