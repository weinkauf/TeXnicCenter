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
#include "Configuration.h"

#include "ChildFrm.h"
#include "MDIFrameManager.h"
#include "CrystalEditViewEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CBCGMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CBCGMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame Konstruktion/Destruktion

CChildFrame::CChildFrame()
{
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::OnCreateClient( LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	// create splitter and add to window manager
	if( m_wndSplitter.Create( this,
		2, 1,
		CSize( 10, 10 ),
		pContext ) )
	{
		theApp.m_pMDIFrameManager->AddChildFrame( this );
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CBCGMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CChildFrame Diagnose

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CBCGMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CBCGMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame Nachrichten-Handler

void CChildFrame::ActivateFrame(int nCmdShow) 
{
	if( g_configuration.m_bOpenDocWndMaximized )
		CBCGMDIChildWnd::ActivateFrame(SW_SHOWMAXIMIZED);
	else
		CBCGMDIChildWnd::ActivateFrame(nCmdShow);
}

void CChildFrame::OnDestroy() 
{
	// remove from MDI-Frame-Manager
	theApp.m_pMDIFrameManager->RemoveChildFrame(this);

	// if this was the last open window,
	// then remember, if the next window should be opened maximized
	if (!GetNextWindow(GW_HWNDNEXT) && !GetNextWindow(GW_HWNDPREV))
		g_configuration.m_bOpenDocWndMaximized = IsZoomed();

	CBCGMDIChildWnd::OnDestroy();
}

void CChildFrame::OnSetFocus(CWnd* pOldWnd) 
{
	if (theApp.m_pMDIFrameManager->GetChildFrameCount())
	{
		CBCGMDIChildWnd::OnSetFocus(pOldWnd);
		theApp.m_pMDIFrameManager->ActivateChildFrame( this );
	}
}


#define VAL_FRAMEINFO_COLCOUNT					_T("Columns")
#define VAL_FRAMEINFO_ROWCOUNT					_T("Rows")
#define VAL_FRAMEINFO_FLAGS							_T("Flags")
#define VAL_FRAMEINFO_SHOWCMD						_T("ShowCmd")
#define VAL_FRAMEINFO_MINPOS_X					_T("MinPos.x")
#define VAL_FRAMEINFO_MINPOS_Y					_T("MinPos.y")
#define VAL_FRAMEINFO_MAXPOS_X					_T("MaxPos.x")
#define VAL_FRAMEINFO_MAXPOS_Y					_T("MaxPos.y")
#define VAL_FRAMEINFO_NORMALPOS_LEFT		_T("NormalPos.left")
#define VAL_FRAMEINFO_NORMALPOS_TOP			_T("NormalPos.top")
#define VAL_FRAMEINFO_NORMALPOS_RIGHT		_T("NormalPos.right")
#define VAL_FRAMEINFO_NORMALPOS_BOTTOM	_T("NormalPos.bottom")
#define	VAL_FRAMEINFO_DOCPATH						_T("Document")
#define VAL_FRAMEINFO_VIEWCLASS					_T("Class")

#define KEY_COLINFO											_T("%s_Col%d")
#define VAL_COLINFO_CXCUR								_T("cxCur")
#define VAL_COLINFO_CXMIN								_T("cxMin")

#define KEY_ROWINFO											_T("%s_Row%d")
#define VAL_ROWINFO_CYCUR								_T("cyCur")
#define VAL_ROWINFO_CYMIN								_T("cyMin")

#define KEY_VIEWINFO										_T("%s_View%d,%d")


BOOL CChildFrame::Serialize(CIniFile &ini, LPCTSTR lpszKey, BOOL bWrite)
{
	if (bWrite)
	{
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// writing data
		WINDOWPLACEMENT	wp;
		CString					strKey;
		int							nRow, nRowCount = m_wndSplitter.GetRowCount();
		int							nColumn, nColumnCount = m_wndSplitter.GetColumnCount();

		GetWindowPlacement(&wp);
		
		ini.SetValue(lpszKey, VAL_FRAMEINFO_COLCOUNT, nColumnCount);
		ini.SetValue(lpszKey, VAL_FRAMEINFO_ROWCOUNT, nRowCount);
		ini.SetValue(lpszKey, VAL_FRAMEINFO_FLAGS, (int)wp.flags);
		ini.SetValue(lpszKey, VAL_FRAMEINFO_SHOWCMD, (int)wp.showCmd);
		ini.SetValue(lpszKey, VAL_FRAMEINFO_MINPOS_X, wp.ptMinPosition.x);
		ini.SetValue(lpszKey, VAL_FRAMEINFO_MINPOS_Y, wp.ptMinPosition.y);
		ini.SetValue(lpszKey, VAL_FRAMEINFO_MAXPOS_X, wp.ptMaxPosition.x);
		ini.SetValue(lpszKey, VAL_FRAMEINFO_MAXPOS_Y, wp.ptMaxPosition.y);
		ini.SetValue(lpszKey, VAL_FRAMEINFO_NORMALPOS_LEFT, wp.rcNormalPosition.left);
		ini.SetValue(lpszKey, VAL_FRAMEINFO_NORMALPOS_TOP, wp.rcNormalPosition.top);
		ini.SetValue(lpszKey, VAL_FRAMEINFO_NORMALPOS_RIGHT, wp.rcNormalPosition.right);
		ini.SetValue(lpszKey, VAL_FRAMEINFO_NORMALPOS_BOTTOM, wp.rcNormalPosition.bottom);

		ini.SetValue(lpszKey, VAL_FRAMEINFO_VIEWCLASS, m_wndSplitter.GetPane(0, 0)->GetRuntimeClass()->m_lpszClassName);
		ini.SetValue(lpszKey, VAL_FRAMEINFO_DOCPATH, ((CView*)m_wndSplitter.GetPane(0, 0))->GetDocument()->GetPathName());

		// storing column and row information
		int	cxCur, cxMin, cyCur, cyMin;
		
		for (nRow = 0; nRow < nRowCount; nRow++)
		{
			strKey.Format(KEY_ROWINFO, lpszKey, nRow);
			m_wndSplitter.GetRowInfo(nRow, cyCur, cyMin);

			ini.SetValue(strKey, VAL_ROWINFO_CYCUR, cyCur);
			ini.SetValue(strKey, VAL_ROWINFO_CYMIN, cyMin);
		}

		for (nColumn = 0; nColumn < nColumnCount; nColumn++)
		{
			strKey.Format(KEY_COLINFO, lpszKey, nColumn);
			m_wndSplitter.GetColumnInfo(nColumn, cxCur, cxMin);

			ini.SetValue(strKey, VAL_COLINFO_CXCUR, cxCur);
			ini.SetValue(strKey, VAL_COLINFO_CXMIN, cxMin);
		}

		for (nRow = 0; nRow < nRowCount; nRow++)
		{
			for (nColumn = 0; nColumn < nColumnCount; nColumn++)
			{
				strKey.Format(KEY_VIEWINFO, lpszKey, nRow, nColumn);
				((CCrystalEditViewEx*)m_wndSplitter.GetPane(nRow, nColumn))->Serialize(ini, strKey, bWrite);
			}
		}
	} //if
	else
	{
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// reading data
		WINDOWPLACEMENT	wp;
		CString					strKey, strClass, strDocPath;
		int							nRow, nRowCount = m_wndSplitter.GetRowCount();
		int							nColumn, nColumnCount = m_wndSplitter.GetColumnCount();

		nColumnCount = ini.GetValue(lpszKey, VAL_FRAMEINFO_COLCOUNT, 1);
		nRowCount = ini.GetValue(lpszKey, VAL_FRAMEINFO_ROWCOUNT, 1);
		wp.flags = (UINT)ini.GetValue(lpszKey, VAL_FRAMEINFO_FLAGS, 0);
		wp.showCmd = (UINT)ini.GetValue(lpszKey, VAL_FRAMEINFO_SHOWCMD, SW_SHOWNORMAL);
		wp.ptMinPosition.x = ini.GetValue(lpszKey, VAL_FRAMEINFO_MINPOS_X, 0);
		wp.ptMinPosition.y = ini.GetValue(lpszKey, VAL_FRAMEINFO_MINPOS_Y, 0);
		wp.ptMaxPosition.x = ini.GetValue(lpszKey, VAL_FRAMEINFO_MAXPOS_X, 0);
		wp.ptMaxPosition.y = ini.GetValue(lpszKey, VAL_FRAMEINFO_MAXPOS_Y, 0);
		wp.rcNormalPosition.left = ini.GetValue(lpszKey, VAL_FRAMEINFO_NORMALPOS_LEFT, 0);
		wp.rcNormalPosition.top = ini.GetValue(lpszKey, VAL_FRAMEINFO_NORMALPOS_TOP, 0);
		wp.rcNormalPosition.right = ini.GetValue(lpszKey, VAL_FRAMEINFO_NORMALPOS_RIGHT, 0);
		wp.rcNormalPosition.bottom = ini.GetValue(lpszKey, VAL_FRAMEINFO_NORMALPOS_BOTTOM, 0);
		strClass = ini.GetValue(lpszKey, VAL_FRAMEINFO_VIEWCLASS, _T(""));
		strDocPath = ini.GetValue(lpszKey, VAL_FRAMEINFO_DOCPATH, _T(""));

		if (strClass.IsEmpty())
			return FALSE;
		if (strDocPath.IsEmpty())
			return FALSE;

		// reading column and row information
		CCreateContext	cc;

		cc.m_pCurrentDoc = NULL;
		cc.m_pNewDocTemplate = theApp.GetLatexDocTemplate();
		cc.m_pLastView = NULL;
		cc.m_pCurrentFrame = NULL;
		if (strClass == _T("CLatexEdit"))
			cc.m_pNewViewClass = RUNTIME_CLASS(CLatexEdit);
		else
		{
			TRACE(_T("Unknown view type '%s'\n"), strClass);
			return FALSE;
		}

		// create view for each pane
		for (nRow = 0; nRow < nRowCount; nRow++)
		{
			for (nColumn = 0; nColumn < nColumnCount; nColumn++)
			{
				// generate key
				strKey.Format(KEY_VIEWINFO, lpszKey, nRow, nColumn);

				if (nRow == 0 && nColumn == 0)
				{
					// try to create and open document
					CDocument	*pDoc = theApp.GetLatexDocument(strDocPath);
					if (!pDoc)
						return FALSE;

					// create frame and restore window position
					if (!LoadFrame(IDR_LATEXTYPE, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, AfxGetMainWnd(), &cc))
					{
						TRACE0(_T("Window has not been created\n"));
						return FALSE;
					}

					SetWindowPlacement(&wp);

					// create view and attach it to the document
					CView			*pView = (CView*)m_wndSplitter.GetPane(0, 0);

					ASSERT(pView->IsKindOf(RUNTIME_CLASS(CView)));
					ASSERT_VALID(pView);

					SetActiveView(pView);
					pDoc->AddView(pView);
					pView->SendMessage(WM_INITIALUPDATE);

					m_wndSplitter.SetActivePane(0, 0);

					// set row and column info
					strKey.Format(KEY_ROWINFO, lpszKey, nRow);
					m_wndSplitter.SetRowInfo(nRow, 
						ini.GetValue(strKey, VAL_ROWINFO_CYCUR, 0),
						ini.GetValue(strKey, VAL_ROWINFO_CYMIN, 0));

					strKey.Format(KEY_COLINFO, lpszKey, nColumn);
					m_wndSplitter.SetColumnInfo(nColumn,
						ini.GetValue(strKey, VAL_COLINFO_CXCUR, 0),
						ini.GetValue(strKey, VAL_COLINFO_CXMIN, 0));
				}
				else
				{
					// split window
					if (nRow >= m_wndSplitter.GetRowCount())
					{
						strKey.Format(KEY_ROWINFO, lpszKey, nRow - 1);
						if (!m_wndSplitter.SplitRow(ini.GetValue(strKey, VAL_ROWINFO_CYCUR, 0)))
							continue;

						strKey.Format(KEY_ROWINFO, lpszKey, nRow);
						m_wndSplitter.SetRowInfo(nRow, 
							ini.GetValue(strKey, VAL_ROWINFO_CYCUR, 0),
							ini.GetValue(strKey, VAL_ROWINFO_CYMIN, 0));
					}
					if (nColumn >= m_wndSplitter.GetColumnCount())
					{
						strKey.Format(KEY_COLINFO, lpszKey, nColumn - 1);
						if (!m_wndSplitter.SplitColumn(ini.GetValue(strKey, VAL_COLINFO_CXCUR, 0)))
							continue;

						strKey.Format(KEY_COLINFO, lpszKey, nColumn);
						m_wndSplitter.SetColumnInfo(nColumn,
							ini.GetValue(strKey, VAL_COLINFO_CXCUR, 0),
							ini.GetValue(strKey, VAL_COLINFO_CXMIN, 0));
					}
				}
			}
		}

		// serialize view information
		for (nRow = 0; nRow < m_wndSplitter.GetRowCount(); nRow++)
		{
			for (nColumn = 0; nColumn < m_wndSplitter.GetColumnCount(); nColumn++)
			{
				// check view type
				CWnd	*pWnd = m_wndSplitter.GetPane(nRow, nColumn);

				ASSERT(pWnd && IsWindow(pWnd->m_hWnd));
				ASSERT(pWnd->IsKindOf(RUNTIME_CLASS(CCrystalEditViewEx)));
				if (!pWnd || !IsWindow(pWnd->m_hWnd) || !pWnd->IsKindOf(RUNTIME_CLASS(CCrystalEditViewEx)))
					continue;

				// serialize view information
				strKey.Format(KEY_VIEWINFO, lpszKey, nRow, nColumn);
				if (!((CCrystalEditViewEx*)pWnd)->Serialize(ini, strKey, bWrite))
					m_wndSplitter.DeleteView(nRow, nColumn);
			}
		}
	} // if

	return TRUE;
}

CString CChildFrame::GetPathNameOfDocument()
{
	return (((CView*)m_wndSplitter.GetPane(0, 0))->GetDocument()->GetPathName());
}
