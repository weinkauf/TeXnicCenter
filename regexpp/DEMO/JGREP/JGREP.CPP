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
  *	FILE     jgrep.cpp
  *	VERSION  2.00
  */

#include <stdio.h>
#include <regex>
#ifdef OLD_IOSTREAM
#include <iostream.h>
#else
#include <iostream>
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
#endif
#pragma hrdstop

#include "fileiter.h"

#include "jgrep.h"


//
// class ogrep_iterator
// outputs the results of reg_grep to screen:
template <class iterator, class Allocator >
class ogrep_iterator
{
   unsigned int& lines;
   const char* filename;
   unsigned int last_line;
   iterator end_of_storage;
public:
   ogrep_iterator(unsigned int& i, const char* p, iterator e) : lines(i), filename(p), last_line(-1), end_of_storage(e) {}
   ogrep_iterator(const ogrep_iterator& o) : lines(o.lines), filename(o.filename), last_line(o.last_line), end_of_storage(o.end_of_storage) {}
   bool operator () (const reg_match<iterator, Allocator>& i);
};

// ideally we'd ignor the allocator type and use a template member function
// to deel with the allocator type passed to reg_grep, unfortunately most
// compilers don't support this feature yet, so we'll have to be sure that
// the allocator passed to instances of this class match that used in our
// regular expression classes.

template <class iterator, class Allocator>
bool ogrep_iterator<iterator, Allocator>::operator()(const reg_match<iterator, Allocator>& i)
{
   if(last_line == (unsigned int)-1)
      cout << "File " << filename << ":" << endl;
   if(last_line != i.line())
   {
      ++lines;
      last_line = i.line();
      if(count_only == 0)
      {
         if(show_lines)
            cout << i.line() << "\t";
         iterator ptr = i.line_start();
         while((ptr != end_of_storage) && (*ptr != '\n'))++ptr;
         iterator pos = i.line_start();
         while(pos != ptr)
         {
            cout.put(*pos);
            ++pos;
         }
         cout << endl;
      }
   }
   return true;
}


void process_grep(const char* file)
{
   mapfile f(file);
   unsigned int count = 0;
   ogrep_iterator<mapfile::iterator, alloc_type> oi(count, file, f.end());
   if(files_only)
   {
      bool ok;
      reg_match<mapfile::iterator, alloc_type> m;
      //if(use_case)
         ok = reg_search(f.begin(), f.end(), m, e, match_not_dot_newline | match_not_dot_null);
      //else
         //ok = reg_search(f.begin(), f.end(), m, ei, match_not_dot_newline | match_not_dot_null);
      if(ok)
         cout << "File " << file << endl;
   }
   else
   {
      //if(use_case)
         reg_grep(oi, f.begin(), f.end(), e, match_not_dot_newline | match_not_dot_null);
      //else
         //reg_grep(oi, f.begin(), f.end(), ei, match_not_dot_newline | match_not_dot_null);
      if(count)
      {
         if(verbose || count_only)
         {
            cout << count << " lines match" << endl;
            return;
         }
      }
      else if(verbose)
      {
         cout << "File " << file << ":" << endl << "0 lines match" << endl;
      }
   }
}






