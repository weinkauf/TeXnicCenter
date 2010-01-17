/********************************************************************
*
* File created by COM Interface Wrapper Wizard (CIWW) ver 1.00.000
* at Wed Oct 01 18:25:36 2003
* 
* (CIWW - Copyright © 2001-2002 Sven Wiegand (sven.wiegand@web.de))
*
********************************************************************/

#include "stdafx.h"
#include "domxml.h"

namespace MsXml
{


#include "DOMXML_raw.h"


static GUID LIBID = {0xf5078f18, 0xc551, 0x11d3, {0x89, 0xb9, 0x00, 0x00, 0xf8, 0x1f, 0xe2, 0x21}};

#define LIBVER_MAJOR 4
#define LIBVER_MINOR 0

//--------------------------------------------------------------------
// class CXMLDOMDocument
//--------------------------------------------------------------------

CXMLDOMDocument::CXMLDOMDocument()
:	CInterfaceCallingWrapper<IXMLDOMDocument>()
{}


CXMLDOMDocument::CXMLDOMDocument(IXMLDOMDocument *p, BOOL bIncrementRefCount /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMDocument>(p, bIncrementRefCount)
{}


CXMLDOMDocument::CXMLDOMDocument(const CXMLDOMDocument &o)
:	CInterfaceCallingWrapper<IXMLDOMDocument>(o)
{}


CXMLDOMDocument::CXMLDOMDocument(IUnknown *pUnknown, BOOL bIncrementRefCount /*= FALSE*/, BOOL bThrowException /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMDocument>(pUnknown, bIncrementRefCount, bThrowException)
{}


CString CXMLDOMDocument::GetNodeName()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->get_nodeName(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


_variant_t CXMLDOMDocument::GetNodeValue()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->get_nodeValue(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMDocument::SetNodeValue(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->put_nodeValue(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	VariantClear(&varrhs);
}


DOMNodeType CXMLDOMDocument::GetNodeType()
{
	DOMNodeType o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->get_nodeType(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	return o;
}


CXMLDOMNode CXMLDOMDocument::GetParentNode()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->get_parentNode(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	return o;
}


CXMLDOMNodeList CXMLDOMDocument::GetChildNodes()
{
	IXMLDOMNodeList* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->get_childNodes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	return o;
}


CXMLDOMNode CXMLDOMDocument::GetFirstChild()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->get_firstChild(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	return o;
}


CXMLDOMNode CXMLDOMDocument::GetLastChild()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->get_lastChild(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	return o;
}


CXMLDOMNode CXMLDOMDocument::GetPreviousSibling()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->get_previousSibling(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	return o;
}


CXMLDOMNode CXMLDOMDocument::GetNextSibling()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->get_nextSibling(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	return o;
}


CXMLDOMNamedNodeMap CXMLDOMDocument::GetAttributes()
{
	IXMLDOMNamedNodeMap* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->get_attributes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	return o;
}


CXMLDOMNode CXMLDOMDocument::InsertBefore(IXMLDOMNode* newChild, _variant_t refChild)
{
	VARIANT	varrefChild = refChild.Detach();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->insertBefore(newChild, varrefChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	VariantClear(&varrefChild);
	return o;
}


CXMLDOMNode CXMLDOMDocument::ReplaceChild(IXMLDOMNode* newChild, IXMLDOMNode* oldChild)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->replaceChild(newChild, oldChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	return o;
}


CXMLDOMNode CXMLDOMDocument::RemoveChild(IXMLDOMNode* childNode)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->removeChild(childNode, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	return o;
}


CXMLDOMNode CXMLDOMDocument::AppendChild(IXMLDOMNode* newChild)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->appendChild(newChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	return o;
}


BOOL CXMLDOMDocument::HasChildNodes()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->hasChildNodes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	return o;
}


CXMLDOMDocument CXMLDOMDocument::GetOwnerDocument()
{
	IXMLDOMDocument* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->get_ownerDocument(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	return o;
}


CXMLDOMNode CXMLDOMDocument::CloneNode(BOOL deep)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->cloneNode(deep, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	return o;
}


CString CXMLDOMDocument::GetNodeTypeString()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->get_nodeTypeString(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMDocument::GetText()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->get_text(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMDocument::SetText(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->put_text(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	SysFreeString(bstrrhs);
}


BOOL CXMLDOMDocument::GetSpecified()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->get_specified(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	return o;
}


CXMLDOMNode CXMLDOMDocument::GetDefinition()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->get_definition(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	return o;
}


_variant_t CXMLDOMDocument::GetNodeTypedValue()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->get_nodeTypedValue(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMDocument::SetNodeTypedValue(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->put_nodeTypedValue(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	VariantClear(&varrhs);
}


_variant_t CXMLDOMDocument::GetDataType()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->get_dataType(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMDocument::SetDataType(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->put_dataType(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	SysFreeString(bstrrhs);
}


CString CXMLDOMDocument::GetXml()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->get_xml(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMDocument::TransformNode(IXMLDOMNode* stylesheet)
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->transformNode(stylesheet, &bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CXMLDOMNodeList CXMLDOMDocument::SelectNodes(LPCTSTR queryString)
{
	CString	strqueryString(queryString);
	BSTR	bstrqueryString = strqueryString.AllocSysString();
	IXMLDOMNodeList* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->selectNodes(bstrqueryString, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	SysFreeString(bstrqueryString);
	return o;
}


CXMLDOMNode CXMLDOMDocument::SelectSingleNode(LPCTSTR queryString)
{
	CString	strqueryString(queryString);
	BSTR	bstrqueryString = strqueryString.AllocSysString();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->selectSingleNode(bstrqueryString, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	SysFreeString(bstrqueryString);
	return o;
}


BOOL CXMLDOMDocument::GetParsed()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->get_parsed(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	return o;
}


CString CXMLDOMDocument::GetNamespaceURI()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->get_namespaceURI(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMDocument::GetPrefix()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->get_prefix(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMDocument::GetBaseName()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->get_baseName(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMDocument::TransformNodeToObject(IXMLDOMNode* stylesheet, _variant_t outputObject)
{
	VARIANT	varoutputObject = outputObject.Detach();

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->transformNodeToObject(stylesheet, varoutputObject);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	VariantClear(&varoutputObject);
}


CXMLDOMDocumentType CXMLDOMDocument::GetDoctype()
{
	IXMLDOMDocumentType* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->get_doctype(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	return o;
}


CXMLDOMImplementation CXMLDOMDocument::GetImplementation()
{
	IXMLDOMImplementation* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->get_implementation(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	return o;
}


CXMLDOMElement CXMLDOMDocument::GetDocumentElement()
{
	IXMLDOMElement* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->get_documentElement(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	return o;
}


void CXMLDOMDocument::SetRefDocumentElement(IXMLDOMElement* rhs)
{

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->putref_documentElement(rhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

}


CXMLDOMElement CXMLDOMDocument::CreateElement(LPCTSTR tagName)
{
	CString	strtagName(tagName);
	BSTR	bstrtagName = strtagName.AllocSysString();
	IXMLDOMElement* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->createElement(bstrtagName, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	SysFreeString(bstrtagName);
	return o;
}


CXMLDOMDocumentFragment CXMLDOMDocument::CreateDocumentFragment()
{
	IXMLDOMDocumentFragment* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->createDocumentFragment(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	return o;
}


CXMLDOMText CXMLDOMDocument::CreateTextNode(LPCTSTR data)
{
	CString	strdata(data);
	BSTR	bstrdata = strdata.AllocSysString();
	IXMLDOMText* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->createTextNode(bstrdata, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	SysFreeString(bstrdata);
	return o;
}


CXMLDOMComment CXMLDOMDocument::CreateComment(LPCTSTR data)
{
	CString	strdata(data);
	BSTR	bstrdata = strdata.AllocSysString();
	IXMLDOMComment* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->createComment(bstrdata, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	SysFreeString(bstrdata);
	return o;
}


CXMLDOMCDATASection CXMLDOMDocument::CreateCDATASection(LPCTSTR data)
{
	CString	strdata(data);
	BSTR	bstrdata = strdata.AllocSysString();
	IXMLDOMCDATASection* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->createCDATASection(bstrdata, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	SysFreeString(bstrdata);
	return o;
}


CXMLDOMProcessingInstruction CXMLDOMDocument::CreateProcessingInstruction(LPCTSTR target, LPCTSTR data)
{
	CString	strtarget(target);
	BSTR	bstrtarget = strtarget.AllocSysString();
	CString	strdata(data);
	BSTR	bstrdata = strdata.AllocSysString();
	IXMLDOMProcessingInstruction* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->createProcessingInstruction(bstrtarget, bstrdata, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	SysFreeString(bstrtarget);
	SysFreeString(bstrdata);
	return o;
}


CXMLDOMAttribute CXMLDOMDocument::CreateAttribute(LPCTSTR name)
{
	CString	strname(name);
	BSTR	bstrname = strname.AllocSysString();
	IXMLDOMAttribute* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->createAttribute(bstrname, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	SysFreeString(bstrname);
	return o;
}


CXMLDOMEntityReference CXMLDOMDocument::CreateEntityReference(LPCTSTR name)
{
	CString	strname(name);
	BSTR	bstrname = strname.AllocSysString();
	IXMLDOMEntityReference* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->createEntityReference(bstrname, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	SysFreeString(bstrname);
	return o;
}


CXMLDOMNodeList CXMLDOMDocument::GetElementsByTagName(LPCTSTR tagName)
{
	CString	strtagName(tagName);
	BSTR	bstrtagName = strtagName.AllocSysString();
	IXMLDOMNodeList* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->getElementsByTagName(bstrtagName, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	SysFreeString(bstrtagName);
	return o;
}


CXMLDOMNode CXMLDOMDocument::CreateNode(_variant_t type, LPCTSTR name, LPCTSTR namespaceURI)
{
	VARIANT	vartype = type.Detach();
	CString	strname(name);
	BSTR	bstrname = strname.AllocSysString();
	CString	strnamespaceURI(namespaceURI);
	BSTR	bstrnamespaceURI = strnamespaceURI.AllocSysString();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->createNode(vartype, bstrname, bstrnamespaceURI, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	VariantClear(&vartype);
	SysFreeString(bstrname);
	SysFreeString(bstrnamespaceURI);
	return o;
}


CXMLDOMNode CXMLDOMDocument::NodeFromID(LPCTSTR idString)
{
	CString	stridString(idString);
	BSTR	bstridString = stridString.AllocSysString();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->nodeFromID(bstridString, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	SysFreeString(bstridString);
	return o;
}


BOOL CXMLDOMDocument::Load(_variant_t xmlSource)
{
	VARIANT	varxmlSource = xmlSource.Detach();
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->load(varxmlSource, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	VariantClear(&varxmlSource);
	return o;
}


long CXMLDOMDocument::GetReadyState()
{
	long o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->get_readyState(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	return o;
}


CXMLDOMParseError CXMLDOMDocument::GetParseError()
{
	IXMLDOMParseError* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->get_parseError(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	return o;
}


CString CXMLDOMDocument::GetUrl()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->get_url(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


BOOL CXMLDOMDocument::GetAsync()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->get_async(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	return o;
}


void CXMLDOMDocument::SetAsync(BOOL rhs)
{

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->put_async(rhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

}


void CXMLDOMDocument::Abort()
{

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->abort();
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

}


BOOL CXMLDOMDocument::LoadXML(LPCTSTR bstrXML)
{
	CString	strbstrXML(bstrXML);
	BSTR	bstrbstrXML = strbstrXML.AllocSysString();
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->loadXML(bstrbstrXML, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	SysFreeString(bstrbstrXML);
	return o;
}


void CXMLDOMDocument::Save(_variant_t destination)
{
	VARIANT	vardestination = destination.Detach();

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->save(vardestination);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	VariantClear(&vardestination);
}


BOOL CXMLDOMDocument::GetValidateOnParse()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->get_validateOnParse(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	return o;
}


void CXMLDOMDocument::SetValidateOnParse(BOOL rhs)
{

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->put_validateOnParse(rhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

}


BOOL CXMLDOMDocument::GetResolveExternals()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->get_resolveExternals(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	return o;
}


void CXMLDOMDocument::SetResolveExternals(BOOL rhs)
{

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->put_resolveExternals(rhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

}


BOOL CXMLDOMDocument::GetPreserveWhiteSpace()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->get_preserveWhiteSpace(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	return o;
}


void CXMLDOMDocument::SetPreserveWhiteSpace(BOOL rhs)
{

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->put_preserveWhiteSpace(rhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

}


void CXMLDOMDocument::SetOnreadystatechange(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->put_onreadystatechange(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	VariantClear(&varrhs);
}


void CXMLDOMDocument::SetOndataavailable(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->put_ondataavailable(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	VariantClear(&varrhs);
}


void CXMLDOMDocument::SetOntransformnode(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMDocument*)P())->put_ontransformnode(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument));

	VariantClear(&varrhs);
}




//--------------------------------------------------------------------
// class CXMLDOMDocument2
//--------------------------------------------------------------------

CXMLDOMDocument2::CXMLDOMDocument2()
:	CInterfaceCallingWrapper<IXMLDOMDocument2>()
{}


CXMLDOMDocument2::CXMLDOMDocument2(IXMLDOMDocument2 *p, BOOL bIncrementRefCount /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMDocument2>(p, bIncrementRefCount)
{}


CXMLDOMDocument2::CXMLDOMDocument2(const CXMLDOMDocument2 &o)
:	CInterfaceCallingWrapper<IXMLDOMDocument2>(o)
{}


CXMLDOMDocument2::CXMLDOMDocument2(IUnknown *pUnknown, BOOL bIncrementRefCount /*= FALSE*/, BOOL bThrowException /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMDocument2>(pUnknown, bIncrementRefCount, bThrowException)
{}


CString CXMLDOMDocument2::GetNodeName()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->get_nodeName(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


_variant_t CXMLDOMDocument2::GetNodeValue()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->get_nodeValue(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMDocument2::SetNodeValue(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->put_nodeValue(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	VariantClear(&varrhs);
}


DOMNodeType CXMLDOMDocument2::GetNodeType()
{
	DOMNodeType o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->get_nodeType(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	return o;
}


CXMLDOMNode CXMLDOMDocument2::GetParentNode()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->get_parentNode(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	return o;
}


CXMLDOMNodeList CXMLDOMDocument2::GetChildNodes()
{
	IXMLDOMNodeList* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->get_childNodes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	return o;
}


CXMLDOMNode CXMLDOMDocument2::GetFirstChild()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->get_firstChild(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	return o;
}


CXMLDOMNode CXMLDOMDocument2::GetLastChild()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->get_lastChild(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	return o;
}


CXMLDOMNode CXMLDOMDocument2::GetPreviousSibling()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->get_previousSibling(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	return o;
}


CXMLDOMNode CXMLDOMDocument2::GetNextSibling()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->get_nextSibling(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	return o;
}


CXMLDOMNamedNodeMap CXMLDOMDocument2::GetAttributes()
{
	IXMLDOMNamedNodeMap* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->get_attributes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	return o;
}


CXMLDOMNode CXMLDOMDocument2::InsertBefore(IXMLDOMNode* newChild, _variant_t refChild)
{
	VARIANT	varrefChild = refChild.Detach();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->insertBefore(newChild, varrefChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	VariantClear(&varrefChild);
	return o;
}


CXMLDOMNode CXMLDOMDocument2::ReplaceChild(IXMLDOMNode* newChild, IXMLDOMNode* oldChild)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->replaceChild(newChild, oldChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	return o;
}


CXMLDOMNode CXMLDOMDocument2::RemoveChild(IXMLDOMNode* childNode)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->removeChild(childNode, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	return o;
}


CXMLDOMNode CXMLDOMDocument2::AppendChild(IXMLDOMNode* newChild)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->appendChild(newChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	return o;
}


BOOL CXMLDOMDocument2::HasChildNodes()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->hasChildNodes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	return o;
}


CXMLDOMDocument CXMLDOMDocument2::GetOwnerDocument()
{
	IXMLDOMDocument* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->get_ownerDocument(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	return o;
}


CXMLDOMNode CXMLDOMDocument2::CloneNode(BOOL deep)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->cloneNode(deep, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	return o;
}


CString CXMLDOMDocument2::GetNodeTypeString()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->get_nodeTypeString(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMDocument2::GetText()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->get_text(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMDocument2::SetText(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->put_text(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	SysFreeString(bstrrhs);
}


BOOL CXMLDOMDocument2::GetSpecified()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->get_specified(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	return o;
}


CXMLDOMNode CXMLDOMDocument2::GetDefinition()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->get_definition(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	return o;
}


_variant_t CXMLDOMDocument2::GetNodeTypedValue()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->get_nodeTypedValue(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMDocument2::SetNodeTypedValue(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->put_nodeTypedValue(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	VariantClear(&varrhs);
}


_variant_t CXMLDOMDocument2::GetDataType()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->get_dataType(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMDocument2::SetDataType(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->put_dataType(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	SysFreeString(bstrrhs);
}


CString CXMLDOMDocument2::GetXml()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->get_xml(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMDocument2::TransformNode(IXMLDOMNode* stylesheet)
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->transformNode(stylesheet, &bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CXMLDOMNodeList CXMLDOMDocument2::SelectNodes(LPCTSTR queryString)
{
	CString	strqueryString(queryString);
	BSTR	bstrqueryString = strqueryString.AllocSysString();
	IXMLDOMNodeList* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->selectNodes(bstrqueryString, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	SysFreeString(bstrqueryString);
	return o;
}


CXMLDOMNode CXMLDOMDocument2::SelectSingleNode(LPCTSTR queryString)
{
	CString	strqueryString(queryString);
	BSTR	bstrqueryString = strqueryString.AllocSysString();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->selectSingleNode(bstrqueryString, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	SysFreeString(bstrqueryString);
	return o;
}


BOOL CXMLDOMDocument2::GetParsed()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->get_parsed(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	return o;
}


CString CXMLDOMDocument2::GetNamespaceURI()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->get_namespaceURI(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMDocument2::GetPrefix()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->get_prefix(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMDocument2::GetBaseName()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->get_baseName(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMDocument2::TransformNodeToObject(IXMLDOMNode* stylesheet, _variant_t outputObject)
{
	VARIANT	varoutputObject = outputObject.Detach();

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->transformNodeToObject(stylesheet, varoutputObject);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	VariantClear(&varoutputObject);
}


CXMLDOMDocumentType CXMLDOMDocument2::GetDoctype()
{
	IXMLDOMDocumentType* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->get_doctype(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	return o;
}


CXMLDOMImplementation CXMLDOMDocument2::GetImplementation()
{
	IXMLDOMImplementation* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->get_implementation(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	return o;
}


CXMLDOMElement CXMLDOMDocument2::GetDocumentElement()
{
	IXMLDOMElement* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->get_documentElement(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	return o;
}


void CXMLDOMDocument2::SetRefDocumentElement(IXMLDOMElement* rhs)
{

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->putref_documentElement(rhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

}


CXMLDOMElement CXMLDOMDocument2::CreateElement(LPCTSTR tagName)
{
	CString	strtagName(tagName);
	BSTR	bstrtagName = strtagName.AllocSysString();
	IXMLDOMElement* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->createElement(bstrtagName, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	SysFreeString(bstrtagName);
	return o;
}


CXMLDOMDocumentFragment CXMLDOMDocument2::CreateDocumentFragment()
{
	IXMLDOMDocumentFragment* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->createDocumentFragment(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	return o;
}


CXMLDOMText CXMLDOMDocument2::CreateTextNode(LPCTSTR data)
{
	CString	strdata(data);
	BSTR	bstrdata = strdata.AllocSysString();
	IXMLDOMText* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->createTextNode(bstrdata, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	SysFreeString(bstrdata);
	return o;
}


CXMLDOMComment CXMLDOMDocument2::CreateComment(LPCTSTR data)
{
	CString	strdata(data);
	BSTR	bstrdata = strdata.AllocSysString();
	IXMLDOMComment* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->createComment(bstrdata, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	SysFreeString(bstrdata);
	return o;
}


CXMLDOMCDATASection CXMLDOMDocument2::CreateCDATASection(LPCTSTR data)
{
	CString	strdata(data);
	BSTR	bstrdata = strdata.AllocSysString();
	IXMLDOMCDATASection* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->createCDATASection(bstrdata, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	SysFreeString(bstrdata);
	return o;
}


CXMLDOMProcessingInstruction CXMLDOMDocument2::CreateProcessingInstruction(LPCTSTR target, LPCTSTR data)
{
	CString	strtarget(target);
	BSTR	bstrtarget = strtarget.AllocSysString();
	CString	strdata(data);
	BSTR	bstrdata = strdata.AllocSysString();
	IXMLDOMProcessingInstruction* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->createProcessingInstruction(bstrtarget, bstrdata, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	SysFreeString(bstrtarget);
	SysFreeString(bstrdata);
	return o;
}


CXMLDOMAttribute CXMLDOMDocument2::CreateAttribute(LPCTSTR name)
{
	CString	strname(name);
	BSTR	bstrname = strname.AllocSysString();
	IXMLDOMAttribute* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->createAttribute(bstrname, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	SysFreeString(bstrname);
	return o;
}


CXMLDOMEntityReference CXMLDOMDocument2::CreateEntityReference(LPCTSTR name)
{
	CString	strname(name);
	BSTR	bstrname = strname.AllocSysString();
	IXMLDOMEntityReference* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->createEntityReference(bstrname, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	SysFreeString(bstrname);
	return o;
}


CXMLDOMNodeList CXMLDOMDocument2::GetElementsByTagName(LPCTSTR tagName)
{
	CString	strtagName(tagName);
	BSTR	bstrtagName = strtagName.AllocSysString();
	IXMLDOMNodeList* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->getElementsByTagName(bstrtagName, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	SysFreeString(bstrtagName);
	return o;
}


CXMLDOMNode CXMLDOMDocument2::CreateNode(_variant_t type, LPCTSTR name, LPCTSTR namespaceURI)
{
	VARIANT	vartype = type.Detach();
	CString	strname(name);
	BSTR	bstrname = strname.AllocSysString();
	CString	strnamespaceURI(namespaceURI);
	BSTR	bstrnamespaceURI = strnamespaceURI.AllocSysString();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->createNode(vartype, bstrname, bstrnamespaceURI, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	VariantClear(&vartype);
	SysFreeString(bstrname);
	SysFreeString(bstrnamespaceURI);
	return o;
}


CXMLDOMNode CXMLDOMDocument2::NodeFromID(LPCTSTR idString)
{
	CString	stridString(idString);
	BSTR	bstridString = stridString.AllocSysString();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->nodeFromID(bstridString, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	SysFreeString(bstridString);
	return o;
}


BOOL CXMLDOMDocument2::Load(_variant_t xmlSource)
{
	VARIANT	varxmlSource = xmlSource.Detach();
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->load(varxmlSource, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	VariantClear(&varxmlSource);
	return o;
}


long CXMLDOMDocument2::GetReadyState()
{
	long o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->get_readyState(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	return o;
}


CXMLDOMParseError CXMLDOMDocument2::GetParseError()
{
	IXMLDOMParseError* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->get_parseError(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	return o;
}


CString CXMLDOMDocument2::GetUrl()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->get_url(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


BOOL CXMLDOMDocument2::GetAsync()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->get_async(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	return o;
}


void CXMLDOMDocument2::SetAsync(BOOL rhs)
{

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->put_async(rhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

}


void CXMLDOMDocument2::Abort()
{

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->abort();
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

}


BOOL CXMLDOMDocument2::LoadXML(LPCTSTR bstrXML)
{
	CString	strbstrXML(bstrXML);
	BSTR	bstrbstrXML = strbstrXML.AllocSysString();
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->loadXML(bstrbstrXML, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	SysFreeString(bstrbstrXML);
	return o;
}


void CXMLDOMDocument2::Save(_variant_t destination)
{
	VARIANT	vardestination = destination.Detach();

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->save(vardestination);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	VariantClear(&vardestination);
}


BOOL CXMLDOMDocument2::GetValidateOnParse()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->get_validateOnParse(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	return o;
}


void CXMLDOMDocument2::SetValidateOnParse(BOOL rhs)
{

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->put_validateOnParse(rhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

}


BOOL CXMLDOMDocument2::GetResolveExternals()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->get_resolveExternals(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	return o;
}


void CXMLDOMDocument2::SetResolveExternals(BOOL rhs)
{

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->put_resolveExternals(rhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

}


BOOL CXMLDOMDocument2::GetPreserveWhiteSpace()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->get_preserveWhiteSpace(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	return o;
}


void CXMLDOMDocument2::SetPreserveWhiteSpace(BOOL rhs)
{

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->put_preserveWhiteSpace(rhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

}


void CXMLDOMDocument2::SetOnreadystatechange(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->put_onreadystatechange(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	VariantClear(&varrhs);
}


void CXMLDOMDocument2::SetOndataavailable(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->put_ondataavailable(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	VariantClear(&varrhs);
}


void CXMLDOMDocument2::SetOntransformnode(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->put_ontransformnode(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	VariantClear(&varrhs);
}


CXMLDOMSchemaCollection CXMLDOMDocument2::GetNamespaces()
{
	IXMLDOMSchemaCollection* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->get_namespaces(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	return o;
}


_variant_t CXMLDOMDocument2::GetSchemas()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->get_schemas(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMDocument2::SetRefSchemas(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->putref_schemas(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	VariantClear(&varrhs);
}


CXMLDOMParseError CXMLDOMDocument2::Validate()
{
	IXMLDOMParseError* o;

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->validate(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	return o;
}


void CXMLDOMDocument2::SetProperty(LPCTSTR name, _variant_t value)
{
	CString	strname(name);
	BSTR	bstrname = strname.AllocSysString();
	VARIANT	varvalue = value.Detach();

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->setProperty(bstrname, varvalue);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	SysFreeString(bstrname);
	VariantClear(&varvalue);
}


_variant_t CXMLDOMDocument2::GetProperty(LPCTSTR name)
{
	CString	strname(name);
	BSTR	bstrname = strname.AllocSysString();
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMDocument2*)P())->getProperty(bstrname, &varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocument2));

	SysFreeString(bstrname);
	_variant_t	o(varO, false);
	return o;
}




//--------------------------------------------------------------------
// class CXMLDOMNamedNodeMap
//--------------------------------------------------------------------

CXMLDOMNamedNodeMap::CXMLDOMNamedNodeMap()
:	CInterfaceCallingWrapper<IXMLDOMNamedNodeMap>()
{}


CXMLDOMNamedNodeMap::CXMLDOMNamedNodeMap(IXMLDOMNamedNodeMap *p, BOOL bIncrementRefCount /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMNamedNodeMap>(p, bIncrementRefCount)
{}


CXMLDOMNamedNodeMap::CXMLDOMNamedNodeMap(const CXMLDOMNamedNodeMap &o)
:	CInterfaceCallingWrapper<IXMLDOMNamedNodeMap>(o)
{}


CXMLDOMNamedNodeMap::CXMLDOMNamedNodeMap(IUnknown *pUnknown, BOOL bIncrementRefCount /*= FALSE*/, BOOL bThrowException /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMNamedNodeMap>(pUnknown, bIncrementRefCount, bThrowException)
{}


CXMLDOMNode CXMLDOMNamedNodeMap::GetNamedItem(LPCTSTR name)
{
	CString	strname(name);
	BSTR	bstrname = strname.AllocSysString();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMNamedNodeMap*)P())->getNamedItem(bstrname, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNamedNodeMap));

	SysFreeString(bstrname);
	return o;
}


CXMLDOMNode CXMLDOMNamedNodeMap::SetNamedItem(IXMLDOMNode* newItem)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMNamedNodeMap*)P())->setNamedItem(newItem, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNamedNodeMap));

	return o;
}


CXMLDOMNode CXMLDOMNamedNodeMap::RemoveNamedItem(LPCTSTR name)
{
	CString	strname(name);
	BSTR	bstrname = strname.AllocSysString();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMNamedNodeMap*)P())->removeNamedItem(bstrname, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNamedNodeMap));

	SysFreeString(bstrname);
	return o;
}


const CXMLDOMNode CXMLDOMNamedNodeMap::GetItem( long index ) const
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMNamedNodeMap*)GetP())->get_item(index, &o);
	if (hr != S_OK) AfxThrowComException(hr, GetP(), &__uuidof(IXMLDOMNamedNodeMap));

	return o;
}


long CXMLDOMNamedNodeMap::GetLength()
{
	long o;

	HRESULT	hr = ((Raw_IXMLDOMNamedNodeMap*)P())->get_length(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNamedNodeMap));

	return o;
}


CXMLDOMNode CXMLDOMNamedNodeMap::GetQualifiedItem(LPCTSTR baseName, LPCTSTR namespaceURI)
{
	CString	strbaseName(baseName);
	BSTR	bstrbaseName = strbaseName.AllocSysString();
	CString	strnamespaceURI(namespaceURI);
	BSTR	bstrnamespaceURI = strnamespaceURI.AllocSysString();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMNamedNodeMap*)P())->getQualifiedItem(bstrbaseName, bstrnamespaceURI, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNamedNodeMap));

	SysFreeString(bstrbaseName);
	SysFreeString(bstrnamespaceURI);
	return o;
}


CXMLDOMNode CXMLDOMNamedNodeMap::RemoveQualifiedItem(LPCTSTR baseName, LPCTSTR namespaceURI)
{
	CString	strbaseName(baseName);
	BSTR	bstrbaseName = strbaseName.AllocSysString();
	CString	strnamespaceURI(namespaceURI);
	BSTR	bstrnamespaceURI = strnamespaceURI.AllocSysString();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMNamedNodeMap*)P())->removeQualifiedItem(bstrbaseName, bstrnamespaceURI, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNamedNodeMap));

	SysFreeString(bstrbaseName);
	SysFreeString(bstrnamespaceURI);
	return o;
}


CXMLDOMNode CXMLDOMNamedNodeMap::NextNode()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMNamedNodeMap*)P())->nextNode(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNamedNodeMap));

	return o;
}


void CXMLDOMNamedNodeMap::Reset()
{

	HRESULT	hr = ((Raw_IXMLDOMNamedNodeMap*)P())->reset();
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNamedNodeMap));

}


IUnknown* CXMLDOMNamedNodeMap::Get_newEnum()
{
	IUnknown* o;

	HRESULT	hr = ((Raw_IXMLDOMNamedNodeMap*)P())->get__newEnum(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNamedNodeMap));

	return o;
}




//--------------------------------------------------------------------
// class CXMLDOMNode
//--------------------------------------------------------------------

CXMLDOMNode::CXMLDOMNode()
:	CInterfaceCallingWrapper<IXMLDOMNode>()
{}


CXMLDOMNode::CXMLDOMNode(IXMLDOMNode *p, BOOL bIncrementRefCount /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMNode>(p, bIncrementRefCount)
{}


CXMLDOMNode::CXMLDOMNode(const CXMLDOMNode &o)
:	CInterfaceCallingWrapper<IXMLDOMNode>(o)
{}


CXMLDOMNode::CXMLDOMNode(IUnknown *pUnknown, BOOL bIncrementRefCount /*= FALSE*/, BOOL bThrowException /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMNode>(pUnknown, bIncrementRefCount, bThrowException)
{}


const CString CXMLDOMNode::GetNodeName() const
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMNode*)GetP())->get_nodeName(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, GetP(), &__uuidof(IXMLDOMNode));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


_variant_t CXMLDOMNode::GetNodeValue()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMNode*)P())->get_nodeValue(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNode));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMNode::SetNodeValue(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMNode*)P())->put_nodeValue(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNode));

	VariantClear(&varrhs);
}


DOMNodeType CXMLDOMNode::GetNodeType()
{
	DOMNodeType o;

	HRESULT	hr = ((Raw_IXMLDOMNode*)P())->get_nodeType(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNode));

	return o;
}


CXMLDOMNode CXMLDOMNode::GetParentNode()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMNode*)P())->get_parentNode(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNode));

	return o;
}


const CXMLDOMNodeList CXMLDOMNode::GetChildNodes() const
{
	IXMLDOMNodeList* o;

	HRESULT	hr = ((Raw_IXMLDOMNode*)GetP())->get_childNodes(&o);
	if (hr != S_OK) AfxThrowComException(hr, GetP(), &__uuidof(IXMLDOMNode));

	return o;
}


CXMLDOMNode CXMLDOMNode::GetFirstChild()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMNode*)P())->get_firstChild(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNode));

	return o;
}


CXMLDOMNode CXMLDOMNode::GetLastChild()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMNode*)P())->get_lastChild(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNode));

	return o;
}


CXMLDOMNode CXMLDOMNode::GetPreviousSibling()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMNode*)P())->get_previousSibling(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNode));

	return o;
}


CXMLDOMNode CXMLDOMNode::GetNextSibling()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMNode*)P())->get_nextSibling(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNode));

	return o;
}


MsXml::CXMLDOMNamedNodeMap CXMLDOMNode::GetAttributes() const
{
	IXMLDOMNamedNodeMap* o;

	HRESULT	hr = ((Raw_IXMLDOMNode*)GetP())->get_attributes(&o);
	if (hr != S_OK) AfxThrowComException(hr, GetP(), &__uuidof(IXMLDOMNode));

	return o;
}


CXMLDOMNode CXMLDOMNode::InsertBefore(IXMLDOMNode* newChild, _variant_t refChild)
{
	VARIANT	varrefChild = refChild.Detach();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMNode*)P())->insertBefore(newChild, varrefChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNode));

	VariantClear(&varrefChild);
	return o;
}


CXMLDOMNode CXMLDOMNode::ReplaceChild(IXMLDOMNode* newChild, IXMLDOMNode* oldChild)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMNode*)P())->replaceChild(newChild, oldChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNode));

	return o;
}


CXMLDOMNode CXMLDOMNode::RemoveChild(IXMLDOMNode* childNode)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMNode*)P())->removeChild(childNode, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNode));

	return o;
}


CXMLDOMNode CXMLDOMNode::AppendChild(IXMLDOMNode* newChild)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMNode*)P())->appendChild(newChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNode));

	return o;
}


BOOL CXMLDOMNode::HasChildNodes()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMNode*)P())->hasChildNodes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNode));

	return o;
}


CXMLDOMDocument CXMLDOMNode::GetOwnerDocument()
{
	IXMLDOMDocument* o;

	HRESULT	hr = ((Raw_IXMLDOMNode*)P())->get_ownerDocument(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNode));

	return o;
}


CXMLDOMNode CXMLDOMNode::CloneNode(BOOL deep)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMNode*)P())->cloneNode(deep, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNode));

	return o;
}


const CString CXMLDOMNode::GetNodeTypeString() const
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMNode*)GetP())->get_nodeTypeString(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, GetP(), &__uuidof(IXMLDOMNode));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


const CString CXMLDOMNode::GetText() const
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMNode*)GetP())->get_text(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, GetP(), &__uuidof(IXMLDOMNode));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMNode::SetText(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMNode*)P())->put_text(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNode));

	SysFreeString(bstrrhs);
}


BOOL CXMLDOMNode::GetSpecified()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMNode*)P())->get_specified(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNode));

	return o;
}


CXMLDOMNode CXMLDOMNode::GetDefinition()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMNode*)P())->get_definition(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNode));

	return o;
}


_variant_t CXMLDOMNode::GetNodeTypedValue()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMNode*)P())->get_nodeTypedValue(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNode));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMNode::SetNodeTypedValue(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMNode*)P())->put_nodeTypedValue(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNode));

	VariantClear(&varrhs);
}


