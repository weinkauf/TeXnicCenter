#pragma once

#include "FontOccManager.h"

class PropertyPage : public CPropertyPage {
    CDialogTemplate res_;

public:

    PropertyPage(UINT id)
    : CPropertyPage(id)
    {
        //AllocPSP(sizeof(PROPSHEETPAGE));
        //CommonConstruct(0,0);

        //m_psp.dwFlags |= PSP_DLGINDIRECT;

        //VERIFY(res_.Load(MAKEINTRESOURCE(id)));

        //LOGFONT lf; WORD size;

        //GetDisplayFont(lf,size);
        //res_.SetFont(lf.lfFaceName,size);

        //m_psp.pResource = static_cast<LPDLGTEMPLATE>(::GlobalLock(res_.m_hTemplate));

    }
};