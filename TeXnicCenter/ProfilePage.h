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

#if !defined(AFX_PROFILEPAGE_H__AF58958E_0A74_11D5_A222_006097239934__INCLUDED_)
#define AFX_PROFILEPAGE_H__AF58958E_0A74_11D5_A222_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PropertyDialog.h"
#include "Profile.h"

class CProfilePage : public CPropertyDialogPage
{
// construction/destruction
public:
	CProfilePage(UINT unIDTemplate);

// operations
public:
	virtual void OnUpdateDataSet(CProfile *pProfile) = 0;
	virtual BOOL ApplyChanges();
	virtual BOOL OnApply();

// implementations
protected:
	/**
	Enables or disables all controls of the page.
	*/
	void EnableAllControls(BOOL bEnableAllControls = TRUE);

// overridings
protected:
	//{{AFX_VIRTUAL(CProfilePage)
	//}}AFX_VIRTUAL

// message handlers
protected:
	//{{AFX_MSG(CProfilePage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_PROFILEPAGE_H__AF58958E_0A74_11D5_A222_006097239934__INCLUDED_
