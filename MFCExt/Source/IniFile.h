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

#if !defined(AFX_INIFILE_H__87221920_37C9_11D4_A221_006097239934__INCLUDED_)
#define AFX_INIFILE_H__87221920_37C9_11D4_A221_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <fstream.h>
#include <afxtempl.h>

/**
Class for simple INI-file handling.

Class is mainly based on the code of Adam Clauss' CIniFile class.

@ingroup mfcext

@author Sven Wiegand
*/
class AFX_EXT_CLASS CIniFile  
{
// Construction/Destruction
public:
	/**
	Constructs the object.

	@param strPath
		Path of the INI-file to work with or an empty string, if you want
		to set the path later using SetPath()
	*/
	CIniFile( CString strPath = "" );

	virtual ~CIniFile();

// Operations
public:
	/** 
	Sets the path of ini file to read and write from.

	@param strNewPath
		Full path of the INI-file to work with.
	*/
	void SetPath(CString strNewPath);

	/**
	Reads the INI file.

	@return
		TRUE if successful, FALSE otherwise.
	*/
	BOOL ReadFile();

	/**
	Writes the INI file.
	*/
	void WriteFile(); 

	/**
	Deletes all stored INI data.
	*/
	void Reset();

	/**
	Returns number of keys currently in the INI file.
	*/
	int GetNumKeys();

	/** 
	Returns number of values stored for specified key 
	*/
	int GetNumValues(CString keyname);

	/**
	Checks, if the specified value exists in the INI data.

	@param keyname
		Name of the key, the value is assumed to be contained in.
	@param valuename
		Name of the value, to check for.

	@return
		TRUE if the specified value exists, FALSE otherwise.
	*/
	BOOL VerifyValue(CString keyname, CString valuename);

	/**
	Checks, if the specified key exists in the INI data.

	@param keyname
		Name of the key, to check for.

	@return
		TRUE if the specified key exisits, FALSE otherwise.
	*/
	BOOL VerifyKey(CString keyname);

	/**
	Deletes the specified value from the INI data.

	@param keyname
		Name of the key, the value is contained in.
	@param valuename
		Name of the value to delete.

	@return
		TRUE if the value has been deleted successfully, FALSE if the
		value did not exist.
	*/
	BOOL DeleteValue(CString keyname, CString valuename);

	/**
	Deletes the specified key and all of its values.

	@param keyname
		Name of the key to delete.

	@return
		TRUE if the key and all its values has been deleted successfully,
		FALSE if the key did not exist.
	*/
	BOOL DeleteKey(CString keyname);

	/**
	Returns the specified string from the INI data.

	@param keyname
		Name of the key, the value is contained in.
	@param valuename
		Name of the string to return.
	@param strDefault
		String to return, if the specified value does not exist.

	@return
		The string value specified by keyname and valuename or the 
		default value, if the specified value does not exist.
	*/
	CString GetValue(CString keyname, CString valuename, CString strDefault); 

	/**
	Returns the integer from the INI data.

	@param keyname
		Name of the key, the value is contained in.
	@param valuename
		Name of the string to return.
	@param nDefault
		Integer to return, if the specified value does not exist.

	@return
		The integer value specified by keyname and valuename or the 
		default value, if the specified value does not exist.
	*/
	int GetValue(CString keyname, CString valuename, int nDefault); 

	/**
	Returns the specified string from the INI data.

	@param keyname
		Name of the key, the value is contained in.
	@param valuename
		Name of the string to return.
	@param dDefault
		Double value to return, if the specified value does not exist.

	@return
		The double value specified by keyname and valuename or the 
		default value, if the specified value does not exist.
	*/
	double GetValue(CString keyname, CString valuename, double dDefault);

	/**
	Sets a string value in the INI data.

	@param key
		Name of the key, the value should be contained in.
	@param valuename
		Name of the value.
	@param value
		Value to store.
	@param create
		Specify TRUE here, to create the specified value, if it does not
		exisit in the INI data. If you specify FALSE here, the value will
		only be set, if it is already existing, otherwise FALSE is 
		returned.

	@return
		If create has been set to TRUE, the function will always return
		TRUE, otherwise, the function returns TRUE, if the value was 
		already existing and has been set successfuly to the specified
		value.
	*/
	BOOL SetValue(CString key, CString valuename, CString value, BOOL create = TRUE);

	/**
	Sets a string value in the INI data.

	@param key
		Name of the key, the value should be contained in.
	@param valuename
		Name of the value.
	@param value
		Value to store.
	@param create
		Specify TRUE here, to create the specified value, if it does not
		exisit in the INI data. If you specify FALSE here, the value will
		only be set, if it is already existing, otherwise FALSE is 
		returned.

	@return
		If create has been set to TRUE, the function will always return
		TRUE, otherwise, the function returns TRUE, if the value was 
		already existing and has been set successfuly to the specified
		value.
	*/
	BOOL SetValue(CString key, CString valuename, int value, BOOL create = TRUE);

	/**
	Sets a string value in the INI data.

	@param key
		Name of the key, the value should be contained in.
	@param valuename
		Name of the value.
	@param value
		Value to store.
	@param create
		Specify TRUE here, to create the specified value, if it does not
		exisit in the INI data. If you specify FALSE here, the value will
		only be set, if it is already existing, otherwise FALSE is 
		returned.

	@return
		If create has been set to TRUE, the function will always return
		TRUE, otherwise, the function returns TRUE, if the value was 
		already existing and has been set successfuly to the specified
		value.
	*/
	BOOL SetValue(CString key, CString valuename, double value, BOOL create = TRUE);

// Implementation helpers
protected:
	/** overloaded to take CString */
	istream & getline( istream & is, CString & str );

	/** 
	returns index of specified value, in the specified key, or -1 
	if not found 
	*/
	int FindValue(int keynum, CString valuename);

	/** returns index of specified key, or -1 if not found */
	int FindKey(CString keyname);

// attributes
protected:
	/** stores pathname of ini file to read/write */
	CString path;
	
	/** all keys are of this time */ 
	struct key
	{
		/** list of values in key */
		CArray<CString, CString> values; 

		/** corresponding list of value names */
		CArray<CString, CString> names;
	};

	/** list of keys in ini */
	CArray<key, key> keys; 

	/** corresponding list of keynames */
	CArray<CString, CString> names; 

};

#endif // !defined(AFX_INIFILE_H__87221920_37C9_11D4_A221_006097239934__INCLUDED_)
