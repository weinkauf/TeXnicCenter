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

#include "stdafx.h"
#include "stdafx.h"
#include "TextSourceFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// construction/destruction
CTextSourceFile::CTextSourceFile()
:	m_pAr( NULL ),
	m_bFileOpen( FALSE )
{
}

CTextSourceFile::~CTextSourceFile()
{
	try
	{
		if( m_pAr )
			m_pAr->Close();
	}
	catch( ... )
	{}

	try
	{
		if( m_bFileOpen )
			m_file.Close();
	}
	catch( ... )
	{}

	if( m_pAr )
		delete m_pAr;
}

//////////////////////////////////////////////////////////////////////
// operations
BOOL CTextSourceFile::Create( LPCTSTR lpszFile )
{
	// create file
	if( !(m_bFileOpen = m_file.Open( lpszFile, CFile::modeRead | CFile::shareDenyNone )) )
		return FALSE;

	// create archiv
	try
	{
		m_pAr = new CArchive( &m_file, CArchive::load );
	}
	catch( ... )
	{
		return FALSE;
	}

	return TRUE;
}


void CTextSourceFile::Delete()
{
	delete this;
}

BOOL CTextSourceFile::GetNextLine( LPCTSTR &lpLine, int &nLength )
{
	ASSERT( m_pAr );

	m_strCurrentLine.Empty();

	if( m_pAr->ReadString( m_strCurrentLine ) )
	{
		nLength = m_strCurrentLine.GetLength();
		lpLine = m_strCurrentLine;
		return TRUE;
	}
	else
		return FALSE;
}