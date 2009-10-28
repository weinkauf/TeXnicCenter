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
#include "TeXnicCenter.h"
#include "FileViewEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DD_TIMER_ID               1000 //TODO
#define SCROLL_BORDER             10
#define SCROLL_SPEED_ZONE_WIDTH   20

/////////////////////////////////////////////////////////////////////////////
// CCFileViewEx

CCFileViewEx::CCFileViewEx()
		: m_pDragImage(NULL),
		m_htiDrag(NULL),
		m_htiDrop(NULL),
		m_htiOldDrop(NULL),
		m_bLDragging(FALSE),
		m_idTimer(0),
		m_bCopyDrag(FALSE),
		m_bKeepIndentLevel(TRUE)
{
	m_hDropCursor = ::LoadCursor(AfxGetInstanceHandle(),
	                             MAKEINTRESOURCE(IDC_SINGLEDRAG));
	m_hcurSCopyDrop = ::LoadCursor(AfxGetInstanceHandle(),
	                               MAKEINTRESOURCE(IDC_SDRAGCOPY));
	m_hNoDropCursor = ::LoadCursor(NULL,IDC_NO);
	m_nScrollTimerID = 0;
	m_nTimerTicks = 0;
	m_bModified = FALSE;
	m_bDoBranchCopy = TRUE; //For TeXnicCenter specifics
}

CCFileViewEx::~CCFileViewEx()
{
}

BEGIN_MESSAGE_MAP(CCFileViewEx,CCTreeCtrlEx)
	//{{AFX_MSG_MAP(CCFileViewEx)
	ON_NOTIFY_REFLECT(NM_RCLICK,OnNMRightClick)
	ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT,OnBeginLabelEdit)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG,OnBeginDrag)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT,OnEndLabelEdit)
	ON_NOTIFY_REFLECT(TVN_KEYDOWN,OnNMKeyDown)
	ON_NOTIFY_REFLECT(NM_DBLCLK,OnNMLDoubleClick)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED,OnItemExpanded)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED,OnSelectionChanged)
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_DESTROY()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_FILEVIEW_CLEAN,OnProjectClean)
	ON_UPDATE_COMMAND_UI(ID_FILEVIEW_CLEAN,OnUpdateProjectClean)
	ON_COMMAND(ID_FILEVIEW_ADD_FILES,OnProjectAddFiles)
	ON_UPDATE_COMMAND_UI(ID_FILEVIEW_ADD_FILES,OnUpdateProjectAddFiles)
	ON_COMMAND(ID_FILEVIEW_BUILD,OnProjectBuild)
	ON_UPDATE_COMMAND_UI(ID_FILEVIEW_BUILD,OnUpdateProjectBuild)
	ON_COMMAND(ID_FILEVIEW_COMPILE,OnFileCompile)
	ON_UPDATE_COMMAND_UI(ID_FILEVIEW_COMPILE,OnUpdateFileCompile)
	ON_COMMAND(ID_FILEVIEW_DELETE,OnFileDelete)
	ON_UPDATE_COMMAND_UI(ID_FILEVIEW_DELETE,OnUpdateFileDelete)
	ON_COMMAND(ID_FILEVIEW_INSERT_FILES,OnFolderInsertFiles)
	ON_UPDATE_COMMAND_UI(ID_FILEVIEW_INSERT_FILES,OnUpdateFolderInsertFiles)
	ON_COMMAND(ID_FILEVIEW_PROPERTIES,OnFileViewProperties)
	ON_UPDATE_COMMAND_UI(ID_FILEVIEW_PROPERTIES,OnUpdateFileViewProperties)
	ON_COMMAND(ID_FILEVIEW_REBUILD_ALL,OnProjectRebuildAll)
	ON_UPDATE_COMMAND_UI(ID_FILEVIEW_REBUILD_ALL,OnUpdateProjectRebuildAll)
	ON_COMMAND(ID_FILEVIEW_RENAME,OnFileRename)
	ON_UPDATE_COMMAND_UI(ID_FILEVIEW_RENAME,OnUpdateFileRename)
	ON_COMMAND(ID_FILEVIEW_SET_MAIN,OnFileSetAsMain)
	ON_UPDATE_COMMAND_UI(ID_FILEVIEW_SET_MAIN,OnUpdateFileSetAsMain)
	ON_COMMAND(ID_FILEVIEW_OPEN,OnFileOpen)
	ON_UPDATE_COMMAND_UI(ID_FILEVIEW_OPEN,OnUpdateFileOpen)
	ON_COMMAND(ID_FILEVIEW_UPDATE_OUTPUT,OnUpdateOutputFiles)
	ON_UPDATE_COMMAND_UI(ID_FILEVIEW_UPDATE_OUTPUT,OnUpdateUpdateOutputFiles)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCFileViewEx message handlers

void CCFileViewEx::OnNMRightClick(NMHDR* /*pNMHDR*/,LRESULT* pResult)
{
	BOOL bOnTreeItem = FALSE;
	UINT nFlags;
	CPoint curPoint;
	GetCursorPos(&curPoint);
	ScreenToClient(&curPoint);
	CTreeCursor CurSel = HitTest(curPoint,&nFlags);

	// Restrict the hit tesing to right clicking on the tree on,
	// necessarily on the items...
	if ((CurSel != NULL)/* &&(TVHT_ONITEM & nFlags)*/)
	{
		CurSel.Select(TVGN_CARET);
		m_ItemSel = CurSel;
		bOnTreeItem = TRUE;
	}

	if (bOnTreeItem)
	{
		UINT nImageID = m_ItemSel.GetImageID();
		switch (nImageID)
		{
			case IID_PROJECT:
				DoPopupMenu(IDR_FILEVIEW_PROJECT);
				break;
			case IID_CLOSEFOLDER:
			case IID_OPENFOLDER:
				DoPopupMenu(IDR_FILEVIEW_FOLDER);
				break;
			default:
				DoPopupMenu(IDR_FILEVIEW_FILE);
				break;
		}
	}
	else
	{
		GetCursorPos(&curPoint);
		//theApp.m_pMainWnd->SendMessage(BCGM_TOOLBARMENU, 0,
		//	curPoint.y << 16 | curPoint.x);
	}

	*pResult = 0;
}

void CCFileViewEx::DoPopupMenu(UINT nMenuID,const CPoint* ppt/* = NULL*/)
{
	CPoint ptCur = CPoint(0,0);
	if (ppt == NULL)
	{
		GetCursorPos(&ptCur); //TODO--Make sure the point is within the tree rect
	}
	else
		ptCur = *ppt;

	//     CWnd* pWndPopupOwner = this;
	//
	//     while (pWndPopupOwner->GetStyle() & WS_CHILD)
	//		 pWndPopupOwner = pWndPopupOwner->GetParent();

	theApp.ShowPopupMenu(nMenuID,ptCur,/*pWndPopupOwner*/AfxGetMainWnd());
}

