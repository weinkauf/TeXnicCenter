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

#if !defined(AFX_SPELLCHECKDLG_H__BA0F97E0_01C2_48D3_99AB_AEC5AC884C88__INCLUDED_)
#define AFX_SPELLCHECKDLG_H__BA0F97E0_01C2_48D3_99AB_AEC5AC884C88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpellCheckDlg.h : header file
//

#include "cedefs.h"
#include "editcmd.h"
#include "CCrystalEditView.h"

/////////////////////////////////////////////////////////////////////////////
// CSpellCheckDlg dialog

class CRYSEDIT_CLASS_DECL CSpellCheckDlg : public CDialog 
{
    // Construction
public:
    CSpellCheckDlg(CCrystalEditView *pBuddy, Speller *pSpell, CWnd* pParent = NULL);
    virtual ~CSpellCheckDlg();

    // Dialog Data
    //{{AFX_DATA(CSpellCheckDlg)

    enum {
        IDD = IDD_SPELL_CHECK
    };
    CListCtrl c_SuggestList;
    CEdit c_Text;
    //}}AFX_DATA

    /** Set this member to <VAR>false</VAR> to override spell checking in selection only. */
    bool m_bSelection;

    /** Set this member to <VAR>false</VAR> to surpress spell check done message. */
    bool m_bDoneMessage;

    /** Reset the view and speller 
    @param pBuddy The view this dialog is attached to.
    @param pSpell The spell object that does most of the work.
     */
    void Reset(CCrystalEditView *pBuddy, Speller *pSpell);

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSpellCheckDlg)
public:
    virtual int DoModal();
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
    //{{AFX_MSG(CSpellCheckDlg)
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

    CCrystalEditView *m_pBuddy;
    CCrystalTextBuffer *m_pTextBuffer;
    CCrystalParser *m_pParser;
    CPoint m_ptStart, m_ptEnd;
    Speller *m_pSpell;
    int m_nCurLine, m_nCurStart, m_nCurEnd;
    TCHAR m_pWordBuffer[MAXWORDLEN + 1];
    bool m_bEditing;
    bool m_bNoSuggestions;
    bool m_bNewLine;

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
    static int FirstDifference(const TCHAR *one, const TCHAR *two)
    {
        int index = 0;
        while (*one != _T('\0') && *two != _T('\0')) {
            if (*one++ != *two++)
                return index;
            ++index;
        }
        if (*one == _T('\0') && *two == _T('\0'))
            return -1; // no difference
        return index;
    }

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
    static int LastDifference(const TCHAR *one, int &oneIndex, const TCHAR *two, int &twoIndex)
    {
        oneIndex = _tcsclen(one);
        twoIndex = _tcsclen(two);
        int nLength = 0;
        while (oneIndex >= 0 && twoIndex >= 0) {
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
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPELLCHECKDLG_H__BA0F97E0_01C2_48D3_99AB_AEC5AC884C88__INCLUDED_)
