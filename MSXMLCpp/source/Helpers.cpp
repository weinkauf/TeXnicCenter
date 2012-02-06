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

#include "stdafx.h"
#include "Helpers.h"
#include "Stream.h"
#ifdef UNDER_CE
	#include "CeSaxXml.h"
#else
	#include "SAXXML.h"
#endif

namespace MsXml
{

//-------------------------------------------------------------------
// global helpers
//-------------------------------------------------------------------

HRESULT DOMSavePretty(IXMLDOMDocument *pDoc, LPCTSTR lpszPath, LPCTSTR lpszEncoding /* = _T("ISO-88591") */)
{
	HRESULT	hr = S_OK;

	// Create file
	CFile				File;
	if (!File.Open(lpszPath, CFile::modeCreate|CFile::modeWrite))
		return E_FAIL;

	// create stream for file
	CFileStream	Stream(&File, FALSE);

	// create reader for DOM
	CVBSAXXMLReader	Reader;
	#ifdef UNDER_CE
		hr = Reader.CoCreateInstance(CSAXXMLReaderClass::s_ClsId);
	#else
		hr = Reader.CoCreateInstance(CSAXXMLReader60Class::s_ClsId);
	#endif
	if (FAILED(hr))
		return hr;
		
	// create writer
	#ifdef UNDER_CE
		CMXXMLWriterClass	Writer;
	#else
		CMXXMLWriter60Class	Writer;
	#endif
	Writer.Create(CLSCTX_INPROC);
	Writer.GetMXWriter().SetIndent(VARIANT_TRUE);
	Writer.GetMXWriter().SetEncoding(lpszEncoding);
	Writer.GetMXWriter().SetOutput(&Stream);

	Reader.SetRefContentHandler(Writer.GetVBSAXContentHandler());
	Reader.SetRefErrorHandler(Writer.GetVBSAXErrorHandler());

	hr = S_OK;
	try
	{
		Reader.Parse(pDoc);
	}
	catch (CComException *pE)
	{
		hr = pE->GetResult();
		pE->Delete();
	}

	Writer.GetMXWriter().Flush();
	File.Close();

	return hr;
}

} //namespace MsXml