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
  *
  *	FILE     regress.cpp
  *	VERSION  2.00
  *
  * main() and associated code for regress.
  *
  */

#include <stdlib.h>
#include <regex>
#ifdef OLD_IOSTREAM
#include <fstream.h>
#else
#include <fstream>
using std::ifstream;
using std::ofstream;
//using std::cin;
//using std::cout;
using std::endl;
#endif

#ifdef __BORLANDC__
#pragma hrdstop
#endif

#include "regress.h"

#include "rlocale.h"


//
// declare all our globals here:
//

string_type expression;
string_type __text;
string_type format_string;
string_type merge_string;
int matches[MAX_MATCHES];

char file[MAX_PATH];
int line;
bool do_test = false;
unsigned int flags[5];

void usage()
{
   cout <<

"Usage: regress filename [filename2...]\n"
"see tests.txt for an example test script\n"
"and the script format.\n";

}

int main(int argc, char * argv[])
{
   if(argc < 2)
       usage();
   int i;
   for(i = 1; i < argc; ++i)
   {
      reset_error();
      ifstream is(argv[i]);
      if(is.good() == false)
      {
         cout << "Error unable to open file: " << argv[i] << endl << endl;
         continue;
      }
      strcpy(file, argv[i]);
      line = 0;
      unsigned int tests = 0;
      while(is.good())
      {
         string_type s;
         get_line(is, s);
         ++line;
         parse_input_line(s);
         if(do_test)
         {
            run_tests();
            ++tests;
         }
      }
      cout << line << " lines, " << tests << " tests completed in file " << argv[i] << endl;
   }

   #ifdef RE_LOCALE_CPP
   //
   // change the locale and run the tests again:
   //
   cout << "switching to alternate locale..." << endl;
   setuplocale();
   for(i = 1; i < argc; ++i)
   {
      reset_error();
      ifstream is(argv[i]);
      if(is.good() == false)
      {
         cout << "Error unable to open file: " << argv[i] << endl << endl;
         continue;
      }
      strcpy(file, argv[i]);
      line = 0;
      unsigned int tests = 0;
      while(is.good())
      {
         string_type s;
         get_line(is, s);
         ++line;
         parse_input_line(s);
         if(do_test)
         {
            run_tests();
            ++tests;
         }
      }
      cout << line << " lines, " << tests << " tests completed in file " << argv[i] << endl;
   }
   #endif
   return 0;
}

#ifdef TEST_UNICODE

ostream& operator << (ostream& os, const wchar_t* s)
{
   while(*s)
   {
      os.put((char)*s);
      ++s;
   }
   return os;
}

ostream& operator << (ostream& os, const string_type& s)
{
   string_type::const_iterator i, j;
   i = s.begin();
   j = s.end();
   --j;
   while(i != j)
   {
      os.put((char)*i);
      ++i;
   }
   return os;
}

istream& get_line(istream& is, nstring_type& s, char delim)
{
   char c = (char)is.get();
   s.erase(s.begin(), s.end());
   while((c != delim) && (c != EOF))
   {
      s.push_back(c);
      c = (char)is.get();
   }
   s.push_back(0);
   return is;
}

istream& get_line(istream& is, string_type& s, char delim)
{
   nstring_type t;
   get_line(is, t, delim);
   s.erase(s.begin(), s.end());
   nstring_type::iterator i, j;
   i = t.begin();
   j = t.end();
   wchar_t* buf = new wchar_t[MB_CUR_MAX + 1];
   int cchars;
   while(i != j)
   {
      cchars = mbtowc(buf, i, j - i);
      if(cchars == -1)
         break;
      if(cchars == 0)
         break;
      s.insert(s.end(), buf, buf + cchars);
      i += cchars;
   }
   s.push_back(0);
   delete[] buf;
   return is;
}


#else

ostream& operator << (ostream& os, const string_type& s)
{
   os << s.begin();
   return os;
}


istream& get_line(istream& is, string_type& s, char delim)
{
   char c = (char)is.get();
   s.erase(s.begin(), s.end());
   while((c != delim) && (c != EOF))
   {
      s.push_back(c);
      c = (char)is.get();
   }
   s.push_back(0);
   return is;
}

#endif


