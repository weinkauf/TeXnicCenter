/********************************************************************
 *
 * This file is part of the TeXnicCenter-system
 *
 * Copyright (C) 1999-2000 Sven Wiegand
 * Copyright (C) 2000-$CurrentYear$ ToolsCenter
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * If you have further questions or if you want to support
 * further TeXnicCenter development, visit the TeXnicCenter-homepage
 *
 *    http://www.ToolsCenter.org
 *
 *********************************************************************/

#if !defined(AFX_PROCESS_H__46F6D321_233B_11D3_929E_D167829DA498__INCLUDED_)
#define AFX_PROCESS_H__46F6D321_233B_11D3_929E_D167829DA498__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/** @addtogroup process

        @{
 */

class CProcessException : public CException {
    DECLARE_DYNAMIC(CProcessException)

public:
    /// Do not use this constructor, use AfxThrowProcessException() istead.
    CProcessException();

    // overridables
public:
    virtual BOOL GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext = NULL);

    // attributes
public:
    /// value that specifies, why the exception was thrown.

    enum cause {
        generic = 0,
        notCreated,
        waitTimeOut
    } m_cause;
};

/// throws a pointer to a CProcessException
void AfxThrowProcessException(int nCause);

class CProcess {
public:
    /// standard constructor
    CProcess();
    virtual ~CProcess();

    // operations
public:
    /**
    Creates the process by calling CreateProcess().
	
    See documentation of CreateProcess() in Win31-API-documentation for
    parameter description.

    @return TRUE if successfull, FALSE otherwise.
     */
    BOOL Create(
                LPCTSTR lpszCmdLine,
                LPSECURITY_ATTRIBUTES lpsaProcess = NULL,
                LPSECURITY_ATTRIBUTES lpsaThread = NULL,
                BOOL bInheritHandles = FALSE,
                DWORD dwCreate = 0,
                LPVOID lpvEnvironment = NULL,
                LPCTSTR lpszCurDir = NULL,
                LPSTARTUPINFO lpsiStartInfo = NULL);

    /**
    Creates a console process without a visible window by calling 
    CreateProcess(). Should be used only for console processes.
	
    See documentation of CreateProcess() in Win31-API-documentation for
    parameter description.

    @param hStdInput Handle, the new process should use as standard input, or
            NULL, if the process should use the "normal" standard input.
    @param hStdOutput Handle, the new process should use as standard output, or
            NULL, if the process should use the "normal" standard output.
    @param hStdErr Handle, the new process should use as standard error output, or
            NULL, if the process should use the "normal" standard error output.

    @return TRUE if successfull, FALSE otherwise.
     */
    BOOL CreateHiddenConsole(
                             LPCTSTR lpszCmdLine,
                             HANDLE hStdInput, HANDLE hStdOutput, HANDLE hStdError,
                             DWORD dwCreate = 0,
                             LPCTSTR lpszCurDir = NULL,
                             LPSTARTUPINFO lpsiStartInfo = NULL);

    /**
    Creates a console process without a visible window and returns a 
    handle, where the created process writes its output to. You are 
    responsible for closing the retrieved handles. Should	be only used 
    for console processes.

    The redirection of standard output and standard error output is realised by
    a pipe.

    See documentation of CreateProcess() in Win31-API-documentation for
    parameter description.

    @param phStdOut After a successfull call this is the file handle, the process 
            writes its output to.
    @param phStdErr After a successfull call this is the file handle, the process
            writes its error messages to.

    @return TRUE if successfull, FALSE otherwise.
     */
    BOOL CreateHiddenConsole(
                             LPCTSTR lpszCmdLine,
                             PHANDLE phStdOut, PHANDLE phStdError,
                             DWORD dwCreate = 0,
                             LPCTSTR lpszCurDir = NULL,
                             LPSTARTUPINFO lpsiStartInfo = NULL);

    /**
    Returns the exit code the process returned.

    This value is STILL_ACTIVE, if the process is still running

    @throws CProcessException* if the system call was not successfull.
     */
    DWORD GetExitCode() const;

    /**
    Waits until the process exits.

    This call is blocking and does not return, until the process terminates. If
    the process has allready terminated, this function returns immediately.

    @param dwTimeOut Specifies the time-out interval, in milliseconds. 
            The function returns if the interval elapses, even if the process 
            has not terminated. If dwMilliseconds is INFINITE, the function's 
            time-out interval never elapses.

    @return The exit code the process terminated with.

    @throws CProcessException* if a time out occured or the system call was not
            successfull.
     */
    DWORD WaitForProcess(DWORD dwTimeOut = INFINITE) const;

    // attributes
protected:
    PROCESS_INFORMATION m_processInfo;

    /** TRUE, if a process has been started, else FALSE */
    BOOL m_bProcess;

public:
    ///	Returns the handle of the process.

    HANDLE GetProcessHandle() const
    {
        return m_processInfo.hProcess;
    }

    /// Returns the handle of the main thread of the process

    HANDLE GetThreadHandle() const
    {
        return m_processInfo.hThread;
    }

    /// Returns the ID of the process

    DWORD GetProcessID() const
    {
        return m_processInfo.dwProcessId;
    }

    /// Returns the ID of the main thread of the process

    DWORD GetThreadID() const
    {
        return m_processInfo.dwThreadId;
    }
};

/** @} */

#endif // !defined(AFX_PROCESS_H__46F6D321_233B_11D3_929E_D167829DA498__INCLUDED_)

