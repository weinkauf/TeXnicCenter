/********************************************************************
*
* This file is part of the MFCExt-Library
*
* Copyright (C) 1999-2000 Sven Wiegand
* Copyright (C) 2000-2001 ToolsCenter
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

#if !defined(AFX_PATHTOOL_H__837269C0_19DC_11D4_A221_006097239934__INCLUDED_)
#define AFX_PATHTOOL_H__837269C0_19DC_11D4_A221_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/**
The class CPathTool provides a lot of Methods for handling path
descriptions, such as gettingt the file of a path, getting the
file extension of a path, getting a relative path, catenating
two paths, etc.

@author Sven Wiegand
*/
class AFX_EXT_CLASS CPathTool  
{
// construction/destruction
public:
	/**
	Constructs a CPathTool-object.

	@param lpszPath
		Path to perfomr operations with.
	*/
	CPathTool( LPCTSTR lpszPath = NULL );

// static
public:
	/**
	Replaces the place holders in lpszText with the path lpszPath.

	Place holders are having the following syntax: %[s]<p>, where
	[s] means, that the real place holder <p> can (but not must) be
	prefixed by an 's', which has the effect, that the real place
	holder <p> will be interpreted in 8.3-format (short path names).

	To get a '%', insert "%%".

	Valid characters for <p> are:
	<ul>
		<li><b>%p</b> Full path.
		<li><b>%b</b> Base path -- full path without file extension.
		<li><b>%r</b> Drive letter including colon (':').
		<li><b>%d</b> Directory of the path.
		<li><b>%n</b> Full file name (including extension).
		<li><b>%t</b> Title of the file (name without extension)
		<li><b>%e</b> File extension.
		<li><b>%P</b> Same as %p but using slash ('/') as path seperator.
		<li><b>%B</b> Same as %b but using slash ('/') as path seperator.
		<li><b>%D</b> Same as %d but using slash ('/') as path seperator.
	</ul>

	If the text "app.exe %D/otherFile.%e" and the path
	"C:\Temp\MyFile\test.txt" would be specified, the result would be
	"C:/Temp/otherFile.txt".

	If the text "%sp" and the path 
	"C:\Program Files\SomeApp\Application.exe" would be specified, the
	result would be "C:\Progra~1\SomeApp\Applic~1.exe".
	*/
	static CString Format( LPCTSTR lpszText, LPCTSTR lpszPath );

	/**
	Catenates two path descriptions by ensuring, that there is exactly
	one Backslash ('\') between the both pathts, no matter, if 
	lpszBasePath already ends with a backslash or lpszPath already
	starts with a backslash.

	@param lpszBasePath
		Path, lpszPath should be appended to.
	@param lpszPath
		Path, that should be appende to lpszBasePath.

	@return
		The two paths catenated.
	*/
	static CString Cat( LPCTSTR lpszBasePath, LPCTSTR lpszPath );

	/**
	Returns the drive letter (including the colon (':')) of the path or 
	an empty string, if there	is no drive letter in the path.

	Example: Would return "C:" for "C:\MyFiles\Test.txt".

	@param lpszPath
		Path to analyze.
	*/
	static CString GetDrive( LPCTSTR lpszPath );

	/**
	Returns the directory of the path.

	Example: Would return "C:\MyFiles" for "C:\MyFiles\Text.txt".

	@param lpszPath
		Path to analyze.
	*/
	static CString GetDirectory( LPCTSTR lpszPath );

	/**
	Returns the directory excluding the drive.

	Example: Would return "MyFiles" for "C:\MyFiles\Text.txt".

	@param lpszPath
		Path to analyze.
	*/
	static CString GetDirectoryWithoutDrive( LPCTSTR lpszPath );

	/**
	Returns the complete path without file extension.

	Example: Would return "C:\MyFiles\Text" for "C:\MyFiles\Text.txt".

	@param lpszPath
		Path to return the base of.
	*/
	static CString GetBase( LPCTSTR lpszPath );

	/**
	Returns the file of the path.

	Example: Would return "Test.txt" for "C:\MyFiles\Test.txt".

	@param lpszPath
		Path to analyze.
	*/
	static CString GetFile( LPCTSTR lpszPath );

