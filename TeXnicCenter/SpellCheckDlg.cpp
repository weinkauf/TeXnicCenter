/********************************************************************
 *
 * This file is part of the TeXnicCenter-system
 *
 * Copyright (C) 2002 Chris Norris
 * Copyright (C) 2000-$CurrentYear$ ToolsCenter
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * If you have further questions or if you want to support
 * further TeXnicCenter development, visit the TeXnicCenter-homepage
 *
 *    http://www.ToolsCenter.org
 *
 *********************************************************************/

#include "stdafx.h"
#include "resource.h"

#include <set>
#include <algorithm>
#include <functional>

#include "Speller.h"
#include "SpellCheckDlg.h"
#include "DocumentTokenizer.h"
#include "LaTeXView.h"
#include "LaTeXDocument.h"
#include "SpellerBackgroundThread.h"
#include "EncodingConverter.h"


SpellCheckDlg::SpellCheckDlg(LaTeXView* view, Speller* speller, CWnd* pParent /*= NULL*/)
: CDialog(SpellCheckDlg::IDD, pParent)
, tokenizer_(0)
, selection_only_(true)
, show_done_message_(true)
, editing_(false)
, speller_(0)
, current_line_(0)
{
	if (view)
		Reset(view,speller);	
}

void SpellCheckDlg::Reset(LaTeXView *pBuddy, Speller *pSpell)
{
	VERIFY(view_ = pBuddy);
	tokenizer_ = view_->GetTokenizer();
	tokenizer_->ResetState();
	speller_ = pSpell;
}

void SpellCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SpellCheckDlg)
	DDX_Control(pDX, IDC_SPELL_SUGGEST, suggest_list_);
	DDX_Control(pDX, IDC_SPELL_TEXT, edit_);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(SpellCheckDlg, CDialog)
	//{{AFX_MSG_MAP(SpellCheckDlg)
	ON_BN_CLICKED(IDC_SPELL_IGNORE, OnSpellIgnore)
	ON_BN_CLICKED(IDC_SPELL_IGNORE_ALL, OnSpellIgnoreAll)
	ON_BN_CLICKED(IDC_SPELL_NEXT, OnSpellNext)
	ON_EN_CHANGE(IDC_SPELL_TEXT, OnChangeSpellText)
	ON_BN_CLICKED(IDC_SPELL_RESUME, OnSpellResume)
	ON_BN_CLICKED(IDC_SPELL_REPLACE, OnSpellReplace)
	ON_NOTIFY(NM_DBLCLK, IDC_SPELL_SUGGEST, OnDblclkSpellSuggest)
	ON_BN_CLICKED(IDC_SPELL_ADD, OnSpellAdd)
	ON_BN_CLICKED(IDC_SPELL_REPLACE_ALL, OnSpellReplaceAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// SpellCheckDlg message handlers

void SpellCheckDlg::OnSpellIgnore()
{
	if (editing_)
	{
		// Unedit
		editing_ = false;
		new_line_ = true;
		CString label;
		label.LoadString(IDS_SPELL_IGNORE);
		GetDlgItem(IDC_SPELL_IGNORE)->SetWindowText(label);
		GetDlgItem(IDC_SPELL_IGNORE)->Invalidate();
		OnSpellError();
	}
	else
	{
		// Ignore
		current_line_start_pos_ = current_line_end_pos_;
		DoNextWord();
		OnSpellError();
	}
}

void SpellCheckDlg::OnSpellIgnoreAll()
{
	speller_->Ignore(word);
	
	if (view_->GetDocument()->IsSpellerThreadAttached())
		view_->GetDocument()->GetSpellerThread()->RecheckSingleLineSpelling(current_line_,view_);

	current_line_start_pos_ = current_line_end_pos_;

	DoNextWord();
	OnSpellError();
}

void SpellCheckDlg::OnSpellAdd()
{
	speller_->Add(word);
	
	if (view_->GetDocument()->IsSpellerThreadAttached())
		view_->GetDocument()->GetSpellerThread()->RecheckSingleLineSpelling(current_line_,view_);

	current_line_start_pos_ = current_line_end_pos_;

	DoNextWord();
	OnSpellError();
}

void SpellCheckDlg::OnSpellNext()
{
	current_line_start_pos_ = current_line_end_pos_ = 0;
	++current_line_;
	new_line_ = true;

	DoNextWord();
	OnSpellError();
}

BOOL SpellCheckDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (view_ == NULL || speller_ == NULL)
		return TRUE;

	long s = view_->GetCtrl().GetSelectionStart();
	long e = view_->GetCtrl().GetSelectionEnd();

	if (s != e && selection_only_)
	{
		start_pos_ = s;
		end_pos_ = e;
	}
	else
	{
		start_pos_ = 0;
		end_pos_ = view_->GetCtrl().GetLength();
	}

	current_line_ = view_->GetCtrl().LineFromPosition(start_pos_);

	current_line_start_pos_ = current_line_end_pos_ = start_pos_ - view_->GetCtrl().PositionFromLine(current_line_);
	new_line_ = true;

	// Select the first word
	DoNextWord();
	OnSpellError();

	return FALSE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void SpellCheckDlg::DoNextWord()
{
	CString text;

	while (current_line_ < view_->GetCtrl().LineFromPosition(end_pos_) || 
		(current_line_ == view_->GetCtrl().LineFromPosition(end_pos_) && 
		view_->GetCtrl().PositionFromLine(current_line_) + current_line_end_pos_ < end_pos_))
	{
		text = view_->GetLineText(current_line_);

		while (tokenizer_->NextWord(text, current_line_start_pos_, current_line_end_pos_))
		{
			if (abs(current_line_start_pos_ - current_line_end_pos_) < MAXWORDLEN)
			{
				word = text.Mid(current_line_start_pos_,current_line_end_pos_ - current_line_start_pos_);

				if (!speller_->Spell(word))
				{
					bool decorate = true;
					long pos = view_->GetCtrl().PositionFromLine(current_line_);

					const CString left = text.Left(current_line_start_pos_);
					
					// Special case for words that end e.g. with a dot, such as etc., bzgl. and so on,
					// see also SpellerBackgroundThread::DoCheckLine
					if (current_line_end_pos_ < text.GetLength())
						decorate = !speller_->Spell(word + text[current_line_end_pos_]);

					if (decorate) {
						// We operate on UTF-8 bytes, thus the conversion
						std::vector<char> buffer1, buffer2;

#ifdef UNICODE
						UTF16toUTF8(left,left.GetLength(),buffer1);
						UTF16toUTF8(word,word.GetLength(),buffer2);
#else
						ANSItoUTF8(left,left.GetLength(),buffer1);
						ANSItoUTF8(word,word.GetLength(),buffer2);
#endif
						long s = pos + static_cast<long>(buffer1.size());
						long e = s + static_cast<long>(buffer2.size());

						view_->GetCtrl().SetSel(s,e);
						view_->GetCtrl().EnsureVisible(current_line_);

						return;
					}					
				}
			}

			// else the word was too long, skip it
			current_line_start_pos_ = current_line_end_pos_;
		}

		// next line
		++current_line_;
		new_line_ = true;
		current_line_start_pos_ = current_line_end_pos_ = 0;
	}

	// We are done
	if (show_done_message_)
		AfxMessageBox(IDS_SPELL_DONE, MB_OK | MB_ICONINFORMATION);

	OnOK();
}

void SpellCheckDlg::OnSpellError()
{

	if (!(current_line_ < view_->GetCtrl().LineFromPosition(end_pos_) || 
		(current_line_ == view_->GetCtrl().LineFromPosition(end_pos_) && 
		view_->GetCtrl().PositionFromLine(current_line_) + current_line_end_pos_ < end_pos_)))
		return;

	if (new_line_)
	{
		edit_.SetWindowText(view_->GetLineText(current_line_));
		new_line_ = false;
	}

	edit_.SetSel(current_line_start_pos_, current_line_end_pos_);
	edit_.SetFocus();
	edit_.Invalidate();

	suggest_list_.DeleteAllItems();

	CStringArray ssList;
	int nCount = speller_->Suggest(word,ssList);

	if (nCount < 1)
	{
		no_suggestions_ = true;
		CString label;
		label.LoadString(IDS_SPELL_NO_SUGGESTIONS);
		VERIFY(suggest_list_.InsertItem(0, label) != -1);
	}
	else
	{
		no_suggestions_ = false;

		for (int i = 0; i < nCount; ++i)
			VERIFY(suggest_list_.InsertItem(i, ssList[i]) != -1);
	}

	suggest_list_.EnableWindow(!no_suggestions_);
	GetDlgItem(IDC_SPELL_REPLACE)->EnableWindow(!no_suggestions_);
	GetDlgItem(IDC_SPELL_REPLACE_ALL)->EnableWindow(!no_suggestions_);
}

void SpellCheckDlg::OnChangeSpellText()
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// The user has taken control of the line. Disable all the controls except
	// Cancel, Resume, and Unedit / Ignore.
	if (!editing_)
	{
		editing_ = true;
		suggest_list_.EnableWindow(false);
		GetDlgItem(IDC_SPELL_IGNORE_ALL)->EnableWindow(false);
		GetDlgItem(IDC_SPELL_REPLACE)->EnableWindow(false);
		GetDlgItem(IDC_SPELL_REPLACE_ALL)->EnableWindow(false);
		GetDlgItem(IDC_SPELL_NEXT)->EnableWindow(false);
		GetDlgItem(IDC_SPELL_RESUME)->EnableWindow(true);

		CString label;
		label.LoadString(IDS_SPELL_UNEDIT);
		GetDlgItem(IDC_SPELL_IGNORE)->SetWindowText(label);
		GetDlgItem(IDC_SPELL_IGNORE)->Invalidate();
	}
}

