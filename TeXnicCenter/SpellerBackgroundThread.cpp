/********************************************************************
 *
 * This file is part of the TeXnicCenter-system
 *
 * Copyright (C) 2002-$CurrentYear$ Chris Norris
 * Copyright (C) 2002-$CurrentYear$ ToolsCenter
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

/********************************************************************
 *
 * $Id$
 *
 ********************************************************************/

#include "stdafx.h"
#include "SpellerBackgroundThread.h"

#include "CodeView.h"
#include "Speller.h"
#include "DocumentTokenizer.h"
#include "configuration.h"


enum {
	ID_BG_UPDATE_BUFFER = WM_USER,
	ID_BG_UPDATE_LINE,
	ID_BG_RESET_SPELLER,
	ID_BG_ENABLE_SPELLER,
	ID_BG_INVALIDATE_VIEW
};

const int ShortWordLength = 10;

IMPLEMENT_DYNCREATE(SpellerBackgroundThread, CWinThread)

BEGIN_MESSAGE_MAP(SpellerBackgroundThread, CWinThread)
	ON_THREAD_MESSAGE(ID_BG_UPDATE_BUFFER, &SpellerBackgroundThread::OnUpdateBuffer)
	ON_THREAD_MESSAGE(ID_BG_UPDATE_LINE, &SpellerBackgroundThread::OnUpdateLine)
	ON_THREAD_MESSAGE(ID_BG_RESET_SPELLER, &SpellerBackgroundThread::OnGetSpeller)
	ON_THREAD_MESSAGE(ID_BG_ENABLE_SPELLER, &SpellerBackgroundThread::OnEnableSpeller)
	ON_THREAD_MESSAGE(ID_BG_INVALIDATE_VIEW, &SpellerBackgroundThread::OnInvalidateView)
END_MESSAGE_MAP()


SpellerBackgroundThread::SpellerBackgroundThread()
: speller_(0), spell_enabled_(false)
{
}

void SpellerBackgroundThread::OnUpdateBuffer(WPARAM /*wParam*/, LPARAM lParam)
{
	CodeView* pTextView = reinterpret_cast<CodeView*>(lParam);

	ASSERT(pTextView);

	if (IsViewValid(pTextView)) {
		// Check if there are any redundant message in the message queue.
		// This should really improve speed on slower machines and machines
		// doing background processing.
		MSG msg;
		while (::PeekMessage(&msg, NULL, ID_BG_UPDATE_BUFFER, ID_BG_UPDATE_BUFFER, PM_NOREMOVE) != 0)
		{
			if (msg.lParam == lParam)
			{
				// Remove and discard the duplicate
				::PeekMessage(&msg, NULL, ID_BG_UPDATE_BUFFER, ID_BG_UPDATE_BUFFER, PM_REMOVE);
				continue;
			}
			break;
		}

		if (spell_enabled_)
			SpellCheckBuffer(pTextView);
		else
			RemoveBufferAttributes(pTextView);
	}
}

void SpellerBackgroundThread::OnUpdateLine(WPARAM wParam, LPARAM lParam)
{
	CodeView *pTextView = (CodeView *)lParam;
	ASSERT(pTextView);
	int nLine = (int)wParam;

	if (IsViewValid(pTextView)) {
		// Check if there are any redundant message in the message queue.
		// This should really improve speed on slower machines and machines
		// doing background processing.
		MSG msg;

		while (::PeekMessage(&msg, NULL, ID_BG_UPDATE_BUFFER, ID_BG_UPDATE_LINE, PM_NOREMOVE) != 0)
		{
			if (msg.lParam == lParam)
			{
				// This buffer has a message. What could it be?
				if (msg.message == ID_BG_UPDATE_BUFFER)
				{
					// A message to check the whole buffer is in the queue.
					// There is no point checking a single line -- we're done.
					return;
				}
				else if (msg.message == ID_BG_UPDATE_LINE && msg.wParam == wParam)
				{
					// A duplicate message. Remove and discard.
					::PeekMessage(&msg, NULL, ID_BG_UPDATE_LINE, ID_BG_UPDATE_LINE, PM_REMOVE);
					continue;
				}
				// else  We need to handle this message
			}
			break;
		}

		if (spell_enabled_)
		{
			SpellCheckSingleLine(pTextView, nLine);
			Sleep(3000); // Give the typist a chance to get ahead to prevent flicker
		}
	}
}

void SpellerBackgroundThread::OnGetSpeller(WPARAM /*wParam*/, LPARAM lParam)
{
	SpellerSource *pSource = (SpellerSource *)lParam;

	if (pSource) {
		Speller *pSpell = pSource->GetSpeller();
		SetSpeller(pSpell);
	}
}

void SpellerBackgroundThread::OnEnableSpeller(WPARAM wParam, LPARAM lParam)
{
	bool bSpellEnabled = wParam != 0;

	if (bSpellEnabled == spell_enabled_)
		return;

	spell_enabled_ = bSpellEnabled;

	if (lParam != 0)
		OnUpdateBuffer(0, lParam);
}

void SpellerBackgroundThread::OnInvalidateView(WPARAM /*wParam*/, LPARAM lParam)
{
	CodeView* pTextView = reinterpret_cast<CodeView*>(lParam);

	if (pTextView) {
		m_pInvalidViews.AddHead(pTextView);
		// Unlock the view
		pTextView->Unlock();
	}
}

