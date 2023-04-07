#include "stdafx.h"
#include "AutoCompleteListbox.h"

#include "AutoCompleteDlg.h"
#include "FontOccManager.h"
#include "configuration.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace
{
	enum
	{
		BMP_HEIGHT = 15,
		BMP_WIDTH = 16
	};

	const int padding = 4;

	void AdjustRect(CRect& rect)
	{
		rect.bottom += padding;
		rect.right += BMP_WIDTH + padding * 3; // bmp right, text left an right
	}
}

CAutoCompleteListBox::CAutoCompleteListBox()
{
	EnableToolTips();

	//Init Fonts
	// - normal weight
	LOGFONT lf = GetDisplayFont();
	m_NormalFont.CreateFontIndirect(&lf);
	// - bold
	lf.lfWeight = FW_BOLD;
	m_BoldFont.CreateFontIndirect(&lf);
	// - italic
	lf = GetDisplayFont();
	lf.lfItalic = 1;
	m_ItalicFont.CreateFontIndirect(&lf);
}

CAutoCompleteListBox::~CAutoCompleteListBox()
{
}

BEGIN_MESSAGE_MAP(CAutoCompleteListBox,CListBox)
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY_EX(TTN_NEEDTEXTW,0,OnToolTipText)
	ON_NOTIFY_EX(TTN_NEEDTEXTA,0,OnToolTipText)
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

void CAutoCompleteListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	ASSERT(lpDrawItemStruct->CtlType == ODT_LISTBOX);

	if (lpDrawItemStruct->itemState == ODS_FOCUS /*|| lpDrawItemStruct->itemID < 0*/ || lpDrawItemStruct->itemID >= static_cast<UINT>(GetCount()))
	{
		return;
	}

	// fetch LaTeX command from item data
	CLaTeXCommand *lc = (CLaTeXCommand*) GetItemData(lpDrawItemStruct->itemID);

	if (lc == NULL)   // sanity check
	{
		TRACE2("Warning: lc is NULL for DrawItem: id = %d, state=0x%x\n",lpDrawItemStruct->itemID,lpDrawItemStruct->itemState);
		return;
	}

	CString str;
	GetText(lpDrawItemStruct->itemID, str);
	ASSERT(str.GetLength() >= 0);

	LPCTSTR lpszText = str;
	CDC dc;

	dc.Attach(lpDrawItemStruct->hDC);

	// Save these value to restore them when done drawing.
	COLORREF crOldTextColor = dc.GetTextColor();
	COLORREF crOldBkColor = dc.GetBkColor();

	// Subdivide the space between bitmap and text and leave some pixels in between
	const CRect rcItem = lpDrawItemStruct->rcItem;
	// - bitmap
	CRect bmpRect = rcItem;
	bmpRect.right = bmpRect.left + BMP_WIDTH + padding;

	CRect fillRect = lpDrawItemStruct->rcItem;
	fillRect.left = bmpRect.right;

	CRect textRect = rcItem;
	textRect.left = fillRect.left + padding;

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));

	// If this item is selected, highlight it.  Earlier, we set the background
	// color and the text color to appropriate values.  But these values are not
	// appropriate on Vista anymore.  Also, erase rect by filling it with the
	// background color.
	if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
			(lpDrawItemStruct->itemState & ODS_SELECTED))
	{
		dc.FillSolidRect(&fillRect, ::GetSysColor(COLOR_BTNFACE));
	}
	else
	{
		dc.FillSolidRect(&fillRect, crOldBkColor);
	}

	SharedStyleFilePtr parent(lc->GetParent().lock());

	// Select Font: bold or normal
	if (parent && parent->IsDocClass())
	{
		dc.SelectObject(&m_BoldFont);
	}
	else
	{
		dc.SelectObject(&m_NormalFont);
	}

	// Draw the primary text
	dc.DrawText(lpszText, &textRect, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

	// Draw the comment text, if any
	if (!lc->GetDescription().IsEmpty())
	{
		// Get the horizontal extent of the primary text. We do not want to
		// overprint it.
		CSize PrimaryExtent = GetItemMeasure(&dc, lpszText);
		// - remove it from the text rect
		textRect.left += PrimaryExtent.cx; //padding not needed, looks good without
		// - and get some white space at the right
		textRect.right -= padding;

		dc.SelectObject(&m_ItalicFont);
		dc.SetTextColor(::GetSysColor(COLOR_GRAYTEXT));
		dc.DrawText(lc->GetDescription(), &textRect, DT_SINGLELINE | DT_RIGHT | 
			DT_WORD_ELLIPSIS | DT_VCENTER);
	}

	// Draw Bitmap, if available
	if (lc->GetIconIndex() != -1)
	{
		DrawBitmap(&dc,lc->GetIconFileName(),lc->GetIconIndex(),bmpRect);
	}

	// Focus Rectangle; draw only if needed
	if ((lpDrawItemStruct->itemAction & ODA_FOCUS) &&
			(lpDrawItemStruct->itemState & ODS_FOCUS))
	{
		dc.DrawFocusRect(fillRect);
	}

	// Reset the background color and the text color back to their original
	// values.
	dc.SetTextColor(crOldTextColor);
	dc.SetBkColor(crOldBkColor);

	dc.Detach(); // disconnect from DC
}

void CAutoCompleteListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// This func is called once by the system (at creation of this box) to get
	// info about how large the items are.  Height is most important since the
	// listbox items need to have a certain distance

	ASSERT(lpMeasureItemStruct->CtlType == ODT_LISTBOX);

	CDC* pDC = GetDC();
	CFont* pOldFont = pDC->SelectObject(&m_BoldFont);

	// Measure the height of all the letters and LaTeX stuff that we might have
	// in this box We are no interested in the width here

	const CSize size =
		GetItemMeasure(pDC,_T("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890\\{}()[]"));

	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);

	lpMeasureItemStruct->itemHeight = size.cy;
}

CSize CAutoCompleteListBox::GetLargestItemTextExtent()
{
	CSize LargestItemExtent(-1,-1);

	if (GetSafeHwnd())
	{
		CDC* pDC = GetDC();
		CFont* pOldFont = pDC->SelectObject(&m_BoldFont);

		CRect rect;
		CSize ItemExtent;

		const int NumItems = GetCount();
		for (int i = 0; i < NumItems; i++)
		{
			//Get the command
			CLaTeXCommand* lc = (CLaTeXCommand*) GetItemData(i);

			//Get text of the item and measure it with the correct font
			CString ItemText;
			GetText(i, ItemText);

			SharedStyleFilePtr parent(lc->GetParent().lock());

			//Select Font: bold or normal
			if (parent && parent->IsDocClass())
			{
				pDC->SelectObject(&m_BoldFont);
			}
			else
			{
				pDC->SelectObject(&m_NormalFont);
			}

			//Measure the primary text
			ItemExtent = GetItemMeasure(pDC, ItemText);

			//Measure the comment text, if any
			if (!lc->GetDescription().IsEmpty())
			{
				pDC->SelectObject(&m_ItalicFont);
				CSize CommentExtent = GetItemMeasure(pDC, lc->GetDescription());
				ItemExtent.cx += CommentExtent.cx + padding;
			}

			//Record the largest one
			if (ItemExtent.cx > LargestItemExtent.cx) LargestItemExtent.cx = ItemExtent.cx;
			if (ItemExtent.cy > LargestItemExtent.cy) LargestItemExtent.cy = ItemExtent.cy;
		}

		pDC->SelectObject(pOldFont);
		ReleaseDC(pDC);
	}

	return LargestItemExtent;
}

CSize CAutoCompleteListBox::GetItemMeasure(CDC* dc, LPCTSTR text)
{
	CRect rc;
	dc->DrawText(text,-1,&rc,DT_SINGLELINE|DT_CALCRECT);

	AdjustRect(rc);

	return rc.Size();
}

int CAutoCompleteListBox::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct)
{
	ASSERT(lpCompareItemStruct->CtlType == ODT_LISTBOX);
	LPCTSTR lpszText1 = (LPCTSTR) lpCompareItemStruct->itemData1;
	ASSERT(lpszText1 != NULL);
	LPCTSTR lpszText2 = (LPCTSTR) lpCompareItemStruct->itemData2;
	ASSERT(lpszText2 != NULL);
	return _tcscmp(lpszText2,lpszText1);
}

