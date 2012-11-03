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

#if !defined(AFX_DEBUGTOOLS_H__078DB7B5_531E_40F9_8D09_82EC8BB19C6C__INCLUDED_)
#define AFX_DEBUGTOOLS_H__078DB7B5_531E_40F9_8D09_82EC8BB19C6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MsXmlCpp.h"

#ifdef _DEBUG
	void AFX_EXPORT AfxDumpXmlNode(IXMLDOMNode *pNode, DWORD dwCategory = traceAppMsg, UINT unLevel = 0);
#endif

#ifdef _DEBUG
	#define TRACE_XML(pNode) AfxDumpXmlNode(pNode)
	#define ATLTRACE_XML(category, level, pNode) AfxDumpXmlNode(pNode, category, level)
#else
	#define AfxDumpXmlNode(pNode, dwCategory, unLevel)
	#define TRACE_XML(pNode)
	#define ATLTRACE_XML(category, level, pNode)
#endif

#endif // !defined(AFX_DEBUGTOOLS_H__078DB7B5_531E_40F9_8D09_82EC8BB19C6C__INCLUDED_)
