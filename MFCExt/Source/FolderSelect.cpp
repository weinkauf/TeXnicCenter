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
#include "FolderSelect.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//-------------------------------------------------------------------
// class CFolderSelect
//-------------------------------------------------------------------

CFolderSelect::CFolderSelect( LPCTSTR lpszTitle, UINT unFlags /*= 0*/, CWnd* pwndParent /*= NULL*/, BFFCALLBACK lpfn /*= NULL*/, LPARAM lParam /*= 0*/ )
{
	Init(lpszTitle, unFlags, pwndParent, lpfn, lParam);
}

CFolderSelect::CFolderSelect( LPCTSTR lpszTitle, LPCTSTR lpszInitPath, UINT unFlags /*= 0*/, CWnd *pwndParent /*= NULL*/ )
{
	m_strInitFolder = lpszInitPath;
	if (!m_strInitFolder.IsEmpty())
	{
		Init(lpszTitle, unFlags, pwndParent, (BFFCALLBACK) FolderSelectDlgCallback, (LONG_PTR)(LPCTSTR)m_strInitFolder);
	}
	else
	{
		Init(lpszTitle, unFlags, pwndParent, NULL, NULL);
	}
}

CFolderSelect::~CFolderSelect()
{}


void CFolderSelect::Init(LPCTSTR lpszTitle, UINT unFlags, CWnd* pwndParent, BFFCALLBACK lpfn, LPARAM lParam)
{
	m_strTitle = lpszTitle;
	m_bi.hwndOwner = pwndParent? pwndParent->GetSafeHwnd() : NULL;
	m_bi.pidlRoot = NULL;
	m_bi.pszDisplayName = m_strDisplayName.GetBuffer( MAX_PATH );
	m_bi.lpszTitle = m_strTitle;
	m_bi.ulFlags = unFlags | BIF_NEWDIALOGSTYLE;
	m_bi.lpfn = lpfn;
	m_bi.lParam = lParam;
}


UINT CFolderSelect::DoModal()
{
	// Display dialog
	LPITEMIDLIST	lpidl;

	lpidl = SHBrowseForFolder( &m_bi );
	m_strDisplayName.ReleaseBuffer();
	if( !lpidl )
		return IDCANCEL;

	// store results
	m_nImage = m_bi.iImage;

	SHGetPathFromIDList( lpidl, m_strPath.GetBuffer( MAX_PATH ) );
	m_strPath.ReleaseBuffer();

	// free memory
	LPMALLOC	lpMalloc;
	SHGetMalloc( &lpMalloc );
	if( lpMalloc )
		lpMalloc->Free( lpidl );

	return IDOK;
}


//---------------------------------------------------------------------

int CALLBACK FolderSelectDlgCallback(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	if (uMsg == BFFM_INITIALIZED)
	{
		//Select the folder
		::SendMessage(hwnd, BFFM_SETSELECTION, true, lpData);
	}

	return 0;
}

