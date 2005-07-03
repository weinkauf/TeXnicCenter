#if !defined(AFX_ADVICE_H__13C50CB9_BDEF_406D_BD7B_1E0F6059A767__INCLUDED_)
#define AFX_ADVICE_H__13C50CB9_BDEF_406D_BD7B_1E0F6059A767__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Advice.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdvice window
const COLORREF ADVICE_BK_COLOR = RGB(0xC0, 0xFF, 0xC0);

class CAdvice : public CStatic
{
// Construction
public:
	CAdvice();

// Attributes
public:

// Operations
public:
	virtual BOOL Create(LPCTSTR lpszText, DWORD dwStyle,const RECT& rect, CWnd* pParentWnd, UINT nID=0);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdvice)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetWindowText(LPCTSTR text);
	virtual ~CAdvice();
	void EnableFocusChange() {m_IgnoreFocusChange = FALSE;}
	void DisableFocusChange() {m_IgnoreFocusChange = TRUE;}

	// Generated message map functions
protected:
	//{{AFX_MSG(CAdvice)
	afx_msg void OnPaint();
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	BOOL m_IgnoreFocusChange;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADVICE_H__13C50CB9_BDEF_406D_BD7B_1E0F6059A767__INCLUDED_)
