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
#include "TeXnicCenter.h"
#include "Configuration.h"
#include "LatexDoc.h"
#include "global.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// one and only global configuration object
CConfiguration	g_configuration;

//-------------------------------------------------------------------
// class CConfiguration
//-------------------------------------------------------------------

CConfiguration::CConfiguration()
:	m_nStandardFileFormat( CRLF_STYLE_DOS )
{}


void CConfiguration::Serialize( SERDIRECTION direction )
{
	CString	strSection;

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// options
	strSection = "Settings\\Options";
	SerializeProfileStringArray( strSection, "ProjectTemplatePaths", &m_astrProjectTemplatePaths, direction );
	SerializeProfileStringArray( strSection, "DocumentTemplatePaths", &m_astrDocumentTemplatePaths, direction );
	SerializeProfileInt( strSection, "SaveBeforeCompilation", (int*)&m_bSaveBeforeCompilation, direction, TRUE );
	SerializeProfileInt( strSection, "SaveNewDocuments", (int*)&m_bSaveNewDocuments, direction, FALSE);

	SerializeProfileInt( strSection, "WndMenuMaxEntries", &m_nWndMenuMaxEntries, direction, 10 );

	SerializeProfileInt( strSection, "ParseInterval", &m_nParseInterval, direction, 1000 );

	// file
	SerializeProfileInt( strSection, "SaveAutomatic", (int*)&m_bSaveAutomatic, direction, TRUE );
	SerializeProfileInt( strSection, "SaveInterval", (int*)&m_unSaveInterval, direction, 10 );
	SerializeProfileInt( strSection, "StandardFileFormat", &m_nStandardFileFormat, direction, CRLF_STYLE_DOS );
	SerializeProfileString( strSection, "DefaultPath", &m_strDefaultPath, direction );
	SerializeProfileInt( strSection, "OpenDocWndMaximized", (int*)&m_bOpenDocWndMaximized, direction, FALSE );
	SerializeProfileString( strSection, "LastOpenedFolder", &m_strLastOpenedFolder, direction );
	SerializeProfileInt(strSection, "LastTabProjectTemplateDlg", (int*)&m_nLastTabProjectTemplateDlg, direction, 0);
	SerializeProfileInt(strSection, "LastTabDocumentTemplateDlg", (int*)&m_nLastTabDocumentTemplateDlg, direction, 0);

	// quotation mark replacement
	SerializeProfileInt( strSection, "ReplaceQuotationMarks", (int*)&m_bReplaceQuotationMarks, direction, TRUE );
	SerializeProfileString( strSection, "OpeningQuotationMark", &m_strOpeningQuotationMark, direction, _T("\"`") );
	SerializeProfileString( strSection, "ClosingQuotationMark", &m_strClosingQuotationMark, direction, _T("\"'") );

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// session
	strSection = "Session";
	SerializeProfileInt( strSection, "LoadLastProject", (int*)&m_bLoadLastProject, direction, TRUE );
	SerializeProfileString( strSection, "LastProject", &m_strLastProject, direction );

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Editor settings
	strSection = "Settings\\Editor";

	if(direction == Load)
	{
		CFont	font;
		LOGFONT	logFont;
		font.CreatePointFont(100, "Courier New");
		font.GetLogFont(&logFont);
		SerializeProfileData(strSection, "EditorFont", &m_fontEditor, direction, sizeof(m_fontEditor), &logFont);
		SerializeProfileData(_T("Settings"), "OutputFont", &m_fontOutput, direction, sizeof(m_fontOutput), &logFont);

		font.DeleteObject();
		font.CreatePointFont(80, "MS Sans Serif");
		font.GetLogFont(&logFont);
		SerializeProfileData(_T("Settings"), "NavigatorFont", &m_fontNavigator, direction, sizeof(m_fontNavigator), &logFont);
		font.DeleteObject();
	}
	else
	{
		SerializeProfileData(strSection, "EditorFont", &m_fontEditor, direction, sizeof(m_fontEditor));
		SerializeProfileData(_T("Settings"), "OutputFont", &m_fontOutput, direction, sizeof(m_fontOutput));
		SerializeProfileData(_T("Settings"), "NavigatorFont", &m_fontNavigator, direction, sizeof(m_fontNavigator));
	}

	// cursor settings
	int	nInsertCaretForm = CCrystalTextView::GetCaretInsertForm();
	int nInsertCaretMode = CCrystalTextView::GetCaretInsertMode();
	int	nOverwriteCaretForm = CCrystalTextView::GetCaretOverwriteForm();
	int	nOverwriteCaretMode = CCrystalTextView::GetCaretOverwriteMode();
	SerializeProfileInt(strSection, "InsertCaretForm", &nInsertCaretForm, direction, nInsertCaretForm);
	SerializeProfileInt(strSection, "InsertCaretMode", &nInsertCaretMode, direction, nInsertCaretMode);
	SerializeProfileInt(strSection, "OverwriteCaretForm", &nOverwriteCaretForm, direction, nOverwriteCaretForm);
	SerializeProfileInt(strSection, "OverwriteCaretMode", &nOverwriteCaretMode, direction, nOverwriteCaretMode);
	CCrystalTextView::SetCaretInsertStyle(nInsertCaretForm, nInsertCaretMode);
	CCrystalTextView::SetCaretOverwriteStyle(nOverwriteCaretForm, nOverwriteCaretMode);

	SerializeProfileInt( strSection, "EditorTabWidth", &m_nTabWidth, direction, 2 );
	for( int i = 0; i < CCrystalTextView::COLORINDEX_ERRORBKGND; i++ )
	{
		CString	strFormat;
		strFormat.Format( _T("EditorColor%d"), i );
		SerializeProfileInt( strSection, strFormat, (int*)&m_aEditorColors[i], direction, (int)0xFFFFFFFF );
	}

	SerializeProfileInt( strSection, "ViewWhitespaces", (int*)&m_bViewWhitespaces, direction, FALSE );

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Find in files settings
	strSection = "Settings\\FindInFiles";

	SerializeProfileString( strSection, "FileTypes", &m_strFileFindFileTypes, direction );
	SerializeProfileInt( strSection, "FindWholeWords", (int*)&m_bFileFindWholeWords, direction, FALSE );
	SerializeProfileInt( strSection, "CaseSensitive", (int*)&m_bFileFindCaseSensitive, direction, FALSE );
	SerializeProfileInt( strSection, "FindRegularExpression", (int*)&m_bFileFindRegularExpression, direction, FALSE );
	SerializeProfileInt( strSection, "IncludeSubFolders", (int*)&m_bFileFindSubFolders, direction, TRUE );
	SerializeProfileInt( strSection, "OutputBuffer", (int*)&m_nFileFindOutput, direction, 0 );

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Skin initialization parameters
	if (direction == Load)
	{
		SerializeProfileString(_T("Settings"), _T("SkinDirectory"), &m_strSkinDirectory, direction, CPathTool::Cat(CPathTool::GetDirectory(theApp.m_pszHelpFilePath), _T("Skins")));
		SerializeProfileString(_T("Settings"), _T("SkinURL"), &m_strSkinUrl, direction, _T("http://www.bcgsoft.com/Skins"));
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Language
	strSection = _T("Settings\\Language");

	SerializeProfileString( strSection, "Language", &m_strLanguageDefault, direction, AfxLoadString(IDS_LANGUAGE) );
	SerializeProfileString( strSection, "Dialect", &m_strLanguageDialectDefault, direction, AfxLoadString(IDS_DIALECT) );
	SerializeProfileString( strSection, "Locale", &m_strLocale, direction, _T("") );

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Language-Spelling
	strSection = _T("Settings\\Language-Spelling");

	SerializeProfileString( strSection, "PersonalDictionary", &m_strSpellPersonalDictionary, direction, _T("") );
	SerializeProfileString( strSection, "DictionaryPath", &m_strSpellDictionaryPath, direction, _T(""));
	if (!CPathTool::Exists(m_strSpellDictionaryPath))
		m_strSpellDictionaryPath = CPathTool::Cat(CPathTool::GetDirectory(theApp.m_pszHelpFilePath), _T("Language"));
	SerializeProfileInt( strSection, "SkipComments", (int*)&m_bSpellSkipComments, direction, 1 );
	SerializeProfileInt( strSection, "SkipNumbers", (int*)&m_bSpellSkipNumbers, direction, 1 );
	SerializeProfileInt( strSection, "SkipTags", (int*)&m_bSpellSkipTags, direction, 1 );
	SerializeProfileInt( strSection, "SkipUppercase", (int*)&m_bSpellSkipCaps, direction, 1 );
	SerializeProfileInt( strSection, "MainDictionaryOnly", (int*)&m_bSpellMainDictOnly, direction, 1 );
	SerializeProfileInt( strSection, "Enable", (int*)&m_bSpellEnable, direction, 0 );

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// File Cleaning
	strSection = "Settings\\FileClean";

	SerializeProfileInt( strSection, "Confirm", (int*)&m_bFileCleanConfirm, direction, 1 );
	//Serialize the ItemArray
	if (direction == Load)
	{
		m_aFileCleanItems.SerializeFromRegistry(CPathTool::Cat(theApp.m_strRegistryRoot, strSection));
	}
	else
	{
		m_aFileCleanItems.SerializeToRegistry(CPathTool::Cat(theApp.m_strRegistryRoot, strSection));
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Accessibility
	strSection = "Settings\\Accessibility";
	
	SerializeProfileInt(strSection, _T("OptimizeMenuForVisuallyHandicappedUsers"), (int*)&m_bOptimizeMenuForVisuallyHandicappedUsersOnNextStart, direction, FALSE);
	if (direction==Load)
		m_bOptimizeMenuForVisuallyHandicappedUsers = m_bOptimizeMenuForVisuallyHandicappedUsersOnNextStart;
}


void CConfiguration::SerializeProfileInt( LPCTSTR szSection, LPCTSTR szEntry, 
										 int* pnValue, SERDIRECTION direction, int nDefault )
{
	if( direction == Save )
		AfxGetApp()->WriteProfileInt( szSection, szEntry, *pnValue );
	else
		*pnValue = (int)AfxGetApp()->GetProfileInt( szSection, szEntry, nDefault );
}


void CConfiguration::SerializeProfileString( LPCTSTR szSection, LPCTSTR szEntry, 
									CString *pstrString, SERDIRECTION direction, LPCTSTR szDefault )
{
	if( direction == Save )
		AfxGetApp()->WriteProfileString( szSection, szEntry, *pstrString );
	else
		*pstrString = AfxGetApp()->GetProfileString( szSection, szEntry, szDefault );
}


void CConfiguration::SerializeProfileData( LPCTSTR szSection, LPCTSTR szEntry,
									LPVOID lpData, SERDIRECTION direction, UINT unDataLen, LPVOID lpDefaultData /*= NULL*/ )
{
	CString	strSection = theApp.m_strRegistryRoot + _T("\\") + szSection;

	if( direction == Save )
	{
		HKEY	hKey;
		DWORD	dwDisposition;
		if( 
			RegCreateKeyEx( HKEY_CURRENT_USER, strSection, 0, "Binary", 0, KEY_WRITE,
				NULL, &hKey, &dwDisposition ) != ERROR_SUCCESS )
			return;

		RegSetValueEx( hKey, szEntry, 0, REG_BINARY, (BYTE*)lpData, unDataLen );
		RegCloseKey( hKey );
	}
	else
	{
		HKEY	hKey;
		if( 
			RegOpenKeyEx( HKEY_CURRENT_USER, strSection, 0, KEY_READ, &hKey ) != ERROR_SUCCESS )
		{
			memcpy( lpData, lpDefaultData, unDataLen );
			return;
		}

		DWORD	dwType;

		if (RegQueryValueEx( hKey, szEntry, NULL, &dwType, (BYTE*)lpData, (LPDWORD)&unDataLen ) != ERROR_SUCCESS) {
			memcpy( lpData, lpDefaultData, unDataLen );
			return;
		}
	}
}


BOOL CConfiguration::SerializeProfileWndPlacement( LPCTSTR szSection, 
						WINDOWPLACEMENT* lpWndPlc, SERDIRECTION direction )
{
	if( direction == Save )
	{
		ASSERT( lpWndPlc->length );

		BOOL	bIconified = lpWndPlc->showCmd == SW_SHOWMINIMIZED;
		BOOL	bMaximized = lpWndPlc->showCmd == SW_SHOWMAXIMIZED;

		SerializeProfileData( szSection, "Place", lpWndPlc, Save, sizeof( WINDOWPLACEMENT ) );
		SerializeProfileInt( szSection, "Iconified", (int*)&bIconified, Save );
		SerializeProfileInt( szSection, "Maximized", (int*)&bMaximized, Save );

		return TRUE;
	}
	else
	{
		BOOL	bIconified;
		BOOL	bMaximized;

		lpWndPlc->length = 0;	// mark invalid
		SerializeProfileData( szSection, "Place", lpWndPlc, Load, sizeof( WINDOWPLACEMENT ), lpWndPlc );
		if( !lpWndPlc->length )
			return FALSE;

		SerializeProfileInt( szSection, "Iconified", (int*)&bIconified, Load );
		SerializeProfileInt( szSection, "Maximized", (int*)&bMaximized, Load );

		if( bIconified )
		{
			lpWndPlc->showCmd = SW_SHOWMINNOACTIVE;
			if( bMaximized )
				lpWndPlc->flags = WPF_RESTORETOMAXIMIZED;
			else
				lpWndPlc->flags = WPF_SETMINPOSITION;
		}
		else if( bMaximized )
		{
			lpWndPlc->showCmd = SW_SHOWMAXIMIZED;
			lpWndPlc->flags = WPF_RESTORETOMAXIMIZED;
		}
		else
		{
			lpWndPlc->showCmd = SW_NORMAL;
			lpWndPlc->flags = WPF_SETMINPOSITION;
		}

		return TRUE;
	}
}

BOOL CConfiguration::SerializeProfileStringArray( LPCTSTR szSection, LPCTSTR szEntry,
												CStringArray *pastrArray, SERDIRECTION direction )
{
	ASSERT( pastrArray );

	if( direction == Save )
	{
		// store array
		int				i;
		int				nSize = pastrArray->GetSize();
		CString		strSection = CString( szSection ) + _T("\\") + szEntry;
		LPCTSTR		szFormat = "String%d";
		CString		strKey;
		CString		strElement;

		// store number of elements
		SerializeProfileInt( strSection, _T("Size"), &nSize, Save );

		// store each string
		for( i = 0; i < nSize; i++ )
		{
			strKey.Format( szFormat, i );
			strElement = pastrArray->GetAt( i );
			SerializeProfileString( strSection, strKey, &strElement, Save );
		}
	}
	else
	{
		// load array
		int				i;
		int				nSize;
		CString		strSection = CString( szSection ) + _T("\\") + szEntry;
		LPCTSTR		szFormat = "String%d";
		CString		strKey;
		CString		strElement;

		// load number of elements
		SerializeProfileInt( strSection, _T("Size"), &nSize, Load );
		if( !nSize )
			return FALSE;

		// load each string
		for( i = 0; i < nSize; i++ )
		{
			strKey.Format( szFormat, i );
			SerializeProfileString( strSection, strKey, &strElement, Load );
			pastrArray->Add( strElement );
		}
	}

	return TRUE;
}


BOOL CConfiguration::RegDeleteSection( LPCTSTR lpszSection, BOOL bAdmin /*= FALSE*/ )
{
	ASSERT(lpszSection);

	CString	strSection = theApp.m_strRegistryRoot + _T("\\") + lpszSection;

	LONG lReturn = ::RegDeleteKey( bAdmin ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER,
									strSection );

	if(lReturn == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}
