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
#include "InsertGraphicDialog.h"
#include "global.h"
#include "Configuration.h"
#include <locale.h>
//#include "../Speller/Character.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CInsertGraphicDialog

CInsertGraphicDialog::CInsertGraphicDialog(CWnd* pParent /*=NULL*/)
		: CInsertFloatObjectDialog(CInsertGraphicDialog::IDD,CInsertFloatObjectDialog::figure,FALSE,pParent)
{
	//{{AFX_DATA_INIT(CInsertGraphicDialog)
	m_bScale = FALSE;
	m_bHCenter = TRUE;
	//m_strFile = _T("");
	m_bFloat = TRUE;
	m_unScale = 100;
	m_bScaleToText = TRUE;
	//}}AFX_DATA_INIT
}

BOOL CInsertGraphicDialog::SetProperties(const CString &strProperties)
{
	if (strProperties.Find(_T("\\includegraphics")) == -1)
		return FALSE;

	///////////////////////////////////////////////////////////////////
	// is floating
	m_bFloat = CInsertFloatObjectDialog::SetProperties(strProperties);
	m_strContents.Empty();

	//	///////////////////////////////////////////////////////////////////
	//	// is scaled?
	//	if (m_bScale && ( (m_unScale != 100) || (m_bScaleToText) ))
	//	{
	//		CString	strScale;
	//		if (m_bScaleToText)
	//		{
	//			strScale.Format( _T("[width=%.2f\\textwidth]"), m_unScale / 100.0 );
	//		}
	//		else
	//		{
	//			strScale.Format( _T("[scale=%.2f]"), m_unScale / 100.0 );
	//		}
	//
	//		strScale.Replace( _T(','), _T('.') );
	//		strResult += strScale;
	//	}


	///////////////////////////////////////////////////////////////////
	// is scaled?
	int nPos = -1;
	m_bScale = false;

	//Find type of scaling, if scaling at all
	// - Normal Scale
	CString strFind = _T("[scale=");
	// - Scale to \textwidth
	CString strFind2 = _T("[width=");
	if ((nPos = strProperties.Find(strFind)) > -1)
	{
		m_bScale = true;
		m_bScaleToText = false;

		nPos = nPos + strFind.GetLength();
	}
	else if ((nPos = strProperties.Find(strFind2)) > -1)
	{
		m_bScale = true;
		m_bScaleToText = true;

		nPos = nPos + strFind2.GetLength();
	}

	//Now for the scaling factor
	if (m_bScale)
	{
		ASSERT(nPos > -1);

		// get scale factor
		CString strScale;
		struct lconv *pConv = localeconv();

		for (; nPos < strProperties.GetLength(); nPos++)
		{
			if (_istdigit(strProperties[nPos]) || strProperties[nPos] == _T('.') || strProperties[nPos] == _T(','))
				strScale += strProperties[nPos];
			else
				break;
		}

		if (*pConv->decimal_point == ',')
			strScale.Replace(_T('.'),_T(','));
		else if (*pConv->decimal_point == '.')
			strScale.Replace(_T(','),_T('.'));

		if (!strScale.IsEmpty())
			m_unScale = (UINT)(_tstof(strScale) * 100);
	}

	///////////////////////////////////////////////////////////////////
	// is horizonally centered
	if ((strProperties.Find(_T("\\centering")) > -1) || (strProperties.Find(_T("\\begin{center}")) > -1))
		m_bHCenter = TRUE;
	else
		m_bHCenter = FALSE;

	///////////////////////////////////////////////////////////////////
	// get file path
	strFind = _T("\\includegraphics");

	//Empty filename
	m_strFile.Empty();

	if ((nPos = strProperties.Find(strFind)) > -1)
	{
		nPos = nPos + strFind.GetLength();

		//Forward to opening brace {
		nPos = strProperties.Find(_T('{'),nPos);
		if (nPos > -1)
		{
			++nPos; //Jump over the opening brace {
			//Read the filename
			for (; nPos < strProperties.GetLength(); nPos++)
			{
				if (strProperties[nPos] == _T('}'))
					break;
				else
					m_strFile += strProperties[nPos];
			}
		}
	}
	else
		return FALSE;

	return TRUE;
}

CString CInsertGraphicDialog::GetProperties()
{
	///////////////////////////////////////////////////////////////////
	// statics
	CString strResult(_T("\\includegraphics"));

	///////////////////////////////////////////////////////////////////
	// is scaled?
	if (m_bScale && ((m_unScale != 100) || (m_bScaleToText)))
	{
		CString strScale;
		if (m_bScaleToText)
		{
			//Scale to \textwidth
			strScale.Format(_T("[width=%.2f\\textwidth]"),m_unScale / 100.0);
		}
		else
		{
			//Normal Scale
			strScale.Format(_T("[scale=%.2f]"),m_unScale / 100.0);
		}

		strScale.Replace(_T(','),_T('.'));
		strResult += strScale;
	}

	///////////////////////////////////////////////////////////////////
	// statics
	strResult += _T('{');
	strResult += m_strFile;
	strResult += _T("}\r");

	///////////////////////////////////////////////////////////////////
	// is centered?
	if (m_bHCenter)
	{
		if (m_bFloat)
			strResult = _T("\\centering\r\t\t") + strResult;
		else
			strResult = _T("\\begin{center}\r\t") + strResult + _T("\\end{center}");
	}

	///////////////////////////////////////////////////////////////////
	// is floating?
	if (m_bFloat)
	{
		CString strPreResult = strResult;
		strResult = CInsertFloatObjectDialog::GetProperties();

		int nPos = strResult.Find(_T('\001'));
		strResult.Delete(nPos);
		strResult.Insert(nPos,strPreResult);
	}
	else
		strResult.Insert(0,_T('\r'));


	///////////////////////////////////////////////////////////////////
	// Empty Lines?
	strResult.Replace(_T("\r\r"),_T("\r"));

	return strResult;
}

