// PackageScanProgress.cpp : implementation file
//


#include "stdafx.h"
#include "texniccenter.h"
#include "PackageScanProgress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPackageScanProgress dialog


CPackageScanProgress::CPackageScanProgress(CWnd* pParent /*=NULL*/)
	: CDialog(CPackageScanProgress::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPackageScanProgress)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	Create(CPackageScanProgress::IDD, pParent);
	m_CommandsFound = 0;
}


void CPackageScanProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPackageScanProgress)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPackageScanProgress, CDialog)
	//{{AFX_MSG_MAP(CPackageScanProgress)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPackageScanProgress message handlers

void CPackageScanProgress::OnDirectoryFound(CString &dirname) 
{
	SetText(IDC_DIR, dirname);
}

void CPackageScanProgress::OnFileFound(CString &filename) 
{
	SetText(IDC_FILE, filename);
}

BOOL CPackageScanProgress::OnQueryCancel() 
{
	return m_Cancel;
}

void CPackageScanProgress::OnCommandFound(CLaTeXCommand &command) 
{
	m_CommandsFound++;
	SetText(IDC_COMMANDS, command.ToLaTeX());
}

BOOL CPackageScanProgress::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetText(IDC_DIR, "...");
	SetText(IDC_FILE, "...");
	SetText(IDC_COMMANDS, "...");

	m_Cancel = FALSE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPackageScanProgress::SetText(int id, LPCTSTR text)
{
	CStatic *l = (CStatic *)GetDlgItem(id);
	ASSERT(l != NULL);
	l->SetWindowText(text);
}

void CPackageScanProgress::OnCancel() 
{
	m_Cancel = TRUE;
	
}
