#pragma once

class CAdvice;
class CAutoCompleteDlg;
class LaTeXViewListener;
class LaTeXDocument;

#if 1

#include "ThemedCtrl.h"

// LaTeXView view

class LaTeXView : public CScintillaView
{
    ThemedCtrl theme_;

	DECLARE_DYNCREATE(LaTeXView)

public:
	enum Encoding
	{
		ASCII,
		UTF8,
		UTF16LE,
		UTF16BE,
		UTF32LE,
		UTF32BE
	};

	long old_pos_start_, old_pos_end_;

private:
	Encoding encoding_;
	UINT code_page_;
	bool m_bCreateBackupFile;
	int style_number_;

protected:
	LaTeXView();           // protected constructor used by dynamic creation
	virtual ~LaTeXView();

public:
	enum
	{
		//	Base colors
		COLORINDEX_WHITESPACE,
		COLORINDEX_BKGND,
		COLORINDEX_NORMALTEXT,
		COLORINDEX_SELMARGIN,
		COLORINDEX_SELBKGND,
		COLORINDEX_SELTEXT,
		//	Syntax colors
		COLORINDEX_KEYWORD,
		COLORINDEX_COMMENT,
		COLORINDEX_NUMBER,
		COLORINDEX_OPERATOR, // [JRT]:
		COLORINDEX_STRING,
		COLORINDEX_PREPROCESSOR,

		// Brackets higlighting
		COLORINDEX_PAIRSTRINGBKGND,
		COLORINDEX_PAIRSTRINGTEXT,
		COLORINDEX_BADPAIRSTRINGBKGND,
		COLORINDEX_BADPAIRSTRINGTEXT,
		COLORINDEX_CURPAIRSTRINGBKGND,
		COLORINDEX_CURPAIRSTRINGTEXT,
		COLORINDEX_PAIRBLOCKBKGND,

		//	Compiler/debugger colors
		COLORINDEX_ERRORBKGND,
		COLORINDEX_ERRORTEXT,
		COLORINDEX_EXECUTIONBKGND,
		COLORINDEX_EXECUTIONTEXT,
		COLORINDEX_BREAKPOINTBKGND,
		COLORINDEX_BREAKPOINTTEXT
		//	...
		//	Expandable: custom elements are allowed.
	};

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
    virtual void OnInitialUpdate();

	void UpdateSettings();
	void SetAStyle(int style, COLORREF fore, COLORREF back = RGB(255,255,255), int size = -1, LPCTSTR face = 0);
    void OnCharAdded(SCNotification* n);
    virtual void Serialize(CArchive& ar);

	Encoding GetEncoding() const;

	UINT GetCodePage() const;
	Encoding DetectEncoding(const BYTE* data, SIZE_T& pos, SIZE_T size);
	COLORREF GetAutomaticColor(int nColorIndex);
	COLORREF GetColor(int nColorIndex);
	void OnUpdateUI(SCNotification* pSCNotification);
	void InstantAdvice();
	void HideAdvice();
	void OnACBackspace();
	void OnACChar(UINT nKey,UINT nRepCount,UINT nFlags);
	void OnACCommandCancelled();
	void OnACCommandSelect(const CLaTeXCommand* cmd);
	void OnACHelp(CString &cmd);
	CAutoCompleteDlg* CreateListBox(CString &keyword, long pos);

private:
	CAdvice *m_InstTip;
	CAutoCompleteDlg* m_CompletionListBox;
	LaTeXViewListener *m_Proxy;
	bool m_AutoCompleteActive;
	CWinThread* m_pBackgroundThread;
public:
	void GoToLine(int line);
	void GetWordBeforeCursor(CString& strKeyword, long& start,bool bSelect = true);
	static bool IsAutoCompletionCharacter(TCHAR tc);
	int GetLineLength(int line);
	afx_msg void OnEditGoto();
	int GetLineCount(void);
	int GetCurrentLine(void);
	const CString GetLineText(int line);
	void InsertText(const CString& text);
	afx_msg void OnEditToggleWhiteSpaceView();
	afx_msg void OnEditToggleShowLineEnding();
	afx_msg void OnUpdateEditToggleWhiteSpaceView(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditToggleShowLineEnding(CCmdUI *pCmdUI);
	afx_msg void OnSpellFile();

	DWORD LoadFromFile(LPCTSTR pszFileName);
	DWORD LoadFromFile( HANDLE file );
	DWORD SaveToFile(LPCTSTR pszFileName, bool bClearModifiedFlag = true);
	DWORD SaveToFile( HANDLE file );
	DWORD SaveToFile( HANDLE file, LPCWSTR text, int length);
	void ConvertToMultiByte(LPCWSTR input, int cch, std::vector<BYTE>& buffer, Encoding encoding, UINT cp);
	afx_msg void OnEditDeleteLine();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnEditOutsource();
	LaTeXDocument* GetDocument() const;
	int GetNumberOfMatches(const CString& keyword);
	afx_msg void OnQueryCompletion();
	void RestoreFocus();
};

#endif
