#include "stdafx.h"
#include "TeXnicCenter.h"

#include <iostream>
#include <iomanip>
#include <sstream>
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
#include "EncodingConverter.h"


#pragma region Helper functions and classes

const CStringA FormatColorSpec(COLORREF clr, bool use_model_curly_brace = false)
{
	const float max = 255.0f;

	const float r = GetRValue(clr) / max;
	const float g = GetGValue(clr) / max;
	const float b = GetBValue(clr) / max;

	std::ostringstream oss;
	oss.imbue(std::locale::classic()); // US locale for the dot as float separator
	oss << std::setprecision(2) << r << ',' << g << ',' << b;

	CStringA spec;
	const char* fmt = use_model_curly_brace ? "{rgb}{%s}" : "[rgb]{%s}";

	spec.Format(fmt,oss.str().c_str());

	return spec;
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

IMPLEMENT_DYNCREATE(LaTeXView, LaTeXViewBase)

LaTeXView::LaTeXView()
: autocompletion_active_(false)
, instant_advice_tip_(0)
, autocompletion_list_(0)
, listener_(new LaTeXViewListener(this))
{
}

LaTeXView::~LaTeXView()
{
}

BEGIN_MESSAGE_MAP(LaTeXView, LaTeXViewBase)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_SPELL_FILE, &LaTeXView::OnSpellFile)
	ON_COMMAND(ID_EDIT_DELETE_LINE, &LaTeXView::OnEditDeleteLine)
	ON_COMMAND(ID_EDIT_OUTSOURCE, &LaTeXView::OnEditOutsource)
	ON_COMMAND(ID_QUERY_COMPLETION, &LaTeXView::OnQueryCompletion)
	ON_COMMAND_EX_RANGE(ID_INSERT_A, ID_INSERT_ZETA, &LaTeXView::OnInsertLaTeXConstruct)
	ON_COMMAND_EX_RANGE(ID_FORMAT_BFSERIES, ID_FORMAT_UPSHAPE, &LaTeXView::OnInsertLaTeXConstruct)
	ON_COMMAND_EX_RANGE(ID_TEXTMODULES_FIRST,ID_TEXTMODULES_LAST, &LaTeXView::OnInsertLaTeXConstruct)	
	ON_COMMAND_EX(ID_EDIT_BLOCKCOMMENT_INSERT, &LaTeXView::OnBlockComment)
	ON_COMMAND_EX(ID_EDIT_BLOCKCOMMENT_REMOVE, &LaTeXView::OnBlockComment)
	ON_COMMAND_EX(ID_EDIT_BLOCKCOMMENT_TOOGLE, &LaTeXView::OnBlockComment)
	ON_COMMAND(ID_FORMAT_TEXT_FORE_COLOR, &LaTeXView::OnFormatTextForeColor)
	ON_COMMAND(ID_FORMAT_TEXT_BACK_COLOR, &LaTeXView::OnFormatTextBackColor)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_TEXT_FORE_COLOR, &LaTeXView::OnUpdateFormatTextForeColor)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_TEXT_BACK_COLOR, &LaTeXView::OnUpdateFormatTextBackColor)
END_MESSAGE_MAP()


// LaTeXView diagnostics

#ifdef _DEBUG
void LaTeXView::AssertValid() const
{
	LaTeXViewBase::AssertValid();
}

#ifndef _WIN32_WCE
void LaTeXView::Dump(CDumpContext& dc) const
{
	LaTeXViewBase::Dump(dc);
}
#endif
#endif //_DEBUG


// LaTeXView message handlers

int LaTeXView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (LaTeXViewBase::OnCreate(lpCreateStruct) == -1)
		return -1;

	//EnableHardWrap();

	old_pos_start_ = old_pos_end_ = -1;
	autocompletion_active_ = false;

	CScintillaCtrl& rCtrl = GetCtrl();
	rCtrl.SetLexer(SCLEX_TEX); // TeX Lexer

	EnableAutoIndent();
	rCtrl.SetCaretSticky(FALSE);

	return 0;
}

