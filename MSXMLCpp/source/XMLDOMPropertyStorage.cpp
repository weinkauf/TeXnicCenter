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
#include "XMLDOMPropertyStorage.h"


namespace MsXml
{

	
//-------------------------------------------------------------------
// class CXMLDOMPropertyStorage
//-------------------------------------------------------------------

CXMLDOMPropertyStorage::CXMLDOMPropertyStorage()
:	CXMLDOMElement()
{}


CXMLDOMPropertyStorage::CXMLDOMPropertyStorage(IXMLDOMElement *p, BOOL bIncrementRefCount /*= FALSE*/)
:	CXMLDOMElement(p, bIncrementRefCount)
{}


CXMLDOMPropertyStorage::CXMLDOMPropertyStorage(const CXMLDOMElement &o)
:	CXMLDOMElement(o)
{}


CXMLDOMPropertyStorage::CXMLDOMPropertyStorage(IUnknown *pUnknown, BOOL bIncrementRefCount /*= FALSE*/, BOOL bThrowException /*= FALSE*/)
:	CXMLDOMElement(pUnknown, bIncrementRefCount, bThrowException)
{}


CXMLDOMPropertyStorage::CXMLDOMPropertyStorage(IXMLDOMNode *p, BOOL bIncrementRefCount /*= FALSE*/)
:	CXMLDOMElement(p, bIncrementRefCount)
{}


CXMLDOMPropertyStorage::CXMLDOMPropertyStorage(CXMLDOMNode &o)
:	CXMLDOMElement((IXMLDOMNode*)o, TRUE)
{}


/////////////////////////////////////////////////////////////////////
// operations

CXMLDOMElement CXMLDOMPropertyStorage::SetProperty(LPCTSTR lpszName, const _variant_t &Property)
{
	CString	strType = MapType(Property.vt);
	if (strType.IsEmpty())
		AfxThrowComException(E_FAIL);

	return SetProperty(lpszName, strType, Property);
}


CXMLDOMElement CXMLDOMPropertyStorage::SetProperty(LPCTSTR lpszName, LPCTSTR lpszDataType, const _variant_t &Property)
{
	// get the wanted element or create one if necessary
	CXMLDOMElement	Element;
	BOOL						bNewlyCreated = FALSE;

	try
	{
		Element.Attach((IXMLDOMElement*)SelectSingleNode(lpszName).QueryInterface(IID_IXMLDOMElement));
	}
	catch (CComException *pE)
	{
		pE->Delete();

		// element does not exist -- lets create one.
		Element = GetOwnerDocument().CreateElement(lpszName);
		bNewlyCreated = TRUE;
	}

	ASSERT(Element);
	if (!Element)
		AfxThrowComException(E_FAIL);

	// set element data
	Element.SetDataType(lpszDataType);
	Element.SetNodeTypedValue(Property);

	// append element to node, if newly created
	if (bNewlyCreated)
		AppendChild(Element);

	return Element;
}


CXMLDOMElement CXMLDOMPropertyStorage::SetPropertyAttribute(LPCTSTR lpszName, LPCTSTR lpszAttributeName, const _variant_t &Property)
{
	// get the wanted element or create one if necessary
	CXMLDOMElement	Element;
	BOOL						bNewlyCreated = FALSE;

	try
	{
		Element.Attach((IXMLDOMElement*)SelectSingleNode(lpszName).QueryInterface(IID_IXMLDOMElement));
	}
	catch (CComException *pE)
	{
		pE->Delete();

		// element does not exist -- lets create one.
		Element = GetOwnerDocument().CreateElement(lpszName);
		bNewlyCreated = TRUE;
	}

	ASSERT(Element);
	if (!Element)
		AfxThrowComException(E_FAIL);

	// set data
	Element.SetAttribute(lpszAttributeName, Property);

	// append element to node, if newly created
	if (bNewlyCreated)
		AppendChild(Element);

	return Element;
}


CXMLDOMElement CXMLDOMPropertyStorage::SetPropertyArray(LPCTSTR lpszArrayName, LPCTSTR lpszName, const CVariantArray &aValues)
{
	// get the wanted element or create one if necessary
	CXMLDOMElement	Element;
	BOOL						bNewlyCreated = FALSE;

	try
	{
		Element.Attach((IXMLDOMElement*)SelectSingleNode(lpszArrayName).QueryInterface(IID_IXMLDOMElement));

		// remove any already existing childs of the element
		CXMLDOMNodeList	Childs(Element.GetChildNodes());
		for (int i = Childs.GetLength()-1; i >= 0; --i)
			Element.RemoveChild(Childs.GetItem(i));
	}
	catch (CComException *pE)
	{
		pE->Delete();

		// element does not exist -- lets create one.
		Element = GetOwnerDocument().CreateElement(lpszArrayName);
		bNewlyCreated = TRUE;
	}

	ASSERT(Element);
	if (!Element)
		AfxThrowComException(E_FAIL);

	// create a child for each element in the array
	for (int i = 0; i < aValues.GetSize(); i++)
	{
		_variant_t			Value(aValues[i]);
		CXMLDOMElement	ChildElement(GetOwnerDocument().CreateElement(lpszName));
		ChildElement.SetDataType(MapType(Value.vt));
		ChildElement.SetNodeTypedValue(Value);
		Element.AppendChild(ChildElement);
	}

	// append element to node, if newly created
	if (bNewlyCreated)
		AppendChild(Element);

	return Element;
}


_variant_t CXMLDOMPropertyStorage::GetProperty(LPCTSTR lpszName, const _variant_t &Default /* = _variant_t */, BOOL bEnsureRightType /*= TRUE */)
{
	try
	{
		CXMLDOMElement	Element(SelectSingleNode(lpszName).QueryInterface(IID_IXMLDOMElement));

		// if necessary, check if type of element and provided default 
		// value match.
		if (Default.vt != VT_EMPTY && bEnsureRightType)
		{
			_variant_t	Type = Element.GetDataType();
			if (Type.vt == VT_EMPTY || (LPCTSTR)(_bstr_t)Type != MapType(Default.vt))
				AfxThrowComException(S_FALSE);
		}

		return Element.GetNodeTypedValue();
	}
	catch (CComException *pE)
	{
		if (Default.vt == VT_EMPTY)
			throw pE;

		pE->Delete();
		return Default;
	}
}


_variant_t CXMLDOMPropertyStorage::GetProperty(LPCTSTR lpszName, LPCTSTR lpszType, const _variant_t &Default /* = _variant_t */)
{
	try
	{
		CXMLDOMElement	Element(SelectSingleNode(lpszName).QueryInterface(IID_IXMLDOMElement));

		// check type match
		// value match.
		_variant_t	Type = Element.GetDataType();
		if (Type.vt == VT_EMPTY || (LPCTSTR)(_bstr_t)Type != CString(lpszType))
			AfxThrowComException(S_FALSE);

		return Element.GetNodeTypedValue();
	}
	catch (CComException *pE)
	{
		if (Default.vt == VT_EMPTY)
			throw pE; // forward exception

		pE->Delete();
		return Default;
	}
}


_variant_t CXMLDOMPropertyStorage::GetPropertyAttribute(LPCTSTR lpszName, LPCTSTR lpszAttributeName, const _variant_t &Default /* = _variant_t */)
{
	try
	{
		CXMLDOMElement	Element(SelectSingleNode(lpszName).QueryInterface(IID_IXMLDOMElement));
		return Element.GetAttribute(lpszAttributeName);
	}
	catch (CComException *pE)
	{
		if (Default.vt == VT_EMPTY)
			throw pE;	// forward exception

		pE->Delete();
		return Default;
	}
}


INT_PTR CXMLDOMPropertyStorage::GetPropertyArray(LPCTSTR lpszArrayName, LPCTSTR lpszName, CVariantArray &aValues)
{
	aValues.RemoveAll();

	try
	{
		CXMLDOMElement	Element(SelectSingleNode(lpszArrayName).QueryInterface(IID_IXMLDOMElement));
		CXMLDOMNodeList	Childs(Element.SelectNodes(lpszName));
		long						lChildCount = Childs.GetLength();

		for (long i = 0; i < lChildCount; i++)
		{
			CXMLDOMElement	Child(Childs.GetItem(i).QueryInterface(IID_IXMLDOMElement));
			aValues.Add(Child.GetNodeTypedValue());
		}

		return aValues.GetSize();
	}
	catch (CComException *pE)
	{
		pE->Delete();
		return aValues.GetSize();
	}
}


/////////////////////////////////////////////////////////////////////
// static helpers

CString CXMLDOMPropertyStorage::MapType(VARTYPE vt)
{
	if (vt&VT_ARRAY)
		return _T("bin.hex");

	switch (vt)
	{
	case VT_BSTR:
		return _T("string");
	case VT_CY:
		return _T("Fixed.14.4");
	case VT_BOOL:
		return _T("bool");
	case VT_DATE:
		return _T("dateTime");
	case VT_I1:
		return _T("i1");
	case VT_I2:
		return _T("i2");
	case VT_I4:
		return _T("i4");
	case VT_UI1:
		return _T("ui1");
	case VT_UI2:
		return _T("ui2");
	case VT_UI4:
		return _T("ui4");
	case VT_R4:
		return _T("r4");
	case VT_R8:
		return _T("r8");
	default:
		return _T("");
	};
}

} // namespace MsXml
