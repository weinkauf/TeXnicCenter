#ifndef _AFFIXMGR_HXX_
#define _AFFIXMGR_HXX_

#include "atypes.hxx"
#include "baseaffix.hxx"
#include "hashmgr.hxx"

class AffixMgr
{

  int                 numpfx;
  int                 numsfx;
  Affix*              pTable[MAXAFFIXES];
  Affix*              sTable[MAXAFFIXES];
  HashMgr *           pHMgr;
  char *              trystring;
  char *              encoding;

public:
 
  AffixMgr(const char * affpath, HashMgr * ptr);
  ~AffixMgr();
  struct hentry * affix_check(const char * word, int len);
  struct hentry * lookup(const char * word);
  struct hentry * cross_check(const char * word, int len, int sfxopts, Affix* ppfx);
  char * get_encoding();
  char * get_try_string();
            
private:
  int  parse_file(const char * affpath);
  void encodeit(struct affentry * ptr, char * cs);
  void strtolower(char * cs);

};

#endif
