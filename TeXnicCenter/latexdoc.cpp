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
#include "TeXnicCenter.h"

#include "LatexDoc.h"
#include "TextFileSaveDialog.h"
#include "Configuration.h"
#include "global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// class CLaTeXDoc
//-------------------------------------------------------------------

IMPLEMENT_DYNCREATE(CLaTeXDoc,CDocument)

BEGIN_MESSAGE_MAP(CLaTeXDoc,CDocument)
    //{{AFX_MSG_MAP(CLaTeXDoc)
    ON_UPDATE_COMMAND_UI(ID_FILE_SAVE,OnUpdateFileSave)
    ON_COMMAND(ID_FILE_SAVE_COPY_AS,OnFileSaveCopyAs)
    //}}AFX_MSG_MAP
    ON_COMMAND(ID_FILE_SEND_MAIL,OnFileSendMail)
    ON_UPDATE_COMMAND_UI(ID_FILE_SEND_MAIL,OnUpdateFileSendMail)
END_MESSAGE_MAP()


CLaTeXDoc::CLaTeXDoc()
: m_bSaveCopy(FALSE)
, m_nCRLFMode(-1)
{
    m_pTextBuffer = new CCrystalTextBufferEx(this);
    ASSERT(m_pTextBuffer);
}

CLaTeXDoc::~CLaTeXDoc()
{
    if (m_pTextBuffer)
        delete m_pTextBuffer;
}

BOOL CLaTeXDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    // *
    if (m_strPathName.IsEmpty())
        m_strPathName = m_strTitle;

    // textbuffer must be created before use
    IFNOT(m_pTextBuffer)
    return FALSE;

    m_pTextBuffer->InitNew();
    m_pTextBuffer->SetCRLFMode(CConfiguration::GetInstance()->m_nStandardFileFormat);

    return TRUE;
}

void CLaTeXDoc::SetErrorMark(int nLine)
{
    if (nLine < 0) {
        // remove flag
        int nLine = m_pTextBuffer->GetLineWithFlag(LF_COMPILATION_ERROR);
        if (nLine > -1)
            m_pTextBuffer->SetLineFlag(nLine,LF_COMPILATION_ERROR,FALSE,FALSE);
    }
    else if (nLine >= 0 && nLine <= m_pTextBuffer->GetLineCount())
        m_pTextBuffer->SetLineFlag(nLine - 1,LF_COMPILATION_ERROR,TRUE);
}

void CLaTeXDoc::CheckForFileChanges()
{
    WORD wChanges = GetFileChanges();

    if (wChanges & chReadOnly)
        UpdateReadOnlyFlag();
    else if (wChanges)
        UpdateTextBufferOnExternalChange();
}


/////////////////////////////////////////////////////////////////////////////
// CLaTeXDoc Diagnose

#ifdef _DEBUG

void CLaTeXDoc::AssertValid() const
{
    CDocument::AssertValid();
}

void CLaTeXDoc::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// operations

void CLaTeXDoc::DeleteContents()
{
    SetFilePath(NULL);

    m_pTextBuffer->FreeAll();
    m_nCurrentLine = 0;

    CDocument::DeleteContents();
}

BOOL CLaTeXDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
    DWORD dwResult = SaveToFile(lpszPathName,m_nCRLFMode,!m_bSaveCopy);

    if (dwResult != 0) {
        CString strMsg;
        strMsg.Format(STE_FILE_INUSE,
                AfxLoadString(IDS_SAVE),
                lpszPathName,
                AfxFormatSystemString(dwResult));
        AfxMessageBox(strMsg,MB_ICONEXCLAMATION | MB_OK);

        m_nCRLFMode = -1;
        m_bSaveCopy = FALSE;
        return FALSE;
    }
    else if (!m_bSaveCopy && m_nCRLFMode >= 0) {
        m_pTextBuffer->SetCRLFMode(m_nCRLFMode);
    }

    m_nCRLFMode = -1;
    m_bSaveCopy = FALSE;
    theApp.m_pMainWnd->SendMessage(WM_COMMAND,ID_DOCUMENT_SAVED);
    return TRUE;
}

