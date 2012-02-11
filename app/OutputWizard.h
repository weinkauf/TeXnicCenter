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

#include <stack>

#include "OutputWizardDistributionPath.h"
#include "OutputWizardFinish.h"
#include "OutputWizardMikTex.h"
#include "OutputWizardViewer.h"
#include "OutputWizardWelcome.h"
#include "Profile.h"

/**
 * The output wizard allows the user a step by step configuration of the output
 * types.
 *
 * @ingroup dialogs
 * @ingroup profiles
 *
 * @see COutputWizardMiKTeX
 * @see COutputWizardDistributionPath
 * @see COutputWizardViewer
 * @see COutputWizardFinish
 * 
 * @author Sven Wiegand
 */
class COutputWizard : public CPropertySheet
{
	DECLARE_DYNAMIC(COutputWizard)
public:
	COutputWizard(CProfileMap &profiles, CWnd* pParentWnd = NULL);
	virtual ~COutputWizard();

	virtual INT_PTR DoModal();
	virtual void BuildPropPageArray();

	/**
	 * @brief Returns the name of the detected distribution.
	 */
	const CString& GetDistributionName() const;

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnBack();
	afx_msg void OnNext();
	afx_msg void OnFinish();
	virtual BOOL OnInitDialog();

private:
	friend class OutputWizardPage;
	friend class COutputWizardDistributionPath;
	friend class COutputWizardFinish;
	friend class COutputWizardMiKTeX;
	friend class COutputWizardViewer;
	friend class COutputWizardWelcome;

	enum tagPage
	{
		pageWelcome = 0,
		pageMikTex,
		pageDistributionPath,
		pageDviViewer,
		pagePsViewer,
		pagePdfViewer,
		pageFinish,
		pageCount
	};

	enum tagViewer
	{
		viewerDvi = 0,
		viewerPs,
		viewerPdf,
		viewerCount
	};

	enum Distribution
	{
		Unknown,
		MiKTeX,
		TeXLive
	};

	const CString FindTeXLiveInstallLocation();
	const CString FindMiKTeXInstallLocation();

	/**
	 * Generates the full path for the specified @a fileName using
	 * distribution's 'bin' directory as root.
	 * 
	 * @param fileName File name.
	 * 
	 * @return Full path of the file within distribution's 'bin' directory.
	 */
	const CString GetDistributionFilePath( LPCTSTR fileName ) const;

	/**
	 * Gets a value indicating whether the compiler with the specified @a
	 * fileName is available in distribution's 'bin' directory.
	 * 
	 * @param fileName Compiler's executable name, e.g. pdflatex.exe.
	 * 
	 * @return @c true if the specified executable is available, @c false
	 *         otherwise.
	 */
	bool IsCompilerAvailable(LPCTSTR fileName) const;

	CString FindApplicationForDocType(LPCTSTR lpszExt);

	/**
	 * @brief Returns a string from the registry.
	 */
	static const CString ReadStringFromRegistry(bool bAdmin, const CString& Path, const CString& Key);

	void LookForDistribution();
	BOOL LookForLatex();
	void LookForDviViewer();
	void LookForPs();
	void LookForPdf();
	void ShowInformation();
	void GenerateOutputProfiles();
	void SetupMakeIndex(CProfile &p);
	void SetupBibTeX(CProfile &p);
	void GeneratePDFProfile(const CString& name, const CString& latexArguments,
			const CString& viewerPath, const CString& latexFileName );
	void GeneratePDFProfile(const CString& name, const CString& latexArguments,
			const CString& viewerPath);
	void SetupYAP(CProfile &p);
	void SetupAcrobatDDE(CProfile &p);
	void SetupSumatraDDE(CProfile &p);
	void AssignPDFViewer(CProfile &p, const CString& path);
	void AssignPDFViewer(CProfile& p);
	void SetupGenericPDF(CProfile &p);

	/** \c true if latex has been found. */
	bool m_bLatexInstalled;

	/** \c true if dvips has been found. */
	bool m_bDvipsInstalled;

	/** \c true if pdflatex has been found. */
	bool m_bPdfLatexInstalled;

	/** \c true if Ghostscript has been found. */
	bool m_bGhostscriptInstalled;

	/** \c true if SumatraPDF has been found. */
	bool sumatra_installed_;

	/// \c true if XeLaTeX has been found.
	bool xelatexInstalled_;

	/// \c true if LuaLaTeX has been found.
	bool lualatexInstalled_;

	CString sumatra_path_;

	/**
	 * TRUE if Postscript conversion is called via the ps2pdf script, e.g. from
	 * miktex.
	 */
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

	bool dvipdfm_installed_;
	CString dvipdfm_path_;
	CString distribution_name_;
	Distribution distribution_;
};
