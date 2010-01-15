/********************************************************************
 *
 * This file is part of the TeXnicCenter-system
 *
 * Copyright (C) 1999-2000 Sven Wiegand
 * Copyright (C) 2000-$CurrentYear$ ToolsCenter
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * If you have further questions or if you want to support
 * further TeXnicCenter development, visit the TeXnicCenter-homepage
 *
 *    http://www.ToolsCenter.org
 *
 *********************************************************************/

#include "stdafx.h"
#include "OutputInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//-------------------------------------------------------------------
// class COutputInfo
//-------------------------------------------------------------------

COutputInfo::COutputInfo(const CString& strSrcFile, const Nullable<int>& nSrcLine,
						 const Nullable<int>& nOutputLine,
                         const CString& strError /*= _T("")*/, int nErrorID /*= - 1*/)
{
	m_strSrcFile = strSrcFile;
	m_nSrcLine = nSrcLine;
	m_nOutputLine = nOutputLine;
	m_strError = strError;
	m_nErrorID = nErrorID;
}

COutputInfo::COutputInfo()
		: m_nErrorID(-1)
{
}

void COutputInfo::Clear()
{
	m_strSrcFile.Empty();
	m_nSrcLine = m_nOutputLine = Nullable<int>();
	m_strError.Empty();
	m_nErrorID = -1;
}

const Nullable<int>& COutputInfo::GetSourceLine() const
{
	return m_nSrcLine;
}

void COutputInfo::SetErrorMessage( const CString& val )
{
	m_strError = val;
}

const Nullable<int>& COutputInfo::GetOutputLine() const
{
	return m_nOutputLine;
}

void COutputInfo::SetOutputLine( int val )
{
	m_nOutputLine = val;
}

const CString& COutputInfo::GetErrorMessage() const
{
	return m_strError;
}

void COutputInfo::SetSourceLine( const Nullable<int>& val )
{
	m_nSrcLine = val;
}

void COutputInfo::SetSourceFile( const CString& val )
{
	m_strSrcFile = val;
}

const CString& COutputInfo::GetSourceFile() const
{
	return !m_nSrcLine.HasValue() && !package_.IsEmpty() ? package_ : m_strSrcFile;
}

const CString& COutputInfo::GetPackageName() const
{
	return package_;
}

void COutputInfo::SetPackage( const CString& val )
{
	package_ = val;
}