#include "license.readme"

#include <cstdlib>
#include <cctype>
#include <cstring>
#include <cstdio>

#include "stdafx.h"
#include "suggestmgr.hxx"


extern char * mystrdup(const char *);

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

SuggestMgr::SuggestMgr(const char * tryme, int maxn, AffixMgr * aptr)
{
	// register affix manager and check in string of chars to 
	// try when building candidate suggestions
	pAMgr = aptr;
	ctry = mystrdup(tryme);
	ctryl = 0;
	if (ctry)
		ctryl = strlen(ctry);
	maxSug = maxn;
}


SuggestMgr::~SuggestMgr()
{
	delete [] ctry;
}


// generate suggestions for a mispelled word
//    pass in address of array of char * pointers

int SuggestMgr::suggest(char*** slst, const char * word)
{
	int nsug = 0;
	char ** wlst = new char*[maxSug];
	memset(wlst,0, maxSug*sizeof(char*));

	if (wlst == NULL) return -1;

	// did we forget to add a char
	nsug = forgotchar(wlst, word, nsug);

	// did we swap the order of chars by mistake
	if ((nsug < maxSug) && (nsug > -1))
		nsug = swapchar(wlst, word, nsug);

	// did we add a char that should not be there
	if ((nsug < maxSug) && (nsug > -1))
		nsug = extrachar(wlst, word, nsug);

	// did we just hit the wrong key in place of a good char
	if ((nsug < maxSug) && (nsug > -1))
		nsug = badchar(wlst, word, nsug);

	// perhaps we forgot to hit space and two words ran together
	if ((nsug < maxSug) && (nsug > -1))
		nsug = twowords(wlst, word, nsug);

	if (nsug < 1) {
		for (int i=0;i<maxSug; i++)
			delete [] wlst[i];
		delete [] wlst;
		return -1;
	}

	*slst = wlst;
	return nsug;
}


// error is wrong char in place of correct one
int SuggestMgr::badchar(char ** wlst, const char * word, int ns)
{
	char	tmpc;
	char	candidate[MAXSWL];

	int wl = strlen(word);
	int cwrd;
	strcpy (candidate, word);

	// swap out each char one by one and try all the tryme
	// chars in its place to see if that makes a good word
	for (int i=0; i < wl; i++) {
		tmpc = candidate[i];
		for (int j=0; j < ctryl; j++) {
			if (ctry[j] == tmpc) continue;
			candidate[i] = ctry[j];
			cwrd = 1;
			for (int k=0; k < ns; k++)
				if (strcmp(candidate,wlst[k]) == 0) cwrd = 0;
			if ((cwrd) && check(candidate,wl)) {
				if (ns < maxSug) {
					wlst[ns] = mystrdup(candidate);
					if (wlst[ns] == NULL) return -1;
					ns++;
				} else return ns;
			}
			candidate[i] = tmpc;
		}
	}
	return ns;
}


// error is word has an extra letter it does not need 
int SuggestMgr::extrachar(char** wlst, const char * word, int ns)
{
	char candidate[MAXSWL];
	const char * p;
	char *r;
	int cwrd;

	int wl = strlen(word);
	if (wl < 2) return 0;

	// try omitting one char of word at a time
	strcpy (candidate, word + 1);
	for (p = word, r = candidate;  *p != 0;  ) {
		cwrd = 1;
		for (int k=0; k < ns; k++)
			if (strcmp(candidate,wlst[k]) == 0) cwrd = 0;
		if ((cwrd) && check(candidate,wl-1)) {
			if (ns < maxSug) {
				wlst[ns] = mystrdup(candidate);
				if (wlst[ns] == NULL) return -1;
				ns++;
			} else return ns; 
		}
		*r++ = *p++;
	}
	return ns;
}


// error is mising a letter it needs
int SuggestMgr::forgotchar(char ** wlst, const char * word, int ns)
{
	char candidate[MAXSWL];
	const char *p;
	char *q;
	int cwrd;

	int wl = strlen(word);

	// try inserting a tryme character before every letter
	strcpy(candidate + 1, word);
	for (p = word, q = candidate;  *p != 0;  )  {
		for (int i = 0;  i < ctryl;  i++) {
			*q = ctry[i];
			cwrd = 1;
			for (int k=0; k < ns; k++)
				if (strcmp(candidate,wlst[k]) == 0) cwrd = 0;
			if ((cwrd) && check(candidate,wl+1)) {
				if (ns < maxSug) {
					wlst[ns] = mystrdup(candidate);
					if (wlst[ns] == NULL) return -1;
					ns++;
				} else return ns; 
			}
		}
		*q++ = *p++;
	}

	// now try adding one to end */
	for (int i = 0;  i < ctryl;  i++) {
		*q = ctry[i];
		cwrd = 1;
		for (int k=0; k < ns; k++)
			if (strcmp(candidate,wlst[k]) == 0) cwrd = 0;
		if ((cwrd) && check(candidate,wl+1)) {
			if (ns < maxSug) {
				wlst[ns] = mystrdup(candidate);
				if (wlst[ns] == NULL) return -1;
				ns++;
			} else return ns;
		}
	}
	return ns;
}


/* error is should have been two words */
int SuggestMgr::twowords(char ** wlst, const char * word, int ns)
{
	char candidate[MAXSWL];
	char * p;

	int wl=strlen(word);
	if (wl < 3) return ns;
	strcpy(candidate + 1, word);

	// split the string into two pieces after every char
	// if both pieces are good words make them a suggestion
	for (p = candidate + 1;  p[1] != '\0';  p++) {
		p[-1] = *p;
		*p = '\0';
		if (check(candidate,strlen(candidate))) {
			if (check((p+1),strlen(p+1))) {
				*p = ' ';
				if (ns < maxSug) {
					wlst[ns] = mystrdup(candidate);
					if (wlst[ns] == NULL) return -1;
					ns++;
				} else return ns;
			}
		}
	}
	return ns;
}


// error is adjacent letter were swapped
int SuggestMgr::swapchar(char ** wlst, const char * word, int ns)
{
	char candidate[MAXSWL];
	char *p;
	char tmpc;
	int cwrd;

	int wl = strlen(word);

	// try swapping adjacent chars one by one
	strcpy(candidate, word);
	for (p = candidate;  p[1] != 0;  p++) {
		tmpc = *p;
		*p = p[1];
		p[1] = tmpc;
		cwrd = 1;
		for (int k=0; k < ns; k++)
			if (strcmp(candidate,wlst[k]) == 0) cwrd = 0;
		if ((cwrd) && check(candidate,wl)) {
			if (ns < maxSug) {
				wlst[ns] = mystrdup(candidate);
				if (wlst[ns] == NULL) return -1;
				ns++;
			} else return ns;
		}
		tmpc = *p;
		*p = p[1];
		p[1] = tmpc;
	}
	return ns;
}


// see if a candidate suggestion is spelled correctly
// needs to check both root words and words with affixes
int SuggestMgr::check(const char * word, int len)
{
	struct hentry * rv=NULL;
	if (pAMgr) { 
		rv = pAMgr->lookup(word);
		if (rv == NULL) rv = pAMgr->affix_check(word,len);
	}
	if (rv) return 1;
	return 0;
}
