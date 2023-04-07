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

/********************************************************************
 *
 * $Id$
 *
 ********************************************************************/

#include "stdafx.h"
#include "resource.h"
#include "EndOfLineMode.h"
#include "DocumentNewDialog.h"
#include "global.h"
#include "LaTeXDocument.h"
#include "LaTeXView.h"
#include "Configuration.h"
#include "TeXnicCenter.h"
#include "textsourcefile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//-------------------------------------------------------------------
// CEmptyDocumentTemplateItem
//-------------------------------------------------------------------

IMPLEMENT_DYNCREATE(CEmptyDocumentTemplateItem,CObject);

CEmptyDocumentTemplateItem::CEmptyDocumentTemplateItem()
{
}

BOOL CEmptyDocumentTemplateItem::InitItem(LPCTSTR /*lpszPath*/,CImageList &ImageList32,CImageList &ImageList16)
{
	//add image to image list and remember index
	HICON hIcon = theApp.LoadIcon(IDR_LATEXDOCTYPE);
	m_nImageIndex = hIcon ? ImageList32.Add(hIcon) : -1;
	ImageList16.Add(hIcon);

	return TRUE;
}

const CString CEmptyDocumentTemplateItem::GetTitle() const
{
	return CString((LPCTSTR) STE_EMPTYDOCUMENT_TITLE);
}

const CString CEmptyDocumentTemplateItem::GetDescription() const
{
	return CString((LPCTSTR) STE_EMPTYDOCUMENT_DESCRIPTION);
}

int CEmptyDocumentTemplateItem::GetImageIndex() const
{
	return m_nImageIndex;
}

BOOL CEmptyDocumentTemplateItem::InitDocument(LPCTSTR lpszPath,LPCTSTR lpszCr)
{
	CString strTargetPath(lpszPath);

	if (!CreateFile(strTargetPath,lpszCr))
	{
		AfxMessageBox(STE_PROJECT_CANNOTCREATEMAINFILE,MB_ICONSTOP | MB_OK);
		return FALSE;
	}

	return TRUE;
}

BOOL CEmptyDocumentTemplateItem::CreateFile(LPCTSTR lpszTargetPath,LPCTSTR lpszCrLf)
{
	// create empty main file
	TextDocument doc;
	return doc.Write(lpszTargetPath,lpszCrLf);
}


//-------------------------------------------------------------------
// CFileBasedDocumentTemplateItem
//-------------------------------------------------------------------

IMPLEMENT_DYNCREATE(CFileBasedDocumentTemplateItem,CObject);

CFileBasedDocumentTemplateItem::CFileBasedDocumentTemplateItem()
		: CDocumentTemplateItem(),
		m_nImageIndex(-1)
{
}

