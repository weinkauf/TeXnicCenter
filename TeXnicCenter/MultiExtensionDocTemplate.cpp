#include "stdafx.h"
#include "TeXnicCenter.h"
#include "MultiExtensionDocTemplate.h"

MultiExtensionDocTemplate::MultiExtensionDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass,
                                                     CRuntimeClass* pFrameClass,
                                                     CRuntimeClass* pViewClass) :
	CMultiDocTemplate(nIDResource, pDocClass, pFrameClass, pViewClass)
{
	
}

BOOL MultiExtensionDocTemplate::GetDocString(CString& rString, DocStringIndex i) const
{
	CString strTemp, strLeft, strRight;
	int nFindPos;
	AfxExtractSubString(strTemp, m_strDocStrings, static_cast<int>(i));
	
	if (i == CDocTemplate::filterExt) {
		nFindPos = strTemp.Find(_T(';'));
		
		if (-1 != nFindPos) {
			//string contains two extensions
			strLeft = strTemp.Left(nFindPos + 1);
			strRight = strTemp.Right(strTemp.GetLength() - nFindPos - 1);
			strTemp = strLeft + strRight;
		}
	}

	rString = strTemp;

	return TRUE;
}

CDocTemplate::Confidence MultiExtensionDocTemplate::MatchDocType(LPCTSTR pszPathName,
                                                                 CDocument*& rpDocMatch)
{
	ASSERT(pszPathName != NULL);
	rpDocMatch = NULL;	
	
	// go through all documents
	POSITION pos = GetFirstDocPosition();
	
	while (pos != NULL) {
		CDocument* pDoc = GetNextDoc(pos);
		if (pDoc->GetPathName() == pszPathName) {
			// already open
			rpDocMatch = pDoc;
			return yesAlreadyOpen;
		}
	} // end while

	// see if it matches either suffix
	CString strFilterExt;
	
	if (GetDocString(strFilterExt, CDocTemplate::filterExt) && !strFilterExt.IsEmpty()) {
		// see if extension matches
		ASSERT(strFilterExt[0] == _T('.'));
		CString ext1, ext2;
		int nDot = CString(pszPathName).ReverseFind(_T('.'));		
		int nSemi = strFilterExt.Find(_T(';'));

		if (-1 != nSemi) {
			// string contains two extensions
			ext1 = strFilterExt.Left(nSemi);
			ext2 = strFilterExt.Mid(nSemi + 2);
			// check for a match against either extension
			if (nDot >= 0 && (lstrcmpi(pszPathName + nDot, ext1) == 0 || lstrcmpi(pszPathName
			        + nDot, ext2) == 0))
				return yesAttemptNative; // extension matches
		}
		else { // string contains a single extension
			if (nDot >= 0 && (lstrcmpi(pszPathName + nDot, strFilterExt) == 0))
				return yesAttemptNative; // extension matches
		}
	}
	
	return yesAttemptForeign; //unknown document type
}