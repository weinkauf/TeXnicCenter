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
 *	 http://www.ToolsCenter.org
 *
 *********************************************************************/

/********************************************************************
 *
 * $Id$
 *
 ********************************************************************/

#if !defined(AFX_STRUCTUREPARSER_H__843BC262_339C_11D3_929E_444553540000__INCLUDED_)
#define AFX_STRUCTUREPARSER_H__843BC262_339C_11D3_929E_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <stack>

#include "TextSource.h"
#include "OutputInfo.h"

class CParseOutputHandler {

public:
    virtual void OnParseLineInfo(COutputInfo &/*line*/, int /*nLevel*/, int /*nSeverity*/)
    {
    }

    virtual void OnParseBegin(bool /*bCancelState*/)
    {
    }

    virtual void OnParseEnd(bool /*bResult*/,int /*nFiles*/,int /*nLines*/)
    {
    }

    enum tagSeverity {
        none = 0,
        information,
        warning,
        error,
        typeCount
    };

protected:
    /** The number of Output-Lines, that have been issued */
    int m_nOutputLines;
};

/**
An object of this class contains information about one item
of a document structure (for example a header).

@author Sven Wiegand
 */
class CStructureItem {
    //Construction/Destruction
    mutable int main_project_file_;

public:
    typedef std::vector<CString> LabelContainer;

private:
    LabelContainer labels_;

public:
    CStructureItem();

    //Attributes
public:
    /** Type this item is of. */
    int m_nType;

    int GetType() const;
    void SetType(int val);

    /** Path of file this item occurs in. */
    CString m_strPath;

    const CString& GetPath() const;
    void SetPath(const CString& val);

    /** Line in the file, this item occurs on. */
    int m_nLine;

    int GetLine() const;
    void SetLine(int val);

    /** Caption of this item */
    CString m_strCaption;

    const CString& GetCaption() const;
    void SetCaption(const CString& val);

    /** Label of this item */
    //CString m_strLabel;

    const CString GetLabel() const;
    void SetLabel(const CString& val);

    bool HasLabels() const;

    /** Title of this item */
    CString m_strTitle;

    const CString& GetTitle() const;
    void SetTitle(const CString& val);

    const LabelContainer& GetLabels() const;

    LabelContainer& GetLabels();

    void AddLabel(const CString& l)
    {
        labels_.push_back(l);
    }

    void RemoveLabels()
    {
        labels_.clear();
    }

    /** Comment of this item */
    CString m_strComment;

    const CString& GetComment() const;
    void SetComment(const CString& val);

    /** Index of the parent item or -1 if this item has no parent item */
    int m_nParent;

    int GetParent() const;
    void SetParent(int val);

    bool IsMainProjectFile() const;
};

/**
An object of this class is used by a CStructureParser-object to
handle the events of the CStructureParser-object.

This is an abstract base class -- you have to derive your own class
that implements the pure virtuals.

@author Sven Wiegand
 */
class CStructureParserHandler {
public:
    /**
    Called by the owning CStructureParser-object, when parsing is
    finished.

    @param bSuccess
            TRUE, if parsing finished successfully, FALSE otherwise.
     */
    virtual void OnParsingFinished(BOOL bSuccess) = 0;
};


/**
An Array of CStructureItem-objects.

@author Sven Wiegand
 */

typedef std::vector<CStructureItem> StructureItemContainer;

const int MAX_DEPTH	= 6;

/**
This class is used to parse the structure of a latex project

@author Sven Wiegand
 */
class CStructureParser {
    /** This function is used as a worker thread to parse the structure. */
    friend UINT StructureParserThread(LPVOID pStructureParser);

    // Item types
private:

    struct COOKIE {
        int nCookieType;
        int nItemIndex;
    };

    typedef std::stack<COOKIE> CCookieStack;

public:

    enum tagItemType {
        generic = 0,
        header,
        equation,
        quote,
        quotation,
        center,
        verse,
        itemization,
        enumeration,
        description,
        figure,
        table,
        unknownEnv,
        texFile,
        group,
        bibFile,
        graphicFile,
        bibItem,
        missingTexFile,
        missingGraphicFile,
        missingBibFile,
        // If you add a new type, also add a description string to m_sItemNames
        typeCount
    };

    static const CString m_sItemNames[typeCount];

    // construction/destruction
protected:
    /** Standard constructor. Do not use this one! */
    CStructureParser();
public:
    /**
    Constructs an object of the class CStructureParser.

    @param pStructureParserHandler
            Object that handles the events of the CStructureParser.
    @param pTextSourceManager
            Object that is used to get CTextSource objects.
     */
    CStructureParser(CStructureParserHandler *pStructureParserHandler,
                     /*CTextSourceManager *pTextSourceManager, */
                     CParseOutputHandler *pParseOutputHandler);

    virtual ~CStructureParser();

