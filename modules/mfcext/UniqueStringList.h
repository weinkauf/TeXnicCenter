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

#ifndef __UNIQUESTRINGLIST_H__
#define __UNIQUESTRINGLIST_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
A StringList, which contains only unique strings.
Before adding a string to the list, it checks, whether
this string is already present in the list. If yes, the string
will not be added again. If not, the StringList behaves like
the CStringList from the MFC.

Strings can be compared either with respect to case or not.
This is defined by the attribute bIgnoreCase, which defaults to true.

@ingroup mfcext

@author Tino Weinkauf
*/
class  CUniqueStringList : public CStringList
{
public:
	///Standard Constructor
	CUniqueStringList();
	virtual ~CUniqueStringList();

//Operations
public:
	///Overridden to prevent identical strings
	POSITION AddHead(const CString& newElement);
	///Overridden to prevent identical strings
	POSITION AddTail(const CString& newElement);
	///Overridden to prevent identical strings
	POSITION InsertBefore(POSITION position, const CString& newElement);
	///Overridden to prevent identical strings
	POSITION InsertAfter(POSITION position, const CString& newElement);

	///Removes every String from this list, if it could be found in the given list.
	void SubtractList(const CStringList& cmpStrList);

	/**
	Returns a string with all list entries in it.
	Will be formatted the following way:

	[lpszBefore][entry0][lpszAfter][lpszSeparator][lpszBefore][entry1][lpszAfter][lpszSeparator]...
	*/
	CString FlattenList(LPCTSTR lpszBefore, LPCTSTR lpszAfter,
											LPCTSTR lpszSeparator, bool bTrimSpaces = true) const;

protected:
	/**
	Returns, whether the given String already exists in the list.
	Returns NULL, if it does not exist.
	Returns the position in the list, if it exists.

	We need this function, because CStringList::Find() is only case sensitive.

	@see bIgnoreCase
	*/
	POSITION FindString(const CString& strToCmp);

//Attributes
public:
	/**
	If false, strings are compared case sensitive.
	Default: true == case insensitive
	*/
	bool bIgnoreCase;

};
#endif //__UNIQUESTRINGLIST_H__