_variant_t CXMLDOMNode::GetDataType()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMNode*)P())->get_dataType(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNode));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMNode::SetDataType(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMNode*)P())->put_dataType(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNode));

	SysFreeString(bstrrhs);
}


CString CXMLDOMNode::GetXml()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMNode*)P())->get_xml(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNode));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMNode::TransformNode(IXMLDOMNode* stylesheet)
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMNode*)P())->transformNode(stylesheet, &bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNode));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CXMLDOMNodeList CXMLDOMNode::SelectNodes(LPCTSTR queryString)
{
	CString	strqueryString(queryString);
	BSTR	bstrqueryString = strqueryString.AllocSysString();
	IXMLDOMNodeList* o;

	HRESULT	hr = ((Raw_IXMLDOMNode*)P())->selectNodes(bstrqueryString, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNode));

	SysFreeString(bstrqueryString);
	return o;
}


CXMLDOMNode CXMLDOMNode::SelectSingleNode(LPCTSTR queryString)
{
	CString	strqueryString(queryString);
	BSTR	bstrqueryString = strqueryString.AllocSysString();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMNode*)P())->selectSingleNode(bstrqueryString, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNode));

	SysFreeString(bstrqueryString);
	return o;
}


BOOL CXMLDOMNode::GetParsed()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMNode*)P())->get_parsed(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNode));

	return o;
}


CString CXMLDOMNode::GetNamespaceURI()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMNode*)P())->get_namespaceURI(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNode));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMNode::GetPrefix()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMNode*)P())->get_prefix(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNode));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMNode::GetBaseName()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMNode*)P())->get_baseName(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNode));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMNode::TransformNodeToObject(IXMLDOMNode* stylesheet, _variant_t outputObject)
{
	VARIANT	varoutputObject = outputObject.Detach();

	HRESULT	hr = ((Raw_IXMLDOMNode*)P())->transformNodeToObject(stylesheet, varoutputObject);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNode));

	VariantClear(&varoutputObject);
}




//--------------------------------------------------------------------
// class CXMLDOMNodeList
//--------------------------------------------------------------------

CXMLDOMNodeList::CXMLDOMNodeList()
:	CInterfaceCallingWrapper<IXMLDOMNodeList>()
{}


CXMLDOMNodeList::CXMLDOMNodeList(IXMLDOMNodeList *p, BOOL bIncrementRefCount /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMNodeList>(p, bIncrementRefCount)
{}


CXMLDOMNodeList::CXMLDOMNodeList(const CXMLDOMNodeList &o)
:	CInterfaceCallingWrapper<IXMLDOMNodeList>(o)
{}


CXMLDOMNodeList::CXMLDOMNodeList(IUnknown *pUnknown, BOOL bIncrementRefCount /*= FALSE*/, BOOL bThrowException /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMNodeList>(pUnknown, bIncrementRefCount, bThrowException)
{}


CXMLDOMNode CXMLDOMNodeList::GetItem(long index)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMNodeList*)P())->get_item(index, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNodeList));

	return o;
}


long CXMLDOMNodeList::GetLength() const
{
	long o;

	HRESULT	hr = ((Raw_IXMLDOMNodeList*)GetP())->get_length(&o);
	if (hr != S_OK) AfxThrowComException(hr, GetP(), &__uuidof(IXMLDOMNodeList));

	return o;
}


CXMLDOMNode CXMLDOMNodeList::NextNode()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMNodeList*)P())->nextNode(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNodeList));

	return o;
}


void CXMLDOMNodeList::Reset()
{

	HRESULT	hr = ((Raw_IXMLDOMNodeList*)P())->reset();
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNodeList));

}


IUnknown* CXMLDOMNodeList::Get_newEnum()
{
	IUnknown* o;

	HRESULT	hr = ((Raw_IXMLDOMNodeList*)P())->get__newEnum(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNodeList));

	return o;
}




//--------------------------------------------------------------------
// class CXMLDOMParseError
//--------------------------------------------------------------------

CXMLDOMParseError::CXMLDOMParseError()
:	CInterfaceCallingWrapper<IXMLDOMParseError>()
{}


CXMLDOMParseError::CXMLDOMParseError(IXMLDOMParseError *p, BOOL bIncrementRefCount /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMParseError>(p, bIncrementRefCount)
{}


CXMLDOMParseError::CXMLDOMParseError(const CXMLDOMParseError &o)
:	CInterfaceCallingWrapper<IXMLDOMParseError>(o)
{}


CXMLDOMParseError::CXMLDOMParseError(IUnknown *pUnknown, BOOL bIncrementRefCount /*= FALSE*/, BOOL bThrowException /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMParseError>(pUnknown, bIncrementRefCount, bThrowException)
{}


long CXMLDOMParseError::GetErrorCode()
{
	long o;

	HRESULT	hr = ((Raw_IXMLDOMParseError*)P())->get_errorCode(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMParseError));

	return o;
}


CString CXMLDOMParseError::GetUrl()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMParseError*)P())->get_url(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMParseError));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMParseError::GetReason()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMParseError*)P())->get_reason(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMParseError));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMParseError::GetSrcText()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMParseError*)P())->get_srcText(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMParseError));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


long CXMLDOMParseError::GetLine()
{
	long o;

	HRESULT	hr = ((Raw_IXMLDOMParseError*)P())->get_line(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMParseError));

	return o;
}


long CXMLDOMParseError::GetLinepos()
{
	long o;

	HRESULT	hr = ((Raw_IXMLDOMParseError*)P())->get_linepos(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMParseError));

	return o;
}


long CXMLDOMParseError::GetFilepos()
{
	long o;

	HRESULT	hr = ((Raw_IXMLDOMParseError*)P())->get_filepos(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMParseError));

	return o;
}




//--------------------------------------------------------------------
// class CXMLHTTPRequest
//--------------------------------------------------------------------

CXMLHTTPRequest::CXMLHTTPRequest()
:	CInterfaceCallingWrapper<IXMLHTTPRequest>()
{}


