#pragma once

// MultiExtensionDocTemplate command target

class MultiExtensionDocTemplate : public CMultiDocTemplate {
public:
	MultiExtensionDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass,
	                          CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);

	BOOL GetDocString(CString& rString, DocStringIndex index) const;
	CDocTemplate::Confidence MatchDocType(LPCTSTR pszPathName, CDocument*& rpDocMatch);
	
};

