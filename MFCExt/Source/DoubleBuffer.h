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

#if !defined(AFX_DOUBLEBUFFER_H__D73ED7E2_91E3_11D3_BB3D_0000E8EA812D__INCLUDED_)
#define AFX_DOUBLEBUFFER_H__D73ED7E2_91E3_11D3_BB3D_0000E8EA812D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxmt.h>					// MFC-Multi-Threading-Erweiterungen

/**
Die Klasse CDoubleBuffer stellt einen threadsicheren Doppelpuffer zur
Verfügung, bei dem jeder der beiden Hauptpuffer <var>BUFSIZE</var> Elemente
vom Typ <var>TYPE</var> aufnehmen kann.

@ingroup mfcext

@author Sven Wiegand
*/
template<class TYPE, int BUFFERSIZE>
class /*AFX_EXT_CLASS*/ CDoubleBuffer
{
// Konstruktion/Destruktion
public:
	CDoubleBuffer();

// Operatoren
public:
	/**
	Kopiert die Daten aus dem Lesepuffer des spezifizierten Puffers
	in den Schreibpuffer dieses Puffers.

	@param buffer
		Puffer, dessen lesbare Daten in diesen Puffer kopiert werden sollen.
	*/
	CDoubleBuffer &operator=( CDoubleBuffer &buffer );

// Operationen
public:
	/**
	Markiert alle Datensätze als ungültig.
	*/
	void Clear();

	/**
	Leert den zur Zeit als Lesepuffer aktiven Puffer und macht ihn zum
	Schreibpuffer und macht den Schreibpuffer zum Lesepuffer.
	*/
	void SwitchBuffer();

	/**
	Fügt einen Datensatz zum Schreibpuffer hinzu.

	@param element
		Referenz auf den Datensatz, der zum Schreibpuffer hinzugefügt
		werden soll.
	*/
	void Add( const TYPE &element );

	/**
	Gibt die Anzahl der Elemente im Lesepuffer zurueck.
	*/
	int GetSize();

	/**
	Gibt den spezifizierten Datensatz zurück.

	@param nIndex
		Index des zu lesenden Elementes. <b>Muss</b> zwischen 0 und
		dem Wert, den <var>GetSize()</var> liefert liegen.

	@return
		Kopie des gewünschten Datensatz.
	*/
	const TYPE Get( int nIndex );

// Attribute
protected:
	/**
	Die beiden eigentlichen Puffer
	*/
	TYPE	m_aaElements[2][BUFFERSIZE];

	/**
	Anzahl der gültigen Elemente im Puffer 1 bzw. 2.
	*/
	int m_anValidElements[2];

	/**
	Index des Puffers in <var>m_aaElements</var>, der gerade als
	Schreibpuffer dient.
	*/
	int m_nWriteBufferIndex;

	/**
	Index des Puffers in <var>m_aaElements</var>, der gerade als
	Lesepuffer dient.
	*/
	int m_nReadBufferIndex;

	/**
	Synchronisationsobjekte für den Zugriff auf die Daten.

	<var>m_aCriticalSection[write]</var>
	ist gesperrt, wenn gerade Daten mit der Methode <var>Add()</var>
	geschrieben werden oder wenn der Puffer gerade mit <var>Switch()</var>
	gewechselt wird.
	*/
	CSemaphore m_writeAccess;

	/**
	Synchronisationsobjekte für den Zugriff auf die Daten.

	<var>m_aCriticalSection[read]</var>
	ist gesperrt, wenn gerade Daten mit der Methode <var>Get()</var>
	gelesen werden oder wenn der Puffer gerade mit <var>Switch()</var>
	gewechselt wird.
	*/
	CSemaphore m_readAccess;

#ifdef _DEBUG
	/**
	Zählt, wie oft die Methode <var>Add</var> für den aktuellen Schreibpuffer
	aufgerufen wird, egal ob das Element wirklich hinzugefügt wird oder nicht
	(weil evtl. schon maximal zulässige Anzahl von Elementen vorhanden ist).
	*/
	int m_nAddCount;
#endif
};


