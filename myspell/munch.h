/* munch header file */

#define MAX_LN_LEN    200
#define MAX_WD_LEN    200
#define MAX_PREFIXES  256
#define MAX_SUFFIXES  256
#define MAX_ROOTS      20
 
#define ROTATE_LEN      5
 
#define ROTATE(v,q) \
   (v) = ((v) << (q)) | (((v) >> (32 - q)) & ((1 << (q))-1));

#define SET_SIZE      256

#define XPROD  (1 << 0)

/* the affix table entry */

struct affent
{
    char *  appnd;
    char *  strip;
    short   appndl;
    short   stripl;
    char    achar;
    char    cpf;   
    short   numconds;
    char    conds[SET_SIZE];
};


struct affixptr
{
    struct affent * aep;
    int		    num;
};

/* the prefix and suffix table */
int	numpfx;		/* Number of prefixes in table */
int     numsfx;		/* Number of suffixes in table */

/* the prefix table */
struct affixptr          ptable[MAX_PREFIXES];

/* the suffix table */
struct affixptr          stable[MAX_SUFFIXES];


/* data structure to store results of lookups */
struct matches
{
    struct hentry *	hashent;	/* hash table entry */
    struct affent *	prefix;		/* Prefix used, or NULL */
    struct affent *	suffix;		/* Suffix used, or NULL */
};

int    numroots;	          /* number of root words found */
struct matches  roots[MAX_ROOTS]; /* list of root words found */

/* hashing stuff */

struct hentry
{
  char * word;
  char * affstr;
  struct hentry * next;
  int keep;
};

 
int             tablesize;
struct hentry * tableptr;


/* the routines */

void parse_aff_file(FILE* afflst);

void encodeit(struct affent * ptr, char * cs);

char * mystrsep(char ** stringp, const char delim);

int load_tables(FILE * wrdlst);

int hash(const char *);

int add_word(char *);

struct hentry * lookup(const char *);

void strtolower(char * p);

void aff_chk (const char * word, int len);

void pfx_chk (const char * word, int len, struct affent* ep, int num);

void suf_chk (const char * word, int len, struct affent * ep, int num, 
	      struct affent * pfxent, int cpflag);

void add_affix_char(struct hentry * hent, char ac);
