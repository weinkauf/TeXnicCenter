/********************************************************************
*
* File created by COM Interface Wrapper Wizard (CIWW) ver 1.00.000
* at Wed Oct 01 18:25:36 2003
* 
* (CIWW - Copyright © 2001-2002 Sven Wiegand (sven.wiegand@web.de))
*
********************************************************************/

#ifndef __SAXXML_H__
#define __SAXXML_H__

#include "../../InterfaceWrapper/Include/InterfaceWrapper.h"
#include <msxml2.h>

namespace MsXml
{



class CMXAttributes;
class CMXWriter;
class CVBSAXXMLReader;
class CVBSAXLocator;
class CVBSAXAttributes;
class CVBSAXXMLFilter;
class CSAXXMLReaderClass;
class CSAXXMLReader30Class;
class CSAXXMLReader40Class;
class CMXXMLWriterClass;
class CMXXMLWriter30Class;
class CMXXMLWriter40Class;
class CSAXAttributesClass;
class CSAXAttributes30Class;
class CSAXAttributes40Class;


class CMXAttributes : public CInterfaceCallingWrapper<IMXAttributes>
{
// construction/destruction
public:
	CMXAttributes();
	CMXAttributes(IMXAttributes *p, BOOL bIncrementRefCount = FALSE);
	CMXAttributes(const CMXAttributes &o);
	CMXAttributes(IUnknown *pUnknown, BOOL bIncrementRefCount = FALSE, BOOL bThrowException = FALSE);

// method wrappers
public:
	/** Add an attribute to the end of the list. */
	void AddAttribute(LPCTSTR strURI, LPCTSTR strLocalName, LPCTSTR strQName, LPCTSTR strType, LPCTSTR strValue);

	/** Add an attribute, whose value is equal to the indexed attribute in the input attributes object, to the end of the list. */
	void AddAttributeFromIndex(_variant_t varAtts, INT nIndex);

	/** Clear the attribute list for reuse. */
	void Clear();

	/** Remove an attribute from the list. */
	void RemoveAttribute(INT nIndex);

	/** Set an attribute in the list. */
	void SetAttribute(INT nIndex, LPCTSTR strURI, LPCTSTR strLocalName, LPCTSTR strQName, LPCTSTR strType, LPCTSTR strValue);

	/** Copy an entire Attributes object. */
	void SetAttributes(_variant_t varAtts);

	/** Set the local name of a specific attribute. */
	void SetLocalName(INT nIndex, LPCTSTR strLocalName);

	/** Set the qualified name of a specific attribute. */
	void SetQName(INT nIndex, LPCTSTR strQName);

	/** Set the type of a specific attribute. */
	void SetType(INT nIndex, LPCTSTR strType);

	/** Set the Namespace URI of a specific attribute. */
	void SetURI(INT nIndex, LPCTSTR strURI);

	/** Set the value of a specific attribute. */
	void SetValue(INT nIndex, LPCTSTR strValue);

};



class CMXWriter : public CInterfaceCallingWrapper<IMXWriter>
{
// construction/destruction
public:
	CMXWriter();
	CMXWriter(IMXWriter *p, BOOL bIncrementRefCount = FALSE);
	CMXWriter(const CMXWriter &o);
	CMXWriter(IUnknown *pUnknown, BOOL bIncrementRefCount = FALSE, BOOL bThrowException = FALSE);

// method wrappers
public:
	/** Set or get the output. */
	void SetOutput(_variant_t rhs);

	/** Set or get the output. */
	_variant_t GetOutput();

	/** Set or get the output encoding. */
	void SetEncoding(LPCTSTR rhs);

	/** Set or get the output encoding. */
	CString GetEncoding();

	/** Determine whether or not to write the byte order mark */
	void SetByteOrderMark(BOOL rhs);

	/** Determine whether or not to write the byte order mark */
	BOOL GetByteOrderMark();

	/** Enable or disable auto indent mode. */
	void SetIndent(BOOL rhs);

	/** Enable or disable auto indent mode. */
	BOOL GetIndent();

	/** Set or get the standalone document declaration. */
	void SetStandalone(BOOL rhs);

	/** Set or get the standalone document declaration. */
	BOOL GetStandalone();

	/** Determine whether or not to omit the XML declaration. */
	void SetOmitXMLDeclaration(BOOL rhs);

