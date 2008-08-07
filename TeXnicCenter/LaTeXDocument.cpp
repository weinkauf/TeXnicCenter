// LaTeXDocument.cpp : implementation file
//

#include "stdafx.h"
#include "TeXnicCenter.h"
#include "LaTeXDocument.h"

#include "LaTeXView.h"
#include "global.h"


// LaTeXDocument

IMPLEMENT_DYNCREATE(LaTeXDocument, CScintillaDoc)

LaTeXDocument::LaTeXDocument()
: m_bSaveCopy(false)
{
}

BOOL LaTeXDocument::OnNewDocument()
{
	if (!CScintillaDoc::OnNewDocument())
		return FALSE;

    // *
    if (m_strPathName.IsEmpty())
        m_strPathName = m_strTitle;

	return TRUE;
}

LaTeXDocument::~LaTeXDocument()
{
}


BEGIN_MESSAGE_MAP(LaTeXDocument, CScintillaDoc)
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

void LaTeXDocument::SetErrorMark(int line)
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
