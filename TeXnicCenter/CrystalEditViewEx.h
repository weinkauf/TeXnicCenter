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

#if !defined(AFX_CRYSTALEDITVIEWEX_H__BAFA7E63_5437_11D4_A222_006097239934__INCLUDED_)
#define AFX_CRYSTALEDITVIEWEX_H__BAFA7E63_5437_11D4_A222_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/**
This class supports serialization of the editors state to an INI-file,
so that the state can be restored later.

@author Sven Wiegand
*/
class CCrystalEditViewEx : public CCrystalEditView
{
// construction/destruction
protected:
	CCrystalEditViewEx();
	DECLARE_DYNCREATE(CCrystalEditViewEx)

// operations
public:
	/**
	Stores view information to the specified ini-file or restores it 
	from the specified file.

	@param ini
		The ini-file to read the view information from or to write the 
		information to.
	@param bWrite
		FALSE if the information should be read from the file, TRUE if it
		should be written to the file.		

	@return
		FALSE if the document, specified in the ini file has not been 
		opened, TRUE otherwise.
	*/
	BOOL Serialize(CIniFile &ini, LPCTSTR lpszKey, BOOL bWrite);

// overridings
protected:
	//{{AFX_VIRTUAL(CCrystalEditViewEx)
	//}}AFX_VIRTUAL

// implementations helpers
protected:

// message handlers
protected:
	//{{AFX_MSG(CCrystalEditViewEx)
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_CRYSTALEDITVIEWEX_H__BAFA7E63_5437_11D4_A222_006097239934__INCLUDED_
