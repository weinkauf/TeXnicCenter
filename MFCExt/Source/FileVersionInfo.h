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

#if !defined(AFX_FILEVERSION_H__F828004C_7680_40FE_A08D_7BB4FF05B4CC__INCLUDED_)
#define AFX_FILEVERSION_H__F828004C_7680_40FE_A08D_7BB4FF05B4CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
Helps dealing with Version Information of files.

@ingroup mfcext

@author Sven Wiegand
*/
class AFX_EXT_CLASS CFileVersionInfo
{
// construction/destruction
public:
	CFileVersionInfo();
	virtual ~CFileVersionInfo();

// operations
public:
	BOOL Create(HMODULE hModule = NULL);
	BOOL Create(LPCTSTR lpszFileName);

// attribute operations
public:
	WORD GetFileVersion(int nIndex) const;
	WORD GetProductVersion(int nIndex) const;
	DWORD GetFileFlagsMask() const;
	DWORD GetFileFlags() const;
	DWORD GetFileOs() const;
	DWORD GetFileType() const;
	DWORD GetFileSubtype() const;
	CTime GetFileDate() const;

	CString GetCompanyName() const;
	CString GetFileDescription() const;
	CString GetFileVersion() const;
	CString GetInternalName() const;
	CString GetLegalCopyright() const;
	CString GetOriginalFileName() const;
	CString GetProductName() const;
	CString GetProductVersion() const;
	CString GetComments() const;
	CString GetLegalTrademarks() const;
	CString GetPrivateBuild() const;
	CString GetSpecialBuild() const;

// implementation helpers
protected:
	virtual void Reset();
	BOOL GetTranslationId(LPVOID lpData, UINT unBlockSize, WORD wLangId, DWORD &dwId, BOOL bPrimaryEnough = FALSE);

// attributes
private:
	VS_FIXEDFILEINFO m_FileInfo;

	CString m_strCompanyName;
	CString m_strFileDescription;
	CString m_strFileVersion;
	CString m_strInternalName;
	CString m_strLegalCopyright;
	CString m_strOriginalFileName;
	CString m_strProductName;
	CString m_strProductVersion;
	CString m_strComments;
	CString m_strLegalTrademarks;
	CString m_strPrivateBuild;
	CString m_strSpecialBuild;
};

#endif // !defined(AFX_FILEVERSION_H__F828004C_7680_40FE_A08D_7BB4FF05B4CC__INCLUDED_)