	/** Determine whether or not to omit the XML declaration. */
	BOOL GetOmitXMLDeclaration();

	/** Set or get the xml version info. */
	void SetVersion(LPCTSTR rhs);

	/** Set or get the xml version info. */
	CString GetVersion();

	/** When enabled, the writer no longer escapes out its input when writing it out. */
	void SetDisableOutputEscaping(BOOL rhs);

	/** When enabled, the writer no longer escapes out its input when writing it out. */
	BOOL GetDisableOutputEscaping();

	/** Flushes all writer buffers forcing the writer to write to the underlying output object */
	void Flush();

};



class CVBSAXXMLReader : public CInterfaceCallingWrapper<IVBSAXXMLReader>
{
// construction/destruction
public:
	CVBSAXXMLReader();
	CVBSAXXMLReader(IVBSAXXMLReader *p, BOOL bIncrementRefCount = FALSE);
	CVBSAXXMLReader(const CVBSAXXMLReader &o);
	CVBSAXXMLReader(IUnknown *pUnknown, BOOL bIncrementRefCount = FALSE, BOOL bThrowException = FALSE);

// method wrappers
public:
	/** Look up the value of a feature. */
	BOOL GetFeature(LPCTSTR strName);

	/** Set the state of a feature. */
	void PutFeature(LPCTSTR strName, BOOL fValue);

	/** Look up the value of a property. */
	_variant_t GetProperty(LPCTSTR strName);

	/** Set the value of a property. */
	void PutProperty(LPCTSTR strName, _variant_t varValue);

	/** Allow an application to register an entity resolver or look up the current entity resolver. */
	IVBSAXEntityResolver* GetEntityResolver();

	/** Allow an application to register an entity resolver or look up the current entity resolver. */
	void SetRefEntityResolver(IVBSAXEntityResolver* rhs);

	/** Allow an application to register a content event handler or look up the current content event handler. */
	IVBSAXContentHandler* GetContentHandler();

	/** Allow an application to register a content event handler or look up the current content event handler. */
	void SetRefContentHandler(IVBSAXContentHandler* rhs);

	/** Allow an application to register a DTD event handler or look up the current DTD event handler. */
	IVBSAXDTDHandler* GetDtdHandler();

	/** Allow an application to register a DTD event handler or look up the current DTD event handler. */
	void SetRefDtdHandler(IVBSAXDTDHandler* rhs);

	/** Allow an application to register an error event handler or look up the current error event handler. */
	IVBSAXErrorHandler* GetErrorHandler();

	/** Allow an application to register an error event handler or look up the current error event handler. */
	void SetRefErrorHandler(IVBSAXErrorHandler* rhs);

	/** Set or get the base URL for the document. */
	CString GetBaseURL();

	/** Set or get the base URL for the document. */
	void SetBaseURL(LPCTSTR rhs);

	/** Set or get the secure base URL for the document. */
	CString GetSecureBaseURL();

	/** Set or get the secure base URL for the document. */
	void SetSecureBaseURL(LPCTSTR rhs);

	/** Parse an XML document. */
	void Parse(_variant_t varInput);

	/** Parse an XML document from a system identifier (URI). */
	void ParseURL(LPCTSTR strURL);

};



class CVBSAXLocator : public CInterfaceCallingWrapper<IVBSAXLocator>
{
// construction/destruction
public:
	CVBSAXLocator();
	CVBSAXLocator(IVBSAXLocator *p, BOOL bIncrementRefCount = FALSE);
	CVBSAXLocator(const CVBSAXLocator &o);
	CVBSAXLocator(IUnknown *pUnknown, BOOL bIncrementRefCount = FALSE, BOOL bThrowException = FALSE);

// method wrappers
public:
	/** Get the column number where the current document event ends. */
	INT GetColumnNumber();

	/** Get the line number where the current document event ends. */
	INT GetLineNumber();

	/** Get the public identifier for the current document event. */
	CString GetPublicId();

	/** Get the system identifier for the current document event. */
	CString GetSystemId();

};



class CVBSAXAttributes : public CInterfaceCallingWrapper<IVBSAXAttributes>
{
// construction/destruction
public:
	CVBSAXAttributes();
	CVBSAXAttributes(IVBSAXAttributes *p, BOOL bIncrementRefCount = FALSE);
	CVBSAXAttributes(const CVBSAXAttributes &o);
	CVBSAXAttributes(IUnknown *pUnknown, BOOL bIncrementRefCount = FALSE, BOOL bThrowException = FALSE);

// method wrappers
public:
	/** Get the number of attributes in the list. */
	INT GetLength();

