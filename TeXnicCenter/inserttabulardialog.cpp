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
#include "InsertTabularDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CInsertTabularDialog 


CInsertTabularDialog::CInsertTabularDialog(CWnd* pParent /*=NULL*/)
: CInsertFloatObjectDialog(CInsertTabularDialog::IDD, CInsertFloatObjectDialog::table, TRUE, pParent)
{
	//{{AFX_DATA_INIT(CInsertTabularDialog)
	m_bHCenter = TRUE;
	m_bFloat = TRUE;
	m_nPosition = 1;
	//}}AFX_DATA_INIT
}


BOOL CInsertTabularDialog::SetProperties( const CString &strProperties )
{
	if( strProperties.Find( _T("\\begin{tabular}") ) == -1 )
		return FALSE;

	///////////////////////////////////////////////////////////////////
	// is floating
	m_bFloat = CInsertFloatObjectDialog::SetProperties( strProperties );

	///////////////////////////////////////////////////////////////////
	// is horizonally centered
	if ( (strProperties.Find("\\centering") > -1) || (strProperties.Find("\\begin{center}") > -1) )
		m_bHCenter = TRUE;
	else
		m_bHCenter = FALSE;

	///////////////////////////////////////////////////////////////////
	// position
	int				nPos = -1;
	CString		strFind = _T("\\begin{tabular}[");

	if( (nPos = strProperties.Find( strFind )) > -1  )
	{
		nPos+= strFind.GetLength();

		switch( strProperties[nPos] )
		{
			case _T('t'):
				m_nPosition = 0;
				break;
			case _T('b'):
				m_nPosition = 2;
				break;
			default:
				m_nPosition = 1;
		}
	}

	///////////////////////////////////////////////////////////////////	
	// contents
	m_strContents.Empty();

	strFind = _T("\\begin{tabular}");

	if( (nPos = strProperties.Find( strFind )) > -1 )
	{
		nPos+= strFind.GetLength();
		int	nEndPos = -1;
		int	nStartPos = strProperties.Find( _T('{'), nPos );

		if( nStartPos == -1 )
			nStartPos = strProperties.Find( _T('\r'), nPos );

		strFind = _T("\\end{tabular}");

		nEndPos = strProperties.Find( strFind );
		if( nEndPos > -1 )
			for( ; nEndPos >= nStartPos && strProperties[nEndPos] != _T('\r'); nEndPos-- );

		if( nStartPos > -1 && nEndPos > -1 && nEndPos - nStartPos > 0 )
			m_strContents = strProperties.Mid( nStartPos, nEndPos - nStartPos + 1 );
		else
			m_strContents.Empty();
	}

	return TRUE;
}


CString CInsertTabularDialog::GetProperties()
{
	///////////////////////////////////////////////////////////////////
	// statics
	CString	strResult = _T("\\begin{tabular}");

	///////////////////////////////////////////////////////////////////
	// extra position argument?
	if( m_nPosition != 1 )
	{
		strResult+= _T('[');
		if( m_nPosition == 0 )
			strResult+= _T('t');
		else
			strResult+= _T('b');
		strResult+= _T(']');
	}

	///////////////////////////////////////////////////////////////////
	// statics
	if( m_bHCenter && m_bFloat )
		strResult+= (m_strContents.IsEmpty())?
			_T("\r\t\t\t\001\r\t\t\\end{tabular}\r") :
			m_strContents + _T("\t\t\\end{tabular}\r");
	else if( (m_bHCenter && !m_bFloat) || (!m_bHCenter && m_bFloat) )	// xor
		strResult+= (m_strContents.IsEmpty())?
			_T("\r\t\t\001\r\t\\end{tabular}\r"):
			m_strContents + _T("\t\\end{tabular}\r");
	else
		strResult+= (m_strContents.IsEmpty())?
			_T("\r\t\001\r\\end{tabular}\r"):
			m_strContents + _T("\\end{tabular}\r");

	///////////////////////////////////////////////////////////////////
	// is centered?
	if( m_bHCenter )
	{
		if( m_bFloat )
			strResult = _T("\\centering\r\t\t") + strResult;
		else
			strResult = _T("\\begin{center}\r\t") + strResult + _T("\\end{center}\r");
	}

	///////////////////////////////////////////////////////////////////
	// is floating?
	if( m_bFloat )
	{
		CString	strContents = m_strContents;
		CString	strPreResult = strResult;
		m_strContents.Empty();
		strResult = CInsertFloatObjectDialog::GetProperties();
		m_strContents = strContents;
		
		int	nPos = strResult.Find( _T('\001') );
		strResult.Delete( nPos );
		strResult.Insert( nPos, strPreResult );
	}
	else
		strResult.Insert( 0, _T('\r') );


	///////////////////////////////////////////////////////////////////
	// Empty Lines?
	strResult.Replace("\r\r", "\r");

	return strResult;
}


void CInsertTabularDialog::DoDataExchange(CDataExchange* pDX)
{
	CInsertFloatObjectDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInsertTabularDialog)
	DDX_Check(pDX, IDC_TABULAR_CENTER, m_bHCenter);
	DDX_Check(pDX, IDC_TABULAR_FLOAT, m_bFloat);
	DDX_Radio(pDX, IDC_TABULAR_VTOP, m_nPosition);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInsertTabularDialog, CInsertFloatObjectDialog)
	//{{AFX_MSG_MAP(CInsertTabularDialog)
	ON_BN_CLICKED(IDC_TABULAR_FLOAT, OnTabularFloat)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CInsertTabularDialog 
BOOL CInsertTabularDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// enable/disable windows
	EnableFloatOptions( m_bFloat );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void CInsertTabularDialog::OnTabularFloat() 
{
	UpdateData();

	EnableFloatOptions( m_bFloat );
}
