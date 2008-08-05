////////////////////////////////////////////////////////////////////////////
//	File:		CCrystalTextBuffer.h
//	Version:	1.0.0.0
//	Created:	29-Dec-1998
//
//	Author:		Stcherbatchenko Andrei
//	E-mail:		windfall@gmx.de
//
//	Interface of the CCrystalTextBuffer class, a part of Crystal Edit -
//	syntax coloring text editor.
//
//	You are free to use or modify this code to the following restrictions:
//	- Acknowledge me somewhere in your about box, simple "Parts of code by.."
//	will be enough. If you can't (or don't want to), contact me personally.
//	- LEAVE THIS HEADER INTACT
////////////////////////////////////////////////////////////////////////////

/********************************************************************
 *
 * $RCSfile: CCrystalTextBuffer.h,v $
 * $Revision$
 * $Date$
 *
 * $Author$
 *
 * $Log: CCrystalTextBuffer.h,v $
 * Revision 1.7  2002/04/29 22:55:07  cnorris
 * Spell checker niceness
 *
 * Revision 1.6  2002/04/29 19:45:17  cnorris
 * Make text attribute structure thread safe
 *
 * Revision 1.5  2002/04/24 00:46:04  cnorris
 * realtime spell check
 *
 * Revision 1.4  2002/04/23 21:45:09  cnorris
 * realtime spell check
 *
 * Revision 1.3  2002/04/09 23:30:17  cnorris
 * LoadFromFile and SaveToFile return the result of GetLastError to provide
 * usefull information about the type of error that occured.
 *
 * Revision 1.2  2002/03/26 16:24:58  cnorris
 * Re-wrote LineInfo to manage its own data and manipulation
 *
 * Revision 1.1.1.1  2002/02/26 08:11:53  svenwiegand
 * Initial revision
 *
 * Revision 1.0  2000-05-31 21:55:24+02  sven_wiegand
 * Initial revision
 *
 *
 ********************************************************************/

#if !defined(AFX_CCRYSTALTEXTBUFFER_H__AD7F2F49_6CB3_11D2_8C32_0080ADB86836__INCLUDED_)
#define AFX_CCRYSTALTEXTBUFFER_H__AD7F2F49_6CB3_11D2_8C32_0080ADB86836__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <vector>

#include "cedefs.h"
#include "CCrystalTextView.h"
#include "../../MFCExt/Include/ExtCollection.h"

#ifndef __AFXTEMPL_H__
#pragma message("Include <afxtempl.h> in your stdafx.h to avoid this message")
#include <afxtempl.h>
#endif

#define UNDO_DESCRIP_BUF        32

enum LINEFLAGS
{
	LF_BOOKMARK_FIRST = 0x00000001L,
	LF_EXECUTION = 0x00010000L,
	LF_BREAKPOINT = 0x00020000L,
	LF_COMPILATION_ERROR = 0x00040000L,
	LF_BOOKMARKS = 0x00080000L,
	LF_INVALID_BREAKPOINT = 0x00100000L
};

#define LF_BOOKMARK(id)		(LF_BOOKMARK_FIRST << id)

enum CRLFSTYLE
{
	CRLF_STYLE_AUTOMATIC = -1,
	CRLF_STYLE_DOS = 0,
	CRLF_STYLE_UNIX = 1,
	CRLF_STYLE_MAC = 2
};

enum
{
	CE_ACTION_UNKNOWN = 0,
	CE_ACTION_PASTE = 1,
	CE_ACTION_DELSEL = 2,
	CE_ACTION_CUT = 3,
	CE_ACTION_TYPING = 4,
	CE_ACTION_BACKSPACE = 5,
	CE_ACTION_INDENT = 6,
	CE_ACTION_DRAGDROP = 7,
	CE_ACTION_REPLACE = 8,
	CE_ACTION_DELETE = 9,
	CE_ACTION_AUTOINDENT = 10
	//	...
	//	Expandable: user actions allowed
};


