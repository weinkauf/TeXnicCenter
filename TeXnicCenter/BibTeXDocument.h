#pragma once

#include "LaTeXDocumentBase.h"

// BibTeXDocument document

class BibTeXDocument : public LaTeXDocumentBase
{
	DECLARE_DYNCREATE(BibTeXDocument)

public:
	BibTeXDocument();
	virtual ~BibTeXDocument();

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
};
