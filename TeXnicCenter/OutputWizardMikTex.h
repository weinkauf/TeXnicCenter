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

#if !defined(AFX_OUTPUTWIZARDMIKTEX_H__627C2BE4_5B16_11D4_A222_006097239934__INCLUDED_)
#define AFX_OUTPUTWIZARDMIKTEX_H__627C2BE4_5B16_11D4_A222_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OutputWizardPage.h"

/**
This page belongs to the output wizard (COutputWizard) and informs
the user, that MiKTeX has been found on his system. The user then
can choose, if TeXnicCenter should be configured for the use with
MiKTeX or not.

@ingroup dialogs
@ingroup profiles

@author Sven Wiegand
 */
class COutputWizardMiKTeX : public OutputWizardPage
{
	DECLARE_DYNCREATE(COutputWizardMiKTeX)

public:
	COutputWizardMiKTeX(COutputWizard* w = 0);
	virtual ~COutputWizardMiKTeX();

// Overridings
protected:
//{{AFX_VIRTUAL(COutputWizardMiKTeX)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Message Handlers
protected:
	//{{AFX_MSG(COutputWizardMiKTeX)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

// Constants
public:
	enum tagChoice
	{
		yes = 0,
		no
	};

// Dialog Data
public:
	//{{AFX_DATA(COutputWizardMiKTeX)

	enum
	{
		IDD = IDD_OUTPUTWIZARD_MIKTEX
	};
	int m_nChoice;
	//}}AFX_DATA
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_OUTPUTWIZARDMIKTEX_H__627C2BE4_5B16_11D4_A222_006097239934__INCLUDED_