CXMLHTTPRequest::CXMLHTTPRequest(IXMLHTTPRequest *p, BOOL bIncrementRefCount /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLHTTPRequest>(p, bIncrementRefCount)
{}


CXMLHTTPRequest::CXMLHTTPRequest(const CXMLHTTPRequest &o)
:	CInterfaceCallingWrapper<IXMLHTTPRequest>(o)
{}


CXMLHTTPRequest::CXMLHTTPRequest(IUnknown *pUnknown, BOOL bIncrementRefCount /*= FALSE*/, BOOL bThrowException /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLHTTPRequest>(pUnknown, bIncrementRefCount, bThrowException)
{}


void CXMLHTTPRequest::Open(LPCTSTR bstrMethod, LPCTSTR bstrUrl, _variant_t varAsync, _variant_t bstrUser, _variant_t bstrPassword)
{
	CString	strbstrMethod(bstrMethod);
	BSTR	bstrbstrMethod = strbstrMethod.AllocSysString();
	CString	strbstrUrl(bstrUrl);
	BSTR	bstrbstrUrl = strbstrUrl.AllocSysString();
	VARIANT	varvarAsync = varAsync.Detach();
	VARIANT	varbstrUser = bstrUser.Detach();
	VARIANT	varbstrPassword = bstrPassword.Detach();

	HRESULT	hr = ((Raw_IXMLHTTPRequest*)P())->open(bstrbstrMethod, bstrbstrUrl, varvarAsync, varbstrUser, varbstrPassword);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLHTTPRequest));

	SysFreeString(bstrbstrMethod);
	SysFreeString(bstrbstrUrl);
	VariantClear(&varvarAsync);
	VariantClear(&varbstrUser);
	VariantClear(&varbstrPassword);
}


void CXMLHTTPRequest::SetRequestHeader(LPCTSTR bstrHeader, LPCTSTR bstrValue)
{
	CString	strbstrHeader(bstrHeader);
	BSTR	bstrbstrHeader = strbstrHeader.AllocSysString();
	CString	strbstrValue(bstrValue);
	BSTR	bstrbstrValue = strbstrValue.AllocSysString();

	HRESULT	hr = ((Raw_IXMLHTTPRequest*)P())->setRequestHeader(bstrbstrHeader, bstrbstrValue);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLHTTPRequest));

	SysFreeString(bstrbstrHeader);
	SysFreeString(bstrbstrValue);
}


CString CXMLHTTPRequest::GetResponseHeader(LPCTSTR bstrHeader)
{
	CString	strbstrHeader(bstrHeader);
	BSTR	bstrbstrHeader = strbstrHeader.AllocSysString();
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLHTTPRequest*)P())->getResponseHeader(bstrbstrHeader, &bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLHTTPRequest));

	SysFreeString(bstrbstrHeader);
	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLHTTPRequest::GetAllResponseHeaders()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLHTTPRequest*)P())->getAllResponseHeaders(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLHTTPRequest));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLHTTPRequest::Send(_variant_t varBody)
{
	VARIANT	varvarBody = varBody.Detach();

	HRESULT	hr = ((Raw_IXMLHTTPRequest*)P())->send(varvarBody);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLHTTPRequest));

	VariantClear(&varvarBody);
}


void CXMLHTTPRequest::Abort()
{

	HRESULT	hr = ((Raw_IXMLHTTPRequest*)P())->abort();
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLHTTPRequest));

}


long CXMLHTTPRequest::GetStatus()
{
	long o;

	HRESULT	hr = ((Raw_IXMLHTTPRequest*)P())->get_status(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLHTTPRequest));

	return o;
}


CString CXMLHTTPRequest::GetStatusText()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLHTTPRequest*)P())->get_statusText(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLHTTPRequest));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


IDispatch* CXMLHTTPRequest::GetResponseXML()
{
	IDispatch* o;

	HRESULT	hr = ((Raw_IXMLHTTPRequest*)P())->get_responseXML(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLHTTPRequest));

	return o;
}


CString CXMLHTTPRequest::GetResponseText()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLHTTPRequest*)P())->get_responseText(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLHTTPRequest));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


_variant_t CXMLHTTPRequest::GetResponseBody()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLHTTPRequest*)P())->get_responseBody(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLHTTPRequest));

	_variant_t	o(varO, false);
	return o;
}


_variant_t CXMLHTTPRequest::GetResponseStream()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLHTTPRequest*)P())->get_responseStream(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLHTTPRequest));

	_variant_t	o(varO, false);
	return o;
}


long CXMLHTTPRequest::GetReadyState()
{
	long o;

	HRESULT	hr = ((Raw_IXMLHTTPRequest*)P())->get_readyState(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLHTTPRequest));

	return o;
}


void CXMLHTTPRequest::SetOnreadystatechange(IDispatch* rhs)
{

	HRESULT	hr = ((Raw_IXMLHTTPRequest*)P())->put_onreadystatechange(rhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLHTTPRequest));

}




//--------------------------------------------------------------------
// class CServerXMLHTTPRequest
//--------------------------------------------------------------------

CServerXMLHTTPRequest::CServerXMLHTTPRequest()
:	CInterfaceCallingWrapper<IServerXMLHTTPRequest>()
{}


CServerXMLHTTPRequest::CServerXMLHTTPRequest(IServerXMLHTTPRequest *p, BOOL bIncrementRefCount /*= FALSE*/)
:	CInterfaceCallingWrapper<IServerXMLHTTPRequest>(p, bIncrementRefCount)
{}


CServerXMLHTTPRequest::CServerXMLHTTPRequest(const CServerXMLHTTPRequest &o)
:	CInterfaceCallingWrapper<IServerXMLHTTPRequest>(o)
{}


CServerXMLHTTPRequest::CServerXMLHTTPRequest(IUnknown *pUnknown, BOOL bIncrementRefCount /*= FALSE*/, BOOL bThrowException /*= FALSE*/)
:	CInterfaceCallingWrapper<IServerXMLHTTPRequest>(pUnknown, bIncrementRefCount, bThrowException)
{}


void CServerXMLHTTPRequest::Open(LPCTSTR bstrMethod, LPCTSTR bstrUrl, _variant_t varAsync, _variant_t bstrUser, _variant_t bstrPassword)
{
	CString	strbstrMethod(bstrMethod);
	BSTR	bstrbstrMethod = strbstrMethod.AllocSysString();
	CString	strbstrUrl(bstrUrl);
	BSTR	bstrbstrUrl = strbstrUrl.AllocSysString();
	VARIANT	varvarAsync = varAsync.Detach();
	VARIANT	varbstrUser = bstrUser.Detach();
	VARIANT	varbstrPassword = bstrPassword.Detach();

	HRESULT	hr = ((Raw_IServerXMLHTTPRequest*)P())->open(bstrbstrMethod, bstrbstrUrl, varvarAsync, varbstrUser, varbstrPassword);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IServerXMLHTTPRequest));

	SysFreeString(bstrbstrMethod);
	SysFreeString(bstrbstrUrl);
	VariantClear(&varvarAsync);
	VariantClear(&varbstrUser);
	VariantClear(&varbstrPassword);
}


void CServerXMLHTTPRequest::SetRequestHeader(LPCTSTR bstrHeader, LPCTSTR bstrValue)
{
	CString	strbstrHeader(bstrHeader);
	BSTR	bstrbstrHeader = strbstrHeader.AllocSysString();
	CString	strbstrValue(bstrValue);
	BSTR	bstrbstrValue = strbstrValue.AllocSysString();

	HRESULT	hr = ((Raw_IServerXMLHTTPRequest*)P())->setRequestHeader(bstrbstrHeader, bstrbstrValue);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IServerXMLHTTPRequest));

	SysFreeString(bstrbstrHeader);
	SysFreeString(bstrbstrValue);
}


CString CServerXMLHTTPRequest::GetResponseHeader(LPCTSTR bstrHeader)
{
	CString	strbstrHeader(bstrHeader);
	BSTR	bstrbstrHeader = strbstrHeader.AllocSysString();
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IServerXMLHTTPRequest*)P())->getResponseHeader(bstrbstrHeader, &bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IServerXMLHTTPRequest));

	SysFreeString(bstrbstrHeader);
	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CServerXMLHTTPRequest::GetAllResponseHeaders()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IServerXMLHTTPRequest*)P())->getAllResponseHeaders(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IServerXMLHTTPRequest));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CServerXMLHTTPRequest::Send(_variant_t varBody)
{
	VARIANT	varvarBody = varBody.Detach();

	HRESULT	hr = ((Raw_IServerXMLHTTPRequest*)P())->send(varvarBody);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IServerXMLHTTPRequest));

	VariantClear(&varvarBody);
}


void CServerXMLHTTPRequest::Abort()
{

	HRESULT	hr = ((Raw_IServerXMLHTTPRequest*)P())->abort();
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IServerXMLHTTPRequest));

}


long CServerXMLHTTPRequest::GetStatus()
{
	long o;

	HRESULT	hr = ((Raw_IServerXMLHTTPRequest*)P())->get_status(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IServerXMLHTTPRequest));

	return o;
}


CString CServerXMLHTTPRequest::GetStatusText()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IServerXMLHTTPRequest*)P())->get_statusText(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IServerXMLHTTPRequest));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


IDispatch* CServerXMLHTTPRequest::GetResponseXML()
{
	IDispatch* o;

	HRESULT	hr = ((Raw_IServerXMLHTTPRequest*)P())->get_responseXML(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IServerXMLHTTPRequest));

	return o;
}


CString CServerXMLHTTPRequest::GetResponseText()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IServerXMLHTTPRequest*)P())->get_responseText(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IServerXMLHTTPRequest));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


_variant_t CServerXMLHTTPRequest::GetResponseBody()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IServerXMLHTTPRequest*)P())->get_responseBody(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IServerXMLHTTPRequest));

	_variant_t	o(varO, false);
	return o;
}


_variant_t CServerXMLHTTPRequest::GetResponseStream()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IServerXMLHTTPRequest*)P())->get_responseStream(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IServerXMLHTTPRequest));

	_variant_t	o(varO, false);
	return o;
}


long CServerXMLHTTPRequest::GetReadyState()
{
	long o;

	HRESULT	hr = ((Raw_IServerXMLHTTPRequest*)P())->get_readyState(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IServerXMLHTTPRequest));

	return o;
}


void CServerXMLHTTPRequest::SetOnreadystatechange(IDispatch* rhs)
{

	HRESULT	hr = ((Raw_IServerXMLHTTPRequest*)P())->put_onreadystatechange(rhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IServerXMLHTTPRequest));

}


void CServerXMLHTTPRequest::SetTimeouts(long resolveTimeout, long connectTimeout, long sendTimeout, long receiveTimeout)
{

	HRESULT	hr = ((Raw_IServerXMLHTTPRequest*)P())->setTimeouts(resolveTimeout, connectTimeout, sendTimeout, receiveTimeout);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IServerXMLHTTPRequest));

}


BOOL CServerXMLHTTPRequest::WaitForResponse(_variant_t timeoutInSeconds)
{
	VARIANT	vartimeoutInSeconds = timeoutInSeconds.Detach();
	short	o;

	HRESULT	hr = ((Raw_IServerXMLHTTPRequest*)P())->waitForResponse(vartimeoutInSeconds, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IServerXMLHTTPRequest));

	VariantClear(&vartimeoutInSeconds);
	return o;
}


_variant_t CServerXMLHTTPRequest::GetOption(SERVERXMLHTTP_OPTION option)
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IServerXMLHTTPRequest*)P())->getOption(option, &varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IServerXMLHTTPRequest));

	_variant_t	o(varO, false);
	return o;
}


void CServerXMLHTTPRequest::SetOption(SERVERXMLHTTP_OPTION option, _variant_t value)
{
	VARIANT	varvalue = value.Detach();

	HRESULT	hr = ((Raw_IServerXMLHTTPRequest*)P())->setOption(option, varvalue);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IServerXMLHTTPRequest));

	VariantClear(&varvalue);
}




//--------------------------------------------------------------------
// class CXMLDOMAttribute
//--------------------------------------------------------------------

CXMLDOMAttribute::CXMLDOMAttribute()
:	CInterfaceCallingWrapper<IXMLDOMAttribute>()
{}


