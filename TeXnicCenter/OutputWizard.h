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

#if !defined(AFX_OUTPUTWIZARD_H__627C2BE3_5B16_11D4_A222_006097239934__INCLUDED_)
#define AFX_OUTPUTWIZARD_H__627C2BE3_5B16_11D4_A222_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stack>

#include "OutputWizardWelcome.h"
#include "OutputWizardMikTex.h"
#include "OutputWizardDistributionPath.h"
#include "OutputWizardViewer.h"
#include "OutputWizardFinish.h"
#include "Profile.h"

/**
The output wizard allows the user a step by step configuration of the
output types.

@ingroup dialogs
@ingroup profiles

@see COutputWizardMiKTeX
@see COutputWizardDistributionPath
@see COutputWizardViewer
@see COutputWizardFinish

@author Sven Wiegand
 */
class COutputWizard : public CPropertySheet {

    DECLARE_DYNAMIC(COutputWizard)

    friend class OutputWizardPage;
    friend class COutputWizardDistributionPath;
    friend class COutputWizardFinish;
    friend class COutputWizardMiKTeX;
    friend class COutputWizardViewer;
    friend class COutputWizardWelcome;

    // Construction/Destruction
public:
    COutputWizard(CProfileMap &profiles, CWnd* pParentWnd = NULL);
    virtual ~COutputWizard();

    // Operations
public:

    // Overridings
protected:
    //{{AFX_VIRTUAL(COutputWizard)
public:
    virtual int DoModal();
    virtual BOOL OnInitDialog();
    //}}AFX_VIRTUAL

    // Implementation Helpers
protected:
    /**
     */
    CString FindApplicationForDocType(LPCTSTR lpszExt);

    ///Returns a string from the registry.
    const CString ReadStringFromRegistry(bool bAdmin, const CString& Path, const CString& Key);

    /**
     */
    void SetActivePage(int nPage);

    /**
     */
    void LookForMikTex();

    /**
     */
    BOOL LookForLatex();

    /**
     */
    void LookForDviViewer();

    /**
     */
    void LookForPs();

    /**
     */
    void LookForPdf();

    /**
     */
    void ShowInformation();

    /**
     */
    void GenerateOutputProfiles();

    // Message handlers
protected:
    afx_msg void OnBack();
    afx_msg void OnNext();
    afx_msg void OnFinish();
    //{{AFX_MSG(COutputWizard)
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()

    // Constants
protected:
    enum tagPage {
        pageWelcome = 0,
        pageMikTex,
        pageDistributionPath,
        pageDviViewer,
        pagePsViewer,
        pagePdfViewer,
        pageFinish,
        pageCount
    };

    enum tagViewer {
        viewerDvi = 0,
        viewerPs,
        viewerPdf,
        viewerCount
    };

    // Attributes
protected:
    /** TRUE if MiKTeX has been found. */
    bool m_bMikTexInstalled;

    /** TRUE if latex has been found. */
    bool m_bLatexInstalled;

    /** TRUE if dvips has been found. */
    bool m_bDvipsInstalled;

    /** TRUE if pdflatex has been found. */
    bool m_bPdfLatexInstalled;

    /** TRUE if Ghostscript has been found. */
    bool m_bGhostscriptInstalled;

    /** TRUE if Postscript conversion is called via the ps2pdf script, e.g. from miktex. */
    bool m_bGhostscriptViaPS2PDF;

    /** Path to Ghostscript. */
    CString m_strGhostscriptPath;

    /** Profile map to modify */
    CProfileMap &m_profiles;

    /** Stack for storing recently showed pages. */
    std::stack<int> m_stackPageHistory;

    /** The pages */
    COutputWizardWelcome m_wndPageWelcome;
    COutputWizardMiKTeX m_wndPageMikTex;
    COutputWizardDistributionPath m_wndPageDistributionPath;
    COutputWizardViewer m_wndPageDviViewer;
    COutputWizardViewer m_wndPagePsViewer;
    COutputWizardViewer m_wndPagePdfViewer;
    COutputWizardFinish m_wndPageFinish;

public:
    virtual void BuildPropPageArray(void);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_OUTPUTWIZARD_H__627C2BE3_5B16_11D4_A222_006097239934__INCLUDED_
