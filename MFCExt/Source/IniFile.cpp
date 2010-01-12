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
#include <sstream>
#include <vector>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//-------------------------------------------------------------------
// class CIniFile
//-------------------------------------------------------------------

namespace {
	template<class E, class T> 
	std::basic_ostream<E, T>& endl(std::basic_ostream<E, T>& os)
	{
		return os.put(os.widen('\r')).put(os.widen('\n')).flush();
	}

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
	ATL::CAtlFile file;

	if (FAILED(file.Create(path,GENERIC_READ,FILE_SHARE_READ,OPEN_EXISTING)))
		return false;

	ULONGLONG s = 0;
	
	if (FAILED(file.GetSize(s)))
		return false;

	if (!s)
		return true;

	std::vector<char> data(s);
	file.Read(&data[0],s);

	int cch = ::MultiByteToWideChar(CP_UTF8,0,&data[0],s,0,0);

	std::vector<wchar_t> buffer(cch + 1);
	::MultiByteToWideChar(CP_UTF8,0,&data[0],s,&buffer[0],cch + 1);

	std::basic_istringstream<TCHAR> inifile(&buffer[0]);

    CString readinfo;
	int curkey = -1, curval = -1;

	CString keyname, valuename, value;
	CString temp;

    std::basic_string<TCHAR> line;

    while (getline(inifile,line)) {
        readinfo.SetString(line.c_str(),line.size());
		readinfo.Trim(_T('\r'));
		readinfo.Trim(_T('\n'));

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
	std::basic_ostringstream<TCHAR> inifile;

	for (int keynum = 0; keynum <= names.GetUpperBound(); keynum++) {
		if (!keys[keynum].names.IsEmpty()) {
			inifile <<_T('[') << names[keynum] <<_T(']') << endl;

			for (int valuenum = 0; valuenum <= keys[keynum].names.GetUpperBound(); valuenum++) {
				inifile << keys[keynum].names[valuenum] <<_T("=") << keys[keynum].values[valuenum];

				if (valuenum != keys[keynum].names.GetUpperBound())
					inifile << endl;
				else
					if (keynum < names.GetSize())
						inifile << endl;
			}

			if (keynum < names.GetSize())
				inifile << endl;
		}
	}

	// TODO: Handle legacy projects
	std::basic_string<TCHAR> s = inifile.str();
	
	int cch = ::WideCharToMultiByte(CP_UTF8,0,s.c_str(),s.size(),0,0,0,0);
	
	std::vector<char> data(cch);
	::WideCharToMultiByte(CP_UTF8,0,s.c_str(),s.size(),&data[0],cch,0,0);

	ATL::CAtlFile file;
	HRESULT result = file.Create(path,GENERIC_WRITE,FILE_SHARE_READ,CREATE_ALWAYS);

	if (SUCCEEDED(result))
		file.Write(&data[0],data.size());
	else 
		TRACE("%s failed, code: %x\n", __FUNCTION__, result);
}

void CIniFile::Reset()
{
	keys.SetSize(0);
	names.SetSize(0);
}

INT_PTR CIniFile::GetNumKeys() const
{
	return keys.GetSize();
}

INT_PTR CIniFile::GetNumValues( const CString& keyname ) const
{
	INT_PTR keynum = FindKey(keyname);

	if (keynum == -1)
		return -1;
	else
		return keys[keynum].names.GetSize();
}

bool CIniFile::VerifyValue( const CString& keyname, const CString& valuename ) const
{
	INT_PTR	keynum = FindKey(keyname);
	INT_PTR	valuenum = FindValue(keynum, valuename);

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
	INT_PTR keynum = FindKey(keyname), valuenum = FindValue(keynum,valuename);

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
	INT_PTR keynum = FindKey(key), valuenum = 0;
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
	INT_PTR keynum = FindKey(keyname), valuenum = FindValue(keynum,valuename);

	if (keynum == -1 || valuenum == -1)
		return 0;

	keys[keynum].names.RemoveAt(valuenum);
	keys[keynum].values.RemoveAt(valuenum);
	return 1;
}

bool CIniFile::DeleteKey(const CString& keyname)
{
	INT_PTR keynum = FindKey(keyname);
	
	if (keynum == -1)
		return 0;

	keys.RemoveAt(keynum);
	names.RemoveAt(keynum);
	return 1;
}

INT_PTR CIniFile::FindKey( const CString& keyname ) const
{
	INT_PTR keynum = 0;
	while ( keynum < keys.GetSize() && names[keynum] != keyname)
		++keynum;
	if (keynum == keys.GetSize())
		return -1;
	return keynum;
}

INT_PTR CIniFile::FindValue( INT_PTR keynum, const CString& valuename ) const
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

bool CIniFile::GetValues( const CString& key, std::multimap<CString,CString>& result ) const
{
	result.clear();
	INT_PTR index = FindKey(key);

	if (index != -1) 
	{
		const INT_PTR size = keys[index].names.GetSize();

		for (int i = 0; i < size; ++i)
			result.insert(std::make_pair(keys[index].names[i],keys[index].values[i]));
	}

	return index != -1;
}