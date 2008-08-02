/********************************************************************
 *
 * This file is part of the TeXnicCenter-system
 *
 * Copyright (C) 2007 Tino Weinkauf
 * Copyright (C) 2007-$CurrentYear$ ToolsCenter
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * If you have further questions or if you want to support
 * further TeXnicCenter development, visit the TeXnicCenter-homepage
 *
 *    http://www.ToolsCenter.org
 *
 *********************************************************************/

/********************************************************************
 *
 * $Id$
 *
 ********************************************************************/

#include "stdafx.h"
#include "texniccenter.h"
#include "TextOutsourceDlg.h"
#include "configuration.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextOutsourceDlg dialog

CTextOutsourceDlg::CTextOutsourceDlg(CWnd* pParent /*=NULL*/)
: CDialog(CTextOutsourceDlg::IDD, pParent)
, m_BrowseFolderButton(IDC_OUTSOURCE_DIRECTORY, CString((LPCTSTR)STE_GET_PATH))
//, NewPath(_T(""))
{
    //{{AFX_DATA_INIT(CTextOutsourceDlg)
    //CmdLeft = _T("");
    //CmdRight = _T("");
    //m_Directory = _T("");
    //m_Filename = _T("");
    m_nIncludeType = 0;
    m_bOpenNewFile = FALSE;
    m_nOpenNewFileType = -1;
    //}}AFX_DATA_INIT

    /////////////////////////////////////
    //Init with data from configuration

    m_nIncludeType = CConfiguration::GetInstance()->m_TextOutsource_nIncludeType;
    if (m_nIncludeType == 2) {
        CmdLeft = CConfiguration::GetInstance()->m_TextOutsource_strUserCmdLeft;
        CmdRight = CConfiguration::GetInstance()->m_TextOutsource_strUserCmdRight;
    }

    m_bOpenNewFile = CConfiguration::GetInstance()->m_TextOutsource_bOpenNewFile;
    m_nOpenNewFileType = CConfiguration::GetInstance()->m_TextOutsource_nOpenNewFileType;
}

void CTextOutsourceDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CTextOutsourceDlg)
    DDX_Control(pDX, IDC_OUTSOURCE_OPEN_BACKGROUND, m_wndRadioOpenBackground);
    DDX_Control(pDX, IDC_OUTSOURCE_BROWSEBUTTON, m_BrowseFolderButton);
    DDX_Control(pDX, IDOK, m_wndOkButton);
    DDX_Control(pDX, IDC_OUTSOURCE_CMDRIGHT, m_CmdRightCtrl);
    DDX_Control(pDX, IDC_OUTSOURCE_CMDLEFT, m_CmdLeftCtrl);
    DDX_Text(pDX, IDC_OUTSOURCE_CMDLEFT, CmdLeft);
    DDX_Text(pDX, IDC_OUTSOURCE_CMDRIGHT, CmdRight);
    DDX_Text(pDX, IDC_OUTSOURCE_DIRECTORY, m_Directory);
    DDX_Text(pDX, IDC_OUTSOURCE_FILENAME, m_Filename);
    DDX_Radio(pDX, IDC_OUTSOURCE_CMDINPUT, m_nIncludeType);
    DDX_Check(pDX, IDC_OUTSOURCE_OPEN, m_bOpenNewFile);
    DDX_Radio(pDX, IDC_OUTSOURCE_OPEN_BACKGROUND, m_nOpenNewFileType);
    //}}AFX_DATA_MAP

    DDX_Control(pDX, IDC_OUTSOURCE_OPEN_FOREGROUND, m_wndRadioOpenForeground);
}

BEGIN_MESSAGE_MAP(CTextOutsourceDlg, CDialog)
//{{AFX_MSG_MAP(CTextOutsourceDlg)
ON_BN_CLICKED(IDC_OUTSOURCE_CMDINPUT, OnIncludeTypeChanged)
ON_EN_CHANGE(IDC_OUTSOURCE_FILENAME, UpdateControlStates)
ON_BN_CLICKED(IDC_OUTSOURCE_CMDINCLUDE, OnIncludeTypeChanged)
ON_BN_CLICKED(IDC_OUTSOURCE_CMDUSERDEF, OnIncludeTypeChanged)
ON_EN_CHANGE(IDC_OUTSOURCE_DIRECTORY, UpdateControlStates)
ON_BN_CLICKED(IDC_OUTSOURCE_OPEN, UpdateControlStates)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextOutsourceDlg message handlers

