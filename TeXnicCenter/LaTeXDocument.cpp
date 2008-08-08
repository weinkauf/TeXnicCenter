#include "stdafx.h"
#include "TeXnicCenter.h"
#include "LaTeXDocument.h"

#include "LaTeXView.h"
#include "global.h"
#include "textfilesavedialog.h"
#include "configuration.h"

#pragma region Line ending mode conversion functions

int ToScintillaMode(int m)
{
	int mode = SC_EOL_CRLF;

	switch (m) {
		case CRLF_STYLE_UNIX: mode = SC_EOL_LF; break;
		case CRLF_STYLE_MAC: mode = SC_EOL_CR; break;
	}

	return mode;
}

int FromScintillaMode(int m)
{
	int mode = CRLF_STYLE_DOS;

	switch (m) {
		case SC_EOL_CR: mode = CRLF_STYLE_MAC; break;
		case SC_EOL_LF: mode = CRLF_STYLE_UNIX; break;
	}

	return mode;
}

#pragma endregion


// LaTeXDocument

IMPLEMENT_DYNCREATE(LaTeXDocument, CScintillaDoc)

LaTeXDocument::LaTeXDocument()
: m_bSaveCopy(false)
, m_nCRLFMode(-1)
{
}

BOOL LaTeXDocument::OnNewDocument()
{
	if (!CScintillaDoc::OnNewDocument())
		return FALSE;

    // *
    if (m_strPathName.IsEmpty())
        m_strPathName = m_strTitle;

	GetView()->GetCtrl().SetEOLMode(ToScintillaMode(CConfiguration::GetInstance()->m_nStandardFileFormat));

	return TRUE;
}

LaTeXDocument::~LaTeXDocument()
{
}


BEGIN_MESSAGE_MAP(LaTeXDocument, CScintillaDoc)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, &LaTeXDocument::OnUpdateFileSave)
	ON_COMMAND(ID_FILE_SAVE_COPY_AS, &LaTeXDocument::OnFileSaveCopyAs)
END_MESSAGE_MAP()


// LaTeXDocument diagnostics

#ifdef _DEBUG
void LaTeXDocument::AssertValid() const
{
	CScintillaDoc::AssertValid();
}

#ifndef _WIN32_WCE
void LaTeXDocument::Dump(CDumpContext& dc) const
{
	CScintillaDoc::Dump(dc);
}
#endif
#endif //_DEBUG

// LaTeXDocument commands

void LaTeXDocument::SetModifiedFlag(BOOL modified)
{
    if (m_bModified != modified) {
        const TCHAR ch = _T('*');
        CString title = GetTitle();

        if (modified)
            title += ch;
        else
            title.TrimRight(ch);

        SetTitle(title);
    }

    __super::SetModifiedFlag(modified);
}

BOOL LaTeXDocument::OnOpenDocument(LPCTSTR lpszPathName)
{
    if (!CScintillaDoc::OnOpenDocument(lpszPathName))
        return FALSE;
    
    return TRUE;
}

void LaTeXDocument::SetErrorMark(int /*line*/)
{
}

BOOL LaTeXDocument::OnSaveDocument(LPCTSTR lpszPathName)
{
	DWORD result = static_cast<LaTeXView*>(GetView())->SaveToFile(lpszPathName,!m_bSaveCopy);
	BOOL b;

	if (result != ERROR_SUCCESS) {
		CString strMsg;
		strMsg.Format(STE_FILE_INUSE,
			AfxLoadString(IDS_SAVE),
			lpszPathName,
			AfxFormatSystemString(result));
		AfxMessageBox(strMsg,MB_ICONEXCLAMATION | MB_OK);

		m_bSaveCopy = false;
		b = FALSE;
	}
	else {
		b = TRUE;

		m_bSaveCopy = false;
		theApp.m_pMainWnd->SendMessage(WM_COMMAND,ID_DOCUMENT_SAVED);
	}

	return TRUE;
}

void LaTeXDocument::CheckForFileChanges()
{
	WORD wChanges = GetFileChanges();

	if (wChanges & chReadOnly)
		UpdateReadOnlyFlag();
	else if (wChanges)
		UpdateTextBufferOnExternalChange();
}

void LaTeXDocument::OnUpdateFileSave(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(IsModified());
}

