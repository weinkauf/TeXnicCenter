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

#include "stdafx.h"
#include "resource.h"
#include "ProfilePagePreprocessor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// class CProfilePagePreprocessor
//-------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CProfilePagePreprocessor, CProfilePage)
	ON_NOTIFY(BCGCELN_AFTERADDITEM, IDC_PREPROCESSORLIST, OnItemAction)
	ON_NOTIFY(BCGCELN_BEFOREREMOVEITEM, IDC_PREPROCESSORLIST, OnItemAction)
	ON_NOTIFY(BCGCELN_SELECTIONCHANGED, IDC_PREPROCESSORLIST, OnItemAction)
	ON_NOTIFY(BCGCELN_AFTERRENAMEITEM, IDC_PREPROCESSORLIST, OnItemAction)
	ON_NOTIFY(BCGCELN_AFTERMOVEITEMDOWN, IDC_PREPROCESSORLIST, OnItemAction)
	ON_NOTIFY(BCGCELN_AFTERMOVEITEMUP, IDC_PREPROCESSORLIST, OnItemAction)
END_MESSAGE_MAP()


CProfilePagePreprocessor::CProfilePagePreprocessor()
		: CProfilePage(CProfilePagePreprocessor::IDD),
		m_wndBrowsePreprocessor(IDC_EDIT_PRPEXE, TRUE, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, CString((LPCTSTR)STE_COMPILER_FILTER)),
		m_wndArgumentsPh(IDR_POPUP_PLACEHOLDER_MAIN_OPT),
		m_wndInputPh(IDR_POPUP_PLACEHOLDER_MAIN_OPT),
		m_wndOutputPh(IDR_POPUP_PLACEHOLDER_MAIN_OPT),
		m_nSelectedItem(-1),
		m_pProfile(NULL)
{
	//{{AFX_DATA_INIT(CProfilePagePreprocessor)
	//}}AFX_DATA_INIT
}

void CProfilePagePreprocessor::UpdateControlStates()
{
	int nItem = m_nSelectedItem;

	if (m_pProfile)
		EnableAllControls();

	m_wndTitleExe.EnableWindow(nItem > -1);
	m_wndExe.EnableWindow(nItem > -1);
	m_wndBrowsePreprocessor.EnableWindow(nItem > -1);
	m_wndTitleArguments.EnableWindow(nItem > -1);
	m_wndArguments.EnableWindow(nItem > -1);
	m_wndArgumentsPh.EnableWindow(nItem > -1);
	m_wndTitleInput.EnableWindow(nItem > -1);
	m_wndInput.EnableWindow(nItem > -1);
	m_wndInputPh.EnableWindow(nItem > -1);
	m_wndTitleOutput.EnableWindow(nItem > -1);
	m_wndOutput.EnableWindow(nItem > -1);
	m_wndOutputPh.EnableWindow(nItem > -1);

	if (!m_pProfile)
		EnableAllControls(FALSE);
}

void CProfilePagePreprocessor::RefillList()
{
	if (!IsWindow(m_wndList.m_hWnd))
		return;

	// clear list
	while (m_wndList.GetCount())
		m_wndList.RemoveItem(0);

	if (!m_pProfile)
		return;

	// fill list
	for (int i = 0; i < m_pProfile->GetPreProcessorArray().GetSize(); i++)
		m_wndList.AddItem(m_pProfile->GetPreProcessorArray().GetAt(i).GetTitle());
}

void CProfilePagePreprocessor::OnUpdateDataSet(CProfile *pProfile)
{
	m_pProfile = pProfile;

	if (IsWindow(m_hWnd))
	{
		UpdateControlStates();
		RefillList();
	}
}

void CProfilePagePreprocessor::OnAddItem(int nIndex)
{
	if (!m_pProfile)
		return;

	CPreProcessor pp(m_wndList.GetItemText(nIndex));
	m_pProfile->GetPreProcessorArray().InsertAt(nIndex,pp);
}

BOOL CProfilePagePreprocessor::OnRemoveItem(int nIndex)
{
	if (!m_pProfile)
		return TRUE;

	m_pProfile->GetPreProcessorArray().RemoveAt(nIndex);
	m_nSelectedItem = -1;
	return TRUE;
}

void CProfilePagePreprocessor::OnSelectionChanged()
{
	UpdateData();
	m_nSelectedItem = m_wndList.GetSelItem();
	UpdateControlStates();
	UpdateData(FALSE);
}

void CProfilePagePreprocessor::OnRenameItem(int nIndex)
{
	if (!m_pProfile)
		return;

	m_pProfile->GetPreProcessorArray()[nIndex].SetTitle(m_wndList.GetItemText(nIndex));
}

void CProfilePagePreprocessor::OnMoveItemUp(int nIndex)
{
	if (!m_pProfile)
		return;

	CPreProcessor pp0 = m_pProfile->GetPreProcessorArray()[nIndex];
	CPreProcessor pp1 = m_pProfile->GetPreProcessorArray()[nIndex + 1];
	m_pProfile->GetPreProcessorArray().SetAt(nIndex, pp1);
	m_pProfile->GetPreProcessorArray().SetAt(nIndex + 1, pp0);

	m_nSelectedItem = nIndex;
	UpdateData(FALSE);
}

