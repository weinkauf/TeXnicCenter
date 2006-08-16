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

#if !defined(AFX_CONFIGURATION_H__E7A12DE1_107E_11D3_929E_8FAE545EAC4D__INCLUDED_)
#define AFX_CONFIGURATION_H__E7A12DE1_107E_11D3_929E_8FAE545EAC4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileClean.h"
#include "TextModules.h"


class CConfiguration  
{
public:
	CConfiguration();

	/** Symbols for the second parameter of Serialize() */
	typedef enum{Load, Save} SERDIRECTION;

	/**
	Stores the configuration to the registry or reads it from the registry.

	@direction
		Specifies, if the function should store the configuration in the registry
		(CConfiguration::Save) or load it from the registry (CConfiguration::Load)
	*/
	void Serialize( SERDIRECTION direction );

// operations
protected:
	/**
	Writes an integer to or reads an integer from the registry.

	@param szSection
			Section in the registry, that contains the integer.
	@param szEntry
			Key of the integer.
	@param pnValue
			Pointer to a variable that should receive the read integer or that
			contains the integer to write
	@param bStoring
			CConfiguration::Save to store pnValue or CConfiguration::Load
			to read an integer to pnValue.
	@param szDefault
			If you try to read an integer and the specified key does not exist
			in the registry, then this default value will be used.
	*/
	void SerializeProfileInt( LPCTSTR szSection, LPCTSTR szEntry, 
						int* pnValue, SERDIRECTION direction, int nDefault = 0 );

	/**
	Writes a string to or reads a string from the registry.

	@param szSection
			Section in the registry, that contains the string.
	@param szEntry
			Key of the string
	@param pstrString
			Pointer to a variable that should receive the read string or that
			contains the string to write
	@param bStoring
			CConfiguration::Save to store pstrString or CConfiguration::Load
			to read a string to pstrString.
	@param szDefault
			If you try to read a string and the specified key does not exist
			in the registry, then this default value will be used.
	*/
	void SerializeProfileString( LPCTSTR szSection, LPCTSTR szEntry, 
						CString *pstrString, SERDIRECTION direction, LPCTSTR szDefault = "" );

	/**
	Writes binary data to or read binary data from the registry.

	@param szSection
			Section in the registry, that contains the data.
	@param szEntry
			Key of the data.
	@param pbyData
			Pointer to a buffer that should receive the read data or that
			contains the data to write.
	@param bStoring
			CConfiguration::Save to store pbyData or CConfiguration::Load
			to read data to pbyData.
	@param nDataLen
			Number of bytes pointed to by pbyData. The method returns FALSE,
			if data is read and the read data has another size than nDataLen.
	@param szDefault
			If you try to read data and the specified key does not exist
			in the registry, then this default value will be used.
	*/
	void SerializeProfileData( LPCTSTR szSection, LPCTSTR szEntry,
						LPVOID lpData, SERDIRECTION direction, UINT unDataLen, LPVOID lpDefaulData = NULL );

	/**
	Writes a windows placement to or read it from the registry.

	<b>Remarks:</b> If direction is CConfiguration::Load and the specified section
	does not contain the data, then FALSE will be returned and lpWndPlc->length will be 0.

	@param szSection
		Section in the registry, that contains the data.
	@param lpWndPlc
		Pointer to structure to write from or to read to.
	@param direction
		CConfiguration::Save to store the data, CConfiguration::Load to read the data.

	@return
		TRUE if successfull, FALSE otherwise.
	*/
	BOOL SerializeProfileWndPlacement( LPCTSTR szSection, WINDOWPLACEMENT* lpWndPlc, 
						SERDIRECTION direction );

