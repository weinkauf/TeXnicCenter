#pragma once

#include <vector>
#include <memory>

#include "ThemedCtrl.h"
#include "DocumentTokenizer.h"
#include "CodeBookmark.h"

class SpellerBackgroundThread;
class CodeDocument;

int GetLogFontPointSize(const LOGFONT& lf);

/// Base class for Code editors with support
/// for Unicode loading and saving, spell checking,
/// incremental search, bookmarking and so on
class CodeView : 
	public CScintillaView
{
	ThemedCtrl theme_;
	volatile LONG hold_count_;	
	mutable std::auto_ptr<DocumentTokenizer> tokenizer_;
	
	// Hard wrap
	bool hard_wrap_enabled_;
	bool remove_leading_spaces_;
	bool suppress_speller_;

	long last_change_pos_;

	DECLARE_DYNAMIC(CodeView)

protected:
	CodeView();           // protected constructor used by dynamic creation
	~CodeView();

	virtual DocumentTokenizer* NewDocumentTokenizer() const = 0;

public:
	CodeDocument* GetDocument() const;

	void EnableHardWrap(bool e = true);
	bool IsHardWrapEnabled() const;
	void SetRemoveLeadingSpaces(bool e = true);
	bool GetRemoveLeadingSpaces() const;

	static int GetFoldingMargin() { return 2; }

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif	
#endif
	int GetLineLength(int line, bool direct = true);
	int GetLineCount(bool direct = true);
	int GetCurrentLine(bool direct = true);
	const CString GetLineText(int line, bool direct = true);
	void InsertText(const CString& text);
	void GoToLine(int line, bool direct = true);

	DocumentTokenizer* GetTokenizer() const;
	int Lock(bool exclusive = false);
	int Unlock(bool exclusive = false);

	bool IsLineMarginVisible();

protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnViewWhiteSpace();
	afx_msg void OnViewLineEnding();
	afx_msg void OnUpdateViewWhiteSpace(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewShowLineEnding(CCmdUI *pCmdUI);
	afx_msg void OnEditDeleteLine();
	afx_msg void OnSysColorChange();
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	afx_msg void OnEditGoto();
	afx_msg void OnViewWordWrap();
	afx_msg void OnUpdateViewWordWrap(CCmdUI *pCmdUI);
	void OnUpdatePositionIndicator(CCmdUI* pCmdUI);
	void OnUpdateEncodingIndicator(CCmdUI* pCmdUI);
	void OnUpdateEOLModeIndicator(CCmdUI* pCmdUI);
	void TextNotFound(LPCTSTR /*lpszFind*/, BOOL /*bNext*/, BOOL /*bCase*/, BOOL /*bWord*/, BOOL /*bRegularExpression*/, BOOL /*bReplaced*/);
	afx_msg void OnViewLineNumbers();
	afx_msg void OnUpdateViewLineNumbers(CCmdUI *pCmdUI);
	void UpdateLineNumberMargin();
	void OnCharAdded(SCNotification* n);
	
	virtual void OnSettingsChanged();

protected:
	DECLARE_MESSAGE_MAP()	

	void HardWrapRange(long s, long e);

	template<class F>
	void ForEveryView(F f);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnEditGotoNextBookmark();
	afx_msg void OnEditGotoPrevBookmark();
	afx_msg void OnUpdateEditGotoPrevBookmark(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditGotoNextBookmark(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditClearAllBookmarks(CCmdUI *pCmdUI);
	int GetPreviousBookmark();
	int GetNextBookmark();
	void DefineMarker(int marker, int markerType, COLORREF fore, COLORREF back);

public:
	virtual bool Serialize(CIniFile &ini, LPCTSTR lpszKey, bool write);
protected:
	void OnMarginClick(SCNotification* n);
	void OnUpdateUI(SCNotification* n);

private:
	class ShadowWindow :
		public CWindowImpl<ShadowWindow>
	{
		CodeView* view;
		long original_pos;
		long match_start_pos, match_end_pos;
		bool found;
		bool incremental_search_enabled;
		CString search_text;
		bool search_forward;
		int rounds;
		bool update_status_bar;
		bool first_match;
		HCURSOR search_forward_cursor, search_backward_cursor;

	public:
		bool autoindent_enabled;

		void SetSearchForward(bool forward = true);

		ShadowWindow(CodeView* v);
		~ShadowWindow();

		BEGIN_MSG_MAP(CodeView::ShadowWindow)
			MESSAGE_HANDLER(WM_CHAR, OnChar)
			MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
			MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocus)
			MESSAGE_HANDLER(WM_LBUTTONDOWN, OnMouseButtonDown)
			MESSAGE_HANDLER(WM_RBUTTONDOWN, OnMouseButtonDown)
			MESSAGE_HANDLER(WM_MBUTTONDOWN, OnMouseButtonDown)
			MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
			MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		END_MSG_MAP()

		LRESULT OnChar(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);		

		static bool IsNewLine(TCHAR ch);
		LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnMouseButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);		
		LRESULT OnSetCursor(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

		void HandleChar(TCHAR ch, bool one_level_recursion = false);
		void EnableIncrementalSearch(bool enable = true);
		void UpdateStatusBarMessage();
		void UpdateCursor();

		bool IsIncrementalSearchEnabled() const;
	};

	ShadowWindow shadow_;

protected:
	afx_msg void OnEditFindIncrementalForward();
	afx_msg void OnEditFindIncrementalBackward();
	void GetReplaceAllTarget(long& s, long& e);
	afx_msg void OnEditGotoLastChange();
	afx_msg void OnUpdateEditGotoLastChange(CCmdUI *pCmdUI);
	void OnModified(SCNotification* n);
	afx_msg void OnViewIndentationGuides();
	afx_msg void OnUpdateViewIndentationGuides(CCmdUI *pCmdUI);
	afx_msg void OnEditSelBiggerBlock();
	void OnZoom(SCNotification* n);

public:
	CScintillaFindReplaceDlg* CreateFindReplaceDialog(void);

	bool IsAutoIndentEnabled() const;
	void EnableAutoIndent(bool enable = true);

	void EnableFolding(bool enable = true);
	bool IsFoldingEnabled();
	void ShowIndentationGuides(bool show);

	void SetModified(bool modified = true);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	static bool TerminatesIncrementalSearch( UINT ch );
	static bool IsDirectionKey( UINT ch );
};

template<class F>
inline void CodeView::ForEveryView(F f)
{
	CDocTemplate* t = GetDocument()->GetDocTemplate();
	POSITION pos = t->GetFirstDocPosition();

	while (pos) {
		CDocument* doc = reinterpret_cast<CDocument*>(t->GetNextDoc(pos));

		if (CodeDocument* codedoc = dynamic_cast<CodeDocument*>(doc)) {
			ASSERT(codedoc->GetView()->IsKindOf(RUNTIME_CLASS(CodeView)));
			f(static_cast<CodeView*>(codedoc->GetView()));
		}		
	}
}