CXMLDOMAttribute::CXMLDOMAttribute(IXMLDOMAttribute *p, BOOL bIncrementRefCount /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMAttribute>(p, bIncrementRefCount)
{}


CXMLDOMAttribute::CXMLDOMAttribute(const CXMLDOMAttribute &o)
:	CInterfaceCallingWrapper<IXMLDOMAttribute>(o)
{}


CXMLDOMAttribute::CXMLDOMAttribute(IUnknown *pUnknown, BOOL bIncrementRefCount /*= FALSE*/, BOOL bThrowException /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMAttribute>(pUnknown, bIncrementRefCount, bThrowException)
{}


CString CXMLDOMAttribute::GetNodeName()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->get_nodeName(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


_variant_t CXMLDOMAttribute::GetNodeValue()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->get_nodeValue(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMAttribute::SetNodeValue(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->put_nodeValue(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	VariantClear(&varrhs);
}


DOMNodeType CXMLDOMAttribute::GetNodeType()
{
	DOMNodeType o;

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->get_nodeType(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	return o;
}


CXMLDOMNode CXMLDOMAttribute::GetParentNode()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->get_parentNode(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	return o;
}


CXMLDOMNodeList CXMLDOMAttribute::GetChildNodes()
{
	IXMLDOMNodeList* o;

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->get_childNodes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	return o;
}


CXMLDOMNode CXMLDOMAttribute::GetFirstChild()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->get_firstChild(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	return o;
}


CXMLDOMNode CXMLDOMAttribute::GetLastChild()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->get_lastChild(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	return o;
}


CXMLDOMNode CXMLDOMAttribute::GetPreviousSibling()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->get_previousSibling(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	return o;
}


CXMLDOMNode CXMLDOMAttribute::GetNextSibling()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->get_nextSibling(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	return o;
}


CXMLDOMNamedNodeMap CXMLDOMAttribute::GetAttributes()
{
	IXMLDOMNamedNodeMap* o;

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->get_attributes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	return o;
}


CXMLDOMNode CXMLDOMAttribute::InsertBefore(IXMLDOMNode* newChild, _variant_t refChild)
{
	VARIANT	varrefChild = refChild.Detach();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->insertBefore(newChild, varrefChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	VariantClear(&varrefChild);
	return o;
}


CXMLDOMNode CXMLDOMAttribute::ReplaceChild(IXMLDOMNode* newChild, IXMLDOMNode* oldChild)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->replaceChild(newChild, oldChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	return o;
}


CXMLDOMNode CXMLDOMAttribute::RemoveChild(IXMLDOMNode* childNode)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->removeChild(childNode, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	return o;
}


CXMLDOMNode CXMLDOMAttribute::AppendChild(IXMLDOMNode* newChild)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->appendChild(newChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	return o;
}


BOOL CXMLDOMAttribute::HasChildNodes()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->hasChildNodes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	return o;
}


CXMLDOMDocument CXMLDOMAttribute::GetOwnerDocument()
{
	IXMLDOMDocument* o;

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->get_ownerDocument(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	return o;
}


CXMLDOMNode CXMLDOMAttribute::CloneNode(BOOL deep)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->cloneNode(deep, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	return o;
}


CString CXMLDOMAttribute::GetNodeTypeString()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->get_nodeTypeString(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMAttribute::GetText()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->get_text(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMAttribute::SetText(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->put_text(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	SysFreeString(bstrrhs);
}


BOOL CXMLDOMAttribute::GetSpecified()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->get_specified(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	return o;
}


CXMLDOMNode CXMLDOMAttribute::GetDefinition()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->get_definition(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	return o;
}


_variant_t CXMLDOMAttribute::GetNodeTypedValue()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->get_nodeTypedValue(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMAttribute::SetNodeTypedValue(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->put_nodeTypedValue(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	VariantClear(&varrhs);
}


_variant_t CXMLDOMAttribute::GetDataType()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->get_dataType(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMAttribute::SetDataType(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->put_dataType(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	SysFreeString(bstrrhs);
}


CString CXMLDOMAttribute::GetXml()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->get_xml(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMAttribute::TransformNode(IXMLDOMNode* stylesheet)
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->transformNode(stylesheet, &bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CXMLDOMNodeList CXMLDOMAttribute::SelectNodes(LPCTSTR queryString)
{
	CString	strqueryString(queryString);
	BSTR	bstrqueryString = strqueryString.AllocSysString();
	IXMLDOMNodeList* o;

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->selectNodes(bstrqueryString, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	SysFreeString(bstrqueryString);
	return o;
}


CXMLDOMNode CXMLDOMAttribute::SelectSingleNode(LPCTSTR queryString)
{
	CString	strqueryString(queryString);
	BSTR	bstrqueryString = strqueryString.AllocSysString();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->selectSingleNode(bstrqueryString, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	SysFreeString(bstrqueryString);
	return o;
}


BOOL CXMLDOMAttribute::GetParsed()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->get_parsed(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	return o;
}


CString CXMLDOMAttribute::GetNamespaceURI()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->get_namespaceURI(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMAttribute::GetPrefix()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->get_prefix(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMAttribute::GetBaseName()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->get_baseName(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMAttribute::TransformNodeToObject(IXMLDOMNode* stylesheet, _variant_t outputObject)
{
	VARIANT	varoutputObject = outputObject.Detach();

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->transformNodeToObject(stylesheet, varoutputObject);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	VariantClear(&varoutputObject);
}


CString CXMLDOMAttribute::GetName()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->get_name(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


_variant_t CXMLDOMAttribute::GetValue()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->get_value(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMAttribute::SetValue(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMAttribute*)P())->put_value(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMAttribute));

	VariantClear(&varrhs);
}




//--------------------------------------------------------------------
// class CXMLDOMCDATASection
//--------------------------------------------------------------------

CXMLDOMCDATASection::CXMLDOMCDATASection()
:	CInterfaceCallingWrapper<IXMLDOMCDATASection>()
{}


CXMLDOMCDATASection::CXMLDOMCDATASection(IXMLDOMCDATASection *p, BOOL bIncrementRefCount /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMCDATASection>(p, bIncrementRefCount)
{}


CXMLDOMCDATASection::CXMLDOMCDATASection(const CXMLDOMCDATASection &o)
:	CInterfaceCallingWrapper<IXMLDOMCDATASection>(o)
{}


CXMLDOMCDATASection::CXMLDOMCDATASection(IUnknown *pUnknown, BOOL bIncrementRefCount /*= FALSE*/, BOOL bThrowException /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMCDATASection>(pUnknown, bIncrementRefCount, bThrowException)
{}


CString CXMLDOMCDATASection::GetNodeName()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->get_nodeName(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


_variant_t CXMLDOMCDATASection::GetNodeValue()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->get_nodeValue(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMCDATASection::SetNodeValue(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->put_nodeValue(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	VariantClear(&varrhs);
}


DOMNodeType CXMLDOMCDATASection::GetNodeType()
{
	DOMNodeType o;

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->get_nodeType(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	return o;
}


CXMLDOMNode CXMLDOMCDATASection::GetParentNode()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->get_parentNode(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	return o;
}


CXMLDOMNodeList CXMLDOMCDATASection::GetChildNodes()
{
	IXMLDOMNodeList* o;

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->get_childNodes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	return o;
}


CXMLDOMNode CXMLDOMCDATASection::GetFirstChild()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->get_firstChild(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	return o;
}


CXMLDOMNode CXMLDOMCDATASection::GetLastChild()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->get_lastChild(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	return o;
}


CXMLDOMNode CXMLDOMCDATASection::GetPreviousSibling()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->get_previousSibling(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	return o;
}


CXMLDOMNode CXMLDOMCDATASection::GetNextSibling()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->get_nextSibling(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	return o;
}


CXMLDOMNamedNodeMap CXMLDOMCDATASection::GetAttributes()
{
	IXMLDOMNamedNodeMap* o;

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->get_attributes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	return o;
}


CXMLDOMNode CXMLDOMCDATASection::InsertBefore(IXMLDOMNode* newChild, _variant_t refChild)
{
	VARIANT	varrefChild = refChild.Detach();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->insertBefore(newChild, varrefChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	VariantClear(&varrefChild);
	return o;
}


CXMLDOMNode CXMLDOMCDATASection::ReplaceChild(IXMLDOMNode* newChild, IXMLDOMNode* oldChild)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->replaceChild(newChild, oldChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	return o;
}


CXMLDOMNode CXMLDOMCDATASection::RemoveChild(IXMLDOMNode* childNode)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->removeChild(childNode, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	return o;
}


CXMLDOMNode CXMLDOMCDATASection::AppendChild(IXMLDOMNode* newChild)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->appendChild(newChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	return o;
}


BOOL CXMLDOMCDATASection::HasChildNodes()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->hasChildNodes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	return o;
}


CXMLDOMDocument CXMLDOMCDATASection::GetOwnerDocument()
{
	IXMLDOMDocument* o;

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->get_ownerDocument(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	return o;
}


CXMLDOMNode CXMLDOMCDATASection::CloneNode(BOOL deep)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->cloneNode(deep, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	return o;
}


CString CXMLDOMCDATASection::GetNodeTypeString()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->get_nodeTypeString(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMCDATASection::GetText()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->get_text(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMCDATASection::SetText(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->put_text(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	SysFreeString(bstrrhs);
}


BOOL CXMLDOMCDATASection::GetSpecified()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->get_specified(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	return o;
}


CXMLDOMNode CXMLDOMCDATASection::GetDefinition()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->get_definition(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	return o;
}


_variant_t CXMLDOMCDATASection::GetNodeTypedValue()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->get_nodeTypedValue(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMCDATASection::SetNodeTypedValue(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->put_nodeTypedValue(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	VariantClear(&varrhs);
}


_variant_t CXMLDOMCDATASection::GetDataType()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->get_dataType(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMCDATASection::SetDataType(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->put_dataType(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	SysFreeString(bstrrhs);
}


CString CXMLDOMCDATASection::GetXml()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->get_xml(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMCDATASection::TransformNode(IXMLDOMNode* stylesheet)
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->transformNode(stylesheet, &bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CXMLDOMNodeList CXMLDOMCDATASection::SelectNodes(LPCTSTR queryString)
{
	CString	strqueryString(queryString);
	BSTR	bstrqueryString = strqueryString.AllocSysString();
	IXMLDOMNodeList* o;

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->selectNodes(bstrqueryString, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	SysFreeString(bstrqueryString);
	return o;
}


CXMLDOMNode CXMLDOMCDATASection::SelectSingleNode(LPCTSTR queryString)
{
	CString	strqueryString(queryString);
	BSTR	bstrqueryString = strqueryString.AllocSysString();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->selectSingleNode(bstrqueryString, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	SysFreeString(bstrqueryString);
	return o;
}


BOOL CXMLDOMCDATASection::GetParsed()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->get_parsed(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	return o;
}


CString CXMLDOMCDATASection::GetNamespaceURI()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->get_namespaceURI(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMCDATASection::GetPrefix()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->get_prefix(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMCDATASection::GetBaseName()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->get_baseName(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMCDATASection::TransformNodeToObject(IXMLDOMNode* stylesheet, _variant_t outputObject)
{
	VARIANT	varoutputObject = outputObject.Detach();

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->transformNodeToObject(stylesheet, varoutputObject);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	VariantClear(&varoutputObject);
}


CString CXMLDOMCDATASection::GetData()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->get_data(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMCDATASection::SetData(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->put_data(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	SysFreeString(bstrrhs);
}


long CXMLDOMCDATASection::GetLength()
{
	long o;

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->get_length(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	return o;
}


CString CXMLDOMCDATASection::SubstringData(long offset, long count)
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->substringData(offset, count, &bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMCDATASection::AppendData(LPCTSTR data)
{
	CString	strdata(data);
	BSTR	bstrdata = strdata.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->appendData(bstrdata);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	SysFreeString(bstrdata);
}


void CXMLDOMCDATASection::InsertData(long offset, LPCTSTR data)
{
	CString	strdata(data);
	BSTR	bstrdata = strdata.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->insertData(offset, bstrdata);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	SysFreeString(bstrdata);
}


void CXMLDOMCDATASection::DeleteData(long offset, long count)
{

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->deleteData(offset, count);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

}


void CXMLDOMCDATASection::ReplaceData(long offset, long count, LPCTSTR data)
{
	CString	strdata(data);
	BSTR	bstrdata = strdata.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->replaceData(offset, count, bstrdata);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	SysFreeString(bstrdata);
}


CXMLDOMText CXMLDOMCDATASection::SplitText(long offset)
{
	IXMLDOMText* o;

	HRESULT	hr = ((Raw_IXMLDOMCDATASection*)P())->splitText(offset, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCDATASection));

	return o;
}




//--------------------------------------------------------------------
// class CXMLDOMCharacterData
//--------------------------------------------------------------------

CXMLDOMCharacterData::CXMLDOMCharacterData()
:	CInterfaceCallingWrapper<IXMLDOMCharacterData>()
{}


CXMLDOMCharacterData::CXMLDOMCharacterData(IXMLDOMCharacterData *p, BOOL bIncrementRefCount /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMCharacterData>(p, bIncrementRefCount)
{}


CXMLDOMCharacterData::CXMLDOMCharacterData(const CXMLDOMCharacterData &o)
:	CInterfaceCallingWrapper<IXMLDOMCharacterData>(o)
{}


CXMLDOMCharacterData::CXMLDOMCharacterData(IUnknown *pUnknown, BOOL bIncrementRefCount /*= FALSE*/, BOOL bThrowException /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMCharacterData>(pUnknown, bIncrementRefCount, bThrowException)
{}


CString CXMLDOMCharacterData::GetNodeName()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->get_nodeName(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


_variant_t CXMLDOMCharacterData::GetNodeValue()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->get_nodeValue(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMCharacterData::SetNodeValue(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->put_nodeValue(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	VariantClear(&varrhs);
}


DOMNodeType CXMLDOMCharacterData::GetNodeType()
{
	DOMNodeType o;

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->get_nodeType(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	return o;
}


CXMLDOMNode CXMLDOMCharacterData::GetParentNode()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->get_parentNode(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	return o;
}


CXMLDOMNodeList CXMLDOMCharacterData::GetChildNodes()
{
	IXMLDOMNodeList* o;

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->get_childNodes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	return o;
}


CXMLDOMNode CXMLDOMCharacterData::GetFirstChild()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->get_firstChild(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	return o;
}


CXMLDOMNode CXMLDOMCharacterData::GetLastChild()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->get_lastChild(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	return o;
}


CXMLDOMNode CXMLDOMCharacterData::GetPreviousSibling()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->get_previousSibling(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	return o;
}


CXMLDOMNode CXMLDOMCharacterData::GetNextSibling()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->get_nextSibling(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	return o;
}


CXMLDOMNamedNodeMap CXMLDOMCharacterData::GetAttributes()
{
	IXMLDOMNamedNodeMap* o;

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->get_attributes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	return o;
}


CXMLDOMNode CXMLDOMCharacterData::InsertBefore(IXMLDOMNode* newChild, _variant_t refChild)
{
	VARIANT	varrefChild = refChild.Detach();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->insertBefore(newChild, varrefChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	VariantClear(&varrefChild);
	return o;
}


CXMLDOMNode CXMLDOMCharacterData::ReplaceChild(IXMLDOMNode* newChild, IXMLDOMNode* oldChild)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->replaceChild(newChild, oldChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	return o;
}


CXMLDOMNode CXMLDOMCharacterData::RemoveChild(IXMLDOMNode* childNode)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->removeChild(childNode, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	return o;
}


CXMLDOMNode CXMLDOMCharacterData::AppendChild(IXMLDOMNode* newChild)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->appendChild(newChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	return o;
}


BOOL CXMLDOMCharacterData::HasChildNodes()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->hasChildNodes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	return o;
}


CXMLDOMDocument CXMLDOMCharacterData::GetOwnerDocument()
{
	IXMLDOMDocument* o;

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->get_ownerDocument(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	return o;
}


CXMLDOMNode CXMLDOMCharacterData::CloneNode(BOOL deep)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->cloneNode(deep, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	return o;
}


CString CXMLDOMCharacterData::GetNodeTypeString()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->get_nodeTypeString(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMCharacterData::GetText()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->get_text(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMCharacterData::SetText(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->put_text(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	SysFreeString(bstrrhs);
}


BOOL CXMLDOMCharacterData::GetSpecified()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->get_specified(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	return o;
}


CXMLDOMNode CXMLDOMCharacterData::GetDefinition()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->get_definition(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	return o;
}


_variant_t CXMLDOMCharacterData::GetNodeTypedValue()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->get_nodeTypedValue(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMCharacterData::SetNodeTypedValue(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->put_nodeTypedValue(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	VariantClear(&varrhs);
}


_variant_t CXMLDOMCharacterData::GetDataType()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->get_dataType(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMCharacterData::SetDataType(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->put_dataType(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	SysFreeString(bstrrhs);
}


CString CXMLDOMCharacterData::GetXml()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->get_xml(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMCharacterData::TransformNode(IXMLDOMNode* stylesheet)
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->transformNode(stylesheet, &bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CXMLDOMNodeList CXMLDOMCharacterData::SelectNodes(LPCTSTR queryString)
{
	CString	strqueryString(queryString);
	BSTR	bstrqueryString = strqueryString.AllocSysString();
	IXMLDOMNodeList* o;

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->selectNodes(bstrqueryString, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	SysFreeString(bstrqueryString);
	return o;
}


CXMLDOMNode CXMLDOMCharacterData::SelectSingleNode(LPCTSTR queryString)
{
	CString	strqueryString(queryString);
	BSTR	bstrqueryString = strqueryString.AllocSysString();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->selectSingleNode(bstrqueryString, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	SysFreeString(bstrqueryString);
	return o;
}


BOOL CXMLDOMCharacterData::GetParsed()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->get_parsed(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	return o;
}


CString CXMLDOMCharacterData::GetNamespaceURI()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->get_namespaceURI(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMCharacterData::GetPrefix()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->get_prefix(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMCharacterData::GetBaseName()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->get_baseName(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMCharacterData::TransformNodeToObject(IXMLDOMNode* stylesheet, _variant_t outputObject)
{
	VARIANT	varoutputObject = outputObject.Detach();

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->transformNodeToObject(stylesheet, varoutputObject);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	VariantClear(&varoutputObject);
}


CString CXMLDOMCharacterData::GetData()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->get_data(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMCharacterData::SetData(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->put_data(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	SysFreeString(bstrrhs);
}


long CXMLDOMCharacterData::GetLength()
{
	long o;

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->get_length(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	return o;
}


CString CXMLDOMCharacterData::SubstringData(long offset, long count)
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->substringData(offset, count, &bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMCharacterData::AppendData(LPCTSTR data)
{
	CString	strdata(data);
	BSTR	bstrdata = strdata.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->appendData(bstrdata);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	SysFreeString(bstrdata);
}


void CXMLDOMCharacterData::InsertData(long offset, LPCTSTR data)
{
	CString	strdata(data);
	BSTR	bstrdata = strdata.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->insertData(offset, bstrdata);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	SysFreeString(bstrdata);
}


void CXMLDOMCharacterData::DeleteData(long offset, long count)
{

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->deleteData(offset, count);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

}


void CXMLDOMCharacterData::ReplaceData(long offset, long count, LPCTSTR data)
{
	CString	strdata(data);
	BSTR	bstrdata = strdata.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMCharacterData*)P())->replaceData(offset, count, bstrdata);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMCharacterData));

	SysFreeString(bstrdata);
}




//--------------------------------------------------------------------
// class CXMLDOMComment
//--------------------------------------------------------------------

CXMLDOMComment::CXMLDOMComment()
:	CInterfaceCallingWrapper<IXMLDOMComment>()
{}


CXMLDOMComment::CXMLDOMComment(IXMLDOMComment *p, BOOL bIncrementRefCount /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMComment>(p, bIncrementRefCount)
{}


CXMLDOMComment::CXMLDOMComment(const CXMLDOMComment &o)
:	CInterfaceCallingWrapper<IXMLDOMComment>(o)
{}


CXMLDOMComment::CXMLDOMComment(IUnknown *pUnknown, BOOL bIncrementRefCount /*= FALSE*/, BOOL bThrowException /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMComment>(pUnknown, bIncrementRefCount, bThrowException)
{}


CString CXMLDOMComment::GetNodeName()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->get_nodeName(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


_variant_t CXMLDOMComment::GetNodeValue()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->get_nodeValue(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMComment::SetNodeValue(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->put_nodeValue(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	VariantClear(&varrhs);
}


DOMNodeType CXMLDOMComment::GetNodeType()
{
	DOMNodeType o;

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->get_nodeType(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	return o;
}


CXMLDOMNode CXMLDOMComment::GetParentNode()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->get_parentNode(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	return o;
}


CXMLDOMNodeList CXMLDOMComment::GetChildNodes()
{
	IXMLDOMNodeList* o;

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->get_childNodes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	return o;
}


CXMLDOMNode CXMLDOMComment::GetFirstChild()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->get_firstChild(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	return o;
}


CXMLDOMNode CXMLDOMComment::GetLastChild()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->get_lastChild(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	return o;
}


CXMLDOMNode CXMLDOMComment::GetPreviousSibling()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->get_previousSibling(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	return o;
}


CXMLDOMNode CXMLDOMComment::GetNextSibling()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->get_nextSibling(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	return o;
}


CXMLDOMNamedNodeMap CXMLDOMComment::GetAttributes()
{
	IXMLDOMNamedNodeMap* o;

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->get_attributes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	return o;
}


CXMLDOMNode CXMLDOMComment::InsertBefore(IXMLDOMNode* newChild, _variant_t refChild)
{
	VARIANT	varrefChild = refChild.Detach();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->insertBefore(newChild, varrefChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	VariantClear(&varrefChild);
	return o;
}


CXMLDOMNode CXMLDOMComment::ReplaceChild(IXMLDOMNode* newChild, IXMLDOMNode* oldChild)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->replaceChild(newChild, oldChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	return o;
}


CXMLDOMNode CXMLDOMComment::RemoveChild(IXMLDOMNode* childNode)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->removeChild(childNode, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	return o;
}


CXMLDOMNode CXMLDOMComment::AppendChild(IXMLDOMNode* newChild)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->appendChild(newChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	return o;
}


BOOL CXMLDOMComment::HasChildNodes()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->hasChildNodes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	return o;
}


CXMLDOMDocument CXMLDOMComment::GetOwnerDocument()
{
	IXMLDOMDocument* o;

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->get_ownerDocument(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	return o;
}


CXMLDOMNode CXMLDOMComment::CloneNode(BOOL deep)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->cloneNode(deep, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	return o;
}


CString CXMLDOMComment::GetNodeTypeString()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->get_nodeTypeString(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMComment::GetText()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->get_text(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMComment::SetText(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->put_text(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	SysFreeString(bstrrhs);
}


BOOL CXMLDOMComment::GetSpecified()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->get_specified(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	return o;
}


CXMLDOMNode CXMLDOMComment::GetDefinition()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->get_definition(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	return o;
}


_variant_t CXMLDOMComment::GetNodeTypedValue()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->get_nodeTypedValue(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMComment::SetNodeTypedValue(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->put_nodeTypedValue(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	VariantClear(&varrhs);
}


_variant_t CXMLDOMComment::GetDataType()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->get_dataType(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMComment::SetDataType(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->put_dataType(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	SysFreeString(bstrrhs);
}


CString CXMLDOMComment::GetXml()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->get_xml(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMComment::TransformNode(IXMLDOMNode* stylesheet)
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->transformNode(stylesheet, &bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CXMLDOMNodeList CXMLDOMComment::SelectNodes(LPCTSTR queryString)
{
	CString	strqueryString(queryString);
	BSTR	bstrqueryString = strqueryString.AllocSysString();
	IXMLDOMNodeList* o;

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->selectNodes(bstrqueryString, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	SysFreeString(bstrqueryString);
	return o;
}


CXMLDOMNode CXMLDOMComment::SelectSingleNode(LPCTSTR queryString)
{
	CString	strqueryString(queryString);
	BSTR	bstrqueryString = strqueryString.AllocSysString();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->selectSingleNode(bstrqueryString, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	SysFreeString(bstrqueryString);
	return o;
}


BOOL CXMLDOMComment::GetParsed()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->get_parsed(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	return o;
}


CString CXMLDOMComment::GetNamespaceURI()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->get_namespaceURI(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMComment::GetPrefix()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->get_prefix(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMComment::GetBaseName()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->get_baseName(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMComment::TransformNodeToObject(IXMLDOMNode* stylesheet, _variant_t outputObject)
{
	VARIANT	varoutputObject = outputObject.Detach();

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->transformNodeToObject(stylesheet, varoutputObject);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	VariantClear(&varoutputObject);
}


CString CXMLDOMComment::GetData()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->get_data(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMComment::SetData(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->put_data(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	SysFreeString(bstrrhs);
}


long CXMLDOMComment::GetLength()
{
	long o;

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->get_length(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	return o;
}


CString CXMLDOMComment::SubstringData(long offset, long count)
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->substringData(offset, count, &bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMComment::AppendData(LPCTSTR data)
{
	CString	strdata(data);
	BSTR	bstrdata = strdata.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->appendData(bstrdata);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	SysFreeString(bstrdata);
}


void CXMLDOMComment::InsertData(long offset, LPCTSTR data)
{
	CString	strdata(data);
	BSTR	bstrdata = strdata.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->insertData(offset, bstrdata);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	SysFreeString(bstrdata);
}


void CXMLDOMComment::DeleteData(long offset, long count)
{

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->deleteData(offset, count);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

}


void CXMLDOMComment::ReplaceData(long offset, long count, LPCTSTR data)
{
	CString	strdata(data);
	BSTR	bstrdata = strdata.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMComment*)P())->replaceData(offset, count, bstrdata);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMComment));

	SysFreeString(bstrdata);
}




//--------------------------------------------------------------------
// class CXMLDOMDocumentFragment
//--------------------------------------------------------------------

CXMLDOMDocumentFragment::CXMLDOMDocumentFragment()
:	CInterfaceCallingWrapper<IXMLDOMDocumentFragment>()
{}


CXMLDOMDocumentFragment::CXMLDOMDocumentFragment(IXMLDOMDocumentFragment *p, BOOL bIncrementRefCount /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMDocumentFragment>(p, bIncrementRefCount)
{}


CXMLDOMDocumentFragment::CXMLDOMDocumentFragment(const CXMLDOMDocumentFragment &o)
:	CInterfaceCallingWrapper<IXMLDOMDocumentFragment>(o)
{}


CXMLDOMDocumentFragment::CXMLDOMDocumentFragment(IUnknown *pUnknown, BOOL bIncrementRefCount /*= FALSE*/, BOOL bThrowException /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMDocumentFragment>(pUnknown, bIncrementRefCount, bThrowException)
{}


CString CXMLDOMDocumentFragment::GetNodeName()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMDocumentFragment*)P())->get_nodeName(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentFragment));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


_variant_t CXMLDOMDocumentFragment::GetNodeValue()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMDocumentFragment*)P())->get_nodeValue(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentFragment));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMDocumentFragment::SetNodeValue(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMDocumentFragment*)P())->put_nodeValue(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentFragment));

	VariantClear(&varrhs);
}


DOMNodeType CXMLDOMDocumentFragment::GetNodeType()
{
	DOMNodeType o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentFragment*)P())->get_nodeType(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentFragment));

	return o;
}


CXMLDOMNode CXMLDOMDocumentFragment::GetParentNode()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentFragment*)P())->get_parentNode(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentFragment));

	return o;
}


CXMLDOMNodeList CXMLDOMDocumentFragment::GetChildNodes()
{
	IXMLDOMNodeList* o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentFragment*)P())->get_childNodes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentFragment));

	return o;
}


CXMLDOMNode CXMLDOMDocumentFragment::GetFirstChild()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentFragment*)P())->get_firstChild(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentFragment));

	return o;
}


CXMLDOMNode CXMLDOMDocumentFragment::GetLastChild()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentFragment*)P())->get_lastChild(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentFragment));

	return o;
}


CXMLDOMNode CXMLDOMDocumentFragment::GetPreviousSibling()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentFragment*)P())->get_previousSibling(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentFragment));

	return o;
}


CXMLDOMNode CXMLDOMDocumentFragment::GetNextSibling()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentFragment*)P())->get_nextSibling(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentFragment));

	return o;
}


CXMLDOMNamedNodeMap CXMLDOMDocumentFragment::GetAttributes()
{
	IXMLDOMNamedNodeMap* o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentFragment*)P())->get_attributes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentFragment));

	return o;
}


CXMLDOMNode CXMLDOMDocumentFragment::InsertBefore(IXMLDOMNode* newChild, _variant_t refChild)
{
	VARIANT	varrefChild = refChild.Detach();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentFragment*)P())->insertBefore(newChild, varrefChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentFragment));

	VariantClear(&varrefChild);
	return o;
}


CXMLDOMNode CXMLDOMDocumentFragment::ReplaceChild(IXMLDOMNode* newChild, IXMLDOMNode* oldChild)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentFragment*)P())->replaceChild(newChild, oldChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentFragment));

	return o;
}


CXMLDOMNode CXMLDOMDocumentFragment::RemoveChild(IXMLDOMNode* childNode)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentFragment*)P())->removeChild(childNode, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentFragment));

	return o;
}


CXMLDOMNode CXMLDOMDocumentFragment::AppendChild(IXMLDOMNode* newChild)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentFragment*)P())->appendChild(newChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentFragment));

	return o;
}


BOOL CXMLDOMDocumentFragment::HasChildNodes()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentFragment*)P())->hasChildNodes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentFragment));

	return o;
}


CXMLDOMDocument CXMLDOMDocumentFragment::GetOwnerDocument()
{
	IXMLDOMDocument* o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentFragment*)P())->get_ownerDocument(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentFragment));

	return o;
}


CXMLDOMNode CXMLDOMDocumentFragment::CloneNode(BOOL deep)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentFragment*)P())->cloneNode(deep, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentFragment));

	return o;
}


CString CXMLDOMDocumentFragment::GetNodeTypeString()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMDocumentFragment*)P())->get_nodeTypeString(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentFragment));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMDocumentFragment::GetText()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMDocumentFragment*)P())->get_text(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentFragment));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMDocumentFragment::SetText(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMDocumentFragment*)P())->put_text(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentFragment));

	SysFreeString(bstrrhs);
}


BOOL CXMLDOMDocumentFragment::GetSpecified()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentFragment*)P())->get_specified(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentFragment));

	return o;
}


CXMLDOMNode CXMLDOMDocumentFragment::GetDefinition()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentFragment*)P())->get_definition(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentFragment));

	return o;
}


_variant_t CXMLDOMDocumentFragment::GetNodeTypedValue()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMDocumentFragment*)P())->get_nodeTypedValue(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentFragment));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMDocumentFragment::SetNodeTypedValue(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMDocumentFragment*)P())->put_nodeTypedValue(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentFragment));

	VariantClear(&varrhs);
}


