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

#pragma once

#include "NavigatorView.h"
#include "StructureItem.h"

class StructureTreeCtrl :
			public NavigatorTreeCtrl
{
	DECLARE_DYNCREATE(StructureTreeCtrl)

protected:
	virtual void OnUpdate(CProjectView* pSender,LPARAM lHint,LPVOID pHint);
	void GotoItem(HTREEITEM item);
	bool ShouldBeExpanded(HTREEITEM item) const;

	DECLARE_MESSAGE_MAP()

private:
	void OnParsingFinished();
	LRESULT OnGotoBibliographyLine(WPARAM w, LPARAM l);
	void InsertBibItems(const StructureItem& item, StructureItemContainer::difference_type index, HTREEITEM parentItem);

public:
	bool IsFolder(HTREEITEM) const;
};