	/**
	Writes a string array to or reads a string array from the registry.

	@param szSection
			Section in the registry, that contains the string array .
	@param szEntry
			Key of the string array
	@param pastrArray
			Pointer to a variable that should receive the read string array or that
			contains the string array to write.
	@param bStoring
			CConfiguration::Save to store pastrArray or CConfiguration::Load
			to read a string to pastrArray.

	@return
		TRUE if successfull, FALSE if the array has not been read.
	*/
	BOOL SerializeProfileStringArray( LPCTSTR szSection, LPCTSTR szEntry, 
						CStringArray *pastrArray, SERDIRECTION direction );

	/**
	Deletes the specified key from the registry.

	@param lpszSection
		Path of the key to remove from the registry.
	@param bAdmin
		TRUE, if lpszPath should be relative to HKEY_LOCAL_MACHINE, 
		FALSE if lpszPath should be relative to HKEY_CURRENT_USER.

	@return
		TRUE if successfull, FALSE otherwise.
	*/
	BOOL RegDeleteSection( LPCTSTR lpszSection, BOOL bAdmin = FALSE );

/* types */
public:
	typedef struct tagSizeDockBarPlace
	{
		DWORD		dwDockingFlag;
		CRect		rectBar;
	} SIZEDOCKBARPLACE;

/* Attributes */
public:
	/** TRUE, if document-windows should be opened maximized */
	BOOL m_bOpenDocWndMaximized;

	/** Paths to search for project templates in. */
	CStringArray m_astrProjectTemplatePaths;

	/** Paths to search for document templates in. */
	CStringArray m_astrDocumentTemplatePaths;

	/** Maximal number of entries (editor windows) in the window menu. */
	int m_nWndMenuMaxEntries;

	///////////////////////////////////////////////////////////////////
	// File	Handling

	/** TRUE, if TeXnicCenter should always save newly created files. */
	BOOL m_bSaveNewDocuments;

	/** TRUE, if TeXnicCenter should save all modified tex-files, before compilation.*/
	BOOL m_bSaveBeforeCompilation;

	/** TRUE, if TeXnicCenter should save all open files in given intervals. */
	BOOL m_bSaveAutomatic;

	/** The interval, TeXnicCenter should save all open files in. @see m_bSaveAutomatic. */
	UINT m_unSaveInterval;

	/** Standard file format used to create new documents or to save documents. */
	int m_nStandardFileFormat;

	/** Default path. */
	CString m_strDefaultPath;

	/** Last opened Folder for Load- and Save-Dialogs. */
	CString m_strLastOpenedFolder;

	///////////////////////////////////////////////////////////////////
	// File	Handling - Template dialog params

	/** Last opened tab of the project template dialog */
	int m_nLastTabProjectTemplateDlg;

	/** Last opened tab of the document template dialog */
	int m_nLastTabDocumentTemplateDlg;


	///////////////////////////////////////////////////////////////////
	// character replacement

	/** TRUE, if the Editor should replace opening and closing quotation marks while typing.*/
	BOOL m_bReplaceQuotationMarks;

	/** Text, the Editor should replace an opening quotation mark with. @see m_bReplaceQuotationMarks.*/
	CString m_strOpeningQuotationMark;

	/** Text, the Editor should replace an closing quotation mark with. @see m_bReplaceQuotationMarks.*/
	CString m_strClosingQuotationMark;


	/** Interval in milliseconds to parse the project. */
	int m_nParseInterval;

	///////////////////////////////////////////////////////////////////
	// Project specific

	/** TRUE, if we should load the last project at startup. */
	BOOL m_bLoadLastProject;

	/** Path of the latest project. */
	CString m_strLastProject;

	///////////////////////////////////////////////////////////////////
	// Editor settings

	/** The font, used in the editor */
	LOGFONT m_fontEditor;

	/** Colors for editor components */
	COLORREF m_aEditorColors[CCrystalTextView::COLORINDEX_ERRORBKGND];

	/** Width of a tab in characters */
	int m_nTabWidth;
	
	/** TRUE, if white spaces shout be displayed as special chars, FALSE otherwise */
	BOOL m_bViewWhitespaces;

	/** TRUE, if line numbers should be shown */
	BOOL m_bShowLineNumbers;

