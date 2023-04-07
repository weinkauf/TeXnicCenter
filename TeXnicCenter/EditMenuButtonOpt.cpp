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
#include "EditMenuButtonOpt.h"
#include "PlaceholderSingleOptDlg.h"
#include "PlaceholderSetsOptDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// CEditMenuButtonOpt
//-------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CEditMenuButtonOpt,CEditMenuButton)
	//{{AFX_MSG_MAP(CEditMenuButtonOpt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CEditMenuButtonOpt::CEditMenuButtonOpt(UINT unMenuId,BOOL bRightArrow /*= TRUE*/)
		: CEditMenuButton(unMenuId,bRightArrow)
{
}

void CEditMenuButtonOpt::OnShowMenu()
{
	CMFCMenuButton::OnShowMenu();

	if (!m_nMenuResult)
		return;

	//Do we have a valid edit control?
	ASSERT(m_pEdit && IsWindow(m_pEdit->m_hWnd));

	if (!m_pEdit || !IsWindow(m_pEdit->m_hWnd))
		return;

	//Get the result as a string
	CString strResult = CString(MAKEINTRESOURCE(m_nMenuResult));

	//Does the result from the menu start with '\n'?
	if (strResult[0] != _T('\n'))
	{
		//No. So we insert it unmodified
		m_pEdit->InsertText(strResult);
	}
	else
	{
		//Yes. Let the user define some options.

		//First: What kind of dialog is requested?
		// - Find "[" and "]"
		int nDlgIDStart = strResult.Find(_T('['));
		int nDlgIDEnd = strResult.Find(_T(']'));
		ASSERT(nDlgIDStart > 0);
		ASSERT(nDlgIDEnd > 0);
		ASSERT((nDlgIDEnd - nDlgIDStart - 1) > 0);
		//We get the internal Dialog ID
		int nDlgID = _ttoi(strResult.Mid(nDlgIDStart + 1,nDlgIDEnd - nDlgIDStart - 1));

		//Get the base string, if any
		CString strBase = strResult.Mid(nDlgIDEnd + 1);

		//Now create, init and show the dialog
		switch (nDlgID)
		{
			case 1: //The Dialog for definition of placeholders for single files
				// - OPTIONS ALLOWED
			case 2: //The Dialog for definition of placeholders for single files
				// - NO OPTIONS ALLOWED
			{
				CPlaceholderSingleOptDlg SingleDlg;
				SingleDlg.SetPlaceholderBase(strBase);

				if (nDlgID == 2)
				{
					SingleDlg.SetAllowOptions(false);
				}
				else
				{
					SingleDlg.SetAllowOptions(true);
				}

				if (SingleDlg.DoModal() == IDOK)
				{
					//Insert the constructed Placeholder for single files
					m_pEdit->InsertText(SingleDlg.strPlaceholder);
				}
				break;
			}


			case 3: //The Dialog for definition of placeholders for file sets
				// - NO OPTIONS ALLOWED; Files shown on more than one line
			case 4: //The Dialog for definition of placeholders for file sets
				// - OPTIONS ALLOWED; Files shown on one line
			{
				CPlaceholderSetsOptDlg SetsDlg;

				if (nDlgID == 3)
				{
					SetsDlg.SetAllowOptions(false);
					SetsDlg.SetShowOneLineExample(false);
				}
				else
				{
					SetsDlg.SetAllowOptions(true);
					SetsDlg.SetShowOneLineExample(true);
				}

				if (SetsDlg.DoModal() == IDOK)
				{
					//Insert the constructed Placeholder for file sets
					m_pEdit->InsertText(SetsDlg.strPlaceholder);
				}

				break;
			}


			default:
				ASSERT(false);
				return;
		}
	}
}