	/** Look up an attribute's Namespace URI by index. */
	CString GetURI(INT nIndex);

	/** Look up an attribute's local name by index. */
	CString GetLocalName(INT nIndex);

	/** Look up an attribute's XML 1.0 qualified name by index. */
	CString GetQName(INT nIndex);

	/** Look up the index of an attribute by Namespace name. */
	INT GetIndexFromName(LPCTSTR strURI, LPCTSTR strLocalName);

	/** Look up the index of an attribute by XML 1.0 qualified name. */
	INT GetIndexFromQName(LPCTSTR strQName);

	/** Look up an attribute's type by index. */
	CString GetType(INT nIndex);

	/** Look up an attribute's type by Namespace name. */
	CString GetTypeFromName(LPCTSTR strURI, LPCTSTR strLocalName);

	/** Look up an attribute's type by XML 1.0 qualified name. */
	CString GetTypeFromQName(LPCTSTR strQName);

	/** Look up an attribute's value by index. */
	CString GetValue(INT nIndex);

	/** Look up an attribute's value by Namespace name. */
	CString GetValueFromName(LPCTSTR strURI, LPCTSTR strLocalName);

	/** Look up an attribute's value by XML 1.0 qualified name. */
	CString GetValueFromQName(LPCTSTR strQName);

};



class CVBSAXXMLFilter : public CInterfaceCallingWrapper<IVBSAXXMLFilter>
{
// construction/destruction
public:
	CVBSAXXMLFilter();
	CVBSAXXMLFilter(IVBSAXXMLFilter *p, BOOL bIncrementRefCount = FALSE);
	CVBSAXXMLFilter(const CVBSAXXMLFilter &o);
	CVBSAXXMLFilter(IUnknown *pUnknown, BOOL bIncrementRefCount = FALSE, BOOL bThrowException = FALSE);

// method wrappers
public:
	/** Set or get the parent reader */
	CVBSAXXMLReader GetParent();

