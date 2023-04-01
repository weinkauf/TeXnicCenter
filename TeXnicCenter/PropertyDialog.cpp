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

#include "stdafx.h"
#include "PropertyDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// class CPropertyDialogPage
//-------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CPropertyDialogPage, CDialog)
	//{{AFX_MSG_MAP(CPropertyDialogPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CPropertyDialogPage::CPropertyDialogPage()
		: CDialog()
{
}

CPropertyDialogPage::CPropertyDialogPage(UINT unIDTemplate, UINT unIDCaption /*= 0*/)
		: CDialog(unIDTemplate)
{
	m_strCaption.LoadString(unIDCaption);
}

CPropertyDialogPage::CPropertyDialogPage(LPCTSTR lpszTemplateName, UINT unIDCaption /*= 0*/)
		: CDialog(lpszTemplateName)
{
	m_strCaption.LoadString(unIDCaption);
}

void CPropertyDialogPage::Construct(UINT unIDTemplate, UINT unIDCaption /*= 0*/)
{
	m_strCaption.LoadString(unIDCaption);

	// CDialog initialization
	m_pPropertyDialog = NULL;
	m_lpszTemplateName = MAKEINTRESOURCE(unIDTemplate);
	m_nIDHelp = unIDTemplate;
}

void CPropertyDialogPage::Construct(LPCTSTR lpszTemplateName, UINT unIDCaption /*= 0*/)
{
	m_strCaption.LoadString(unIDCaption);

	// CDialog initialization
	ASSERT(HIWORD(lpszTemplateName) == 0 ||
	       AfxIsValidString(lpszTemplateName));

	m_pPropertyDialog = NULL;
	m_lpszTemplateName = lpszTemplateName;
	if (HIWORD(m_lpszTemplateName) == 0)
		m_nIDHelp = LOWORD((DWORD)m_lpszTemplateName);
}

BOOL CPropertyDialogPage::Create(CTabCtrl *pParentWnd, CPropertyDialog *pPropertyDialog)
{
	m_pPropertyDialog = pPropertyDialog;
	return CDialog::Create(m_lpszTemplateName, static_cast<CWnd*>(pParentWnd));
}

BOOL CPropertyDialogPage::OnInitDialog()
{
	if (::IsAppThemed())
		::EnableThemeDialogTexture(m_hWnd, ETDT_ENABLETAB);

	CDialog::OnInitDialog();

	// get title
	if (m_strCaption.IsEmpty())
		GetWindowText(m_strCaption);

	// modify style
	ModifyStyle(0xFFFFFFFF, WS_CHILD | WS_CLIPSIBLINGS | WS_TABSTOP);
	ModifyStyleEx(0xFFFFFF, 0);

	return TRUE;
}

CString CPropertyDialogPage::GetTitle() const
{
	return m_strCaption;
}

void CPropertyDialogPage::SetModified(BOOL bChanged /*= TRUE*/)
{
	m_bIsModified = bChanged;

	if (m_pPropertyDialog)
		m_pPropertyDialog->SetModified(bChanged);
}

void CPropertyDialogPage::OnCancel()
{
}

BOOL CPropertyDialogPage::OnKillActive()
{
	return UpdateData(TRUE);
}

void CPropertyDialogPage::OnOK()
{
	SetModified(FALSE);
}

BOOL CPropertyDialogPage::OnSetActive()
{
	return TRUE;
}

BOOL CPropertyDialogPage::OnApply()
{
	OnOK();
	return TRUE;
}

void CPropertyDialogPage::OnReset()
{
}

BOOL CPropertyDialogPage::OnQueryCancel()
{
	return TRUE;
}

BOOL CPropertyDialogPage::IsModified()
{
	return m_bIsModified;
}


//-------------------------------------------------------------------
// class CPropertyDialog
//-------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CPropertyDialog, CDialog)
	ON_BN_CLICKED(ID_APPLY_NOW, OnApply)
	//{{AFX_MSG_MAP(CPropertyDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CPropertyDialog::CPropertyDialog()
		: CDialog()
{
	Init();
}

CPropertyDialog::CPropertyDialog(LPCTSTR lpszTemplateName, UINT unTabCtrlTemplateId, CWnd *pParentWnd /*= NULL*/, int nSelectPage /*= 0*/)
		: CDialog(lpszTemplateName, pParentWnd)
{
	Init();
	m_unTabCtrlTemplateId = unTabCtrlTemplateId;
	m_nActivePage = nSelectPage;
}

CPropertyDialog::CPropertyDialog(UINT unIDTemplate, UINT unTabCtrlTemplateId, CWnd *pParentWnd /*= NULL*/, int nSelectPage /*= 0*/)
		: CDialog(unIDTemplate, pParentWnd)
{
	Init();
	m_unTabCtrlTemplateId = unTabCtrlTemplateId;
	m_nActivePage = nSelectPage;
}

