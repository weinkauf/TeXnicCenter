#include "stdafx.h"
#include "resource.h"
#include "LaTeXView.h"
#include "global.h"
#include "textfilesavedialog.h"
#include "configuration.h"
#include "CodeBookmark.h"
#include "CodeDocument.h"
#include "LaTeXDocumentBase.h"


IMPLEMENT_DYNAMIC(LaTeXDocumentBase, CodeDocument)

LaTeXDocumentBase::LaTeXDocumentBase()
{
}

LaTeXDocumentBase::~LaTeXDocumentBase()
{
}

BEGIN_MESSAGE_MAP(LaTeXDocumentBase, CodeDocument)
END_MESSAGE_MAP()

#ifdef _DEBUG
void LaTeXDocumentBase::AssertValid() const
{
	CodeDocument::AssertValid();
}

#ifndef _WIN32_WCE
void LaTeXDocumentBase::Dump(CDumpContext& dc) const
{
	CodeDocument::Dump(dc);
}
#endif
#endif //_DEBUG

CodeDocument* CodeView::GetDocument() const
{
	return static_cast<CodeDocument*>(CScintillaView::GetDocument());
}