void CCFileViewEx::DoPopupMenu(CTreeCursor xItem,const CPoint* ppt/* = NULL*/)
{
	int nImageID = xItem.GetImageID();
	switch (nImageID)
	{
		case IID_PROJECT:
			DoPopupMenu(IDR_FILEVIEW_PROJECT,ppt);
			break;
		case IID_CLOSEFOLDER:
		case IID_OPENFOLDER:
			DoPopupMenu(IDR_FILEVIEW_FOLDER,ppt);
			break;
		default:
			DoPopupMenu(IDR_FILEVIEW_FILE,ppt);
			break;
	}
}

void CCFileViewEx::OnBeginLabelEdit(NMHDR* pNMHDR,LRESULT* pResult)
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*) pNMHDR;

	// Setting pResult to 1 (*pResult = 1) will prevent the editing of the item
	// Setting it to 0 (*pResult = 0) will enable the editing of the item
	ASSERT(m_ItemSel == pTVDispInfo->item.hItem); // Need be true...
	if (!CanEditItem(pTVDispInfo->item.hItem))
	{
		*pResult = 1;
		return;
	}

	*pResult = 0;
}

void CCFileViewEx::OnBeginDrag(NMHDR* pNMHDR,LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*) pNMHDR;

	*pResult = 0;

	m_htiDrag = pNMTreeView->itemNew.hItem;
	m_htiDrop = NULL;
	SelectItem(m_htiDrag);

	// Determine if the item can be dragged, if not return...
	if (!IsDragSource(m_htiDrag))
		return;

	// Get the image list for dragging
	m_pDragImage = CreateDragImageEx(m_htiDrag); // TODO
	// CreateDragImage() returns a NULL if no image list associated with
	// the tree control
	ASSERT(m_pDragImage);
	if (!m_pDragImage)
		return;

	m_bLDragging = TRUE;

	// If the control key is held down, this is a copy operation
	// See: The Windows Interface Guidelines to Software Design, p.87
	m_bCopyDrag = ::GetKeyState(VK_CONTROL) < 0;

	CPoint pt(0,0);

	IMAGEINFO ii;
	m_pDragImage->GetImageInfo(0,&ii);
	pt.x = (ii.rcImage.right - ii.rcImage.left) / 2;
	pt.y = (ii.rcImage.bottom - ii.rcImage.top) / 2;

	m_pDragImage->BeginDrag(0,pt);
	pt = pNMTreeView->ptDrag;
	ClientToScreen(&pt);
	m_pDragImage->DragEnter(NULL,pt);

	SetCapture();

	// Set up the auto-scrolling timer
	m_nScrollTimerID = SetTimer(1,75,NULL);
}

void CCFileViewEx::OnEndLabelEdit(NMHDR* pNMHDR,LRESULT* pResult)
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*) pNMHDR;

	// We need to actually set the value.  Since only the text is being
	// edited, we set the mask to TVIF_TEXT and set the item.  If
	// the pszText field is NULL, the edit was cancelled

	if (pTVDispInfo->item.pszText != NULL)
	{
		CString strOldKey = m_ItemSel.GetText();
		CString strNewKey = CString(pTVDispInfo->item.pszText);
		ASSERT(strOldKey != strNewKey);
		ProjectFileItem* pFileItem = NULL;
		// If it is not in our file list, there must be something wrong, leave!
		if (!m_xFileList.Lookup(strOldKey,pFileItem))
		{
			ASSERT(FALSE); // The item is not in the databank, why?
			*pResult = 0;
			return;
		}

		ASSERT(pFileItem); // No reason why this should be null, how come?

		// Set the new item text, and update the item data in the file list
		pTVDispInfo->item.mask = TVIF_TEXT;
		if (SetItem(&pTVDispInfo->item))
		{
			CPathTool xFile(pFileItem->strFileName);
			CString strNewFileName = xFile.GetDirectory() + _T("\\") + strNewKey;
			// 1. If the new file does not exist in the current directory,
			//    then rename the old file to the new file.
			if (!CPathTool::Exists(strNewFileName))
			{
				::MoveFile(pFileItem->strFileName,strNewFileName);
			}
			// 2. Remove the original item from the map, since the key is changed
			VERIFY(m_xFileList.RemoveKey(strOldKey));
			// 3. Update the display name item, which is changed
			pFileItem->strDispName = strNewKey;
			// 4. Set the new file name
			pFileItem->strFileName = strNewFileName;
			// 5. Set a new item, based on the new key
			m_xFileList.SetAt(strNewKey,pFileItem);
			// 6. Finally, set the main file information in the project
			theApp.GetProject()->SetMainPath(strNewFileName);
		}
	}

	m_bModified = TRUE;
	*pResult = 0;
}

void CCFileViewEx::OnNMKeyDown(NMHDR* pNMHDR,LRESULT* pResult)
{
	TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*) pNMHDR;

	// Allow keyboard deletion of some items
	if (pTVKeyDown->wVKey == VK_DELETE)
	{
		BOOL bShowWarnings = TRUE; //((??? *)AfxGetApp())->m_bShowWarnings; //TODO--user option
		int retCode = IDYES;
		BOOL bCanDelete = TRUE;
		//int nImageID = m_ItemSel.GetImageID();

		bCanDelete = CanDeleteItem(m_ItemSel);

		if (bCanDelete && bShowWarnings)
		{
			// TODO: --work out a warning box with do not show this again option button
			CString strWarning = _T("Are you sure you want to delete this item?"); // TODO
			retCode = AfxMessageBox(strWarning + _T("\n\n\t") + m_ItemSel.GetText(),
			                        MB_YESNO | MB_ICONWARNING);
		}
		if (bCanDelete && retCode == IDYES)
			DeleteItem(m_ItemSel);
	}

	// Process the (Shift + F10 == 121) key combination, an equivalent of right-click
	if ((::GetKeyState(VK_SHIFT) < 0) && pTVKeyDown->wVKey == 121)
	{
		HTREEITEM hItem = GetSelectedItem();
		CRect rect;
		GetItemRect(hItem,&rect,TRUE);
		ClientToScreen(rect);
		CPoint pt = CPoint(rect.right,rect.top);
		DoPopupMenu(m_ItemSel,&pt);
	}

	if (m_bLDragging)
	{
		if (pTVKeyDown->wVKey == VK_CONTROL)
			m_bCopyDrag = TRUE;
		else
			m_bCopyDrag = FALSE;

		if (m_bCopyDrag &&
		        (GetCursor() != m_hNoDropCursor && GetCursor() != m_hcurSCopyDrop))
			SetCursor(m_hcurSCopyDrop);
		else if (!m_bCopyDrag &&
		         (GetCursor() != m_hNoDropCursor && GetCursor() != m_hDropCursor))
			SetCursor(m_hDropCursor);
	}

	*pResult = 0;
}

