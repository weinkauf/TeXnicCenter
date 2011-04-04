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

#include "stdafx.h"
#include "SystemInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define BUFSIZE 80

const TCHAR* VERBOSE_OS[] = {
	_T("Unknown"),
	_T("Windows 95"),
	_T("Windows 95 OSR2"),
	_T("Windows 98"),
	_T("Windows 98 SE"),
	_T("Windows ME"),
	_T("Windows NT (Unknown)"),
	_T("Windows NT 3.51"),
	_T("Windows NT 4.0 Workstation"),
	_T("Windows NT 4.0 Server"),
	_T("Windows NT 4.0 Server Enterprise"),
	_T("Windows 2000 (Unknown)"),
	_T("Windows 2000 Professional"),
	_T("Windows 2000 Server"),
	_T("Windows 2000 Server Datacenter"),
	_T("Windows 2000 Server Advanced"),
	_T("Windows XP (Unknown)"),
	_T("Windows XP Home"),
	_T("Windows XP Professional"),
	_T("Windows Server 2003"),
	_T("Windows Server 2003 Enterprise"),
	_T("Windows Server 2003 Datacenter"),
	_T("Windows Server 2003 WebEdition"),
	_T("Win32s"),
	_T("Windows Vista"),
	_T("Windows Server 2008"),
	_T("Windows Server 2008 R2"),
	_T("Windows 7")
};	

CSystemInfo::CSystemInfo()
{
	DetectWindowsVersion();
}

CSystemInfo::~CSystemInfo()
{

}

