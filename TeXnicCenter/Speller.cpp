#include "stdafx.h"
#include "Speller.h"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <functional>
#include <iterator>
#include <cstdlib>
#include <memory>

#include "EncodingConverter.h"


Speller::Speller( LPCTSTR affpath, LPCTSTR dpath, LPCTSTR key /*= 0*/ ) 
: added_modified_(false)
, ignored_modified_(false)
{
	std::ifstream in(affpath);
	std::string line;

	bool stop = false;

	// Search for SET <encoding>
	while (!stop) {
		if (getline(in,line)) {
			if (!line.empty() && line[0] != '#' && line.size() > 5) {
				const std::string head("SET");
				std::string::size_type pos1 = line.find(head);

				if (pos1 != std::string::npos) {
					pos1 += head.size();

					std::string::size_type pos2 = line.find_first_not_of(" \t",pos1);

					if (pos2 == std::string::npos)
						pos2 = pos1; // Something like SETUTF-8 occurred, without a space in between

					std::string::size_type pos3 = line.find_first_of(" \t",pos2);

					to_code_.assign(line,pos2,pos3 - pos2);
					stop = true;
				}
			}
		}
		else
			stop = true;
	}

	in.close();

	if (!to_code_.empty()) {
		// Make sure the encoding can be used with iconv
		if (!IsEncodingAvailable(to_code_.c_str())) {
#ifdef _DEBUG
			const std::string old_to_code = to_code_;
#endif
			// Fall back to user's ANSI code page
			std::ostringstream oss;
			oss << "CP" << ::GetACP();
			to_code_ = oss.str();

#ifdef _DEBUG
			USES_CONVERSION;
#endif
			TRACE2("Encoding scheme %s not found, falling back to %s",A2CT(old_to_code.c_str()),
				A2CT(to_code_.c_str()));
		}
	}
	
	utf8_ = to_code_ == "UTF-8";

	USES_CONVERSION;
	spell_.reset(new Hunspell(T2CA(affpath),T2CA(dpath),T2CA(key)));
}

bool Speller::IsAddedModified() const
{
	return added_modified_;
}

bool Speller::IsIgnoredModified() const
{
	return ignored_modified_;
}

int Speller::Add( LPCTSTR w )
{
	if (added_words_.insert(w).second)
		added_modified_ = true;

	return DoAdd(w);
}

void Speller::AddDictionary( LPCTSTR dpath, LPCTSTR key /*= 0*/ )
{
	USES_CONVERSION;
	spell_->add_dic(T2CA(dpath),T2CA(key));
}

int Speller::Remove( LPCTSTR word )
{
	std::vector<char> buffer;

#ifdef UNICODE
	UTF16toUTF8(word,std::wcslen(word),buffer);
#else
	ANSItoUTF8(word,std::strlen(word),buffer);
#endif

	buffer.push_back(0);

	return RemoveUTF8(&buffer[0]);
}

void Speller::Ignore( LPCTSTR word )
{
	if (ignored_words_.insert(word).second)
		ignored_modified_ = true;
}

bool Speller::Spell( LPCTSTR word )
{
	std::vector<char> buffer;

#ifdef UNICODE
	UTF16toUTF8(word,std::wcslen(word),buffer);
#else
	ANSItoUTF8(word,std::strlen(word),buffer);
#endif

	buffer.push_back(0);

	bool result = SpellUTF8(&buffer[0]) != 0;

	if (!result)
		result = added_words_.find(word) != added_words_.end() ||
		ignored_words_.find(word) != ignored_words_.end();

	return result;
}

int Speller::Suggest( LPCTSTR word, CStringArray& a )
{
	std::vector<char> buffer;

#ifdef UNICODE
	UTF16toUTF8(word,std::wcslen(word),buffer);
#else
	ANSItoUTF8(word,std::strlen(word),buffer);
#endif

	buffer.push_back(0);

	return SuggestUTF8(&buffer[0],a);
}

void Speller::DoConvertFromUTF8(const char* word, std::vector<char>& buffer)
{
	FromUTF8(to_code_.c_str(),word,std::strlen(word),buffer);
	buffer.push_back(0); // Terminating null
}

int Speller::SuggestUTF8( const char* word, CStringArray& a )
{
	a.RemoveAll();

	char** l;
	int count;
	
	if (!utf8_) {
		std::vector<char> buffer;
		DoConvertFromUTF8(word,buffer);
		
		count = spell_->suggest(&l,&buffer[0]);
	}
	else
		count = spell_->suggest(&l,word);

	std::vector<wchar_t> buffer;

	if (count > 0)
	{
		for (int i = 0; i < count; ++i)
		{
			if (utf8_)
				UTF8toUTF16(l[i],std::strlen(l[i]),buffer);
			else
				ToUTF16(to_code_.c_str(),l[i],std::strlen(l[i]),buffer);

			if (!buffer.empty())
				a.Add(CStringW(&buffer[0],buffer.size()));

			std::free(l[i]);

			buffer.clear();
		}

		std::free(l);
	}

	return a.GetSize();
}

