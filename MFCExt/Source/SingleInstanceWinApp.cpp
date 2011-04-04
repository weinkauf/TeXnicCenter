/********************************************************************
*
* This file is part of the MFCExt-Library
*
* Copyright (C) 1999-2000 Sven Wiegand
* Copyright (C) 2000-$CurrentYear$ ToolsCenter
* 
* This library is free software; you can redistribute it and/or
* modify, but leave the headers intact and do not remove any 
* copyrights from the source.
*
* This library does not only contain file from us, but also from
* third party developers. Look at the source file headers to get
* detailed information.
*
* If you have further questions visit our homepage
*
*    http://www.ToolsCenter.org
*
********************************************************************/

/********************************************************************
*
* $Id$
*
********************************************************************/

#include "stdafx.h"
#include "SingleInstanceWinApp.h"
#include <dde.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DDE_SERVICE	((LPTSTR)(LPCTSTR)(CString( m_pszAppName )))
#define DDE_TOPIC		_T("SingleInstanceHandling")

CSingleInstanceWinApp *g_pOneApp;

//-------------------------------------------------------------------
// class CSIACommandLineInfo
//-------------------------------------------------------------------
class CSIACommandLineInfo : public CCommandLineInfo
{
// Construction/destruction
public:
	/**
	If both strings are empty, m_bWantSingleInstance becomes TRUE, no
	matter of the command line.
	*/
	CSIACommandLineInfo( const CString &strWantSISwitch, const CString &strWantSISwitchShort );

// Overridings
protected:
	virtual void ParseParam( LPCTSTR lpszParam, BOOL bFlag, BOOL bLast );

// Attributes
public:
	BOOL m_bWantSingleInstance;

protected:
	CString m_strWantSISwitch;
	CString m_strWantSISwitchShort;
};

CSIACommandLineInfo::CSIACommandLineInfo( const CString &strWantSISwitch, const CString &strWantSISwitchShort )
:	CCommandLineInfo(),
	m_bWantSingleInstance( FALSE ),
	m_strWantSISwitch( strWantSISwitch ),
	m_strWantSISwitchShort( strWantSISwitchShort )
{
	if( m_strWantSISwitch.IsEmpty() && m_strWantSISwitchShort.IsEmpty() )
		m_bWantSingleInstance = TRUE;
}


void CSIACommandLineInfo::ParseParam( LPCTSTR lpszParam, BOOL bFlag, BOOL bLast )
{
	if( m_bWantSingleInstance )
	{
		CCommandLineInfo::ParseParam( lpszParam, bFlag, bLast );
		return;
	}

	if( !m_strWantSISwitch.IsEmpty() && bFlag && m_strWantSISwitch == lpszParam )
		m_bWantSingleInstance = TRUE;
	else if( !m_strWantSISwitchShort.IsEmpty() && bFlag && m_strWantSISwitchShort == lpszParam )
		m_bWantSingleInstance = TRUE;
	else
		CCommandLineInfo::ParseParam( lpszParam, bFlag, bLast );
}


//-------------------------------------------------------------------
// friends of CSingleInstanceWinApp
//-------------------------------------------------------------------

HDDEDATA CALLBACK DdeCallback( UINT unType, UINT unFmt, HCONV hconv, HSZ hsz1,
	HSZ hsz2, HDDEDATA hdata, DWORD dwData1, DWORD dwData2 )
{
	return ( g_pOneApp->AppDdeCallback( unType, unFmt, hconv, hsz1, hsz2, 
		hdata, dwData1, dwData2 ) );
} 


//-------------------------------------------------------------------
// class CSingleInstanceWinApp
//-------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CSingleInstanceWinApp, CWinApp)
	//{{AFX_MSG_MAP(CSingleInstanceWinApp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CSingleInstanceWinApp::CSingleInstanceWinApp()
:	CWinApp(),
	m_bWasRunningAlready( FALSE ),
	m_dwIdInstance( 0 ),
	m_strWantSingleInstanceSwitch(_T("SingleInstance")),
	m_strWantSingleInstanceSwitchShort(_T("1"))
{
	g_pOneApp = this;
}


CSingleInstanceWinApp::~CSingleInstanceWinApp()
{}


BOOL CSingleInstanceWinApp::InitInstance()
{
	CSIACommandLineInfo cmdInfo( m_strWantSingleInstanceSwitch, m_strWantSingleInstanceSwitchShort );
	ParseCommandLine(cmdInfo);

	// if we should run embedded create a new instance
	if( cmdInfo.m_bRunEmbedded )
		return TRUE;

	// Initiate DDE-conversation
	if( DdeInitialize( &m_dwIdInstance, (PFNCALLBACK)DdeCallback, 0, 0 ) ) 
		return FALSE; 

	m_hszService = DdeCreateStringHandle( m_dwIdInstance, DDE_SERVICE, CP_WINANSI );
	m_hszTopic = DdeCreateStringHandle ( m_dwIdInstance, DDE_TOPIC, CP_WINANSI ); 

	// try to find the first instance
	m_hconvServer = DdeConnect( m_dwIdInstance, m_hszService, m_hszTopic, NULL );

	if( m_hconvServer )
	{
		// There is already an instance running!
		// Lets send our commands and quit
		m_bWasRunningAlready = TRUE;

		// If more instances are allowed, start this instance, but
		// don't register DDE name service, so that the first instance
		// started, is the master, that answers all DDE requests
		if( !cmdInfo.m_bWantSingleInstance )
			return TRUE;

		// Only one instance is allowed.
		// So lets forward our commands to the already running instance...
		ForwardCommands( m_hconvServer );

		// ... and quit this instance
		return FALSE;
	}
	
	if( !DdeNameService( m_dwIdInstance, m_hszService, 0, DNS_REGISTER ) )
		TRACE0("Registration of DDE name service failed");

	return TRUE;
}


