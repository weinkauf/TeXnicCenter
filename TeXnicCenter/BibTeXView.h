#pragma once

#include "LaTeXViewBase.h"

// BibTeXView view

class BibTeXView : public LaTeXViewBase
{
	DECLARE_DYNCREATE(BibTeXView)

protected:
	BibTeXView();           // protected constructor used by dynamic creation
	virtual ~BibTeXView();

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

protected:
	DocumentTokenizer* NewDocumentTokenizer() const;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnSettingsChanged();

private:
	void UpdateSettings();
};


