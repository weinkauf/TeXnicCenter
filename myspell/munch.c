/* Munch a word list and generate a smaller root word list with affixes*/

#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <error.h>
#include <errno.h>
#include <sys/mman.h>

#include "munch.h"

int main(int argc, char** argv)
{

  int i, j, k, l, m;
  int rl, p , nwl;
  int numents;

  FILE * wrdlst;
  FILE * afflst;

  char *nword, *wf, *af;

  struct hentry * ep;
  struct hentry * ep1;
  struct affent * pfxp;
  struct affent * sfxp;

  /* first parse the command line options */
  /* arg1 - wordlist, arg2 - affix file */

  if (argv[1]) {
       wf = strdup(argv[1]);
  } else {
    fprintf(stderr,"correct syntax is:\n"); 
    fprintf(stderr,"munch word_list_file affix_file\n");
    exit(1);
  }
  if (argv[2]) {
       af = strdup(argv[2]);
  } else {
    fprintf(stderr,"correct syntax is:\n"); 
    fprintf(stderr,"munch word_list_file affix_file\n");
    exit(1);
  }

  /* open the affix file */
  afflst = fopen(af,"r");
  if (!afflst) {
    fprintf(stderr,"Error - could not open affix description file\n");
    exit(1);
  }

  /* step one is to parse the affix file building up the internal
     affix data structures */

  numpfx = 0;
  numsfx = 0;

  parse_aff_file(afflst);
  fclose(afflst);

  fprintf(stderr,"parsed in %d prefixes and %d suffixes\n",numpfx,numsfx);

  /* affix file is now parsed so create hash table of wordlist on the fly */

  /* open the wordlist */
  wrdlst = fopen(wf,"r");
  if (!wrdlst) {
    fprintf(stderr,"Error - could not open word list file\n");
    exit(1);
  }

  if (load_tables(wrdlst)) {
    fprintf(stderr,"Error building hash tables\n");
    exit(1);
  }
  fclose(wrdlst);

  for (i=0; i< tablesize; i++) {
    ep = &tableptr[i];
    if (ep->word == NULL) continue;
    for (  ;  ep != NULL;  ep = ep->next) {
      numroots = 0;
      aff_chk(ep->word,strlen(ep->word));
      if (numroots) {
         /* now there might be a number of combinations */
         /* of prefixes and suffixes that might match this */
         /* word.  So how to choose?  As a first shot look */
         /* for the shortest remaining root word to */
         /* to maximize the combinatorial power */
         rl = 1000;
         p = -1;
         for (j = 0; j < numroots; j++){
            nword = (roots[j].hashent)->word;
            nwl = strlen(nword);
            if (nwl < rl) {
               rl = nwl;
               p = j;
            } 
         }
         ep1 = roots[p].hashent;
         pfxp = roots[p].prefix;
         sfxp = roots[p].suffix;
         ep1->keep = 1;
         if (pfxp != NULL) add_affix_char(ep1,pfxp->achar);
         if (sfxp != NULL) add_affix_char(ep1,sfxp->achar);
      } else {
         ep->keep = 1;
      }
    }
  } 


  /* now output only the words to keep along with affixes info */
  /* first count how many words that is */
  k = 0;
  for (i=0; i< tablesize; i++) {
    ep = &tableptr[i];
    if (ep->word == NULL) continue;
    for (  ;  ep != NULL;  ep = ep->next) {
       if (ep->keep > 0) k++;
    }
  }
  fprintf(stdout,"%d\n",k);

  for (i=0; i< tablesize; i++) {
    ep = &tableptr[i];
    if (ep->word == NULL) continue;
    for (  ;  ep != NULL;  ep = ep->next) {
      if (ep->keep > 0) {
        if (ep->affstr != NULL) { 
	  fprintf(stdout,"%s/%s\n",ep->word,ep->affstr);
	} else {
          fprintf(stdout,"%s\n",ep->word);
        }
      }
    }
  }
  return 0;
}


