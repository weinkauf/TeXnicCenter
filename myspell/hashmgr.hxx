#ifndef _HASHMGR_HXX_
#define _HASHMGR_HXX_

#include "htypes.hxx"

class HashMgr
{
  int             hashsize;
  int             tablesize;
  struct hentry * tableptr;

public:
  HashMgr(const char * tpath);
  ~HashMgr();

  struct hentry * lookup(const char *);
  int hash(const char *);
  int save_tables(const char * tpath);

private:
  int load_tables(const char * tpath);
  int add_word(const char * word, int wl, const char * ap, int al);

};

#endif