	/** Set or get the parent reader */
	void SetRefParent(IVBSAXXMLReader* rhs);

};



class CSAXXMLReaderClass : public CCoClassWrapper<IVBSAXXMLReader>
{
// construction/destruction
public:
	CSAXXMLReaderClass();

// operations
public:
	CVBSAXXMLReader GetVBSAXXMLReader();
	ISAXXMLReader* GetSAXXMLReader();
	IMXReaderControl* GetMXReaderControl();

// Attributes
public:
	static const CLSID s_ClsId;
};



class CSAXXMLReader30Class : public CCoClassWrapper<IVBSAXXMLReader>
{
// construction/destruction
public:
	CSAXXMLReader30Class();

// operations
public:
	CVBSAXXMLReader GetVBSAXXMLReader();
	ISAXXMLReader* GetSAXXMLReader();
	IMXReaderControl* GetMXReaderControl();

// Attributes
public:
	static const CLSID s_ClsId;
};



class CSAXXMLReader40Class : public CCoClassWrapper<IVBSAXXMLReader>
{
// construction/destruction
public:
	CSAXXMLReader40Class();

// operations
public:
	CVBSAXXMLReader GetVBSAXXMLReader();
	ISAXXMLReader* GetSAXXMLReader();

// Attributes
public:
	static const CLSID s_ClsId;
};



class CMXXMLWriterClass : public CCoClassWrapper<IMXWriter>
{
// construction/destruction
public:
	CMXXMLWriterClass();

// operations
public:
	CMXWriter GetMXWriter();
	ISAXContentHandler* GetSAXContentHandler();
	ISAXErrorHandler* GetSAXErrorHandler();
	ISAXDTDHandler* GetSAXDTDHandler();
	ISAXLexicalHandler* GetSAXLexicalHandler();
	ISAXDeclHandler* GetSAXDeclHandler();
	IVBSAXContentHandler* GetVBSAXContentHandler();
	IVBSAXDeclHandler* GetVBSAXDeclHandler();
	IVBSAXDTDHandler* GetVBSAXDTDHandler();
	IVBSAXErrorHandler* GetVBSAXErrorHandler();
	IVBSAXLexicalHandler* GetVBSAXLexicalHandler();

// Attributes
public:
	static const CLSID s_ClsId;
};



class CMXXMLWriter30Class : public CCoClassWrapper<IMXWriter>
{
// construction/destruction
public:
	CMXXMLWriter30Class();

// operations
public:
	CMXWriter GetMXWriter();
	ISAXContentHandler* GetSAXContentHandler();
	ISAXDeclHandler* GetSAXDeclHandler();
	ISAXDTDHandler* GetSAXDTDHandler();
	ISAXErrorHandler* GetSAXErrorHandler();
	ISAXLexicalHandler* GetSAXLexicalHandler();
	IVBSAXContentHandler* GetVBSAXContentHandler();
	IVBSAXDeclHandler* GetVBSAXDeclHandler();
	IVBSAXDTDHandler* GetVBSAXDTDHandler();
	IVBSAXErrorHandler* GetVBSAXErrorHandler();
	IVBSAXLexicalHandler* GetVBSAXLexicalHandler();

// Attributes
public:
	static const CLSID s_ClsId;
};



class CMXXMLWriter40Class : public CCoClassWrapper<IMXWriter>
{
// construction/destruction
public:
	CMXXMLWriter40Class();

// operations
public:
	CMXWriter GetMXWriter();
	ISAXContentHandler* GetSAXContentHandler();
	ISAXDeclHandler* GetSAXDeclHandler();
	ISAXDTDHandler* GetSAXDTDHandler();
	ISAXErrorHandler* GetSAXErrorHandler();
	ISAXLexicalHandler* GetSAXLexicalHandler();
	IVBSAXContentHandler* GetVBSAXContentHandler();
	IVBSAXDeclHandler* GetVBSAXDeclHandler();
	IVBSAXDTDHandler* GetVBSAXDTDHandler();
	IVBSAXErrorHandler* GetVBSAXErrorHandler();
	IVBSAXLexicalHandler* GetVBSAXLexicalHandler();

// Attributes
public:
	static const CLSID s_ClsId;
};



class CSAXAttributesClass : public CCoClassWrapper<IMXAttributes>
{
// construction/destruction
public:
	CSAXAttributesClass();

// operations
public:
	CMXAttributes GetMXAttributes();
	CVBSAXAttributes GetVBSAXAttributes();
	ISAXAttributes* GetSAXAttributes();

// Attributes
public:
	static const CLSID s_ClsId;
};



class CSAXAttributes30Class : public CCoClassWrapper<IMXAttributes>
{
// construction/destruction
public:
	CSAXAttributes30Class();

// operations
public:
	CMXAttributes GetMXAttributes();
	CVBSAXAttributes GetVBSAXAttributes();
	ISAXAttributes* GetSAXAttributes();

// Attributes
public:
	static const CLSID s_ClsId;
};



class CSAXAttributes40Class : public CCoClassWrapper<IMXAttributes>
{
// construction/destruction
public:
	CSAXAttributes40Class();

// operations
public:
	CMXAttributes GetMXAttributes();
	CVBSAXAttributes GetVBSAXAttributes();
	ISAXAttributes* GetSAXAttributes();

// Attributes
public:
	static const CLSID s_ClsId;
};



class CVBSAXContentHandlerImpl : public CDispatchInterfaceImplementationWrapper<IVBSAXContentHandler>
{
// construction/destruction
public:
	CVBSAXContentHandlerImpl(AFX_MODULE_STATE *pModuleState, BOOL bNoDelete = FALSE);
	CVBSAXContentHandlerImpl(GET_MODULE_STATE_FUNCTION pGetModuleStateFunction, BOOL bNoDelete = FALSE);
	CVBSAXContentHandlerImpl(AFX_MODULE_STATE *pModuleState, IUnknown *pOwner);
	CVBSAXContentHandlerImpl(GET_MODULE_STATE_FUNCTION pGetModuleStateFunction, IUnknown *pOwner);

// method wrappers
protected:
	/** Receive an object for locating the origin of SAX document events. */
	virtual void SetRefDocumentLocator(CVBSAXLocator rhs) = 0;

	/** Receive notification of the beginning of a document. */
	virtual void StartDocument() = 0;

	/** Receive notification of the end of a document. */
	virtual void EndDocument() = 0;

	/** Begin the scope of a prefix-URI Namespace mapping. */
	virtual void StartPrefixMapping(CString& strPrefix, CString& strURI) = 0;

