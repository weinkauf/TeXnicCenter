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
 * $Id: structureview.cpp,v 1.11 2005/07/03 16:49:35 niteria Exp $
 *
 ********************************************************************/

#include "stdafx.h"
#include "TeXnicCenter.h"
#include "StructureTreeCtrl.h"
#include "StructureParser.h"
#include "LatexProject.h"
#include "FontOccManager.h"


IMPLEMENT_DYNCREATE(StructureTreeCtrl, NavigatorTreeCtrl)

BEGIN_MESSAGE_MAP(StructureTreeCtrl, NavigatorTreeCtrl)
END_MESSAGE_MAP()

void StructureTreeCtrl::OnUpdate(CProjectView* /*pSender*/, LPARAM lHint, LPVOID /*pHint*/)
{
	switch (lHint)
	{
		case COutputDoc::hintParsingFinished :
		{
			OnParsingFinished();
		}
		break;
	}
}

void StructureTreeCtrl::OnParsingFinished()
{
	//How many items do we have in this view currently?
	//If none, i.e. first filled after loading a project, then expand the zeroth level
	const bool bExpandAll = (GetCount() == 0);

	// remember expanded items
	CString strSelectedItem = GetItemPath(GetSelectedItem());
	CStringArray astrExpandedItems;
	GetExpandedItems(astrExpandedItems);

	// fill view with parsing results
	const StructureItemContainer &a = GetProject()->GetStructureItems();
	CArray<HTREEITEM, HTREEITEM> ahItems;

	// initialization
	ahItems.SetSize(a.size(), 0);

	//Lock
	SetRedraw(FALSE);
	// empty view
	DeleteAllItems();

	// fill view
	for (StructureItemContainer::const_iterator it = a.begin(); it != a.end(); ++it)
	{
		const CStructureItem &si = *it;//a.GetAt(i);
		const int i = std::distance(a.begin(),it);

		switch (si.GetType())
		{
		case CStructureParser::equation :
		case CStructureParser::figure :
		case CStructureParser::table :
		case CStructureParser::header :
		case CStructureParser::bibFile:
			//case CStructureParser::unknownEnv:
			{
				//Better display all stuff, even without a title
				//if (si.m_strTitle.GetLength() == 0)
				//	break; //no title -> no display

				CString title = si.GetTitle();
				int parent = si.GetParent();

				if (si.GetType() == CStructureParser::header)
					title.Replace(_T("\\-"),_T("")); // Cleanup \-
				else if (si.GetType() == CStructureParser::bibFile) {
					title.Format(IDS_BIBLIOGRAPHY,si.GetPath());
					parent = -1; // Bibliography items are always root nodes
				}

				if (parent <= -1)
					ahItems[i] = InsertItem(title, si.GetType(), si.GetType());
				else
					ahItems[i] = InsertItem(title, si.GetType(), si.GetType(), ahItems[parent]);

				// remember Array-Index
				SetItemData(ahItems[i], i);
			}
			break;
		}
	}

	//try to expand items
	if (!bExpandAll)
	{
		ExpandItems(astrExpandedItems);
		SelectItem(GetItemByPath(strSelectedItem));
	}
	else
	{
		ExpandItemsByLevel(0);
		EnsureVisible(GetNextItem(NULL, TVGN_ROOT));
	}

	//Unlock
	SetRedraw();
	Invalidate();
}

bool StructureTreeCtrl::IsFolder( HTREEITEM ) const
{
	// No folders here
	return false;
}