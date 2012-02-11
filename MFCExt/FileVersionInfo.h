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
class CFileVersionInfo
{
// construction/destruction
public:
// construction/destruction
public:
	/**
	Constructs an empty object.

	Call Create() to fill the class with version information.
	*/
	CFileVersionInfo();

	//@{
	/**
	Constructs an object and fills it with the version information
	of the specified module.

	See description of Create() for a documentation of the parameters.

	@see Create()
	*/
	CFileVersionInfo(HMODULE hModule, DWORD dwLanguageId = (DWORD)-1);
	CFileVersionInfo(LPCTSTR lpszFileName, DWORD dwLanguageId = (DWORD)-1);
	//@}

	virtual ~CFileVersionInfo();

// operations
public:
	//@{
	/**
	Loads the version information for the specified module.

	@param hModule
		Instance handle of the module to load the information for or
		NULL to load the information for the module, that has been used
		to create the current process.
	@param dwLanguageCodepageId
		Identifier for a language and codepage to load the version
		information in. If no information in this format is available,
		the method will load nothing. Specify 0xFFFFFFFF if the method
		should determine the language by itself. See VerQueryValue of the
		SDK documentation for details of the format of this id.
	*/
	BOOL Create(HMODULE hModule = NULL, DWORD dwLanguageCodepageId = (DWORD)-1);
	BOOL Create(LPCTSTR lpszFileName, DWORD dwLanguageCodepageId = (DWORD)-1);
	//@}

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

	/**
	Returns the identifier of the language, for which the version
	information has been loaded or 0xFFFF if no version information
	has been loaded.

	@return
		A value like created by MAKELANGID.
	*/
	LANGID GetLanguageId() const;

	/**
	Returns the codepage identifier the loaded version information
	is listed in or 0xFFFF if no version information has been loaded.
	*/
	WORD GetCodePageId() const;

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

	LANGID m_wLanguageId;
	WORD m_wCodePageId;
};

#endif // !defined(AFX_FILEVERSION_H__F828004C_7680_40FE_A08D_7BB4FF05B4CC__INCLUDED_)
