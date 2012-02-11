/********************************************************************
*
* � Photon Laser Engineering GmbH
*
* Diese Datei ist urheberrechtlich gesch�tzt. Ohne ausdr�ckliche 
* schriftliche Erlaubnis der Photon Laser Engineering GmbH darf kein 
* Teil dieser Datei f�r irgendwelche Zwecke vervielf�ltigt oder 
* �bertragen werden unabh�ngig davon, auf welche Art und Weise oder 
* mit welchen Mitteln elektronisch oder mechanisch, dies geschieht.
*
*********************************************************************
*
* $Workfile:$
* $Revision$
* $Modtime:$
* $Author$
*
* �nderungen:
*	$History:$
*
*********************************************************************/

#include "stdafx.h"
#include "DebugTools.h"

//-------------------------------------------------------------------
// globals
//-------------------------------------------------------------------

#ifdef _DEBUG

	void AfxDumpXmlNode(IXMLDOMNode *pNode, DWORD dwCategory, UINT unLevel)
	{
		if (pNode==NULL)
			ATLTRACE2(dwCategory, unLevel, _T("NULL\n"));
		else
		{
			try
			{
				MsXml::CXMLDOMNode	Node(pNode, TRUE);
				ATLTRACE2(dwCategory, unLevel, _T("%s\n"), Node.GetXml());
			}
			catch (CComException *pE)
			{
				ATLTRACE2(dwCategory, unLevel, _T("AfxDumpXmlNode caused COM-error %08x\n"), pE->GetResult());
				pE->Delete();
			}
			catch (CException *pE)
			{
				ATLTRACE2(dwCategory, unLevel, _T("AfxDumpXmlNode caused MFC-exception\n"));
				pE->Delete();
			}
			catch (...)
			{
				ATLTRACE2(dwCategory, unLevel, _T("AfxDumpXmlNode caused general exception\n"));
			}
		}
	}

#endif