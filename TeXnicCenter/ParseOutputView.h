/********************************************************************
*
* This file is part of the TeXnicCenter-system
*
* Copyright (C) 2002 Chris Norris
* Copyright (C) 2002-$CurrentYear$ ToolsCenter
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

// ParseOutputView.h: interface for the CParseOutputView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARSEOUTPUTVIEW_H__4CEB94E9_8137_4E0D_ABC4_A97B9BA5EAA0__INCLUDED_)
#define AFX_PARSEOUTPUTVIEW_H__4CEB94E9_8137_4E0D_ABC4_A97B9BA5EAA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OutputView.h"

class CParseOutputView : public COutputView  
{
	DECLARE_DYNCREATE(CParseOutputView)

// construction/destruction
public:
	CParseOutputView();
	virtual ~CParseOutputView();

	enum tagImage
	{
		imageNone = 0,
		imageError,
		imageWarning,
		imageInfo
	};

	void OnUpdate(COutputView* pSender, LPARAM lHint, CObject* pHint); 	//UPDATE

	//{{AFX_VIRTUAL(CParseOutputView)
	protected:
	//}}AFX_VIRTUAL

#ifdef _DEBUG
	void AssertValid() const;
	void Dump(CDumpContext& dc) const;
#endif //_DEBUG

// message handlers
protected:
	//{{AFX_MSG(CParseOutputView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	/** images to use in this view */
	CImageList m_images;
};

#endif // !defined(AFX_PARSEOUTPUTVIEW_H__4CEB94E9_8137_4E0D_ABC4_A97B9BA5EAA0__INCLUDED_)
