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
{
	m_nTabWidth = CConfiguration::GetInstance()->m_nTabWidth;
	m_nFixedColumn = CConfiguration::GetInstance()->m_nFixedColumnWrap;
	m_nWordWrapStyle = CConfiguration::GetInstance()->m_WordWrapStyle - 1;
}

COptionPageEditor::~COptionPageEditor()
{
}

void COptionPageEditor::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX,IDC_OPTIONS_EDITOR_WORDWRAP_FIXEDCOLUMN,m_wndFixedColumn);
	DDX_Text(pDX,IDC_EDITOR_TABWIDTH,m_nTabWidth);
	DDV_MinMaxUInt(pDX,m_nTabWidth,1,20);
	DDX_Text(pDX,IDC_OPTIONS_EDITOR_WORDWRAP_FIXEDCOLUMN,m_nFixedColumn);
	DDV_MinMaxUInt(pDX,m_nFixedColumn,1,1000);
	DDX_CBIndex(pDX,IDC_OPTIONS_EDITOR_WORDWRAP_STYLE,m_nWordWrapStyle);
	DDX_Check(pDX,IDC_SHOW_LINE_BELOW_FOLD,show_line_below_fold_);
	DDX_Check(pDX,IDC_SHOW_LINE_BELOW_NO_FOLD,show_line_below_no_fold_);
	DDX_Check(pDX,IDC_SHOW_LINE_ABOVE_FOLD,show_line_above_fold_);
	DDX_Check(pDX,IDC_SHOW_LINE_ABOVE_NO_FOLD,show_line_above_no_fold_);
	DDX_Check(pDX,IDC_USE_SPACES,use_spaces_);
	DDX_Check(pDX,IDC_FOLD_COMPACT,fold_compact_);
}

BEGIN_MESSAGE_MAP(COptionPageEditor,CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_OPTIONS_EDITOR_WORDWRAP_STYLE,UpdateControlStates)
END_MESSAGE_MAP()

// COptionPageEditor message handlers

void COptionPageEditor::OnOK()
{
	UpdateData();

	// TODO:
	/* PLEASE NOTE:

	        Disabling the line wrapping is currently not possible since the edit control
	        crashes due to a problem with styling. This problem occurs only in combination
	        with online spell checking.

	        If this problem has been fixed, please edit

	                OptionPageEditor.cpp (m_nWordWrapStyle +/- 1)
	                        and its dialog resource
	 */

	// Store settings to configuration
	CConfiguration::GetInstance()->m_nTabWidth = m_nTabWidth;
	CConfiguration::GetInstance()->m_nFixedColumnWrap = m_nFixedColumn;

	CConfiguration::GetInstance()->SetShowLineBelowFold(show_line_below_fold_ != 0);
	CConfiguration::GetInstance()->SetShowLineBelowNoFold(show_line_below_no_fold_ != 0);
	CConfiguration::GetInstance()->SetShowLineAboveFold(show_line_above_fold_ != 0);
	CConfiguration::GetInstance()->SetShowLineAboveNoFold(show_line_above_no_fold_ != 0);

	CConfiguration::GetInstance()->SetUseSpaces(use_spaces_ != 0);
	CConfiguration::GetInstance()->SetFoldCompact(fold_compact_ != 0);

	// TODO: Word wrap
	//CConfiguration::GetInstance()->m_WordWrapStyle = (TCRYSTALWORDWRAP)m_nWordWrapStyle + 1;

	// this message will be send to all windows of the application
	AfxGetMainWnd()->SendMessage(WM_SYSCOLORCHANGE);

	CMFCPropertyPage::OnOK();
}

void COptionPageEditor::UpdateControlStates()
{
	UpdateData();

	//m_wndFixedColumn.EnableWindow(m_nWordWrapStyle + 1 == WORD_WRAP_FIXEDCOLUMN);
}

BOOL COptionPageEditor::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();

	UpdateControlStates();

	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
