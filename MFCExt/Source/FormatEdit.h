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

#if !defined(AFX_FORMATEDIT_H__D1B2D7E4_A0B6_11D3_941D_000000000000__INCLUDED_)
#define AFX_FORMATEDIT_H__D1B2D7E4_A0B6_11D3_941D_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//-------------------------------------------------------------------
// class CFormatEdit
//-------------------------------------------------------------------

/**
Ein Objekt der Klasse <var>CFormatEdit</var> stellt ein Eingabefeld
zur Verfuegung, fuer das eine Menge zulaessiger Zeichen definiert werden
kann. Betaetigt der Benutzer eine Taste, deren Zeichen nicht in dieser
Menge definiert ist, so wird der Tastendruck ignoriert.

@ingroup mfcext

@author Sven Wiegand
*/
class AFX_EXT_CLASS CFormatEdit : public CEdit
{
// Konstruktion/Destruktion
public:
	/**
	Konstruktor.

	@param lpszCharSet
		Zeichenkette, die alle Zeichen enthaelt, die im Eingabefeld zulaessig
		sind, oder <var>NULL</var>, wenn alle Zeichen zulaessig sind.
	*/
	CFormatEdit( LPCTSTR lpszCharSet = NULL );

// Operationen
public:
	/**
	Setzt die Menge der zulaessigen Zeichen.

	@param lpszCharSet
		Alle Zeichen, die in diesem String vorhanden sind, koennen vom
		Benutzer eingegeben werden, alle anderen sind verboten. Ist der
		Zeiger <var>NULL</var> oder enthaelt der String keine Zeichen,
		so sind alle Zeichen erlaubt.
	*/
	void SetCharSet( LPCTSTR lpszCharSet );

// Überschreibungen
protected:
	//{{AFX_VIRTUAL(CFormatEdit)
	//}}AFX_VIRTUAL

// Implementierungshelfer
public:

// Nachrichtenbearbeiter
protected:
	//{{AFX_MSG(CFormatEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

// Attribute
protected:
	/** Menge der zulaessigen Zeichen */
	CString m_strCharSet;
};


//-------------------------------------------------------------------
// class CFormatEdit
//-------------------------------------------------------------------

/**
Ein Objekt der Klasse CNumberEdit stellt ein Eingabefeld
dar, das ausschliesslich die Eingabe von Zahlen erlaubt.

Waehrend der Eingabe werden nicht zulaessige Tasten mit einem akustischen
Signal quittiert und ignoriert und es wird ueberprueft, ob sich die Zahl
in einem definierbaren Bereich befindet.
*/
class AFX_EXT_CLASS CNumberEdit : public CFormatEdit
{
// Typen
enum tagNumberType
{
	ntChar = 0, 
	ntUChar,
	ntShort,
	ntUShort,
	ntLong,
	ntDouble
};

// Konstruktion/Destruktion
public:
	/**
	Konstruktor.

	Das erzeugte Eingabefeld erlaubt die Eingabe von Zeichen,
	die dem spezifizierten Typ entsprechen.

	@param nType
		Typ, dessen Zeichen im Eingabefeld zugelassen sind.
		Ein Wert der Aufzaehlung <var>CNumberEdit::tagNumberType</var>.
	*/
	CNumberEdit( int nType = ntLong );

	/**
	Konstruktor.

	Das erzeugte Eingabefeld ueberwacht die eingegebene Zahl
	auf die spezifizierten Grenzen und laesst nur die Zeichen
	zu, die durch den Typ der oberen und unteren Grenze definiert
	sind.

	Um Mehrdeutigkeiten zu vermeiden kann ein explizites casten der
	Parameter notwendig sein.

	@param xMin
		Minimal zulaessiger Eingabewert.
	@param xMax
		Maximal zulaessiger Eingabewert.
	*/
	CNumberEdit( char cMin, char cMax );
	CNumberEdit( UCHAR ucMin, UCHAR ucMax );
	CNumberEdit( short sMin, short sMax );
	CNumberEdit( USHORT usMin, USHORT usMax );
	CNumberEdit( long lMin, long lMax );
	CNumberEdit( double dMin, double dMax );

// Operationen
public:
	/**
	Definiert den Zahlentyp, den das Eingabefeld einlesen soll. 
	
	Die obere
	und untere Grenze des Wertebereichs werden auf die minimal und maximal
	zulaessigen Werte des spezifizierten Zahlentyps gesetzt.

	Soll ein anderer Wertebereich gesetzt werden, so kann direkt die Methode
	<var>SetRange()</var> aufgerufen werden.

	@see #SetRange

	@param nType
		Ein Wert der Aufzaehlung <var>CNumberEdit::tagNumberType</var>,
		der den einzulesenden Zahlentyp beschreibt.
	*/
	void SetType( int nType );

	/**
	Definiert den Zahlentyp, den das Eingabefeld einlesen soll und den
	Wertebereich, in dem die einzulesende Zahl liegen soll.

	Um Mehrdeutigkeiten zu vermeiden kann ein explizites casten der
	Parameter notwendig sein.

	@param xMin
		Kleinster zulaessiger Wert.
	@param xMax
		Groesster zulaessiger Wert.
	*/
	void SetRange( char cMin, char cMax );
	void SetRange( UCHAR ucMin, UCHAR ucMax );
	void SetRange( short sMin, short sMax );
	void SetRange( USHORT usMin, USHORT usMax );
	void SetRange( long lMin, long lMax );
	void SetRange( double dMin, double dMax );

// Implementierungshelfer
protected:
	/**
	Definiert Typ und Wertebereich.

	@param nType
		Ein Wert der Aufzaehlung <var>CNumberEdit::tagNumberType</var>,
		der den einzulesenden Zaehlentyp beschreibt.
	@param xMin
		Kleinster zulaessiger Wert.
	@param xMax
		Groesster zulaessiger Wert.
	*/
	void SetType( int nType, long lMin, long lMax );
	void SetType( int nType, double dMin, double dMax );

	/**
	Setzt die zulaessigen Zeichen fuer das Eingabefeld in Abhaengigkeit
	der Objektattribute.
	*/
	void Prepare();

// Nachrichtenbearbeiter
protected:
	//{{AFX_MSG(CNumberEdit)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

// Attribute
protected:
	/** Zahlentyp, der eingelesen werden soll. */
	int m_nType;

	/** Obere Grenze fuer integer-Typen. */
	long m_lMax;

	/** Untere Grenze fuer integer-Typen. */
	long m_lMin;

	/** Obere Grenze fuer float-Typen. */
	double m_dMax;

	/** Untere Grenze fuer float-Type. */
	double m_dMin;

	/** Aktueller Text des Controls. */
	CString m_strText;

	/** Anfang des momentan selektierten Bereichs. */
	int m_nSelStart;

	/** Ende des momentan selektierten Bereichs. */
	int m_nSelEnd;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_FORMATEDIT_H__D1B2D7E4_A0B6_11D3_941D_000000000000__INCLUDED_
