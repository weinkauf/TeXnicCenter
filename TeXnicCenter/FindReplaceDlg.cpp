// FindReplaceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TeXnicCenter.h"
#include "FindReplaceDlg.h"

#include "ScintillaDocView.h"

// FindReplaceDlg dialog

IMPLEMENT_DYNAMIC(FindReplaceDlg, CScintillaFindReplaceDlg)

FindReplaceDlg::FindReplaceDlg()
: whole_file_(1)
{

}

FindReplaceDlg::~FindReplaceDlg()
{
}

void FindReplaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CScintillaFindReplaceDlg::DoDataExchange(pDX);
	DDX_Radio(pDX,IDC_EDIT_SCOPE_SELECTION,whole_file_);
}


BEGIN_MESSAGE_MAP(FindReplaceDlg, CScintillaFindReplaceDlg)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// FindReplaceDlg message handlers

BOOL FindReplaceDlg::OnInitDialog()
{
	CScintillaFindReplaceDlg::OnInitDialog();

	bool view_found = false;

	if (CFrameWnd* tlf = GetTopLevelFrame()) {
		if (CFrameWnd* cf = tlf->GetActiveFrame()) {
			if (CScintillaView* view = dynamic_cast<CScintillaView*>(cf->GetActiveView())) {
				whole_file_ = view->GetCtrl().GetSelectionStart() == view->GetCtrl().GetSelectionEnd();
				UpdateData(FALSE);
				view_found = true;
			}
		}
	}	

	ASSERT(view_found);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

bool FindReplaceDlg::ReplaceSelection() const
{
	ASSERT(whole_file_ != -1);
	return !whole_file_;
}

void FindReplaceDlg::OnDestroy()
{
	UpdateData(TRUE);

	CScintillaFindReplaceDlg::OnDestroy();
}
