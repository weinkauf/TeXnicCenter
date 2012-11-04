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

/**
@addtogroup mfcext

@{
*/

/**
Laedt den String mit der spezifizierten Kennung aus der Stringtabelle
und gibt ihn zurueck.
*/
 CString AfxLoadStringEx( UINT unID );


/**
Laedt den String <var>unID</var> und ersetzt ein enthaltenes '%s' durch
den Text <var>lpszText</var>. Der entstandene String wird zurueckgegeben.
*/
 CString AfxFormatString( UINT unID, LPCTSTR lpszText );


/**
Laedt den String <var>unID</var> und ersetzt ein enthaltenes '%d' unter
Beruecksichtigung der Formatierungsanweisungen durch
den Wert von <var>n</var>. Der entstandene String wird zurueckgegeben.
*/
 CString AfxFormatString( UINT unID, int n );


/**
Laedt den String <var>unID</var> und ersetzt ein enthaltenes '%f' unter
Beruecksichtigung der Formatierungsanweisungen durch
den Wert von <var>d</var>. Der entstandene String wird zurueckgegeben.
*/
 CString AfxFormatString( UINT unID, double d );


/**
Zeigt eine MessageBox an.

@see MSDN: MessageBox

@param unPromptID
	Kennung der Stringresource, die die anzuzgeigende Meldung enthaelt.
@param unTitleID
	Kennung der Stringresource, die den anzuzgeigenden Titel enthaelt.
@param unType
	MessageBox-Style.
@param pwndParent
	Zeiger auf das Elternfenster oder <var>NULL</var>, falls kein
	Elternfenster verwendet werden soll.

@return
	0, wenn ein Fehler aufgetreten ist, sonst die Kennung des Betaetigten
	Buttons.
*/
 int MessageBoxEx( UINT unPromptID, UINT unTitleID, UINT unType = MB_OK, CWnd *pwndParent = NULL );


/**
Zeigt eine MessageBox an.

@see MSDN: MessageBox

@param lpszPrompt
	Anzuzeigende Meldung.
@param unTitleID
	Kennung der Stringresource, die den anzuzgeigenden Titel enthaelt.
@param unType
	MessageBox-Style.
@param pwndParent
	Zeiger auf das Elternfenster oder <var>NULL</var>, falls kein
	Elternfenster verwendet werden soll.

@return
	0, wenn ein Fehler aufgetreten ist, sonst die Kennung des Betaetigten
	Buttons.
*/
 int MessageBoxEx( LPCTSTR lpszPrompt, UINT unTitleID, UINT unType = MB_OK, CWnd *pwndParent = NULL );


/** @} */
