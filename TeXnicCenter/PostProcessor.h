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

/********************************************************************
 *
 * $Id$
 *
 ********************************************************************/

#if !defined(AFX_POSTPROCESSOR_H__6FC6E481_04F2_11D5_A222_006097239934__INCLUDED_)
#define AFX_POSTPROCESSOR_H__6FC6E481_04F2_11D5_A222_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/** An array of postprocessor specifications */
class CPostProcessorArray : public CArray<class CPostProcessor, class CPostProcessor&>
	{
		// operations
	public:
		/**
		Executes all postprocessors in the array in the index order.

		Calls Execute() for each postprocessor in the array.

		The Method will return when all postprocessors has been executed or
		one has returned an error. In that case the returned value will
		specify the index of the postprocessor in the array that has
		returned an error.

		@param lpszMainPath
		        Path of the project's main file. This parameter will be used
		        to expand the placeholders in the several properties of the
		        object.
		@param lpszWorkingDir
		        Path of the directory that should be used as the initial
		        directory for the postprocessor.
		@param hOutput
		        Handle informative output should be directed to.

		@return
		        Will return -1 if all postprocessor have been executed
		        successfully or a value >= 0 if the Execute()-method for one
		        postprocessor has returned FALSE. In this case the return value
		        will specify the index of the postprocessor in the array that
		        caused the error.

		@see CPostProcessor::Execute
		 */
		int ExecuteAll(LPCTSTR lpszMainPath, LPCTSTR lpszWorkingDir, HANDLE hOutput);

		/**
		Removes the directory specifications from all path specifications
		of this processor array.

		This is usefull when exporting profiles.
		 */
		void RemoveDirectorySpecifications();

		// registry serialization
	public:
		/**
		Stores the complete array to the current position in the given
		registry object, whereby each post processor will be stored as a
		single string.
		 */
		BOOL SerializeToRegistry(CBCGRegistryEx &reg) const;

		/**
		Reads the complete array from the current position of the given
		registry object.
		 */
		BOOL SerializeFromRegistry(CBCGRegistryEx &reg);

		/**
		Serializes the post processors to the specified XML element.

		@exception CComException
		 */
		void SaveXml(MsXml::CXMLDOMElement xmlPostProcessors) const;

		/**
		Loads the post processors from the specified XML element.

		@exception CComException
		 */
		void LoadXml(MsXml::CXMLDOMElement xmlPostProcessors);
	};

/**
Specification the properties of a compiler to be called, after the
(La)TeX-compiler (i.e. dvips).

@author Sven Wiegand
 */
class CPostProcessor
{
// construction/destruction
public:
	CPostProcessor(LPCTSTR lpszTitle = _T(""), LPCTSTR lpszPath = _T(""),
	               LPCTSTR lpszArguments = _T(""),
	               LPCTSTR lpszInputFile = _T(""), LPCTSTR lpszOutputFile = _T(""));
	virtual ~CPostProcessor();

// attribute operations
public:
	/** Sets the title */
	void SetTitle(LPCTSTR lpszTitle);

	/** Sets the path */
	void SetPath(LPCTSTR lpszPath);

	/**
	Sets the argument list to pass to the postprocessor's command line.

	Can contain placeholders.
	 */
	void SetArguments(LPCTSTR lpszArguments);

	/**
	Sets the full path of the file to use as standard input for the
	postproecessor.

	The parameter can contain place holders. If the parameter is an
	empty string, no input redirection will be used.
	 */
	void SetInputFile(LPCTSTR lpszInputFile);

	/**
	Sets the full path of the file to write the standard output of
	the postprocessor to.

	The parameter can contain place holders. If the parameter is an
	empty string, no output redirection will be used.
	 */
	void SetOutputFile(LPCTSTR lpszOutputFile);

