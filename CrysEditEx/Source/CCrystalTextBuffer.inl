////////////////////////////////////////////////////////////////////////////
//	File:		CCrystalTextBuffer.inl
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
* $RCSfile$
* $Revision$
* $Date$
*
* $Author$
*
* $Log$
* Revision 1.1.1.1  2002/02/26 08:11:53  svenwiegand
* Initial revision
*
* Revision 1.0  2000-05-31 21:55:24+02  sven_wiegand
* Initial revision
*
*
********************************************************************/

#ifndef __CCrystalTextBuffer_INL_INCLUDED
#define __CCrystalTextBuffer_INL_INCLUDED

#include "CCrystalTextBuffer.h"

CE_INLINE BOOL CCrystalTextBuffer::IsModified() const
{
	return m_bModified;
}

CE_INLINE void CCrystalTextBuffer::ReleaseLineAttributes()
{
	::LeaveCriticalSection( &m_csLineAttributes );
}

CE_INLINE void CCrystalTextBuffer::LockLineAttributes()
{
	::EnterCriticalSection( &m_csLineAttributes );
}


#endif