void CPropertyDialog::Init()
{
	m_unTabCtrlTemplateId = ~0U;
	m_pTabCtrl = NULL;
	m_pApplyNowButton = NULL;
	m_nActivePage = -1;
}

int CPropertyDialog::GetActiveIndex() const
{
	return m_nActivePage;
}

int CPropertyDialog::GetPageIndex(CPropertyDialogPage *pPage) const
{
	for (int i = 0; i < m_apPages.GetSize(); i++)
	{
		if (pPage == m_apPages[i])
			return i;
	}

	return -1;
}

int CPropertyDialog::GetPageCount() const
{
	return m_apPages.GetSize();
}

CPropertyDialogPage *CPropertyDialog::GetPage(int nPage) const
{
	ASSERT(nPage >= 0 && nPage < m_apPages.GetSize());
	if (nPage < 0 || nPage >= m_apPages.GetSize())
		return NULL;

	return m_apPages[nPage];
}

CPropertyDialogPage *CPropertyDialog::GetActivePage() const
{
	ASSERT(m_nActivePage >= 0 && m_nActivePage < m_apPages.GetSize());
	if (m_nActivePage < 0 || m_nActivePage >= m_apPages.GetSize())
		return NULL;

	return m_apPages[m_nActivePage];
}

BOOL CPropertyDialog::SetActivePage(int nPage)
{
	return ActivatePage(nPage);
}

BOOL CPropertyDialog::SetActivePage(CPropertyDialogPage *pPage)
{
	return SetActivePage(GetPageIndex(pPage));
}

CTabCtrl *CPropertyDialog::GetTabControl()
{
	return m_pTabCtrl;
}

BOOL CPropertyDialog::Create(LPCTSTR lpszTemplateName, UINT unTabCtrlTemplateId, CWnd *pParentWnd /*= NULL*/)
{
	m_unTabCtrlTemplateId = unTabCtrlTemplateId;
	return CDialog::Create(lpszTemplateName, pParentWnd);
}

BOOL CPropertyDialog::Create(UINT unIDTemplate, UINT unTabCtrlTemplateId, CWnd *pParentWnd /*= NULL*/)
{
	m_unTabCtrlTemplateId = unTabCtrlTemplateId;
	return CDialog::Create(unIDTemplate, pParentWnd);
}

INT_PTR CPropertyDialog::DoModal()
{
	return CDialog::DoModal();
}

void CPropertyDialog::AddPage(CPropertyDialogPage *pPage)
{
	ASSERT(pPage);
	int nPage = m_apPages.Add(pPage);

	if (IsWindow(m_hWnd) && nPage > -1)
		DynamicAddPage(nPage);
}

void CPropertyDialog::RemovePage(CPropertyDialogPage *pPage)
{
	RemovePage(GetPageIndex(pPage));
}

void CPropertyDialog::RemovePage(int nPage)
{
	ASSERT(nPage >= 0 && nPage < m_apPages.GetSize());
	if (nPage < 0 || nPage >= m_apPages.GetSize())
		return;

	if (IsWindow(m_hWnd))
		DynamicRemovePage(nPage);

	m_apPages.RemoveAt(nPage);
}

BOOL CPropertyDialog::ActivatePage(int nPage)
{
	if (!IsWindow(m_hWnd))
		return FALSE;

	if (nPage < 0 || nPage >= m_apPages.GetSize())
		return FALSE;

	if (!m_apPages[nPage] || !IsWindow(m_apPages[nPage]->m_hWnd))
		return FALSE;

	if (m_nActivePage >= 0 && m_nActivePage < m_apPages.GetSize() && m_apPages[m_nActivePage] && IsWindow(m_apPages[m_nActivePage]->m_hWnd))
	{
		if (!m_apPages[m_nActivePage]->OnKillActive())
			return FALSE;
		m_apPages[m_nActivePage]->ShowWindow(SW_HIDE);
	}

	// activate page
	if (!m_apPages[nPage]->OnSetActive())
	{
		ActivatePage(m_nActivePage);
		return FALSE;
	}

	m_apPages[nPage]->ShowWindow(SW_SHOW);
	m_pTabCtrl->SetCurSel(nPage);

	m_nActivePage = nPage;

	return TRUE;
}

