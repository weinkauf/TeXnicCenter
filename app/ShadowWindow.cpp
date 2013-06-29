#include "stdafx.h"
#include "resource.h"

#include "CodeView.h"
#include "CharType.h"

CodeView::ShadowWindow::ShadowWindow(CodeView* v) 
: incremental_search_enabled(false)
, view(v)
, search_forward(true)
, search_forward_cursor(0)
, search_backward_cursor(0)
, update_status_bar(0)
, autoindent_enabled(false)
{
}

CodeView::ShadowWindow::~ShadowWindow()
{
	if (search_forward_cursor)
		::DestroyCursor(search_forward_cursor);

	if (search_backward_cursor)
		::DestroyCursor(search_backward_cursor);
}

void CodeView::ShadowWindow::UpdateStatusBarMessage()
{
	CString message;

	if (!found)
		message.Format(IDS_FIND_INCREMENTAL_FAILED,search_text);
	else
		message.Format(search_forward ? IDS_FIND_INCREMENTAL_FORWARD : 
			IDS_FIND_INCREMENTAL_BACKWARD,search_text);

	view->GetTopLevelFrame()->SetMessageText(message);
}

LRESULT CodeView::ShadowWindow::OnChar(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if (!incremental_search_enabled)
		DefWindowProc(); // Pass the message to Scintilla

	bool is_new_line = IsNewLine(static_cast<TCHAR>(wParam));

	// Autoindent: Line has been added
	if (is_new_line) {
		const int line = view->GetCurrentLine();

		if (line > 0) {
			const int prev_line = line - 1;
			int indent = view->GetCtrl().GetLineIndentation(prev_line);
			int width = view->GetCtrl().GetIndent();

			if (!width) // Use tab width
				width = view->GetCtrl().GetTabWidth();

			indent /= width;

			while (indent--)
				view->GetCtrl().Tab();
		}
	}

	// Incremental search
	if (incremental_search_enabled) { // Absorb the message
		if (wParam == VK_ESCAPE || is_new_line)
			EnableIncrementalSearch(false);
		else if (wParam == VK_BACK) {
			if (!search_text.IsEmpty())
				search_text.Delete(search_text.GetLength() - 1);

			found = true;
			view->GetCtrl().GotoPos(original_pos);
			view->GetCtrl().SearchAnchor();

			HandleChar(0);
		}
		else
			HandleChar(static_cast<TCHAR>(wParam));
	}

	return 0;
}

LRESULT CodeView::ShadowWindow::OnKeyDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = incremental_search_enabled;
	return 0;
}

LRESULT CodeView::ShadowWindow::OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	// Scintilla lost focus, disable incremental search
	EnableIncrementalSearch(false);
	bHandled = FALSE;
	return 0;
}

LRESULT CodeView::ShadowWindow::OnMouseButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE; // Pass the message to Scintilla

	// User clicked, cancel the incremental search
	if (incremental_search_enabled)
		EnableIncrementalSearch(false);

	return 0;
}

LRESULT CodeView::ShadowWindow::OnSetCursor(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = incremental_search_enabled;

	if (incremental_search_enabled)
		UpdateCursor();

	return 1; // if not handled, do not let DefWindowProc process this message any further
}

void CodeView::ShadowWindow::UpdateCursor()
{
	HCURSOR cursor = search_forward ? search_forward_cursor : search_backward_cursor;
	ASSERT(cursor);
	::SetCursor(cursor);
}

void CodeView::ShadowWindow::SetSearchForward( bool forward /*= true*/ )
{
	search_forward = forward;

	// Delay loading the cursor
	if (search_forward && !search_forward_cursor)
		search_forward_cursor = ::LoadCursor(AfxGetResourceHandle(),MAKEINTRESOURCE(IDC_INCREMENTAL_SEARCH));
	else if (!search_forward && !search_backward_cursor)
		search_backward_cursor = ::LoadCursor(AfxGetResourceHandle(),MAKEINTRESOURCE(IDC_REVERSE_INCREMENTAL_SEARCH));

	UpdateCursor();
}

