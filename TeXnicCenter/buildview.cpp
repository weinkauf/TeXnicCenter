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
#include "BuildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// class CBuildView
//-------------------------------------------------------------------

IMPLEMENT_DYNCREATE(CBuildView,COutputView)

BEGIN_MESSAGE_MAP(CBuildView,COutputView)
	//{{AFX_MSG_MAP(CBuildView)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CBuildView::CBuildView()
{
}

CBuildView::~CBuildView()
{
}

void CBuildView::OnUpdate(COutputView* pSender,LPARAM lHint,CObject* pHint)
{
	if (pSender == this)
		return;

	switch ((COutputDoc::HINT)lHint) //UPDATE
	{
		case COutputDoc::hintSelectBuildLine :
			SelectLine(*(int*) pHint,TRUE);
			break;
	}
}


#ifdef _DEBUG

void CBuildView::AssertValid() const
{
	COutputView::AssertValid();
}

void CBuildView::Dump(CDumpContext& dc) const
{
	COutputView::Dump(dc);
}
#endif //_DEBUG

int CBuildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COutputView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_images.m_hImageList = ::ImageList_LoadImage(AfxGetResourceHandle(),
	                        MAKEINTRESOURCE(IDB_PARSE_VIEW),16,1,RGB(255,0,255),IMAGE_BITMAP,
	                        LR_CREATEDIBSECTION);

	SetImageList(&m_images);

	return 0;
}

void CBuildView::Visit(OutputViewVisitor& v)
{
	v.Accept(this);
}
