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
#include "FileView.h"
#include "global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// class CFileView
//-------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CFileView, CNavigatorView)
	//{{AFX_MSG_MAP(CFileView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CFileView::CFileView()
{
}


CFileView::~CFileView()
{
}


void CFileView::OnUpdate(CProjectView* pSender, LPARAM lHint, LPVOID pHint) 
{
	switch( lHint )
	{
		case COutputDoc::hintParsingFinished:
			{
				//-----------------------------------------------------------
				// remember expanded items
				CString strSelectedItem = GetItemPath(GetSelectedItem());
				CStringArray astrExpandedItems;
				GetExpandedItems(astrExpandedItems);

				//-----------------------------------------------------------
				// fill tree
				CStructureItemArray	&a = GetProject()->m_aStructureItems;

				// initialization
				DeleteAllItems();

				HTREEITEM hTexParent = InsertItem(AfxLoadString(STE_TEX_FILES), 
					CStructureParser::texFile, CStructureParser::texFile, 
					TVI_ROOT, TVI_SORT);
				HTREEITEM hBibParent = InsertItem(AfxLoadString(STE_BIB_FILES), 
					CStructureParser::bibFile, CStructureParser::bibFile, 
					TVI_ROOT, TVI_SORT);
				HTREEITEM hGaphicParent = InsertItem(AfxLoadString(STE_GRAPHICS_FILES), 
					CStructureParser::graphicFile, CStructureParser::graphicFile, 
					TVI_ROOT, TVI_SORT);

				HTREEITEM hItem;
				// fill view
				for( int i = 0; i < a.GetSize(); i++ )
				{
					CStructureItem	&si = a.GetAt( i );

					switch( si.m_nType )
					{
					case CStructureParser::texFile:
						hItem = InsertItem( 
							si.m_strPath, 
							si.m_nType, si.m_nType, 
							hTexParent, TVI_SORT );
						SetItemData( hItem, i );
						break;
					case CStructureParser::graphicFile:
						hItem = InsertItem( 
							si.m_strPath, 
							si.m_nType, si.m_nType, 
							hGaphicParent, TVI_SORT );
						SetItemData( hItem, i );
						break;
					case CStructureParser::bibFile:
						hItem = InsertItem( 
							si.m_strPath, 
							si.m_nType, si.m_nType, 
							hBibParent, TVI_SORT );
						SetItemData( hItem, i );
						break;
					}
				}

				//-----------------------------------------------------------
				//try to expand items
				ExpandItems(astrExpandedItems);
				SelectItem(GetItemByPath(strSelectedItem));
			}
			break;
	}
}
