/********************************************************************
*
* This file is part of the MFCExt-Library
*
* Copyright (C) 1999-2000 Sven Wiegand
* Copyright (C) 2000-2001 ToolsCenter
* 
* This library is free software; you can redistribute it and/or
* modify, but leave the headers intact and do not remove any 
* copyrights from the source.
*
* This library does not only contain file from us, but also from
* third party developers. Look at the source file headers to get
* detailed information.
*
* If you have further questions visit our homepage
*
*    http://www.ToolsCenter.org
*
********************************************************************/

#ifndef __DEBUG_H__
#define __DEBUG_H__


/**
Dieses Makro vereinfacht die Kontrolle von Bedingungen, die in der
Debug-Version eine ASSERT-Meldung bringen sollen und gleichzeitig
(auch in der Release-Version) bei nicht erfuellter Bedingung eine
Aktion ausloesen.

Somit fuehrt der folgende Code:
<pre>
@@ IFNOT( nIndex >= 0 && nIndex < m_nSize )
@@	return FALSE;
</pre>
In der Debug-Version eines Programms dazu, dass zunaechst eine
Meldung fuer eine nicht erfuellte Behauptung angezeigt wird 
(ASSERT( FALSE )) und danach die Anweisung <pre>return FALSE</pre>
ausgefuehrt wird.

In der Release-Version wird nur die Anweisung <pre>return FALSE</pre>
ausgefuehrt.

@param expr
	Bedingung, die ueberprueft werden soll.
*/
#define IFNOT( expr )\
	ASSERT( expr );\
	if( !(expr) )




#endif //__DEBUG_H__