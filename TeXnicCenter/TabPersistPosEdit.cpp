/********************************************************************
*
* This file is part of the TeXnicCenter-system
*
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

/********************************************************************
*
* $Id$
*
********************************************************************/

#include "stdafx.h"
#include "TabPersistPosEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CTabPersistPosEdit::CTabPersistPosEdit()
{
}

CTabPersistPosEdit::~CTabPersistPosEdit()
{
}


BEGIN_MESSAGE_MAP(CTabPersistPosEdit, CPersistPosEdit)
	//{{AFX_MSG_MAP(CTabPersistPosEdit)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabPersistPosEdit message handlers

void CTabPersistPosEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	//Retrieve the TAB-key and insert tabs
	if (nChar == VK_TAB)
	{
		//NOTE: We totally ignore the nRepCnt,
		// because it is too difficult to implement
		// in the case of removing tabs.
		// However, for inserting it would be easy.
		// But lets stay consistent.

		//Undo the automatic loosing of the focus.
		SetFocus();

		//Insert or deleted trailing tabs?
		// - Get key state of Shift key (MSB == 0/1)
		bool bInsertTabs = (GetKeyState(VK_SHIFT) >= 0);

		//Get Selection
		int SelStart = 0;
		int SelEnd = 0;
		GetSel(SelStart, SelEnd);
		ASSERT(SelStart <= SelEnd);

		//Lines that contribute to the selection - at least partly
		int LineSelStart = LineFromChar(SelStart);
		int LineSelEnd = LineFromChar(SelEnd);
		ASSERT(LineSelStart <= LineSelEnd);
		int nLines = LineSelEnd - LineSelStart + 1;

		//If we have just some selected chars in one line,
		// then we replace the current selection.
//		//But not, if this line is fully selected.
//		bool bOneLineFullySelected = (LineIndex(LineSelStart) == SelStart)
//																	&& (LineIndex(LineSelStart) + LineLength(SelStart) == SelEnd)
//																	&& (SelStart != SelEnd);
		if (nLines == 1)// && !bOneLineFullySelected)
		{
			if (bInsertTabs) ReplaceSel(_T("\t"), true);
			return;
		}

		//Is the end of the selection at the start of a new line?
		//In this case, we do not treat the last line as being selected.
		if (LineIndex(LineSelEnd) == SelEnd)
		{
			nLines--;
			LineSelEnd--;
		}

		//If we have more selected lines, /*or one fully selected line*/
		// then we intend all those lines and grow the selection.
		//So lets retrieve the text and add some tabs.
		CString Text;
		GetWindowText(Text);

		// - Insert/delete the tabs at the beginning of each line.
		int nTabsRemoved = 0;
		for(int i=0;i<nLines;i++)
		{
			//Insert or delete?
			if (bInsertTabs)
			{
				Text.Insert(LineIndex(LineSelStart + i) + i, _T('\t'));
			}
			else
			{
				//Do we have tabs at the beginning?
				if (Text[LineIndex(LineSelStart + i) - nTabsRemoved] == _T('\t'))
				{
					Text.Delete(LineIndex(LineSelStart + i) - nTabsRemoved);
					nTabsRemoved++;
				}

			}
		}

		// - Set the new text and make a good selection
		SetWindowText(Text);
		SetSel(LineIndex(LineSelStart),
						LineIndex(LineSelEnd) + LineLength(LineIndex(LineSelEnd)) + 2);

		return;
	}

	CPersistPosEdit::OnChar(nChar, nRepCnt, nFlags);
}


