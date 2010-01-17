#include "stdafx.h"
#include "TeXnicCenter.h"
#include "LaTeXDocument.h"

#include "LaTeXView.h"
#include "global.h"
#include "textfilesavedialog.h"
#include "configuration.h"
#include "CodeBookmark.h"
#include "TextModulesDlg.h"

// LaTeXDocument

IMPLEMENT_DYNCREATE(LaTeXDocument, CodeDocument)

LaTeXDocument::LaTeXDocument()
{
}

LaTeXDocument::~LaTeXDocument()
{
}


BEGIN_MESSAGE_MAP(LaTeXDocument, CodeDocument)
	ON_COMMAND(ID_TEXTMODULES_DEFINE, &LaTeXDocument::OnTextModulesDefine)
	ON_UPDATE_COMMAND_UI(ID_TEXTMODULES_FIRST, &LaTeXDocument::OnUpdateTextModulesList)
END_MESSAGE_MAP()


// LaTeXDocument diagnostics

#ifdef _DEBUG
void LaTeXDocument::AssertValid() const
{
	CodeDocument::AssertValid();
}

#ifndef _WIN32_WCE
void LaTeXDocument::Dump(CDumpContext& dc) const
{
	CodeDocument::Dump(dc);
}
#endif
#endif //_DEBUG

// LaTeXDocument commands


void LaTeXDocument::OnTextModulesDefine()
{
	CTextModulesDlg dlg;

	//Give a copy of the TextModules
	dlg.m_tmGroup.RemoveAll();

	for (int i = 0; i < CConfiguration::GetInstance()->m_aTextModules.GetSize(); i++)
		dlg.m_tmGroup.InsertSorted(CConfiguration::GetInstance()->m_aTextModules[i]);

	if (dlg.DoModal() == IDOK) {
		CConfiguration::GetInstance()->m_aTextModules.RemoveAll();

		for (int i = 0; i < dlg.m_tmGroup.GetSize(); i++)
			CConfiguration::GetInstance()->m_aTextModules.InsertSorted(dlg.m_tmGroup[i]);
	}
}

void LaTeXDocument::OnUpdateTextModulesList(CCmdUI *pCmdUI)
{
	const CTextModuleGroup* ptmGroup = &CConfiguration::GetInstance()->m_aTextModules;

	if (CConfiguration::GetInstance()->m_bOptimizeMenuForVisuallyHandicappedUsers) {
		// handling for normal windows menus
		// (visually handicapped user mode)

		// update message from menu bar?
		//CMenu	*pMenu = pCmdUI->m_pSubMenu;
		CMenu* pMenu = pCmdUI->m_pMenu;

		if (!pMenu || !IsMenu(pMenu->GetSafeHmenu()))
			return;

		int i;

		//Delete old entries
		for (i = pMenu->GetMenuItemCount(); i >= 3; --i)
			pMenu->DeleteMenu(i,MF_BYPOSITION);

		//Anything defined at all?
		if (ptmGroup->IsEmpty()) {
			pCmdUI->SetText(AfxLoadString(IDS_TEXTMODULES_NONEDEFINED));
			pCmdUI->Enable(false);
			return;
		}

		//There is something to insert. So lets delete the last entry.
		ASSERT(pMenu->GetMenuItemCount() == 3);
		if (pMenu->GetMenuItemCount() > 2)
			pMenu->DeleteMenu(2,MF_BYPOSITION);

		for (i = 0; i < ptmGroup->GetSize(); i++) {
			//Get Name to display
			CString strDisplayName(ptmGroup->GetAt(i).Name);

			//Add number for the first ten entries
			if (i < 10) {
				CString strFormat;
				if (i == 9)
					strFormat.Format(_T("1&0 %s"),strDisplayName);
				else
					strFormat.Format(_T("&%d %s"),i + 1,strDisplayName);

				strDisplayName = strFormat;
			}

			pMenu->AppendMenu(MF_STRING,ID_TEXTMODULES_FIRST + i,strDisplayName);
		}
	}
	else {
		// update message from menu bar?
		if (!pCmdUI->m_pOther || !pCmdUI->m_pOther->IsKindOf(RUNTIME_CLASS(CMFCPopupMenuBar)))
			return;

		CMFCPopupMenuBar* pMenu = (CMFCPopupMenuBar*)pCmdUI->m_pOther;
		bool bChange = false;

		//Anything defined at all?
		if (ptmGroup->IsEmpty()) {
			pCmdUI->SetText(AfxLoadString(IDS_TEXTMODULES_NONEDEFINED));
			pCmdUI->Enable(FALSE);
			//Delete the rest
			//TODO: We have an assertion in the BCG later on,
			// if there is something to delete here.
			// Anyway, it works perfectly after this.
			// Someone with the full sources needs to have a look at it.
			while (pMenu->GetCount() > 3) {
				pMenu->RemoveButton(pMenu->GetCount() - 1);
				bChange = true;
			}

			return;
		}

		//Insert all entries
		int i, j;

		for (i = 0,j = 2; i < ptmGroup->GetSize(); i++, j++) {
			//Get Name to display
			CString strDisplayName(ptmGroup->GetAt(i).Name);

			//Add number for the first ten entries
			if (i < 10) {
				CString strFormat;

				if (i == 9)
					strFormat.Format(_T("1&0 %s"),strDisplayName);
				else
					strFormat.Format(_T("&%d %s"),i + 1,strDisplayName);

				strDisplayName = strFormat;
			}

			if (pMenu->GetCount() > j && pMenu->GetButtonText(j) != strDisplayName) {
				pMenu->SetButtonText(j,strDisplayName);
				bChange = true;
			}
			else if (pMenu->GetCount() <= j) {
				int nIndex = pMenu->InsertButton(CMFCToolBarMenuButton(ID_TEXTMODULES_FIRST + i,0,-1,strDisplayName),j);
				ASSERT(nIndex > -1);
				UNUSED_ALWAYS(nIndex);

				bChange = true;
			}
		}

		ASSERT(j == ptmGroup->GetSize() + 2);

		//Remove additional entries
		while (j < pMenu->GetCount()) {
			pMenu->RemoveButton(pMenu->GetCount() - 1);
			bChange = true;
		}
	}
}

const CString LaTeXDocument::GetExtensionFilter() const
{
	return CString(MAKEINTRESOURCE(STE_FILE_LATEXFILTER));
}