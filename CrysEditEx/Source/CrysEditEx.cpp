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
* $RCSfile$
* $Revision$
* $Date$
*
* $Author$
*
* $Log$
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

static AFX_EXTENSION_MODULE CrysEditExDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);

	CCrystalTextView::s_hResourceInst = hInstance;

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("CRYSEDITEX.DLL Initializing!\n");
		
		if (!AfxInitExtensionModule(CrysEditExDLL, hInstance))
			return 0;

		new CDynLinkLibrary(CrysEditExDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("CRYSEDITEX.DLL Terminating!\n");
		AfxTermExtensionModule(CrysEditExDLL);
	}
	return 1;   // OK
}
