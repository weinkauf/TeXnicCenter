// AutoCompleteListbox.cpp : implementation file
//

#include "stdafx.h"
#include "texniccenter.h"
#include "AutoCompleteListbox.h"
#include "configuration.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class CAutoCompleteDlg;
/////////////////////////////////////////////////////////////////////////////
// CAutoCompleteListbox

CAutoCompleteListbox::CAutoCompleteListbox()
{
	m_PictureCache = new CMapStringToPtr();
	EnableToolTips();

	//Init Fonts
	// - normal weight
	m_NormalFont.CreateFontIndirect(&g_configuration.m_fontEditor);
	// - bold
	LOGFONT BoldLF = g_configuration.m_fontEditor;
	BoldLF.lfWeight = FW_BOLD;
	m_BoldFont.CreateFontIndirect(&BoldLF);
}

CAutoCompleteListbox::~CAutoCompleteListbox()
{
	//Cleanup picture cache
	if (m_PictureCache)
	{
		POSITION pos = m_PictureCache->GetStartPosition();
		m_PictureCache->RemoveAll();
		delete m_PictureCache;
	}
}


BEGIN_MESSAGE_MAP(CAutoCompleteListbox, CListBox)
	//{{AFX_MSG_MAP(CAutoCompleteListbox)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAutoCompleteListbox message handlers

void CAutoCompleteListbox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	ASSERT(lpDrawItemStruct->CtlType == ODT_LISTBOX);
	CString str;

	if (lpDrawItemStruct->itemState == ODS_FOCUS || lpDrawItemStruct->itemID <0 || lpDrawItemStruct->itemID>= GetCount()) {		
		return;		
	}

	// fetch LaTeX command from item data
	CLaTeXCommand *lc = (CLaTeXCommand*)GetItemData(lpDrawItemStruct->itemID);

	if (lc == NULL) { // sanity check
		TRACE("Warning: lc is NULL for DrawItem: id = %d, state=0x%x\n", lpDrawItemStruct->itemID, lpDrawItemStruct->itemState);
		return;
	}
	
	GetText(lpDrawItemStruct->itemID, str);
	ASSERT(str.GetLength() >= 0);
	LPCTSTR lpszText = str;
	CDC dc;

	dc.Attach(lpDrawItemStruct->hDC);

	// Save these value to restore them when done drawing.
	COLORREF crOldTextColor = dc.GetTextColor();
	COLORREF crOldBkColor = dc.GetBkColor();

	//Select Font: bold or normal
	if (lc->GetParent() != NULL && lc->GetParent()->IsDocClass())
	{ 
		dc.SelectObject(&m_BoldFont);
	}
	else
	{
		dc.SelectObject(&m_NormalFont);
	}

	// If this item is selected, set the background color 
	// and the text color to appropriate values. Also, erase
	// rect by filling it with the background color.
	if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
	  (lpDrawItemStruct->itemState & ODS_SELECTED))	{
	  dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
	  dc.SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
	  dc.FillSolidRect(&lpDrawItemStruct->rcItem, 
		 ::GetSysColor(COLOR_HIGHLIGHT));
	} else {
	  dc.FillSolidRect(&lpDrawItemStruct->rcItem, crOldBkColor);
	}

	//Subdivide the space between bitmap and text and leave some pixels inbetween
	const CRect rcItem = lpDrawItemStruct->rcItem;
	// - bitmap
	CRect bmpRect = rcItem;
	bmpRect.right = bmpRect.left + BMP_WIDTH;
	//bmpRect.InflateRect(-1, -1);
	// - text
	CRect textRect = rcItem;
	textRect.left = bmpRect.right + 2;

	//Draw the text
	dc.DrawText(lpszText, strlen(lpszText), &textRect, DT_SINGLELINE|DT_VCENTER);

	//Draw Bitmap, if available
	if (lc->GetIconIndex() != -1)
	{
		DrawBitmap(&dc, lc->GetIconFile(), lc->GetIconIndex(), bmpRect);
	}

	//Focus Rectangle
	if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
	  (lpDrawItemStruct->itemState & ODS_SELECTED))
	{
		dc.DrawFocusRect(rcItem);
	}

	// Reset the background color and the text color back to their
	// original values.
	dc.SetTextColor(crOldTextColor);
	dc.SetBkColor(crOldBkColor);
	
	dc.Detach(); // disconnect from DC
}

void CAutoCompleteListbox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	//This func is called once by the system (at creation of this box)
	//to get info about how large the items are.
	//Height is most important since the listbox items need to have a certain distance

	ASSERT(lpMeasureItemStruct->CtlType == ODT_LISTBOX);

	CDC* pDC = GetDC();
	CFont* pOldFont = pDC->SelectObject(&m_BoldFont);

	//Measure the height of all the letters and LaTeX stuff that we might have in this box
	//We are no interested in the width here
	CSize sz = pDC->GetTextExtent("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890\\{}()[]");

	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);

	lpMeasureItemStruct->itemHeight = sz.cy + 1;
}


CSize CAutoCompleteListbox::GetLargestItemTextExtent()
{
	CSize LargestItemExtent(-1, -1);

	if (GetSafeHwnd())
	{
		CDC* pDC = GetDC();
		CFont* pOldFont = pDC->SelectObject(&m_BoldFont);

		const int NumItems = GetCount();
		for(int i=0;i<NumItems;i++)
		{
			//Get text of the item and measure it
			CString ItemText;
			GetText(i, ItemText);
			CSize ItemExtent = pDC->GetTextExtent(ItemText);

			//Record the largest one
			if (ItemExtent.cx > LargestItemExtent.cx) LargestItemExtent.cx = ItemExtent.cx;
			if (ItemExtent.cy > LargestItemExtent.cy) LargestItemExtent.cy = ItemExtent.cy;
		}

		pDC->SelectObject(pOldFont);
		ReleaseDC(pDC);
	}

	return LargestItemExtent;
}


