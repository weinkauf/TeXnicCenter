/********************************************************************
 *
 * This file is part of the TeXnicCenter-system
 *
 * Copyright (C) 2002-$CurrentYear$ Chris Norris
 * Copyright (C) 2002-$CurrentYear$ ToolsCenter
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

// BackgroundThread.h: interface for the CBackgroundThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BACKGROUNDTHREAD_H__0740E10A_BAFA_4EE0_8D1B_D3C4FABB86AD__INCLUDED_)
#define AFX_BACKGROUNDTHREAD_H__0740E10A_BAFA_4EE0_8D1B_D3C4FABB86AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CCrystalTextBuffer.h"
#include "..\..\MFCExt\Include\ExtTools.h"

class CRYSEDIT_CLASS_DECL CSpellerSource 
{
public:
    /**
    Get a speller
    @return Initialized and configured spell checker
     */
    virtual Speller *GetSpeller() = 0;
};

class CRYSEDIT_CLASS_DECL CBackgroundThread : public CWinThread {
    DECLARE_DYNCREATE(CBackgroundThread)

public:
    CBackgroundThread();
    virtual ~CBackgroundThread();

    // message handlers
public:
    afx_msg void AFX_MSG_CALL OnUpdateBuffer(WPARAM wParam, LPARAM lParam);
    afx_msg void AFX_MSG_CALL OnUpdateLine(WPARAM wParam, LPARAM lParam);
    afx_msg void AFX_MSG_CALL OnGetSpeller(WPARAM wParam, LPARAM lParam);
    afx_msg void AFX_MSG_CALL OnEnableSpeller(WPARAM wParam, LPARAM lParam);
    afx_msg void AFX_MSG_CALL OnInvalidateView(WPARAM wParam, LPARAM lParam);

    DECLARE_MESSAGE_MAP()

    // overrides
public:
    virtual BOOL InitInstance();
    virtual BOOL OnIdle(LONG lCount);

public:
    /**
    Set the speller
    @param pSpell Pointer to speller object
     */
    void SetSpeller(Speller *pSpell);

protected:
    /**
    Spell check a single line
    @param pTextView View to check
    @param nLine Line number
     */
    void SpellCheckSingleLine(CCrystalTextView *pTextView, int nLine);

    void SpellDecorate( int &nStart, int nEnd, LPTSTR szLine, TCHAR * pWordBuffer, CCrystalTextBuffer * pBuffer, int nLine, CCrystalParser * pParser );
    /**
    Spell check an entire buffer.
    @param pTextView View to check
     */
    void SpellCheckBuffer(CCrystalTextView *pTextView);

    /**
    Remove attributes from a buffer
    @param pTextView Buffer to remove attributes from
    @param attrType The type of attributes to remove
     */
    void RemoveBufferAttributes(CCrystalTextView *pTextView, CCrystalTextBuffer::CTextAttribute::tagAttribute attrType);

    /**
    Internal method to determine is a view is still valid. A view is invalidated when it sends
    a ID_BG_INVALIDATE_VIEW message.
    @param pView The view to check
    @return <VAR>TRUE</VAR> if valid else <VAR>FALSE</VAR>.
     */
    BOOL IsValidView(void *pView);

protected:
    Speller *m_pSpell;
    BOOL m_bSpellEnabled;
    CPtrList m_pInvalidViews;
};

#endif // !defined(AFX_BACKGROUNDTHREAD_H__0740E10A_BAFA_4EE0_8D1B_D3C4FABB86AD__INCLUDED_)
