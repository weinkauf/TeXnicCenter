// XP.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "XP.h"
#include "OfficeXP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CXPApp

BEGIN_MESSAGE_MAP(CXPApp, CWinApp)
	//{{AFX_MSG_MAP(CXPApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXPApp construction

CXPApp::CXPApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CXPApp object

CXPApp theApp;

CRuntimeClass* __stdcall BCGCBGetSkinClass (int iIndex)
{
	switch (iIndex)
	{
	case 0:
		return RUNTIME_CLASS (COfficeXP);
	}

	return NULL;
}
//*************************************************************************************
int __stdcall BCGCBGetSkinCount ()
{
	return 1;
}
//*************************************************************************************
LPCTSTR __stdcall BCGCBGetSkinName (int iIndex)
{
	switch (iIndex)
	{
	case 0:
		return _T("Office XP");
	}

	return NULL;
}
//*************************************************************************************
BOOL __stdcall BCGCBGetSkinVersion(int& iVersionMajor,int& iVersionMinor)
{
	iVersionMajor = _BCGCB_VERSION_MAJOR;
	iVersionMinor = _BCGCB_VERSION_MINOR;

	return TRUE;
}
//*************************************************************************************
BOOL __stdcall BCGCBPreviewSkin (CDC* pDC, int iIndex, CRect rectPreview)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState ())

	switch (iIndex)
	{
	case 0:
		{
			CBitmap bmpPreview;
			bmpPreview.LoadBitmap (IDB_OFFICEXP_PREVIEW);

			pDC->DrawState (rectPreview.TopLeft (), rectPreview.Size (), 
							&bmpPreview, DSS_NORMAL);
		}
		return TRUE;
	}

	return FALSE;
}
//*************************************************************************************
LPCTSTR __stdcall BCGCBGetSkinAuthor (int /*iIndex*/)
{
	return _T("ToolsCenter");
}
//*************************************************************************************
LPCTSTR __stdcall BCGCBGetSkinAuthorURL (int /*iIndex*/)
{
	return _T("sven@ToolsCenter.org");
}
//*************************************************************************************
LPCTSTR __stdcall BCGCBGetSkinAuthorMail (int /*iIndex*/)
{
	return _T("www.ToolsCenter.org");
}
//*************************************************************************************
int __stdcall BCGCBGetSkinUpdate ()
{
	return 1;
}
