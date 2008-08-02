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

/********************************************************************
 *
 * $Id$
 *
 ********************************************************************/

#include "stdafx.h"
#include "BackgroundThread.h"
#include "CCrystalTextBuffer.h"
#include "editcmd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CBackgroundThread, CWinThread)

BEGIN_MESSAGE_MAP(CBackgroundThread, CWinThread)
ON_THREAD_MESSAGE(ID_BG_UPDATE_BUFFER, OnUpdateBuffer)
ON_THREAD_MESSAGE(ID_BG_UPDATE_LINE, OnUpdateLine)
ON_THREAD_MESSAGE(ID_BG_RESET_SPELLER, OnGetSpeller)
ON_THREAD_MESSAGE(ID_BG_ENABLE_SPELLER, OnEnableSpeller)
ON_THREAD_MESSAGE(ID_BG_INVALIDATE_VIEW, OnInvalidateView)
END_MESSAGE_MAP()


CBackgroundThread::CBackgroundThread()
{
    m_pSpell = NULL;
    m_bSpellEnabled = false;
}

CBackgroundThread::~CBackgroundThread()
{
}


//////////////////////////////////////////////////////////////////////
// message handlers

void CBackgroundThread::OnUpdateBuffer(WPARAM wParam, LPARAM lParam)
{
    CCrystalTextView *pTextView = (CCrystalTextView *)lParam;
    ASSERT(pTextView);
    if (IsValidView(pTextView)) {
        // Check if there are any redundant message in the message queue.
        // This should really improve speed on slower machines and machines 
        // doing background processing.
        MSG msg;
        while (::PeekMessage(&msg, NULL, ID_BG_UPDATE_BUFFER, ID_BG_UPDATE_BUFFER, PM_NOREMOVE) != 0) {
            if (msg.lParam == lParam) {
                // Remove and discard the duplicate
                ::PeekMessage(&msg, NULL, ID_BG_UPDATE_BUFFER, ID_BG_UPDATE_BUFFER, PM_REMOVE);
                continue;
            }
            break;
        }

        if (m_bSpellEnabled)
            SpellCheckBuffer(pTextView);
        else
            RemoveBufferAttributes(pTextView, CCrystalTextBuffer::CTextAttribute::spellError);
    }
}

void CBackgroundThread::OnUpdateLine(WPARAM wParam, LPARAM lParam)
{
    CCrystalTextView *pTextView = (CCrystalTextView *)lParam;
    ASSERT(pTextView);
    int nLine = (int)wParam;

    if (IsValidView(pTextView)) {
        // Check if there are any redundant message in the message queue.
        // This should really improve speed on slower machines and machines 
        // doing background processing.
        MSG msg;

        while (::PeekMessage(&msg, NULL, ID_BG_UPDATE_BUFFER, ID_BG_UPDATE_LINE, PM_NOREMOVE) != 0) {
            if (msg.lParam == lParam) {
                // This buffer has a message. What could it be? 
                if (msg.message == ID_BG_UPDATE_BUFFER) {
                    // A message to check the whole buffer is in the queue.
                    // There is no point checking a single line -- we're done.
                    return;
                }
                else if (msg.message == ID_BG_UPDATE_LINE && msg.wParam == wParam) {
                    // A duplicate message. Remove and discard.
                    ::PeekMessage(&msg, NULL, ID_BG_UPDATE_LINE, ID_BG_UPDATE_LINE, PM_REMOVE);
                    continue;
                }
                // else  We need to handle this message
            }
            break;
        }

        if (m_bSpellEnabled) {
            SpellCheckSingleLine(pTextView, nLine);
            Sleep(3000); // Give the typist a chance to get ahead to prevent flicker
        }
    }
}

void CBackgroundThread::OnGetSpeller(WPARAM wParam, LPARAM lParam)
{
    CSpellerSource *pSource = (CSpellerSource *)lParam;
    if (pSource) {
        Speller *pSpell = pSource->GetSpeller();
        SetSpeller(pSpell);
    }
}

void CBackgroundThread::OnEnableSpeller(WPARAM wParam, LPARAM lParam)
{
    BOOL bSpellEnabled = (BOOL)wParam;
    if (bSpellEnabled == m_bSpellEnabled)
        return;

    m_bSpellEnabled = bSpellEnabled;
    if (lParam != 0)
        OnUpdateBuffer(0, lParam);
}

void CBackgroundThread::OnInvalidateView(WPARAM wParam, LPARAM lParam)
{
    CCrystalTextView *pTextView = (CCrystalTextView *)lParam;
    if (pTextView) {
        m_pInvalidViews.AddHead(pTextView);
        // Release the view
        pTextView->Release();
    }
}


//////////////////////////////////////////////////////////////////////
// overrides

BOOL CBackgroundThread::InitInstance()
{
    return TRUE;
}

BOOL CBackgroundThread::OnIdle(LONG lCount)
{
    m_pInvalidViews.RemoveAll();
    return 0;
}


//////////////////////////////////////////////////////////////////////
// implementation

void CBackgroundThread::SetSpeller(Speller *pSpell)
{
    m_pSpell = pSpell;
}

