#ifndef _SUGGESTMGR_HXX_
#define _SUGGESTMGR_HXX_

#define MAXSWL 100

#include "atypes.hxx"
#include "baseaffix.hxx"
#include "affixmgr.hxx"

class SuggestMgr
{
	char		*ctry;
	int			ctryl;
	AffixMgr	*pAMgr;
	int			maxSug;

public:
	SuggestMgr(const char * tryme, int maxn, AffixMgr *aptr);
	~SuggestMgr();

	int suggest(char*** slst, const char * word);
	int check(const char *, int);
	inline void release_suggest(char ** slst)
	{
		if (slst) {
			for (int i = 0; i < maxSug; ++i)
				delete [] slst[i];
			delete [] slst;
		}
	}

private:
	int forgotchar(char **, const char *, int);
	int swapchar(char **, const char *, int);
	int extrachar(char **, const char *, int);
	int badchar(char **, const char *, int);
	int twowords(char **, const char *, int);
};

#endif

