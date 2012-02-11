#pragma once

#include "FileChangeWatcher.h"
#include "structureparser.h"
#include "CodeDocument.h"
#include "Nullable.h"

// LaTeXDocumentBase document

class LaTeXDocumentBase : 
	public CodeDocument
{
	DECLARE_DYNAMIC(LaTeXDocumentBase)

public:
	LaTeXDocumentBase();
	~LaTeXDocumentBase();

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

public:
	using CodeDocument::SaveFile;
};
