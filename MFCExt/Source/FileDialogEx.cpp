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

#include "stdafx.h"
#include "FileDialogEx.h"
#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//-------------------------------------------------------------------
// class CFileDialogEx
//-------------------------------------------------------------------

IMPLEMENT_DYNAMIC(CFileDialogEx, CFileDialog)

BEGIN_MESSAGE_MAP(CFileDialogEx, CFileDialog)
	//{{AFX_MSG_MAP(CFileDialogEx)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CFileDialogEx::CFileDialogEx(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd)
:	CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
}


/////////////////////////////////////////////////////////////////////
// Overridings

int CFileDialogEx::DoModal()
{
	struct OPENFILENAMEEX : public OPENFILENAME 
	{ 
		void*	pvReserved;
		DWORD	dwReserved;
		DWORD	FlagsEx;
	} OfnEx;
	DWORD	dwWinMajor;

	ZeroMemory(&OfnEx, sizeof(OfnEx));
	CopyMemory(&OfnEx, &m_ofn, sizeof(m_ofn));

	dwWinMajor = (DWORD)(LOBYTE(LOWORD(::GetVersion())));
	if (dwWinMajor >= 5)
		OfnEx.lStructSize = sizeof(OfnEx);
	else
		OfnEx.lStructSize = sizeof(OPENFILENAME);

	ASSERT_VALID(this);
	ASSERT(OfnEx.Flags & OFN_ENABLEHOOK);
	ASSERT(OfnEx.lpfnHook != NULL); // can still be a user hook

	// zero out the file buffer for consistent parsing later
	ASSERT(AfxIsValidAddress(OfnEx.lpstrFile, OfnEx.nMaxFile));
	DWORD nOffset = lstrlen(OfnEx.lpstrFile)+1;
	ASSERT(nOffset <= OfnEx.nMaxFile);
	memset(OfnEx.lpstrFile+nOffset, 0, (OfnEx.nMaxFile-nOffset)*sizeof(TCHAR));

	// WINBUG: This is a special case for the file open/save dialog,
	//  which sometimes pumps while it is coming up but before it has
	//  disabled the main window.
	HWND hWndFocus = ::GetFocus();
	BOOL bEnableParent = FALSE;
	OfnEx.hwndOwner = PreModal();
	AfxUnhookWindowCreate();
	if (OfnEx.hwndOwner != NULL && ::IsWindowEnabled(OfnEx.hwndOwner))
	{
		bEnableParent = TRUE;
		::EnableWindow(OfnEx.hwndOwner, FALSE);
	}

	_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
	ASSERT(pThreadState->m_pAlternateWndInit == NULL);

	if (OfnEx.Flags & OFN_EXPLORER)
		pThreadState->m_pAlternateWndInit = this;
	else
		AfxHookWindowCreate(this);

	int nResult;
	if (m_bOpenFileDialog)
		nResult = ::GetOpenFileName(&OfnEx);
	else
		nResult = ::GetSaveFileName(&OfnEx);

	if (nResult)
		ASSERT(pThreadState->m_pAlternateWndInit == NULL);
	pThreadState->m_pAlternateWndInit = NULL;

	// WINBUG: Second part of special case for file open/save dialog.
	if (bEnableParent)
		::EnableWindow(OfnEx.hwndOwner, TRUE);
	if (::IsWindow(hWndFocus))
		::SetFocus(hWndFocus);

	PostModal();
	return nResult ? nResult : IDCANCEL;
}
