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

#if !defined(AFX_BUILDVIEW_H__0EE022A6_20D1_11D3_929E_DA6C20A3421F__INCLUDED_)
#define AFX_BUILDVIEW_H__0EE022A6_20D1_11D3_929E_DA6C20A3421F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OutputView.h"

class CBuildView : public COutputView {
    DECLARE_DYNCREATE(CBuildView)

    // construction/destruction
public:
    CBuildView();
    virtual ~CBuildView();

    // operations
public:

    // constants
public:

    enum tagImage {
        imageNone = 0,
        imageError,
        imageWarning,
        imageBadBox
    };

    // overridings
protected:
    virtual void OnUpdate(COutputView* pSender, LPARAM lHint, CObject* pHint);

    //{{AFX_VIRTUAL(CBuildView)
protected:
    //}}AFX_VIRTUAL

    // debugging
protected:
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    // message handlers
protected:
    //{{AFX_MSG(CBuildView)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

    // attributes
protected:
    /** images to use in this view */
    CImageList m_images;
public:
    void Visit(OutputViewVisitor& v);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_BUILDVIEW_H__0EE022A6_20D1_11D3_929E_DA6C20A3421F__INCLUDED_
