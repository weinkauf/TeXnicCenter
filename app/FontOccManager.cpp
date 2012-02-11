#include "stdafx.h"
#include "FontOccManager.h"

#ifndef _SIZEOF_STRUCT
#define _SIZEOF_STRUCT(structname, member)  (((int)((LPBYTE)(&((structname*)0)->member) - ((LPBYTE)((structname*)0)))) + sizeof(((structname*)0)->member))
#endif

#if !defined(_WIN32_WCE) && (WINVER >= 0x0600) && !defined(NONCLIENTMETRICS_V1_SIZE)
#define NONCLIENTMETRICS_V1_SIZE   _SIZEOF_STRUCT(NONCLIENTMETRICS, lfMessageFont)
#endif // !defined(_WIN32_WCE) && (WINVER >= 0x0600) && !defined(NONCLIENTMETRICS_V1_SIZE)

const LOGFONT GetDisplayFont()
{
	NONCLIENTMETRICS ncm;
	ncm.cbSize = NONCLIENTMETRICS_V1_SIZE;

	if (!::SystemParametersInfo(SPI_GETNONCLIENTMETRICS,NONCLIENTMETRICS_V1_SIZE,&ncm,0))
	{
		// Fallback
		CFont font;
		font.Attach(::GetStockObject(DEFAULT_GUI_FONT));
		font.GetLogFont(&ncm.lfMessageFont);
		font.Detach();
	}

	return ncm.lfMessageFont;
}

void GetDisplayFont(LOGFONT &lf,WORD & wDefSize)
{
	lf = GetDisplayFont();

	CWindowDC dc(0);

	if (lf.lfHeight < 0)
		lf.lfHeight = -lf.lfHeight;

	wDefSize = static_cast<WORD>(::MulDiv(lf.lfHeight,72,dc.GetDeviceCaps(LOGPIXELSY)));
}

const DLGTEMPLATE* FontOccManager::PreCreateDialog(_AFX_OCC_DIALOG_INFO* pOccDialogInfo,const DLGTEMPLATE* pOrigTemplate)
{
	const DLGTEMPLATE *lpNewTemplate = COccManager::PreCreateDialog(pOccDialogInfo,pOrigTemplate);

	if (/*LOBYTE(GetVersion()) >= 6 &&*/!pOccDialogInfo->m_pNewTemplate)
	{
		CDialogTemplate temp(lpNewTemplate);

		if (!valid_)
		{
			GetDisplayFont(lf_,def_size_);
			valid_ = true;
		}

		temp.SetFont(lf_.lfFaceName,def_size_);
		lpNewTemplate = static_cast<LPDLGTEMPLATE>(temp.Detach());
	}

	return lpNewTemplate;
}

FontOccManager::FontOccManager()
		: valid_(false),lf_()
{
}

void FixPropSheetFont(PROPSHEETHEADER& psh,CPtrArray& pages)
{
	LOGFONT lf;
	WORD defsize;
	GetDisplayFont(lf,defsize);

	PROPSHEETPAGE* ppsp = const_cast<PROPSHEETPAGE*>(psh.ppsp);

	for (UINT i = 0; i < psh.nPages; ++i)
	{
		CPropertyPage* page = static_cast<CPropertyPage*>(pages[i]);
		ASSERT(ppsp->dwFlags & PSP_DLGINDIRECT && ppsp->pResource);

		CDialogTemplate t(const_cast<LPDLGTEMPLATE>(ppsp->pResource));
		t.SetFont(lf.lfFaceName,defsize);
		ppsp->pResource = static_cast<LPDLGTEMPLATE>(t.Detach());

		(BYTE*&) ppsp += page->m_psp.dwSize;
	}
}