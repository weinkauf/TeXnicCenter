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

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//-------------------------------------------------------------------
// class CIniFile
//-------------------------------------------------------------------

CIniFile::CIniFile(CString inipath /*= ""*/)
:	path( inipath )
{}


CIniFile::~CIniFile()
{}


void CIniFile::SetPath(CString newpath)
{
	path = newpath;
}

CString CIniFile::GetPath()
{
	return path;
}


BOOL CIniFile::ReadFile()
{
	CFile file;
	CFileStatus status;
	if (!file.GetStatus(path,status))
		return 0;
	ifstream inifile;
	CString readinfo;
	inifile.open(path);
	int curkey = -1, curval = -1;
	if (inifile.fail())
		return 0;

	CString keyname, valuename, value;
	CString temp;
	while (getline(inifile,readinfo))
	{
		if (readinfo != "")
		{
			if (readinfo[0] == '[' && readinfo[readinfo.GetLength()-1] == ']') //if a section heading
			{
				keyname = readinfo;
				keyname.TrimLeft('[');
				keyname.TrimRight(']');
			}
			else //if a value
			{
				valuename = readinfo.Left(readinfo.Find("="));
				value = readinfo.Right(readinfo.GetLength()-valuename.GetLength()-1);
				SetValue(keyname,valuename,value);
			}
		}
	}
	inifile.close();
	return 1;
}


void CIniFile::WriteFile()
{
	ofstream inifile;
	inifile.open(path);
	for (int keynum = 0; keynum <= names.GetUpperBound(); keynum++)
	{
		if (keys[keynum].names.GetSize() != 0)
		{
			inifile << '[' << names[keynum] << ']' << endl;
			for (int valuenum = 0; valuenum <= keys[keynum].names.GetUpperBound(); valuenum++)
			{
				inifile << keys[keynum].names[valuenum] << "=" << keys[keynum].values[valuenum];
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
	inifile.close();
}


void CIniFile::Reset()
{
	keys.SetSize(0);
	names.SetSize(0);
}


int CIniFile::GetNumKeys()
{
	return keys.GetSize();
}


int CIniFile::GetNumValues(CString keyname)
{
	int keynum = FindKey(keyname);
	if (keynum == -1)
		return -1;
	else
		return keys[keynum].names.GetSize();
}


BOOL CIniFile::VerifyValue(CString keyname, CString valuename)
{
	int	keynum = FindKey(keyname);
	int	valuenum = FindValue(keynum, valuename);

	if (keynum == -1 || valuenum == -1)
		return FALSE;
	else
		return TRUE;
}


BOOL CIniFile::VerifyKey(CString keyname)
{
	if( FindKey(keyname) == -1 )
		return FALSE;
	else
		return TRUE;
}


CString CIniFile::GetValue(CString keyname, CString valuename, CString strDefault)
{
	int keynum = FindKey(keyname), valuenum = FindValue(keynum,valuename);

	if (keynum == -1)
		return strDefault;

	if (valuenum == -1)
		return strDefault;

	return keys[keynum].values[valuenum];
}


int CIniFile::GetValue(CString keyname, CString valuename, int nDefault)
{
	CString	strDefault;
	strDefault.Format( _T("%d"), nDefault );
	return _ttol( GetValue(keyname, valuename, strDefault) );
}


double CIniFile::GetValue(CString keyname, CString valuename, double dDefault)
{
	CString	strDefault;
	strDefault.Format( _T("%e"), dDefault );
	return _ttoi( GetValue(keyname, valuename, strDefault ) );
}


BOOL CIniFile::SetValue(CString keyname, CString valuename, CString value, BOOL create)
{
	int keynum = FindKey(keyname), valuenum = 0;
	//find key
	if (keynum == -1) //if key doesn't exist
	{
		if (!create) //and user does not want to create it,
			return 0; //stop entering this key
		names.SetSize(names.GetSize()+1);
		keys.SetSize(keys.GetSize()+1);
		keynum = names.GetSize()-1;
		names[keynum] = keyname;
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


BOOL CIniFile::SetValue(CString keyname, CString valuename, int value, BOOL create)
{
	CString temp;
	temp.Format("%d",value);
	return SetValue(keyname, valuename, temp, create);
}


BOOL CIniFile::SetValue(CString keyname, CString valuename, double value, BOOL create)
{
	CString temp;
	temp.Format("%e",value);
	return SetValue(keyname, valuename, temp, create);
}


BOOL CIniFile::DeleteValue(CString keyname, CString valuename)
{
	int keynum = FindKey(keyname), valuenum = FindValue(keynum,valuename);
	if (keynum == -1 || valuenum == -1)
		return 0;

	keys[keynum].names.RemoveAt(valuenum);
	keys[keynum].values.RemoveAt(valuenum);
	return 1;
}


BOOL CIniFile::DeleteKey(CString keyname)
{
	int keynum = FindKey(keyname);
	if (keynum == -1)
		return 0;
	keys.RemoveAt(keynum);
	names.RemoveAt(keynum);
	return 1;
}


int CIniFile::FindKey(CString keyname)
{
	int keynum = 0;
	while ( keynum < keys.GetSize() && names[keynum] != keyname)
		keynum++;
	if (keynum == keys.GetSize())
		return -1;
	return keynum;
}


int CIniFile::FindValue(int keynum, CString valuename)
{
	if (keynum == -1)
		return -1;
	int valuenum = 0;
	while (valuenum < keys[keynum].names.GetSize() && keys[keynum].names[valuenum] != valuename)
		valuenum++;
	if (valuenum == keys[keynum].names.GetSize())
		return -1;
	return valuenum;
}


istream & CIniFile:: getline(istream & is, CString & str)
{
    char buf[2048];
    is.getline(buf,2048);
    str = buf;
    return is;
}
