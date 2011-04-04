#if !defined(AFX_ADVICE_H__13C50CB9_BDEF_406D_BD7B_1E0F6059A767__INCLUDED_)
#define AFX_ADVICE_H__13C50CB9_BDEF_406D_BD7B_1E0F6059A767__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Advice.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdvice window

class CAdvice : public CStatic
{
	CFont font_;
// Construction
public:
	CAdvice();

// Attributes
public:

// Operations
public:
	virtual BOOL Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = 0);

// Implementation
public:
	void SetWindowText(LPCTSTR text);
	~CAdvice();

	void EnableFocusChange();
	void DisableFocusChange();

// Generated message map functions
protected:
	//{{AFX_MSG(CAdvice)
	afx_msg void OnPaint();
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnActivateApp(BOOL bActive, DWORD dwThreadId);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	LRESULT OnFloatStatus(WPARAM wParam, LPARAM /*lParam*/);
private:
	BOOL m_IgnoreFocusChange;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADVICE_H__13C50CB9_BDEF_406D_BD7B_1E0F6059A767__INCLUDED_)
