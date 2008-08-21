#pragma once
#include <vector>
#include <algorithm>
#include <functional>
#include <limits>
#include <cstring>
#include <cstddef>

inline UINT GetDragQueryFileCount(HDROP hDrop)
{
	return ::DragQueryFile(hDrop,~0u,0,0);
}

template<class OutputIterator>
inline void DragQueryFile(HDROP hDrop, OutputIterator out, UINT n)
{
	for (UINT i = 0; i < n; ++i) {
		const UINT length = ::DragQueryFile(hDrop,i,0,0);

		CString text;
		LPTSTR psz = text.GetBufferSetLength(static_cast<int>(length));
		::DragQueryFile(hDrop,i,psz,length + 1);
		text.ReleaseBuffer(static_cast<int>(length));

		*out++ = text;
	}
}

template<class OutputIterator>
inline void DragQueryFile(HDROP hDrop, OutputIterator out)
{
	DragQueryFile(hDrop,out,GetDragQueryFileCount(hDrop));
}

template<class B, class O>
struct ComObjectInstantiate;

template<class B>
struct ComObjectInstantiate<B,ATL::CComObject<B> >
{
	static HRESULT CreateInstance(CComObject<B>** p)
	{
		return CComObject<B>::CreateInstance(p);
	}
};

template<class B>
struct ComObjectInstantiate<B,ATL::CComObjectNoLock<B> >
{
	static HRESULT CreateInstance(CComObjectNoLock<B>** p)
	{
		*p = new CComObjectNoLock<B>;
		return S_OK;
	}
};

template<class T>
class IDropSourceImpl :
	public IDropSource
{
public:
	STDMETHOD(GiveFeedback)(DWORD /*dwEffect*/)
	{
		return DRAGDROP_S_USEDEFAULTCURSORS;
	}

	STDMETHOD(QueryContinueDrag)(BOOL fEscapePressed, DWORD grfKeyState)
	{
		if (fEscapePressed)
			return DRAGDROP_S_CANCEL;

		if ((grfKeyState & MK_LBUTTON) != MK_LBUTTON)
			return DRAGDROP_S_DROP;

		return S_OK;
	}
};