BOOL CFileBasedDocumentTemplateItem::InitItem(LPCTSTR lpszPath,CImageList &ImageList32,CImageList &ImageList16)
{
	m_strPath = lpszPath;

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//extract description, if available
	CTextSourceFile TextFile;
	if (TextFile.Create(lpszPath))
	{
		LPCTSTR lpLine;
		int nLength;
		if (TextFile.GetNextLine(lpLine, nLength))
		{
			CString strLine(lpLine, nLength);
			CString strKey(_T("%DESCRIPTION: "));
			CString strStartOfLine = strLine.Left(strKey.GetLength());
			strStartOfLine.MakeUpper();

			if (strStartOfLine == strKey)
				m_strDescription = strLine.Right(strLine.GetLength() - strKey.GetLength());
		}
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//generate title
	m_strTitle = CPathTool::GetFileTitle(m_strPath);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//add image to image list and remember index
	HICON hIcon = theApp.LoadIcon(IDR_LATEXDOCTYPE);
	m_nImageIndex = hIcon ? ImageList32.Add(hIcon) : -1;
	ImageList16.Add(hIcon);

	return TRUE;
}

const CString CFileBasedDocumentTemplateItem::GetTitle() const
{
	return m_strTitle;
}

const CString CFileBasedDocumentTemplateItem::GetDescription() const
{
	return m_strDescription;
}

int CFileBasedDocumentTemplateItem::GetImageIndex() const
{
	return m_nImageIndex;
}

BOOL CFileBasedDocumentTemplateItem::InitDocument(LPCTSTR lpszPath,LPCTSTR lpszCr)
{
	CString strTargetPath(lpszPath);

	if (!CreateFile(strTargetPath,lpszCr))
	{
		AfxMessageBox(STE_DOCUMENT_CANNOTCREATE,MB_ICONSTOP | MB_OK);
		return FALSE;
	}

	return TRUE;
}

BOOL CFileBasedDocumentTemplateItem::CreateFile(LPCTSTR lpszTargetPath,LPCTSTR lpszCrLf)
{
	CStringW text;
	TextDocument doc;

	bool result = doc.Read(m_strPath,text);

	if (result)
	{
		const CStringW lineendings(L"\r\n");
		int index = text.FindOneOf(lineendings);

		if (index != -1)
		{
			CStringW line = text.Left(index);
			const CStringW strKey(L"%DESCRIPTION: ");
			CStringW strStartOfLine = line.Left(strKey.GetLength());

			strStartOfLine.MakeUpper();

			if (strKey == strStartOfLine) 
			{
				text.Delete(0,index);
				text.TrimLeft(lineendings + L' ');
			}
		}

		LPCWSTR le = GetLineEnding(static_cast<LPCWSTR>(text),text.GetLength());

		USES_CONVERSION;

		if (std::wcscmp(le,T2CW(lpszCrLf)) != 0) // Line endings not equal
			text.Replace(le,lpszCrLf);

		result = doc.Write(lpszTargetPath,text);
	}

	return result;
}


//-------------------------------------------------------------------
// CWizardBasedDocumentTemplateItem
//-------------------------------------------------------------------

IMPLEMENT_DYNCREATE(CWizardBasedDocumentTemplateItem,CObject);

CWizardBasedDocumentTemplateItem::CWizardBasedDocumentTemplateItem()
		: CDocumentTemplateItem(),
		m_nImageIndex(-1)
{
}

BOOL CWizardBasedDocumentTemplateItem::InitItem(LPCTSTR lpszPath,CImageList &ImageList32,CImageList &ImageList16)
{
	m_strPath = lpszPath;

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// retrieve the wizard interface
	CString strProgId;
	strProgId.Format(_T("%s.DocumentWizard"), (LPCTSTR)CPathTool::GetFileTitle(m_strPath));

	BSTR bstrProgId = strProgId.AllocSysString();
	HRESULT result = m_wizard.CoCreateInstance(bstrProgId,NULL,CLSCTX_INPROC_SERVER);

	if (result != S_OK)
		return FALSE;

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// get the icon
	HICON hIcon = NULL;

	int nIconIndex = m_wizard.GetIconIndex();
	if (nIconIndex >= 0)
		hIcon = ExtractIcon(AfxGetInstanceHandle(),m_strPath,nIconIndex);
	if (!hIcon)
		hIcon = theApp.LoadIcon(IDR_LATEXWIZARD);

	m_nImageIndex = ImageList32.Add(hIcon);
	ImageList16.Add(hIcon);

	return TRUE;
}

const CString CWizardBasedDocumentTemplateItem::GetTitle() const
{
	return m_wizard.GetTitle();
}

const CString CWizardBasedDocumentTemplateItem::GetDescription() const
{
	return m_wizard.GetDescription();
}

int CWizardBasedDocumentTemplateItem::GetImageIndex() const
{
	return m_nImageIndex;
}

BOOL CWizardBasedDocumentTemplateItem::InitDocument(LPCTSTR lpszPath,LPCTSTR lpszCr)
{
	BOOL bResult = m_wizard.DoModal(lpszPath,lpszCr);
	return bResult;
}


//-------------------------------------------------------------------
// CDocumentNewDialog
//-------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CDocumentNewDialog,CTemplateDialog)
	//{{AFX_MSG_MAP(CDocumentNewDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CDocumentNewDialog::CDocumentNewDialog(CWnd* pParent /*=NULL*/)
		: CTemplateDialog(CDocumentNewDialog::IDD,pParent)
{
	//{{AFX_DATA_INIT(CDocumentNewDialog)
	//}}AFX_DATA_INIT

	//Add template for empty project
	std::unique_ptr<CTemplateItem> pItem(new CEmptyDocumentTemplateItem);
	AddTemplateItem(CString((LPCTSTR) STE_EMPTYDOCUMENT_CATEGORY), 
		std::move(pItem));

	m_nFileFormat = CConfiguration::GetInstance()->m_nStandardFileFormat;

	//Set the info about the first tab to be activated
	m_nFirstTab = CConfiguration::GetInstance()->m_nLastTabDocumentTemplateDlg;
}

CDocumentNewDialog::~CDocumentNewDialog()
{
	//First tab to be activated, when dialog is run next time
	CConfiguration::GetInstance()->m_nLastTabDocumentTemplateDlg = m_nLastTab;
}

void CDocumentNewDialog::DoDataExchange(CDataExchange* pDX)
{
	CTemplateDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDocumentNewDialog)
	DDX_CBIndex(pDX,IDC_FILE_FORMAT,m_nFileFormat);
	//}}AFX_DATA_MAP
}