DWORD CLaTeXDoc::SaveToFile(LPCTSTR lpszPathName,int nCrlfStyle /*= CRLF_STYLE_AUTOMATIC*/,BOOL bClearModifiedFlag /*= TRUE*/)
{
    CheckForFileChanges();
    DWORD dwResult = m_pTextBuffer->SaveToFile(lpszPathName,nCrlfStyle,bClearModifiedFlag);
    SnapFileState();

    return dwResult;
}

DWORD CLaTeXDoc::LoadBuffer(LPCTSTR lpszPath)
{
    m_pTextBuffer->FreeAll();
    DWORD dwResult = m_pTextBuffer->LoadFromFile(lpszPath);

    if (dwResult == 0)
        SetModifiedFlag();

    return dwResult;
}

BOOL CLaTeXDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    return (m_pTextBuffer->LoadFromFile(lpszPathName) == 0);
}

void CLaTeXDoc::OnUpdateFileSave(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(IsModified());
}

BOOL CLaTeXDoc::DoSave(LPCTSTR lpszPathName,BOOL bReplace /*= TRUE*/)
{
    CString newName(lpszPathName);
    if (newName.IsEmpty()) {
        CDocTemplate* pTemplate = GetDocTemplate();
        ASSERT(pTemplate != NULL);

        CString strExt;
        pTemplate->GetDocString(strExt,CDocTemplate::filterExt);
        ASSERT(!strExt.IsEmpty() && strExt[0] == '.');

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

        CTextFileSaveDialog dlg(
                bReplace ? AFX_IDS_SAVEFILE : AFX_IDS_SAVEFILECOPY,
                strExt,newName,
                OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOREADONLYRETURN,
                m_pTextBuffer ? m_pTextBuffer->GetCRLFMode() : CConfiguration::GetInstance()->m_nStandardFileFormat,
                CString((LPCTSTR)STE_FILE_LATEXFILTER));

        //Show the dialog
        if (dlg.DoModal() != IDOK) {
            //It was canceled - the PathName is not set.
            // Therefore, we use GetLastOpenedFolder.
            AfxSetLastDirectory(dlg.GetLastOpenedFolder());
            return false;
        }

        AfxSetLastDirectory(CPathTool::GetDirectory(dlg.GetPathName()));
        m_nCRLFMode = dlg.GetFileFormat();
        m_bSaveCopy = !bReplace;

        newName = dlg.GetPathName();
    }
    else
        m_nCRLFMode = m_pTextBuffer->GetCRLFMode();

    return CDocument::DoSave(newName,bReplace);
}

void CLaTeXDoc::OnFileSaveCopyAs()
{
    DoSave(NULL,FALSE);
}

void CLaTeXDoc::SetPathName(LPCTSTR lpszPathName,BOOL bAddToMRU)
{
    SetFilePath(lpszPathName);
    CDocument::SetPathName(lpszPathName,bAddToMRU);
}

void CLaTeXDoc::UpdateReadOnlyFlag()
{
    CFileStatus fs;

    if (CFile::GetStatus(GetPathName(),fs))
        m_pTextBuffer->SetReadOnly(fs.m_attribute & CFile::readOnly);
}

void CLaTeXDoc::UpdateTextBufferOnExternalChange()
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
        DWORD dwResult = m_pTextBuffer->LoadFromFile(GetPathName());
        if (dwResult != 0) {
            strMsg.Format(STE_FILE_INUSE,
                    AfxLoadString(IDS_OPEN),
                    GetPathName(),
                    AfxFormatSystemString(dwResult));
            AfxMessageBox(strMsg,MB_ICONINFORMATION | MB_OK);
            m_pTextBuffer->SetReadOnly(TRUE);
        }
    }
}


/////////////////////////////////////////////////////////////////////////////
// implementation of CTextSource-virtuals

BOOL CLaTeXDoc::GetNextLine(LPCTSTR &lpLine,int &nLength)
{
    AssertValid();

    if (m_nCurrentLine >= m_pTextBuffer->GetLineCount())
        return FALSE;

    lpLine = m_pTextBuffer->GetLineChars(m_nCurrentLine);
    nLength = m_pTextBuffer->GetLineLength(m_nCurrentLine++);
    return TRUE;
}

void CLaTeXDoc::Delete()
{
}

void CLaTeXDoc::SetModifiedFlag(BOOL modified)
{
    if (IsModified() != modified) {
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
