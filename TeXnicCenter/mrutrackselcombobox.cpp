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
#include "MRUTrackSelComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// class CMRUTrackSelComboBox
//-------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CMRUTrackSelComboBox,CComboBox)
	//{{AFX_MSG_MAP(CMRUTrackSelComboBox)
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CMRUTrackSelComboBox::CMRUTrackSelComboBox()
		: CMRUComboBox(),
		m_nRecentEditSelStart(0),
		m_nRecentEditSelEnd(0)
{
}

CMRUTrackSelComboBox::CMRUTrackSelComboBox(
    LPCTSTR lpszRegKey,LPCTSTR lpszValueFormat,int nMaxMRUSize /*= MRUC_DEFAULT_MRU_SIZE*/,
    BOOL bAutoLoadOnConstruction /*= TRUE*/,
    BOOL bAutoSaveOnDestroy /*= TRUE*/,BOOL bAutoSaveAfterAdd /*= FALSE*/,BOOL bAutoRefresh /*= FALSE*/)

		: CMRUComboBox(lpszRegKey,lpszValueFormat,nMaxMRUSize,bAutoLoadOnConstruction,bAutoSaveOnDestroy,bAutoSaveAfterAdd,bAutoRefresh),
		m_nRecentEditSelStart(0),
		m_nRecentEditSelEnd(0)
{
}

void CMRUTrackSelComboBox::OnLButtonUp(UINT nFlags,CPoint point)
{
	DWORD dwEditSel = GetEditSel();
	m_nRecentEditSelStart = LOWORD(dwEditSel);
	m_nRecentEditSelEnd = HIWORD(dwEditSel);

	CComboBox::OnLButtonUp(nFlags,point);
}
