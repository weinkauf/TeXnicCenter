////////////////////////////////////////////////////////////////////////////
//	File:		CrystalTextBufferEx.cpp
//
//	Author:		Sven Wiegand
//	E-mail:		sven.wiegand@gmx.de
//
//	Implementation of the CCrystalTextBufferEx class, a part of Crystal Edit -
//	syntax coloring text editor.
//
//	You are free to use or modify this code.
//
////////////////////////////////////////////////////////////////////////////

/********************************************************************
*
* $RCSfile$
* $Revision$
* $Date$
*
* $Author$
*
* $Log$
* Revision 1.0  2000-05-31 21:55:37+02  sven_wiegand
* Initial revision
*
*
********************************************************************/

#include "stdafx.h"
#include "CrystalTextBufferEx.h"

IMPLEMENT_DYNCREATE(CCrystalTextBufferEx,CCrystalTextBuffer)


CCrystalTextBufferEx::CCrystalTextBufferEx( CDocument *pDoc )
{
	ASSERT( pDoc );
	m_pDoc = pDoc;
}


void CCrystalTextBufferEx::SetModified( BOOL bModified )
{
	ASSERT( m_pDoc );
	if( !m_pDoc )
		return;

	m_pDoc->SetModifiedFlag( bModified );
}


BOOL CCrystalTextBufferEx::GetLine( int nLineIndex, CString &strLine )
{
	int		nLineLength = CCrystalTextBuffer::GetLineLength( nLineIndex );

	if( nLineLength <= 0 )
		return FALSE;

	strncpy( strLine.GetBuffer( nLineLength ), CCrystalTextBuffer::GetLineChars( nLineIndex ), nLineLength );
	strLine.ReleaseBuffer( nLineLength );

	return TRUE;
}