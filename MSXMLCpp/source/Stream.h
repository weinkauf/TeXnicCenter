/********************************************************************
*
* $Workfile:$
* $Revision$
* $Modtime:$
* $Author$
*
* Änderungen:
*	$History:$
*
*********************************************************************/

/********************************************************************
*
* Copyright (C) 2002 Sven Wiegand
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
*********************************************************************
*
* Darf mit freundlicher Genehmigung von Sven Wiegand von der
* Photon Laser Engineering GmbH uneingeschraenkt verwendet werden, 
* sofern dieser Header unveraendert bleibt.
*
********************************************************************/

#if !defined(AFX_STREAM_H__BB5AA7A2_A3D5_4B55_A456_8CEAF4512FB2__INCLUDED_)
#define AFX_STREAM_H__BB5AA7A2_A3D5_4B55_A456_8CEAF4512FB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace MsXml
{

/**
An implementation of the COM-Interface IStream, that works on any
CFile-object.

This class requires Run-Time-Type-Information (RTTI) to be enabled
for the compiler.

@author Sven Wiegand
*/
class AFX_EXT_CLASS CFileStream : public IStream
{
// construction/destruction
public:
	/**
	@param pFile
		Pointer to the file object to use.
	@param bAutoDestroyFile
		TRUE if the attached CFile object should be destroyes using
		Close() and delete, when the reference count of the object 
		decrements to zero. FALSE if the attached CFile object should
		neither be close nor destoryed on destruction.
	*/
	CFileStream(CFile *pFile, BOOL bAutoDestroyFile = TRUE);
	virtual ~CFileStream();

// operatione
public:
	/**
	Returns a pointer to the file object in use.
	*/
	CFile *GetFile() const;

// IUnknown-implementation
public:
	STDMETHODIMP QueryInterface(REFIID riid, void** ppv);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();

// ISequentialStream-implementation
public:
	STDMETHODIMP Read(void *pv, ULONG cb, ULONG *pcbRead);
	STDMETHODIMP Write(const void *pv, ULONG cb, ULONG *pcbWritten);
	
// IStream-implementation
public:
	STDMETHODIMP Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER *plibNewPosition);
	STDMETHODIMP SetSize(ULARGE_INTEGER libNewSize);
	STDMETHODIMP CopyTo(IStream *pstm, ULARGE_INTEGER cb, ULARGE_INTEGER *pcbRead, ULARGE_INTEGER *pcbWritten);
	STDMETHODIMP Commit(DWORD grfCommitFlag);
	STDMETHODIMP Revert();
	STDMETHODIMP LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType);
	STDMETHODIMP UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType);
	STDMETHODIMP Stat(STATSTG *pstatstg, DWORD grfStatFlag);
	STDMETHODIMP Clone(IStream **ppstm);

// implementation helper
protected:
	/**
	Returns the corresponding HRESULT for the m_cause memeber of the
	given exception.

	@param pE
		The exception that should be converted to a HRESULT-value.
	@param bDelete
		TRUE if the Delete() method of the exception object should be
		called, FALSE otherwise.
	*/
	virtual HRESULT HResultFromException(CException *pE, BOOL bDelete = TRUE);

// attributes
private:
	/** The file object */
	CFile *m_pFile;

	/** Auto destroy CFile object on destruction? */
	BOOL m_bAutoDestroyFile;

	/** Reference Counter */
	long m_cRef;

	/** Module state for this object */
	AFX_MODULE_STATE *m_pModuleState;
};

} //namespace MsXml

#endif // !defined(AFX_STREAM_H__BB5AA7A2_A3D5_4B55_A456_8CEAF4512FB2__INCLUDED_)
