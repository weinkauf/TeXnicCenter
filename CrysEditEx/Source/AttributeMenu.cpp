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

// AttributeMenu.cpp: implementation of the CAttributeMenu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttributeMenu.h"
#include "CCrystalTextBuffer.h"
#include "editcmd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAttributeMenu::CAttributeMenu( CCrystalEditView *pView, CPoint textPoint )
{
	m_pView = pView;
	m_textPoint = textPoint;

	VERIFY( m_menu.LoadMenu(IDR_POPUP_ATTRIBUTE) );
}


CAttributeMenu::~CAttributeMenu()
{
}


BOOL CAttributeMenu::ShowSpellMenu(MySpell *pSpell, CPoint screenPoint)
{
	CCrystalTextBuffer *pTextBuffer = m_pView->LocateTextBuffer();
	ASSERT( pTextBuffer );
	CCrystalTextBuffer::TextAttributeListType *list = pTextBuffer->GetLineAttributes(m_textPoint.y);
	if (list == NULL)
	{
		pTextBuffer->ReleaseLineAttributes();
		return FALSE;
	}
	POSITION pos = list->GetHeadPosition();
	CCrystalTextBuffer::CTextAttribute attr;
	int attrCount = 0;
	BOOL retValue = FALSE;
	while ( pos != NULL )
	{
		attr = list->GetNext( pos );
		if ( attr.m_nStartPos <= m_textPoint.x && attr.m_nEndPos > m_textPoint.x && 
			 attr.m_Attribute == CCrystalTextBuffer::CTextAttribute::spellError )
		{
			++attrCount;
			break;
		}
		else if (attr.m_nStartPos > m_textPoint.x)
			break;
	}
	if ( attrCount > 0 )
	{
		m_attribute = attr;
		// Get the word
		LPCTSTR szLine = pTextBuffer->GetLineChars(m_textPoint.y);
		char *strWord = (char*) _alloca((attr.m_nEndPos - attr.m_nStartPos + 1)*sizeof(char));
		for (int i = attr.m_nStartPos; i < attr.m_nEndPos; ++i)
			strWord[i - attr.m_nStartPos] = (char)szLine[i];
		strWord[i - attr.m_nStartPos] = 0;

		// Get the suggestion list
		char **aSuggestList;
		int nSuggestCount =  pSpell->suggest( &aSuggestList, strWord );

		// Populate the menu
		CMenu* pPopup = m_menu.GetSubMenu( CCrystalTextBuffer::CTextAttribute::spellError );
		ASSERT( pPopup );
		pPopup->InsertMenu( 0, MF_BYPOSITION|MF_SEPARATOR, 0, _T("") );
		if (nSuggestCount > spellErrorCount)
			nSuggestCount = spellErrorCount;
		for ( i = 0; i < nSuggestCount; ++i )
			pPopup->InsertMenu( i, MF_STRING|MF_ENABLED|MF_BYPOSITION, i+spellErrorStart, A2T(aSuggestList[i]) );

		int nSel = pPopup->TrackPopupMenu( TPM_LEFTALIGN|TPM_RIGHTBUTTON|TPM_RETURNCMD|TPM_NONOTIFY, screenPoint.x, screenPoint.y, m_pView );
		if ( nSel >= spellErrorStart && nSel < spellErrorStop )
		{
			// Save the previous selection
			CPoint ptStart, ptEnd;
			m_pView->GetSelection(ptStart, ptEnd);

			// Replace the word
			m_pView->SetSelection( CPoint(m_attribute.m_nStartPos, m_textPoint.y), CPoint(m_attribute.m_nEndPos, m_textPoint.y) );
			m_pView->ReplaceSelection(aSuggestList[nSel-spellErrorStart]);

			// Restore the old selection
			m_pView->SetSelection(ptStart, ptEnd);
		}
		else if ( nSel == ID_EDIT_SPELL_ADD )
		{
			ASSERT(IDOK == AfxMessageBox(_T("Sorry, not implemented yet"), IDOK));
		}
		else if ( nSel == ID_EDIT_SPELL_IGNORE_ALL )
		{
			ASSERT(IDOK == AfxMessageBox(_T("Sorry, not implemented yet"), IDOK));
		}
		if ( nSuggestCount > 0 )
			pSpell->release_suggest( aSuggestList );
		retValue = TRUE;
	}
	// Clean up and leave
	pTextBuffer->ReleaseLineAttributes();
	return retValue;
}