	/** End the scope of a prefix-URI mapping. */
	virtual void EndPrefixMapping(CString& strPrefix) = 0;

	/** Receive notification of the beginning of an element. */
	virtual void StartElement(CString& strNamespaceURI, CString& strLocalName, CString& strQName, CVBSAXAttributes oAttributes) = 0;

	/** Receive notification of the end of an element. */
	virtual void EndElement(CString& strNamespaceURI, CString& strLocalName, CString& strQName) = 0;

	/** Receive notification of character data. */
	virtual void Characters(CString& strChars) = 0;

	/** Receive notification of ignorable whitespace in element content. */
	virtual void IgnorableWhitespace(CString& strChars) = 0;

	/** Receive notification of a processing instruction. */
	virtual void ProcessingInstruction(CString& strTarget, CString& strData) = 0;

	/** Receive notification of a skipped entity. */
	virtual void SkippedEntity(CString& strName) = 0;

// overridings
public:
	IVBSAXContentHandler* GetInterface();
	IUnknown* GetInterfaceById(REFIID riid);

// interface implementation
protected:
	class XVBSAXContentHandler /*: public IVBSAXContentHandler*/
	{
		STDMETHOD(QueryInterface)(REFIID riid, void **ppv);

		STDMETHOD_(ULONG, AddRef)();

		STDMETHOD_(ULONG, Release)();

		STDMETHOD(GetTypeInfoCount)(UINT *pctinfo);

		STDMETHOD(GetTypeInfo)(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo);

		STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgid);

		STDMETHOD(Invoke)(DISPID id, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr);

		/** Receive an object for locating the origin of SAX document events. */
		STDMETHOD(putref_documentLocator)(IVBSAXLocator* rhs);

		/** Receive notification of the beginning of a document. */
		STDMETHOD(startDocument)();

		/** Receive notification of the end of a document. */
		STDMETHOD(endDocument)();

		/** Begin the scope of a prefix-URI Namespace mapping. */
		STDMETHOD(startPrefixMapping)(BSTR* strPrefix, BSTR* strURI);

		/** End the scope of a prefix-URI mapping. */
		STDMETHOD(endPrefixMapping)(BSTR* strPrefix);

		/** Receive notification of the beginning of an element. */
		STDMETHOD(startElement)(BSTR* strNamespaceURI, BSTR* strLocalName, BSTR* strQName, IVBSAXAttributes* oAttributes);

		/** Receive notification of the end of an element. */
		STDMETHOD(endElement)(BSTR* strNamespaceURI, BSTR* strLocalName, BSTR* strQName);

		/** Receive notification of character data. */
		STDMETHOD(characters)(BSTR* strChars);

		/** Receive notification of ignorable whitespace in element content. */
		STDMETHOD(ignorableWhitespace)(BSTR* strChars);

		/** Receive notification of a processing instruction. */
		STDMETHOD(processingInstruction)(BSTR* strTarget, BSTR* strData);

		/** Receive notification of a skipped entity. */
		STDMETHOD(skippedEntity)(BSTR* strName);

	} m_xVBSAXContentHandler;
	friend XVBSAXContentHandler;

};



class CVBSAXDeclHandlerImpl : public CDispatchInterfaceImplementationWrapper<IVBSAXDeclHandler>
{
// construction/destruction
public:
	CVBSAXDeclHandlerImpl(AFX_MODULE_STATE *pModuleState, BOOL bNoDelete = FALSE);
	CVBSAXDeclHandlerImpl(GET_MODULE_STATE_FUNCTION pGetModuleStateFunction, BOOL bNoDelete = FALSE);
	CVBSAXDeclHandlerImpl(AFX_MODULE_STATE *pModuleState, IUnknown *pOwner);
	CVBSAXDeclHandlerImpl(GET_MODULE_STATE_FUNCTION pGetModuleStateFunction, IUnknown *pOwner);

// method wrappers
protected:
	/** Report an element type declaration. */
	virtual void ElementDecl(CString& strName, CString& strModel) = 0;

	/** Report an attribute type declaration. */
	virtual void AttributeDecl(CString& strElementName, CString& strAttributeName, CString& strType, CString& strValueDefault, CString& strValue) = 0;

	/** Report an internal entity declaration. */
	virtual void InternalEntityDecl(CString& strName, CString& strValue) = 0;

