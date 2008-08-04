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

// AttributeMenu.h: interface for the CAttributeMenu class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATTRIBUTEMENU_H__D0D279E5_3764_4C10_97D4_CBD4EC716AFE__INCLUDED_)
#define AFX_ATTRIBUTEMENU_H__D0D279E5_3764_4C10_97D4_CBD4EC716AFE__INCLUDED_

#include "CCrystalEditView.h"
#include "CCrystalTextBuffer.h"
#include "editcmd.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRYSEDIT_CLASS_DECL CAttributeMenu
{
public:
	CAttributeMenu(CCrystalEditView *pView, const CPoint& textPoint);
	virtual ~CAttributeMenu();

	BOOL ShowSpellMenu(Speller *pSpell, const CPoint& screenPoint);

protected:

	typedef enum tagBase
	{
		none,
		spellErrorCount = MAXSUGGESTION,
		spellErrorStart = ID_SPELL_ERROR_FIRST,
		spellErrorStop = ID_SPELL_ERROR_LAST
	};

	CCrystalEditView *m_pView;
	CPoint m_textPoint;
	CMenu m_menu;
	CCrystalTextBuffer::CTextAttribute m_attribute;

protected:
	virtual int TrackPopupMenu(CView* view, const CPoint& pt, CMenu* menu);
};

#endif // !defined(AFX_ATTRIBUTEMENU_H__D0D279E5_3764_4C10_97D4_CBD4EC716AFE__INCLUDED_)
