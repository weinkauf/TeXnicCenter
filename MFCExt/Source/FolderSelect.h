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

#if !defined(AFX_FOLDERSELECT_H__8D566541_1865_11D4_A221_006097239934__INCLUDED_)
#define AFX_FOLDERSELECT_H__8D566541_1865_11D4_A221_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/**
An object of this class allows the user to select a folder. It is a
wrapper class for SHBrowseForFolder.

@ingroup mfcext

@author Sven Wiegand
*/
class AFX_EXT_CLASS CFolderSelect  
{
// construction/destruction
public:
	/**
	Constructs the CFolderSelect-object. Call DoModal() to display the
	dialog.

	See description of BROWSEINFO in the MSDN for parameters.
	*/
	CFolderSelect( LPCTSTR lpszTitle, UINT unFlags = 0, CWnd *pwndParent = NULL, BFFCALLBACK lpfn = NULL, LPARAM lParam = 0 );
	virtual ~CFolderSelect();

// operations
public:
	/**
	Shows the dialog the user can select a folder in.

	@return
		IDOK if the user pressed the OK-Button or IDCANCEL if the user
		pressed the cancel-Button.
	*/
	UINT DoModal();

	/**
	Returns the path of the selected item or an empty string, if no
	item has been selected.
	*/
	CString GetPath();

	/**
	Returns the display name of the folder selected by the user.
	*/
	CString GetDisplayName();

	/**
	Returns the image associated with the selected folder. 
	
	The image is specified as an index to the system image list.
	*/
	int GetImage();

// attributes
protected:
	/** Titel, der angezeigt werden soll. */
	CString	m_strTitle;

	/** Structure for SHBrowseForFolder filled by the constructor. */
	BROWSEINFO m_bi;

	/** Stores the result of SHBrowseForFolder */
	CString m_strPath;

	/** Display name of the selected folder. */
	CString m_strDisplayName;

	/** Image associated with the selected folder. */
	int m_nImage;
};


// inlines
inline
CString CFolderSelect::GetPath()
{
	return m_strPath;
}


inline
CString CFolderSelect::GetDisplayName()
{
	return m_strDisplayName;
}


inline
int CFolderSelect::GetImage()
{
	return m_nImage;
}


#endif // !defined(AFX_FOLDERSELECT_H__8D566541_1865_11D4_A221_006097239934__INCLUDED_)
