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

#include <algorithm>

#include "CCrystalTextBuffer.h"
#include "editcmd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAttributeMenu::CAttributeMenu(CCrystalEditView *pView, const CPoint& textPoint)
{
	m_pView = pView;
	m_textPoint = textPoint;
}

CAttributeMenu::~CAttributeMenu()
{
}

BOOL CAttributeMenu::ShowSpellMenu(Speller *pSpell, const CPoint& screenPoint)
{
	CCrystalResources cr;

	CCrystalTextBuffer *pTextBuffer = m_pView->LocateTextBuffer();
	ASSERT(pTextBuffer);
	CCrystalTextBuffer::TextAttributeListType *list = pTextBuffer->GetLineAttributes(m_textPoint.y);
	if (list == NULL)
	{
		pTextBuffer->ReleaseLineAttributes();
		return FALSE;
	}
	POSITION pos = list->GetHeadPosition();
	POSITION prev = pos;
	CCrystalTextBuffer::CTextAttribute attr;
	int attrCount = 0;
	BOOL retValue = FALSE;

	while (pos != NULL)
	{
		prev = pos; // remember the position of the attribute we're working on.
		attr = list->GetNext(pos);
		if (attr.m_nStartPos <= m_textPoint.x && attr.m_nEndPos > m_textPoint.x &&
		        attr.m_Attribute == CCrystalTextBuffer::CTextAttribute::spellError)
		{
			++attrCount;
			break;
		}
		else if (attr.m_nStartPos > m_textPoint.x)
			break;
	}
	if (attrCount > 0)
	{
		m_attribute = attr;
		// Get the word
		LPCTSTR szLine = pTextBuffer->GetLineChars(m_textPoint.y);
		TCHAR* strWord = (TCHAR*)_alloca((attr.m_nEndPos - attr.m_nStartPos + 1) * sizeof(TCHAR));

		for (int i = attr.m_nStartPos; i < attr.m_nEndPos; ++i)
			strWord[i - attr.m_nStartPos] = (TCHAR)szLine[i];
		strWord[i - attr.m_nStartPos] = 0;

		// Test the word again
		if (pSpell->spell(strWord))
		{
			// Word was found, remove the attribute
			if (prev)
				list->RemoveAt(prev);
		}
		else
		{
			// FIX: Menu ID offset; context manager creates
			// a submenu for an item with the ID set to 0
			const UINT start = 1;

			// Get the suggestion list
			CStringArray aSuggestList;
			int nSuggestCount = pSpell->suggest(strWord,aSuggestList);

			if (!m_menu)
				m_menu.LoadMenu(IDR_POPUP_ATTRIBUTE);

			// Populate the menu
			CMenu* pPopup = m_menu.GetSubMenu(CCrystalTextBuffer::CTextAttribute::spellError);
			ASSERT(pPopup);
			pPopup->InsertMenu(0, MF_BYPOSITION | MF_SEPARATOR, 0, _T(""));

			if (nSuggestCount == 0)
			{
				CString noSuggestions;
				noSuggestions.LoadString(IDS_SPELL_NO_SUGGESTIONS);
				pPopup->InsertMenu(0, MF_STRING | MF_GRAYED | MF_BYPOSITION | MF_DISABLED, start, noSuggestions);
			}
			else
			{
				if (nSuggestCount > spellErrorCount)
					nSuggestCount = spellErrorCount;

				for (i = 0; i < nSuggestCount; ++i)
					pPopup->InsertMenu(i, MF_STRING | MF_ENABLED | MF_BYPOSITION, i + spellErrorStart, aSuggestList[i]);
			}

			int nSel = TrackPopupMenu(m_pView, CPoint(screenPoint.x, screenPoint.y + 10), pPopup);

			if (nSel >= spellErrorStart && nSel < spellErrorStop)
			{
				// Save the previous selection
				CPoint ptStart, ptEnd;
				m_pView->GetSelection(ptStart, ptEnd);

				// Replace the word
				m_pView->SetSelection(CPoint(m_attribute.m_nStartPos, m_textPoint.y), CPoint(m_attribute.m_nEndPos, m_textPoint.y));
				m_pView->ReplaceSelection(aSuggestList[nSel - spellErrorStart]);

				// Restore the old selection
				m_pView->SetSelection(ptStart, ptEnd);
			}
			else if (nSel == ID_EDIT_SPELL_ADD)
			{
				pSpell->add(strWord);

				if (prev)
					list->RemoveAt(prev);
			}
			else if (nSel == ID_EDIT_SPELL_IGNORE_ALL)
			{
				pSpell->ignore(strWord);

				if (prev)
					list->RemoveAt(prev);
			}
			// Trigger the line to be re-checked
			m_pView->OnEditOperation(CE_ACTION_UNKNOWN, NULL);

			retValue = TRUE;
		}
	}
	// Clean up and leave
	pTextBuffer->ReleaseLineAttributes();
	pTextBuffer->UpdateViews(NULL, NULL, UPDATE_LINEATTR, m_textPoint.y);

	return retValue;
}

int CAttributeMenu::TrackPopupMenu(CView* view, const CPoint& pt, CMenu* menu)
{
	return menu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD | TPM_NONOTIFY, pt.x, pt.y + 10, m_pView);
}
