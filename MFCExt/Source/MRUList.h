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

#if !defined(AFX_MRULIST_H__C8D50DC1_2052_11D4_A221_006097239934__INCLUDED_)
#define AFX_MRULIST_H__C8D50DC1_2052_11D4_A221_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

/**
An object of this class provides list of most recently used objects.

The specified TYPE must provide the ==Operator, so that CMRUList can
test, if an specified object is already stored in the list.

@ingroup mfcext

@author Sven Wiegand
*/
template<class TYPE, class ARG_TYPE>
class AFX_EXT_CLASS CMRUList : protected CArray<TYPE, ARG_TYPE>
{
// construction/destruction
public:
	/**
	Constructs a CMRUList-object.

	@param nSize
		Number of elements that can be stored in the list. If the list
		is full and a new element is added, the least recently added
		element is removed from the list.
	*/
	CMRUList( int nSize );

// operators
public:
	/**
	Returns the element at the specified position.

	@param nIndex
		Zero based index of the element to return.
	*/
	TYPE operator[]( int nIndex );

	// make protected CObject-operators public
	void* PASCAL operator new(size_t nSize);
	void* PASCAL operator new(size_t, void* p);
	void PASCAL operator delete(void* p);
#if _MSC_VER >= 1200
	void PASCAL operator delete(void* p, void* pPlace);
#endif

#if defined(_DEBUG) && !defined(_AFX_NO_DEBUG_CRT)
	// for file name/line number tracking using DEBUG_NEW
	void* PASCAL operator new(size_t nSize, LPCSTR lpszFileName, int nLine);
#if _MSC_VER >= 1200
	void PASCAL operator delete(void *p, LPCSTR lpszFileName, int nLine);
#endif
#endif

// operations
public:
	/**
	Returns the size of the list in elements.
	*/
	int GetSize() const;

	/**
	Returns the number of elements stored in the list.
	*/
	int GetElementCount() const;

	/**
	Adds an element to the list.

	@param element
		Element to add to the list.
	*/
	virtual void Add( ARG_TYPE element );

	/**
	Removes an element from the list.

	@param nIndex
		Zero based index of the element to remove from the list.
	*/
	virtual void Remove( int nIndex );

// attributes
protected:
	/** Number of elements in the list. */
	int m_nElementCount;
};


template<class TYPE, class ARG_TYPE>
CMRUList<TYPE, ARG_TYPE>::CMRUList( int nSize )
:	m_nElementCount( 0 )
{
	SetSize( nSize );
}


template<class TYPE, class ARG_TYPE>
inline 
TYPE CMRUList<TYPE, ARG_TYPE>::operator[]( int nIndex )
{
	return CArray<TYPE, ARG_TYPE>::operator[]( nIndex );
}


template<class TYPE, class ARG_TYPE>
inline 
void* PASCAL CMRUList<TYPE, ARG_TYPE>::operator new(size_t nSize)
{
	return CArray<TYPE, ARG_TYPE>::operator new( nSize );
}


template<class TYPE, class ARG_TYPE>
inline 
void* PASCAL CMRUList<TYPE, ARG_TYPE>::operator new(size_t nSize , void* p)
{
	return CArray<TYPE, ARG_TYPE>::operator new( nSize, p );
}


template<class TYPE, class ARG_TYPE>
inline 
void PASCAL CMRUList<TYPE, ARG_TYPE>::operator delete(void* p)
{
	CArray<TYPE, ARG_TYPE>::operator delete( p );
}


#if _MSC_VER >= 1200
template<class TYPE, class ARG_TYPE>
inline 
void PASCAL CMRUList<TYPE, ARG_TYPE>::operator delete(void* p, void* pPlace)
{
	return CArray<TYPE, ARG_TYPE>::operator delete( p, pPlace );
}
#endif


#if defined(_DEBUG) && !defined(_AFX_NO_DEBUG_CRT)
template<class TYPE, class ARG_TYPE>
inline 
void* PASCAL CMRUList<TYPE, ARG_TYPE>::operator new(size_t nSize, LPCSTR lpszFileName, int nLine)
{
	return CArray<TYPE, ARG_TYPE>::operator new( nSize, lpszFileName, nLine );
}


#if _MSC_VER >= 1200
template<class TYPE, class ARG_TYPE>
inline 
void PASCAL CMRUList<TYPE, ARG_TYPE>::operator delete(void *p, LPCSTR lpszFileName, int nLine)
{
	CArray<TYPE, ARG_TYPE>::operator delete( p, lpszFileName, nLine );
}


#endif
#endif


template<class TYPE, class ARG_TYPE>
inline 
int CMRUList<TYPE, ARG_TYPE>::GetSize() const
{
	return CArray<TYPE, ARG_TYPE>::GetSize();
}


template<class TYPE, class ARG_TYPE>
inline
int CMRUList<TYPE, ARG_TYPE>::GetElementCount() const
{
	return m_nElementCount;
}


template<class TYPE, class ARG_TYPE>
void CMRUList<TYPE, ARG_TYPE>::Add( ARG_TYPE element )
{
	// check, if element already exists...
	int	nCurrentPosition = -1;

	for( int i = 0; i < m_nElementCount; i++ )
	{
		if( GetAt( i ) == element )
		{
			nCurrentPosition = i;
			break;
		}
	}

	// if element is at start of list, then we don't have to do anything
	if( nCurrentPosition == 0 )
		return;

	//Remove element at the found position
	if( nCurrentPosition > 0 )
		Remove( nCurrentPosition );

	// Insert element at beginning of list if we have space for it
	if( m_nElementCount < GetSize() )
	{
		InsertAt( 0, element );
		m_nElementCount++;
	}
	else
	{
		//We do not have space for it.
		// So we need to remove the least recently used element first
		if (m_nElementCount > 0) //Should always be true, but who knows...
		{
			Remove( m_nElementCount - 1 );
			//Now add
			InsertAt( 0, element );
			m_nElementCount++;
		}
	}
}


template<class TYPE, class ARG_TYPE>
void CMRUList<TYPE, ARG_TYPE>::Remove( int nIndex )
{
	// remove element
	RemoveAt( nIndex );
	m_nElementCount--;
}


#endif // !defined(AFX_MRULIST_H__C8D50DC1_2052_11D4_A221_006097239934__INCLUDED_)