void parse_aff_file(FILE * afflst)
{  
    int i, k, j;
    int numents;
    char achar;
    short ff=0;
    char ft;
    struct affent * ptr= NULL;
    struct affent * nptr= NULL;
    char * line = new char[MAX_LN_LEN;

    while (fgets(line,MAX_LN_LEN,afflst)) {
       k = strlen(line) - 1;
       *(line+k) = 0;
       ft = ' ';
       fprintf(stderr,"parsing line: %s\n",line);
       if (strncmp(line,"PFX",3) == 0) ft = 'P';
       if (strncmp(line,"SFX",3) == 0) ft = 'S';
       if (ft != ' ') {
          char * tp = line;
          char * piece;
          i = 0;
          while ((piece=mystrsep(&tp,' '))) {
             if (*piece != '\0') {
                 switch(i) {
                    case 0: break;
                    case 1: { achar = *piece; break; }
                    case 2: { if (*piece == 'Y') ff = XPROD; break; }
                    case 3: { numents = atoi(piece); 
                              ptr = new affent[numents];
                              ptr->achar = achar;
                              ptr->cpf = ff;
	                      fprintf(stderr,"parsing %c entries %d\n",achar,numents);
                            }
                     default:
                 }
                 i++;
             }
             delete [] piece;
          }
		  delete [] piece;
          /* now parse all of the sub entries*/
          nptr = ptr;
          for (j=0; j < numents; j++) {
             fgets(line,MAX_LN_LEN,afflst);
             k = strlen(line) - 1;
             *(line+k) = 0;
             tp = line;
             i = 0;
             while ((piece=mystrsep(&tp,' '))) {
                if (*piece != '\0') {
                    switch(i) {
                       case 0: { if (nptr != ptr) {
                                   nptr->achar = ptr->achar;
                                   nptr->cpf = ptr->cpf;
                                 }
                                 break;
                               }
                       case 1: break;
                       case 2: { nptr->strip = strdup(piece);
                                 nptr->stripl = strlen(nptr->strip);
                                 if (strcmp(nptr->strip,"0") == 0) {
                                   nptr->strip=strdup("");
				   nptr->stripl = 0;
                                 }   
                                 break; 
                               }
                       case 3: { nptr->appnd = strdup(piece);
                                 nptr->appndl = strlen(nptr->appnd);
                                 if (strcmp(nptr->appnd,"0") == 0) {
                                   nptr->appnd=strdup("");
				   nptr->appndl = 0;
                                 }   
                                 break; 
                               }
                       case 4: { encodeit(nptr,piece);}
                               fprintf(stderr, "   affix: %s %d, strip: %s %d\n",nptr->appnd,
                                                   nptr->appndl,nptr->strip,nptr->stripl);
                       default:
                    }
                    i++;
                }
                delete [] piece;
             }
			 delete [] piece;
             nptr++;
          }
          if (ft == 'P') {
             ptable[numpfx].aep = ptr;
             ptable[numpfx].num = numents;
             fprintf(stderr,"ptable %d num is %d\n",numpfx,ptable[numpfx].num);
             numpfx++;
          } else {
             stable[numsfx].aep = ptr;
             stable[numsfx].num = numents;
             fprintf(stderr,"stable %d num is %d\n",numsfx,stable[numsfx].num);
             numsfx++;
          }
          ptr = NULL;
          nptr = NULL;
          numents = 0;
          achar='\0';
       }
    }
    delete [] line;
}


void encodeit(struct affent * ptr, char * cs)
{
  int nc;
  int neg;
  int grp;
  char c;
  int n;
  int ec;   
  int nm;
  int i, j, k;
  unsigned char * conds;
  char mbr[MAX_WD_LEN];

  /* now clear the conditions array */
  for (i=0;i<SET_SIZE;i++) ptr->conds[i] = (unsigned char) 0;

  /* now parse the string to create the conds array */
  nc = strlen(cs);
  neg = 0;  /* complement indicator */
  grp = 0;  /* group indicator */
  n = 0;    /* number of conditions */
  ec = 0;   /* end condition indicator */
  nm = 0;   /* number of member in group */
  i = 0;
  if (strcmp(cs,".")==0) {
    ptr->numconds = 0;
    return;
  }
  while (i < nc) {
    c = *(cs + i);
    if (c == '[') {
       grp = 1;
       c = 0;
    }
    if ((grp == 1) && (c == '^')) {
       neg = 1;
       c = 0;
    }
    if (c == ']') {
       ec = 1;
       c = 0;
    }
    if ((grp == 1) && (c != 0)) {
      *(mbr + nm) = c;
      nm++;
      c = 0;
    }
    if (c != 0) {
       ec = 1;
    }
    if (ec) {
      if (grp == 1) {
        if (neg == 0) {
	  for (j=0;j<nm;j++) {
	     k = (unsigned int) mbr[j];
             ptr->conds[k] = ptr->conds[k] | (1 << n);
          }
	} else {
	   for (j=0;j<SET_SIZE;j++) ptr->conds[j] = ptr->conds[j] | (1 << n);
	   for (j=0;j<nm;j++) {
	     k = (unsigned int) mbr[j];
             ptr->conds[k] = ptr->conds[k] & ~(1 << n);
	   }
        }
        neg = 0;
        grp = 0;   
        nm = 0;
      } else {
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



/* search for a prefix */
void pfx_chk (const char * word, int len, struct affent* ep, int num)
{
    struct affent *     aent;
    int			cond;
    int	tlen;
    struct hentry *	hent;
    unsigned char *	cp;		
    int			i;
    char	        tword[MAX_WD_LEN];

    for (aent = ep, i = num; i > 0; aent++, i--) {

	tlen = len - aent->appndl;

	if (tlen > 0 &&  (aent->appndl == 0 ||  
            strncmp(aent->appnd, word, aent->appndl) == 0)
	    &&  tlen + aent->stripl >= aent->numconds) {

	    if (aent->stripl) strcpy (tword, aent->strip);
	    strcpy((tword + aent->stripl), (word + aent->appndl));

            /* now go through the conds and make sure they all match */
	    cp = (unsigned char *) tword;
	    for (cond = 0;  cond < aent->numconds;  cond++) {
		if ((aent->conds[*cp++] & (1 << cond)) == 0)
		    break;
	    }

	    if (cond >= aent->numconds) {
		tlen += aent->stripl;
		if ((hent = lookup(tword)) != NULL) {
		   if (numroots < MAX_ROOTS) {
		       roots[numroots].hashent = hent;
		       roots[numroots].prefix = aent;
		       roots[numroots].suffix = NULL;
		       numroots++;
		   }
		}
	    }
	}
    }
}



void suf_chk (const char * word, int len, struct affent * ep, 
              int num, struct affent * pfxent, int cpflag)
{
    struct affent *     aent;	
    int	                tlen;	
    int			cond;	
    struct hentry *	hent;	
    unsigned char *	cp;
    int			i;
    char	        tword[MAX_WD_LEN];

    for (aent = ep, i = num; i > 0; aent++, i--) {

        if ((cpflag & XPROD) != 0 &&  (aent->cpf & XPROD) == 0)
	    continue;

	tlen = len - aent->appndl;
	if (tlen > 0  &&  (aent->appndl == 0 ||  
            strcmp(aent->appnd, (word + tlen)) == 0)
	    &&  tlen + aent->stripl >= aent->numconds) {

	    strcpy (tword, word);
	    cp = (unsigned char *) (tword + tlen);
	    if (aent->stripl) {
	       strcpy ((char *)cp, aent->strip);
	       tlen += aent->stripl;
	       cp = (unsigned char *)(tword + tlen);
	    } else *cp = '\0';

	    for (cond = aent->numconds;  --cond >= 0;  ) {
		if ((aent->conds[*--cp] & (1 << cond)) == 0) break;
	    }
	    if (cond < 0) {
	       if ((hent = lookup(tword)) != NULL) {
		  if (numroots < MAX_ROOTS) {
		     roots[numroots].hashent = hent;
		     roots[numroots].prefix = pfxent;
		     roots[numroots].suffix = aent;
		     numroots++;
		  }
	       }
	    }
	}
    }
}



void aff_chk (const char * word, int len)
{
    int i;
    int j;
    int nh=0;
    char * nword;
    int nwl;

    for (i=0; i < numpfx; i++) {
       pfx_chk(word, len, ptable[i].aep, ptable[i].num);
    }

    nh = numroots;

    if (nh > 0) {
       for (j=0;j<nh;j++){
         if (roots[j].prefix->cpf > 0) {
            nword = strdup((roots[j].hashent)->word);
            nwl = strlen(nword);
            for (i=0; i < numsfx; i++) {
               suf_chk(nword,nwl,stable[i].aep, stable[i].num, roots[j].prefix, XPROD);
            }
            delete [] nword;
         }
       }
    }
    for (i=0; i < numsfx; i++) {
       suf_chk(word, len, stable[i].aep, stable[i].num, NULL, 0);
    }
}


void strtolower(char * p)
{
  char * c = p;
  while (*c) {
    *c = (char) tolower((unsigned char)(*c));
    c++;
  }
}


char * mystrsep(char ** stringp, const char delim)
{
  char * dp;
  int nc;
  char * rv = NULL;
  char * mp = *stringp;
  int n = strlen(mp);
  if (n > 0) {
     dp = (char *)memchr(mp,(int)((unsigned char)delim),n);
     if (dp) {
        *stringp = dp+1;
        nc = (int)((unsigned long)dp - (unsigned long)mp); 
        rv = new char[nc+1];
        memcpy(rv,mp,nc);
        *(rv+nc) = '\0';
        return rv;
     } else {
       rv = new char[n+1];
       memcpy(rv, mp, n);
       *(rv+n) = '\0';
       *stringp = mp + n;
       return rv;
     }
  }
  return NULL;
}


// lookup a root word in the hashtable

struct hentry * lookup(const char *word)
{
    struct hentry * dp;
    dp = &tableptr[hash(word)];
    if (dp->word == NULL) return NULL;
    for (  ;  dp != NULL;  dp = dp->next) {
      if (strcmp(word,dp->word) == 0) return dp;
    }
    return NULL;
}



// add a word to the hash table (private)

int add_word(char * word)
{
    int i;
    struct hentry * dp;
	struct hentry * hp = new hentry;

    hp->word = word;
    hp->affstr = NULL;
    hp->keep = 0;
    hp->next = NULL;

    i = hash(word);
    dp = &tableptr[i];
    
    if (dp->word == NULL) {
      *dp = *hp;
       delete hp;
    } else {
      while (dp->next != NULL) dp=dp->next; 
      dp->next = hp;
    }
    return 0;
}     



// load a word list and build a hash table on the fly

int load_tables(FILE * wdlst)
{
  int i, j, k;
  int wl;
  char * ap;
  char ts[MAX_LN_LEN];

  // first read the first line of file to get hash table size */
  if (! fgets(ts, MAX_LN_LEN-1,wdlst)) return 2;
  tablesize = atoi(ts);
  tablesize + tablesize + 5;
  if ((tablesize %2) == 0) tablesize++;

  // allocate the hash table
  tableptr = new hentry[tablesize];
  if (! tableptr) return 3;

  // loop thorugh all words on much list and add to hash
  // table and store away word and affix strings in tmpfile

  while (fgets(ts,MAX_LN_LEN-1,wdlst)) {
    wl = strlen(ts);
    ts[wl-1] = '\0';

    ap = strdup(ts);
    add_word(ap);

  }
  return 0;
}


// the hash function is a simple load and rotate
// algorithm borrowed

int hash(const char * word)
{
    int i;
    long  hv = 0;
    for (i=0; i < 4  &&  *word != 0; i++)
	hv = (hv << 8) | tolower(*word++);
    while (*word != 0) {
      ROTATE(hv,ROTATE_LEN);
      hv ^= tolower(*word++);
    }
    return (unsigned long) hv % tablesize;
}


void add_affix_char(struct hentry * ep, char ac)
{
  int al;
  int i;
  char * tmp;
  if (ep->affstr == NULL) {
     ep->affstr = new char[2];
     *(ep->affstr) = ac;
     *((ep->affstr)+1) = '\0';
     return;
  }
  al = strlen(ep->affstr);
  for (i=0; i< al; i++)
    if (ac == (ep->affstr)[i]) return;
  tmp = new char[al+2];
  memcpy(tmp,ep->affstr,(al+1));
  *(tmp+al) = ac;
  *(tmp+al+1)='\0';
  delete [] ep->affstr;
  ep->affstr = tmp;
  return;
}
