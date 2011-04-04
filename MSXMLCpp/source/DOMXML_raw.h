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

struct Raw_IXMLDOMDocument : Raw_IDispatch
{
	virtual HRESULT __stdcall get_nodeName(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_nodeValue(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_nodeValue(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall get_nodeType(
		DOMNodeType* Result) = 0;
	virtual HRESULT __stdcall get_parentNode(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_childNodes(
		IXMLDOMNodeList** Result) = 0;
	virtual HRESULT __stdcall get_firstChild(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_lastChild(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_previousSibling(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nextSibling(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_attributes(
		IXMLDOMNamedNodeMap** Result) = 0;
	virtual HRESULT __stdcall insertBefore(
		IXMLDOMNode* newChild,
		VARIANT refChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall replaceChild(
		IXMLDOMNode* newChild,
		IXMLDOMNode* oldChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall removeChild(
		IXMLDOMNode* childNode,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall appendChild(
		IXMLDOMNode* newChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall hasChildNodes(
		short* Result) = 0;
	virtual HRESULT __stdcall get_ownerDocument(
		IXMLDOMDocument** Result) = 0;
	virtual HRESULT __stdcall cloneNode(
		short deep,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nodeTypeString(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_text(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall put_text(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_specified(
		short* Result) = 0;
	virtual HRESULT __stdcall get_definition(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nodeTypedValue(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_nodeTypedValue(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall get_dataType(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_dataType(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_xml(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall transformNode(
		IXMLDOMNode* stylesheet,
		BSTR* Result) = 0;
	virtual HRESULT __stdcall selectNodes(
		BSTR queryString,
		IXMLDOMNodeList** Result) = 0;
	virtual HRESULT __stdcall selectSingleNode(
		BSTR queryString,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_parsed(
		short* Result) = 0;
	virtual HRESULT __stdcall get_namespaceURI(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_prefix(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_baseName(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall transformNodeToObject(
		IXMLDOMNode* stylesheet,
		VARIANT outputObject) = 0;
	virtual HRESULT __stdcall get_doctype(
		IXMLDOMDocumentType** Result) = 0;
	virtual HRESULT __stdcall get_implementation(
		IXMLDOMImplementation** Result) = 0;
	virtual HRESULT __stdcall get_documentElement(
		IXMLDOMElement** Result) = 0;
	virtual HRESULT __stdcall putref_documentElement(
		IXMLDOMElement* rhs) = 0;
	virtual HRESULT __stdcall createElement(
		BSTR tagName,
		IXMLDOMElement** Result) = 0;
	virtual HRESULT __stdcall createDocumentFragment(
		IXMLDOMDocumentFragment** Result) = 0;
	virtual HRESULT __stdcall createTextNode(
		BSTR data,
		IXMLDOMText** Result) = 0;
	virtual HRESULT __stdcall createComment(
		BSTR data,
		IXMLDOMComment** Result) = 0;
	virtual HRESULT __stdcall createCDATASection(
		BSTR data,
		IXMLDOMCDATASection** Result) = 0;
	virtual HRESULT __stdcall createProcessingInstruction(
		BSTR target,
		BSTR data,
		IXMLDOMProcessingInstruction** Result) = 0;
	virtual HRESULT __stdcall createAttribute(
		BSTR name,
		IXMLDOMAttribute** Result) = 0;
	virtual HRESULT __stdcall createEntityReference(
		BSTR name,
		IXMLDOMEntityReference** Result) = 0;
	virtual HRESULT __stdcall getElementsByTagName(
		BSTR tagName,
		IXMLDOMNodeList** Result) = 0;
	virtual HRESULT __stdcall createNode(
		VARIANT type,
		BSTR name,
		BSTR namespaceURI,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall nodeFromID(
		BSTR idString,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall load(
		VARIANT xmlSource,
		short* Result) = 0;
	virtual HRESULT __stdcall get_readyState(
		long* Result) = 0;
	virtual HRESULT __stdcall get_parseError(
		IXMLDOMParseError** Result) = 0;
	virtual HRESULT __stdcall get_url(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_async(
		short* Result) = 0;
	virtual HRESULT __stdcall put_async(
		short rhs) = 0;
	virtual HRESULT __stdcall abort() = 0;
	virtual HRESULT __stdcall loadXML(
		BSTR bstrXML,
		short* Result) = 0;
	virtual HRESULT __stdcall save(
		VARIANT destination) = 0;
	virtual HRESULT __stdcall get_validateOnParse(
		short* Result) = 0;
	virtual HRESULT __stdcall put_validateOnParse(
		short rhs) = 0;
	virtual HRESULT __stdcall get_resolveExternals(
		short* Result) = 0;
	virtual HRESULT __stdcall put_resolveExternals(
		short rhs) = 0;
	virtual HRESULT __stdcall get_preserveWhiteSpace(
		short* Result) = 0;
	virtual HRESULT __stdcall put_preserveWhiteSpace(
		short rhs) = 0;
	virtual HRESULT __stdcall put_onreadystatechange(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall put_ondataavailable(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall put_ontransformnode(
		VARIANT rhs) = 0;
};

struct Raw_IXMLDOMDocument2 : Raw_IDispatch
{
	virtual HRESULT __stdcall get_nodeName(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_nodeValue(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_nodeValue(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall get_nodeType(
		DOMNodeType* Result) = 0;
	virtual HRESULT __stdcall get_parentNode(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_childNodes(
		IXMLDOMNodeList** Result) = 0;
	virtual HRESULT __stdcall get_firstChild(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_lastChild(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_previousSibling(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nextSibling(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_attributes(
		IXMLDOMNamedNodeMap** Result) = 0;
	virtual HRESULT __stdcall insertBefore(
		IXMLDOMNode* newChild,
		VARIANT refChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall replaceChild(
		IXMLDOMNode* newChild,
		IXMLDOMNode* oldChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall removeChild(
		IXMLDOMNode* childNode,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall appendChild(
		IXMLDOMNode* newChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall hasChildNodes(
		short* Result) = 0;
	virtual HRESULT __stdcall get_ownerDocument(
		IXMLDOMDocument** Result) = 0;
	virtual HRESULT __stdcall cloneNode(
		short deep,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nodeTypeString(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_text(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall put_text(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_specified(
		short* Result) = 0;
	virtual HRESULT __stdcall get_definition(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nodeTypedValue(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_nodeTypedValue(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall get_dataType(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_dataType(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_xml(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall transformNode(
		IXMLDOMNode* stylesheet,
		BSTR* Result) = 0;
	virtual HRESULT __stdcall selectNodes(
		BSTR queryString,
		IXMLDOMNodeList** Result) = 0;
	virtual HRESULT __stdcall selectSingleNode(
		BSTR queryString,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_parsed(
		short* Result) = 0;
	virtual HRESULT __stdcall get_namespaceURI(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_prefix(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_baseName(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall transformNodeToObject(
		IXMLDOMNode* stylesheet,
		VARIANT outputObject) = 0;
	virtual HRESULT __stdcall get_doctype(
		IXMLDOMDocumentType** Result) = 0;
	virtual HRESULT __stdcall get_implementation(
		IXMLDOMImplementation** Result) = 0;
	virtual HRESULT __stdcall get_documentElement(
		IXMLDOMElement** Result) = 0;
	virtual HRESULT __stdcall putref_documentElement(
		IXMLDOMElement* rhs) = 0;
	virtual HRESULT __stdcall createElement(
		BSTR tagName,
		IXMLDOMElement** Result) = 0;
	virtual HRESULT __stdcall createDocumentFragment(
		IXMLDOMDocumentFragment** Result) = 0;
	virtual HRESULT __stdcall createTextNode(
		BSTR data,
		IXMLDOMText** Result) = 0;
	virtual HRESULT __stdcall createComment(
		BSTR data,
		IXMLDOMComment** Result) = 0;
	virtual HRESULT __stdcall createCDATASection(
		BSTR data,
		IXMLDOMCDATASection** Result) = 0;
	virtual HRESULT __stdcall createProcessingInstruction(
		BSTR target,
		BSTR data,
		IXMLDOMProcessingInstruction** Result) = 0;
	virtual HRESULT __stdcall createAttribute(
		BSTR name,
		IXMLDOMAttribute** Result) = 0;
	virtual HRESULT __stdcall createEntityReference(
		BSTR name,
		IXMLDOMEntityReference** Result) = 0;
	virtual HRESULT __stdcall getElementsByTagName(
		BSTR tagName,
		IXMLDOMNodeList** Result) = 0;
	virtual HRESULT __stdcall createNode(
		VARIANT type,
		BSTR name,
		BSTR namespaceURI,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall nodeFromID(
		BSTR idString,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall load(
		VARIANT xmlSource,
		short* Result) = 0;
	virtual HRESULT __stdcall get_readyState(
		long* Result) = 0;
	virtual HRESULT __stdcall get_parseError(
		IXMLDOMParseError** Result) = 0;
	virtual HRESULT __stdcall get_url(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_async(
		short* Result) = 0;
	virtual HRESULT __stdcall put_async(
		short rhs) = 0;
	virtual HRESULT __stdcall abort() = 0;
	virtual HRESULT __stdcall loadXML(
		BSTR bstrXML,
		short* Result) = 0;
	virtual HRESULT __stdcall save(
		VARIANT destination) = 0;
	virtual HRESULT __stdcall get_validateOnParse(
		short* Result) = 0;
	virtual HRESULT __stdcall put_validateOnParse(
		short rhs) = 0;
	virtual HRESULT __stdcall get_resolveExternals(
		short* Result) = 0;
	virtual HRESULT __stdcall put_resolveExternals(
		short rhs) = 0;
	virtual HRESULT __stdcall get_preserveWhiteSpace(
		short* Result) = 0;
	virtual HRESULT __stdcall put_preserveWhiteSpace(
		short rhs) = 0;
	virtual HRESULT __stdcall put_onreadystatechange(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall put_ondataavailable(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall put_ontransformnode(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall get_namespaces(
		IXMLDOMSchemaCollection** Result) = 0;
	virtual HRESULT __stdcall get_schemas(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall putref_schemas(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall validate(
		IXMLDOMParseError** Result) = 0;
	virtual HRESULT __stdcall setProperty(
		BSTR name,
		VARIANT value) = 0;
	virtual HRESULT __stdcall getProperty(
		BSTR name,
		VARIANT* Result) = 0;
};

struct Raw_IXMLDOMNamedNodeMap : Raw_IDispatch
{
	virtual HRESULT __stdcall getNamedItem(
		BSTR name,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall setNamedItem(
		IXMLDOMNode* newItem,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall removeNamedItem(
		BSTR name,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_item(
		long index,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_length(
		long* Result) = 0;
	virtual HRESULT __stdcall getQualifiedItem(
		BSTR baseName,
		BSTR namespaceURI,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall removeQualifiedItem(
		BSTR baseName,
		BSTR namespaceURI,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall nextNode(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall reset() = 0;
	virtual HRESULT __stdcall get__newEnum(
		IUnknown** Result) = 0;
};

struct Raw_IXMLDOMNode : Raw_IDispatch
{
	virtual HRESULT __stdcall get_nodeName(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_nodeValue(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_nodeValue(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall get_nodeType(
		DOMNodeType* Result) = 0;
	virtual HRESULT __stdcall get_parentNode(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_childNodes(
		IXMLDOMNodeList** Result) = 0;
	virtual HRESULT __stdcall get_firstChild(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_lastChild(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_previousSibling(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nextSibling(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_attributes(
		IXMLDOMNamedNodeMap** Result) = 0;
	virtual HRESULT __stdcall insertBefore(
		IXMLDOMNode* newChild,
		VARIANT refChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall replaceChild(
		IXMLDOMNode* newChild,
		IXMLDOMNode* oldChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall removeChild(
		IXMLDOMNode* childNode,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall appendChild(
		IXMLDOMNode* newChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall hasChildNodes(
		short* Result) = 0;
	virtual HRESULT __stdcall get_ownerDocument(
		IXMLDOMDocument** Result) = 0;
	virtual HRESULT __stdcall cloneNode(
		short deep,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nodeTypeString(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_text(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall put_text(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_specified(
		short* Result) = 0;
	virtual HRESULT __stdcall get_definition(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nodeTypedValue(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_nodeTypedValue(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall get_dataType(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_dataType(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_xml(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall transformNode(
		IXMLDOMNode* stylesheet,
		BSTR* Result) = 0;
	virtual HRESULT __stdcall selectNodes(
		BSTR queryString,
		IXMLDOMNodeList** Result) = 0;
	virtual HRESULT __stdcall selectSingleNode(
		BSTR queryString,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_parsed(
		short* Result) = 0;
	virtual HRESULT __stdcall get_namespaceURI(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_prefix(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_baseName(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall transformNodeToObject(
		IXMLDOMNode* stylesheet,
		VARIANT outputObject) = 0;
};

struct Raw_IXMLDOMNodeList : Raw_IDispatch
{
	virtual HRESULT __stdcall get_item(
		long index,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_length(
		long* Result) = 0;
	virtual HRESULT __stdcall nextNode(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall reset() = 0;
	virtual HRESULT __stdcall get__newEnum(
		IUnknown** Result) = 0;
};

struct Raw_IXMLDOMParseError : Raw_IDispatch
{
	virtual HRESULT __stdcall get_errorCode(
		long* Result) = 0;
	virtual HRESULT __stdcall get_url(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_reason(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_srcText(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_line(
		long* Result) = 0;
	virtual HRESULT __stdcall get_linepos(
		long* Result) = 0;
	virtual HRESULT __stdcall get_filepos(
		long* Result) = 0;
};

struct Raw_IXMLHTTPRequest : Raw_IDispatch
{
	virtual HRESULT __stdcall open(
		BSTR bstrMethod,
		BSTR bstrUrl,
		VARIANT varAsync,
		VARIANT bstrUser,
		VARIANT bstrPassword) = 0;
	virtual HRESULT __stdcall setRequestHeader(
		BSTR bstrHeader,
		BSTR bstrValue) = 0;
	virtual HRESULT __stdcall getResponseHeader(
		BSTR bstrHeader,
		BSTR* Result) = 0;
	virtual HRESULT __stdcall getAllResponseHeaders(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall send(
		VARIANT varBody) = 0;
	virtual HRESULT __stdcall abort() = 0;
	virtual HRESULT __stdcall get_status(
		long* Result) = 0;
	virtual HRESULT __stdcall get_statusText(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_responseXML(
		IDispatch** Result) = 0;
	virtual HRESULT __stdcall get_responseText(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_responseBody(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall get_responseStream(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall get_readyState(
		long* Result) = 0;
	virtual HRESULT __stdcall put_onreadystatechange(
		IDispatch* rhs) = 0;
};

struct Raw_IServerXMLHTTPRequest : Raw_IDispatch
{
	virtual HRESULT __stdcall open(
		BSTR bstrMethod,
		BSTR bstrUrl,
		VARIANT varAsync,
		VARIANT bstrUser,
		VARIANT bstrPassword) = 0;
	virtual HRESULT __stdcall setRequestHeader(
		BSTR bstrHeader,
		BSTR bstrValue) = 0;
	virtual HRESULT __stdcall getResponseHeader(
		BSTR bstrHeader,
		BSTR* Result) = 0;
	virtual HRESULT __stdcall getAllResponseHeaders(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall send(
		VARIANT varBody) = 0;
	virtual HRESULT __stdcall abort() = 0;
	virtual HRESULT __stdcall get_status(
		long* Result) = 0;
	virtual HRESULT __stdcall get_statusText(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_responseXML(
		IDispatch** Result) = 0;
	virtual HRESULT __stdcall get_responseText(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_responseBody(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall get_responseStream(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall get_readyState(
		long* Result) = 0;
	virtual HRESULT __stdcall put_onreadystatechange(
		IDispatch* rhs) = 0;
	virtual HRESULT __stdcall setTimeouts(
		long resolveTimeout,
		long connectTimeout,
		long sendTimeout,
		long receiveTimeout) = 0;
	virtual HRESULT __stdcall waitForResponse(
		VARIANT timeoutInSeconds,
		short* Result) = 0;
	virtual HRESULT __stdcall getOption(
		SERVERXMLHTTP_OPTION option,
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall setOption(
		SERVERXMLHTTP_OPTION option,
		VARIANT value) = 0;
};

struct Raw_IXMLDOMAttribute : Raw_IDispatch
{
	virtual HRESULT __stdcall get_nodeName(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_nodeValue(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_nodeValue(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall get_nodeType(
		DOMNodeType* Result) = 0;
	virtual HRESULT __stdcall get_parentNode(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_childNodes(
		IXMLDOMNodeList** Result) = 0;
	virtual HRESULT __stdcall get_firstChild(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_lastChild(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_previousSibling(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nextSibling(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_attributes(
		IXMLDOMNamedNodeMap** Result) = 0;
	virtual HRESULT __stdcall insertBefore(
		IXMLDOMNode* newChild,
		VARIANT refChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall replaceChild(
		IXMLDOMNode* newChild,
		IXMLDOMNode* oldChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall removeChild(
		IXMLDOMNode* childNode,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall appendChild(
		IXMLDOMNode* newChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall hasChildNodes(
		short* Result) = 0;
	virtual HRESULT __stdcall get_ownerDocument(
		IXMLDOMDocument** Result) = 0;
	virtual HRESULT __stdcall cloneNode(
		short deep,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nodeTypeString(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_text(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall put_text(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_specified(
		short* Result) = 0;
	virtual HRESULT __stdcall get_definition(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nodeTypedValue(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_nodeTypedValue(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall get_dataType(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_dataType(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_xml(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall transformNode(
		IXMLDOMNode* stylesheet,
		BSTR* Result) = 0;
	virtual HRESULT __stdcall selectNodes(
		BSTR queryString,
		IXMLDOMNodeList** Result) = 0;
	virtual HRESULT __stdcall selectSingleNode(
		BSTR queryString,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_parsed(
		short* Result) = 0;
	virtual HRESULT __stdcall get_namespaceURI(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_prefix(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_baseName(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall transformNodeToObject(
		IXMLDOMNode* stylesheet,
		VARIANT outputObject) = 0;
	virtual HRESULT __stdcall get_name(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_value(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_value(
		VARIANT rhs) = 0;
};

struct Raw_IXMLDOMCDATASection : Raw_IDispatch
{
	virtual HRESULT __stdcall get_nodeName(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_nodeValue(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_nodeValue(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall get_nodeType(
		DOMNodeType* Result) = 0;
	virtual HRESULT __stdcall get_parentNode(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_childNodes(
		IXMLDOMNodeList** Result) = 0;
	virtual HRESULT __stdcall get_firstChild(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_lastChild(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_previousSibling(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nextSibling(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_attributes(
		IXMLDOMNamedNodeMap** Result) = 0;
	virtual HRESULT __stdcall insertBefore(
		IXMLDOMNode* newChild,
		VARIANT refChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall replaceChild(
		IXMLDOMNode* newChild,
		IXMLDOMNode* oldChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall removeChild(
		IXMLDOMNode* childNode,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall appendChild(
		IXMLDOMNode* newChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall hasChildNodes(
		short* Result) = 0;
	virtual HRESULT __stdcall get_ownerDocument(
		IXMLDOMDocument** Result) = 0;
	virtual HRESULT __stdcall cloneNode(
		short deep,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nodeTypeString(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_text(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall put_text(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_specified(
		short* Result) = 0;
	virtual HRESULT __stdcall get_definition(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nodeTypedValue(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_nodeTypedValue(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall get_dataType(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_dataType(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_xml(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall transformNode(
		IXMLDOMNode* stylesheet,
		BSTR* Result) = 0;
	virtual HRESULT __stdcall selectNodes(
		BSTR queryString,
		IXMLDOMNodeList** Result) = 0;
	virtual HRESULT __stdcall selectSingleNode(
		BSTR queryString,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_parsed(
		short* Result) = 0;
	virtual HRESULT __stdcall get_namespaceURI(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_prefix(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_baseName(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall transformNodeToObject(
		IXMLDOMNode* stylesheet,
		VARIANT outputObject) = 0;
	virtual HRESULT __stdcall get_data(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall put_data(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_length(
		long* Result) = 0;
	virtual HRESULT __stdcall substringData(
		long offset,
		long count,
		BSTR* Result) = 0;
	virtual HRESULT __stdcall appendData(
		BSTR data) = 0;
	virtual HRESULT __stdcall insertData(
		long offset,
		BSTR data) = 0;
	virtual HRESULT __stdcall deleteData(
		long offset,
		long count) = 0;
	virtual HRESULT __stdcall replaceData(
		long offset,
		long count,
		BSTR data) = 0;
	virtual HRESULT __stdcall splitText(
		long offset,
		IXMLDOMText** Result) = 0;
};

struct Raw_IXMLDOMCharacterData : Raw_IDispatch
{
	virtual HRESULT __stdcall get_nodeName(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_nodeValue(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_nodeValue(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall get_nodeType(
		DOMNodeType* Result) = 0;
	virtual HRESULT __stdcall get_parentNode(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_childNodes(
		IXMLDOMNodeList** Result) = 0;
	virtual HRESULT __stdcall get_firstChild(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_lastChild(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_previousSibling(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nextSibling(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_attributes(
		IXMLDOMNamedNodeMap** Result) = 0;
	virtual HRESULT __stdcall insertBefore(
		IXMLDOMNode* newChild,
		VARIANT refChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall replaceChild(
		IXMLDOMNode* newChild,
		IXMLDOMNode* oldChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall removeChild(
		IXMLDOMNode* childNode,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall appendChild(
		IXMLDOMNode* newChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall hasChildNodes(
		short* Result) = 0;
	virtual HRESULT __stdcall get_ownerDocument(
		IXMLDOMDocument** Result) = 0;
	virtual HRESULT __stdcall cloneNode(
		short deep,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nodeTypeString(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_text(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall put_text(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_specified(
		short* Result) = 0;
	virtual HRESULT __stdcall get_definition(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nodeTypedValue(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_nodeTypedValue(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall get_dataType(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_dataType(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_xml(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall transformNode(
		IXMLDOMNode* stylesheet,
		BSTR* Result) = 0;
	virtual HRESULT __stdcall selectNodes(
		BSTR queryString,
		IXMLDOMNodeList** Result) = 0;
	virtual HRESULT __stdcall selectSingleNode(
		BSTR queryString,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_parsed(
		short* Result) = 0;
	virtual HRESULT __stdcall get_namespaceURI(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_prefix(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_baseName(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall transformNodeToObject(
		IXMLDOMNode* stylesheet,
		VARIANT outputObject) = 0;
	virtual HRESULT __stdcall get_data(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall put_data(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_length(
		long* Result) = 0;
	virtual HRESULT __stdcall substringData(
		long offset,
		long count,
		BSTR* Result) = 0;
	virtual HRESULT __stdcall appendData(
		BSTR data) = 0;
	virtual HRESULT __stdcall insertData(
		long offset,
		BSTR data) = 0;
	virtual HRESULT __stdcall deleteData(
		long offset,
		long count) = 0;
	virtual HRESULT __stdcall replaceData(
		long offset,
		long count,
		BSTR data) = 0;
};

struct Raw_IXMLDOMComment : Raw_IDispatch
{
	virtual HRESULT __stdcall get_nodeName(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_nodeValue(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_nodeValue(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall get_nodeType(
		DOMNodeType* Result) = 0;
	virtual HRESULT __stdcall get_parentNode(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_childNodes(
		IXMLDOMNodeList** Result) = 0;
	virtual HRESULT __stdcall get_firstChild(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_lastChild(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_previousSibling(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nextSibling(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_attributes(
		IXMLDOMNamedNodeMap** Result) = 0;
	virtual HRESULT __stdcall insertBefore(
		IXMLDOMNode* newChild,
		VARIANT refChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall replaceChild(
		IXMLDOMNode* newChild,
		IXMLDOMNode* oldChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall removeChild(
		IXMLDOMNode* childNode,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall appendChild(
		IXMLDOMNode* newChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall hasChildNodes(
		short* Result) = 0;
	virtual HRESULT __stdcall get_ownerDocument(
		IXMLDOMDocument** Result) = 0;
	virtual HRESULT __stdcall cloneNode(
		short deep,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nodeTypeString(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_text(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall put_text(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_specified(
		short* Result) = 0;
	virtual HRESULT __stdcall get_definition(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nodeTypedValue(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_nodeTypedValue(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall get_dataType(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_dataType(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_xml(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall transformNode(
		IXMLDOMNode* stylesheet,
		BSTR* Result) = 0;
	virtual HRESULT __stdcall selectNodes(
		BSTR queryString,
		IXMLDOMNodeList** Result) = 0;
	virtual HRESULT __stdcall selectSingleNode(
		BSTR queryString,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_parsed(
		short* Result) = 0;
	virtual HRESULT __stdcall get_namespaceURI(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_prefix(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_baseName(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall transformNodeToObject(
		IXMLDOMNode* stylesheet,
		VARIANT outputObject) = 0;
	virtual HRESULT __stdcall get_data(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall put_data(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_length(
		long* Result) = 0;
	virtual HRESULT __stdcall substringData(
		long offset,
		long count,
		BSTR* Result) = 0;
	virtual HRESULT __stdcall appendData(
		BSTR data) = 0;
	virtual HRESULT __stdcall insertData(
		long offset,
		BSTR data) = 0;
	virtual HRESULT __stdcall deleteData(
		long offset,
		long count) = 0;
	virtual HRESULT __stdcall replaceData(
		long offset,
		long count,
		BSTR data) = 0;
};

struct Raw_IXMLDOMDocumentFragment : Raw_IDispatch
{
	virtual HRESULT __stdcall get_nodeName(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_nodeValue(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_nodeValue(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall get_nodeType(
		DOMNodeType* Result) = 0;
	virtual HRESULT __stdcall get_parentNode(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_childNodes(
		IXMLDOMNodeList** Result) = 0;
	virtual HRESULT __stdcall get_firstChild(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_lastChild(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_previousSibling(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nextSibling(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_attributes(
		IXMLDOMNamedNodeMap** Result) = 0;
	virtual HRESULT __stdcall insertBefore(
		IXMLDOMNode* newChild,
		VARIANT refChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall replaceChild(
		IXMLDOMNode* newChild,
		IXMLDOMNode* oldChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall removeChild(
		IXMLDOMNode* childNode,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall appendChild(
		IXMLDOMNode* newChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall hasChildNodes(
		short* Result) = 0;
	virtual HRESULT __stdcall get_ownerDocument(
		IXMLDOMDocument** Result) = 0;
	virtual HRESULT __stdcall cloneNode(
		short deep,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nodeTypeString(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_text(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall put_text(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_specified(
		short* Result) = 0;
	virtual HRESULT __stdcall get_definition(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nodeTypedValue(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_nodeTypedValue(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall get_dataType(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_dataType(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_xml(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall transformNode(
		IXMLDOMNode* stylesheet,
		BSTR* Result) = 0;
	virtual HRESULT __stdcall selectNodes(
		BSTR queryString,
		IXMLDOMNodeList** Result) = 0;
	virtual HRESULT __stdcall selectSingleNode(
		BSTR queryString,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_parsed(
		short* Result) = 0;
	virtual HRESULT __stdcall get_namespaceURI(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_prefix(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_baseName(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall transformNodeToObject(
		IXMLDOMNode* stylesheet,
		VARIANT outputObject) = 0;
};

struct Raw_IXMLDOMDocumentType : Raw_IDispatch
{
	virtual HRESULT __stdcall get_nodeName(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_nodeValue(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_nodeValue(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall get_nodeType(
		DOMNodeType* Result) = 0;
	virtual HRESULT __stdcall get_parentNode(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_childNodes(
		IXMLDOMNodeList** Result) = 0;
	virtual HRESULT __stdcall get_firstChild(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_lastChild(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_previousSibling(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nextSibling(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_attributes(
		IXMLDOMNamedNodeMap** Result) = 0;
	virtual HRESULT __stdcall insertBefore(
		IXMLDOMNode* newChild,
		VARIANT refChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall replaceChild(
		IXMLDOMNode* newChild,
		IXMLDOMNode* oldChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall removeChild(
		IXMLDOMNode* childNode,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall appendChild(
		IXMLDOMNode* newChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall hasChildNodes(
		short* Result) = 0;
	virtual HRESULT __stdcall get_ownerDocument(
		IXMLDOMDocument** Result) = 0;
	virtual HRESULT __stdcall cloneNode(
		short deep,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nodeTypeString(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_text(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall put_text(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_specified(
		short* Result) = 0;
	virtual HRESULT __stdcall get_definition(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nodeTypedValue(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_nodeTypedValue(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall get_dataType(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_dataType(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_xml(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall transformNode(
		IXMLDOMNode* stylesheet,
		BSTR* Result) = 0;
	virtual HRESULT __stdcall selectNodes(
		BSTR queryString,
		IXMLDOMNodeList** Result) = 0;
	virtual HRESULT __stdcall selectSingleNode(
		BSTR queryString,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_parsed(
		short* Result) = 0;
	virtual HRESULT __stdcall get_namespaceURI(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_prefix(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_baseName(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall transformNodeToObject(
		IXMLDOMNode* stylesheet,
		VARIANT outputObject) = 0;
	virtual HRESULT __stdcall get_name(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_entities(
		IXMLDOMNamedNodeMap** Result) = 0;
	virtual HRESULT __stdcall get_notations(
		IXMLDOMNamedNodeMap** Result) = 0;
};

struct Raw_IXMLDOMElement : Raw_IDispatch
{
	virtual HRESULT __stdcall get_nodeName(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_nodeValue(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_nodeValue(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall get_nodeType(
		DOMNodeType* Result) = 0;
	virtual HRESULT __stdcall get_parentNode(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_childNodes(
		IXMLDOMNodeList** Result) = 0;
	virtual HRESULT __stdcall get_firstChild(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_lastChild(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_previousSibling(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nextSibling(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_attributes(
		IXMLDOMNamedNodeMap** Result) = 0;
	virtual HRESULT __stdcall insertBefore(
		IXMLDOMNode* newChild,
		VARIANT refChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall replaceChild(
		IXMLDOMNode* newChild,
		IXMLDOMNode* oldChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall removeChild(
		IXMLDOMNode* childNode,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall appendChild(
		IXMLDOMNode* newChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall hasChildNodes(
		short* Result) = 0;
	virtual HRESULT __stdcall get_ownerDocument(
		IXMLDOMDocument** Result) = 0;
	virtual HRESULT __stdcall cloneNode(
		short deep,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nodeTypeString(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_text(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall put_text(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_specified(
		short* Result) = 0;
	virtual HRESULT __stdcall get_definition(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nodeTypedValue(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_nodeTypedValue(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall get_dataType(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_dataType(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_xml(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall transformNode(
		IXMLDOMNode* stylesheet,
		BSTR* Result) = 0;
	virtual HRESULT __stdcall selectNodes(
		BSTR queryString,
		IXMLDOMNodeList** Result) = 0;
	virtual HRESULT __stdcall selectSingleNode(
		BSTR queryString,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_parsed(
		short* Result) = 0;
	virtual HRESULT __stdcall get_namespaceURI(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_prefix(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_baseName(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall transformNodeToObject(
		IXMLDOMNode* stylesheet,
		VARIANT outputObject) = 0;
	virtual HRESULT __stdcall get_tagName(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall getAttribute(
		BSTR name,
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall setAttribute(
		BSTR name,
		VARIANT value) = 0;
	virtual HRESULT __stdcall removeAttribute(
		BSTR name) = 0;
	virtual HRESULT __stdcall getAttributeNode(
		BSTR name,
		IXMLDOMAttribute** Result) = 0;
	virtual HRESULT __stdcall setAttributeNode(
		IXMLDOMAttribute* DOMAttribute,
		IXMLDOMAttribute** Result) = 0;
	virtual HRESULT __stdcall removeAttributeNode(
		IXMLDOMAttribute* DOMAttribute,
		IXMLDOMAttribute** Result) = 0;
	virtual HRESULT __stdcall getElementsByTagName(
		BSTR tagName,
		IXMLDOMNodeList** Result) = 0;
	virtual HRESULT __stdcall normalize() = 0;
};

struct Raw_IXMLDOMEntity : Raw_IDispatch
{
	virtual HRESULT __stdcall get_nodeName(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_nodeValue(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_nodeValue(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall get_nodeType(
		DOMNodeType* Result) = 0;
	virtual HRESULT __stdcall get_parentNode(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_childNodes(
		IXMLDOMNodeList** Result) = 0;
	virtual HRESULT __stdcall get_firstChild(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_lastChild(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_previousSibling(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nextSibling(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_attributes(
		IXMLDOMNamedNodeMap** Result) = 0;
	virtual HRESULT __stdcall insertBefore(
		IXMLDOMNode* newChild,
		VARIANT refChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall replaceChild(
		IXMLDOMNode* newChild,
		IXMLDOMNode* oldChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall removeChild(
		IXMLDOMNode* childNode,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall appendChild(
		IXMLDOMNode* newChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall hasChildNodes(
		short* Result) = 0;
	virtual HRESULT __stdcall get_ownerDocument(
		IXMLDOMDocument** Result) = 0;
	virtual HRESULT __stdcall cloneNode(
		short deep,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nodeTypeString(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_text(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall put_text(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_specified(
		short* Result) = 0;
	virtual HRESULT __stdcall get_definition(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nodeTypedValue(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_nodeTypedValue(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall get_dataType(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_dataType(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_xml(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall transformNode(
		IXMLDOMNode* stylesheet,
		BSTR* Result) = 0;
	virtual HRESULT __stdcall selectNodes(
		BSTR queryString,
		IXMLDOMNodeList** Result) = 0;
	virtual HRESULT __stdcall selectSingleNode(
		BSTR queryString,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_parsed(
		short* Result) = 0;
	virtual HRESULT __stdcall get_namespaceURI(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_prefix(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_baseName(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall transformNodeToObject(
		IXMLDOMNode* stylesheet,
		VARIANT outputObject) = 0;
	virtual HRESULT __stdcall get_publicId(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall get_systemId(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall get_notationName(
		BSTR* Result) = 0;
};

struct Raw_IXMLDOMEntityReference : Raw_IDispatch
{
	virtual HRESULT __stdcall get_nodeName(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_nodeValue(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_nodeValue(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall get_nodeType(
		DOMNodeType* Result) = 0;
	virtual HRESULT __stdcall get_parentNode(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_childNodes(
		IXMLDOMNodeList** Result) = 0;
	virtual HRESULT __stdcall get_firstChild(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_lastChild(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_previousSibling(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nextSibling(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_attributes(
		IXMLDOMNamedNodeMap** Result) = 0;
	virtual HRESULT __stdcall insertBefore(
		IXMLDOMNode* newChild,
		VARIANT refChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall replaceChild(
		IXMLDOMNode* newChild,
		IXMLDOMNode* oldChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall removeChild(
		IXMLDOMNode* childNode,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall appendChild(
		IXMLDOMNode* newChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall hasChildNodes(
		short* Result) = 0;
	virtual HRESULT __stdcall get_ownerDocument(
		IXMLDOMDocument** Result) = 0;
	virtual HRESULT __stdcall cloneNode(
		short deep,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nodeTypeString(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_text(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall put_text(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_specified(
		short* Result) = 0;
	virtual HRESULT __stdcall get_definition(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nodeTypedValue(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_nodeTypedValue(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall get_dataType(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_dataType(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_xml(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall transformNode(
		IXMLDOMNode* stylesheet,
		BSTR* Result) = 0;
	virtual HRESULT __stdcall selectNodes(
		BSTR queryString,
		IXMLDOMNodeList** Result) = 0;
	virtual HRESULT __stdcall selectSingleNode(
		BSTR queryString,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_parsed(
		short* Result) = 0;
	virtual HRESULT __stdcall get_namespaceURI(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_prefix(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_baseName(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall transformNodeToObject(
		IXMLDOMNode* stylesheet,
		VARIANT outputObject) = 0;
};

struct Raw_IXMLDOMImplementation : Raw_IDispatch
{
	virtual HRESULT __stdcall hasFeature(
		BSTR feature,
		BSTR version,
		short* Result) = 0;
};

struct Raw_IXMLDOMNotation : Raw_IDispatch
{
	virtual HRESULT __stdcall get_nodeName(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_nodeValue(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_nodeValue(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall get_nodeType(
		DOMNodeType* Result) = 0;
	virtual HRESULT __stdcall get_parentNode(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_childNodes(
		IXMLDOMNodeList** Result) = 0;
	virtual HRESULT __stdcall get_firstChild(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_lastChild(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_previousSibling(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nextSibling(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_attributes(
		IXMLDOMNamedNodeMap** Result) = 0;
	virtual HRESULT __stdcall insertBefore(
		IXMLDOMNode* newChild,
		VARIANT refChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall replaceChild(
		IXMLDOMNode* newChild,
		IXMLDOMNode* oldChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall removeChild(
		IXMLDOMNode* childNode,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall appendChild(
		IXMLDOMNode* newChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall hasChildNodes(
		short* Result) = 0;
	virtual HRESULT __stdcall get_ownerDocument(
		IXMLDOMDocument** Result) = 0;
	virtual HRESULT __stdcall cloneNode(
		short deep,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nodeTypeString(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_text(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall put_text(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_specified(
		short* Result) = 0;
	virtual HRESULT __stdcall get_definition(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nodeTypedValue(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_nodeTypedValue(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall get_dataType(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_dataType(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_xml(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall transformNode(
		IXMLDOMNode* stylesheet,
		BSTR* Result) = 0;
	virtual HRESULT __stdcall selectNodes(
		BSTR queryString,
		IXMLDOMNodeList** Result) = 0;
	virtual HRESULT __stdcall selectSingleNode(
		BSTR queryString,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_parsed(
		short* Result) = 0;
	virtual HRESULT __stdcall get_namespaceURI(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_prefix(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_baseName(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall transformNodeToObject(
		IXMLDOMNode* stylesheet,
		VARIANT outputObject) = 0;
	virtual HRESULT __stdcall get_publicId(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall get_systemId(
		VARIANT* Result) = 0;
};

struct Raw_IXMLDOMProcessingInstruction : Raw_IDispatch
{
	virtual HRESULT __stdcall get_nodeName(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_nodeValue(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_nodeValue(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall get_nodeType(
		DOMNodeType* Result) = 0;
	virtual HRESULT __stdcall get_parentNode(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_childNodes(
		IXMLDOMNodeList** Result) = 0;
	virtual HRESULT __stdcall get_firstChild(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_lastChild(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_previousSibling(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nextSibling(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_attributes(
		IXMLDOMNamedNodeMap** Result) = 0;
	virtual HRESULT __stdcall insertBefore(
		IXMLDOMNode* newChild,
		VARIANT refChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall replaceChild(
		IXMLDOMNode* newChild,
		IXMLDOMNode* oldChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall removeChild(
		IXMLDOMNode* childNode,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall appendChild(
		IXMLDOMNode* newChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall hasChildNodes(
		short* Result) = 0;
	virtual HRESULT __stdcall get_ownerDocument(
		IXMLDOMDocument** Result) = 0;
	virtual HRESULT __stdcall cloneNode(
		short deep,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nodeTypeString(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_text(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall put_text(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_specified(
		short* Result) = 0;
	virtual HRESULT __stdcall get_definition(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nodeTypedValue(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_nodeTypedValue(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall get_dataType(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_dataType(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_xml(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall transformNode(
		IXMLDOMNode* stylesheet,
		BSTR* Result) = 0;
	virtual HRESULT __stdcall selectNodes(
		BSTR queryString,
		IXMLDOMNodeList** Result) = 0;
	virtual HRESULT __stdcall selectSingleNode(
		BSTR queryString,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_parsed(
		short* Result) = 0;
	virtual HRESULT __stdcall get_namespaceURI(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_prefix(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_baseName(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall transformNodeToObject(
		IXMLDOMNode* stylesheet,
		VARIANT outputObject) = 0;
	virtual HRESULT __stdcall get_target(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_data(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall put_data(
		BSTR rhs) = 0;
};

struct Raw_IXMLDOMSchemaCollection : Raw_IDispatch
{
	virtual HRESULT __stdcall add(
		BSTR namespaceURI,
		VARIANT var) = 0;
	virtual HRESULT __stdcall get(
		BSTR namespaceURI,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall remove(
		BSTR namespaceURI) = 0;
	virtual HRESULT __stdcall get_length(
		long* Result) = 0;
	virtual HRESULT __stdcall get_namespaceURI(
		long index,
		BSTR* Result) = 0;
	virtual HRESULT __stdcall addCollection(
		IXMLDOMSchemaCollection* otherCollection) = 0;
	virtual HRESULT __stdcall get__newEnum(
		IUnknown** Result) = 0;
};

struct Raw_IXMLDOMSchemaCollection2 : Raw_IDispatch
{
	virtual HRESULT __stdcall add(
		BSTR namespaceURI,
		VARIANT var) = 0;
	virtual HRESULT __stdcall get(
		BSTR namespaceURI,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall remove(
		BSTR namespaceURI) = 0;
	virtual HRESULT __stdcall get_length(
		long* Result) = 0;
	virtual HRESULT __stdcall get_namespaceURI(
		long index,
		BSTR* Result) = 0;
	virtual HRESULT __stdcall addCollection(
		IXMLDOMSchemaCollection* otherCollection) = 0;
	virtual HRESULT __stdcall get__newEnum(
		IUnknown** Result) = 0;
	virtual HRESULT __stdcall validate() = 0;
	virtual HRESULT __stdcall put_validateOnLoad(
		short rhs) = 0;
	virtual HRESULT __stdcall get_validateOnLoad(
		short* Result) = 0;
	virtual HRESULT __stdcall getSchema(
		BSTR namespaceURI,
		ISchema** Result) = 0;
	virtual HRESULT __stdcall getDeclaration(
		IXMLDOMNode* node,
		ISchemaItem** Result) = 0;
};

struct Raw_IXMLDOMSelection : Raw_IDispatch
{
	virtual HRESULT __stdcall get_item(
		long index,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_length(
		long* Result) = 0;
	virtual HRESULT __stdcall nextNode(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall reset() = 0;
	virtual HRESULT __stdcall get__newEnum(
		IUnknown** Result) = 0;
	virtual HRESULT __stdcall get_expr(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall put_expr(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_context(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall putref_context(
		IXMLDOMNode* rhs) = 0;
	virtual HRESULT __stdcall peekNode(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall matches(
		IXMLDOMNode* pNode,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall removeNext(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall removeAll() = 0;
	virtual HRESULT __stdcall clone(
		IXMLDOMSelection** Result) = 0;
	virtual HRESULT __stdcall getProperty(
		BSTR name,
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall setProperty(
		BSTR name,
		VARIANT value) = 0;
};

struct Raw_IXMLDOMText : Raw_IDispatch
{
	virtual HRESULT __stdcall get_nodeName(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_nodeValue(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_nodeValue(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall get_nodeType(
		DOMNodeType* Result) = 0;
	virtual HRESULT __stdcall get_parentNode(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_childNodes(
		IXMLDOMNodeList** Result) = 0;
	virtual HRESULT __stdcall get_firstChild(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_lastChild(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_previousSibling(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nextSibling(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_attributes(
		IXMLDOMNamedNodeMap** Result) = 0;
	virtual HRESULT __stdcall insertBefore(
		IXMLDOMNode* newChild,
		VARIANT refChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall replaceChild(
		IXMLDOMNode* newChild,
		IXMLDOMNode* oldChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall removeChild(
		IXMLDOMNode* childNode,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall appendChild(
		IXMLDOMNode* newChild,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall hasChildNodes(
		short* Result) = 0;
	virtual HRESULT __stdcall get_ownerDocument(
		IXMLDOMDocument** Result) = 0;
	virtual HRESULT __stdcall cloneNode(
		short deep,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nodeTypeString(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_text(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall put_text(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_specified(
		short* Result) = 0;
	virtual HRESULT __stdcall get_definition(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_nodeTypedValue(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_nodeTypedValue(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall get_dataType(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall put_dataType(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_xml(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall transformNode(
		IXMLDOMNode* stylesheet,
		BSTR* Result) = 0;
	virtual HRESULT __stdcall selectNodes(
		BSTR queryString,
		IXMLDOMNodeList** Result) = 0;
	virtual HRESULT __stdcall selectSingleNode(
		BSTR queryString,
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall get_parsed(
		short* Result) = 0;
	virtual HRESULT __stdcall get_namespaceURI(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_prefix(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_baseName(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall transformNodeToObject(
		IXMLDOMNode* stylesheet,
		VARIANT outputObject) = 0;
	virtual HRESULT __stdcall get_data(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall put_data(
		BSTR rhs) = 0;
	virtual HRESULT __stdcall get_length(
		long* Result) = 0;
	virtual HRESULT __stdcall substringData(
		long offset,
		long count,
		BSTR* Result) = 0;
	virtual HRESULT __stdcall appendData(
		BSTR data) = 0;
	virtual HRESULT __stdcall insertData(
		long offset,
		BSTR data) = 0;
	virtual HRESULT __stdcall deleteData(
		long offset,
		long count) = 0;
	virtual HRESULT __stdcall replaceData(
		long offset,
		long count,
		BSTR data) = 0;
	virtual HRESULT __stdcall splitText(
		long offset,
		IXMLDOMText** Result) = 0;
};

struct Raw_IXSLProcessor : Raw_IDispatch
{
	virtual HRESULT __stdcall put_input(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall get_input(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall get_ownerTemplate(
		IXSLTemplate** Result) = 0;
	virtual HRESULT __stdcall setStartMode(
		BSTR mode,
		BSTR namespaceURI) = 0;
	virtual HRESULT __stdcall get_startMode(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall get_startModeURI(
		BSTR* Result) = 0;
	virtual HRESULT __stdcall put_output(
		VARIANT rhs) = 0;
	virtual HRESULT __stdcall get_output(
		VARIANT* Result) = 0;
	virtual HRESULT __stdcall transform(
		short* Result) = 0;
	virtual HRESULT __stdcall reset() = 0;
	virtual HRESULT __stdcall get_readyState(
		long* Result) = 0;
	virtual HRESULT __stdcall addParameter(
		BSTR baseName,
		VARIANT parameter,
		BSTR namespaceURI) = 0;
	virtual HRESULT __stdcall addObject(
		IDispatch* obj,
		BSTR namespaceURI) = 0;
	virtual HRESULT __stdcall get_stylesheet(
		IXMLDOMNode** Result) = 0;
};

struct Raw_IXSLTemplate : Raw_IDispatch
{
	virtual HRESULT __stdcall putref_stylesheet(
		IXMLDOMNode* rhs) = 0;
	virtual HRESULT __stdcall get_stylesheet(
		IXMLDOMNode** Result) = 0;
	virtual HRESULT __stdcall createProcessor(
		IXSLProcessor** Result) = 0;
};

