#pragma once

// ThemedCtrl

class ThemedCtrl : 
    public ATL::CWindowImpl<ThemedCtrl>
{
    bool theming_supported_;
    HTHEME m_hTheme;

    BEGIN_MSG_MAP(ThemedCtrl)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MESSAGE_HANDLER(WM_NCPAINT, OnNcPaint)
    END_MSG_MAP()

public:
    LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnNcPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    void Initialize();
};