BOOL LaTeXDocument::DoSave(LPCTSTR lpszPathName, BOOL bReplace /*= TRUE*/)
{
	CString newName(lpszPathName);

	if (newName.IsEmpty()) {
		CDocTemplate* pTemplate = GetDocTemplate();
		ASSERT(pTemplate != NULL);

		CString strExt;
		pTemplate->GetDocString(strExt,CDocTemplate::filterExt);
		ASSERT(!strExt.IsEmpty() && strExt[0] == _T('.'));

		newName = m_strPathName;

		if (bReplace && newName.IsEmpty()) {
			newName = m_strTitle;
			// check for dubious filename
			int iBad = newName.FindOneOf(_T(" #%;/\\"));
			if (iBad != -1)
				newName.ReleaseBuffer(iBad);

			// append the default suffix if there is one
			if (!strExt.IsEmpty())
				newName += strExt;
		}

		int mode = FromScintillaMode(GetView()->GetCtrl().GetEOLMode());

		if (m_nCRLFMode == -1) // Not set yet
			m_nCRLFMode = ToScintillaMode(mode);

		CTextFileSaveDialog dlg(
			bReplace ? AFX_IDS_SAVEFILE : AFX_IDS_SAVEFILECOPY,
			strExt,newName,
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOREADONLYRETURN,
			mode,CString(MAKEINTRESOURCE(STE_FILE_LATEXFILTER)));

		//Show the dialog
		if (dlg.DoModal() != IDOK) {
			//It was canceled - the PathName is not set.
			// Therefore, we use GetLastOpenedFolder.
			AfxSetLastDirectory(dlg.GetLastOpenedFolder());
			return false;
		}

		AfxSetLastDirectory(CPathTool::GetDirectory(dlg.GetPathName()));
		m_nCRLFMode = ToScintillaMode(dlg.GetFileFormat());

		// Delay line ending conversion as long as possible
		// LaTeXView::SaveToFile(HANDLE) will perform if it's succeeded so far

		m_bSaveCopy = !bReplace;
		newName = dlg.GetPathName();
	}
	else
		m_nCRLFMode = GetView()->GetCtrl().GetEOLMode();

	return CDocument::DoSave(newName,bReplace);
}

BOOL LaTeXDocument::GetNextLine(LPCTSTR &lpLine, int &nLength)
{
	AssertValid();

	if (m_nCurrentLine >= GetView()->GetCtrl().GetLineCount())
		return FALSE;

	static_cast<LaTeXView*>(GetView())->GetLineText(m_nCurrentLine++);
	lpLine = line_;
	nLength = line_.GetLength();

	return TRUE;
}

void LaTeXDocument::DeleteContents()
{
	SetFilePath(NULL);

	if (CScintillaView* v = GetView())
		v->GetCtrl().ClearAll();

	m_nCurrentLine = 0;

	CDocument::DeleteContents();
}

void LaTeXDocument::Delete()
{
}

void LaTeXDocument::UpdateReadOnlyFlag()
{
	CFileStatus fs;

	if (CFile::GetStatus(GetPathName(),fs))
		GetView()->GetCtrl().SetReadOnly(fs.m_attribute & CFile::readOnly);
}

void LaTeXDocument::UpdateTextBufferOnExternalChange()
{
	CString strMsg;
	int nResult;

	if (IsModified()) {
		strMsg.Format(STE_FILE_EXTERNALCHANGEEX,GetPathName());
		nResult = AfxMessageBox(strMsg,MB_ICONEXCLAMATION | MB_YESNO);
	}
	else {
		strMsg.Format(STE_FILE_EXTERNALCHANGE,GetPathName());
		nResult = AfxMessageBox(strMsg,MB_ICONINFORMATION | MB_YESNO);
	}

	if (nResult == IDYES) {
		DWORD dwResult = static_cast<LaTeXView*>(GetView())->LoadFromFile(GetPathName());

		if (dwResult != ERROR_SUCCESS) {
			strMsg.Format(STE_FILE_INUSE,
				AfxLoadString(IDS_OPEN),
				GetPathName(),
				AfxFormatSystemString(dwResult));
			AfxMessageBox(strMsg,MB_ICONINFORMATION | MB_OK);
			GetView()->GetCtrl().SetReadOnly(TRUE);
		}
	}
}

void LaTeXDocument::SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU)
{
	SetFilePath(lpszPathName);
	CDocument::SetPathName(lpszPathName,bAddToMRU);
}

void LaTeXDocument::OnFileSaveCopyAs()
{
	DoSave(NULL,FALSE);
}

int LaTeXDocument::GetSavedEOLMode() const
{
	return m_nCRLFMode;
}