BOOL CCFileViewEx::PreTranslateMessage(MSG* pMsg)
{
	// If special keys
	if (GetEditControl() && (pMsg->message == WM_KEYDOWN) &&
	        (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) &&
	        !m_bLDragging)
	{
		static const TCHAR szEditClass[] = _T("EDIT");
		TCHAR szFocusName[sizeof(szEditClass) / sizeof(szEditClass[0]) + 1];
		HWND hFocus = ::GetFocus();

		::GetClassName(hFocus,szFocusName,
		               sizeof(szFocusName) / sizeof(szFocusName[0]));
		// If an edit control...
		if (lstrcmpi(szFocusName,szEditClass) == 0)
		{
			if (::GetWindowLong(hFocus,GWL_STYLE) & ES_WANTRETURN)
			{
				::TranslateMessage(pMsg);
				::DispatchMessage(pMsg);
				return TRUE;
			}
		}

		if (::SendMessage(hFocus,WM_GETDLGCODE,0,0) &
		        (DLGC_WANTALLKEYS | DLGC_WANTCHARS | DLGC_WANTMESSAGE))
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;
		}
	}
	else if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_F2))
	{
		// With the keyboard, enable editing with the F2 function key.
		CTreeCursor hItem = GetSelectedItem();
		if (hItem)
			EditLabel(hItem);
	}
	else if ((pMsg->message == WM_KEYDOWN) &&
	         (pMsg->wParam == VK_ESCAPE) && m_bLDragging)
	{
		m_bLDragging = 0;
		CImageList::DragLeave(NULL);
		CImageList::EndDrag();
		ReleaseCapture();
		SelectDropTarget(NULL);
		if (m_pDragImage)
		{
			delete m_pDragImage;
			m_pDragImage = NULL;
		}
		if (m_idTimer)
		{
			KillTimer(m_idTimer);
			m_idTimer = 0;
		}
		if (m_nScrollTimerID)
		{
			KillTimer(m_nScrollTimerID);
			m_nScrollTimerID = 0;
		}
		return TRUE; // DO NOT process further
	}

	return CCTreeCtrlEx::PreTranslateMessage(pMsg);
}

void CCFileViewEx::OnNMLDoubleClick(NMHDR* /*pNMHDR*/,LRESULT* pResult)
{
	CTreeCursor hItem = GetSelectedItem();
	int nImageID = hItem.GetImageID();
	// The ff. delete actions are not allowed
	//    a. The deletion of the folders
	//    b. The deletion of the input or BiBTeX files
	if ((nImageID == IID_PROJECT) || (nImageID == IID_CLOSEFOLDER) ||
	        (nImageID == IID_OPENFOLDER))
	{
		*pResult = 0;
		return;
	}

	ProjectFileItem* pFileItem = NULL;
	if (m_xFileList.Lookup(hItem.GetText(),pFileItem))
	{
		//TODO--Test that the file exist
		AfxGetApp()->OpenDocumentFile(pFileItem->strFileName);
	}

	*pResult = 0;
}

void CCFileViewEx::PopulateTree()
{
	DeleteAllItems();
	UpdateWindow();

	// Insert root node by call to tree object this time
	m_fProject = GetRootItem().AddTail(_T("Project: Test"),IID_PROJECT);

	// From now on call via iterators
	m_fMainFolder = m_fProject.AddTail(_T("Main Files"),IID_CLOSEFOLDER);

	m_fInputFolder = m_fProject.AddTail(_T("Input Files"),IID_CLOSEFOLDER);
	m_fBiBTeXFolder = m_fProject.AddTail(_T("BiBTeX Files"),IID_CLOSEFOLDER);
	m_fOutputFolder = m_fProject.AddTail(_T("Output Files"),IID_CLOSEFOLDER);
	m_fProjectFilesFolder = m_fProject.AddTail(_T("Project Files"),IID_CLOSEFOLDER);

	/*	int i;
	        CString strLabel;
	        for (i = 0; i < 6; i++)
	        {
	                strLabel.Format(_T("Main File %d"), i + 1);
	                if (i == 2)
	                        AddItem(FID_MAINFILES, IID_MAINFILE, strLabel + _T(".tex"));
	                else
	                        AddItem(FID_MAINFILES, IID_TEXFILE, strLabel + _T(".tex"));
	        }

	        m_fInputFolder = m_fProject.AddTail(_T("Input Files"),   IID_CLOSEFOLDER);
	        for (i = 0; i < 4; i++)
	        {
	                strLabel.Format(_T("Input File %d"), i + 1);
	                AddItem(FID_INPUTFILES, IID_INPUTFILE, strLabel + _T(".tex"));
	        }
	 */
	m_fProject.Expand();
}

void CCFileViewEx::PopulateTree(CString& strFileName)
{
	DeleteAllItems();
	UpdateWindow();

	CPathTool xFile(strFileName);
	CString strProjectName = _T("Project: ") + xFile.GetFileTitle();
	// Insert root node by call to tree object this time
	m_fProject = GetRootItem().AddTail(strProjectName,IID_PROJECT);

	// From now on call via iterators
	m_fMainFolder = m_fProject.AddTail(_T("Main Files"),IID_CLOSEFOLDER);

	m_fInputFolder = m_fProject.AddTail(_T("Input Files"),IID_CLOSEFOLDER);
	m_fBiBTeXFolder = m_fProject.AddTail(_T("BiBTeX Files"),IID_CLOSEFOLDER);
	m_fOutputFolder = m_fProject.AddTail(_T("Output Files"),IID_CLOSEFOLDER);
	m_fProjectFilesFolder = m_fProject.AddTail(_T("Project Files"),IID_CLOSEFOLDER);

	AddItem(FID_MAINFILES,IID_MAINFILE,strFileName);
	//TODO--launch a thread to parse and add the input files

	/*	int i;
	        CString strLabel;
	        for (i = 0; i < 6; i++)
	        {
	                strLabel.Format(_T("Main File %d"), i + 1);
	                if (i == 2)
	                        AddItem(FID_MAINFILES, IID_MAINFILE, strLabel + _T(".tex"));
	                else
	                        AddItem(FID_MAINFILES, IID_TEXFILE, strLabel + _T(".tex"));
	        }

	        m_fInputFolder = m_fProject.AddTail(_T("Input Files"),   IID_CLOSEFOLDER);
	        for (i = 0; i < 4; i++)
	        {
	                strLabel.Format(_T("Input File %d"), i + 1);
	                AddItem(FID_INPUTFILES, IID_INPUTFILE, strLabel + _T(".tex"));
	        }
	 */
	m_fProject.Expand();
}

