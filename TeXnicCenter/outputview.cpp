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
#include "OutputView.h"
#include "configuration.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// class COutputView
//-------------------------------------------------------------------

IMPLEMENT_DYNCREATE(COutputView, CListCtrl)

BEGIN_MESSAGE_MAP(COutputView, CListCtrl)
	ON_MESSAGE(OPW_ADD_LINE, OnAddLine)
	ON_MESSAGE(OPW_RESET, OnReset)
	//{{AFX_MSG_MAP(COutputView)
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SHOWWINDOW()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	ON_WM_SYSCOLORCHANGE()
END_MESSAGE_MAP()


COutputView::COutputView()
:	CListCtrl(),
	m_bInitialized(FALSE),
	m_pOutputDoc(NULL)
{
	// disable margin
	//SetSelectionMargin( FALSE );
	
	//m_textBuffer.InitNew();
	//m_textBuffer.AddView( this );
	//m_nLineCount  = 0;

	// create dummy font, so that m_font.DestroyObject() wont make any
	// problems
	m_font.CreatePointFont(100, _T("Courier New"));
}


COutputView::~COutputView()
{
	//m_textBuffer.FreeAll();
}


BOOL COutputView::Create(const RECT &rect, CWnd *pwndParent)
{
	ASSERT(pwndParent && IsWindow(pwndParent->m_hWnd));

	return CListCtrl::Create(
		WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_NOCOLUMNHEADER | LVS_SHOWSELALWAYS | LVS_SINGLESEL, 
		rect, pwndParent, 0);
}


void COutputView::AttachDoc(COutputDoc *pDocument)
{
	m_pOutputDoc = pDocument;
	m_bInitialized = TRUE;
}


COutputDoc *COutputView::GetDocument() const
{
	ASSERT(m_pOutputDoc->IsKindOf(RUNTIME_CLASS(COutputDoc)));
	return (COutputDoc*)m_pOutputDoc;
}


int COutputView::GetLineCount() const
{
	return GetItemCount();
}


CImageList *COutputView::SetImageList(CImageList *pImageList)
{
	return CListCtrl::SetImageList(pImageList, LVSIL_SMALL);
}


CImageList *COutputView::GetImageList() const
{
	return CListCtrl::GetImageList(LVSIL_SMALL);
}


/*
DWORD COutputView::ParseLine( DWORD dwCookie, int nLineIndex, 
														 CListCtrl::TEXTBLOCK *pBuf, int &nActualItems )
{
	if( pBuf )
	{
		pBuf[nActualItems].m_nCharPos = 0;
		pBuf[nActualItems].m_nColorIndex = COLORINDEX_NORMALTEXT;
	}

	return 0;
}
*/


LONG COutputView::OnReset(UINT wParam, LONG lParam)
{
	ResetView();
	return 0;
}


void COutputView::ResetView()
{
	/*
	if( GetLineCount() > 1 || GetLineLength( 0 ) > 0 )
		m_textBuffer.DeleteText( 
			this, 0, 0, GetLineCount() - 1, 
			GetLineLength( GetLineCount() - 1 )? GetLineLength( GetLineCount() - 1 ) - 1 : 0 );

	m_bDoNotScroll = FALSE;
	m_nLineCount = 0;
	*/
	DeleteAllItems();
}


LONG COutputView::OnAddLine(UINT wParam, LONG lParam)
{
	AddLine((LPCTSTR)wParam);
	return 0L;
}


