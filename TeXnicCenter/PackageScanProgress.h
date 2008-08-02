#if !defined(AFX_PACKAGESCANPROGRESS_H__B04DB6B1_A420_4ED8_A59C_E68A0016261B__INCLUDED_)
#define AFX_PACKAGESCANPROGRESS_H__B04DB6B1_A420_4ED8_A59C_E68A0016261B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PackageScanProgress.h : header file
//
#include "stylefile.h"

/////////////////////////////////////////////////////////////////////////////
// CPackageScanProgress dialog

class CPackageScanProgress : public CDialog, public CStyleFileListener {
    // Construction
public:
    CPackageScanProgress(CWnd* pParent = NULL); // standard constructor

    // Dialog Data
    //{{AFX_DATA(CPackageScanProgress)

    enum {
        IDD = IDD_PACKAGE_SCAN_PROGRESS
    };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPackageScanProgress)
protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
    //}}AFX_VIRTUAL


    // Implementation
    virtual void OnDirectoryFound(const CString &dirname);
    virtual void OnFileFound(const CString &filename);
    virtual void OnCommandFound(CLaTeXCommand &command);
    virtual bool OnQueryCancel();
protected:

    // Generated message map functions
    //{{AFX_MSG(CPackageScanProgress)
    virtual BOOL OnInitDialog();
    afx_msg void OnCancel();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    bool m_Cancel;
    void SetText(int id, LPCTSTR text);
    int m_CommandsFound;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PACKAGESCANPROGRESS_H__B04DB6B1_A420_4ED8_A59C_E68A0016261B__INCLUDED_)
