#if !defined(AFX_AUTOCOMPLETELISTBOX_H__D0F69718_12F4_432D_A83C_B31C668A219E__INCLUDED_)
#define AFX_AUTOCOMPLETELISTBOX_H__D0F69718_12F4_432D_A83C_B31C668A219E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <memory>

class CAutoCompleteListBox 
	: public CListBox
{
public:
	CAutoCompleteListBox();
	virtual ~CAutoCompleteListBox();

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	CSize GetItemMeasure(CDC* dc, LPCTSTR text);
	CSize GetLargestItemTextExtent();
	
protected:
	DECLARE_MESSAGE_MAP()

	INT_PTR OnToolHitTest(CPoint point, TOOLINFO * pTI) const;
	BOOL OnToolTipText(UINT id, NMHDR * pNMHDR, LRESULT * pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);

private:
	typedef std::map<CString, std::unique_ptr<CImageList> > ImageListMap;

	BOOL DrawBitmap(CDC* pDC, const CString& file, UINT index, const CRect& rect);
	CImageList* LoadBitmapFromFile(const CString& filename);

	ImageListMap image_cache_;

	CFont m_NormalFont;
	CFont m_BoldFont;
	CFont m_ItalicFont;
};

#endif // !defined(AFX_AUTOCOMPLETELISTBOX_H__D0F69718_12F4_432D_A83C_B31C668A219E__INCLUDED_)