	/** Report a parsed external entity declaration. */
	virtual void ExternalEntityDecl(CString& strName, CString& strPublicId, CString& strSystemId) = 0;

// overridings
public:
	IVBSAXDeclHandler* GetInterface();
	IUnknown* GetInterfaceById(REFIID riid);

// interface implementation
protected:
	class XVBSAXDeclHandler /*: public IVBSAXDeclHandler*/
	{
		STDMETHOD(QueryInterface)(REFIID riid, void **ppv);

		STDMETHOD_(ULONG, AddRef)();

		STDMETHOD_(ULONG, Release)();

		STDMETHOD(GetTypeInfoCount)(UINT *pctinfo);

		STDMETHOD(GetTypeInfo)(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo);

		STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgid);

		STDMETHOD(Invoke)(DISPID id, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr);

		/** Report an element type declaration. */
		STDMETHOD(elementDecl)(BSTR* strName, BSTR* strModel);

		/** Report an attribute type declaration. */
		STDMETHOD(attributeDecl)(BSTR* strElementName, BSTR* strAttributeName, BSTR* strType, BSTR* strValueDefault, BSTR* strValue);

		/** Report an internal entity declaration. */
		STDMETHOD(internalEntityDecl)(BSTR* strName, BSTR* strValue);

		/** Report a parsed external entity declaration. */
		STDMETHOD(externalEntityDecl)(BSTR* strName, BSTR* strPublicId, BSTR* strSystemId);

	} m_xVBSAXDeclHandler;
	friend XVBSAXDeclHandler;

};



class CVBSAXDTDHandlerImpl : public CDispatchInterfaceImplementationWrapper<IVBSAXDTDHandler>
{
// construction/destruction
public:
	CVBSAXDTDHandlerImpl(AFX_MODULE_STATE *pModuleState, BOOL bNoDelete = FALSE);
	CVBSAXDTDHandlerImpl(GET_MODULE_STATE_FUNCTION pGetModuleStateFunction, BOOL bNoDelete = FALSE);
	CVBSAXDTDHandlerImpl(AFX_MODULE_STATE *pModuleState, IUnknown *pOwner);
	CVBSAXDTDHandlerImpl(GET_MODULE_STATE_FUNCTION pGetModuleStateFunction, IUnknown *pOwner);

// method wrappers
protected:
	/** Receive notification of a notation declaration event. */
	virtual void NotationDecl(CString& strName, CString& strPublicId, CString& strSystemId) = 0;

	/** Receive notification of an unparsed entity declaration event. */
	virtual void UnparsedEntityDecl(CString& strName, CString& strPublicId, CString& strSystemId, CString& strNotationName) = 0;

// overridings
public:
	IVBSAXDTDHandler* GetInterface();
	IUnknown* GetInterfaceById(REFIID riid);

// interface implementation
protected:
	class XVBSAXDTDHandler /*: public IVBSAXDTDHandler*/
	{
		STDMETHOD(QueryInterface)(REFIID riid, void **ppv);

		STDMETHOD_(ULONG, AddRef)();

		STDMETHOD_(ULONG, Release)();

		STDMETHOD(GetTypeInfoCount)(UINT *pctinfo);

		STDMETHOD(GetTypeInfo)(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo);

		STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgid);

		STDMETHOD(Invoke)(DISPID id, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr);

		/** Receive notification of a notation declaration event. */
		STDMETHOD(notationDecl)(BSTR* strName, BSTR* strPublicId, BSTR* strSystemId);

		/** Receive notification of an unparsed entity declaration event. */
		STDMETHOD(unparsedEntityDecl)(BSTR* strName, BSTR* strPublicId, BSTR* strSystemId, BSTR* strNotationName);

	} m_xVBSAXDTDHandler;
	friend XVBSAXDTDHandler;

};



class CVBSAXLexicalHandlerImpl : public CDispatchInterfaceImplementationWrapper<IVBSAXLexicalHandler>
{
// construction/destruction
public:
	CVBSAXLexicalHandlerImpl(AFX_MODULE_STATE *pModuleState, BOOL bNoDelete = FALSE);
	CVBSAXLexicalHandlerImpl(GET_MODULE_STATE_FUNCTION pGetModuleStateFunction, BOOL bNoDelete = FALSE);
	CVBSAXLexicalHandlerImpl(AFX_MODULE_STATE *pModuleState, IUnknown *pOwner);
	CVBSAXLexicalHandlerImpl(GET_MODULE_STATE_FUNCTION pGetModuleStateFunction, IUnknown *pOwner);

// method wrappers
protected:
	/** Report the start of DTD declarations, if any. */
	virtual void StartDTD(CString& strName, CString& strPublicId, CString& strSystemId) = 0;

