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

#if !defined(AFX_IPROJECT_H__E313E425_582A_11D5_A222_006097239934__INCLUDED_)
#define AFX_IPROJECT_H__E313E425_582A_11D5_A222_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace InterfaceImplementationWrappers
{

/**	C++-Wrapper for COM-interface IProject.

@note Works only when used as aggregated class.

@author Sven Wiegand
*/
class IProject : public Interfaces::IProject  
{
// overridables
protected:
	virtual CString GetPath_() const = 0;
	virtual CString GetTitle_() const = 0;
	virtual CString GetMainFile_() const = 0;
	virtual BOOL GetUsesBibTex_() const = 0;
	virtual void SetUsesBibTex_(BOOL bUsesBibTex) = 0;
	virtual BOOL GetUsesMakeIndex_() const = 0;
	virtual void SetUsesMakeIndex_(BOOL bUsesMakeIndex) = 0;
	
// implementation of Interfaces::IProject
private:
	STDMETHOD(get_Path)(BSTR *path);
	STDMETHOD(get_Title)(BSTR *title);
	STDMETHOD(get_MainFile)(BSTR *mainFile);
	STDMETHOD(get_UsesBibTex)(BOOL *usesBibTex);
	STDMETHOD(put_UsesBibTex)(BOOL usesBibTex);
	STDMETHOD(get_UsesMakeIndex)(BOOL *usesMakeIndex);
	STDMETHOD(put_UsesMakeIndex)(BOOL usesMakeIndex);
};

} // namespace

#endif // !defined(AFX_IPROJECT_H__E313E425_582A_11D5_A222_006097239934__INCLUDED_)
