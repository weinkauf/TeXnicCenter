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
#include "ProjectNewDialog.h"
#include "Global.h"
#include "configuration.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//-------------------------------------------------------------------
// CEmptyProjectTemplateItem
//-------------------------------------------------------------------

IMPLEMENT_DYNCREATE(CEmptyProjectTemplateItem, CObject);


CEmptyProjectTemplateItem::CEmptyProjectTemplateItem()
:	CProjectTemplateItem()
{}


BOOL CEmptyProjectTemplateItem::InitItem(LPCTSTR lpszPath, CImageList &ImageList)
{
	//add image to image list and remember index
	HICON	hIcon = theApp.LoadIcon(IDR_LATEXTYPE);
	m_nImageIndex = hIcon? ImageList.Add(hIcon) : -1;

	return TRUE;
}


CString CEmptyProjectTemplateItem::GetTitle()
{
	return CString((LPCTSTR)STE_EMPTYPROJECT_TITLE);
}


CString CEmptyProjectTemplateItem::GetDescription()
{
	return CString((LPCTSTR)STE_EMPTYPROJECT_DESCRIPTION);
}


int CEmptyProjectTemplateItem::GetImageIndex()
{
	return m_nImageIndex;
}


BOOL CEmptyProjectTemplateItem::InitProject(CLatexProject *pProject, LPCTSTR lpszCr)
{
	CString	strTargetPath(pProject->GetMainPath());

	if (CPathTool::Exists(strTargetPath))
	{
		CString	strMessage;
		strMessage.Format(STE_PROJECT_MAINFILEEXISTS, strTargetPath);
		if (AfxMessageBox(strMessage, MB_ICONWARNING|MB_YESNO) == IDNO)
		{
			if (AfxMessageBox(STE_PROJECT_USEEXISTINGFILE, MB_ICONQUESTION|MB_YESNO) == IDNO)
				return FALSE;
			else
				return TRUE;
		}
	}

	if (!CreateMainFile(strTargetPath, lpszCr))
	{
		AfxMessageBox(STE_PROJECT_CANNOTCREATEMAINFILE, MB_ICONSTOP|MB_OK);
		return FALSE;
	}

	return TRUE;
}


BOOL CEmptyProjectTemplateItem::CreateMainFile(LPCTSTR lpszTargetPath, LPCTSTR lpszCrLf)
{
	// create empty main file
	try
	{
		CFile			file(lpszTargetPath, CFile::modeCreate|CFile::modeWrite);
		CArchive	ar(&file, CArchive::store);
		ar.WriteString(lpszCrLf);	// place one line feed to define the format of the file
		ar.Close();
		file.Close();
	}
	catch (CException *pE)
	{
		pE->Delete();
		return FALSE;
	}

	return TRUE;
}


//-------------------------------------------------------------------
// CFileBasedProjectTemplateItem
//-------------------------------------------------------------------

IMPLEMENT_DYNCREATE(CFileBasedProjectTemplateItem, CObject);


CFileBasedProjectTemplateItem::CFileBasedProjectTemplateItem()
:	CProjectTemplateItem(),
	m_nImageIndex(-1)
{}


