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
	SerializeProfileStringArray( strSection, _T("ProjectTemplatePaths"), &m_astrProjectTemplatePaths, direction );
	SerializeProfileStringArray( strSection, _T("DocumentTemplatePaths"), &m_astrDocumentTemplatePaths, direction );
	SerializeProfileInt( strSection, _T("SaveBeforeCompilation"), (int*)&m_bSaveBeforeCompilation, direction, TRUE );
	SerializeProfileInt( strSection, _T("SaveNewDocuments"), (int*)&m_bSaveNewDocuments, direction, FALSE);

	SerializeProfileInt( strSection, _T("WndMenuMaxEntries"), &m_nWndMenuMaxEntries, direction, 10 );

	SerializeProfileInt( strSection, _T("ParseInterval"), &m_nParseInterval, direction, 1000 );

	// file
	SerializeProfileInt( strSection, _T("SaveAutomatic"), (int*)&m_bSaveAutomatic, direction, TRUE );
	SerializeProfileInt( strSection, _T("SaveInterval"), (int*)&m_unSaveInterval, direction, 10 );
	SerializeProfileInt( strSection, _T("StandardFileFormat"), &m_nStandardFileFormat, direction, CRLF_STYLE_DOS );
	SerializeProfileString( strSection, _T("DefaultPath"), &m_strDefaultPath, direction );
	SerializeProfileInt( strSection, _T("OpenDocWndMaximized"), (int*)&m_bOpenDocWndMaximized, direction, FALSE );
	SerializeProfileString( strSection, _T("LastOpenedFolder"), &m_strLastOpenedFolder, direction );
	SerializeProfileInt(strSection, _T("LastTabProjectTemplateDlg"), (int*)&m_nLastTabProjectTemplateDlg, direction, 0);
	SerializeProfileInt(strSection, _T("LastTabDocumentTemplateDlg"), (int*)&m_nLastTabDocumentTemplateDlg, direction, 0);

	// quotation mark replacement
	SerializeProfileInt( strSection, _T("ReplaceQuotationMarks"), (int*)&m_bReplaceQuotationMarks, direction, TRUE );
	SerializeProfileString( strSection, _T("OpeningQuotationMark"), &m_strOpeningQuotationMark, direction, _T("\"`") );
	SerializeProfileString( strSection, _T("ClosingQuotationMark"), &m_strClosingQuotationMark, direction, _T("\"'") );

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// session
	strSection = "Session";
	SerializeProfileInt( strSection, _T("LoadLastProject"), (int*)&m_bLoadLastProject, direction, TRUE );
	SerializeProfileString( strSection, _T("LastProject"), &m_strLastProject, direction );

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Editor settings
	strSection = _T("Settings\\Editor");

	if(direction == Load)
	{
		CFont	font;
		LOGFONT	logFont;
		font.CreatePointFont(100, _T("Courier New"));
		font.GetLogFont(&logFont);
		SerializeProfileData(strSection, _T("EditorFont"), &m_fontEditor, direction, sizeof(m_fontEditor), &logFont);
		SerializeProfileData(_T("Settings"), _T("OutputFont"), &m_fontOutput, direction, sizeof(m_fontOutput), &logFont);

		font.DeleteObject();
		font.CreatePointFont(80, _T("MS Sans Serif"));
		font.GetLogFont(&logFont);
		SerializeProfileData(_T("Settings"), _T("NavigatorFont"), &m_fontNavigator, direction, sizeof(m_fontNavigator), &logFont);
		font.DeleteObject();
	}
	else
	{
		SerializeProfileData(strSection, _T("EditorFont"), &m_fontEditor, direction, sizeof(m_fontEditor));
		SerializeProfileData(_T("Settings"), _T("OutputFont"), &m_fontOutput, direction, sizeof(m_fontOutput));
		SerializeProfileData(_T("Settings"), _T("NavigatorFont"), &m_fontNavigator, direction, sizeof(m_fontNavigator));
	}

	// cursor settings
	int	nInsertCaretForm = CCrystalTextView::GetCaretInsertForm();
	int nInsertCaretMode = CCrystalTextView::GetCaretInsertMode();
	int	nOverwriteCaretForm = CCrystalTextView::GetCaretOverwriteForm();
	int	nOverwriteCaretMode = CCrystalTextView::GetCaretOverwriteMode();
	SerializeProfileInt(strSection, _T("InsertCaretForm"), &nInsertCaretForm, direction, nInsertCaretForm);
	SerializeProfileInt(strSection, _T("InsertCaretMode"), &nInsertCaretMode, direction, nInsertCaretMode);
	SerializeProfileInt(strSection, _T("OverwriteCaretForm"), &nOverwriteCaretForm, direction, nOverwriteCaretForm);
	SerializeProfileInt(strSection, _T("OverwriteCaretMode"), &nOverwriteCaretMode, direction, nOverwriteCaretMode);
	CCrystalTextView::SetCaretInsertStyle(nInsertCaretForm, nInsertCaretMode);
	CCrystalTextView::SetCaretOverwriteStyle(nOverwriteCaretForm, nOverwriteCaretMode);

	SerializeProfileInt( strSection, _T("EditorTabWidth"), &m_nTabWidth, direction, 2 );
	SerializeProfileInt( strSection, _T("EditorShowLineNumbers"), &m_bShowLineNumbers, direction, 0);
	for( int i = 0; i < CCrystalTextView::COLORINDEX_ERRORBKGND; i++ )
	{
		CString	strFormat;
		strFormat.Format( _T("EditorColor%d"), i );
		SerializeProfileInt( strSection, strFormat, (int*)&m_aEditorColors[i], direction, (int)0xFFFFFFFF );
	}

	SerializeProfileInt( strSection, _T("ViewWhitespaces"), (int*)&m_bViewWhitespaces, direction, FALSE );

	SerializeProfileInt( strSection, _T("WordWrapStyle"), (int*)&m_WordWrapStyle, direction, WORD_WRAP_WINDOW );
	SerializeProfileInt( strSection, _T("FixedColumnWrap"), (int*)&m_nFixedColumnWrap, direction, 80 );

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Find in files settings
	strSection = "Settings\\FindInFiles";

	SerializeProfileString( strSection, _T("FileTypes"), &m_strFileFindFileTypes, direction );
	SerializeProfileInt( strSection, _T("FindWholeWords"), (int*)&m_bFileFindWholeWords, direction, FALSE );
	SerializeProfileInt( strSection, _T("CaseSensitive"), (int*)&m_bFileFindCaseSensitive, direction, FALSE );
	SerializeProfileInt( strSection, _T("FindRegularExpression"), (int*)&m_bFileFindRegularExpression, direction, FALSE );
	SerializeProfileInt( strSection, _T("IncludeSubFolders"), (int*)&m_bFileFindSubFolders, direction, TRUE );
	SerializeProfileInt( strSection, _T("OutputBuffer"), (int*)&m_nFileFindOutput, direction, 0 );

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Skin initialization parameters
	if (direction == Load)
	{
		SerializeProfileString(_T("Settings"), _T("SkinDirectory"), &m_strSkinDirectory, direction, CPathTool::Cat(CPathTool::GetDirectory(theApp.m_pszHelpFilePath), _T("Skins")));
		SerializeProfileString(_T("Settings"), _T("SkinURL"), &m_strSkinUrl, direction, _T("http://www.bcgsoft.com/Skins"));
	}

	// choose default look and feel in dependency of the system we are
	// running on.
	// 
	// default: Windows 2000 style
	// when theming is enabled: Windows XP style
	CString			strDefaultLookAndFeel(_T("Windows 2000"));
	HINSTANCE		hThemeLib = LoadLibrary(_T("UxTheme.dll"));
	if (hThemeLib)
	{
		typedef BOOL (__stdcall *TIsThemeActive)();
		//TIsThemeActive	IsThemeActive = (TIsThemeActive)GetProcAddress(hThemeLib, _T("IsThemeActive"));
		TIsThemeActive	IsThemeActive = (TIsThemeActive)GetProcAddress(hThemeLib, "IsThemeActive");
		if (IsThemeActive && IsThemeActive())
			strDefaultLookAndFeel = _T("Windows XP");

		FreeLibrary(hThemeLib);
	}

	SerializeProfileString(_T("Settings"), _T("LookAndFeel"), &m_strLookAndFeelOnNextStart, direction, strDefaultLookAndFeel);
	if (direction==Load)
		m_strLookAndFeel = m_strLookAndFeelOnNextStart;

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Language
	strSection = _T("Settings\\Language");

	// help german users from older version to upgrade -- so set german
	// language as default on german systems.
	SerializeProfileString( strSection, _T("GuiLanguage"), &m_strGuiLanguageOnNextStart, direction, ((GetUserDefaultLangID()&0xFF)==0x07)? _T("Deutsch") : _T("English"));
	if (direction==Load)
	{
		m_strGuiLanguage = m_strGuiLanguageOnNextStart;
		SerializeProfileString(strSection, _T("GuiLanguageOnLastSession"), &m_strGuiLanguageOnLastSession, direction, m_strGuiLanguage);
	}
	else
		SerializeProfileString(strSection, _T("GuiLanguageOnLastSession"), &m_strGuiLanguage, direction);

	SerializeProfileString( strSection, _T("Language"), &m_strLanguageDefault, direction, AfxLoadString(IDS_LANGUAGE) );
	SerializeProfileString( strSection, _T("Dialect"), &m_strLanguageDialectDefault, direction, AfxLoadString(IDS_DIALECT) );
	SerializeProfileString( strSection, _T("Locale"), &m_strLocale, direction, _T("") );

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Language-Spelling
	strSection = _T("Settings\\Language-Spelling");

	SerializeProfileString( strSection, _T("PersonalDictionary"), &m_strSpellPersonalDictionary, direction, _T("") );
	SerializeProfileString( strSection, _T("DictionaryPath"), &m_strSpellDictionaryPath, direction, _T(""));
	if (!CPathTool::Exists(m_strSpellDictionaryPath))
		m_strSpellDictionaryPath = CPathTool::Cat(CPathTool::GetDirectory(theApp.m_pszHelpFilePath), _T("Language"));
	SerializeProfileInt( strSection, _T("SkipComments"), (int*)&m_bSpellSkipComments, direction, 1 );
	SerializeProfileInt( strSection, _T("SkipNumbers"), (int*)&m_bSpellSkipNumbers, direction, 1 );
	SerializeProfileInt( strSection, _T("SkipTags"), (int*)&m_bSpellSkipTags, direction, 1 );
	SerializeProfileInt( strSection, _T("SkipUppercase"), (int*)&m_bSpellSkipCaps, direction, 1 );
	SerializeProfileInt( strSection, _T("MainDictionaryOnly"), (int*)&m_bSpellMainDictOnly, direction, 1 );
	SerializeProfileInt( strSection, _T("Enable"), (int*)&m_bSpellEnable, direction, 0 );

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// TextModules
	strSection = "Settings\\TextModules";

	//Serialize the currently one and only Group of TextModules
	if (direction == Load)
	{
		m_aTextModules.SerializeFromRegistry(CPathTool::Cat(theApp.m_strRegistryRoot, strSection));
	}
	else
	{
		m_aTextModules.SerializeToRegistry(CPathTool::Cat(theApp.m_strRegistryRoot, strSection));
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// File Cleaning
	strSection = "Settings\\FileClean";

	SerializeProfileInt( strSection, _T("Confirm"), (int*)&m_bFileCleanConfirm, direction, 1 );
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

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Profiles
//	strSection = "Profiles";
//	SerializeProfileString( strSection, _T("QuickRunFilePattern"), &m_strQuickRunTmpFilePattern, direction, _T("%bm_part.tex"));
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
			RegCreateKeyEx( HKEY_CURRENT_USER, strSection, 0, _T("Binary"), 0, KEY_WRITE,
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

		SerializeProfileData( szSection, _T("Place"), lpWndPlc, Save, sizeof( WINDOWPLACEMENT ) );
		SerializeProfileInt( szSection, _T("Iconified"), (int*)&bIconified, Save );
		SerializeProfileInt( szSection, _T("Maximized"), (int*)&bMaximized, Save );

		return TRUE;
	}
	else
	{
		BOOL	bIconified;
		BOOL	bMaximized;

		lpWndPlc->length = 0;	// mark invalid
		SerializeProfileData( szSection, _T("Place"), lpWndPlc, Load, sizeof( WINDOWPLACEMENT ), lpWndPlc );
		if( !lpWndPlc->length )
			return FALSE;

		SerializeProfileInt( szSection, _T("Iconified"), (int*)&bIconified, Load );
		SerializeProfileInt( szSection, _T("Maximized"), (int*)&bMaximized, Load );

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
		LPCTSTR		szFormat = _T("String%d");
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
		LPCTSTR		szFormat = _T("String%d");
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
