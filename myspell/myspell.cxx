#include "license.readme"

#include <cctype>
#include <cstring>
#include <cstdlib>
#include <cstdio>

#include "stdafx.h"
#include "myspell.hxx"


MySpell::MySpell(const char * affpath, const char * dpath)
{
    /* first set up the hash manager */
    pHMgr = new HashMgr(dpath);

    /* next set up the affix manager */
    /* it needs access to the hash manager lookup methods */
    pAMgr = new AffixMgr(affpath,pHMgr);

    /* get the preferred try string from the Affix Manager for that dictionary */
    char * try_string = pAMgr->get_try_string();

    /* and finally set up the suggestion manager */
    pSMgr = new SuggestMgr(try_string, 10, pAMgr);

    if (try_string) free(try_string);
}


MySpell::~MySpell()
{
    if (pSMgr) delete pSMgr;
    if (pAMgr) delete pAMgr;
    if (pHMgr) delete pHMgr;
    pSMgr = NULL;
    pAMgr = NULL;
    pHMgr = NULL;
}


int MySpell::captype(const char * word)
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


void MySpell::mkallcap(char * p)
{
  while (*p != '\0') { 
    *p = toupper((unsigned char) *p);
    p++;
  }
}


void MySpell::mkallsmall(char * p)
{
  while (*p != '\0') { 
    *p = tolower((unsigned char) *p);
    p++;
  }
}


int MySpell::spell(const char * word)
{
  char * rv=NULL;
  char wspace[MAXWORDLEN+1];
  int wl = strlen(word);
  if (wl > MAXWORDLEN) return 0;
  int ct = captype(word);

  switch(ct) {
     case HUHCAP:
     case NOCAP:   { 
                     rv = check(word); 
                     break;
                   }

     case ALLCAP:
     case INITCAP: { 
                     memcpy(wspace,word,(wl+1));
                     mkallsmall(wspace);
                     rv = check(wspace);
                     if (rv == NULL) rv = check(word);
                     break; 
                   }
  }
  if (rv) return 1;
  return 0;
}


char * MySpell::check(const char * word)
{
  struct hentry * he = NULL;
  if (pHMgr)
     he = pHMgr->lookup (word);

  if ((he == NULL) && (pAMgr)) {
     // try stripping off affixes */
     he = pAMgr->affix_check(word, strlen(word));
  }

  if (he) return he->word;
  return NULL;
}



int MySpell::suggest(char*** slst, const char * word)
{
  char wspace[MAXWORDLEN+1];
  if (! pSMgr) return 0;
  int wl = strlen(word);
  int ct = captype(word);
  int ns = 0;
  switch(ct) {
     case NOCAP:   { 
                     ns = pSMgr->suggest(slst, word); 
                     break;
                   }

     case INITCAP: { 
                     memcpy(wspace,word,(wl+1));
                     mkallsmall(wspace);
                     ns = pSMgr->suggest(slst, wspace);
                     for (int j=0; j < ns; j++)
                       mkinitcap((*slst)[j]);
                     break;
                   }

     case HUHCAP: { 
                     ns = pSMgr->suggest(slst, word);
                     if (ns == 0) {
                        memcpy(wspace,word,(wl+1));
                        mkallsmall(wspace);
                        ns = pSMgr->suggest(slst, wspace);
                     }
                     break;
                   }

     case ALLCAP: { 
                     memcpy(wspace,word,(wl+1));
                     mkallsmall(wspace);
                     ns = pSMgr->suggest(slst, wspace);
                     for (int j=0; j < ns; j++)
                       mkallcap((*slst)[j]);
                     break;
                   }
  }
  return ns;
}



void MySpell::mkinitcap(char * p)
{
  if (*p != '\0') *p = toupper((unsigned char)*p);
}


