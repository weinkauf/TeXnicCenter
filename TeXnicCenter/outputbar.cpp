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
#include "TeXnicCenter.h"
#include "OutputBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// class COutputBar
//-------------------------------------------------------------------

COutputBar::COutputBar()
:	m_pwndBuildView( NULL ),
	m_pwndGrepView1( NULL ),
	m_pwndGrepView2( NULL ),
	m_pwndShellView( NULL ),
	m_pwndParseView( NULL )
{
	m_pOutputDoc = NULL;
}


COutputBar::~COutputBar()
{
	DeleteViews();
	if (m_pOutputDoc != NULL)
	{
		delete m_pOutputDoc;
		m_pOutputDoc = NULL;
	}
}


BEGIN_MESSAGE_MAP(COutputBar, CBCGSizingControlBar)
	//{{AFX_MSG_MAP(COutputBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


int COutputBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGSizingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create tabs window:
	if( !m_wndTabs.Create( CBCGTabWnd::STYLE_FLAT/*_SHARED_HORZ_SCROLL*/, rectDummy, this, 1 ) )
	{
		TRACE0("Failed to create workspace tab window\n");
		return -1;      // fail to create
	}

	//m_wndTabs.SetImageList( IDB_OUTPUTTAB, 16, RGB(0, 0x80, 0x80) );
	CreateViews(NULL);
	
	return 0;
}


void COutputBar::CreateViews(COutputDoc *pDocument )  //UPDATE
{
	if (pDocument == NULL) //UPDATE
	{
		if (m_pOutputDoc == NULL)
			m_pOutputDoc = new COutputDoc();
		//m_pOutputDoc->SetWorkingDir(theApp.GetWorkingDir());
		pDocument = m_pOutputDoc;
	}
	// Check if views are allready existing
	if( m_pwndBuildView && IsWindow( m_pwndBuildView->m_hWnd ) )
		return;

	// Initialize create context
	/*
	CCreateContext	cc;
	ZeroMemory( &cc, sizeof( cc ) );
	*/

	// if no context specified, generate one 
	// from the currently selected client if possible...

	// Create views:
	CRect						rectDummy;
	rectDummy.SetRectEmpty();

	m_pwndBuildView = new CBuildView;
	m_pwndGrepView1 = new CGrepView;
	m_pwndGrepView2 = new CGrepView;
	m_pwndParseView = new CParseOutputView;
	//m_pwndShellView = new CShellView;

	ASSERT(m_pwndBuildView);
	ASSERT(m_pwndGrepView1);
	ASSERT(m_pwndGrepView2);
	ASSERT(m_pwndParseView);
	//ASSERT(m_pwndShellView);

	if( !m_pwndBuildView || !m_pwndGrepView1 || 
		!m_pwndGrepView2 || !m_pwndParseView)// || !m_pwndShellView)
	{
		delete m_pwndBuildView;
		delete m_pwndGrepView1;
		delete m_pwndGrepView2;
		delete m_pwndParseView;
		/*
		if( m_pwndShellView )
			delete m_pwndShellView;
		*/

		return;
	}

	//cc.m_pNewViewClass      = RUNTIME_CLASS( CBuildView );
	if (!m_pwndBuildView->Create(rectDummy, &m_wndTabs))
	{
		TRACE0("Failed to create build view\n");
		return;
	}
	m_pwndBuildView->SetOwner(this);

	//cc.m_pNewViewClass      = RUNTIME_CLASS( CGrepView );
	if (!m_pwndGrepView1->Create(rectDummy, &m_wndTabs))
	{
		TRACE0("Failed to create first grep view\n");
		return;
	}
	m_pwndGrepView1->SetOwner(this);

	//cc.m_pNewViewClass      = RUNTIME_CLASS( CGrepView );
	if (!m_pwndGrepView2->Create(rectDummy, &m_wndTabs))
	{
		TRACE0("Failed to create second grep view\n");
		return;
	}
	m_pwndGrepView2->SetOwner(this);

	if (!m_pwndParseView->Create(rectDummy, &m_wndTabs))
	{
		TRACE0("Failed to create parse output view\n");
		return;
	}
	m_pwndParseView->SetOwner(this);

	/*
	cc.m_pNewViewClass      = RUNTIME_CLASS( CShellView );
	if( !m_pwndShellView->Create( NULL, NULL, AFX_WS_DEFAULT_VIEW & ~WS_BORDER, 
		rectDummy, &m_wndTabs, 0, &cc ) )
	{
		TRACE0("Failed to create second grep view\n");
		return;
	}
	*/

	// Explicitly set the views in the documents
	static_cast<COutputDoc*>(pDocument)->SetAllViews(m_pwndBuildView, 
		m_pwndGrepView1, m_pwndGrepView2, m_pwndParseView);

	// Attach document to views
	m_pwndBuildView->AttachDoc(pDocument);
	m_pwndGrepView1->AttachDoc(pDocument);
	m_pwndGrepView2->AttachDoc(pDocument);
	m_pwndParseView->AttachDoc(pDocument);

	// Attach views to tab:
	m_wndTabs.AddTab( m_pwndBuildView, CString( (LPCTSTR)STE_TAB_BUILD ),  -1 );
	m_wndTabs.AddTab( m_pwndGrepView1, CString( (LPCTSTR)STE_TAB_GREP_1 ), -1 );
	m_wndTabs.AddTab( m_pwndGrepView2, CString( (LPCTSTR)STE_TAB_GREP_2 ), -1 );
	m_wndTabs.AddTab( m_pwndParseView, CString( (LPCTSTR)STE_TAB_PARSE ), -1 );
	//m_wndTabs.AddTab( m_pwndShellView, CString( _T("DOS Shell") ),         -1 );   //TODO-to resource file

	// Set grep view indices
	m_pwndGrepView1->SetIndex( 0 );
	m_pwndGrepView2->SetIndex( 1 );

	// Sorry, but we have to activate one view manually, so that the
	// project-document can receive messages
	//((CFrameWnd*)GetParent())->SetActiveView( m_pwndBuildView );
}


void COutputBar::DeleteViews()
{
	// Check if views are valid windows
	if (m_pwndBuildView && IsWindow(m_pwndBuildView->m_hWnd))
		// Detach views from tab ...
		m_wndTabs.RemoveAllTabs();

	// ... and destroy them
	delete m_pwndBuildView;
	m_pwndBuildView = NULL;
	delete m_pwndGrepView1;
	m_pwndGrepView1 = NULL;
	delete m_pwndGrepView2;
	m_pwndGrepView2 = NULL;
	delete m_pwndParseView;
	m_pwndParseView = NULL;
	/*
	delete m_pwndShellView;
	m_pwndShellView = NULL;
	*/
}


void COutputBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGSizingControlBar::OnSize(nType, cx, cy);
	
	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy,
		SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);	
}
