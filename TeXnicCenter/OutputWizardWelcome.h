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

#if !defined(AFX_OUTPUTWIZARDWELCOME_H__627C2BE9_5B16_11D4_A222_006097239934__INCLUDED_)
#define AFX_OUTPUTWIZARDWELCOME_H__627C2BE9_5B16_11D4_A222_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OutputWizardPage.h"

/**
This property sheet is part of the output wizard (COutputWizard).
It shows the user a welcome message and gives him some general
instructions.

@ingroup dialogs
@ingroup profiles

@author Sven Wiegand
 */
class COutputWizardWelcome : public OutputWizardPage
{
	DECLARE_DYNCREATE(COutputWizardWelcome)

public:
	COutputWizardWelcome(COutputWizard* w = 0);
	virtual ~COutputWizardWelcome();

	// Dialogfelddaten
	//{{AFX_DATA(COutputWizardWelcome)

	enum
	{
		IDD = IDD_OUTPUTWIZARD_WELCOME
	};
	// HINWEIS - Der Klassen-Assistent f�gt hier Datenelemente ein.
	//    Innerhalb dieser generierten Quellcodeabschnitte NICHTS BEARBEITEN!
	//}}AFX_DATA


	// �berschreibungen
	// Der Klassen-Assistent generiert virtuelle Funktions�berschreibungen
//{{AFX_VIRTUAL(COutputWizardWelcome)
protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV-Unterst�tzung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(COutputWizardWelcome)
	// HINWEIS: Der Klassen-Assistent f�gt hier Member-Funktionen ein
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f�gt unmittelbar vor der vorhergehenden Zeile zus�tzliche Deklarationen ein.

#endif // AFX_OUTPUTWIZARDWELCOME_H__627C2BE9_5B16_11D4_A222_006097239934__INCLUDED_
