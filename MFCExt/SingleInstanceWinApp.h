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

#if !defined(AFX_SINGLEINSTANCEWINAPP_H__78123A40_33D8_11D4_A221_006097239934__INCLUDED_)
#define AFX_SINGLEINSTANCEWINAPP_H__78123A40_33D8_11D4_A221_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <ddeml.h>

/**
When you use this class as the base class for your application class,
your application becomes a single instance application.

This means, that only one instance of your application is created. If
the user tries to start a new one, the already existing instance 
is activated and command line and DDE-options are forwarded to the
already existing instance.

<b>Important:</b> This class implements the InitInstance() and
ExitInstance() virtual methods of CWinApp. You have to call 
CSingleInstanceWinApp::InitInstance() at the beginning of your
InitInstance()-implementation and 
CSingleInstanceWinApp::ExitInstance() at the end of your
ExitInstance()-implementation.

Further on you have to override the pure virtuals GetServerName().

This class is based on the article "Single Instance Program" by 
Raymon Shawn (raymon@public1.sz.js.cn), found at Code Guru
(http://www.codeguru.com/misc/single_instance3.shtml)

@ingroup mfcext

@author Sven Wiegand
*/
class  CSingleInstanceWinApp : public CWinApp
{
// friends
	friend HDDEDATA CALLBACK DdeCallback( UINT unType, UINT unFmt, HCONV hconv, HSZ hsz1, HSZ hsz2, HDDEDATA hdata, DWORD dwData1, DWORD dwData2 );

// construction/destruction
public:
	CSingleInstanceWinApp();
	virtual ~CSingleInstanceWinApp();

// Implementation helpers
private:
	HDDEDATA AppDdeCallback( UINT unType, UINT unFmt, HCONV hconv, HSZ hsz1, HSZ hsz2, HDDEDATA hdata, DWORD dwData1, DWORD dwData2 );

protected:
	/**
	Sends the specified DDE-command to the already running instance
	of the application.

	@param lpszDdeCommand
		The command to send to the already running instance of the
		application (i.e. "[open(\"test.txt\")]").

	@return
		TRUE if successfull, FALSE otherwise.
	*/
	BOOL SendDdeCommand( LPCTSTR lpszDdeCommand );

// Overridables
protected:
	/**
	Override this virtual, to forward command line options, specific
	to your application, to the already running instance of your 
	application as DDE-command.

	Use SendDdeCommand() to send a DDE-command to the already running
	instance of your application.

	Use DdeClientTransaction() with the given handle, to send data
	that is <em>not</em> a DDE-command to the already running instance.

	The base implementation handles the standard command line options
	for opening and printing documents.

	@param hconvServer
		Handle to the DDE-conversation between the DDE-server (the 
		already running instance of the app) and the DDE-client
		(this instance of the app).
	*/
	virtual void ForwardCommands( HCONV hconvServer );

// Overridings
protected:
	//{{AFX_VIRTUAL(CSingleInstanceWinApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Message handlers
protected:
	//{{AFX_MSG(CSingleInstanceWinApp)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

// Attributes
private:
	/** 
	Will be set to TRUE, if an instance of this application was already
	running.
	*/
	BOOL m_bWasRunningAlready;

	/** DDE-specific instance ID. */
	DWORD m_dwIdInstance;

	/** 
	DDE-string that specifies the provided DDE-service 
	(application name) 
	*/
	HSZ m_hszService;

	/**
	DDE-string that specifies the supported DDE-services.
	*/
	HSZ m_hszTopic;

	/**
	Handle to the DDE-conversation with an already running instance
	of this application.
	*/
	HCONV m_hconvServer;

	/** 
	Command line switch (without '/' or '-'), or an empty string, if
	there has been specified no command line switch.
	*/
	CString m_strWantSingleInstanceSwitch;

	/** Short version of m_strWantSingleInstanceSwitch */
	CString m_strWantSingleInstanceSwitchShort;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_SINGLEINSTANCEWINAPP_H__78123A40_33D8_11D4_A221_006097239934__INCLUDED_
