/********************************************************************
*
* This file is part of the MFCExt-Library
*
* Copyright (C) 1999-2000 Sven Wiegand
* Copyright (C) 2000-$CurrentYear$ ToolsCenter
* 
* This library is free software; you can redistribute it and/or
* modify, but leave the headers intact and do not remove any 
* copyrights from the source.
*
* This library does not only contain file from us, but also from
* third party developers. Look at the source file headers to get
* detailed information.
*
* If you have further questions visit our homepage
*
*    http://www.ToolsCenter.org
*
********************************************************************/

/********************************************************************
*
* $Id$
*
********************************************************************/

#if !defined(AFX_COMBOCOLORPICKER_H__B2348841_5541_11D1_8756_00A0C9181E86__INCLUDED_)
#define AFX_COMBOCOLORPICKER_H__B2348841_5541_11D1_8756_00A0C9181E86__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ComboColorPicker.h : header file
// © 1997 Baldvin Hansson

/////////////////////////////////////////////////////////////////////////////
// CComboColorPicker window

/**
A Combo Box for picking colors.

@ingroup mfcext

@author Baldvin Hansson
*/
class  CComboColorPicker : public CComboBox
{
// Construction
public:
	CComboColorPicker();

// Attributes
private:
	bool m_bInitialized;
	static COLORREF m_rgStandardColors[];
public:

// Operations
private:
	void InitializeData();

public:
	COLORREF GetSelectedColor();
	BOOL SelectColor( COLORREF color );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComboColorPicker)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

// Implementation
public:
	virtual ~CComboColorPicker();

	// Generated message map functions
protected:
	//{{AFX_MSG(CComboColorPicker)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMBOCOLORPICKER_H__B2348841_5541_11D1_8756_00A0C9181E86__INCLUDED_)
