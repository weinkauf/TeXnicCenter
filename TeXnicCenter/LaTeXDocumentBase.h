#pragma once

#include "FileChangeWatcher.h"
#include "structureparser.h"
#include "CodeDocument.h"
#include "Nullable.h"

// LaTeXDocumentBase document

class LaTeXDocumentBase : 
	public CodeDocument,
	public CTextSource,
	public CFileChangeWatcher
{
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

	DECLARE_MESSAGE_MAP()

public:    
	void CheckForFileChanges();

public:
	BOOL GetNextLine(LPCTSTR &lpLine, int &nLength);
	void DeleteContents();
	void Delete();
	void UpdateReadOnlyFlag();
	void UpdateTextBufferOnExternalChange();
	void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU);
public:
	using CodeDocument::SaveFile;

protected:
	void OnBookmarkAdded(const CodeBookmark& b);
	void OnBookmarkRemoved(const CodeBookmark& b);
	void OnRemovedAllBookmarks(void);

public:
	virtual void OnCloseDocument();
};
