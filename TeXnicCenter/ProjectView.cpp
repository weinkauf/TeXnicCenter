/********************************************************************
 *
 * This file is part of the TeXnicCenter-system
 *
 * Copyright (C) 1999-2000 Sven Wiegand
 * Copyright (C) 2000-$CurrentYear$ ToolsCenter
 *
 * This program is free software you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * If you have further questions or if you want to support
 * further TeXnicCenter development, visit the TeXnicCenter-homepage
 *
 *    http://www.ToolsCenter.org
 *
 *********************************************************************/

#include "stdafx.h"
#include "stdafx.h"
#include "ProjectView.h"
#include "Project.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//-------------------------------------------------------------------
// class CProjectView
//-------------------------------------------------------------------

CProjectView::CProjectView()
		: m_pProject(NULL)
{
}

CProjectView::~CProjectView()
{
	ASSERT(!IsAttached());
	// Warning: Do not call RemoveView here which otherwise will result in a 
	// pure virtual function call of the Clear method
}

void CProjectView::OnUpdate(CProjectView *pSender, LPARAM lHint, LPVOID pHint)
{
	UNUSED_ALWAYS(pSender);
	UNUSED_ALWAYS(lHint);
	UNUSED_ALWAYS(pHint);
}

void CProjectView::OnClosingProject()
{
}

void CProjectView::OnOpeningProject()
{
}

bool CProjectView::IsAttached() const
{
	return m_pProject != 0;
}