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

#if !defined(AFX_SORTARRAY_H__A010A5E5_1F74_11D4_A221_006097239934__INCLUDED_)
#define AFX_SORTARRAY_H__A010A5E5_1F74_11D4_A221_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
Array with sorting capabilities.

The objects specified to store must provide operators
for comparison.

@ingroup mfcext

@author Sven Wiegand
*/
template<class TYPE, class ARG_TYPE>
class AFX_EXT_CLASS CSortArray : public CArray<TYPE, ARG_TYPE>  
{
// construction/destruction
public:
	CSortArray();

// operations
public:
	/**
	Inserts an element at his sorting position in the array.

	<b>Remarks:</b> This methode assumes that the array is sorted
	so far.

	@param newElement
		Element to insert in the array.
	@param bSortAscending
		TRUE if the element should be inserted in ascending order,
		FALSE if it should be inserted in descending order.
	@param bAllowDuplicate
		TRUE if the same element can be more than one times in the
		array, FALSE otherwise.

	@return
		The index of the new element in the array or -1 if the string
		has not been inserted.
	*/
	virtual int InsertSorted( ARG_TYPE newElement, BOOL bSortAscending = TRUE, BOOL bAllowDuplicate = FALSE );

// implementation helpers
protected:
	/**
	Simple recursive search.
	*/
	int InsertSorted( int nStart, int nEnd, ARG_TYPE newElement, BOOL bSortAscending, BOOL bAllowDuplicate );
};


// implementation
template<class TYPE, class ARG_TYPE>
CSortArray<TYPE, ARG_TYPE>::CSortArray()
:	CArray<TYPE, ARG_TYPE>()
{}


template<class TYPE, class ARG_TYPE>
int CSortArray<TYPE, ARG_TYPE>::InsertSorted( 
	ARG_TYPE newElement, BOOL bSortAscending /*= TRUE*/, BOOL bAllowDuplicate /*= FALSE*/ )
{
	return InsertSorted( 0, GetUpperBound(), newElement, bSortAscending, bAllowDuplicate );
}


template<class TYPE, class ARG_TYPE>
int CSortArray<TYPE, ARG_TYPE>::InsertSorted(
	int nStart, int nEnd, ARG_TYPE newElement, BOOL bSortAscending, BOOL bAllowDuplicate )
{
	// if array is empty, append to end
	if( !GetSize() )
		return Add( newElement );

	// insert sorted
	ASSERT( nStart >= 0 && nStart < GetSize() );
	ASSERT( nEnd >= 0 && nEnd < GetSize() );

	// get position to compare with
	int	nLookAt = nStart + (nEnd - nStart) / 2;

	if( nStart == nEnd )
	{
		if( newElement == GetAt( nLookAt ) )
		{
			if( !bAllowDuplicate )
				return -1;
			else
			{
				InsertAt( nLookAt, newElement );
				return nLookAt;
			}
		}
		else if( newElement > GetAt( nLookAt ) )
		{
			// new element is greater
			if( bSortAscending )
				nLookAt++;

			InsertAt( nLookAt, newElement );
			return nLookAt;
		}
		else if( newElement < GetAt( nLookAt ) )
		{
			// new element is lower
			if( !bSortAscending )
				nLookAt++;

			InsertAt( nLookAt, newElement );
			return nLookAt;
		}

		// we never should reach this point
		ASSERT( FALSE );
		return -1;
	}
	else if (nEnd == (nStart + 1))
	{
		if (bSortAscending)
		{
			if (newElement <= GetAt(nStart))
				return InsertSorted(nStart, nStart, newElement, bSortAscending, bAllowDuplicate);
			else
				return InsertSorted(nEnd, nEnd, newElement, bSortAscending, bAllowDuplicate);
		}
		else
		{
			if (newElement <= GetAt(nEnd))
				return InsertSorted(nEnd, nEnd, newElement, bSortAscending, bAllowDuplicate);
			else
				return InsertSorted(nStart, nStart, newElement, bSortAscending, bAllowDuplicate);
		}
	}
	else if( newElement == GetAt( nLookAt ) )
	{
		// elements are equal
		if( !bAllowDuplicate )
			return -1;
		else
		{
			InsertAt( nLookAt, newElement );
			return nLookAt;
		}
	}
	else if( newElement > GetAt( nLookAt ) )
	{
		// new element is greater
		if( bSortAscending )
			return InsertSorted( nLookAt, nEnd, newElement, bSortAscending, bAllowDuplicate );
		else
			return InsertSorted( nStart, nLookAt, newElement, bSortAscending, bAllowDuplicate );
	}
	else if( newElement < GetAt( nLookAt ) )
	{
		// new element is lower
		if( bSortAscending )
			return InsertSorted( nStart, nLookAt, newElement, bSortAscending, bAllowDuplicate );
		else
			return InsertSorted( nLookAt, nEnd, newElement, bSortAscending, bAllowDuplicate );
	}

	// we cannot reach this point
	ASSERT( FALSE );
	return -1;
}


#endif // !defined(AFX_SORTARRAY_H__A010A5E5_1F74_11D4_A221_006097239934__INCLUDED_)
