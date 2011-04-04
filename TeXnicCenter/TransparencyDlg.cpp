// TransparencyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TransparencyDlg.h"
#include "configuration.h"

// TransparencyDlg dialog

IMPLEMENT_DYNAMIC(TransparencyDlg, CDialog)

TransparencyDlg::TransparencyDlg(CWnd* pParent /*=NULL*/)
: CDialog(TransparencyDlg::IDD, pParent)
, old_value_(CConfiguration::GetInstance()->GetTransparency())
{
}

TransparencyDlg::~TransparencyDlg()
{
}

void TransparencyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, slider_);
}


BEGIN_MESSAGE_MAP(TransparencyDlg, CDialog)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// TransparencyDlg message handlers

BOOL TransparencyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	slider_.SetBuddy(GetDlgItem(IDC_SLIDER_VALUE),FALSE);
	slider_.SetRange(0,100);
	slider_.SetPos(old_value_);
	UpdateValueLabel();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void TransparencyDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar->GetSafeHwnd() == slider_.GetSafeHwnd()) {
		UpdateValueLabel();
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void TransparencyDlg::OnOK()
{
	CConfiguration::GetInstance()->SetTransparency(slider_.GetPos());
	CDialog::OnOK();
}

void TransparencyDlg::OnCancel()
{
	SetTransparency(AfxGetMainWnd(),old_value_);
	// Windows 2000 fix
	AfxGetMainWnd()->RedrawWindow(0,0,RDW_ALLCHILDREN|RDW_INVALIDATE|RDW_UPDATENOW);
	CDialog::OnCancel();
}

void TransparencyDlg::SetTransparency( CWnd* w, int value )
{
	if (value != 0 && !(w->GetExStyle() & WS_EX_LAYERED))
		w->ModifyStyleEx(0,WS_EX_LAYERED);
	else if (value == 0 && w->GetExStyle() & WS_EX_LAYERED)
		w->ModifyStyleEx(WS_EX_LAYERED,0);

	::SetLayeredWindowAttributes(w->GetSafeHwnd(),0,
		static_cast<BYTE>((100 - value) * 255 / 100.0),LWA_ALPHA);
}

//int TransparencyDlg::GetTransparency( CWnd* w )
//{
//	BYTE b;
//	DWORD flags;
//
//	if (!::GetLayeredWindowAttributes(w->GetSafeHwnd(),0,&b,&flags) || !(flags & LWA_ALPHA))
//		b = 255;
//
//	return static_cast<int>((255 - b) * 100 / 255.0);
//}

void TransparencyDlg::UpdateValueLabel()
{
	int value = slider_.GetPos();

	if (CWnd* w = AfxGetMainWnd()) {
		CString fmt;
		fmt.Format(_T("%i %%"),value);

		SetDlgItemText(IDC_SLIDER_VALUE,fmt);

		SetTransparency(w, value);
	}
}