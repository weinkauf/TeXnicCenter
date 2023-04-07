/********************************************************************
 *
 * This file is part of the TeXnicCenter-system
 *
 * Copyright (C) 1999-2000 Sven Wiegand
 * Copyright (C) 2000-$CurrentYear$ ToolsCenter
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * If you have further questions or if you want to support
 * further TeXnicCenter development, visit the TeXnicCenter-homepage
 *
 *    http://www.ToolsCenter.org
 *
 *********************************************************************/

/********************************************************************
 *
 * $Id$
 *
 ********************************************************************/

#if !defined(AFX_PROPERTYDIALOG_H__488E4023_075E_11D5_A222_006097239934__INCLUDED_)
#define AFX_PROPERTYDIALOG_H__488E4023_075E_11D5_A222_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
Provides a property page based on a template.

@ingroup dialogs

@see CPropertyDialog

@author Sven Wiegand
 */
class CPropertyDialogPage : public CDialog
{
	friend class CPropertyDialog;

// construction/destruction
public:
	CPropertyDialogPage();
	CPropertyDialogPage(UINT unIDTemplate, UINT unIDCaption = 0);
	CPropertyDialogPage(LPCTSTR lpszTemplateName, UINT unIDCaption = 0);

	//TODO: This code seems unused, but it does do something with the helpID.
	// We need help IDs for the property pages so that users understand the
	// options for defining profiles.
	void Construct(UINT unIDTemplate, UINT unIDCaption = 0);
	void Construct(LPCTSTR lpszTemplateName, UINT unIDCaption = 0);

// operations
public:
	void SetModified(BOOL bChanged = TRUE);

// overridables
public:
	virtual void OnCancel();
	virtual BOOL OnKillActive();
	virtual void OnOK();
	virtual BOOL OnSetActive();
	virtual BOOL OnApply();
	virtual void OnReset();
	virtual BOOL OnQueryCancel();

// implementation
protected:
	/** Called by CPropertyDialog to create the page */
	BOOL Create(CTabCtrl *pParentWnd, class CPropertyDialog *pPropertyDialog);

	/**
	Called by CPropertyDialog after creation to retrieve the title to
	display on the tab for this page
	 */
	CString GetTitle() const;

	/**
	Called by the CPropertyDialog to determine if data on the page has
	changed
	 */
	BOOL IsModified();

// message handlers
protected:
	//{{AFX_MSG(CPropertyDialogPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// attributes
private:
	/** Pointer to the parent dialog or NULL */
	class CPropertyDialog *m_pPropertyDialog;

	/** Title of the page */
	CString m_strCaption;

	/** TRUE if there is changed data on the page, FALSE otherwise */
	BOOL m_bIsModified;
};

/**
This class provides a normal dialog based on a template, which can
contain property pages (see CPropertyDialogPage).

It is similar to CPropertySheet with the difference that you can
place additional controls in the sheet.

@ingroup dialogs

@author Sven Wiegand
 */
class CPropertyDialog : public CDialog
{
	friend CPropertyDialogPage;

// construction/destruction
public:
	CPropertyDialog();
	CPropertyDialog(LPCTSTR lpszTemplateName, UINT unTabCtrlTemplateId, CWnd *pParentWnd = NULL, int nSelectPage = 0);
	CPropertyDialog(UINT unIDTemplate, UINT unTabCtrlTemplateId, CWnd *pParentWnd = NULL, int nSelectPage = 0);

// attribute operationen
public:
	int GetActiveIndex() const;
	int GetPageIndex(CPropertyDialogPage *pPage) const;
	int GetPageCount() const;
	CPropertyDialogPage *GetPage(int nPage) const;
	CPropertyDialogPage *GetActivePage() const;
	BOOL SetActivePage(int nPage);
	BOOL SetActivePage(CPropertyDialogPage *pPage);
	CTabCtrl *GetTabControl();

// operations
public:
	BOOL Create(LPCTSTR lpszTemplateName, UINT unTabCtrlTemplateId, CWnd *pParentWnd = NULL);
	BOOL Create(UINT unIDTemplate, UINT unTabCtrlTemplateId, CWnd *pParentWnd = NULL);
	INT_PTR DoModal();
	void AddPage(CPropertyDialogPage *pPage);
	void RemovePage(CPropertyDialogPage *pPage);
	void RemovePage(int nPage);

// Overridings
protected:
//{{AFX_VIRTUAL(CPropertyDialog)
protected:
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// implementation
protected:
	void Init();
	BOOL ActivatePage(int nPage);
	BOOL DynamicAddPage(int nIndex);
	BOOL DynamicRemovePage(int nIndex);

// communication interface to CPropertyDialogPage
protected:
	/** Called by a page if its SetModified()-method has been called */
	void SetModified(BOOL bChanged);

// message handlers
protected:
	afx_msg void OnApply();
	//{{AFX_MSG(CPropertyDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// attributes
protected:
	UINT m_unTabCtrlTemplateId;
	CTabCtrl *m_pTabCtrl;
	CArray<CPropertyDialogPage*, CPropertyDialogPage*> m_apPages;
	int m_nActivePage;

	/**
	Pointer to the 'Apply Now'-button if there is one, or NULL
	otherwise (ID_APPLY_NOW).
	 */
	CWnd *m_pApplyNowButton;
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_PROPERTYDIALOG_H__488E4023_075E_11D5_A222_006097239934__INCLUDED_
