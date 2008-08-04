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

#if !defined(AFX_FILEVIEWEX_H__0517A828_D18F_11D3_A727_009027900694__INCLUDED_)
#define AFX_FILEVIEWEX_H__0517A828_D18F_11D3_A727_009027900694__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileViewEx.h : header file
//

#include "TreeCtrlEx.h"

struct ProjectFileItem
{
	UINT nFolderType;
	CString strFolderName;
	UINT nFileType;
	CString strFileName;
	CString strDispName;
	BOOL bIsMainFile;
	BOOL bIsCompilable;

	ProjectFileItem()
	{
		// unnecessary
		//strFolderName.Empty();
		//strFileName.Empty();
		//strDispName.Empty();
		nFolderType = 0;
		nFileType = 0;
		bIsMainFile = FALSE;
		bIsCompilable = FALSE;
	}
};

/////////////////////////////////////////////////////////////////////////////
// CCFileViewEx window

class CCFileViewEx : public CCTreeCtrlEx
{
// Construction
public:
	CCFileViewEx();

// Attributes
public:
	void SetModifiedFlag(BOOL bModified);
	BOOL IsModified() const;

protected:
	CTreeCursor m_ItemSel;
	CTreeCursor m_ItemMainFile;

	// Drag and drop support attributes of the tree
	CImageList* m_pDragImage;
	HTREEITEM m_htiDrag, m_htiDrop, m_htiOldDrop;
	BOOL m_bLDragging;
	UINT m_idTimer;
	BOOL m_bKeepIndentLevel;
	HCURSOR m_hDropCursor, m_hNoDropCursor, m_hcurSCopyDrop;
	BOOL m_bCopyDrag;
	// Scrolling...
	UINT m_nScrollTimerID;
	UINT m_nTimerTicks;

	//
	BOOL m_bModified;

// Operations
public:

	enum ImageIDS
	{
		IID_PROJECT = 0,
		IID_CLOSEFOLDER,
		IID_OPENFOLDER,
		IID_TEXFILE,
		IID_FILE,
		IID_INPUTFILE,
		IID_MAINFILE,
		IID_OUTPUTFILE,
		IID_BIBTEXFILE
	};

	enum FolderIDS
	{
		FID_PROJECT = 0,
		FID_MAINFILES,
		FID_INPUTFILES,
		FID_BIBTEXFILES,
		FID_OUTPUTFILE,
		FID_PROJECTFILES
	};

	HTREEITEM CopyBranch(HTREEITEM htiBranch, HTREEITEM htiNewParent,
	                     HTREEITEM htiAfter = TVI_LAST);
	HTREEITEM CopyItem(HTREEITEM hti, HTREEITEM htiNewParent,
	                   HTREEITEM htiAfter = TVI_LAST);

	virtual HTREEITEM DoCopyBranch(HTREEITEM htiBranch, HTREEITEM htiNewParent,
	                               HTREEITEM htiAfter = TVI_LAST);
	// Overrides
	// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CCFileViewEx)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCFileViewEx();

	virtual void PopulateTree();
	virtual void PopulateTree(CString& strFileName);
	virtual HTREEITEM AddItem(WORD nFolderType, WORD nItemType, LPCTSTR lpszName);
	virtual void DeleteItem(CTreeCursor& itemDelete);

// Generated message map functions
protected:
	virtual BOOL CanEditItem(HTREEITEM hItem);
	virtual BOOL CanAddItem(HTREEITEM hItem);
	virtual BOOL CanDeleteItem(HTREEITEM hItem);
	int GetIndentLevel(HTREEITEM hItem);
	HTREEITEM GetNextItem(HTREEITEM hItem);
	virtual BOOL IsDragSource(HTREEITEM hItem);
	virtual HTREEITEM GetDropTarget(HTREEITEM hItem);
	CImageList* CreateDragImageEx(HTREEITEM hItem);

	CImageList m_ctlImage;

	// Some bookmarks to places in the tree
	CTreeCursor m_fProject;
	CTreeCursor m_fMainFolder;
	CTreeCursor m_fInputFolder;
	CTreeCursor m_fBiBTeXFolder;
	CTreeCursor m_fOutputFolder;
	CTreeCursor m_fProjectFilesFolder;

	BOOL m_bDoBranchCopy;

	CMap<CString, LPCTSTR, ProjectFileItem*, ProjectFileItem*> m_xFileList;

	virtual void DoPopupMenu(UINT nMenuID, const CPoint* ppt = NULL);
	virtual void DoPopupMenu(CTreeCursor xItem, const CPoint* ppt = NULL);

	//{{AFX_MSG(CCFileViewEx)
	afx_msg void OnNMRightClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMKeyDown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMLDoubleClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemExpanded(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelectionChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnSysColorChange();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnProjectClean();
	afx_msg void OnUpdateProjectClean(CCmdUI* pCmdUI);
	afx_msg void OnProjectAddFiles();
	afx_msg void OnUpdateProjectAddFiles(CCmdUI* pCmdUI);
	afx_msg void OnProjectBuild();
	afx_msg void OnUpdateProjectBuild(CCmdUI* pCmdUI);
	afx_msg void OnFileCompile();
	afx_msg void OnUpdateFileCompile(CCmdUI* pCmdUI);
	afx_msg void OnFileDelete();
	afx_msg void OnUpdateFileDelete(CCmdUI* pCmdUI);
	afx_msg void OnFolderInsertFiles();
	afx_msg void OnUpdateFolderInsertFiles(CCmdUI* pCmdUI);
	afx_msg void OnFileViewProperties();
	afx_msg void OnUpdateFileViewProperties(CCmdUI* pCmdUI);
	afx_msg void OnProjectRebuildAll();
	afx_msg void OnUpdateProjectRebuildAll(CCmdUI* pCmdUI);
	afx_msg void OnFileRename();
	afx_msg void OnUpdateFileRename(CCmdUI* pCmdUI);
	afx_msg void OnFileSetAsMain();
	afx_msg void OnUpdateFileSetAsMain(CCmdUI* pCmdUI);
	afx_msg void OnFileOpen();
	afx_msg void OnUpdateFileOpen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOutputFiles();
	afx_msg void OnUpdateUpdateOutputFiles(CCmdUI* pCmdUI);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

inline void CCFileViewEx::SetModifiedFlag(BOOL bModified)
{
	m_bModified = bModified;
}

inline BOOL CCFileViewEx::IsModified() const
{
	return m_bModified;
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEVIEWEX_H__0517A828_D18F_11D3_A727_009027900694__INCLUDED_)