int CSingleInstanceWinApp::ExitInstance()
{
	// only unregister the DDE server for first instance
	if( !m_bWasRunningAlready )
	{
		if( !DdeNameService( m_dwIdInstance, m_hszService, 0, DNS_UNREGISTER ) )
			TRACE0("Unregistration of DDE name service failed");
	}

	DdeFreeStringHandle( m_dwIdInstance, m_hszService );
	DdeFreeStringHandle( m_dwIdInstance, m_hszTopic ); 

	DdeUninitialize( m_dwIdInstance );	

	return CWinApp::ExitInstance();
}


HDDEDATA CSingleInstanceWinApp::AppDdeCallback( UINT unType, UINT unFmt, HCONV hconv, HSZ hsz1,
	HSZ hsz2, HDDEDATA hdata, DWORD dwData1, DWORD dwData2 )
{
	int				nCount;
	CString		strText;
	HDDEDATA	hRet = (HDDEDATA)NULL;
	CWnd*			pWnd; 

	switch( unType )
	{
		case XTYP_CONNECT:
			nCount = DdeQueryString( m_dwIdInstance, hsz1, NULL, 0, CP_WINANSI );
			DdeQueryString( m_dwIdInstance, hsz1, strText.GetBuffer( nCount ), nCount+1, CP_WINANSI );
			strText.ReleaseBuffer();

			if( strText == DDE_TOPIC )
			{
				pWnd = AfxGetMainWnd();
				//pWnd->ShowWindow(SW_RESTORE);
				pWnd->BringWindowToTop( );
				pWnd->SetForegroundWindow( ); 

				hRet = (HDDEDATA) DDE_FACK;
			}
			return hRet; 

		case XTYP_EXECUTE:
			// forward command to main window
			nCount = DdeGetData( hdata, NULL, 0, 0 );
			DdeGetData( hdata, (LPBYTE)strText.GetBuffer( nCount ), nCount, 0 );
			strText.ReleaseBuffer();

			pWnd = AfxGetMainWnd();
			if( !pWnd )
				return hRet;

			{
				// Senn DDE command to applications main window
				HGLOBAL	hCommand;

				hCommand = GlobalAlloc( GMEM_DDESHARE, nCount + 1 );
				if( !hCommand )
					return hRet;
				
				CopyMemory( (PVOID)hCommand, (CONST VOID*)(LPCTSTR)strText, nCount );
				SendMessage( pWnd->m_hWnd, WM_DDE_EXECUTE, (WPARAM)NULL, (LPARAM)hCommand );

				GlobalFree( hCommand );
			}
			return hRet;
      
		default: 
			return hRet;
	}
} 


void CSingleInstanceWinApp::ForwardCommands( HCONV hconvServer )
{
	CCommandLineInfo	cmdInfo;
	CString						strCmd;
	ParseCommandLine(cmdInfo);

	switch( cmdInfo.m_nShellCommand )
	{
		case CCommandLineInfo::FileOpen:
			strCmd.Format( _T("[open(\"%s\")]"), cmdInfo.m_strFileName );
			SendDdeCommand( strCmd );
			break;

		case CCommandLineInfo::FilePrint:
			strCmd.Format( _T("[print(\"%s\")]"), cmdInfo.m_strFileName );
			SendDdeCommand( strCmd );
			break;

		case CCommandLineInfo::FilePrintTo:
			strCmd.Format( _T("[printto(\"%s\",\"%s\",\"%s\",\"%s\")]"), 
				cmdInfo.m_strFileName, 
				cmdInfo.m_strPrinterName,
				cmdInfo.m_strDriverName,
				cmdInfo.m_strPortName );
			SendDdeCommand( strCmd );
			break;
	}
}


BOOL CSingleInstanceWinApp::SendDdeCommand( LPCTSTR lpszDdeCommand )
{
	if( !m_hconvServer )
		return FALSE;

	CString	strCmd( lpszDdeCommand );
	if( strCmd.IsEmpty() )
		return TRUE;

	// ensure 8-bit-string
	int			nLen = strCmd.GetLength();
	TCHAR		*szCmd = new TCHAR[nLen + 1];

	if( !szCmd )
		return FALSE;

	for( int i = 0; i < nLen; i++ )
		szCmd[i] = (TCHAR)strCmd[i];
	szCmd[nLen] = '\0';

	// send command
	HDDEDATA	hData;

	hData = DdeClientTransaction( (LPBYTE)szCmd, nLen + 1, m_hconvServer, 0, 0, XTYP_EXECUTE, 0, NULL );

	delete[] szCmd;

	if( !hData )
		return FALSE;

	DdeFreeDataHandle( hData );
	return TRUE;
}
