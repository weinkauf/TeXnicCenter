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

/********************************************************************
 *
 * $Id$
 *
 ********************************************************************/

#if !defined(AFX_PROJECTVIEW_H__980BE3C5_25FC_11D5_A222_006097239934__INCLUDED_)
#define AFX_PROJECTVIEW_H__980BE3C5_25FC_11D5_A222_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// forwards
class CProject;

/**
Interface to be implemented by all classes, that want to be views
of a CProject-object.

@author Sven Wiegand
 */
class CProjectView
{
// construction/destruction
public:
	CProjectView();
	virtual ~CProjectView();

// operations
public:
	CProject *GetProject() const;
	bool IsAttached() const;

// overridables
protected:
	virtual void OnUpdate(CProjectView *pSender, LPARAM lHint, LPVOID pHint);
	virtual void OnClosingProject();

// attributes
private:
	CProject *m_pProject;

// friends
private:
	friend CProject;
public:
	virtual void Clear() = 0;
	
protected:
	virtual void OnOpeningProject();
};


// inlines

inline
CProject *CProjectView::GetProject() const
{
	return m_pProject;
}

#endif // !defined(AFX_PROJECTVIEW_H__980BE3C5_25FC_11D5_A222_006097239934__INCLUDED_)
