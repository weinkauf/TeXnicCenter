#if !defined(AFX_AUTOCOMPLETEDLG_H__E9ECE7DF_EC8F_4C71_B979_F8D5D62BBB13__INCLUDED_)
#define AFX_AUTOCOMPLETEDLG_H__E9ECE7DF_EC8F_4C71_B979_F8D5D62BBB13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <memory>

#include "StyleFileContainer.h"

/* Minimum keyword length to trigger listbox */
const int MINIMUM_KEYWORD_LENGTH = 2;

class CAutoCompleteListBox;
class CAutoCompleteListener;

class CAutoCompleteDlg : public CWnd
{
public:
	enum
	{
		ID_AUTOCOMPLETE = 1234
	};

	CAutoCompleteDlg(CStyleFileContainer *sfc, CWnd* pParent = NULL);
	~CAutoCompleteDlg();

	/** Select an item in the listbox by the zero-based position. See also CListBox::SetCurSel */
	void SetCurSel(int);
	/**
	        Updates listbox selection so that the best match for the given keyword is selected.

	        @param newKeyword Changed keyword

	        @return True, if a match was found and selection was updated successfully or FALSE, if
	        no match have been found.
	 */
	BOOL UpdateSelection(CString &newKeyword);
	/**
	        Forces the listbox to (re)initialize their content with possible completions.

	        @param keyword Keyword to search possible completions for

	        @return True, if two or more matches were found and listbox should be shown.
	 */
	BOOL InitWithKeyword(const CString &keyword);

	/**
	        Registers a listener (usually an edit window) to receive events of the listbox.
	        Passing NULL to this functions drops existing listeners.

	        @param listener Pointer to an instance of CAutoCompleteListener.
	 */
	void SetListener(CAutoCompleteListener *listener);

	///Adjusts the size and position of the dialog to match its content.
	void AdjustSizeAndPosition(const int EditorLineHeight);

	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	static int GetMinimumKeywordLength();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	BOOL Create(CWnd* pParentWnd);
	LRESULT OnFloatStatus(WPARAM wParam, LPARAM lParam);
	void MoveSelection(int delta);

private:
	friend class CAutoCompleteListBox;

	void ApplySelection(const TCHAR AdditionalInsertChar = 0);
	void CancelSelection();
	void Help();
	void DrawListItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void MeasureListItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	int CompareListItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	void MakeTransparent(int value);

	CString m_CurrentKeyword;
	CStyleFileContainer *m_Container;
	CAutoCompleteListener *m_Listener;
	BOOL m_Visible;
	const std::unique_ptr<CAutoCompleteListBox> m_Box;
	SharedObjectMap map;
};

/**
Abstract interface representing events of an auto complete listbox.

Windows which like to receive events of should implement CAutoCompleteListener
and register themselves via <tt>CAutoCompleteListBox#SetListener</tt>.

NOTE: Cursor left/right events will follow.

@see CAutoCompleteListBox, CLaTeXCommand

@author Oliver Wieland
 */

class CAutoCompleteListener
{
public:
	/**
	        Called, if user had confirmed a completion

	        @param Pointer to an instance of the corresponding command (maybe a command
	        or an environment).
	 */
	virtual void OnACCommandSelect(const CLaTeXCommand* cmd, const TCHAR AdditionalInsertChar = 0) = 0;
	/**
	        Called, if user had cancelled auto completion
	 */
	virtual void OnACCommandCancelled() = 0;
	/**
	        Called, if user has requested help (F1 key)

	        @param command Selected keyword
	 */
	virtual void OnACHelp(CString &command) = 0;
	/**
	        Called, if user pressed backspace, the edit window should delete the current TCHAR to sync with the
	        listbox selection
	 */
	virtual void OnACBackspace() = 0;
	/**
	        Called, if user typed a TCHAR (see documentation of WM_CHAR message for details). This event may be passed
	        to existing "OnChar..." handlers.
	 */
	virtual void OnACChar(UINT nKey, UINT nRepCount, UINT nFlags) = 0;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.



#endif // !defined(AFX_AUTOCOMPLETEDLG_H__E9ECE7DF_EC8F_4C71_B979_F8D5D62BBB13__INCLUDED_)
