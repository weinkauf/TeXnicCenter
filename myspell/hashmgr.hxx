#ifndef _HASHMGR_HXX_
#define _HASHMGR_HXX_

#include "htypes.hxx"

class HashMgr
{
protected:
	int				hashsize;
	int				tablesize;
	struct hentry	*tableptr;

public:
	HashMgr(const char * tpath);
	~HashMgr();

	struct hentry * lookup(const char *) const;
	int hash(const char *) const;
	int save_tables(const char * tpath) const;

private:
	int load_tables(const char * tpath);
	int add_word(const char * word, int wl, const char * ap, int al);
};

#endif
