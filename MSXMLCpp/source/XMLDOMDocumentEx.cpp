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
* Copyright (C) 2001 Sven Wiegand
* 
* This file is free software; you can redistribute it and/or
* modify, but leave the headers intact and do not remove any 
* copyrights from the source.
*
* If you have further questions, suggestions or bug fixes, please
* let me know
*
*    sven.wiegand@web.de
*
********************************************************************/

#include "stdafx.h"
#include "XMLDOMDocumentEx.h"
#include "Helpers.h"


namespace MsXml
{

//-------------------------------------------------------------------
// CXMLDOMDocumentEx
//-------------------------------------------------------------------

CXMLDOMDocumentEx::CXMLDOMDocumentEx()
:	CXMLDOMDocument2()
{}


CXMLDOMDocumentEx::CXMLDOMDocumentEx(IXMLDOMDocument2 *p, BOOL bIncrementRefCount /* = FALSE */)
:	CXMLDOMDocument2(p, bIncrementRefCount)
{}


CXMLDOMDocumentEx::CXMLDOMDocumentEx(const CXMLDOMDocument2 &o)
:	CXMLDOMDocument2(o)
{}


CXMLDOMDocumentEx::CXMLDOMDocumentEx(IUnknown *pUnknown, BOOL bIncrementRefCount /* = FALSE */, BOOL bThrowException /* = FALSE */)
:	CXMLDOMDocument2(pUnknown, bIncrementRefCount, bThrowException)
{}


/////////////////////////////////////////////////////////////////////
// operations

void CXMLDOMDocumentEx::Create(LPCTSTR lpszDocumentNamespaces /* = NULL */, LPCTSTR lpszSchemaPath /* = NULL */, LPCTSTR lpszSchemaNamespace /* = _T("") */)
{
	// create XML document
	HRESULT	hr;
	hr = CoCreateInstance(MsXml::CDOMDocument40Class::s_ClsId);
	if (FAILED(hr))
		AfxThrowComException(hr);

	SetAsync(VARIANT_FALSE);
	SetProperty(_T("SelectionLanguage"), _T("XPath"));

	// set document namespaces if required
	if (lpszDocumentNamespaces)
		SetProperty(_T("SelectionNamespaces"), lpszDocumentNamespaces);

	// add schema (if specified)
	if (lpszSchemaPath)
		AddSchema(lpszSchemaPath, lpszSchemaNamespace);
}


void CXMLDOMDocumentEx::Destroy()
{
	Detach(TRUE);
}


void CXMLDOMDocumentEx::AddSchema(LPCTSTR lpszSchema, LPCTSTR lpszSchemaNamespace)
{
	MsXml::CXMLDOMSchemaCollection	xmlSchemaCache;
	try
	{
		IUnknown	*pSchemaCache = GetSchemas();
		if (!pSchemaCache)
			AfxThrowComException(S_FALSE);

		CInterfaceCallingWrapper<IUnknown>	schemaCache(pSchemaCache, FALSE, TRUE);
		xmlSchemaCache.Attach((IXMLDOMSchemaCollection*)schemaCache.QueryInterface(IID_IXMLDOMSchemaCollection, TRUE));
	}
	catch (CComException *pE)
	{
		pE->Delete();
	}
	if (!xmlSchemaCache)
	{
		HRESULT	hr = xmlSchemaCache.CoCreateInstance(MsXml::CXMLSchemaCache40Class::s_ClsId);
		if (FAILED(hr) || !xmlSchemaCache)
			AfxThrowComException(hr);
		SetRefSchemas(xmlSchemaCache.P());
	}
	xmlSchemaCache.Add(lpszSchemaNamespace, lpszSchema);
}


void CXMLDOMDocumentEx::RemoveSchema(LPCTSTR lpszSchemaNamespace)
{
	MsXml::CXMLDOMSchemaCollection	xmlSchemaCache;
	IUnknown	*pSchemaCache = GetSchemas();
	if (!pSchemaCache)
		AfxThrowComException(S_FALSE);

	CInterfaceCallingWrapper<IUnknown>	schemaCache(pSchemaCache, FALSE, TRUE);
	xmlSchemaCache.Attach((IXMLDOMSchemaCollection*)schemaCache.QueryInterface(IID_IXMLDOMSchemaCollection, TRUE));
	xmlSchemaCache.Remove(lpszSchemaNamespace);
}


void CXMLDOMDocumentEx::SavePretty(LPCTSTR lpszPath, LPCTSTR lpszEncoding)
{
	HRESULT	hr = DOMSavePretty(P(), lpszPath, lpszEncoding);
	if (hr!=S_OK) AfxThrowComException(hr);
}


}//namespace MsXml