// BibTeXDocument.cpp : implementation file
//

#include "stdafx.h"
#include "BibTeXDocument.h"


// BibTeXDocument

IMPLEMENT_DYNCREATE(BibTeXDocument, LaTeXDocumentBase)

BibTeXDocument::BibTeXDocument()
{
}

BOOL BibTeXDocument::OnNewDocument()
{
	if (!LaTeXDocumentBase::OnNewDocument())
		return FALSE;
	return TRUE;
}

BibTeXDocument::~BibTeXDocument()
{
}

BEGIN_MESSAGE_MAP(BibTeXDocument, LaTeXDocumentBase)
END_MESSAGE_MAP()


// BibTeXDocument diagnostics

#ifdef _DEBUG
void BibTeXDocument::AssertValid() const
{
	LaTeXDocumentBase::AssertValid();
}

#ifndef _WIN32_WCE
void BibTeXDocument::Dump(CDumpContext& dc) const
{
	LaTeXDocumentBase::Dump(dc);
}

#endif
#endif //_DEBUG


// BibTeXDocument commands
