#ifndef SYSTEMINFO_H
#define SYSTEMINFO_H

/********************************************************************
*
* This file is part of the TeXnicCenter-system
* 
* Copyright (C) Marius Bancila (thanks for providing this class)
*
*********************************************************************/

/********************************************************************
*
* $Id$
*
********************************************************************/


#include <Windows.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


typedef enum WindowsVersion
{
	Win_Unknown = 0,
	Win95,
	Win95_OSR2,
	Win98,
	Win98_SE,
	WinMe,
	WinNT_Unknown,
	WinNT_351,
	WinNT_40_Workstation,
	WinNT_40_Server,
	WinNT_40_Server_Enterprise,
	Win2000_Unknown,
	Win2000_Professional,
	Win2000_Server,
	Win2000_Server_Datacenter,
	Win2000_Server_Advanced,
	WinXP_Unknown,
	WinXP_Home,
	WinXP_Professional,
	WinServer2003,
	WinServer2003_Enterprise,
	WinServer2003_Datacenter,
	WinServer2003_WebEdition,
	Win32s
};

class AFX_EXT_CLASS CSystemInfo  
{
	WindowsVersion	m_nWinVersion;
	TCHAR			m_szServicePack[128];
	OSVERSIONINFOEX m_osvi;
	BOOL			m_bOsVersionInfoEx;

private:
	int DetectWindowsVersion();	 

public:
	
	CSystemInfo();
	virtual ~CSystemInfo();

	WindowsVersion GetWindowsVersion() const;		// returns the windows version
	BOOL IsNTPlatform() const;						// TRUE if NT platform
	BOOL IsWindowsPlatform() const;					// TRUE is Windows platform
	BOOL IsWin32sPlatform() const;					// TRUE is Win32s platform
	DWORD GetMajorVersion() const;					// returns major version
	DWORD GetMinorVersion() const;					// returns minor version
	DWORD GetBuildNumber() const;					// returns build number
	DWORD GetPlatformID() const;					// returns platform ID
	void GetServicePackInfo(TCHAR* szServicePack) const;// additional information about service pack
	const CString ToString();
};

#endif