	/**
	Returns the file extension of the path without the dot ('.').

	Example: Would return "txt" for "C:\MyFiles\Test.txt".

	@param lpszPath
		Path to analyze.
	*/
	static CString GetFileExtension( LPCTSTR lpszPath );

	/**
	Returns the file title of the path.

	The file title ist the file without the extension.

	Example: Would return "Test" for "C:\MyFiles\Test.txt".

	@param lpszPath
		Path to analyze.
	*/
	static CString GetFileTitle( LPCTSTR lpszPath );

	/**
	Returns the path in the old 8.3-format or an empty string, if the
	function failed.

	Example: Would return "C:\Progra~1\TestAp~1.exe" for
	"C:\Program Files\TestApplication.exe"

	<b>Remark:</b> The specified file or directory must exist!

	@param lpszPath
		Path to convert to 8.3-format. The path must exist.
	*/
	static CString GetShortPath( LPCTSTR lpszPath );

	/**
	Converts the given 8.3-format path to a long path (elements can
	be longer than eight characters).

	Example: Would return "C:\Program Files\TestApplication.exe" for
	"C:\Progra~1\TestAp~1.exe"

	<b>Remark:</b> The specified file or directory must exist!

	@param lpszPath
		Path to convert from 8.3-format. The path must exist.
	*/
	static CString GetLongPath( LPCTSTR lpszPath );

	/**
	Returns the path with slashes ('/') as path speperators instead
	of backslashes ('\'). Leaves the path unchanged, if slashes are
	already used as seperators.
	
	Example: Would return "C:/MyFiles/Test.txt" for "C:\MyFiles\Test.txt"

	@param lpszPath
		Path to convert.
	*/
	static CString GetSlashPath( LPCTSTR lpszPath );

	/**
	Returns the path with backslashes ('\') as path speperators instead
	of slashes ('/'). Leaves the path unchanged, if backslashes are
	already used as seperators.
	
	Example: Would return "C:\MyFiles\Test.txt" for "C:/MyFiles/Test.txt"

	@param lpszPath
		Path to convert.
	*/
	static CString GetBackslashPath( LPCTSTR lpszPath );

	/**
	Returns a path that describes the position of lpszFrom path
	relative to lpszTo.

	Remarks: The specified paths do not need to exist really.

	@param lpszFrom
		Path from where the relative path to lpszTo should be generated.
	@param lpszTo
		Path to where the relative path from lpszFrom should be generated.
	@param bFromIsDir
		TRUE if lpszFrom specifies a directory, FALSE otherwise.
	@param bToIsDir
		TRUE if lpszTo specifies a directory, FALSE otherwise.
	*/
	static CString GetRelativePath( LPCTSTR lpszFrom, LPCTSTR lpszTo, BOOL bFromIsDir, BOOL bToIsDir );

	/**
	Returns a path that describes the position of lpszFrom path
	relative to lpszTo.

	Remarks: The specified paths must exist, otherwise an empty string
	is returned.

	@param lpszFrom
		Path from where the relative path to lpszTo should be generated.
	@param lpszTo
		Path to where the relative path from lpszFrom should be generated.
	*/
	static CString GetRelativePath( LPCTSTR lpszFrom, LPCTSTR lpszTo );

	/**
	Returns TRUE, if the specified path really exists in the file 
	system.

	Remarks: If you do not specify a whole path (including drive), the
	result depends on the current directory.
	*/
	static BOOL Exists( LPCTSTR lpszPath );

	/**
	Returns TRUE if the specified path is just a drive.

	Remarks: The function only tries to determine the result out of the 
	name, it does not check, if the specified path really exists and
	if it is of the specified type.

	Example: Would return TRUE for "C:", "A:", etc.
	Would return FALSE for "C:\MyFiles\Test.txt", "Test.txt", etc.
	*/
	static BOOL IsDrive( LPCTSTR lpszPath );

	/**
	Returns TRUE if the specified path really exists in the file system
	and if it is a directory.

	Remarks: If you do not specify a whole path (including drive), the
	result depends on the current directory.
	*/
	static BOOL IsDirectory( LPCTSTR lpszPath );

