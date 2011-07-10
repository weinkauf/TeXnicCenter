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
#include "PathTool.h"
#include <Shlwapi.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//-------------------------------------------------------------------
// class CPathTool
//-------------------------------------------------------------------

CPathTool::CPathTool( LPCTSTR lpszPath /*= NULL*/ )
{
	if( lpszPath )
		m_strPath = lpszPath;
}


/////////////////////////////////////////////////////////////////////
// statics

CString CPathTool::Cat( LPCTSTR lpszBasePath, LPCTSTR lpszPath, const bool bCanonicalize /*= false*/ )
{
	CString	strBasePath = GetBackslashPath( lpszBasePath );
	CString	strPath = GetBackslashPath( lpszPath );
	BOOL		bWasSlashPath = IsSlashPath( lpszBasePath );

	// remove all backslashs from the end of base path
	while( strBasePath.GetLength() && strBasePath[strBasePath.GetLength() - 1] == _T('\\') )
		strBasePath = strBasePath.Left( strBasePath.GetLength() - 1 );

	// remove all backslashs from start of path
	while( strPath.GetLength() && strPath[0] == _T('\\') )
		strPath = strPath.Right( strPath.GetLength() - 1 );

	// append path to base path
	strBasePath+= _T('\\') + strPath;

	//Canonicalize - resolve relative path components
	if (bCanonicalize) strBasePath = GetCanonicalPath(strBasePath);

	return bWasSlashPath? GetSlashPath( strBasePath ) : strBasePath;
}


CString	CPathTool::Format( LPCTSTR lpszText, LPCTSTR lpszPath )
{
	CString		strFormat( lpszText );
	CPathTool	path( lpszPath );

	// store '%'
	strFormat.Replace( _T("%%"), _T("\a") );

	// replace place holders
	path.EnsureBackslashPath();
	path.EnsureLongPath();
	strFormat.Replace( _T("%p"), path.GetPath() );
	strFormat.Replace( _T("%r"), path.GetDrive() );
	strFormat.Replace( _T("%b"), CPathTool::Cat( path.GetDirectory(), path.GetFileTitle() ) );
	strFormat.Replace( _T("%d"), path.GetDirectory() );
	strFormat.Replace( _T("%n"), path.GetFile() );
	strFormat.Replace( _T("%t"), path.GetFileTitle() );
	strFormat.Replace( _T("%e"), path.GetFileExtension() );

	path.EnsureSlashPath();
	strFormat.Replace( _T("%P"), path.GetPath() );
	strFormat.Replace( _T("%B"), CPathTool::Cat( path.GetDirectory(), path.GetFileTitle() ) );
	strFormat.Replace( _T("%D"), path.GetDirectory() );

	path.EnsureBackslashPath();
	path.EnsureShortPath();
	strFormat.Replace( _T("%sp"), path.GetPath() );
	strFormat.Replace( _T("%sr"), path.GetDrive() );
	strFormat.Replace( _T("%sb"), CPathTool::Cat( path.GetDirectory(), path.GetFileTitle() ) );
	strFormat.Replace( _T("%sd"), path.GetDirectory() );
	strFormat.Replace( _T("%sn"), path.GetFile() );
	strFormat.Replace( _T("%st"), path.GetFileTitle() );
	strFormat.Replace( _T("%se"), path.GetFileExtension() );

	path.EnsureSlashPath();
	strFormat.Replace( _T("%sP"), path.GetPath() );
	strFormat.Replace( _T("%sB"), CPathTool::Cat( path.GetDirectory(), path.GetFileTitle() ) );
	strFormat.Replace( _T("%sD"), path.GetDirectory() );

	// restore '%'
	strFormat.Replace( _T("\a"), _T("%") );

	return strFormat;
}


CString CPathTool::GetDrive( LPCTSTR lpszPath )
{
	CString	strPath( lpszPath );

	if( strPath.GetLength() < 2 || strPath[1] != _T(':') )
		return CString();

	return strPath.Left( 2 );
}


CString CPathTool::GetDirectory( LPCTSTR lpszPath )
{
	CString	strPath = GetBackslashPath( lpszPath );
	BOOL bWasSlashPath = IsSlashPath( lpszPath );

	//Find last backslash
	int nIndex = strPath.ReverseFind( _T('\\') );
	if (nIndex < 0)
	{
		return CString();
	}

	if (nIndex == (strPath.GetLength() - 1))
	{
		//of the kind "C:\", etc. - i.e., with an ending backslash
		return bWasSlashPath ? GetSlashPath( strPath ) : strPath;
	}

	return bWasSlashPath ? GetSlashPath( strPath.Left( nIndex ) ) : strPath.Left( nIndex );
}


