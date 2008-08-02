// AutoCompleteListbox.cpp : implementation file
//

#include "stdafx.h"
#include "texniccenter.h"

#include <algorithm>
#include <functional>

#include "AutoCompleteListbox.h"
#include "FontOccManager.h"
#include "configuration.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class CAutoCompleteDlg;
/////////////////////////////////////////////////////////////////////////////
// CAutoCompleteListBox

namespace {
    enum {
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
}

CAutoCompleteListBox::~CAutoCompleteListBox()
{
    using namespace std::tr1::placeholders;

    // Destroy every image list in the map
    std::for_each(image_cache_.begin(),image_cache_.end(),
        std::tr1::bind(std::ptr_fun(::ImageList_Destroy),
            std::tr1::bind(&ImageCacheContainer::value_type::second,_1)));
}

BEGIN_MESSAGE_MAP(CAutoCompleteListBox,CListBox)
    ON_WM_LBUTTONDBLCLK()
    ON_NOTIFY_EX(TTN_NEEDTEXTW,0,OnToolTipText)
    ON_NOTIFY_EX(TTN_NEEDTEXTA,0,OnToolTipText)
    ON_WM_KILLFOCUS()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAutoCompleteListBox message handlers

void CAutoCompleteListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    ASSERT(lpDrawItemStruct->CtlType == ODT_LISTBOX);
    CString str;

    if (lpDrawItemStruct->itemState == ODS_FOCUS /*|| lpDrawItemStruct->itemID < 0*/ || lpDrawItemStruct->itemID >= static_cast<UINT> (GetCount())) {
        return;
    }

    // fetch LaTeX command from item data
    CLaTeXCommand *lc = (CLaTeXCommand*) GetItemData(lpDrawItemStruct->itemID);

    if (lc == NULL) { // sanity check
        TRACE("Warning: lc is NULL for DrawItem: id = %d, state=0x%x\n",lpDrawItemStruct->itemID,lpDrawItemStruct->itemState);
        return;
    }

    GetText(lpDrawItemStruct->itemID,str);
    ASSERT(str.GetLength() >= 0);

    LPCTSTR lpszText = str;
    CDC dc;

    dc.Attach(lpDrawItemStruct->hDC);

    // Save these value to restore them when done drawing.
    COLORREF crOldTextColor = dc.GetTextColor();
    COLORREF crOldBkColor = dc.GetBkColor();

    //Select Font: bold or normal
    if (lc->GetParent() != NULL && lc->GetParent()->IsDocClass()) {
        dc.SelectObject(&m_BoldFont);
    }
    else {
        dc.SelectObject(&m_NormalFont);
    }

    //Subdivide the space between bitmap and text and leave some pixels in between
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

    // If this item is selected, set the background color 
    // and the text color to appropriate values. Also, erase
    // rect by filling it with the background color.
    if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
        (lpDrawItemStruct->itemState & ODS_SELECTED)) {
            //dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
            //dc.SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
            dc.FillSolidRect(&fillRect,
                    ::GetSysColor(COLOR_BTNFACE));
    }
    else {
        
        //dc.SetBkColor(::GetSysColor(COLOR_WINDOW));
        dc.FillSolidRect(&fillRect,crOldBkColor);
    }

    //Draw the text
    dc.DrawText(lpszText,_tcslen(lpszText),&textRect,DT_SINGLELINE | DT_VCENTER);

    //Draw Bitmap, if available
    if (lc->GetIconIndex() != -1) {
        DrawBitmap(&dc,lc->GetIconFileName(),lc->GetIconIndex(),bmpRect);
    }

    //Focus Rectangle; draw only if needed
    if ((lpDrawItemStruct->itemAction & ODA_FOCUS) &&
            (lpDrawItemStruct->itemState & ODS_FOCUS)) {
        dc.DrawFocusRect(fillRect);
    }

    // Reset the background color and the text color back to their
    // original values.
    dc.SetTextColor(crOldTextColor);
    dc.SetBkColor(crOldBkColor);

    dc.Detach(); // disconnect from DC
}

void CAutoCompleteListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
    //This func is called once by the system (at creation of this box)
    //to get info about how large the items are.
    //Height is most important since the listbox items need to have a certain distance

    ASSERT(lpMeasureItemStruct->CtlType == ODT_LISTBOX);

    CDC* pDC = GetDC();
    CFont* pOldFont = pDC->SelectObject(&m_BoldFont);

    //Measure the height of all the letters and LaTeX stuff that we might have in this box
    //We are no interested in the width here

    const CSize size = 
        GetItemMeasure(pDC,_T("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890\\{}()[]"));

    pDC->SelectObject(pOldFont);
    ReleaseDC(pDC);

    lpMeasureItemStruct->itemHeight = size.cy;
}

CSize CAutoCompleteListBox::GetLargestItemTextExtent()
{
    CSize LargestItemExtent(-1,-1);

    if (GetSafeHwnd()) {
        CDC* pDC = GetDC();
        CFont* pOldFont = pDC->SelectObject(&m_BoldFont);

        CRect rect;
        CSize ItemExtent;

        const int NumItems = GetCount();
        for (int i = 0; i < NumItems; i++) {
            //Get text of the item and measure it
            CString ItemText;
            GetText(i,ItemText);

            ItemExtent = GetItemMeasure(pDC,ItemText);

            //Record the largest one
            if (ItemExtent.cx > LargestItemExtent.cx) LargestItemExtent.cx = ItemExtent.cx;
            if (ItemExtent.cy > LargestItemExtent.cy) LargestItemExtent.cy = ItemExtent.cy;
        }

        pDC->SelectObject(pOldFont);
        ReleaseDC(pDC);
    }

    return LargestItemExtent;
}

const CSize CAutoCompleteListBox::GetItemMeasure(CDC* dc, LPCTSTR text)
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

BOOL CAutoCompleteListBox::DrawBitmap( CDC* pDC, const CString& file, UINT index, const CRect& rect )
{
    ASSERT_VALID(pDC);

    // load IDB_BITMAP1 from our resources
    HIMAGELIST himl = LoadBitmapFromFile(file);

    if (himl) {
        int cx, cy;
        ::ImageList_GetIconSize(himl,&cx,&cy);

        // Center the image
        ::ImageList_Draw(himl,index,pDC->m_hDC,rect.left + (rect.Width() - cx) / 2,
            rect.top + (rect.Height() - cy) / 2,ILD_TRANSPARENT);
    }

    return himl != 0;
}

/* Loads a bitmap from file. Will be replaced by CImage, if we'll we upgrade to a higher version of MFC/ATL */
HIMAGELIST CAutoCompleteListBox::LoadBitmapFromFile( const CString& filename )
{
    //HBITMAP hbm;
    HIMAGELIST hbm;

    //if (!m_PictureCache->Lookup(filename,(void*&) hbm)) { // not in cache?
    ImageCacheContainer::iterator it = image_cache_.find(filename);

    if (it == image_cache_.end()) {
        hbm = ::ImageList_LoadImage(0,filename,16,1,RGB(255,255,255),
            IMAGE_BITMAP,LR_DEFAULTSIZE|LR_LOADFROMFILE|LR_CREATEDIBSECTION);
        // ... create it
        if (hbm) {
            // ... and put it into the map
            image_cache_.insert(std::make_pair(filename,hbm));
            //m_PictureCache->SetAt(filename,hbm);
        }
        else {
            TRACE("Could not load bitmap from file %s\n",filename);
            return NULL;
        }
    }
    else
        hbm = it->second;

    return hbm;//CBitmap::FromHandle(hbm);
}

/** Treat dblclk as confirmation */
void CAutoCompleteListBox::OnLButtonDblClk(UINT nFlags,CPoint point)
{
    CListBox::OnLButtonDblClk(nFlags,point);
    CAutoCompleteDlg *w = dynamic_cast<CAutoCompleteDlg*> (GetOwner());

    if (w != NULL) {
        w->ApplySelection();
    }
}

INT_PTR CAutoCompleteListBox::OnToolHitTest( CPoint point, TOOLINFO * pTI ) const
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
