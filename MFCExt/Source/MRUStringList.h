/********************************************************************
*
* This file is part of the MFCExt-Library
*
* Copyright (C) 1999-2000 Sven Wiegand
* Copyright (C) 2000-2001 ToolsCenter
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

#if !defined(AFX_MRUSTRINGLIST_H__C8D50DC5_2052_11D4_A221_006097239934__INCLUDED_)
#define AFX_MRUSTRINGLIST_H__C8D50DC5_2052_11D4_A221_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MRUList.h"

/**
An object of this class is a list of most recently used strings.

You can use such a list for example to store the most recently used
query in a search dialog or something like this.

The list can be stored to and loaded from the registry.

For further information see the template class CMRUList.

@author Sven Wiegand
*/
class AFX_EXT_CLASS CMRUStringList : public CMRUList<CString, LPCTSTR>  
{
// construction/destruction
public:
	/**
	Constructs a CMRUStringList-object.

	@param lpszSection
		Section in the registry, where the list is written to and load
		from.
	@param lpszNameFormat
		String, describing, how to name the strings while writing them
		to the registry or reading them from the registry. This string
		must contain a "%d" which will be replaced by the zero based
		index of each element when this element is written to or read
		from the registry. If the string is i.e. "SearchQuery%d", then
		the first element of the list will be stored with the name
		"SearchQuery1", the second one with the name "SearchQuery2", etc.
	@param nSize
		Number of elements that can be stored in the list. If the list
		is full and a new element is added, the least recently added
		element is removed from the list.
	*/
	CMRUStringList( LPCTSTR lpszSection, LPCTSTR lpszNameFormat, int nSize );

// operations
public:
	/** 
	Reads the list from the registry.

	The section from where the list should be read is specified by
	the lpszSection-parameter at construction.

	@return
		TRUE if the list has been read, FALSE otherwise.
	*/
	virtual BOOL ReadList();

	/** 
	Writes the list to the registry.

	The section to where the list should be written is specified by
	the lpszSection-parameter at construction.

	@return
		TRUE if the list has been written, FALSE otherwise.
	*/
	virtual BOOL WriteList();

// overridings
public:
	void Add( LPCTSTR lpszElement );

// attributes
protected:
	/** Section in the registry to access for writing/reading. */
	CString m_strSection;

	/** Name format for writing and reading the list entries. */
	CString m_strNameFormat;
};


// inlines
inline
void CMRUStringList::Add( LPCTSTR lpszElement )
{
	if( CString( lpszElement ).IsEmpty() )
		return;	// do not add empty strings to the list

	CMRUList<CString, LPCTSTR>::Add( lpszElement );
}


#endif // !defined(AFX_MRUSTRINGLIST_H__C8D50DC5_2052_11D4_A221_006097239934__INCLUDED_)
