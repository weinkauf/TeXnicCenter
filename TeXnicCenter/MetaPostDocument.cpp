#include "stdafx.h"
#include "MetaPostDocument.h"

IMPLEMENT_DYNCREATE(MetaPostDocument, LaTeXDocumentBase)

MetaPostDocument::MetaPostDocument(void)
{
}

MetaPostDocument::~MetaPostDocument(void)
{
}

// MetaPostDocument diagnostics

#ifdef _DEBUG
void MetaPostDocument::AssertValid() const
{
	LaTeXDocumentBase::AssertValid();
}

#ifndef _WIN32_WCE
void MetaPostDocument::Dump(CDumpContext& dc) const
{
	LaTeXDocumentBase::Dump(dc);
}
#endif
#endif //_DEBUG
