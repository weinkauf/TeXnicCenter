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

#if !defined(AFX_BIBTEXENTRY_H__F01E9D3A_27FE_44B2_A8F3_AD4490011BE2__INCLUDED_)
#define AFX_BIBTEXENTRY_H__F01E9D3A_27FE_44B2_A8F3_AD4490011BE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StructureParser.h"


#define BIBTEX_ENTRY_SIZE 15

extern const TCHAR* BibTypeVerbose[];



class CBiBTeXEntry : public CObject, public CStructureItem  
{
public:
	
	static enum BibType {Book=0, Article, Booklet, Manual, Inproceedings, 
						Conference, Inbook, Incollection, Masterthesis,
						Misc, Phdthesis, Proceedings, Techreport,
						Unpublished, String, Comment, Preamble, Unknown};


	CBiBTeXEntry(CString key, BibType type=Book);
	virtual ~CBiBTeXEntry();

	BOOL SetField(CString name, CString value, BOOL forceOverwrite=FALSE);
	BOOL GetField(CString name, CString &value);

	BibType GetType() const {return m_Type;}
	void SetType(BibType type) {m_Type = type;}

	CString GetKey() const {return m_Key;}

	CString ToString();
	CString ToCaption();
private:
	CMapStringToString m_Fields;
	BibType m_Type;	
	CString m_Key;
};

#endif // !defined(AFX_BIBTEXENTRY_H__F01E9D3A_27FE_44B2_A8F3_AD4490011BE2__INCLUDED_)
