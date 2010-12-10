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



#include "stdafx.h"

#include "StyleFile.h"
#include "LaTeXCommand.h"
#include "NewCommand.h"
#include "NewEnvironment.h"
#include "DeclareOption.h"
#include "StyleFileContainer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

const TCHAR* const TOKENS[] =
{
	_T("\\newcommand"),
	_T("\\newenvironment"),
	_T("\\DeclareOption"),
	_T("\\RequirePackage"),
	_T("\\def"),
	_T("\\let"),
	_T("\\ProvidesClass"),
	_T("\\ProvidesPackage"),
	_T("\\DeclareTextSymbol"),
	_T("\\DeclareMathSymbol"),
	_T("\\DeclareTextCommand")
};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStyleFile::CStyleFile(const CString &name, const CString &desc, bool isClass)
{
	//m_Filename = _T("");
	m_Name = name;
	m_Desc = desc;
	m_IsClass = isClass;
	Init();
}

CStyleFile::CStyleFile(CStyleFile &file, bool isClass)
{
	m_Filename = file.m_Filename;
	m_Name = file.GetName();
	//m_Desc = _T("");
	m_IsClass = isClass;
	Init();
}

CStyleFile::CStyleFile(const CString& filename, bool isClass)
{
	m_Filename = filename;
	m_Name = CPathTool::GetFileTitle(filename);
	//m_Desc = _T("");
	m_IsClass = isClass;
	Init();
}

void CStyleFile::Init()
{
	m_Listener = NULL;
}

CStyleFile::~CStyleFile()
{
}

CStyleFile& CStyleFile::operator =(const CStyleFile& sf)
{
	if (this != &sf)
	{
		m_Filename = sf.m_Filename;
	}

	return *this;
}

/**
 Processes a file, if it contains valuable commands. The file is loaded into a buffer
 a is passed to <it>ParseBuffer</it>, where all commands will be extracted.
 */
void CStyleFile::ProcessFile()
{
	CFile f;

	try
	{
		f.Open(m_Filename, CFile::modeRead);
		ULONGLONG l = f.GetLength();

		CString text;
		f.Read(text.GetBuffer(l), l * sizeof(TCHAR));
		text.ReleaseBuffer();

		if (HasCommands(text))
		{
			ParseBuffer(text);
		}
	}
	catch (CFileException& ex)
	{
		TRACE(_T("Error opening style file: %s\n"), ex);
		UNUSED_ALWAYS(ex);

		f.Close();
	}

	f.Close();
}

/**
 Checks if a buffer contains given keywords.
 */
bool CStyleFile::HasCommands(const TCHAR *buf)
{
	for (int i = 0; i < TOKEN_COUNT; i++)
	{
		if (_tcsstr(buf, TOKENS[i]) != NULL)
		{
			return true;
		}
	}
	return false;
}

/**
 Parses a buffer and creates appropriate instances. It works on a very low level.
 Ugly, but fast :-)
 */
