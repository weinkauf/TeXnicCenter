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

#if !defined(AFX_PROJECTPROPERTYDIALOG_H__DB3E5C01_323C_11D3_929E_83C047CE4AE2__INCLUDED_)
#define AFX_PROJECTPROPERTYDIALOG_H__DB3E5C01_323C_11D3_929E_83C047CE4AE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/**	Edits the properties of the current project.

@ingroup dialogs

@author Sven Wiegand
*/
class CProjectPropertyDialog : public CDialog
{
// Construction/Destruction
public:
	CProjectPropertyDialog(CWnd* pParent = NULL);

// virtuals
protected:
	//{{AFX_VIRTUAL(CProjectPropertyDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// message handlers
protected:
	//{{AFX_MSG(CProjectPropertyDialog)
	afx_msg void OnBrowseMainFile();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeOptionsLanguage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// dialog data
public:
	//{{AFX_DATA(CProjectPropertyDialog)
	enum { IDD = IDD_PROJECT_PROPERTIES };
	CString	m_strMainFile;
	BOOL	m_bUseMakeIndex;
	BOOL	m_bUseBibTex;
	//}}AFX_DATA

// attributes
public:
	/** project's directory */
	CString m_strProjectDir;

	CArray<CString, CString&>	m_aLanguage;
	CArray<CString, CString&>	m_aDialect;
	CString						m_strLanguageCurrent;
	CString						m_strDialectCurrent;


private:
	void	FindDictionaries();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_PROJECTPROPERTYDIALOG_H__DB3E5C01_323C_11D3_929E_83C047CE4AE2__INCLUDED_
