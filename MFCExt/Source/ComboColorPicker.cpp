// ComboColorPicker.cpp : implementation file
// © 1997 Baldvin Hansson

#include "stdafx.h"
#include "ComboColorPicker.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComboColorPicker

COLORREF CComboColorPicker::m_rgStandardColors[] = {
	0xFFFFFFFF,
	RGB(0, 0, 0),			// Black
	RGB(255, 255, 255),		// White
	RGB(128, 0, 0),			// Dark Red
	RGB(0, 128, 0),			// Dark Green
	RGB(128, 128, 0),		// Dark Yellow
	RGB(0, 0, 128),			// Dark Blue
	RGB(128, 0, 128),		// Dark Magenta
	RGB(0, 128, 128),		// Dark Cyan
	RGB(192, 192, 192),		// Light Grey
	RGB(128, 128, 128),		// Dark Grey
	RGB(255, 0, 0),			// Red
	RGB(0, 255, 0),			// Green
	RGB(255, 255, 0),		// Yellow
	RGB(0, 0, 255),			// Blue
	RGB(255, 0, 255),		// Magenta
	RGB(0, 255, 255),		// Cyan
	};

CComboColorPicker::CComboColorPicker()
{
	m_bInitialized = false;
}

CComboColorPicker::~CComboColorPicker()
{
}


BEGIN_MESSAGE_MAP(CComboColorPicker, CComboBox)
	//{{AFX_MSG_MAP(CComboColorPicker)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComboColorPicker message handlers

int CComboColorPicker::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	InitializeData();

	return 0;
}

void CComboColorPicker::PreSubclassWindow() 
{
	InitializeData();

	// set item height
	CDC		*pDC = GetDC();
	CSize	sizeAutoText = pDC->GetTextExtent( CString( _T("default") ) );
	SetItemHeight( 0, sizeAutoText.cy + 4 );
	SetItemHeight( -1, sizeAutoText.cy + 4 );
	
	CComboBox::PreSubclassWindow();
}

void CComboColorPicker::InitializeData()
{
	int nItem;

	if (m_bInitialized)
		return;

	for (int nColor = 0; nColor < sizeof(m_rgStandardColors)/sizeof(COLORREF); nColor++)
	{
		nItem = AddString((LPCTSTR)m_rgStandardColors[nColor]);	// Here we could affect the sort order at run-time

		if (CB_ERRSPACE == nItem)
			break;
	}

	m_bInitialized = true;
}

void CComboColorPicker::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC dc;
	CBrush brushBlack;
	brushBlack.CreateStockObject(BLACK_BRUSH);
	
	if (!dc.Attach(lpDrawItemStruct->hDC))
		return;

	COLORREF rgbTextColor = dc.GetTextColor();
	COLORREF rgbBkColor = dc.GetBkColor();

	if (lpDrawItemStruct->itemAction & ODA_FOCUS)
	{
		dc.DrawFocusRect(&lpDrawItemStruct->rcItem);
	}
	else if (lpDrawItemStruct->itemAction & ODA_DRAWENTIRE)
	{
		if (lpDrawItemStruct->itemState & ODS_FOCUS)
			dc.DrawFocusRect(&lpDrawItemStruct->rcItem);
		else
			dc.ExtTextOut(0, 0, ETO_OPAQUE, &lpDrawItemStruct->rcItem, _T(""), 0, NULL);
	}

	if (0 <= (int)lpDrawItemStruct->itemID)	// Any item selected?
	{
		::InflateRect(&lpDrawItemStruct->rcItem, -2, -2);

		if( (COLORREF)lpDrawItemStruct->itemData == 0xFFFFFFFF )
		{
			CSize	textSize = dc.GetTextExtent( _T("default") );
			dc.FillSolidRect( &lpDrawItemStruct->rcItem, RGB( 0xFF, 0xFF, 0xFF ) );
			dc.FrameRect( &lpDrawItemStruct->rcItem, &brushBlack );

			dc.SetTextColor( RGB( 0x00, 0x00, 0x00 ) );
			dc.SetBkColor( RGB( 0xFF, 0xFF, 0xFF ) );
			dc.SetTextAlign( TA_CENTER | TA_TOP | TA_NOUPDATECP );
			dc.TextOut( 
				lpDrawItemStruct->rcItem.left + (lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left) / 2, 
				lpDrawItemStruct->rcItem.top +
					((lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top) / 2) -
					(textSize.cy / 2), 
				_T("default") );
		}
		else
		{
			dc.FillSolidRect(&lpDrawItemStruct->rcItem, (COLORREF)lpDrawItemStruct->itemData);
			dc.FrameRect(&lpDrawItemStruct->rcItem, &brushBlack);
		}
	}

	// Restore the DC state
	dc.SetTextColor(rgbTextColor);
	dc.SetBkColor(rgbBkColor);
	dc.Detach();
}


COLORREF CComboColorPicker::GetSelectedColor()
{
	int nItem = GetCurSel();
	if (CB_ERR == nItem)
		return RGB(0, 0, 0);	// Default to black if nothing is selected

	return m_rgStandardColors[nItem];
}


BOOL CComboColorPicker::SelectColor( COLORREF color )
{
	for( int i = 0; i < GetCount(); i++ )
	{
		if( (COLORREF)GetItemData( i ) == color )
		{
			SetCurSel( i );
			return TRUE;
		}
	}

	// not found
	return FALSE;
}