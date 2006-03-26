#if !defined(AFX_AUTOCOMPLETELISTBOX_H__D0F69718_12F4_432D_A83C_B31C668A219E__INCLUDED_)
#define AFX_AUTOCOMPLETELISTBOX_H__D0F69718_12F4_432D_A83C_B31C668A219E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoCompleteListbox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAutoCompleteListbox window
const BMP_HEIGHT = 15;
const BMP_WIDTH = 16;

class CAutoCompleteListbox : public CListBox
{
// Construction
public:
	CAutoCompleteListbox();
	
// Attributes
public:
private:
	CFont				*m_BoldFont;
	CMapStringToPtr		*m_PictureCache;

// Operations
private:	
	BOOL DrawBitmap(CDC* pDC, CString file, UINT index, CRect rect);
	CBitmap* CAutoCompleteListbox::LoadBitmapFromFile(CString filename);
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoCompleteListbox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAutoCompleteListbox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAutoCompleteListbox)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOCOMPLETELISTBOX_H__D0F69718_12F4_432D_A83C_B31C668A219E__INCLUDED_)
