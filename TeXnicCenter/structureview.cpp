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
*	 http://www.ToolsCenter.org
*
*********************************************************************/

/********************************************************************
*
* $Id$
*
********************************************************************/

#include "stdafx.h"
#include "TeXnicCenter.h"
#include "StructureView.h"
#include "StructureParser.h"
#include "LatexProject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// class CStructureView
//-------------------------------------------------------------------

//IMPLEMENT_DYNCREATE(CStructureView, CNavigatorView)

BEGIN_MESSAGE_MAP(CStructureView, CNavigatorView)
	//{{AFX_MSG_MAP(CStructureView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CStructureView::CStructureView()
{
}


CStructureView::~CStructureView()
{
}


void CStructureView::OnUpdate(CProjectView* pSender, LPARAM lHint, LPVOID pHint) 
{
	switch( lHint )
	{
		case COutputDoc::hintParsingFinished:
			{
				//-----------------------------------------------------------
				// remember expanded items
				CString 			strSelectedItem = GetItemPath( GetSelectedItem() );
				CStringArray	astrExpandedItems;
				GetExpandedItems( astrExpandedItems );

				//-----------------------------------------------------------
				// fill view with parsing results
				const CStructureItemArray &a = GetProject()->m_aStructureItems;
				CArray<HTREEITEM, HTREEITEM>	ahItems;

				// initialization
				ahItems.SetSize( a.GetSize(), 0 );

				// empty view
				DeleteAllItems();

				// fill view
				for( int i = 0; i < a.GetSize(); i++ )
				{
					const CStructureItem &si = a.GetAt( i );

					switch( si.m_nType )
					{
					case CStructureParser::equation:
					case CStructureParser::figure:
					case CStructureParser::table:
					case CStructureParser::header:
					//case CStructureParser::unknownEnv:
						{
							//Better display all stuff, even without a title
							//if (si.m_strTitle.GetLength() == 0)
							//	break; //no title -> no display

							if( si.m_nParent == -1 )
								ahItems[i] = InsertItem( si.m_strTitle, si.m_nType, si.m_nType );
							else
								ahItems[i] = InsertItem( 
									si.m_strTitle, si.m_nType, si.m_nType, 
									(si.m_nParent > -1)? ahItems[si.m_nParent] : TVI_ROOT );

							// remember Array-Index
							SetItemData( ahItems[i], i );
						}
						break;
					}
				}

				//-----------------------------------------------------------
				//try to expand items
				ExpandItems( astrExpandedItems );
				SelectItem( GetItemByPath( strSelectedItem ) );
			}
			break;
	}
}