void CodeView::ShadowWindow::EnableIncrementalSearch(bool enable)
{
	bool was_active = incremental_search_enabled;
	incremental_search_enabled = enable;

	// Incremental search was not active and we just started it
	if (!was_active && incremental_search_enabled) {
		// Reset the data
		search_text.Empty();
		found = true;
		rounds = 0;
		update_status_bar = true;
		first_match = false;
		view->GetCtrl().ShowCursor(false);
		UpdateCursor();

		// Save the position and set the search anchor
		match_start_pos = match_end_pos = original_pos = view->GetCtrl().GetCurrentPos();
		view->GetCtrl().SearchAnchor();

		// Notify the user
		UpdateStatusBarMessage();
	}
	else if (was_active && incremental_search_enabled) { // Get the next match
		if (search_forward)
			match_start_pos = match_end_pos;
		else
			match_end_pos = match_start_pos;

		view->GetCtrl().GotoPos(match_end_pos);
		view->GetCtrl().SearchAnchor();

		HandleChar(0);
	}
	else if (was_active && !incremental_search_enabled) { // Search end
		view->GetTopLevelFrame()->SetMessageText(AFX_IDS_IDLEMESSAGE);
		view->GetCtrl().ShowCursor(true);
		// Let Scintilla restore its cursor
		view->GetCtrl().SetCursor(SC_CURSORNORMAL);
	}
}

void CodeView::ShadowWindow::HandleChar( TCHAR ch, bool one_level_recursion )
{	
	if (found) {
		if (CharTraitsT::IsPrint(ch))
			search_text += ch;

		int (CScintillaCtrl::* search)(int,LPCTSTR,BOOL);

		if (search_forward) // Forwards
			search = &CScintillaCtrl::SearchNext;
		else // Backwards
			search = &CScintillaCtrl::SearchPrev;

		found = (view->GetCtrl().*search)(0, search_text, FALSE) >= 0;
		bool recursion = false;

		// End of document reached
		if (!found && !one_level_recursion) {
			++rounds; recursion = true;

			if (search_forward)
				match_end_pos = 0;
			else
				match_end_pos = view->GetCtrl().GetLength();

			found = true;

			view->GetCtrl().GotoPos(match_end_pos);
			view->GetCtrl().SearchAnchor();

			HandleChar(0,true); // Call this function again without executing this if block
		}

		if (!found) {
			::MessageBeep(MB_OK);
		}

		// Text that has been matched will be selected by Scintilla
		// however, the caret will be placed at the start of selection
		// instead of at the end - what we actually want

		// So, get the selection
		long s = view->GetCtrl().GetSelectionStart();
		long e = view->GetCtrl().GetSelectionEnd();

		if (!first_match) {
			first_match = false;
			original_pos = s;
		}

		match_start_pos = s;
		match_end_pos = e;

		// Set the caret to the end position
		view->GetCtrl().SetCurrentPos(e);
		// Restore the selection
		view->GetCtrl().SetSel(s,e);
		view->GetCtrl().SearchAnchor();

		update_status_bar = true;

		if (rounds > 0) {
			ASSERT(rounds == 1);

			const long& match_pos = search_forward ? match_end_pos : match_start_pos;

			if (search_forward && match_pos >= original_pos || 
				!search_forward && match_pos <= original_pos) {					
					TRACE3("Round %i completed: match pos %i, original pos %i\n",rounds,match_pos,original_pos);
					--rounds;
					TRACE0("Incremental search: passed start of search\n");

					if (found) {
						CString message;
						message.Format(IDS_INCREMENTAL_SEARCH_PASSED_START_SEARCH,search_text);

						view->GetTopLevelFrame()->SetMessageText(message);
						update_status_bar = false;
					}
			}
		}

		if (update_status_bar && !recursion) {
			// Show the message in the status bar
			UpdateStatusBarMessage();
		}
		else if (!one_level_recursion)
			update_status_bar = true;
	}
}

LRESULT CodeView::ShadowWindow::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// Let the parent process the message
	DefWindowProc();
	// now unsubclass
	UnsubclassWindow();

	return 0;
}

bool CodeView::ShadowWindow::IsIncrementalSearchEnabled() const
{
	return incremental_search_enabled;
}