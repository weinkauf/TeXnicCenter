#include "license.readme"

#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cstdio>

#include "stdafx.h"
#include "affixmgr.hxx"
#include "affix.hxx"

// remove end of line char(s)
void mychomp(char * s);

// duplicate string
char * mystrdup(const char * s);

// parse string into tokens with char delimiter
char * mystrsep(char ** stringp, const char delim);

AffixMgr::AffixMgr(const char * affpath, HashMgr* ptr) 
{
  // register hash manager and load affix data from aff file
  pHMgr = ptr;
  numpfx = 0;
  numsfx = 0;
  trystring = NULL;
  encoding=NULL;
  if (parse_file(affpath)) {
     numpfx = 0;
     numsfx = 0;
     fprintf(stderr,"Failure loading aff file\n");
     fflush(stderr);
  }
}


AffixMgr::~AffixMgr() 
{
  for (int i=0;i<numpfx;i++)
    delete (pTable[i]);
  for (int j=0;j<numsfx;j++)
    delete (sTable[j]);
  if (trystring) free(trystring);
  trystring=NULL;
  if (encoding) free(encoding);
  encoding=NULL;
  numpfx = 0;
  numsfx = 0;
  pHMgr = NULL;
}


// read in aff file and build up prefix and suffix data structures
int  AffixMgr::parse_file(const char * affpath)
{

  int i, j;
  int numents;


  // io buffers
  char line[MAXLNLEN+1];

  // open the affix file
  FILE * afflst;
  afflst = fopen(affpath,"r");
  if (!afflst) {
    fprintf(stderr,"Error - could not open affix description file\n");
    return 1;
  }

  // step one is to parse the affix file building up the internal
  // affix data structures

  numpfx = 0;       // number of prefixes
  numsfx = 0;       // number of suffixes
  numents = 0;      // number of affentry structures to parse
  char flag='\0';   // affix char identifier
  {  
    short ff=0;
    char ft;
    struct affentry * ptr= NULL;
    struct affentry * nptr= NULL;


    // read in each line ignoring any that do not
    // start with PFX or SFX

    while (fgets(line,MAXLNLEN,afflst)) {
       mychomp(line);

       /* parse in the try string */
       if (strncmp(line,"TRY",3) == 0) {
          char * tp = line;
          char * piece;
          i = 0;
          while ((piece=mystrsep(&tp,' '))) {
             if (*piece != '\0') {
                 switch(i) {
                    case 0: break;
                    case 1: 
                       if (trystring != NULL)
                          free(trystring);
                       trystring = mystrdup(piece);
                       break;
                    default: break;
                 }
                 i++;
             }
             free(piece);
          }
       }

       /* parse in the name of the character set used by the .dict and .aff */
       if (strncmp(line,"SET",3) == 0) {
          char * tp = line;
          char * piece;
          i = 0;
          while ((piece=mystrsep(&tp,' '))) {
             if (*piece != '\0') {
                 switch(i) {
                    case 0: break;
                    case 1: 
                       if (encoding != NULL)
                          free(encoding);
                       encoding = mystrdup(piece); 
                       break;
                    default: break;
                 }
                 i++;
             }
             free(piece);
          }
       }

       // get the type of this affix: P - prefix, S - suffix
       ft = ' ';
       if (strncmp(line,"PFX",3) == 0) ft = 'P';
       if (strncmp(line,"SFX",3) == 0) ft = 'S';
       if (ft != ' ') {
          char * tp = line;
          char * piece;
          i = 0;

          // split line into pieces

          while ((piece=mystrsep(&tp,' '))) {
             if (*piece != '\0') {
                 switch(i) {
                    // piece 1 - is type of affix
                    case 0: break;

                    // piece 2 - is affix char
                    case 1: { flag = *piece; break; }

                    // piece 3 - is cross product indicator 
                    case 2: { if (*piece == 'Y') ff = XPRODUCT; break; }

                    // piece 4 - is number of affentries
                    case 3:
                       numents = atoi(piece);
                       ASSERT(ptr == NULL);
                       if (ptr != NULL)
                         free(ptr);
                       ptr = (struct affentry *)malloc(numents *  sizeof(struct affentry));
                       memset(ptr, 0, numents * sizeof(struct affentry));
                       ptr->cpflag = ff;

                    default: break;
                 }
                 i++;
             }
             free(piece);
          }

          // store away ptr to first sffentry
          nptr = ptr;

          // now parse numents affentries for this affix
          for (j=0; j < numents; j++) {
             fgets(line,MAXLNLEN,afflst);
             mychomp(line);
             tp = line;
             i = 0;

             // split line into pieces
             while ((piece=mystrsep(&tp,' '))) {
                if (*piece != '\0') {
                    switch(i) {

                       // piece 1 - is type
                       case 0: { if (nptr != ptr) nptr->cpflag = ptr->cpflag;
                                 break;
                               }

                       // piece 2 - is affix char
                       case 1: break;

                       // piece 3 - is string to strip or 0 for null 
                       case 2: { nptr->strip = mystrdup(piece);
                                 nptr->stripl = strlen(nptr->strip);
                                 if (strcmp(nptr->strip,"0") == 0) {
                                   nptr->strip=mystrdup("");
                                   nptr->stripl = 0;
                                 }
                                 break; 
                               }

                       // piece 4 - is affix string or 0 for null
                       case 3: { nptr->appnd = mystrdup(piece);
                                 nptr->appndl = strlen(nptr->appnd);
                                 if (strcmp(nptr->appnd,"0") == 0) {
                                   nptr->appnd=mystrdup("");
                                   nptr->appndl = 0;
                                 }   
                                 break; 
                               }

                       // piece 5 - is the conditions descriptions
                       case 4: { encodeit(nptr,piece);}
                       default: break;
                    }
                    i++;
                }
                free(piece);
             }
             nptr++;
          }
         
          // now create the correct Affix Object (Prefix or Suffix)
          if (ft == 'P') {
             pTable[numpfx++] = (Affix *) new Prefix(this,flag,numents,ptr);
          } else {
             sTable[numsfx++] = (Affix *) new Suffix(this,flag,numents,ptr);
          }
          free(ptr);
          ptr = NULL;
          nptr = NULL;
          numents = 0;
       }
    }
  }
  fclose(afflst);
  return 0;
}