/////////////////////////////////////////////////////////////////////////////
// CUpdateContext class

class CUpdateContext
{
public:
	virtual void RecalcPoint(CPoint &ptPoint) = 0;
};


/////////////////////////////////////////////////////////////////////////////
// CCrystalTextBuffer command target

class CRYSEDIT_CLASS_DECL CCrystalTextBuffer : public CCmdTarget
{
	DECLARE_DYNCREATE(CCrystalTextBuffer)

private:
	BOOL m_bInit;
	BOOL m_bReadOnly;
	BOOL m_bModified;
	BOOL m_bFirstModify;
	int m_nCRLFMode;
	BOOL m_bCreateBackupFile;
	int m_nUndoBufSize;
	int FindLineWithFlag(DWORD dwFlag);

#pragma pack(push, 1)
//	Nested class declarations
public:
	enum Encoding
	{
		ASCII,
		UTF8,
		UTF16LE,
		UTF16BE,
		UTF32LE,
		UTF32BE
	};

private:
	Encoding encoding_;
	UINT code_page_;

public:
	Encoding GetEncoding() const;
	UINT GetCodePage() const;

	class CTextAttribute
	{
	public:

		enum tagAttribute
		{
			none,
			spellError = 1,
			// ... your new attribute type here
			nAttributeCount // must be last attribute
		};

		CTextAttribute()
		{
		}

		CTextAttribute(int nStart, int nEnd, tagAttribute attribute)
		{
			m_nStartPos = nStart;
			m_nEndPos = nEnd;
			m_Attribute = attribute;
		}

		bool operator<(const CTextAttribute &rhs) const
		{
			return (m_nStartPos < rhs.m_nStartPos);
		}

		CTextAttribute & operator+=(int n)
		{
			m_nStartPos += n;
			m_nEndPos += n;
			return *this;
		}

		CTextAttribute & operator-=(int n)
		{
			m_nStartPos -= n;
			m_nEndPos -= n;
			return *this;
		}

		CTextAttribute & operator=(const CTextAttribute &rhs)
		{
			m_nStartPos = rhs.m_nStartPos;
			m_nEndPos = rhs.m_nEndPos;
			m_Attribute = rhs.m_Attribute;
			return *this;
		}

	public:
		int m_nStartPos;
		int m_nEndPos;
		tagAttribute m_Attribute;
	};
	typedef CSortList<CTextAttribute, CTextAttribute&> TextAttributeListType;

protected:

	class CLineInfo
	{
	public:

		/**
		 * Default constructor
		 */
		CLineInfo()
		{
			memset(this, 0, sizeof(CLineInfo));
		}

		/**
		Constructor
		@param pszLine
		        Text to initialize line with. Does not need to be null terminated in nLength given.
		@param nLength
		        Length of string. Default value (-1) indicates nLength should be calculated.
		 */
		explicit CLineInfo(LPCTSTR pszLine, int nLength = -1)
		{
			memset(this, 0, sizeof(CLineInfo));
			Append(pszLine, nLength);
		}

		/**
		Constructor
		@param nSize
		        Initialize line with enough capacity to contain at least nSize characters.
		 */
		explicit CLineInfo(int nSize)
		{
			memset(this, 0, sizeof(CLineInfo));
			GrowTo(nSize);
		}

		/**
		Grow (or shrink) the line to accommodate at least nMinSize characters.
		@param nMinSize
		        Minimum number of characters (not including NULL terminator).
		 */
		void GrowTo(int nMinSize);

		/**
		Append text to end of line. If required line is grown to accommodate new string.
		@param pszLine
		        Text to append. Does not need to be null terminated in nLength given.
		@param nLength
		        Length of string to append. Default value (-1) indicates nLength should be calculated.
		 */
		void Append(LPCTSTR pszLine, int nLength = -1);

