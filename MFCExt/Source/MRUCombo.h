/////////////////////////////////////////////////////////////////////////////
//
// MRUCombo.h: Header file for the CMRUComboBox class.
//
// Written by Michael Dunn <mdunn at inreach dot com>
//
/////////////////////////////////////////////////////////////////////////////
//
// Revision history:
//
//  9/9/1998: First release.
//  2/5/2000: Sven Wiegand: CRecentFileList was replaced by CMRUStringList
//
/////////////////////////////////////////////////////////////////////////////

/********************************************************************
*
* $Id$
*
********************************************************************/

#if !defined(AFX_MRUCOMBO_H__BDE24F13_4632_11D2_9505_D07F50C10000__INCLUDED_)
#define AFX_MRUCOMBO_H__BDE24F13_4632_11D2_9505_D07F50C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MRUCombo.h : header file
//

#include "MRUStringList.h"

/////////////////////////////////////////////////////////////////////////////
// CMRUComboBox window

#define MRUC_DEFAULT_MRU_SIZE   10

/**
A combo box, that supports MRU-Strings.

This is done using CMRUStringList.

@ingroup mfcext

@author Michael Dunn <mdunn at inreach dot com>
@author Sven Wiegand
*/
class AFX_EXT_CLASS CMRUComboBox : public CComboBox
{
// Construction
public:
	CMRUComboBox();
	CMRUComboBox(
		LPCTSTR lpszRegKey, LPCTSTR lpszValueFormat, int nMaxMRUSize = MRUC_DEFAULT_MRU_SIZE, 
		BOOL bAutoLoadOnConstruction = TRUE, 
		BOOL bAutoSaveOnDestroy = TRUE, BOOL bAutoSaveAfterAdd = FALSE, BOOL bAutoRefresh = FALSE );

// Attributes & accessor functions
public:
    void           SetMRURegKey ( LPCTSTR szRegKey );
    const CString& GetMRURegKey() const;

    BOOL           SetMRUValueFormat ( LPCTSTR szValueFormat );
    const CString& GetMRUValueFormat() const;

    int            SetMaxMRUSize ( int nMaxSize );
    int            GetMaxMRUSize() const;

    BOOL SetAutoSaveOnDestroy   ( BOOL bAutoSave );
    BOOL SetAutoSaveAfterAdd    ( BOOL bAutoSave );
    BOOL SetAutoRefreshAfterAdd ( BOOL bAutoRefresh );

protected:
    CMRUStringList*     m_pMRU;
    CString             m_cstrRegKey;
    CString             m_cstrRegValueFormat;
    int                 m_nMaxMRUSize;
    BOOL                m_bSaveOnDestroy;
    BOOL                m_bSaveAfterAdd;
    BOOL                m_bRefreshAfterAdd;

// Operations
public:
    BOOL AddToMRU ( LPCTSTR szNewItem );
    void EmptyMRU();

    void RefreshCtrl();

    BOOL LoadMRU();
    BOOL SaveMRU();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMRUComboBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMRUComboBox();

protected:
    BOOL AllocNewMRU();
    BOOL VerifyMRUParams() const;

    BOOL m_bParamsChanged;

	// Generated message map functions
protected:
	//{{AFX_MSG(CMRUComboBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MRUCOMBO_H__BDE24F13_4632_11D2_9505_D07F50C10000__INCLUDED_)
