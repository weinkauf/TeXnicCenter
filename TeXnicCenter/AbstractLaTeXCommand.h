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

#if !defined(AFX_ABSTRACTLATEXCOMMAND_H__F6CB60C7_B5EA_4FFC_A1A1_DEAEDB79A29C__INCLUDED_)
#define AFX_ABSTRACTLATEXCOMMAND_H__F6CB60C7_B5EA_4FFC_A1A1_DEAEDB79A29C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <memory>

class CStyleFile;

/**
 * @brief @ref CStyleFile weak pointer type.
 */
typedef std::tr1::weak_ptr<CStyleFile> WeakStyleFilePtr;

/**
 * @brief @ref CStyleFile smart pointer type.
 */
typedef std::tr1::shared_ptr<CStyleFile> SharedStyleFilePtr;

/**
CAbstractLaTeXCommand is the base class for all LaTeX constructs handled by
CStylefile(Container).

@see CStylefile
*/
class CAbstractLaTeXCommand : public CObject
{
public:

	DECLARE_DYNAMIC(CAbstractLaTeXCommand)
	CAbstractLaTeXCommand(const WeakStyleFilePtr& parent, const CString &name);
	CAbstractLaTeXCommand(CAbstractLaTeXCommand &cmd);

	virtual ~CAbstractLaTeXCommand();

	/** Returns a pointer to the containing style or class file */
	const WeakStyleFilePtr& GetStyleFile() const;;

	/** Returns the LaTeX string representation
	@param showParString If true, the appropriate amount of bracket pairs is inserted behind the
	command.
	*/
	virtual const CString ToLaTeX(BOOL showParString = TRUE) const;
	/** Returns a unique identifier of the object (may be used as hash value) */
	virtual const CString ToString() const;
	/** Set the description (free text) of the function */
	void SetDescription(const CString &desc);

	const CString& GetDescription() const;

	/** Name of the command (without any decorations )*/
	const CString& GetName() const;

	/** Returns a pointer to the style file which contains this command */
	const WeakStyleFilePtr& GetParent() const;

	COLORREF GetIconMaskColor() const;
	const CString& GetHighColorIconFileName() const;

	/** Sets/gets the icon index (position of the icon in the bitmap). A bitmap must have a size of n * 16 x 15 pixel, where
	n is the number of icons in the bitmap. Example: A bitmap with 5 icons must have a size of 80 x 15 pixel.
	*/
	void SetIconIndex(int idx);
	int GetIconIndex() const;
	/** Sets/gets the icon file name */
	void SetIconFileName(const CString& file);
	const CString& GetIconFileName() const;
	void SetHighColorIconFileName(const CString& file);

	virtual void Dump(CDumpContext& dc) const;
private:
	CString m_Description;
	CString m_Name;
	WeakStyleFilePtr m_Parent;
	int m_IconIndex;
	CString m_IconFile;
	COLORREF icon_mask_color_;
	CString high_color_icon_fn_;

protected:
	void SetName(const CString &name);
};

#endif // !defined(AFX_ABSTRACTLATEXCOMMAND_H__F6CB60C7_B5EA_4FFC_A1A1_DEAEDB79A29C__INCLUDED_)
