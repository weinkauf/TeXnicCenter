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
#include "stdafx.h"
#include "PlaceHolder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//-------------------------------------------------------------------
// global functions
//-------------------------------------------------------------------

CString AfxExpandPlaceholders(LPCTSTR lpszStringWithPlaceholders, 
															LPCTSTR lpszMainPath /*= NULL*/, 
															LPCTSTR lpszCurrentPath /*= NULL*/, 
															long lCurrentLine /*= -1*/,
															LPCTSTR lpszCurrentSelection /*= NULL*/)
{
	CString		strCmdLine(lpszStringWithPlaceholders);
	CString		strLine;

	if( lCurrentLine > -1 )
		strLine.Format( _T("%d"), lCurrentLine );

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// format command line

	// store "%%"
	strCmdLine.Replace( _T("%%"), _T("\a") );

	// replace place holders
	if (lpszMainPath)
	{
		CPathTool	main(lpszMainPath);

		main.EnsureBackslashPath();
		main.EnsureLongPath();
		strCmdLine.Replace( _T("%pm"), main.GetPath() );
		strCmdLine.Replace( _T("%dm"), main.GetDirectory() );
		strCmdLine.Replace( _T("%nm"), main.GetFile() );
		strCmdLine.Replace( _T("%tm"), main.GetFileTitle() );
		strCmdLine.Replace( _T("%em"), main.GetFileExtension() );
		strCmdLine.Replace( _T("%bm"), CPathTool::Cat( main.GetDirectory(), main.GetFileTitle() ) );

		main.EnsureSlashPath();
		strCmdLine.Replace( _T("%Pm"), main.GetPath() );
		strCmdLine.Replace( _T("%Dm"), main.GetDirectory() );
		strCmdLine.Replace( _T("%Bm"), CPathTool::Cat( main.GetDirectory(), main.GetFileTitle() ) );

		main.EnsureBackslashPath();
		main.EnsureShortPath();
		strCmdLine.Replace( _T("%spm"), main.GetPath() );
		strCmdLine.Replace( _T("%sdm"), main.GetDirectory() );
		strCmdLine.Replace( _T("%snm"), main.GetFile() );
		strCmdLine.Replace( _T("%stm"), main.GetFileTitle() );
		strCmdLine.Replace( _T("%sem"), main.GetFileExtension() );
		strCmdLine.Replace( _T("%sbm"), CPathTool::Cat( main.GetDirectory(), main.GetFileTitle() ) );

		main.EnsureSlashPath();
		strCmdLine.Replace( _T("%sPm"), main.GetPath() );
		strCmdLine.Replace( _T("%sDm"), main.GetDirectory() );
		strCmdLine.Replace( _T("%sBm"), CPathTool::Cat( main.GetDirectory(), main.GetFileTitle() ) );
	}

	if( lpszCurrentPath )
	{
		CPathTool	current( lpszCurrentPath );

		current.EnsureBackslashPath();
		current.EnsureLongPath();
		strCmdLine.Replace( _T("%pc"), current.GetPath() );
		strCmdLine.Replace( _T("%dc"), current.GetDirectory() );
		strCmdLine.Replace( _T("%nc"), current.GetFile() );
		strCmdLine.Replace( _T("%tc"), current.GetFileTitle() );
		strCmdLine.Replace( _T("%ec"), current.GetFileExtension() );
		strCmdLine.Replace( _T("%bc"), CPathTool::Cat( current.GetDirectory(), current.GetFileTitle() ) );

		current.EnsureSlashPath();
		strCmdLine.Replace( _T("%Pc"), current.GetPath() );
		strCmdLine.Replace( _T("%Dc"), current.GetDirectory() );
		strCmdLine.Replace( _T("%Bc"), CPathTool::Cat( current.GetDirectory(), current.GetFileTitle() ) );

		current.EnsureBackslashPath();
		current.EnsureShortPath();
		strCmdLine.Replace( _T("%spc"), current.GetPath() );
		strCmdLine.Replace( _T("%sdc"), current.GetDirectory() );
		strCmdLine.Replace( _T("%snc"), current.GetFile() );
		strCmdLine.Replace( _T("%stc"), current.GetFileTitle() );
		strCmdLine.Replace( _T("%sec"), current.GetFileExtension() );
		strCmdLine.Replace( _T("%sbc"), CPathTool::Cat( current.GetDirectory(), current.GetFileTitle() ) );

		current.EnsureSlashPath();
		strCmdLine.Replace( _T("%sPc"), current.GetPath() );
		strCmdLine.Replace( _T("%sDc"), current.GetDirectory() );
		strCmdLine.Replace( _T("%sBc"), CPathTool::Cat( current.GetDirectory(), current.GetFileTitle() ) );
	}

	if( lCurrentLine > -1 )
		strCmdLine.Replace( _T("%l"), strLine );

	if (lpszCurrentSelection != NULL)
		strCmdLine.Replace(_T("%s"), lpszCurrentSelection);

	// restore "%"
	strCmdLine.Replace( _T("\a"), _T("%") );

	return strCmdLine;
}
