/********************************************************************
*
* This file is part of the TeXnicCenter-system
*
* Copyright (C) 1999-2000 Sven Wiegand
* Copyright (C) 2000-$CurrentYear$ ToolsCenter
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

#if !defined(AFX_STACK_H__C6B12AC0_2638_11D3_929E_F9D13B5E8964__INCLUDED_)
#define AFX_STACK_H__C6B12AC0_2638_11D3_929E_F9D13B5E8964__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
Implements a simple stack as template class.

@author Sven Wiegand
*/
template<class TYPE, class ARG_TYPE> 
class CStack : public CObject
{
// construction
public:
	/** Constructs an empty stack */
	CStack();
	virtual ~CStack();

// attributes
public:
	/**
	Sets the prereserved size of the stack an the size, the stack
	grows, when no more space in the allready reserved memory is free.

	@param nNewSize
		New size of the stack. The function will allocate enough memory
		to store nNewSize elements.
	@param nGrowBy
		The size to grow, if a new element is added an the allready allocated
		memory is not enough.

	@throw CMemoryException
	*/
	void SetSize( int nNewSize, int nGrowBy = -1 );

	/**
	Returns TRUE if the stack is empty and FALSE, if there are still elements
	on the stack.
	*/
	BOOL IsEmpty() const;

// operations
public:
	/**
	Frees any extra memory that was allocated while the stack was grown. 
	This function has no effect on the size or upper bound of the array.
	*/
	void FreeExtra();

	/**
	Removes all the elements from this stack. If the stack is already empty, 
	the function still works. 
	*/
	void RemoveAll();

// element access
public:
	/**
	Pushes a new element on the stack

	@param newElement
		Element to push on the stack.
	*/
	void Push( ARG_TYPE newElement );

	/**
	Returns the top element from the stack, but does not pop it from the stack.

	<b>Remarks</b> You have to test, if the stack is not empty before calling
	this method, otherwise the call will result in a failed assertion.
	*/
	TYPE Top() const;

	/**
	Pops the top element from the stack.

	<b>Remarks</b> You have to test, if the stack is not empty before calling
	this method, otherwise the call will result in a failed assertion.
	*/
	TYPE Pop();

protected:
	/** The array the elements are stored in. */
	CArray<TYPE, ARG_TYPE> m_aElements;
};


//////////////////////////////////////////////////////////////////////
// implementation class CStack
template<class TYPE, class ARG_TYPE>
CStack<TYPE, ARG_TYPE>::CStack()
{
}


template<class TYPE, class ARG_TYPE>
CStack<TYPE, ARG_TYPE>::~CStack()
{
	m_aElements.RemoveAll();
}


template<class TYPE, class ARG_TYPE>
void CStack<TYPE, ARG_TYPE>::SetSize( int nNewSize, int nGrowBy /*= -1 */ )
{
	m_aElements.SetSize( nNewSize, nGrowBy );
}


template<class TYPE, class ARG_TYPE>
BOOL CStack<TYPE, ARG_TYPE>::IsEmpty() const
{
	return !m_aElements.GetSize();
}


template<class TYPE, class ARG_TYPE>
void CStack<TYPE, ARG_TYPE>::FreeExtra()
{
	m_aElements.FreeExtra();
}


template<class TYPE, class ARG_TYPE>
void CStack<TYPE, ARG_TYPE>::RemoveAll()
{
	m_aElements.RemoveAll();
}


template<class TYPE, class ARG_TYPE>
void CStack<TYPE, ARG_TYPE>::Push( ARG_TYPE newElement )
{
	m_aElements.Add( newElement );
}


template<class TYPE, class ARG_TYPE>
TYPE CStack<TYPE, ARG_TYPE>::Top() const
{
	ASSERT( !IsEmpty() );
	return m_aElements.GetAt( m_aElements.GetUpperBound() );
}


template<class TYPE, class ARG_TYPE>
TYPE CStack<TYPE, ARG_TYPE>::Pop()
{
	ASSERT( !IsEmpty() );

	TYPE	element = m_aElements.GetAt( m_aElements.GetUpperBound() );
	m_aElements.RemoveAt( m_aElements.GetUpperBound() );
	return element;
}


#endif // !defined(AFX_STACK_H__C6B12AC0_2638_11D3_929E_F9D13B5E8964__INCLUDED_)
