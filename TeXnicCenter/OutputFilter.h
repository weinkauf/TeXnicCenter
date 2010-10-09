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

#if !defined(AFX_OUTPUTFILTER_H__078A6980_2141_11D5_A222_006097239934__INCLUDED_)
#define AFX_OUTPUTFILTER_H__078A6980_2141_11D5_A222_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <deque>
#include <vector>

class COutputDoc;
class COutputView;
class COutputInfo;

/**
An object of this or an derived class can be used to parse the output
of an command line tool (i.e. the tex/latex-compiler) and write it to
an COutputView-derived class.

The parsing is done line by line by the virtual method ParseLine().

This base class does not do any parsing -- it only writes the output
to the specified view.

@author Sven Wiegand
 */
class COutputFilter : public CWorkerThread
{
	int errors_, warnings_, bad_boxes_;

// construction/destruction
public:
	/**
	Constructs a COutputFilter-object.

	See description of CWorkerThread::CWorkerThread for details.
	 */
	COutputFilter(BOOL bAutoDelete = FALSE);
    ~COutputFilter();

// operations
public:
	/**
	Creates the output filter with the specified properties.

	@param phFilterInput
	        Pointer to the variable that will receive the handle, the output
	        that should be passed by the filter can be written to.
	@param pDoc
	        Document the errors and warnings should be reported to.
	@param pView
	        Output-view, the output entered by *phFilterInput should be
	        written to.
	@param others
	        The other parameters are described by CWorkerThread::Create()

	@return
	        TRUE on success, FALSE otherwise.
	 */
	BOOL Create(PHANDLE phFilterInput, COutputDoc *pDoc, COutputView *pView,
	            BOOL bAutoCloseHandle = TRUE,
	            int nPriority = THREAD_PRIORITY_BELOW_NORMAL, DWORD dwCreationFlags = 0);

// implementation helpers
protected:
	/**
	Adds the specified line of text to the filter's output view.

	Can be used by derived classes to show additional information to
	the user.
	 */
	void AddLine(LPCTSTR lpszLine);

	/**
	Returns the zero based index of the currently parsed line in the
	output view.
	 */
	int GetCurrentOutputLine() const;

	/**
	Adds an error to the error list.
	 */
	void AddError(COutputInfo &error);

	/**
	Adds a warning to the warning list.
	 */
	void AddWarning(COutputInfo &warning);

	/**
	Adds a bad box to the bad box list.
	 */
	void AddBadBox(COutputInfo &badbox);

// overridables
public:
	/**
	Should return a result string that informs the user about how many
	errors and warnings have been found in the output or an empty
	string if the filter does not really parse the output.

	The string must not contain line feeds.

	This base implementation returns an empty string.
	 */
	virtual CString GetResultString();

protected:
	/**
	Called by the thread, each time, a whole line has been received, to
	parse this line.

	This default implementation returns immediately.

	@param strLine
	        Line to parse.
	@param dwCookie
	        Value returned by the last call to ParseLine() or zero if this
	        is the first call.

	@return
	        The returned value will be passed to the next ParseLine()-call
	        by the dwCookie-parameter. You can use this to remember states,
	        entered in that line and important for the following lines.
	 */
	virtual DWORD ParseLine(const CString& strLine, DWORD dwCookie);

	/**
	Called when all the output has been parsed and the thread is about
	to terminate.

	This default implementation just returns TRUE.

	@return
	        Should return TRUE if there where no errors reported in the
	        parsed output or FALSE, if there where errors and the build
	        process should be canceled.
	 */
	virtual BOOL OnTerminate();

// overridings
private:
	volatile bool cancel_;
    volatile bool stop_;

    typedef std::vector<char> CharVector;
    typedef std::deque<CharVector> DispatchDeque;

    DispatchDeque dispatchQueue_;
    CEvent processLineEvent_;
    CCriticalSection dispatchMonitor_;

    void ProcessLine(CharVector& line);
    static UINT LineDispatcherThread(LPVOID data);
    UINT LineDispatcherThread();

protected:
	virtual UINT Run();

// attributes
protected:
	/** The document, the errors and warnings should be reported to. */
	COutputDoc *m_pDoc;

private:
	/** The view, the output should be written to. */
	COutputView *m_pView;

	/** The read end of the pipe, the output to parse is written to */
	HANDLE m_hOutput;

public:
	int GetErrorCount() const;
	int GetWarningCount() const;
	int GetBadBoxCount() const;
	void Cancel();
};

#endif // !defined(AFX_OUTPUTFILTER_H__078A6980_2141_11D5_A222_006097239934__INCLUDED_)
