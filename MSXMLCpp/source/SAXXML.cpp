/********************************************************************
*
* File created by COM Interface Wrapper Wizard (CIWW) ver 1.00.000
* at Wed Oct 01 18:25:36 2003
* 
* (CIWW - Copyright © 2001-2002 Sven Wiegand (sven.wiegand@web.de))
*
********************************************************************/

#include "stdafx.h"
#include "saxxml.h"

namespace MsXml
{


#include "SAXXML_raw.h"


static GUID LIBID = {0xf5078f18, 0xc551, 0x11d3, {0x89, 0xb9, 0x00, 0x00, 0xf8, 0x1f, 0xe2, 0x21}};

#define LIBVER_MAJOR 4
#define LIBVER_MINOR 0

//--------------------------------------------------------------------
// class CMXAttributes
//--------------------------------------------------------------------

CMXAttributes::CMXAttributes()
:	CInterfaceCallingWrapper<IMXAttributes>()
{}


CMXAttributes::CMXAttributes(IMXAttributes *p, BOOL bIncrementRefCount /*= FALSE*/)
:	CInterfaceCallingWrapper<IMXAttributes>(p, bIncrementRefCount)
{}


CMXAttributes::CMXAttributes(const CMXAttributes &o)
:	CInterfaceCallingWrapper<IMXAttributes>(o)
{}


CMXAttributes::CMXAttributes(IUnknown *pUnknown, BOOL bIncrementRefCount /*= FALSE*/, BOOL bThrowException /*= FALSE*/)
:	CInterfaceCallingWrapper<IMXAttributes>(pUnknown, bIncrementRefCount, bThrowException)
{}


void CMXAttributes::AddAttribute(LPCTSTR strURI, LPCTSTR strLocalName, LPCTSTR strQName, LPCTSTR strType, LPCTSTR strValue)
{
	CString	strstrURI(strURI);
	BSTR	bstrstrURI = strstrURI.AllocSysString();
	CString	strstrLocalName(strLocalName);
	BSTR	bstrstrLocalName = strstrLocalName.AllocSysString();
	CString	strstrQName(strQName);
	BSTR	bstrstrQName = strstrQName.AllocSysString();
	CString	strstrType(strType);
	BSTR	bstrstrType = strstrType.AllocSysString();
	CString	strstrValue(strValue);
	BSTR	bstrstrValue = strstrValue.AllocSysString();

	HRESULT	hr = ((Raw_IMXAttributes*)P())->addAttribute(bstrstrURI, bstrstrLocalName, bstrstrQName, bstrstrType, bstrstrValue);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IMXAttributes));

	SysFreeString(bstrstrURI);
	SysFreeString(bstrstrLocalName);
	SysFreeString(bstrstrQName);
	SysFreeString(bstrstrType);
	SysFreeString(bstrstrValue);
}


void CMXAttributes::AddAttributeFromIndex(_variant_t varAtts, INT nIndex)
{
	VARIANT	varvarAtts = varAtts.Detach();

	HRESULT	hr = ((Raw_IMXAttributes*)P())->addAttributeFromIndex(varvarAtts, nIndex);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IMXAttributes));

	VariantClear(&varvarAtts);
}


void CMXAttributes::Clear()
{

	HRESULT	hr = ((Raw_IMXAttributes*)P())->clear();
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IMXAttributes));

}


void CMXAttributes::RemoveAttribute(INT nIndex)
{

	HRESULT	hr = ((Raw_IMXAttributes*)P())->removeAttribute(nIndex);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IMXAttributes));

}


void CMXAttributes::SetAttribute(INT nIndex, LPCTSTR strURI, LPCTSTR strLocalName, LPCTSTR strQName, LPCTSTR strType, LPCTSTR strValue)
{
	CString	strstrURI(strURI);
	BSTR	bstrstrURI = strstrURI.AllocSysString();
	CString	strstrLocalName(strLocalName);
	BSTR	bstrstrLocalName = strstrLocalName.AllocSysString();
	CString	strstrQName(strQName);
	BSTR	bstrstrQName = strstrQName.AllocSysString();
	CString	strstrType(strType);
	BSTR	bstrstrType = strstrType.AllocSysString();
	CString	strstrValue(strValue);
	BSTR	bstrstrValue = strstrValue.AllocSysString();

	HRESULT	hr = ((Raw_IMXAttributes*)P())->setAttribute(nIndex, bstrstrURI, bstrstrLocalName, bstrstrQName, bstrstrType, bstrstrValue);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IMXAttributes));

	SysFreeString(bstrstrURI);
	SysFreeString(bstrstrLocalName);
	SysFreeString(bstrstrQName);
	SysFreeString(bstrstrType);
	SysFreeString(bstrstrValue);
}


void CMXAttributes::SetAttributes(_variant_t varAtts)
{
	VARIANT	varvarAtts = varAtts.Detach();

	HRESULT	hr = ((Raw_IMXAttributes*)P())->setAttributes(varvarAtts);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IMXAttributes));

	VariantClear(&varvarAtts);
}


void CMXAttributes::SetLocalName(INT nIndex, LPCTSTR strLocalName)
{
	CString	strstrLocalName(strLocalName);
	BSTR	bstrstrLocalName = strstrLocalName.AllocSysString();

	HRESULT	hr = ((Raw_IMXAttributes*)P())->setLocalName(nIndex, bstrstrLocalName);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IMXAttributes));

	SysFreeString(bstrstrLocalName);
}


void CMXAttributes::SetQName(INT nIndex, LPCTSTR strQName)
{
	CString	strstrQName(strQName);
	BSTR	bstrstrQName = strstrQName.AllocSysString();

	HRESULT	hr = ((Raw_IMXAttributes*)P())->setQName(nIndex, bstrstrQName);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IMXAttributes));

	SysFreeString(bstrstrQName);
}


void CMXAttributes::SetType(INT nIndex, LPCTSTR strType)
{
	CString	strstrType(strType);
	BSTR	bstrstrType = strstrType.AllocSysString();

	HRESULT	hr = ((Raw_IMXAttributes*)P())->setType(nIndex, bstrstrType);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IMXAttributes));

	SysFreeString(bstrstrType);
}


void CMXAttributes::SetURI(INT nIndex, LPCTSTR strURI)
{
	CString	strstrURI(strURI);
	BSTR	bstrstrURI = strstrURI.AllocSysString();

	HRESULT	hr = ((Raw_IMXAttributes*)P())->setURI(nIndex, bstrstrURI);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IMXAttributes));

	SysFreeString(bstrstrURI);
}


void CMXAttributes::SetValue(INT nIndex, LPCTSTR strValue)
{
	CString	strstrValue(strValue);
	BSTR	bstrstrValue = strstrValue.AllocSysString();

	HRESULT	hr = ((Raw_IMXAttributes*)P())->setValue(nIndex, bstrstrValue);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IMXAttributes));

	SysFreeString(bstrstrValue);
}




//--------------------------------------------------------------------
// class CMXWriter
//--------------------------------------------------------------------

CMXWriter::CMXWriter()
:	CInterfaceCallingWrapper<IMXWriter>()
{}


CMXWriter::CMXWriter(IMXWriter *p, BOOL bIncrementRefCount /*= FALSE*/)
:	CInterfaceCallingWrapper<IMXWriter>(p, bIncrementRefCount)
{}


CMXWriter::CMXWriter(const CMXWriter &o)
:	CInterfaceCallingWrapper<IMXWriter>(o)
{}


CMXWriter::CMXWriter(IUnknown *pUnknown, BOOL bIncrementRefCount /*= FALSE*/, BOOL bThrowException /*= FALSE*/)
:	CInterfaceCallingWrapper<IMXWriter>(pUnknown, bIncrementRefCount, bThrowException)
{}


void CMXWriter::SetOutput(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IMXWriter*)P())->put_output(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IMXWriter));

	VariantClear(&varrhs);
}


_variant_t CMXWriter::GetOutput()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IMXWriter*)P())->get_output(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IMXWriter));

	_variant_t	o(varO, false);
	return o;
}


void CMXWriter::SetEncoding(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IMXWriter*)P())->put_encoding(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IMXWriter));

	SysFreeString(bstrrhs);
}


CString CMXWriter::GetEncoding()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IMXWriter*)P())->get_encoding(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IMXWriter));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CMXWriter::SetByteOrderMark(BOOL rhs)
{

	HRESULT	hr = ((Raw_IMXWriter*)P())->put_byteOrderMark(rhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IMXWriter));

}


BOOL CMXWriter::GetByteOrderMark()
{
	short	o;

	HRESULT	hr = ((Raw_IMXWriter*)P())->get_byteOrderMark(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IMXWriter));

	return o;
}


void CMXWriter::SetIndent(BOOL rhs)
{

	HRESULT	hr = ((Raw_IMXWriter*)P())->put_indent(rhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IMXWriter));

}


BOOL CMXWriter::GetIndent()
{
	short	o;

	HRESULT	hr = ((Raw_IMXWriter*)P())->get_indent(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IMXWriter));

	return o;
}


void CMXWriter::SetStandalone(BOOL rhs)
{

	HRESULT	hr = ((Raw_IMXWriter*)P())->put_standalone(rhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IMXWriter));

}


BOOL CMXWriter::GetStandalone()
{
	short	o;

	HRESULT	hr = ((Raw_IMXWriter*)P())->get_standalone(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IMXWriter));

	return o;
}


void CMXWriter::SetOmitXMLDeclaration(BOOL rhs)
{

	HRESULT	hr = ((Raw_IMXWriter*)P())->put_omitXMLDeclaration(rhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IMXWriter));

}


BOOL CMXWriter::GetOmitXMLDeclaration()
{
	short	o;

	HRESULT	hr = ((Raw_IMXWriter*)P())->get_omitXMLDeclaration(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IMXWriter));

	return o;
}


void CMXWriter::SetVersion(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IMXWriter*)P())->put_version(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IMXWriter));

	SysFreeString(bstrrhs);
}


CString CMXWriter::GetVersion()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IMXWriter*)P())->get_version(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IMXWriter));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CMXWriter::SetDisableOutputEscaping(BOOL rhs)
{

	HRESULT	hr = ((Raw_IMXWriter*)P())->put_disableOutputEscaping(rhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IMXWriter));

}


BOOL CMXWriter::GetDisableOutputEscaping()
{
	short	o;

	HRESULT	hr = ((Raw_IMXWriter*)P())->get_disableOutputEscaping(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IMXWriter));

	return o;
}


void CMXWriter::Flush()
{

	HRESULT	hr = ((Raw_IMXWriter*)P())->flush();
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IMXWriter));

}




//--------------------------------------------------------------------
// class CVBSAXXMLReader
//--------------------------------------------------------------------

CVBSAXXMLReader::CVBSAXXMLReader()
:	CInterfaceCallingWrapper<IVBSAXXMLReader>()
{}


CVBSAXXMLReader::CVBSAXXMLReader(IVBSAXXMLReader *p, BOOL bIncrementRefCount /*= FALSE*/)
:	CInterfaceCallingWrapper<IVBSAXXMLReader>(p, bIncrementRefCount)
{}


CVBSAXXMLReader::CVBSAXXMLReader(const CVBSAXXMLReader &o)
:	CInterfaceCallingWrapper<IVBSAXXMLReader>(o)
{}


