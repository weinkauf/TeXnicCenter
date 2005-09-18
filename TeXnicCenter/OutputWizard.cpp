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

#include "stdafx.h"
#include "TeXnicCenter.h"
#include "OutputWizard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// class COutputWizard
//-------------------------------------------------------------------

IMPLEMENT_DYNAMIC(COutputWizard, CPropertySheet)


BEGIN_MESSAGE_MAP(COutputWizard, CPropertySheet)
	ON_BN_CLICKED(ID_WIZBACK, OnBack)
	ON_BN_CLICKED(ID_WIZNEXT, OnNext)
	ON_BN_CLICKED(ID_WIZFINISH, OnFinish)
	//{{AFX_MSG_MAP(COutputWizard)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


COutputWizard::COutputWizard(CProfileMap &profiles, CWnd* pParentWnd)
:	CPropertySheet(_T(""), pParentWnd, 0),
	m_profiles(profiles),
	m_wndPageWelcome(IDD_OUTPUTWIZARD_WELCOME),
	m_wndPageDviViewer(IDD_OUTPUTWIZARD_DVIVIEWER),
	m_wndPagePsViewer(IDD_OUTPUTWIZARD_PSVIEWER),
	m_wndPagePdfViewer(IDD_OUTPUTWIZARD_PDFVIEWER),
	m_bMikTexInstalled(false),
	m_bLatexInstalled(false),
	m_bDvipsInstalled(false),
	m_bPdfLatexInstalled(false),
	m_bGhostscriptInstalled(false)
{
	AddPage(&m_wndPageWelcome);
	AddPage(&m_wndPageMikTex);
	AddPage(&m_wndPageDistributionPath);
	AddPage(&m_wndPageDviViewer);
	AddPage(&m_wndPagePsViewer);
	AddPage(&m_wndPagePdfViewer);
	AddPage(&m_wndPageFinish);
	SetWizardMode();
}


COutputWizard::~COutputWizard()
{
	m_stackPageHistory.RemoveAll();
}


void COutputWizard::OnBack()
{
	switch (m_stackPageHistory.Top())
	{
		case pageWelcome:
			SetWizardButtons(PSWIZB_NEXT);
			break;

		default:
			SetWizardButtons(PSWIZB_NEXT | PSWIZB_BACK);
			break;
	}

	SetActivePage(m_stackPageHistory.Pop());
}


void COutputWizard::OnNext()
{
	GetActivePage()->UpdateData();
	m_stackPageHistory.Push(GetActiveIndex());
	switch (m_stackPageHistory.Top())
	{
		case pageWelcome:
			LookForMikTex();
			break;

		case pageMikTex:
			if (m_wndPageMikTex.m_nChoice == COutputWizardMikTex::yes && LookForLatex())
				LookForDviViewer();
			else
				SetActivePage(pageDistributionPath);
			break;

		case pageDistributionPath:
			if (LookForLatex())
				LookForDviViewer();
			else
				m_stackPageHistory.Pop();
			break;

		case pageDviViewer:
			LookForPs();
			break;

		case pagePsViewer:
			LookForPdf();
			break;

		case pagePdfViewer:
			ShowInformation();
			break;

		default:
			ASSERT(false);
	}
}


void COutputWizard::OnFinish()
{
	GenerateOutputProfiles();
	EndDialog(IDOK);
}


int COutputWizard::DoModal() 
{
	m_psh.dwFlags&= ~PSH_HASHELP;
	
	return CPropertySheet::DoModal();
}


BOOL COutputWizard::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	
	SetWizardButtons(PSWIZB_NEXT);
	
	return bResult;
}


