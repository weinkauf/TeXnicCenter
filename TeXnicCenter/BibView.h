// BibView.h: interface for the CBibView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BIBVIEW_H__0000D78A_D526_4484_8509_77D8892C0310__INCLUDED_)
#define AFX_BIBVIEW_H__0000D78A_D526_4484_8509_77D8892C0310__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NavigatorView.h"

class CBibView : public CNavigatorView  
{
public:
	CBibView();
	virtual ~CBibView();

	virtual void OnUpdate(CProjectView* pSender, LPARAM lHint, LPVOID pHint);
};

#endif // !defined(AFX_BIBVIEW_H__0000D78A_D526_4484_8509_77D8892C0310__INCLUDED_)
