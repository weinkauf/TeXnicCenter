#include "license.readme"


#include <cctype>
#include <cstring>
#include <cstdlib>
#include <cstdio>

#include "stdafx.h"
#include "affix.hxx"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

Prefix::Prefix(AffixMgr* pmgr, unsigned char a, int num, affentry* dp)
{
	pmyMgr = pmgr;
	flag = a;
	first = dp;
	numents = num;
}


Prefix::~Prefix()
{
	affentry *ptrAff = first;
	for (int i = 0; i < numents; ++i) {
		delete [] ptrAff->appnd;
		delete [] ptrAff->strip;
		++ptrAff;
	}
	delete [] first;
}


// check if this prefix is present
struct hentry * Prefix::check(HashMgr *pHMgr, const char * word, int len) const
{
	struct affentry *   pp;	// pointer to current prefix affentry
	int			cond;	// condition number being examined
	int	                tmpl;   // length of tmpword
	struct hentry *     he;     // hash entry of root word or NULL
	unsigned char *	cp;		
	int			i;
	char	        tmpword[MAXWORDLEN];

	for (pp = first, i = numents; i > 0; pp++, i--)  {

		// a match if the prefix matches the beginning of the 
		// word, if the remaining root word has positive length
		// and if there are enough chars in new root word to meet 
		// the number of characters conditions to test

		tmpl = len - pp->appndl;
		if (tmpl > 0 &&  (pp->appndl == 0 || 
		strncmp ((pp->appnd), word, pp->appndl) == 0) &&  
		tmpl + pp->stripl >= pp->numconds) {

			// generate new root word by removing prefix and adding
			// back any characters that would have been stripped

			if (pp->stripl) strcpy (tmpword, (pp->strip));
			strcpy ((tmpword + pp->stripl), (word + pp->appndl));

			// now make sure all of the conditions on characters
			// are met.  Please see the appendix at the end of
			// this file for more info on exactly what is being
			// tested

			cp = (unsigned char *)tmpword;
			for (cond = 0;  cond < pp->numconds;  cond++) {
			if ((pp->conds[*cp++] & (1 << cond)) == 0) break;
			}

			// if all conditions are met then check if resulting
			// root word in the dictionary

			if (cond >= pp->numconds) {
				tmpl += pp->stripl;
				if ((he = pmyMgr->lookup(pHMgr, tmpword)) != NULL) {
				if (TESTAFF(he->astr, flag, he->alen)) return he;
				}

				// prefix matched but no root word was found 
				// if XPRODUCT is allowed, try again but now 
				// ross checked combined with a suffix

				if (pp->cpflag & XPRODUCT) {
					he = pmyMgr->cross_check(pHMgr, tmpword, tmpl, XPRODUCT, this);
					if (he) return he;
				}
			}
		}
	}
	return NULL;
}


Suffix::Suffix(AffixMgr * pmgr, unsigned char a, int num, affentry* dp)
{
	pmyMgr = pmgr;
	flag = a;
	first = dp;
	numents = num;
}


Suffix::~Suffix()
{
	affentry *ptrAff = first;
	for (int i = 0; i < numents; ++i) {
		delete [] ptrAff->appnd;
		delete [] ptrAff->strip;
		++ptrAff;
	}
	delete [] first;
}


// see if this suffix is present in the word 
struct hentry * Suffix::check(HashMgr *pHMgr, const char * word, int len, int optflags, Prefix* ppfx) const
{
	struct affentry	*sp;	         // suffix entry pointer
	int				tmpl;		 // length of tmpword 
	int				cond;		 // condition beng examined
	struct hentry	*he;              // hash entry pointer
	unsigned char	*cp;
	int				i;
	char			tmpword[MAXWORDLEN];


	for (sp = first, i = numents; i > 0; sp++, i--) {

		// if this suffix is being cross checked with a prefix
		// but it does not support cross products skip it

		if ((optflags & XPRODUCT) != 0 &&  (sp->cpflag & XPRODUCT) == 0)
		continue;

		// a match if the suffix matches the end of the 
		// word, if the remaining root word has positive length
		// and if there are enough chars in new root word to meet 
		// the number of characters conditions to test

		tmpl = len - sp->appndl;
		if (tmpl > 0  &&  (sp->appndl == 0
		||  strcmp ((sp->appnd), (word + tmpl)) == 0)
		&&  tmpl + sp->stripl >= sp->numconds) {

			// generate new root word by removing suffix and adding
			// back any characters that would have been stripped or
			// or null terminating the shorter string

			strcpy (tmpword, word);
			cp = (unsigned char *)(tmpword + tmpl);
			if (sp->stripl) {
			strcpy ((char *)cp, (sp->strip));
			tmpl += sp->stripl;
			cp = (unsigned char *)(tmpword + tmpl);
			} else *cp = '\0';

			// now make sure all of the conditions on characters
			// are met.  Please see the appendix at the end of
			// this file for more info on exactly what is being
			// tested

			for (cond = sp->numconds;  --cond >= 0; ) {
			if ((sp->conds[*--cp] & (1 << cond)) == 0) break;
			}

			// if all conditions are met then check if resulting
			// root word in the dictionary

			if (cond < 0) {
				if ((he = pmyMgr->lookup(pHMgr, tmpword)) != NULL) {
					if (TESTAFF(he->astr,flag,he->alen) && 
					((optflags & XPRODUCT) == 0 || 
					TESTAFF(he->astr,ppfx->flag,he->alen))) return he;
				}  
			}
		}
	}
	return NULL;
}


