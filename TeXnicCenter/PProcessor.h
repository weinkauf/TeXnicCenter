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

#if !defined(AFX_PREPROCESSOR_H__6FC6E481_04F2_11D5_A222_006097239934__INCLUDED_)
#define AFX_PREPROCESSOR_H__6FC6E481_04F2_11D5_A222_006097239934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RegistryStack;

/** An array of pre-/postprocessor specifications.
*/
class CPProcessorArray : public CArray<class CPProcessor, class CPProcessor&>
{
	// operations
public:
	/**
	Executes all pre-/postprocessors in the array in the index order.

	Calls Execute() for each pre-/postprocessor in the array.

	The Method will return when all pre-/postprocessors has been executed or
	one has returned an error. In that case the returned value will
	specify the index of the pre-/postprocessor in the array that has
	returned an error.

	@param lpszMainPath
		    Path of the project's main file. This parameter will be used
		    to expand the placeholders in the several properties of the
		    object.
	@param lpszWorkingDir
		    Path of the directory that should be used as the initial
		    directory for the pre-/postprocessor.
	@param hOutput
		    Handle informative output should be directed to.

	@return
		    Will return -1 if all pre-/postprocessor have been executed
		    successfully or a value >= 0 if the Execute()-method for one
		    pre-/postprocessor has returned FALSE. In this case the return value
		    will specify the index of the pre-/postprocessor in the array that
		    caused the error.

	@see CPProcessor::Execute
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
	registry object, whereby each processor will be stored as a
	single string.
		*/
	BOOL SerializeToRegistry(RegistryStack &reg, LPCTSTR lpszTypeName) const;

	/**
	Reads the complete array from the current position of the given
	registry object.
		*/
	BOOL SerializeFromRegistry(RegistryStack &reg, LPCTSTR lpszTypeName);

	/**
	Serializes the processors to the specified XML element.

	@exception CComException
		*/
	void SaveXml(MsXml::CXMLDOMElement xmlPProcessors) const;

	/**
	Loads the processors from the specified XML element.

	@exception CComException
		*/
	void LoadXml(MsXml::CXMLDOMElement xmlPProcessors);
};

/** Specification of a tool to be called before/after the (La)TeX-compiler is called.

	@author Alexander Mueller
 */

class CPProcessor
{
// construction/destruction
public:
	CPProcessor(LPCTSTR lpszTitle = _T(""), LPCTSTR lpszPath = _T(""),
	               LPCTSTR lpszArguments = _T(""),
	               LPCTSTR lpszInputFile = _T(""), LPCTSTR lpszOutputFile = _T(""));
	virtual ~CPProcessor();

// attribute operations
public:
	/** Sets the title */
	void SetTitle(LPCTSTR lpszTitle);

	/** Sets the path */
	void SetPath(LPCTSTR lpszPath);

	/**
	Sets the argument list to pass to the processor's command line.

	Can contain placeholders.
	 */
	void SetArguments(LPCTSTR lpszArguments);

	/**
	Sets the full path of the file to use as standard input for the
	proecessor.

	The parameter can contain place holders. If the parameter is an
	empty string, no input redirection will be used.
	 */
	void SetInputFile(LPCTSTR lpszInputFile);

	/**
	Sets the full path of the file to write the standard output of
	the processor to.

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
	Executes the processor.

	First this method expands the placeholders in the argumengts, the
	input-file and the output-file properties and then it starts the
	processor.

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
	        directory for the processor.
	@param hOutput
	        Handle informative output should be directed to.
	@param phProcess
	        Pointer to a variable that should receive the handle of the
	        process generated to execute the processor or NULL if the
	        caller is not interested in this handle. If non-NULL, the handle
	        will be assigned, before the methode blocks on waiting for the
	        process to terminate. Therefore the handle can be used to
	        terminate the process, while the method is blocking.
	 */
	BOOL Execute(LPCTSTR lpszMainPath, LPCTSTR lpszWorkingDir, HANDLE hOutput, PHANDLE phProcess = NULL);

	/**
	Returns the process-ID of the currently executed processor.

	The returned value is only valid, if the processor is running,
	initiated by a call to Execute().

	The value can be used as a parameter to GenerateConsoleCtrlEvent().
	 */
	DWORD GetProcessGroupID() const;

	/**
	Tries to cancel the last call to the Execute()-method sending a
	CTRL_BREAK_EVENT to the processor.
	 */
	BOOL CancelExecution();

	/**
	Removes the directory specifications from all path specifications
	of this processor.

	This is useful when exporting profiles.
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
	Serializes the processor to the specified XML element.

	@exception CComException
	 */
	void SaveXml(MsXml::CXMLDOMElement xmlPProcessor) const;

	/**
	Loads the processor from the specified XML element.

	@exception CComException
	 */
	void LoadXml(MsXml::CXMLDOMElement xmlPProcessor);

// attributes
protected:
	/** Title identifying the prossesor */
	CString m_strTitle;

	/** Full path to the processor */
	CString m_strPath;

	/** Arguments to pass to the processors command line */
	CString m_strArguments;

	/**
	Path of the file to use as standard input for the processor
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


#endif // !defined(AFX_POSTPROCESSOR_H__6FC6E481_04F2_11D5_A222_006097239934__INCLUDED_)
