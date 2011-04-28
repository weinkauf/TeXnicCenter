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

#include "stdafx.h"
#include "Stream.h"
#include <atlconv.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace MsXml
{

//-------------------------------------------------------------------
// class CFileStream
//-------------------------------------------------------------------

#define MANAGE_STREAM_STATE \
	AFX_MANAGE_STATE(m_pModuleState)


CFileStream::CFileStream(CFile *pFile, BOOL bAutoDestroyFile /*= TRUE*/)
:	m_pFile(pFile),
	m_bAutoDestroyFile(bAutoDestroyFile),
	m_cRef(1),
	m_pModuleState(NULL)
{
#ifdef _AFXDLL
	m_pModuleState = AfxGetModuleState();
	ASSERT(m_pModuleState != NULL);
#endif
}


CFileStream::~CFileStream()
{
	if (m_bAutoDestroyFile)
		delete (m_pFile);
}


/////////////////////////////////////////////////////////////////////
// operations

CFile* CFileStream::GetFile() const
{
	return m_pFile;
}


/////////////////////////////////////////////////////////////////////
// IUnknown-implemenation

STDMETHODIMP CFileStream::QueryInterface(REFIID riid, void** ppv)
{
	if (riid == IID_IUnknown)
		*ppv = (void*)static_cast<IUnknown*>(this);
	else if (riid == IID_ISequentialStream)
		*ppv = (void*)static_cast<ISequentialStream*>(this);
	else if (riid == IID_IStream)
		*ppv = (void*)static_cast<IStream*>(this);
	else
		*ppv = NULL;

	if (*ppv)
	{
		AddRef();
		return S_OK;
	}
	else
		return E_NOINTERFACE;
}


STDMETHODIMP_(ULONG) CFileStream::AddRef()
{
	return (ULONG)InterlockedIncrement(&m_cRef);
}


STDMETHODIMP_(ULONG) CFileStream::Release()
{
	ASSERT(m_cRef > 0);
	LONG	cRef = InterlockedDecrement(&m_cRef);
	if (cRef == 0)
		delete this;

	return cRef;
}


/////////////////////////////////////////////////////////////////////
// ISequentialStream-implementation

STDMETHODIMP CFileStream::Read(void *pv, ULONG cb, ULONG *pcbRead)
{
	MANAGE_STREAM_STATE;

	try
	{
		UINT	unRead = m_pFile->Read(pv, cb);
		if (pcbRead)
			*pcbRead = unRead;

		if (unRead == 0)
			return S_FALSE;
		else
			return S_OK;
	}
	catch (CException *pE)
	{
		if (pcbRead)
			*pcbRead = 0;
		return HResultFromException(pE);
	}
}


STDMETHODIMP CFileStream::Write(const void *pv, ULONG cb, ULONG *pcbWritten)
{
	MANAGE_STREAM_STATE;

	try
	{
		m_pFile->Write(pv, cb);
		if (pcbWritten)
			*pcbWritten = cb;

		return S_OK;
	}
	catch (CException *pE)
	{
		if (pcbWritten)
			*pcbWritten = 0;
		return HResultFromException(pE);
	}
}


/////////////////////////////////////////////////////////////////////
// IStream-implementation

STDMETHODIMP CFileStream::Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER *plibNewPosition)
{
	MANAGE_STREAM_STATE;

	try
	{
		// CFile does only support 4 GB
		if (dlibMove.QuadPart != (long)dlibMove.QuadPart)
			AfxThrowFileException(CFileException::badSeek);

		UINT unSeekFrom;
		if (dwOrigin == STREAM_SEEK_CUR)
			unSeekFrom = CFile::current;
		else if (dwOrigin == STREAM_SEEK_END)
			unSeekFrom = CFile::end;
		else if (dwOrigin == STREAM_SEEK_SET)
			unSeekFrom = CFile::begin;
		else
		{
			ASSERT(FALSE);
			AfxThrowFileException(CFileException::badSeek);
		}

		ULONGLONG	ullOffset = m_pFile->Seek((LONG)dlibMove.QuadPart, unSeekFrom);
		if (plibNewPosition)
			plibNewPosition->QuadPart = ullOffset;

		return S_OK;
	}
	catch (CException *pE)
	{
		if (plibNewPosition)
			*(ULONGLONG*)plibNewPosition = 0;
		return HResultFromException(pE);
	}
}