CString COutputWizard::FindApplicationForDocType(LPCTSTR lpszExt)
{
	HKEY		hKey = NULL;
	LONG		lResult;
	DWORD		dwType, dwSize;
	CString	strValue;

	try
	{
		lResult = RegOpenKeyEx(HKEY_CLASSES_ROOT, lpszExt, 0, KEY_READ, &hKey);
		if (lResult != ERROR_SUCCESS)
			throw false;

		dwSize = 0;
		lResult = RegQueryValueEx(hKey, NULL, NULL, &dwType, NULL, &dwSize);
		if (lResult != ERROR_SUCCESS || !dwSize || dwType != REG_SZ)
			throw false;
		
		lResult = RegQueryValueEx(hKey, NULL, NULL, &dwType, (LPBYTE)strValue.GetBuffer(dwSize), &dwSize);
		strValue.ReleaseBuffer();
		RegCloseKey(hKey);

		lResult = RegOpenKeyEx(HKEY_CLASSES_ROOT, CPathTool::Cat(strValue, _T("shell\\open\\command")), 0, KEY_READ, &hKey);
		if (lResult != ERROR_SUCCESS)
			throw false;

		dwSize = 0;
		lResult = RegQueryValueEx(hKey, NULL, NULL, &dwType, NULL, &dwSize);
		if (lResult != ERROR_SUCCESS || !dwSize || dwType != REG_SZ)
			throw false;

		lResult = RegQueryValueEx(hKey, NULL, NULL, &dwType, (LPBYTE)strValue.GetBuffer(dwSize), &dwSize);
		strValue.ReleaseBuffer();
		RegCloseKey(hKey);
	}
	catch(...)
	{
		if (hKey)
			RegCloseKey(hKey);

		return _T("");
	}

	// remove command line options from the end of the command
	strValue.MakeUpper();
	int	nIndex = strValue.Find(_T(".EXE"));
	if (nIndex < 0)
		nIndex = strValue.Find(_T(".COM"));

	strValue = strValue.Left(nIndex + 4);
	if (strValue.GetLength() > 0 && strValue[0] == _T('"'))
		strValue = strValue.Right(strValue.GetLength() - 1);

	return strValue;
}


void COutputWizard::SetActivePage(int nPage)
{
	switch (nPage)
	{
		case pageWelcome:
			SetWizardButtons(PSWIZB_NEXT);
			break;

		case pageMikTex:
		case pageDistributionPath:
		case pageDviViewer:
		case pagePsViewer:
		case pagePdfViewer:
			SetWizardButtons(PSWIZB_NEXT | PSWIZB_BACK);
			break;

		case pageFinish:
			SetWizardButtons(PSWIZB_FINISH | PSWIZB_BACK);
			break;

		default:
			// Unknown page!
			ASSERT(false);
	}

	CPropertySheet::SetActivePage(nPage);
}


void COutputWizard::LookForMikTex()
{
	CBCGRegistry userReg(false, true);
	CBCGRegistry adminReg(true, true);
	CString	strPath(_T(""));

	//Get installation path of MiKTeX
	//Where did miktex wrote its stuff?
	// ==> mikTeX Install Options: 'install only for me' and 'install for all'
	// User HKCU is preferred; so first try the HKLM and then let HKCU overwrite it.
	if (adminReg.Open(_T("Software\\MiK\\MiKTeX\\CurrentVersion\\MiKTeX")))
	{
		CString adminPath;
		if (adminReg.Read(_T("Install Root"), adminPath))
		{
			adminPath = CPathTool::Cat(adminPath, _T("MiKTeX\\bin"));
			if (CPathTool::Exists(adminPath))
				strPath = adminPath;
		}

		adminReg.Close();
	}

	if (userReg.Open(_T("Software\\MiK\\MiKTeX\\CurrentVersion\\MiKTeX")))
	{
		CString userPath;
		if (userReg.Read(_T("Install Root"), userPath))
		{
			userPath = CPathTool::Cat(userPath, _T("MiKTeX\\bin"));
			if (CPathTool::Exists(userPath))
				strPath = userPath;
		}

		userReg.Close();
	}

	//Did we find a path?
	if (strPath.GetLength() == 0)
	{
		m_bMikTexInstalled = false;
		SetActivePage(pageDistributionPath);
		return;
	}

	//Yes, everything is fine.
	m_bMikTexInstalled = true;
	m_wndPageDistributionPath.m_strPath = strPath;
	SetActivePage(pageMikTex);
}