const int ShortWordLength = 10;

void CBackgroundThread::SpellCheckSingleLine(CCrystalTextView *pTextView, int nLine)
{
    if (m_pSpell == NULL)
        return;

    ASSERT(pTextView);
    pTextView->Hold();

    TCHAR pWordBuffer[MAXWORDLEN + 1];

    CCrystalParser *pParser = pTextView->GetParser();
    CCrystalTextBuffer *pBuffer = pTextView->LocateTextBuffer();
    ASSERT(nLine < pBuffer->GetLineCount());

    if (!(nLine < pBuffer->GetLineCount())) {
        pTextView->Release();
        return; // Done
    }

    pBuffer->ClearLineAttributes(nLine, CCrystalTextBuffer::CTextAttribute::spellError);
    LPTSTR szLine = pBuffer->GetLineChars(nLine);
    int nStart = 0, nEnd = 0;

    pParser->NextWord(nLine, nStart, nEnd);

    SpellDecorate(nStart, nEnd, szLine, pWordBuffer, pBuffer, nLine, pParser);

    pBuffer->UpdateViews(NULL, NULL, UPDATE_LINEATTR, nLine);
    pTextView->Release();
}

void CBackgroundThread::SpellCheckBuffer(CCrystalTextView *pTextView)
{
    if (m_pSpell == NULL)
        return;

    ASSERT(pTextView);
    pTextView->Hold();

    int nLineIndex = 0;
    TCHAR pWordBuffer[MAXWORDLEN + 1];
    CCrystalParser *pParser = pTextView->GetParser();
    CCrystalTextBuffer *pBuffer = pTextView->LocateTextBuffer();

    while (nLineIndex < pBuffer->GetLineCount()) {
        pBuffer->ClearLineAttributes(nLineIndex, CCrystalTextBuffer::CTextAttribute::spellError);
        LPTSTR szLine = pBuffer->GetLineChars(nLineIndex);
        int nStart = 0, nEnd = 0;

        pParser->NextWord(nLineIndex, nStart, nEnd);

        SpellDecorate(nStart, nEnd, szLine, pWordBuffer, pBuffer, nLineIndex,pParser);

        pBuffer->UpdateViews(NULL, NULL, UPDATE_LINEATTR, nLineIndex);
        ++nLineIndex;
    }

    pTextView->Release();
    return;
}

void CBackgroundThread::RemoveBufferAttributes(CCrystalTextView *pTextView, CCrystalTextBuffer::CTextAttribute::tagAttribute attrType)
{
    ASSERT(pTextView);
    pTextView->Hold();

    CCrystalTextBuffer *pBuffer = pTextView->LocateTextBuffer();
    int nLineIndex = 0;
    while (nLineIndex < pBuffer->GetLineCount())
        pBuffer->ClearLineAttributes(nLineIndex++, attrType);

    pBuffer->UpdateViews(NULL, NULL, UPDATE_LINEATTR, -1);
    pTextView->Release();
}

BOOL CBackgroundThread::IsValidView(void *pView)
{
    // This in affect gives the ID_BG_INVALIDATE_VIEW message the highest priority, 
    // which is what we want.
    MSG msg;

    while (::PeekMessage(&msg, NULL, ID_BG_INVALIDATE_VIEW, ID_BG_INVALIDATE_VIEW, PM_REMOVE) != 0)
        OnInvalidateView(msg.wParam, msg.lParam);

    BOOL isValid = (pView != NULL);
    POSITION pos = m_pInvalidViews.GetHeadPosition();

    while (pos != NULL && isValid) {
        if (pView == m_pInvalidViews.GetNext(pos))
            isValid = FALSE;
    }

    return isValid;
}

void CBackgroundThread::SpellDecorate( int &nStart, int nEnd, LPTSTR szLine, TCHAR * pWordBuffer, CCrystalTextBuffer * pBuffer, int nLine, CCrystalParser * pParser )
{
    int length;

    while (nStart != -1) {
        length = nEnd - nStart;

        if (abs(length) < MAXWORDLEN) {
            *std::copy(szLine + nStart,szLine + nEnd,pWordBuffer) = 0;

            if (!m_pSpell->spell(pWordBuffer)) {
                bool decorate = true;

                // Special case for abbrev. like bzw., bzgl. and so on:
                // Make sure it's short word first and check whether there is
                // one more character after this word
                if (length <= ShortWordLength && nEnd + 1 <= _tcslen(szLine)) {
                    // Include this character
                    *std::copy(szLine + nEnd,szLine + nEnd + 1,pWordBuffer + length) = 0;
                    // and recheck the spelling
                    decorate = !m_pSpell->spell(pWordBuffer);
                }

                if (decorate) {
                    CCrystalTextBuffer::CTextAttribute 
                        attribute(nStart, nEnd, CCrystalTextBuffer::CTextAttribute::spellError);
                    pBuffer->InsertLineAttribute(nLine, attribute);
                }
            }
        }

        nStart = nEnd;
        pParser->NextWord(nLine, nStart, nEnd);
    }
}
