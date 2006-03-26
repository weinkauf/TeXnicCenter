// AutoCompleteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "texniccenter.h"
#include "AutoCompleteDlg.h"
#include "AutoCompleteListbox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoCompleteDlg

CAutoCompleteDlg::CAutoCompleteDlg(CStyleFileContainer *sfc, CWnd* pParent)
:CWnd()
{
	ASSERT(sfc != NULL);

	m_CurrentKeyword = "";
	m_Listener = NULL;
	m_Container = sfc;
	m_Visible = FALSE;
	m_Box = new CAutoCompleteListbox;
	if (!Create(0, 0, 0, CRect(200,200, 400, 400), pParent, ID_AUTOCOMPLETE)) {
		TRACE("Window creation (CAutoCompleteDlg) failed!\n");
	} 
}

CAutoCompleteDlg::~CAutoCompleteDlg()
{
	if (m_Box!= NULL) {
		delete m_Box;
	}
	DestroyWindow();
}


BEGIN_MESSAGE_MAP(CAutoCompleteDlg, CWnd)
	//{{AFX_MSG_MAP(CAutoCompleteDlg)
	ON_WM_KILLFOCUS()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAutoCompleteDlg message handlers

BOOL CAutoCompleteDlg::InitWithKeyword(CString &keyword)
{
	if (keyword.GetLength() < MINIMUM_KEYWORD_LENGTH) {
		return FALSE;
	}

	CLaTeXCommand *lc;
	CString key;
	int idx;
	const CMapStringToOb *map;

	map = m_Container->GetPossibleItems(keyword, _T(""));
	
	if (map == NULL || map->GetCount() == 0) { /* Nothing to do */
		m_CurrentKeyword.Empty();
		return FALSE;
	}

	m_CurrentKeyword = keyword;

	POSITION pos = map->GetStartPosition();	

	/* iterate through map */
	if (map->GetCount() == 1) { /* Just on element -> Notify caller immediatly */
		map->GetNextAssoc(pos, key, (CObject*&)lc);
		if (m_Listener != NULL) {
			m_Listener->OnACCommandSelect(lc);
		}
		//TRACE("One element only: %s\n", keyword);
		delete map;
		return FALSE;
	} else {
		if (!::IsWindow(this->GetSafeHwnd())) { // provide safe exit
			TRACE("ERROR: Not a window\n");
			if (m_Listener != NULL) {
				delete map;
				if (m_Listener != NULL) {
					m_Listener->OnACCommandCancelled();
				}
			}	
		}

		m_Box->ResetContent();
		m_Box->InitStorage(map->GetCount(), keyword.GetLength() * 2); /* prepare memory */
		
		while(pos != NULL) { /* fill list with items */
			map->GetNextAssoc(pos, key, (CObject*&)lc);
			idx = m_Box->AddString(lc->ToLaTeX());
			
			int ret = m_Box->SetItemDataPtr(idx, lc); // sync pointer with string 

			if (ret == LB_ERR) {
				TRACE("Error in SetItemDataPtr: %d (idx = %d)\n", ::GetLastError(), idx);
			}
		}	
		ASSERT(map->GetCount() == m_Box->GetCount());
	}

	delete map;
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
	int x = m_Box->FindString(-1, newKeyword);

	if (x != LB_ERR) {
		m_Box->SetCurSel(x);
		m_Box->SetTopIndex(x);
		return TRUE;
	} else {
		return FALSE;
	}
}

BOOL CAutoCompleteDlg::IsCancelChar(WORD theChar) {
	return theChar == VK_SPACE;
}

/* User has comfirmed selection.  */
void CAutoCompleteDlg::ApplySelection()
{
	int n = m_Box->GetCurSel();
	CString selectedItem;
	if (n != LB_ERR) {
		m_Box->GetText(n, selectedItem);	
		
		CLaTeXCommand *lc = (CLaTeXCommand *)m_Box->GetItemData(n);
		ASSERT(lc != NULL);
		if (m_Listener != NULL) {
			m_Listener->OnACCommandSelect(lc);
		} 		
	} else {
		TRACE("Error occurred during ApplySelection, error code %d", ::GetLastError());
	}	
	ShowWindow(SW_HIDE);
}

/* User cancelled completion */
void CAutoCompleteDlg::CancelSelection()
{		
	if (m_Listener != NULL) {
		m_Listener->OnACCommandCancelled();
		
	} 
	ShowWindow(SW_HIDE);
}

/**
 Moves current selection by delta (e. g. delte=-1 moves selection to the previous item, delta=1 to the next
 */
