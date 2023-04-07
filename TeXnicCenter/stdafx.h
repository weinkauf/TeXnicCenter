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

#if !defined(AFX_STDAFX_H__447410E3_1058_11D3_929E_D75EECD5B417__INCLUDED_)
#define AFX_STDAFX_H__447410E3_1058_11D3_929E_D75EECD5B417__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
#define VC_EXTRALEAN		// Selten verwendete Teile der Windows-Header nicht einbinden

#pragma warning(disable: 4786)

#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC-Kern- und -Standardkomponenten
#include <afxext.h>         // MFC-Erweiterungen
#include <afxdtctl.h>				// MFC-Unterstützung für allgemeine Steuerelemente von Internet Explorer 4
#include <afxtempl.h>				// MFC-Template-Klassen
#include <afxpriv.h>

// Scintilla
#include "platform.h"
#include "scintilla.h"
#include "SciLexer.h"

#include "ScintillaCtrl.h"
#include "ScintillaDocView.h"

#include <afxpriv.h>
#include <afxmt.h>
#include <afxcview.h>

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC-Unterstützung für gängige Windows-Steuerelemente
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <atlfile.h>

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars
#include <ExtCollection.h>
#include <ExtControls.h>
#include <ExtTools.h>
#include <ExtApp.h>
#include <ExtCom.h>
#include <ExtDialog.h>
#include <MsXmlCpp.h>
#include <htmlhelp.h>
#include <dwmapi.h>

namespace Interfaces
{
	#include <TeXnicCenterObjects.h>
}

#include <regex>

typedef std::basic_regex<TCHAR> tregex;

#define _ATL_NO_AUTOMATIC_NAMESPACE

#include <atlbase.h>
#include <atlwin.h>
#include <afx.h>

#if defined _M_IX86
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#ifndef UNICODE
#error You have to select a Unicode solution configuration first
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_STDAFX_H__447410E3_1058_11D3_929E_D75EECD5B417__INCLUDED_)
