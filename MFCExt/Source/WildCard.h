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

#if !defined(AFX_WILDCARD_H__4D69DAD9_FF4B_11D4_A222_006097239934__INCLUDED_)
#define AFX_WILDCARD_H__4D69DAD9_FF4B_11D4_A222_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include<afxtempl.h>

/** 
Class for doing simple wildcard matching.

Supported wilcards are:
<ul>
	<li>? - matches every character (exactly one)</li>
	<li>* - matches none or more characters</li>
</ul>

@ingroup mfcext

@author Sven Wiegand (based on match algorithm by Yuantu Huang <yuantuh@techsim.com.au>)
*/
class AFX_EXT_CLASS CWildcard
{
// construction/destruction
public:
	CWildcard(BOOL bCaseSensitive = FALSE);
	CWildcard(LPCTSTR lpszWildcard, BOOL bCaseSensitive = FALSE);
	virtual ~CWildcard();

// operators
public:
	operator LPCTSTR() const;
	CWildcard &operator=(LPCTSTR lpszWildcard);
	BOOL operator==(LPCTSTR lpszStringToMatch) const;

// operations
public:
	void Set(LPCTSTR lpszWildcard);
	BOOL Match(LPCTSTR lpszStringToMatch) const;

// static operation
public:
	static BOOL Match(LPCTSTR lpszWildcard, LPCTSTR lpszStringToMatch, BOOL bCaseSensitive = FALSE);

// implementation
protected:
	static BOOL InternalMatch(LPCTSTR lpszWildcard, LPCTSTR lpszStringToMatch);
	static BOOL Scan(LPCTSTR &lpszWildcard, LPCTSTR &lpszStringToMatch);

	// attributes
private:
	/**
	<var>TRUE</var> to perform case sensitives math, <var>FALSE</var>
	oterhwise.
	*/
	BOOL m_bCaseSensitive;

	/** The wilcard string */
	CString m_strWildcard;
};


/**
An array of <var>CWildcard</var>-objects.

The class allows matching one string with all elements in the array.

@author Sven Wiegand

@see #CWildcard
*/
class AFX_EXT_CLASS CWildcardArray : public CArray<CWildcard, CWildcard&>
{
// construction/destruction
public:
	CWildcardArray();

// matching
public:
	/**
	Matches the given string with all elements in the list.

	@param lpszStringToMatch
		String to match with the elements in the list.

	@return
		Returns <var>TRUE</var> if the given string matches at least one
		element in the list and <var>FALSE</var> if the string does not
		match any elements.
	*/
	BOOL Match(LPCTSTR lpszStringToMatch);
};


// inlines

//-------------------------------------------------------------------
// class CWildcard
//-------------------------------------------------------------------

inline
CWildcard::operator LPCTSTR() const
{
	return m_strWildcard;
}


inline
CWildcard &CWildcard::operator=(LPCTSTR lpszWildcard)
{
	Set(lpszWildcard);
	return *this;
}


inline
BOOL CWildcard::operator==(LPCTSTR lpszStringToMatch) const
{
	return Match(lpszStringToMatch);
}


#endif // !defined(AFX_WILDCARD_H__4D69DAD9_FF4B_11D4_A222_006097239934__INCLUDED_)
