/********************************************************************
*
* This file is part of the TeXnicCenter-system
*
* Copyright (C) 1999-2000 Sven Wiegand
* Copyright (C) 2000-$CurrentYear$ ToolsCenter
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

#include "stdafx.h"
#include "TeXnicCenter.h"
#include "ProfilePageViewer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// class CProfilePageViewer 
//-------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CProfilePageViewer, CProfilePage)
	//{{AFX_MSG_MAP(CProfilePageViewer)
	ON_BN_CLICKED(IDC_VIEW_CMDTYPE, OnCmdTypeChanged)
	ON_BN_CLICKED(IDC_VIEW_CMDTYPE1, OnCmdTypeChanged)
	ON_BN_CLICKED(IDC_VIEWEX_CMDTYPE, OnCmdTypeChanged)
	ON_BN_CLICKED(IDC_VIEWEX_CMDTYPE1, OnCmdTypeChanged)
	ON_BN_CLICKED(IDC_CLOSE_CMDTYPE, OnCmdTypeChanged)
	ON_BN_CLICKED(IDC_CLOSE_CMDTYPE1, OnCmdTypeChanged)
	ON_BN_CLICKED(IDC_CLOSE_CMDTYPE2, OnCmdTypeChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CProfilePageViewer::CProfilePageViewer()
: CProfilePage(CProfilePageViewer::IDD),
	m_wndBrowseViewerPath(IDC_EDIT_VIEWERPATH, TRUE, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, CString((LPCTSTR)STE_COMPILER_FILTER)),
	m_wndViewCmdPh(IDR_POPUP_PLACEHOLDER),
	m_wndViewExCmdPh(IDR_POPUP_PLACEHOLDEREX),
	m_wndCloseCmdPh(IDR_POPUP_PLACEHOLDER),
	m_pProfile(NULL)
{
	//{{AFX_DATA_INIT(CProfilePageViewer)
	m_nViewCmdType = cmdCommandLine;
	m_nViewExCmdType = cmdCommandLine;
	m_nCloseCmdType = cmdNone;
	//}}AFX_DATA_INIT
}


void CProfilePageViewer::UpdateControlStates()
{
	if (m_pProfile)
		EnableAllControls();

	if (m_nViewCmdType == cmdCommandLine)
	{
		m_wndViewTopicTitle.ShowWindow(SW_HIDE);
		m_wndViewDdeTopic.ShowWindow(SW_HIDE);
		m_wndViewServerTitle.ShowWindow(SW_HIDE);
		m_wndViewDdeServer.ShowWindow(SW_HIDE);
	}
	else
	{
		m_wndViewTopicTitle.ShowWindow(SW_SHOW);
		m_wndViewDdeTopic.ShowWindow(SW_SHOW);
		m_wndViewServerTitle.ShowWindow(SW_SHOW);
		m_wndViewDdeServer.ShowWindow(SW_SHOW);
	}

	if (m_nViewExCmdType == cmdCommandLine)
	{
		m_wndViewExTopicTitle.ShowWindow(SW_HIDE);
		m_wndViewExDdeTopic.ShowWindow(SW_HIDE);
		m_wndViewExServerTitle.ShowWindow(SW_HIDE);
		m_wndViewExDdeServer.ShowWindow(SW_HIDE);
	}
	else
	{
		m_wndViewExTopicTitle.ShowWindow(SW_SHOW);
		m_wndViewExDdeTopic.ShowWindow(SW_SHOW);
		m_wndViewExServerTitle.ShowWindow(SW_SHOW);
		m_wndViewExDdeServer.ShowWindow(SW_SHOW);
	}

	if (m_nCloseCmdType == cmdNone)
	{
		m_wndCloseCmdTitle.ShowWindow(SW_HIDE);
		m_wndCloseCmd.ShowWindow(SW_HIDE);
		m_wndCloseCmdPh.ShowWindow(SW_HIDE);
		m_wndCloseCmdTopicTitle.ShowWindow(SW_HIDE);
		m_wndCloseCmdDdeTopic.ShowWindow(SW_HIDE);
		m_wndCloseCmdServerTitle.ShowWindow(SW_HIDE);
		m_wndCloseCmdDdeServer.ShowWindow(SW_HIDE);
	}
	else if (m_nCloseCmdType == cmdCommandLine)
	{
		m_wndCloseCmdTitle.ShowWindow(SW_SHOW);
		m_wndCloseCmd.ShowWindow(SW_SHOW);
		m_wndCloseCmdPh.ShowWindow(SW_SHOW);
		m_wndCloseCmdTopicTitle.ShowWindow(SW_HIDE);
		m_wndCloseCmdDdeTopic.ShowWindow(SW_HIDE);
		m_wndCloseCmdServerTitle.ShowWindow(SW_HIDE);
		m_wndCloseCmdDdeServer.ShowWindow(SW_HIDE);
	}
	else
	{
		m_wndCloseCmdTitle.ShowWindow(SW_SHOW);
		m_wndCloseCmd.ShowWindow(SW_SHOW);
		m_wndCloseCmdPh.ShowWindow(SW_SHOW);
		m_wndCloseCmdTopicTitle.ShowWindow(SW_SHOW);
		m_wndCloseCmdDdeTopic.ShowWindow(SW_SHOW);
		m_wndCloseCmdServerTitle.ShowWindow(SW_SHOW);
		m_wndCloseCmdDdeServer.ShowWindow(SW_SHOW);
	}

	if (!m_pProfile)
		EnableAllControls(FALSE);
}


void CProfilePageViewer::UpdateCmdTypes(BOOL bSaveAndValidate /*= TRUE*/)
{
	CDataExchange	dx(this, bSaveAndValidate);

	DDX_Radio(&dx, IDC_VIEW_CMDTYPE, m_nViewCmdType);
	DDX_Radio(&dx, IDC_VIEWEX_CMDTYPE, m_nViewExCmdType);
	DDX_Radio(&dx, IDC_CLOSE_CMDTYPE, m_nCloseCmdType);

	if (bSaveAndValidate && m_pProfile)
	{
		m_pProfile->GetViewProjectCmd().SetActiveCommand(m_nViewCmdType);
		m_pProfile->GetViewCurrentCmd().SetActiveCommand(m_nViewExCmdType);
		if (m_nCloseCmdType == cmdNone)
			m_pProfile->SetCloseView(FALSE);
		else
		{
			m_pProfile->SetCloseView(TRUE);
			m_pProfile->GetViewCloseCmd().SetActiveCommand(m_nCloseCmdType);
		}
	}
}


