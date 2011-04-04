#pragma once

#include "LaTeXViewBase.h"

class MetaPostView : 
	public LaTeXViewBase
{
	DECLARE_DYNCREATE(MetaPostView)

public:
	MetaPostView(void);
	~MetaPostView(void);

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void UpdateSettings();

	DocumentTokenizer* NewDocumentTokenizer() const;
	int GetLexer() const;

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
};
