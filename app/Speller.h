#pragma once

#include <fstream>
#include <string>
#include <unordered_set>

#pragma push_macro("near")
#undef near
#include "hunspell.hxx"
#pragma pop_macro("near")

class Speller
{
	std::auto_ptr<Hunspell> spell_;

	// No copying
	Speller(const Speller&);
	Speller& operator=(const Speller&);

	typedef std::basic_string<TCHAR> StringType;
	typedef std::tr1::unordered_set<StringType> WordContainer;
	WordContainer ignored_words_;
	bool added_modified_, ignored_modified_;
	bool utf8_;

	WordContainer added_words_;

	std::string to_code_;

public:
	Speller(LPCTSTR affpath, LPCTSTR dpath, LPCTSTR key = 0);

	bool IsAddedModified() const;
	bool IsIgnoredModified() const;

	int Add(LPCTSTR w);
	int AddUTF8(const char* w);

	void AddDictionary(LPCTSTR dpath, LPCTSTR key = 0);

	int Remove(LPCTSTR word);
	int RemoveUTF8(const char* word);

	void Ignore(LPCTSTR word);
	void IgnoreUTF8(const char* word);

	bool Spell(LPCTSTR word);
	bool SpellUTF8(const char* word);

	int Suggest(LPCTSTR word, CStringArray& a);
	int SuggestUTF8(const char* word, CStringArray& a);

	void LoadIgnoredWords(LPCTSTR path);

	void SaveIgnoredWords(LPCTSTR path);

	void LoadPersonalDictionary(LPCTSTR path);
	void SavePersonalDictionary(LPCTSTR path);

private:
	static bool SaveWords( LPCTSTR path, const WordContainer& words );
	static bool LoadWords( LPCTSTR path, WordContainer& words );

	int DoAdd( LPCTSTR w );
	void DoLoadAdd(const StringType& s);
	void DoConvertFromUTF8(const char* word, std::vector<char>& buffer);
};