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
#include "IProject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace InterfaceImplementationWrappers
{

//-------------------------------------------------------------------
// class IProject
//-------------------------------------------------------------------

STDMETHODIMP IProject::get_Path(BSTR *path)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString	strPath = GetPath_();
	*path = strPath.AllocSysString();

	return NOERROR;
}


STDMETHODIMP IProject::get_Title(BSTR *title)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString	strTitle = GetTitle_();
	*title = strTitle.AllocSysString();

	return NOERROR;
}


STDMETHODIMP IProject::get_MainFile(BSTR *mainFile)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString	strMainFile = GetMainFile_();
	*mainFile = strMainFile.AllocSysString();

	return NOERROR;
}


STDMETHODIMP IProject::get_UsesBibTex(BOOL *usesBibTex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*usesBibTex = GetUsesBibTex_();

	return NOERROR;
}


STDMETHODIMP IProject::put_UsesBibTex(BOOL usesBibTex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	SetUsesBibTex_(usesBibTex);

	return NOERROR;
}


STDMETHODIMP IProject::get_UsesMakeIndex(BOOL *usesMakeIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*usesMakeIndex = GetUsesMakeIndex_();

	return NOERROR;
}


STDMETHODIMP IProject::put_UsesMakeIndex(BOOL usesMakeIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	SetUsesMakeIndex_(usesMakeIndex);

	return NOERROR;
}

} // namespace