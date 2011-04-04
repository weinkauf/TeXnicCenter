// AutoCompleteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AutoCompleteDlg.h"
#include "AutoCompleteListbox.h"
#include "MainFrm.h"
#include "TeXnicCenter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CAutoCompleteDlg,CWnd)
	//{{AFX_MSG_MAP(CAutoCompleteDlg)
	ON_WM_KILLFOCUS()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_WM_NCHITTEST()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_MESSAGE(WM_FLOATSTATUS,&CAutoCompleteDlg::OnFloatStatus)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoCompleteDlg

CAutoCompleteDlg::CAutoCompleteDlg(CStyleFileContainer *sfc,CWnd* pParent)
	: m_Box(new CAutoCompleteListBox)
{
	ASSERT(sfc != NULL);

	m_Listener = NULL;
	m_Container = sfc;
	m_Visible = FALSE;

	//Create the window out of sight (otherwise it would pop up in the middle of nowhere for a short moment)
	if (!Create(pParent))
	{
		TRACE0("Window creation (CAutoCompleteDlg) failed!\n");
	}
}

CAutoCompleteDlg::~CAutoCompleteDlg()
{
	DestroyWindow();
}

BOOL CAutoCompleteDlg::Create(LPCTSTR /*lpszClassName*/, LPCTSTR /*lpszWindowName*/, DWORD dwStyle,const RECT& rect, CWnd* pParentWnd, UINT /*nID*/, CCreateContext* pContext)
{
	dwStyle = WS_POPUP|WS_DLGFRAME|WS_CLIPCHILDREN;

	return CWnd::CreateEx(WS_EX_TOOLWINDOW,
	                      AfxRegisterWndClass(
	                          CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS,
	                          theApp.LoadStandardCursor(IDC_ARROW)),//GetSysColorBrush(COLOR_WINDOW)),
	                      _T("AutoCompleteBox"),
	                      dwStyle,
	                      rect,
	                      pParentWnd,
	                      0,
	                      pContext);
}

BOOL CAutoCompleteDlg::Create(CWnd* pParentWnd)
{
	return Create(0,0,0,CRect(0,0,0,0),pParentWnd,ID_AUTOCOMPLETE,0);
}

/////////////////////////////////////////////////////////////////////////////
// CAutoCompleteDlg message handlers

BOOL CAutoCompleteDlg::InitWithKeyword(const CString &keyword)
{
	if (keyword.GetLength() < MINIMUM_KEYWORD_LENGTH)
	{
		return FALSE;
	}

	map.RemoveAll();

	std::tr1::shared_ptr<CObject> c;
	CString key;
	int idx;
	

	m_Container->GetAllPossibleItems(keyword,_T(""),map);

	if (map.GetCount() == 0)   /* Nothing to do */
	{
		m_CurrentKeyword.Empty();
		return FALSE;
	}

	m_CurrentKeyword = keyword;

	SharedLaTeXCommandPtr lc;

	/* Just on element -> Notify caller immediately */
	if (map.GetCount() == 1)
	{
		POSITION pos = map.GetStartPosition();
		map.GetNextAssoc(pos,key, c);

		lc = std::tr1::dynamic_pointer_cast<CLaTeXCommand>(c);

		if (lc && m_Listener != NULL) m_Listener->OnACCommandSelect(lc.get());

		return FALSE;
	}

	/* iterate through map */
	if (!::IsWindow(this->GetSafeHwnd()))   // provide safe exit
	{
		TRACE0("ERROR: Not a window\n");
		if (m_Listener != NULL)
		{
			m_Listener->OnACCommandCancelled();
		}
	}

	m_Box->ResetContent();
	m_Box->InitStorage(map.GetCount(),keyword.GetLength() * 2); /* prepare memory */


	POSITION pos = map.GetStartPosition();
	while (pos != NULL)   /* fill list with items */
	{
		map.GetNextAssoc(pos,key, c);

		lc = std::tr1::dynamic_pointer_cast<CLaTeXCommand>(c);
		idx = m_Box->AddString(lc->ToLaTeX());

		int ret = m_Box->SetItemDataPtr(idx,lc.get()); // sync pointer with string
		if (ret == LB_ERR)
		{
			TRACE2("Error in SetItemDataPtr: %d (idx = %d)\n",::GetLastError(),idx);
		}
	}
	ASSERT(map.GetCount() == m_Box->GetCount());

	return TRUE;
}

