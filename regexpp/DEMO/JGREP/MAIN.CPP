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
  *	FILE     main.cpp
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

#ifndef JM_ALGO_INCLUDED
// HP and SGI STL's use <algo.h> instead
// this will ahve been pulled in by <jm_cfg.h>
// for std::distance
#include <algorithm>
#endif

alloc_type a;

re_type e(a);
//rei_type ei(a);

// flags for output:

bool use_case = true;
bool show_lines = false;
bool count_only = false;
bool files_only = false;
bool recurse = false;
bool regularexs = true;
bool words_only = false;
bool verbose = false;


void usage()
{
   cout <<
"jgrep version 0.95\n"
"usage: jgrep [-options] expression file [files...]\n"
"\n"
"options can be one of the following:\n"
"\n"
"-c   prints a count of the number of matching lines only\n"
"-d   recurses through subdirectories for matching files\n"
"-i   causes case to be ignored when matching\n"
"-l   lists the files which contain a match only\n"
"-n   displays the line numbers of matches\n"
"-r-  causes the expression to be interpreted as a literal string and not\n"
"     as a regular expression\n"
"-w   searches for matches that are whole words only\n"
"-z   verbose mode\n"
"\n"
"expression: a regular expression, or a literal string if -r- is specified\n"
"\n"
"files:  one or more files to search, the names can contain the wildcard\n"
"        characters ? and *\n" << endl;

}

void parse_switch(const char* flag)
{
   ++flag;
   while(*flag)
   {
      switch(*flag)
      {
         case '-':
            switch(*(flag-1))
            {
               case 'c':
                  count_only = false;
                  break;
               case 'd':
                  recurse = false;
                  break;
               case 'i':
                  use_case = false;
                  break;
               case 'l':
                  files_only = false;
                  break;
               case 'n':
                  show_lines = false;
                  break;
               case 'r':
                  regularexs = false;
                  break;
               case 'w':
                  words_only = false;
                  break;
               case 'z':
                  verbose = false;
                  break;
               default:
                  cout << "Undefined option -";
                  cout.put(*flag);
                  cout << endl;
            }
            // turn off prev character:
            break;
         case 'c':
            count_only = true;
            break;
         case 'd':
            recurse = true;
            break;
         case 'i':
            use_case = false;
            break;
         case 'l':
            files_only = true;
            break;
         case 'n':
            show_lines = true;
            break;
         case 'r':
            regularexs = true;
            break;
         case 'w':
            words_only = true;
            break;
         case 'z':
            verbose = true;
            break;
         case '?':
            usage();
            exit(0);
         case '+':
            break;
         default:
            cout << "Undefined option -";
            cout.put(*flag);
            cout << endl;
      }
      ++flag;
   }
}

void HandleFile(const char* wild)
{
   file_iterator end;
   file_iterator start(wild);

   if(recurse)
   {
      // go through sub directories:
      char buf[MAX_PATH];
      strcpy(buf, start.root());
      if(*buf == 0)
      {
         strcpy(buf, ".");
         strcat(buf, directory_iterator::separator());
         strcat(buf, "*");
      }
      else
      {
         strcat(buf, directory_iterator::separator());
         strcat(buf, "*");
      }
      directory_iterator dstart(buf);
      directory_iterator dend;

      // now get the file mask bit of "wild":
      const char* ptr = wild;
      while(*ptr) ++ptr;
      while((ptr != wild) && (*ptr != *directory_iterator::separator()))--ptr;
      if(ptr != wild) ++ptr;

      while(dstart != dend)
      {
         sprintf(buf, "%s%s%s", dstart.path(), directory_iterator::separator(), ptr);
         HandleFile(buf);
         ++dstart;
      }
   }
   __JM_STD::for_each(start, end, process_grep);
}

int done = 0;

void HandleArg(const char* arg)
{
   if(*arg == '-')
   {
      parse_switch(arg);
      return;
   }
   if(done == 0)
   {
      // parse regular expression
      if(regularexs)
      {
         if(words_only == 0)
         {
            e.set_expression(arg, use_case ? regbase::normal : regbase::normal | regbase::icase);
            //ei.set_expression(arg);
         }
         else
         {
            char* buf = new char[strlen(arg) + 8];
            sprintf(buf, "\\<%s\\>", arg);
            e.set_expression(buf, use_case ? regbase::normal : regbase::normal | regbase::icase);
            //ei.set_expression(buf);
            delete[] buf;
         }
      }
      else
      {
         // we need to convert text to literal:
         int len2 = strlen(arg);
         int len = len2 * 5 + 6;
         char buf[8];
         char* buf2 = new char[len];
         *buf2 = 0;
         if(words_only)
            strcpy(buf2, "\\<");
         for(int j = 0; j < len2; ++j)
         {
            sprintf(buf, "\\0%o", int(arg[j]));
            strcat(buf2, buf);
         }
         if(words_only)
            strcat(buf2, "\\>");
         e.set_expression(buf2, use_case ? regbase::normal : regbase::normal | regbase::icase);
         //ei.set_expression(buf2);
         delete[] buf2;
      }
      done = 1;
      return;
   }
   // if we get to here we have one or more file names to process:
   ++done;
   HandleFile(arg);
}

int main(int argc, char * argv[])
{
   for(int i = 1; i < argc; ++i)
      HandleArg(argv[i]);
   if(done < 2)
      usage();
   return 0;
}