STDMETHODIMP CFileStream::SetSize(ULARGE_INTEGER libNewSize)
{
	MANAGE_STREAM_STATE;

	try
	{
		// CFile does only support 4 GB
		if (libNewSize.QuadPart != (ULONG)libNewSize.QuadPart)
			AfxThrowFileException(CFileException::badSeek);

		m_pFile->SetLength((DWORD)libNewSize.QuadPart);
		return S_OK;
	}
	catch (CException *pE)
	{
		return HResultFromException(pE);
	}
}


STDMETHODIMP CFileStream::CopyTo(IStream *pstm, ULARGE_INTEGER cb, ULARGE_INTEGER *pcbRead, ULARGE_INTEGER *pcbWritten)
{
	MANAGE_STREAM_STATE;

	try
	{
		// CFile does only support 4 GB
		if (cb.QuadPart != (ULONG)cb.QuadPart)
			AfxThrowFileException(CFileException::endOfFile);

		BYTE	*pbyBuffer = new BYTE[(int)cb.QuadPart];
		if (!pbyBuffer)
			return E_OUTOFMEMORY;

		UINT	unRead = m_pFile->Read(pbyBuffer, (ULONG)cb.QuadPart);
		if (pcbRead)
			pcbRead->QuadPart = unRead;

		ULONG	unWritten = 0;
		HRESULT	hr = pstm->Write(pbyBuffer, (ULONG)cb.QuadPart, &unWritten);
		if (pcbWritten)
			pcbWritten->QuadPart = unWritten;

		return hr;
	}
	catch (CException *pE)
	{
		if (pcbRead)
			pcbRead->QuadPart = 0;
		if (pcbWritten)
			pcbWritten->QuadPart = 0;

		return HResultFromException(pE);
	}
}


STDMETHODIMP CFileStream::Commit(DWORD grfCommitFlag)
{
	MANAGE_STREAM_STATE;

	try
	{
		m_pFile->Flush();
		return S_OK;
	}
	catch (CException *pE)
	{
		return HResultFromException(pE);
	}
}


STDMETHODIMP CFileStream::Revert()
{
	// not supported
	return S_OK;
}


STDMETHODIMP CFileStream::LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType)
{
	MANAGE_STREAM_STATE;

	#ifdef UNDER_CE
		return STG_E_INVALIDFUNCTION;
	#else
		try
		{
			// CFile does only support 4 GB
			if (libOffset.QuadPart != (ULONG)libOffset.QuadPart || cb.QuadPart != (ULONG)cb.QuadPart)
				AfxThrowFileException(CFileException::lockViolation);

			if (dwLockType != LOCK_EXCLUSIVE)
				return STG_E_INVALIDFUNCTION;

			m_pFile->LockRange((DWORD)libOffset.QuadPart, (DWORD)cb.QuadPart);
			return S_OK;
		}
		catch (CException *pE)
		{
			return HResultFromException(pE);
		}
	#endif
}


STDMETHODIMP CFileStream::UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType)
{
	MANAGE_STREAM_STATE;

	#ifdef UNDER_CE
		return STG_E_INVALIDFUNCTION;
	#else
		try
		{
			// CFile does only support 4 GB
			if (libOffset.QuadPart != (ULONG)libOffset.QuadPart || cb.QuadPart != (ULONG)cb.QuadPart)
				AfxThrowFileException(CFileException::lockViolation);

			if (dwLockType != LOCK_EXCLUSIVE)
				return STG_E_INVALIDFUNCTION;

			m_pFile->UnlockRange((DWORD)libOffset.QuadPart, (DWORD)cb.QuadPart);
			return S_OK;
		}
		catch (CException *pE)
		{
			return HResultFromException(pE);
		}
	#endif
}


