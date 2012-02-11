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

/********************************************************************
 *
 * $Id$
 *
 ********************************************************************/

#include "stdafx.h"
#include "stdafx.h"
#include "IDocumentWizard.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//-------------------------------------------------------------------
// class IDocumentWizard
//-------------------------------------------------------------------
namespace InterfaceAccessWrappers
{

IDocumentWizard::IDocumentWizard()
		: CComPtr<Interfaces::IDocumentWizard>()
{
}

IDocumentWizard::IDocumentWizard(Interfaces::IDocumentWizard *pInterface /*= NULL*/)
		: CComPtr<Interfaces::IDocumentWizard>(pInterface)
{
}

CString IDocumentWizard::GetTitle() const
{
	ASSERT(p);

	BSTR bstrTitle;
	if (p->get_Title(&bstrTitle) != NOERROR)
		return _T("");

	CString strTitle((LPCTSTR) _bstr_t(bstrTitle,false));
	return strTitle;
}

CString IDocumentWizard::GetDescription() const
{
	ASSERT(p);

	BSTR bstrDescription;
	if (p->get_Description(&bstrDescription) != NOERROR)
		return _T("");

	CString strDescription((LPCTSTR) _bstr_t(bstrDescription,false));
	return strDescription;
}

int IDocumentWizard::GetIconIndex() const
{
	ASSERT(p);

	int nIconIndex;
	if (p->get_IconIndex(&nIconIndex) != NOERROR)
		return -1;

	return nIconIndex;
}

BOOL IDocumentWizard::DoModal(LPCTSTR lpszPath,LPCTSTR lpszCr)
{
	CString strPath(lpszPath);
	BSTR bstrPath = strPath.AllocSysString();
	CString strCr(lpszCr);
	BSTR bstrCr = strCr.AllocSysString();

	BOOL bSuccess;
	HRESULT result = p->DoModal(bstrPath,bstrCr,&bSuccess);
	SysFreeString(bstrPath);
	SysFreeString(bstrCr);

	return result == NOERROR && bSuccess;
}

} // namespace