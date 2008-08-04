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
#include "TeXnicCenter.h"
#include "ProfilePagePostprocessor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// class CProfilePagePostprocessor
//-------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CProfilePagePostprocessor, CProfilePage)
	ON_NOTIFY(BCGCELN_AFTERADDITEM, IDC_POSTPROCESSORLIST, OnItemAction)
	ON_NOTIFY(BCGCELN_BEFOREREMOVEITEM, IDC_POSTPROCESSORLIST, OnItemAction)
	ON_NOTIFY(BCGCELN_SELECTIONCHANGED, IDC_POSTPROCESSORLIST, OnItemAction)
	ON_NOTIFY(BCGCELN_AFTERRENAMEITEM, IDC_POSTPROCESSORLIST, OnItemAction)
	ON_NOTIFY(BCGCELN_AFTERMOVEITEMDOWN, IDC_POSTPROCESSORLIST, OnItemAction)
	ON_NOTIFY(BCGCELN_AFTERMOVEITEMUP, IDC_POSTPROCESSORLIST, OnItemAction)
END_MESSAGE_MAP()


CProfilePagePostprocessor::CProfilePagePostprocessor()
		: CProfilePage(CProfilePagePostprocessor::IDD),
		m_wndBrowsePostprocessor(IDC_EDIT_PPEXE, TRUE, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, CString((LPCTSTR)STE_COMPILER_FILTER)),
		m_wndArgumentsPh(IDR_POPUP_PLACEHOLDER_MAIN_OPT),
		m_wndInputPh(IDR_POPUP_PLACEHOLDER_MAIN_OPT),
		m_wndOutputPh(IDR_POPUP_PLACEHOLDER_MAIN_OPT),
		m_nSelectedItem(-1),
		m_pProfile(NULL)
{
	//{{AFX_DATA_INIT(CProfilePagePostprocessor)
	//}}AFX_DATA_INIT
}