void CProfilePageViewer::OnUpdateDataSet(CProfile *pProfile)
{
	m_pProfile = pProfile;

	if (m_pProfile)
	{
		m_nViewCmdType = pProfile->GetViewProjectCmd().GetActiveCommand();
		m_nViewExCmdType = pProfile->GetViewCurrentCmd().GetActiveCommand();
		if (pProfile->GetCloseView())
			m_nCloseCmdType = pProfile->GetViewCloseCmd().GetActiveCommand();
		else
			m_nCloseCmdType = cmdNone;
	}

	if (IsWindow(m_hWnd))
	{
		UpdateControlStates();
		UpdateCmdTypes(FALSE);
		UpdateData(FALSE);
	}
}


void CProfilePageViewer::DoDataExchange(CDataExchange* pDX)
{
	CString	strViewerPath;
	CString	strViewCmd, strViewDdeTopic, strViewDdeServer;
	CString	strViewExCmd, strViewExDdeTopic, strViewExDdeServer;
	CString	strCloseCmd, strCloseDdeTopic, strCloseDdeServer;

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// prepare data
	if (m_pProfile && !pDX->m_bSaveAndValidate)
	{
		// viewer path
		strViewerPath = m_pProfile->GetViewerPath();

		// view command
		CProfile::CCommand	cmdView = m_pProfile->GetViewProjectCmd();
		m_nViewCmdType = cmdView.GetActiveCommand();

		if (m_nViewCmdType == cmdCommandLine)
			strViewCmd = cmdView.GetProcessCommand().GetArguments();
		else
			strViewCmd = cmdView.GetDdeCommand().GetCommand();

		strViewDdeTopic = cmdView.GetDdeCommand().GetTopic();
		strViewDdeServer = cmdView.GetDdeCommand().GetServerName();

		// view current command
		CProfile::CCommand	cmdViewEx = m_pProfile->GetViewCurrentCmd();
		m_nViewExCmdType = cmdViewEx.GetActiveCommand();

		if (m_nViewExCmdType == cmdCommandLine)
			strViewExCmd = cmdViewEx.GetProcessCommand().GetArguments();
		else
			strViewExCmd = cmdViewEx.GetDdeCommand().GetCommand();

		strViewExDdeTopic = cmdViewEx.GetDdeCommand().GetTopic();
		strViewExDdeServer = cmdViewEx.GetDdeCommand().GetServerName();

		// close command
		if (m_pProfile->GetCloseView())
		{
			CProfile::CCommand	cmdClose = m_pProfile->GetViewCloseCmd();
			m_nCloseCmdType = cmdClose.GetActiveCommand();

			if (m_nCloseCmdType == cmdCommandLine)
				strCloseCmd = cmdClose.GetProcessCommand().GetArguments();
			else if (m_nCloseCmdType == cmdDde)
				strCloseCmd = cmdClose.GetDdeCommand().GetCommand();

			strCloseDdeTopic = cmdClose.GetDdeCommand().GetTopic();
			strCloseDdeServer = cmdClose.GetDdeCommand().GetServerName();
		}
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// exchange with controls
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProfilePageViewer)
	DDX_Control(pDX, IDC_TITLE_VIEWEXDDESERVER, m_wndViewExServerTitle);
	DDX_Control(pDX, IDC_TITLE_VIEWDDESERVER, m_wndViewServerTitle);
	DDX_Control(pDX, IDC_TITLE_CLOSEDDESERVER, m_wndCloseCmdServerTitle);
	DDX_Control(pDX, IDC_EDIT_VIEWEXDDESERVER, m_wndViewExDdeServer);
	DDX_Control(pDX, IDC_EDIT_VIEWDDESERVER, m_wndViewDdeServer);
	DDX_Control(pDX, IDC_EDIT_CLOSEDDESERVER, m_wndCloseCmdDdeServer);
	DDX_Control(pDX, IDC_EDIT_CLOSEDDETOPIC, m_wndCloseCmdDdeTopic);
	DDX_Control(pDX, IDC_CLOSECMD_PLACEHOLDER, m_wndCloseCmdPh);
	DDX_Control(pDX, IDC_EDIT_CLOSECMD, m_wndCloseCmd);
	DDX_Control(pDX, IDC_TITLE_CLOSECMD, m_wndCloseCmdTitle);
	DDX_Control(pDX, IDC_VIEWEXCMD_PLACEHOLDER, m_wndViewExCmdPh);
	DDX_Control(pDX, IDC_EDIT_VIEWEXDDETOPIC, m_wndViewExDdeTopic);
	DDX_Control(pDX, IDC_TITLE_VIEWEXDDETOPIC, m_wndViewExTopicTitle);
	DDX_Control(pDX, IDC_EDIT_VIEWEXCMD, m_wndViewExCmd);
	DDX_Control(pDX, IDC_TITLE_VIEWEXCMD, m_wndViewExCmdTitle);
	DDX_Control(pDX, IDC_EDIT_VIEWDDETOPIC, m_wndViewDdeTopic);
	DDX_Control(pDX, IDC_TITLE_VIEWDDETOPIC, m_wndViewTopicTitle);
	DDX_Control(pDX, IDC_TITLE_VIEWCMD, m_wndViewCmdTitle);
	DDX_Control(pDX, IDC_VIEWCMD_PLACEHOLDER, m_wndViewCmdPh);
	DDX_Control(pDX, IDC_EDIT_VIEWCMD, m_wndViewCmd);
	DDX_Control(pDX, IDC_BROWSE_VIEWERPATH, m_wndBrowseViewerPath);
	DDX_Control(pDX, IDC_TITLE_CLOSEDDETOPIC, m_wndCloseCmdTopicTitle);
	DDX_Text(pDX, IDC_EDIT_VIEWCMD, strViewCmd);
	DDX_Text(pDX, IDC_EDIT_VIEWDDETOPIC, strViewDdeTopic);
	DDX_Text(pDX, IDC_EDIT_VIEWEXCMD, strViewExCmd);
	DDX_Text(pDX, IDC_EDIT_VIEWEXDDETOPIC, strViewExDdeTopic);
	DDX_Text(pDX, IDC_EDIT_CLOSECMD, strCloseCmd);
	DDX_Text(pDX, IDC_EDIT_CLOSEDDETOPIC, strCloseDdeTopic);
	DDX_Text(pDX, IDC_EDIT_VIEWERPATH, strViewerPath);
	DDX_Text(pDX, IDC_EDIT_VIEWDDESERVER, strViewDdeServer);
	DDX_Text(pDX, IDC_EDIT_VIEWEXDDESERVER, strViewExDdeServer);
	DDX_Text(pDX, IDC_EDIT_CLOSEDDESERVER, strCloseDdeServer);
	//}}AFX_DATA_MAP

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// retrieve data
	if (m_pProfile && pDX->m_bSaveAndValidate)
	{
		// viewer path
		m_pProfile->SetViewerPath(strViewerPath);

		// view command
		CProfile::CCommand	cmdView = m_pProfile->GetViewProjectCmd();

		cmdView.SetActiveCommand(m_nViewCmdType);
		if (m_nViewCmdType == cmdCommandLine)
		{
			CProcessCommand	cmd;
			cmd.Set(strViewerPath, strViewCmd);
			cmdView.SetProcessCommand(cmd);
		}
		else
		{
			CDdeCommand	cmd;
			cmd.SetExecutable(strViewerPath);
			cmd.SetCommand(strViewCmd);
			cmd.SetServer(strViewDdeServer, strViewDdeTopic);
			cmdView.SetDdeCommand(cmd);
		}

		m_pProfile->SetViewProjectCmd(cmdView);

		// view current command
		CProfile::CCommand	cmdViewEx = m_pProfile->GetViewCurrentCmd();

		cmdViewEx.SetActiveCommand(m_nViewExCmdType);
		if (m_nViewExCmdType == cmdCommandLine)
		{
			CProcessCommand	cmd;
			cmd.Set(strViewerPath, strViewExCmd);
			cmdViewEx.SetProcessCommand(cmd);
		}
		else
		{
			CDdeCommand	cmd;
			cmd.SetExecutable(strViewerPath);
			cmd.SetCommand(strViewExCmd);
			cmd.SetServer(strViewExDdeServer, strViewExDdeTopic);
			cmdViewEx.SetDdeCommand(cmd);
		}

		m_pProfile->SetViewCurrentCmd(cmdViewEx);

		// view command
		if (m_nCloseCmdType == cmdNone)
			m_pProfile->SetCloseView(FALSE);
		else
		{
			m_pProfile->SetCloseView(TRUE);

			CProfile::CCommand	cmdClose = m_pProfile->GetViewCloseCmd();

			cmdClose.SetActiveCommand(m_nCloseCmdType);
			if (m_nCloseCmdType == cmdCommandLine)
			{
				CProcessCommand	cmd;
				cmd.Set(strViewerPath, strCloseCmd);
				cmdClose.SetProcessCommand(cmd);
			}
			else if (m_nCloseCmdType == cmdDde)
			{
				CDdeCommand	cmd;
				cmd.SetCommand(strCloseCmd);
				cmd.SetServer(strCloseDdeServer, strCloseDdeTopic);
				cmdClose.SetDdeCommand(cmd);
			}
			
			m_pProfile->SetViewCloseCmd(cmdClose);
		}
	}
}


BOOL CProfilePageViewer::OnInitDialog() 
{
	CProfilePage::OnInitDialog();
	
	// initialize control
	m_wndViewCmdPh.AttachEditCtrl(&m_wndViewCmd);
	m_wndViewExCmdPh.AttachEditCtrl(&m_wndViewExCmd);
	m_wndCloseCmdPh.AttachEditCtrl(&m_wndCloseCmd);

	UpdateControlStates();
	UpdateCmdTypes(FALSE);
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void CProfilePageViewer::OnCmdTypeChanged() 
{
	// store information for current command types
	UpdateData();

	// get new command types
	UpdateCmdTypes(TRUE);

	// enable neede windows
	UpdateControlStates();

	// show command specific configuration
	UpdateData(FALSE);
}