BOOL COutputWizard::LookForLatex()
{
	CFileFind	ff;
	if (!ff.FindFile(CPathTool::Cat(m_wndPageDistributionPath.m_strPath, _T("latex.exe"))))
	{
		AfxMessageBox(STE_OUTPUTWIZARD_NOLATEX, MB_ICONSTOP | MB_OK);
		m_bLatexInstalled = false;
	}
	else
	{
		ff.Close();
		m_bLatexInstalled = true;
	}

	return m_bLatexInstalled;
}


void COutputWizard::LookForDviViewer()
{
	// Find standard-viewer for dvi
	CString	strViewer = FindApplicationForDocType(_T(".dvi"));

	if (strViewer.IsEmpty())
	{
		SetActivePage(pageDviViewer);
		return;
	}

	// we have found a viewer

	// is it yap?
	m_wndPageDviViewer.m_strPath = strViewer;

	strViewer.MakeUpper();
	if (!strViewer.Find(_T("YAP.EXE")))
	{
		// let user make his configuration
		SetActivePage(pageDviViewer);

		return;
	}

	// yes, it is YAP!
	m_wndPageDviViewer.m_strSingleInstanceOption = _T("-1");
	m_wndPageDviViewer.m_strForwardSearchOption = _T("-s %l\"%Wc\"");

	// Let's do the YAP inverse search configuration
	CBCGRegistry	reg(false, false);

	if (!reg.Open(_T("Software\\MiK\\MiKTeX\\CurrentVersion\\Yap\\Settings")))
	{
		LookForPs();
		return;
	}

	CString	strEditor;
	reg.Read(_T("Editor"), strEditor);
	strEditor.MakeUpper();
	if (!strEditor.IsEmpty() && strEditor.Find(_T("TEXCNTR.EXE")) < 0)
	{
		if (AfxMessageBox(STE_OUTPUTWIZARD_YAPCONFIG, MB_ICONQUESTION | MB_YESNO) == IDNO)
		{
			LookForPs();
			return;
		}
	}

	GetModuleFileName(NULL, strEditor.GetBuffer(_MAX_PATH), _MAX_PATH);
	strEditor.ReleaseBuffer();
	strEditor+= _T(" /ddecmd \"[goto('%f', '%l')]\"");
	reg.Write(_T("Editor"), strEditor);
	reg.Close();

	LookForPs();
}


void COutputWizard::LookForPs()
{
	// Let's see, if dvips is installed, otherwise, we won't need 
	// a postscript-viewer
	CFileFind	ff;
	if (
		!ff.FindFile(CPathTool::Cat(m_wndPageDistributionPath.m_strPath, _T("dvips.exe"))))
	{
		m_bDvipsInstalled = false;

		// There is no dvips, so we don't need any ps-viewer
		// lets look for pdf
		LookForPdf();
		return;
	}
	ff.Close();

	m_bDvipsInstalled = true;

	// DVIPS is installed
	// is there a registered viewer?
	m_wndPagePsViewer.m_strPath = FindApplicationForDocType(_T(".ps"));

	if (m_wndPagePsViewer.m_strPath.Find(_T("GSVIEW32.EXE")) > -1)
	{
		m_wndPagePsViewer.m_strSingleInstanceOption = _T("-e");

		LookForPdf();
	}
	else
	{
		// lets the user make his configuration
		SetActivePage(pagePsViewer);
	}
}


