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

// SpellCheckDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpellCheckDlg.h"
#include "CCrystalEditView.h"
#include "CCrystalTextBuffer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpellCheckDlg dialog

CSpellCheckDlg::CSpellCheckDlg(CCrystalEditView *pBuddy, MySpell *pSpell, CWnd* pParent /*= NULL*/)
	: CDialog(CSpellCheckDlg::IDD, pParent)
{
	if (pBuddy != NULL)
		Reset( pBuddy, pSpell );

	m_bSelection = true;
	m_bDoneMessage = true;
	m_bEditing = false;

	//{{AFX_DATA_INIT(CSpellCheckDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


CSpellCheckDlg::~CSpellCheckDlg()
{
	POSITION pos = m_lstIgnoreWords.GetHeadPosition();
	void *pWord;
	while ( pos != NULL )
	{
		pWord = m_lstIgnoreWords.GetNext( pos );
		if ( pWord != NULL )
			free( pWord );
	}
}


void CSpellCheckDlg::Reset(CCrystalEditView *pBuddy, MySpell *pSpell)
{
	ASSERT( m_pBuddy = pBuddy );
	ASSERT( m_pTextBuffer = pBuddy->LocateTextBuffer() );
	ASSERT( m_pParser = m_pBuddy->GetParser() );
	ASSERT( m_pSpell = pSpell );	
}


void CSpellCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpellCheckDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSpellCheckDlg, CDialog)
	//{{AFX_MSG_MAP(CSpellCheckDlg)
	ON_BN_CLICKED(IDC_SPELL_IGNORE, OnSpellIgnore)
	ON_BN_CLICKED(IDC_SPELL_IGNORE_ALL, OnSpellIgnoreAll)
	ON_BN_CLICKED(IDC_SPELL_NEXT, OnSpellNext)
	ON_EN_CHANGE(IDC_SPELL_TEXT, OnChangeSpellText)
	ON_BN_CLICKED(IDC_SPELL_RESUME, OnSpellResume)
	ON_BN_CLICKED(IDC_SPELL_REPLACE, OnSpellReplace)
	ON_NOTIFY(NM_DBLCLK, IDC_SPELL_SUGGEST, OnDblclkSpellSuggest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSpellCheckDlg message handlers

void CSpellCheckDlg::OnSpellIgnore() 
{
	if ( m_bEditing )
	{
		m_bEditing = false;
		CString label;
		label.LoadString(IDS_SPELL_IGNORE);
		GetDlgItem(IDC_SPELL_IGNORE)->SetWindowText( label );
		GetDlgItem(IDC_SPELL_IGNORE)->Invalidate();
		OnSpellError();
	}
	else 
	{
		m_nCurStart = m_nCurEnd;
		DoNextWord();
		OnSpellError();
	}
}


void CSpellCheckDlg::OnSpellIgnoreAll() 
{
	m_lstIgnoreWords.AddTail( strdup(m_pWordBuffer) );
	m_nCurStart = m_nCurEnd;
	DoNextWord();
	OnSpellError();
}


void CSpellCheckDlg::OnSpellNext() 
{
	m_nCurStart = m_nCurEnd = 0;
	++m_nCurLine;
	DoNextWord();
	OnSpellError();
}


BOOL CSpellCheckDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	ASSERT( m_pBuddy );
	ASSERT( m_pSpell );

	if ( m_pBuddy->IsSelection() && m_bSelection )
	{
		m_pBuddy->GetSelection( m_ptStart, m_ptEnd );
	}
	else
	{
		m_ptStart.x = m_ptStart.y = 0;
		m_ptEnd.y = m_pTextBuffer->GetLineCount() - 1;
		m_ptEnd.x = m_pTextBuffer->GetLineLength( m_ptEnd.y );
	}
	m_nCurLine = m_ptStart.y;
	m_nCurStart = m_nCurEnd = m_ptStart.x;

	// Select the first word
	DoNextWord();
	OnSpellError();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CSpellCheckDlg::DoNextWord()
{
	while ( m_nCurLine < m_ptEnd.y || (m_nCurLine == m_ptEnd.y && m_nCurEnd < m_ptEnd.x) )
	{
		m_pParser->NextWord(m_nCurLine, m_nCurStart, m_nCurEnd);
		LPCSTR szLine = m_pTextBuffer->GetLineChars( m_nCurLine );
		while ( m_nCurStart != -1 )
		{
			if ( (m_nCurStart - m_nCurEnd) <  MAXWORDLEN )
			{
				int i = m_nCurStart;
				int j = 0;
				// Convert string to char*
				while ( i < m_nCurEnd )
					m_pWordBuffer[j++] = (char)szLine[i++];
				m_pWordBuffer[j] = 0;
				if ( !IsIgnoreWord(m_pWordBuffer) && !m_pSpell->spell(m_pWordBuffer))
				{
					m_pBuddy->HighlightText( CPoint(m_nCurStart, m_nCurLine), m_nCurEnd - m_nCurStart );
					return;
				}
			}
			// else the word was too long, skip it
			m_nCurStart = m_nCurEnd;
			m_pParser->NextWord( m_nCurLine, m_nCurStart, m_nCurEnd );
		} 

		// next line
		++m_nCurLine;
		m_nCurStart = m_nCurEnd = 0;
	}
	// We are done
	if ( m_bDoneMessage )
		AfxMessageBox( IDS_SPELL_DONE, MB_OK | MB_ICONINFORMATION );

	OnOK();
}


void CSpellCheckDlg::OnSpellError()
{
	if ( !(m_nCurLine < m_ptEnd.y || (m_nCurLine == m_ptEnd.y && m_nCurEnd < m_ptEnd.x)) )
		return;

	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_SPELL_TEXT);
	pEdit->SetWindowText( m_pTextBuffer->GetLineChars(m_nCurLine) );
	pEdit->SetSel( m_nCurStart, m_nCurEnd );
	pEdit->Invalidate();

	CListCtrl *pList = (CListCtrl*)GetDlgItem(IDC_SPELL_SUGGEST);
	pList->DeleteAllItems();
	char **ssList;
	int nCount = m_pSpell->suggest( &ssList, m_pWordBuffer );
	if (nCount < 1)
	{
		m_bNoSuggestions = true;
		CString label;
		label.LoadString(IDS_SPELL_NO_SUGGESTIONS);
		ASSERT( pList->InsertItem(0, label ) != -1 );
	}
	else
	{
		m_bNoSuggestions = false;
		for ( int i = 0; i < nCount; ++i)
		{
			ASSERT( pList->InsertItem(i, A2T(ssList[i])) != -1);
		}
		m_pSpell->release_suggest( ssList );
	}
	//pList->Invalidate();
	GetDlgItem(IDC_SPELL_SUGGEST)->EnableWindow( !m_bNoSuggestions );
	GetDlgItem(IDC_SPELL_REPLACE)->EnableWindow( !m_bNoSuggestions );
	GetDlgItem(IDC_SPELL_REPLACE_ALL)->EnableWindow( !m_bNoSuggestions );
}


