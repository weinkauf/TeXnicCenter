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

#if !defined(AFX_LATEXDOC_H__447410E9_1058_11D3_929E_D75EECD5B417__INCLUDED_)
#define AFX_LATEXDOC_H__447410E9_1058_11D3_929E_D75EECD5B417__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StructureParser.h"
#include "FileChangeWatcher.h"

class CLaTeXDoc : 
    public CDocument, 
    public CTextSource, 
    public CFileChangeWatcher {

protected:
    CLaTeXDoc();
    DECLARE_DYNCREATE(CLaTeXDoc)

public:
    virtual ~CLaTeXDoc();

    // operations
public:
    /**
    Loads a file into the documents buffer without setting title or path of the
    document.

    @param lpszPath
            Path of the file to load into the buffer.

    @return
            0 if successfull, GetLastError() otherwise.
     */
    DWORD LoadBuffer(LPCTSTR lpszPath);

    /**
    Sets an error mark on the specified line and clears all other error marks.

    @param nLine The line to set the error mark on.
     */
    void SetErrorMark(int nLine);

    /**
    Called to check for file changes.
     */
    void CheckForFileChanges();

    // implementation helpers
protected:
    /** 
    Sets the read only flag of the document depending on the document's
    file.
     */
    void UpdateReadOnlyFlag();

    /**
    Asks the user to reload the document and does so, if wanted.
     */
    void UpdateTextBufferOnExternalChange();

    /**
    Saves the text buffer to the document's file.

    The parameters are passed to the CCrystalTextBuffer::SaveToFile()
    method.
    @return 0 if successfull, GetLastError() otherwise. 
     */
    DWORD SaveToFile(LPCTSTR lpszPathName, int nCrlfStyle = CRLF_STYLE_AUTOMATIC, BOOL bClearModifiedFlag = TRUE);

    // CTextSource overrides
public:
    /** @see CTextSource::GetNextLine */
    virtual BOOL GetNextLine(LPCTSTR &lpLine, int &nLength);

    /** @see CTextSource::Delete */
    virtual void Delete();

    // CDocument overrides
public:
    /**	Save the document data to a file.

    @param lpszPathName
            Path name where to save document file.
            If lpszPathName is NULL then the user will be prompted (SaveAs).

    @note
            lpszPathName can be different than 'm_strPathName'.

    @param bReplace
            If TRUE, it will change file name if successful (SaveAs).
            If FALSE, it will not change path name (SaveCopyAs).
     */
    BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace = TRUE);

    //{{AFX_VIRTUAL(CLaTeXDoc)
public:
    virtual BOOL OnNewDocument();
    virtual void DeleteContents();
    virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE);
    //}}AFX_VIRTUAL

    // Generierte Message-Map-Funktionen
protected:
    //{{AFX_MSG(CLaTeXDoc)
    afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
    afx_msg void OnFileSaveCopyAs();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

    // debugging
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    // attributes
public:
    /** The buffer that contains the document text. */
    CCrystalTextBufferEx *m_pTextBuffer;

protected:
    /** 
    Line counter used by GetNextLine().

    @see GetNextLine
     */
    int m_nCurrentLine;

    /** 
    TRUE, if the call to OnSaveDocument() is used to save a copy of the
    document.
     */
    BOOL m_bSaveCopy;

    /**
    New mode when saving the file
     */
    int m_nCRLFMode;

public:
    void SetModifiedFlag(BOOL modified = TRUE);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_LATEXDOC_H__447410E9_1058_11D3_929E_D75EECD5B417__INCLUDED_)
