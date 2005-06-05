// DeclareOption.h: interface for the CDeclareOption class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DECLAREOPTION_H__251FEC0F_A498_4D2A_817B_5FF0FD3EC37E__INCLUDED_)
#define AFX_DECLAREOPTION_H__251FEC0F_A498_4D2A_817B_5FF0FD3EC37E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LaTeXCommand.h"

class CDeclareOption : public CAbstractLaTeXCommand  
{
public:
	CDeclareOption(const CStyleFile *parent, CString &name);
	virtual ~CDeclareOption();

};

#endif // !defined(AFX_DECLAREOPTION_H__251FEC0F_A498_4D2A_817B_5FF0FD3EC37E__INCLUDED_)