HTREEITEM CCFileViewEx::AddItem(WORD nFolderType,WORD nItemType,LPCTSTR lpszName)
{
	ASSERT(nFolderType != FID_PROJECT); // Adding files to the project folder is
	// not supported

	CPathTool xFile(lpszName);
	CString strFileName = xFile.GetFile();
	CTreeCursor hItemNew;
	ProjectFileItem* pItemData = new ProjectFileItem();
	ASSERT(pItemData);
	switch (nFolderType)
	{
		case FID_MAINFILES:
		{
			ASSERT(nItemType == IID_TEXFILE || nItemType == IID_MAINFILE);
			if (nItemType == IID_TEXFILE)
			{
				hItemNew = m_fMainFolder.AddTail(strFileName,IID_TEXFILE);

				pItemData->strFolderName = m_fMainFolder.GetText();
				pItemData->bIsMainFile = FALSE;
				pItemData->bIsCompilable = TRUE;
			}
			else if (nItemType == IID_MAINFILE)
			{
				// If the current handle to the main file item is valid...
				// 1. Remove the bold text format
				// 2. Reset the image/icon to the normal tex file
				if (m_ItemMainFile.IsValid())
				{
					if (m_ItemMainFile.GetState(TVIS_BOLD) == TVIS_BOLD)
						m_ItemMainFile.SetState(0,TVIS_BOLD);
					m_ItemMainFile.SetImage(IID_TEXFILE,IID_TEXFILE);

					ProjectFileItem* pMainItem =
					    reinterpret_cast<ProjectFileItem*>(GetItemData(m_ItemMainFile));
					ASSERT(pMainItem);
					pMainItem->bIsMainFile = FALSE;
					pMainItem->nFileType = IID_TEXFILE;
				}

				hItemNew = m_ItemMainFile = m_fMainFolder.AddTail(strFileName,IID_MAINFILE);
				m_ItemMainFile.SetState(TVIS_BOLD,TVIS_BOLD);

				pItemData->strFolderName = m_fMainFolder.GetText();
				pItemData->bIsMainFile = TRUE;
				pItemData->bIsCompilable = TRUE;
			}
			break;
		}
		case FID_INPUTFILES:
		{
			ASSERT(nItemType == IID_INPUTFILE);
			if (nItemType == IID_INPUTFILE)
			{
				hItemNew = m_fInputFolder.AddTail(strFileName,IID_INPUTFILE);

				pItemData->strFolderName = m_fMainFolder.GetText();
				pItemData->bIsMainFile = FALSE;
				pItemData->bIsCompilable = TRUE;
			}
			break;
		}
		case FID_BIBTEXFILES:
		{
			ASSERT(nItemType == IID_BIBTEXFILE);
			if (nItemType == IID_BIBTEXFILE)
			{
				hItemNew = m_fBiBTeXFolder.AddTail(strFileName,IID_BIBTEXFILE);

				pItemData->strFolderName = m_fMainFolder.GetText();
				pItemData->bIsMainFile = FALSE;
				pItemData->bIsCompilable = TRUE;
			}
			break;
		}
		case FID_OUTPUTFILE:
		{
			ASSERT(nItemType == IID_OUTPUTFILE);
			if (nItemType == IID_OUTPUTFILE)
			{
				hItemNew = m_fOutputFolder.AddTail(strFileName,IID_OUTPUTFILE);

				pItemData->strFolderName = m_fMainFolder.GetText();
				pItemData->bIsMainFile = FALSE;
				pItemData->bIsCompilable = TRUE;
			}
			break;
		}
		case FID_PROJECTFILES:
		{
			ASSERT(nItemType != IID_MAINFILE); // Main file cannot be added to this folder
			if (nItemType != IID_MAINFILE)
			{
				hItemNew = m_fProjectFilesFolder.AddTail(strFileName,IID_FILE);

				pItemData->strFolderName = m_fProjectFilesFolder.GetText();
				pItemData->bIsMainFile = FALSE;
				pItemData->bIsCompilable = FALSE;
			}
			break;
		}
	}

	pItemData->strFileName = lpszName;
	pItemData->nFolderType = nFolderType;
	pItemData->strDispName = strFileName;
	pItemData->nFileType = nItemType;
	hItemNew.SetData(reinterpret_cast<DWORD>(pItemData));

	// Avoid duplication, if there an item with the same display name,
	// remove it first and delete the associated memory
	ProjectFileItem* pFileItem = NULL;
	if (m_xFileList.Lookup(strFileName,pFileItem))
	{
		VERIFY(m_xFileList.RemoveKey(lpszName));
		delete pFileItem;
		pFileItem = NULL;
	}
	m_xFileList.SetAt(strFileName,pItemData);

	m_bModified = TRUE;
	return hItemNew;
}

void CCFileViewEx::DeleteItem(CTreeCursor& itemDelete)
{
	BOOL bDeleted = FALSE;
	CString strKey = itemDelete.GetText();
	ProjectFileItem* pFileItem = NULL;

	// If it is not in our file list, there must be something wrong...just leave
	if (!m_xFileList.Lookup(strKey,pFileItem))
	{
		ASSERT(FALSE); // The item is not in the database, why?
		return;
	}

	switch (itemDelete.GetImageID())
	{
		case IID_TEXFILE:
		{
			bDeleted = itemDelete.Delete();
			break;
		}
		case IID_MAINFILE:
		{
			ASSERT(m_ItemMainFile == itemDelete);
			bDeleted = /*itemDelete*/m_ItemMainFile.Delete();
			break;
		}
		case IID_OUTPUTFILE:
		{
			bDeleted = itemDelete.Delete();
			break;
		}
		case IID_FILE:
		{
			bDeleted = itemDelete.Delete();
			break;
		}
	}

	// If the item was successfully deleted, then remove it from the list
	// and delete it from memory
	if (bDeleted)
	{
		VERIFY(m_xFileList.RemoveKey(strKey));
		delete pFileItem;
		pFileItem = NULL;
	}
	m_bModified = TRUE;
	SetFocus();
}

int CCFileViewEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CCTreeCtrlEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create the Image List
	m_ctlImage.Create(IDB_FILE_VIEW,16,0,RGB(255,0,255));
	m_ctlImage.SetBkColor(GetSysColor(COLOR_WINDOW));

	// Attach image list to Tree
	//	this->SetImageList(&m_ctlImage, TVSIL_NORMAL);
	this->SetImageList(&m_ctlImage);

	return 0;
}

void CCFileViewEx::OnItemExpanded(NMHDR* pNMHDR,LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*) pNMHDR;
	// TODO: Add your control notification handler code here
	TVITEM itemParent = pNMTreeView->itemNew;
	CTreeCursor hItem(itemParent.hItem,this);
	UINT nImageID = hItem.GetImageID();
	if (nImageID == IID_CLOSEFOLDER || nImageID == IID_OPENFOLDER)
	{
		if (itemParent.state & TVIS_EXPANDED)
			hItem.SetImage(IID_OPENFOLDER,IID_OPENFOLDER);
		else
			hItem.SetImage(IID_CLOSEFOLDER,IID_CLOSEFOLDER);
	}

	*pResult = 0;
}

void CCFileViewEx::OnSelectionChanged(NMHDR* /*pNMHDR*/,LRESULT* pResult)
{
	//NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*) pNMHDR;

	m_ItemSel = GetSelectedItem();

	UINT nImageID = m_ItemSel.GetImageID();
	switch (nImageID)
	{
		case IID_CLOSEFOLDER:
			break;
	}

	// In some case we wish to set the close/open folder but the event is
	// not due to expanding tree...eg. Deleting all items in a folder
	if (nImageID == IID_CLOSEFOLDER || nImageID == IID_OPENFOLDER)
	{
		// If the folder is not having any children...
		if (!m_ItemSel.HasChildren())
		{
			// set the folder to the close type
			m_ItemSel.SetImage(IID_CLOSEFOLDER,IID_CLOSEFOLDER);
		}
		/*		else
		                {
		                        //...if there are children...check the expanded/collaped
		                        // state of the of the current/selected folder and set the image
		                        if (m_ItemSel.GetState(TVIS_EXPANDED) == TVIS_EXPANDED)
		                                m_ItemSel.SetImage(IID_OPENFOLDER, IID_OPENFOLDER);
		                        else
		                                m_ItemSel.SetImage(IID_CLOSEFOLDER, IID_CLOSEFOLDER);
		                }
		 */
	}

	*pResult = 0;
}