BOOL CAutoCompleteListBox::DrawBitmap(CDC* pDC, const CString& file, UINT index, const CRect& rect)
{
	ASSERT_VALID(pDC);
	CImageList* imageList = LoadBitmapFromFile(file);

	if (imageList)
	{
		int cx, cy;
		::ImageList_GetIconSize(*imageList, &cx, &cy);

		// Center the image
		::ImageList_Draw(*imageList, index, pDC->m_hDC, rect.left + (rect.Width() - cx) / 2,
			rect.top + (rect.Height() - cy) / 2, ILD_TRANSPARENT);
	}

	return imageList != 0;
}

CImageList* CAutoCompleteListBox::LoadBitmapFromFile(const CString& filename)
{
	CImageList* result = NULL;

	ImageListMap::const_iterator it = image_cache_.find(filename);

	if (it == image_cache_.end())
	{
		HIMAGELIST hImageList = ::ImageList_LoadImage(0, filename, 16, 1, RGB(255,255,255),
			IMAGE_BITMAP, LR_DEFAULTSIZE | LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		
		if (hImageList)
		{
			std::unique_ptr<CImageList> imageList(new CImageList());
			imageList->Attach(hImageList);

			result = imageList.get();

			image_cache_.insert(std::make_pair(filename, std::move(imageList)));
		}
		else
		{
			TRACE1("Could not load bitmap from file %s\n", (LPCTSTR)filename);
		}
	}
	else
		result = it->second.get();

	return result;
}

/** Treat dblclk as confirmation */
void CAutoCompleteListBox::OnLButtonDblClk(UINT nFlags,CPoint point)
{
	CListBox::OnLButtonDblClk(nFlags,point);
	CAutoCompleteDlg *w = dynamic_cast<CAutoCompleteDlg*>(GetOwner());

	if (w != NULL)
	{
		w->ApplySelection();
	}
}

INT_PTR CAutoCompleteListBox::OnToolHitTest(CPoint point, TOOLINFO * pTI) const
{
	RECT cellrect; // cellrect - to hold the bounding rect
	BOOL tmp = FALSE;
	int row = ItemFromPoint(point,tmp); //we call the ItemFromPoint function to determine the row,
	//note that in NT this function may fail use the ItemFromPointNT member function

	if (row == -1)
		return -1;

	//set up the TOOLINFO structure. GetItemRect(row,&cellrect);
	GetItemRect(row,&cellrect);
	pTI->rect = cellrect;
	pTI->hwnd = m_hWnd;
	pTI->uId = static_cast<UINT_PTR>(row); //The ‘uId’ is assigned a value according to the row value.
	pTI->lpszText = LPSTR_TEXTCALLBACK; //Send a TTN_NEEDTEXT messages

	return static_cast<INT_PTR>(pTI->uId);
}

BOOL CAutoCompleteListBox::OnToolTipText(UINT /*id*/, NMHDR * pNMHDR, LRESULT * pResult)
{
	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*) pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*) pNMHDR;
	CString strTipText;
	UINT nID = pNMHDR->idFrom; //list box item index

	CLaTeXCommand *lc = (CLaTeXCommand*) GetItemData(nID);
	strTipText = lc->GetDescription();
	//display item text as tool tip
#ifndef _UNICODE
	if (pNMHDR->code == TTN_NEEDTEXTA)
		lstrcpyn(pTTTA->szText,strTipText,80);
	else
		_mbstowcsz(pTTTW->szText,strTipText,80);
#else
	if (pNMHDR->code == TTN_NEEDTEXTA)
		_wcstombsz(pTTTA->szText,strTipText,80);
	else
		lstrcpyn(pTTTW->szText,strTipText,80);
#endif
	*pResult = 0;

	return TRUE;
}

void CAutoCompleteListBox::OnKillFocus(CWnd* pNewWnd)
{
	CListBox::OnKillFocus(pNewWnd);

	if (pNewWnd != GetParent())
		GetParent()->SendMessage(WM_KILLFOCUS,reinterpret_cast<WPARAM>(pNewWnd->GetSafeHwnd()));
}