	/**
	Returns TRUE if the specified path really exists in the file system
	and if it is a file.

	Remarks: If you do not specify a whole path (including drive), the
	result depends on the current directory.
	*/
	static BOOL IsFile( LPCTSTR lpszPath );

	/**
	Returns TRUE, if slashes are used as path separators or FALSE, if
	backslashes are used or no separators are in the path.

	@param lpszPath
		Path to check.
	*/
	static BOOL IsSlashPath( LPCTSTR lpszPath );

// basics
public:
	/**
	Sets the path to work with.

	@param lpszPath
		Path to work with.
	*/
	void SetPath( LPCTSTR lpszPath );

	/**
	Returns the path.
	*/
	CString GetPath() const;

	/**
	Returns the pointer to the path description.
	*/
	operator LPCTSTR() const;

// operations
public:
	/**
	Converts the path to the old 8.3-format or leaves it unchanged,
	if it is already in 8.3-format or if the function fails.

	Example: Would convert to "C:\Progra~1\TestAp~1.exe" for
	"C:\Program Files\TestApplication.exe"

	<b>Remark:</b> The specified file or directory must exist!

	@return 
		TRUE if successfull, FALSE otherwise.
	*/
	BOOL EnsureShortPath();

	/**
	Converts the path to the long format or leaves it unchanged,
	if it is already in long format or if the function fails.

	Example: Would convert to "C:\Progra~1\TestAp~1.exe" for
	"C:\Program Files\TestApplication.exe"

	<b>Remark:</b> The specified file or directory must exist!

	@return 
		TRUE if successfull, FALSE otherwise.
	*/
	BOOL EnsureLongPath();

	/**
	Converts the path to slash-style or leaves it unchanged, if it
	is already in slash-style.

	In slash style, the slash ('/') is the path separator instead
	of the backslash ('\').
	*/
	void EnsureSlashPath();

	/**
	Converts the path to backslash-style or leaves it unchanged, if it
	is already in backslash-style.

	In backslash style, the backslash ('\') is the path separator instead
	of the slash ('/').
	*/
	void EnsureBackslashPath();

	/**
	Replaces the place holders in lpszText with the path.

	Place holders are having the following syntax: %[s]<p>, where
	[s] means, that the real place holder <p> can (but not must) be
	prefixed by an 's', which has the effect, that the real place
	holder <p> will be interpreted in 8.3-format (short path names).

	To get a '%', insert "%%".

	Valid characters for <p> are:
	<ul>
		<li><b>%p</b> Full path.
		<li><b>%b</b> Base path -- full path without file extension.
		<li><b>%r</b> Drive letter including colon (':').
		<li><b>%d</b> Directory of the path (excluding last backslash 
			('\')) (i.e. "C:\MyFiles\Test.txt" would result in "C:\MyFiles").
		<li><b>%n</b> Full file name (including extension).
		<li><b>%t</b> Title of the file (name without extension)
		<li><b>%e</b> File extension.
		<li><b>%P</b> Same as %p but using slash ('/') as path seperator.
		<li><b>%B</b> Same as %b but using slash ('/') as path seperator.
		<li><b>%D</b> Same as %d but using slash ('/') as path seperator.
	</ul>

	If the text "app.exe %D/otherFile.%e" and the path
	"C:\Temp\MyFile\test.txt" would be specified, the result would be
	"C:/Temp/otherFile.txt".

	If the text "%sp" and the path 
	"C:\Program Files\SomeApp\Application.exe" would be specified, the
	result would be "C:\Progra~1\SomeApp\Applic~1.exe".
	*/
	CString Format( LPCTSTR lpszText ) const;

	/**
	Returns the path to the old 8.3-format or an empty string, if the
	function failed.

	Example: Would return "C:\Progra~1\TestAp~1.exe" for
	"C:\Program Files\TestApplication.exe"

	<b>Remark:</b> The specified file or directory must exist!
	*/
	CString GetShortPath() const;