CVBSAXXMLReader::CVBSAXXMLReader(IUnknown *pUnknown, BOOL bIncrementRefCount /*= FALSE*/, BOOL bThrowException /*= FALSE*/)
:	CInterfaceCallingWrapper<IVBSAXXMLReader>(pUnknown, bIncrementRefCount, bThrowException)
{}


BOOL CVBSAXXMLReader::GetFeature(LPCTSTR strName)
{
	CString	strstrName(strName);
	BSTR	bstrstrName = strstrName.AllocSysString();
	short	o;

	HRESULT	hr = ((Raw_IVBSAXXMLReader*)P())->getFeature(bstrstrName, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IVBSAXXMLReader));

	SysFreeString(bstrstrName);
	return o;
}


void CVBSAXXMLReader::PutFeature(LPCTSTR strName, BOOL fValue)
{
	CString	strstrName(strName);
	BSTR	bstrstrName = strstrName.AllocSysString();

	HRESULT	hr = ((Raw_IVBSAXXMLReader*)P())->putFeature(bstrstrName, fValue);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IVBSAXXMLReader));

	SysFreeString(bstrstrName);
}


_variant_t CVBSAXXMLReader::GetProperty(LPCTSTR strName)
{
	CString	strstrName(strName);
	BSTR	bstrstrName = strstrName.AllocSysString();
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IVBSAXXMLReader*)P())->getProperty(bstrstrName, &varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IVBSAXXMLReader));

	SysFreeString(bstrstrName);
	_variant_t	o(varO, false);
	return o;
}


void CVBSAXXMLReader::PutProperty(LPCTSTR strName, _variant_t varValue)
{
	CString	strstrName(strName);
	BSTR	bstrstrName = strstrName.AllocSysString();
	VARIANT	varvarValue = varValue.Detach();

	HRESULT	hr = ((Raw_IVBSAXXMLReader*)P())->putProperty(bstrstrName, varvarValue);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IVBSAXXMLReader));

	SysFreeString(bstrstrName);
	VariantClear(&varvarValue);
}


IVBSAXEntityResolver* CVBSAXXMLReader::GetEntityResolver()
{
	IVBSAXEntityResolver* o;

	HRESULT	hr = ((Raw_IVBSAXXMLReader*)P())->get_entityResolver(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IVBSAXXMLReader));

	return o;
}


void CVBSAXXMLReader::SetRefEntityResolver(IVBSAXEntityResolver* rhs)
{

	HRESULT	hr = ((Raw_IVBSAXXMLReader*)P())->putref_entityResolver(rhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IVBSAXXMLReader));

}


IVBSAXContentHandler* CVBSAXXMLReader::GetContentHandler()
{
	IVBSAXContentHandler* o;

	HRESULT	hr = ((Raw_IVBSAXXMLReader*)P())->get_contentHandler(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IVBSAXXMLReader));

	return o;
}


void CVBSAXXMLReader::SetRefContentHandler(IVBSAXContentHandler* rhs)
{

	HRESULT	hr = ((Raw_IVBSAXXMLReader*)P())->putref_contentHandler(rhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IVBSAXXMLReader));

}


IVBSAXDTDHandler* CVBSAXXMLReader::GetDtdHandler()
{
	IVBSAXDTDHandler* o;

	HRESULT	hr = ((Raw_IVBSAXXMLReader*)P())->get_dtdHandler(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IVBSAXXMLReader));

	return o;
}


void CVBSAXXMLReader::SetRefDtdHandler(IVBSAXDTDHandler* rhs)
{

	HRESULT	hr = ((Raw_IVBSAXXMLReader*)P())->putref_dtdHandler(rhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IVBSAXXMLReader));

}


IVBSAXErrorHandler* CVBSAXXMLReader::GetErrorHandler()
{
	IVBSAXErrorHandler* o;

	HRESULT	hr = ((Raw_IVBSAXXMLReader*)P())->get_errorHandler(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IVBSAXXMLReader));

	return o;
}


void CVBSAXXMLReader::SetRefErrorHandler(IVBSAXErrorHandler* rhs)
{

	HRESULT	hr = ((Raw_IVBSAXXMLReader*)P())->putref_errorHandler(rhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IVBSAXXMLReader));

}


CString CVBSAXXMLReader::GetBaseURL()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IVBSAXXMLReader*)P())->get_baseURL(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IVBSAXXMLReader));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CVBSAXXMLReader::SetBaseURL(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IVBSAXXMLReader*)P())->put_baseURL(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IVBSAXXMLReader));

	SysFreeString(bstrrhs);
}


CString CVBSAXXMLReader::GetSecureBaseURL()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IVBSAXXMLReader*)P())->get_secureBaseURL(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IVBSAXXMLReader));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CVBSAXXMLReader::SetSecureBaseURL(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IVBSAXXMLReader*)P())->put_secureBaseURL(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IVBSAXXMLReader));

	SysFreeString(bstrrhs);
}


void CVBSAXXMLReader::Parse(_variant_t varInput)
{
	VARIANT	varvarInput = varInput.Detach();

	HRESULT	hr = ((Raw_IVBSAXXMLReader*)P())->parse(varvarInput);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IVBSAXXMLReader));

	VariantClear(&varvarInput);
}


void CVBSAXXMLReader::ParseURL(LPCTSTR strURL)
{
	CString	strstrURL(strURL);
	BSTR	bstrstrURL = strstrURL.AllocSysString();

	HRESULT	hr = ((Raw_IVBSAXXMLReader*)P())->parseURL(bstrstrURL);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IVBSAXXMLReader));

	SysFreeString(bstrstrURL);
}




//--------------------------------------------------------------------
// class CVBSAXLocator
//--------------------------------------------------------------------

CVBSAXLocator::CVBSAXLocator()
:	CInterfaceCallingWrapper<IVBSAXLocator>()
{}


CVBSAXLocator::CVBSAXLocator(IVBSAXLocator *p, BOOL bIncrementRefCount /*= FALSE*/)
:	CInterfaceCallingWrapper<IVBSAXLocator>(p, bIncrementRefCount)
{}


CVBSAXLocator::CVBSAXLocator(const CVBSAXLocator &o)
:	CInterfaceCallingWrapper<IVBSAXLocator>(o)
{}


CVBSAXLocator::CVBSAXLocator(IUnknown *pUnknown, BOOL bIncrementRefCount /*= FALSE*/, BOOL bThrowException /*= FALSE*/)
:	CInterfaceCallingWrapper<IVBSAXLocator>(pUnknown, bIncrementRefCount, bThrowException)
{}


INT CVBSAXLocator::GetColumnNumber()
{
	INT o;

	HRESULT	hr = ((Raw_IVBSAXLocator*)P())->get_columnNumber(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IVBSAXLocator));

	return o;
}


INT CVBSAXLocator::GetLineNumber()
{
	INT o;

	HRESULT	hr = ((Raw_IVBSAXLocator*)P())->get_lineNumber(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IVBSAXLocator));

	return o;
}


CString CVBSAXLocator::GetPublicId()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IVBSAXLocator*)P())->get_publicId(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IVBSAXLocator));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CVBSAXLocator::GetSystemId()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IVBSAXLocator*)P())->get_systemId(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IVBSAXLocator));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}




//--------------------------------------------------------------------
// class CVBSAXAttributes
//--------------------------------------------------------------------

CVBSAXAttributes::CVBSAXAttributes()
:	CInterfaceCallingWrapper<IVBSAXAttributes>()
{}


CVBSAXAttributes::CVBSAXAttributes(IVBSAXAttributes *p, BOOL bIncrementRefCount /*= FALSE*/)
:	CInterfaceCallingWrapper<IVBSAXAttributes>(p, bIncrementRefCount)
{}


CVBSAXAttributes::CVBSAXAttributes(const CVBSAXAttributes &o)
:	CInterfaceCallingWrapper<IVBSAXAttributes>(o)
{}


CVBSAXAttributes::CVBSAXAttributes(IUnknown *pUnknown, BOOL bIncrementRefCount /*= FALSE*/, BOOL bThrowException /*= FALSE*/)
:	CInterfaceCallingWrapper<IVBSAXAttributes>(pUnknown, bIncrementRefCount, bThrowException)
{}


INT CVBSAXAttributes::GetLength()
{
	INT o;

	HRESULT	hr = ((Raw_IVBSAXAttributes*)P())->get_length(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IVBSAXAttributes));

	return o;
}


CString CVBSAXAttributes::GetURI(INT nIndex)
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IVBSAXAttributes*)P())->getURI(nIndex, &bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IVBSAXAttributes));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CVBSAXAttributes::GetLocalName(INT nIndex)
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IVBSAXAttributes*)P())->getLocalName(nIndex, &bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IVBSAXAttributes));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CVBSAXAttributes::GetQName(INT nIndex)
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IVBSAXAttributes*)P())->getQName(nIndex, &bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IVBSAXAttributes));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


INT CVBSAXAttributes::GetIndexFromName(LPCTSTR strURI, LPCTSTR strLocalName)
{
	CString	strstrURI(strURI);
	BSTR	bstrstrURI = strstrURI.AllocSysString();
	CString	strstrLocalName(strLocalName);
	BSTR	bstrstrLocalName = strstrLocalName.AllocSysString();
	INT o;

	HRESULT	hr = ((Raw_IVBSAXAttributes*)P())->getIndexFromName(bstrstrURI, bstrstrLocalName, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IVBSAXAttributes));

	SysFreeString(bstrstrURI);
	SysFreeString(bstrstrLocalName);
	return o;
}


INT CVBSAXAttributes::GetIndexFromQName(LPCTSTR strQName)
{
	CString	strstrQName(strQName);
	BSTR	bstrstrQName = strstrQName.AllocSysString();
	INT o;

	HRESULT	hr = ((Raw_IVBSAXAttributes*)P())->getIndexFromQName(bstrstrQName, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IVBSAXAttributes));

	SysFreeString(bstrstrQName);
	return o;
}


CString CVBSAXAttributes::GetType(INT nIndex)
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IVBSAXAttributes*)P())->getType(nIndex, &bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IVBSAXAttributes));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CVBSAXAttributes::GetTypeFromName(LPCTSTR strURI, LPCTSTR strLocalName)
{
	CString	strstrURI(strURI);
	BSTR	bstrstrURI = strstrURI.AllocSysString();
	CString	strstrLocalName(strLocalName);
	BSTR	bstrstrLocalName = strstrLocalName.AllocSysString();
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IVBSAXAttributes*)P())->getTypeFromName(bstrstrURI, bstrstrLocalName, &bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IVBSAXAttributes));

	SysFreeString(bstrstrURI);
	SysFreeString(bstrstrLocalName);
	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CVBSAXAttributes::GetTypeFromQName(LPCTSTR strQName)
{
	CString	strstrQName(strQName);
	BSTR	bstrstrQName = strstrQName.AllocSysString();
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IVBSAXAttributes*)P())->getTypeFromQName(bstrstrQName, &bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IVBSAXAttributes));

	SysFreeString(bstrstrQName);
	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CVBSAXAttributes::GetValue(INT nIndex)
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IVBSAXAttributes*)P())->getValue(nIndex, &bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IVBSAXAttributes));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CVBSAXAttributes::GetValueFromName(LPCTSTR strURI, LPCTSTR strLocalName)
{
	CString	strstrURI(strURI);
	BSTR	bstrstrURI = strstrURI.AllocSysString();
	CString	strstrLocalName(strLocalName);
	BSTR	bstrstrLocalName = strstrLocalName.AllocSysString();
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IVBSAXAttributes*)P())->getValueFromName(bstrstrURI, bstrstrLocalName, &bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IVBSAXAttributes));

	SysFreeString(bstrstrURI);
	SysFreeString(bstrstrLocalName);
	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CVBSAXAttributes::GetValueFromQName(LPCTSTR strQName)
{
	CString	strstrQName(strQName);
	BSTR	bstrstrQName = strstrQName.AllocSysString();
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IVBSAXAttributes*)P())->getValueFromQName(bstrstrQName, &bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IVBSAXAttributes));

	SysFreeString(bstrstrQName);
	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}