void COutputWizard::LookForPdf()
{
	// Let's see, if pdflatex is installed
	CFileFind	ff;
	m_bPdfLatexInstalled = false;
	if (ff.FindFile(CPathTool::Cat(m_wndPageDistributionPath.m_strPath, _T("pdflatex.exe"))))
	{
		m_bPdfLatexInstalled = true;
	}
	ff.Close();


	//Lets see, if Ghostscript is installed
	// We find the Ghostscript dll in the registry here:
	//  Path: HKEY_LOCAL_MACHINE\SOFTWARE\AFPL Ghostscript\<version>
	//  Entry: GS_DLL=f:\prog\gstools\gs\gs8.12\bin\gsdll32.dll
	// We need the directory of GS_DLL. There we find gswin32c.exe.
	m_bGhostscriptInstalled = false;
	CBCGRegistry gsReg(true, true); //HKEY_LOCAL_MACHINE, ReadOnly
	if (gsReg.Open("SOFTWARE\\AFPL Ghostscript"))
	{
		//Read the subkeys - i.e. subfolders under "AFPL Ghostscript"
		CStringArray SubKeys;
		gsReg.ReadSubKeys(SubKeys);

		//We look for the newest version
		int idGSVersionSubKey = -1;
		int gsMajorVersion = -1;
		int gsMinorVersion = -1;
		for(int i=0;i<SubKeys.GetSize();i++)
		{
			int idxPoint = SubKeys[i].Find(_T('.'));
			if (idxPoint <= 0) continue;
			int gsMajorVersionThis = atoi( SubKeys[i].Left(idxPoint) );
			int gsMinorVersionThis = atoi( SubKeys[i].Right(SubKeys[i].GetLength() - idxPoint - 1) );
			// (8.12 vs. 8.11) 80012 > 80011;  (8.12 at init) 80012 > -10001
			if ( (gsMajorVersionThis * 10000 + gsMinorVersionThis) > (gsMajorVersion * 10000 + gsMinorVersion) )
			{
				idGSVersionSubKey = i;
				gsMajorVersion = gsMajorVersionThis;
				gsMinorVersion = gsMinorVersionThis;
			}
		}

		//Did we find a version? And can we go into that subdir?
		if ( (idGSVersionSubKey >= 0) && (gsReg.Open(SubKeys[idGSVersionSubKey])) )
		{
			if (gsReg.Read(_T("GS_DLL"), m_strGhostscriptPath))
			{
				m_strGhostscriptPath = CPathTool::Cat(CPathTool::GetDirectory(m_strGhostscriptPath), _T("gswin32c.exe"));
				if (ff.FindFile(m_strGhostscriptPath))
					m_bGhostscriptInstalled = true;

				ff.Close();
			}
		}

		gsReg.Close();
	}


	//Can the user create PDFs?
	if (!m_bPdfLatexInstalled && !m_bGhostscriptInstalled)
	{
		// There is no pdflatex or Ghostscript,
		// so we don't need a pdf-viewer.
		// Lets finish.
		ShowInformation();
		return;
	}


	// OK, the user can generate PDF-documents.
	// Lets look for AcrobatReader to view them
	CString	strViewer = FindApplicationForDocType(_T(".pdf"));

	m_wndPagePdfViewer.m_strPath = strViewer;

	if (strViewer.Find(_T("ACRORD32.EXE")) > -1)
	{
		// standard viewer is Acrobat Reader
		ShowInformation();
		m_wndPagePdfViewer.m_strSingleInstanceOption.Empty();
		m_wndPagePdfViewer.m_strForwardSearchOption.Empty();
	}
	else
		// lets user make his configuration
		SetActivePage(pagePdfViewer);
}


void COutputWizard::ShowInformation()
{
	SetWizardButtons(PSWIZB_FINISH | PSWIZB_BACK);

	// generate list of output types to create
	m_wndPageFinish.m_strList.Empty();

	if (m_bLatexInstalled)
		m_wndPageFinish.m_strList+= _T("- ") + CString((LPCTSTR)STE_OUTPUTWIZARD_DVITYPE) + _T("\n\n");
	if (m_bDvipsInstalled && m_bLatexInstalled)
		m_wndPageFinish.m_strList+= _T("- ") + CString((LPCTSTR)STE_OUTPUTWIZARD_PSTYPE) + _T("\n\n");
	if (m_bPdfLatexInstalled)
		m_wndPageFinish.m_strList+= _T("- ") + CString((LPCTSTR)STE_OUTPUTWIZARD_PDFTYPE) + _T("\n\n");
	if (m_bGhostscriptInstalled)
		m_wndPageFinish.m_strList+= _T("- ") + CString((LPCTSTR)STE_OUTPUTWIZARD_PDFVIAPSTYPE) + _T("\n\n");

	SetActivePage(pageFinish);
}


