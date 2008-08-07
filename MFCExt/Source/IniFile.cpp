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
#include "IniFile.h"

#include <string>
#include <fstream>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//-------------------------------------------------------------------
// class CIniFile
//-------------------------------------------------------------------

namespace {
	std::basic_ostream<TCHAR>& operator<<(std::basic_ostream<TCHAR>& out, const CString& s)
	{
		return out << static_cast<LPCTSTR>(s);
	}
}

CIniFile::CIniFile(const CString& inipath /*=_T("")*/)
: path( inipath )
{
}

void CIniFile::SetPath( const CString& strNewPath )
{
	path = strNewPath;
}

const CString& CIniFile::GetPath() const
{
	return path;
}

bool CIniFile::ReadFile()
{
	//CFile file;
	//CFileStatus status;

	//if (!file.GetStatus(path,status))
	//	return 0;

	std::basic_ifstream<TCHAR> inifile(path);

    if (!inifile)
        return false;

    CString readinfo;
	int curkey = -1, curval = -1;

	CString keyname, valuename, value;
	CString temp;

    std::basic_string<TCHAR> line;

    while (getline(inifile,line)) {
        readinfo.SetString(line.c_str(),line.size());

        if (!readinfo.IsEmpty()) {
			if (readinfo[0] ==_T('[') && readinfo[readinfo.GetLength() - 1] ==_T(']')) //if a section heading
			{
				keyname = readinfo;
				keyname.TrimLeft(_T('['));
				keyname.TrimRight(_T(']'));
			}
			else //if a value
			{
				valuename = readinfo.Left(readinfo.Find(_T("=")));
				value = readinfo.Right(readinfo.GetLength()-valuename.GetLength()-1);
				SetValue(keyname,valuename,value);
			}
		}
	}

	return true;
}

void CIniFile::WriteFile()
{
	std::basic_ofstream<TCHAR> inifile(path);

	for (int keynum = 0; keynum <= names.GetUpperBound(); keynum++) {
		if (!keys[keynum].names.IsEmpty()) {
			inifile <<_T('[') << names[keynum] <<_T(']') << std::endl;

			for (int valuenum = 0; valuenum <= keys[keynum].names.GetUpperBound(); valuenum++) {
				inifile << keys[keynum].names[valuenum] <<_T("=") << keys[keynum].values[valuenum];

				if (valuenum != keys[keynum].names.GetUpperBound())
					inifile << std::endl;
				else
					if (keynum < names.GetSize())
						inifile << std::endl;
			}

			if (keynum < names.GetSize())
				inifile << std::endl;
		}
	}
}

void CIniFile::Reset()
{
	keys.SetSize(0);
	names.SetSize(0);
}

int CIniFile::GetNumKeys() const
{
	return keys.GetSize();
}

int CIniFile::GetNumValues( const CString& keyname ) const
{
	int keynum = FindKey(keyname);
	if (keynum == -1)
		return -1;
	else
		return keys[keynum].names.GetSize();
}

bool CIniFile::VerifyValue( const CString& keyname, const CString& valuename ) const
{
	int	keynum = FindKey(keyname);
	int	valuenum = FindValue(keynum, valuename);

	if (keynum == -1 || valuenum == -1)
		return FALSE;
	else
		return TRUE;
}

bool CIniFile::VerifyKey( const CString& keyname ) const
{
	if( FindKey(keyname) == -1 )
		return FALSE;
	else
		return TRUE;
}

const CString CIniFile::GetValue( const CString& keyname, const CString& valuename, const CString& strDefault ) const
{
	int keynum = FindKey(keyname), valuenum = FindValue(keynum,valuename);

	if (keynum == -1)
		return strDefault;

	if (valuenum == -1)
		return strDefault;

	return keys[keynum].values[valuenum];
}

int CIniFile::GetValue(const CString& keyname, const CString& valuename, int nDefault) const
{
	CString	strDefault;
	strDefault.Format( _T("%d"), nDefault );
	return _ttol( GetValue(keyname, valuename, strDefault) );
}

double CIniFile::GetValue(const CString& keyname, const CString& valuename, double dDefault) const
{
	CString	strDefault;
	strDefault.Format( _T("%e"), dDefault );
	return _ttoi( GetValue(keyname, valuename, strDefault ) );
}

bool CIniFile::SetValue( const CString& key, const CString& valuename, const CString& value, bool create /*= TRUE*/ )
{
	int keynum = FindKey(key), valuenum = 0;
	//find key
	if (keynum == -1) //if key doesn't exist
	{
		if (!create) //and user does not want to create it,
			return 0; //stop entering this key
		names.SetSize(names.GetSize()+1);
		keys.SetSize(keys.GetSize()+1);
		keynum = names.GetSize()-1;
		names[keynum] = key;
	}

	//find value
	valuenum = FindValue(keynum,valuename);
	if (valuenum == -1)
	{
		if (!create)
			return 0;
		keys[keynum].names.SetSize(keys[keynum].names.GetSize()+1);
		keys[keynum].values.SetSize(keys[keynum].names.GetSize()+1);
		valuenum = keys[keynum].names.GetSize()-1;
		keys[keynum].names[valuenum] = valuename;
	}
	keys[keynum].values[valuenum] = value;
	return 1;
}

bool CIniFile::SetValue(const CString& keyname, const CString& valuename, int value, bool create)
{
	CString temp;
	temp.Format(_T("%d"),value);
	return SetValue(keyname, valuename, temp, create);
}

bool CIniFile::SetValue(const CString& keyname, const CString& valuename, double value, bool create)
{
	CString temp;
	temp.Format(_T("%e"),value);
	return SetValue(keyname, valuename, temp, create);
}

bool CIniFile::DeleteValue(const CString& keyname, const CString& valuename)
{
	int keynum = FindKey(keyname), valuenum = FindValue(keynum,valuename);
	if (keynum == -1 || valuenum == -1)
		return 0;

	keys[keynum].names.RemoveAt(valuenum);
	keys[keynum].values.RemoveAt(valuenum);
	return 1;
}

bool CIniFile::DeleteKey(const CString& keyname)
{
	int keynum = FindKey(keyname);
	if (keynum == -1)
		return 0;
	keys.RemoveAt(keynum);
	names.RemoveAt(keynum);
	return 1;
}

int CIniFile::FindKey( const CString& keyname ) const
{
	int keynum = 0;
	while ( keynum < keys.GetSize() && names[keynum] != keyname)
		++keynum;
	if (keynum == keys.GetSize())
		return -1;
	return keynum;
}

int CIniFile::FindValue( int keynum, const CString& valuename ) const
{
	if (keynum == -1)
		return -1;

	int valuenum = 0;

	while (valuenum < keys[keynum].names.GetSize() && keys[keynum].names[valuenum] != valuename)
		++valuenum;

	if (valuenum == keys[keynum].names.GetSize())
		return -1;

	return valuenum;
}