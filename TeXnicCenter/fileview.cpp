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
#include "FileView.h"
#include "FontOccManager.h"
#include "global.h"

BEGIN_MESSAGE_MAP(CFileView,NavigatorTreeCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CFileView::OnNMCustomdraw)
END_MESSAGE_MAP()


CFileView::CFileView()
{
	LOGFONT lf = GetDisplayFont();
	lf.lfWeight = FW_BOLD;
	bold_.CreateFontIndirect(&lf);
}

CFileView::~CFileView()
{
}

void CFileView::OnUpdate(CProjectView* /*pSender*/, LPARAM lHint, LPVOID /*pHint*/)
{
	switch (lHint)
	{
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
			// fill tree
			const StructureItemContainer &a = GetProject()->m_aStructureItems;

			//Lock
			//LockWindowUpdate();
			SetRedraw(FALSE);

			// initialization
			DeleteAllItems();

			HTREEITEM hTexParent = InsertItem(AfxLoadString(STE_TEX_FILES),
			                                  CStructureParser::texFile,CStructureParser::texFile,
			                                  TVI_ROOT,TVI_FIRST);
			HTREEITEM hBibParent = InsertItem(AfxLoadString(STE_BIB_FILES),
			                                  CStructureParser::bibFile,CStructureParser::bibFile,
			                                  TVI_ROOT,TVI_LAST);
			HTREEITEM hGraphicParent = InsertItem(AfxLoadString(STE_GRAPHICS_FILES),
			                                      CStructureParser::graphicFile,CStructureParser::graphicFile,
			                                      TVI_ROOT,TVI_LAST);

			HTREEITEM hItem;
			CString text;

			// fill view
			//for (int i = 0; i < a.GetSize(); i++) {
			for (StructureItemContainer::const_iterator it = a.begin(); it != a.end(); ++it)
			{
				const CStructureItem &si = *it;//a.GetAt(i);

				if (it->IsMainProjectFile())
					text = CPathTool::GetFile(it->/*a[i]*/m_strTitle);
				else
					text = it->m_strTitle;

				const int i = std::distance(a.begin(),it);

				switch (si.m_nType)
				{
					case CStructureParser::texFile :
					case CStructureParser::missingTexFile :
						hItem = InsertItem(
						            text,
						            si.GetType(),si.GetType(),
						            hTexParent,TVI_SORT);
						SetItemData(hItem,i);
						break;
					case CStructureParser::graphicFile :
					case CStructureParser::missingGraphicFile :
						hItem = InsertItem(
						            text,
						            si.GetType(),si.GetType(),
						            hGraphicParent,TVI_SORT);
						SetItemData(hItem,i);
						break;
					case CStructureParser::bibFile :
					case CStructureParser::missingBibFile :
						hItem = InsertItem(
						            text,
						            si.GetType(),si.GetType(),
						            hBibParent,TVI_SORT);
						SetItemData(hItem,i);
						break;
				}
			}

			//-----------------------------------------------------------
			//try to expand items
			if (!bExpandAll)
			{
				ExpandItems(astrExpandedItems);
				SelectItem(GetItemByPath(strSelectedItem));
			}
			else
			{
				ExpandItemsByLevel(0);
				EnsureVisible(GetNextItem(NULL,TVGN_ROOT));
			}

			//Unlock
			//UnlockWindowUpdate();
			SetRedraw();
			Invalidate();
		}
		break;
	}
}

void CFileView::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	switch (pNMCD->dwDrawStage)
	{
		case CDDS_PREPAINT:
			*pResult = CDRF_NOTIFYITEMDRAW;
			break;
		case CDDS_ITEMPREPAINT:
			if (!pNMCD->lItemlParam && GetParentItem(reinterpret_cast<HTREEITEM>(pNMCD->dwItemSpec)))
			{
				::SelectObject(pNMCD->hdc,bold_.m_hObject);
				*pResult = CDRF_NEWFONT;
				break;
			}
		default:
			*pResult = 0;
	}
}

BOOL CFileView::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (message == WM_SETFONT)
	{
		bold_.DeleteObject();

		LOGFONT lf;

		if (wParam)
		{
			CFont font;

			font.Attach(reinterpret_cast<HFONT>(wParam));
			font.GetLogFont(&lf);
			font.Detach();
		}
		else // Default system font
			lf = GetDisplayFont();

		lf.lfWeight = FW_BOLD;

		bold_.CreateFontIndirect(&lf);
	}

	return NavigatorTreeCtrl::OnWndMsg(message, wParam, lParam, pResult);
}

bool CFileView::OnBeginDragDrop(const CStructureItem& item, CString& text, UINT keystate)
{
	switch (keystate)
	{
		case 0:
			text.Format(_T("\\input{%s}"),CPathTool::GetFileTitle(item.GetTitle()));
			break;
		case MK_CONTROL:
			text.Format(_T("\\include{%s}"),CPathTool::GetFileTitle(item.GetTitle()));
			break;
	}

	static_cast<CFrameWnd*>(AfxGetMainWnd())->SetMessageText(text);

	return !text.IsEmpty();
}