void CCFileViewEx::OnRButtonDown(UINT nFlags,CPoint point)
{
	//	CRect rcItem;
	UINT Flags = 0;

	CTreeCursor hItem = HitTest(point,&Flags);
	if ((hItem != NULL) && (TVHT_ONITEM & nFlags))
	{
		hItem.Select(TVGN_CARET);
		m_ItemSel = hItem;
		//		GetItemRect(hItem, &rcItem, TRUE);
		//		if (rcItem.PtInRect(point))
		//			SelectItem(hItem);
		//		else
		//		   SelectItem(m_ItemSel);
	}

	CCTreeCtrlEx::OnRButtonDown(nFlags,point);
}

void CCFileViewEx::OnLButtonDown(UINT nFlags,CPoint point)
{
	UINT Flags = 0;
	CTreeCursor hItem = HitTest(point,&Flags);
	if ((hItem != NULL) && (TVHT_ONITEM & nFlags))
	{
		hItem.Select(TVGN_CARET);
		m_ItemSel = hItem;
		//		GetItemRect(hItem, &rcItem, TRUE);
		//		if (rcItem.PtInRect(point))
		//			SelectItem(hItem);
		//		else
		//		   SelectItem(m_ItemSel);
	}

	CCTreeCtrlEx::OnLButtonDown(nFlags,point);
}

void CCFileViewEx::OnLButtonDblClk(UINT nFlags,CPoint point)
{
	/*	CTreeCursor hItem = GetSelectedItem();
	        AfxMessageBox(
	                _T("CCFileViewEx::OnLButtonDblClk(UINT nFlags, CPoint point)\n\n\t")
	                + hItem.GetText());
	 */
	CCTreeCtrlEx::OnLButtonDblClk(nFlags,point);
}

void CCFileViewEx::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == m_idTimer)
	{
		HTREEITEM htiFloat = GetDropHilightItem();
		if (htiFloat && htiFloat == m_htiDrop)
		{
			if (ItemHasChildren(htiFloat))
			{
				Expand(htiFloat,TVE_EXPAND); //TODO-manualy set the open folders
			}
		}
	}

	if (nIDEvent == m_nScrollTimerID)
	{
		// Doesn't matter that we didn't initialize m_timerticks
		++m_nTimerTicks;

		POINT pt;
		GetCursorPos(&pt);
		CRect rect;
		GetClientRect(&rect);
		ClientToScreen(&rect);

		// NOTE: Screen coordinate is being used because the call
		// to DragEnter had used the Desktop window.
		CImageList::DragMove(pt);

		int iMaxV = GetScrollLimit(SB_VERT);
		int iPosV = GetScrollPos(SB_VERT);
		// The cursor must not only be SOMEWHERE above/beneath the tree control
		// BUT RIGHT above or beneath it
		// i.e. the x-coordinates must be those of the control (+/- SCROLL_BORDER)
		if (pt.x < rect.left - SCROLL_BORDER)
			; // Too much to the left
		else if (pt.x > rect.right + SCROLL_BORDER)
			; // Too much to the right
		else if ((pt.y < rect.top + SCROLL_BORDER) && iPosV)
		{
			HTREEITEM hitem = GetFirstVisibleItem();
			// We need to scroll up
			// Scroll slowly if cursor near the treeview control
			int slowscroll = 6 -
			                 (rect.top + SCROLL_BORDER - pt.y) / SCROLL_SPEED_ZONE_WIDTH;
			if (0 == (m_nTimerTicks % (slowscroll > 0 ? slowscroll : 1)))
			{
				CImageList::DragShowNolock(FALSE);
				SendMessage(WM_VSCROLL,SB_LINEUP);
				SelectDropTarget(hitem);
				m_htiDrop = hitem;
				CImageList::DragShowNolock(TRUE);
			}
		}
		else if ((pt.y > rect.bottom - SCROLL_BORDER) && (iPosV != iMaxV))
		{
			HTREEITEM hitem = GetFirstVisibleItem();
			while (hitem != NULL) //TODO
				hitem = GetNextVisibleItem(hitem);

			// We need to scroll down
			// Scroll slowly if cursor near the treeview control
			int slowscroll = 6 -
			                 (pt.y - rect.bottom + SCROLL_BORDER) / SCROLL_SPEED_ZONE_WIDTH;
			if (0 == (m_nTimerTicks % (slowscroll > 0 ? slowscroll : 1)))
			{
				CImageList::DragShowNolock(FALSE);
				SendMessage(WM_VSCROLL,SB_LINEDOWN);
				int nCount = GetVisibleCount();
				for (int i = 0; i < nCount - 1; ++i)
					hitem = GetNextVisibleItem(hitem);
				if (hitem)
					SelectDropTarget(hitem);
				m_htiDrop = hitem;
				CImageList::DragShowNolock(TRUE);
			}
		}

		// The cursor must be in a small zone IN the treecontrol at the left/right
		int iPosH = GetScrollPos(SB_HORZ);
		int iMaxH = GetScrollLimit(SB_HORZ);

		if (!rect.PtInRect(pt))
			return; // not in TreeCtrl
		else if ((pt.x < rect.left + SCROLL_BORDER) && (iPosH != 0))
		{
			// We need to scroll to the left
			CImageList::DragShowNolock(FALSE);
			SendMessage(WM_HSCROLL,SB_LINELEFT);
			CImageList::DragShowNolock(TRUE);
		}
		else if ((pt.x > rect.right - SCROLL_BORDER) && (iPosH != iMaxH))
		{
			// We need to scroll to the right
			CImageList::DragShowNolock(FALSE);
			SendMessage(WM_HSCROLL,SB_LINERIGHT);
			CImageList::DragShowNolock(TRUE);
		}
	}

	CCTreeCtrlEx::OnTimer(nIDEvent);
}