void COutputWizard::GenerateOutputProfiles()
{
	CString	strError;

	//Some things, that we will reuse inside this function
	// - Options for normal latex
	// - %Wm, because of the src-specials for forward/inverse search. Otherwise, things might break.
	CString strLatexOptions(_T("--src -interaction=nonstopmode \"%Wm\""));
	// - Only miktex support the -max-print-line=N feature
	if (m_bMikTexInstalled) strLatexOptions = _T("--src -interaction=nonstopmode -max-print-line=120 \"%Wm\"");
	// - Options for PDFLatex
	// - %pm, because it doesn't matter here. I guess, we could use %Wm as well. But %pm is tested and seems to work for all.
	CString strPDFLatexOptions(_T("-interaction=nonstopmode \"%pm\""));
	// - Only miktex support the -max-print-line=N feature
	if (m_bMikTexInstalled) strPDFLatexOptions = _T("-interaction=nonstopmode -max-print-line=120 \"%pm\"");

	// LaTeX => DVI
	if (m_bLatexInstalled)
	{
		CString	strProfile((LPCTSTR)STE_OUTPUTWIZARD_DVITYPE);

		strError.Format(STE_OUTPUTWIZARD_OUTPUTTYPEEXISTS, strProfile);
		BOOL		bExists = m_profiles.Exists(strProfile);
		if (!bExists || AfxMessageBox(strError, MB_ICONQUESTION | MB_YESNO) == IDYES)
		{
			if (bExists)
				m_profiles.Remove(strProfile);

			// create profile
			CProfile p;

			p.SetLatexPath(
				CPathTool::Cat(m_wndPageDistributionPath.m_strPath, _T("latex.exe")), strLatexOptions);
			p.SetBibTexPath(
				CPathTool::Cat(m_wndPageDistributionPath.m_strPath, _T("bibtex.exe")),
				_T("\"%bm\""));
			p.SetMakeIndexPath(
				CPathTool::Cat(m_wndPageDistributionPath.m_strPath, _T("makeindex.exe")),
				_T("\"%bm\""));

			// add viewer settings
			if (!m_wndPageDviViewer.m_strPath.IsEmpty())
			{
				p.SetViewerPath(m_wndPageDviViewer.m_strPath);

				CProcessCommand			cmd;
				CProfile::CCommand	profileCmd;
				profileCmd.SetActiveCommand(CProfile::CCommand::typeProcess);

				cmd.Set(
					m_wndPageDviViewer.m_strPath, 
					m_wndPageDviViewer.m_strSingleInstanceOption + _T(" \"%bm.dvi\""));
				profileCmd.SetProcessCommand(cmd);
				p.SetViewProjectCmd(profileCmd);

				cmd.Set(
					m_wndPageDviViewer.m_strPath, 
					m_wndPageDviViewer.m_strSingleInstanceOption + _T(' ')
					+ m_wndPageDviViewer.m_strForwardSearchOption + _T(" \"%bm.dvi\""));
				profileCmd.SetProcessCommand(cmd);
				p.SetViewCurrentCmd(profileCmd);
			}

			// add profile to map
			m_profiles.Add(strProfile, p);
		}
	}

	// LaTeX => PS
	if (m_bLatexInstalled && m_bDvipsInstalled)
	{
		CString	strProfile((LPCTSTR)STE_OUTPUTWIZARD_PSTYPE);

		strError.Format(STE_OUTPUTWIZARD_OUTPUTTYPEEXISTS, strProfile);
		BOOL		bExists = m_profiles.Exists(strProfile);
		if (!bExists || AfxMessageBox(strError, MB_ICONQUESTION | MB_YESNO) == IDYES)
		{
			if (bExists)
				m_profiles.Remove(strProfile);

			// create profile
			CProfile p;

			p.SetLatexPath(
				CPathTool::Cat(m_wndPageDistributionPath.m_strPath, _T("latex.exe")), strLatexOptions);
			p.SetBibTexPath(
				CPathTool::Cat(m_wndPageDistributionPath.m_strPath, _T("bibtex.exe")),
				_T("\"%bm\""));
			p.SetMakeIndexPath(
				CPathTool::Cat(m_wndPageDistributionPath.m_strPath, _T("makeindex.exe")),
				_T("\"%bm\""));

			// add post processor dvips
			CPostProcessor	pp(
				_T("DviPs"), CPathTool::Cat(m_wndPageDistributionPath.m_strPath, _T("dvips.exe")),
				_T("\"%Bm.dvi\""));
			p.GetPostProcessorArray().Add(pp);

			// add viewer settings
			if (!m_wndPagePsViewer.m_strPath.IsEmpty())
			{
				p.SetViewerPath(m_wndPagePsViewer.m_strPath);

				CProcessCommand			cmd;
				CProfile::CCommand	profileCmd;
				profileCmd.SetActiveCommand(CProfile::CCommand::typeProcess);

				cmd.Set(
					m_wndPagePsViewer.m_strPath, 
					m_wndPagePsViewer.m_strSingleInstanceOption + _T(" \"%bm.ps\""));
				profileCmd.SetProcessCommand(cmd);
				p.SetViewProjectCmd(profileCmd);

				cmd.Set(
					m_wndPagePsViewer.m_strPath, 
					m_wndPagePsViewer.m_strSingleInstanceOption + _T(' ')
					+ m_wndPagePsViewer.m_strForwardSearchOption + _T(" \"%bm.ps\""));
				profileCmd.SetProcessCommand(cmd);
				p.SetViewCurrentCmd(profileCmd);
			}

			// add profile to map
			m_profiles.Add(strProfile, p);
		}
	}

	// LaTeX => PDF
	if (m_bPdfLatexInstalled)
	{
		CString	strProfile((LPCTSTR)STE_OUTPUTWIZARD_PDFTYPE);

		strError.Format(STE_OUTPUTWIZARD_OUTPUTTYPEEXISTS, strProfile);
		BOOL		bExists = m_profiles.Exists(strProfile);
		if (!bExists || AfxMessageBox(strError, MB_ICONQUESTION | MB_YESNO) == IDYES)
		{
			if (bExists)
				m_profiles.Remove(strProfile);

			// create profile
			CProfile p;

			p.SetLatexPath(
				CPathTool::Cat(m_wndPageDistributionPath.m_strPath, _T("pdflatex.exe")), strPDFLatexOptions);
			p.SetBibTexPath(
				CPathTool::Cat(m_wndPageDistributionPath.m_strPath, _T("bibtex.exe")),
				_T("\"%bm\""));
			p.SetMakeIndexPath(
				CPathTool::Cat(m_wndPageDistributionPath.m_strPath, _T("makeindex.exe")),
				_T("\"%bm\""));

			// add viewer settings
			if (!m_wndPagePdfViewer.m_strPath.IsEmpty())
			{
				p.SetViewerPath(m_wndPagePdfViewer.m_strPath);

				if (CPathTool::GetFile(m_wndPagePdfViewer.m_strPath).CompareNoCase(_T("AcroRd32.exe")) == 0)
				{
					// Acrobat reader specific commands
					CDdeCommand					cmd;
					CProfile::CCommand	profileCmd;
					profileCmd.SetActiveCommand(CProfile::CCommand::typeDde);

					cmd.SetExecutable(m_wndPagePdfViewer.m_strPath);
					cmd.SetCommand(_T("[DocOpen(\"%bm.pdf\")][FileOpen(\"%bm.pdf\")]"));
					cmd.SetServer(_T("acroview"), _T("control"));
					profileCmd.SetDdeCommand(cmd);
					p.SetViewProjectCmd(profileCmd);

					p.SetViewCurrentCmd(profileCmd);

					cmd.SetCommand(_T("[DocClose(\"%bm.pdf\")]"));
					profileCmd.SetDdeCommand(cmd);
					p.SetViewCloseCmd(profileCmd);
					p.SetCloseView(true);
				}
				else
				{
					// general commands
					CProcessCommand			cmd;
					CProfile::CCommand	profileCmd;
					profileCmd.SetActiveCommand(CProfile::CCommand::typeProcess);

					cmd.Set(
						m_wndPagePdfViewer.m_strPath, 
						m_wndPagePdfViewer.m_strSingleInstanceOption + _T(" \"%bm.pdf\""));
					profileCmd.SetProcessCommand(cmd);
					p.SetViewProjectCmd(profileCmd);

					cmd.Set(
						m_wndPagePdfViewer.m_strPath, 
						m_wndPagePdfViewer.m_strSingleInstanceOption + _T(' ')
						+ m_wndPagePdfViewer.m_strForwardSearchOption + _T(" \"%bm.pdf\""));
					profileCmd.SetProcessCommand(cmd);
					p.SetViewCurrentCmd(profileCmd);
				}
			}

			// add profile to map
			m_profiles.Add(strProfile, p);
		}
	}

	// LaTeX => PS => PDF
	if (m_bLatexInstalled && m_bDvipsInstalled && m_bGhostscriptInstalled)
	{
		CString	strProfile((LPCTSTR)STE_OUTPUTWIZARD_PDFVIAPSTYPE);

		strError.Format(STE_OUTPUTWIZARD_OUTPUTTYPEEXISTS, strProfile);
		BOOL		bExists = m_profiles.Exists(strProfile);
		if (!bExists || AfxMessageBox(strError, MB_ICONQUESTION | MB_YESNO) == IDYES)
		{
			if (bExists)
				m_profiles.Remove(strProfile);

			// create profile
			CProfile p;

			p.SetLatexPath(
				CPathTool::Cat(m_wndPageDistributionPath.m_strPath, _T("latex.exe")), strLatexOptions);
			p.SetBibTexPath(
				CPathTool::Cat(m_wndPageDistributionPath.m_strPath, _T("bibtex.exe")),
				_T("\"%bm\""));
			p.SetMakeIndexPath(
				CPathTool::Cat(m_wndPageDistributionPath.m_strPath, _T("makeindex.exe")),
				_T("\"%bm\""));

			// add post processor dvips
			CPostProcessor	ppDVIPS(
				_T("DviPs (PDF)"), CPathTool::Cat(m_wndPageDistributionPath.m_strPath, _T("dvips.exe")),
				_T("-P pdf \"%Bm.dvi\""));
			p.GetPostProcessorArray().Add(ppDVIPS);

			// add post processor Ghostscript ps2pdf
			CPostProcessor	ppGS(
				_T("Ghostscript (ps2pdf)"), m_strGhostscriptPath,
				_T("-sPAPERSIZE=a4 -dSAFER -dBATCH -dNOPAUSE -sDEVICE=pdfwrite -sOutputFile=\"%bm.pdf\" -c save pop -f \"%bm.ps\""));
			p.GetPostProcessorArray().Add(ppGS);

			// add viewer settings
			if (!m_wndPagePdfViewer.m_strPath.IsEmpty())
			{
				p.SetViewerPath(m_wndPagePdfViewer.m_strPath);

				if (CPathTool::GetFile(m_wndPagePdfViewer.m_strPath).CompareNoCase(_T("AcroRd32.exe")) == 0)
				{
					// Acrobat reader specific commands
					CDdeCommand					cmd;
					CProfile::CCommand	profileCmd;
					profileCmd.SetActiveCommand(CProfile::CCommand::typeDde);

					cmd.SetExecutable(m_wndPagePdfViewer.m_strPath);
					cmd.SetCommand(_T("[DocOpen(\"%bm.pdf\")][FileOpen(\"%bm.pdf\")]"));
					cmd.SetServer(_T("acroview"), _T("control"));
					profileCmd.SetDdeCommand(cmd);
					p.SetViewProjectCmd(profileCmd);

					p.SetViewCurrentCmd(profileCmd);

					cmd.SetCommand(_T("[DocClose(\"%bm.pdf\")]"));
					profileCmd.SetDdeCommand(cmd);
					p.SetViewCloseCmd(profileCmd);
					p.SetCloseView(true);
				}
				else
				{
					// general commands
					CProcessCommand			cmd;
					CProfile::CCommand	profileCmd;
					profileCmd.SetActiveCommand(CProfile::CCommand::typeProcess);

					cmd.Set(
						m_wndPagePdfViewer.m_strPath, 
						m_wndPagePdfViewer.m_strSingleInstanceOption + _T(" \"%bm.pdf\""));
					profileCmd.SetProcessCommand(cmd);
					p.SetViewProjectCmd(profileCmd);

					cmd.Set(
						m_wndPagePdfViewer.m_strPath, 
						m_wndPagePdfViewer.m_strSingleInstanceOption + _T(' ')
						+ m_wndPagePdfViewer.m_strForwardSearchOption + _T(" \"%bm.pdf\""));
					profileCmd.SetProcessCommand(cmd);
					p.SetViewCurrentCmd(profileCmd);
				}
			}

			// add profile to map
			m_profiles.Add(strProfile, p);
		}
	}



	/*
	if (m_bDvipsInstalled && m_bLatexInstalled)
	{
		strError.Format(STE_OUTPUTWIZARD_OUTPUTTYPEEXISTS, CString((LPCTSTR)STE_OUTPUTWIZARD_PSTYPE));
		nIndex = g_configuration.m_aCompilerTypes.FindOutfileDescr(CString((LPCTSTR)STE_OUTPUTWIZARD_PSTYPE));		
		if (nIndex == -1 || AfxMessageBox(strError, MB_ICONQUESTION | MB_YESNO) == IDYES)
		{
			if (nIndex > -1)
				g_configuration.m_aCompilerTypes.RemoveAt(nIndex);

			// create output type DVI
			CCompilerType		ct;
			ct.m_nInfileType = g_configuration.m_aCompilerTypes.FindOutfileExt(_T("DVI"));
			ct.m_strOutfileDescr.LoadString(STE_OUTPUTWIZARD_PSTYPE);
			ct.m_strOutfileExt = _T("PS");
			ct.m_strCompilerCmd = 
				CPathTool::Cat(m_wndPageDistributionPath.m_strPath, _T("dvips.exe")) + _T(" \"%Bm.dvi\"");

			if (!m_wndPagePsViewer.m_strPath.IsEmpty())
			{
				ct.m_strViewerStartCmd = 
					m_wndPagePsViewer.m_strPath + _T(' ') + m_wndPagePsViewer.m_strSingleInstanceOption + _T(" \"%bm.ps\"");
				ct.m_strViewerStartCmdEx = 
					m_wndPagePsViewer.m_strPath + _T(' ') + m_wndPagePsViewer.m_strSingleInstanceOption + 
					_T(' ') + m_wndPagePsViewer.m_strForwardSearchOption + _T(" \"%bm.ps\"");
			}

			g_configuration.m_aCompilerTypes.Add(ct);
		}
	}

	if (m_bPdfLatexInstalled)
	{
		strError.Format(STE_OUTPUTWIZARD_OUTPUTTYPEEXISTS, CString((LPCTSTR)STE_OUTPUTWIZARD_PDFTYPE));
		nIndex = g_configuration.m_aCompilerTypes.FindOutfileDescr(CString((LPCTSTR)STE_OUTPUTWIZARD_PDFTYPE));
		if (nIndex == -1 || AfxMessageBox(strError, MB_ICONQUESTION | MB_YESNO) == IDYES)
		{
			if (nIndex > -1)
				g_configuration.m_aCompilerTypes.RemoveAt(nIndex);

			// create output type DVI
			CCompilerType		ct;
			ct.m_nInfileType = -1;
			ct.m_strOutfileDescr.LoadString(STE_OUTPUTWIZARD_PDFTYPE);
			ct.m_strOutfileExt = _T("PDF");
			ct.m_strCompilerCmd = 
				CPathTool::Cat(m_wndPageDistributionPath.m_strPath, _T("pdflatex.exe")) + _T(" \\nonstopmode \\input \"%nm\"");

			if (!m_wndPagePdfViewer.m_strPath.IsEmpty())
			{
				ct.m_strViewerStartCmd = 
					m_wndPagePdfViewer.m_strPath + _T(' ') + m_wndPagePdfViewer.m_strSingleInstanceOption + _T(" \"%bm.pdf\"");
				ct.m_strViewerStartCmdEx = 
					m_wndPagePdfViewer.m_strPath + _T(' ') + m_wndPagePdfViewer.m_strSingleInstanceOption + 
					_T(' ') + m_wndPagePdfViewer.m_strForwardSearchOption + _T(" \"%bm.pdf\"");
			}

			g_configuration.m_aCompilerTypes.Add(ct);
		}
	}
	*/
}

