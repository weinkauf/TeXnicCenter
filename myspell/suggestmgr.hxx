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

	int suggest(char*** slst, const char * word) const;
	int check(const char *, int) const;
	inline void release_suggest(char ** slst)  const
	{
		if (slst) {
			for (int i = 0; i < maxSug; ++i)
				delete [] slst[i];
			delete [] slst;
		}
	}

private:
	int forgotchar(char **, const char *, int) const;
	int swapchar(char **, const char *, int) const;
	int extrachar(char **, const char *, int) const;
	int badchar(char **, const char *, int) const;
	int twowords(char **, const char *, int) const;
};

#endif

