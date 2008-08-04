////////////////////////////////////////////////////////////////////////////
//	File:		CrystalParser.cpp
//
//	Author:		Sven Wiegand
//	E-mail:		sven.wiegand@gmx.de
//
//	Implementation of the CCrystalParser class, a part of Crystal Edit -
//	syntax coloring text editor.
//
//	You are free to use or modify this code.
//
////////////////////////////////////////////////////////////////////////////

/********************************************************************
 *
 * $RCSfile: CrysEditEx.cpp,v $
 * $Revision$
 * $Date$
 *
 * $Author$
 *
 * $Log: CrysEditEx.cpp,v $
 * Revision 1.2  2003/12/16 20:04:38  svenwiegand
 * Implemented Feature 726766: "Option for selecting the language for the GUI"
 *
 * Revision 1.1.1.1  2002/02/26 08:11:59  svenwiegand
 * Initial revision
 *
 * Revision 1.0  2000-05-31 21:55:31+02  sven_wiegand
 * Initial revision
 *
 *
 ********************************************************************/

#include "stdafx.h"

#include <afxdllx.h>

#include "CCrystalTextView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// globals
//-------------------------------------------------------------------

static AFX_EXTENSION_MODULE CrysEditExDLL = {NULL, NULL
                                            };

extern "C" int APIENTRY
	DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("CRYSEDITEX.DLL Initializing!\n");

		if (!AfxInitExtensionModule(CrysEditExDLL, hInstance))
			return 0;

		new CDynLinkLibrary(CrysEditExDLL);

		CCrystalTextView::s_hResourceInst = hInstance;
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("CRYSEDITEX.DLL Terminating!\n");
		AfxTermExtensionModule(CrysEditExDLL);
	}
	return 1; // OK
}


//-------------------------------------------------------------------
// globals
//-------------------------------------------------------------------

void CRYSEDIT_CLASS_DECL CrystalEditExSetResourceHandle(HINSTANCE hResourceDll)
{
	CCrystalTextView::s_hResourceInst = hResourceDll;
}

void CRYSEDIT_CLASS_DECL CrystalEditExResetResourceHandle()
{
	CCrystalTextView::s_hResourceInst = CrysEditExDLL.hModule;
}