CString CPathTool::GetParentDirectory( LPCTSTR lpszPath )
{
	CString	strPath = GetBackslashPath( lpszPath );
	BOOL bWasSlashPath = IsSlashPath( lpszPath );

	//Find last backslash
	int nIndex = strPath.ReverseFind( _T('\\') );
	if( nIndex < 0 )
	{
		return CString();
	}

	return bWasSlashPath ? GetSlashPath( strPath.Left( nIndex ) ) : strPath.Left( nIndex );
}


CString CPathTool::GetDirectoryWithoutDrive( LPCTSTR lpszPath )
{
	CString	strDir = GetDirectory( GetBackslashPath( lpszPath ) );
	BOOL		bWasSlashPath = IsSlashPath( lpszPath );

	if( GetDrive( strDir ).IsEmpty() )
		return bWasSlashPath? GetSlashPath( strDir ) : strDir;
	else
	{
		strDir = strDir.Right( strDir.GetLength() - 2 );
		return bWasSlashPath? GetSlashPath( strDir ) : strDir;
	}
}


CString CPathTool::GetBase( LPCTSTR lpszPath )
{
	return Cat( GetDirectory( lpszPath ), GetFileTitle( lpszPath ) );
}


CString CPathTool::GetFile( LPCTSTR lpszPath )
{
	CString	strPath( GetBackslashPath( lpszPath ) );
	int			nIndex;

	// find file
	nIndex = strPath.ReverseFind( _T('\\') );
	if( nIndex < 0 && !GetDrive( lpszPath ).IsEmpty() )
		return CString();	// only a drive description
	else if( nIndex < 0 )
		return strPath; // all belongs to the file description -- no directory
	else if( strPath )
		return strPath.Right( strPath.GetLength() - nIndex - 1 );
	else
		return CString();
}



CString CPathTool::GetFileExtension( LPCTSTR lpszPath )
{
	CString	strFile = GetFile( lpszPath );

	// file found?
	if( strFile.IsEmpty() )
		return CString();	// no file

	// find extension
	int			nIndex;

	nIndex = strFile.ReverseFind( _T('.') );
	if ( ( nIndex < 0 ) || (nIndex == strFile.GetLength() - 1) )
	{
		return CString();	// no or empty (i.e. ends with a point) extension
	}

	return strFile.Right( strFile.GetLength() - nIndex - 1 );
}



CString CPathTool::GetFileTitle( LPCTSTR lpszPath )
{
	CString	strFile = GetFile( lpszPath );

	// file found?
	if( strFile.IsEmpty() )
		return CString(); // no file

	// find extension
	int	nIndex;

	nIndex = strFile.ReverseFind( _T('.') );
	if( nIndex < 0 )
		return strFile;
	else
		return strFile.Left( nIndex );
}


CString CPathTool::GetShortPath( LPCTSTR lpszPath )
{
	CString	strShortPath;
	DWORD		dwResult;
	BOOL		bWasSlashPath = IsSlashPath( lpszPath );

	dwResult = GetShortPathName( GetBackslashPath( lpszPath ), strShortPath.GetBuffer( _MAX_PATH ), _MAX_PATH );
	strShortPath.ReleaseBuffer( dwResult? -1 : 0 );

	return bWasSlashPath? GetSlashPath( strShortPath ) : strShortPath;
}


CString CPathTool::GetLongPath( LPCTSTR lpszPath )
{
	// GetLongPathName() at this time only supported for Win98 and WinNT 5 (Win2000)
	/*
	CString	strLongPath;
	DWORD		dwResult;
	BOOL		bWasSlashPath = IsSlashPath( lpszPath );

	dwResult = GetLongPathName( GetBackslashPath( lpszPath ), strLongPath.GetBuffer( _MAX_PATH ), _MAX_PATH );
	strLongPath.ReleaseBuffer( dwResult? -1 : 0 );

	return bWasSlashPath? GetSlashPath( strLongPath ) : strLongPath;
	*/
	return lpszPath;
}


CString	CPathTool::GetSlashPath( LPCTSTR lpszPath )
{
	CString	strPath( lpszPath );

	strPath.Replace( _T('\\'), _T('/') );

	return strPath;
}


CString	CPathTool::GetBackslashPath( LPCTSTR lpszPath )
{
	CString	strPath( lpszPath );

	strPath.Replace( _T('/'), _T('\\') );

	return strPath;
}