void CCFileViewEx::OnMouseMove(UINT nFlags,CPoint point)
{
	CCTreeCtrlEx::OnMouseMove(nFlags,point);

	HTREEITEM hti;
	UINT flags;

	if (m_bLDragging)
	{
		POINT pt = point;
		ClientToScreen(&pt);
		CImageList::DragMove(pt);

		hti = HitTest(point,&flags);
		if (hti != NULL)
		{
			CImageList::DragShowNolock(FALSE);

			if (m_htiOldDrop == NULL)
				m_htiOldDrop = GetDropHilightItem();

			SelectDropTarget(hti);

			m_htiDrop = hti;

			if (m_idTimer && (hti == m_htiOldDrop))
			{
				KillTimer(m_idTimer);
				m_idTimer = 0;
			}

			if (!m_idTimer)
				m_idTimer = SetTimer(DD_TIMER_ID,2000,NULL);

			CImageList::DragShowNolock(TRUE);
		}

		if (nFlags & MK_CONTROL)
			m_bCopyDrag = TRUE;
		else
			m_bCopyDrag = FALSE;

		if (hti)
			hti = GetDropTarget(hti);
		if (hti)
		{
			if (m_bCopyDrag)
				SetCursor(m_hcurSCopyDrop);
			else
				SetCursor(m_hDropCursor);
		}
		else
		{
			SetCursor(m_hNoDropCursor);
		}
	}
}

void CCFileViewEx::OnLButtonUp(UINT nFlags,CPoint point)
{
	CCTreeCtrlEx::OnLButtonUp(nFlags,point);

	if (m_bLDragging)
	{
		m_bLDragging = FALSE;
		CImageList::DragLeave(this);
		CImageList::EndDrag();

		ReleaseCapture();

		if (m_pDragImage)
		{
			delete m_pDragImage;
			m_pDragImage = NULL;
		}
		if (m_idTimer)
		{
			KillTimer(m_idTimer);
			m_idTimer = 0;
		}
		if (m_nScrollTimerID)
		{
			KillTimer(m_nScrollTimerID);
			m_nScrollTimerID = 0;
		}

		// Remove the drop target highlighting
		SelectDropTarget(NULL);
		m_htiOldDrop = NULL;

		if ((m_htiDrag == m_htiDrop) || (m_htiDrop == NULL))
			return;

		// If the drag item is an ancestor of the drop item then return
		HTREEITEM htiParent = m_htiDrop;
		while ((htiParent = GetParentItem(htiParent)) != NULL)
		{
			if (htiParent == m_htiDrag)
				return;
		}

		// check for same indentation level
		HTREEITEM htiPosition = TVI_LAST;

		if (m_bKeepIndentLevel)
		{
			int nDragIndent = GetIndentLevel(m_htiDrag);
			int nDropIndent = GetIndentLevel(m_htiDrop);

			if (nDragIndent == nDropIndent)
			{
				// add after the drop item, as child of drop item's parent
				htiPosition = m_htiDrop;
				m_htiDrag = GetParentItem(m_htiDrop);
			}
			else if (nDragIndent == (nDropIndent + 1))
			{
				// if dropped on 'upper' indentation, add as last child of drop item
				htiPosition = TVI_LAST;
			}
			else
			{
				// invalid drop target
				return;
			}
		}

		// If the current cursor indicates no drop, then leave here
		if (::GetCursor() == m_hNoDropCursor)
			return;

		m_htiDrop = GetDropTarget(m_htiDrop);
		// If there is no drop target, then leave here
		if (m_htiDrop == NULL)
			return;

		Expand(m_htiDrop,TVE_EXPAND);
		HTREEITEM htiNew = CopyBranch(m_htiDrag,m_htiDrop,htiPosition);

		if (!m_bCopyDrag)
		{
			CTreeCursor item(m_htiDrag,this);
			DeleteItem(item);
			m_bCopyDrag = FALSE;
		}
		SelectItem(htiNew);
	}
}

void CCFileViewEx::OnDestroy()
{
	if (m_pDragImage)
	{
		delete m_pDragImage;
		m_pDragImage = NULL;
	}
	if (m_idTimer)
	{
		KillTimer(m_idTimer);
		m_idTimer = 0;
	}
	if (m_nScrollTimerID)
	{
		KillTimer(m_nScrollTimerID);
		m_nScrollTimerID = 0;
	}

	POSITION pos = m_xFileList.GetStartPosition();
	while (pos != NULL)
	{
		CString strKey;
		ProjectFileItem* pFileItem;
		// Gets key (strKey) and value (pFileItem)
		m_xFileList.GetNextAssoc(pos,strKey,pFileItem);
		if (pFileItem != NULL)
		{
			delete pFileItem;
			pFileItem = NULL;
		}
	}
	m_xFileList.RemoveAll();

	CCTreeCtrlEx::OnDestroy();
}

HTREEITEM CCFileViewEx::CopyItem(HTREEITEM hti,HTREEITEM htiNewParent,
                                 HTREEITEM htiAfter /*= TVI_LAST*/)
{
	TVINSERTSTRUCT insert;
	::ZeroMemory(&insert,sizeof(TVINSERTSTRUCT));

	HTREEITEM htiNew = NULL;
	CString text;

	insert.item.hItem = hti;
	insert.item.mask = TVIF_CHILDREN | TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	GetItem(&(insert.item));
	text = GetItemText(hti);

	insert.item.cchTextMax = text.GetLength();
	insert.item.pszText = text.LockBuffer();

	insert.hParent = htiNewParent;
	insert.hInsertAfter = htiAfter;
	insert.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
	htiNew = InsertItem(&insert);
	text.UnlockBuffer();

	SetItemData(htiNew,GetItemData(hti));
	SetItemState(htiNew,
	             GetItemState(hti,TVIS_STATEIMAGEMASK),TVIS_STATEIMAGEMASK);

	return htiNew;
}

HTREEITEM CCFileViewEx::CopyBranch(HTREEITEM hti,HTREEITEM htiNewParent,
                                   HTREEITEM htiAfter /*= TVI_LAST*/)
{
	if (m_bDoBranchCopy)
		return DoCopyBranch(hti,htiNewParent,htiAfter);

	HTREEITEM htiChild = NULL;
	HTREEITEM htiNew = CopyItem(hti,htiNewParent,htiAfter);
	htiChild = GetChildItem(hti);
	while (htiChild != NULL)
	{
		CopyBranch(htiChild,htiNew);
		htiChild = GetNextSiblingItem(htiChild);
	}

	return htiNew;
}

HTREEITEM CCFileViewEx::DoCopyBranch(HTREEITEM hti,HTREEITEM htiNewParent,
                                     HTREEITEM /*htiAfter*/ /*= TVI_LAST*/)
{
	HTREEITEM htiNew = NULL;
	CTreeCursor xItem(hti,this);
	if (htiNewParent == m_fMainFolder)
		htiNew = AddItem(FID_MAINFILES,IID_TEXFILE,xItem.GetText());

	if (htiNewParent == m_fProjectFilesFolder)
		htiNew = AddItem(FID_PROJECTFILES,IID_FILE,xItem.GetText());

	return htiNew;
}

void CCFileViewEx::OnSysColorChange()
{
	CCTreeCtrlEx::OnSysColorChange();

	// Reset the background color of our image list when notified
	m_ctlImage.SetBkColor(GetSysColor(COLOR_WINDOW));
}

int CCFileViewEx::GetIndentLevel(HTREEITEM hItem)
{
	int nIndent = 0;
	while ((hItem = GetParentItem(hItem)) != NULL)
	{
		++nIndent;
	}

	return nIndent;
}