void CStyleFile::ParseBuffer(const TCHAR *buf)
{

	for (int i = 0; i < TOKEN_COUNT; i++)
	{
		const TCHAR *t, *token;
		TCHAR nameBuf[255];
        std::size_t lTok = _tcslen(TOKENS[i]);

		token = _tcsstr(buf, TOKENS[i]);
		while (token != NULL)
		{
			t = token;
			token += lTok;
			bool hasStar = false;
			bool isPrivate = false;
			int nOptions = 0;

			const TCHAR *first = _tcsstr(token, _T("\\"));
			const TCHAR *second = _tcsstr(token, _T("{"));
			const TCHAR *close = _tcsstr(token, _T("}"));
			const TCHAR *openBr = _tcsstr(token, _T("["));
			const TCHAR *closeBr = _tcsstr(token, _T("]"));

			const TCHAR *p;

			if (first == NULL && second != NULL)
			{
				p = second;
			}
			else if (first != NULL && second == NULL)
			{
				p = first;
			}
			else if (first != NULL && second != NULL)
			{
				p = (first < second ? first : second);
			}
			else
			{
				token = _tcsstr(token + 1, TOKENS[i]);
				continue;
			}

			if (*token == _T('*'))
			{
				hasStar = true;
			}

			if (i == LATEX_COMMAND)   /* Commands have a leading _T('\') */
			{
				++token;
			}

			/* Get number of options */
			nOptions = ExtractOptionCount(close, openBr, closeBr);

			while (!(_istalpha(*p) || _istdigit(*p) || *p == _T('@'))) ++p; /* skip comments, white spaces, etc... */

			const TCHAR *pStart = p;

			while (_istalpha(*p) || _istdigit(*p) || *p == _T('@'))
			{
				if (*p == _T('@'))
				{
					isPrivate = true; /* mark as internal command */
				}
				++p;
			}

			int l = p - pStart;

			if (l < 255 && l > 1 && !isPrivate)   /* Command valid? */
			{
				_tcsncpy((TCHAR*) & nameBuf, pStart, l);
				nameBuf[l] = 0;

				/* Create appropriate instance */
				std::tr1::shared_ptr<CAbstractLaTeXCommand> lc(CreateItem(i, nameBuf, hasStar, nOptions));

				if (lc != NULL)   /* Instance valid -> Notify container and add to list */
				{

					switch (i)
					{
						case LATEX_COMMAND:
						case LATEX_ENVIRONMENT:
						case LATEX_DEF:
						case LATEX_LET:
						case LATEX_TXT_SYMBOL:
						case LATEX_MATH_SYMBOL:
						case LATEX_TXT_COMMAND: 
							{
								SharedLaTeXCommandPtr cmd = std::tr1::dynamic_pointer_cast<CLaTeXCommand>(lc);
								/* Some commands may be duplicate due to conditional definitions in the
								   style file. */
								if (!AddCommand(cmd))
								{
								}
							}
							break;
						case LATEX_OPTION:
							if (!CStyleFileContainer::ContainsString(&m_Options, lc->GetName()))
							{
								m_Options.Add(lc->GetName());
							}

							break;
						case LATEX_REQPACKAGE:
							m_ReqPackages.Add(lc->GetName());
							break;
						case LATEX_CLS_DESC:
						case LATEX_STY_DESC:
							ExtractDescription(close, openBr, closeBr, m_Desc);
							TRACE(_T("Desc: %s: <%s>\n"), m_Name, m_Desc);
							break;
					}
				}
			}
			else   /* For debug purposes only */
			{
				if (l > 1 && !isPrivate)
				{
					TRACE(_T("!! Buffer to small: Needs %d bytes\n"), l);
				}
				else if (!isPrivate)
				{
					_tcsncpy(nameBuf, t, 50);
					_tcsncpy(((TCHAR*)(&nameBuf) + 50), _T("..."), 3);
					nameBuf[53] = 0;

					TRACE(_T("** Invalid name: %s\n"), nameBuf);

					_tcsncpy(nameBuf, pStart, 10);
					_tcsncpy(((TCHAR*)(&nameBuf) + 10), _T("..."), 3);
					nameBuf[13] = 0;

					TRACE(_T("** pStart = %s\n"), nameBuf);

					_tcsncpy(nameBuf, p, 10);
					_tcsncpy(((TCHAR*)(&nameBuf) + 10), _T("..."), 3);
					nameBuf[13] = 0;

					TRACE(_T("** p = %s\n"), nameBuf);
				}
			}

			/* Get next token: */
			token = _tcsstr(token + 1, TOKENS[i]);
		}
	}
}

/**
 Retrieves the number of options of a command or environment. Does not work for commands
 defined via _T('\def') (TeX-Style).
 */
