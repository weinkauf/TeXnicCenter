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

#if !defined(AFX_OUTPUTWIZARDFINISH_H__627C2BE7_5B16_11D4_A222_006097239934__INCLUDED_)
#define AFX_OUTPUTWIZARDFINISH_H__627C2BE7_5B16_11D4_A222_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/**
This property page is part of the output wizard (COutputWizard).
It shows the user, which output types are about to be generated.

@author Sven Wiegand
*/
class COutputWizardFinish : public CPropertyPage
{
	DECLARE_DYNCREATE(COutputWizardFinish)

// Construction/Destruction
public:
	COutputWizardFinish();
	virtual ~COutputWizardFinish();

// Overridings
protected:
	//{{AFX_VIRTUAL(COutputWizardFinish)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Message Handlers
protected:
	//{{AFX_MSG(COutputWizardFinish)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dialog Data
public:
	//{{AFX_DATA(COutputWizardFinish)
	enum { IDD = IDD_OUTPUTWIZARD_FINISH };
	CString	m_strList;
	//}}AFX_DATA
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_OUTPUTWIZARDFINISH_H__627C2BE7_5B16_11D4_A222_006097239934__INCLUDED_