STDMETHODIMP CFileStream::Stat(STATSTG *pstatstg, DWORD grfStatFlag)
{
	MANAGE_STREAM_STATE;

	CFileStatus	fs;
	if (!m_pFile->GetStatus(fs))
		return STG_E_ACCESSDENIED;

	SYSTEMTIME	mtime, ctime, atime;
	fs.m_mtime.GetAsSystemTime(mtime);
	fs.m_ctime.GetAsSystemTime(ctime);
	fs.m_atime.GetAsSystemTime(atime);

	ZeroMemory(pstatstg, sizeof(STATSTG));
	pstatstg->type = STGTY_STORAGE;
	pstatstg->cbSize.QuadPart = fs.m_size;
	SystemTimeToFileTime(&mtime, &pstatstg->atime);
	SystemTimeToFileTime(&ctime, &pstatstg->ctime);
	SystemTimeToFileTime(&atime, &pstatstg->atime);
	pstatstg->grfMode = fs.m_attribute; //?
	pstatstg->grfLocksSupported = LOCK_EXCLUSIVE;
	pstatstg->clsid = CLSID_NULL;

	if (!(grfStatFlag&STATFLAG_NONAME))
	{
		USES_CONVERSION;
		CString		m_strFile = m_pFile->GetFileName();
		LPOLESTR	lpolestrFile = T2OLE(const_cast<LPTSTR>((LPCTSTR)m_strFile));
		pstatstg->pwcsName = (LPOLESTR)CoTaskMemAlloc(sizeof(WCHAR)*(m_strFile.GetLength()+1));
		wcscpy(pstatstg->pwcsName, lpolestrFile);
	}

	return S_OK;
}


STDMETHODIMP CFileStream::Clone(IStream **ppstm)
{
	MANAGE_STREAM_STATE;

	#ifdef UNDER_CE
		return STG_E_INVALIDFUNCTION;
	#else
		try
		{
			CFile	*pFile = m_pFile->Duplicate();
			if (!pFile)
			{
				*ppstm = NULL;
				return STG_E_INSUFFICIENTMEMORY;
			}

			CFileStream	*pStream = new CFileStream(pFile);
			if (!pStream)
			{
				pFile->Close();
				delete pFile;
				*ppstm = NULL;
				return STG_E_INSUFFICIENTMEMORY;
			}

			*ppstm = static_cast<IStream*>(pStream);
			return S_OK;
		}
		catch (CException *pE)
		{
			return HResultFromException(pE);
		}
	#endif
}


/////////////////////////////////////////////////////////////////////
// implementation helper

HRESULT CFileStream::HResultFromException(CException *pE, BOOL bDelete /* = TRUE */)
{
	HRESULT	hr;

	if (dynamic_cast<CFileException*>(pE))
	{
		switch (dynamic_cast<CFileException*>(pE)->m_cause)
		{
			case CFileException::none:
				hr = S_OK;
				break;
			case CFileException::tooManyOpenFiles:
				hr = E_FAIL;
				break;
			case CFileException::accessDenied:
				hr = STG_E_ACCESSDENIED;
				break;
			case CFileException::invalidFile:
				hr = E_FAIL;
				break;
			case CFileException::directoryFull:
				hr = STG_E_MEDIUMFULL;
				break;
			case CFileException::badSeek:
				hr = STG_E_INVALIDPOINTER;
				break;
			case CFileException::hardIO:
				hr = STG_E_WRITEFAULT;
				break;
			case CFileException::sharingViolation:
				hr = STG_E_LOCKVIOLATION;
				break;
			case CFileException::lockViolation:
				hr = STG_E_LOCKVIOLATION;
				break;
			case CFileException::diskFull:
				hr = STG_E_MEDIUMFULL;
				break;
			case CFileException::endOfFile:
				hr = E_FAIL;
				break;
			default:
				hr = E_FAIL;
		}
	}
	else if (dynamic_cast<CMemoryException*>(pE))
		hr = STG_E_MEDIUMFULL;
	else if (dynamic_cast<CNotSupportedException*>(pE))
		hr = STG_E_INVALIDFUNCTION;

	if (bDelete)
		pE->Delete();

	return hr;
}

} //namespace MsXml