// takes aff file condition string and creates the
// conds array - please see the appendix at the end of the
// file affix.cxx which describes what is going on here
// in much more detail

void AffixMgr::encodeit(struct affentry * ptr, char * cs)
{
  char c;
  int i, j, k;
  char mbr[MAXLNLEN];

  // now clear the conditions array */
  for (i=0;i<SETSIZE;i++) ptr->conds[i] = (unsigned char) 0;

  // now parse the string to create the conds array */
  int nc = strlen(cs);
  int neg = 0;   // complement indicator
  int grp = 0;   // group indicator
  int n = 0;     // number of conditions
  int ec = 0;    // end condition indicator
  int nm = 0;    // number of member in group

  // if no condition just return
  if (strcmp(cs,".")==0) {
    ptr->numconds = 0;
    return;
  }

  i = 0;
  while (i < nc) {
    c = *(cs + i);

    // start group indicator
    if (c == '[') {
       grp = 1;
       c = 0;
    }

    // complement flag
    if ((grp == 1) && (c == '^')) {
       neg = 1;
       c = 0;
    }

    // end goup indicator
    if (c == ']') {
       ec = 1;
       c = 0;
    }

    // add character of group to list
    if ((grp == 1) && (c != 0)) {
      *(mbr + nm) = c;
      nm++;
      c = 0;
    }

    // end of condition 
    if (c != 0) {
       ec = 1;
    }

    
    if (ec) {
      if (grp == 1) {
        if (neg == 0) {
          // set the proper bits in the condition array vals for those chars
          for (j=0;j<nm;j++) {
             k = (unsigned int) mbr[j];
             ptr->conds[k] = ptr->conds[k] | (1 << n);
          }
        } else {
          // complement so set all of them and then unset indicated ones
          for (j=0;j<SETSIZE;j++) ptr->conds[j] = ptr->conds[j] | (1 << n);
          for (j=0;j<nm;j++) {
            k = (unsigned int) mbr[j];
            ptr->conds[k] = ptr->conds[k] & ~(1 << n);
          }
        }
        neg = 0;
        grp = 0;
        nm = 0;
      } else {
        // not a group so just set the proper bit for this char
        ptr->conds[(unsigned int) c] = ptr->conds[(unsigned int)c] | (1 << n);
      }
      n++;
      ec = 0;
    }

    i++;
  }
  ptr->numconds = n;
  return;
}



