/********************************************************************
*
* $Workfile:$
* $Revision$
* $Modtime:$
* $Author$
*
* Änderungen:
*	$History:$
*
*********************************************************************/

/********************************************************************
*
* Copyright (C) 2001 Sven Wiegand
*
* This file is free software; you can redistribute it and/or
* modify, but leave the headers intact and do not remove any
* copyrights from the source.
*
* If you have further questions, suggestions or bug fixes, please
* let me know
*
*    sven.wiegand@web.de
*
********************************************************************/

#include "stdafx.h"
#include <afxdllx.h>
#include "../InterfaceWrapper/InterfaceWrapper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static AFX_EXTENSION_MODULE MSXMLCPPDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("MSXMLCPP.DLL Initializing!\n");
		
		if (!AfxInitExtensionModule(MSXMLCPPDLL, hInstance))
			return 0;

		new CDynLinkLibrary(MSXMLCPPDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("MSXMLCPP.DLL Terminating!\n");
		AfxTermExtensionModule(MSXMLCPPDLL);
	}
	return 1;
}