		/**
		Insert text into middle of line. If required line is grown to accommodate new string.
		@param nPos
		        Insertion point.
		@param pszLine
		        Text to insert. Does not need to be null terminated in nLength given.
		@param nLength
		        Length of string to insert. Default value (-1) indicates nLength should be calculated.
		 */
		void InsertText(int nPos, LPCTSTR pszLine, int nLength = -1);

		/**
		Remove text from line.
		@param nPos
		        Removal point.
		@param nCount
		        Number of characters to remove.
		@param bCompact
		        Should extra space, if any, in line be freed?
		 */
		void RemoveText(int nPos, int nCount, bool bCompact = false);

		/**
		Trim line to new length.
		@param nPos
		        Length of new string.
		@param bCompact
		        Should extra space, if any, in line be freed?
		 */
		void TrimText(int nLength, bool bCompact = false);

		/**
		Remove extra line capacity while maintaining buffer alignment.
		 */
		void FreeExtra()
		{
			GrowTo(m_nLength);
		}

		/**
		Destroy line and release dynamic memory.
		 */
		void Destroy()
		{
			delete [] m_pcLine;
			delete m_lstAttributes;
			memset(this, 0, sizeof(CLineInfo));
		}

		/**
		Get the flag bits.
		@return Flag bits of line.
		 */
		DWORD GetFlags() const
		{
			return m_dwFlags;
		}

		/**
		Set the flag bits.
		@param dwFlags Flag bits.
		 */
		void SetFlags(DWORD dwFlags)
		{
			m_dwFlags = dwFlags;
		}

		/**
		Get the length of the line.
		@return Length of line.
		 */
		int GetLength() const
		{
			return m_nLength;
		}

		/**
		Insert a line attribute
		@param insertMe Attribute to insert to the attribute list.
		 */
		inline
		void InsertAttribute(CTextAttribute &insertMe)
		{
			if (m_lstAttributes == NULL)
				m_lstAttributes = new TextAttributeListType;
			m_lstAttributes->InsertSorted(insertMe);
		}

		/**
		Clear all line attributes.
		 */
		void ClearAttributes();

		/**
		Clear all line attributes that match the given type.
		@param attribute Attribute type to remove.
		 */
		void ClearAttributes(CTextAttribute::tagAttribute attribute);

		/**
		Clear all line attributes within the range (nStart nEnd].
		@param nStart The start position of the clear
		@param nEnd The end position of the clear
		@bIncludeOverlap If <VAR>TRUE</VAR> clear attributes that overlap
		range, else only clear attributes completely contained within range.
		 */
		void ClearAttributes(int nStart, int nEnd, BOOL bIncludeOverlap = TRUE);

		/**
		Shift attributes along line. The attribute may be shifted backwards.
		@param nStart Starting point of shift.
		@param nSpaces Number of spaces to shift. May be negative.
		 */
		void ShiftAttributes(int nStart, int nSpaces);

		/**
		Get the number of line attributes.
		@return Number of attributes
		 */
		inline
		int GetAttributeCount()
		{
			if (m_lstAttributes != NULL)
				return m_lstAttributes->GetCount();
			return 0;
		}

		/** Get the number of attributes that match the given type.
		@param attribute Attribute type to count.
		@return Number of matched attributes.
		 */
		int GetAttributeCount(CTextAttribute::tagAttribute attribute);

		/** Character buffer */
		TCHAR *m_pcLine;

		/** Line attribute list */
		TextAttributeListType *m_lstAttributes;
	protected:
		int m_nMax;
		int m_nLength;
		DWORD m_dwFlags;
	};

	enum
	{
		UNDO_INSERT = 0x0001,
		UNDO_BEGINGROUP = 0x0100
	};

	//	[JRT] Support For Descriptions On Undo/Redo Actions

	struct SUndoRecord
	{
		DWORD m_dwFlags;

		CPoint m_ptStartPos, m_ptEndPos; //	Block of text participating
		int m_nAction; //	For information only: action type

private:
		//	TCHAR	*m_pcText;
		//	Since in most cases we have 1 character here,
		//	we should invent a better way. Note: 2 * sizeof(WORD) <= sizeof(TCHAR*)
		//
		//	Here we will use the following trick: on Win32 platforms high-order word
		//	of any pointer will be != 0. So we can store 1 character strings without
		//	allocating memory.
		//