void SpellCheckDlg::OnSpellResume()
{
	ASSERT(editing_);
	editing_ = false;

	GetDlgItem(IDC_SPELL_IGNORE_ALL)->EnableWindow(true);
	GetDlgItem(IDC_SPELL_NEXT)->EnableWindow(true);
	GetDlgItem(IDC_SPELL_RESUME)->EnableWindow(false);
	CString label;
	label.LoadString(IDS_SPELL_IGNORE);
	GetDlgItem(IDC_SPELL_IGNORE)->SetWindowText(label);

	// Find the first change position
	CString newText;
	edit_.GetWindowText(newText);
	int start = FirstDifference(view_->GetLineText(current_line_), newText);
	if (start != -1)
	{
		int endOld, endNew;
		LastDifference(view_->GetLineText(current_line_), endOld, newText, endNew);
		++endOld; // point one past last difference
		++endNew; // point one past last difference
		LPTSTR szNewText = newText.GetBuffer(0);
		szNewText[endNew] = _T('\0');

		long s = view_->GetCtrl().PositionFromLine(current_line_) + start;
		view_->GetCtrl().SetSel(s,s + endOld - start);

		view_->GetCtrl().ReplaceSel(&szNewText[start]);
		newText.ReleaseBuffer(0);
		current_line_start_pos_ = current_line_end_pos_ = 0;
	}
	// else no change was made
	DoNextWord();
	OnSpellError();
}

void SpellCheckDlg::OnSpellReplace()
{
	// Get the selected suggestion text
	POSITION pos = suggest_list_.GetFirstSelectedItemPosition();

	if (!pos)
		return;

	const CString newText = suggest_list_.GetItemText(suggest_list_.GetNextSelectedItem(pos), 0);
	view_->GetCtrl().ReplaceSel(newText);
	
	DoNextWord();
	OnSpellError();
}

void SpellCheckDlg::OnSpellReplaceAll()
{
	// Get the selected suggestion text
	POSITION pos = suggest_list_.GetFirstSelectedItemPosition();

	if (!pos)
		return;

	const CString newText = suggest_list_.GetItemText(suggest_list_.GetNextSelectedItem(pos), 0);
	const CString oldText = word;

	CScintillaCtrl& c = view_->GetCtrl();
	c.SetSearchFlags(SCFIND_MATCHCASE);

	c.BeginUndoAction();

	// Search from start to end position
	c.SetTargetStart(start_pos_);
	c.SetTargetEnd(end_pos_);

	long find_pos;
#if 0
	std::set<int> invalidated_lines;
#endif // 0
	
	while ((find_pos = c.SearchInTarget(oldText.GetLength(),oldText)) != -1)
	{
		c.ReplaceTarget(newText.GetLength(),newText);
		c.SetTargetStart(c.GetTargetEnd() + 1);
		c.SetTargetEnd(end_pos_); // Still end position
#if 0
		invalidated_lines.insert(c.LineFromPosition(find_pos));
#endif // 0
	}

	c.EndUndoAction();

#if 0
	if (SpellerBackgroundThread* s = view_->GetDocument()->GetSpellerThread()) {
		using namespace std::tr1;
		using namespace placeholders;

		// Recheck the spelling on lines that have been modified
		std::for_each(invalidated_lines.begin(),invalidated_lines.end(),
			bind(&SpellerBackgroundThread::RecheckSingleLineSpelling,s,_1,view_));
	}
#endif // 0

	DoNextWord();
	OnSpellError();
}

void SpellCheckDlg::OnDblclkSpellSuggest(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	OnSpellReplace();
	*pResult = 0;
}

int SpellCheckDlg::DoModal()
{
	if (speller_ == NULL)
		return IDABORT;

	return CDialog::DoModal();
}

int SpellCheckDlg::FirstDifference( const TCHAR *one, const TCHAR *two )
{
	int index = 0;
	while (*one != _T('\0') && *two != _T('\0'))
	{
		if (*one++ != *two++)
			return index;
		++index;
	}
	if (*one == _T('\0') && *two == _T('\0'))
		return -1; // no difference
	return index;
}

int SpellCheckDlg::LastDifference( const TCHAR *one, int &oneIndex, const TCHAR *two, int &twoIndex )
{
	oneIndex = _tcsclen(one);
	twoIndex = _tcsclen(two);
	int nLength = 0;
	while (oneIndex >= 0 && twoIndex >= 0)
	{
		if (one[oneIndex] != two[twoIndex])
			break;
		--oneIndex;
		--twoIndex;
		++nLength;
	}
	if (oneIndex == -1 && twoIndex == -1)
		return -1; // no difference
	return nLength;
}

bool SpellCheckDlg::GetShowDoneMessage() const
{
	return show_done_message_;
}

void SpellCheckDlg::SetShowDoneMessage( bool val /*= true*/ )
{
	show_done_message_ = val;
}

bool SpellCheckDlg::GetCheckSelectionOnly() const
{
	return selection_only_;
}

void SpellCheckDlg::SetCheckSelectionOnly( bool val /*= true*/ )
{
	selection_only_ = val;
}