#include "stdafx.h"
#include "TeXnicCenter.h"
#include "LaTeXDocument.h"

#include "LaTeXView.h"
#include "global.h"
#include "textfilesavedialog.h"
#include "configuration.h"
#include "CodeBookmark.h"

// LaTeXDocument

IMPLEMENT_DYNCREATE(LaTeXDocument, CodeDocument)

LaTeXDocument::LaTeXDocument()
: save_copy_(false)
, EOL_mode_(-1)
{
}

LaTeXDocument::~LaTeXDocument()
{
}


BEGIN_MESSAGE_MAP(LaTeXDocument, CodeDocument)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, &LaTeXDocument::OnUpdateFileSave)
	ON_COMMAND(ID_FILE_SAVE_COPY_AS, &LaTeXDocument::OnFileSaveCopyAs)
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

BOOL LaTeXDocument::OnNewDocument()
{
	if (!CodeDocument::OnNewDocument())
		return FALSE;

	// *
	if (m_strPathName.IsEmpty())
		m_strPathName = m_strTitle;

	GetView()->GetCtrl().SetEOLMode(ToScintillaMode(CConfiguration::GetInstance()->m_nStandardFileFormat));

	return TRUE;
}

BOOL LaTeXDocument::OnOpenDocument(LPCTSTR lpszPathName)
{
    if (!CodeDocument::OnOpenDocument(lpszPathName))
        return FALSE;

	// Restore the bookmarks, if any
	if (CLaTeXProject* p = theApp.GetProject()) {
		BookmarkContainerType bookmarks;

		if (p->GetBookmarks(lpszPathName,bookmarks))
			SetBookmarks(bookmarks.begin(),bookmarks.end());
	}
    
    return TRUE;
}

BOOL LaTeXDocument::OnSaveDocument(LPCTSTR lpszPathName)
{
	DWORD result = SaveFile(lpszPathName,!save_copy_);
	BOOL b;

	if (result != ERROR_SUCCESS) {
		CString strMsg;
		strMsg.Format(STE_FILE_INUSE,
			AfxLoadString(IDS_SAVE),
			lpszPathName,
			AfxFormatSystemString(result));
		AfxMessageBox(strMsg,MB_ICONEXCLAMATION | MB_OK);

		save_copy_ = false;
		b = FALSE;
	}
	else {
		b = TRUE;

		save_copy_ = false;
		theApp.m_pMainWnd->SendMessage(WM_COMMAND,ID_DOCUMENT_SAVED);
	}

	SnapFileState();

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

	if (ShowSaveDialog(lpszPathName)) {
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

		if (EOL_mode_ == -1) // Not set yet
			EOL_mode_ = ToScintillaMode(mode);

		CTextFileSaveDialog dlg(
			bReplace ? AFX_IDS_SAVEFILE : AFX_IDS_SAVEFILECOPY,
			strExt,newName,
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOREADONLYRETURN,
			mode,CString(MAKEINTRESOURCE(STE_FILE_LATEXFILTER)));

		dlg.SetUseBOM(GetUseBOM());
		dlg.SetEncoding(GetEncoding());
		
		//Show the dialog
		if (dlg.DoModal() != IDOK) {
			//It was canceled - the PathName is not set.
			// Therefore, we use GetLastOpenedFolder.
			AfxSetLastDirectory(dlg.GetLastOpenedFolder());
			return false;
		}

		AfxSetLastDirectory(CPathTool::GetDirectory(dlg.GetPathName()));
		EOL_mode_ = ToScintillaMode(dlg.GetFileFormat());

		// Delay line ending conversion as long as possible
		// LaTeXView::SaveFile(HANDLE) will perform if it's succeeded so far

		SetEncoding(dlg.GetEncoding());
		SetUseBOM(dlg.GetUseBOM());

		save_copy_ = !bReplace;
		newName = dlg.GetPathName();
	}
	else
		EOL_mode_ = GetView()->GetCtrl().GetEOLMode();

	return CodeDocument::DoSave(newName,bReplace);
}

BOOL LaTeXDocument::GetNextLine(LPCTSTR &lpLine, int &nLength)
{
	AssertValid();

	if (current_line_ >= GetView()->GetCtrl().GetLineCount())
		return FALSE;

	static_cast<LaTeXView*>(GetView())->GetLineText(current_line_++);
	lpLine = line_;
	nLength = line_.GetLength();

	return TRUE;
}

void LaTeXDocument::DeleteContents()
{
	SetFilePath(NULL);

	if (CScintillaView* v = GetView())
		v->GetCtrl().ClearAll();

	current_line_ = 0;

	CodeDocument::DeleteContents();
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
		DWORD dwResult = LoadFile(GetPathName());

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
	CodeDocument::SetPathName(lpszPathName,bAddToMRU);
}

void LaTeXDocument::OnFileSaveCopyAs()
{
	DoSave(NULL,FALSE);
}

int LaTeXDocument::GetSavedEOLMode() const
{
	return EOL_mode_;
}

DWORD LaTeXDocument::SaveFile(HANDLE file)
{
	const int mode = GetSavedEOLMode();

	if (GetView()->GetCtrl().GetEOLMode() != mode) {
		GetView()->GetCtrl().SetEOLMode(mode);
		GetView()->GetCtrl().ConvertEOLs(mode);
	}

	return CodeDocument::SaveFile(file);
}

void LaTeXDocument::OnBookmarkAdded(const CodeBookmark& b)
{
	if (CLaTeXProject* p = theApp.GetProject())
		p->AddBookmark(GetFilePath(),b);
}

void LaTeXDocument::OnBookmarkRemoved(const CodeBookmark& b)
{
	if (CLaTeXProject* p = theApp.GetProject())
		p->RemoveBookmark(GetFilePath(),b);
}

void LaTeXDocument::OnRemovedAllBookmarks(void)
{
	if (CLaTeXProject* p = theApp.GetProject())
		p->RemoveAllBookmarks(GetFilePath());
}