_variant_t CXMLDOMDocumentFragment::GetDataType()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMDocumentFragment*)P())->get_dataType(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentFragment));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMDocumentFragment::SetDataType(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMDocumentFragment*)P())->put_dataType(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentFragment));

	SysFreeString(bstrrhs);
}


CString CXMLDOMDocumentFragment::GetXml()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMDocumentFragment*)P())->get_xml(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentFragment));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMDocumentFragment::TransformNode(IXMLDOMNode* stylesheet)
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMDocumentFragment*)P())->transformNode(stylesheet, &bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentFragment));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CXMLDOMNodeList CXMLDOMDocumentFragment::SelectNodes(LPCTSTR queryString)
{
	CString	strqueryString(queryString);
	BSTR	bstrqueryString = strqueryString.AllocSysString();
	IXMLDOMNodeList* o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentFragment*)P())->selectNodes(bstrqueryString, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentFragment));

	SysFreeString(bstrqueryString);
	return o;
}


CXMLDOMNode CXMLDOMDocumentFragment::SelectSingleNode(LPCTSTR queryString)
{
	CString	strqueryString(queryString);
	BSTR	bstrqueryString = strqueryString.AllocSysString();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentFragment*)P())->selectSingleNode(bstrqueryString, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentFragment));

	SysFreeString(bstrqueryString);
	return o;
}


BOOL CXMLDOMDocumentFragment::GetParsed()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentFragment*)P())->get_parsed(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentFragment));

	return o;
}


CString CXMLDOMDocumentFragment::GetNamespaceURI()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMDocumentFragment*)P())->get_namespaceURI(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentFragment));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMDocumentFragment::GetPrefix()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMDocumentFragment*)P())->get_prefix(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentFragment));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMDocumentFragment::GetBaseName()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMDocumentFragment*)P())->get_baseName(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentFragment));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMDocumentFragment::TransformNodeToObject(IXMLDOMNode* stylesheet, _variant_t outputObject)
{
	VARIANT	varoutputObject = outputObject.Detach();

	HRESULT	hr = ((Raw_IXMLDOMDocumentFragment*)P())->transformNodeToObject(stylesheet, varoutputObject);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentFragment));

	VariantClear(&varoutputObject);
}




//--------------------------------------------------------------------
// class CXMLDOMDocumentType
//--------------------------------------------------------------------

CXMLDOMDocumentType::CXMLDOMDocumentType()
:	CInterfaceCallingWrapper<IXMLDOMDocumentType>()
{}


CXMLDOMDocumentType::CXMLDOMDocumentType(IXMLDOMDocumentType *p, BOOL bIncrementRefCount /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMDocumentType>(p, bIncrementRefCount)
{}


CXMLDOMDocumentType::CXMLDOMDocumentType(const CXMLDOMDocumentType &o)
:	CInterfaceCallingWrapper<IXMLDOMDocumentType>(o)
{}


CXMLDOMDocumentType::CXMLDOMDocumentType(IUnknown *pUnknown, BOOL bIncrementRefCount /*= FALSE*/, BOOL bThrowException /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMDocumentType>(pUnknown, bIncrementRefCount, bThrowException)
{}


CString CXMLDOMDocumentType::GetNodeName()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->get_nodeName(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


_variant_t CXMLDOMDocumentType::GetNodeValue()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->get_nodeValue(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMDocumentType::SetNodeValue(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->put_nodeValue(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	VariantClear(&varrhs);
}


DOMNodeType CXMLDOMDocumentType::GetNodeType()
{
	DOMNodeType o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->get_nodeType(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	return o;
}


CXMLDOMNode CXMLDOMDocumentType::GetParentNode()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->get_parentNode(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	return o;
}


CXMLDOMNodeList CXMLDOMDocumentType::GetChildNodes()
{
	IXMLDOMNodeList* o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->get_childNodes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	return o;
}


CXMLDOMNode CXMLDOMDocumentType::GetFirstChild()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->get_firstChild(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	return o;
}


CXMLDOMNode CXMLDOMDocumentType::GetLastChild()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->get_lastChild(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	return o;
}


CXMLDOMNode CXMLDOMDocumentType::GetPreviousSibling()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->get_previousSibling(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	return o;
}


CXMLDOMNode CXMLDOMDocumentType::GetNextSibling()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->get_nextSibling(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	return o;
}


CXMLDOMNamedNodeMap CXMLDOMDocumentType::GetAttributes()
{
	IXMLDOMNamedNodeMap* o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->get_attributes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	return o;
}


CXMLDOMNode CXMLDOMDocumentType::InsertBefore(IXMLDOMNode* newChild, _variant_t refChild)
{
	VARIANT	varrefChild = refChild.Detach();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->insertBefore(newChild, varrefChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	VariantClear(&varrefChild);
	return o;
}


CXMLDOMNode CXMLDOMDocumentType::ReplaceChild(IXMLDOMNode* newChild, IXMLDOMNode* oldChild)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->replaceChild(newChild, oldChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	return o;
}


CXMLDOMNode CXMLDOMDocumentType::RemoveChild(IXMLDOMNode* childNode)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->removeChild(childNode, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	return o;
}


CXMLDOMNode CXMLDOMDocumentType::AppendChild(IXMLDOMNode* newChild)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->appendChild(newChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	return o;
}


BOOL CXMLDOMDocumentType::HasChildNodes()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->hasChildNodes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	return o;
}


CXMLDOMDocument CXMLDOMDocumentType::GetOwnerDocument()
{
	IXMLDOMDocument* o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->get_ownerDocument(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	return o;
}


CXMLDOMNode CXMLDOMDocumentType::CloneNode(BOOL deep)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->cloneNode(deep, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	return o;
}


CString CXMLDOMDocumentType::GetNodeTypeString()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->get_nodeTypeString(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMDocumentType::GetText()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->get_text(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMDocumentType::SetText(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->put_text(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	SysFreeString(bstrrhs);
}


BOOL CXMLDOMDocumentType::GetSpecified()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->get_specified(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	return o;
}


CXMLDOMNode CXMLDOMDocumentType::GetDefinition()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->get_definition(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	return o;
}


_variant_t CXMLDOMDocumentType::GetNodeTypedValue()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->get_nodeTypedValue(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMDocumentType::SetNodeTypedValue(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->put_nodeTypedValue(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	VariantClear(&varrhs);
}


_variant_t CXMLDOMDocumentType::GetDataType()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->get_dataType(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMDocumentType::SetDataType(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->put_dataType(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	SysFreeString(bstrrhs);
}


CString CXMLDOMDocumentType::GetXml()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->get_xml(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMDocumentType::TransformNode(IXMLDOMNode* stylesheet)
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->transformNode(stylesheet, &bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CXMLDOMNodeList CXMLDOMDocumentType::SelectNodes(LPCTSTR queryString)
{
	CString	strqueryString(queryString);
	BSTR	bstrqueryString = strqueryString.AllocSysString();
	IXMLDOMNodeList* o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->selectNodes(bstrqueryString, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	SysFreeString(bstrqueryString);
	return o;
}


CXMLDOMNode CXMLDOMDocumentType::SelectSingleNode(LPCTSTR queryString)
{
	CString	strqueryString(queryString);
	BSTR	bstrqueryString = strqueryString.AllocSysString();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->selectSingleNode(bstrqueryString, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	SysFreeString(bstrqueryString);
	return o;
}


BOOL CXMLDOMDocumentType::GetParsed()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->get_parsed(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	return o;
}


CString CXMLDOMDocumentType::GetNamespaceURI()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->get_namespaceURI(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMDocumentType::GetPrefix()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->get_prefix(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMDocumentType::GetBaseName()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->get_baseName(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMDocumentType::TransformNodeToObject(IXMLDOMNode* stylesheet, _variant_t outputObject)
{
	VARIANT	varoutputObject = outputObject.Detach();

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->transformNodeToObject(stylesheet, varoutputObject);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	VariantClear(&varoutputObject);
}


CString CXMLDOMDocumentType::GetName()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->get_name(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CXMLDOMNamedNodeMap CXMLDOMDocumentType::GetEntities()
{
	IXMLDOMNamedNodeMap* o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->get_entities(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	return o;
}


CXMLDOMNamedNodeMap CXMLDOMDocumentType::GetNotations()
{
	IXMLDOMNamedNodeMap* o;

	HRESULT	hr = ((Raw_IXMLDOMDocumentType*)P())->get_notations(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMDocumentType));

	return o;
}




//--------------------------------------------------------------------
// class CXMLDOMElement
//--------------------------------------------------------------------

CXMLDOMElement::CXMLDOMElement()
:	CInterfaceCallingWrapper<IXMLDOMElement>()
{}


CXMLDOMElement::CXMLDOMElement(IXMLDOMElement *p, BOOL bIncrementRefCount /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMElement>(p, bIncrementRefCount)
{}


CXMLDOMElement::CXMLDOMElement(const CXMLDOMElement &o)
:	CInterfaceCallingWrapper<IXMLDOMElement>(o)
{}


CXMLDOMElement::CXMLDOMElement(IUnknown *pUnknown, BOOL bIncrementRefCount /*= FALSE*/, BOOL bThrowException /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMElement>(pUnknown, bIncrementRefCount, bThrowException)
{}


CString CXMLDOMElement::GetNodeName()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->get_nodeName(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


_variant_t CXMLDOMElement::GetNodeValue()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->get_nodeValue(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMElement::SetNodeValue(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->put_nodeValue(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	VariantClear(&varrhs);
}


DOMNodeType CXMLDOMElement::GetNodeType()
{
	DOMNodeType o;

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->get_nodeType(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	return o;
}


CXMLDOMNode CXMLDOMElement::GetParentNode()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->get_parentNode(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	return o;
}


CXMLDOMNodeList CXMLDOMElement::GetChildNodes()
{
	IXMLDOMNodeList* o;

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->get_childNodes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	return o;
}


CXMLDOMNode CXMLDOMElement::GetFirstChild()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->get_firstChild(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	return o;
}


CXMLDOMNode CXMLDOMElement::GetLastChild()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->get_lastChild(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	return o;
}


CXMLDOMNode CXMLDOMElement::GetPreviousSibling()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->get_previousSibling(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	return o;
}


CXMLDOMNode CXMLDOMElement::GetNextSibling()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->get_nextSibling(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	return o;
}


CXMLDOMNamedNodeMap CXMLDOMElement::GetAttributes()
{
	IXMLDOMNamedNodeMap* o;

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->get_attributes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	return o;
}


CXMLDOMNode CXMLDOMElement::InsertBefore(IXMLDOMNode* newChild, _variant_t refChild)
{
	VARIANT	varrefChild = refChild.Detach();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->insertBefore(newChild, varrefChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	VariantClear(&varrefChild);
	return o;
}


CXMLDOMNode CXMLDOMElement::ReplaceChild(IXMLDOMNode* newChild, IXMLDOMNode* oldChild)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->replaceChild(newChild, oldChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	return o;
}


CXMLDOMNode CXMLDOMElement::RemoveChild(IXMLDOMNode* childNode)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->removeChild(childNode, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	return o;
}


CXMLDOMNode CXMLDOMElement::AppendChild(IXMLDOMNode* newChild)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->appendChild(newChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	return o;
}


BOOL CXMLDOMElement::HasChildNodes()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->hasChildNodes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	return o;
}


CXMLDOMDocument CXMLDOMElement::GetOwnerDocument()
{
	IXMLDOMDocument* o;

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->get_ownerDocument(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	return o;
}


CXMLDOMNode CXMLDOMElement::CloneNode(BOOL deep)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->cloneNode(deep, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	return o;
}


CString CXMLDOMElement::GetNodeTypeString()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->get_nodeTypeString(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMElement::GetText()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->get_text(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMElement::SetText(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->put_text(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	SysFreeString(bstrrhs);
}


BOOL CXMLDOMElement::GetSpecified()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->get_specified(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	return o;
}


CXMLDOMNode CXMLDOMElement::GetDefinition()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->get_definition(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	return o;
}


_variant_t CXMLDOMElement::GetNodeTypedValue()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->get_nodeTypedValue(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMElement::SetNodeTypedValue(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->put_nodeTypedValue(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	VariantClear(&varrhs);
}


_variant_t CXMLDOMElement::GetDataType()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->get_dataType(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMElement::SetDataType(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->put_dataType(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	SysFreeString(bstrrhs);
}


CString CXMLDOMElement::GetXml()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->get_xml(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMElement::TransformNode(IXMLDOMNode* stylesheet)
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->transformNode(stylesheet, &bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CXMLDOMNodeList CXMLDOMElement::SelectNodes(LPCTSTR queryString)
{
	CString	strqueryString(queryString);
	BSTR	bstrqueryString = strqueryString.AllocSysString();
	IXMLDOMNodeList* o;

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->selectNodes(bstrqueryString, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	SysFreeString(bstrqueryString);
	return o;
}


CXMLDOMNode CXMLDOMElement::SelectSingleNode(LPCTSTR queryString)
{
	CString	strqueryString(queryString);
	BSTR	bstrqueryString = strqueryString.AllocSysString();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->selectSingleNode(bstrqueryString, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	SysFreeString(bstrqueryString);
	return o;
}


BOOL CXMLDOMElement::GetParsed()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->get_parsed(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	return o;
}


CString CXMLDOMElement::GetNamespaceURI()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->get_namespaceURI(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMElement::GetPrefix()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->get_prefix(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMElement::GetBaseName()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->get_baseName(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMElement::TransformNodeToObject(IXMLDOMNode* stylesheet, _variant_t outputObject)
{
	VARIANT	varoutputObject = outputObject.Detach();

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->transformNodeToObject(stylesheet, varoutputObject);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	VariantClear(&varoutputObject);
}


CString CXMLDOMElement::GetTagName()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->get_tagName(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


_variant_t CXMLDOMElement::GetAttribute(LPCTSTR name)
{
	CString	strname(name);
	BSTR	bstrname = strname.AllocSysString();
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->getAttribute(bstrname, &varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	SysFreeString(bstrname);
	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMElement::SetAttribute(LPCTSTR name, _variant_t value)
{
	CString	strname(name);
	BSTR	bstrname = strname.AllocSysString();
	VARIANT	varvalue = value.Detach();

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->setAttribute(bstrname, varvalue);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	SysFreeString(bstrname);
	VariantClear(&varvalue);
}


void CXMLDOMElement::RemoveAttribute(LPCTSTR name)
{
	CString	strname(name);
	BSTR	bstrname = strname.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->removeAttribute(bstrname);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	SysFreeString(bstrname);
}


CXMLDOMAttribute CXMLDOMElement::GetAttributeNode(LPCTSTR name)
{
	CString	strname(name);
	BSTR	bstrname = strname.AllocSysString();
	IXMLDOMAttribute* o;

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->getAttributeNode(bstrname, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	SysFreeString(bstrname);
	return o;
}


CXMLDOMAttribute CXMLDOMElement::SetAttributeNode(IXMLDOMAttribute* DOMAttribute)
{
	IXMLDOMAttribute* o;

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->setAttributeNode(DOMAttribute, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	return o;
}


CXMLDOMAttribute CXMLDOMElement::RemoveAttributeNode(IXMLDOMAttribute* DOMAttribute)
{
	IXMLDOMAttribute* o;

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->removeAttributeNode(DOMAttribute, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	return o;
}


CXMLDOMNodeList CXMLDOMElement::GetElementsByTagName(LPCTSTR tagName)
{
	CString	strtagName(tagName);
	BSTR	bstrtagName = strtagName.AllocSysString();
	IXMLDOMNodeList* o;

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->getElementsByTagName(bstrtagName, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

	SysFreeString(bstrtagName);
	return o;
}


void CXMLDOMElement::Normalize()
{

	HRESULT	hr = ((Raw_IXMLDOMElement*)P())->normalize();
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMElement));

}




//--------------------------------------------------------------------
// class CXMLDOMEntity
//--------------------------------------------------------------------

CXMLDOMEntity::CXMLDOMEntity()
:	CInterfaceCallingWrapper<IXMLDOMEntity>()
{}


CXMLDOMEntity::CXMLDOMEntity(IXMLDOMEntity *p, BOOL bIncrementRefCount /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMEntity>(p, bIncrementRefCount)
{}


CXMLDOMEntity::CXMLDOMEntity(const CXMLDOMEntity &o)
:	CInterfaceCallingWrapper<IXMLDOMEntity>(o)
{}


CXMLDOMEntity::CXMLDOMEntity(IUnknown *pUnknown, BOOL bIncrementRefCount /*= FALSE*/, BOOL bThrowException /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMEntity>(pUnknown, bIncrementRefCount, bThrowException)
{}


CString CXMLDOMEntity::GetNodeName()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->get_nodeName(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


_variant_t CXMLDOMEntity::GetNodeValue()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->get_nodeValue(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMEntity::SetNodeValue(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->put_nodeValue(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	VariantClear(&varrhs);
}


DOMNodeType CXMLDOMEntity::GetNodeType()
{
	DOMNodeType o;

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->get_nodeType(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	return o;
}


CXMLDOMNode CXMLDOMEntity::GetParentNode()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->get_parentNode(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	return o;
}


CXMLDOMNodeList CXMLDOMEntity::GetChildNodes()
{
	IXMLDOMNodeList* o;

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->get_childNodes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	return o;
}


CXMLDOMNode CXMLDOMEntity::GetFirstChild()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->get_firstChild(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	return o;
}


CXMLDOMNode CXMLDOMEntity::GetLastChild()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->get_lastChild(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	return o;
}


CXMLDOMNode CXMLDOMEntity::GetPreviousSibling()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->get_previousSibling(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	return o;
}


CXMLDOMNode CXMLDOMEntity::GetNextSibling()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->get_nextSibling(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	return o;
}


CXMLDOMNamedNodeMap CXMLDOMEntity::GetAttributes()
{
	IXMLDOMNamedNodeMap* o;

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->get_attributes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	return o;
}


CXMLDOMNode CXMLDOMEntity::InsertBefore(IXMLDOMNode* newChild, _variant_t refChild)
{
	VARIANT	varrefChild = refChild.Detach();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->insertBefore(newChild, varrefChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	VariantClear(&varrefChild);
	return o;
}


CXMLDOMNode CXMLDOMEntity::ReplaceChild(IXMLDOMNode* newChild, IXMLDOMNode* oldChild)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->replaceChild(newChild, oldChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	return o;
}


CXMLDOMNode CXMLDOMEntity::RemoveChild(IXMLDOMNode* childNode)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->removeChild(childNode, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	return o;
}


CXMLDOMNode CXMLDOMEntity::AppendChild(IXMLDOMNode* newChild)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->appendChild(newChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	return o;
}


BOOL CXMLDOMEntity::HasChildNodes()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->hasChildNodes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	return o;
}


CXMLDOMDocument CXMLDOMEntity::GetOwnerDocument()
{
	IXMLDOMDocument* o;

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->get_ownerDocument(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	return o;
}


CXMLDOMNode CXMLDOMEntity::CloneNode(BOOL deep)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->cloneNode(deep, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	return o;
}


CString CXMLDOMEntity::GetNodeTypeString()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->get_nodeTypeString(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMEntity::GetText()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->get_text(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMEntity::SetText(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->put_text(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	SysFreeString(bstrrhs);
}


BOOL CXMLDOMEntity::GetSpecified()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->get_specified(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	return o;
}


CXMLDOMNode CXMLDOMEntity::GetDefinition()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->get_definition(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	return o;
}


_variant_t CXMLDOMEntity::GetNodeTypedValue()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->get_nodeTypedValue(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMEntity::SetNodeTypedValue(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->put_nodeTypedValue(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	VariantClear(&varrhs);
}


_variant_t CXMLDOMEntity::GetDataType()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->get_dataType(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMEntity::SetDataType(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->put_dataType(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	SysFreeString(bstrrhs);
}


CString CXMLDOMEntity::GetXml()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->get_xml(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMEntity::TransformNode(IXMLDOMNode* stylesheet)
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->transformNode(stylesheet, &bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CXMLDOMNodeList CXMLDOMEntity::SelectNodes(LPCTSTR queryString)
{
	CString	strqueryString(queryString);
	BSTR	bstrqueryString = strqueryString.AllocSysString();
	IXMLDOMNodeList* o;

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->selectNodes(bstrqueryString, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	SysFreeString(bstrqueryString);
	return o;
}


CXMLDOMNode CXMLDOMEntity::SelectSingleNode(LPCTSTR queryString)
{
	CString	strqueryString(queryString);
	BSTR	bstrqueryString = strqueryString.AllocSysString();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->selectSingleNode(bstrqueryString, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	SysFreeString(bstrqueryString);
	return o;
}


BOOL CXMLDOMEntity::GetParsed()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->get_parsed(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	return o;
}


CString CXMLDOMEntity::GetNamespaceURI()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->get_namespaceURI(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMEntity::GetPrefix()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->get_prefix(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMEntity::GetBaseName()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->get_baseName(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMEntity::TransformNodeToObject(IXMLDOMNode* stylesheet, _variant_t outputObject)
{
	VARIANT	varoutputObject = outputObject.Detach();

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->transformNodeToObject(stylesheet, varoutputObject);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	VariantClear(&varoutputObject);
}


_variant_t CXMLDOMEntity::GetPublicId()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->get_publicId(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	_variant_t	o(varO, false);
	return o;
}


_variant_t CXMLDOMEntity::GetSystemId()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->get_systemId(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	_variant_t	o(varO, false);
	return o;
}


CString CXMLDOMEntity::GetNotationName()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMEntity*)P())->get_notationName(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntity));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}




//--------------------------------------------------------------------
// class CXMLDOMEntityReference
//--------------------------------------------------------------------

CXMLDOMEntityReference::CXMLDOMEntityReference()
:	CInterfaceCallingWrapper<IXMLDOMEntityReference>()
{}


CXMLDOMEntityReference::CXMLDOMEntityReference(IXMLDOMEntityReference *p, BOOL bIncrementRefCount /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMEntityReference>(p, bIncrementRefCount)
{}


CXMLDOMEntityReference::CXMLDOMEntityReference(const CXMLDOMEntityReference &o)
:	CInterfaceCallingWrapper<IXMLDOMEntityReference>(o)
{}


CXMLDOMEntityReference::CXMLDOMEntityReference(IUnknown *pUnknown, BOOL bIncrementRefCount /*= FALSE*/, BOOL bThrowException /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMEntityReference>(pUnknown, bIncrementRefCount, bThrowException)
{}


CString CXMLDOMEntityReference::GetNodeName()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMEntityReference*)P())->get_nodeName(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntityReference));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


_variant_t CXMLDOMEntityReference::GetNodeValue()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMEntityReference*)P())->get_nodeValue(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntityReference));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMEntityReference::SetNodeValue(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMEntityReference*)P())->put_nodeValue(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntityReference));

	VariantClear(&varrhs);
}


DOMNodeType CXMLDOMEntityReference::GetNodeType()
{
	DOMNodeType o;

	HRESULT	hr = ((Raw_IXMLDOMEntityReference*)P())->get_nodeType(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntityReference));

	return o;
}


CXMLDOMNode CXMLDOMEntityReference::GetParentNode()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMEntityReference*)P())->get_parentNode(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntityReference));

	return o;
}


CXMLDOMNodeList CXMLDOMEntityReference::GetChildNodes()
{
	IXMLDOMNodeList* o;

	HRESULT	hr = ((Raw_IXMLDOMEntityReference*)P())->get_childNodes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntityReference));

	return o;
}


CXMLDOMNode CXMLDOMEntityReference::GetFirstChild()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMEntityReference*)P())->get_firstChild(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntityReference));

	return o;
}


CXMLDOMNode CXMLDOMEntityReference::GetLastChild()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMEntityReference*)P())->get_lastChild(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntityReference));

	return o;
}


CXMLDOMNode CXMLDOMEntityReference::GetPreviousSibling()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMEntityReference*)P())->get_previousSibling(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntityReference));

	return o;
}


CXMLDOMNode CXMLDOMEntityReference::GetNextSibling()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMEntityReference*)P())->get_nextSibling(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntityReference));

	return o;
}


CXMLDOMNamedNodeMap CXMLDOMEntityReference::GetAttributes()
{
	IXMLDOMNamedNodeMap* o;

	HRESULT	hr = ((Raw_IXMLDOMEntityReference*)P())->get_attributes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntityReference));

	return o;
}


CXMLDOMNode CXMLDOMEntityReference::InsertBefore(IXMLDOMNode* newChild, _variant_t refChild)
{
	VARIANT	varrefChild = refChild.Detach();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMEntityReference*)P())->insertBefore(newChild, varrefChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntityReference));

	VariantClear(&varrefChild);
	return o;
}


CXMLDOMNode CXMLDOMEntityReference::ReplaceChild(IXMLDOMNode* newChild, IXMLDOMNode* oldChild)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMEntityReference*)P())->replaceChild(newChild, oldChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntityReference));

	return o;
}


CXMLDOMNode CXMLDOMEntityReference::RemoveChild(IXMLDOMNode* childNode)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMEntityReference*)P())->removeChild(childNode, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntityReference));

	return o;
}


CXMLDOMNode CXMLDOMEntityReference::AppendChild(IXMLDOMNode* newChild)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMEntityReference*)P())->appendChild(newChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntityReference));

	return o;
}


BOOL CXMLDOMEntityReference::HasChildNodes()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMEntityReference*)P())->hasChildNodes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntityReference));

	return o;
}


CXMLDOMDocument CXMLDOMEntityReference::GetOwnerDocument()
{
	IXMLDOMDocument* o;

	HRESULT	hr = ((Raw_IXMLDOMEntityReference*)P())->get_ownerDocument(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntityReference));

	return o;
}


CXMLDOMNode CXMLDOMEntityReference::CloneNode(BOOL deep)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMEntityReference*)P())->cloneNode(deep, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntityReference));

	return o;
}


CString CXMLDOMEntityReference::GetNodeTypeString()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMEntityReference*)P())->get_nodeTypeString(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntityReference));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMEntityReference::GetText()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMEntityReference*)P())->get_text(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntityReference));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMEntityReference::SetText(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMEntityReference*)P())->put_text(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntityReference));

	SysFreeString(bstrrhs);
}


