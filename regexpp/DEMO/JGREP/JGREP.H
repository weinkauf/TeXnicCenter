/*
 *
 * Copyright (c) 1998-9
 * Dr John Maddock
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Dr John Maddock makes no representations
 * about the suitability of this software for any purpose.  
 * It is provided "as is" without express or implied warranty.
 *
 */
 
 /*
  *	FILE     jgrep.h
  *	VERSION  2.00
  */

#ifndef __JGREP_H
#define __JGREP_H

#include <regex.h>


// case sensitive reg_expression determines our allocator type:
#ifdef JM_NO_DEFAULT_PARAM
typedef reg_expression<char, char_regex_traits<char>, jm_def_alloc> re_type;
#else
typedef reg_expression<char, char_regex_traits<char> > re_type;
#endif
typedef re_type::alloc_type alloc_type;

// all other types use the same allocator:
typedef reg_expression<char, char_regex_traits_i<char>, alloc_type > rei_type;

// now declare static (global) data, including an allocator
// instance which we'll pass to all instances that require an allocator.

extern alloc_type a;

extern re_type e;
extern rei_type ei;

// flags for output:

extern bool use_case;
extern bool show_lines;
extern bool count_only;
extern bool files_only;
extern bool recurse;
extern bool regularexs;
extern bool words_only;
extern bool verbose;

void process_grep(const char* file);

#ifdef __DOSFIX

#include "fileiter.h"

bool __reg_search(mapfile::iterator, mapfile::iterator, reg_match<mapfile::iterator>&, const re_type&);
bool __reg_search(mapfile::iterator, mapfile::iterator, reg_match<mapfile::iterator>&, const rei_type&);

#define reg_search __reg_search

#endif

#endif



