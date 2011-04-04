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

#include "Configuration.h"
#include "ChildFrm.h"
#include "LaTeXView.h"
#include "MainFrm.h"
#include "TeXnicCenter.h"

HBITMAP CreateBitmapFromWindow(CWnd* wnd)
{
	ENSURE_ARG(wnd);

	CWindowDC wdc(wnd);

	CDC dc;
	dc.CreateCompatibleDC(&wdc);

	CRect rect;
	wnd->GetWindowRect(&rect);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&wdc, rect.Width(), rect.Height());

	CBitmap* oldBmp = dc.SelectObject(&bmp);

	wnd->SendMessage(WM_PRINT, reinterpret_cast<WPARAM>(dc.m_hDC), PRF_CHILDREN | PRF_CLIENT);

	dc.SelectObject(oldBmp);

	return reinterpret_cast<HBITMAP>(bmp.Detach());
}


IMPLEMENT_DYNCREATE(CChildFrame,CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame,CMDIChildWndEx)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_WM_MDIACTIVATE()
END_MESSAGE_MAP()


CChildFrame::CChildFrame()
{
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs,
                                 CCreateContext* pContext)
{
	////BOOL result = __super::OnCreateClient(lpcs,pContext);
	return __super::OnCreateClient(lpcs,pContext);
	//// create splitter and add to window manager
	//if (m_wndSplitter.Create(this,
	//                         2,1,
	//                         CSize(10,10),
	//                         pContext))
	//{
	//	//if (result)
	//	//    theApp.m_pMDIFrameManager->AddChildFrame(this);
	//	//
	//	return TRUE;
	//	//return result;
	//}
	//else
	//	return FALSE;
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CMDIChildWndEx::PreCreateWindow(cs))
		return FALSE;

	cs.style |= FWS_PREFIXTITLE;

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CChildFrame Diagnose

#ifdef _DEBUG

void CChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame Nachrichten-Handler

void CChildFrame::ActivateFrame(int nCmdShow)
{
	if (CConfiguration::GetInstance()->m_bOpenDocWndMaximized)
		CMDIChildWndEx::ActivateFrame(SW_SHOWMAXIMIZED);
	else
		CMDIChildWndEx::ActivateFrame(nCmdShow);
}

void CChildFrame::OnDestroy()
{
	// if this was the last open window,
	// then remember, if the next window should be opened maximized
	if (!GetNextWindow(GW_HWNDNEXT) && !GetNextWindow(GW_HWNDPREV))
		CConfiguration::GetInstance()->m_bOpenDocWndMaximized = IsZoomed() != 0;

	CMainFrame* m = dynamic_cast<CMainFrame*>(GetTopLevelFrame());
	ENSURE(m);

	m->UnregisterChildFrame(this);

	CMDIChildWndEx::OnDestroy();
}

void CChildFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	CWnd* w = GetWindow(GW_CHILD);

	while (w && !w->IsKindOf(RUNTIME_CLASS(CodeView)))
		w = w->GetWindow(GW_HWNDNEXT);

	if (w)
		w->SetFocus();
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

