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

#if !defined(AFX_XMLDOMPROPERTYSTORAGE_H__C3325D5D_7FEC_477C_8E8F_771823FF3B1F__INCLUDED_)
#define AFX_XMLDOMPROPERTYSTORAGE_H__C3325D5D_7FEC_477C_8E8F_771823FF3B1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef UNDER_CE
	#include "CeDomXml.h"
#else
	#include "DOMXML.h"
#endif
#include <afxtempl.h>

namespace MsXml
{

typedef CArray<_variant_t, _variant_t&> CVariantArray;


class AFX_EXT_CLASS CXMLDOMPropertyStorage : public CXMLDOMElement  
{
// construction/destruction
public:
	CXMLDOMPropertyStorage();
	CXMLDOMPropertyStorage(IXMLDOMElement *p, BOOL bIncrementRefCount = FALSE);
	CXMLDOMPropertyStorage(const CXMLDOMElement &o);
	CXMLDOMPropertyStorage(IUnknown *pUnknown, BOOL bIncrementRefCount = FALSE, BOOL bThrowException = FALSE);
	CXMLDOMPropertyStorage(IXMLDOMNode *p, BOOL bIncrementRefCount = FALSE);
	CXMLDOMPropertyStorage(CXMLDOMNode &o);

// operations
public:
	/**
	Stores a property in this node.

	Checks if a XMLDOM-element with the specified name exists as a
	direct child of this node. If not, one is created and appended as
	a child to this node. Sets the specified data type using	
	SetDataType(), sets the specified value using SetNodeTypedValue().

	If one of the operations fails, a CComException is thrown.

	@param lpszName
		The name of the element node.
	@param Property
		Value to store in the newly created element. See 
		IDOMXMLElement::SetNodeTypedValue() for details.

	@return
		XMLDOM-element used to store the data in.

	@throws CComException
	*/
	CXMLDOMElement SetProperty(LPCTSTR lpszName, const _variant_t &Property);

	/**
	Stores a property in this node.

	Checks if a XMLDOM-element with the specified name exists as a
	direct child of this node. If not, one is created and appended as
	a child to this node. Sets the specified data type using	
	SetDataType(), sets the specified value using SetNodeTypedValue().

	If one of the operations fails, a CComException is thrown.

	@param lpszName
		The name of the element node.
	@param lpszDataType
		Type of the property to store. See IDOMXMLElement::SetDataType() 
		for details.
	@param Property
		Value to store in the newly created element. See 
		IDOMXMLElement::SetNodeTypedValue() for details.

	@return
		XMLDOM-element used to store the data in.

	@throws CComException
	*/
	CXMLDOMElement SetProperty(LPCTSTR lpszName, LPCTSTR lpszDataType, const _variant_t &Property);

	/**
	Sets an attribute of a property in this node.

	Checks, if a XMLDOM-element with the specified name exists as a
	direct child of this node. If not, the method creates one. Sets
	the specified attribute of the specified element to the specified
	value using SetAttribute().

	If one of the operation fails, a CComException is thrown.

	@param lpszName
		The name of the element node.
	@param lpszAttributeName
		The name of the attribute to set.
	@param Property
		Value to store in the specified attribute.

	@return
		XMLDOM-element used to store the attribute in.		

	@throws CComException
	*/
	CXMLDOMElement SetPropertyAttribute(LPCTSTR lpszName, LPCTSTR lpszAttributeName, const _variant_t &Property);

	/**
	Adds a list of values to the storage.

	Creates the Element lpszArrayName as a child of this element, if it
	does not already exist. For each element in the given Array, a 
	element	of the name lpszName will be create as a child of the 
	lpszArrayName-element and the value will be set to the 
	corresponding array value.

	If the lpszArrayName-Element does alreay exist, all childs will
	be removed and newly added.

	@return
		The element that contains the array elements.
	*/
	CXMLDOMElement SetPropertyArray(LPCTSTR lpszArrayName, LPCTSTR lpszName, const CVariantArray &aValues);

	/**
	Returns a property stored in this node.

	Searches for the element with the specified name (has to be a 
	direct child of this node) and returns its typed data.

	@param lpszName
		Name of the element that contains the data to read.
	@param Default
		Default value to return, if the specified element does not exist.
		If an empty value is provided (VT_EMPTY) as default value, an
		exception will be thrown, if the specified element does not 
		exist.
	@param bEnsureType
		If TRUE, the element's value will only be returned, if it is of
		the same type as the value provided by the Default parameter.
		If the types do not match, the Default parameter will be returned
		instead.

	@throws CComException
	*/
	_variant_t GetProperty(LPCTSTR lpszName, const _variant_t &Default = _variant_t(), BOOL bEnsureRightType = TRUE);

	/**
	Returns a property stored in this node.

	Searches for the element with the specified name (has to be a 
	direct child of this node) and returns its typed data.

	@param lpszName
		Name of the element that contains the data to read.
	@param lpszType
		Assumed type of the data in the specified element
	@param Default
		Default value to return, if the specified element does not exist
		or if the data in the element is not of the specified type.
		If an empty value is provided (VT_EMPTY) as default value, an
		exception will be thrown, if the specified element does not 
		exist or is not of the specified type.

	@throws CComException
	*/
	_variant_t GetProperty(LPCTSTR lpszName, LPCTSTR lpszType, const _variant_t &Default = _variant_t());

	/**
	Returns an attribute of a property in this node.

	@param lpszName
		Name of the element node.
	@param lpszAttributeName
		The name of the attribute to retrieve.
	@param Default
		Default value to return, if the specified element or attribute
		does not exist.	If an empty value is provided (VT_EMPTY) as 
		default value, an	exception will be thrown, if the specified 
		element or attribute does not exist.
	*/
	_variant_t GetPropertyAttribute(LPCTSTR lpszName, LPCTSTR lpszAttributeName, const _variant_t &Default = _variant_t());

	/**
	Retrieves a list of values from the storage.

	Fills the array aValues with the values of all lpszName elements,
	which are childs of the element lpszArrayName.

	Array will be emptied before filled by the method.

	@return
		The number or elements in the array.
	*/
	int GetPropertyArray(LPCTSTR lpszArrayName, LPCTSTR lpszName, CVariantArray &aValues);

// static helpers
public:
	/**
	Returns the matching XML element data type for the given variant
	type or an empty string, if there does not exist a matching XML
	element data type.
	*/
	static CString MapType(VARTYPE vt);
};


} // namespace MsXml

#endif // !defined(AFX_XMLDOMPROPERTYSTORAGE_H__C3325D5D_7FEC_477C_8E8F_771823FF3B1F__INCLUDED_)
