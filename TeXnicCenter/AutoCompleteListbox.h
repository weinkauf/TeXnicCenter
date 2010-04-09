#if !defined(AFX_AUTOCOMPLETELISTBOX_H__D0F69718_12F4_432D_A83C_B31C668A219E__INCLUDED_)
#define AFX_AUTOCOMPLETELISTBOX_H__D0F69718_12F4_432D_A83C_B31C668A219E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoCompleteListbox.h : header file

#include <map>
//

/////////////////////////////////////////////////////////////////////////////
// CAutoCompleteListBox window

class CAutoCompleteListBox : public CListBox
{
	typedef std::map<CString,HIMAGELIST> ImageCacheContainer;
	ImageCacheContainer image_cache_;

	//Types and Enums

// Construction
public:
	CAutoCompleteListBox();

// Attributes
public:
private:
	CFont m_NormalFont;
	CFont m_BoldFont;
	CFont m_ItalicFont;
	//CMapStringToPtr* m_PictureCache;

// Operations
private:
	BOOL DrawBitmap(CDC* pDC, const CString& file, UINT index, const CRect& rect);
	HIMAGELIST LoadBitmapFromFile(const CString& filename);

protected:
	INT_PTR OnToolHitTest(CPoint point, TOOLINFO * pTI) const;
	BOOL OnToolTipText(UINT id, NMHDR * pNMHDR, LRESULT * pResult);

public:
	///Returns the size of the largest item in pixels (measured with the bold font, so this is an upper estimate)
	CSize GetLargestItemTextExtent();

	// Overrides
	// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CAutoCompleteListBox)
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);

public:
	const CSize GetItemMeasure(CDC* dc, LPCTSTR text);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAutoCompleteListBox();

// Generated message map functions
protected:
	//{{AFX_MSG(CAutoCompleteListBox)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOCOMPLETELISTBOX_H__D0F69718_12F4_432D_A83C_B31C668A219E__INCLUDED_)
