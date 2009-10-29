// LaTeXDocumentBase.cpp : implementation file
//

#include "stdafx.h"
#include "TeXnicCenter.h"

#include "LaTeXView.h"
#include "global.h"
#include "textfilesavedialog.h"
#include "configuration.h"
#include "CodeBookmark.h"
#include "CodeDocument.h"
#include "LaTeXDocumentBase.h"


// LaTeXDocumentBase

IMPLEMENT_DYNAMIC(LaTeXDocumentBase, CodeDocument)


LaTeXDocumentBase::LaTeXDocumentBase()
: save_copy_(false)
, EOL_mode_(-1)
{
}

BOOL LaTeXDocumentBase::OnNewDocument()
{
	if (!CodeDocument::OnNewDocument())
		return FALSE;

	// *
	if (m_strPathName.IsEmpty())
		m_strPathName = m_strTitle;

	GetView()->GetCtrl().SetEOLMode(ToScintillaMode(CConfiguration::GetInstance()->m_nStandardFileFormat));

	return TRUE;
}


LaTeXDocumentBase::~LaTeXDocumentBase()
{
}


BEGIN_MESSAGE_MAP(LaTeXDocumentBase, CodeDocument)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, &LaTeXDocumentBase::OnUpdateFileSave)
	ON_COMMAND(ID_FILE_SAVE_COPY_AS, &LaTeXDocumentBase::OnFileSaveCopyAs)
END_MESSAGE_MAP()


// LaTeXDocumentBase diagnostics

#ifdef _DEBUG
void LaTeXDocumentBase::AssertValid() const
{
	CodeDocument::AssertValid();
}

#ifndef _WIN32_WCE
void LaTeXDocumentBase::Dump(CDumpContext& dc) const
{
	CodeDocument::Dump(dc);
}
#endif
#endif //_DEBUG


// LaTeXDocumentBase commands

void LaTeXDocumentBase::DeleteContents()
{
	SetFilePath(NULL);

	if (CScintillaView* v = GetView())
		v->GetCtrl().ClearAll();

	current_line_ = 0;

	CodeDocument::DeleteContents();
}

void LaTeXDocumentBase::Delete()
{
}

void LaTeXDocumentBase::UpdateReadOnlyFlag()
{
	CFileStatus fs;

	if (CFile::GetStatus(GetPathName(),fs))
		GetView()->GetCtrl().SetReadOnly(fs.m_attribute & CFile::readOnly);
}


void LaTeXDocumentBase::CheckForFileChanges()
{
	WORD wChanges = GetFileChanges();

	if (wChanges & chReadOnly)
		UpdateReadOnlyFlag();
	else if (wChanges)
		UpdateTextBufferOnExternalChange();
}

void LaTeXDocumentBase::OnUpdateFileSave(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(IsModified());
}

BOOL LaTeXDocumentBase::DoSave(LPCTSTR lpszPathName, BOOL bReplace /*= TRUE*/)
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

BOOL LaTeXDocumentBase::GetNextLine(LPCTSTR &lpLine, int &nLength)
{
	AssertValid();

	if (current_line_ >= GetView()->GetCtrl().GetLineCount())
		return FALSE;

	static_cast<LaTeXView*>(GetView())->GetLineText(current_line_++);
	lpLine = line_;
	nLength = line_.GetLength();

	return TRUE;
}

void LaTeXDocumentBase::UpdateTextBufferOnExternalChange()
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

void LaTeXDocumentBase::SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU)
{
	SetFilePath(lpszPathName);
	CodeDocument::SetPathName(lpszPathName,bAddToMRU);
}

void LaTeXDocumentBase::OnFileSaveCopyAs()
{
	DoSave(NULL,FALSE);
}

int LaTeXDocumentBase::GetSavedEOLMode() const
{
	return EOL_mode_;
}

DWORD LaTeXDocumentBase::SaveFile(HANDLE file, bool throw_on_invalid_sequence)
{
	const int mode = GetSavedEOLMode();

	if (GetView()->GetCtrl().GetEOLMode() != mode) {
		GetView()->GetCtrl().SetEOLMode(mode);
		GetView()->GetCtrl().ConvertEOLs(mode);
	}

	return CodeDocument::SaveFile(file,throw_on_invalid_sequence);
}

void LaTeXDocumentBase::OnBookmarkAdded(const CodeBookmark& b)
{
	if (CLaTeXProject* p = theApp.GetProject())
		p->AddBookmark(GetFilePath(),b);
}

void LaTeXDocumentBase::OnBookmarkRemoved(const CodeBookmark& b)
{
	if (CLaTeXProject* p = theApp.GetProject())
		p->RemoveBookmark(GetFilePath(),b);
}

void LaTeXDocumentBase::OnRemovedAllBookmarks(void)
{
	if (CLaTeXProject* p = theApp.GetProject())
		p->RemoveAllBookmarks(GetFilePath());
}

void LaTeXDocumentBase::OnCloseDocument()
{
	if (GetView()) { // Is the view attached?
		if (CLaTeXProject* p = theApp.GetProject())
			p->SetFoldingPoints(GetPathName(),GetContractedFoldingPoints());
	}

	CodeDocument::OnCloseDocument();
}

BOOL LaTeXDocumentBase::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CodeDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	if (CLaTeXProject* p = theApp.GetProject()) {
		BookmarkContainerType bookmarks;

		// Restore the bookmarks, if any
		if (p->GetBookmarks(lpszPathName,bookmarks))
			SetBookmarks(bookmarks.begin(),bookmarks.end());

		FoldingPointContainerType points;

		// Restore the folding
		if (p->GetFoldingPoints(lpszPathName,points))
			SetFoldingPoints(points.begin(),points.end());
	}

	return TRUE;
}

BOOL LaTeXDocumentBase::OnSaveDocument(LPCTSTR lpszPathName)
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
