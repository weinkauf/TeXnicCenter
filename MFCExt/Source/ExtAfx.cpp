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

/////////////////////////////////////////////////////////////////////
//
// global helper functions
//
// author: Sven Wiegand
//
/////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../include/ExtAfx.h"

CString AfxLoadStringEx( UINT unID )
{
	CString	str( (LPCTSTR)unID );
	return str;
}


CString AfxFormatString( UINT unID, LPCTSTR lpszText )
{
	CString	str;
	str.Format( unID, lpszText );
	return str;
}


CString AfxFormatString( UINT unID, int n )
{
	CString	str;
	str.Format( unID, n );
	return str;
}


CString AfxFormatString( UINT unID, double d )
{
	CString	str;
	str.Format( unID, d );
	return str;
}


int MessageBoxEx( UINT unPromptID, UINT unTitleID, UINT unType /*= MB_OK*/, CWnd *pwndParent /*= NULL*/ )
{
	return MessageBox( 
		pwndParent? (IsWindow( pwndParent->m_hWnd )? pwndParent->m_hWnd : NULL) : NULL,
		AfxLoadStringEx( unPromptID ), AfxLoadStringEx( unTitleID ), unType );
}


int MessageBoxEx( LPCTSTR lpszPrompt, UINT unTitleID, UINT unType /*= MB_OK*/, CWnd *pwndParent /*= NULL*/ )
{
	return MessageBox( 
		pwndParent? (IsWindow( pwndParent->m_hWnd )? pwndParent->m_hWnd : NULL) : NULL,
		lpszPrompt, AfxLoadStringEx( unTitleID ), unType );
}
