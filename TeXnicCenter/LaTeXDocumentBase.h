#pragma once

#include "FileChangeWatcher.h"
#include "structureparser.h"
#include "CodeDocument.h"

// LaTeXDocumentBase document

class LaTeXDocumentBase : 
	public CodeDocument,
	public CTextSource,
	public CFileChangeWatcher
{
	bool save_copy_;
	int EOL_mode_;
	int current_line_;
	CString line_;

	DECLARE_DYNAMIC(LaTeXDocumentBase)

public:
	LaTeXDocumentBase();
	~LaTeXDocumentBase();

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	afx_msg void OnFileSaveCopyAs();
	afx_msg void OnUpdateFileSave(CCmdUI *pCmdUI);

	DECLARE_MESSAGE_MAP()

public:    
	void CheckForFileChanges();
	BOOL DoSave(LPCTSTR lpszPathName,BOOL bReplace = TRUE);

public:
	BOOL GetNextLine(LPCTSTR &lpLine, int &nLength);
	void DeleteContents();
	void Delete();
	void UpdateReadOnlyFlag();
	void UpdateTextBufferOnExternalChange();
	void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU);

	int GetSavedEOLMode() const;

public:
	DWORD SaveFile(HANDLE file, bool throw_on_invalid_sequence);
	using CodeDocument::SaveFile;

protected:
	void OnBookmarkAdded(const CodeBookmark& b);
	void OnBookmarkRemoved(const CodeBookmark& b);
	void OnRemovedAllBookmarks(void);

public:
	virtual void OnCloseDocument();
};
