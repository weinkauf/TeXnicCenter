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
#include "WildCard.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//-------------------------------------------------------------------
// class CWildcard
//-------------------------------------------------------------------

CWildcard::CWildcard(BOOL bCaseSensitive /*= FALSE*/)
:	m_bCaseSensitive(bCaseSensitive)
{}


CWildcard::CWildcard(LPCTSTR lpszWildcard, BOOL bCaseSensitive /*= FALSE*/)
:	m_bCaseSensitive(bCaseSensitive)
{
	Set(lpszWildcard);
}


CWildcard::~CWildcard()
{}


void CWildcard::Set(LPCTSTR lpszWildcard)
{
	m_strWildcard = lpszWildcard;
	if (!m_bCaseSensitive)
		m_strWildcard.MakeUpper();
}


BOOL CWildcard::Match(LPCTSTR lpszStringToMatch) const
{
	CString	str(lpszStringToMatch);
	if (!m_bCaseSensitive)
		str.MakeUpper();

	return InternalMatch(m_strWildcard, str);
}


BOOL CWildcard::Match(LPCTSTR lpszWildcard, LPCTSTR lpszStringToMatch, BOOL bCaseSensitive /*= FALSE*/)
{
	CString	strWildcard(lpszWildcard);
	CString	strString(lpszStringToMatch);

	if (!bCaseSensitive)
	{
		strWildcard.MakeUpper();
		strString.MakeUpper();
	}

	return InternalMatch(strWildcard, strString);
}


BOOL CWildcard::InternalMatch(LPCTSTR Wildcards, LPCTSTR str)
{
	BOOL Yes = 1;

	//iterate and delete _T('?') and _T('*') one by one
	while(*Wildcards != _T('\0') && Yes && *str != _T('\0'))
	{
		if (*Wildcards == _T('?')) str ++;
		else if (*Wildcards == _T('*'))
		{
			Yes = Scan(Wildcards, str);
			Wildcards --;
		}
		else
		{
			Yes = (*Wildcards == *str);
			str ++;
		}
		Wildcards ++;
	}
	while (*Wildcards == _T('*') && Yes)  Wildcards ++;

	return Yes && *str == _T('\0') && *Wildcards == _T('\0');
}


BOOL CWildcard::Scan(LPCTSTR& Wildcards, LPCTSTR& str)
{
	// remove the _T('?') and _T('*')
	for(Wildcards ++; *str != _T('\0') && (*Wildcards == _T('?') || *Wildcards == _T('*')); Wildcards ++)
		if (*Wildcards == _T('?')) str ++;
	while ( *Wildcards == _T('*')) Wildcards ++;
	
	// if str is empty and Wildcards has more characters or,
	// Wildcards is empty, return 
	if (*str == _T('\0') && *Wildcards != _T('\0')) return FALSE;
	if (*str == _T('\0') && *Wildcards == _T('\0'))	return TRUE; 
	// else search substring
	else
	{
		LPCTSTR wdsCopy = Wildcards;
		LPCTSTR strCopy = str;
		BOOL  Yes     = 1;
		do 
		{
			if (!InternalMatch(Wildcards, str))	strCopy ++;
			Wildcards = wdsCopy;
			str		  = strCopy;
			while ((*Wildcards != *str) && (*str != _T('\0'))) str ++;
			wdsCopy = Wildcards;
			strCopy = str;
		}while ((*str != _T('\0')) ? !InternalMatch(Wildcards, str) : (Yes = FALSE) != FALSE);

		if (*str == _T('\0') && *Wildcards == _T('\0'))	return TRUE;

		return Yes;
	}
}


//-------------------------------------------------------------------
// class CWildcardArray
//-------------------------------------------------------------------

CWildcardArray::CWildcardArray()
:	CArray<CWildcard, CWildcard&>()
{}


BOOL CWildcardArray::Match(LPCTSTR lpszStringToMatch)
{
	for (int i = 0; i < GetSize(); i++)
	{
		if (GetAt(i).Match(lpszStringToMatch))
			return TRUE;
	}

	// found nothing
	return FALSE;
}
