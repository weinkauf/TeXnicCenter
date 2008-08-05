////////////////////////////////////////////////////////////////////////////
//	File:		StdAfx.h
//
//	Author:		Sven Wiegand
//	E-mail:		sven.wiegand@gmx.de
//
//	You are free to use or modify this code.
//
////////////////////////////////////////////////////////////////////////////

/********************************************************************
 *
 * $RCSfile: StdAfx.h,v $
 * $Revision$
 * $Date$
 *
 * $Author$
 *
 * $Log: StdAfx.h,v $
 * Revision 1.3  2005/03/08 16:36:08  vachis
 * Selection of block between bracket pairs in a hierarchical manner (Ctrl+M)
 * Bracket highlighting disabled if a selection is present
 * New default colours for bracket highlighting
 *
 * Revision 1.2  2005/02/15 09:43:12  vachis
 * Implemented feature 536164: Matching Bracket Highlight
 *
 * Revision 1.1.1.1  2002/02/26 08:12:01  svenwiegand
 * Initial revision
 *
 * Revision 1.0  2000-05-31 21:55:44+02  sven_wiegand
 * Initial revision
 *
 *
 ********************************************************************/

#if !defined(AFX_STDAFX_H__5353A20D_1059_11D3_929E_CEFE4CA77E5E__INCLUDED_)
#define AFX_STDAFX_H__5353A20D_1059_11D3_929E_CEFE4CA77E5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Selten verwendete Teile der Windows-Header nicht einbinden

#include <afxwin.h>         // MFC-Kern- und -Standardkomponenten
#include <afxext.h>         // MFC-Erweiterungen
#include <afxtempl.h>				// MFC Templateclasses
#include <afxpriv.h>

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE-Klassen
#include <afxodlgs.h>       // MFC OLE-Dialogfeldklassen
#include <afxdisp.h>        // MFC Automatisierungsklassen
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC-Datenbankklassen
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO-Datenbankklassen
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC-Unterstützung für allgemeine Steuerelemente von Internet Explorer 4
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC-Unterstützung für gängige Windows-Steuerelemente
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars

#include <atlfile.h>

#undef min
#undef max

#include "Speller.h"

// globale Variablen
//extern HINSTANCE		g_hDllInstance;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_STDAFX_H__5353A20D_1059_11D3_929E_CEFE4CA77E5E__INCLUDED_)
