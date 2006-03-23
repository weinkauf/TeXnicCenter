#if !defined(AFX_AUTOCOMPLETELISTBOX_H__D0F69718_12F4_432D_A83C_B31C668A219E__INCLUDED_)
#define AFX_AUTOCOMPLETELISTBOX_H__D0F69718_12F4_432D_A83C_B31C668A219E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoCompleteListbox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAutoCompleteListbox window

class CAutoCompleteListbox : public CListBox
{
// Construction
public:
	CAutoCompleteListbox();

// Attributes
public:
private:
	CFont *m_BoldFont;

// Operations
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
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOCOMPLETELISTBOX_H__D0F69718_12F4_432D_A83C_B31C668A219E__INCLUDED_)
