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

#if !defined(AFX_MRUTRACKSELCOMBOBOX_H__4313FC00_2115_11D4_A221_006097239934__INCLUDED_)
#define AFX_MRUTRACKSELCOMBOBOX_H__4313FC00_2115_11D4_A221_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
An object of this class is a combo box, from which you can always
get the selection start and end of the edit box, the last time,
the box had the focus.

@ingroup guielements

@author Sven Wiegand
 */
class CMRUTrackSelComboBox : public CMRUComboBox
{
// construction/destruction
public:
	CMRUTrackSelComboBox();
	CMRUTrackSelComboBox(
	    LPCTSTR lpszRegKey, LPCTSTR lpszValueFormat, int nMaxMRUSize = MRUC_DEFAULT_MRU_SIZE,
	    BOOL bAutoLoadOnConstruction = TRUE,
	    BOOL bAutoSaveOnDestroy = TRUE, BOOL bAutoSaveAfterAdd = FALSE, BOOL bAutoRefresh = FALSE);

// operations
public:
	/**
	Returns the start of the selection in the edit box, the last time
	the edit box had the input focus.
	 */
	int GetRecentEditSelStart() const;

	/**
	Returns the end of the selection in the edit box, the last time
	the edit box had the input focus.
	 */
	int GetRecentEditSelEnd() const;


// overridings
protected:
	//{{AFX_VIRTUAL(CMRUTrackSelComboBox)
	//}}AFX_VIRTUAL

// message handlers
protected:
	//{{AFX_MSG(CMRUTrackSelComboBox)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

// attributes
protected:
	/** Start of the selection, the last time the edit box hd the focus */
	int m_nRecentEditSelStart;

	/** End of the selection, the last time the edit box had the focus. */
	int m_nRecentEditSelEnd;
};


// inlines

inline
int CMRUTrackSelComboBox::GetRecentEditSelStart() const
{
	return m_nRecentEditSelStart;
}

inline
int CMRUTrackSelComboBox::GetRecentEditSelEnd() const
{
	return m_nRecentEditSelEnd;
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_MRUTRACKSELCOMBOBOX_H__4313FC00_2115_11D4_A221_006097239934__INCLUDED_
