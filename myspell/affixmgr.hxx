#ifndef _AFFIXMGR_HXX_
#define _AFFIXMGR_HXX_

#include "atypes.hxx"
#include "baseaffix.hxx"
#include "hashmgr.hxx"

class AffixMgr
{
protected:
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
  struct hentry * affix_check(const char * word, int len) const;
  struct hentry * lookup(const char * word) const;
  struct hentry * cross_check(const char * word, int len, int sfxopts, const Affix* ppfx) const;
  char * get_encoding() const;
  char * get_try_string() const;
            
private:
  int  parse_file(const char * affpath);
  void encodeit(struct affentry * ptr, char * cs) const;
  void strtolower(char * cs) const;

};

#endif
