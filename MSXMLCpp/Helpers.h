/********************************************************************
*
* © Photon Laser Engineering GmbH
*
* Diese Datei ist urheberrechtlich geschützt. Ohne ausdrückliche
* schriftliche Erlaubnis der Photon Laser Engineering GmbH darf kein
* Teil dieser Datei für irgendwelche Zwecke vervielfältigt oder
* übertragen werden unabhängig davon, auf welche Art und Weise oder
* mit welchen Mitteln elektronisch oder mechanisch, dies geschieht.
*
*********************************************************************
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

#if !defined(AFX_HELPERS_H__71897570_EE6D_4704_B71E_36FA5797BC9A__INCLUDED_)
#define AFX_HELPERS_H__71897570_EE6D_4704_B71E_36FA5797BC9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace MsXml
{

	/**
	Stores the given DOM-document to the file with the specified path
	using pretty printing (new-lines and indentation).

	If the specified file does already exist, it will be overwritten.

	@remarks Does only support files up to 4 GByte in size.

	@param pDoc
		DOM-document that should be safed.
	@param lpszPath
		Path of the file to save to (full path or relative to working dir).
	@param lpszEncoding
		Encoding to use for file writing. "ISO-8859-1" is the default
		encoding.

	@return
		S_OK on success or an error code describing the error.
	*/
	HRESULT DOMSavePretty(IXMLDOMDocument *pDoc, LPCTSTR lpszPath, LPCTSTR lpszEncoding = _T("ISO-8859-1"));

} //namespace MsXml

#endif // !defined(AFX_HELPERS_H__71897570_EE6D_4704_B71E_36FA5797BC9A__INCLUDED_)