	/**
	Returns the converted to a long path (elements can
	be longer than eight characters).

	Example: Would return "C:\Program Files\TestApplication.exe" for
	"C:\Progra~1\TestAp~1.exe"

	<b>Remark:</b> The specified file or directory must exist!
	*/
	CString GetLongPath() const;

	/**
	Returns the path with slashes ('/') as path speperators instead
	of backslashes ('\'). Leaves the path unchanged, if slashes are
	already used as seperators.
	
	Example: Would return "C:/MyFiles/Test.txt" for "C:\MyFiles\Test.txt"
	*/
	CString GetSlashPath() const;

	/**
	Returns the path with backslashes ('\') as path speperators instead
	of slashes ('/'). Leaves the path unchanged, if backslashes are
	already used as seperators.
	
	Example: Would return "C:\MyFiles\Test.txt" for "C:/MyFiles/Test.txt"
	*/
	CString GetBackslashPath() const;

	/**
	Returns the drive letter (including the colon (':')) of the path or 
	an empty string, if there	is no drive letter in the path.

	Example: Would return "C:" for "C:\MyFiles\Test.txt".
	*/
	CString GetDrive() const;

	/**
	Returns the directory of the path.

	Example: Would return "C:\MyFiles" for "C:\MyFiles\Text.txt".
	*/
	CString GetDirectory() const;

	/**
	Returns the directory excluding the drive.

	Example: Would return "MyFiles" for "C:\MyFiles\Text.txt".
	*/
	CString GetDirectoryWithoutDrive() const;

	/**
	Returns the full path without file extension.

	Example: Would return "C:\MyFiles\Text" for "C:\MyFiles\Text.txt".
	*/
	CString GetBase() const;

	/**
	Returns the file of the path.

	Example: Would return "Test.txt" for "C:\MyFiles\Test.txt".
	*/
	CString GetFile() const;

	/**
	Returns the file extension of the path.

	Example: Would return "txt" for "C:\MyFiles\Test.txt".
	*/
	CString GetFileExtension() const;

	/**
	Returns the file title of the path.

	The file title ist the file without the extension.

	Example: Would return "Test" for "C:\MyFiles\Test.txt".
	*/
	CString GetFileTitle() const;

	/**
	Appends lpszPath to the path by ensuring, that the both parts are
	separated by exactly one backslash ('\').

	@param lpszPath
		Path to append.

	@return
		The new path.
	*/
	CString Append( LPCTSTR lpszPath );

	/**
	Appends lpszPath to the path by ensuring, that the both parts are
	separated by exactly one backslash ('\').

	@param lpszPath
		Path to append.

	@return
		The new path.
	*/
	CString operator+=( LPCTSTR lpszPath );

	/**
	Returns a path that describes the position of lpszFrom path
	relative to lpszTo.

	Remarks: The specified paths do not need to exist really.

	@param lpszTo
		Path to where the relative path from lpszFrom should be generated.
	@param bFromIsDir
		TRUE if this path specifies a directory, FALSE otherwise.
	@param bToIsDir
		TRUE if lpszTo specifies a directory, FALSE otherwise.
	*/
	CString GetRelativePath( LPCTSTR lpszTo, BOOL bFromIsDir, BOOL bToIsDir ) const;

	/**
	Returns a path that describes the position of lpszFrom path
	relative to lpszTo.

	Remarks: The specified paths must exist, otherwise an empty string
	is returned.

	@param lpszTo
		Path to where the relative path from lpszFrom should be generated.
	*/
	CString GetRelativePath( LPCTSTR lpszTo ) const;

	/**
	Returns TRUE, if the specified path really exists in the file 
	system.

	Remarks: If you do not specify a whole path (including drive), the
	result depends on the current directory.
	*/
	BOOL Exists() const;

	/**
	Returns TRUE if the specified path is just a drive.

	Remarks: The function only tries to determine the result out of the 
	name, it does not check, if the specified path really exists and
	if it is of the specified type.

	Example: Would return TRUE for "C:", "A:", etc.
	Would return FALSE for "C:\MyFiles\Test.txt", "Test.txt", etc.
	*/
	BOOL IsDrive() const;

