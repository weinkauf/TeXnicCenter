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
#include "EnvironmentView.h"
#include "global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// CEnvironmentView
//-------------------------------------------------------------------

//IMPLEMENT_DYNCREATE(CEnvironmentView, NavigatorTreeCtrl)

BEGIN_MESSAGE_MAP(CEnvironmentView,NavigatorTreeCtrl)
	//{{AFX_MSG_MAP(CEnvironmentView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CEnvironmentView::CEnvironmentView()
{
}

CEnvironmentView::~CEnvironmentView()
{
}

void CEnvironmentView::OnUpdate(CProjectView* /*pSender*/,LPARAM lHint,LPVOID /*pHint*/) //UPDATE
{
	switch (lHint)
	{
		case COutputDoc::hintParsingFinished :
		{
			//-----------------------------------------------------------
			// remember expanded items
			CString strSelectedItem = GetItemPath(GetSelectedItem());
			CStringArray astrExpandedItems;
			GetExpandedItems(astrExpandedItems);

			//-----------------------------------------------------------
			// fill view with parsing results
			const StructureItemContainer &a = GetProject()->m_aStructureItems;

			//Lock
			LockWindowUpdate();

			// initialization
			DeleteAllItems();

			HTREEITEM ahParentItems[CStructureParser::typeCount];
			int id;

			for (int i = 0; i < CStructureParser::typeCount; i++)
			{
				switch (i)
				{
					case CStructureParser::header :
						id = STE_ITEMTYPE_HEADER;
						break;
					case CStructureParser::equation :
						id = STE_ITEMTYPE_EQUATION;
						break;
					case CStructureParser::figure :
						id = STE_ITEMTYPE_FIGURE;
						break;
					case CStructureParser::table :
						id = STE_ITEMTYPE_TABLE;
						break;
					//case CStructureParser::description:
					//	id = STE_ITEMTYPE_DESCRIPTION;
					//	break;
					//case CStructureParser::enumeration:
					//	id = STE_ITEMTYPE_ENUMERATION;
					//	break;
					//case CStructureParser::quotation:
					//	id = STE_ITEMTYPE_QUOTATION;
					//	break;
					//case CStructureParser::quote:
					//	id = STE_ITEMTYPE_QUOTE;
					//	break;
					//case CStructureParser::verse:
					//	id = STE_ITEMTYPE_VERSE;
					//	break;
					case CStructureParser::unknownEnv :
						id = STE_ITEMTYPE_UNKNOWNENV;
						break;
					default:
						id = -1;
						ahParentItems[i] = NULL;
				}

				if (id != -1)
				{
					ahParentItems[i] = InsertItem(
						                       AfxLoadString(id),
						                       CStructureParser::group,CStructureParser::group,
						                       TVI_ROOT,TVI_SORT);
					SetItemData(ahParentItems[i], static_cast<DWORD_PTR>(-1));
				}
			}

			// fill view
			//for (int i = 0; i < a.GetSize(); i++) {
			for (StructureItemContainer::const_iterator it = a.begin(); it != a.end(); ++it)
			{
				const CStructureItem &si = *it;//a.GetAt(i);

				switch (si.GetType())
				{
					case CStructureParser::header :
					case CStructureParser::equation :
					case CStructureParser::figure :
					case CStructureParser::table :
					case CStructureParser::unknownEnv :
					{
						//Better display all stuff, even without a title
						//if (si.m_strTitle.GetLength() == 0)
						//	break; //no title -> no display

						HTREEITEM hItem = InsertItem(
						                      si.GetTitle(),
						                      si.GetType(),si.GetType(),
						                      ahParentItems[si.GetType()],TVI_SORT);
						SetItemData(hItem,std::distance(a.begin(),it));
					}
					break;
				}
			}

			//-----------------------------------------------------------
			//try to expand items
			ExpandItems(astrExpandedItems);
			SelectItem(GetItemByPath(strSelectedItem));

			//Unlock
			UnlockWindowUpdate();
		}
		break;
	}
}