BOOL SpellerBackgroundThread::InitInstance()
{
	return TRUE;
}

BOOL SpellerBackgroundThread::OnIdle(LONG /*lCount*/)
{
	m_pInvalidViews.RemoveAll();
	return 0;
}

void SpellerBackgroundThread::SetSpeller(Speller *pSpell)
{
	speller_ = pSpell;
}

void SpellerBackgroundThread::SpellCheckSingleLine(CodeView* pTextView, int nLine)
{
	if (speller_) {
		CScintillaCtrl& c = pTextView->GetCtrl();
		pTextView->Lock();

		pTextView->GetTokenizer()->ResetState();

		if (::IsWindow(c) && nLine < c.GetLineCount(FALSE))
			DoCheckLine(pTextView,nLine);

		pTextView->Unlock();
	}
}

void SpellerBackgroundThread::SpellCheckBuffer(CodeView* pTextView)
{
	if (speller_ == NULL)
		return;

	ASSERT(pTextView);
	pTextView->Lock();

	pTextView->GetTokenizer()->ResetState();

	int line = 0;

	CScintillaCtrl& c = pTextView->GetCtrl();
	const int line_count = c.GetLineCount(FALSE);

	CString text;

	while (line < line_count)
		DoCheckLine(pTextView,line++,text);

	pTextView->Unlock();
}

void SpellerBackgroundThread::RemoveBufferAttributes(CodeView* pTextView)
{
	ASSERT(pTextView);
	pTextView->Lock();
	pTextView->GetCtrl().IndicatorClearRange(0,pTextView->GetCtrl().GetLength());
	pTextView->Unlock();
}

bool SpellerBackgroundThread::IsViewValid( void *pView )
{
	// This in affect gives the ID_BG_INVALIDATE_VIEW message the highest priority,
	// which is what we want.
	MSG msg;

	while (::PeekMessage(&msg, NULL, ID_BG_INVALIDATE_VIEW, ID_BG_INVALIDATE_VIEW, PM_REMOVE) != 0)
		OnInvalidateView(msg.wParam, msg.lParam);

	bool isValid = (pView != NULL);
	POSITION pos = m_pInvalidViews.GetHeadPosition();

	while (pos != NULL && isValid)
	{
		if (pView == m_pInvalidViews.GetNext(pos))
			isValid = false;
	}

	return isValid;
}



void SpellerBackgroundThread::DoCheckLine(CodeView* view, int line, CString &text)
{
	CScintillaCtrl &c = view->GetCtrl();
	DocumentTokenizer* tokenizer = view->GetTokenizer();

	long line_start = c.PositionFromLine(line,FALSE);
	long line_end = c.GetLineEndPosition(line,FALSE);

	// Clear the line indicators
	c.IndicatorClearRange(line_start,line_end - line_start,FALSE);
	long length = line_end - line_start;

	if (length > 0) {
		c.GetLineEx(line,text.GetBuffer(length + 1),length + 1,FALSE);
		text.ReleaseBuffer(length);

		int start = 0, end = 0;

		while (tokenizer->NextWord(text,text.GetLength(),start,end)) {
			int word_length = end - start;

			if (word_length > 0 && word_length < MAXWORDLEN) {
				CString word(text.Mid(start,word_length));

				if (!speller_->Spell(word)) {
					bool decorate = true;
					// Special case for abbrev. like bzw., bzgl. and so on:
					// Make sure it's short word first and check whether there is
					// one more character after this word
					if (word_length <= ShortWordLength && end + 1 <= length) {
						// Include this character and recheck the spelling
						word += text[end];
						decorate = !speller_->Spell(word);
					}

					if (decorate) {
						// Find word's start and end positions, add 1 otherwise
						// the previous word will be found
						long s = c.WordStartPosition(line_start + start + 1,TRUE,FALSE);
						long e = c.WordEndPosition(s,TRUE,FALSE);
						c.IndicatorFillRange(s,e - s,FALSE);
						//c.IndicatorFillRange(line_start + start,word_length,FALSE);
					}
				}
			}

			start = end;
		}
	}
}

void SpellerBackgroundThread::DoCheckLine(CodeView* view, int line)
{
	CString text;
	DoCheckLine(view,line,text);
}

void SpellerBackgroundThread::RecheckSpelling( CodeView* view )
{
	PostThreadMessage(ID_BG_UPDATE_BUFFER,0,reinterpret_cast<LPARAM>(view));
}

void SpellerBackgroundThread::RecheckSingleLineSpelling( int line, CodeView* view )
{
	PostThreadMessage(ID_BG_UPDATE_LINE,static_cast<WPARAM>(line),reinterpret_cast<LPARAM>(view));
}

void SpellerBackgroundThread::EnableSpeller( bool enable /*= true*/ )
{
	PostThreadMessage(ID_BG_ENABLE_SPELLER,enable,0);
}

void SpellerBackgroundThread::ResetSpeller( SpellerSource* s )
{
	PostThreadMessage(ID_BG_RESET_SPELLER,0,reinterpret_cast<LPARAM>(s));
}

void SpellerBackgroundThread::InvalidateView( CodeView* view )
{
	PostThreadMessage(ID_BG_INVALIDATE_VIEW,0,reinterpret_cast<LPARAM>(view));
}