void CCFileViewEx::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// storing code
		HTREEITEM hti = GetRootItem();
		while (hti)
		{
			int indent = GetIndentLevel(hti);
			while (indent--)
				ar.WriteString(_T("\t"));

			ar.WriteString(GetItemText(hti) + _T("\r\n"));
			hti = GetNextItem(hti);
		}
	}
	else
	{
		// loading code
		CString sLine;
		if (!ar.ReadString(sLine))
			return;

		HTREEITEM hti = NULL;
		int indent,baseindent = 0;

		while (sLine[baseindent] == _T('\t'))
			++baseindent;

		do
		{
			if (sLine.GetLength() == 0)
				continue;

			for (indent = 0; sLine[indent] == _T('\t'); indent++)
				; // We don't need a body

			sLine = sLine.Right(sLine.GetLength() - indent);
			indent -= baseindent;

			HTREEITEM parent;
			int previndent = GetIndentLevel(hti);
			if (indent == previndent)
			{
				parent = GetParentItem(hti);
			}
			else if (indent > previndent)
			{
				parent = hti;
			}
			else
			{
				int nLevelsUp = previndent - indent;
				parent = GetParentItem(hti);
				while (nLevelsUp--)
					parent = GetParentItem(parent);
			}

			hti = InsertItem(sLine,parent ? parent : TVI_ROOT,TVI_LAST);
		}
		while (ar.ReadString(sLine));
	}
}

// GetNextItem	- Get next item as if outline was completely expanded
// Returns		- The item immediately below the reference item
// hItem		- The reference item

HTREEITEM CCFileViewEx::GetNextItem(HTREEITEM hItem)
{
	HTREEITEM hti;

	if (ItemHasChildren(hItem))
	{
		return GetChildItem(hItem); // return first child
	}
	else
	{
		// return next sibling item
		// Go up the tree to find a parent's sibling if needed.
		while ((hti = GetNextSiblingItem(hItem)) == NULL)
		{
			if ((hItem = GetParentItem(hItem)) == NULL)
				return NULL;
		}
	}
	return hti;
}

BOOL CCFileViewEx::IsDragSource(HTREEITEM hItem)
{
	BOOL bCanDrag = TRUE;
	CTreeCursor hItemCur(hItem,this);

	switch (hItemCur.GetImageID())
	{
		case IID_PROJECT:
		case IID_CLOSEFOLDER:
		case IID_OPENFOLDER:
		case IID_INPUTFILE:
		case IID_OUTPUTFILE:
		case IID_BIBTEXFILE:
		{
			bCanDrag = FALSE;
		}
		break;
	}

	return bCanDrag; // all items are valid sources
}

HTREEITEM CCFileViewEx::GetDropTarget(HTREEITEM hItem)
{
	// inhibit drop on the drop source or its parent
	// Also, disallow an item being a child of itself
	if ((hItem != m_htiDrag) && (hItem != GetParentItem(m_htiDrag)))
	{
		// Enforce Special TeXnicCenter specific drag/drop rules...
		// 1. Prevent dropping file items into the root/project folder
		if (hItem == m_fProject)
			return NULL;
		// 2. Dropping into the Input, BiBTeX, Output folders are not allowed
		if ((hItem == m_fInputFolder) || (hItem == m_fBiBTeXFolder) ||
		        (hItem == m_fOutputFolder))
			return NULL;
		// 3. Make suret the no drop cursor is indicated for file on file target
		if (GetIndentLevel(hItem) == GetIndentLevel(m_htiDrag))
			return NULL;

		HTREEITEM htiParent = hItem;
		while ((htiParent = GetParentItem(htiParent)) != NULL)
		{
			if (htiParent == m_htiDrag)
				return NULL;
		}
		return hItem;
	}
	return NULL;
}

CImageList* CCFileViewEx::CreateDragImageEx(HTREEITEM hItem)
{
	if (GetImageList(TVSIL_NORMAL) != NULL)
	{
		return CreateDragImage(hItem);
	}

	CRect rect;
	GetItemRect(hItem,rect,TRUE);
	rect.OffsetRect(-rect.left,-rect.top);

	// Create bitmap
	CClientDC dc(this);
	CDC memDC;

	if (!memDC.CreateCompatibleDC(&dc))
		return NULL;

	CBitmap bitmap;
	if (!bitmap.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height()))
		return NULL;

	CBitmap* pOldMemDCBitmap = memDC.SelectObject(&bitmap);
	CFont* pOldFont = memDC.SelectObject(GetFont());

	memDC.FillSolidRect(&rect,RGB(0,255,0)); // Here green is used as mask color
	memDC.SetTextColor(GetSysColor(COLOR_GRAYTEXT));
	memDC.TextOut(rect.left,rect.top,GetItemText(hItem));

	memDC.SelectObject(pOldFont);
	memDC.SelectObject(pOldMemDCBitmap);

	// Create imagelist
	CImageList* pImageList = new CImageList;
	pImageList->Create(rect.Width(),rect.Height(),
	                   ILC_COLOR | ILC_MASK,0,1);
	pImageList->Add(&bitmap,RGB(0,255,0)); // Here green is used as mask color

	return pImageList;
}

void CCFileViewEx::OnKeyUp(UINT nChar,UINT nRepCnt,UINT nFlags)
{
	if (m_bLDragging)
	{
		m_bCopyDrag = ::GetKeyState(VK_CONTROL) < 0;
		if (m_bCopyDrag &&
		        (GetCursor() != m_hNoDropCursor && GetCursor() != m_hcurSCopyDrop))
		{
			SetCursor(m_hcurSCopyDrop);
		}
		else if (!m_bCopyDrag &&
		         (GetCursor() != m_hNoDropCursor && GetCursor() != m_hDropCursor))
		{
			SetCursor(m_hDropCursor);
		}
	}

	CCTreeCtrlEx::OnKeyUp(nChar,nRepCnt,nFlags);
}

void CCFileViewEx::OnKeyDown(UINT nChar,UINT nRepCnt,UINT nFlags)
{
	if (m_bLDragging)
	{
		m_bCopyDrag = ::GetKeyState(VK_CONTROL) < 0;
		if (m_bCopyDrag &&
		        (GetCursor() != m_hNoDropCursor && GetCursor() != m_hcurSCopyDrop))
		{
			SetCursor(m_hcurSCopyDrop);
		}
		else if (!m_bCopyDrag &&
		         (GetCursor() != m_hNoDropCursor && GetCursor() != m_hDropCursor))
		{
			SetCursor(m_hDropCursor);
		}
	}

	CCTreeCtrlEx::OnKeyDown(nChar,nRepCnt,nFlags);
}

BOOL CCFileViewEx::CanDeleteItem(HTREEITEM hItem)
{
	CTreeCursor xItem(hItem,this);
	int nImageID = xItem.GetImageID();
	// The ff. delete actions are not allowed
	//    a. The deletion of the folders
	//    b. The deletion of the input or BiBTeX files
	if ((nImageID == IID_PROJECT) || (nImageID == IID_CLOSEFOLDER) ||
	        (nImageID == IID_OPENFOLDER) || (nImageID == IID_INPUTFILE) ||
	        (nImageID == IID_BIBTEXFILE))
	{
		TRACE0("Deletion of this item is not allowed!\n");
		return FALSE; // Prevent the deletion
	}

	return TRUE;
}