bool CChildFrame::Serialize( CIniFile &ini, LPCTSTR lpszKey, bool write )
{
	const CString strBaseDir = CPathTool::GetDirectory(ini.GetPath());

	if (write)
	{
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// writing data
		WINDOWPLACEMENT wp;
		CString strKey;
		//int nRow,nRowCount = m_wndSplitter.GetRowCount();
		//int nColumn,nColumnCount = m_wndSplitter.GetColumnCount();

		GetWindowPlacement(&wp);

		//ini.SetValue(lpszKey,VAL_FRAMEINFO_COLCOUNT,nColumnCount);
		//ini.SetValue(lpszKey,VAL_FRAMEINFO_ROWCOUNT,nRowCount);
		ini.SetValue(lpszKey,VAL_FRAMEINFO_FLAGS,(int) wp.flags);
		ini.SetValue(lpszKey,VAL_FRAMEINFO_SHOWCMD,(int) wp.showCmd);
		ini.SetValue(lpszKey,VAL_FRAMEINFO_MINPOS_X,wp.ptMinPosition.x);
		ini.SetValue(lpszKey,VAL_FRAMEINFO_MINPOS_Y,wp.ptMinPosition.y);
		ini.SetValue(lpszKey,VAL_FRAMEINFO_MAXPOS_X,wp.ptMaxPosition.x);
		ini.SetValue(lpszKey,VAL_FRAMEINFO_MAXPOS_Y,wp.ptMaxPosition.y);
		ini.SetValue(lpszKey,VAL_FRAMEINFO_NORMALPOS_LEFT,wp.rcNormalPosition.left);
		ini.SetValue(lpszKey,VAL_FRAMEINFO_NORMALPOS_TOP,wp.rcNormalPosition.top);
		ini.SetValue(lpszKey,VAL_FRAMEINFO_NORMALPOS_RIGHT,wp.rcNormalPosition.right);
		ini.SetValue(lpszKey,VAL_FRAMEINFO_NORMALPOS_BOTTOM,wp.rcNormalPosition.bottom);

		USES_CONVERSION;

		ini.SetValue(lpszKey,VAL_FRAMEINFO_VIEWCLASS,
		             A2CT(GetWindow(GW_CHILD)->GetRuntimeClass()->m_lpszClassName));
		ini.SetValue(lpszKey,VAL_FRAMEINFO_DOCPATH,
		             CPathTool::GetRelativePath(strBaseDir,GetPathNameOfDocument()));

#if 0
		// storing column and row information
		int cxCur,cxMin,cyCur,cyMin;

		for (nRow = 0; nRow < nRowCount; nRow++)
		{
			strKey.Format(KEY_ROWINFO,lpszKey,nRow);
			m_wndSplitter.GetRowInfo(nRow,cyCur,cyMin);

			ini.SetValue(strKey,VAL_ROWINFO_CYCUR,cyCur);
			ini.SetValue(strKey,VAL_ROWINFO_CYMIN,cyMin);
		}

		for (nColumn = 0; nColumn < nColumnCount; nColumn++)
		{
			strKey.Format(KEY_COLINFO,lpszKey,nColumn);
			m_wndSplitter.GetColumnInfo(nColumn,cxCur,cxMin);

			ini.SetValue(strKey,VAL_COLINFO_CXCUR,cxCur);
			ini.SetValue(strKey,VAL_COLINFO_CXMIN,cxMin);
		}

#endif // 0

		strKey.Format(KEY_VIEWINFO,lpszKey,0,0);//nRow,nColumn);
		static_cast<CodeView*>(GetWindow(GW_CHILD))->Serialize(ini,strKey,write);

	} //if
	else
	{
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// reading data
		WINDOWPLACEMENT wp;
		CString strKey,strClass,strDocPath;

		wp.flags = (UINT) ini.GetValue(lpszKey,VAL_FRAMEINFO_FLAGS,0);
		wp.showCmd = (UINT) ini.GetValue(lpszKey,VAL_FRAMEINFO_SHOWCMD,SW_SHOWNORMAL);
		wp.ptMinPosition.x = ini.GetValue(lpszKey,VAL_FRAMEINFO_MINPOS_X,0);
		wp.ptMinPosition.y = ini.GetValue(lpszKey,VAL_FRAMEINFO_MINPOS_Y,0);
		wp.ptMaxPosition.x = ini.GetValue(lpszKey,VAL_FRAMEINFO_MAXPOS_X,0);
		wp.ptMaxPosition.y = ini.GetValue(lpszKey,VAL_FRAMEINFO_MAXPOS_Y,0);
		wp.rcNormalPosition.left = ini.GetValue(lpszKey,VAL_FRAMEINFO_NORMALPOS_LEFT,0);
		wp.rcNormalPosition.top = ini.GetValue(lpszKey,VAL_FRAMEINFO_NORMALPOS_TOP,0);
		wp.rcNormalPosition.right = ini.GetValue(lpszKey,VAL_FRAMEINFO_NORMALPOS_RIGHT,0);
		wp.rcNormalPosition.bottom = ini.GetValue(lpszKey,VAL_FRAMEINFO_NORMALPOS_BOTTOM,0);
	
		if (!theApp.GetShowMDITabs())
			SetWindowPlacement(&wp);

		// check view type
		CWnd *pWnd = GetWindow(GW_CHILD);

		ASSERT(pWnd && IsWindow(pWnd->m_hWnd));
		ASSERT(pWnd->IsKindOf(RUNTIME_CLASS(CodeView)));

		strKey.Format(KEY_VIEWINFO,lpszKey,0,0);

		if (CodeView* view = dynamic_cast<CodeView*>(pWnd))
			view->Serialize(ini,strKey,write);
	} // if

	return TRUE;
}

CString CChildFrame::GetPathNameOfDocument()
{
	CView* pView = dynamic_cast<CView*> (GetActiveView());

	if (pView)
	{
		CDocument* pDoc = pView->GetDocument();

		if (pDoc)
		{
			return pDoc->GetPathName();
		}
	}

	return CString();
}


int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	CMainFrame* m = dynamic_cast<CMainFrame*>(GetTopLevelFrame());
	ENSURE(m);
	m->RegisterChildFrame(this);

	return 0;
}


void CChildFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
	CMDIChildWndEx::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);

	// Prevent the recently used frame from being updated while closing the frame
	if (bActivate)
	{
		CChildFrame* frame = dynamic_cast<CChildFrame*>(pActivateWnd);

		//Make this child the most recently used one.
		//But only, if Ctrl key is not pressed.
		if (frame && !frame->m_bToBeDestroyed && GetKeyState(VK_CONTROL) >= 0)
		{
			CMainFrame* m = dynamic_cast<CMainFrame*>(GetTopLevelFrame());
			ENSURE(m);

			m->SetRecentlyUsedChildFrame(frame);
		}
	}
}
