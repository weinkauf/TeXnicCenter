#pragma once

#include "LaTeXDocumentBase.h"

class CodeBookmark;

class LaTeXDocument : 
	public LaTeXDocumentBase
{
	DECLARE_DYNCREATE(LaTeXDocument)

public:
	LaTeXDocument();
	~LaTeXDocument();

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	const CString GetExtensionFilter() const;
	afx_msg void OnTextModulesDefine();
	afx_msg void OnUpdateTextModulesList(CCmdUI *pCmdUI);

	DECLARE_MESSAGE_MAP()
};
