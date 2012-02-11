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
#include "resource.h"
#include "InsertFloatObjectDialog.h"
#include "global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CInsertFloatObjectDialog

CInsertFloatObjectDialog::CInsertFloatObjectDialog(int nType /*= figure*/,CWnd* pParent /*=NULL*/)
		: CDialog(CInsertFloatObjectDialog::IDD,pParent),
		m_bAutoLabel(TRUE)
{
	//m_strCaption = _T("");
	//m_strLabel = _T("");
	m_bPosBottom = TRUE;
	m_bPosHere = FALSE;
	m_bPosPage = TRUE;
	m_bPosTop = TRUE;
	m_nWidth = 0;

	m_nType = nType;
	//CInsertFloatObjectDialog( CInsertFloatObjectDialog::IDD, nType, pParent );
}

CInsertFloatObjectDialog::CInsertFloatObjectDialog(UINT nIDTemplate,int nType,BOOL bAutoLabel /*= TRUE*/,CWnd *pParent /*= NULL*/)
		: CDialog(nIDTemplate,pParent),
		m_bAutoLabel(bAutoLabel)
{
	//m_strCaption = _T("");
	//m_strLabel = _T("");
	m_bPosBottom = TRUE;
	m_bPosHere = FALSE;
	m_bPosPage = TRUE;
	m_bPosTop = TRUE;
	m_nWidth = 0;

	m_nType = nType;
}

BOOL CInsertFloatObjectDialog::SetProperties(const CString &strProperties)
{
	CString strType = AfxLoadString(STE_FLOATTYPE_FIGURE + m_nType);

	// is valid
	if (strProperties.Find(_T("\\begin{") + strType + _T('}')) == -1 &&
	        strProperties.Find(_T("\\begin{") + strType + _T("*}")) == -1)
		return FALSE;

	// width (has star)
	if (strProperties.Find(_T("\\begin{") + strType + _T("*}")) > -1)
		m_nWidth = 1;
	else
		m_nWidth = 0;

	// position
	int nPos = -1;
	if ((nPos = strProperties.Find(_T("\\begin{") + strType + (m_nWidth ? _T("*}[") : _T("}[")))) > -1)
	{
		nPos = strProperties.Find(_T('['),nPos);
		if (nPos > -1)
		{
			m_bPosHere = m_bPosTop = m_bPosBottom = m_bPosPage = FALSE;

			for (++nPos; nPos < strProperties.GetLength() && strProperties[nPos] != _T(']'); nPos++)
			{
				switch (strProperties[nPos])
				{
					case _T('h') :
						m_bPosHere = TRUE;
						break;
					case _T('t') :
						m_bPosTop = TRUE;
						break;
					case _T('b') :
						m_bPosBottom = TRUE;
						break;
					case _T('p') :
						m_bPosPage = TRUE;
						break;
				}
			}
		}
	}

	// caption
	CString strFind = _T("\\caption{");

	if ((nPos = strProperties.Find(strFind)) > -1)
	{
		m_strCaption.Empty();

		for (nPos += strFind.GetLength(); nPos < strProperties.GetLength(); nPos++)
		{
			if (strProperties[nPos] == _T('}'))
				break;
			else
				m_strCaption += strProperties[nPos];
		}

		// label
		strFind = _T("\\label{");
		if ((nPos = strProperties.Find(strFind)) > -1)
		{
			m_strLabel.Empty();

			for (nPos += strFind.GetLength(); nPos < strProperties.GetLength(); nPos++)
			{
				if (strProperties[nPos] == _T('}'))
					break;
				else
					m_strLabel += strProperties[nPos];
			}
		}
	}

	// contents
	strFind = _T("\\begin{") + strType;
	if ((nPos = strProperties.Find(strFind)) > -1)
	{
		int nStartPos = strProperties.Find(_T('\r'));
		int nEndPos = -1;

		if (nStartPos == -1)
			return TRUE;

		if (!m_strCaption.IsEmpty())
			strFind = _T("\\caption");
		else
			strFind = _T("\\end{") + strType;

		nEndPos = strProperties.Find(strFind);
		if (nEndPos > -1)
			for (; nEndPos >= nStartPos && strProperties[nEndPos] != _T('\r'); nEndPos--);

		if (nEndPos - nStartPos > 0)
			m_strContents = strProperties.Mid(nStartPos,nEndPos - nStartPos + 1);
		else
			m_strContents.Empty();
	}

	return TRUE;
}