void CProfilePagePreprocessor::OnMoveItemDown(int nIndex)
{
	if (!m_pProfile)
		return;

	CPreProcessor pp0 = m_pProfile->GetPreProcessorArray()[nIndex - 1];
	CPreProcessor pp1 = m_pProfile->GetPreProcessorArray()[nIndex];
	m_pProfile->GetPreProcessorArray().SetAt(nIndex - 1, pp1);
	m_pProfile->GetPreProcessorArray().SetAt(nIndex, pp0);

	m_nSelectedItem = nIndex;
	UpdateData(FALSE);
}

void CProfilePagePreprocessor::DoDataExchange(CDataExchange* pDX)
{
	CString strExecutable, strArguments, strInput, strOutput;
	int nPreProcessor = m_nSelectedItem;

	if (m_pProfile && nPreProcessor > -1 && nPreProcessor < m_pProfile->GetPreProcessorArray().GetSize() && !pDX->m_bSaveAndValidate)
	{
		// prepare data for controls
		CPreProcessor &pp = m_pProfile->GetPreProcessorArray()[nPreProcessor];

		strExecutable = pp.GetPath();
		strArguments = pp.GetArguments();
		strInput = pp.GetInputFile();
		strOutput = pp.GetOutputFile();
	}

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProfilePagePreprocessor)
	DDX_Control(pDX, IDC_EDIT_PRPEXE, m_wndExe);
	DDX_Control(pDX, IDC_TITLE_ARGUMENTS, m_wndTitleArguments);
	DDX_Control(pDX, IDC_TITLE_OUTPUT, m_wndTitleOutput);
	DDX_Control(pDX, IDC_TITLE_INPUT, m_wndTitleInput);
	DDX_Control(pDX, IDC_TITLE_EXE, m_wndTitleExe);
	DDX_Control(pDX, IDC_PREPROCESSORLIST, m_wndList);
	DDX_Control(pDX, IDC_INSERTPH_PRPOUTPUT, m_wndOutputPh);
	DDX_Control(pDX, IDC_INSERTPH_PRPINPUT, m_wndInputPh);
	DDX_Control(pDX, IDC_INSERTPH_PRPARGUMENTS, m_wndArgumentsPh);
	DDX_Control(pDX, IDC_EDIT_PRPOUTPUT, m_wndOutput);
	DDX_Control(pDX, IDC_EDIT_PRPINPUT, m_wndInput);
	DDX_Control(pDX, IDC_EDIT_PRPARGUMENTS, m_wndArguments);
	DDX_Control(pDX, IDC_BROWSE_PREPROCESSOR, m_wndBrowsePreprocessor);
	DDX_Text(pDX, IDC_EDIT_PRPEXE, strExecutable);
	DDX_Text(pDX, IDC_EDIT_PRPARGUMENTS, strArguments);
	DDX_Text(pDX, IDC_EDIT_PRPINPUT, strInput);
	DDX_Text(pDX, IDC_EDIT_PRPOUTPUT, strOutput);
	//}}AFX_DATA_MAP

	if (m_pProfile && nPreProcessor > -1 && nPreProcessor < m_pProfile->GetPreProcessorArray().GetSize() && pDX->m_bSaveAndValidate)
	{
		// prepare data for controls
		CPreProcessor &pp = m_pProfile->GetPreProcessorArray()[nPreProcessor];

		pp.SetPath(strExecutable);
		pp.SetArguments(strArguments);
		pp.SetInputFile(strInput);
		pp.SetOutputFile(strOutput);
	}
}

BOOL CProfilePagePreprocessor::OnInitDialog()
{
	CProfilePage::OnInitDialog();

	// initialize controls
	m_wndList.SetStandardButtons();
	m_wndArgumentsPh.AttachEditCtrl(&m_wndArguments);
	m_wndInputPh.AttachEditCtrl(&m_wndInput);
	m_wndOutputPh.AttachEditCtrl(&m_wndOutput);

	// fill list
	if (m_pProfile)
		RefillList();

	UpdateControlStates();

	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void CProfilePagePreprocessor::OnItemAction(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMBCGCEDITLISTBOX pHdr = (LPNMBCGCEDITLISTBOX)pNMHDR;

	*pResult = 0;

	switch (pHdr->hdr.code)
	{
		case BCGCELN_AFTERADDITEM:
			OnAddItem(pHdr->iItem);
			break;

		case BCGCELN_BEFOREREMOVEITEM:
			*pResult = !OnRemoveItem(pHdr->iItem);
			break;

		case BCGCELN_SELECTIONCHANGED:
			OnSelectionChanged();
			break;

		case BCGCELN_AFTERRENAMEITEM:
			OnRenameItem(pHdr->iItem);
			break;

		case BCGCELN_AFTERMOVEITEMUP:
			OnMoveItemUp(pHdr->iItem);
			break;

		case BCGCELN_AFTERMOVEITEMDOWN:
			OnMoveItemDown(pHdr->iItem);
			break;

		default:
			ASSERT(FALSE);
	}
}