int CSystemInfo::DetectWindowsVersion()
{
	m_bOsVersionInfoEx = FALSE;
	m_nWinVersion = Win_Unknown;
	memset(m_szServicePack, 0, sizeof(m_szServicePack));
	
	// Try calling GetVersionEx using the OSVERSIONINFOEX structure.
	ZeroMemory(&m_osvi, sizeof(OSVERSIONINFOEX));
	m_osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	
	if( !(m_bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &m_osvi)) )
	{
		// If that fails, try using the OSVERSIONINFO structure.
		m_osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (! GetVersionEx ( (OSVERSIONINFO *) &m_osvi) ) 
			return 0;
	}
	
	switch (m_osvi.dwPlatformId)
	{

	// Test for the Windows NT product family.
	case VER_PLATFORM_WIN32_NT:
		
		// Test for the specific product family
		if (m_osvi.dwMajorVersion == 6 && m_osvi.dwMinorVersion == 1)
			m_nWinVersion = m_osvi.wProductType != VER_NT_WORKSTATION ? WindowsServer2008R2 : Windows7;
		else if (m_osvi.dwMajorVersion == 6 && m_osvi.dwMinorVersion == 0)
			m_nWinVersion = m_osvi.wProductType != VER_NT_WORKSTATION ? WindowsServer2008 : WindowsVista;
		else if ( m_osvi.dwMajorVersion == 5 && m_osvi.dwMinorVersion == 2 )
            m_nWinVersion = WinServer2003;
		else if ( m_osvi.dwMajorVersion == 5 && m_osvi.dwMinorVersion == 1 )
			m_nWinVersion = WinXP_Unknown;
		else if ( m_osvi.dwMajorVersion == 5 && m_osvi.dwMinorVersion == 0 )
			m_nWinVersion = Win2000_Unknown;
		else if ( m_osvi.dwMajorVersion <= 4 )
            m_nWinVersion = WinNT_Unknown;
		
		// Test for specific product on Windows NT 4.0 SP6 and later.

		if( m_bOsVersionInfoEx )
		{
			/*
            // Test for the workstation type.
            if ( m_osvi.wProductType == VER_NT_WORKSTATION )
            {
				if( m_osvi.dwMajorVersion == 4 )
					m_nWinVersion = WinNT_40_Workstation;
				else if( m_osvi.wSuiteMask & VER_SUITE_PERSONAL )
				{
					if(m_osvi.dwMinorVersion == 1)
						m_nWinVersion = WinXP_Home;
				}
				else
				{
					if(m_osvi.dwMinorVersion == 1)
						m_nWinVersion = WinXP_Professional;
					else if(m_osvi.dwMinorVersion == 0)
						m_nWinVersion = Win2000_Professional;
				}
            }
            
            // Test for the server type.
            else if ( m_osvi.wProductType == VER_NT_SERVER )
            {
				// Windows Server 2003
				if( m_osvi.dwMajorVersion == 5 && m_osvi.dwMinorVersion == 2 )
				{
					if( m_osvi.wSuiteMask & VER_SUITE_DATACENTER )
						m_nWinVersion = WinServer2003_Datacenter;
					else if( m_osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						m_nWinVersion = WinServer2003_Enterprise;
					else if ( m_osvi.wSuiteMask == VER_SUITE_BLADE )
						m_nWinVersion = WinServer2003_WebEdition;
					else
						m_nWinVersion = WinServer2003;
				}
				
				// Windows 2000
				else if( m_osvi.dwMajorVersion == 5 && m_osvi.dwMinorVersion == 0 )
				{
					if( m_osvi.wSuiteMask & VER_SUITE_DATACENTER )
						m_nWinVersion = Win2000_Server_Datacenter;
					else if( m_osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						m_nWinVersion = Win2000_Server_Advanced;
					else
						m_nWinVersion = Win2000_Server;
				}
				
				// Windows NT 4.0 
				else 
				{
					if( m_osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						m_nWinVersion = WinNT_40_Server_Enterprise;					
					else
						m_nWinVersion = WinNT_40_Server;
				}
            }*/
		}
		else  // Test for specific product on Windows NT 4.0 SP5 and earlier
		{
            HKEY hKey;
            TCHAR szProductType[BUFSIZE];
            DWORD dwBufLen=BUFSIZE;
            LONG lRet;
			
            lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
				_T("SYSTEM\\CurrentControlSet\\Control\\ProductOptions"),
				0, KEY_QUERY_VALUE, &hKey );
            if( lRet != ERROR_SUCCESS )
				return FALSE;
			
            lRet = RegQueryValueEx( hKey, _T("ProductType"), NULL, NULL,
				(LPBYTE) szProductType, &dwBufLen);
            if( (lRet != ERROR_SUCCESS) || (dwBufLen > BUFSIZE) )
				return FALSE;
			
            RegCloseKey( hKey );
			
            if ( lstrcmpi( _T("WINNT"), szProductType) == 0 )
			{
				if ( m_osvi.dwMajorVersion <= 4 )
					m_nWinVersion = WinNT_40_Workstation;
			}
            if ( lstrcmpi( _T("LANMANNT"), szProductType) == 0 )
			{
				if ( m_osvi.dwMajorVersion == 5 && m_osvi.dwMinorVersion == 2 )
					m_nWinVersion = WinServer2003;
				
				if ( m_osvi.dwMajorVersion == 5 && m_osvi.dwMinorVersion == 0 )
					m_nWinVersion = Win2000_Server;
				
				if ( m_osvi.dwMajorVersion <= 4 )
					m_nWinVersion = WinNT_40_Server;
			}
            if ( lstrcmpi( _T("SERVERNT"), szProductType) == 0 )
			{
				if ( m_osvi.dwMajorVersion == 5 && m_osvi.dwMinorVersion == 2 )
					m_nWinVersion = WinServer2003_Enterprise;
				
				if ( m_osvi.dwMajorVersion == 5 && m_osvi.dwMinorVersion == 0 )
					m_nWinVersion = Win2000_Server_Advanced;
				
				if ( m_osvi.dwMajorVersion <= 4 )
					m_nWinVersion = WinNT_40_Server_Enterprise;
			}
		}
		
		// Display service pack (if any) and build number.
		
		if( m_osvi.dwMajorVersion == 4 && 
			lstrcmpi( m_osvi.szCSDVersion, _T("Service Pack 6") ) == 0 )
		{
            HKEY hKey;
            LONG lRet;
			
            // Test for SP6 versus SP6a.
            lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
				_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Hotfix\\Q246009"),
				0, KEY_QUERY_VALUE, &hKey );
            if( lRet == ERROR_SUCCESS )
				_stprintf(m_szServicePack,_T("Service Pack 6a (Build %d)\n"), m_osvi.dwBuildNumber & 0xFFFF );         
            else // Windows NT 4.0 prior to SP6a
            {
				_stprintf(m_szServicePack,_T("%s (Build %d)\n"),
					m_osvi.szCSDVersion,
					m_osvi.dwBuildNumber & 0xFFFF);
            }
			
            RegCloseKey( hKey );
		}
		else // Windows NT 3.51 and earlier or Windows 2000 and later
		{
            _stprintf(m_szServicePack,_T("%s (Build %d)\n"),
				m_osvi.szCSDVersion,
				m_osvi.dwBuildNumber & 0xFFFF);
		}
		break;
		
		// Test for the Windows 95 product family.
      case VER_PLATFORM_WIN32_WINDOWS:
		  if (m_osvi.dwMajorVersion == 4 && m_osvi.dwMinorVersion == 0)
		  {
			  m_nWinVersion = Win95;
			  if ( m_osvi.szCSDVersion[1] ==_T('C') || m_osvi.szCSDVersion[1] ==_T('B') )
				  m_nWinVersion = Win95_OSR2;
		  } 
		  
		  if (m_osvi.dwMajorVersion == 4 && m_osvi.dwMinorVersion == 10)
		  {
			  m_nWinVersion = Win98;
			  if ( m_osvi.szCSDVersion[1] ==_T('A') )
				  m_nWinVersion = Win98_SE;
		  } 
		  
		  if (m_osvi.dwMajorVersion == 4 && m_osvi.dwMinorVersion == 90)
		  {
			  m_nWinVersion = WinMe;
		  } 
		  break;
		  
      case VER_PLATFORM_WIN32s:
		  m_nWinVersion = Win32s;
		  break;
   }

   return 1;
}

WindowsVersion CSystemInfo::GetWindowsVersion() const
{
	return m_nWinVersion;
}

BOOL CSystemInfo::IsNTPlatform() const
{
	return m_osvi.dwPlatformId == VER_PLATFORM_WIN32_NT;
}

BOOL CSystemInfo::IsWindowsPlatform() const
{
	return m_osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS;
}

BOOL CSystemInfo::IsWin32sPlatform() const
{
	return m_osvi.dwPlatformId == VER_PLATFORM_WIN32s;
}
	
DWORD CSystemInfo::GetMajorVersion() const
{
	return m_osvi.dwMajorVersion;
}

DWORD CSystemInfo::GetMinorVersion() const
{
	return m_osvi.dwMinorVersion;
}

DWORD CSystemInfo::GetBuildNumber() const
{
	return m_osvi.dwBuildNumber;
}

DWORD CSystemInfo::GetPlatformID() const
{
	return m_osvi.dwPlatformId;	
}

// PARAMETER szServicePack must not be NULL
void CSystemInfo::GetServicePackInfo(TCHAR* szServicePack) const
{
	if(szServicePack == NULL) return;
	
	_tcscpy(szServicePack, m_szServicePack);
}

const CString CSystemInfo::ToString()
{
	CString str;
	str.Format(_T("V%d.%d, %s, %s"), 
		GetMajorVersion(),
		GetMinorVersion(),
		VERBOSE_OS[m_nWinVersion],
		m_szServicePack);
	return str;
}
