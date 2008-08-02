#pragma once

class DialogTemplate : 
    public CDialogTemplate 
{
public:

    static UINT GetTemplateSize(const DLGTEMPLATE* pTemplate)
    {
        return CDialogTemplate::GetTemplateSize(pTemplate);
    }

    void Attach(LPDLGTEMPLATE lpTemplate)
    {
        ASSERT(!m_hTemplate);
        m_hTemplate = reinterpret_cast<HGLOBAL>(lpTemplate);
        m_dwTemplateSize = DialogTemplate::GetTemplateSize(lpTemplate);
    }
};