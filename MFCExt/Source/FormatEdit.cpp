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
#include "FormatEdit.h"

#include <limits.h>
#include <locale.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// CFormatEdit
//-------------------------------------------------------------------


CFormatEdit::CFormatEdit( LPCTSTR lpszCharSet /*= NULL*/ )
: CEdit()
{
	SetCharSet( lpszCharSet );
}


BEGIN_MESSAGE_MAP(CFormatEdit, CEdit)
	//{{AFX_MSG_MAP(CFormatEdit)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CFormatEdit::SetCharSet( LPCTSTR lpszCharSet )
{
	if( !lpszCharSet )
		m_strCharSet.Empty();
	else
		m_strCharSet = lpszCharSet;
}


void CFormatEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// Bestimmte Zeichen immer durchlassen
	if( iscntrl( nChar ) )
	{
		CEdit::OnChar( nChar, nRepCnt, nFlags );
		return;
	}

	// Ueberpruefen, ob Zeichen zulaessig ist
	if( m_strCharSet.IsEmpty() || m_strCharSet.Find( (TCHAR)nChar ) > -1 )
	{
		CEdit::OnChar( nChar, nRepCnt, nFlags );
		return;
	}

	// Nichtzulaessigen Zeichen ignorieren
	MessageBeep( MB_ICONSTOP );
}


//-------------------------------------------------------------------
// CNumberEdit
//-------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CNumberEdit, CFormatEdit)
	//{{AFX_MSG_MAP(CNumberEdit)
	ON_WM_KEYDOWN()
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CNumberEdit::CNumberEdit( int nType /*= ntLong*/ )
: CFormatEdit(),
	m_nSelStart( 0 ),
	m_nSelEnd( 0 )
{
	SetType( nType );
}


CNumberEdit::CNumberEdit( TCHAR cMin, TCHAR cMax )
: CFormatEdit(),
	m_nSelStart( 0 ),
	m_nSelEnd( 0 )

{
	SetRange( cMin, cMax );
}


CNumberEdit::CNumberEdit( UCHAR ucMin, UCHAR ucMax )
: CFormatEdit(),
	m_nSelStart( 0 ),
	m_nSelEnd( 0 )
{
	SetRange( ucMin, ucMax );
}


CNumberEdit::CNumberEdit( short sMin, short sMax )
: CFormatEdit(),
	m_nSelStart( 0 ),
	m_nSelEnd( 0 )
{
	SetRange( sMin, sMax );
}


CNumberEdit::CNumberEdit( USHORT usMin, USHORT usMax )
: CFormatEdit(),
	m_nSelStart( 0 ),
	m_nSelEnd( 0 )
{
	SetRange( usMin, usMax );
}


CNumberEdit::CNumberEdit( long lMin, long lMax )
: CFormatEdit(),
	m_nSelStart( 0 ),
	m_nSelEnd( 0 )
{
	SetRange( lMin, lMax );
}


CNumberEdit::CNumberEdit( double dMin, double dMax )
: CFormatEdit(),
	m_nSelStart( 0 ),
	m_nSelEnd( 0 )
{
	SetRange( dMin, dMax );
}


void CNumberEdit::SetType( int nType )
{
	switch( nType )
	{
		case ntChar:
			SetRange( (TCHAR)CHAR_MIN, (TCHAR)CHAR_MAX );
			break;
		case ntUChar:
			SetRange( (UCHAR)0, (UCHAR)UCHAR_MAX );
			break;
		case ntShort:
			SetRange( (short)SHRT_MIN, (SHORT)SHRT_MAX );
			break;
		case ntUShort:
			SetRange( (USHORT)0, (USHORT)USHRT_MAX );
			break;
		case ntLong:
			SetRange( (long)LONG_MIN, (long)LONG_MAX );
			break;

		default:
			SetType( ntLong );
	}
}


void CNumberEdit::SetRange( TCHAR cMin, TCHAR cMax )
{
	SetType( ntChar, (long)cMin, (ULONG)cMax );
}


void CNumberEdit::SetRange( UCHAR ucMin, UCHAR ucMax )
{
	SetType( ntUChar, (long)ucMin, (ULONG)ucMax );
}


void CNumberEdit::SetRange( short sMin, short sMax )
{
	SetType( ntShort, (long)sMin, (ULONG)sMax );
}


void CNumberEdit::SetRange( USHORT usMin, USHORT usMax )
{
	SetType( ntUShort, (long)usMin, (ULONG)usMax );
}


void CNumberEdit::SetRange( long lMin, long lMax )
{
	SetType( ntLong, (long)lMin, (ULONG)lMax );
}


void CNumberEdit::SetRange( double dMin, double dMax )
{
	SetType( ntDouble, dMin, dMax );
}


void CNumberEdit::SetType( int nType, long lMin, long lMax )
{
	m_nType = nType;

	m_lMin = lMin;
	m_lMax = lMax;
	m_dMin = (double)lMin;
	m_dMax = (double)lMax;

	Prepare();
}

void CNumberEdit::SetType( int nType, double dMin, double dMax )
{
	m_nType = nType;

	m_lMin = (long)dMin;
	m_lMax = (long)dMax;
	m_dMin = dMin;
	m_dMax = dMax;

	Prepare();
}


void CNumberEdit::Prepare()
{
	if( m_nType == ntDouble )
	{
		struct lconv	*pConv = localeconv();
		CString				charSet( _T("1234567890") );

		if( m_dMin < 0.0 || m_dMax < 0.0 )
			charSet+= _T('-');

		SetCharSet( charSet + pConv->decimal_point );
	}
	else
	{
		CString	charSet( _T("1234567890") );
		if( m_lMin < 0 || m_lMax < 0 )
			charSet+= _T('-');

		SetCharSet( charSet );
	}
}


void CNumberEdit::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	// aktuellen Text und aktuelle Position ermitteln
	GetSel( m_nSelStart, m_nSelEnd );
	GetWindowText( m_strText );

	CFormatEdit::OnKeyDown( nChar, nRepCnt, nFlags );
}


void CNumberEdit::OnChange() 
{
	CString	strNew;

	GetWindowText( strNew );

	if( m_nType == ntDouble )
	{
		double	dVal = _tstof( strNew );

		if( dVal >= m_dMin && dVal <= m_dMax )
			return;
	}
	else
	{
		long		lVal = _tstol( strNew );

		if( lVal >= m_lMin && lVal <= m_lMax )
			return;
	}

	// Ausserhalb des Wertebereichs!
	// Alten Wert wiederherstellen
	SetWindowText( m_strText );
	SetSel( m_nSelStart, m_nSelEnd );

	MessageBeep( MB_ICONSTOP );
}
