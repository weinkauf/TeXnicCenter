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

#if !defined(AFX_TEMPLATEDIALOG_H__5588FFA4_2C94_11D3_929E_444553540000__INCLUDED_)
#define AFX_TEMPLATEDIALOG_H__5588FFA4_2C94_11D3_929E_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/** @addtogroup templates
        @{
 */

/**	Abstract base class for all elements listed in the template list
control of a template dialog.

@author Sven Wiegand
 */
class CTemplateItem : public CObject
{
// construction/destruction
public:
	CTemplateItem();
	virtual ~CTemplateItem();

// operations
public:
	/**
	Called to initialize the attributes of the item. Has to be called
	before one of the attribute-operations can be called.

	@param lpszPath
	        Path to the template file.
	@param ImageList32, ImageList16
	        Image list, the index returned by GetImageIndex() references to.

	@return
	        Nonzero if successfull, FALSE otherwise.
	 */
	virtual BOOL InitItem(LPCTSTR lpszPath,CImageList &ImageList32,CImageList &ImageList16) = 0;

	/** Should return the title of the template. */
	virtual const CString GetTitle() const = 0;

	/** Should return a description of the template. */
	virtual const CString GetDescription() const = 0;

	/**
	Index to the bitmap for this item in the image list specified by
	InitItem().
	 */
	virtual int GetImageIndex() const = 0;
};


/** An array of template items */
typedef CArray<CTemplateItem*,CTemplateItem*> CTemplateItemArray;

void AFXAPI DestructElements(CTemplateItem **pElements,int nCount);

/**
Dialog for creating new elements based on template files.

@ingroup dialogs

@author Sven Wiegand
 */
class CTemplateDialog : public CDialog
{
// construction/destruction
public:
	CTemplateDialog(UINT unIDTemplate,CWnd* pParent = NULL);
	virtual ~CTemplateDialog(){}

// operations
public:
	/**
	Defines the extensions of files that can be used as templates.

	@param lpszExt
	        Name (with wild cards) that specifies files to use as document
	        templates (for example "*.dot" or "*.cpp").
	@param pTemplateItemClass
	        Class to store the information about templates of this type in.
	 */
	void AddTemplateFilter(LPCTSTR lpszName,CRuntimeClass *pTemplateItemClass);

	/**
	Adds a directory, the dialog should search in for document templates.

	@param lpszSearchDir
	        Complete path of the directory to search file templates in.
	 */
	void AddSearchDir(LPCTSTR lpszSearchDir);

	/**
	Adds the specified item to the specified category (tab).
	 */
	void AddTemplateItem(LPCTSTR lpszCategory,CTemplateItem *pItem);

// implementation helpers
protected:
	/**
	Collects the available templates and stores them for later use.
	 */
	virtual void CollectTemplates();

	/**
	Fills the template list with the files of the selected directory tab.
	 */
	virtual void FillTemplateList();

	/**
	Updates the template specific controls in the dialog based on the
	new selection (i.e. the template description).
	 */
	virtual void OnTemplateSelectionChanged();

	/**
	Updates the state of the controls depending on the current
	settings.

	This base implementation enables the dialog's OK button, if there
	is a selected template. Otherwise the button will be disabled.
	 */
	virtual void UpdateControlStates();

	/**
	Returns the index of the currently selected item in the template
	list or -1 if there is no selected item.
	 */
	int GetSelectedItem() const;

	/**
	Updates the view type of the template list in dependency of the
	current value of m_nListViewType.
	 */
	void UpdateTemplateListViewType();

// overridables
protected:
	/**
	Performs the action to create a document/project based on the
	current settings. Will be called, when the 'Create' button will
	be pressed.
	 */
	virtual void Create() = 0;

// overridings
protected:
//{{AFX_VIRTUAL(CTemplateDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// implementation
protected:
	//{{AFX_MSG(CTemplateDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTabCategories(NMHDR* pNMHDR,LRESULT* pResult);
	afx_msg void OnTemplateItemChanged(NMHDR* pNMHDR,LRESULT* pResult);
	afx_msg void OnCreate();
	afx_msg void OnDblClkTemplate(NMHDR* pNMHDR,LRESULT* pResult);
	afx_msg void OnViewTypeSelection();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// dialog data
protected:
	//{{AFX_DATA(CTemplateDialog)
	CListCtrl m_wndTemplateList;
	CTabCtrl m_wndCategoriesTab;
	CString m_strDescription;
	int m_nListViewType;
	//}}AFX_DATA

// constants
protected:

	/** Constants describing the view modes for the template list. */
	enum tagListViewType
	{
		lvtIcons = 0,
		lvtList
	};

// attributes
protected:
	///The Tab of m_wndCategoriesTab to be activated first.
	int m_nFirstTab;

	///The Tab of m_wndCategoriesTab that was activated last.
	int m_nLastTab;

private:
	/** All valid file names (including wild cards) */
	CStringArray m_astrFilters;

	/** Runtime class objects for the filters stored in m_astrFilters */
	CArray<CRuntimeClass*,CRuntimeClass*> m_apTemplateItemClass;

	/** All paths to search in */
	CStringArray m_astrSearchPaths;

	/** Stores all files by their subdirectories, the key is the subdirectory. */
	CMap<CString,LPCTSTR,CTemplateItemArray*,CTemplateItemArray*> m_mapSubdirToTemplates;

	/** Image list for the template list (large icons) */
	CImageList m_ImageList32;

	/** Image list for the template list (small icons) */
	CImageList m_ImageList16;
};

void AFXAPI DestructElements(CTemplateItemArray* *pElements,int nCount);

/** @} */

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_TEMPLATEDIALOG_H__5588FFA4_2C94_11D3_929E_444553540000__INCLUDED_