BOOL CXMLDOMEntityReference::GetSpecified()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMEntityReference*)P())->get_specified(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntityReference));

	return o;
}


CXMLDOMNode CXMLDOMEntityReference::GetDefinition()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMEntityReference*)P())->get_definition(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntityReference));

	return o;
}


_variant_t CXMLDOMEntityReference::GetNodeTypedValue()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMEntityReference*)P())->get_nodeTypedValue(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntityReference));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMEntityReference::SetNodeTypedValue(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMEntityReference*)P())->put_nodeTypedValue(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntityReference));

	VariantClear(&varrhs);
}


_variant_t CXMLDOMEntityReference::GetDataType()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMEntityReference*)P())->get_dataType(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntityReference));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMEntityReference::SetDataType(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMEntityReference*)P())->put_dataType(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntityReference));

	SysFreeString(bstrrhs);
}


CString CXMLDOMEntityReference::GetXml()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMEntityReference*)P())->get_xml(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntityReference));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMEntityReference::TransformNode(IXMLDOMNode* stylesheet)
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMEntityReference*)P())->transformNode(stylesheet, &bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntityReference));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CXMLDOMNodeList CXMLDOMEntityReference::SelectNodes(LPCTSTR queryString)
{
	CString	strqueryString(queryString);
	BSTR	bstrqueryString = strqueryString.AllocSysString();
	IXMLDOMNodeList* o;

	HRESULT	hr = ((Raw_IXMLDOMEntityReference*)P())->selectNodes(bstrqueryString, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntityReference));

	SysFreeString(bstrqueryString);
	return o;
}


CXMLDOMNode CXMLDOMEntityReference::SelectSingleNode(LPCTSTR queryString)
{
	CString	strqueryString(queryString);
	BSTR	bstrqueryString = strqueryString.AllocSysString();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMEntityReference*)P())->selectSingleNode(bstrqueryString, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntityReference));

	SysFreeString(bstrqueryString);
	return o;
}


BOOL CXMLDOMEntityReference::GetParsed()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMEntityReference*)P())->get_parsed(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntityReference));

	return o;
}


CString CXMLDOMEntityReference::GetNamespaceURI()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMEntityReference*)P())->get_namespaceURI(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntityReference));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMEntityReference::GetPrefix()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMEntityReference*)P())->get_prefix(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntityReference));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMEntityReference::GetBaseName()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMEntityReference*)P())->get_baseName(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntityReference));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMEntityReference::TransformNodeToObject(IXMLDOMNode* stylesheet, _variant_t outputObject)
{
	VARIANT	varoutputObject = outputObject.Detach();

	HRESULT	hr = ((Raw_IXMLDOMEntityReference*)P())->transformNodeToObject(stylesheet, varoutputObject);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMEntityReference));

	VariantClear(&varoutputObject);
}




//--------------------------------------------------------------------
// class CXMLDOMImplementation
//--------------------------------------------------------------------

CXMLDOMImplementation::CXMLDOMImplementation()
:	CInterfaceCallingWrapper<IXMLDOMImplementation>()
{}


CXMLDOMImplementation::CXMLDOMImplementation(IXMLDOMImplementation *p, BOOL bIncrementRefCount /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMImplementation>(p, bIncrementRefCount)
{}


CXMLDOMImplementation::CXMLDOMImplementation(const CXMLDOMImplementation &o)
:	CInterfaceCallingWrapper<IXMLDOMImplementation>(o)
{}


CXMLDOMImplementation::CXMLDOMImplementation(IUnknown *pUnknown, BOOL bIncrementRefCount /*= FALSE*/, BOOL bThrowException /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMImplementation>(pUnknown, bIncrementRefCount, bThrowException)
{}


BOOL CXMLDOMImplementation::HasFeature(LPCTSTR feature, LPCTSTR version)
{
	CString	strfeature(feature);
	BSTR	bstrfeature = strfeature.AllocSysString();
	CString	strversion(version);
	BSTR	bstrversion = strversion.AllocSysString();
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMImplementation*)P())->hasFeature(bstrfeature, bstrversion, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMImplementation));

	SysFreeString(bstrfeature);
	SysFreeString(bstrversion);
	return o;
}




//--------------------------------------------------------------------
// class CXMLDOMNotation
//--------------------------------------------------------------------

CXMLDOMNotation::CXMLDOMNotation()
:	CInterfaceCallingWrapper<IXMLDOMNotation>()
{}


CXMLDOMNotation::CXMLDOMNotation(IXMLDOMNotation *p, BOOL bIncrementRefCount /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMNotation>(p, bIncrementRefCount)
{}


CXMLDOMNotation::CXMLDOMNotation(const CXMLDOMNotation &o)
:	CInterfaceCallingWrapper<IXMLDOMNotation>(o)
{}


CXMLDOMNotation::CXMLDOMNotation(IUnknown *pUnknown, BOOL bIncrementRefCount /*= FALSE*/, BOOL bThrowException /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMNotation>(pUnknown, bIncrementRefCount, bThrowException)
{}


CString CXMLDOMNotation::GetNodeName()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->get_nodeName(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


_variant_t CXMLDOMNotation::GetNodeValue()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->get_nodeValue(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMNotation::SetNodeValue(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->put_nodeValue(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	VariantClear(&varrhs);
}


DOMNodeType CXMLDOMNotation::GetNodeType()
{
	DOMNodeType o;

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->get_nodeType(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	return o;
}


CXMLDOMNode CXMLDOMNotation::GetParentNode()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->get_parentNode(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	return o;
}


CXMLDOMNodeList CXMLDOMNotation::GetChildNodes()
{
	IXMLDOMNodeList* o;

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->get_childNodes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	return o;
}


CXMLDOMNode CXMLDOMNotation::GetFirstChild()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->get_firstChild(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	return o;
}


CXMLDOMNode CXMLDOMNotation::GetLastChild()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->get_lastChild(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	return o;
}


CXMLDOMNode CXMLDOMNotation::GetPreviousSibling()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->get_previousSibling(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	return o;
}


CXMLDOMNode CXMLDOMNotation::GetNextSibling()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->get_nextSibling(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	return o;
}


CXMLDOMNamedNodeMap CXMLDOMNotation::GetAttributes()
{
	IXMLDOMNamedNodeMap* o;

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->get_attributes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	return o;
}


CXMLDOMNode CXMLDOMNotation::InsertBefore(IXMLDOMNode* newChild, _variant_t refChild)
{
	VARIANT	varrefChild = refChild.Detach();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->insertBefore(newChild, varrefChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	VariantClear(&varrefChild);
	return o;
}


CXMLDOMNode CXMLDOMNotation::ReplaceChild(IXMLDOMNode* newChild, IXMLDOMNode* oldChild)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->replaceChild(newChild, oldChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	return o;
}


CXMLDOMNode CXMLDOMNotation::RemoveChild(IXMLDOMNode* childNode)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->removeChild(childNode, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	return o;
}


CXMLDOMNode CXMLDOMNotation::AppendChild(IXMLDOMNode* newChild)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->appendChild(newChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	return o;
}


BOOL CXMLDOMNotation::HasChildNodes()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->hasChildNodes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	return o;
}


CXMLDOMDocument CXMLDOMNotation::GetOwnerDocument()
{
	IXMLDOMDocument* o;

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->get_ownerDocument(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	return o;
}


CXMLDOMNode CXMLDOMNotation::CloneNode(BOOL deep)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->cloneNode(deep, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	return o;
}


CString CXMLDOMNotation::GetNodeTypeString()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->get_nodeTypeString(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMNotation::GetText()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->get_text(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMNotation::SetText(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->put_text(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	SysFreeString(bstrrhs);
}


BOOL CXMLDOMNotation::GetSpecified()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->get_specified(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	return o;
}


CXMLDOMNode CXMLDOMNotation::GetDefinition()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->get_definition(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	return o;
}


_variant_t CXMLDOMNotation::GetNodeTypedValue()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->get_nodeTypedValue(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMNotation::SetNodeTypedValue(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->put_nodeTypedValue(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	VariantClear(&varrhs);
}


_variant_t CXMLDOMNotation::GetDataType()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->get_dataType(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMNotation::SetDataType(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->put_dataType(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	SysFreeString(bstrrhs);
}


CString CXMLDOMNotation::GetXml()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->get_xml(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMNotation::TransformNode(IXMLDOMNode* stylesheet)
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->transformNode(stylesheet, &bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CXMLDOMNodeList CXMLDOMNotation::SelectNodes(LPCTSTR queryString)
{
	CString	strqueryString(queryString);
	BSTR	bstrqueryString = strqueryString.AllocSysString();
	IXMLDOMNodeList* o;

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->selectNodes(bstrqueryString, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	SysFreeString(bstrqueryString);
	return o;
}


CXMLDOMNode CXMLDOMNotation::SelectSingleNode(LPCTSTR queryString)
{
	CString	strqueryString(queryString);
	BSTR	bstrqueryString = strqueryString.AllocSysString();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->selectSingleNode(bstrqueryString, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	SysFreeString(bstrqueryString);
	return o;
}


BOOL CXMLDOMNotation::GetParsed()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->get_parsed(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	return o;
}


CString CXMLDOMNotation::GetNamespaceURI()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->get_namespaceURI(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMNotation::GetPrefix()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->get_prefix(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMNotation::GetBaseName()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->get_baseName(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMNotation::TransformNodeToObject(IXMLDOMNode* stylesheet, _variant_t outputObject)
{
	VARIANT	varoutputObject = outputObject.Detach();

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->transformNodeToObject(stylesheet, varoutputObject);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	VariantClear(&varoutputObject);
}


_variant_t CXMLDOMNotation::GetPublicId()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->get_publicId(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	_variant_t	o(varO, false);
	return o;
}


_variant_t CXMLDOMNotation::GetSystemId()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMNotation*)P())->get_systemId(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMNotation));

	_variant_t	o(varO, false);
	return o;
}




//--------------------------------------------------------------------
// class CXMLDOMProcessingInstruction
//--------------------------------------------------------------------

CXMLDOMProcessingInstruction::CXMLDOMProcessingInstruction()
:	CInterfaceCallingWrapper<IXMLDOMProcessingInstruction>()
{}


CXMLDOMProcessingInstruction::CXMLDOMProcessingInstruction(IXMLDOMProcessingInstruction *p, BOOL bIncrementRefCount /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMProcessingInstruction>(p, bIncrementRefCount)
{}


CXMLDOMProcessingInstruction::CXMLDOMProcessingInstruction(const CXMLDOMProcessingInstruction &o)
:	CInterfaceCallingWrapper<IXMLDOMProcessingInstruction>(o)
{}


CXMLDOMProcessingInstruction::CXMLDOMProcessingInstruction(IUnknown *pUnknown, BOOL bIncrementRefCount /*= FALSE*/, BOOL bThrowException /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMProcessingInstruction>(pUnknown, bIncrementRefCount, bThrowException)
{}


CString CXMLDOMProcessingInstruction::GetNodeName()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->get_nodeName(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


_variant_t CXMLDOMProcessingInstruction::GetNodeValue()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->get_nodeValue(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMProcessingInstruction::SetNodeValue(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->put_nodeValue(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	VariantClear(&varrhs);
}


DOMNodeType CXMLDOMProcessingInstruction::GetNodeType()
{
	DOMNodeType o;

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->get_nodeType(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	return o;
}


CXMLDOMNode CXMLDOMProcessingInstruction::GetParentNode()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->get_parentNode(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	return o;
}


CXMLDOMNodeList CXMLDOMProcessingInstruction::GetChildNodes()
{
	IXMLDOMNodeList* o;

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->get_childNodes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	return o;
}


CXMLDOMNode CXMLDOMProcessingInstruction::GetFirstChild()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->get_firstChild(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	return o;
}


CXMLDOMNode CXMLDOMProcessingInstruction::GetLastChild()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->get_lastChild(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	return o;
}


CXMLDOMNode CXMLDOMProcessingInstruction::GetPreviousSibling()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->get_previousSibling(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	return o;
}


CXMLDOMNode CXMLDOMProcessingInstruction::GetNextSibling()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->get_nextSibling(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	return o;
}


CXMLDOMNamedNodeMap CXMLDOMProcessingInstruction::GetAttributes()
{
	IXMLDOMNamedNodeMap* o;

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->get_attributes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	return o;
}


CXMLDOMNode CXMLDOMProcessingInstruction::InsertBefore(IXMLDOMNode* newChild, _variant_t refChild)
{
	VARIANT	varrefChild = refChild.Detach();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->insertBefore(newChild, varrefChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	VariantClear(&varrefChild);
	return o;
}


CXMLDOMNode CXMLDOMProcessingInstruction::ReplaceChild(IXMLDOMNode* newChild, IXMLDOMNode* oldChild)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->replaceChild(newChild, oldChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	return o;
}


CXMLDOMNode CXMLDOMProcessingInstruction::RemoveChild(IXMLDOMNode* childNode)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->removeChild(childNode, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	return o;
}


CXMLDOMNode CXMLDOMProcessingInstruction::AppendChild(IXMLDOMNode* newChild)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->appendChild(newChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	return o;
}


BOOL CXMLDOMProcessingInstruction::HasChildNodes()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->hasChildNodes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	return o;
}


CXMLDOMDocument CXMLDOMProcessingInstruction::GetOwnerDocument()
{
	IXMLDOMDocument* o;

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->get_ownerDocument(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	return o;
}


CXMLDOMNode CXMLDOMProcessingInstruction::CloneNode(BOOL deep)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->cloneNode(deep, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	return o;
}


CString CXMLDOMProcessingInstruction::GetNodeTypeString()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->get_nodeTypeString(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMProcessingInstruction::GetText()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->get_text(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMProcessingInstruction::SetText(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->put_text(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	SysFreeString(bstrrhs);
}


BOOL CXMLDOMProcessingInstruction::GetSpecified()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->get_specified(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	return o;
}


CXMLDOMNode CXMLDOMProcessingInstruction::GetDefinition()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->get_definition(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	return o;
}


_variant_t CXMLDOMProcessingInstruction::GetNodeTypedValue()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->get_nodeTypedValue(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMProcessingInstruction::SetNodeTypedValue(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->put_nodeTypedValue(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	VariantClear(&varrhs);
}


_variant_t CXMLDOMProcessingInstruction::GetDataType()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->get_dataType(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMProcessingInstruction::SetDataType(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->put_dataType(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	SysFreeString(bstrrhs);
}


CString CXMLDOMProcessingInstruction::GetXml()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->get_xml(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMProcessingInstruction::TransformNode(IXMLDOMNode* stylesheet)
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->transformNode(stylesheet, &bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CXMLDOMNodeList CXMLDOMProcessingInstruction::SelectNodes(LPCTSTR queryString)
{
	CString	strqueryString(queryString);
	BSTR	bstrqueryString = strqueryString.AllocSysString();
	IXMLDOMNodeList* o;

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->selectNodes(bstrqueryString, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	SysFreeString(bstrqueryString);
	return o;
}


CXMLDOMNode CXMLDOMProcessingInstruction::SelectSingleNode(LPCTSTR queryString)
{
	CString	strqueryString(queryString);
	BSTR	bstrqueryString = strqueryString.AllocSysString();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->selectSingleNode(bstrqueryString, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	SysFreeString(bstrqueryString);
	return o;
}


BOOL CXMLDOMProcessingInstruction::GetParsed()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->get_parsed(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	return o;
}


CString CXMLDOMProcessingInstruction::GetNamespaceURI()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->get_namespaceURI(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMProcessingInstruction::GetPrefix()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->get_prefix(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMProcessingInstruction::GetBaseName()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->get_baseName(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMProcessingInstruction::TransformNodeToObject(IXMLDOMNode* stylesheet, _variant_t outputObject)
{
	VARIANT	varoutputObject = outputObject.Detach();

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->transformNodeToObject(stylesheet, varoutputObject);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	VariantClear(&varoutputObject);
}


CString CXMLDOMProcessingInstruction::GetTarget()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->get_target(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMProcessingInstruction::GetData()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->get_data(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMProcessingInstruction::SetData(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMProcessingInstruction*)P())->put_data(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMProcessingInstruction));

	SysFreeString(bstrrhs);
}




//--------------------------------------------------------------------
// class CXMLDOMSchemaCollection
//--------------------------------------------------------------------

CXMLDOMSchemaCollection::CXMLDOMSchemaCollection()
:	CInterfaceCallingWrapper<IXMLDOMSchemaCollection>()
{}


CXMLDOMSchemaCollection::CXMLDOMSchemaCollection(IXMLDOMSchemaCollection *p, BOOL bIncrementRefCount /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMSchemaCollection>(p, bIncrementRefCount)
{}


CXMLDOMSchemaCollection::CXMLDOMSchemaCollection(const CXMLDOMSchemaCollection &o)
:	CInterfaceCallingWrapper<IXMLDOMSchemaCollection>(o)
{}


CXMLDOMSchemaCollection::CXMLDOMSchemaCollection(IUnknown *pUnknown, BOOL bIncrementRefCount /*= FALSE*/, BOOL bThrowException /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMSchemaCollection>(pUnknown, bIncrementRefCount, bThrowException)
{}


void CXMLDOMSchemaCollection::Add(LPCTSTR namespaceURI, _variant_t var)
{
	CString	strnamespaceURI(namespaceURI);
	BSTR	bstrnamespaceURI = strnamespaceURI.AllocSysString();
	VARIANT	varvar = var.Detach();

	HRESULT	hr = ((Raw_IXMLDOMSchemaCollection*)P())->add(bstrnamespaceURI, varvar);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMSchemaCollection));

	SysFreeString(bstrnamespaceURI);
	VariantClear(&varvar);
}


CXMLDOMNode CXMLDOMSchemaCollection::Get(LPCTSTR namespaceURI)
{
	CString	strnamespaceURI(namespaceURI);
	BSTR	bstrnamespaceURI = strnamespaceURI.AllocSysString();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMSchemaCollection*)P())->get(bstrnamespaceURI, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMSchemaCollection));

	SysFreeString(bstrnamespaceURI);
	return o;
}


void CXMLDOMSchemaCollection::Remove(LPCTSTR namespaceURI)
{
	CString	strnamespaceURI(namespaceURI);
	BSTR	bstrnamespaceURI = strnamespaceURI.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMSchemaCollection*)P())->remove(bstrnamespaceURI);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMSchemaCollection));

	SysFreeString(bstrnamespaceURI);
}


long CXMLDOMSchemaCollection::GetLength()
{
	long o;

	HRESULT	hr = ((Raw_IXMLDOMSchemaCollection*)P())->get_length(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMSchemaCollection));

	return o;
}


CString CXMLDOMSchemaCollection::GetNamespaceURI(long index)
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMSchemaCollection*)P())->get_namespaceURI(index, &bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMSchemaCollection));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMSchemaCollection::AddCollection(IXMLDOMSchemaCollection* otherCollection)
{

	HRESULT	hr = ((Raw_IXMLDOMSchemaCollection*)P())->addCollection(otherCollection);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMSchemaCollection));

}


IUnknown* CXMLDOMSchemaCollection::Get_newEnum()
{
	IUnknown* o;

	HRESULT	hr = ((Raw_IXMLDOMSchemaCollection*)P())->get__newEnum(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMSchemaCollection));

	return o;
}




//--------------------------------------------------------------------
// class CXMLDOMSchemaCollection2
//--------------------------------------------------------------------

CXMLDOMSchemaCollection2::CXMLDOMSchemaCollection2()
:	CInterfaceCallingWrapper<IXMLDOMSchemaCollection2>()
{}


CXMLDOMSchemaCollection2::CXMLDOMSchemaCollection2(IXMLDOMSchemaCollection2 *p, BOOL bIncrementRefCount /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMSchemaCollection2>(p, bIncrementRefCount)
{}


CXMLDOMSchemaCollection2::CXMLDOMSchemaCollection2(const CXMLDOMSchemaCollection2 &o)
:	CInterfaceCallingWrapper<IXMLDOMSchemaCollection2>(o)
{}


CXMLDOMSchemaCollection2::CXMLDOMSchemaCollection2(IUnknown *pUnknown, BOOL bIncrementRefCount /*= FALSE*/, BOOL bThrowException /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMSchemaCollection2>(pUnknown, bIncrementRefCount, bThrowException)
{}


void CXMLDOMSchemaCollection2::Add(LPCTSTR namespaceURI, _variant_t var)
{
	CString	strnamespaceURI(namespaceURI);
	BSTR	bstrnamespaceURI = strnamespaceURI.AllocSysString();
	VARIANT	varvar = var.Detach();

	HRESULT	hr = ((Raw_IXMLDOMSchemaCollection2*)P())->add(bstrnamespaceURI, varvar);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMSchemaCollection2));

	SysFreeString(bstrnamespaceURI);
	VariantClear(&varvar);
}


CXMLDOMNode CXMLDOMSchemaCollection2::Get(LPCTSTR namespaceURI)
{
	CString	strnamespaceURI(namespaceURI);
	BSTR	bstrnamespaceURI = strnamespaceURI.AllocSysString();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMSchemaCollection2*)P())->get(bstrnamespaceURI, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMSchemaCollection2));

	SysFreeString(bstrnamespaceURI);
	return o;
}


void CXMLDOMSchemaCollection2::Remove(LPCTSTR namespaceURI)
{
	CString	strnamespaceURI(namespaceURI);
	BSTR	bstrnamespaceURI = strnamespaceURI.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMSchemaCollection2*)P())->remove(bstrnamespaceURI);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMSchemaCollection2));

	SysFreeString(bstrnamespaceURI);
}


long CXMLDOMSchemaCollection2::GetLength()
{
	long o;

	HRESULT	hr = ((Raw_IXMLDOMSchemaCollection2*)P())->get_length(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMSchemaCollection2));

	return o;
}


CString CXMLDOMSchemaCollection2::GetNamespaceURI(long index)
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMSchemaCollection2*)P())->get_namespaceURI(index, &bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMSchemaCollection2));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMSchemaCollection2::AddCollection(IXMLDOMSchemaCollection* otherCollection)
{

	HRESULT	hr = ((Raw_IXMLDOMSchemaCollection2*)P())->addCollection(otherCollection);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMSchemaCollection2));

}


IUnknown* CXMLDOMSchemaCollection2::Get_newEnum()
{
	IUnknown* o;

	HRESULT	hr = ((Raw_IXMLDOMSchemaCollection2*)P())->get__newEnum(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMSchemaCollection2));

	return o;
}


void CXMLDOMSchemaCollection2::Validate()
{

	HRESULT	hr = ((Raw_IXMLDOMSchemaCollection2*)P())->validate();
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMSchemaCollection2));

}


void CXMLDOMSchemaCollection2::SetValidateOnLoad(BOOL rhs)
{

	HRESULT	hr = ((Raw_IXMLDOMSchemaCollection2*)P())->put_validateOnLoad(rhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMSchemaCollection2));

}


BOOL CXMLDOMSchemaCollection2::GetValidateOnLoad()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMSchemaCollection2*)P())->get_validateOnLoad(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMSchemaCollection2));

	return o;
}


ISchema* CXMLDOMSchemaCollection2::GetSchema(LPCTSTR namespaceURI)
{
	CString	strnamespaceURI(namespaceURI);
	BSTR	bstrnamespaceURI = strnamespaceURI.AllocSysString();
	ISchema* o;

	HRESULT	hr = ((Raw_IXMLDOMSchemaCollection2*)P())->getSchema(bstrnamespaceURI, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMSchemaCollection2));

	SysFreeString(bstrnamespaceURI);
	return o;
}


ISchemaItem* CXMLDOMSchemaCollection2::GetDeclaration(IXMLDOMNode* node)
{
	ISchemaItem* o;

	HRESULT	hr = ((Raw_IXMLDOMSchemaCollection2*)P())->getDeclaration(node, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMSchemaCollection2));

	return o;
}