//--------------------------------------------------------------------
// class CVBSAXXMLFilter
//--------------------------------------------------------------------

CVBSAXXMLFilter::CVBSAXXMLFilter()
:	CInterfaceCallingWrapper<IVBSAXXMLFilter>()
{}


CVBSAXXMLFilter::CVBSAXXMLFilter(IVBSAXXMLFilter *p, BOOL bIncrementRefCount /*= FALSE*/)
:	CInterfaceCallingWrapper<IVBSAXXMLFilter>(p, bIncrementRefCount)
{}


CVBSAXXMLFilter::CVBSAXXMLFilter(const CVBSAXXMLFilter &o)
:	CInterfaceCallingWrapper<IVBSAXXMLFilter>(o)
{}


CVBSAXXMLFilter::CVBSAXXMLFilter(IUnknown *pUnknown, BOOL bIncrementRefCount /*= FALSE*/, BOOL bThrowException /*= FALSE*/)
:	CInterfaceCallingWrapper<IVBSAXXMLFilter>(pUnknown, bIncrementRefCount, bThrowException)
{}


CVBSAXXMLReader CVBSAXXMLFilter::GetParent()
{
	IVBSAXXMLReader* o;

	HRESULT	hr = ((Raw_IVBSAXXMLFilter*)P())->get_parent(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IVBSAXXMLFilter));

	return o;
}


void CVBSAXXMLFilter::SetRefParent(IVBSAXXMLReader* rhs)
{

	HRESULT	hr = ((Raw_IVBSAXXMLFilter*)P())->putref_parent(rhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IVBSAXXMLFilter));

}




//--------------------------------------------------------------------
// class CSAXXMLReaderClass
//--------------------------------------------------------------------

const CLSID CSAXXMLReaderClass::s_ClsId = {0x079aa557, 0x4a18, 0x424a, {0x8e, 0xee, 0xe3, 0x9f, 0x0a, 0x8d, 0x41, 0xb9}};


CSAXXMLReaderClass::CSAXXMLReaderClass()
:	CCoClassWrapper<IVBSAXXMLReader>(s_ClsId)
{}