template<class T>
class IDropTargetImpl :
	public IDropTarget
{
public:
	STDMETHOD(DragEnter)(IDataObject* pDataObject, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
	{
		ATLTRACE2(atlTraceCOM,0,_T("IDropTargetImpl::DragEnter\n"));
		return S_OK;
	}

	STDMETHOD(DragLeave)()
	{
		ATLTRACE2(atlTraceCOM,0,_T("IDropTargetImpl::DragLeave\n"));
		return S_OK;
	}

	STDMETHOD(DragOver)(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
	{
		*pdwEffect = DropEffectFromKeyState(grfKeyState);

		return S_OK;
	}

	STDMETHOD(Drop)(IDataObject* pDataObject, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
	{
		ATLTRACE2(atlTraceCOM,0,_T("IDropTargetImpl::Drop\n"));
		return S_OK;
	}

	static DWORD DropEffectFromKeyState(DWORD grfKeyState)
	{
		switch (grfKeyState & (MK_CONTROL|MK_SHIFT)) {
			case MK_CONTROL|MK_SHIFT:
				return DROPEFFECT_LINK;
			case MK_CONTROL:
				return DROPEFFECT_COPY;
		}

		return DROPEFFECT_MOVE;
	}
};

class FormatEtc :
	public FORMATETC
{
public:
	FormatEtc()
	: FORMATETC()
	{
	}

	FormatEtc(CLIPFORMAT cf, TYMED tymed, DWORD dwAspect = DVASPECT_CONTENT, DWORD lindex = -1, DVTARGETDEVICE* ptd = 0)
	{
		cfFormat = cf;
		FORMATETC::tymed = tymed;
		FORMATETC::dwAspect = dwAspect;
		FORMATETC::lindex = lindex;
		FORMATETC::ptd = ptd;
	}
};

// Caller owned STGMEDIUM object
class StgMedium :
	public STGMEDIUM
{
public:
	StgMedium()
	: STGMEDIUM()
	{
	}

	explicit StgMedium(DWORD tymed, IUnknown* pUnkForRelease = 0)
	: STGMEDIUM()
	{
		STGMEDIUM::tymed = tymed;
		STGMEDIUM::pUnkForRelease = pUnkForRelease;
	}

	explicit StgMedium(const FormatEtc& fetc, IUnknown* pUnkForRelease = 0)
	: STGMEDIUM()
	{
		tymed = fetc.tymed;
		STGMEDIUM::pUnkForRelease = pUnkForRelease;
	}

	~StgMedium()
	{
		Release();
	}

	void Release()
	{
		::ReleaseStgMedium(this);
	}
};

template <class T, template<class> class EC = ATL::CComObjectNoLock>
class ISimpleDataObjectImpl :
	public IDataObject
{
	struct Data
	{
		STGMEDIUM stg;
		FORMATETC fetc;
		bool release;

		void ReleaseMedium()
		{
			::ReleaseStgMedium(&stg);
		}
	};

	struct EqualFormatEtc :
		public std::binary_function<Data,FORMATETC,bool>
	{
		result_type operator()(const first_argument_type& a, const second_argument_type& b) const
		{
			return a.fetc.cfFormat == b.cfFormat;
		}
	};

	struct ExtractFormatEtc :
		public std::unary_function<Data,FORMATETC>
	{
		result_type operator()(const argument_type& a) const
		{
			return a.fetc;
		}
	};

	typedef std::vector<FORMATETC> FormatEtcContainerType;

	class EnumFORMATETC :
		public CComEnumOnSTL<IEnumFORMATETC,&IID_IEnumFORMATETC,FORMATETC,_Copy<FORMATETC>,FormatEtcContainerType>
	{
	public:
		//BEGIN_COM_MAP(EnumFORMATETC)
		//	COM_INTERFACE_ENTRY(IEnumFORMATETC)
		//END_COM_MAP()

		HRESULT Initialize(const FormatEtcContainerType& fetcs)
		{
			return Init(0,fetcs_ = fetcs);
		}

	private:
		FormatEtcContainerType fetcs_;
	};

	typedef std::vector<Data> DataContainerType;
	DataContainerType data_;

public:
	~ISimpleDataObjectImpl()
	{
		std::for_each(data_.begin(),data_.end(),
			std::mem_fun_ref(&Data::ReleaseMedium));
	}

	STDMETHOD(GetData)(FORMATETC *pformatetcIn, STGMEDIUM *pmedium)
	{
		DataContainerType::iterator result = data_.end();
		HRESULT hr = DoQueryGetData(pformatetcIn,&result);

		if (FAILED(hr))
			return hr;

		ATLASSERT(result != data_.end());

		hr = CopyStgMedium(pmedium,&result->stg,pformatetcIn);

		if (FAILED(hr))
			return hr;

		return S_OK;
	}

	STDMETHOD(GetDataHere)(FORMATETC* /*pformatetc*/, STGMEDIUM* /*pmedium*/)
	{
		ATLTRACENOTIMPL(_T("ISimpleDataObjectImpl::GetDataHere"));
	}

	STDMETHOD(QueryGetData)(FORMATETC* pformatetc)
	{
		return DoQueryGetData(pformatetc);
	}

private:
	STDMETHOD(DoQueryGetData)(FORMATETC* pformatetc, typename DataContainerType::iterator* p  = 0)
	{
		if (pformatetc->lindex != -1)
			return DV_E_LINDEX;

		DataContainerType::iterator result = std::find_if(data_.begin(),data_.end(),
			std::bind2nd(EqualFormatEtc(),*pformatetc));

		if (result == data_.end())
			return DV_E_FORMATETC;

		if (result->fetc.tymed != pformatetc->tymed)
			return DV_E_TYMED;

		if (p)
			*p = result;

		return S_OK;
	}

public:
	STDMETHOD(GetCanonicalFormatEtc)(FORMATETC* /* pformatectIn */,FORMATETC* /* pformatetcOut */)
	{
		ATLTRACENOTIMPL(_T("ISimpleDataObjectImpl::GetCanonicalFormatEtc"));
	}

	STDMETHOD(SetData)(FORMATETC* pformatetc, STGMEDIUM* pmedium, BOOL fRelease)
	{
		ATLASSERT(pformatetc && pmedium);		
		STGMEDIUM stg = *pmedium;
		
		if (!fRelease) {
			if (FAILED(CopyStgMedium(&stg,pmedium,pformatetc)))
				return E_FAIL;
		}

		Data data = {stg,*pformatetc,fRelease != 0};

		DataContainerType::iterator result = std::find_if(data_.begin(),data_.end(),
			std::bind2nd(EqualFormatEtc(),*pformatetc));

		if (result != data_.end()) {
			result->ReleaseMedium();
			*result = data;
		}
		else
			data_.push_back(data);

		return S_OK;
	}

	STDMETHOD(EnumFormatEtc)(DWORD dwDirection, IEnumFORMATETC** ppenumFormatEtc)
	{
		if (dwDirection != DATADIR_GET)
			return E_NOTIMPL;

		typedef EC<EnumFORMATETC> EnumComType;
		EnumComType* p = 0;
		HRESULT result = ComObjectInstantiate<EnumFORMATETC,EnumComType>::CreateInstance(&p);

		if (FAILED(result))
			return E_FAIL;

		FormatEtcContainerType fetcs;
		std::transform(data_.begin(),data_.end(),std::back_inserter(fetcs),ExtractFormatEtc());

		p->Initialize(fetcs);

		if (FAILED(p->QueryInterface(__uuidof(IEnumFORMATETC),reinterpret_cast<void**>(ppenumFormatEtc))))
			return E_FAIL;

		return S_OK;
	}

	STDMETHOD(DAdvise)(FORMATETC* /*pformatetc*/, DWORD /*advf*/, IAdviseSink* /*pAdvSink*/, DWORD* /*pdwConnection*/)
	{
		ATLTRACENOTIMPL(_T("ISimpleDataObjectImpl::DAdvise"));
	}

	STDMETHOD(DUnadvise)(DWORD /*dwConnection*/)
	{
		ATLTRACENOTIMPL(_T("ISimpleDataObjectImpl::DUnadvise"));
	}

	STDMETHOD(EnumDAdvise)(IEnumSTATDATA** /*ppenumAdvise*/)
	{
		ATLTRACENOTIMPL(_T("ISimpleDataObjectImpl::EnumDAdvise"));
	}

private:
	static HRESULT CopyStgMedium(STGMEDIUM* pMedDest, STGMEDIUM* pMedSrc, FORMATETC* pFmtSrc)
	{
		switch (pMedSrc->tymed) {
			case TYMED_GDI:
			case TYMED_FILE:
			case TYMED_ENHMF:
			case TYMED_MFPICT:
			case TYMED_HGLOBAL:
				pMedDest->hGlobal = reinterpret_cast<HGLOBAL>
					(::OleDuplicateData(pMedSrc->hGlobal,pFmtSrc->cfFormat,0));
				break;
			case TYMED_ISTREAM:
				{
					pMedDest->pstm = 0;

					if (FAILED(::CreateStreamOnHGlobal(0,TRUE,&pMedDest->pstm)))
						return E_OUTOFMEMORY;

					ULARGE_INTEGER alot = {0,std::numeric_limits<unsigned long>::max()};
					
					if (FAILED(pMedSrc->pstm->CopyTo(pMedDest->pstm,alot,0,0)))
						return E_FAIL;
				}
				break;
			case TYMED_ISTORAGE:
				{
					pMedDest->pstg = 0;

					if (FAILED(::StgCreateDocfile(0,STGM_READWRITE|STGM_SHARE_EXCLUSIVE|STGM_DELETEONRELEASE|STGM_CREATE,0,&pMedDest->pstg)))
						return E_OUTOFMEMORY;

					if (FAILED(pMedSrc->pstg->CopyTo(0,0,0,pMedDest->pstg)))
						return E_FAIL;
				}
				break;
			case TYMED_NULL:
				pMedDest->hGlobal = pMedSrc->hGlobal;
				break;
			default:
				ATLASSERT(false);
				return DV_E_TYMED;
		}
		
		pMedDest->tymed = pMedSrc->tymed;
		pMedDest->pUnkForRelease = pMedSrc->pUnkForRelease;
		
		return S_OK;
   }

public:
	STDMETHOD(SetData)(const void* data, SIZE_T dwBytes, CLIPFORMAT cf, IUnknown* pUnkForRelease = 0)
	{
		FORMATETC fetc = {cf,0,DVASPECT_CONTENT,-1,TYMED_HGLOBAL};
		STGMEDIUM stg = {fetc.tymed,{0},pUnkForRelease};

		stg.hGlobal = ::GlobalAlloc(GMEM_MOVEABLE,dwBytes);

		if (!stg.hGlobal)
			return E_OUTOFMEMORY;

		std::memcpy(::GlobalLock(stg.hGlobal),data,dwBytes);
		::GlobalUnlock(stg.hGlobal);

		return SetData(&fetc,&stg,TRUE);
	}

	STDMETHOD(SetData)(LPCSTR pszText, SIZE_T dwBytes, IUnknown* pUnkForRelease = 0)
	{
		return SetData(pszText,dwBytes,CF_TEXT,pUnkForRelease);
	}

	STDMETHOD(SetData)(LPCWSTR pszText, SIZE_T dwBytes, IUnknown* pUnkForRelease = 0)
	{
		return SetData(pszText,dwBytes * sizeof(wchar_t),CF_UNICODETEXT,pUnkForRelease);
	}
};

class SimpleDataObject :
	public CComObjectRootEx<CComSingleThreadModel>,
	public ISimpleDataObjectImpl<SimpleDataObject>
{
public:
	BEGIN_COM_MAP(SimpleDataObject)
		COM_INTERFACE_ENTRY(IDataObject)
	END_COM_MAP()
};

class SimpleDropSource :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDropSourceImpl<SimpleDropSource>
{
public:
	BEGIN_COM_MAP(SimpleDropSource)
		COM_INTERFACE_ENTRY(IDropSource)
	END_COM_MAP()
};