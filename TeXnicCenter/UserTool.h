#ifndef __CUSERTOOL_H__
#define __CUSERTOOL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BCGUserTool.h"


class CUserTool : public CBCGUserTool
{
	DECLARE_SERIAL(CUserTool)

// Construction/Destruction
public:
	CUserTool();
	virtual ~CUserTool();

// Operations
public:

// Overrides
public:
	virtual BOOL Invoke();

// Implementation helpers
public:

// Attributes
public:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //__CUSERTOOL_H__
