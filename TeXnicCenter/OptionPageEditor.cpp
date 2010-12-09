#include "stdafx.h"
#include "texniccenter.h"
#include "OptionPageEditor.h"
#include "configuration.h"


IMPLEMENT_DYNCREATE(COptionPageEditor,CMFCPropertyPage)

COptionPageEditor::COptionPageEditor() 
: CMFCPropertyPage(COptionPageEditor::IDD)
, show_line_below_fold_(CConfiguration::GetInstance()->GetShowLineBelowFold())
, show_line_below_no_fold_(CConfiguration::GetInstance()->GetShowLineBelowNoFold())
, show_line_above_fold_(CConfiguration::GetInstance()->GetShowLineAboveFold())
, show_line_above_no_fold_(CConfiguration::GetInstance()->GetShowLineAboveNoFold())
, use_spaces_(CConfiguration::GetInstance()->GetUseSpaces())
, fold_compact_(CConfiguration::GetInstance()->GetFoldCompact())
, close_toolwindows_on_escape_(CConfiguration::GetInstance()->m_bCloseToolWindowsOnEscape)
{
	m_nTabWidth = CConfiguration::GetInstance()->m_nTabWidth;
	m_nFixedColumn = CConfiguration::GetInstance()->m_nFixedColumnWrap;
}

COptionPageEditor::~COptionPageEditor()
{
}

void COptionPageEditor::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);

	DDX_Text(pDX,IDC_EDITOR_TABWIDTH,m_nTabWidth);
	DDV_MinMaxUInt(pDX,m_nTabWidth,1,20);
	DDX_Text(pDX,IDC_OPTIONS_EDITOR_WORDWRAP_FIXEDCOLUMN,m_nFixedColumn);
	DDV_MinMaxUInt(pDX,m_nFixedColumn,1,1000);
	DDX_Check(pDX,IDC_SHOW_LINE_BELOW_FOLD,show_line_below_fold_);
	DDX_Check(pDX,IDC_SHOW_LINE_BELOW_NO_FOLD,show_line_below_no_fold_);
	DDX_Check(pDX,IDC_SHOW_LINE_ABOVE_FOLD,show_line_above_fold_);
	DDX_Check(pDX,IDC_SHOW_LINE_ABOVE_NO_FOLD,show_line_above_no_fold_);
	DDX_Check(pDX,IDC_USE_SPACES,use_spaces_);
	DDX_Check(pDX,IDC_FOLD_COMPACT,fold_compact_);
	DDX_Check(pDX, IDC_EDITOR_CLOSETOOLWINDOWS_ESC, close_toolwindows_on_escape_);
}


// COptionPageEditor message handlers

void COptionPageEditor::OnOK()
{
	UpdateData();

	// Store settings to configuration
	CConfiguration::GetInstance()->m_nTabWidth = m_nTabWidth;
	CConfiguration::GetInstance()->m_nFixedColumnWrap = m_nFixedColumn;

	CConfiguration::GetInstance()->SetShowLineBelowFold(show_line_below_fold_ != 0);
	CConfiguration::GetInstance()->SetShowLineBelowNoFold(show_line_below_no_fold_ != 0);
	CConfiguration::GetInstance()->SetShowLineAboveFold(show_line_above_fold_ != 0);
	CConfiguration::GetInstance()->SetShowLineAboveNoFold(show_line_above_no_fold_ != 0);

	CConfiguration::GetInstance()->SetUseSpaces(use_spaces_ != 0);
	CConfiguration::GetInstance()->SetFoldCompact(fold_compact_ != 0);

	CConfiguration::GetInstance()->m_bCloseToolWindowsOnEscape = close_toolwindows_on_escape_;


	// this message will be send to all windows of the application
	// I guess, we need this to make the changes appear.
	AfxGetMainWnd()->SendMessage(WM_SYSCOLORCHANGE);

	CMFCPropertyPage::OnOK();
}