CVBSAXXMLReader CSAXXMLReaderClass::GetVBSAXXMLReader()
{
	IVBSAXXMLReader *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


ISAXXMLReader* CSAXXMLReaderClass::GetSAXXMLReader()
{
	ISAXXMLReader *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


IMXReaderControl* CSAXXMLReaderClass::GetMXReaderControl()
{
	IMXReaderControl *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}




//--------------------------------------------------------------------
// class CSAXXMLReader30Class
//--------------------------------------------------------------------

const CLSID CSAXXMLReader30Class::s_ClsId = {0x3124c396, 0xfb13, 0x4836, {0xa6, 0xad, 0x13, 0x17, 0xf1, 0x71, 0x36, 0x88}};


CSAXXMLReader30Class::CSAXXMLReader30Class()
:	CCoClassWrapper<IVBSAXXMLReader>(s_ClsId)
{}


CVBSAXXMLReader CSAXXMLReader30Class::GetVBSAXXMLReader()
{
	IVBSAXXMLReader *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


ISAXXMLReader* CSAXXMLReader30Class::GetSAXXMLReader()
{
	ISAXXMLReader *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


IMXReaderControl* CSAXXMLReader30Class::GetMXReaderControl()
{
	IMXReaderControl *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}




//--------------------------------------------------------------------
// class CSAXXMLReader40Class
//--------------------------------------------------------------------

const CLSID CSAXXMLReader40Class::s_ClsId = {0x7c6e29bc, 0x8b8b, 0x4c3d, {0x85, 0x9e, 0xaf, 0x6c, 0xd1, 0x58, 0xbe, 0x0f}};


CSAXXMLReader40Class::CSAXXMLReader40Class()
:	CCoClassWrapper<IVBSAXXMLReader>(s_ClsId)
{}


CVBSAXXMLReader CSAXXMLReader40Class::GetVBSAXXMLReader()
{
	IVBSAXXMLReader *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


ISAXXMLReader* CSAXXMLReader40Class::GetSAXXMLReader()
{
	ISAXXMLReader *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}




//--------------------------------------------------------------------
// class CMXXMLWriterClass
//--------------------------------------------------------------------

const CLSID CMXXMLWriterClass::s_ClsId = {0xfc220ad8, 0xa72a, 0x4ee8, {0x92, 0x6e, 0x0b, 0x7a, 0xd1, 0x52, 0xa0, 0x20}};


CMXXMLWriterClass::CMXXMLWriterClass()
:	CCoClassWrapper<IMXWriter>(s_ClsId)
{}


CMXWriter CMXXMLWriterClass::GetMXWriter()
{
	IMXWriter *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


ISAXContentHandler* CMXXMLWriterClass::GetSAXContentHandler()
{
	ISAXContentHandler *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


ISAXErrorHandler* CMXXMLWriterClass::GetSAXErrorHandler()
{
	ISAXErrorHandler *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


ISAXDTDHandler* CMXXMLWriterClass::GetSAXDTDHandler()
{
	ISAXDTDHandler *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


ISAXLexicalHandler* CMXXMLWriterClass::GetSAXLexicalHandler()
{
	ISAXLexicalHandler *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


ISAXDeclHandler* CMXXMLWriterClass::GetSAXDeclHandler()
{
	ISAXDeclHandler *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


IVBSAXContentHandler* CMXXMLWriterClass::GetVBSAXContentHandler()
{
	IVBSAXContentHandler *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


IVBSAXDeclHandler* CMXXMLWriterClass::GetVBSAXDeclHandler()
{
	IVBSAXDeclHandler *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


IVBSAXDTDHandler* CMXXMLWriterClass::GetVBSAXDTDHandler()
{
	IVBSAXDTDHandler *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


IVBSAXErrorHandler* CMXXMLWriterClass::GetVBSAXErrorHandler()
{
	IVBSAXErrorHandler *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


IVBSAXLexicalHandler* CMXXMLWriterClass::GetVBSAXLexicalHandler()
{
	IVBSAXLexicalHandler *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}




//--------------------------------------------------------------------
// class CMXXMLWriter30Class
//--------------------------------------------------------------------

const CLSID CMXXMLWriter30Class::s_ClsId = {0x3d813dfe, 0x6c91, 0x4a4e, {0x8f, 0x41, 0x04, 0x34, 0x6a, 0x84, 0x1d, 0x9c}};


CMXXMLWriter30Class::CMXXMLWriter30Class()
:	CCoClassWrapper<IMXWriter>(s_ClsId)
{}


CMXWriter CMXXMLWriter30Class::GetMXWriter()
{
	IMXWriter *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


ISAXContentHandler* CMXXMLWriter30Class::GetSAXContentHandler()
{
	ISAXContentHandler *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


ISAXDeclHandler* CMXXMLWriter30Class::GetSAXDeclHandler()
{
	ISAXDeclHandler *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


ISAXDTDHandler* CMXXMLWriter30Class::GetSAXDTDHandler()
{
	ISAXDTDHandler *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


ISAXErrorHandler* CMXXMLWriter30Class::GetSAXErrorHandler()
{
	ISAXErrorHandler *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


ISAXLexicalHandler* CMXXMLWriter30Class::GetSAXLexicalHandler()
{
	ISAXLexicalHandler *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


IVBSAXContentHandler* CMXXMLWriter30Class::GetVBSAXContentHandler()
{
	IVBSAXContentHandler *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


IVBSAXDeclHandler* CMXXMLWriter30Class::GetVBSAXDeclHandler()
{
	IVBSAXDeclHandler *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


IVBSAXDTDHandler* CMXXMLWriter30Class::GetVBSAXDTDHandler()
{
	IVBSAXDTDHandler *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


IVBSAXErrorHandler* CMXXMLWriter30Class::GetVBSAXErrorHandler()
{
	IVBSAXErrorHandler *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


IVBSAXLexicalHandler* CMXXMLWriter30Class::GetVBSAXLexicalHandler()
{
	IVBSAXLexicalHandler *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}




//--------------------------------------------------------------------
// class CMXXMLWriter40Class
//--------------------------------------------------------------------

const CLSID CMXXMLWriter40Class::s_ClsId = {0x88d969c8, 0xf192, 0x11d4, {0xa6, 0x5f, 0x00, 0x40, 0x96, 0x32, 0x51, 0xe5}};


CMXXMLWriter40Class::CMXXMLWriter40Class()
:	CCoClassWrapper<IMXWriter>(s_ClsId)
{}


CMXWriter CMXXMLWriter40Class::GetMXWriter()
{
	IMXWriter *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


ISAXContentHandler* CMXXMLWriter40Class::GetSAXContentHandler()
{
	ISAXContentHandler *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


ISAXDeclHandler* CMXXMLWriter40Class::GetSAXDeclHandler()
{
	ISAXDeclHandler *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


ISAXDTDHandler* CMXXMLWriter40Class::GetSAXDTDHandler()
{
	ISAXDTDHandler *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


ISAXErrorHandler* CMXXMLWriter40Class::GetSAXErrorHandler()
{
	ISAXErrorHandler *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


ISAXLexicalHandler* CMXXMLWriter40Class::GetSAXLexicalHandler()
{
	ISAXLexicalHandler *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


IVBSAXContentHandler* CMXXMLWriter40Class::GetVBSAXContentHandler()
{
	IVBSAXContentHandler *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


IVBSAXDeclHandler* CMXXMLWriter40Class::GetVBSAXDeclHandler()
{
	IVBSAXDeclHandler *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


IVBSAXDTDHandler* CMXXMLWriter40Class::GetVBSAXDTDHandler()
{
	IVBSAXDTDHandler *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


IVBSAXErrorHandler* CMXXMLWriter40Class::GetVBSAXErrorHandler()
{
	IVBSAXErrorHandler *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


IVBSAXLexicalHandler* CMXXMLWriter40Class::GetVBSAXLexicalHandler()
{
	IVBSAXLexicalHandler *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}




//--------------------------------------------------------------------
// class CSAXAttributesClass
//--------------------------------------------------------------------

const CLSID CSAXAttributesClass::s_ClsId = {0x4dd441ad, 0x526d, 0x4a77, {0x9f, 0x1b, 0x98, 0x41, 0xed, 0x80, 0x2f, 0xb0}};


CSAXAttributesClass::CSAXAttributesClass()
:	CCoClassWrapper<IMXAttributes>(s_ClsId)
{}


CMXAttributes CSAXAttributesClass::GetMXAttributes()
{
	IMXAttributes *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


CVBSAXAttributes CSAXAttributesClass::GetVBSAXAttributes()
{
	IVBSAXAttributes *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


ISAXAttributes* CSAXAttributesClass::GetSAXAttributes()
{
	ISAXAttributes *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}




//--------------------------------------------------------------------
// class CSAXAttributes30Class
//--------------------------------------------------------------------

const CLSID CSAXAttributes30Class::s_ClsId = {0x3e784a01, 0xf3ae, 0x4dc0, {0x93, 0x54, 0x95, 0x26, 0xb9, 0x37, 0x0e, 0xba}};


CSAXAttributes30Class::CSAXAttributes30Class()
:	CCoClassWrapper<IMXAttributes>(s_ClsId)
{}


CMXAttributes CSAXAttributes30Class::GetMXAttributes()
{
	IMXAttributes *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


CVBSAXAttributes CSAXAttributes30Class::GetVBSAXAttributes()
{
	IVBSAXAttributes *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


ISAXAttributes* CSAXAttributes30Class::GetSAXAttributes()
{
	ISAXAttributes *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}




//--------------------------------------------------------------------
// class CSAXAttributes40Class
//--------------------------------------------------------------------

const CLSID CSAXAttributes40Class::s_ClsId = {0x88d969ca, 0xf192, 0x11d4, {0xa6, 0x5f, 0x00, 0x40, 0x96, 0x32, 0x51, 0xe5}};


CSAXAttributes40Class::CSAXAttributes40Class()
:	CCoClassWrapper<IMXAttributes>(s_ClsId)
{}


CMXAttributes CSAXAttributes40Class::GetMXAttributes()
{
	IMXAttributes *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


CVBSAXAttributes CSAXAttributes40Class::GetVBSAXAttributes()
{
	IVBSAXAttributes *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


ISAXAttributes* CSAXAttributes40Class::GetSAXAttributes()
{
	ISAXAttributes *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}




//--------------------------------------------------------------------
// class CVBSAXContentHandlerImpl
//--------------------------------------------------------------------

CVBSAXContentHandlerImpl::CVBSAXContentHandlerImpl(AFX_MODULE_STATE *pModuleState, BOOL bNoDelete /*= FALSE*/)
: CDispatchInterfaceImplementationWrapper<IVBSAXContentHandler>(pModuleState, LIBID, LIBVER_MAJOR, LIBVER_MINOR, bNoDelete)
{}


CVBSAXContentHandlerImpl::CVBSAXContentHandlerImpl(GET_MODULE_STATE_FUNCTION pGetModuleStateFunction, BOOL bNoDelete /*= FALSE*/)
: CDispatchInterfaceImplementationWrapper<IVBSAXContentHandler>(pGetModuleStateFunction, LIBID, LIBVER_MAJOR, LIBVER_MINOR, bNoDelete)
{}


CVBSAXContentHandlerImpl::CVBSAXContentHandlerImpl(AFX_MODULE_STATE *pModuleState, IUnknown *pOwner)
: CDispatchInterfaceImplementationWrapper<IVBSAXContentHandler>(pModuleState, LIBID, LIBVER_MAJOR, LIBVER_MINOR, pOwner)
{}


CVBSAXContentHandlerImpl::CVBSAXContentHandlerImpl(GET_MODULE_STATE_FUNCTION pGetModuleStateFunction, IUnknown *pOwner)
: CDispatchInterfaceImplementationWrapper<IVBSAXContentHandler>(pGetModuleStateFunction, LIBID, LIBVER_MAJOR, LIBVER_MINOR, pOwner)
{}


IVBSAXContentHandler* CVBSAXContentHandlerImpl::GetInterface()
{
	return reinterpret_cast<IVBSAXContentHandler*>(&m_xVBSAXContentHandler);
}


IUnknown* CVBSAXContentHandlerImpl::GetInterfaceById(REFIID riid)
{
	return CDispatchInterfaceImplementationWrapper<IVBSAXContentHandler>::GetInterfaceById(riid);
}


STDMETHODIMP CVBSAXContentHandlerImpl::XVBSAXContentHandler::QueryInterface(REFIID riid, void **ppv)
{
	GETTHIS(CVBSAXContentHandlerImpl, m_xVBSAXContentHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());
	return pThis->QueryInterface(riid, ppv);
}


STDMETHODIMP_(ULONG) CVBSAXContentHandlerImpl::XVBSAXContentHandler::AddRef()
{
	GETTHIS(CVBSAXContentHandlerImpl, m_xVBSAXContentHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());
	return pThis->AddRef();
}


STDMETHODIMP_(ULONG) CVBSAXContentHandlerImpl::XVBSAXContentHandler::Release()
{
	GETTHIS(CVBSAXContentHandlerImpl, m_xVBSAXContentHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());
	return pThis->Release();
}


STDMETHODIMP CVBSAXContentHandlerImpl::XVBSAXContentHandler::GetTypeInfoCount(UINT *pctinfo)
{
	GETTHIS(CVBSAXContentHandlerImpl, m_xVBSAXContentHandler);
	return pThis->GetTypeInfoCount(pctinfo);
}


STDMETHODIMP CVBSAXContentHandlerImpl::XVBSAXContentHandler::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo)
{
	GETTHIS(CVBSAXContentHandlerImpl, m_xVBSAXContentHandler);
	return pThis->GetTypeInfo(iTInfo, lcid, ppTInfo);
}


STDMETHODIMP CVBSAXContentHandlerImpl::XVBSAXContentHandler::GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgid)
{
	GETTHIS(CVBSAXContentHandlerImpl, m_xVBSAXContentHandler);
	return pThis->GetIDsOfNames(riid, rgszNames, cNames, lcid, rgid);
}


STDMETHODIMP CVBSAXContentHandlerImpl::XVBSAXContentHandler::Invoke(DISPID id, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)
{
	GETTHIS(CVBSAXContentHandlerImpl, m_xVBSAXContentHandler);
	return pThis->Invoke(id, riid, lcid, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);
}


STDMETHODIMP CVBSAXContentHandlerImpl::XVBSAXContentHandler::putref_documentLocator(IVBSAXLocator* rhs)
{
	GETTHIS(CVBSAXContentHandlerImpl, m_xVBSAXContentHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());

	// implementation wrapper call

	try
	{
		pThis->SetRefDocumentLocator(CVBSAXLocator(rhs, TRUE));
		return S_OK;
	}
	catch (CComException *pE)
	{
		HRESULT	hr = pE->GetResult();
		pE->Delete();
		return hr;
	}

}


STDMETHODIMP CVBSAXContentHandlerImpl::XVBSAXContentHandler::startDocument()
{
	GETTHIS(CVBSAXContentHandlerImpl, m_xVBSAXContentHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());

	// implementation wrapper call

	try
	{
		pThis->StartDocument();
		return S_OK;
	}
	catch (CComException *pE)
	{
		HRESULT	hr = pE->GetResult();
		pE->Delete();
		return hr;
	}

}


STDMETHODIMP CVBSAXContentHandlerImpl::XVBSAXContentHandler::endDocument()
{
	GETTHIS(CVBSAXContentHandlerImpl, m_xVBSAXContentHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());

	// implementation wrapper call

	try
	{
		pThis->EndDocument();
		return S_OK;
	}
	catch (CComException *pE)
	{
		HRESULT	hr = pE->GetResult();
		pE->Delete();
		return hr;
	}

}


STDMETHODIMP CVBSAXContentHandlerImpl::XVBSAXContentHandler::startPrefixMapping(BSTR* strPrefix, BSTR* strURI)
{
	GETTHIS(CVBSAXContentHandlerImpl, m_xVBSAXContentHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());

	// implementation wrapper call
	CString	strstrPrefix((LPCTSTR)_bstr_t(*strPrefix, true));
	CString	strstrURI((LPCTSTR)_bstr_t(*strURI, true));

	try
	{
		pThis->StartPrefixMapping(strstrPrefix, strstrURI);
		*strPrefix = strstrPrefix.AllocSysString();
		*strURI = strstrURI.AllocSysString();
		return S_OK;
	}
	catch (CComException *pE)
	{
		HRESULT	hr = pE->GetResult();
		pE->Delete();
		*strPrefix = strstrPrefix.AllocSysString();
		*strURI = strstrURI.AllocSysString();
		return hr;
	}

}


STDMETHODIMP CVBSAXContentHandlerImpl::XVBSAXContentHandler::endPrefixMapping(BSTR* strPrefix)
{
	GETTHIS(CVBSAXContentHandlerImpl, m_xVBSAXContentHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());

	// implementation wrapper call
	CString	strstrPrefix((LPCTSTR)_bstr_t(*strPrefix, true));

	try
	{
		pThis->EndPrefixMapping(strstrPrefix);
		*strPrefix = strstrPrefix.AllocSysString();
		return S_OK;
	}
	catch (CComException *pE)
	{
		HRESULT	hr = pE->GetResult();
		pE->Delete();
		*strPrefix = strstrPrefix.AllocSysString();
		return hr;
	}

}


STDMETHODIMP CVBSAXContentHandlerImpl::XVBSAXContentHandler::startElement(BSTR* strNamespaceURI, BSTR* strLocalName, BSTR* strQName, IVBSAXAttributes* oAttributes)
{
	GETTHIS(CVBSAXContentHandlerImpl, m_xVBSAXContentHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());

	// implementation wrapper call
	CString	strstrNamespaceURI((LPCTSTR)_bstr_t(*strNamespaceURI, true));
	CString	strstrLocalName((LPCTSTR)_bstr_t(*strLocalName, true));
	CString	strstrQName((LPCTSTR)_bstr_t(*strQName, true));

	try
	{
		pThis->StartElement(strstrNamespaceURI, strstrLocalName, strstrQName, CVBSAXAttributes(oAttributes, TRUE));
		*strNamespaceURI = strstrNamespaceURI.AllocSysString();
		*strLocalName = strstrLocalName.AllocSysString();
		*strQName = strstrQName.AllocSysString();
		return S_OK;
	}
	catch (CComException *pE)
	{
		HRESULT	hr = pE->GetResult();
		pE->Delete();
		*strNamespaceURI = strstrNamespaceURI.AllocSysString();
		*strLocalName = strstrLocalName.AllocSysString();
		*strQName = strstrQName.AllocSysString();
		return hr;
	}

}


STDMETHODIMP CVBSAXContentHandlerImpl::XVBSAXContentHandler::endElement(BSTR* strNamespaceURI, BSTR* strLocalName, BSTR* strQName)
{
	GETTHIS(CVBSAXContentHandlerImpl, m_xVBSAXContentHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());

	// implementation wrapper call
	CString	strstrNamespaceURI((LPCTSTR)_bstr_t(*strNamespaceURI, true));
	CString	strstrLocalName((LPCTSTR)_bstr_t(*strLocalName, true));
	CString	strstrQName((LPCTSTR)_bstr_t(*strQName, true));

	try
	{
		pThis->EndElement(strstrNamespaceURI, strstrLocalName, strstrQName);
		*strNamespaceURI = strstrNamespaceURI.AllocSysString();
		*strLocalName = strstrLocalName.AllocSysString();
		*strQName = strstrQName.AllocSysString();
		return S_OK;
	}
	catch (CComException *pE)
	{
		HRESULT	hr = pE->GetResult();
		pE->Delete();
		*strNamespaceURI = strstrNamespaceURI.AllocSysString();
		*strLocalName = strstrLocalName.AllocSysString();
		*strQName = strstrQName.AllocSysString();
		return hr;
	}

}


STDMETHODIMP CVBSAXContentHandlerImpl::XVBSAXContentHandler::characters(BSTR* strChars)
{
	GETTHIS(CVBSAXContentHandlerImpl, m_xVBSAXContentHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());

	// implementation wrapper call
	CString	strstrChars((LPCTSTR)_bstr_t(*strChars, true));

	try
	{
		pThis->Characters(strstrChars);
		*strChars = strstrChars.AllocSysString();
		return S_OK;
	}
	catch (CComException *pE)
	{
		HRESULT	hr = pE->GetResult();
		pE->Delete();
		*strChars = strstrChars.AllocSysString();
		return hr;
	}

}


STDMETHODIMP CVBSAXContentHandlerImpl::XVBSAXContentHandler::ignorableWhitespace(BSTR* strChars)
{
	GETTHIS(CVBSAXContentHandlerImpl, m_xVBSAXContentHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());

	// implementation wrapper call
	CString	strstrChars((LPCTSTR)_bstr_t(*strChars, true));

	try
	{
		pThis->IgnorableWhitespace(strstrChars);
		*strChars = strstrChars.AllocSysString();
		return S_OK;
	}
	catch (CComException *pE)
	{
		HRESULT	hr = pE->GetResult();
		pE->Delete();
		*strChars = strstrChars.AllocSysString();
		return hr;
	}

}


STDMETHODIMP CVBSAXContentHandlerImpl::XVBSAXContentHandler::processingInstruction(BSTR* strTarget, BSTR* strData)
{
	GETTHIS(CVBSAXContentHandlerImpl, m_xVBSAXContentHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());

	// implementation wrapper call
	CString	strstrTarget((LPCTSTR)_bstr_t(*strTarget, true));
	CString	strstrData((LPCTSTR)_bstr_t(*strData, true));

	try
	{
		pThis->ProcessingInstruction(strstrTarget, strstrData);
		*strTarget = strstrTarget.AllocSysString();
		*strData = strstrData.AllocSysString();
		return S_OK;
	}
	catch (CComException *pE)
	{
		HRESULT	hr = pE->GetResult();
		pE->Delete();
		*strTarget = strstrTarget.AllocSysString();
		*strData = strstrData.AllocSysString();
		return hr;
	}

}


STDMETHODIMP CVBSAXContentHandlerImpl::XVBSAXContentHandler::skippedEntity(BSTR* strName)
{
	GETTHIS(CVBSAXContentHandlerImpl, m_xVBSAXContentHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());

	// implementation wrapper call
	CString	strstrName((LPCTSTR)_bstr_t(*strName, true));

	try
	{
		pThis->SkippedEntity(strstrName);
		*strName = strstrName.AllocSysString();
		return S_OK;
	}
	catch (CComException *pE)
	{
		HRESULT	hr = pE->GetResult();
		pE->Delete();
		*strName = strstrName.AllocSysString();
		return hr;
	}

}




//--------------------------------------------------------------------
// class CVBSAXDeclHandlerImpl
//--------------------------------------------------------------------

CVBSAXDeclHandlerImpl::CVBSAXDeclHandlerImpl(AFX_MODULE_STATE *pModuleState, BOOL bNoDelete /*= FALSE*/)
: CDispatchInterfaceImplementationWrapper<IVBSAXDeclHandler>(pModuleState, LIBID, LIBVER_MAJOR, LIBVER_MINOR, bNoDelete)
{}


CVBSAXDeclHandlerImpl::CVBSAXDeclHandlerImpl(GET_MODULE_STATE_FUNCTION pGetModuleStateFunction, BOOL bNoDelete /*= FALSE*/)
: CDispatchInterfaceImplementationWrapper<IVBSAXDeclHandler>(pGetModuleStateFunction, LIBID, LIBVER_MAJOR, LIBVER_MINOR, bNoDelete)
{}


CVBSAXDeclHandlerImpl::CVBSAXDeclHandlerImpl(AFX_MODULE_STATE *pModuleState, IUnknown *pOwner)
: CDispatchInterfaceImplementationWrapper<IVBSAXDeclHandler>(pModuleState, LIBID, LIBVER_MAJOR, LIBVER_MINOR, pOwner)
{}


CVBSAXDeclHandlerImpl::CVBSAXDeclHandlerImpl(GET_MODULE_STATE_FUNCTION pGetModuleStateFunction, IUnknown *pOwner)
: CDispatchInterfaceImplementationWrapper<IVBSAXDeclHandler>(pGetModuleStateFunction, LIBID, LIBVER_MAJOR, LIBVER_MINOR, pOwner)
{}


IVBSAXDeclHandler* CVBSAXDeclHandlerImpl::GetInterface()
{
	return reinterpret_cast<IVBSAXDeclHandler*>(&m_xVBSAXDeclHandler);
}


IUnknown* CVBSAXDeclHandlerImpl::GetInterfaceById(REFIID riid)
{
	return CDispatchInterfaceImplementationWrapper<IVBSAXDeclHandler>::GetInterfaceById(riid);
}


STDMETHODIMP CVBSAXDeclHandlerImpl::XVBSAXDeclHandler::QueryInterface(REFIID riid, void **ppv)
{
	GETTHIS(CVBSAXDeclHandlerImpl, m_xVBSAXDeclHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());
	return pThis->QueryInterface(riid, ppv);
}


STDMETHODIMP_(ULONG) CVBSAXDeclHandlerImpl::XVBSAXDeclHandler::AddRef()
{
	GETTHIS(CVBSAXDeclHandlerImpl, m_xVBSAXDeclHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());
	return pThis->AddRef();
}


STDMETHODIMP_(ULONG) CVBSAXDeclHandlerImpl::XVBSAXDeclHandler::Release()
{
	GETTHIS(CVBSAXDeclHandlerImpl, m_xVBSAXDeclHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());
	return pThis->Release();
}


STDMETHODIMP CVBSAXDeclHandlerImpl::XVBSAXDeclHandler::GetTypeInfoCount(UINT *pctinfo)
{
	GETTHIS(CVBSAXDeclHandlerImpl, m_xVBSAXDeclHandler);
	return pThis->GetTypeInfoCount(pctinfo);
}


STDMETHODIMP CVBSAXDeclHandlerImpl::XVBSAXDeclHandler::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo)
{
	GETTHIS(CVBSAXDeclHandlerImpl, m_xVBSAXDeclHandler);
	return pThis->GetTypeInfo(iTInfo, lcid, ppTInfo);
}


STDMETHODIMP CVBSAXDeclHandlerImpl::XVBSAXDeclHandler::GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgid)
{
	GETTHIS(CVBSAXDeclHandlerImpl, m_xVBSAXDeclHandler);
	return pThis->GetIDsOfNames(riid, rgszNames, cNames, lcid, rgid);
}


STDMETHODIMP CVBSAXDeclHandlerImpl::XVBSAXDeclHandler::Invoke(DISPID id, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)
{
	GETTHIS(CVBSAXDeclHandlerImpl, m_xVBSAXDeclHandler);
	return pThis->Invoke(id, riid, lcid, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);
}


STDMETHODIMP CVBSAXDeclHandlerImpl::XVBSAXDeclHandler::elementDecl(BSTR* strName, BSTR* strModel)
{
	GETTHIS(CVBSAXDeclHandlerImpl, m_xVBSAXDeclHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());

	// implementation wrapper call
	CString	strstrName((LPCTSTR)_bstr_t(*strName, true));
	CString	strstrModel((LPCTSTR)_bstr_t(*strModel, true));

	try
	{
		pThis->ElementDecl(strstrName, strstrModel);
		*strName = strstrName.AllocSysString();
		*strModel = strstrModel.AllocSysString();
		return S_OK;
	}
	catch (CComException *pE)
	{
		HRESULT	hr = pE->GetResult();
		pE->Delete();
		*strName = strstrName.AllocSysString();
		*strModel = strstrModel.AllocSysString();
		return hr;
	}

}


STDMETHODIMP CVBSAXDeclHandlerImpl::XVBSAXDeclHandler::attributeDecl(BSTR* strElementName, BSTR* strAttributeName, BSTR* strType, BSTR* strValueDefault, BSTR* strValue)
{
	GETTHIS(CVBSAXDeclHandlerImpl, m_xVBSAXDeclHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());

	// implementation wrapper call
	CString	strstrElementName((LPCTSTR)_bstr_t(*strElementName, true));
	CString	strstrAttributeName((LPCTSTR)_bstr_t(*strAttributeName, true));
	CString	strstrType((LPCTSTR)_bstr_t(*strType, true));
	CString	strstrValueDefault((LPCTSTR)_bstr_t(*strValueDefault, true));
	CString	strstrValue((LPCTSTR)_bstr_t(*strValue, true));

	try
	{
		pThis->AttributeDecl(strstrElementName, strstrAttributeName, strstrType, strstrValueDefault, strstrValue);
		*strElementName = strstrElementName.AllocSysString();
		*strAttributeName = strstrAttributeName.AllocSysString();
		*strType = strstrType.AllocSysString();
		*strValueDefault = strstrValueDefault.AllocSysString();
		*strValue = strstrValue.AllocSysString();
		return S_OK;
	}
	catch (CComException *pE)
	{
		HRESULT	hr = pE->GetResult();
		pE->Delete();
		*strElementName = strstrElementName.AllocSysString();
		*strAttributeName = strstrAttributeName.AllocSysString();
		*strType = strstrType.AllocSysString();
		*strValueDefault = strstrValueDefault.AllocSysString();
		*strValue = strstrValue.AllocSysString();
		return hr;
	}

}


STDMETHODIMP CVBSAXDeclHandlerImpl::XVBSAXDeclHandler::internalEntityDecl(BSTR* strName, BSTR* strValue)
{
	GETTHIS(CVBSAXDeclHandlerImpl, m_xVBSAXDeclHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());

	// implementation wrapper call
	CString	strstrName((LPCTSTR)_bstr_t(*strName, true));
	CString	strstrValue((LPCTSTR)_bstr_t(*strValue, true));

	try
	{
		pThis->InternalEntityDecl(strstrName, strstrValue);
		*strName = strstrName.AllocSysString();
		*strValue = strstrValue.AllocSysString();
		return S_OK;
	}
	catch (CComException *pE)
	{
		HRESULT	hr = pE->GetResult();
		pE->Delete();
		*strName = strstrName.AllocSysString();
		*strValue = strstrValue.AllocSysString();
		return hr;
	}

}


STDMETHODIMP CVBSAXDeclHandlerImpl::XVBSAXDeclHandler::externalEntityDecl(BSTR* strName, BSTR* strPublicId, BSTR* strSystemId)
{
	GETTHIS(CVBSAXDeclHandlerImpl, m_xVBSAXDeclHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());

	// implementation wrapper call
	CString	strstrName((LPCTSTR)_bstr_t(*strName, true));
	CString	strstrPublicId((LPCTSTR)_bstr_t(*strPublicId, true));
	CString	strstrSystemId((LPCTSTR)_bstr_t(*strSystemId, true));

	try
	{
		pThis->ExternalEntityDecl(strstrName, strstrPublicId, strstrSystemId);
		*strName = strstrName.AllocSysString();
		*strPublicId = strstrPublicId.AllocSysString();
		*strSystemId = strstrSystemId.AllocSysString();
		return S_OK;
	}
	catch (CComException *pE)
	{
		HRESULT	hr = pE->GetResult();
		pE->Delete();
		*strName = strstrName.AllocSysString();
		*strPublicId = strstrPublicId.AllocSysString();
		*strSystemId = strstrSystemId.AllocSysString();
		return hr;
	}

}




//--------------------------------------------------------------------
// class CVBSAXDTDHandlerImpl
//--------------------------------------------------------------------

CVBSAXDTDHandlerImpl::CVBSAXDTDHandlerImpl(AFX_MODULE_STATE *pModuleState, BOOL bNoDelete /*= FALSE*/)
: CDispatchInterfaceImplementationWrapper<IVBSAXDTDHandler>(pModuleState, LIBID, LIBVER_MAJOR, LIBVER_MINOR, bNoDelete)
{}


CVBSAXDTDHandlerImpl::CVBSAXDTDHandlerImpl(GET_MODULE_STATE_FUNCTION pGetModuleStateFunction, BOOL bNoDelete /*= FALSE*/)
: CDispatchInterfaceImplementationWrapper<IVBSAXDTDHandler>(pGetModuleStateFunction, LIBID, LIBVER_MAJOR, LIBVER_MINOR, bNoDelete)
{}


CVBSAXDTDHandlerImpl::CVBSAXDTDHandlerImpl(AFX_MODULE_STATE *pModuleState, IUnknown *pOwner)
: CDispatchInterfaceImplementationWrapper<IVBSAXDTDHandler>(pModuleState, LIBID, LIBVER_MAJOR, LIBVER_MINOR, pOwner)
{}


CVBSAXDTDHandlerImpl::CVBSAXDTDHandlerImpl(GET_MODULE_STATE_FUNCTION pGetModuleStateFunction, IUnknown *pOwner)
: CDispatchInterfaceImplementationWrapper<IVBSAXDTDHandler>(pGetModuleStateFunction, LIBID, LIBVER_MAJOR, LIBVER_MINOR, pOwner)
{}


IVBSAXDTDHandler* CVBSAXDTDHandlerImpl::GetInterface()
{
	return reinterpret_cast<IVBSAXDTDHandler*>(&m_xVBSAXDTDHandler);
}


IUnknown* CVBSAXDTDHandlerImpl::GetInterfaceById(REFIID riid)
{
	return CDispatchInterfaceImplementationWrapper<IVBSAXDTDHandler>::GetInterfaceById(riid);
}


STDMETHODIMP CVBSAXDTDHandlerImpl::XVBSAXDTDHandler::QueryInterface(REFIID riid, void **ppv)
{
	GETTHIS(CVBSAXDTDHandlerImpl, m_xVBSAXDTDHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());
	return pThis->QueryInterface(riid, ppv);
}


STDMETHODIMP_(ULONG) CVBSAXDTDHandlerImpl::XVBSAXDTDHandler::AddRef()
{
	GETTHIS(CVBSAXDTDHandlerImpl, m_xVBSAXDTDHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());
	return pThis->AddRef();
}


STDMETHODIMP_(ULONG) CVBSAXDTDHandlerImpl::XVBSAXDTDHandler::Release()
{
	GETTHIS(CVBSAXDTDHandlerImpl, m_xVBSAXDTDHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());
	return pThis->Release();
}


STDMETHODIMP CVBSAXDTDHandlerImpl::XVBSAXDTDHandler::GetTypeInfoCount(UINT *pctinfo)
{
	GETTHIS(CVBSAXDTDHandlerImpl, m_xVBSAXDTDHandler);
	return pThis->GetTypeInfoCount(pctinfo);
}


STDMETHODIMP CVBSAXDTDHandlerImpl::XVBSAXDTDHandler::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo)
{
	GETTHIS(CVBSAXDTDHandlerImpl, m_xVBSAXDTDHandler);
	return pThis->GetTypeInfo(iTInfo, lcid, ppTInfo);
}


STDMETHODIMP CVBSAXDTDHandlerImpl::XVBSAXDTDHandler::GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgid)
{
	GETTHIS(CVBSAXDTDHandlerImpl, m_xVBSAXDTDHandler);
	return pThis->GetIDsOfNames(riid, rgszNames, cNames, lcid, rgid);
}


STDMETHODIMP CVBSAXDTDHandlerImpl::XVBSAXDTDHandler::Invoke(DISPID id, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)
{
	GETTHIS(CVBSAXDTDHandlerImpl, m_xVBSAXDTDHandler);
	return pThis->Invoke(id, riid, lcid, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);
}


STDMETHODIMP CVBSAXDTDHandlerImpl::XVBSAXDTDHandler::notationDecl(BSTR* strName, BSTR* strPublicId, BSTR* strSystemId)
{
	GETTHIS(CVBSAXDTDHandlerImpl, m_xVBSAXDTDHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());

	// implementation wrapper call
	CString	strstrName((LPCTSTR)_bstr_t(*strName, true));
	CString	strstrPublicId((LPCTSTR)_bstr_t(*strPublicId, true));
	CString	strstrSystemId((LPCTSTR)_bstr_t(*strSystemId, true));

	try
	{
		pThis->NotationDecl(strstrName, strstrPublicId, strstrSystemId);
		*strName = strstrName.AllocSysString();
		*strPublicId = strstrPublicId.AllocSysString();
		*strSystemId = strstrSystemId.AllocSysString();
		return S_OK;
	}
	catch (CComException *pE)
	{
		HRESULT	hr = pE->GetResult();
		pE->Delete();
		*strName = strstrName.AllocSysString();
		*strPublicId = strstrPublicId.AllocSysString();
		*strSystemId = strstrSystemId.AllocSysString();
		return hr;
	}

}


STDMETHODIMP CVBSAXDTDHandlerImpl::XVBSAXDTDHandler::unparsedEntityDecl(BSTR* strName, BSTR* strPublicId, BSTR* strSystemId, BSTR* strNotationName)
{
	GETTHIS(CVBSAXDTDHandlerImpl, m_xVBSAXDTDHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());

	// implementation wrapper call
	CString	strstrName((LPCTSTR)_bstr_t(*strName, true));
	CString	strstrPublicId((LPCTSTR)_bstr_t(*strPublicId, true));
	CString	strstrSystemId((LPCTSTR)_bstr_t(*strSystemId, true));
	CString	strstrNotationName((LPCTSTR)_bstr_t(*strNotationName, true));

	try
	{
		pThis->UnparsedEntityDecl(strstrName, strstrPublicId, strstrSystemId, strstrNotationName);
		*strName = strstrName.AllocSysString();
		*strPublicId = strstrPublicId.AllocSysString();
		*strSystemId = strstrSystemId.AllocSysString();
		*strNotationName = strstrNotationName.AllocSysString();
		return S_OK;
	}
	catch (CComException *pE)
	{
		HRESULT	hr = pE->GetResult();
		pE->Delete();
		*strName = strstrName.AllocSysString();
		*strPublicId = strstrPublicId.AllocSysString();
		*strSystemId = strstrSystemId.AllocSysString();
		*strNotationName = strstrNotationName.AllocSysString();
		return hr;
	}

}




//--------------------------------------------------------------------
// class CVBSAXLexicalHandlerImpl
//--------------------------------------------------------------------

CVBSAXLexicalHandlerImpl::CVBSAXLexicalHandlerImpl(AFX_MODULE_STATE *pModuleState, BOOL bNoDelete /*= FALSE*/)
: CDispatchInterfaceImplementationWrapper<IVBSAXLexicalHandler>(pModuleState, LIBID, LIBVER_MAJOR, LIBVER_MINOR, bNoDelete)
{}


CVBSAXLexicalHandlerImpl::CVBSAXLexicalHandlerImpl(GET_MODULE_STATE_FUNCTION pGetModuleStateFunction, BOOL bNoDelete /*= FALSE*/)
: CDispatchInterfaceImplementationWrapper<IVBSAXLexicalHandler>(pGetModuleStateFunction, LIBID, LIBVER_MAJOR, LIBVER_MINOR, bNoDelete)
{}


CVBSAXLexicalHandlerImpl::CVBSAXLexicalHandlerImpl(AFX_MODULE_STATE *pModuleState, IUnknown *pOwner)
: CDispatchInterfaceImplementationWrapper<IVBSAXLexicalHandler>(pModuleState, LIBID, LIBVER_MAJOR, LIBVER_MINOR, pOwner)
{}


CVBSAXLexicalHandlerImpl::CVBSAXLexicalHandlerImpl(GET_MODULE_STATE_FUNCTION pGetModuleStateFunction, IUnknown *pOwner)
: CDispatchInterfaceImplementationWrapper<IVBSAXLexicalHandler>(pGetModuleStateFunction, LIBID, LIBVER_MAJOR, LIBVER_MINOR, pOwner)
{}


IVBSAXLexicalHandler* CVBSAXLexicalHandlerImpl::GetInterface()
{
	return reinterpret_cast<IVBSAXLexicalHandler*>(&m_xVBSAXLexicalHandler);
}


IUnknown* CVBSAXLexicalHandlerImpl::GetInterfaceById(REFIID riid)
{
	return CDispatchInterfaceImplementationWrapper<IVBSAXLexicalHandler>::GetInterfaceById(riid);
}


STDMETHODIMP CVBSAXLexicalHandlerImpl::XVBSAXLexicalHandler::QueryInterface(REFIID riid, void **ppv)
{
	GETTHIS(CVBSAXLexicalHandlerImpl, m_xVBSAXLexicalHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());
	return pThis->QueryInterface(riid, ppv);
}


STDMETHODIMP_(ULONG) CVBSAXLexicalHandlerImpl::XVBSAXLexicalHandler::AddRef()
{
	GETTHIS(CVBSAXLexicalHandlerImpl, m_xVBSAXLexicalHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());
	return pThis->AddRef();
}


STDMETHODIMP_(ULONG) CVBSAXLexicalHandlerImpl::XVBSAXLexicalHandler::Release()
{
	GETTHIS(CVBSAXLexicalHandlerImpl, m_xVBSAXLexicalHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());
	return pThis->Release();
}


STDMETHODIMP CVBSAXLexicalHandlerImpl::XVBSAXLexicalHandler::GetTypeInfoCount(UINT *pctinfo)
{
	GETTHIS(CVBSAXLexicalHandlerImpl, m_xVBSAXLexicalHandler);
	return pThis->GetTypeInfoCount(pctinfo);
}


STDMETHODIMP CVBSAXLexicalHandlerImpl::XVBSAXLexicalHandler::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo)
{
	GETTHIS(CVBSAXLexicalHandlerImpl, m_xVBSAXLexicalHandler);
	return pThis->GetTypeInfo(iTInfo, lcid, ppTInfo);
}


STDMETHODIMP CVBSAXLexicalHandlerImpl::XVBSAXLexicalHandler::GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgid)
{
	GETTHIS(CVBSAXLexicalHandlerImpl, m_xVBSAXLexicalHandler);
	return pThis->GetIDsOfNames(riid, rgszNames, cNames, lcid, rgid);
}


STDMETHODIMP CVBSAXLexicalHandlerImpl::XVBSAXLexicalHandler::Invoke(DISPID id, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)
{
	GETTHIS(CVBSAXLexicalHandlerImpl, m_xVBSAXLexicalHandler);
	return pThis->Invoke(id, riid, lcid, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);
}


STDMETHODIMP CVBSAXLexicalHandlerImpl::XVBSAXLexicalHandler::startDTD(BSTR* strName, BSTR* strPublicId, BSTR* strSystemId)
{
	GETTHIS(CVBSAXLexicalHandlerImpl, m_xVBSAXLexicalHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());

	// implementation wrapper call
	CString	strstrName((LPCTSTR)_bstr_t(*strName, true));
	CString	strstrPublicId((LPCTSTR)_bstr_t(*strPublicId, true));
	CString	strstrSystemId((LPCTSTR)_bstr_t(*strSystemId, true));

	try
	{
		pThis->StartDTD(strstrName, strstrPublicId, strstrSystemId);
		*strName = strstrName.AllocSysString();
		*strPublicId = strstrPublicId.AllocSysString();
		*strSystemId = strstrSystemId.AllocSysString();
		return S_OK;
	}
	catch (CComException *pE)
	{
		HRESULT	hr = pE->GetResult();
		pE->Delete();
		*strName = strstrName.AllocSysString();
		*strPublicId = strstrPublicId.AllocSysString();
		*strSystemId = strstrSystemId.AllocSysString();
		return hr;
	}

}


STDMETHODIMP CVBSAXLexicalHandlerImpl::XVBSAXLexicalHandler::endDTD()
{
	GETTHIS(CVBSAXLexicalHandlerImpl, m_xVBSAXLexicalHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());

	// implementation wrapper call

	try
	{
		pThis->EndDTD();
		return S_OK;
	}
	catch (CComException *pE)
	{
		HRESULT	hr = pE->GetResult();
		pE->Delete();
		return hr;
	}

}


STDMETHODIMP CVBSAXLexicalHandlerImpl::XVBSAXLexicalHandler::startEntity(BSTR* strName)
{
	GETTHIS(CVBSAXLexicalHandlerImpl, m_xVBSAXLexicalHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());

	// implementation wrapper call
	CString	strstrName((LPCTSTR)_bstr_t(*strName, true));

	try
	{
		pThis->StartEntity(strstrName);
		*strName = strstrName.AllocSysString();
		return S_OK;
	}
	catch (CComException *pE)
	{
		HRESULT	hr = pE->GetResult();
		pE->Delete();
		*strName = strstrName.AllocSysString();
		return hr;
	}

}


STDMETHODIMP CVBSAXLexicalHandlerImpl::XVBSAXLexicalHandler::endEntity(BSTR* strName)
{
	GETTHIS(CVBSAXLexicalHandlerImpl, m_xVBSAXLexicalHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());

	// implementation wrapper call
	CString	strstrName((LPCTSTR)_bstr_t(*strName, true));

	try
	{
		pThis->EndEntity(strstrName);
		*strName = strstrName.AllocSysString();
		return S_OK;
	}
	catch (CComException *pE)
	{
		HRESULT	hr = pE->GetResult();
		pE->Delete();
		*strName = strstrName.AllocSysString();
		return hr;
	}

}


STDMETHODIMP CVBSAXLexicalHandlerImpl::XVBSAXLexicalHandler::startCDATA()
{
	GETTHIS(CVBSAXLexicalHandlerImpl, m_xVBSAXLexicalHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());

	// implementation wrapper call

	try
	{
		pThis->StartCDATA();
		return S_OK;
	}
	catch (CComException *pE)
	{
		HRESULT	hr = pE->GetResult();
		pE->Delete();
		return hr;
	}

}


STDMETHODIMP CVBSAXLexicalHandlerImpl::XVBSAXLexicalHandler::endCDATA()
{
	GETTHIS(CVBSAXLexicalHandlerImpl, m_xVBSAXLexicalHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());

	// implementation wrapper call

	try
	{
		pThis->EndCDATA();
		return S_OK;
	}
	catch (CComException *pE)
	{
		HRESULT	hr = pE->GetResult();
		pE->Delete();
		return hr;
	}

}


STDMETHODIMP CVBSAXLexicalHandlerImpl::XVBSAXLexicalHandler::comment(BSTR* strChars)
{
	GETTHIS(CVBSAXLexicalHandlerImpl, m_xVBSAXLexicalHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());

	// implementation wrapper call
	CString	strstrChars((LPCTSTR)_bstr_t(*strChars, true));

	try
	{
		pThis->Comment(strstrChars);
		*strChars = strstrChars.AllocSysString();
		return S_OK;
	}
	catch (CComException *pE)
	{
		HRESULT	hr = pE->GetResult();
		pE->Delete();
		*strChars = strstrChars.AllocSysString();
		return hr;
	}

}




//--------------------------------------------------------------------
// class CVBSAXEntityResolverImpl
//--------------------------------------------------------------------

CVBSAXEntityResolverImpl::CVBSAXEntityResolverImpl(AFX_MODULE_STATE *pModuleState, BOOL bNoDelete /*= FALSE*/)
: CDispatchInterfaceImplementationWrapper<IVBSAXEntityResolver>(pModuleState, LIBID, LIBVER_MAJOR, LIBVER_MINOR, bNoDelete)
{}


CVBSAXEntityResolverImpl::CVBSAXEntityResolverImpl(GET_MODULE_STATE_FUNCTION pGetModuleStateFunction, BOOL bNoDelete /*= FALSE*/)
: CDispatchInterfaceImplementationWrapper<IVBSAXEntityResolver>(pGetModuleStateFunction, LIBID, LIBVER_MAJOR, LIBVER_MINOR, bNoDelete)
{}


CVBSAXEntityResolverImpl::CVBSAXEntityResolverImpl(AFX_MODULE_STATE *pModuleState, IUnknown *pOwner)
: CDispatchInterfaceImplementationWrapper<IVBSAXEntityResolver>(pModuleState, LIBID, LIBVER_MAJOR, LIBVER_MINOR, pOwner)
{}


CVBSAXEntityResolverImpl::CVBSAXEntityResolverImpl(GET_MODULE_STATE_FUNCTION pGetModuleStateFunction, IUnknown *pOwner)
: CDispatchInterfaceImplementationWrapper<IVBSAXEntityResolver>(pGetModuleStateFunction, LIBID, LIBVER_MAJOR, LIBVER_MINOR, pOwner)
{}


IVBSAXEntityResolver* CVBSAXEntityResolverImpl::GetInterface()
{
	return reinterpret_cast<IVBSAXEntityResolver*>(&m_xVBSAXEntityResolver);
}


IUnknown* CVBSAXEntityResolverImpl::GetInterfaceById(REFIID riid)
{
	return CDispatchInterfaceImplementationWrapper<IVBSAXEntityResolver>::GetInterfaceById(riid);
}


STDMETHODIMP CVBSAXEntityResolverImpl::XVBSAXEntityResolver::QueryInterface(REFIID riid, void **ppv)
{
	GETTHIS(CVBSAXEntityResolverImpl, m_xVBSAXEntityResolver);
	AFX_MANAGE_STATE(pThis->GetModuleState());
	return pThis->QueryInterface(riid, ppv);
}


STDMETHODIMP_(ULONG) CVBSAXEntityResolverImpl::XVBSAXEntityResolver::AddRef()
{
	GETTHIS(CVBSAXEntityResolverImpl, m_xVBSAXEntityResolver);
	AFX_MANAGE_STATE(pThis->GetModuleState());
	return pThis->AddRef();
}


STDMETHODIMP_(ULONG) CVBSAXEntityResolverImpl::XVBSAXEntityResolver::Release()
{
	GETTHIS(CVBSAXEntityResolverImpl, m_xVBSAXEntityResolver);
	AFX_MANAGE_STATE(pThis->GetModuleState());
	return pThis->Release();
}


STDMETHODIMP CVBSAXEntityResolverImpl::XVBSAXEntityResolver::GetTypeInfoCount(UINT *pctinfo)
{
	GETTHIS(CVBSAXEntityResolverImpl, m_xVBSAXEntityResolver);
	return pThis->GetTypeInfoCount(pctinfo);
}


STDMETHODIMP CVBSAXEntityResolverImpl::XVBSAXEntityResolver::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo)
{
	GETTHIS(CVBSAXEntityResolverImpl, m_xVBSAXEntityResolver);
	return pThis->GetTypeInfo(iTInfo, lcid, ppTInfo);
}


STDMETHODIMP CVBSAXEntityResolverImpl::XVBSAXEntityResolver::GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgid)
{
	GETTHIS(CVBSAXEntityResolverImpl, m_xVBSAXEntityResolver);
	return pThis->GetIDsOfNames(riid, rgszNames, cNames, lcid, rgid);
}


STDMETHODIMP CVBSAXEntityResolverImpl::XVBSAXEntityResolver::Invoke(DISPID id, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)
{
	GETTHIS(CVBSAXEntityResolverImpl, m_xVBSAXEntityResolver);
	return pThis->Invoke(id, riid, lcid, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);
}


STDMETHODIMP CVBSAXEntityResolverImpl::XVBSAXEntityResolver::resolveEntity(BSTR* strPublicId, BSTR* strSystemId, VARIANT* Result)
{
	GETTHIS(CVBSAXEntityResolverImpl, m_xVBSAXEntityResolver);
	AFX_MANAGE_STATE(pThis->GetModuleState());

	// implementation wrapper call
	CString	strstrPublicId((LPCTSTR)_bstr_t(*strPublicId, true));
	CString	strstrSystemId((LPCTSTR)_bstr_t(*strSystemId, true));
	_variant_t o;

	try
	{
		o = pThis->ResolveEntity(strstrPublicId, strstrSystemId);
		*strPublicId = strstrPublicId.AllocSysString();
		*strSystemId = strstrSystemId.AllocSysString();
		*Result = o;
		return S_OK;
	}
	catch (CComException *pE)
	{
		HRESULT	hr = pE->GetResult();
		pE->Delete();
		*strPublicId = strstrPublicId.AllocSysString();
		*strSystemId = strstrSystemId.AllocSysString();
		return hr;
	}

}




//--------------------------------------------------------------------
// class CVBSAXErrorHandlerImpl
//--------------------------------------------------------------------

CVBSAXErrorHandlerImpl::CVBSAXErrorHandlerImpl(AFX_MODULE_STATE *pModuleState, BOOL bNoDelete /*= FALSE*/)
: CDispatchInterfaceImplementationWrapper<IVBSAXErrorHandler>(pModuleState, LIBID, LIBVER_MAJOR, LIBVER_MINOR, bNoDelete)
{}


CVBSAXErrorHandlerImpl::CVBSAXErrorHandlerImpl(GET_MODULE_STATE_FUNCTION pGetModuleStateFunction, BOOL bNoDelete /*= FALSE*/)
: CDispatchInterfaceImplementationWrapper<IVBSAXErrorHandler>(pGetModuleStateFunction, LIBID, LIBVER_MAJOR, LIBVER_MINOR, bNoDelete)
{}


CVBSAXErrorHandlerImpl::CVBSAXErrorHandlerImpl(AFX_MODULE_STATE *pModuleState, IUnknown *pOwner)
: CDispatchInterfaceImplementationWrapper<IVBSAXErrorHandler>(pModuleState, LIBID, LIBVER_MAJOR, LIBVER_MINOR, pOwner)
{}


CVBSAXErrorHandlerImpl::CVBSAXErrorHandlerImpl(GET_MODULE_STATE_FUNCTION pGetModuleStateFunction, IUnknown *pOwner)
: CDispatchInterfaceImplementationWrapper<IVBSAXErrorHandler>(pGetModuleStateFunction, LIBID, LIBVER_MAJOR, LIBVER_MINOR, pOwner)
{}


IVBSAXErrorHandler* CVBSAXErrorHandlerImpl::GetInterface()
{
	return reinterpret_cast<IVBSAXErrorHandler*>(&m_xVBSAXErrorHandler);
}


IUnknown* CVBSAXErrorHandlerImpl::GetInterfaceById(REFIID riid)
{
	return CDispatchInterfaceImplementationWrapper<IVBSAXErrorHandler>::GetInterfaceById(riid);
}


STDMETHODIMP CVBSAXErrorHandlerImpl::XVBSAXErrorHandler::QueryInterface(REFIID riid, void **ppv)
{
	GETTHIS(CVBSAXErrorHandlerImpl, m_xVBSAXErrorHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());
	return pThis->QueryInterface(riid, ppv);
}


STDMETHODIMP_(ULONG) CVBSAXErrorHandlerImpl::XVBSAXErrorHandler::AddRef()
{
	GETTHIS(CVBSAXErrorHandlerImpl, m_xVBSAXErrorHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());
	return pThis->AddRef();
}


STDMETHODIMP_(ULONG) CVBSAXErrorHandlerImpl::XVBSAXErrorHandler::Release()
{
	GETTHIS(CVBSAXErrorHandlerImpl, m_xVBSAXErrorHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());
	return pThis->Release();
}


STDMETHODIMP CVBSAXErrorHandlerImpl::XVBSAXErrorHandler::GetTypeInfoCount(UINT *pctinfo)
{
	GETTHIS(CVBSAXErrorHandlerImpl, m_xVBSAXErrorHandler);
	return pThis->GetTypeInfoCount(pctinfo);
}


STDMETHODIMP CVBSAXErrorHandlerImpl::XVBSAXErrorHandler::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo)
{
	GETTHIS(CVBSAXErrorHandlerImpl, m_xVBSAXErrorHandler);
	return pThis->GetTypeInfo(iTInfo, lcid, ppTInfo);
}


STDMETHODIMP CVBSAXErrorHandlerImpl::XVBSAXErrorHandler::GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgid)
{
	GETTHIS(CVBSAXErrorHandlerImpl, m_xVBSAXErrorHandler);
	return pThis->GetIDsOfNames(riid, rgszNames, cNames, lcid, rgid);
}


STDMETHODIMP CVBSAXErrorHandlerImpl::XVBSAXErrorHandler::Invoke(DISPID id, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)
{
	GETTHIS(CVBSAXErrorHandlerImpl, m_xVBSAXErrorHandler);
	return pThis->Invoke(id, riid, lcid, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);
}


STDMETHODIMP CVBSAXErrorHandlerImpl::XVBSAXErrorHandler::error(IVBSAXLocator* oLocator, BSTR* strErrorMessage, long nErrorCode)
{
	GETTHIS(CVBSAXErrorHandlerImpl, m_xVBSAXErrorHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());

	// implementation wrapper call
	CString	strstrErrorMessage((LPCTSTR)_bstr_t(*strErrorMessage, true));

	try
	{
		pThis->Error(CVBSAXLocator(oLocator, TRUE), strstrErrorMessage, nErrorCode);
		*strErrorMessage = strstrErrorMessage.AllocSysString();
		return S_OK;
	}
	catch (CComException *pE)
	{
		HRESULT	hr = pE->GetResult();
		pE->Delete();
		*strErrorMessage = strstrErrorMessage.AllocSysString();
		return hr;
	}

}


STDMETHODIMP CVBSAXErrorHandlerImpl::XVBSAXErrorHandler::fatalError(IVBSAXLocator* oLocator, BSTR* strErrorMessage, long nErrorCode)
{
	GETTHIS(CVBSAXErrorHandlerImpl, m_xVBSAXErrorHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());

	// implementation wrapper call
	CString	strstrErrorMessage((LPCTSTR)_bstr_t(*strErrorMessage, true));

	try
	{
		pThis->FatalError(CVBSAXLocator(oLocator, TRUE), strstrErrorMessage, nErrorCode);
		*strErrorMessage = strstrErrorMessage.AllocSysString();
		return S_OK;
	}
	catch (CComException *pE)
	{
		HRESULT	hr = pE->GetResult();
		pE->Delete();
		*strErrorMessage = strstrErrorMessage.AllocSysString();
		return hr;
	}

}


STDMETHODIMP CVBSAXErrorHandlerImpl::XVBSAXErrorHandler::ignorableWarning(IVBSAXLocator* oLocator, BSTR* strErrorMessage, long nErrorCode)
{
	GETTHIS(CVBSAXErrorHandlerImpl, m_xVBSAXErrorHandler);
	AFX_MANAGE_STATE(pThis->GetModuleState());

	// implementation wrapper call
	CString	strstrErrorMessage((LPCTSTR)_bstr_t(*strErrorMessage, true));

	try
	{
		pThis->IgnorableWarning(CVBSAXLocator(oLocator, TRUE), strstrErrorMessage, nErrorCode);
		*strErrorMessage = strstrErrorMessage.AllocSysString();
		return S_OK;
	}
	catch (CComException *pE)
	{
		HRESULT	hr = pE->GetResult();
		pE->Delete();
		*strErrorMessage = strstrErrorMessage.AllocSysString();
		return hr;
	}

}




//--------------------------------------------------------------------
// class CVBSAXXMLFilterImpl
//--------------------------------------------------------------------

CVBSAXXMLFilterImpl::CVBSAXXMLFilterImpl(AFX_MODULE_STATE *pModuleState, BOOL bNoDelete /*= FALSE*/)
: CDispatchInterfaceImplementationWrapper<IVBSAXXMLFilter>(pModuleState, LIBID, LIBVER_MAJOR, LIBVER_MINOR, bNoDelete)
{}


CVBSAXXMLFilterImpl::CVBSAXXMLFilterImpl(GET_MODULE_STATE_FUNCTION pGetModuleStateFunction, BOOL bNoDelete /*= FALSE*/)
: CDispatchInterfaceImplementationWrapper<IVBSAXXMLFilter>(pGetModuleStateFunction, LIBID, LIBVER_MAJOR, LIBVER_MINOR, bNoDelete)
{}


CVBSAXXMLFilterImpl::CVBSAXXMLFilterImpl(AFX_MODULE_STATE *pModuleState, IUnknown *pOwner)
: CDispatchInterfaceImplementationWrapper<IVBSAXXMLFilter>(pModuleState, LIBID, LIBVER_MAJOR, LIBVER_MINOR, pOwner)
{}


CVBSAXXMLFilterImpl::CVBSAXXMLFilterImpl(GET_MODULE_STATE_FUNCTION pGetModuleStateFunction, IUnknown *pOwner)
: CDispatchInterfaceImplementationWrapper<IVBSAXXMLFilter>(pGetModuleStateFunction, LIBID, LIBVER_MAJOR, LIBVER_MINOR, pOwner)
{}


IVBSAXXMLFilter* CVBSAXXMLFilterImpl::GetInterface()
{
	return reinterpret_cast<IVBSAXXMLFilter*>(&m_xVBSAXXMLFilter);
}


IUnknown* CVBSAXXMLFilterImpl::GetInterfaceById(REFIID riid)
{
	return CDispatchInterfaceImplementationWrapper<IVBSAXXMLFilter>::GetInterfaceById(riid);
}


STDMETHODIMP CVBSAXXMLFilterImpl::XVBSAXXMLFilter::QueryInterface(REFIID riid, void **ppv)
{
	GETTHIS(CVBSAXXMLFilterImpl, m_xVBSAXXMLFilter);
	AFX_MANAGE_STATE(pThis->GetModuleState());
	return pThis->QueryInterface(riid, ppv);
}


STDMETHODIMP_(ULONG) CVBSAXXMLFilterImpl::XVBSAXXMLFilter::AddRef()
{
	GETTHIS(CVBSAXXMLFilterImpl, m_xVBSAXXMLFilter);
	AFX_MANAGE_STATE(pThis->GetModuleState());
	return pThis->AddRef();
}


STDMETHODIMP_(ULONG) CVBSAXXMLFilterImpl::XVBSAXXMLFilter::Release()
{
	GETTHIS(CVBSAXXMLFilterImpl, m_xVBSAXXMLFilter);
	AFX_MANAGE_STATE(pThis->GetModuleState());
	return pThis->Release();
}


STDMETHODIMP CVBSAXXMLFilterImpl::XVBSAXXMLFilter::GetTypeInfoCount(UINT *pctinfo)
{
	GETTHIS(CVBSAXXMLFilterImpl, m_xVBSAXXMLFilter);
	return pThis->GetTypeInfoCount(pctinfo);
}


STDMETHODIMP CVBSAXXMLFilterImpl::XVBSAXXMLFilter::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo)
{
	GETTHIS(CVBSAXXMLFilterImpl, m_xVBSAXXMLFilter);
	return pThis->GetTypeInfo(iTInfo, lcid, ppTInfo);
}


STDMETHODIMP CVBSAXXMLFilterImpl::XVBSAXXMLFilter::GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgid)
{
	GETTHIS(CVBSAXXMLFilterImpl, m_xVBSAXXMLFilter);
	return pThis->GetIDsOfNames(riid, rgszNames, cNames, lcid, rgid);
}


STDMETHODIMP CVBSAXXMLFilterImpl::XVBSAXXMLFilter::Invoke(DISPID id, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)
{
	GETTHIS(CVBSAXXMLFilterImpl, m_xVBSAXXMLFilter);
	return pThis->Invoke(id, riid, lcid, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);
}


STDMETHODIMP CVBSAXXMLFilterImpl::XVBSAXXMLFilter::get_parent(IVBSAXXMLReader** Result)
{
	GETTHIS(CVBSAXXMLFilterImpl, m_xVBSAXXMLFilter);
	AFX_MANAGE_STATE(pThis->GetModuleState());

	// implementation wrapper call
	IVBSAXXMLReader* o;

	try
	{
		o = pThis->GetParent();
		*Result = o;
		return S_OK;
	}
	catch (CComException *pE)
	{
		HRESULT	hr = pE->GetResult();
		pE->Delete();
		return hr;
	}

}


STDMETHODIMP CVBSAXXMLFilterImpl::XVBSAXXMLFilter::putref_parent(IVBSAXXMLReader* rhs)
{
	GETTHIS(CVBSAXXMLFilterImpl, m_xVBSAXXMLFilter);
	AFX_MANAGE_STATE(pThis->GetModuleState());

	// implementation wrapper call

	try
	{
		pThis->SetRefParent(CVBSAXXMLReader(rhs, TRUE));
		return S_OK;
	}
	catch (CComException *pE)
	{
		HRESULT	hr = pE->GetResult();
		pE->Delete();
		return hr;
	}

}





} // namespace MsXml