	/** Report the end of DTD declarations. */
	virtual void EndDTD() = 0;

	/** Report the beginning of some internal and external XML entities. */
	virtual void StartEntity(CString& strName) = 0;

	/** Report the end of an entity. */
	virtual void EndEntity(CString& strName) = 0;

	/** Report the start of a CDATA section. */
	virtual void StartCDATA() = 0;

	/** Report the end of a CDATA section. */
	virtual void EndCDATA() = 0;

	/** Report an XML comment anywhere in the document. */
	virtual void Comment(CString& strChars) = 0;

// overridings
public:
	IVBSAXLexicalHandler* GetInterface();
	IUnknown* GetInterfaceById(REFIID riid);

// interface implementation
protected:
	class XVBSAXLexicalHandler /*: public IVBSAXLexicalHandler*/
	{
		STDMETHOD(QueryInterface)(REFIID riid, void **ppv);

		STDMETHOD_(ULONG, AddRef)();

		STDMETHOD_(ULONG, Release)();

		STDMETHOD(GetTypeInfoCount)(UINT *pctinfo);

		STDMETHOD(GetTypeInfo)(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo);

		STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgid);

		STDMETHOD(Invoke)(DISPID id, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr);

		/** Report the start of DTD declarations, if any. */
		STDMETHOD(startDTD)(BSTR* strName, BSTR* strPublicId, BSTR* strSystemId);

		/** Report the end of DTD declarations. */
		STDMETHOD(endDTD)();

		/** Report the beginning of some internal and external XML entities. */
		STDMETHOD(startEntity)(BSTR* strName);

		/** Report the end of an entity. */
		STDMETHOD(endEntity)(BSTR* strName);

		/** Report the start of a CDATA section. */
		STDMETHOD(startCDATA)();

		/** Report the end of a CDATA section. */
		STDMETHOD(endCDATA)();

		/** Report an XML comment anywhere in the document. */
		STDMETHOD(comment)(BSTR* strChars);

	} m_xVBSAXLexicalHandler;
	friend XVBSAXLexicalHandler;

};



class CVBSAXEntityResolverImpl : public CDispatchInterfaceImplementationWrapper<IVBSAXEntityResolver>
{
// construction/destruction
public:
	CVBSAXEntityResolverImpl(AFX_MODULE_STATE *pModuleState, BOOL bNoDelete = FALSE);
	CVBSAXEntityResolverImpl(GET_MODULE_STATE_FUNCTION pGetModuleStateFunction, BOOL bNoDelete = FALSE);
	CVBSAXEntityResolverImpl(AFX_MODULE_STATE *pModuleState, IUnknown *pOwner);
	CVBSAXEntityResolverImpl(GET_MODULE_STATE_FUNCTION pGetModuleStateFunction, IUnknown *pOwner);

// method wrappers
protected:
	/** Allow the application to resolve external entities. */
	virtual _variant_t ResolveEntity(CString& strPublicId, CString& strSystemId) = 0;

// overridings
public:
	IVBSAXEntityResolver* GetInterface();
	IUnknown* GetInterfaceById(REFIID riid);

// interface implementation
protected:
	class XVBSAXEntityResolver /*: public IVBSAXEntityResolver*/
	{
		STDMETHOD(QueryInterface)(REFIID riid, void **ppv);

		STDMETHOD_(ULONG, AddRef)();

		STDMETHOD_(ULONG, Release)();

		STDMETHOD(GetTypeInfoCount)(UINT *pctinfo);

		STDMETHOD(GetTypeInfo)(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo);

		STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgid);

		STDMETHOD(Invoke)(DISPID id, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr);

		/** Allow the application to resolve external entities. */
		STDMETHOD(resolveEntity)(BSTR* strPublicId, BSTR* strSystemId, VARIANT* Result);

	} m_xVBSAXEntityResolver;
	friend XVBSAXEntityResolver;

};