/**
 * Attaches a listener which will receive all events of the dialog.
 */
void CAutoCompleteDlg::SetListener(CAutoCompleteListener *listener)
{
	m_Listener = listener;
}

/* Select best entry for given keyword. returns true, if selection was updated, otherwise false. */
BOOL CAutoCompleteDlg::UpdateSelection(CString &newKeyword)
{
	int x = m_Box->FindString(-1,newKeyword);

	if (x != LB_ERR)
	{
		m_Box->SetCurSel(x);
		m_Box->SetTopIndex(x);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CAutoCompleteDlg::IsCancelChar(WORD theChar)
{
	return theChar == VK_SPACE;
}

/* User has comfirmed selection.  */
void CAutoCompleteDlg::ApplySelection()
{
	int n = m_Box->GetCurSel();
	CString selectedItem;
	if (n != LB_ERR)
	{
		m_Box->GetText(n,selectedItem);

		CLaTeXCommand *lc = (CLaTeXCommand *) m_Box->GetItemData(n);
		ASSERT(lc != NULL);
		if (m_Listener != NULL)
		{
			m_Listener->OnACCommandSelect(lc);
		}
	}
	else
	{
		TRACE1("Error occurred during ApplySelection, error code %d",::GetLastError());
	}

	ShowWindow(SW_HIDE);
}

/* User cancelled completion */
void CAutoCompleteDlg::CancelSelection()
{
	if (m_Listener != NULL)
	{
		m_Listener->OnACCommandCancelled();

	}

	ShowWindow(SW_HIDE);
}

/**
 Moves current selection by delta (e. g. delta=-1 moves selection to the previous item, delta=1 to the next
 */
void CAutoCompleteDlg::MoveSelection(int delta)
{
	int nCount = m_Box->GetCount();
	int curSel = m_Box->GetCurSel();
	int newSel = curSel + delta;

	/* Check bounds */
	if (newSel > nCount - 1)
	{
		newSel = nCount - 1;
	}

	if (newSel < 0)
	{
		newSel = 0;
	}
	SetCurSel(newSel);
}

/* User wants help */
void CAutoCompleteDlg::Help()
{
	CString selectedItem;
	int n = m_Box->GetCurSel();

	if (n != LB_ERR)
	{
		m_Box->GetText(n,selectedItem);
		if (m_Listener != NULL)
		{
			m_Listener->OnACHelp(selectedItem);
		}
	}
	else
	{
		TRACE1("Error occurred during ApplySelection, error code %d",::GetLastError());
	}
}

void CAutoCompleteDlg::SetCurSel(int sel)
{
	m_Box->SetCurSel(sel);
}

/* Intercept keyboard input */
BOOL CAutoCompleteDlg::PreTranslateMessage(MSG* pMsg)
{
	if (!IsWindowVisible())   /* are we sleeping? */
	{
		CWnd *wnd = GetParent(); /* pass msg to parent */
		return wnd->PreTranslateMessage(pMsg);
	}

	switch (pMsg->message)
	{
		case WM_KEYUP:
			switch (pMsg->wParam)
			{
				case VK_CONTROL:
					MakeTransparent(0);
					return 1;
			}
			break;

		case WM_KEYDOWN:
			switch (pMsg->wParam)
			{
				case VK_CONTROL:
					MakeTransparent(75);
					return 1;

				case VK_TAB:
				case VK_RETURN:
					ApplySelection();
					return 1;
				case VK_ESCAPE:
					CancelSelection();
					return 1;
					/* list navigation */
				case VK_DOWN:
				case VK_RIGHT:
					MoveSelection(1);
					return 1;
				case VK_UP:
				case VK_LEFT:
					MoveSelection(-1);
					return 1;
				case VK_PRIOR:
					MoveSelection(-5);
					return 1;
				case VK_NEXT:
					MoveSelection(5);
					return 1;
				case VK_HOME:
					m_Box->SetCurSel(0);
					return 1;
				case VK_END:
					m_Box->SetCurSel(m_Box->GetCount() - 1);
					return 1;
					/* Help */
				case VK_HELP:
				case VK_F1:
					Help();
					return 1;
				case VK_SPACE:
					break;
					/* Delete last TCHAR */
				case VK_BACK:
					if (m_CurrentKeyword.GetLength() > MINIMUM_KEYWORD_LENGTH)
					{
						m_CurrentKeyword.Delete(m_CurrentKeyword.GetLength() - 1);
						if (m_Listener != NULL)
						{
							m_Listener->OnACBackspace();
						}
						UpdateSelection(m_CurrentKeyword);
						/* drop box, if nothing is left */
						if (m_CurrentKeyword.GetLength() <= MINIMUM_KEYWORD_LENGTH)
						{
							CancelSelection();
						}
					}
					return 1;
				case VK_DELETE:
				case VK_INSERT:
					/* ignore */
					return 1;
			}
			break;
		case WM_CHAR:
			if (IsWindowVisible())
			{
				if (!IsCancelChar(pMsg->wParam))   /* check, if TCHAR interrupts keyword (e. g. space) */
				{
					m_CurrentKeyword += (TCHAR) pMsg->wParam;
					UpdateSelection(m_CurrentKeyword);
				}
				else
				{
					if (m_Listener != NULL)
					{
						m_Listener->OnACChar(pMsg->wParam,LOWORD(pMsg->lParam),HIWORD(pMsg->lParam));
					}
					CancelSelection();
				}
				if (m_Listener != NULL)
				{
					m_Listener->OnACChar(pMsg->wParam,LOWORD(pMsg->lParam),HIWORD(pMsg->lParam));
				}
			}
			break;
	}

	return CWnd::PreTranslateMessage(pMsg);
}

void CAutoCompleteDlg::OnKillFocus(CWnd* pNewWnd)
{
	if (!IsChild(pNewWnd))
	{
		//TRACE("==> KillFocus, visible = %d\n", IsWindowVisible());
		CancelSelection();
		//TRACE("<== KillFocus\n");
	}
	//else {
	//    TRACE("My window -> not KillFocus\n");
	//}
}

void CAutoCompleteDlg::OnShowWindow(BOOL bShow,UINT nStatus)
{
	//Record the information; so that we know our status
	if (bShow != m_Visible)
	{
		//TRACE("Show window show = %d, status = %d, old visible = %d\n", bShow, nStatus, m_Visible);
		CWnd::OnShowWindow(bShow,nStatus);
		m_Visible = bShow;
	}
}

void CAutoCompleteDlg::OnLButtonDblClk(UINT /*nFlags*/, CPoint /*point*/)
{
	ApplySelection();
}

void CAutoCompleteDlg::OnSize(UINT nType,int cx,int cy)
{
	//Resize the list control contained in the view to
	//fill the entire view when the view's window is resized.

	//Call base class.
	CWnd::OnSize(nType,cx,cy);

	//Resize list to fill the whole view.
	if (::IsWindow(m_Box->GetSafeHwnd())) m_Box->MoveWindow(0,0,cx,cy);
}

void CAutoCompleteDlg::AdjustSizeAndPosition(const int EditorLineHeight)
{
	//Get the size of the largest entry
	const CSize ItemSize = m_Box->GetLargestItemTextExtent();

	//Measure the size of the borders
	CRect WinRect;
	GetWindowRect(&WinRect);

	//Get the desktop width.
	//Since we might have several desktops, we first need to find the right monitor.
	CRect ScreenRect;
	CMainFrame* pFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	if (pFrame && pFrame->GetActiveFrame())
	{
		//Get the rect of the active edit window.
		CRect ParentRect;
		pFrame->GetActiveFrame()->GetWindowRect(&ParentRect);

		//Get the nearest monitor to the parent rect.
		HMONITOR hMonitor;
		hMonitor = MonitorFromRect(ParentRect, MONITOR_DEFAULTTONEAREST);

		//Get the work area of the monitor.
		MONITORINFO mi;
		mi.cbSize = sizeof(mi);
		GetMonitorInfo(hMonitor, &mi);
		ScreenRect = mi.rcWork;
	}
	else
	{
		//If we don't have a parent, we just take the primary monitor.
		if (!::SystemParametersInfo(SPI_GETWORKAREA, 0, &ScreenRect, 0))
		{
			CWnd::GetDesktopWindow()->GetWindowRect(&ScreenRect); // Just in case SPI fails
		}
	}

	//Both Width and Height should not be smaller than a certain size and not larger than the screen
	const int MinWidth = 0; // Fit the area, no min width needed
	const int MaxWidth = static_cast<int>(0.9 * ScreenRect.Width());
	const int MinHeight = 0;//100;
	const int SpaceToTop = WinRect.top - EditorLineHeight;
	const int SpaceToBottom = ScreenRect.bottom - WinRect.top;
	const int MaxHeight = (SpaceToBottom > SpaceToTop) ? SpaceToBottom : SpaceToTop;


	//Width - add some pixels for the left icons and some extra - don't ask, this is magick
	int WantedWidth = ItemSize.cx;
	int RubberWidth = 0;
	// --
	if (WantedWidth > MaxWidth) WantedWidth = MaxWidth;
	if (WantedWidth < MinWidth)
	{
		//Normalize the width, but put the rest into rubber
		RubberWidth = MinWidth - WantedWidth; //This rubber is actually white space
		WantedWidth = MinWidth;
	}
	else
	{
		//We have a large box and a part of it can go away.
		RubberWidth = static_cast<int>(0.1 * WantedWidth);
	}

	//Height - depends on the number of items
	int WantedHeight = m_Box->GetCount() * ItemSize.cy;
	int RubberHeight = 0;
	// --
	if (WantedHeight > MaxHeight) WantedHeight = MaxHeight;
	if (WantedHeight < MinHeight)
	{
		//Normalize the height, but put the rest into rubber
		RubberHeight = MinHeight - WantedHeight; //This rubber is actually white space
		WantedHeight = MinHeight;
	}
	else
	{
		//We have a large box and a part of it can go away.
		RubberHeight = static_cast<int>(0.333 * WantedHeight);

		//We want at least 8 items to be visible. Do not rubber below 8.
		const int Eight = 8 * ItemSize.cy;
		if (WantedHeight - RubberHeight < Eight) RubberHeight = WantedHeight - Eight;
		if (RubberHeight < 0) RubberHeight = 0;
	}


	//Adjust the rectangle accordingly
	WinRect.right = WinRect.left + WantedWidth;
	WinRect.bottom = WinRect.top + WantedHeight;

	AdjustWindowRect(&WinRect,GetStyle(),FALSE);

	//Check, if final window overlaps with desktop
	// - bottom
	if (WinRect.bottom > ScreenRect.bottom)
	{
		const int Diff = WinRect.bottom - ScreenRect.bottom;

		//Make smaller or re-position?
		if (Diff <= RubberHeight)
		{
			WinRect.bottom = ScreenRect.bottom;
		}
		else
		{
			//Is there more space on the other side, i.e., to the top?
			if (WinRect.top - EditorLineHeight > ScreenRect.bottom - WinRect.top)
			{
				//Yes, there we can have more fun
				WinRect.OffsetRect(0,-(EditorLineHeight + WinRect.Height()));
			}
			//No, more space at the bottom
			// ==> Leave Rect as is and cut later
			//else { }
		}
	}

	// Do we need a vertical scrollbar?
	if (m_Box->GetCount() * ItemSize.cy > WinRect.Height())
		WinRect.right += ::GetSystemMetrics(SM_CXVSCROLL) * 2;

	// left
	if (WinRect.left < ScreenRect.left)
		WinRect.OffsetRect(ScreenRect.left - WinRect.left,0);

	// - right
	if (WinRect.right > ScreenRect.right)
	{
		const int Diff = WinRect.right - ScreenRect.right;

		//Make smaller or re-position?
		if (Diff <= RubberWidth)
		{
			WinRect.right = ScreenRect.right;
		}
		else
		{
			//Is there more space on the other side, i.e., to the left?
			if (WinRect.left > ScreenRect.right - WinRect.left)
			{
				//Yes, there we can have more fun
				WinRect.OffsetRect(-Diff,0);
			}
			//No, more space to the right
			// ==> Leave Rect as is and cut later
			// else { }
		}
	}

	//Finally: intersect with screen
	CRect FinalRect;
	FinalRect.IntersectRect(WinRect,ScreenRect);

	//Set it (yes, this works only with screen coords. do not ask me why)
	SetWindowPos(NULL,FinalRect.left,FinalRect.top,FinalRect.Width(),FinalRect.Height(),
	             SWP_NOZORDER|SWP_NOACTIVATE);
}

int CAutoCompleteDlg::GetMinimumKeywordLength()
{
	return MINIMUM_KEYWORD_LENGTH;
}

LRESULT CAutoCompleteDlg::OnNcHitTest(CPoint point)
{
	LRESULT result = CWnd::OnNcHitTest(point);

	if (result == HTBORDER)
	{
		CRect wnd;
		GetWindowRect(&wnd);

		CRect client;
		m_Box->GetWindowRect(&client);

		if (CRect(wnd.left,client.top,client.left,client.bottom).PtInRect(point))
		{
			// Left border
			result = HTLEFT;
		}
		else if (CRect(client.right,client.top,wnd.right,client.bottom).PtInRect(point))
		{
			// Right border
			result = HTRIGHT;
		}
		else if (CRect(client.left,wnd.top,client.right,client.top).PtInRect(point))
		{
			// Top border
			result = HTTOP;
		}
		else if (CRect(client.left,client.bottom,client.right,wnd.bottom).PtInRect(point))
		{
			// Bottom border
			result = HTBOTTOM;
		}
		// and now the edges
		else if (point.x < client.CenterPoint().x)
		{
			if (point.y < client.CenterPoint().y)
				result = HTTOPLEFT;
			else
				result = HTBOTTOMLEFT;
		}
		else
		{
			if (point.y < client.CenterPoint().y)
				result = HTTOPRIGHT;
			else
				result = HTBOTTOMRIGHT;
		}
	}

	return result;
}

BOOL CAutoCompleteDlg::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

int CAutoCompleteDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rc;
	GetClientRect(&rc);

	if (!m_Box->CWnd::CreateEx(WS_EX_WINDOWEDGE,WC_LISTBOX,0,
		WS_CHILD | WS_VISIBLE | (LBS_STANDARD & ~WS_BORDER) | LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT | LBS_OWNERDRAWFIXED | WS_HSCROLL,
		rc,this,1))
		return -1;

	return 0;
}

void CAutoCompleteDlg::OnDestroy()
{
	CWnd::OnDestroy();
}

void CAutoCompleteDlg::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);

	if (m_Box)
		m_Box->SetFocus();
}

LRESULT CAutoCompleteDlg::OnFloatStatus(WPARAM wParam, LPARAM /*lParam*/)
{
	LRESULT result;

	if (wParam == FS_SYNCACTIVE)
	{
		// Respond to this message to prevent the main frame from losing the focus
		result = (GetStyle() & WS_VISIBLE) == WS_VISIBLE;
	}
	else
		result = 0;

	return result;
}

void CAutoCompleteDlg::MakeTransparent(int value)
{
	if (value != 0 && !(this->GetExStyle() & WS_EX_LAYERED))
		this->ModifyStyleEx(0,WS_EX_LAYERED);
	else if (value == 0 && this->GetExStyle() & WS_EX_LAYERED)
		this->ModifyStyleEx(WS_EX_LAYERED,0);

	::SetLayeredWindowAttributes(this->GetSafeHwnd(),0,
		static_cast<BYTE>((100 - value) * 255 / 100.0),LWA_ALPHA);
}

