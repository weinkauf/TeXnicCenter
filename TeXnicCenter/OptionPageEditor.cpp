// OptionPageEditor.cpp : implementation file
//

#include "stdafx.h"
#include "texniccenter.h"
#include "OptionPageEditor.h"
#include "configuration.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionPageEditor property page

IMPLEMENT_DYNCREATE(COptionPageEditor, CPropertyPage)

COptionPageEditor::COptionPageEditor() : CPropertyPage(COptionPageEditor::IDD)
{
	//{{AFX_DATA_INIT(COptionPageEditor)
	m_nTabWidth = g_configuration.m_nTabWidth;
	m_nFixedColumn = g_configuration.m_nFixedColumnWrap;
	m_nWordWrapStyle = g_configuration.m_WordWrapStyle;
	m_bShowLineNumbers = g_configuration.m_bShowLineNumbers;
	//}}AFX_DATA_INIT
}

COptionPageEditor::~COptionPageEditor()
{
}

void COptionPageEditor::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionPageEditor)
	DDX_Control(pDX, IDC_OPTIONS_EDITOR_WORDWRAP_FIXEDCOLUMN, m_wndFixedColumn);
	DDX_Text(pDX, IDC_EDITOR_TABWIDTH, m_nTabWidth);
	DDV_MinMaxUInt(pDX, m_nTabWidth, 1, 20);
	DDX_Text(pDX, IDC_OPTIONS_EDITOR_WORDWRAP_FIXEDCOLUMN, m_nFixedColumn);
	DDV_MinMaxUInt(pDX, m_nFixedColumn, 1, 1000);
	DDX_CBIndex(pDX, IDC_OPTIONS_EDITOR_WORDWRAP_STYLE, m_nWordWrapStyle);
	DDX_Check(pDX, IDC_SHOW_LINENUMBERS, m_bShowLineNumbers);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionPageEditor, CPropertyPage)
	//{{AFX_MSG_MAP(COptionPageEditor)
	ON_CBN_SELCHANGE(IDC_OPTIONS_EDITOR_WORDWRAP_STYLE, UpdateControlStates)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionPageEditor message handlers

void COptionPageEditor::OnOK() 
{
	UpdateData();

	// Store settings to configuration
	g_configuration.m_nTabWidth = m_nTabWidth;
	g_configuration.m_nFixedColumnWrap = m_nFixedColumn;
	g_configuration.m_WordWrapStyle = (TCRYSTALWORDWRAP)m_nWordWrapStyle;
	g_configuration.m_bShowLineNumbers = m_bShowLineNumbers;

	// this message will be send to all windows of the application
	AfxGetMainWnd()->SendMessage(WM_SYSCOLORCHANGE);

	CPropertyPage::OnOK();
}

void COptionPageEditor::UpdateControlStates() 
{
	UpdateData();

	m_wndFixedColumn.EnableWindow(m_nWordWrapStyle == WORD_WRAP_FIXEDCOLUMN);
}


BOOL COptionPageEditor::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	UpdateControlStates();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
