#pragma once

namespace RunTimeHelper
{

inline bool IsCommCtrl6()
{
	DWORD dwMajor = 0, dwMinor = 0;
	HRESULT hRet = ATL::AtlGetCommCtrlVersion(&dwMajor, &dwMinor);
	return (SUCCEEDED(hRet) && (dwMajor >= 6));
}

inline bool IsVista()
{
	OSVERSIONINFO ovi = {sizeof(OSVERSIONINFO)};
	BOOL bRet = ::GetVersionEx(&ovi);
	return ((bRet != FALSE) && (ovi.dwMajorVersion >= 6));
}
}