void CSpellCheckDlg::OnChangeSpellText() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// The user has taken control of the line. Disable all the controls except
	// Cancel, Resume, and Unedit (Ignore).
	if ( !m_bEditing )
	{
		m_bEditing = true;
		GetDlgItem(IDC_SPELL_SUGGEST)->EnableWindow( false );
		GetDlgItem(IDC_SPELL_IGNORE_ALL)->EnableWindow( false );
		GetDlgItem(IDC_SPELL_REPLACE)->EnableWindow( false );
		GetDlgItem(IDC_SPELL_REPLACE_ALL)->EnableWindow( false );
		GetDlgItem(IDC_SPELL_NEXT)->EnableWindow( false );
		GetDlgItem(IDC_SPELL_RESUME)->EnableWindow( true );

		CString label;
		label.LoadString(IDS_SPELL_UNEDIT);
		GetDlgItem(IDC_SPELL_IGNORE)->SetWindowText( label );
		GetDlgItem(IDC_SPELL_IGNORE)->Invalidate();
	}
}


void CSpellCheckDlg::OnSpellResume() 
{
	ASSERT( m_bEditing );
	m_bEditing = false;

	GetDlgItem(IDC_SPELL_IGNORE_ALL)->EnableWindow( true );
	GetDlgItem(IDC_SPELL_NEXT)->EnableWindow( true );
	GetDlgItem(IDC_SPELL_RESUME)->EnableWindow( false );
	CString label;
	label.LoadString(IDS_SPELL_IGNORE);
	GetDlgItem(IDC_SPELL_IGNORE)->SetWindowText( label );

	// Find the first change position
	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_SPELL_TEXT);
	CString newText;
	pEdit->GetWindowText( newText );
	int start = FirstDifference( m_pTextBuffer->GetLineChars(m_nCurLine), newText );
	if ( start != -1 )
	{
		int endOld, endNew;
		LastDifference( m_pTextBuffer->GetLineChars(m_nCurLine), endOld, newText, endNew );
		++endOld; // point one past last difference
		++endNew; // point one past last difference
		LPTSTR szNewText = newText.GetBuffer(0);
		szNewText[endNew] = _T('\0');
		m_pBuddy->HighlightText( CPoint(start, m_nCurLine), endOld-start );
		VERIFY( m_pBuddy->ReplaceSelection( &szNewText[start] ) );
		newText.ReleaseBuffer(0);
	}
	// else no change was made
	m_nCurStart = m_nCurEnd = 0;
	DoNextWord();
	OnSpellError();
}


void CSpellCheckDlg::OnSpellReplace()
{
	// Get the selected suggestion text
	CListCtrl *pList = (CListCtrl*)GetDlgItem(IDC_SPELL_SUGGEST);
	POSITION pos = pList->GetFirstSelectedItemPosition();
	if ( !pos )
		return;

	CString newText = pList->GetItemText( pList->GetNextSelectedItem(pos), 0 );

	VERIFY(m_pBuddy->ReplaceSelection( newText ));
	DoNextWord();
	OnSpellError();
}


BOOL CSpellCheckDlg::IsIgnoreWord(char *pWord)
{
	POSITION pos = m_lstIgnoreWords.GetHeadPosition();
	char *pIgnoreWord;
	while ( pos != NULL )
	{
		pIgnoreWord = (char*)m_lstIgnoreWords.GetNext( pos );
		if ( pWord != NULL && (strcmp( pWord, pIgnoreWord ) == 0) )
			return TRUE;
	}
	return FALSE;
}


void CSpellCheckDlg::OnDblclkSpellSuggest( NMHDR* pNMHDR, LRESULT* pResult ) 
{
	OnSpellReplace();
	*pResult = 0;
}
