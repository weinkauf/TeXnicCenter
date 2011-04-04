#pragma once
#include "afxcmn.h"


// TransparencyDlg dialog

class TransparencyDlg : public CDialog
{
	DECLARE_DYNAMIC(TransparencyDlg)

private:
	int old_value_;

public:
	TransparencyDlg(CWnd* pParent = NULL);   // standard constructor
	~TransparencyDlg();

// Dialog Data
	enum { IDD = IDD_TRANSPARENCY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CSliderCtrl slider_;

protected:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	void UpdateValueLabel();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

public:
	static void SetTransparency( CWnd* w, int value );
	//static int GetTransparency(CWnd* w);
};