	const CString& GetTitle() const;
	const CString& GetPath() const;
	const CString& GetArguments() const;
	const CString& GetInputFile() const;
	const CString& GetOutputFile() const;

// operations
public:
	/**
	Executes the postprocessor.

	First this method expands the placeholders in the argumengts, the
	input-file and the output-file properties and then it starts the
	postprocessor.

	The error output will be written to the specified output handle.
	The standard output will only be written to the specified output
	handle, if no output redirection has been specified by
	SetOutputFile().

	The Method will return when the process has finished.

	@param lpszMainPath
	        Path of the project's main file. This parameter will be used
	        to expand the placeholders in the several properties of the
	        object.
	@param lpszWorkingDir
	        Path of the directory that should be used as the initial
	        directory for the postprocessor.
	@param hOutput
	        Handle informative output should be directed to.
	@param phProcess
	        Pointer to a variable that should receive the handle of the
	        process generated to execute the postprocessor or NULL if the
	        caller is not interested in this handle. If non-NULL, the handle
	        will be assigned, before the methode blocks on waiting for the
	        process to terminate. Therefore the handle can be used to
	        terminate the process, while the method is blocking.
	 */
	BOOL Execute(LPCTSTR lpszMainPath, LPCTSTR lpszWorkingDir, HANDLE hOutput, PHANDLE phProcess = NULL);

	/**
	Returns the process-ID of the currently executed postprocessor.

	The returned value is only valid, if the postprocessor is running,
	initiated by a call to Execute().

	The value can be used as a parameter to GenerateConsoleCtrlEvent().
	 */
	DWORD GetProcessGroupId() const;

	/**
	Tries to cancel the last call to the Execute()-method sending a
	CTRL_BREAK_EVENT to the postprocessor.
	 */
	BOOL CancelExecution();

	/**
	Removes the directory specifications from all path specifications
	of this processor.

	This is usefull when exporting profiles.
	 */
	void RemoveDirectorySpecifications();

// implementation
protected:
	/**
	Gets the argument list, where the place holders are replaced by
	the properties of the given path.
	 */
	CString GetExpandedArguments(LPCTSTR lpszPath) const;

	/**
	Gets the input file property, where the optional place holders are
	replaced by the properties of the given path.
	 */
	CString GetExpandedInputFile(LPCTSTR lpszPath) const;

	/**
	Gets the output file property, where the optional place holders are
	replaced by the properties of the given path.
	 */
	CString GetExpandedOutputFile(LPCTSTR lpszPath) const;

// string serialization
public:
	/**
	Packs the information of this command into one string and returns
	this string.
	 */
	CString SerializeToString() const;

	/**
	Sets the attributes with the information received from the given
	string.

	@return
	        TRUE on success, FALSE if the given string does not have the
	        right format.
	 */
	BOOL SerializeFromString(LPCTSTR lpszPackedInformation);

	/**
	Serializes the post processor to the specified XML element.

	@exception CComException
	 */
	void SaveXml(MsXml::CXMLDOMElement xmlPostProcessor) const;

	/**
	Loads the post processor from the specified XML element.

	@exception CComException
	 */
	void LoadXml(MsXml::CXMLDOMElement xmlPostProcessor);

// attributes
protected:
	/** Title identifieing the postprossesor */
	CString m_strTitle;

	/** Full path to the postprocessor */
	CString m_strPath;

	/** Arguments to pass to the postprocessors command line */
	CString m_strArguments;

	/**
	Path of the file to use as standard input for the postprocessor
	or an empty string, if no input redirection should be used.
	 */
	CString m_strInputFile;

	/**
	Path of the file to write the output of the processor to
	or an empty string, if no output redirection should be used.
	 */
	CString m_strOutputFile;

private:
	/**
	Valid Process ID if the Execute()-method is running, invalid
	otherwise
	 */
	DWORD m_dwProcessGroupId;
};


// inlines

inline const CString& CPostProcessor::GetTitle() const
{
	return m_strTitle;
}

inline const CString& CPostProcessor::GetPath() const
{
	return m_strPath;
}

inline const CString& CPostProcessor::GetArguments() const
{
	return m_strArguments;
}

inline const CString& CPostProcessor::GetInputFile() const
{
	return m_strInputFile;
}

inline const CString& CPostProcessor::GetOutputFile() const
{
	return m_strOutputFile;
}

inline
DWORD CPostProcessor::GetProcessGroupId() const
{
	return m_dwProcessGroupId;
}

#endif // !defined(AFX_POSTPROCESSOR_H__6FC6E481_04F2_11D5_A222_006097239934__INCLUDED_)