		union
		{
			TCHAR *m_pszText; //	For cases when we have > 1 character strings
			TCHAR m_szText[2]; //	For single-character strings
		};

public:
		//	constructor/destructor for this struct

		SUndoRecord()
		{
			memset(this, 0, sizeof(SUndoRecord));
		};

		void SetText(LPCTSTR pszText);
		void FreeText();

		LPCTSTR GetText() const
		{
			if (HIWORD((DWORD)m_pszText) != 0)
				return m_pszText;
			return m_szText;
		};
	};

#pragma pack(pop)

	class CInsertContext : public CUpdateContext
	{
	public:
		CPoint m_ptStart, m_ptEnd;
		virtual void RecalcPoint(CPoint &ptPoint);
	};

	class CDeleteContext : public CUpdateContext
	{
	public:
		CPoint m_ptStart, m_ptEnd;
		virtual void RecalcPoint(CPoint &ptPoint);
	};

	//	Lines of text
	CArray <CLineInfo, CLineInfo&> m_aLines;

	// Critical section to protect line attributes.
	CRITICAL_SECTION m_csLineAttributes;

	//	Undo
	CArray <SUndoRecord, SUndoRecord&> m_aUndoBuf;
	int m_nUndoPosition;
	int m_nSyncPosition;
	BOOL m_bUndoGroup, m_bUndoBeginGroup;

	//BEGIN SW
	/** Position where the last change was made. */
	CPoint m_ptLastChange;
	//END SW

	//	Connected views
	CList <CCrystalTextView *, CCrystalTextView *> m_lpViews;

	//	Helper methods
	void InsertLine(LPCTSTR pszLine, int nLength = -1, int nPosition = -1);
	void AppendLine(int nLineIndex, LPCTSTR pszChars, int nLength = -1);

	//	Implementation
	BOOL InternalInsertText(CCrystalTextView *pSource, int nLine, int nPos, LPCTSTR pszText, int &nEndLine, int &nEndChar);
	BOOL InternalDeleteText(CCrystalTextView *pSource, int nStartLine, int nStartPos, int nEndLine, int nEndPos);

	//	[JRT] Support For Descriptions On Undo/Redo Actions
	void AddUndoRecord(BOOL bInsert, const CPoint &ptStartPos, const CPoint &ptEndPos,
	                   LPCTSTR pszText, int nActionType = CE_ACTION_UNKNOWN);

	//	Overridable: provide action description
	virtual BOOL GetActionDescription(int nAction, CString &desc);

// Operations
public:
	//	Construction/destruction code
	CCrystalTextBuffer();
	virtual ~CCrystalTextBuffer();

	//	Basic functions
	BOOL InitNew(int nCrlfStyle = CRLF_STYLE_DOS);
	DWORD LoadFromFile(LPCTSTR pszFileName, int nCrlfStyle = CRLF_STYLE_AUTOMATIC);
	DWORD SaveToFile(LPCTSTR pszFileName, int nCrlfStyle = CRLF_STYLE_AUTOMATIC, BOOL bClearModifiedFlag = TRUE);
	void FreeAll();

	//	'Dirty' flag
	virtual void SetModified(BOOL bModified = TRUE);
	BOOL IsModified() const;

	//	Connect/disconnect views
	void AddView(CCrystalTextView *pView);
	void RemoveView(CCrystalTextView *pView);

	//	Text access functions
	int GetLineCount();
	int GetLineLength(int nLine);
	LPTSTR GetLineChars(int nLine);
	DWORD GetLineFlags(int nLine);
	int GetLineWithFlag(DWORD dwFlag);
	void SetLineFlag(int nLine, DWORD dwFlag, BOOL bSet, BOOL bRemoveFromPreviousLine = TRUE);
	void GetText(int nStartLine, int nStartChar, int nEndLine, int nEndChar, CString &text, LPCTSTR pszCRLF = NULL);

