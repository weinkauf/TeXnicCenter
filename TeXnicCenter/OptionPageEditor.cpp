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

IMPLEMENT_DYNCREATE(COptionPageEditor,PropertyPage)

COptionPageEditor::COptionPageEditor() : PropertyPage(COptionPageEditor::IDD)
{
    //{{AFX_DATA_INIT(COptionPageEditor)
    m_nTabWidth = CConfiguration::GetInstance()->m_nTabWidth;
    m_nFixedColumn = CConfiguration::GetInstance()->m_nFixedColumnWrap;
    m_nWordWrapStyle = CConfiguration::GetInstance()->m_WordWrapStyle - 1;
    m_bShowLineNumbers = CConfiguration::GetInstance()->m_bShowLineNumbers;
    //}}AFX_DATA_INIT
}

COptionPageEditor::~COptionPageEditor()
{
}

void COptionPageEditor::DoDataExchange(CDataExchange* pDX)
{
    PropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(COptionPageEditor)
    DDX_Control(pDX,IDC_OPTIONS_EDITOR_WORDWRAP_FIXEDCOLUMN,m_wndFixedColumn);
    DDX_Text(pDX,IDC_EDITOR_TABWIDTH,m_nTabWidth);
    DDV_MinMaxUInt(pDX,m_nTabWidth,1,20);
    DDX_Text(pDX,IDC_OPTIONS_EDITOR_WORDWRAP_FIXEDCOLUMN,m_nFixedColumn);
    DDV_MinMaxUInt(pDX,m_nFixedColumn,1,1000);
    DDX_CBIndex(pDX,IDC_OPTIONS_EDITOR_WORDWRAP_STYLE,m_nWordWrapStyle);
    DDX_Check(pDX,IDC_SHOW_LINENUMBERS,m_bShowLineNumbers);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COptionPageEditor,PropertyPage)
//{{AFX_MSG_MAP(COptionPageEditor)
ON_CBN_SELCHANGE(IDC_OPTIONS_EDITOR_WORDWRAP_STYLE,UpdateControlStates)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionPageEditor message handlers

void COptionPageEditor::OnOK()
{
    UpdateData();

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
    CConfiguration::GetInstance()->m_WordWrapStyle = (TCRYSTALWORDWRAP)m_nWordWrapStyle + 1;
    CConfiguration::GetInstance()->m_bShowLineNumbers = m_bShowLineNumbers;

    // this message will be send to all windows of the application
    AfxGetMainWnd()->SendMessage(WM_SYSCOLORCHANGE);

    PropertyPage::OnOK();
}

void COptionPageEditor::UpdateControlStates()
{
    UpdateData();

    m_wndFixedColumn.EnableWindow(m_nWordWrapStyle + 1 == WORD_WRAP_FIXEDCOLUMN);
}

BOOL COptionPageEditor::OnInitDialog()
{
    PropertyPage::OnInitDialog();

    UpdateControlStates();

    return TRUE; // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
