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

	inline void MySpell::release_suggest(char ** slst) const
	{ 
	  if ( pSMgr )
		  pSMgr->release_suggest(slst);
	}
};

#endif
