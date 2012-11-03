#pragma once

#if (((_ATL_VER < 0x0700) && defined(_ATL_DLL)) || (_ATL_VER >= 0x0B00)) && !defined(_WIN32_WCE)

namespace ATL {

	inline HRESULT AtlGetDllVersion(HINSTANCE hInstDLL, DLLVERSIONINFO* pDllVersionInfo)
	{
		ATLASSERT(pDllVersionInfo != NULL);
		if(pDllVersionInfo == NULL)
			return E_INVALIDARG;

		// We must get this function explicitly because some DLLs don't implement it.
		DLLGETVERSIONPROC pfnDllGetVersion = (DLLGETVERSIONPROC)::GetProcAddress(hInstDLL, "DllGetVersion");
		if(pfnDllGetVersion == NULL)
			return E_NOTIMPL;

		return (*pfnDllGetVersion)(pDllVersionInfo);
	}

	inline HRESULT AtlGetDllVersion(LPCTSTR lpstrDllName, DLLVERSIONINFO* pDllVersionInfo)
	{
		HINSTANCE hInstDLL = ::LoadLibrary(lpstrDllName);
		if(hInstDLL == NULL)
			return E_FAIL;
		HRESULT hRet = AtlGetDllVersion(hInstDLL, pDllVersionInfo);
		::FreeLibrary(hInstDLL);
		return hRet;
	}

	inline HRESULT AtlGetCommCtrlVersion(LPDWORD pdwMajor, LPDWORD pdwMinor)
	{
		ATLASSERT(pdwMajor != NULL && pdwMinor != NULL);
		if(pdwMajor == NULL || pdwMinor == NULL)
			return E_INVALIDARG;

		DLLVERSIONINFO dvi;
		::ZeroMemory(&dvi, sizeof(dvi));
		dvi.cbSize = sizeof(dvi);
		HRESULT hRet = AtlGetDllVersion(_T("comctl32.dll"), &dvi);

		if(SUCCEEDED(hRet))
		{
			*pdwMajor = dvi.dwMajorVersion;
			*pdwMinor = dvi.dwMinorVersion;
		}
		else if(hRet == E_NOTIMPL)
		{
			// If DllGetVersion is not there, then the DLL is a version
			// previous to the one shipped with IE 3.x
			*pdwMajor = 4;
			*pdwMinor = 0;
			hRet = S_OK;
		}

		return hRet;
	}
}

#endif

namespace RunTimeHelper
{

	/// Gets a value indicating whether commctrl version 6 is in use.
	///
	/// \return \c true if commctrl version 6 is used, \c false otherwise.
	inline bool IsCommCtrl6()
	{
		DWORD dwMajor = 0, dwMinor = 0;
		HRESULT hRet = ATL::AtlGetCommCtrlVersion(&dwMajor, &dwMinor);
		return (SUCCEEDED(hRet) && (dwMajor >= 6));
	}

	/// Gets a value indicating whether the running operating system
	/// is Windows Vista or higher.
	/// 
	/// \return \c true if Windows Vista or higher is running, \c false otherwise.
	inline bool IsVista()
	{
		OSVERSIONINFO ovi = {sizeof(OSVERSIONINFO)};
		BOOL bRet = ::GetVersionEx(&ovi);
		return ((bRet != FALSE) && (ovi.dwMajorVersion >= 6));
	}

	/// Gets a value indicating whether the running operating system
	/// is Window XP or higher.
	/// 
	/// \return \c true if Windows XP or higher is running, \c false otherwise.
	inline bool IsWindowsXPOrHigher()
	{
		OSVERSIONINFO ovi = {sizeof(OSVERSIONINFO)};
		BOOL bRet = ::GetVersionEx(&ovi);
		return ((bRet != FALSE) && (ovi.dwMajorVersion == 5 &&
			ovi.dwMinorVersion >= 1 || ovi.dwMajorVersion > 5));
	}
}