// Implementierung der Inline-Methoden
template<class TYPE, int BUFFERSIZE>
CDoubleBuffer<TYPE, BUFFERSIZE>::CDoubleBuffer()
:	m_writeAccess(),
	m_readAccess()
#ifdef _DEBUG
	, m_nAddCount( 0 )
#endif
{
	Clear();
}


template<class TYPE, int BUFFERSIZE>
CDoubleBuffer<TYPE, BUFFERSIZE> &CDoubleBuffer<TYPE, BUFFERSIZE>::operator=( CDoubleBuffer &buffer )
{
	CSingleLock	readAccess( &buffer.m_readAccess, TRUE );
	CSingleLock	writeAccess( &m_writeAccess, TRUE );

	for( int i = 0; i < buffer.m_anValidElements[buffer.m_nReadBufferIndex]; i++ )
		m_aaElements[m_nWriteBufferIndex][i] = buffer.m_aaElements[buffer.m_nReadBufferIndex][i];

	m_anValidElements[m_nWriteBufferIndex] = i;

	return *this;
}


template<class TYPE, int BUFFERSIZE>
void CDoubleBuffer<TYPE, BUFFERSIZE>::Clear()
{
	CSingleLock	readAccess( &m_readAccess, TRUE );
	CSingleLock	writeAccess( &m_writeAccess, TRUE );

	m_anValidElements[0] = 0;
	m_anValidElements[1] = 0;
	m_nWriteBufferIndex = 0;
	m_nReadBufferIndex = 1;

#ifdef _DEBUG
	//TRACE( "CDoubleBuffer::Clear: %d Elemente sollten hinzugefügt werden\n", m_nAddCount );
	m_nAddCount = 0;
#endif
}


template<class TYPE, int BUFFERSIZE>
inline void CDoubleBuffer<TYPE, BUFFERSIZE>::SwitchBuffer()
{
	CSingleLock	readAccess( &m_readAccess, TRUE );
	CSingleLock	writeAccess( &m_writeAccess, TRUE );

	if( m_nWriteBufferIndex = m_nReadBufferIndex )
		m_nReadBufferIndex = 0;
	else
		m_nReadBufferIndex = 1;

	m_anValidElements[m_nWriteBufferIndex] = 0;

#ifdef _DEBUG
	//TRACE( "CDoubleBuffer::SwitchBuffer: %d Elemente sollten hinzugefügt werden\n", m_nAddCount );
	m_nAddCount = 0;
#endif
}


template<class TYPE, int BUFFERSIZE>
inline void CDoubleBuffer<TYPE, BUFFERSIZE>::Add( const TYPE &element )
{
	CSingleLock	writeAccess( &m_writeAccess, TRUE );

	if( m_anValidElements[m_nWriteBufferIndex] >= BUFFERSIZE )
		return;

	m_aaElements[m_nWriteBufferIndex][m_anValidElements[m_nWriteBufferIndex]++] = element;

#ifdef _DEBUG
	++m_nAddCount;
#endif
}


template<class TYPE, int BUFFERSIZE>
inline int CDoubleBuffer<TYPE, BUFFERSIZE>::GetSize()
{
	CSingleLock	readAccess( &m_readAccess, TRUE );

	return m_anValidElements[m_nReadBufferIndex];
}


template<class TYPE, int BUFFERSIZE>
inline const TYPE CDoubleBuffer<TYPE, BUFFERSIZE>::Get( int nIndex )
{
	ASSERT( nIndex >= 0 && nIndex < BUFFERSIZE );

	CSingleLock	readAccess( &m_readAccess, TRUE );

	return m_aaElements[m_nReadBufferIndex][nIndex];
}

#endif // !defined(AFX_DOUBLEBUFFER_H__D73ED7E2_91E3_11D3_BB3D_0000E8EA812D__INCLUDED_)