void CAutoCompleteDlg::MoveSelection(int delta)
{
	int nCount = m_Box->GetCount();
	int curSel = m_Box->GetCurSel();
	int newSel = curSel + delta;

	/* Check bounds */
	if (newSel > nCount - 1) {
		newSel = nCount - 1;
	}

	if (newSel < 0) {
		newSel = 0;
	}
	SetCurSel(newSel);	
}

/* User wants help */
void CAutoCompleteDlg::Help()
{
	CString selectedItem;
	int n = m_Box->GetCurSel();

	if (n != LB_ERR) {
		m_Box->GetText(n, selectedItem);	
		if (m_Listener != NULL) {
			m_Listener->OnACHelp(selectedItem);
		}
	} else {
		TRACE("Error occurred during ApplySelection, error code %d", ::GetLastError());
	}
}

void CAutoCompleteDlg::SetCurSel(int sel)
{
	m_Box->SetCurSel(sel);
}

BOOL CAutoCompleteDlg::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	BOOL ok;
	dwStyle = WS_POPUP|WS_VISIBLE|WS_DLGFRAME;

	ok = CWnd::CreateEx(0, 
			AfxRegisterWndClass(
				CS_HREDRAW|CS_VREDRAW|CS_OWNDC|CS_DBLCLKS,
				theApp.LoadStandardCursor(IDC_ARROW),
				GetSysColorBrush(COLOR_WINDOW)), 
			_T("AutoCompleteBox"), 
			dwStyle, 
			rect, 
			pParentWnd, 
			0, 
			pContext);
	
	if (ok) { // creation successful -> create the embedded listbox
		CRect rc;
		GetClientRect(&rc);
		ok &= m_Box->Create(WS_CHILD|WS_VISIBLE|LBS_STANDARD|LBS_HASSTRINGS|LBS_NOINTEGRALHEIGHT|LBS_OWNERDRAWFIXED|WS_HSCROLL, 
			rc, 
			this, 
			1);
	}
	return ok;
}

/* Intercept keyboard input */
BOOL CAutoCompleteDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (!IsWindowVisible()) { /* are we sleeping? */
		CWnd *wnd = GetParent(); /* pass msg to parent */
		return wnd->PreTranslateMessage(pMsg);
	}

	switch(pMsg->message) {
	case WM_KEYDOWN:
		switch(pMsg->wParam) {

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
		/* Delete last char */
		case VK_BACK:
			if (m_CurrentKeyword.GetLength() > MINIMUM_KEYWORD_LENGTH) {
				m_CurrentKeyword.Delete(m_CurrentKeyword.GetLength() - 1);
				if (m_Listener != NULL) {
					m_Listener->OnACBackspace();
				}
				UpdateSelection(m_CurrentKeyword);
				/* drop box, if nothing is left */
				if (m_CurrentKeyword.GetLength() <= MINIMUM_KEYWORD_LENGTH) { 
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
		if (IsWindowVisible()) {
			if (!IsCancelChar(pMsg->wParam)) { /* check, if char interrupts keyword (e. g. space) */
				m_CurrentKeyword += (TCHAR)pMsg->wParam;		
				UpdateSelection(m_CurrentKeyword);
			} else {
				if (m_Listener != NULL) {
					m_Listener->OnACChar(pMsg->wParam, LOWORD(pMsg->lParam), HIWORD(pMsg->lParam));
				}
				CancelSelection();
			}
			if (m_Listener != NULL) {
				m_Listener->OnACChar(pMsg->wParam, LOWORD(pMsg->lParam), HIWORD(pMsg->lParam));
			}
		}
		break;	
	}
	
	return CWnd::PreTranslateMessage(pMsg);
}


void CAutoCompleteDlg::OnKillFocus(CWnd* pNewWnd) 
{
	if (pNewWnd != m_Box && pNewWnd != this) {
		//TRACE("==> KillFocus, visible = %d\n", IsWindowVisible());
		CWnd::OnKillFocus(pNewWnd);
		CancelSelection();
		//TRACE("<== KillFocus\n");
	} else {
		//TRACE("My window -> not KillFocus\n");
	}
}



void CAutoCompleteDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	if (bShow != m_Visible) {
		//TRACE("Show window show = %d, status = %d, old visible = %d\n", bShow, nStatus, m_Visible);
		CWnd::OnShowWindow(bShow, nStatus);
		m_Visible = bShow;
	}
}


void CAutoCompleteDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	ApplySelection();
}
