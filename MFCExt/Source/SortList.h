/********************************************************************
*
* This file is part of the TeXnicCenter-system
*
* Copyright (C) 2002-$CurrentYear$ Chris Norris
* Copyright (C) 2002-$CurrentYear$ ToolsCenter
* 
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation; either version 2 of
* the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
* If you have further questions or if you want to support
* further TeXnicCenter development, visit the TeXnicCenter-homepage
*
*    http://www.ToolsCenter.org
*
*********************************************************************/

// SortList.h: interface for the CSortList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SORTLIST_H__8AA09E39_4FB8_4967_99B3_4DF64DCBDC6B__INCLUDED_)
#define AFX_SORTLIST_H__8AA09E39_4FB8_4967_99B3_4DF64DCBDC6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/* #include <function.h> */

/**
List that maintains elements in sorted order.

@author Chris Norris
*/
template<class TYPE, class ARG_TYPE, class Compare=std::less<TYPE> >
class AFX_EXT_CLASS CSortList : public CList<TYPE, ARG_TYPE>
{
public:
	CSortList() {};
	virtual ~CSortList() {};

public:
	/**
	Insert an element into an already sorted list.
	@param 
		insertMe Element to insert
	@return 
		Position of element insertion
	*/
	POSITION InsertSorted( ARG_TYPE insertMe )
	{
		POSITION pos = GetHeadPosition();
		while ( pos != NULL )
		{
			ARG_TYPE listElement = GetNext( pos );
			if ( Compare()(insertMe, listElement) )
			{
				InsertBefore( pos, insertMe );
				return pos;
			}
		}
		InsertAfter( pos, insertMe );
		return pos;
	}

	/**
	Find the position before an element in the list. The list is searched 
	from back to front.
	<b>Note<\b>:
	<ul>
		<li>The element does not have to exist in the list.<\li>
		<li>The position returned may not be equal to findMe, 
		even if findMe exists in the list.<\li>
	<\ul>
	@param findMe 
		Element to search for
	@param startAfter 
		Position to start search from. If startBefore == NULL, the search 
		starts from the tail of the list.
	*/
	POSITION FindBefore( ARG_TYPE findMe, POSITION startBefore = NULL )
	{
		if ( startBefore == NULL )
			startBefore = GetTailPosition();
		while ( startBefore != NULL )
		{
			ARG_TYPE listElement = GetPrev( startBefore );
			if ( Compare()(listElement, findMe) )
				return startBefore;
		}
		// The whole list was searched without finding an insertion point
		return GetHeadPosition();
	}

	/**
	Find the position after an element in the list. The list is searched from
	front to back.
	<b>Note<\b>:
	<ul>
		<li>The element does not have to exist in the list.<\li>
		<li>The position returned may not be equal to findMe, 
		even if findMe exists in the list.<\li>
	<\ul>
	@param findMe 
		Element to search for
	@param startAfter 
		Position to start search from. If startAfter == NULL, the search 
		starts from the head of the list.
	*/
	POSITION FindAfter( ARG_TYPE findMe, POSITION startAfter = NULL )
	{
		if ( startAfter == NULL )
			startAfter = GetHeadPosition();
		while ( startAfter != NULL )
		{
			ARG_TYPE listElement = GetNext( startAfter );
			if ( Compare()(findMe, listElement) )
				return startAfter;
		}
		// The whole list was searched without finding an insertion point
		return GetTailPosition();
	}

	/**
	Test if the list is strictly sorted. Strictly sorted means for each element 
	in the list (except the first element that has no previous), 
	<code>Compare(<prevElement>, <curElement>) </code> is <VAR>TRUE</VAR>. 
	The default implementation tests for a strictly increasing sequence of 
	elements defined by less-than.

	@return
		<VAR>TRUE</VAR> if list is strictly sorted, else <VAR>FALSE</VAR>.
	*/
	BOOL IsStrictlySorted() const;

	/**
	Test if the list is sorted. Strictly sorted means for each element 
	in the list (except the first element that has no previous), 
	<code>Compare(<prevElement>, <curElement>) </code> is <VAR>TRUE</VAR> OR 
	<code>Compare(<prevElement>, <curElement>) </code> is <VAR>FALSE</VAR>.
	The default implementation of tests for increasing sequence of elements 
	defined by less-than.

	@return
		<VAR>TRUE</VAR> if list is sorted, else <VAR>FALSE</VAR>.
	*/
	BOOL IsSorted() const;
};


template<class TYPE, class ARG_TYPE, class Compare>
BOOL CSortList<TYPE, ARG_TYPE, Compare>::IsStrictlySorted() const 
{
	POSITION pos = GetHeadPosition();
	TYPE prevElement = GetNext( pos );
	while ( pos != NULL )
	{
		ARG_TYPE curElement = GetNext( pos );
		if ( !Compare()(prevElement, curElement) )
			return FALSE;
		prevElement = curElement;
	}
	return TRUE;
}


template<class TYPE, class ARG_TYPE, class Compare>
BOOL CSortList<TYPE, ARG_TYPE, Compare>::IsSorted() const
{
	POSITION pos = GetHeadPosition();
	TYPE prevElement = GetNext( pos );
	while ( pos != NULL )
	{
		ARG_TYPE curElement = GetNext( pos );
		if ( !Compare()(prevElement, curElement) )
			if ( Compare()(curElement, prevElement) )
				return FALSE;
		prevElement = curElement;
	}
	return TRUE;
}


#endif // !defined(AFX_SORTLIST_H__8AA09E39_4FB8_4967_99B3_4DF64DCBDC6B__INCLUDED_)
