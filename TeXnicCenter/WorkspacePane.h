#pragma once

class WorkspacePane : public CDockablePane
{
	DECLARE_DYNAMIC(WorkspacePane)
	CWnd* client_;

public:
	WorkspacePane();

protected:
	DECLARE_MESSAGE_MAP()

public:
	void SetClient(CWnd* p);

private:
	void AdjustLayout(const CRect& rc);
	void AdjustLayout(void);

protected:
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);

public:
	CWnd* GetClient() const;	
};


