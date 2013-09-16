#include "stdafx.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <algorithm>
#include <cstddef>

#include "resource.h"
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
#include "SpellerBackgroundThread.h"
#include "TeXnicCenter.h"

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
	virtual void OnACCommandSelect(const CLaTeXCommand* cmd, const TCHAR AdditionalInsertChar = _T(''))
	{
		p->OnACCommandSelect(cmd, AdditionalInsertChar);
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

	EnableAutoIndent();
	GetCtrl().SetCaretSticky(FALSE);

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
	const LOGFONT& editor_font = CConfiguration::GetInstance()->m_fontEditor;
	const int point_size = GetLogFontPointSize(editor_font);

	//Default text.
	SetAStyle(SCE_TEX_TEXT, GetColor(COLORINDEX_NORMALTEXT), GetColor(COLORINDEX_BKGND));

	//Syntax
	SetAStyle(SCE_TEX_COMMAND, GetColor(COLORINDEX_KEYWORD),GetColor(COLORINDEX_BKGND));
	// - comments displayed in italics
	SetAStyle(SCE_TEX_COMMENT, GetColor(COLORINDEX_COMMENT), GetColor(COLORINDEX_BKGND), point_size, editor_font.lfFaceName, false, true);
	SetAStyle(SCE_TEX_SPECIAL, GetColor(COLORINDEX_SPECIAL), GetColor(COLORINDEX_BKGND));
	SetAStyle(SCE_TEX_SYMBOL, GetColor(COLORINDEX_SYMBOLS), GetColor(COLORINDEX_BKGND));

	SetAStyle(SCE_TEX_GROUP, GetColor(COLORINDEX_GROUP), GetColor(COLORINDEX_BKGND));
	SetAStyle(SCE_TEX_GROUP_NAME, GetColor(COLORINDEX_GROUP_NAME),GetColor(COLORINDEX_BKGND), point_size, editor_font.lfFaceName, true, false);

	SetAStyle(SCE_TEX_STYLE_INCLUSION, GetColor(COLORINDEX_STYLE_INCLUSION), GetColor(COLORINDEX_BKGND), point_size, editor_font.lfFaceName, true, false);
	SetAStyle(SCE_TEX_FILE_INCLUSION, GetColor(COLORINDEX_FILE_INCLUSION), GetColor(COLORINDEX_BKGND), point_size, editor_font.lfFaceName, true, false);

	SetAStyle(SCE_TEX_INLINE_MATH, GetColor(COLORINDEX_INLINE_MATH_TEXT), GetColor(COLORINDEX_BKGND));
	SetAStyle(SCE_TEX_INLINE_MATH_COMMAND, GetColor(COLORINDEX_INLINE_MATH_COMMAND), GetColor(COLORINDEX_BKGND));
	
	SetAStyle(SCE_TEX_DIGIT, GetColor(COLORINDEX_DIGIT), GetColor(COLORINDEX_BKGND));
	SetAStyle(SCE_TEX_UNIT, GetColor(COLORINDEX_UNIT), GetColor(COLORINDEX_BKGND), point_size, editor_font.lfFaceName, true, false);
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
	long ptStart = GetCtrl().GetCurrentPos();
	CPoint ptClient;
	CString keyw = GetWordAroundRange(ptStart, ptStart, true, false, false, true, false, false, false);

	if (keyw.GetLength() > MINIMUM_KEYWORD_LENGTH)
	{
		SharedObjectMap map;

		theApp.m_AvailableCommands.GetAllPossibleItems(keyw,_T(""),map);

		if (map.GetCount() == 1) //keyword is unique -> show the advice
		{
			std::tr1::shared_ptr<CObject> c = map.PGetFirstAssoc()->value;
			SharedLaTeXCommandPtr lc = std::tr1::dynamic_pointer_cast<CLaTeXCommand>(c);

			if (lc != NULL)
			{
				CString AdviceText = lc->GetExpandBefore() + lc->ToLaTeX() + lc->GetExpandAfter();
				if (AdviceText.GetLength() > keyw.GetLength())
				{
					if (instant_advice_tip_ == NULL)
					{
						instant_advice_tip_ = new CAdvice();
						instant_advice_tip_->Create(lc->ToLaTeX(),WS_POPUP | WS_BORDER/*SS_SUNKEN*/,CRect(),CWnd::FromHandle(GetCtrl()));
					}

					if (!instant_advice_tip_->IsWindowVisible())
					{
						// Compute window size
						CPoint ptClient(GetCtrl().PointXFromPosition(ptStart),GetCtrl().PointYFromPosition(ptStart));
						::ClientToScreen(GetSafeHwnd(),&ptClient);
						// Determine if there is space enough to show the window below the text
						ptClient.y += GetCtrl().TextHeight(GetCtrl().LineFromPosition(ptStart));
						// Place and show the window
						instant_advice_tip_->SetWindowPos(NULL,ptClient.x,ptClient.y,0,0,SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
						instant_advice_tip_->SetWindowText(AdviceText);
						instant_advice_tip_->ShowWindow(SW_SHOW);
						SetFocus();

						const UINT advice_tip_timeout = 3000;
						instant_advice_tip_->SetTimer(1,advice_tip_timeout,0);
					}
				}
				else
				{
					//Advice has same length or even smaller than the triggering keyword.
					HideAdvice();
				}
			}
			else
			{
				HideAdvice();
			}
		}
		else
		{
			// Nothing found: Hide window
			HideAdvice();
		}
	}
	else
	{
		//Keyword before cursor is too small
		HideAdvice();
	}
}

void LaTeXView::HideAdvice()
{
	if (instant_advice_tip_) instant_advice_tip_->ShowWindow(SW_HIDE);
}

#pragma endregion

#pragma region Message handlers for auto complete listbox

void LaTeXView::OnACCommandSelect(const CLaTeXCommand* cmd, const TCHAR AdditionalInsertChar)
{
	GetCtrl().BeginUndoAction();

	int initial_line_count = GetCtrl().GetLineCount();

	long ptCaret;

	autocompletion_active_ = false;
	old_pos_start_ = old_pos_end_ = -1;

	ASSERT(cmd != NULL);

	//Get the place where the triggering text starts
	long origs = GetCtrl().GetSelectionStart();

	//Generate string to be inserted before the triggering text
	CString BeforeString = cmd->GetExpandBefore();
	// - and find the actual start for the insertion
	// - Example: "\begin{figu" becomes "\begin{figure}..." and not "\begin{\begin{figure}...".
	for (int i=BeforeString.GetLength()-1;i>=0&&origs>=0;i--)
	{
		if (BeforeString[i] == GetCtrl().GetCharAt(origs - 1)) origs--;
	}
	// - start the selection there
	GetCtrl().SetSelectionStart(origs);
	const int start_line = GetCtrl().LineFromPosition(origs);

	//Generate string to be inserted from the possibly new selection start
	CString InsertString = BeforeString + cmd->ToLaTeX();
	InsertString += cmd->GetExpandAfter();

	//Do the actual insertion
	GetCtrl().ReplaceSel(InsertString);
	const long s = GetCtrl().GetSelectionStart();
	const long e = GetCtrl().GetSelectionEnd();
	GetCtrl().SetSel(-1,s); // drop selection
	bool bTryCursorPositioning = true;

	//Add additional char
	if (AdditionalInsertChar != _T('')) {
		const TCHAR text[] = {static_cast<TCHAR>(AdditionalInsertChar), 0};
		GetCtrl().InsertText(-1, text);
		GetCtrl().SetSel(-1, GetCtrl().GetCurrentPos() + 1);
		bTryCursorPositioning = false; //We obviously want to be where we just added this char
	}

	bool isEnv = cmd->GetExpandAfter().GetLength() && cmd->GetExpandBefore().GetLength();

	if (!isEnv && bTryCursorPositioning) {
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

	if (isEnv && bTryCursorPositioning) {
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
		SetFocus();
	}
}

void LaTeXView::OnACChar(UINT nKey, UINT /*nRepCount*/, UINT /*nFlags*/)
{
	if (autocompletion_list_ && autocompletion_list_->IsWindowVisible())
	{
		//Save the anchor and current position of the selection
		long anchor = GetCtrl().GetAnchor();
		long pos = GetCtrl().GetCurrentPos();

		//Insert the character at the current position
		const TCHAR text[] = {static_cast<TCHAR>(nKey),0};
		GetCtrl().InsertText(-1, text); //-1 inserts at current caret position

		//Restore selection, but including the newly typed character
		GetCtrl().SetAnchor(anchor);
		GetCtrl().SetCurrentPos(pos + 1);
	}
}

void LaTeXView::OnACBackspace()
{
	if (autocompletion_list_ && autocompletion_list_->IsWindowVisible())
	{
		//Save the anchor and current position of the selection
		long anchor = GetCtrl().GetAnchor();
		long pos = GetCtrl().GetCurrentPos();

		//If there is still something to delete
		if (pos > 0 && pos > anchor)
		{
			// - select the character to be removed
			GetCtrl().SetSel(pos - 1, pos);
			// - remove the character
			GetCtrl().ReplaceSel(_T(""));
			// - restore selection
			GetCtrl().SetSel(anchor, GetCtrl().GetCurrentPos());
		}
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
		if (!autocompletion_list_) {
			autocompletion_list_.reset(new CAutoCompleteDlg(&theApp.m_AvailableCommands, this));
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
	return autocompletion_list_.get();
}

int LaTeXView::GetNumberOfMatches( const CString& keyword )
{
	if (keyword.GetLength() < CAutoCompleteDlg::GetMinimumKeywordLength()) return 0;

	SharedObjectMap map;
	theApp.m_AvailableCommands.GetAllPossibleItems(keyword,_T(""),map);
	return map.GetCount();
}

void LaTeXView::AddExtendedWordChars(CString& WordChars)
{
	WordChars.Append(_T("&-+:"));

	/* NOTE:

		All the following chars are allowed in labels.
		But we do not allow all, since some of them are rather strange.

		Allowed:
			'&'
			'_'
			'-'
			'+'
			':'
		Not allowed by us:
			'='
			'^'
			'.'
			';'
			',' //A comma can be used in \cite{weinkauf04a,weinkauf05b} and we want to see this as two bibkeys
			'!'
			'`'
			'�'
			'(' //Especially braces should not be part of a label; this would kill most editor stuff
			')'
			'['
			']'
			'<'
			'>'

		The following chars are allowed in keywords: '\\' '@'
		Furthermore and by default, labels can consist of numbers and letters; keywords only of letters
	*/
}

void LaTeXView::AddEscapeChars(CString& EscapeChars)
{
	EscapeChars.Append(_T("\\"));
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

		//Write the selected lines to the new file - same encoding as the original file
		TextDocument td(GetDocument());
		td.Write(OutsourceDlg.NewPath.GetPath(), GetCtrl().GetSelText());

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
	const long topLeft = GetCtrl().GetCurrentPos();
	keyword = GetWordAroundRange(topLeft, topLeft, true, false, false, true, false, false, true);

	if (!keyword.IsEmpty())
		autocompletion_list_.reset(CreateListBox(keyword, topLeft)); /* setup (and show) list box */
	else
		GetCtrl().SetSel(old_pos_start_,old_pos_end_); /* restore old position */
}


BOOL LaTeXView::OnInsertLaTeXConstruct( UINT nID )
{
	bool bReplaceBell = false;
	CString strInsert; // text to insert

	std::unique_ptr<CInsertFloatObjectDialog> pDlg;

	long ptSelStart = GetCtrl().GetSelectionStart();
	long ptSelEnd = GetCtrl().GetSelectionEnd();

#pragma region ID specific handling

	switch (nID) {
		case ID_INSERT_FIGURE:
			pDlg.reset(new CInsertFloatObjectDialog(CInsertFloatObjectDialog::figure, this));
			break;
		case ID_INSERT_TABLE:
			pDlg.reset(new CInsertFloatObjectDialog(CInsertFloatObjectDialog::table, this));
			break;
		case ID_INSERT_INCLUDEGRAPHICS:
			pDlg.reset(new CInsertGraphicDialog(this));
			break;
		case ID_INSERT_TABULAR:
			pDlg.reset(new CInsertTabularDialog(this));
			break;
	}

	switch (nID) {
	case ID_INSERT_HEADER:
		{
			CInsertHeaderDialog dlg(this);

			if (ptSelStart != ptSelEnd) {
				CString strSel = GetCtrl().GetSelText();

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
				CString strSel = GetCtrl().GetSelText();
				bIsSelectionEnvironment = pDlg->SetProperties(strSel);
			}

			if (pDlg->DoModal() != IDOK) {
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


	//Get info about selection and number of lines
	ptSelStart = GetCtrl().GetSelectionStart();
	ptSelEnd = GetCtrl().GetSelectionEnd();
	const int start_line = GetCtrl().LineFromPosition(ptSelStart);
	const bool bSelectionStartsAtBeginningOfLine = (GetCtrl().PositionFromLine(start_line) == ptSelStart);
	const int end_line = GetCtrl().LineFromPosition(ptSelEnd);
	const bool bSelectionEndsAtEndOfLine = (GetCtrl().GetLineEndPosition(end_line) == ptSelEnd);
	const int initial_line_count = GetCtrl().GetLineCount();

	//When inserting before the beginning of a line, then do not add an extra newline
	if (bSelectionStartsAtBeginningOfLine && strBeforeCursor[0] == _T('\r')) {
		strBeforeCursor = strBeforeCursor.Right(strBeforeCursor.GetLength() - 2);
	}
	//When inserting behind the end of a line, then do not add an extra newline
	if (bSelectionEndsAtEndOfLine && strBehindCursor[strBehindCursor.GetLength() - 1] == _T('\n')) {
		strBehindCursor = strBehindCursor.Left(strBehindCursor.GetLength() - 2);
	}

	//Group the following actions into one single undo command
	GetCtrl().BeginUndoAction();

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

int LaTeXView::GetLexer() const
{
	return SCLEX_TEX;
}
