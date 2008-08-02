////////////////////////////////////////////////////////////////////////////
//	File:		CrystalTextBufferEx.h
//
//	Author:		Sven Wiegand
//	E-mail:		sven.wiegand@gmx.de
//
//	Interface of the CCrystalTextBufferEx class, a part of Crystal Edit -
//	syntax coloring text editor.
//
//	You are free to use or modify this code.
//
////////////////////////////////////////////////////////////////////////////

/********************************************************************
 *
 * $RCSfile: CrystalTextBufferEx.h,v $
 * $Revision$
 * $Date$
 *
 * $Author$
 *
 * $Log: CrystalTextBufferEx.h,v $
 * Revision 1.1.1.1  2002/02/26 08:12:01  svenwiegand
 * Initial revision
 *
 * Revision 1.0  2000-05-31 21:55:37+02  sven_wiegand
 * Initial revision
 *
 *
 ********************************************************************/

#if !defined(AFX_CRYSTALTEXTBUFFEREX_H__5353A212_1059_11D3_929E_CEFE4CA77E5E__INCLUDED_)
#define AFX_CRYSTALTEXTBUFFEREX_H__5353A212_1059_11D3_929E_CEFE4CA77E5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CCrystalTextBuffer.h"

/**
You can use this class instead of CCrystalTextBuffer, when you are working with
MFC's document-view-architecture. 

This class overrides the SetModified()-method of CCrystalTextBuffer, so that
the SetModifiedFlag()-method of the attached document is called, when the buffer
contents has changed. You do not have to derive your own class from this one.

Add the following code to the class declaration of your document:
<pre>
@@	public:
@@		CCrystalTextBufferEx	m_pTextBuffer;
</pre>

Add the following code to the constructor of your document:
<pre>
@@	m_pTextBuffer = new CCrystalTextBufferEx( this );
</pre>

From there on you can use all the function of CCrystalTextBuffer for
saving and loading documents.
 */
class CRYSEDIT_CLASS_DECL CCrystalTextBufferEx : public CCrystalTextBuffer {
    DECLARE_DYNCREATE(CCrystalTextBufferEx)
    // construction/destruction
public:
    /**
    Creates a crystal-text-buffer and attaches a document to it.

    @param pDoc
            The document to attach to this crystal-text-buffer
     */
    CCrystalTextBufferEx(CDocument *pDoc = NULL);

    // operations
public:
    /**
    Sets or clears the modified-flag.

    @param bModified
            TRUE to set the modified-flag otherwise FALSE.
     */
    virtual void SetModified(BOOL bModified = TRUE);

    /**
    Returns the line with the given zero based index, 
    as a CString-object.

    <b>Attention</b>
    You should not use this function in your parse-functions, because they
    need to copy the line to a newly allocated buffer -- this takes some
    time, so that the parsing would maybe become a little bit to slow.

    @param nLineIndex
            Zero based index of the line to retrieve.
    @param &szLine
            Reference to the CString-object to retrieve the line.

    @return 
            TRUE if the operation was successfull, FALSE otherwise.
     */
    BOOL GetLine(int nLineIndex, CString &strLine);

    // attributes
protected:
    /**
    The document attached to this crystal-text-buffer.
     */
    CDocument *m_pDoc;
};

#endif // !defined(AFX_CRYSTALTEXTBUFFEREX_H__5353A212_1059_11D3_929E_CEFE4CA77E5E__INCLUDED_)