    // operations
public:
    /**
    Starts parsing with the main file.

    The parsing is executed by an extra thread. This method
    just starts the thread and returns immediately. When parsing
    is finished, the OnParsingFinished-Method() of the associated
    CStructureParserHandler-object is called.

    @see CStructureParserHandler
    @see CTextSourceManager
    @see CTextSource
    @see CStructureParserHandler::OnParsingFinished

    @param lpszMainPath
            Path of the project's main file.
    @param lpszWorkingDir
            The main directory of the parsed project.
    @param nPriority
            Priority that should be used for the parsing thread. See 
            SetThreadPriority() in the Win-API for valid values.
	
    @return 
            TRUE if successfull, FALSE otherwise.
     */
    BOOL StartParsing(LPCTSTR lpszMainPath,LPCTSTR lpszWorkingDir,int nPriority = THREAD_PRIORITY_IDLE);

    /** Populates the provided StructureItemContainer witht the elements of 
    the last parse. This thread safe, even window threads.

    @param pItemArray
            StructureItemContainer to populate. pItems is purged of all 
            elements before populating.

    @return
            <VAR>TRUE</VAR> on success else <VAR>FALSE</VAR>
     */
    BOOL GetStructureItems(StructureItemContainer *pItemArray);

    /**
    Cancels parsing.
     */
    void CancelParsing();

    /** Signaled when parser is inactive. Use this event to determine when 
    it is safe to destruct a handler. */
    CEvent m_evtParsingDone;

    // implementation
private:

    /** 
    Lock the internal structure item array.
     */
    inline void Lock()
    {
        ::EnterCriticalSection(&m_csSI);
    }

    /** 
    Unlock the internal structure item array.
     */
    inline void Unlock()
    {
        ::LeaveCriticalSection(&m_csSI);
    }

    /**
    Signals the end of parsing.
    @param bParsingResult <var>TRUE</var> if parsing was successful, else <var>FALSE</var>.
    @param cookies Stack of unprocessed cookies from parsing.
     */
    void Done(bool bParsingResult);

    /**
    Empties the cookie stack of all items.
    @param cookies
            Cookie stack
    @param aSI
            Structure Item Array to insert structure elemements into.
     */
    void EmptyCookieStack(CCookieStack &cookies,StructureItemContainer &aSI);

    /**
    Parses the specified file and fills the m_anItem-array.

    This method also parses all files mentioned in \input or
    \include-commands.

    @param lpszPath
            Path of the file to parse.
    @param cookies
            Stack of cookies.
    @param nFileDepth
            The number of files in processing stack.
    @param aSI
            Structure Item Array to insert structure elemements into.

    @return
            <var>TRUE</var> if parsing has been finished successfully, <var>FALSE</var> if
            parsing has been canceled.
     */
    BOOL Parse(LPCTSTR lpszPath,CCookieStack &cookies,int nFileDepth,StructureItemContainer &aSI);

    /**	Adds the specified file to the m_anItem-array.

    This method expects the file name to be resolved.

    @param lpszPath
            Path to the resolved file to add.
    @param nType
            Type of file
    @param aSI
            Structure Item Array to insert structure elements into.
    @param lpszAnnotation
            Annotation string for the file name.
            The displayed caption will look like this "Filename (Annotation)".
            May be NULL.

    @return
            index of file added to m_aStructureItems array.
     */
    int AddFileItem(LPCTSTR lpszPath,int nType, LPCTSTR lpszIncludeFromFile, int nIncludedFileLineNumber, StructureItemContainer &aSI,LPCTSTR lpszAnnotation = NULL);

    /** Creates a title from the caption or label of a CStructureItem.

            Prefers the caption.
            If both label and caption are empty, then it creates a title from the filename
            and the line number.

            @note Creates only a new title, if the current title is empty or if forced by bForce.

            @see CStructureItem

            @param bForce
                    Force the generation of a new title. Even if the current title is non-empty.
     */
    inline void CreateDefaultTitle(CStructureItem& si,bool bForce = false)
    {
        if (si.m_strTitle.IsEmpty() || bForce) {
            if (si.m_strCaption.IsEmpty() && !si.HasLabels()) {
                //Label and Caption empty ==> Generate a title from the filename
                si.m_strTitle.Format(_T("%s(%d)"),ResolveFileName(si.m_strPath),si.m_nLine);
            }
            else {
                if (si.GetCaption().IsEmpty())
                    si.SetTitle(si.GetLabel());
                else
                    si.SetTitle(si.GetCaption());
            }
        }
    }

    /**
    Resolve a file name relative to working directory or absolute.

    @param lpszPath
            Path of file to resolve
    @return
            Resolved path
     */
    CString ResolveFileName(LPCTSTR lpszPath) const;