void CDocumentNewDialog::Create()
{
	UpdateData();

	// Generate temporary file name
	CString strTempPath;
	if (GetTempPath(MAX_PATH,strTempPath.GetBuffer(MAX_PATH)))
		strTempPath.ReleaseBuffer();
	else
	{
		strTempPath.ReleaseBuffer(0);
		strTempPath = CPathTool::GetDirectory(theApp.m_pszHelpFilePath) + _T('\\');
	}

	CString strTempFilePath;
	if (GetTempFileName(strTempPath,_T("TXC"),0,strTempFilePath.GetBuffer(MAX_PATH)))
		strTempFilePath.ReleaseBuffer();
	else
	{
		strTempFilePath.ReleaseBuffer();
		strTempFilePath = CPathTool::Cat(strTempPath,_T("TXC00001.TMP"));
	}

	// retrieve selected element
	CDocumentTemplateItem *pItem = NULL;
	int nItem = GetSelectedItem();
	if (nItem >= 0 && nItem < m_wndTemplateList.GetItemCount())
		pItem = reinterpret_cast<CDocumentTemplateItem*>(m_wndTemplateList.GetItemData(nItem));

	// set carriage return
	CString strCrlf;
	switch (m_nFileFormat)
	{
		case DOSStyleEOLMode:
			strCrlf = _T("\r\n");
			break;
		case UnixStyleEOLMode:
			strCrlf = _T("\r");
			break;
		case MacStyleEOLMode:
			strCrlf = _T("\n");
			break;

		default:
			ASSERT(FALSE);
			strCrlf = _T("\r\n");
	}

	// initialize document
	if (pItem)
	{
		BOOL bResult = pItem->InitDocument(strTempFilePath,strCrlf);
		if (bResult)
		{
			try
			{
				CMultiDocTemplate *pTemplate = theApp.GetLatexDocTemplate();
				if (!pTemplate)
					throw FALSE;

				LaTeXDocument *pDoc = dynamic_cast<LaTeXDocument*>(pTemplate->OpenDocumentFile(NULL));

				if (!pDoc)
					throw FALSE;

				pDoc->LoadFile(strTempFilePath);
				CTemplateDialog::EndDialog(IDOK);

				if (CConfiguration::GetInstance()->m_bSaveNewDocuments)
					pDoc->DoFileSave();
			}
			catch (BOOL)
			{
				ASSERT(FALSE);
				CTemplateDialog::EndDialog(IDCANCEL);
			}
		}
		else
			CTemplateDialog::EndDialog(IDCANCEL);
	}

	// delete temporary file
	DeleteFile(strTempFilePath);
}
