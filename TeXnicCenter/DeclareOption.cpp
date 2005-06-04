// DeclareOption.cpp: implementation of the CDeclareOption class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "texniccenter.h"
#include "DeclareOption.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDeclareOption::CDeclareOption(const CStyleFile *parent, CString &name)
:CLaTeXCommand(parent, name, 0)
{

}

CDeclareOption::~CDeclareOption()
{

}
