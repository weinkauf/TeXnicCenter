#pragma once

#include "FileChangeWatcher.h"
#include "structureparser.h"

#if 1
// LaTeXDocument document

class LaTeXDocument : 
	public CScintillaDoc,
	public CTextSource,
	public CFileChangeWatcher
{
	bool m_bSaveCopy;
	int m_nCRLFMode;
	int m_nCurrentLine;
	CString line_;

	DECLARE_DYNCREATE(LaTeXDocument)

public:
	LaTeXDocument();
	~LaTeXDocument();

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
public:
    void SetModifiedFlag(BOOL modified = TRUE);
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	void SetErrorMark(int line);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	void CheckForFileChanges();
	afx_msg void OnUpdateFileSave(CCmdUI *pCmdUI);

protected:
	BOOL DoSave(LPCTSTR lpszPathName,BOOL bReplace = TRUE);
public:
	BOOL GetNextLine(LPCTSTR &lpLine, int &nLength);
	void DeleteContents();
	void Delete();
	void UpdateReadOnlyFlag();
	void UpdateTextBufferOnExternalChange();
	void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU);
	afx_msg void OnFileSaveCopyAs();
	int GetSavedEOLMode() const;
};
int ToScintillaMode(int m);
int FromScintillaMode(int m);
#endif