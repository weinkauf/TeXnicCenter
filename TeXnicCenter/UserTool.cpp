#include "stdafx.h"
#include "TeXnicCenter.h"
#include "UserTool.h"
#include "PlaceHolder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------
// class CUserTool
//-------------------------------------------------------------------

IMPLEMENT_SERIAL(CUserTool, CBCGUserTool, 0)


CUserTool::CUserTool()
:	CBCGUserTool()
{}


CUserTool::~CUserTool()
{}


BOOL CUserTool::Invoke()
{
	if (m_strCommand.IsEmpty())
	{
		AfxMessageBox(STE_TOOL_NOCMDLINE, MB_ICONINFORMATION|MB_OK);
		return FALSE;
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Collected necessary information
	CString	strMainPath, strCurrentPath, strCurrentSelection;
	long		lCurrentLine = -1;

	// project information
	CLatexProject	*pProject = theApp.GetProject();
	if (pProject)
		strMainPath = pProject->GetMainPath();

	// current document specific information
	CLatexEdit	*pEdit = theApp.GetActiveEditView();
	if (pEdit)
	{
		strCurrentPath = pEdit->GetDocument()->GetPathName();
		pEdit->GetSelectedText(strCurrentSelection);
		lCurrentLine = pEdit->GetCursorPos().y + 1;
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Invoke tool
	CString	strCommand = AfxExpandPlaceholders(
		m_strCommand,
		strMainPath.IsEmpty()? (LPCTSTR)NULL : strMainPath,
		strCurrentPath.IsEmpty()? (LPCTSTR)NULL : strCurrentPath,
		lCurrentLine,
		strCurrentSelection.IsEmpty()? (LPCTSTR)NULL : strCurrentSelection);
	CString	strArguments = AfxExpandPlaceholders(
		m_strArguments,
		strMainPath.IsEmpty()? (LPCTSTR)NULL : strMainPath,
		strCurrentPath.IsEmpty()? (LPCTSTR)NULL : strCurrentPath,
		lCurrentLine,
		strCurrentSelection.IsEmpty()? (LPCTSTR)NULL : strCurrentSelection);
	CString	strInitialDirectory = AfxExpandPlaceholders(
		m_strInitialDirectory,
		strMainPath.IsEmpty()? (LPCTSTR)NULL : strMainPath,
		strCurrentPath.IsEmpty()? (LPCTSTR)NULL : strCurrentPath,
		lCurrentLine,
		strCurrentSelection.IsEmpty()? (LPCTSTR)NULL : strCurrentSelection);

	if (::ShellExecute (AfxGetMainWnd()->GetSafeHwnd (), NULL, strCommand,
		strArguments, strInitialDirectory, 
		SW_SHOWNORMAL) < (HINSTANCE) 32)
	{
		AfxMessageBox(STE_TOOL_CANTINVOKE, MB_ICONSTOP|MB_OK);
		return FALSE;
	}

	return TRUE;
}
