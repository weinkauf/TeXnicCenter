#ifndef _SUGGESTMGR_HXX_
#define _SUGGESTMGR_HXX_

#define MAXSWL 100

#include "atypes.hxx"
#include "baseaffix.hxx"
#include "affixmgr.hxx"

class SuggestMgr
{
protected:
	char		*ctry;
	int			ctryl;
	AffixMgr	*pAMgr;
	int			maxSug;

public:
	SuggestMgr(const char * tryme, int maxn, AffixMgr *aptr);
	~SuggestMgr();

	int suggest(HashMgr *pHMgr, int nsug, char*** slst, const char * word) const;
	int check(HashMgr *pHMgr, const char *, int) const;
	inline void release_suggest(char ** slst)  const
	{
		if (slst) {
			for (int i = 0; i < maxSug; ++i)
				delete [] slst[i];
			delete [] slst;
		}
	}

private:
	int forgotchar(HashMgr *pHMgr, char **, const char *, int) const;
	int swapchar(HashMgr *pHMgr, char **, const char *, int) const;
	int extrachar(HashMgr *pHMgr, char **, const char *, int) const;
	int badchar(HashMgr *pHMgr, char **, const char *, int) const;
	int twowords(HashMgr *pHMgr, char **, const char *, int) const;
};

#endif