BOOL CCFileViewEx::CanAddItem(HTREEITEM /*hItem*/)
{
	return TRUE;
}

BOOL CCFileViewEx::CanEditItem(HTREEITEM /*hItem*/)
{
	// 1. We set the return (pResult) 0 to permit the label edit.
	// 2. We would return 1 to cancel it (for an uneditable entry) for the ff cases
	//      a. If tree item is a folder
	//      b. If tree item is a BiBTex or Input or Output files
	// NOTE: The tree item about to be edited is the currently selected item

	// Obtain the image index from the currently selected item
	UINT nImageID = m_ItemSel.GetImageID();
	// If the image index matches any of the prohibited items above prevent editing
	if ((nImageID == IID_PROJECT) || (nImageID == IID_CLOSEFOLDER) ||
	        (nImageID == IID_OPENFOLDER) || (nImageID == IID_BIBTEXFILE) ||
	        (nImageID == IID_INPUTFILE) || (nImageID == IID_OUTPUTFILE))
	{
		return FALSE;
	}

	return TRUE;
}

void CCFileViewEx::OnProjectClean()
{
	// TODO: Add your command handler code here

}

void CCFileViewEx::OnUpdateProjectClean(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here

}

void CCFileViewEx::OnProjectAddFiles()
{
	// TODO: Add your command handler code here

}

void CCFileViewEx::OnUpdateProjectAddFiles(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here

}

void CCFileViewEx::OnProjectBuild()
{
	// TODO: Add your command handler code here

}

void CCFileViewEx::OnUpdateProjectBuild(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_ItemMainFile != NULL);
}

void CCFileViewEx::OnFileCompile()
{
	// TODO: Add your command handler code here

}

void CCFileViewEx::OnUpdateFileCompile(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here

}

void CCFileViewEx::OnFileDelete()
{
	ASSERT(m_ItemSel == GetSelectedItem()); // ...if not something is wrong
	if (CanDeleteItem(m_ItemSel))
	{
		CTreeCursor item(m_ItemSel,this);
		DeleteItem(item);
	}
}

void CCFileViewEx::OnUpdateFileDelete(CCmdUI* pCmdUI)
{
	ASSERT(m_ItemSel == GetSelectedItem()); // ...if not something is wrong
	pCmdUI->Enable(CanDeleteItem(m_ItemSel));
}

void CCFileViewEx::OnFolderInsertFiles()
{
	// TODO: Add your command handler code here
}

void CCFileViewEx::OnUpdateFolderInsertFiles(CCmdUI* pCmdUI)
{
	ASSERT(m_ItemSel == GetSelectedItem()); // ...if not something is wrong
	BOOL bAllowed = TRUE;
	if ((m_ItemSel == m_fBiBTeXFolder) || (m_ItemSel == m_fInputFolder) ||
	        (m_ItemSel == m_fOutputFolder))
		bAllowed = FALSE;
	pCmdUI->Enable(bAllowed);
}

void CCFileViewEx::OnFileViewProperties()
{
	// TODO: Add your command handler code here

}

void CCFileViewEx::OnUpdateFileViewProperties(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here

}

void CCFileViewEx::OnProjectRebuildAll()
{
	// TODO: Add your command handler code here

}

void CCFileViewEx::OnUpdateProjectRebuildAll(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_ItemMainFile != NULL);
}

void CCFileViewEx::OnFileRename()
{
	ASSERT(m_ItemSel == GetSelectedItem()); // ...if not something is wrong
	if (CanEditItem(m_ItemSel))
		EditLabel(m_ItemSel);
}

void CCFileViewEx::OnUpdateFileRename(CCmdUI* pCmdUI)
{
	ASSERT(m_ItemSel == GetSelectedItem()); //...if not something is wrong
	pCmdUI->Enable(CanEditItem(m_ItemSel));
}

void CCFileViewEx::OnFileSetAsMain()
{
	ASSERT(m_ItemSel == GetSelectedItem()); //...if not something is wrong
	ASSERT(m_ItemSel.GetParent() == m_fMainFolder); //...must be in the main folder

	// If the current handle to the main file item is valid...
	// 1. Remove the bold text format
	// 2. Reset the image/icon to the normal tex file
	if (m_ItemMainFile.IsValid())
	{
		if (m_ItemMainFile.GetState(TVIS_BOLD) == TVIS_BOLD)
			m_ItemMainFile.SetState(0,TVIS_BOLD);
		m_ItemMainFile.SetImage(IID_TEXFILE,IID_TEXFILE);

		ProjectFileItem* pMainItem =
		    reinterpret_cast<ProjectFileItem*>(GetItemData(m_ItemMainFile));
		ASSERT(pMainItem);
		pMainItem->bIsMainFile = FALSE;
		pMainItem->nFileType = IID_TEXFILE;
	}
	if (m_ItemSel == m_ItemMainFile)
		return;
	m_ItemSel.SetState(TVIS_BOLD,TVIS_BOLD);
	m_ItemSel.SetImage(IID_MAINFILE,IID_MAINFILE);

	ProjectFileItem* pMainItem =
	    reinterpret_cast<ProjectFileItem*>(GetItemData(m_ItemSel));
	ASSERT(pMainItem);
	pMainItem->bIsMainFile = TRUE;
	pMainItem->nFileType = IID_MAINFILE;

	m_ItemMainFile = m_ItemSel;
	// Finally, set the main file information in the project
	theApp.GetProject()->SetMainPath(pMainItem->strFileName);
}

void CCFileViewEx::OnUpdateFileSetAsMain(CCmdUI* pCmdUI)
{
	ASSERT(m_ItemSel == GetSelectedItem()); // ...if not something is wrong
	pCmdUI->Enable(m_ItemSel.GetParent() == m_fMainFolder);
	pCmdUI->SetCheck(m_ItemSel == m_ItemMainFile);
}

void CCFileViewEx::OnFileOpen()
{
	ASSERT(m_ItemSel == GetSelectedItem()); // ...if not something is wrong
	ProjectFileItem* pFileItem = NULL;
	if (m_xFileList.Lookup(m_ItemSel.GetText(),pFileItem))
	{
		//TODO--Test that the file exist
		AfxGetApp()->OpenDocumentFile(pFileItem->strFileName);
	}
}

void CCFileViewEx::OnUpdateFileOpen(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here

}

void CCFileViewEx::OnUpdateOutputFiles()
{
	AfxMessageBox(_T("Now going to update output files"));
	// TODO: Add your command handler code here

}

void CCFileViewEx::OnUpdateUpdateOutputFiles(CCmdUI* pCmdUI)
{
	ASSERT(m_ItemSel == GetSelectedItem()); // ...if not something is wrong
	pCmdUI->Enable((m_ItemSel == m_fOutputFolder) || (m_ItemSel == m_fProject));
}
