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
#include "TextModules.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CTextModule
//////////////////////////////////////////////////////////////////////

CTextModule::CTextModule()
:Name(""),
strText(_T("\001")),
bEnableParams(false)
{

}

CTextModule::~CTextModule()
{

}

bool CTextModule::SetText(const CString& strLeft, const CString& strRight)
{
	ASSERT(strLeft.Find(_T('\001')) == -1);
	ASSERT(strRight.Find(_T('\001')) == -1);

	//We need to assure that the placeholders for the params are ok. Later.

	strText = strLeft + _T('\001') + strRight;
	strText.Replace(_T("\r\n"), _T("\r"));

	return true;
}

CString CTextModule::GetLeftText()
{
	ASSERT(strText.Find(_T('\001')) > -1);

	CString strRet = strText.Left(strText.Find(_T('\001')));
	strRet.Replace(_T("\r"), _T("\r\n"));
	return strRet;
}

CString CTextModule::GetRightText()
{
	ASSERT(strText.Find(_T('\001')) > -1);

	CString strRet = strText.Right(strText.GetLength() - strText.Find(_T('\001')) - 1);
	strRet.Replace(_T("\r"), _T("\r\n"));
	return strRet;
}

bool CTextModule::SerializeToRegistry(CBCGRegistryEx &reg) const
{
	reg.Write(_T("Name"), Name);
	reg.Write(_T("Text"), strText);
	//reg.Write(_T("EnableParams"), bEnableParams);

	return true;
}

bool CTextModule::SerializeFromRegistry(CBCGRegistryEx &reg)
{
	reg.Read(_T("Name"), Name);
	reg.Read(_T("Text"), strText);
	//reg.Read(_T("EnableParams"), (int&)bEnableParams);

	return true;
}


//////////////////////////////////////////////////////////////////////
// CTextModuleGroup
//////////////////////////////////////////////////////////////////////

CTextModuleGroup::CTextModuleGroup()
:Name(_T("Standard"))
{

}

CTextModuleGroup::~CTextModuleGroup()
{

}

int CTextModuleGroup::InsertSorted(CTextModule& newElement, bool bSortAscending /*= true*/, bool bAllowDuplicate /*= false*/)
{
	return CSortArray<CTextModule, CTextModule&>::InsertSorted(newElement, bSortAscending, false);
}

void CTextModuleGroup::AddDefaultModules()
{
	CTextModule tm;

	tm.Name = _T("AMS: multline");
	tm.SetText(_T("\\begin{multline}\r\n\t"), _T("\r\n\\end{multline}"));
	InsertSorted(tm);

	tm.Name = _T("AMS: gather");
	tm.SetText(_T("\\begin{gather}\r\n\t"), _T("\r\n\\end{gather}"));
	InsertSorted(tm);

	tm.Name = _T("AMS: align");
	tm.SetText(_T("\\begin{align}\r\n\t"), _T("\r\n\\end{align}"));
	InsertSorted(tm);
	
	tm.Name = _T("AMS: cases");
	tm.SetText(_T("\\begin{cases}\r\n\t"), _T("\r\n\\end{cases}"));
	InsertSorted(tm);
	
	tm.Name = _T("AMS: text");
	tm.SetText(_T("\\text{"), _T("}"));
	InsertSorted(tm);
}

bool CTextModuleGroup::SerializeToRegistry(LPCTSTR strStartSection) const
{
	CBCGRegistryEx reg(false, false); //HKEY_CURRENT_USER, ReadAndWrite
	CString strKey(CPathTool::Cat(strStartSection, _T("Group0")));
	reg.DeleteKey(strKey);
	reg.CreateKey(strKey);

	//Write the Name of this Group
	reg.Write(_T("Name"), Name);

	//Iterate and Write
	int nsize = GetSize();
	int i;
	for(i=0;i<nsize;i++)
	{
		//Create a Title
		CString strTitle;
		strTitle.Format("%d", i);

		//Write the item
		reg.PushKey();
		reg.CreateKey(strTitle);
		GetAt(i).SerializeToRegistry(reg);
		reg.PopKey();
	}

	return true;
}

bool CTextModuleGroup::SerializeFromRegistry(LPCTSTR strStartSection)
{
	//Remove everything
	RemoveAll();

	CBCGRegistryEx reg(false, true); //HKEY_CURRENT_USER, ReadOnly
	CString strKey(CPathTool::Cat(strStartSection, _T("Group0")));

	//Does the key exist?
	if (!reg.Open(strKey))
	{
		//Key does not exist; so I assume, that we need some standards
		AddDefaultModules();

		//Well, lets say everything was fine
		return true;
	}


	//Read the name of this group
	reg.Read(_T("Name"), Name);

	CTextModule tm;

	//Read the Titles
	CStringArray astrKeys;
	reg.ReadSubKeys(astrKeys);
	reg.PushKey();

	int i;
	for (i=0;i<astrKeys.GetSize();i++)
	{
		reg.TopKey();
		reg.Open(astrKeys[i]);

		//Get the Infos and Insert
		tm.SerializeFromRegistry(reg);
		InsertSorted(tm);
	}

	reg.PopKey();

	return true;
}