    /**
    Parses the specified string and fills the m_anItem-array.

    @param lpText
            Pointer to first character of string to parse 
            (not guaranteed to be terminated by null-character).
    @param nLength
            Length of string to parse.
    @param cookies
            Stack of cookies.
    @param strActualFile
            The file this string is located in.
    @param nActualLine
            The actual line in the actual file.
    @param nFileDepth
            The depth of the parsed files.
    @param aSI
            Structure Item Array to insert structure elemements into.
     */
    void ParseString(LPCTSTR lpText,int nLength,CCookieStack &cookies,const CString &strActualFile,
                     int nActualLine,int nFileDepth,StructureItemContainer &aSI);

    /**
    Checks, if there is a LaTeX-command at the specified position.

    The method simply counts the number of backslashes from the
    given position to the beginning of the string. If the number
    modular 2 is zero, then there is no command, otherwise there
    is one.

    @param lpText
            String that contains the possible command
            (not guaranteed to be terminated by null-character).
    @param nPos
            Zero-based index of the first character of the possible
            command (the backslash).

    @return
            TRUE if there is a command at nPos, FALSE otherwise.
     */
    BOOL IsCmdAt(LPCTSTR lpText,int nPos);

    /**
    Extracts the argument, delimited by the character, which is the 
    first character in the given string. Evaluates the balance of 
    the delimiters to ensure to get the right element.

    Would return "This is a \emph{title}" for 
    "{This is a \emph{title}}\dummy[op]{arg}"

    @param strText
            String to extract the argument from.
    @param tcOpeningDelimiter
            Character that starts the argument to extract.
    @param tcClosingDelimiter
            Character that ends the argument to extract.
     */
    CString GetArgument(const CString &strText,TCHAR tcOpeningDelimiter,TCHAR tcClosingDelimiter);

    // attributes
protected:
    /** Handler for this structure parser */
    CStructureParserHandler *m_pStructureParserHandler;

    /** Handler for structure parser output */
    CParseOutputHandler *m_pParseOutputHandler;

    /** The text source manager associated with this object. */
    CTextSourceManager *m_pTextSourceManager;

    /** The array of structure items that describe the structure of the project. */
    StructureItemContainer *m_paStructureItems;

    /** The thread object, that does the parsing. */
    CWinThread *m_pStructureParserThread;

    /** The path of the main file to parse. */
    CString m_strMainPath;

    /** The working dir of the project that is parsed. */
    CString m_strWorkingDir;

    /** Regular expression describing a header. */
    const tregex m_regexHeader;

    /** Regular expression describing a comment. */
    const tregex m_regexComment;

    /** Regular expression describing a inline verbatim. */
    const tregex m_regexInlineVerb;

    /** Regular expression describing a verbatim start. */
    const tregex m_regexVerbStart;

    /** Regular expression describing a verbatim end. */
    const tregex m_regexVerbEnd;

    /** Regular expression describing a figure start. */
    const tregex m_regexFigureStart;

    /** Regular expression describing a figure end. */
    const tregex m_regexFigureEnd;

    /** Regular expression describing a table start. */
    const tregex m_regexTableStart;

    /** Regular expression describing a table end. */
    const tregex m_regexTableEnd;

    /** Regular expression describing an equation start. */
    const tregex m_regexEquationStart;

    /** Regular expression describing an equation end. */
    const tregex m_regexEquationEnd;

    /** Regular expression describing a center start. */
    const tregex m_regexCenterStart;

    /** Regular expression describing a center end. */
    const tregex m_regexCenterEnd;

    /** Regular expression describing the start of an unknown environment. */
    const tregex m_regexUnknownEnvStart;

    /** Regular expression describing the end of an unknown environment. */
    const tregex m_regexUnknownEnvEnd;

    /** Regular expression describing a caption. */
    const tregex m_regexCaption;

    /** Regular expression describing a label. */
    const tregex m_regexLabel;

    /** Regular expression describing an input-command. */
    const tregex m_regexInput;

    /** Regular expression describing the bibliography command. */
    const tregex m_regexBib;

    /** Regular expression describing the appendix command. */
    const tregex m_regexAppendix;

    /** Regular expression describing insert graphics commands. */
    const tregex m_regexGraphic;

    /** Regular expression describing the newcommand command. */
    const tregex m_regexUserCmd;

    /** Regular expression describing the newenvironment command. */
    const tregex m_regexUserEnv;

    /** Array containing the different headers. */
    static CString m_astrHeader[MAX_DEPTH];

    /** Array containing the index of the actual item representing the depth. */
    int m_anItem[MAX_DEPTH];

private:
    /** Used to control access to the m_aStructureItems-member. */
    CRITICAL_SECTION m_csSI;

    /** Actual depth */
    int m_nDepth;

    /** line counter */
    int m_nLineCount;

    /** <var>TRUE</var> if parsing should be stopped and the parser should be destroyed. */
    BOOL m_bCancel;

    /** Lines parsed */
    int m_nLinesParsed;

    /** Files parsed */
    int m_nFilesParsed;

    /** Characters parsed */
    int m_nCharsParsed;
};

#endif // !defined(AFX_STRUCTUREPARSER_H__843BC262_339C_11D3_929E_444553540000__INCLUDED_)