BOOL CPropertyDialog::DynamicAddPage(int nIndex)
{
	ASSERT(nIndex >= 0 && nIndex < m_apPages.GetSize());
	if (nIndex < 0 || nIndex >= m_apPages.GetSize())
		return FALSE;

	CPropertyDialogPage *pPage = m_apPages[nIndex];

	ASSERT(pPage && !IsWindow(pPage->m_hWnd));
	if (!pPage || IsWindow(pPage->m_hWnd))
		return FALSE;

	ASSERT(IsWindow(m_hWnd));
	if (!IsWindow(m_hWnd))
		return FALSE;

	// create window
	if (!pPage->Create(m_pTabCtrl, this))
		return FALSE;

	// add page to tab control
	m_pTabCtrl->InsertItem(nIndex, pPage->GetTitle());

	// move page
	CRect rectDisplayArea;
	m_pTabCtrl->GetWindowRect(rectDisplayArea);
	m_pTabCtrl->AdjustRect(FALSE, rectDisplayArea);
	m_pTabCtrl->ScreenToClient(rectDisplayArea);
	pPage->MoveWindow(rectDisplayArea);
	/*
	pPage->SetWindowPos(
	        m_pTabCtrl,
	        rectDisplayArea.left, rectDisplayArea.top,
	        rectDisplayArea.Width(), rectDisplayArea.Height(),
	        0);
	 */

	return TRUE;
}

BOOL CPropertyDialog::DynamicRemovePage(int nIndex)
{
	ASSERT(nIndex >= 0 && nIndex < m_apPages.GetSize());
	if (nIndex < 0 || nIndex >= m_apPages.GetSize())
		return FALSE;

	CPropertyDialogPage *pPage = m_apPages[nIndex];

	ASSERT(pPage && IsWindow(pPage->m_hWnd));
	if (!pPage || !IsWindow(pPage->m_hWnd))
		return FALSE;

	ASSERT(IsWindow(m_hWnd));
	if (!IsWindow(m_hWnd))
		return FALSE;

	if (nIndex == m_nActivePage)
	{
		if (nIndex > 0)
			ActivatePage(nIndex - 1);
		else if (nIndex < m_apPages.GetUpperBound())
			ActivatePage(nIndex + 1);
	}

	pPage->DestroyWindow();

	return TRUE;
}

void CPropertyDialog::SetModified(BOOL bChanged)
{
	if (!m_pApplyNowButton || !IsWindow(m_pApplyNowButton->m_hWnd))
		return;

	if (bChanged)
		m_pApplyNowButton->EnableWindow();
	else
	{
		for (int i = 0; i < m_apPages.GetSize(); i++)
		{
			if (m_apPages[i]->IsModified())
			{
				m_pApplyNowButton->EnableWindow();
			}
		}

		// no modified pages
		m_pApplyNowButton->EnableWindow(FALSE);
	}
}

void CPropertyDialog::OnCancel()
{
	int i;

	for (i = 0; i < m_apPages.GetSize(); i++)
	{
		if (!m_apPages[i]->OnQueryCancel())
			return;
	}

	for (i = 0; i < m_apPages.GetSize(); i++)
		m_apPages[i]->OnCancel();

	CDialog::OnCancel();
}

void CPropertyDialog::OnOK()
{
	int i;
	for (i = 0; i < m_apPages.GetSize(); i++)
	{
		if (!m_apPages[i]->OnApply())
			return;
	}

	for (i = 0; i < m_apPages.GetSize(); i++)
		m_apPages[i]->OnOK();

	CDialog::OnOK();
}

BOOL CPropertyDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// get tab control
	m_pTabCtrl = reinterpret_cast<CTabCtrl*>(GetDlgItem(m_unTabCtrlTemplateId));
	ASSERT(m_pTabCtrl);
	if (!m_pTabCtrl)
		EndDialog(IDCANCEL);

	// look for 'Apply Now'-Button
	m_pApplyNowButton = GetDlgItem(ID_APPLY_NOW);
#ifdef _DEBUG
	if (m_pApplyNowButton)
		TRACE0("'Apply Now'-button found by CPropertyDialog\n");
#endif

	// add pages to dialog
	for (int i = 0; i < m_apPages.GetSize(); i++)
		DynamicAddPage(i);

	// activate page
	ActivatePage(m_nActivePage);

	return TRUE;
}

BOOL CPropertyDialog::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	int nCtrlId = (int)wParam;
	LPNMHDR lpnmhdr = (LPNMHDR)lParam;
	HWND hFrom = lpnmhdr->hwndFrom;
	UINT unFrom = lpnmhdr->idFrom;
	UINT unCode = lpnmhdr->code;

	UNUSED_ALWAYS(nCtrlId);
	UNUSED_ALWAYS(hFrom);

	// handle tab control
	if (unFrom == m_unTabCtrlTemplateId && unCode == TCN_SELCHANGE)
	{
		ASSERT(m_pTabCtrl && IsWindow(m_pTabCtrl->m_hWnd));
		ASSERT(m_pTabCtrl->m_hWnd == hFrom);

		ActivatePage(m_pTabCtrl->GetCurSel());
	}

	return CDialog::OnNotify(wParam, lParam, pResult);
}

void CPropertyDialog::OnApply()
{
	for (int i = 0; i < m_apPages.GetSize(); i++)
		m_apPages[i]->OnApply();
}