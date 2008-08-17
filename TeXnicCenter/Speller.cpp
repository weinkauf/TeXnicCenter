#include "stdafx.h"
#include "Speller.h"

#include <fstream>
#include <algorithm>
#include <functional>
#include <iterator>
#include <cstdlib>
#include <memory>


Speller::Speller( LPCTSTR affpath, LPCTSTR dpath, LPCTSTR key /*= 0*/ ) 
: added_modified_(false)
, ignored_modified_(false)
{
	USES_CONVERSION_EX;
	spell_.reset(new Hunspell(T2CA_EX(affpath,CP_UTF8),T2CA_EX(dpath,CP_UTF8),T2CA_EX(key,CP_UTF8)));
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

	USES_CONVERSION_EX;
	return spell_->add(T2CA_EX(w,CP_UTF8));
}

void Speller::AddDictionary( LPCTSTR dpath, LPCTSTR key /*= 0*/ )
{
	USES_CONVERSION_EX;
	spell_->add_dic(T2CA_EX(dpath,CP_UTF8),T2CA_EX(key,CP_UTF8));
}

int Speller::Remove( LPCTSTR word )
{
	USES_CONVERSION_EX;
	return RemoveUTF8(T2CA_EX(word,CP_UTF8));
}

void Speller::Ignore( LPCTSTR word )
{
	if (ignored_words_.insert(word).second)
		ignored_modified_ = true;
}

bool Speller::Spell( LPCTSTR word )
{
	USES_CONVERSION_EX;
	bool result = spell_->spell(T2CA_EX(word,CP_UTF8)) != 0;

	if (!result)
		result = added_words_.find(word) != added_words_.end() ||
		ignored_words_.find(word) != ignored_words_.end();

	return result;
}

int Speller::Suggest( LPCTSTR word, CStringArray& a )
{
	USES_CONVERSION_EX;
	return SuggestUTF8(T2CA_EX(word,CP_UTF8),a);
}

int Speller::SuggestUTF8( const char* word, CStringArray& a )
{
	USES_CONVERSION_EX;
	a.RemoveAll();

	char** l;
	int count = spell_->suggest(&l,word);

	if (count > 0)
	{
		for (int i = 0; i < count; ++i)
		{
			a.Add(A2CT_EX(l[i],CP_UTF8));
			std::free(l[i]);
		}

		std::free(l);
	}

	return count;
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
			bind(&Speller::DoAdd,this,_1));
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

void Speller::DoAdd( const StringType& s )
{
	USES_CONVERSION_EX;
	spell_->add(T2CA_EX(s.c_str(),CP_UTF8));
}

bool Speller::SpellUTF8( const char* word )
{
	bool result = spell_->spell(word) != 0;

	if (!result) {
		USES_CONVERSION_EX;
		LPCTSTR w = A2CT_EX(word,CP_UTF8);

		result = added_words_.find(w) != added_words_.end() ||
			ignored_words_.find(w) != ignored_words_.end();
	}

	return result;
}

void Speller::IgnoreUTF8( const char* word )
{
	USES_CONVERSION_EX;

	if (ignored_words_.insert(A2CT_EX(word,CP_UTF8)).second)
		ignored_modified_ = true;
}

int Speller::RemoveUTF8( const char* word )
{
	return spell_->remove(word);
}

int Speller::AddUTF8( const char* w )
{
	USES_CONVERSION_EX;

	if (added_words_.insert(A2CT_EX(w,CP_UTF8)).second)
		added_modified_ = true;

	return spell_->add(w);
}