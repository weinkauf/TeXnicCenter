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

#if !defined(AFX_FILEDIALOGEX_H__F9A18A11_F7BF_4303_8A36_31722C1164D2__INCLUDED_)
#define AFX_FILEDIALOGEX_H__F9A18A11_F7BF_4303_8A36_31722C1164D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/**
A replacement for CFileDialog, that will show a bar with commonly 
used folders on Win2000 or later. Additionally, it stores the
last opened folder even if the dialog was cancelled by the user.

@ingroup mfcext

@author Sven Wiegand
@author Tino Weinkauf
*/
class AFX_EXT_CLASS CFileDialogEx : 
    public CFileDialog
{
	DECLARE_DYNAMIC(CFileDialogEx)

public:
	CFileDialogEx(BOOL bOpenFileDialog,
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);

protected:
	///Catches the folder-changed-notification.
	void OnFolderChange();

//Methods
public:
	///Returns the last opened folder, even if the user cancelled the dialog.
	 const CString& GetLastOpenedFolder() const;

//Attributes
private:
	///Storage to hold the last opened folder.
	mutable CString m_strLastFolder;

//Message handlers
protected:
	//{{AFX_MSG(CFileDialogEx)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_FILEDIALOGEX_H__F9A18A11_F7BF_4303_8A36_31722C1164D2__INCLUDED_
