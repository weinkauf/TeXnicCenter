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
#include "global.h"
#include "TeXnicCenter.h"
#include "configuration.h"

CString AfxLoadString( UINT nID )
{
	CString		szText;

	if( szText.LoadString( nID ) )
		return szText;
	else
		return CString( "" );
}


CString AfxFormatString1( UINT nID, LPCTSTR lpszText )
{
	CString	szText;

	szText.Format( nID, lpszText );
	return szText;
}

CString AfxFormatSystemString( DWORD dwMessageId )
{
	LPTSTR lpBuffer;
	::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		dwMessageId,
		0,
		(LPTSTR)&lpBuffer,
		0,
		NULL);
	CString szText(lpBuffer);
	LocalFree(lpBuffer);
	return szText;
}

CString AfxGetDefaultDirectory(bool bForceNonEmpty /*= true*/, bool bNewProject /*= false*/)
{
	////////////////////////
	//Get default directory
	CString strPersonalDir("");

	// - New Project?
	if (!bNewProject)
	{
		// - Get LastOpenedFolder
		strPersonalDir = g_configuration.m_strLastOpenedFolder;
		if (strPersonalDir.IsEmpty() || !CPathTool::Exists(strPersonalDir))
		{
			// - Project opened? ==> Working Dir is default
			CLatexProject* pLProject = theApp.GetProject();
			if (pLProject) strPersonalDir = pLProject->GetWorkingDir();
		}
	}

	// - No Project? ==> Try it with the default dir from the config
	if (strPersonalDir.IsEmpty())
	{
		strPersonalDir = g_configuration.m_strDefaultPath;
	}

	// - Still empty? ==> Get the system default for "My documents"
	if (strPersonalDir.IsEmpty())
	{
		LPITEMIDLIST lpidl;
		if (SHGetSpecialFolderLocation(AfxGetMainWnd()->m_hWnd, CSIDL_PERSONAL, &lpidl) == NOERROR)
		{
			SHGetPathFromIDList(lpidl, strPersonalDir.GetBuffer(MAX_PATH));
			strPersonalDir.ReleaseBuffer();

			// free memory
			LPMALLOC lpMalloc;
			SHGetMalloc(&lpMalloc);
			if(lpMalloc)
				lpMalloc->Free(lpidl);
		}
	}


	// - Still empty? ==> Hell, this is hard. Lets try this.
	if (bForceNonEmpty && strPersonalDir.IsEmpty())
	{
		strPersonalDir = theApp.GetWorkingDir();
	}

	// - Still empty? ==> Hell, this is hard. Lets try this.
	if (bForceNonEmpty && strPersonalDir.IsEmpty())
	{
		strPersonalDir = "C:\\";
	}

	return strPersonalDir;
}


void AfxSetLastDirectory(CString bLastFolder)
{
	// - Set LastOpenedFolder if not empty
	if (!bLastFolder.IsEmpty())
	{
		g_configuration.m_strLastOpenedFolder = bLastFolder;
	}
}
