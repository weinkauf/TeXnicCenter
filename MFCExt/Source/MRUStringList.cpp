/********************************************************************
*
* This file is part of the MFCExt-Library
*
* Copyright (C) 1999-2000 Sven Wiegand
* Copyright (C) 2000-$CurrentYear$ ToolsCenter
* 
* This library is free software; you can redistribute it and/or
* modify, but leave the headers intact and do not remove any 
* copyrights from the source.
*
* This library does not only contain file from us, but also from
* third party developers. Look at the source file headers to get
* detailed information.
*
* If you have further questions visit our homepage
*
*    http://www.ToolsCenter.org
*
********************************************************************/

/********************************************************************
*
* $Id$
*
********************************************************************/

#include "stdafx.h"
#include "MRUStringList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//-------------------------------------------------------------------
// class CMRUStringList
//-------------------------------------------------------------------


CMRUStringList::CMRUStringList( LPCTSTR lpszSection, LPCTSTR lpszNameFormat, int nSize )
:	CMRUList<CString, LPCTSTR>( nSize ),
	m_strSection( lpszSection ),
	m_strNameFormat( lpszNameFormat )
{
	ASSERT( nSize > 0 );
	ASSERT( lpszSection );
	ASSERT( !m_strSection.IsEmpty() );
	ASSERT( !m_strNameFormat.IsEmpty() );
	ASSERT( m_strNameFormat.Find( _T("%d") ) > -1 );
}


BOOL CMRUStringList::ReadList()
{
	ASSERT( !m_strSection.IsEmpty() );
	ASSERT( !m_strNameFormat.IsEmpty() );
	ASSERT( m_strNameFormat.Find( _T("%d") ) > -1 );

	CString	strValueName;
	CWinApp	*pApp = AfxGetApp();
	ASSERT( pApp );
	if( !pApp )
		return FALSE;

	// read all elements
	int i = 0;
	for( ; i < GetSize(); i++ )
	{
		strValueName.Format( m_strNameFormat, i + 1 );
		SetAt( i, pApp->GetProfileString( m_strSection, strValueName, _T("") ) );
	}

	// get number of elements
	for( i = 0; i < GetSize() && !GetAt( i ).IsEmpty(); i++ );

	m_nElementCount = i;

	return TRUE;
}


BOOL CMRUStringList::WriteList()
{
	ASSERT( !m_strSection.IsEmpty() );
	ASSERT( !m_strNameFormat.IsEmpty() );
	ASSERT( m_strNameFormat.Find( _T("%d") ) > -1 );

	CString	strValueName;
	CWinApp	*pApp = AfxGetApp();
	ASSERT( pApp );
	if( !pApp )
		return FALSE;

	// delete key in the registry
	pApp->WriteProfileString( m_strSection, NULL, NULL );

	// write all elements
	for( int i = 0; i < GetElementCount(); i++ )
	{
		strValueName.Format( m_strNameFormat, i + 1 );
		pApp->WriteProfileString( m_strSection, strValueName, GetAt( i ) );
	}

	return TRUE;
}
