#pragma once

#include "LaTeXDocumentBase.h"

class MetaPostDocument :
	public LaTeXDocumentBase
{
	DECLARE_DYNCREATE(MetaPostDocument)

public:
	MetaPostDocument(void);
	~MetaPostDocument(void);

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
};
