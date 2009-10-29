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

const CString FormatInput(const StructureItem& item)
{
	CString text;
	text.Format(_T("\\input{%s}"),CPathTool::GetFileTitle(item.GetTitle()));

	return text;
}

const CString FormatInclude(const StructureItem& item)
{
	CString text;
	text.Format(_T("\\include{%s}"),CPathTool::GetFileTitle(item.GetTitle()));

	return text;
}

namespace {
	const CString ExtractFileTitle(const CString& text)
	{
		return CPathTool::GetFileTitle(text);
	}
}

BEGIN_MESSAGE_MAP(CFileView,NavigatorTreeCtrl)
	//ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CFileView::OnNMCustomdraw)
	ON_WM_CREATE()
END_MESSAGE_MAP()


CFileView::CFileView()
{
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
		StructureItem::texFile,StructureItem::texFile,
		TVI_ROOT,TVI_FIRST);
	HTREEITEM hBibParent = InsertItem(AfxLoadString(STE_BIB_FILES),
		StructureItem::bibFile,StructureItem::bibFile,
		TVI_ROOT,TVI_LAST);
	HTREEITEM hGraphicParent = InsertItem(AfxLoadString(STE_GRAPHICS_FILES),
		StructureItem::graphicFile,StructureItem::graphicFile,
		TVI_ROOT,TVI_LAST);

	HTREEITEM hItem;
	CString text;
	HTREEITEM parent;

	const CString projectdir = GetProject()->GetDirectory();
	const CString maindir = CPathTool::GetDirectory(GetProject()->GetMainPath());

	// Maps the (flat) relative path and item type to a parent tree item
	typedef std::map<std::pair<CString,int>,HTREEITEM> ParentDirectoryContainerType;
	ParentDirectoryContainerType parents;

	int unique_type;

	// fill view
	for (StructureItemContainer::const_iterator it = a.begin(); it != a.end(); ++it)
	{
		const StructureItem &si = *it;
		text = CPathTool::GetFile(it->GetTitle());

		CString reldir = CPathTool::GetRelativePath(maindir,CPathTool::GetParentDirectory(
			CPathTool::GetAbsolutePath(maindir,it->GetPath())),TRUE,TRUE);

		const int i = std::distance(a.begin(),it);

		switch (si.GetType())
		{
		case StructureItem::texFile :
		case StructureItem::missingTexFile :
			unique_type = StructureItem::texFile;
			parent = hTexParent;
			break;
		case StructureItem::graphicFile :
		case StructureItem::missingGraphicFile :
			unique_type = StructureItem::graphicFile;
			parent = hGraphicParent;
			break;
		case StructureItem::bibFile :
		case StructureItem::missingBibFile :
			unique_type = StructureItem::bibFile;
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

			UINT state = 0;
			UINT flags = TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;

			if (si.IsMainProjectFile()) {
				state = TVIS_BOLD;
				flags |= TVIF_STATE;
			}

			hItem = InsertItem(flags,text,si.GetType(),si.GetType(),state,state,i,parent,TVI_SORT);
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
	MapKeyStateToFormat(MK_SHIFT,bind(ExtractFileTitle,bind(&StructureItem::GetTitle,_1)));

	return 0;
}
