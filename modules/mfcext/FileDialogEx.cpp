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

void CFileDialogEx::OnFolderChange()
{
	m_strLastFolder = GetFolderPath(); //uses CDM_GETFOLDERPATH

	//NOTE: In my (old?) MSDN the func GetFolderPath() is not documented,
	// but you can find it in AFXDLGS.H as a public method.
}

const CString& CFileDialogEx::GetLastOpenedFolder() const
{
    if (m_bVistaStyle)
        m_strLastFolder = GetFolderPath();

    return m_strLastFolder;
}