CString CInsertFloatObjectDialog::GetProperties()
{
	// name of environment ("figure" or "table")
	CString strType = AfxLoadString(STE_FLOATTYPE_FIGURE + m_nType);

	// has star?
	if (m_nWidth)
		strType += _T('*');

	CString strResult = _T("\r\\begin{") + strType + _T("}");

	// Position
	// if not default
	if (!(!m_bPosHere && m_bPosTop && m_bPosBottom && m_bPosPage))
	{
		strResult += _T('[');
		if (m_bPosHere)
			strResult += _T('h');
		if (m_bPosTop)
			strResult += _T('t');
		if (m_bPosBottom)
			strResult += _T('b');
		if (m_bPosPage)
			strResult += _T('p');
		strResult += _T(']');
	}

	// statics
	if (!m_strContents.IsEmpty())
		strResult += m_strContents;
	else
		strResult += _T("\r\t\001\r");


	// caption
	if (!m_strCaption.IsEmpty())
		strResult += _T("\t\\caption{") + m_strCaption + _T("}\r");

	// label
	if (!m_strLabel.IsEmpty())
		strResult += _T("\t\\label{") + m_strLabel + _T("}\r");

	// statics
	strResult += _T("\\end{") + strType + _T("}\r");

	return strResult;
}

void CInsertFloatObjectDialog::EnableFloatOptions(BOOL bEnable /*= TRUE*/)
{
	m_wndCaptionEdit.EnableWindow(bEnable);
	m_wndLabelEdit.EnableWindow(bEnable);
	m_wndPosBottom.EnableWindow(bEnable);
	m_wndPosHere.EnableWindow(bEnable);
	m_wndPosPage.EnableWindow(bEnable);
	m_wndPosTop.EnableWindow(bEnable);
	m_wndGroupPos.EnableWindow(bEnable);
	m_wndGroupWidth.EnableWindow(bEnable);
	m_wndLabelCaption.EnableWindow(bEnable);
	m_wndLabelLabel.EnableWindow(bEnable);
	m_wndPosOneCol.EnableWindow(bEnable);
	m_wndPosTwoCol.EnableWindow(bEnable);
}

void CInsertFloatObjectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInsertFloatObjectDialog)
	DDX_Control(pDX,IDC_FLOAT_POSTWOCOL,m_wndPosTwoCol);
	DDX_Control(pDX,IDC_FLOAT_POSONECOL,m_wndPosOneCol);
	DDX_Control(pDX,IDC_LABELLABEL,m_wndLabelLabel);
	DDX_Control(pDX,IDC_LABELCAPTION,m_wndLabelCaption);
	DDX_Control(pDX,IDC_GROUPWIDTH,m_wndGroupWidth);
	DDX_Control(pDX,IDC_GROUPPOS,m_wndGroupPos);
	DDX_Control(pDX,IDC_FLOAT_POSTOP,m_wndPosTop);
	DDX_Control(pDX,IDC_FLOAT_POSPAGE,m_wndPosPage);
	DDX_Control(pDX,IDC_FLOAT_POSHERE,m_wndPosHere);
	DDX_Control(pDX,IDC_FLOAT_POSBOTTOM,m_wndPosBottom);
	DDX_Control(pDX,IDC_FLOAT_CAPTION,m_wndCaptionEdit);
	DDX_Control(pDX,IDC_FLOAT_LABEL,m_wndLabelEdit);
	DDX_Text(pDX,IDC_FLOAT_CAPTION,m_strCaption);
	DDX_Text(pDX,IDC_FLOAT_LABEL,m_strLabel);
	DDX_Check(pDX,IDC_FLOAT_POSBOTTOM,m_bPosBottom);
	DDX_Check(pDX,IDC_FLOAT_POSHERE,m_bPosHere);
	DDX_Check(pDX,IDC_FLOAT_POSPAGE,m_bPosPage);
	DDX_Check(pDX,IDC_FLOAT_POSTOP,m_bPosTop);
	DDX_Radio(pDX,IDC_FLOAT_POSONECOL,m_nWidth);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInsertFloatObjectDialog,CDialog)
	//{{AFX_MSG_MAP(CInsertFloatObjectDialog)
	ON_EN_CHANGE(IDC_FLOAT_CAPTION,OnChangeFloatCaption)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CInsertFloatObjectDialog

BOOL CInsertFloatObjectDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set Title
	SetWindowText(AfxLoadString(STE_FLOATTITLE_INSERT_FIGURE + m_nType));

	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void CInsertFloatObjectDialog::OnChangeFloatCaption()
{
	UpdateData();

	// auto generate label?
	if (!m_bAutoLabel)
		return;

	// auto generate label
	BOOL bWhiteSpace = FALSE;
	TCHAR strC[2] = _T(" ");

	m_strLabel = AfxLoadString(STE_FLOATPREFIX_FIGURE + m_nType);

	for (int i = 0; i < m_strCaption.GetLength(); i++)
	{
		if (!_istalnum(m_strCaption[i]))
			bWhiteSpace = TRUE;
		else
		{
			strC[0] = m_strCaption[i];
			m_strLabel += (bWhiteSpace ? _tcsupr(strC) : strC);
			bWhiteSpace = FALSE;
		}
	}

	if (m_strCaption.IsEmpty())
		m_strLabel.Empty();

	UpdateData(FALSE);
}
