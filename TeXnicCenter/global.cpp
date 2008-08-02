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

CString AfxLoadString(UINT nID)
{
    CString szText;
    szText.LoadString(nID);

    return szText;
}

CString AfxFormatString1(UINT nID,LPCTSTR lpszText)
{
    CString szText;
    szText.Format(nID,lpszText);

    return szText;
}

CString AfxFormatSystemString(DWORD dwMessageId)
{
    LPTSTR lpBuffer;
    ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            dwMessageId,
            0,
            (LPTSTR) & lpBuffer,
            0,
            NULL);
    CString szText(lpBuffer);
    LocalFree(lpBuffer);
    return szText;
}

CString AfxGetDefaultDirectory(bool bForceNonEmpty /*= true*/,bool bNewProject /*= false*/)
{
    ////////////////////////
    //Get default directory
    CString strPersonalDir;

    // - New Project?
    if (!bNewProject) {
        // - Get LastOpenedFolder
        strPersonalDir = CConfiguration::GetInstance()->m_strLastOpenedFolder;
        if (strPersonalDir.IsEmpty() || !CPathTool::Exists(strPersonalDir)) {
            // - Project opened? ==> Working Dir is default
            CLaTeXProject* pLProject = theApp.GetProject();
            if (pLProject) strPersonalDir = pLProject->GetWorkingDir();
        }
    }

    // - No Project? ==> Try it with the default dir from the config
    if (strPersonalDir.IsEmpty()) {
        strPersonalDir = CConfiguration::GetInstance()->m_strDefaultPath;
    }

    // - Still empty? ==> Get the system default for "My documents"
    if (strPersonalDir.IsEmpty()) {
        LPITEMIDLIST lpidl;
        if (SHGetSpecialFolderLocation(AfxGetMainWnd()->m_hWnd,CSIDL_PERSONAL,&lpidl) == NOERROR) {
            SHGetPathFromIDList(lpidl,strPersonalDir.GetBuffer(MAX_PATH));
            strPersonalDir.ReleaseBuffer();

            // free memory
            LPMALLOC lpMalloc;
            SHGetMalloc(&lpMalloc);
            if (lpMalloc)
                lpMalloc->Free(lpidl);
        }
    }


    // - Still empty? ==> Hell, this is hard. Lets try this.
    if (bForceNonEmpty && strPersonalDir.IsEmpty()) {
        strPersonalDir = theApp.GetWorkingDir();
    }

    // - Still empty? ==> Hell, this is hard. Lets try this.
    if (bForceNonEmpty && strPersonalDir.IsEmpty()) {
        strPersonalDir = _T("C:\\");
    }

    return strPersonalDir;
}

void AfxSetLastDirectory( const CString& strLastFolder )
{
    // - Set LastOpenedFolder if not empty
    if (!strLastFolder.IsEmpty()) {
        CConfiguration::GetInstance()->m_strLastOpenedFolder = strLastFolder;
    }
}

// Raffi: get dictionaries
//----------------------------------------------------------------

void AfxFindDictionaries(CArray<CString,CString&> &aLanguage,
                          CArray<CString,CString&> &aDialect)
{
    CString dicFileMatch = CConfiguration::GetInstance()->m_strSpellDictionaryPath + _T("\\*.dic");

    WIN32_FIND_DATA dirInfo;
    HANDLE hFile;
    BOOL bNext = TRUE;

    hFile = FindFirstFile(dicFileMatch,&dirInfo);

    while (hFile != INVALID_HANDLE_VALUE && bNext) {
        // Dictionary file format LANG_DIALACT{-extra}?.dic
        // Example: de_DE.dic en_US-slang.dic 
        // Get the language and dialect of all installed dictionaries.
        TCHAR* dash;
        TCHAR* dot;
        dash = _tcschr(dirInfo.cFileName,_T('_'));

        if (dash != NULL)
            dot = _tcschr(dash,_T('.'));

        if ((dash != NULL) && (dot != NULL)) {
            CString lang(dirInfo.cFileName,dash - dirInfo.cFileName);
            CString dialect(dash + 1,dot - dash - 1);
            aDialect.Add(dialect);
            aLanguage.Add(lang);
        }

        bNext = FindNextFile(hFile,&dirInfo);
    }

    FindClose(hFile);
}
