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

#include <regex> 
#ifdef OLD_IOSTREAM
#include <iostream.h>
#else
#include <iostream>
using std::cout;
using std::cin;
using std::cerr;
using std::istream;
using std::ostream;
using std::endl;
#endif
#include "hptimer.h"
#include "timer.h"

#ifndef JM_ALGO_INCLUDED
#include <algorithm>
#endif


//
// under MSDOS the clock() function doesn't give us a high enough
// resolution to time each event, so we'll have to stick to
// an average in that case:

#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32)
// high performance timer, must have micro-second resolution
#define START1
#define STOP1
#define START2 t.start();
#define STOP2  t.stop();
#define DIVIDER 1
#else
// low resolution timer, ie MSDOS
#define START1  t.start();
#define STOP1   t.stop();
#define START2
#define STOP2
#define DIVIDER iters
#endif

wstring_type widen(const string_type& s);
string_type narrow(const wstring_type& s);
void to_deque(deque_type& d, const string_type& s);
istream& getline(istream& is, string_type& s, char delim);

bool operator == (const string_type& s, const char* p)
{
   return strncmp(s.begin(), p, s.size()) == 0;
}

#ifdef _MSC_VER

ostream& operator<<(ostream& os, __int64 i )   
{
   char buf[20];
   sprintf(buf,"%I64d", i );
   os << buf;
   return os;
}

#endif

ostream& operator << (ostream& os, const string_type& s)
{
   string_type::const_iterator i, j;
   i = s.begin();
   j = s.end();
   while(i != j)
   {
      os.put(*i);
      ++i;
   }
   return os;
}

