/********************************************************************
*
* This file is part of the MFCExt-Library
*
* Copyright (C) 1999-2000 Sven Wiegand
* Copyright (C) 2000-2002 ToolsCenter
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
#include "UniqueStringList.h"


CUniqueStringList::CUniqueStringList()
:bIgnoreCase(true)
{
}


CUniqueStringList::~CUniqueStringList()
{
}


///Overridden to prevent identical strings
POSITION CUniqueStringList::AddHead(const CString& newElement)
{
	if (FindString(newElement) == NULL)
		return CStringList::AddHead(newElement);
	else
		return 0;
}


///Overridden to prevent identical strings
POSITION CUniqueStringList::AddTail(const CString& newElement)
{
	if (FindString(newElement) == NULL)
		return CStringList::AddTail(newElement);
	else
		return 0;
}


///Overridden to prevent identical strings
POSITION CUniqueStringList::InsertBefore(POSITION position, const CString& newElement)
{
	if (FindString(newElement) == NULL)
		return CStringList::InsertBefore(position, newElement);
	else
		return 0;
}


///Overridden to prevent identical strings
POSITION CUniqueStringList::InsertAfter(POSITION position, const CString& newElement)
{
	if (FindString(newElement) == NULL)
		return CStringList::InsertAfter(position, newElement);
	else
		return 0;
}


///Removes every String from this list, if it could be found in the given list.
void CUniqueStringList::SubtractList(const CStringList& cmpStrList)
{
	POSITION pos, pos2;

	//Iterate through the given list
	pos = cmpStrList.GetHeadPosition();
	while (pos != NULL)
	{
		//Get the position of an identical string
		pos2 = FindString(cmpStrList.GetAt(pos));
		if (pos2 != NULL)
		{
			RemoveAt(pos2); //Remove the duplicate in this list.
		}

		//We do not need to look deeper into *this* list,
		// because it should be unique at any time.
		cmpStrList.GetNext(pos);
	}
}


CString CUniqueStringList::FlattenList(LPCTSTR lpszBefore,
																			 LPCTSTR lpszAfter,
																			 LPCTSTR lpszSeparator,
																			 bool bTrimSpaces /*= true*/) const
{
	CString strBefore = "";
	CString strAfter = "";
	CString strSeparator = "";
	CString retStr = "";
	POSITION pos;

	//Do we have NULL pointers?
	if (lpszBefore) strBefore = lpszBefore;
	if (lpszAfter) strAfter = lpszAfter;
	if (lpszSeparator) strSeparator = lpszSeparator;

	//Iterate through the list
	pos = GetHeadPosition();
	while (pos != NULL)
	{
		//Format this entry
		retStr += strBefore + GetAt(pos) + strAfter + strSeparator;

		//Next
		GetNext(pos);
	}

	//Trim?
	if (bTrimSpaces)
	{
		retStr.TrimLeft();
		retStr.TrimRight();
	}

	return retStr;
}


/**
Returns, whether the given String already exists in the list.
Returns NULL, if it does not exist.
Returns the position in the list, if it exists.

We need this function, because CStringList::Find() is only case sensitive.

@see bIgnoreCase
*/
POSITION CUniqueStringList::FindString(const CString& strToCmp)
{
	POSITION pos;

	pos = GetHeadPosition();
	while (pos != NULL)
	{
		if ( bIgnoreCase ? !strToCmp.CompareNoCase(GetAt(pos)) : !strToCmp.Compare(GetAt(pos)))
			return pos;

		GetNext(pos);
	}

	//If we came here, we did not find a matching string in the list
	return NULL;
}
