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
  AffixMgr*       pAMgr;
  HashMgr*        pHMgr;
  SuggestMgr*     pSMgr;

public:
  MySpell(const char * affpath, const char * dpath);
  ~MySpell();

  int suggest(char*** slst, const char * word);
  int spell(const char *);

private:
   int    captype(const char *);
   void   mkinitcap(char *);
   void   mkallcap(char *);
   void   mkallsmall(char *);
   char * check(const char *);
};

#endif
