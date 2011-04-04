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

#if !defined(AFX_XMLDOMDOCUMENTEX_H__1E43C25C_B421_4E91_ACD5_57FB135F9DDA__INCLUDED_)
#define AFX_XMLDOMDOCUMENTEX_H__1E43C25C_B421_4E91_ACD5_57FB135F9DDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DOMXML.h"

namespace MsXml
{

/**
Adds some methods, to make the work with CXMLDOMDocument2 more easier.

@author Sven Wiegand
*/
class AFX_EXT_CLASS CXMLDOMDocumentEx : public CXMLDOMDocument2  
{
// construction/destruction
public:
	CXMLDOMDocumentEx();
	CXMLDOMDocumentEx(IXMLDOMDocument2 *p, BOOL bIncrementRefCount = FALSE);
	CXMLDOMDocumentEx(const CXMLDOMDocument2 &o);
	CXMLDOMDocumentEx(IUnknown *pUnknown, BOOL bIncrementRefCount = FALSE, BOOL bThrowException = FALSE);

// operations
public:
	/**
	Creates a IXMLDOMDocument2-COM-object and attaches it to this 
	object.

	If you no longer need the object you can call Destroy(), otherwise
	the used resources will automatically be freed when the destructor
	is executed.

	For the created document the async-property will be set to FALSE 
	and XPath will be the selection language.

	@exception CComException
		Will be thrown, if the creation fails.

	@param lpszDocumentNamespaces
		A space delimited set of Namespace names. When namespaces are
		given, you can use qualified names for methods like SelectNode()
		etc.
	@param lpszSchema
		If a XML-schema should be attached to the document, then specify
		the path to the schema file here.
	@param lpszSchemaNamespace
		The namespace to associate with the XML-schema specified by
		lpszSchema.
	*/
	void Create(LPCTSTR lpszDocumentNamespaces = NULL, LPCTSTR lpszSchemaPath = NULL, LPCTSTR lpszSchemaNamespace = _T(""));

	/**
	Frees the attached object (if any).

	If this is method is not called explicitly, the used resources will
	be freed by the destructor.
	*/
	void Destroy();

	/**
	Attaches an additional XML-schema to the document.

	@exception CComException
		Will be thrown, if the operation fails.

	@param lpszSchemaPath
		Path of the XML-schema file.
	@param lpszSchemaNamespace
		The namespace to associate with the XML-schema.
	*/
	void AddSchema(LPCTSTR lpszSchema, LPCTSTR lpszSchemaNamespace);

	/**
	Removes the schema with the specified namespace from the collection
	of schemas attached to the document.

	@exception CComException
		Will be thrown, if the operation fails.
	*/
	void RemoveSchema(LPCTSTR lpszSchemaNamespace);

	/**
	Saves the XML document formatted into a file.

	The normal Save()-method does not apply formatting and writes the
	whole document in a single line, which isn't nice if you would like
	to take a view on it in a text editor. This method creates multiple
	lines and indents each line according to its position in the XML
	hierarchy.

	@exception CComException
		Will be thrown, if an error occurs.

	@param lpszPath
		Path of the file to save to (full path or relative to working 
		dir). If the specified file already exists, it will be 
		overwritten.
	@param lpszEncoding
		Encoding to use for file writing. "ISO-8859-1" is the default
		encoding.
	*/
	void SavePretty(LPCTSTR lpszPath, LPCTSTR lpszEncoding = _T("ISO-8859-1"));
};

}//namespace MsXml

#endif // !defined(AFX_XMLDOMDOCUMENTEX_H__1E43C25C_B421_4E91_ACD5_57FB135F9DDA__INCLUDED_)
