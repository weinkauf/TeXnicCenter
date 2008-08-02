#pragma once

class PrivateToolBar : public CMFCToolBar
{
    void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
    {
        CMFCToolBar::OnUpdateCmdUI(static_cast<CFrameWnd*>(GetOwner()), bDisableIfNoHndler);
    }

    BOOL AllowShowOnList() const { return FALSE; /* Don't show the toolbar in the customize dialog */ }

public:
    void AdjustStyle()
    {
        DWORD style = GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY;
        style &= ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | 
            CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT);

        SetPaneStyle(style);
    }
};