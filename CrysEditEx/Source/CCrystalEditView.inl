////////////////////////////////////////////////////////////////////////////
//	File:		CCrystalEditView.inl
//	Version:	1.0.0.0
//	Created:	29-Dec-1998
//
//	Author:		Stcherbatchenko Andrei
//	E-mail:		windfall@gmx.de
//
//	Inline functions of Crystal Edit classes
//
//	You are free to use or modify this code to the following restrictions:
//	- Acknowledge me somewhere in your about box, simple "Parts of code by.."
//	will be enough. If you can't (or don't want to), contact me personally.
//	- LEAVE THIS HEADER INTACT
////////////////////////////////////////////////////////////////////////////

/********************************************************************
*
* $RCSfile: CCrystalEditView.inl,v $
* $Revision$
* $Date$
*
* $Author$
*
* $Log: CCrystalEditView.inl,v $
* Revision 1.2  2003/12/06 19:59:58  svenwiegand
* - Implemented Feature 601708 + additions: The user can now set the styles for
*   the text cursor independant for the insert and the overwrite mode. The cursor
*   style is defined by its form (line or block) and its mode (blinking or
*   not-blinking). The defaults are 'line' for insert cursor and 'block' for
*   overwrite cursor -- both are blinking.
*
* Revision 1.1.1.1  2002/02/26 08:11:52  svenwiegand
* Initial revision
*
* Revision 1.0  2000-05-31 21:55:22+02  sven_wiegand
* Initial revision
*
*
********************************************************************/

#ifndef __CCrystalEditView_INL_INCLUDED
#define __CCrystalEditView_INL_INCLUDED

#include "CCrystalEditView.h"

CE_INLINE BOOL CCrystalEditView::GetOverwriteMode() const
{
	return m_bOvrMode;
}

CE_INLINE void CCrystalEditView::SetOverwriteMode(BOOL bOvrMode /*= TRUE*/)
{
	m_bOvrMode = bOvrMode;
	UpdateCaret();
}

CE_INLINE BOOL CCrystalEditView::GetDisableBSAtSOL() const
{
	return m_bDisableBSAtSOL;
}

CE_INLINE BOOL CCrystalEditView::GetAutoIndent() const
{
	return m_bAutoIndent;
}

CE_INLINE void CCrystalEditView::SetAutoIndent(BOOL bAutoIndent)
{
	m_bAutoIndent = bAutoIndent;
}

#endif
