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

#pragma once

class CodeView;
class DocumentTokenizer;
class Speller;

class SpellCheckDlg : public CDialog
{
	DocumentTokenizer* tokenizer_;
	CodeView* view_;
	long start_pos_, end_pos_;
	Speller* speller_;
	int current_line_, current_line_start_pos_, current_line_end_pos_;
	CString word;
	bool editing_;
	bool no_suggestions_;
	bool new_line_;
	CListCtrl suggest_list_;
	CEdit edit_;

	/** Set this member to <VAR>false</VAR> to override spell checking in selection only. */
	bool selection_only_;
	
	/** Set this member to <VAR>false</VAR> to surpress spell check done message. */
	bool show_done_message_;
	
// Construction
public:
	bool GetShowDoneMessage() const;
	void SetShowDoneMessage(bool val = true);
	bool GetCheckSelectionOnly() const;
	void SetCheckSelectionOnly(bool val = true);

	SpellCheckDlg(CodeView* view, Speller* speller, CWnd* pParent = NULL);

	enum
	{
		IDD = IDD_SPELL_CHECK
	};	

	/** Reset the view and speller
	@param pBuddy The view this dialog is attached to.
	@param pSpell The spell object that does most of the work.
	 */
	void Reset(CodeView *pBuddy, Speller *pSpell);

	// Overrides
	// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(SpellCheckDlg)
public:
	virtual INT_PTR DoModal();
protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	/**
	Handle the spelling error by configuring the suggestion list.
	 */
	void OnSpellError();

	/**
	Find the next word that is not found in the spell checker.
	 */
	void DoNextWord();

	// Generated message map functions
	//{{AFX_MSG(SpellCheckDlg)
	afx_msg void OnSpellIgnore();
	afx_msg void OnSpellIgnoreAll();
	afx_msg void OnSpellNext();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeSpellText();
	afx_msg void OnSpellResume();
	afx_msg void OnSpellReplace();
	afx_msg void OnDblclkSpellSuggest(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSpellAdd();
	afx_msg void OnSpellReplaceAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	// These a general routines that should be put somewhere else

	/**
	Find the first difference in two strings. Starting from the front of
	both strings, find the first character that differs in both strings.

	@param one
	        The first string to compare.
	@param two
	        The second string to compare.
	@return
	        The index of the first character that differs in the two strings. If
	        both strings are identical return -1.
	 */
	static int FirstDifference(const TCHAR *one, const TCHAR *two);

	/**
	Find the last difference in two strings. Starting from the back of both
	strings, find the first character that differs.

	@param one
	        The first string to compare
	@param oneIndex
	        The index of the last difference in string one. Return -1 if both
	        strings identical.
	@param two
	        The second string to compare
	@param twoIndex
	        The index of the last difference in string two. Return -1 if both
	        strings identical.
	@return
	        The length of the common string found at the end of string one and
	        string two. If both strings are identical return -1.
	 */
	static int LastDifference(const TCHAR *one, int &oneIndex, const TCHAR *two, int &twoIndex);
};
