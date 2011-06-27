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

#if !defined(AFX_COLLECTIONEXCEPTION_H__ADDAFA88_86F4_11D3_93E6_000000000000__INCLUDED_)
#define AFX_COLLECTIONEXCEPTION_H__ADDAFA88_86F4_11D3_93E6_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/**
Objekte dieser Klasse werden als Exceptions von andere Objekten ausgeloest.
Bei den anderen Objekten handelt es sich um Sammlungs-Objekte (Arrays, Listen, etc.).

@ingroup mfcext

@author Sven Wiegand
*/
class  CCollectionException : public CException  
{
// Typen
public:
	/** 
	Diese Aufzaehlung enthalte Konstanten, die den aufgetretenen Fehler genauer
	spezifizieren.
	*/
	enum tagCause
	{
		none = 0,
		generic,
		outOfBounds,
		notFound
	};

// Konstruktion/Destruktion
protected:
	/**
	Konstruktor.

	@param nCause
		Grund fuer das Ausloesen der Ausnahme.
	*/
	CCollectionException( int nCause = CCollectionException::none );

// statische Operationen
public:
	/**
	Erzeugt ein CCollectionException-Objekt auf dem Heap und loest es durch
	einen Aufruf von throw aus.

	@param nCause
		Kennung, die den Grund der Exceptionausloesung beschreibt.
	*/
	static void Throw( int nCause = CCollectionException::none );

// Attribute
public:
	/**
	Spezifiziert den Grund, warum die Exception ausgeloest wurde.

	Folgende Werte sind definiert.
	<ul>
		<li><b>none</b> Es ist kein Fehler aufgetreten
		<li><b>generic</b> Allgemeiner, nicht genauer spezifizierbarer Fehler
		<li><b>outOfBounds</b> Es wurde versucht auf ein Element ausserhalb der 
			zulaessigen Grenzen zuzugreifen
		<li><b>notFound</b> Das spezifizierte Element existiert nicht.
	</ul>
	*/
	int m_nCause;
};

#endif // !defined(AFX_COLLECTIONEXCEPTION_H__ADDAFA88_86F4_11D3_93E6_000000000000__INCLUDED_)
