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
  *	FILE     timer.h
  *	VERSION  2.00
  */

#ifndef __TIMER_H
#define __TIMER_H

//
// the current std::string class implimentations vary quite a bit
// and tend not to function "out of the box" for 16-bit builds.
// as a temporary work around we'll use vector instead

#ifdef OLD_IOSTREAM
#include <iostream.h>
#else
#include <iostream>
using std::cout;
using std::cin;
using std::cerr;
#endif
#include <deque>
#include <vector>
#include <regex.h>


typedef __JM_STD::vector<char> string_type;
typedef __JM_STD::vector<wchar_t> wstring_type;
typedef __JM_STD::deque<char> deque_type;

typedef reg_expression<char, char_regex_traits<char>, jm_def_alloc > re_type;
typedef reg_expression<wchar_t, char_regex_traits<wchar_t>, jm_def_alloc > wre_type;

#ifdef __DOSFIX

// this header is only required to fix some of the problems we get
// with segments > 64K under MSDOS

#define reg_search __reg_search

bool __reg_search(string_type::iterator, string_type::iterator, reg_match<string_type::iterator>&, const re_type&);
bool __reg_search(wstring_type::iterator, wstring_type::iterator, reg_match<wstring_type::iterator>&, const wre_type&);
bool __reg_search(deque_type::iterator, deque_type::iterator, reg_match<deque_type::iterator>&, const re_type&);



#endif

#endif