class CVBSAXErrorHandlerImpl : public CDispatchInterfaceImplementationWrapper<IVBSAXErrorHandler>
{
// construction/destruction
public:
	CVBSAXErrorHandlerImpl(AFX_MODULE_STATE *pModuleState, BOOL bNoDelete = FALSE);
	CVBSAXErrorHandlerImpl(GET_MODULE_STATE_FUNCTION pGetModuleStateFunction, BOOL bNoDelete = FALSE);
	CVBSAXErrorHandlerImpl(AFX_MODULE_STATE *pModuleState, IUnknown *pOwner);
	CVBSAXErrorHandlerImpl(GET_MODULE_STATE_FUNCTION pGetModuleStateFunction, IUnknown *pOwner);

// method wrappers
protected:
	/** Receive notification of a recoverable error. */
	virtual void Error(CVBSAXLocator oLocator, CString& strErrorMessage, long nErrorCode) = 0;

	/** Receive notification of a non-recoverable error. */
	virtual void FatalError(CVBSAXLocator oLocator, CString& strErrorMessage, long nErrorCode) = 0;

	/** Receive notification of an ignorable warning. */
	virtual void IgnorableWarning(CVBSAXLocator oLocator, CString& strErrorMessage, long nErrorCode) = 0;

// overridings
public:
	IVBSAXErrorHandler* GetInterface();
	IUnknown* GetInterfaceById(REFIID riid);

// interface implementation
protected:
	class XVBSAXErrorHandler /*: public IVBSAXErrorHandler*/
	{
		STDMETHOD(QueryInterface)(REFIID riid, void **ppv);

		STDMETHOD_(ULONG, AddRef)();

		STDMETHOD_(ULONG, Release)();

		STDMETHOD(GetTypeInfoCount)(UINT *pctinfo);

		STDMETHOD(GetTypeInfo)(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo);

		STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgid);

		STDMETHOD(Invoke)(DISPID id, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr);

		/** Receive notification of a recoverable error. */
		STDMETHOD(error)(IVBSAXLocator* oLocator, BSTR* strErrorMessage, long nErrorCode);

		/** Receive notification of a non-recoverable error. */
		STDMETHOD(fatalError)(IVBSAXLocator* oLocator, BSTR* strErrorMessage, long nErrorCode);

		/** Receive notification of an ignorable warning. */
		STDMETHOD(ignorableWarning)(IVBSAXLocator* oLocator, BSTR* strErrorMessage, long nErrorCode);

	} m_xVBSAXErrorHandler;
	friend XVBSAXErrorHandler;

};



class CVBSAXXMLFilterImpl : public CDispatchInterfaceImplementationWrapper<IVBSAXXMLFilter>
{
// construction/destruction
public:
	CVBSAXXMLFilterImpl(AFX_MODULE_STATE *pModuleState, BOOL bNoDelete = FALSE);
	CVBSAXXMLFilterImpl(GET_MODULE_STATE_FUNCTION pGetModuleStateFunction, BOOL bNoDelete = FALSE);
	CVBSAXXMLFilterImpl(AFX_MODULE_STATE *pModuleState, IUnknown *pOwner);
	CVBSAXXMLFilterImpl(GET_MODULE_STATE_FUNCTION pGetModuleStateFunction, IUnknown *pOwner);

// method wrappers
protected:
	/** Set or get the parent reader */
	virtual IVBSAXXMLReader* GetParent() = 0;

	/** Set or get the parent reader */
	virtual void SetRefParent(CVBSAXXMLReader rhs) = 0;

// overridings
public:
	IVBSAXXMLFilter* GetInterface();
	IUnknown* GetInterfaceById(REFIID riid);

// interface implementation
protected:
	class XVBSAXXMLFilter /*: public IVBSAXXMLFilter*/
	{
		STDMETHOD(QueryInterface)(REFIID riid, void **ppv);

		STDMETHOD_(ULONG, AddRef)();

		STDMETHOD_(ULONG, Release)();

		STDMETHOD(GetTypeInfoCount)(UINT *pctinfo);

		STDMETHOD(GetTypeInfo)(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo);

		STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgid);

		STDMETHOD(Invoke)(DISPID id, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr);

		/** Set or get the parent reader */
		STDMETHOD(get_parent)(IVBSAXXMLReader** Result);

		/** Set or get the parent reader */
		STDMETHOD(putref_parent)(IVBSAXXMLReader* rhs);

	} m_xVBSAXXMLFilter;
	friend XVBSAXXMLFilter;

};




} // namespace MsXml

#endif