CString CPathTool::GetRelativePath( LPCTSTR lpszFrom, LPCTSTR lpszTo, BOOL bFromIsDir, BOOL bToIsDir )
{
	CString	strResult;
	BOOL		bResult;
	CString	strFrom( GetBackslashPath( lpszFrom ) );
	CString	strTo( GetBackslashPath( lpszTo ) );
	BOOL		bWasSlashPath = IsSlashPath( lpszFrom ) && IsSlashPath( lpszTo );

	if( strFrom.Right( 1 ) == _T('\\') && strFrom.GetLength() > 1)
		strFrom = strFrom.Left( strFrom.GetLength() - 1 );
	if( strTo.Right( 1 ) == _T('\\') && strTo.GetLength() > 1)
		strTo = strTo.Left( strTo.GetLength() - 1 );

	bResult = PathRelativePathTo( 
		strResult.GetBuffer( MAX_PATH ), 
		strFrom, bFromIsDir? FILE_ATTRIBUTE_DIRECTORY : FILE_ATTRIBUTE_NORMAL, 
		strTo, bToIsDir? FILE_ATTRIBUTE_DIRECTORY : FILE_ATTRIBUTE_NORMAL );
	strResult.ReleaseBuffer();

	// If the the PathRelativePathTo failed, the paths are on different drives.
	if( !bResult )
		strResult = strTo;

	// remove leading ".\"
	while (strResult.Left(2) == _T(".\\"))
		strResult = strResult.Right(strResult.GetLength()-2);
	
	return bWasSlashPath? GetSlashPath( strResult ) : strResult;
}


CString CPathTool::GetRelativePath( LPCTSTR lpszFrom, LPCTSTR lpszTo )
{
	return GetRelativePath(
		lpszFrom, lpszTo, 
		Exists(lpszFrom)? IsDirectory(lpszFrom) : FALSE, 
		Exists(lpszTo)? IsDirectory(lpszTo) : FALSE);
}

CString CPathTool::GetAbsolutePath( LPCTSTR lpszDirectory, LPCTSTR lpszRelative, const bool bCanonicalize /*= false*/ )
{
	if (!IsRelativePath(lpszRelative)) return lpszRelative;
	if (IsRelativePath(lpszDirectory)) return lpszRelative;

	return Cat(lpszDirectory, lpszRelative, bCanonicalize);
}

CString CPathTool::GetCanonicalPath( LPCTSTR lpszPath )
{
	CString strCanonicalPath;
	const BOOL bSuccess = PathCanonicalize(strCanonicalPath.GetBuffer(MAX_PATH), lpszPath);
	strCanonicalPath.ReleaseBuffer();
	if (bSuccess)
		return strCanonicalPath;
	else
		return CString(lpszPath);
}

BOOL CPathTool::Exists( LPCTSTR lpszPath )
{
	return (IsFile(lpszPath) || IsDirectory(lpszPath));
}


BOOL CPathTool::IsDrive( LPCTSTR lpszPath )
{
	CString	strPath( lpszPath );
	CString	strDrive = GetDrive( lpszPath );

	if( !strDrive.IsEmpty() && strPath.GetLength() == 2 )
		return TRUE;
	else
		return FALSE;
}


BOOL CPathTool::IsDirectory( LPCTSTR lpszPath )
{
	if (lpszPath == NULL) return false;
	if (lpszPath[0] == _T('\0')) return false;

	//We test the following way:
	//If given path is a directory, we can always
	//make a grep in that directory using "Path\\*".
	//If it is not a directory, this will fail.
	//So this will work for drives like "A:" or "C:" as well.
	CString TestPath = GetBackslashPath(lpszPath);
	TestPath.TrimLeft();
	TestPath.TrimRight();
	if (TestPath[TestPath.GetLength()-1] != _T('\\'))
	{
		TestPath += "\\*";
	}
	else
	{
		TestPath += "*";
	}

	CFileFind ff;
	BOOL bResult = ff.FindFile(TestPath);
	ff.Close();

	return bResult;
}


BOOL CPathTool::IsFile( LPCTSTR lpszPath )
{
	if (lpszPath == NULL) return false;
	if (lpszPath[0] == _T('\0')) return false;

	CFileFind	ff;
	if (!ff.FindFile(GetBackslashPath(lpszPath)))
		return FALSE;

	ff.FindNextFile();

	BOOL	bResult = !ff.IsDirectory();
	ff.Close();

	return bResult;
}


BOOL CPathTool::IsSlashPath( LPCTSTR lpszPath )
{
	CString	strPath( lpszPath );

	if( strPath.Find( _T('/') ) > -1 )
		return TRUE;
	else
		return FALSE;
}

BOOL CPathTool::IsRelativePath( LPCTSTR lpszPath )
{
	return (GetDrive(lpszPath).GetLength() == 0);
}

CString CPathTool::GetTempFileName(LPCTSTR lpPathName, LPCTSTR lpPrefixString, UINT uUnique)
{
	CString fileName;
	UINT result = ::GetTempFileName(lpPathName, lpPrefixString, uUnique, 
		fileName.GetBufferSetLength(MAX_PATH + 1));
	// Discard the string buffer if an error occurred, i.e., result is 0
	fileName.ReleaseBuffer(result == 0 ? 0 : -1);

	return fileName;
}
