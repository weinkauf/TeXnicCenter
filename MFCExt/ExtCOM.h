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

#include "../Source/ComHelpers.h"

#ifndef __COMTOOL_H__
#define __COMTOOL_H__

/**
@addtogroup mfcext

@{
*/

/**
Makro zur Implementierung der IUnknown Methoden einer aggregierten
Interface-Implementierung.

@param theClass
	Die umgebende (aggregierende) Klasse
@param localClass
	Die gekapselte (aggregierte) Klasse <b>ohne</b> vorangestelltes 'X'
*/
#define IMPLEMENT_IUNKNOWN(theClass, localClass)\
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
Makro zur Implementierung der IUnknown Methoden einer aggregierten
Interface-Implementierung. Dieses Makro ist mit IMPLEMENT_IUNKNOWN
nahezu identisch, bis auf den Unterschied, das AddRef() immer 2 und
Release() immer 1 zurueckgibt. Dieses Makro eignet sich somit fuer
Implementierungen, deren Objekte nicht dynamisch auf dem Heap erzeugt
werden.

@param theClass
	Die umgebende (aggregierende) Klasse
@param localClass
	Die gekapselte (aggregierte) Klasse <b>ohne</b> vorangestelltes 'X'
*/
#define IMPLEMENT_STATIC_IUNKNOWN(theClass, localClass)\
	STDMETHODIMP_(ULONG) theClass::X##localClass::AddRef()\
	{\
		return 2;\
	}\
	\
	STDMETHODIMP_(ULONG) theClass::X##localClass::Release()\
	{\
		return 1;\
	}\
	\
	STDMETHODIMP theClass::X##localClass::QueryInterface( REFIID iid, void **ppvObj )\
	{\
		METHOD_PROLOGUE(theClass, localClass);\
		return pThis->ExternalQueryInterface( &iid, ppvObj );\
	}


/** @} */

#endif //__COMTOOL_H__