void CProfilePagePostprocessor::UpdateControlStates()
{
	int nItem = m_nSelectedItem;

	if (m_pProfile)
		EnableAllControls();

	m_wndTitleExe.EnableWindow(nItem > -1);
	m_wndExe.EnableWindow(nItem > -1);
	m_wndBrowsePostprocessor.EnableWindow(nItem > -1);
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

void CProfilePagePostprocessor::RefillList()
{
	if (!IsWindow(m_wndList.m_hWnd))
		return;

	// clear list
	while (m_wndList.GetCount())
		m_wndList.RemoveItem(0);

	if (!m_pProfile)
		return;

	// fill list
	for (int i = 0; i < m_pProfile->GetPostProcessorArray().GetSize(); i++)
		m_wndList.AddItem(m_pProfile->GetPostProcessorArray().GetAt(i).GetTitle());
}

void CProfilePagePostprocessor::OnUpdateDataSet(CProfile *pProfile)
{
	m_pProfile = pProfile;

	if (IsWindow(m_hWnd))
	{
		UpdateControlStates();
		RefillList();
	}
}

void CProfilePagePostprocessor::OnAddItem(int nIndex)
{
	if (!m_pProfile)
		return;

	CPostProcessor pp(m_wndList.GetItemText(nIndex));
	m_pProfile->GetPostProcessorArray().InsertAt(nIndex,pp);
}

BOOL CProfilePagePostprocessor::OnRemoveItem(int nIndex)
{
	if (!m_pProfile)
		return TRUE;

	m_pProfile->GetPostProcessorArray().RemoveAt(nIndex);
	m_nSelectedItem = -1;
	return TRUE;
}

void CProfilePagePostprocessor::OnSelectionChanged()
{
	UpdateData();
	m_nSelectedItem = m_wndList.GetSelItem();
	UpdateControlStates();
	UpdateData(FALSE);
}

void CProfilePagePostprocessor::OnRenameItem(int nIndex)
{
	if (!m_pProfile)
		return;

	m_pProfile->GetPostProcessorArray()[nIndex].SetTitle(m_wndList.GetItemText(nIndex));
}

void CProfilePagePostprocessor::OnMoveItemUp(int nIndex)
{
	if (!m_pProfile)
		return;

	CPostProcessor pp0 = m_pProfile->GetPostProcessorArray()[nIndex];
	CPostProcessor pp1 = m_pProfile->GetPostProcessorArray()[nIndex + 1];
	m_pProfile->GetPostProcessorArray().SetAt(nIndex, pp1);
	m_pProfile->GetPostProcessorArray().SetAt(nIndex + 1, pp0);

	m_nSelectedItem = nIndex;
	UpdateData(FALSE);
}

void CProfilePagePostprocessor::OnMoveItemDown(int nIndex)
{
	if (!m_pProfile)
		return;

	CPostProcessor pp0 = m_pProfile->GetPostProcessorArray()[nIndex - 1];
	CPostProcessor pp1 = m_pProfile->GetPostProcessorArray()[nIndex];
	m_pProfile->GetPostProcessorArray().SetAt(nIndex - 1, pp1);
	m_pProfile->GetPostProcessorArray().SetAt(nIndex, pp0);

	m_nSelectedItem = nIndex;
	UpdateData(FALSE);
}

void CProfilePagePostprocessor::DoDataExchange(CDataExchange* pDX)
{
	CString strExecutable, strArguments, strInput, strOutput;
	int nPostProcessor = m_nSelectedItem;

	if (m_pProfile && nPostProcessor > -1 && nPostProcessor < m_pProfile->GetPostProcessorArray().GetSize() && !pDX->m_bSaveAndValidate)
	{
		// prepare data for controls
		CPostProcessor &pp = m_pProfile->GetPostProcessorArray()[nPostProcessor];

		strExecutable = pp.GetPath();
		strArguments = pp.GetArguments();
		strInput = pp.GetInputFile();
		strOutput = pp.GetOutputFile();
	}

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProfilePagePostprocessor)
	DDX_Control(pDX, IDC_EDIT_PPEXE, m_wndExe);
	DDX_Control(pDX, IDC_TITLE_ARGUMENTS, m_wndTitleArguments);
	DDX_Control(pDX, IDC_TITLE_OUTPUT, m_wndTitleOutput);
	DDX_Control(pDX, IDC_TITLE_INPUT, m_wndTitleInput);
	DDX_Control(pDX, IDC_TITLE_EXE, m_wndTitleExe);
	DDX_Control(pDX, IDC_POSTPROCESSORLIST, m_wndList);
	DDX_Control(pDX, IDC_INSERTPH_PPOUTPUT, m_wndOutputPh);
	DDX_Control(pDX, IDC_INSERTPH_PPINPUT, m_wndInputPh);
	DDX_Control(pDX, IDC_INSERTPH_PPARGUMENTS, m_wndArgumentsPh);
	DDX_Control(pDX, IDC_EDIT_PPOUTPUT, m_wndOutput);
	DDX_Control(pDX, IDC_EDIT_PPINPUT, m_wndInput);
	DDX_Control(pDX, IDC_EDIT_PPARGUMENTS, m_wndArguments);
	DDX_Control(pDX, IDC_BROWSE_POSTPROCESSOR, m_wndBrowsePostprocessor);
	DDX_Text(pDX, IDC_EDIT_PPEXE, strExecutable);
	DDX_Text(pDX, IDC_EDIT_PPARGUMENTS, strArguments);
	DDX_Text(pDX, IDC_EDIT_PPINPUT, strInput);
	DDX_Text(pDX, IDC_EDIT_PPOUTPUT, strOutput);
	//}}AFX_DATA_MAP

	if (m_pProfile && nPostProcessor > -1 && nPostProcessor < m_pProfile->GetPostProcessorArray().GetSize() && pDX->m_bSaveAndValidate)
	{
		// prepare data for controls
		CPostProcessor &pp = m_pProfile->GetPostProcessorArray()[nPostProcessor];

		pp.SetPath(strExecutable);
		pp.SetArguments(strArguments);
		pp.SetInputFile(strInput);
		pp.SetOutputFile(strOutput);
	}
}

BOOL CProfilePagePostprocessor::OnInitDialog()
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

void CProfilePagePostprocessor::OnItemAction(NMHDR *pNMHDR, LRESULT *pResult)
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