//--------------------------------------------------------------------
// class CXMLDOMSelection
//--------------------------------------------------------------------

CXMLDOMSelection::CXMLDOMSelection()
:	CInterfaceCallingWrapper<IXMLDOMSelection>()
{}


CXMLDOMSelection::CXMLDOMSelection(IXMLDOMSelection *p, BOOL bIncrementRefCount /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMSelection>(p, bIncrementRefCount)
{}


CXMLDOMSelection::CXMLDOMSelection(const CXMLDOMSelection &o)
:	CInterfaceCallingWrapper<IXMLDOMSelection>(o)
{}


CXMLDOMSelection::CXMLDOMSelection(IUnknown *pUnknown, BOOL bIncrementRefCount /*= FALSE*/, BOOL bThrowException /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMSelection>(pUnknown, bIncrementRefCount, bThrowException)
{}


CXMLDOMNode CXMLDOMSelection::GetItem(long index)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMSelection*)P())->get_item(index, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMSelection));

	return o;
}


long CXMLDOMSelection::GetLength()
{
	long o;

	HRESULT	hr = ((Raw_IXMLDOMSelection*)P())->get_length(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMSelection));

	return o;
}


CXMLDOMNode CXMLDOMSelection::NextNode()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMSelection*)P())->nextNode(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMSelection));

	return o;
}


void CXMLDOMSelection::Reset()
{

	HRESULT	hr = ((Raw_IXMLDOMSelection*)P())->reset();
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMSelection));

}


IUnknown* CXMLDOMSelection::Get_newEnum()
{
	IUnknown* o;

	HRESULT	hr = ((Raw_IXMLDOMSelection*)P())->get__newEnum(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMSelection));

	return o;
}


CString CXMLDOMSelection::GetExpr()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMSelection*)P())->get_expr(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMSelection));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMSelection::SetExpr(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMSelection*)P())->put_expr(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMSelection));

	SysFreeString(bstrrhs);
}


CXMLDOMNode CXMLDOMSelection::GetContext()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMSelection*)P())->get_context(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMSelection));

	return o;
}


void CXMLDOMSelection::SetRefContext(IXMLDOMNode* rhs)
{

	HRESULT	hr = ((Raw_IXMLDOMSelection*)P())->putref_context(rhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMSelection));

}


CXMLDOMNode CXMLDOMSelection::PeekNode()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMSelection*)P())->peekNode(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMSelection));

	return o;
}


CXMLDOMNode CXMLDOMSelection::Matches(IXMLDOMNode* pNode)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMSelection*)P())->matches(pNode, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMSelection));

	return o;
}


CXMLDOMNode CXMLDOMSelection::RemoveNext()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMSelection*)P())->removeNext(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMSelection));

	return o;
}


void CXMLDOMSelection::RemoveAll()
{

	HRESULT	hr = ((Raw_IXMLDOMSelection*)P())->removeAll();
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMSelection));

}


CXMLDOMSelection CXMLDOMSelection::Clone()
{
	IXMLDOMSelection* o;

	HRESULT	hr = ((Raw_IXMLDOMSelection*)P())->clone(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMSelection));

	return o;
}


_variant_t CXMLDOMSelection::GetProperty(LPCTSTR name)
{
	CString	strname(name);
	BSTR	bstrname = strname.AllocSysString();
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMSelection*)P())->getProperty(bstrname, &varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMSelection));

	SysFreeString(bstrname);
	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMSelection::SetProperty(LPCTSTR name, _variant_t value)
{
	CString	strname(name);
	BSTR	bstrname = strname.AllocSysString();
	VARIANT	varvalue = value.Detach();

	HRESULT	hr = ((Raw_IXMLDOMSelection*)P())->setProperty(bstrname, varvalue);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMSelection));

	SysFreeString(bstrname);
	VariantClear(&varvalue);
}




//--------------------------------------------------------------------
// class CXMLDOMText
//--------------------------------------------------------------------

CXMLDOMText::CXMLDOMText()
:	CInterfaceCallingWrapper<IXMLDOMText>()
{}


CXMLDOMText::CXMLDOMText(IXMLDOMText *p, BOOL bIncrementRefCount /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMText>(p, bIncrementRefCount)
{}


CXMLDOMText::CXMLDOMText(const CXMLDOMText &o)
:	CInterfaceCallingWrapper<IXMLDOMText>(o)
{}


CXMLDOMText::CXMLDOMText(IUnknown *pUnknown, BOOL bIncrementRefCount /*= FALSE*/, BOOL bThrowException /*= FALSE*/)
:	CInterfaceCallingWrapper<IXMLDOMText>(pUnknown, bIncrementRefCount, bThrowException)
{}


CString CXMLDOMText::GetNodeName()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->get_nodeName(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


_variant_t CXMLDOMText::GetNodeValue()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->get_nodeValue(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMText::SetNodeValue(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->put_nodeValue(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	VariantClear(&varrhs);
}


DOMNodeType CXMLDOMText::GetNodeType()
{
	DOMNodeType o;

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->get_nodeType(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	return o;
}


CXMLDOMNode CXMLDOMText::GetParentNode()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->get_parentNode(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	return o;
}


CXMLDOMNodeList CXMLDOMText::GetChildNodes()
{
	IXMLDOMNodeList* o;

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->get_childNodes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	return o;
}


CXMLDOMNode CXMLDOMText::GetFirstChild()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->get_firstChild(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	return o;
}


CXMLDOMNode CXMLDOMText::GetLastChild()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->get_lastChild(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	return o;
}


CXMLDOMNode CXMLDOMText::GetPreviousSibling()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->get_previousSibling(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	return o;
}


CXMLDOMNode CXMLDOMText::GetNextSibling()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->get_nextSibling(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	return o;
}


CXMLDOMNamedNodeMap CXMLDOMText::GetAttributes()
{
	IXMLDOMNamedNodeMap* o;

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->get_attributes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	return o;
}


CXMLDOMNode CXMLDOMText::InsertBefore(IXMLDOMNode* newChild, _variant_t refChild)
{
	VARIANT	varrefChild = refChild.Detach();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->insertBefore(newChild, varrefChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	VariantClear(&varrefChild);
	return o;
}


CXMLDOMNode CXMLDOMText::ReplaceChild(IXMLDOMNode* newChild, IXMLDOMNode* oldChild)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->replaceChild(newChild, oldChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	return o;
}


CXMLDOMNode CXMLDOMText::RemoveChild(IXMLDOMNode* childNode)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->removeChild(childNode, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	return o;
}


CXMLDOMNode CXMLDOMText::AppendChild(IXMLDOMNode* newChild)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->appendChild(newChild, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	return o;
}


BOOL CXMLDOMText::HasChildNodes()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->hasChildNodes(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	return o;
}


CXMLDOMDocument CXMLDOMText::GetOwnerDocument()
{
	IXMLDOMDocument* o;

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->get_ownerDocument(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	return o;
}


CXMLDOMNode CXMLDOMText::CloneNode(BOOL deep)
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->cloneNode(deep, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	return o;
}


CString CXMLDOMText::GetNodeTypeString()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->get_nodeTypeString(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMText::GetText()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->get_text(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMText::SetText(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->put_text(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	SysFreeString(bstrrhs);
}


BOOL CXMLDOMText::GetSpecified()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->get_specified(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	return o;
}


CXMLDOMNode CXMLDOMText::GetDefinition()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->get_definition(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	return o;
}


_variant_t CXMLDOMText::GetNodeTypedValue()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->get_nodeTypedValue(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMText::SetNodeTypedValue(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->put_nodeTypedValue(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	VariantClear(&varrhs);
}


_variant_t CXMLDOMText::GetDataType()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->get_dataType(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	_variant_t	o(varO, false);
	return o;
}


void CXMLDOMText::SetDataType(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->put_dataType(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	SysFreeString(bstrrhs);
}


CString CXMLDOMText::GetXml()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->get_xml(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMText::TransformNode(IXMLDOMNode* stylesheet)
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->transformNode(stylesheet, &bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CXMLDOMNodeList CXMLDOMText::SelectNodes(LPCTSTR queryString)
{
	CString	strqueryString(queryString);
	BSTR	bstrqueryString = strqueryString.AllocSysString();
	IXMLDOMNodeList* o;

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->selectNodes(bstrqueryString, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	SysFreeString(bstrqueryString);
	return o;
}


CXMLDOMNode CXMLDOMText::SelectSingleNode(LPCTSTR queryString)
{
	CString	strqueryString(queryString);
	BSTR	bstrqueryString = strqueryString.AllocSysString();
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->selectSingleNode(bstrqueryString, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	SysFreeString(bstrqueryString);
	return o;
}


BOOL CXMLDOMText::GetParsed()
{
	short	o;

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->get_parsed(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	return o;
}


CString CXMLDOMText::GetNamespaceURI()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->get_namespaceURI(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMText::GetPrefix()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->get_prefix(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXMLDOMText::GetBaseName()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->get_baseName(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMText::TransformNodeToObject(IXMLDOMNode* stylesheet, _variant_t outputObject)
{
	VARIANT	varoutputObject = outputObject.Detach();

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->transformNodeToObject(stylesheet, varoutputObject);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	VariantClear(&varoutputObject);
}


CString CXMLDOMText::GetData()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->get_data(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMText::SetData(LPCTSTR rhs)
{
	CString	strrhs(rhs);
	BSTR	bstrrhs = strrhs.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->put_data(bstrrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	SysFreeString(bstrrhs);
}


long CXMLDOMText::GetLength()
{
	long o;

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->get_length(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	return o;
}


CString CXMLDOMText::SubstringData(long offset, long count)
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->substringData(offset, count, &bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXMLDOMText::AppendData(LPCTSTR data)
{
	CString	strdata(data);
	BSTR	bstrdata = strdata.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->appendData(bstrdata);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	SysFreeString(bstrdata);
}


void CXMLDOMText::InsertData(long offset, LPCTSTR data)
{
	CString	strdata(data);
	BSTR	bstrdata = strdata.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->insertData(offset, bstrdata);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	SysFreeString(bstrdata);
}


void CXMLDOMText::DeleteData(long offset, long count)
{

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->deleteData(offset, count);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

}


void CXMLDOMText::ReplaceData(long offset, long count, LPCTSTR data)
{
	CString	strdata(data);
	BSTR	bstrdata = strdata.AllocSysString();

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->replaceData(offset, count, bstrdata);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	SysFreeString(bstrdata);
}


CXMLDOMText CXMLDOMText::SplitText(long offset)
{
	IXMLDOMText* o;

	HRESULT	hr = ((Raw_IXMLDOMText*)P())->splitText(offset, &o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXMLDOMText));

	return o;
}




//--------------------------------------------------------------------
// class CXSLProcessor
//--------------------------------------------------------------------

CXSLProcessor::CXSLProcessor()
:	CInterfaceCallingWrapper<IXSLProcessor>()
{}


CXSLProcessor::CXSLProcessor(IXSLProcessor *p, BOOL bIncrementRefCount /*= FALSE*/)
:	CInterfaceCallingWrapper<IXSLProcessor>(p, bIncrementRefCount)
{}


CXSLProcessor::CXSLProcessor(const CXSLProcessor &o)
:	CInterfaceCallingWrapper<IXSLProcessor>(o)
{}


CXSLProcessor::CXSLProcessor(IUnknown *pUnknown, BOOL bIncrementRefCount /*= FALSE*/, BOOL bThrowException /*= FALSE*/)
:	CInterfaceCallingWrapper<IXSLProcessor>(pUnknown, bIncrementRefCount, bThrowException)
{}


void CXSLProcessor::SetInput(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXSLProcessor*)P())->put_input(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXSLProcessor));

	VariantClear(&varrhs);
}


_variant_t CXSLProcessor::GetInput()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXSLProcessor*)P())->get_input(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXSLProcessor));

	_variant_t	o(varO, false);
	return o;
}


CXSLTemplate CXSLProcessor::GetOwnerTemplate()
{
	IXSLTemplate* o;

	HRESULT	hr = ((Raw_IXSLProcessor*)P())->get_ownerTemplate(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXSLProcessor));

	return o;
}


void CXSLProcessor::SetStartMode(LPCTSTR mode, LPCTSTR namespaceURI)
{
	CString	strmode(mode);
	BSTR	bstrmode = strmode.AllocSysString();
	CString	strnamespaceURI(namespaceURI);
	BSTR	bstrnamespaceURI = strnamespaceURI.AllocSysString();

	HRESULT	hr = ((Raw_IXSLProcessor*)P())->setStartMode(bstrmode, bstrnamespaceURI);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXSLProcessor));

	SysFreeString(bstrmode);
	SysFreeString(bstrnamespaceURI);
}


CString CXSLProcessor::GetStartMode()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXSLProcessor*)P())->get_startMode(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXSLProcessor));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


CString CXSLProcessor::GetStartModeURI()
{
	BSTR	bstrO = NULL;

	HRESULT	hr = ((Raw_IXSLProcessor*)P())->get_startModeURI(&bstrO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXSLProcessor));

	CString	o((LPCTSTR)_bstr_t(bstrO, false));
	return o;
}


void CXSLProcessor::SetOutput(_variant_t rhs)
{
	VARIANT	varrhs = rhs.Detach();

	HRESULT	hr = ((Raw_IXSLProcessor*)P())->put_output(varrhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXSLProcessor));

	VariantClear(&varrhs);
}


_variant_t CXSLProcessor::GetOutput()
{
	VARIANT	varO;
	VariantInit(&varO);

	HRESULT	hr = ((Raw_IXSLProcessor*)P())->get_output(&varO);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXSLProcessor));

	_variant_t	o(varO, false);
	return o;
}


BOOL CXSLProcessor::Transform()
{
	short	o;

	HRESULT	hr = ((Raw_IXSLProcessor*)P())->transform(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXSLProcessor));

	return o;
}


void CXSLProcessor::Reset()
{

	HRESULT	hr = ((Raw_IXSLProcessor*)P())->reset();
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXSLProcessor));

}


long CXSLProcessor::GetReadyState()
{
	long o;

	HRESULT	hr = ((Raw_IXSLProcessor*)P())->get_readyState(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXSLProcessor));

	return o;
}


void CXSLProcessor::AddParameter(LPCTSTR baseName, _variant_t parameter, LPCTSTR namespaceURI)
{
	CString	strbaseName(baseName);
	BSTR	bstrbaseName = strbaseName.AllocSysString();
	VARIANT	varparameter = parameter.Detach();
	CString	strnamespaceURI(namespaceURI);
	BSTR	bstrnamespaceURI = strnamespaceURI.AllocSysString();

	HRESULT	hr = ((Raw_IXSLProcessor*)P())->addParameter(bstrbaseName, varparameter, bstrnamespaceURI);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXSLProcessor));

	SysFreeString(bstrbaseName);
	VariantClear(&varparameter);
	SysFreeString(bstrnamespaceURI);
}


void CXSLProcessor::AddObject(IDispatch* obj, LPCTSTR namespaceURI)
{
	CString	strnamespaceURI(namespaceURI);
	BSTR	bstrnamespaceURI = strnamespaceURI.AllocSysString();

	HRESULT	hr = ((Raw_IXSLProcessor*)P())->addObject(obj, bstrnamespaceURI);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXSLProcessor));

	SysFreeString(bstrnamespaceURI);
}


