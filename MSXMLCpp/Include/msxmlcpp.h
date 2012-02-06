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

#include <msxml6.h>
#ifdef UNDER_CE
	#include "../Source/CeDomXml.h"
	#include "../Source/CeSaxXml.h"
#else
	#include "../Source/DOMXML.h"
	#include "../Source/SAXXML.h"
#endif
#include "../Source/XMLDOMPropertyStorage.h"
#include "../Source/XMLDOMDocumentEx.h"
#include "../Source/Helpers.h"