void LaTeXView::OnInitialUpdate()
{
    LaTeXViewBase::OnInitialUpdate();	
}

void LaTeXView::OnCharAdded(SCNotification* n)
{
	LaTeXViewBase::OnCharAdded(n);

	long pos = GetCtrl().GetCurrentPos();

	if (pos > 0) {
		int ch = n->ch;

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


void LaTeXView::UpdateSettings()
{
	CScintillaCtrl& rCtrl = GetCtrl();

	const LOGFONT& editor_font = CConfiguration::GetInstance()->m_fontEditor;
	const int point_size = GetLogFontPointSize(editor_font);

	SetAStyle(STYLE_DEFAULT, GetColor(COLORINDEX_NORMALTEXT),GetColor(COLORINDEX_BKGND),
		point_size,CConfiguration::GetInstance()->m_fontEditor.lfFaceName);
	
	SetAStyle(SCE_TEX_COMMENT, GetColor(COLORINDEX_COMMENT),GetColor(COLORINDEX_BKGND)); // Includes comments' color
	SetAStyle(SCE_TEX_GROUP,RGB(125,167,217),GetColor(COLORINDEX_BKGND));

	SetAStyle(SCE_TEX_INLINE_MATH,GetColor(COLORINDEX_INLINE_MATH_TEXT),GetColor(COLORINDEX_BKGND));
	SetAStyle(SCE_TEX_INLINE_MATH_COMMAND,GetColor(COLORINDEX_INLINE_MATH_COMMAND),GetColor(COLORINDEX_BKGND));
	SetAStyle(SCE_TEX_GROUP_NAME,GetColor(COLORINDEX_GROUP_NAME),GetColor(COLORINDEX_BKGND));
	
	SetAStyle(SCE_TEX_TEXT,GetColor(COLORINDEX_NORMALTEXT),GetColor(COLORINDEX_BKGND));
	SetAStyle(SCE_TEX_SPECIAL, RGB(158,11,15),GetColor(COLORINDEX_BKGND));
	SetAStyle(SCE_TEX_SYMBOL, RGB(145,0,145),GetColor(COLORINDEX_BKGND));
	SetAStyle(SCE_TEX_DIGIT, GetColor(COLORINDEX_DIGIT),GetColor(COLORINDEX_BKGND));

	SetAStyle(SCE_TEX_DOCUMENTCLASS, RGB(0x8b, 0, 0), GetColor(COLORINDEX_BKGND));
	SetAStyle(SCE_TEX_USE_PACKAGE, RGB(0, 0x80, 0x80), GetColor(COLORINDEX_BKGND));

#pragma region Comments 

	// Comments displayed in italics
	rCtrl.StyleSetFont(SCE_TEX_COMMENT,editor_font.lfFaceName);
	rCtrl.StyleSetSize(SCE_TEX_COMMENT,point_size);
	rCtrl.StyleSetItalic(SCE_TEX_COMMENT,TRUE);

#pragma endregion

	rCtrl.StyleSetFont(SCE_TEX_UNIT,editor_font.lfFaceName);
	rCtrl.StyleSetSize(SCE_TEX_UNIT,point_size);
	rCtrl.StyleSetBold(SCE_TEX_UNIT,TRUE);
	rCtrl.StyleSetFore(SCE_TEX_UNIT,GetColor(COLORINDEX_UNIT));

	rCtrl.UsePopUp(FALSE);

	SetAStyle(SCE_TEX_COMMAND, GetColor(COLORINDEX_KEYWORD),GetColor(COLORINDEX_BKGND));
	rCtrl.SetSelFore(TRUE,GetColor(COLORINDEX_SELTEXT));
	rCtrl.SetSelBack(TRUE,GetColor(COLORINDEX_SELBKGND));

	rCtrl.StyleSetItalic(STYLE_DEFAULT,editor_font.lfItalic);
	rCtrl.StyleSetItalic(STYLE_DEFAULT,editor_font.lfWeight >= FW_BOLD);

#pragma region Brace highlighting

	rCtrl.StyleSetFont(STYLE_BRACELIGHT,editor_font.lfFaceName);
	rCtrl.StyleSetSize(STYLE_BRACELIGHT,point_size);
	rCtrl.StyleSetBold(STYLE_BRACELIGHT,TRUE);
	rCtrl.StyleSetFore(STYLE_BRACELIGHT,RGB(0,0,0));
	rCtrl.StyleSetBack(STYLE_BRACELIGHT,GetColor(COLORINDEX_PAIRSTRINGBKGND));

	rCtrl.StyleSetFont(STYLE_BRACEBAD,editor_font.lfFaceName);
	rCtrl.StyleSetSize(STYLE_BRACEBAD,point_size);
	rCtrl.StyleSetBold(STYLE_BRACEBAD,TRUE);
	rCtrl.StyleSetFore(STYLE_BRACEBAD,RGB(255,0,0));
	rCtrl.StyleSetBack(STYLE_BRACEBAD,GetColor(COLORINDEX_BADPAIRSTRINGBKGND));

#pragma endregion

	rCtrl.StyleSetFont(SCE_TEX_GROUP_NAME,editor_font.lfFaceName);
	rCtrl.StyleSetSize(SCE_TEX_GROUP_NAME,point_size);
	rCtrl.StyleSetBold(SCE_TEX_GROUP_NAME,TRUE);

#pragma region Caret

	rCtrl.SetCaretStyle(CConfiguration::GetInstance()->IsInsertCaretLine() ? CARETSTYLE_LINE : CARETSTYLE_BLOCK);
	rCtrl.SetCaretPeriod(CConfiguration::GetInstance()->IsBlinkInsertCaret() ? 500 : 0);

#pragma endregion

	rCtrl.StyleSetFont(SCE_TEX_DOCUMENTCLASS,editor_font.lfFaceName);
	rCtrl.StyleSetSize(SCE_TEX_DOCUMENTCLASS,point_size);
	rCtrl.StyleSetBold(SCE_TEX_DOCUMENTCLASS,TRUE);

	rCtrl.StyleSetFont(SCE_TEX_USE_PACKAGE,editor_font.lfFaceName);
	rCtrl.StyleSetSize(SCE_TEX_USE_PACKAGE,point_size);
	rCtrl.StyleSetItalic(SCE_TEX_USE_PACKAGE,TRUE);
}

#pragma region Scintilla notifications

void LaTeXView::OnModified(SCNotification* n)
{
	LaTeXViewBase::OnModified(n);

	// Text has been inserted or deleted but it's not a part of a
	// multi step undo/redo action or it is the last action in this undo/redo chain
	if (n->modificationType & (SC_MOD_INSERTTEXT|SC_MOD_DELETETEXT) && 
		(!(n->modificationType & SC_MULTISTEPUNDOREDO) || n->modificationType & SC_LASTSTEPINUNDOREDO)) {
		GetDocument()->SetModifiedFlag();

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
		SharedObjectMap map;

		theApp.m_AvailableCommands.GetAllPossibleItems(keyw,_T(""),map);

		if (map.GetCount() == 1) { //keyword is unique -> OK
			std::tr1::shared_ptr<CObject> c;
			POSITION pos = map.GetStartPosition();
			map.GetNextAssoc(pos,key,c);

			SharedLaTeXCommandPtr lc = std::tr1::dynamic_pointer_cast<CLaTeXCommand>(c);

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
					instant_advice_tip_->SetWindowPos(NULL,ptClient.x,ptClient.y,0,0,SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
					instant_advice_tip_->SetWindowText(lc->GetExpandBefore() + lc->ToLaTeX() + lc->GetExpandAfter());
					instant_advice_tip_->ShowWindow(SW_SHOW);
					SetFocus();

					const UINT advice_tip_timeout = 5000;
					instant_advice_tip_->SetTimer(1,advice_tip_timeout,0);
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
	GetCtrl().BeginUndoAction();

	int initial_line_count = GetCtrl().GetLineCount();

	long ptCaret;

	autocompletion_active_ = false;
	old_pos_start_ = old_pos_end_ = -1;

	ASSERT(cmd != NULL);

	//Generate string to be inserted
	CString InsertString = cmd->GetExpandBefore(); // collect completion...
	InsertString += cmd->ToLaTeX();
	InsertString += cmd->GetExpandAfter();

	//Get the place where we started with the completion
	const long origs = GetCtrl().GetSelectionStart();
	const int start_line = GetCtrl().LineFromPosition(origs);

	//Do the actual insertion
	GetCtrl().ReplaceSel(InsertString);
	const long s = GetCtrl().GetSelectionStart();
	const long e = GetCtrl().GetSelectionEnd();
	GetCtrl().SetSel(-1,s); // drop selection	

	bool isEnv = cmd->GetExpandAfter().GetLength() && cmd->GetExpandBefore().GetLength();

	if (!isEnv) {
		bool bSetPosition = false;

		//If a command was inserted and it contains a brace, we might want to go there
		if (InsertString.FindOneOf(_T("{[(")) != -1) {

			//Find an opening brace
			TextToFind ttf;
			ttf.chrg.cpMin = origs;
			ttf.chrg.cpMax = e;
			ttf.lpstrText = "[{\\[\\(]";
			const long BracePos = GetCtrl().FindTextW(SCFIND_REGEXP + SCFIND_POSIX, &ttf);

			//Place caret after the first opening brace
			if (BracePos != -1 && BracePos + 1 < GetCtrl().GetLength()) {
				GetCtrl().GotoPos(BracePos + 1);
				bSetPosition = true;
			}
		}

		// no brace found; drop selection and place cursor after inserted word
		if (!bSetPosition) 
			GetCtrl().GotoPos(e);
	}

	Reindent(initial_line_count, start_line);

	if (isEnv) {
		// env was inserted -> place cursor at end of next row
		// but only after the newly inserted environment has been correctly indented
		long l = GetCtrl().LineFromPosition(origs);
		ptCaret = GetCtrl().GetLineEndPosition(l + 1);

		GetCtrl().GotoPos(ptCaret);
	}

	GetCtrl().EndUndoAction();

	SetFocus(); // set focus back to edit view
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
			autocompletion_list_->SetListener(listener_.get());
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

	SharedObjectMap map;
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
		long start = GetCtrl().PositionFromLine(line);

		// Get the position from where we start the backward search
		long l = pos - start;

		if (l > 0) {
			std::vector<char> data(l + 1);

			Sci_TextRange range;
			range.chrg.cpMin = start;
			range.chrg.cpMax = pos;
			range.lpstrText = &data[0];

			GetCtrl().GetTextRange(&range);

			if (true) {
				long EndX = l - 1;
				long CurrentX = EndX;

				//Backward search: go to first character of the current word
				for (; CurrentX >= 0; CurrentX--) {
					if (!IsAutoCompletionCharacter(data[CurrentX])) {
						++CurrentX; //This is the last valid TCHAR
						break;
					}
				}

				if (CurrentX < 0) 
					CurrentX = 0;

				if (CurrentX <= EndX) {
					ASSERT(CurrentX >= 0);
					ASSERT(EndX - CurrentX >= 0);

					std::vector<wchar_t> conv;
					UTF8toUTF16(range.lpstrText + CurrentX, EndX - CurrentX, conv);

					if (!conv.empty())
						strKeyword = CString(&conv[0], conv.size());
					
					if (bSelect)
						GetCtrl().SetSel(start + CurrentX,pos);

					a = start + CurrentX;
				}
				else
					strKeyword.Empty();
			}
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

void LaTeXView::OnEditOutsource()
{
	//Get the file name of ourselves
	LaTeXDocument* pDoc = GetDocument();

	if (!pDoc) return;

	//Create the dialog
	CTextOutsourceDlg OutsourceDlg;
	CPathTool OldPath = pDoc->GetPathName();

	//Suggest a directory for the new file
	OutsourceDlg.m_Directory = OldPath.GetDirectory();

	//Show the dialog
	if (OutsourceDlg.DoModal() == IDOK) {
		//Get the new file name and write the selected lines to it
		CAtlFile NewFile;

		if (SUCCEEDED(NewFile.Create(OutsourceDlg.NewPath.GetPath(),GENERIC_WRITE,FILE_SHARE_READ,CREATE_ALWAYS))) {
			//Get selected text - may be empty
			long s = GetCtrl().GetSelectionStart();
			long e = GetCtrl().GetSelectionEnd();

			std::vector<char> buffer;
		
			buffer.resize((e - s) + 1);
			GetCtrl().GetSelText(&buffer[0]);

			char* text = &buffer[0]; 
			std::size_t n = buffer.size() - 1;

			// Write it to the file
			TextDocument td(GetDocument());
			td.WriteUTF8(NewFile,text,n);

			NewFile.Close();

			//Get relative path - omit tex-extension
			CString RelativeFilePath = CPathTool::GetRelativePath(OldPath.GetDirectory(),
				(OutsourceDlg.NewPath.GetFileExtension() == _T("tex"))
				? CPathTool(OutsourceDlg.NewPath.GetBase())
				: OutsourceDlg.NewPath);

			// GetRelativePath uses the backslash as a separator
			// which denotes a TeX command start; replace it by /
			RelativeFilePath.Replace(_T('\\'),_T('/'));

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
			ASSERT(CConfiguration::GetInstance()->m_aTextModules.GetSize() > 
				static_cast<INT_PTR>(nID - ID_TEXTMODULES_FIRST));

			if (CConfiguration::GetInstance()->m_aTextModules.GetSize() > static_cast<INT_PTR>(nID - ID_TEXTMODULES_FIRST))
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
	CString strBeforeCursor, strBehindCursor;
	strInsert = strInsert.Right(strInsert.GetLength() - strInsert.ReverseFind(_T('\n')) - 1);
	strInsert.Replace(_T("\r"),_T("\r\n"));

	int nIndentation = 0;
	int nSplitPos = strInsert.Find(_T('\001'));

	if (bReplaceBell && nSplitPos > -1) {
		int nPos = nSplitPos - 1;

		while (nPos >= 0 && strInsert[nPos] == _T('\t'))
			--nPos;

		nIndentation = nSplitPos - nPos - 1;
		strBeforeCursor = strInsert.Left(nPos + 1);
		strBehindCursor = strInsert.Right(strInsert.GetLength() - nSplitPos - 1);
	}
	else
		strBeforeCursor = strInsert;

	GetCtrl().BeginUndoAction();

	// get selection
	ptSelStart = GetCtrl().GetSelectionStart();
	ptSelEnd = GetCtrl().GetSelectionEnd();

	int start_line = GetCtrl().LineFromPosition(ptSelStart);
	const int initial_line_count = GetCtrl().GetLineCount();

#pragma region Insertion
	// test, if selection anchor is at the beginning or at the end of
	// selection.
	bool bAnchorAtEndOfSelection = ptSelEnd != GetCtrl().GetCurrentPos();
	long ptNewSelStart = ptSelStart;

	// insert text before selection
	if (!strBeforeCursor.IsEmpty()) {		
		std::vector<char> buffer;
		UTF16toUTF8(strBeforeCursor,strBeforeCursor.GetLength(),buffer);
		buffer.push_back(0);

		GetCtrl().InsertText(ptSelStart,&buffer[0]);

		buffer.pop_back(); // Remove the null above

		long offset = static_cast<long>(buffer.size());
		ptNewSelStart += offset;
		ptSelEnd += offset;
	}

	// insert text behind selection
	if (!strBehindCursor.IsEmpty()) {
		std::vector<char> buffer;
		UTF16toUTF8(strBehindCursor,strBehindCursor.GetLength(),buffer);
		buffer.push_back(0);

		GetCtrl().InsertText(ptSelEnd,&buffer[0]);
	}

	// indent selection if wanted
	long ptStart = ptNewSelStart;
	long ptEnd = ptSelEnd;

	if (nIndentation) {
		for (int l = GetCtrl().LineFromPosition(ptStart); l <= GetCtrl().LineFromPosition(ptEnd); l++) {
			GetCtrl().GotoPos(GetCtrl().PositionFromLine(l));

			for (int n = 0; n < nIndentation; n++)
				GetCtrl().Tab();
		}

		int indent;

		if (GetCtrl().GetUseTabs())
			indent = nIndentation;
		else {
			indent = GetCtrl().GetIndent();

			if (!indent)
				indent = nIndentation; // Same as the tab width, see Scintilla doc
		}

		if (GetCtrl().LineFromPosition(ptStart) == GetCtrl().LineFromPosition(ptEnd)) {
			ptStart += indent;
			ptEnd += indent;
		}
		else if (GetCtrl().GetLineEndPosition(GetCtrl().LineFromPosition(ptEnd)) - ptEnd > 0)
			ptEnd += indent;
	}

	// Set Anchor and cursor for selection
	if (ptStart != ptEnd) {
		long pos = bAnchorAtEndOfSelection ? ptEnd : ptStart;
		GetCtrl().SetAnchor(pos);
		GetCtrl().SetSel(ptStart,ptEnd);		
	}
	else // set cursor
		GetCtrl().GotoPos(ptStart);

#pragma endregion

	Reindent(initial_line_count, start_line);

	GetCtrl().EndUndoAction();

	SetFocus();

	return TRUE;
}


DocumentTokenizer* LaTeXView::NewDocumentTokenizer() const
{
	return new LaTeXTokenizer;
}

void LaTeXView::OnSettingsChanged()
{
	UpdateSettings();
	LaTeXViewBase::OnSettingsChanged();
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
			int end_line_pos = GetCtrl().GetLineEndPosition(i);
			long line_start = GetCtrl().PositionFromLine(i);
			long pos = line_start;
			bool stop = false;

			while (!stop && pos < end_line_pos) {
				if (!std::isspace(GetCtrl().GetCharAt(pos),std::locale::classic()))
					stop = true;
				else
					++pos;
			}

			start_pos = pos - line_start;
			comment_test = static_cast<char>(GetCtrl().GetCharAt(pos));
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

	if (count != 0) {
		// Restore the original selection
		const int sgn = count > 0 ? 1 : -1;
		GetCtrl().SetSel(ptStartSel + sgn,ptEndSel + count);
	}

	// End Undo Group
	GetCtrl().EndUndoAction();

	return TRUE;

}

UINT GetControlKeyState()
{
	UINT keystate = 0;

	if (::GetKeyState(VK_CONTROL) >> 15 & 1)
		keystate |= MK_CONTROL;

	if (::GetKeyState(VK_SHIFT) >> 15 & 1)
		keystate |= MK_SHIFT;

	return keystate;
}

void LaTeXView::OnFormatTextForeColor()
{
	CScintillaCtrl& c = GetCtrl();

	long s = c.GetSelectionStart();
	long e = c.GetSelectionEnd();	

	COLORREF clr = CMFCColorMenuButton::GetColorByCmdID(ID_FORMAT_TEXT_FORE_COLOR);

	if (clr == 0xffffffff)
		clr = 0; // Black

	c.BeginUndoAction();

	switch (GetControlKeyState()) {
		default:
			{
				const CStringA spec = FormatColorSpec(clr);

				if (s == e) {
					// No text selected, use \color{}
					CStringA text;
					text.Format("\\color%s",spec);

					c.InsertText(s,text);
					e = s + text.GetLength();
					c.GotoPos(e);
				}
				else {
					// A selection exists
					CStringA text;

					// Get the selected text
					const int length = e - s;
					c.GetSelText(text.GetBuffer(length + 1));
					text.ReleaseBuffer(length);

					// Generate the \textcolor string
					CStringA fmt1;
					fmt1.Format("\\textcolor%s{",spec);

					CStringA fmt2;
					fmt2.Format("%s}",text);

					// Replace the selection
					c.ReplaceSel(fmt1 + fmt2);
					// and restore the original selection
					//e = s + fmt.GetLength();
					c.SetSel(s + fmt1.GetLength(),s + fmt1.GetLength() + text.GetLength());
				}
			}
			break;
		case MK_CONTROL: // Ctrl+click, insert the color define
			{
				if (s != e) // Text is selected
					c.ReplaceSel(""); // Remove the text

				const CStringA pre("\\definecolor{");
				const CStringA placeholder(MAKEINTRESOURCE(IDS_DEFINE_COLOR_NAME_PLACEHOLDER));

				c.InsertText(s,pre + placeholder + '}' + FormatColorSpec(clr,true));
				s += pre.GetLength();
				e = s + placeholder.GetLength();
				
				c.SetSel(s,e);
			}
			break;
	}

	c.EndUndoAction();
}

void LaTeXView::OnFormatTextBackColor()
{
	CScintillaCtrl& c = GetCtrl();

	long s = c.GetSelectionStart();
	long e = c.GetSelectionEnd();

	COLORREF clr = CMFCColorMenuButton::GetColorByCmdID(ID_FORMAT_TEXT_BACK_COLOR);

	if (clr == 0xffffffff)
		clr = 0; // Black

	const CStringA spec = FormatColorSpec(clr);

	c.BeginUndoAction();

	CStringA text;

	// Get the selected text
	const int length = e - s;
	c.GetSelText(text.GetBuffer(length + 1));
	text.ReleaseBuffer(length);

	// Generate the \textcolor string
	CStringA fmt1;
	fmt1.Format("\\colorbox%s{",spec);

	CStringA fmt2;
	fmt2.Format("%s}",text);

	// Replace the selection
	c.ReplaceSel(fmt1 + fmt2);	
	// and restore the original selection
	c.SetSel(s + fmt1.GetLength(),s + fmt1.GetLength() + text.GetLength());

	c.EndUndoAction();
}

void LaTeXView::OnUpdateFormatTextForeColor(CCmdUI* /*pCmdUI*/)
{
	// For now, always enabled
}

void LaTeXView::OnUpdateFormatTextBackColor(CCmdUI* pCmdUI)
{
	// Enable only, if the selection is not empty
	pCmdUI->Enable(GetCtrl().GetSelectionStart() != GetCtrl().GetSelectionEnd());
}

void LaTeXView::Reindent( int initial_line_count, int start_line )
{
	int current_line_count = GetCtrl().GetLineCount();
	int lines_added = current_line_count - initial_line_count;

	// We're not interested in the first line, since it will be already correctly indented
	if (--lines_added > 0) {
		int indent = GetCtrl().GetLineIndentation(start_line);
		++start_line;

		int end_line = start_line + lines_added;

		for ( ; start_line <= end_line; ++start_line) {
			if (GetLineLength(start_line) != 0) {
				// Set the indentation to the size of the first line plus the indent of the current line
				GetCtrl().SetLineIndentation(start_line,indent + GetCtrl().GetLineIndentation(start_line));
			}
			else {
				// Setting indentation on empty lines doesn't have any effect
				// thus we need to insert tabs
				GetCtrl().GotoPos(GetCtrl().PositionFromLine(start_line));
				
				int count = indent;
				int width = GetCtrl().GetIndent();

				if (!width) // Use tab width
					width = GetCtrl().GetTabWidth();

				count /= width;

				for (int i = 0; i < count; ++i)
					GetCtrl().Tab();
			}
		}
	}
}