	/**
	Returns TRUE if the specified path really exists in the file system
	and if it is a directory.

	Remarks: If you do not specify a whole path (including drive), the
	result depends on the current directory.
	*/
	BOOL IsDirectory() const;

	/**
	Returns TRUE if the specified path really exists in the file system
	and if it is a file.

	Remarks: If you do not specify a whole path (including drive), the
	result depends on the current directory.
	*/
	BOOL IsFile() const;

	/**
	Returns TRUE, if slashes are used as path separators or FALSE, if
	backslashes are used or no separators are in the path.
	*/
	BOOL IsSlashPath() const;

// Attributes:
protected:
	/** The path */
	CString m_strPath;
};


// Inlines
inline
void CPathTool::SetPath( LPCTSTR lpszPath )
{
	m_strPath = lpszPath;
}


inline
CString CPathTool::GetPath() const
{
	return m_strPath;
}


inline 
BOOL CPathTool::EnsureShortPath()
{
	CString	strPath = GetShortPath();
	if( strPath.IsEmpty() )
		return FALSE;

	m_strPath = strPath;
	return TRUE;
}


inline
BOOL CPathTool::EnsureLongPath()
{
	CString	strPath = GetLongPath();
	if( strPath.IsEmpty() )
		return FALSE;

	m_strPath = strPath;
	return TRUE;
}


inline
void CPathTool::EnsureSlashPath()
{
	m_strPath = GetSlashPath();
}


inline
void CPathTool::EnsureBackslashPath()
{
	m_strPath = GetBackslashPath();
}


inline
CString CPathTool::Format( LPCTSTR lpszText ) const
{
	return Format( m_strPath, lpszText );
}


inline
CString CPathTool::GetShortPath() const
{
	return GetShortPath( m_strPath );
}


inline
CString CPathTool::GetLongPath() const
{
	return GetLongPath( m_strPath );
}


inline
CString CPathTool::GetSlashPath() const
{
	return GetSlashPath( m_strPath );
}


inline
CString CPathTool::GetBackslashPath() const
{
	return GetBackslashPath( m_strPath );
}


inline
CPathTool::operator LPCTSTR() const
{
	return (LPCTSTR)m_strPath;
}


inline
CString CPathTool::GetDrive() const
{
	return GetDrive( m_strPath );
}


inline
CString CPathTool::GetDirectory() const
{
	return GetDirectory( m_strPath );
}


inline
CString CPathTool::GetDirectoryWithoutDrive() const
{
	return GetDirectoryWithoutDrive( m_strPath );
}


inline
CString CPathTool::GetBase() const
{
	return GetBase( m_strPath );
}


inline
CString CPathTool::GetFile() const
{
	return GetFile( m_strPath );
}


inline
CString CPathTool::GetFileExtension() const
{
	return GetFileExtension( m_strPath );
}


inline
CString CPathTool::GetFileTitle() const
{
	return GetFileTitle( m_strPath );
}


inline
CString CPathTool::Append( LPCTSTR lpszPath )
{
	return m_strPath = Cat( m_strPath, lpszPath );
}


inline
CString CPathTool::operator+=( LPCTSTR lpszPath )
{
	return Append( lpszPath );
}


inline
CString CPathTool::GetRelativePath( LPCTSTR lpszTo, BOOL bFromIsDir, BOOL bToIsDir ) const
{
	return GetRelativePath( m_strPath, lpszTo, bFromIsDir, bToIsDir );
}


inline
CString CPathTool::GetRelativePath( LPCTSTR lpszTo ) const
{
	return GetRelativePath( m_strPath, lpszTo );
}


inline
BOOL CPathTool::Exists() const
{
	return Exists( m_strPath );
}


inline
BOOL CPathTool::IsDrive() const
{
	return IsDrive( m_strPath );
}


inline
BOOL CPathTool::IsDirectory() const
{
	return IsDirectory( m_strPath );
}


inline
BOOL CPathTool::IsFile() const
{
	return IsFile( m_strPath );
}


inline
BOOL CPathTool::IsSlashPath() const
{
	return IsSlashPath( m_strPath );
}

#endif // !defined(AFX_PATHTOOL_H__837269C0_19DC_11D4_A221_006097239934__INCLUDED_)
