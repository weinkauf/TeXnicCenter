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
	// Collect necessary information
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
	//Be careful with the InitialDirectory: With CreateProcess it is not allowed
	// to be quoted or empty (In this case, standard is dir of main file).
	CString	strInitialDirectory = AfxExpandPlaceholders(
		m_strInitialDirectory.IsEmpty() ? "%dm" : m_strInitialDirectory,
		strMainPath.IsEmpty()? (LPCTSTR)NULL : strMainPath,
		strCurrentPath.IsEmpty()? (LPCTSTR)NULL : strCurrentPath,
		lCurrentLine,
		strCurrentSelection.IsEmpty()? (LPCTSTR)NULL : strCurrentSelection,
		false);
	strInitialDirectory.TrimLeft(_T('\"'));
	strInitialDirectory.TrimRight(_T('\"'));

	CProcessCommand	pc;
	pc.Set(m_strCommand, m_strArguments);//Args will be expanded by Execute
	CProcess *p = pc.Execute(strInitialDirectory, strMainPath, strCurrentPath,
														lCurrentLine, strCurrentSelection, true);
	if (!p)
	{
		TCHAR systemError[100];
		::FormatMessage(
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK,
			NULL,
			pc.GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			systemError,
			100,
			NULL 
		);

		CString strProcessResult;
		strProcessResult.Format(STE_LATEX_START_FAILED_EXT, pc.GetExecutable() + _T(' ') + pc.GetArguments(), systemError);
		AfxMessageBox(strProcessResult, MB_ICONSTOP|MB_OK);
		return FALSE;
	}

	//Delete the process object
	delete p;

	return TRUE;
}
