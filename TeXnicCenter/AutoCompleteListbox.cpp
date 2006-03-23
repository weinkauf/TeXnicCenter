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

/////////////////////////////////////////////////////////////////////////////
// CAutoCompleteListbox

CAutoCompleteListbox::CAutoCompleteListbox()
{
	m_BoldFont = NULL;
}

CAutoCompleteListbox::~CAutoCompleteListbox()
{
	if (m_BoldFont != NULL) { // kill font, if necessary
		delete m_BoldFont;
	}
}


BEGIN_MESSAGE_MAP(CAutoCompleteListbox, CListBox)
	//{{AFX_MSG_MAP(CAutoCompleteListbox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
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
		TRACE("%s is from class %s\n", str, cs->GetName());
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

	// Draw the text.
	dc.DrawText(
	  lpszText,
	  strlen(lpszText),
	  &lpDrawItemStruct->rcItem,
	  DT_SINGLELINE|DT_VCENTER);

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