#if 0

Appendix:  Understanding Affix Code


An affix is either a  prefix or a suffix attached to root words to make 
other words.

Basically a Prefix or a Suffix is implemented as a sequence of
affentry structures which store information about the prefix
or suffix along with supporting routines to check if a word
has a particular prefix or suffix or a combination.

The structure affentry is defined as follows:

struct affentry
{
   char * strip;          // string to strip before adding affix
   char * appnd;          // the affix string to add
   short  stripl;         // length of the strip string
   short  appndl;         // length of the affix string
   short  numconds;       // the number of conditions that must be met
   short  cpflag;         // crossproduct flag (combine both a prefix and a suffix) 
   char   conds[SETSIZE]; // array which encodes the conditions to be met
};


Here is a suffix borrowed from the en_US.aff file.  This file 
is whitespace delimited.

SFX D Y 4
SFX D   0     e          d
SFX D   y     ied        [^aeiou]y
SFX D   0     ed         [^ey]
SFX D   0     ed         [aeiou]y

This information can be interpreted as follows:

In the first line has 4 fields:

Field
-----
1     SFX - indicates this is a suffix
2     D   - is the name of the character which represents this suffix
3     Y   - indicates it can be combined with prefixes (cross product)
4     4   - indicates that sequence of 4 affentry structures are needed to
               properly store the affix information

The remaining lines describe the unique information for the 4 affentry 
structures that make up this affix.  Each line can be interpreted
as follows: (note fields 1 and 2 are as a check against line 1 info)

Field
-----
1     SFX         - indicates this is a suffix
2     D           - is the name of the character which represents this affix
3     y           - the string of chars to strip off before adding affix
                         (a 0 here indicates the NULL string)
4     ied         - the string of affix characters to add
5     [^aeiou]y   - the conditions which must be met before the affix
                    can be applied

Field 5 is interesting.  Since this is a suffix, field 5 tells us that
there are 2 conditions that must be met.  The first condition is that 
the next to the last character in the word must *NOT* be any of the 
following "a", "e", "i", "o" or "u".  The second condition is that
the last character of the word must end in "y".

So how can we encode this information concisely and be able to 
test for both conditions in a fast manner?  The answer is found
but studying the wonderful ispell code of Geoff Kuenning, et.al. 
(now available under a normal BSD license).

If we set up a conds array of 256 bytes indexed (0 to 255) and access it
using a character (cast to an unsigned char) of a string, we have 8 bits
of information we can store about that character.  Specifically we
could use each bit to say if that character is allowed in any of the 
last (or first for prefixes) 8 characters of the word.

Basically, each character at one end of the word (up to the number 
of conditions) is used to index into the conds array and the resulting 
value found there says whether the that character is valid for a 
specific character position in the word.  

For prefixes, it does this by setting bit 0 if that char is valid 
in the first position, bit 1 if valid in the second position, and so on. 

If a bit is not set, then that char is not valid for that postion in the
word.

If working with suffixes bit 0 is used for the character closest 
to the front, bit 1 for the next character towards the end, ..., 
with bit numconds-1 representing the last char at the end of the string. 

Note: since entries in the conds[] are 8 bits, only 8 conditions 
(read that only 8 character positions) can be examined at one
end of a word (the beginning for prefixes and the end for suffixes.

So to make this clearer, lets encode the conds array values for the 
first two affentries for the suffix D described earlier.


  For the first affentry:    
     numconds = 1             (only examine the last character)

     conds['e'] =  (1 << 0)   (the word must end in an E)
     all others are all 0

  For the second affentry:
     numconds = 2             (only examine the last two characters)     

     conds[X] = conds[X] | (1 << 0)     (aeiou are not allowed)
         where X is all characters *but* a, e, i, o, or u
         

     conds['y'] = (1 << 1)     (the last char must be a y)
     all other bits for all other entries in the conds array are zero


#endif

