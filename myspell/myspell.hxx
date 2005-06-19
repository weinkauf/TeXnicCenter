#include "hashmgr.hxx"
#include "baseaffix.hxx"
#include "affixmgr.hxx"
#include "suggestmgr.hxx"

#define NOCAP   0
#define INITCAP 1
#define ALLCAP  2
#define HUHCAP  3

#include "../CrysEditEx/Source/cedefs.h"

#ifndef _MYSPELLMGR_HXX_
#define _MYSPELLMGR_HXX_

class CRYSEDIT_CLASS_DECL MySpell
{
	AffixMgr	*pAMgr;
	HashMgr		*pHMgr;
	SuggestMgr	*pSMgr;

	int			bModified;
	int			bMainOnly;
	HashMgr		*pIgnoreHash;
	HashMgr		*pAddHash;
	char		*strPersonalDictionaryPath;

private:
	int captype(const char *) const;
	void mkinitcap(char *) const;
	void mkallcap(char *) const;
	void mkallsmall(char *) const;
	char* check(const char *) const;

public:
	MySpell(const char * affpath, const char * dpath);
	~MySpell();

	int suggest(char*** slst, const char * word) const;
	int spell(const char *) const;

	void add_word(const char *word);
	void ignore_word(const char *word);

	void set_personal_dictionary(const char *fileName);
	int open_personal_dictionary();
	int save_personal_dictionary(const char *fileName);

	int open_ignored_words(const char *fileName);
	int save_ignored_words(const char *fileName);

	inline void suggest_main(int suggestFromMainOnly)
	{
		bMainOnly = suggestFromMainOnly;
	}

	inline int ismodified() const 
	{ 
		return bModified; 
	}

	inline void MySpell::release_suggest(char *** slst) const
	{ 
		if (slst != NULL) {
			pSMgr->release_suggest(*slst);
			*slst = NULL;
		}
	}

};

#endif