void CTextOutsourceDlg::OnOK()
{
    //Get data from UI
    UpdateData(TRUE);

    //Create new path name
    CheckAndFixFileName();
    // - get directory
    NewPath = CPathTool(m_Directory);
    if (!NewPath.Exists()) {
        AfxMessageBox(STE_NOFOLDER, MB_ICONEXCLAMATION | MB_OK);
        return;
    }
    // - append file name
    if (m_Filename.GetLength() <= 0) return; //Should not happen, because the ok button should be grayed out
    NewPath.Append(m_Filename);

    //Append ".tex" extension?
    if (NewPath.GetFileExtension().GetLength() == 0) {
        NewPath = CPathTool(NewPath.GetBase() + _T(".tex"));
    }

    //Check that it does not exist or ask for permission to overwrite.
    if (NewPath.Exists()) {
        CString MsgText;
        MsgText.Format(STE_DOCUMENT_ALREADYEXISTS, NewPath.GetPath());
        if (AfxMessageBox(MsgText, MB_ICONWARNING | MB_YESNO) == IDNO) return;
    }

    //Save settings in the config
    CConfiguration::GetInstance()->m_TextOutsource_nIncludeType = m_nIncludeType;
    CConfiguration::GetInstance()->m_TextOutsource_bOpenNewFile = m_bOpenNewFile;
    CConfiguration::GetInstance()->m_TextOutsource_nOpenNewFileType = m_nOpenNewFileType;

    CDialog::OnOK();
}

void CTextOutsourceDlg::OnIncludeTypeChanged()
{
    //In order to save user commands
    const int OldIncludeType = m_nIncludeType;

    //Get (new) data from UI
    UpdateData(TRUE);

    //Save user commands, if we are in user style and switch to default style
    if (OldIncludeType == 2 && m_nIncludeType != 2) {
        CConfiguration::GetInstance()->m_TextOutsource_strUserCmdLeft = CmdLeft;
        CConfiguration::GetInstance()->m_TextOutsource_strUserCmdRight = CmdRight;
    }

    //Depending on the value of the radio group, new text is inserted.
    bool bActivateEditCtrls(true);
    switch (m_nIncludeType) {
        case 0:
        {
            CmdLeft = _T("\\input{");
            CmdRight = _T("}");
            bActivateEditCtrls = false;
            break;
        }

        case 1:
        {
            CmdLeft = _T("\\include{");
            CmdRight = _T("}");
            bActivateEditCtrls = false;
            break;
        }

        case 2:
        {
            CmdLeft = CConfiguration::GetInstance()->m_TextOutsource_strUserCmdLeft;
            CmdRight = CConfiguration::GetInstance()->m_TextOutsource_strUserCmdRight;
            bActivateEditCtrls = true;
            break;
        }
    }

    //Activate or deactivate edit controls?
    m_CmdLeftCtrl.EnableWindow(bActivateEditCtrls);
    m_CmdRightCtrl.EnableWindow(bActivateEditCtrls);

    UpdateData(FALSE);
}

BOOL CTextOutsourceDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    //Init the edit controls depending on the state of the radio box
    OnIncludeTypeChanged();

    //Init other stuff
    UpdateControlStates();

    return TRUE; // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CTextOutsourceDlg::UpdateControlStates()
{
    UpdateData(true);

    CheckAndFixFileName();
    m_wndOkButton.EnableWindow(m_Filename.GetLength() > 0 && m_Directory.GetLength() > 0);

    m_wndRadioOpenBackground.EnableWindow(m_bOpenNewFile);
    m_wndRadioOpenForeground.EnableWindow(m_bOpenNewFile);
}

void CTextOutsourceDlg::CheckAndFixFileName()
{
    m_Filename.TrimLeft();
    m_Filename.TrimRight();
    m_Directory.TrimLeft();
    m_Directory.TrimRight();
}