void COutputView::AddLine(LPCTSTR lpszLine, int nImage /*= -1*/, int nIndent /*= 0*/)
{
	/*
	int				nEndLine, nEndPos;
	CString		strLine = lpszLine;

	m_textBuffer.InsertText( this, GetLineCount() - 1, 0, strLine + _T("\r\n"), 
		nEndLine, nEndPos );
	RecalcHorzScrollBar();

	// Ensure that new line is visible
	if( !m_bDoNotScroll )
		EnsureVisible( CPoint( 0, GetLineCount() -1 ) );
	*/

	int		nItem = GetItemCount();
	CString strLine(EnsureStringReadable(lpszLine));
	LVITEM	lvItem;

	lvItem.mask = LVIF_INDENT | LVIF_TEXT;
	lvItem.iItem = nItem;
	lvItem.iSubItem = 0;
	//lvItem.state
	lvItem.stateMask = 0;
	lvItem.iIndent = nIndent;
	lvItem.cchTextMax = strLine.GetLength();
	lvItem.pszText = strLine.GetBuffer(0);
	lvItem.iImage = nImage;
	//lvItem.lParam

	if ( nImage != -1 )
		lvItem.mask |= LVIF_IMAGE;

	nItem = InsertItem(&lvItem);
	strLine.ReleaseBuffer();

	SetColumnWidth(0, LVSCW_AUTOSIZE);

	// first line? then select
	if (nItem == 0)
		SelectLine(nItem);
	else
	{
		// was previous line the selected one?
		// => then make the new line the selected one and ensure, that it
		//    is visible,
		// => otherwise don't scroll anywhere
		POSITION	pos = GetFirstSelectedItemPosition();
		if (pos && GetNextSelectedItem(pos) == nItem-1)
			SelectLine(nItem);
	}
}


BOOL COutputView::SetLineImage(int nLine, int nImage)
{
	LVITEM	item;

	item.iItem = nLine;
	item.iSubItem = 0;
	item.mask = LVIF_IMAGE;
	item.iImage = nImage;

	return CListCtrl::SetItem(&item);
}


void COutputView::SelectLine(int nLine, BOOL bPlaceAtTop /*= FALSE*/)
{
	//nLine--; // item-index is zero-based
	ASSERT(nLine >= 0 && nLine < GetLineCount());
	/*
	m_textBuffer.SetLineFlag( nLine, LF_BREAKPOINT, TRUE );
	EnsureVisible( CPoint( 0, nLine ) );
	*/

	SetItemState(nLine, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
	EnsureVisible(nLine, FALSE);

	if (bPlaceAtTop)
	{
		CRect	rect;
		GetItemRect(nLine, rect, LVIR_BOUNDS);
		Scroll(CSize(0, rect.top));
	}
}


/*
CCrystalTextBuffer *COutputView::LocateTextBuffer()
{
	return &m_textBuffer;
}
*/


int COutputView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// set font
	ApplyFont();

	// modify style
	ModifyStyle(0xFFFFFFFF, 
		WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_NOCOLUMNHEADER | LVS_SHOWSELALWAYS | LVS_SINGLESEL);
	InsertColumn(0, _T(""));

	//AttachToBuffer();
	
	return 0;
}


BOOL COutputView::PreCreateWindow(CREATESTRUCT& cs) 
{
	return CListCtrl::PreCreateWindow(cs);
}


void COutputView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// activate message on the line that was double clicked on
	/*
	CPoint	pos = GetCursorPos();
	
	GetDocument()->ActivateMessageByOutputLine( pos.y + 1 );	// convert zero-based to one-based
	*/

	POSITION	pos = GetFirstSelectedItemPosition();
	if (!pos)
		return;

	GetDocument()->ActivateMessageByOutputLine(GetNextSelectedItem(pos));
}



void COutputView::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CListCtrl::OnShowWindow(bShow, nStatus);
	
	if (bShow && GetDocument())
		GetDocument()->OnOutputViewActivated( this );
}


void COutputView::DoOnUpdate(COutputView *pSender, LPARAM lHint, CObject *pHint)
{
	OnUpdate(pSender, lHint, pHint);
}


void COutputView::OnUpdate(COutputView *pSender, LPARAM lHint, CObject *pHint)
{}


void COutputView::ApplyFont()
{
	m_font.DeleteObject();
	m_font.CreateFontIndirect(&g_configuration.m_fontOutput);
	SetFont(&m_font);
}


CString COutputView::EnsureStringReadable(LPCTSTR lpszString)
{
	CString	strResult;

	for (int i = 0; i < _tcslen(lpszString); i++)
	{
		switch (lpszString[i])
		{
		case _T('\t'):
			strResult+= _T("  ");
			break;

		default:
			strResult+= lpszString[i];
		}
	}

	return strResult;
}


void COutputView::OnSysColorChange()
{
	CListCtrl::OnSysColorChange();

	ApplyFont();
}


#ifdef _DEBUG
void COutputView::AssertValid() const
{
	CListCtrl::AssertValid();
}

void COutputView::Dump(CDumpContext& dc) const
{
	CListCtrl::Dump(dc);
}
#endif //_DEBUG
