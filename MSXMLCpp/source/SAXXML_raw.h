/********************************************************************
*
* File created by COM Interface Wrapper Wizard (CIWW) ver 1.00.000
* at Wed Oct 01 18:25:36 2003
* 
* (CIWW - Copyright © 2001-2002 Sven Wiegand (sven.wiegand@web.de))
*
********************************************************************/

struct Raw_IUnknown
{
	virtual HRESULT __stdcall QueryInterface(
		GUID* riid,
		void** ppvObj) = 0;
	virtual ULONG __stdcall AddRef() = 0;
	virtual ULONG __stdcall Release() = 0;
};

struct Raw_IDispatch : Raw_IUnknown
{
	virtual HRESULT __stdcall GetTypeInfoCount(
		UINT* pctinfo) = 0;
	virtual HRESULT __stdcall GetTypeInfo(
		UINT itinfo,
		ULONG lcid,
		void** pptinfo) = 0;
	virtual HRESULT __stdcall GetIDsOfNames(
		GUID* riid,
		CHAR** rgszNames,
		UINT cNames,
		ULONG lcid,
		long* rgdispid) = 0;
	virtual HRESULT __stdcall Invoke(
		long dispidMember,
		GUID* riid,
		ULONG lcid,
		USHORT wFlags,
		DISPPARAMS* pdispparams,
		VARIANT* pvarResult,
		EXCEPINFO* pexcepinfo,
		UINT* puArgErr) = 0;
};

struct Raw_IMXAttributes : Raw_IDispatch
{
	virtual HRESULT __stdcall addAttribute(
		BSTR strURI,
		BSTR strLocalName,
		BSTR strQName,
		BSTR strType,
		BSTR strValue) = 0;
	virtual HRESULT __stdcall addAttributeFromIndex(
		VARIANT varAtts,
		INT nIndex) = 0;
	virtual HRESULT __stdcall clear() = 0;
	virtual HRESULT __stdcall removeAttribute(
		INT nIndex) = 0;
	virtual HRESULT __stdcall setAttribute(
		INT nIndex,
		BSTR strURI,
		BSTR strLocalName,
		BSTR strQName,
		BSTR strType,
		BSTR strValue) = 0;
	virtual HRESULT __stdcall setAttributes(
		VARIANT varAtts) = 0;
	virtual HRESULT __stdcall setLocalName(
		INT nIndex,
		BSTR strLocalName) = 0;
	virtual HRESULT __stdcall setQName(
		INT nIndex,
		BSTR strQName) = 0;
	virtual HRESULT __stdcall setType(
		INT nIndex,
		BSTR strType) = 0;
	virtual HRESULT __stdcall setURI(
		INT nIndex,
		BSTR strURI) = 0;
	virtual HRESULT __stdcall setValue(
		INT nIndex,
		BSTR strValue) = 0;
};

struct Raw_IMXWriter : Raw_IDispatch
{
	virtual HRESULT __stdcall put_output(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall get_output(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_encoding(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_encoding(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall put_byteOrderMark(
		short rhs) = 0;
	virtual HRESULT __stdcall get_byteOrderMark(
		short* Result) = 0;
	virtual HRESULT __stdcall put_indent(
		short rhs) = 0;
	virtual HRESULT __stdcall get_indent(
		short* Result) = 0;
	virtual HRESULT __stdcall put_standalone(
		short rhs) = 0;
	virtual HRESULT __stdcall get_standalone(
		short* Result) = 0;
	virtual HRESULT __stdcall put_omitXMLDeclaration(
		short rhs) = 0;
	virtual HRESULT __stdcall get_omitXMLDeclaration(
		short* Result) = 0;
	virtual HRESULT __stdcall put_version(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_version(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall put_disableOutputEscaping(
		short rhs) = 0;
	virtual HRESULT __stdcall get_disableOutputEscaping(
		short* Result) = 0;
	virtual HRESULT __stdcall flush() = 0;
};

struct Raw_IVBSAXXMLReader : Raw_IDispatch
{
	virtual HRESULT __stdcall getFeature(
		BSTR strName,
		short* Result) = 0;
	virtual HRESULT __stdcall putFeature(
		BSTR strName,
		short fValue) = 0;
	virtual HRESULT __stdcall getProperty(
		BSTR strName,
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall putProperty(
		BSTR strName,
		VARIANT varValue) = 0;
	virtual HRESULT __stdcall get_entityResolver(
		IVBSAXEntityResolver** Result) = 0;
	virtual HRESULT __stdcall putref_entityResolver(
		IVBSAXEntityResolver* rhs) = 0;
	virtual HRESULT __stdcall get_contentHandler(
		IVBSAXContentHandler** Result) = 0;
	virtual HRESULT __stdcall putref_contentHandler(
		IVBSAXContentHandler* rhs) = 0;
	virtual HRESULT __stdcall get_dtdHandler(
		IVBSAXDTDHandler** Result) = 0;
	virtual HRESULT __stdcall putref_dtdHandler(
		IVBSAXDTDHandler* rhs) = 0;
	virtual HRESULT __stdcall get_errorHandler(
		IVBSAXErrorHandler** Result) = 0;
	virtual HRESULT __stdcall putref_errorHandler(
		IVBSAXErrorHandler* rhs) = 0;
	virtual HRESULT __stdcall get_baseURL(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall put_baseURL(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_secureBaseURL(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall put_secureBaseURL(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall parse(
		VARIANT varInput) = 0;
	virtual HRESULT __stdcall parseURL(
		BSTR strURL) = 0;
};

struct Raw_IVBSAXLocator : Raw_IDispatch
{
	virtual HRESULT __stdcall get_columnNumber(
		INT* Result) = 0;
	virtual HRESULT __stdcall get_lineNumber(
		INT* Result) = 0;
	virtual HRESULT __stdcall get_publicId(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_systemId(
		BSTR* Result) = 0;
};

struct Raw_IVBSAXAttributes : Raw_IDispatch
{
	virtual HRESULT __stdcall get_length(
		INT* Result) = 0;
	virtual HRESULT __stdcall getURI(
		INT nIndex,
		BSTR* Result) = 0;
	virtual HRESULT __stdcall getLocalName(
		INT nIndex,
		BSTR* Result) = 0;
	virtual HRESULT __stdcall getQName(
		INT nIndex,
		BSTR* Result) = 0;
	virtual HRESULT __stdcall getIndexFromName(
		BSTR strURI,
		BSTR strLocalName,
		INT* Result) = 0;
	virtual HRESULT __stdcall getIndexFromQName(
		BSTR strQName,
		INT* Result) = 0;
	virtual HRESULT __stdcall getType(
		INT nIndex,
		BSTR* Result) = 0;
	virtual HRESULT __stdcall getTypeFromName(
		BSTR strURI,
		BSTR strLocalName,
		BSTR* Result) = 0;
	virtual HRESULT __stdcall getTypeFromQName(
		BSTR strQName,
		BSTR* Result) = 0;
	virtual HRESULT __stdcall getValue(
		INT nIndex,
		BSTR* Result) = 0;
	virtual HRESULT __stdcall getValueFromName(
		BSTR strURI,
		BSTR strLocalName,
		BSTR* Result) = 0;
	virtual HRESULT __stdcall getValueFromQName(
		BSTR strQName,
		BSTR* Result) = 0;
};

struct Raw_IVBSAXXMLFilter : Raw_IDispatch
{
	virtual HRESULT __stdcall get_parent(
		IVBSAXXMLReader** Result) = 0;
	virtual HRESULT __stdcall putref_parent(
		IVBSAXXMLReader* rhs) = 0;
};