BOOL CFileBasedProjectTemplateItem::InitItem(LPCTSTR lpszPath, CImageList &ImageList)
{
	m_strPath = lpszPath;

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//extract description, if available
	CFile	file;
	if (!file.Open(m_strPath, CFile::modeRead))
		return FALSE;

	try
	{
		CArchive	ar(&file, CArchive::load);
		CString		strLine;

		if (ar.ReadString(strLine))
		{
			CString	strKey(_T("%DESCRIPTION: "));
			CString	strStartOfLine = strLine.Left(strKey.GetLength());
			strStartOfLine.MakeUpper();

			if (strStartOfLine == strKey)
				m_strDescription = strLine.Right(strLine.GetLength()-strKey.GetLength());
		}

		ar.Close();
		file.Close();
	}
	catch (CException *pE)
	{
		file.Abort();
		pE->Delete();
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//generate title
	m_strTitle = CPathTool::GetFileTitle(m_strPath);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//add image to image list and remember index
	HICON	hIcon = theApp.LoadIcon(IDR_LATEXTYPE);
	m_nImageIndex = hIcon? ImageList.Add(hIcon) : -1;

	return TRUE;
}


CString CFileBasedProjectTemplateItem::GetTitle()
{
	return m_strTitle;
}


CString CFileBasedProjectTemplateItem::GetDescription()
{
	return m_strDescription;
}


int CFileBasedProjectTemplateItem::GetImageIndex()
{
	return m_nImageIndex;
}


BOOL CFileBasedProjectTemplateItem::InitProject(CLatexProject *pProject, LPCTSTR lpszCr)
{
	CString	strTargetPath(pProject->GetMainPath());

	if (CPathTool::Exists(strTargetPath))
	{
		CString	strMessage;
		strMessage.Format(STE_PROJECT_MAINFILEEXISTS, strTargetPath);
		if (AfxMessageBox(strMessage, MB_ICONWARNING|MB_YESNO) == IDNO)
		{
			if (AfxMessageBox(STE_PROJECT_USEEXISTINGFILE, MB_ICONQUESTION|MB_YESNO) == IDNO)
				return FALSE;
			else
				return TRUE;
		}
	}

	if (!CreateMainFile(strTargetPath, lpszCr))
	{
		AfxMessageBox(STE_PROJECT_CANNOTCREATEMAINFILE, MB_ICONSTOP|MB_OK);
		return FALSE;
	}

	return TRUE;
}


BOOL CFileBasedProjectTemplateItem::CreateMainFile(LPCTSTR lpszTargetPath, LPCTSTR lpszCrLf)
{
	CFile	sourceFile;
	if (!sourceFile.Open(m_strPath, CFile::modeRead))
		return FALSE;

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// copy source file to destination file
	CFile	destFile;
	if (!destFile.Open(lpszTargetPath, CFile::modeCreate|CFile::modeWrite))
	{
		sourceFile.Close();
		return FALSE;
	}

	try
	{
		CArchive	source(&sourceFile, CArchive::load);
		CArchive	dest(&destFile, CArchive::store);
		CString		strLine;

		if (source.ReadString(strLine))
		{
			// skip leading descriptions
			CString	strKey(_T("%DESCRIPTION: "));
			CString	strStartOfLine = strLine.Left(strKey.GetLength());
			strStartOfLine.MakeUpper();

			if (strStartOfLine == strKey)
			{
				// skip leading empty lines
				while (source.ReadString(strLine) && strLine.IsEmpty());
			}
		}

		if (!strLine.IsEmpty())
			dest.WriteString(strLine + lpszCrLf);

		// copy other lines
		while (source.ReadString(strLine))
			dest.WriteString(strLine + lpszCrLf);

		source.Close();
		dest.Close();
		sourceFile.Close();
		destFile.Close();
	}
	catch (CException *pE)
	{
		sourceFile.Abort();
		destFile.Abort();
		pE->Delete();
	}

	return TRUE;
}


//-------------------------------------------------------------------
// CWizardBasedProjectTemplateItem
//-------------------------------------------------------------------

IMPLEMENT_DYNCREATE(CWizardBasedProjectTemplateItem, CObject);


CWizardBasedProjectTemplateItem::CWizardBasedProjectTemplateItem()
:	CProjectTemplateItem(),
	m_nImageIndex(-1)
{}


BOOL CWizardBasedProjectTemplateItem::InitItem(LPCTSTR lpszPath, CImageList &ImageList)
{
	m_strPath = lpszPath;

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// retrieve the wizard interface 
	CString	strProgId;
	strProgId.Format(_T("%s.ProjectWizard"), CPathTool::GetFileTitle(m_strPath));

	BSTR		bstrProgId = strProgId.AllocSysString();
	HRESULT	result = m_wizard.CoCreateInstance(bstrProgId, NULL, CLSCTX_INPROC_SERVER);

	if (result != S_OK)
		return FALSE;

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// get the icon
	HICON	hIcon = NULL;

	int	nIconIndex = m_wizard.GetIconIndex();
	if (nIconIndex >= 0)
		hIcon = ExtractIcon(AfxGetInstanceHandle(), m_strPath, nIconIndex);
	if (!hIcon)
		hIcon = theApp.LoadIcon(IDR_LATEXWIZARD);

	m_nImageIndex = ImageList.Add(hIcon);

	return TRUE;
}


CString CWizardBasedProjectTemplateItem::GetTitle()
{
	return m_wizard.GetTitle();
}


CString CWizardBasedProjectTemplateItem::GetDescription()
{
	return m_wizard.GetDescription();
}


int CWizardBasedProjectTemplateItem::GetImageIndex()
{
	return m_nImageIndex;
}


BOOL CWizardBasedProjectTemplateItem::InitProject(CLatexProject *pProject, LPCTSTR lpszCr)
{
	Interfaces::IProject	*pProjectInterface = pProject->GetProjectInterface();
	ASSERT(pProjectInterface);
	if (!pProjectInterface)
		return FALSE;

	BOOL	bResult = m_wizard.DoModal(pProjectInterface, lpszCr);
	pProjectInterface->Release();

	return bResult;
}


//-------------------------------------------------------------------
// CProjectNewDialog 
//-------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CProjectNewDialog, CTemplateDialog)
	//{{AFX_MSG_MAP(CProjectNewDialog)
	ON_EN_CHANGE(IDC_EDIT_PROJECTNAME, OnChangeProjectName)
	ON_EN_CHANGE(IDC_EDIT_PROJECTPATH, OnChangeProjectPath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CProjectNewDialog::CProjectNewDialog(CWnd* pParent /*=NULL*/)
: CTemplateDialog(CProjectNewDialog::IDD, pParent),
	m_wndBrowseButton(IDC_EDIT_PROJECTPATH, CString((LPCTSTR)STE_SELECT_PROJECT_PATH)),
	m_bIgnoreProjectPathChange(FALSE)
{
	//{{AFX_DATA_INIT(CProjectNewDialog)
	m_strProjectName = _T("");
	m_bUseBibTex = FALSE;
	m_bUseMakeIndex = FALSE;
	//}}AFX_DATA_INIT

	// Add template for empty project
	CTemplateItem	*pItem = new CEmptyProjectTemplateItem;
	if (pItem)
		AddTemplateItem(CString((LPCTSTR)STE_EMPTYPROJECT_CATEGORY), pItem);

	// Get Project base path
	m_strProjectBasePath = g_configuration.m_strProjectPath;
	if (m_strProjectBasePath.IsEmpty())
	{
		LPITEMIDLIST	lpidl;
		if (SHGetSpecialFolderLocation(AfxGetMainWnd()->m_hWnd, CSIDL_PERSONAL, &lpidl) == NOERROR)
		{
			SHGetPathFromIDList(lpidl, m_strProjectBasePath.GetBuffer(MAX_PATH));
			m_strProjectBasePath.ReleaseBuffer();

			// free memory
			LPMALLOC	lpMalloc;
			SHGetMalloc(&lpMalloc);
			if(lpMalloc)
				lpMalloc->Free(lpidl);
		}
	}

	m_strProjectPath = m_strProjectBasePath;
	m_nFileFormat = g_configuration.m_nStandardFileFormat;
}


void CProjectNewDialog::UpdateControlStates()
{
	m_wndOkButton.EnableWindow(!m_strProjectName.IsEmpty() && !m_strProjectPath.IsEmpty() && GetSelectedItem() > -1);
}


BOOL CProjectNewDialog::CreateDirectoryRecursive(LPCTSTR lpszDirectory)
{
	if (lpszDirectory[0] == _T('\0'))
		return FALSE;

	if (CPathTool::IsDirectory(lpszDirectory))
		return TRUE;

	CString	strParent = CPathTool::GetDirectory(lpszDirectory);
	if (!CPathTool::IsDirectory(strParent))
	{
		if (!CreateDirectoryRecursive(strParent))
			return FALSE;
	}

	return CreateDirectory(lpszDirectory, NULL);
}


void CProjectNewDialog::DoDataExchange(CDataExchange* pDX)
{
	CTemplateDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProjectNewDialog)
	DDX_Control(pDX, IDC_BUTTON_CREATE, m_wndOkButton);
	DDX_Control(pDX, IDC_BUTTON_BROWSE, m_wndBrowseButton);
	DDX_Text(pDX, IDC_EDIT_PROJECTNAME, m_strProjectName);
	DDX_Text(pDX, IDC_EDIT_PROJECTPATH, m_strProjectPath);
	DDX_CBIndex(pDX, IDC_SELECT_FILEFORMAT, m_nFileFormat);
	DDX_Check(pDX, IDC_CHECK_BIBTEX, m_bUseBibTex);
	DDX_Check(pDX, IDC_CHECK_MAKEINDEX, m_bUseMakeIndex);
	//}}AFX_DATA_MAP
}


void CProjectNewDialog::OnChangeProjectName() 
{
	UpdateData();
	UpdateControlStates();

	m_strProjectPath = CPathTool::Cat(m_strProjectBasePath, m_strProjectName);

	m_bIgnoreProjectPathChange = TRUE;
	UpdateData(FALSE);
	m_bIgnoreProjectPathChange = FALSE;
}


void CProjectNewDialog::OnChangeProjectPath() 
{
	UpdateData();
	UpdateControlStates();

	if (m_bIgnoreProjectPathChange)
		return;

	m_strProjectBasePath = m_strProjectPath;
}


void CProjectNewDialog::Create() 
{
	UpdateData();

	// retrieve selected element
	CProjectTemplateItem	*pItem = NULL;
	int	nItem = GetSelectedItem();
	if (nItem >= 0 && nItem < m_wndTemplateList.GetItemCount())
		pItem = reinterpret_cast<CProjectTemplateItem*>(m_wndTemplateList.GetItemData(nItem));

	// retrieve and modify project
	CLatexProject	*pProject = theApp.GetProject();
	ASSERT(pProject);
	if (!pProject)
		return;

	pProject->SetPathName(CPathTool::Cat(m_strProjectPath, m_strProjectName + _T(".tcp")));
	pProject->SetMainPath(CPathTool::Cat(m_strProjectPath, m_strProjectName + _T(".tex")));
	pProject->SetRunBibTex(m_bUseBibTex);
	pProject->SetRunMakeIndex(m_bUseMakeIndex);

	// create path to the project
	if (!CreateDirectoryRecursive(pProject->GetWorkingDir()))
	{
		AfxMessageBox(STE_PROJECTDIR_CREATE_ERR, MB_ICONSTOP|MB_OK);
		return;
	}
	pProject->OnSaveProject(pProject->GetPathName());

	// set carriage return
	CString				strCrlf;
	switch (m_nFileFormat)
	{
		case CRLF_STYLE_DOS:
			strCrlf = _T("\r\n");
			break;
		case CRLF_STYLE_UNIX:
			strCrlf = _T("\n");
			break;
		case CRLF_STYLE_MAC:
			strCrlf = _T("\n\r");
			break;

		default:
			ASSERT(FALSE);
			strCrlf = _T("\r\n");
	}

	// initialize project
	if (pItem)
	{
		BOOL	bResult = pItem->InitProject(theApp.GetProject(), strCrlf);
		if (bResult)
		{
			// open new main file
			theApp.OpenLatexDocument(theApp.GetProject()->GetMainPath());
			CTemplateDialog::EndDialog(IDOK);
		}
		else
			CTemplateDialog::EndDialog(IDCANCEL);
	}
}
