/********************************************************************
*
* This file is part of the TeXnicCenter-system
*
* Copyright (C) 1999-2000 Sven Wiegand
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
#include "TeXnicCenter.h"
#include "CrystalEditViewEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// class CCrystalEditViewEx
//-------------------------------------------------------------------

IMPLEMENT_DYNCREATE(CCrystalEditViewEx, CCrystalEditView)


BEGIN_MESSAGE_MAP(CCrystalEditViewEx, CCrystalEditView)
	//{{AFX_MSG_MAP(CCrystalEditViewEx)
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CCrystalEditViewEx::CCrystalEditViewEx()
:	CCrystalEditView()
{}


#define VAL_VIEWINFO_CURSOR_ROW					_T("Cursor.row")
#define VAL_VIEWINFO_CURSOR_COLUMN			_T("Cursor.column")
#define VAL_VIEWINFO_TOPSUBLINE					_T("TopSubLine")

BOOL CCrystalEditViewEx::Serialize(CIniFile &ini, LPCTSTR lpszKey, BOOL bWrite)
{
	if (bWrite)
	{
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// store information
		//ini.SetValue(lpszKey, VAL_VIEWINFO_DOCPATH, GetDocument()->GetPathName());
		ini.SetValue(lpszKey, VAL_VIEWINFO_CURSOR_ROW, GetCursorPos().y);
		ini.SetValue(lpszKey, VAL_VIEWINFO_CURSOR_COLUMN, GetCursorPos().x);
		ini.SetValue(lpszKey, VAL_VIEWINFO_TOPSUBLINE, m_nTopSubLine);
	}
	else
	{
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// read information
		// set cursor position
		CPoint	ptCursor;

		ptCursor.y = ini.GetValue(lpszKey, VAL_VIEWINFO_CURSOR_ROW, 0);
		ptCursor.x = ini.GetValue(lpszKey, VAL_VIEWINFO_CURSOR_COLUMN, 0);

		try
		{
			if (ptCursor.x < 0 || ptCursor.y < 0)
				throw FALSE;
			if (ptCursor.y >= GetLineCount())
				throw FALSE;
			if (ptCursor.x >= GetLineLength(ptCursor.y))
				throw FALSE;

			SetCursorPos(ptCursor);
		}
		catch(...)
		{}

		// scroll to right position
		int	nSubLineIndex = ini.GetValue(lpszKey, VAL_VIEWINFO_TOPSUBLINE, 0);
		if (nSubLineIndex >= 0 && nSubLineIndex < GetSubLineCount())
			ScrollToSubLine(nSubLineIndex);
	}

	return TRUE;
}


BOOL CCrystalEditViewEx::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	int nScrollTo = m_nTopSubLine;

	nScrollTo += (zDelta < 0) ? 2 : -2;

	if (nScrollTo < 0)
		nScrollTo = 0;
	if (nScrollTo < GetSubLineCount())
		ScrollToSubLine(nScrollTo);

	return TRUE;
}
