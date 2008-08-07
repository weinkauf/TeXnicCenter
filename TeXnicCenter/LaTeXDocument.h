#pragma once

#if 1
// LaTeXDocument document

class LaTeXDocument : public CScintillaDoc
{
	bool m_bSaveCopy;

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
};

#endif