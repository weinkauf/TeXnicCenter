#pragma once


// FindReplaceDlg dialog

class FindReplaceDlg : public CScintillaFindReplaceDlg
{
	int whole_file_;

	DECLARE_DYNAMIC(FindReplaceDlg)

public:
	FindReplaceDlg();   // standard constructor
	~FindReplaceDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()

public:	
	bool ReplaceSelection() const;
};