	//	Attributes
	int GetCRLFMode();
	void SetCRLFMode(int nCRLFMode);
	BOOL GetReadOnly() const;
	void SetReadOnly(BOOL bReadOnly = TRUE);

	//	Text modification functions
	BOOL InsertText(CCrystalTextView *pSource, int nLine, int nPos, LPCTSTR pszText, int &nEndLine, int &nEndChar, int nAction = CE_ACTION_UNKNOWN);
	BOOL DeleteText(CCrystalTextView *pSource, int nStartLine, int nStartPos, int nEndLine, int nEndPos, int nAction = CE_ACTION_UNKNOWN);

	//	Undo/Redo
	BOOL CanUndo();
	BOOL CanRedo();
	BOOL Undo(CPoint &ptCursorPos);
	BOOL Redo(CPoint &ptCursorPos);

	//	Undo grouping
	void BeginUndoGroup(BOOL bMergeWithPrevious = FALSE);
	void FlushUndoGroup(CCrystalTextView *pSource);

	//BEGIN SW
	/**
	Returns the position where the last changes where made.
	 */
	CPoint GetLastChangePos() const;
	//END SW

	//	Browse undo sequence
	POSITION GetUndoDescription(CString &desc, POSITION pos = NULL);
	POSITION GetRedoDescription(CString &desc, POSITION pos = NULL);

	//	Notify all connected views about changes in text
	void UpdateViews(CCrystalTextView *pSource, CUpdateContext *pContext,
	                 DWORD dwUpdateFlags, int nLineIndex = -1);

	// More bookmarks
	int FindNextBookmarkLine(int nCurrentLine = 0);
	int FindPrevBookmarkLine(int nCurrentLine = 0);

public:
	// Line attribute operations

	/**
	Clear all matching text attributes from a line.
	@param nLine Line number
	@param attribute Attribute type to remove
	 */
	void ClearLineAttributes(int nLine, CTextAttribute::tagAttribute attribute);

	/**
	Clear all text attribute from a line.
	@param nLine Line number
	 */
	void ClearLineAttributes(int nLine);

	/**
	Insert a text attribute.
	@param nLine Line number to insert attribute in.
	@param attribute Line attribute to insert.
	 */
	void InsertLineAttribute(int nLine, CTextAttribute &attribute);

	/**
	Get the first text attribute within the given range.
	@param nLine Line number
	@param nStart Start of search range
	@param nEnd End of search range
	 */
	CTextAttribute* GetLineAttribute(int nLine, int nStart, int nEnd);

	/**
	Get the text attribute list. Must be matched by a call to
	ReleaseLineAttributes().
	@param nLine Line number to get.
	 */
	TextAttributeListType* GetLineAttributes(int nLine);

	/**
	Release the text attributes. Allow other threads access to the text
	attribute data structure.
	 */
	inline
	void ReleaseLineAttributes();

	/**
	Lock the text attributes. Prevent other threads from accessing the text
	attribute data structre. Must be released with a call to ReleaseLineAttributes()
	 */
	inline
	void LockLineAttributes();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCrystalTextBuffer)
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCrystalTextBuffer)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	/** Deletes a complete line of text
	    @param line Line to delete
	*/
	BOOL DeleteLine(CCrystalTextView* source, int line);
	static Encoding DetectEncoding(const BYTE* data, SIZE_T& pos, SIZE_T size);
	static void ConvertToMultiByte(LPCWSTR input, int cch, std::vector<BYTE>& buffer, Encoding encoding, UINT cp);
};

#if ! (defined(CE_FROM_DLL) || defined(CE_DLL_BUILD))
#include "CCrystalTextBuffer.inl"
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CCRYSTALTEXTBUFFER_H__AD7F2F49_6CB3_11D2_8C32_0080ADB86836__INCLUDED_)