	/** The word warp style. */
	DWORD m_dwWordWrapStyle;

	/** Wethter word wrapping is on or off */
	BOOL m_bWordWrapEnabled;

	/**  amount of column to wrap words */
	int m_nFixedColumnWrap;

	///////////////////////////////////////////////////////////////////
	// Other window settings

	/** The font to use for the navigator */
	LOGFONT m_fontNavigator;

	/** The font to use for the output view */
	LOGFONT m_fontOutput;

	///////////////////////////////////////////////////////////////////
	// find in files settings

	/** file types, the last time was searched for. */
	CString m_strFileFindFileTypes;

	/** TRUE if whole words where searched last time. */
	BOOL m_bFileFindWholeWords;

	/** TRUE if last search was case sensitive. */
	BOOL m_bFileFindCaseSensitive;

	/** TRUE if last search was a regular expression. */
	BOOL m_bFileFindRegularExpression;

	/** TRUE if last time sub folders where searched. */
	BOOL m_bFileFindSubFolders;

	/** Index of output buffer, where last time the output was written to */
	int m_nFileFindOutput;

	///////////////////////////////////////////////////////////////////
	// Skin initialization

	/** Local skin path */
	CString m_strSkinDirectory;

	/** URL, where to download skins from */
	CString m_strSkinUrl;

	/** Look and feel currently in use */
	CString m_strLookAndFeel;

	/** Look and feel activated on next start */
	CString m_strLookAndFeelOnNextStart;


	///////////////////////////////////////////////////////////////////
	// Language and spelling

	/** User interface language currently in use. */
	CString m_strGuiLanguage;

	/** User interface language activated on next start. */
	CString m_strGuiLanguageOnNextStart;

	/** User interface language, that was active in the last session. */
	CString m_strGuiLanguageOnLastSession;

	/** Lanuage of the dictionary. */
	CString m_strLanguageDefault;

	/** Dialect of the dictionary. */
	CString m_strLanguageDialectDefault;

	/** Locale */
	CString m_strLocale;

	/** Directory containing dictionaries. */
	CString m_strSpellDictionaryPath;

	/** Path of personal dictionary. */
	CString m_strSpellPersonalDictionary;

	/** Skip comments. */
	BOOL m_bSpellSkipComments;

	/** Skip words with numbers. */
	BOOL m_bSpellSkipNumbers;

	/** Skip LaTeX tags. */
	BOOL m_bSpellSkipTags;

	/** Skip all uppsercase words .*/
	BOOL m_bSpellSkipCaps;

	/** Suggest from main dictionary only. */
	BOOL m_bSpellMainDictOnly;

	/** Enable spell check. */
	BOOL m_bSpellEnable;

	///////////////////////////////////////////////////////////////////
	// TextModules
	
	/** Group that holds the user defined TextModules. */
	CTextModuleGroup m_aTextModules;

	///////////////////////////////////////////////////////////////////
	// File Cleaning
	
	/** Array to hold the patterns for file cleaning. */
	CFileCleanItemArray m_aFileCleanItems;

	bool m_bFileCleanConfirm;

	///////////////////////////////////////////////////////////////////
	// Accessability

	/** TRUE to optimize the GUI for visually handicapped users. */
	BOOL m_bOptimizeMenuForVisuallyHandicappedUsers;

	/** 
	TRUE to optimize the GUI for visually handicapped users in the
	next session.
	*/
	BOOL m_bOptimizeMenuForVisuallyHandicappedUsersOnNextStart;

	///////////////////////////////////////////////////////////////////
	// Accessability

	/** TRUE to optimize the GUI for visually handicapped users. */
	CString m_strQuickRunTmpFilePattern;
};

// declaration of one and only global configuration object
extern CConfiguration g_configuration;

#endif // !defined(AFX_CONFIGURATION_H__E7A12DE1_107E_11D3_929E_8FAE545EAC4D__INCLUDED_)