int CStyleFile::ExtractOptionCount(const TCHAR *closePar, const TCHAR *openBr, const TCHAR *closeBr)
{
	if (closePar == NULL || openBr == NULL || closeBr == NULL) return -2; /* No option avalable */

	int d = closeBr - openBr;
	if (d < 0 || d > 2) return -1;
	TCHAR *p = (TCHAR*)closePar;
	TCHAR buf[10];

	++p;
	while (p < openBr)
	{
		if (_istdigit(*p) || _istalpha(*p))
		{
			return -3;
		}
		++p;
	}

	_tcsncpy(buf, openBr + 1, d);
	buf[d] = 0;

	return _ttoi(buf);
}

/**
 Creates an instance of a LaTeX command (factory pattern)
 */
CAbstractLaTeXCommand * CStyleFile::CreateItem(int type, const CString &name, bool hasStar,int noOfParams)
{
	switch (type)
	{
		case LATEX_COMMAND:
		case LATEX_DEF:
		case LATEX_LET:
			return new CNewCommand(shared_from_this(), name, noOfParams, hasStar);
			break;
		case LATEX_TXT_SYMBOL:
		case LATEX_MATH_SYMBOL:
			return new CNewCommand(shared_from_this(), name, 0, hasStar);
			break;
		case LATEX_TXT_COMMAND:
			return new CNewCommand(shared_from_this(), name, noOfParams, hasStar);
			break;
		case LATEX_ENVIRONMENT:
			return new CNewEnvironment(shared_from_this(), name, noOfParams);
			break;
		case LATEX_OPTION:
			return new CDeclareOption(shared_from_this(), name);
			break;
		case LATEX_REQPACKAGE:
			return new CDeclareOption(shared_from_this(), name); /* murx */
			break;
		case LATEX_CLS_DESC:
		case LATEX_STY_DESC:
			return new CDeclareOption(shared_from_this(), name); /* murx */
			break;
	}
	throw INVALID_LATEX_ITEM;
}

/**
 Registers a listener for style file events. An existing listener will be dropped.
 */
void CStyleFile::SetListener(CStyleFileListener* listener)
{
	m_Listener = listener;
}

/**
 Adds a command to the style file. Returns true, if command was inserted successfully or false,
 if not (e. g. command already exists)
 */
bool CStyleFile::AddCommand(SharedLaTeXCommandPtr& cmd)
{
	std::tr1::shared_ptr<CObject> dummy;

	if (!m_Commands.Lookup(cmd->ToString(), dummy))
	{
		m_Commands.SetAt(cmd->ToString(), cmd);

		if (m_Listener != NULL)
		{
			m_Listener->OnCommandFound(*cmd);
		}
		return true;
	}
	else
	{
		TRACE(_T("** Duplicate key: %s\n"), cmd->ToString());
	}
	return false;
}

/**
 Adds an option to the style file. Returns true, if command was inserted successfully or false,
 if not (e. g. command already exists)
 */
bool CStyleFile::AddOption(std::tr1::shared_ptr<CDeclareOption>& cmd)
{
	if (!CStyleFileContainer::ContainsString(&m_Options, cmd->GetName()))
	{
		m_Options.Add(cmd->GetName());
		return true;
	}

	return false;
}

/**
 Creates a LaTeX command (\newcommand) with the given parameters and sets
 current style file as parent. The parameter desc is an optional description.
 */
std::tr1::shared_ptr<CNewCommand> CStyleFile::AddCommand(const CString &name, int noOfParams, const CString &desc)
{
	std::tr1::shared_ptr<CNewCommand> nc (new CNewCommand(shared_from_this(), name, noOfParams));
	nc->SetDescription(desc);

	SharedLaTeXCommandPtr cmd = std::tr1::static_pointer_cast<CLaTeXCommand>(nc);

	if (AddCommand(cmd))
	{
		return nc;
	}
	else
	{
		nc.reset();
	}

	return nc;
}

/**
 Adds an option to this style file.
 */
