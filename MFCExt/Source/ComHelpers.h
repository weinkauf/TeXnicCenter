/********************************************************************
*
* This file is part of the MFCExt_Library
*
* Copyright (C) 1999_2000 Sven Wiegand
* Copyright (C) 2000_2001 ToolsCenter
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

#if !defined(AFX_COMHELPERS_H__60BFF6A0_5832_11d5_A222_006097239934__INCLUDED_)
#define AFX_COMHELPERS_H__60BFF6A0_5832_11d5_A222_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
Implements the IUnknown-virtuals.
*/
#define IMPLEMENT_AGGREGATED_IUNKNOWN(theClass, localClass)\
	STDMETHODIMP_(ULONG) theClass::X##localClass::AddRef()\
	{\
		METHOD_PROLOGUE(theClass, localClass);\
		return pThis->ExternalAddRef();\
	}\
	\
	STDMETHODIMP_(ULONG) theClass::X##localClass::Release()\
	{\
		METHOD_PROLOGUE(theClass, localClass);\
		return pThis->ExternalRelease();\
	}\
	\
	STDMETHODIMP theClass::X##localClass::QueryInterface( REFIID iid, void **ppvObj )\
	{\
		METHOD_PROLOGUE(theClass, localClass);\
		return pThis->ExternalQueryInterface( &iid, ppvObj );\
	}

/**
Sets pThis to the pointer of the outer class.

Same as METHOD_PROLOGUE without AFX_MANAGE_STATE.
*/
#define GET_OUTER(theClass, localClass) \
	theClass* pThis = \
		((theClass*)((BYTE*)this - offsetof(theClass, m_x##localClass))); \
	pThis;

#endif // !defined(AFX_COMHELPERS_H__60BFF6A0_5832_11d5_A222_006097239934__INCLUDED_)