void CInsertGraphicDialog::DoDataExchange(CDataExchange* pDX)
{
	CInsertFloatObjectDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInsertGraphicDialog)
	DDX_Control(pDX,IDC_GRAPHIC_SCALETEXTWIDTH,m_wndScaleToText);
	DDX_Control(pDX,IDC_LABELSCALE,m_wndScaleLabel);
	DDX_Control(pDX,IDC_SPIN_SCALE,m_wndScaleSpin);
	DDX_Control(pDX,IDC_GRAPHIC_SCALE,m_wndScale);
	DDX_Check(pDX,IDC_GRAPHIC_BSCALE,m_bScale);
	DDX_Check(pDX,IDC_GRAPHIC_CENTER,m_bHCenter);
	DDX_Text(pDX,IDC_GRAPHIC_FILE,m_strFile);
	DDX_Check(pDX,IDC_GRAPHIC_FLOAT,m_bFloat);
	DDX_Text(pDX,IDC_GRAPHIC_SCALE,m_unScale);
	DDX_Check(pDX,IDC_GRAPHIC_SCALETEXTWIDTH,m_bScaleToText);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInsertGraphicDialog,CInsertFloatObjectDialog)
	//{{AFX_MSG_MAP(CInsertGraphicDialog)
	ON_BN_CLICKED(IDC_GRAPHIC_BROWSE,OnGraphicBrowse)
	ON_BN_CLICKED(IDC_GRAPHIC_BSCALE,OnGraphicBscale)
	ON_BN_CLICKED(IDC_GRAPHIC_FLOAT,OnGraphicFloat)
	ON_EN_CHANGE(IDC_GRAPHIC_FILE,OnChangePath)
	ON_EN_CHANGE(IDC_FLOAT_CAPTION,OnChangeFloatCaption)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CInsertGraphicDialog

BOOL CInsertGraphicDialog::OnInitDialog()
{
	CInsertFloatObjectDialog::OnInitDialog();

	// set spin range
	m_wndScaleSpin.SetRange(0,0x7fff);

	// enable/disable windows
	m_wndScale.EnableWindow(m_bScale);
	m_wndScaleSpin.EnableWindow(m_bScale);
	m_wndScaleLabel.EnableWindow(m_bScale);
	m_wndScaleToText.EnableWindow(m_bScale);

	EnableFloatOptions(m_bFloat);

	OnChangePath();

	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void CInsertGraphicDialog::OnGraphicBrowse()
{
	UpdateData(TRUE);

	CFileDialogEx dlg(
	    TRUE,NULL,m_strFile,
	    OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR,
	    AfxLoadString(STE_GRAPHIC_FILES),this);

	//Get default path
	CString strInitialDir = AfxGetDefaultDirectory();
	dlg.m_ofn.lpstrInitialDir = strInitialDir;

	//Show the dialog
	if (dlg.DoModal() != IDOK)
	{
		//It was cancelled - the PathName is not set.
		// Therefore, we use GetLastOpenedFolder.
		AfxSetLastDirectory(dlg.GetLastOpenedFolder());
		return;
	}

	AfxSetLastDirectory(CPathTool::GetDirectory(dlg.GetPathName()));
	m_strFile = dlg.GetPathName();

	//Get path relative to project dir
	CLaTeXProject* pLProject = theApp.GetProject();
	if (pLProject)
	{
		m_strFile = CPathTool::GetRelativePath(pLProject->GetWorkingDir(),m_strFile);
	}

	m_strFile.Replace(_T('\\'),_T('/'));

	UpdateData(FALSE);

	OnChangePath();
}

void CInsertGraphicDialog::OnGraphicBscale()
{
	UpdateData(TRUE);

	m_wndScale.EnableWindow(m_bScale);
	m_wndScaleSpin.EnableWindow(m_bScale);
	m_wndScaleLabel.EnableWindow(m_bScale);
	m_wndScaleToText.EnableWindow(m_bScale);
}

void CInsertGraphicDialog::OnGraphicFloat()
{
	UpdateData(TRUE);

	EnableFloatOptions(m_bFloat);
}

void CInsertGraphicDialog::OnOK()
{
	UpdateData(TRUE);
	m_strFile.Replace(_T('\\'),_T('/'));
	UpdateData(FALSE);

	CInsertFloatObjectDialog::OnOK();
}

void CInsertGraphicDialog::OnChangePath()
{
	// generate label out of path
	UpdateData();

	// auto generate label
	CString strLabel(m_strFile);
	int nPos;

	// throw away path
	if ((nPos = strLabel.ReverseFind(_T('\\'))) >= 0)
		strLabel = strLabel.Right(strLabel.GetLength() - nPos - 1);
	else if ((nPos = strLabel.ReverseFind(_T('/'))) >= 0)
		strLabel = strLabel.Right(strLabel.GetLength() - nPos - 1);

	// throw away extension
	if ((nPos = strLabel.ReverseFind(_T('.'))) >= 0)
		strLabel = strLabel.Left(nPos);

	// set label
	m_strLabel = CString((LPCTSTR) STE_FLOATPREFIX_FIGURE + CInsertFloatObjectDialog::figure) + strLabel;
	if (m_strFile.IsEmpty())
		m_strLabel.Empty();

	UpdateData(FALSE);
}

void CInsertGraphicDialog::OnChangeFloatCaption()
{
	// does nothing!
	// just implemented to prevent CInsertFloatObjectDialog::OnChangeFloatCaption()
	// from changing the label field
}