CXMLDOMNode CXSLProcessor::GetStylesheet()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXSLProcessor*)P())->get_stylesheet(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXSLProcessor));

	return o;
}




//--------------------------------------------------------------------
// class CXSLTemplate
//--------------------------------------------------------------------

CXSLTemplate::CXSLTemplate()
:	CInterfaceCallingWrapper<IXSLTemplate>()
{}


CXSLTemplate::CXSLTemplate(IXSLTemplate *p, BOOL bIncrementRefCount /*= FALSE*/)
:	CInterfaceCallingWrapper<IXSLTemplate>(p, bIncrementRefCount)
{}


CXSLTemplate::CXSLTemplate(const CXSLTemplate &o)
:	CInterfaceCallingWrapper<IXSLTemplate>(o)
{}


CXSLTemplate::CXSLTemplate(IUnknown *pUnknown, BOOL bIncrementRefCount /*= FALSE*/, BOOL bThrowException /*= FALSE*/)
:	CInterfaceCallingWrapper<IXSLTemplate>(pUnknown, bIncrementRefCount, bThrowException)
{}


void CXSLTemplate::SetRefStylesheet(IXMLDOMNode* rhs)
{

	HRESULT	hr = ((Raw_IXSLTemplate*)P())->putref_stylesheet(rhs);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXSLTemplate));

}


CXMLDOMNode CXSLTemplate::GetStylesheet()
{
	IXMLDOMNode* o;

	HRESULT	hr = ((Raw_IXSLTemplate*)P())->get_stylesheet(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXSLTemplate));

	return o;
}


CXSLProcessor CXSLTemplate::CreateProcessor()
{
	IXSLProcessor* o;

	HRESULT	hr = ((Raw_IXSLTemplate*)P())->createProcessor(&o);
	if (hr != S_OK) AfxThrowComException(hr, P(), &__uuidof(IXSLTemplate));

	return o;
}




//--------------------------------------------------------------------
// class CDOMDocumentClass
//--------------------------------------------------------------------

const CLSID CDOMDocumentClass::s_ClsId = {0xf6d90f11, 0x9c73, 0x11d3, {0xb3, 0x2e, 0x00, 0xc0, 0x4f, 0x99, 0x0b, 0xb4}};


CDOMDocumentClass::CDOMDocumentClass()
:	CCoClassWrapper<IXMLDOMDocument2>(s_ClsId)
{}


CXMLDOMDocument2 CDOMDocumentClass::GetXMLDOMDocument2()
{
	IXMLDOMDocument2 *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


XMLDOMDocumentEvents* CDOMDocumentClass::GetXMLDOMDocumentEvents()
{
	XMLDOMDocumentEvents *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}




//--------------------------------------------------------------------
// class CDOMDocument26Class
//--------------------------------------------------------------------

const CLSID CDOMDocument26Class::s_ClsId = {0xf5078f1b, 0xc551, 0x11d3, {0x89, 0xb9, 0x00, 0x00, 0xf8, 0x1f, 0xe2, 0x21}};


CDOMDocument26Class::CDOMDocument26Class()
:	CCoClassWrapper<IXMLDOMDocument2>(s_ClsId)
{}


CXMLDOMDocument2 CDOMDocument26Class::GetXMLDOMDocument2()
{
	IXMLDOMDocument2 *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


XMLDOMDocumentEvents* CDOMDocument26Class::GetXMLDOMDocumentEvents()
{
	XMLDOMDocumentEvents *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}




//--------------------------------------------------------------------
// class CDOMDocument30Class
//--------------------------------------------------------------------

const CLSID CDOMDocument30Class::s_ClsId = {0xf5078f32, 0xc551, 0x11d3, {0x89, 0xb9, 0x00, 0x00, 0xf8, 0x1f, 0xe2, 0x21}};


CDOMDocument30Class::CDOMDocument30Class()
:	CCoClassWrapper<IXMLDOMDocument2>(s_ClsId)
{}


CXMLDOMDocument2 CDOMDocument30Class::GetXMLDOMDocument2()
{
	IXMLDOMDocument2 *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


XMLDOMDocumentEvents* CDOMDocument30Class::GetXMLDOMDocumentEvents()
{
	XMLDOMDocumentEvents *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}




//--------------------------------------------------------------------
// class CDOMDocument40Class
//--------------------------------------------------------------------

const CLSID CDOMDocument40Class::s_ClsId = {0x88d969c0, 0xf192, 0x11d4, {0xa6, 0x5f, 0x00, 0x40, 0x96, 0x32, 0x51, 0xe5}};


CDOMDocument40Class::CDOMDocument40Class()
:	CCoClassWrapper<IXMLDOMDocument2>(s_ClsId)
{}


CXMLDOMDocument2 CDOMDocument40Class::GetXMLDOMDocument2()
{
	IXMLDOMDocument2 *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


XMLDOMDocumentEvents* CDOMDocument40Class::GetXMLDOMDocumentEvents()
{
	XMLDOMDocumentEvents *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}




//--------------------------------------------------------------------
// class CFreeThreadedDOMDocumentClass
//--------------------------------------------------------------------

const CLSID CFreeThreadedDOMDocumentClass::s_ClsId = {0xf6d90f12, 0x9c73, 0x11d3, {0xb3, 0x2e, 0x00, 0xc0, 0x4f, 0x99, 0x0b, 0xb4}};


CFreeThreadedDOMDocumentClass::CFreeThreadedDOMDocumentClass()
:	CCoClassWrapper<IXMLDOMDocument2>(s_ClsId)
{}


CXMLDOMDocument2 CFreeThreadedDOMDocumentClass::GetXMLDOMDocument2()
{
	IXMLDOMDocument2 *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


XMLDOMDocumentEvents* CFreeThreadedDOMDocumentClass::GetXMLDOMDocumentEvents()
{
	XMLDOMDocumentEvents *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}




//--------------------------------------------------------------------
// class CFreeThreadedDOMDocument26Class
//--------------------------------------------------------------------

const CLSID CFreeThreadedDOMDocument26Class::s_ClsId = {0xf5078f1c, 0xc551, 0x11d3, {0x89, 0xb9, 0x00, 0x00, 0xf8, 0x1f, 0xe2, 0x21}};


CFreeThreadedDOMDocument26Class::CFreeThreadedDOMDocument26Class()
:	CCoClassWrapper<IXMLDOMDocument2>(s_ClsId)
{}


CXMLDOMDocument2 CFreeThreadedDOMDocument26Class::GetXMLDOMDocument2()
{
	IXMLDOMDocument2 *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


XMLDOMDocumentEvents* CFreeThreadedDOMDocument26Class::GetXMLDOMDocumentEvents()
{
	XMLDOMDocumentEvents *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}




//--------------------------------------------------------------------
// class CFreeThreadedDOMDocument30Class
//--------------------------------------------------------------------

const CLSID CFreeThreadedDOMDocument30Class::s_ClsId = {0xf5078f33, 0xc551, 0x11d3, {0x89, 0xb9, 0x00, 0x00, 0xf8, 0x1f, 0xe2, 0x21}};


CFreeThreadedDOMDocument30Class::CFreeThreadedDOMDocument30Class()
:	CCoClassWrapper<IXMLDOMDocument2>(s_ClsId)
{}


CXMLDOMDocument2 CFreeThreadedDOMDocument30Class::GetXMLDOMDocument2()
{
	IXMLDOMDocument2 *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


XMLDOMDocumentEvents* CFreeThreadedDOMDocument30Class::GetXMLDOMDocumentEvents()
{
	XMLDOMDocumentEvents *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}




//--------------------------------------------------------------------
// class CFreeThreadedDOMDocument40Class
//--------------------------------------------------------------------

const CLSID CFreeThreadedDOMDocument40Class::s_ClsId = {0x88d969c1, 0xf192, 0x11d4, {0xa6, 0x5f, 0x00, 0x40, 0x96, 0x32, 0x51, 0xe5}};


CFreeThreadedDOMDocument40Class::CFreeThreadedDOMDocument40Class()
:	CCoClassWrapper<IXMLDOMDocument2>(s_ClsId)
{}


CXMLDOMDocument2 CFreeThreadedDOMDocument40Class::GetXMLDOMDocument2()
{
	IXMLDOMDocument2 *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}


XMLDOMDocumentEvents* CFreeThreadedDOMDocument40Class::GetXMLDOMDocumentEvents()
{
	XMLDOMDocumentEvents *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}




//--------------------------------------------------------------------
// class CXMLSchemaCacheClass
//--------------------------------------------------------------------

const CLSID CXMLSchemaCacheClass::s_ClsId = {0x373984c9, 0xb845, 0x449b, {0x91, 0xe7, 0x45, 0xac, 0x83, 0x03, 0x6a, 0xde}};


CXMLSchemaCacheClass::CXMLSchemaCacheClass()
:	CCoClassWrapper<IXMLDOMSchemaCollection>(s_ClsId)
{}


CXMLDOMSchemaCollection CXMLSchemaCacheClass::GetXMLDOMSchemaCollection()
{
	IXMLDOMSchemaCollection *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}




//--------------------------------------------------------------------
// class CXMLSchemaCache26Class
//--------------------------------------------------------------------

const CLSID CXMLSchemaCache26Class::s_ClsId = {0xf5078f1d, 0xc551, 0x11d3, {0x89, 0xb9, 0x00, 0x00, 0xf8, 0x1f, 0xe2, 0x21}};


CXMLSchemaCache26Class::CXMLSchemaCache26Class()
:	CCoClassWrapper<IXMLDOMSchemaCollection>(s_ClsId)
{}


CXMLDOMSchemaCollection CXMLSchemaCache26Class::GetXMLDOMSchemaCollection()
{
	IXMLDOMSchemaCollection *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}




//--------------------------------------------------------------------
// class CXMLSchemaCache30Class
//--------------------------------------------------------------------

const CLSID CXMLSchemaCache30Class::s_ClsId = {0xf5078f34, 0xc551, 0x11d3, {0x89, 0xb9, 0x00, 0x00, 0xf8, 0x1f, 0xe2, 0x21}};


CXMLSchemaCache30Class::CXMLSchemaCache30Class()
:	CCoClassWrapper<IXMLDOMSchemaCollection>(s_ClsId)
{}


CXMLDOMSchemaCollection CXMLSchemaCache30Class::GetXMLDOMSchemaCollection()
{
	IXMLDOMSchemaCollection *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}




//--------------------------------------------------------------------
// class CXMLSchemaCache40Class
//--------------------------------------------------------------------

const CLSID CXMLSchemaCache40Class::s_ClsId = {0x88d969c2, 0xf192, 0x11d4, {0xa6, 0x5f, 0x00, 0x40, 0x96, 0x32, 0x51, 0xe5}};


CXMLSchemaCache40Class::CXMLSchemaCache40Class()
:	CCoClassWrapper<IXMLDOMSchemaCollection2>(s_ClsId)
{}


CXMLDOMSchemaCollection2 CXMLSchemaCache40Class::GetXMLDOMSchemaCollection2()
{
	IXMLDOMSchemaCollection2 *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}




//--------------------------------------------------------------------
// class CXSLTemplateClass
//--------------------------------------------------------------------

const CLSID CXSLTemplateClass::s_ClsId = {0x2933bf94, 0x7b36, 0x11d2, {0xb2, 0x0e, 0x00, 0xc0, 0x4f, 0x98, 0x3e, 0x60}};


CXSLTemplateClass::CXSLTemplateClass()
:	CCoClassWrapper<IXSLTemplate>(s_ClsId)
{}


CXSLTemplate CXSLTemplateClass::GetXSLTemplate()
{
	IXSLTemplate *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}




//--------------------------------------------------------------------
// class CXSLTemplate26Class
//--------------------------------------------------------------------

const CLSID CXSLTemplate26Class::s_ClsId = {0xf5078f21, 0xc551, 0x11d3, {0x89, 0xb9, 0x00, 0x00, 0xf8, 0x1f, 0xe2, 0x21}};


CXSLTemplate26Class::CXSLTemplate26Class()
:	CCoClassWrapper<IXSLTemplate>(s_ClsId)
{}


CXSLTemplate CXSLTemplate26Class::GetXSLTemplate()
{
	IXSLTemplate *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}




//--------------------------------------------------------------------
// class CXSLTemplate30Class
//--------------------------------------------------------------------

const CLSID CXSLTemplate30Class::s_ClsId = {0xf5078f36, 0xc551, 0x11d3, {0x89, 0xb9, 0x00, 0x00, 0xf8, 0x1f, 0xe2, 0x21}};


CXSLTemplate30Class::CXSLTemplate30Class()
:	CCoClassWrapper<IXSLTemplate>(s_ClsId)
{}


CXSLTemplate CXSLTemplate30Class::GetXSLTemplate()
{
	IXSLTemplate *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}




//--------------------------------------------------------------------
// class CXSLTemplate40Class
//--------------------------------------------------------------------

const CLSID CXSLTemplate40Class::s_ClsId = {0x88d969c3, 0xf192, 0x11d4, {0xa6, 0x5f, 0x00, 0x40, 0x96, 0x32, 0x51, 0xe5}};


CXSLTemplate40Class::CXSLTemplate40Class()
:	CCoClassWrapper<IXSLTemplate>(s_ClsId)
{}


CXSLTemplate CXSLTemplate40Class::GetXSLTemplate()
{
	IXSLTemplate *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}




//--------------------------------------------------------------------
// class CXMLHTTPClass
//--------------------------------------------------------------------

const CLSID CXMLHTTPClass::s_ClsId = {0xf6d90f16, 0x9c73, 0x11d3, {0xb3, 0x2e, 0x00, 0xc0, 0x4f, 0x99, 0x0b, 0xb4}};


CXMLHTTPClass::CXMLHTTPClass()
:	CCoClassWrapper<IXMLHTTPRequest>(s_ClsId)
{}


CXMLHTTPRequest CXMLHTTPClass::GetXMLHTTPRequest()
{
	IXMLHTTPRequest *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}




//--------------------------------------------------------------------
// class CXMLHTTP26Class
//--------------------------------------------------------------------

const CLSID CXMLHTTP26Class::s_ClsId = {0xf5078f1e, 0xc551, 0x11d3, {0x89, 0xb9, 0x00, 0x00, 0xf8, 0x1f, 0xe2, 0x21}};


CXMLHTTP26Class::CXMLHTTP26Class()
:	CCoClassWrapper<IXMLHTTPRequest>(s_ClsId)
{}


CXMLHTTPRequest CXMLHTTP26Class::GetXMLHTTPRequest()
{
	IXMLHTTPRequest *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}




//--------------------------------------------------------------------
// class CXMLHTTP30Class
//--------------------------------------------------------------------

const CLSID CXMLHTTP30Class::s_ClsId = {0xf5078f35, 0xc551, 0x11d3, {0x89, 0xb9, 0x00, 0x00, 0xf8, 0x1f, 0xe2, 0x21}};


CXMLHTTP30Class::CXMLHTTP30Class()
:	CCoClassWrapper<IXMLHTTPRequest>(s_ClsId)
{}


CXMLHTTPRequest CXMLHTTP30Class::GetXMLHTTPRequest()
{
	IXMLHTTPRequest *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}




//--------------------------------------------------------------------
// class CXMLHTTP40Class
//--------------------------------------------------------------------

const CLSID CXMLHTTP40Class::s_ClsId = {0x88d969c5, 0xf192, 0x11d4, {0xa6, 0x5f, 0x00, 0x40, 0x96, 0x32, 0x51, 0xe5}};


CXMLHTTP40Class::CXMLHTTP40Class()
:	CCoClassWrapper<IXMLHTTPRequest>(s_ClsId)
{}


CXMLHTTPRequest CXMLHTTP40Class::GetXMLHTTPRequest()
{
	IXMLHTTPRequest *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}




//--------------------------------------------------------------------
// class CServerXMLHTTPClass
//--------------------------------------------------------------------

const CLSID CServerXMLHTTPClass::s_ClsId = {0xafba6b42, 0x5692, 0x48ea, {0x81, 0x41, 0xdc, 0x51, 0x7d, 0xcf, 0x0e, 0xf1}};


CServerXMLHTTPClass::CServerXMLHTTPClass()
:	CCoClassWrapper<IServerXMLHTTPRequest2>(s_ClsId)
{}


IServerXMLHTTPRequest2* CServerXMLHTTPClass::GetServerXMLHTTPRequest2()
{
	IServerXMLHTTPRequest2 *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}




//--------------------------------------------------------------------
// class CServerXMLHTTP30Class
//--------------------------------------------------------------------

const CLSID CServerXMLHTTP30Class::s_ClsId = {0xafb40ffd, 0xb609, 0x40a3, {0x98, 0x28, 0xf8, 0x8b, 0xbe, 0x11, 0xe4, 0xe3}};


CServerXMLHTTP30Class::CServerXMLHTTP30Class()
:	CCoClassWrapper<IServerXMLHTTPRequest2>(s_ClsId)
{}


IServerXMLHTTPRequest2* CServerXMLHTTP30Class::GetServerXMLHTTPRequest2()
{
	IServerXMLHTTPRequest2 *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}




//--------------------------------------------------------------------
// class CServerXMLHTTP40Class
//--------------------------------------------------------------------

const CLSID CServerXMLHTTP40Class::s_ClsId = {0x88d969c6, 0xf192, 0x11d4, {0xa6, 0x5f, 0x00, 0x40, 0x96, 0x32, 0x51, 0xe5}};


CServerXMLHTTP40Class::CServerXMLHTTP40Class()
:	CCoClassWrapper<IServerXMLHTTPRequest2>(s_ClsId)
{}


IServerXMLHTTPRequest2* CServerXMLHTTP40Class::GetServerXMLHTTPRequest2()
{
	IServerXMLHTTPRequest2 *p = NULL;
	HRESULT	hr = m_p.QueryInterface(&p);
	if (hr != S_OK) AfxThrowComException(hr);
	return p;
}





} // namespace MsXml

