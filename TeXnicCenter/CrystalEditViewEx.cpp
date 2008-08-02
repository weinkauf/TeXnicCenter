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

/********************************************************************
 *
 * $Id$
 *
 ********************************************************************/

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

IMPLEMENT_DYNCREATE(CCrystalEditViewEx,CCrystalEditView)


CCrystalEditViewEx::CCrystalEditViewEx()
: CCrystalEditView()
{
}


LPCTSTR const VAL_VIEWINFO_CURSOR_ROW = _T("Cursor.row");
LPCTSTR const VAL_VIEWINFO_CURSOR_COLUMN = _T("Cursor.column");
LPCTSTR const VAL_VIEWINFO_TOPSUBLINE = _T("TopSubLine");

BOOL CCrystalEditViewEx::Serialize(CIniFile &ini,LPCTSTR lpszKey,BOOL bWrite)
{
    if (bWrite) {
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // store information
        //ini.SetValue(lpszKey, VAL_VIEWINFO_DOCPATH, GetDocument()->GetPathName());
        ini.SetValue(lpszKey,VAL_VIEWINFO_CURSOR_ROW,GetCursorPos().y);
        ini.SetValue(lpszKey,VAL_VIEWINFO_CURSOR_COLUMN,GetCursorPos().x);
        ini.SetValue(lpszKey,VAL_VIEWINFO_TOPSUBLINE,m_nTopSubLine);
    }
    else {
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // read information
        // set cursor position
        CPoint ptCursor;

        ptCursor.y = ini.GetValue(lpszKey,VAL_VIEWINFO_CURSOR_ROW,0);
        ptCursor.x = ini.GetValue(lpszKey,VAL_VIEWINFO_CURSOR_COLUMN,0);

        try {
            if (ptCursor.x < 0 || ptCursor.y < 0)
                throw FALSE;
            if (ptCursor.y >= GetLineCount())
                throw FALSE;
            if (ptCursor.x >= GetLineLength(ptCursor.y) && ptCursor.x)
                throw FALSE;

            SetCursorPos(ptCursor);
            SetAnchor(ptCursor);
            SetSelection(ptCursor,ptCursor);
        }
        catch (...) {
            TRACE("Invalid cursor position: %s line %d\n",__FILE__,__LINE__);
        }

        // scroll to right position
        int nSubLineIndex = ini.GetValue(lpszKey,VAL_VIEWINFO_TOPSUBLINE,0);

        if (nSubLineIndex >= 0 && nSubLineIndex < GetSubLineCount())
            ScrollToSubLine(nSubLineIndex);
    }

    return TRUE;
}

