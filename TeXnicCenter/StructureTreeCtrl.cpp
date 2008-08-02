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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// class StructureTreeCtrl
//-------------------------------------------------------------------

IMPLEMENT_DYNCREATE(StructureTreeCtrl, NavigatorTreeCtrl)

BEGIN_MESSAGE_MAP(StructureTreeCtrl, NavigatorTreeCtrl)
END_MESSAGE_MAP()


StructureTreeCtrl::StructureTreeCtrl()
{
    
}

StructureTreeCtrl::~StructureTreeCtrl()
{
}

void StructureTreeCtrl::OnUpdate(CProjectView* pSender, LPARAM lHint, LPVOID pHint)
{
    switch (lHint) {
        case COutputDoc::hintParsingFinished :
        {
            //How many items do we have in this view currently?
            //If none, i.e. first filled after loading a project, then expand the zeroth level
            const bool bExpandAll = (GetCount() == 0);

            //-----------------------------------------------------------
            // remember expanded items
            CString strSelectedItem = GetItemPath(GetSelectedItem());
            CStringArray astrExpandedItems;
            GetExpandedItems(astrExpandedItems);

            //-----------------------------------------------------------
            // fill view with parsing results
            const StructureItemContainer &a = GetProject()->m_aStructureItems;
            CArray<HTREEITEM, HTREEITEM> ahItems;

            // initialization
            ahItems.SetSize(a.size(), 0);

            //Lock
            SetRedraw(FALSE);
            //LockWindowUpdate();

            // empty view
            DeleteAllItems();

            // fill view
            //for (int i = 0; i < a.GetSize(); i++) {
            for (StructureItemContainer::const_iterator it = a.begin(); it != a.end(); ++it) {
                const CStructureItem &si = *it;//a.GetAt(i);
                const int i = std::distance(a.begin(),it);

                switch (si.GetType()) {
                    case CStructureParser::equation :
                    case CStructureParser::figure :
                    case CStructureParser::table :
                    case CStructureParser::header :
                                //case CStructureParser::unknownEnv:
                    {
                        //Better display all stuff, even without a title
                        //if (si.m_strTitle.GetLength() == 0)
                        //	break; //no title -> no display

                        if (si.m_nParent == -1)
                            ahItems[i] = InsertItem(si.GetTitle(), si.GetType(), si.GetType());
                        else
                            ahItems[i] = InsertItem(
                                si.GetTitle(), si.GetType(), si.GetType(),
                                (si.GetParent() > -1) ? ahItems[si.GetParent()] : TVI_ROOT);

                        // remember Array-Index
                        SetItemData(ahItems[i], i);
                    }
                        break;
                }
            }

            //-----------------------------------------------------------
            //try to expand items
            if (!bExpandAll) {
                ExpandItems(astrExpandedItems);
                SelectItem(GetItemByPath(strSelectedItem));
            }
            else {
                ExpandItemsByLevel(0);
                EnsureVisible(GetNextItem(NULL, TVGN_ROOT));
            }

            //Unlock
            //UnlockWindowUpdate();
            SetRedraw();
            Invalidate();
        }
            break;
    }
}
