// AutoCompleteListbox.cpp : implementation file
//

#include "stdafx.h"
#include "texniccenter.h"
#include "AutoCompleteListbox.h"

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
	m_BoldFont = NULL;
	m_PictureCache = new CMapStringToPtr();
}

CAutoCompleteListbox::~CAutoCompleteListbox()
{
	if (m_BoldFont != NULL) { // kill font, if necessary
		delete m_BoldFont;
	}

	// cleanup picture cache
	POSITION pos = m_PictureCache->GetStartPosition();
	m_PictureCache->RemoveAll();
	delete m_PictureCache;
}


BEGIN_MESSAGE_MAP(CAutoCompleteListbox, CListBox)
	//{{AFX_MSG_MAP(CAutoCompleteListbox)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
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

	HGDIOBJ hOldFont = dc.SelectStockObject(DEFAULT_GUI_FONT);

	if (m_BoldFont == NULL) { // lazy creation of bold font
		CFont *tmp = dc.GetCurrentFont();
		LOGFONT lf;
		tmp->GetLogFont(&lf);
		lf.lfWeight = FW_BOLD;

		m_BoldFont = new CFont();
		m_BoldFont->CreateFontIndirect(&lf);
	}

	const CStyleFile *cs = lc->GetParent();
	if (cs != NULL && cs->IsDocClass()) { // bold, if command is part of a document class
		dc.SelectObject(m_BoldFont->GetSafeHandle());
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

	CRect rcItem = lpDrawItemStruct->rcItem;
	int bmpW = rcItem.bottom - rcItem.top;	
	CRect bmpRect = CRect(rcItem.left, rcItem.top, rcItem.left + bmpW, rcItem.bottom);
	bmpRect.InflateRect(-1, -1);
	CRect textRect = CRect(rcItem.left  + bmpW + 2, rcItem.top, rcItem.right, rcItem.bottom);
	// Draw the text.
	dc.DrawText(
	  lpszText,
	  strlen(lpszText),
	  &textRect,
	  DT_SINGLELINE|DT_VCENTER);
	
	if (lc->GetIconIndex() != -1) {
		DrawBitmap(&dc, lc->GetIconFile(), lc->GetIconIndex(), bmpRect);
	}

	// Reset the background color and the text color back to their
	// original values.
	dc.SetTextColor(crOldTextColor);
	dc.SetBkColor(crOldBkColor);
	dc.SelectObject(hOldFont);
	
	dc.Detach(); // disconnect from DC
}

void CAutoCompleteListbox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	ASSERT(lpMeasureItemStruct->CtlType == ODT_LISTBOX);
	
	CSize   sz;
	CDC*    pDC = GetDC();

	HGDIOBJ hOldFont = pDC->SelectStockObject(DEFAULT_GUI_FONT);
	pDC->SelectObject(m_BoldFont);	
	sz = pDC->GetTextExtent("W"); // FIXME
	pDC->SelectObject(hOldFont);

	ReleaseDC(pDC);

	lpMeasureItemStruct->itemHeight = sz.cy + 1;		
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


BOOL CAutoCompleteListbox::DrawBitmap(CDC* pDC, CString file, UINT index, CRect rect) {
	ASSERT_VALID(pDC);
	// load IDB_BITMAP1 from our resources
	CBitmap *bmp = LoadBitmapFromFile(file);
	if (bmp != NULL) {
		// Get the size of the bitmap
		BITMAP bmpInfo;
		bmp->GetBitmap(&bmpInfo);

		if ((index + 1) * BMP_WIDTH > bmpInfo.bmWidth) {
			TRACE("Bitmap too small for index %d (width is only %d)\n", index, bmpInfo.bmWidth);
			return FALSE;
		}
		// Create an in-memory DC compatible with the
		// display DC we're using to paint
		CDC dcMemory;
		dcMemory.CreateCompatibleDC(pDC);

		// Select the bitmap into the in-memory DC
		CBitmap* pOldBitmap = dcMemory.SelectObject(bmp);

		// Copy the bits from the in-memory DC into the on-
		// screen DC to actually do the painting. Use the centerpoint
		// we computed for the target offset.
		pDC->StretchBlt(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 
			&dcMemory, index * BMP_WIDTH, 0, BMP_WIDTH, BMP_HEIGHT, SRCCOPY);

		dcMemory.SelectObject(pOldBitmap);
		return TRUE;
	} else {
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