int main()
{
   re_type e1;
   wre_type e2;
   reg_match<string_type::iterator, jm_def_alloc> m1;
   reg_match<wstring_type::iterator, jm_def_alloc> m2;
   reg_match<deque_type::iterator, jm_def_alloc> m3;
   string_type s1, s2;
   wstring_type ws1, ws2;
   deque_type ds;
   regex_t r;
   regmatch_t* m;
   size_t n;
   hptimer t;
   bool result;
   int iters = 10000;
   htp_i t_reg, t_posix;

   while(true)
   {
      cout << "Enter expression: ";
      getline(cin, s1, '\n');
      if(s1 == "quit")
         break;
      ws1 = widen(s1);
      e1.set_expression(s1.begin(), s1.end());
      e2.set_expression(ws1.begin(), ws1.end());
      s1.push_back('\0');
      int code = regcomp(&r, s1.begin(), REG_EXTENDED);
      if(code != 0)
      {
         char buf[256];
         regerror(code, &r, buf, 256);
         cout << buf << endl;
      }
      n = r.re_nsub + 1;
      m = (regmatch_t *) malloc(sizeof(regmatch_t) * n);

      if(e1.flags() & regbase::failbit)
      {
         cout << "error: " << e1.errmsg() << endl << endl;
         continue;
      }
      if(e2.flags() & regbase::failbit)
      {
         cout << "error: " << e2.errmsg() << endl << endl;
         continue;
      }

      while(true)
      {
         cout << "Enter string: ";
         getline(cin, s2, '\n');
         if(s2 == "quit")
            break;

         ws2 = widen(s2);
         to_deque(ds, s2);

         int i;

         // this is a dummy, the first interval set measured
         // is always much longer than the others under win32
         for(i =0; i < iters / 10; ++i)
         {
            result = reg_search(s2.begin(), s2.end(), m1, e1);
         }

         // measure time interval for reg_expression<char>
         START1
         for(i =0; i < iters; ++i)
         {
            START2
            result = reg_search(s2.begin(), s2.end(), m1, e1);
            STOP2
         }
         STOP1
         t_reg = t.time() / DIVIDER;
         cout << "regex<char> time: " << t_reg << "us (" << (t.minval() / DIVIDER) << "," << (t.maxval() / DIVIDER) << ")" << endl;
         t.reset();
         if(result)
         {
            for(i = 0; i < m1.size(); ++i)
            {
               //string_type temp(m1[i].first, m1[i].second);
               cout << "match " << i << ": \"";
               cout.write(m1[i].first, m1[i].second - m1[i].first);
               cout << "\" (matched=" << m1[i].matched << ")" << endl;
            }
            cout << "match $`: \"";
            cout.write(m1[-1].first, m1[-1].second - m1[-1].first);
            cout << "\" (matched=" << m1[-1].matched << ")" << endl;
            cout << "match $': \"";
            cout.write(m1[-2].first, m1[-2].second - m1[-2].first);
            cout << "\" (matched=" << m1[-2].matched << ")" << endl << endl;
         }

         // measure time interval for reg_expression<wchar_t>
         START1
         for(i =0; i < iters; ++i)
         {
            START2
            result = reg_search(ws2.begin(), ws2.end(), m2, e2);
            STOP2
         }
         STOP1
         cout << "regex<wchar_t> time: " << t_reg << "us (" << (t.minval() / DIVIDER) << "," << (t.maxval() / DIVIDER) << ")" << endl;
         t.reset();
         if(result)
         {
            for(i = 0; i < m2.size(); ++i)
            {
               wstring_type temp(m2[i].first, m2[i].second);
               string_type temp2 = narrow(temp);
               cout << "match " << i << ": \"" << temp2;
               cout << "\" (matched=" << m1[i].matched << ")" << endl;
            }
            cout << "match $`: \"";
            wstring_type temp(m2[-1].first, m2[-1].second);
            string_type temp2 = narrow(temp);
            cout << temp2;
            cout << "\" (matched=" << m1[-1].matched << ")" << endl;
            cout << "match $': \"";
            wstring_type temp3(m2[-2].first, m2[-2].second);
            string_type temp4 = narrow(temp3);
            cout << temp4;
            cout << "\" (matched=" << m1[-2].matched << ")" << endl << endl;
         }

         // measure time interval for reg_expression<char> using a deque
         START1
         for(i =0; i < iters; ++i)
         {
            START2
            result = reg_search(ds.begin(), ds.end(), m3, e1);
            STOP2
         }
         STOP1
         t_reg = t.time() / DIVIDER;
         cout << "regex<std::deque<char>::iterator>: " << t_reg << "us (" << (t.minval() / DIVIDER) << "," << (t.maxval() / DIVIDER) << ")" << endl;
         t.reset();

         if(result)
         {
            string_type temp;
            for(i = 0; i < m3.size(); ++i)
            {
               __JM_STD::copy(deque_type::iterator(m3[i].first), deque_type::iterator(m3[i].second), __JM_STD::back_inserter(temp));
               cout << "match " << i << ": \"" << temp;
               cout << "\" (matched=" << m1[i].matched << ")" << endl;
               temp.erase(temp.begin(), temp.end());
            }
            cout << "match $`: \"";
            __JM_STD::copy(deque_type::iterator(m3[-1].first), deque_type::iterator(m3[-1].second), __JM_STD::back_inserter(temp));
            cout << temp;
            temp.erase(temp.begin(), temp.end());
            cout << "\" (matched=" << m1[-1].matched << ")" << endl;
            cout << "match $': \"";
            __JM_STD::copy(deque_type::iterator(m3[-2].first), deque_type::iterator(m3[-2].second), __JM_STD::back_inserter(temp));
            cout << temp;
            temp.erase(temp.begin(), temp.end());
            cout << "\" (matched=" << m1[-2].matched << ")" << endl << endl;
         }

         // measure time interval for POSIX matcher:
         s2.push_back('\0');
         START1
         for(i =0; i < iters; ++i)
         {
            START2
            result = regexec(&r, s2.begin(), n, m, 0);
            STOP2
         }
         STOP1
         t_posix = t.time() / DIVIDER;
         cout << "POSIX regexec time: " << t_posix << "us (" << (t.minval() / DIVIDER) << "," << (t.maxval() / DIVIDER) << ")" << endl;
         t.reset();
         if(result == 0)
         {
            for(i = 0; i < n; ++i)
            {
               if(m[i].rm_so >= 0)
               {
                  string_type temp(s2.begin() + m[i].rm_so, s2.begin() + m[i].rm_eo);
                  cout << "match " << i << ": \"" << temp << "\"" << endl;
               }
               else
                  cout << "match " << i << ": \"\"" << endl;   // no match
            }
         }
      }
      regfree(&r);
      free(m);
   }
   return 0;
}


wstring_type widen(const string_type& s)
{
   wstring_type result;
   for(unsigned i = 0; i < s.size(); ++i)
      result.push_back(s[i]);
   return result;
}

string_type narrow(const wstring_type& s)
{
   string_type result;
   for(unsigned i = 0; i < s.size(); ++i)
      result.push_back(s[i]);
   return result;
}

void to_deque(deque_type& d, const string_type& s)
{
   d.erase(d.begin(), d.end());
   string_type::const_iterator i, j;
   i = s.begin();
   j = s.end();
   while(i != j)
   {
      d.push_back(*i);
      ++i;
   }
}

istream& getline(istream& is, string_type& s, char delim)
{
   char c = (char)is.get();
   s.erase(s.begin(), s.end());
   while(c != delim)
   {
      s.push_back(c);
      c = (char)is.get();
   }
   return is;
}





