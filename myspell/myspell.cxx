#include "license.readme"

#include <cctype>
#include <cstring>
#include <cstdlib>
#include <cstdio>

#include "stdafx.h"
#include "myspell.hxx"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

MySpell::MySpell(const char * affpath, const char * dpath)
{
	/* create the ignore and add hashes */
	pIgnoreHash = new HashMgr;
	pAddHash = new HashMgr;

	/* first set up the main dict hash manager */
	pHMgr = new HashMgr(dpath);

	/* next set up the affix manager */
	pAMgr = new AffixMgr(affpath);

	/* get the preferred try string from the Affix Manager for that dictionary */
	char * try_string = pAMgr->get_try_string();

	/* and finally set up the suggestion manager */
	pSMgr = new SuggestMgr(try_string, 10, pAMgr);

	delete [] try_string;

	bModified = 0;
	bMainOnly = 1;
	strPersonalDictionaryPath = NULL;
}


MySpell::~MySpell()
{
	delete pSMgr;
	delete pAMgr;
	delete pHMgr;
	delete pIgnoreHash;
	delete pAddHash;
	delete [] strPersonalDictionaryPath;
}


int MySpell::captype(const char * word) const
{
	int ncap = 0;
	int nc = 0;
	const char * p = word;
	while (*p != '\0') {
		nc++;
		if (isupper(*p)) ncap++;
		p++;
	}
	if (ncap == 0) return NOCAP;
	if ((ncap == 1) && isupper(*word)) return INITCAP;
	if (ncap == nc) return ALLCAP;
	return HUHCAP;
}


void MySpell::mkallcap(char * p) const
{
	while (*p != '\0') { 
		*p = toupper((unsigned char) *p);
		p++;
	}
}


void MySpell::mkallsmall(char * p) const
{
	while (*p != '\0') {
		*p = tolower((unsigned char) *p);
		p++;
	}
}


int MySpell::spell(const char * word) const
{
	char * rv=NULL;
	char wspace[MAXWORDLEN+1];
	int wl = strlen(word);
	if (wl > MAXWORDLEN) return 0;
	int ct = captype(word);

	switch(ct) {
		case HUHCAP:
		case NOCAP:
			rv = check(word); 
			break;

		case ALLCAP:
		case INITCAP:
			memcpy(wspace,word,(wl+1));
			mkallsmall(wspace);
			rv = check(wspace);
			if (rv == NULL) rv = check(word);
			break; 

	}
	if (rv) return 1;
	return 0;
}


char * MySpell::check(const char * word) const
{
	int nLength = strlen(word);
	struct hentry * he = NULL;
	if (pHMgr)
		he = pHMgr->lookup (word);

	if ((he == NULL) && pIgnoreHash)
		he = pIgnoreHash->lookup(word);

	if ((he == NULL) && pAddHash)
		he = pAddHash->lookup(word);

	if ((he == NULL) && pAMgr && pHMgr) {
		// try stripping off affixes */
		he = pAMgr->affix_check(pHMgr, word, nLength);
	}

	if ((he == NULL) && pAMgr && pAddHash && !bMainOnly)
		he = pAMgr->affix_check(pAddHash, word, nLength);

	if (he) return he->word;
	return NULL;
}


int MySpell::suggest(char*** slst, const char * word) const
{
	char wspace[MAXWORDLEN+1];
	if (! pSMgr) return 0;
	int wl = strlen(word);
	int ct = captype(word);
	int ns = 0;
	int j;
	switch(ct) {
		case NOCAP:
			ns = pSMgr->suggest(pHMgr, -1, slst, word);
			if ( !bMainOnly )
				ns = pSMgr->suggest(pAddHash, ns, slst, word);
			break;

		case INITCAP:
			memcpy(wspace,word,(wl+1));
			mkallsmall(wspace);
			ns = pSMgr->suggest(pHMgr, -1, slst, wspace);
			if ( !bMainOnly )
				ns = pSMgr->suggest(pAddHash, ns, slst, word);
			for (j=0; j < ns; j++)
				mkinitcap((*slst)[j]);
			break;

		case HUHCAP:
			memcpy(wspace,word,(wl+1));
			mkallsmall(wspace);
			ns = pSMgr->suggest(pHMgr, -1, slst, word);
			ns = pSMgr->suggest(pHMgr, ns,slst, wspace);
			if ( !bMainOnly ) {
				ns = pSMgr->suggest(pAddHash, ns, slst, word);
				ns = pSMgr->suggest(pAddHash, ns, slst, wspace);
			}
			break;

		case ALLCAP:
			memcpy(wspace,word,(wl+1));
			mkallsmall(wspace);
			ns = pSMgr->suggest(pHMgr, -1, slst, wspace);
			if ( !bMainOnly )
				ns = pSMgr->suggest(pAddHash, ns, slst, word);
			for (j=0; j < ns; j++)
				mkallcap((*slst)[j]);
			break;
	}
	if (ns < 1)
		release_suggest(slst);
	return ns;
}


void MySpell::mkinitcap(char * p) const
{
	if (*p != '\0') *p = toupper((unsigned char)*p);
}


void MySpell::ignore_word(const char *word)
{
	if (pIgnoreHash)
		pIgnoreHash->add_word(word);
}


void MySpell::add_word(const char *word)
{
	if (pAddHash) {
		pAddHash->add_word(word);
		bModified = true;
	}
}


void MySpell::set_personal_dictionary(const char *fileName)
{
	delete [] strPersonalDictionaryPath;
	strPersonalDictionaryPath = strdup(fileName);
}


int MySpell::open_personal_dictionary()
{
	delete pAddHash;
	if (strPersonalDictionaryPath == NULL) {
		// Create an empty dictionary
		pAddHash = new HashMgr;
	} else {
		try {
			pAddHash = new HashMgr(strPersonalDictionaryPath);
		} catch (...) {
			return -1;
		}
	}
	return 0;
}


int MySpell::save_personal_dictionary(const char *fileName)
{
	int retValue = -1;
	if (fileName && pAddHash) {
		retValue = pAddHash->save_tables(fileName);
		if (retValue == 0)
			bModified = false;
	}
	return retValue;
}

