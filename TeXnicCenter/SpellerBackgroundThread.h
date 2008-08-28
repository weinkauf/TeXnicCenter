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

#pragma once

#include <set>

class Speller;
class CodeView;

class SpellerSource
{
public:
	/**
	Get a speller
	@return Initialized and configured spell checker
	 */
	virtual Speller *GetSpeller() = 0;
};

class SpellerBackgroundThread : public CWinThread
{
	DECLARE_DYNCREATE(SpellerBackgroundThread)

	CCriticalSection invalidate_monitor_;

	typedef std::set<CodeView*> InvalidViewContainerType;
	InvalidViewContainerType invalid_views_;

public:
	SpellerBackgroundThread();

// message handlers
private:
	afx_msg void AFX_MSG_CALL OnUpdateBuffer(WPARAM wParam, LPARAM lParam);
	afx_msg void AFX_MSG_CALL OnUpdateLine(WPARAM wParam, LPARAM lParam);
	afx_msg void AFX_MSG_CALL OnGetSpeller(WPARAM wParam, LPARAM lParam);

private:
	afx_msg void AFX_MSG_CALL OnEnableSpeller(WPARAM wParam, LPARAM lParam);
public:
	//afx_msg void AFX_MSG_CALL OnInvalidateView(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

// overrides
private:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);

public:
	/**
	Set the speller
	@param pSpell Pointer to speller object
	 */
	void SetSpeller(Speller *pSpell);

protected:
	/**
	Spell check a single line
	@param pTextView View to check
	@param nLine Line number
	 */
	void SpellCheckSingleLine(CodeView* pTextView, int nLine);

	/**
	Spell check an entire buffer.
	@param pTextView View to check
	 */
	void SpellCheckBuffer(CodeView* pTextView);

private:
	void DoCheckLine(CodeView* view, int line, CString &text);
	void DoCheckLine(CodeView* view, int line);

	/**
	Remove attributes from a buffer
	@param pTextView Buffer to remove attributes from
	 */
	void RemoveBufferAttributes(CodeView* pTextView);

	/**
	Internal method to determine is a view is still valid. A view is invalidated when it sends
	a ID_BG_INVALIDATE_VIEW message.
	@param pView The view to check
	@return <VAR>TRUE</VAR> if valid else <VAR>FALSE</VAR>.
	 */

public:
	void RecheckSpelling(CodeView* view);
	void RecheckSingleLineSpelling(int line, CodeView* view);
	void EnableSpeller(bool enable = true);
	void ResetSpeller(SpellerSource* s);
	void InvalidateView(CodeView* view);

private:
	bool IsViewValid(CodeView* view);
	
	Speller *speller_;
	bool spell_enabled_;
	//CPtrList m_pInvalidViews;
};