bool CStyleFile::AddOption(const CString &name, const CString &/*desc*/)
{
	std::tr1::shared_ptr<CDeclareOption> d(new CDeclareOption(shared_from_this(), name));

	if (AddOption(d))
	{
		return true;
	}
	else
	{
	}

	return false;
}

/**
 Creates a LaTeX environment (\newenvironment) with the given parameters and sets
 current style file as parent. The parameter desc is an optional description.
 */

std::tr1::shared_ptr<CNewEnvironment> CStyleFile::AddEnvironment(const CString &name, int noOfParams, const CString &desc)
{
	std::tr1::shared_ptr<CNewEnvironment> ne(new CNewEnvironment(shared_from_this(), name, noOfParams));
	ne->SetDescription(desc);

	SharedLaTeXCommandPtr cmd = std::tr1::static_pointer_cast<CLaTeXCommand>(ne);

	if (AddCommand(cmd))
	{
		return ne;
	}
	else
	{
		ne.reset();
	}

	return ne;
}

/**
 Extracts a description string from a \ProvidesXXX command. Works similar as ExtractOptionCount
 */
int CStyleFile::ExtractDescription(const TCHAR *closePar, const TCHAR *openBr, const TCHAR *closeBr, CString &desc)
{
	if (closePar == NULL || openBr == NULL || closeBr == NULL)
	{
		desc.Empty();
		return -2; /* No desc available */
	}

	int d = closeBr - openBr - 1;
	if (d <= 0)
	{
		desc.Empty();
		return -4;
	}

	if (d >= MAX_DESC_LEN)
	{
		d = MAX_DESC_LEN - 1;
	}
	TCHAR *p = (TCHAR*)closePar;

	++p;
	while (p < openBr)
	{
		if (_istdigit(*p) || _istalpha(*p))
		{
			desc.Empty();
			return -3;
		}
		++p;
	}

	TCHAR buf[MAX_DESC_LEN];
	_tcsncpy(buf, openBr + 1, d);
	buf[d] = 0;
	desc = buf;

	return 0;
}

const CString CStyleFile::ToString() const
{
	return CString((m_IsClass ? _T("Class ") : _T("File ")) + m_Name);
}

void CStyleFile::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
	dc << m_Name;
}

void CStyleFile::GetPossibleItems(const CString& Partial, SharedObjectMap& Result)
{
	const int SearchLength = Partial.GetLength();
	ASSERT(SearchLength != 0);

	POSITION pos = m_Commands.GetStartPosition();
	while (pos != NULL)
	{
		CString key;
		std::tr1::shared_ptr<CObject> pObj;
		m_Commands.GetNextAssoc(pos, key, pObj);

		SharedLaTeXCommandPtr pLatexCmd = std::tr1::dynamic_pointer_cast<CLaTeXCommand>(pObj);
		if (!pLatexCmd) continue;

		if ((key.GetLength() >= SearchLength) && (key.Left(SearchLength).CompareNoCase(Partial) == 0))
		{
			Result[key] = pLatexCmd;

			/*
			if (!tmp->Lookup(key, (CObject*&)LatexCmd) || LatexCmd->GetParent()->IsDocClass()) {
			        tmp->SetAt(LatexCmd->ToLaTeX(), LatexCmd);
			}
			 */
		}
	}
}

const CString& CStyleFile::GetFileName() const
{
	return m_Filename;
}

const CString& CStyleFile::GetName() const
{
	return m_Name;
}

const CString& CStyleFile::GetDescription() const
{
	return m_Desc;
}

bool CStyleFile::IsDocClass() const
{
	return m_IsClass;
}

const SharedObjectMap*  CStyleFile::GetCommands() const
{
	return &m_Commands;
}

const CStringArray * CStyleFile::GetOptions() const
{
	return &m_Options;
}

const CStringArray * CStyleFile::GetRequiredPackages() const
{
	return &m_ReqPackages;
}