/* cross check suffixes with a specific prefix */
struct hentry * AffixMgr::cross_check (const char * word, int len, 
                       int sfxopts, Affix * ppfx)
{
    int i=0;
    struct hentry * rv= NULL;
    // go through every possible suffix with this prefix
    while (i < numsfx)  {
      rv = ((Suffix *)sTable[i])->check(word, len, sfxopts, (Prefix *)ppfx); 
      if (rv) return rv;
      i++;
    }
    return NULL;
}



// check if word with affixes is correctly spelled
struct hentry * AffixMgr::affix_check (const char * word, int len)
{
    int i;
    struct hentry * rv= NULL;
    
    // first check all prefixes (also cross check with suffixes if need be)
    i=0;
    while (i < numpfx) {
      rv = ((Prefix *)pTable[i])->check(word, len); 
      if (rv) return rv;
      i++;
    }

    // if still not found check all suffixes
    i = 0;
    while (i < numsfx) {
      rv = ((Suffix *)sTable[i])->check(word, len, 0, NULL); 
      if (rv) return rv;
      i++;
    }

    return NULL;
}



// utility method to convert a string to lower case
void AffixMgr::strtolower(char * p)
{
  char * c = p;
  while (*c) {
    *c = (char) tolower((unsigned char)(*c));
    c++;
  }
}


// return text encoding of dictionary
char * AffixMgr::get_encoding()
{
  if (! encoding ) return NULL;
  return mystrdup(encoding);
}


// return the preferred try string for suggestions
char * AffixMgr::get_try_string()
{
  if (! trystring ) return NULL;
  return mystrdup(trystring);
}


// utility method to look up root words in hash table
struct hentry * AffixMgr::lookup(const char * word)
{
  if (! pHMgr) return NULL;
  return pHMgr->lookup(word);
}


// strip strings into token based on single char delimiter
// acts like strsep() but only uses a delim char and not 
// a delim string

char * mystrsep(char ** stringp, const char delim)
{
  char * rv = NULL;
  char * mp = *stringp;
  int n = strlen(mp);
  if (n > 0) {
     char * dp = (char *)memchr(mp,(int)((unsigned char)delim),n);
     if (dp) {
        *stringp = dp+1;
        int nc = (int)((unsigned long)dp - (unsigned long)mp); 
        rv = (char *) malloc(nc+1);
        memcpy(rv,mp,nc);
        *(rv+nc) = '\0';
        return rv;
     } else {
       rv = (char *) malloc(n+1);
       memcpy(rv, mp, n);
       *(rv+n) = '\0';
       *stringp = mp + n;
       return rv;
     }
  }
  return NULL;
}


char * mystrdup(const char * s)
{
  char * d = NULL;
  if (s) {
     int sl = strlen(s);
     d = (char *) malloc(((sl+1) * sizeof(char)));
     if (d) memcpy(d,s,((sl+1)*sizeof(char)));
  }
  return d;
}


void mychomp(char * s)
{
  int k = strlen(s);
  if (k > 0) *(s+k-1) = '\0';
  if ((k > 1) && (*(s+k-2) == '\r')) *(s+k-2) = '\0';
}