int CAutoCompleteListbox::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct) 
{
	ASSERT(lpCompareItemStruct->CtlType == ODT_LISTBOX);
	LPCTSTR lpszText1 = (LPCTSTR) lpCompareItemStruct->itemData1;
	ASSERT(lpszText1 != NULL);
	LPCTSTR lpszText2 = (LPCTSTR) lpCompareItemStruct->itemData2;
	ASSERT(lpszText2 != NULL);
	return strcmp(lpszText2, lpszText1);
}


BOOL CAutoCompleteListbox::DrawBitmap(CDC* pDC, CString file, UINT index, CRect rect)
{
	ASSERT_VALID(pDC);

	// load IDB_BITMAP1 from our resources
	CBitmap *bmp = LoadBitmapFromFile(file);
	if (bmp != NULL)
	{
		// Get the size of the bitmap
		BITMAP bmpInfo;
		bmp->GetBitmap(&bmpInfo);

		if ((index + 1) * BMP_WIDTH > bmpInfo.bmWidth)
		{
			TRACE("Bitmap too small for index %d (width is only %d)\n", index, bmpInfo.bmWidth);
			return FALSE;
		}

		// Create an in-memory DC compatible with the
		// display DC we're using to paint
		CDC dcMemory;
		dcMemory.CreateCompatibleDC(pDC);

		// Select the bitmap into the in-memory DC
		//CBitmap* pOldBitmap = dcMemory.SelectObject(bmp);
		dcMemory.SelectObject(bmp);

		//We vertically center the bitmap
		int VSpace = (rect.Height() - BMP_HEIGHT) / 2; //After all, we're a LaTeX editor, aren't we?
		if (VSpace < 0) VSpace = 0;

		//Copy the bits from the in-memory DC into the on-screen DC to actually do the painting.
		pDC->StretchBlt(rect.left, rect.top + VSpace, BMP_WIDTH, BMP_HEIGHT,
						&dcMemory,
						index * BMP_WIDTH, 0, BMP_WIDTH, BMP_HEIGHT, SRCCOPY);

		//dcMemory.SelectObject(pOldBitmap);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/* Loads a bitmap from file. Will be replaced by CImage, if we'll we upgrade to a higher version of MFC/ATL */
CBitmap *CAutoCompleteListbox::LoadBitmapFromFile(CString filename) {
	HBITMAP hbm;
	
	if (!m_PictureCache->Lookup(filename, (void*&)hbm)) { // not in cache?
		hbm = (HBITMAP)::LoadImage( AfxGetInstanceHandle(),											
				_T(filename),
				IMAGE_BITMAP,
				0,
				0,
				LR_LOADFROMFILE | LR_DEFAULTSIZE );
		// ... create it
		if (hbm) {
			// ... and put it into the map
			m_PictureCache->SetAt(filename, hbm);
		} else {
			TRACE("Could not load bitmap from file %s\n", filename);
			return NULL;
		}
	}
	return CBitmap::FromHandle(hbm);
}

/** Treat dblclk as confirmation */
void CAutoCompleteListbox::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CListBox::OnLButtonDblClk(nFlags, point);
	CAutoCompleteDlg *w = dynamic_cast<CAutoCompleteDlg*>(GetOwner());
	
	if (w != NULL) {
		w->ApplySelection();
	}
}

int CAutoCompleteListbox::OnToolHitTest(CPoint point, TOOLINFO * pTI) const
{
    int row;
    RECT cellrect; // cellrect - to hold the bounding rect
    BOOL tmp = FALSE;
    row = ItemFromPoint(point,tmp); //we call the ItemFromPoint function to determine the row,
    //note that in NT this function may fail use the ItemFromPointNT member function
    if ( row == -1 ) 
        return -1;

    //set up the TOOLINFO structure. GetItemRect(row,&cellrect);
    GetItemRect(row,&cellrect);
    pTI->rect = cellrect;
    pTI->hwnd = m_hWnd;
    pTI->uId = (UINT)((row)); //The ‘uId’ is assigned a value according to the row value.
    pTI->lpszText = LPSTR_TEXTCALLBACK;  //Send a TTN_NEEDTEXT messages 
    return pTI->uId;
}

BOOL CAutoCompleteListbox::OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
// need to handle both ANSI and UNICODE versions of the message
    TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
    TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
    CString strTipText;
    UINT nID = pNMHDR->idFrom;  //list box item index 

	CLaTeXCommand *lc = (CLaTeXCommand*)GetItemData(nID);
    strTipText = lc->GetDescription();
//display item text as tool tip 
#ifndef _UNICODE
    if (pNMHDR->code == TTN_NEEDTEXTA)
    lstrcpyn(pTTTA->szText, strTipText, 80);
else
    _mbstowcsz(pTTTW->szText, strTipText, 80);
#else
    if (pNMHDR->code == TTN_NEEDTEXTA)
    _wcstombsz(pTTTA->szText, strTipText, 80);
    else
    lstrcpyn(pTTTW->szText, strTipText, 80);
    #endif
    *pResult = 0;

	return TRUE; 
}


