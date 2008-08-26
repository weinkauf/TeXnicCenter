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

#include <map>
#include <utility>

#include "FileView.h"
#include "FontOccManager.h"
#include "global.h"
#include "OleDrop.h"


BEGIN_MESSAGE_MAP(CFileView,NavigatorTreeCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CFileView::OnNMCustomdraw)
	ON_WM_CREATE()
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
			OnParsingFinished();
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

void CFileView::OnParsingFinished()
{
	//How many items do we have in this view currently?
	//If none, i.e. first filled after loading a project, then expand the zeroth level
	const bool bExpandAll = (GetCount() == 0);

	// remember expanded items
	CString strSelectedItem = GetItemPath(GetSelectedItem());
	CStringArray astrExpandedItems;
	GetExpandedItems(astrExpandedItems);

	// fill tree
	const StructureItemContainer &a = GetProject()->m_aStructureItems;

	SetRedraw(FALSE);
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
	HTREEITEM parent;

	const CString projectdir = GetProject()->GetProjectDir();

	// Maps the (flat) relative path and item type to a parent tree item
	typedef std::map<std::pair<CString,int>,HTREEITEM> ParentDirectoryContainerType;
	ParentDirectoryContainerType parents;

	int unique_type;

	// fill view
	for (StructureItemContainer::const_iterator it = a.begin(); it != a.end(); ++it)
	{
		const CStructureItem &si = *it;
		text = CPathTool::GetFile(it->GetTitle());

		CString reldir = CPathTool::GetRelativePath(projectdir,CPathTool::GetParentDirectory(
			CPathTool::GetAbsolutePath(projectdir,it->GetPath())),TRUE,TRUE);

		const int i = std::distance(a.begin(),it);

		switch (si.GetType())
		{
		case CStructureParser::texFile :
		case CStructureParser::missingTexFile :
			unique_type = CStructureParser::texFile;
			parent = hTexParent;
			break;
		case CStructureParser::graphicFile :
		case CStructureParser::missingGraphicFile :
			unique_type = CStructureParser::graphicFile;
			parent = hGraphicParent;
			break;
		case CStructureParser::bibFile :
		case CStructureParser::missingBibFile :
			unique_type = CStructureParser::bibFile;
			parent = hBibParent;
			break;
		default:
			parent = 0;
		}

		if (parent)
		{
			if (reldir != _T(".")) 
			{
				ParentDirectoryContainerType::iterator it = 
					parents.find(std::make_pair(reldir,unique_type));

				if (it != parents.end())
					parent = it->second;
				else 
				{
					CString component, path;
					int index;

					LPCTSTR const sep = _T("\\/");
					bool stop = false;

					const int folder_image = 14;

					while (!stop)
					{
						index = reldir.FindOneOf(sep);							

						if (index == -1) 
						{
							index = reldir.GetLength();
							stop = true;
						}

						component = reldir.Left(index);
						path += component;

						it = parents.find(std::make_pair(path,unique_type));

						if (it != parents.end())
							parent = it->second;
						else
						{
							// We insert the current path component only
							parent = InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,
								component,folder_image,folder_image,0,0,-1,parent,TVI_SORT);
							// but store the whole relative path
							parents.insert(std::make_pair(std::make_pair(path,unique_type),parent));
						}

						if (!stop) 
						{
							path += reldir[index]; // add the \ or /
							reldir.Delete(0,index + 1);
						}
					}
				}						
			}

			hItem = InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,text,si.GetType(),
				si.GetType(),0,0,i,parent,TVI_SORT);
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
		EnsureVisible(GetNextItem(NULL,TVGN_ROOT));
	}

	SetRedraw();
	Invalidate();
}

const CString FormatInput(const CStructureItem& item)
{
	CString text;
	text.Format(_T("\\input{%s}"),CPathTool::GetFileTitle(item.GetTitle()));

	return text;
}

const CString FormatInclude(const CStructureItem& item)
{
	CString text;
	text.Format(_T("\\include{%s}"),CPathTool::GetFileTitle(item.GetTitle()));

	return text;
}

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (NavigatorTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	ClearKeyStateToFormat();
	ClearKeyStateToMessage();

	using namespace std::tr1;
	using namespace placeholders;

	MapKeyStateToFormat(0,FormatInput);
	MapKeyStateToFormat(MK_CONTROL,FormatInclude);
	MapKeyStateToFormat(MK_SHIFT,bind(&CStructureItem::GetTitle,_1));

	return 0;
}
