#pragma once

class FontOccManager : public COccManager {
    LOGFONT lf_;
    WORD def_size_;
    bool valid_;
public:
    const DLGTEMPLATE* PreCreateDialog(_AFX_OCC_DIALOG_INFO* pOccDialogInfo, const DLGTEMPLATE* pOrigTemplate);
    FontOccManager(void);
};

const LOGFONT GetDisplayFont();
void GetDisplayFont(LOGFONT &lf, WORD & wDefSize);
void FixPropSheetFont(PROPSHEETHEADER& psh, CPtrArray& pages);

inline HFONT CreateDefaultDisplayFont()
{
    const LOGFONT lf = GetDisplayFont();
    return ::CreateFontIndirect(&lf);
}