void Speller::LoadIgnoredWords( LPCTSTR path )
{
	std::basic_ifstream<TCHAR> in(path);
	typedef std::istream_iterator<StringType,StringType::value_type,StringType::traits_type> iterator;

	if (in)
		std::copy(iterator(in),iterator(),std::inserter(ignored_words_,ignored_words_.end()));
}

void Speller::SaveIgnoredWords( LPCTSTR path )
{
	std::basic_ofstream<TCHAR> out(path);
	typedef std::ostream_iterator<StringType,StringType::value_type,StringType::traits_type> iterator;

	if (out)
	{
		std::copy(ignored_words_.begin(),ignored_words_.end(),
			iterator(out,_T("\n")));
		ignored_modified_ = false;
	}
}

void Speller::LoadPersonalDictionary( LPCTSTR path )
{
	std::basic_ifstream<TCHAR> in(path);
	typedef std::istream_iterator<StringType,StringType::value_type,StringType::traits_type> iterator;

	if (in)
	{
		std::copy(iterator(in),iterator(),
			std::inserter(added_words_,added_words_.end()));

		using std::tr1::bind;
		using namespace std::tr1::placeholders;

		std::for_each(added_words_.begin(),added_words_.end(),
			bind(&Speller::DoLoadAdd,this,_1));
	}
}

void Speller::SavePersonalDictionary( LPCTSTR path )
{
	std::basic_ofstream<TCHAR> out(path);
	typedef std::ostream_iterator<StringType,StringType::value_type,StringType::traits_type> iterator;

	if (out)
	{
		std::copy(added_words_.begin(),added_words_.end(),
			iterator(out,_T("\n")));
		added_modified_ = false;
	}
}

void Speller::DoLoadAdd( const StringType& s )
{
	DoAdd(s.c_str());
}

bool Speller::SpellUTF8( const char* word )
{
	bool result;

	if (!utf8_) {
		std::vector<char> buffer;
		FromUTF8(to_code_.c_str(),word,std::strlen(word),buffer);

		buffer.push_back(0); // Terminating null

		result = spell_->spell(&buffer[0]) != 0;
	}
	else
		result = spell_->spell(word) != 0;

	if (!result) {
#ifdef UNICODE
		std::vector<wchar_t> buffer;
		UTF8toUTF16(word,std::strlen(word),buffer);
#else
		std::vector<char> buffer;
		UTF8toANSI(word,std::strlen(word),buffer);
#endif

		if (!buffer.empty()) {
			StringType w(&buffer[0],buffer.size());
			
			result = added_words_.find(w) != added_words_.end() ||
				ignored_words_.find(w) != ignored_words_.end();
		}
	}

	return result;
}

void Speller::IgnoreUTF8( const char* word )
{
#ifdef UNICODE
	std::vector<wchar_t> buffer;
	UTF8toUTF16(word,std::strlen(word),buffer);
#else
	std::vector<char> buffer;
	UTF8toANSI(word,std::strlen(word),buffer);
#endif

	buffer.push_back(0);

	if (ignored_words_.insert(&buffer[0]).second)
		ignored_modified_ = true;
}

int Speller::RemoveUTF8( const char* word )
{
	int result;

	if (!utf8_) {
		std::vector<char> buffer;
		DoConvertFromUTF8(word,buffer);
		
		result = spell_->remove(&buffer[0]);
	}
	else
		result = spell_->remove(word);

	return result;
}

int Speller::AddUTF8( const char* word )
{
	int result;

	if (!utf8_) {
		std::vector<char> buffer;
		DoConvertFromUTF8(word,buffer);

		result = spell_->add(&buffer[0]);
	}
	else
		result = spell_->add(word);

#ifdef UNICODE
	std::vector<wchar_t> buffer;
	UTF8toUTF16(word,std::strlen(word),buffer);
#else
	std::vector<wchar_t> buffer;
	UTF8toANSI(word,std::strlen(word),buffer);
#endif

	buffer.push_back(0);

	if (added_words_.insert(&buffer[0]).second)
		added_modified_ = true;

	return result;
}

int Speller::DoAdd( LPCTSTR w )
{
	std::vector<char> buffer;

	if (utf8_) {
#ifdef UNICODE
		UTF16toUTF8(w,std::wcslen(w),buffer);
#else
		ANSItoUTF8(w,std::strlen(w),buffer);
#endif
	}
	else {
#ifdef UNICODE
		FromUTF16(to_code_.c_str(),w,std::wcslen(w),buffer);
#else
		FromANSI(to_code_.c_str(),w,std::strlen(w),buffer);
#endif
	}

	buffer.push_back(0);

	return spell_->add(&buffer[0]);
}