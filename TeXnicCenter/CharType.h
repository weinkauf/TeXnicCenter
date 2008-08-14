#pragma once

#include <stdexcept>

template<class Ch>
class CharTraitsBase;

template<>
class CharTraitsBase<char>
{
public:
	static bool GetCharType(char ch, DWORD type, WORD& flags, LCID locale = LOCALE_USER_DEFAULT)
	{
		return ::GetStringTypeExA(locale,type,&ch,1,&flags) != 0;
	}
};

template<>
class CharTraitsBase<wchar_t>
{
public:
	static bool GetCharType(wchar_t ch, DWORD type, WORD& flags, LCID locale = LOCALE_USER_DEFAULT)
	{
		return ::GetStringTypeExW(locale,type,&ch,1,&flags) != 0;
	}
};

template<class Ch>
class CharTraits :
			public CharTraitsBase<Ch>
{
public:
	typedef Ch CharType;

	static bool MatchCharType(CharType ch, DWORD type, WORD flags, LCID locale = LOCALE_USER_DEFAULT)
	{
		WORD d;
		bool result;

		if (GetCharType(ch,type,d,locale))
			result = (d & flags) != 0;
		else
			result = false;

		return result;
	}

	static bool IsAlnum(CharType ch, LCID lcid = LOCALE_USER_DEFAULT)
	{
		return MatchCharType(ch,CT_CTYPE1,C1_ALPHA|C1_DIGIT,lcid);
	}

	static bool IsAlpha(CharType ch, LCID lcid = LOCALE_USER_DEFAULT)
	{
		return MatchCharType(ch,CT_CTYPE1,C1_ALPHA,lcid);
	}

	static bool IsDigit(CharType ch, LCID lcid = LOCALE_USER_DEFAULT)
	{
		return MatchCharType(ch,CT_CTYPE1,C1_DIGIT,lcid);
	}

	static bool IsPrint(CharType ch, LCID lcid = LOCALE_USER_DEFAULT)
	{
		return MatchCharType(ch,CT_CTYPE1,C1_UPPER|C1_LOWER|C1_ALPHA|C1_DIGIT|C1_BLANK|C1_PUNCT|C1_SPACE|C1_XDIGIT,lcid);
	}

	static bool IsPunct(CharType ch, LCID lcid = LOCALE_USER_DEFAULT)
	{
		return MatchCharType(ch,CT_CTYPE1,C1_PUNCT,lcid);
	}

	static bool IsSpace(CharType ch, LCID lcid = LOCALE_USER_DEFAULT)
	{
		return MatchCharType(ch,CT_CTYPE1,C1_SPACE,lcid);
	}

	static bool IsXdigit(CharType ch, LCID lcid = LOCALE_USER_DEFAULT)
	{
		return MatchCharType(ch,CT_CTYPE1,C1_XDIGIT,lcid);
	}

	static bool IsBlank(CharType ch, LCID lcid = LOCALE_USER_DEFAULT)
	{
		return MatchCharType(ch,CT_CTYPE1,C1_BLANK,lcid);
	}

	static bool IsCntrl(CharType ch, LCID lcid = LOCALE_USER_DEFAULT)
	{
		return MatchCharType(ch,CT_CTYPE1,C1_CNTRL,lcid);
	}

	static bool IsUpper(CharType ch, LCID lcid = LOCALE_USER_DEFAULT)
	{
		return MatchCharType(ch,CT_CTYPE1,C1_UPPER,lcid);
	}

	static bool IsLower(CharType ch, LCID lcid = LOCALE_USER_DEFAULT)
	{
		return MatchCharType(ch,CT_CTYPE1,C1_LOWER,lcid);
	}
};

typedef CharTraits<char> CharTraitsA;
typedef CharTraits<wchar_t> CharTraitsW;
typedef CharTraits<TCHAR> CharTraitsT;

class Win32LocaleError :
			public std::runtime_error
{
public:
	explicit Win32LocaleError(const std::string& message = "")
			: runtime_error(message)
	{
	}
};

//template<class Ch, class Tr = CharTraits<Ch> >
class Win32Locale
{
	//typedef typename Tr::CharType CharType;
	//typedef Tr TraitsType;

	LCID lcid_;

	void SetLCID(LCID lcid)
	{
		if (!::IsValidLocale(lcid,LCID_SUPPORTED))
			throw std::invalid_argument("Invalid LCID");

		lcid_ = lcid;
	}

public:
	explicit Win32Locale(LCID lcid = LOCALE_USER_DEFAULT)
	{
		SetLCID(lcid);
	}

	LCID GetLCID() const
	{
		return lcid_;
	}

	int GetInfo(LCTYPE type, LPTSTR data, int cch) const
	{
		return ::GetLocaleInfo(lcid_,type,data,cch);
	}

	bool GetInfo(LCTYPE type, CString& text) const
	{
		const int cch = GetInfo(type,0,0);

		if (cch > 0)
		{
			GetInfo(type,text.GetBuffer(cch),cch);
			text.ReleaseBuffer();
		}

		return cch > 0;
	}

	const CString GetSLanguage() const
	{
		CString text;

		if (!GetInfo(LOCALE_SLANGUAGE,text))
			throw Win32LocaleError("Failed to retrieve the SLanguage");

		return text;
	}

	const CString GetSNativeLangName() const
	{
		CString text;

		if (!GetInfo(LOCALE_SNATIVELANGNAME,text))
			throw Win32LocaleError("Failed to retrieve the SNativeLangName");

		return text;
	}
};