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
  *	FILE     tests.cpp
  *	VERSION  2.00
  *
  * the actual tests conducted by regress.
  *
  */


#include <regex.h>
#include "regress.h"
#include "rlocale.h"


template <class C, class T, class A>
void cpp_eh_tests(const reg_expression<C, T, A>& )
{
   bool thrown = false;
   // try set_expression form first:
   try
   {
      A a;
      reg_expression<C, T, A> e(a);
      e.set_expression(expression.begin(), flags[2] | regbase::use_except);
   }
   catch(const __JM::bad_expression&)
   {
      thrown = true;
   }
   catch(...){}
   if(!thrown)
   {
      begin_error();
      cout << "Error: expected exception not thrown" << endl;
   }

   // now try constructor form:
   thrown = false;
   try
   {
      A a;
      reg_expression<C, T, A> e(expression.begin(), flags[2] | regbase::use_except, a);
   }
   catch(const __JM::bad_expression&)
   {
      thrown = true;
   }
   catch(...){}
   if(!thrown)
   {
      begin_error();
      cout << "Error: expected exception not thrown" << endl;
   }

}

debug_iterator<string_type::iterator> find_last_line(debug_iterator<string_type::iterator> start, debug_iterator<string_type::iterator> end)
{
   debug_iterator<string_type::iterator> result = start;
   while(start != end)
   {
      if(*start == '\n')
      {
         ++start;
         result = start;
      }
      else
         ++start;
   }
   return result;
}

unsigned int count_lines(debug_iterator<string_type::iterator> start, debug_iterator<string_type::iterator> end)
{
   unsigned int result = 0;
   while(start != end)
   {
      if(*start == '\n')
         ++result;
      ++start;
   }
   return result;
}

class grep_test_predicate
{
   int match_id;
   debug_iterator<string_type::iterator> base, term;
public:
   grep_test_predicate(debug_iterator<string_type::iterator> i, debug_iterator<string_type::iterator> j) : base(i), term(j) { match_id = 0; }
   ~grep_test_predicate(){}
   bool operator()(const __JM::reg_match< debug_iterator<string_type::iterator>, jm_debug_alloc >& m);
};

bool grep_test_predicate::operator()(const __JM::reg_match< debug_iterator<string_type::iterator>, jm_debug_alloc >& m)
{
   int start, end;
   start = m[0].first - base;
   end = m[0].second - base;
   if((matches[match_id] != start) || (matches[match_id + 1] != end))
   {
      begin_error();
      cout << "regex++ grep match error: found [" << start << "," << end << "] expected [" << matches[match_id] << "," << matches[match_id+1] << "]" << endl;
   }

   //
   // check $`:
   start = m[-1].first - base;
   end = m[-1].second - base;
   if(match_id && 
      ( (end != matches[match_id]) || (start != matches[match_id - 1]) )
   )
   {
      begin_error();
      cout << "regex++ grep error in $`: found [" << start << "," << end << "] expected [" << matches[match_id-1] << "," << matches[match_id] << "]" << endl;
   }
   else if((!match_id) && ((start != 0) || (end != matches[0])))
   {
      begin_error();
      cout << "regex++ grep error in $`: found [" << start << "," << end << "] expected [" << 0 << "," << matches[0] << "]" << endl;
   }

   //
   // check $':
   start = m[-2].first - base;
   end = m[-2].second - base;
   if((start != matches[match_id + 1]) || (end != (term-base)))
   {
      begin_error();
      cout << "regex++ grep error in $': found [" << start << "," << end << "] expected [" << matches[match_id + 1] << "," << (term-base) << "]" << endl;
   }

   //
   // now check line()
   start = m.line();
   end = count_lines(base, m[0].first) + 1;
   if(start != end)
   {
      begin_error();
      cout << "regex++ grep error in line(): found " << start << " expected " << end << endl;
   }

   // 
   // now check line_start()
   start = m.line_start() - base;
   end = find_last_line(base, m[0].first) - base;
   if(start != end)
   {
      begin_error();
      cout << "regex++ grep error in line_start(): found " << start << " expected " << end << endl;
   }

   match_id += 2;
   return true;
}


template <class C, class T, class A>
void cpp_tests(const reg_expression<C, T, A>& e, bool recurse = true)
{
   if(flags[4] & REG_MERGE)
   {
      //
      // test merge code:
      //
      string_type s;
      reg_merge(__JM_STD::back_inserter(s), __text.begin(), __text.end() - 1, e, format_string.begin(), bool(flags[4] & REG_MERGE_COPY));
      s.push_back(0);
      if(s != merge_string)
      {
         begin_error();
         cout << "merge result mismatch: found \"" << s << "\" expected \"" << merge_string << "\"" << endl;
      }
      return;
   }
   
   
   if(recurse)
   {
      // copy and assign test:
      reg_expression<C, T, A> e2(e);
      cpp_tests(e2, false);
      e2 = e;
      cpp_tests(e2, false);
   }
   
   if(e.error_code())
   {
      // verify that we don't expect expression to compile
      if((*__text.begin() != JM_STR('!')) || (*(__text.begin() + 1) != 0))
      {
         begin_error();
         cout << "Expression did not compile using regex++ API" << endl;
      }
      #ifndef JM_NO_EXCEPTIONS
      else if(recurse)
         cpp_eh_tests(e);
      #endif
   }
   else if(flags[4] & REG_GREP)
   {
      // try to do grep:
      debug_iterator<string_type::iterator> x(__text.begin(), __text.begin(), __text.end() - 1);
      debug_iterator<string_type::iterator> y(__text.end() - 1, __text.begin(), __text.end() - 1);
      grep_test_predicate oi(x, y);
      reg_grep(oi, x, y, e, flags[3]);
   }
   else
   {
      // try to find match
      reg_match< debug_iterator<string_type::iterator>, jm_def_alloc > m;
      debug_iterator<string_type::iterator> x(__text.begin(), __text.begin(), __text.end() - 1);
      debug_iterator<string_type::iterator> y(__text.end() - 1, __text.begin(), __text.end() - 1);
      if(reg_search(x, y, m, e, flags[3]))
      {
         // match found compare what matched with what we expect:
         int j = 0;
         for(unsigned int i = 0; i < m.size(); ++i, j += 2)
         {
            if(m[i].matched == false)
            {
               if(matches[j] != -1)
               {
                  begin_error();
                  cout << "regex++ API result mismatch in sub-expression " << i <<
                          ", found (" << (m[i].first - x) << "," <<
                          (m[i].second - x) << ") expected (" <<
                          matches[j] << "," << matches[j+1] << ")" << endl;
               }
            }
            else if(((m[i].first - __text.begin()) != matches[j]) || ((m[i].second - __text.begin()) != matches[j+1]))
            {
               begin_error();
               cout << "regex++ API result mismatch in sub-expression " << i <<
                       ", found (" << (m[i].first - x) << "," <<
                       (m[i].second - x) << ") expected (" <<
                       matches[j] << "," << matches[j+1] << ")" << endl;
            }
         }
         //
         // now check $` and $':
         //
         if((m[-1].first != x) || (m[-1].second != m[0].first))
         {
            begin_error();
            cout << "regex++ API result mismatch in $` (match -1), found (" <<
               (m[-1].first - x) << "," << (m[-1].second - x) << ") expected (0" <<
               "," << matches[0] << ")" << endl;
         }
         if((m[-2].first != m[0].second) || (m[-2].second != y))
         {
            begin_error();
            cout << "regex++ API result mismatch in $' (match -2), found (" <<
               (m[-2].first - x) << "," << (m[-2].second - x) << ") expected (" <<
               matches[1] << "," << (y-x) << ")" << endl;
         }
      }
      else
      {
         // match not found
         if(matches[0] != -1)
         {
            begin_error();
            cout << "Match expected but not found using regex++ API" << endl;
         }
      }
   }
}

#if !defined(JM_NO_EXCEPTIONS) && !defined(JM_NO_STRING_H) && !defined(TEST_UNICODE)

unsigned int hl_match_id;

bool hl_grep_test_proc(const RegEx& e)
{
   int start, end;
   start = e.Position(0);
   end = start + e.Length();
   if((matches[hl_match_id] != start) || (matches[hl_match_id + 1] != end))
   {
      begin_error();
      cout << "class RegEx grep match error: found [" << start << "," << end << "] expected [" << matches[hl_match_id] << "," << matches[hl_match_id+1] << "]" << endl;
   }

   //
   // check $`:
   start = e.Position(-1);
   end = start + e.Length(-1);
   if(start == -1)
   {
      if(hl_match_id && 
         ( matches[hl_match_id] != matches[hl_match_id - 1] )
      )
      {
         begin_error();
         cout << "class RegEx grep error in $`: found [" << start << "," << end << "] expected [" << matches[hl_match_id-1] << "," << matches[hl_match_id] << "]" << endl;
      }
      else if((!hl_match_id) && (0 != matches[0]))
      {
         begin_error();
         cout << "class RegEx grep error in $`: found [" << start << "," << end << "] expected [" << 0 << "," << matches[0] << "]" << endl;
      }
   }
   else
   {
      if(hl_match_id && 
         ( (end != matches[hl_match_id]) || (start != matches[hl_match_id - 1]) )
      )
      {
         begin_error();
         cout << "class RegEx grep error in $`: found [" << start << "," << end << "] expected [" << matches[hl_match_id-1] << "," << matches[hl_match_id] << "]" << endl;
      }
      else if((!hl_match_id) && ((start != 0) || (end != matches[0])))
      {
         begin_error();
         cout << "class RegEx grep error in $`: found [" << start << "," << end << "] expected [" << 0 << "," << matches[0] << "]" << endl;
      }
   }

   //
   // check $':
   start = e.Position(-2);
   end = start + e.Length(-2);
   if(start == -1)
   {
      if(matches[hl_match_id + 1] != __text.size() - 1)
      {
         begin_error();
         cout << "class RegEx grep error in $': found [" << start << "," << end << "] expected [" << matches[hl_match_id + 1] << "," << (__text.size() - 1) << "]" << endl;
      }
   }
   else if((start != matches[hl_match_id + 1]) || (end != __text.size() - 1))
   {
      begin_error();
      cout << "class RegEx grep error in $': found [" << start << "," << end << "] expected [" << matches[hl_match_id + 1] << "," << (__text.size() - 1) << "]" << endl;
   }

   hl_match_id += 2;
   return true;
}

void cpp_hl_tests(RegEx& e, bool recurse = true)
{
   if(flags[4] & REG_MERGE)
      return;

   if(recurse)
   {
      // copy and assign test:
      RegEx e2(e);
      cpp_hl_tests(e2, false);
      e2 = e;
      cpp_hl_tests(e2, false);
   }

   if(flags[4] & REG_GREP)
   {
      // try to do grep:
      hl_match_id = 0;
      GrepCallback cb = hl_grep_test_proc;
      e.Grep(cb, __text.begin(), flags[3]);
   }
   else
   {
      if(e.Search(__text.begin(), flags[3]))
      {
         unsigned int i = 0;
         unsigned int j = 0;
         while(matches[j] != -2)
         {
            if( (matches[j] != e.Position(i)) || (matches[j+1] - matches[j] != e.Length(i)) )
            {
               begin_error();
               cout << "RegEx::Search error in subexpression " << i << ": found [" << e.Position(i) << "," << (e.Position(i) + e.Length(i)) << "] expected [" << matches[j] << "," << matches[j+1] << "]" << endl;
            }
            ++i;
            j += 2;
         }
      }
      else
      {
         if(matches[0] != -1)
         {
            begin_error();
            cout << "match expected but not found with RexEx::Search" << endl;
         }
      }
   }
}
#endif

template <class iterator>
bool has_nulls(iterator i, iterator j)
{
   while(i != j)
   {
      if(*i == 0)
         return true;
      ++i;
   }
   return false;
}

#ifdef TEST_UNICODE
#undef regcomp
#undef regerror
#undef regexec
#undef regfree
#undef regex_t
#define regcomp regcompW
#define regerror regerrorW
#define regexec regexecW
#define regfree regfreeW
#define regex_t regex_tW
#endif


void run_tests()
{
#ifndef TEST_UNICODE
   if(flags[4] & REG_UNICODE_ONLY)
      return;
#endif
#ifndef NO_CPP_TEST
   #ifndef JM_NO_EXCEPTIONS
   try
   {
   #endif
      unsigned int f = flags[2] & ~regbase::use_except;
      if(flags[0] & REG_ICASE)
         f |= regbase::icase;
      reg_expression<char_t, char_regex_traits<char_t>, jm_debug_alloc > e(expression.begin(), f);
      cpp_tests(e, true);
   #ifndef JM_NO_EXCEPTIONS
   }
   #ifndef JM_NO_EXCEPTION_H
   catch(const __JM_STD::exception& e)
   {
      begin_error();
      cout << "Unexpected exception thrown from C++ library: " << e.what() << endl;
   }
   #endif
   catch(...)
   {
      begin_error();
      cout << "Unexpected exception thrown from C++ library" << endl;
   }
   #endif
#endif

#if !defined(JM_NO_EXCEPTIONS) && !defined(JM_NO_STRING_H) && !defined(TEST_UNICODE)
   try
   {
      if((flags[2] == regbase::normal) && (has_nulls(__text.begin(), __text.end() - 1) == false))
      {
         RegEx e;
         e.SetExpression(expression.begin(), flags[0] & REG_ICASE);
         cpp_hl_tests(e, true);
      }
   }
   catch(const bad_expression& e)
   {
      if(*__text.begin() != '!')
      {
         begin_error();
         cout << "Expression did not compile with class RegEx" << endl;
      }
   }
   catch(...)
   {
      begin_error();
      cout << "Unexpected exception thrown from RegEx::SetExpression" << endl;
   }
#endif

   if(flags[4] & (REG_NO_POSIX_TEST | REG_GREP | REG_MERGE | REG_MERGE_COPY))
      return;
   regex_t posix_expression;
   regmatch_t m[MAX_MATCHES];
   if(regcomp(&posix_expression, expression.begin(), flags[0]) == 0)
   {
      if(flags[1] & REG_STARTEND)
      {
         m[0].rm_so = 0;
         m[0].rm_eo = __text.size() - 1;
      }
      if(regexec(&posix_expression, __text.begin(), MAX_MATCHES, m, flags[1]))
      {
         // match not found
         if(matches[0] != -1)
         {
            begin_error();
            cout << "Match expected but not found using POSIX API" << endl;
         }
      }
      else
      {
         // match found compare what matched with what we expect:
         int j = 0;
         for(unsigned int i = 0; i <= posix_expression.re_nsub; ++i, j += 2)
         {
            if((m[i].rm_so != matches[j]) || (m[i].rm_eo != matches[j+1]))
            {
               begin_error();
               cout << "POSIX API result mismatch in sub-expression " << i << ", found (" << m[i].rm_so << "," << m[i].rm_eo <<
                        ") expected (" << matches[j] << "," << matches[j+1] << ")" << endl;
            }
         }
      }
      regfree(&posix_expression);
   }
   else
   {
      // expression did not compile
      if((*__text.begin() != JM_STR('!')) || (*(__text.begin() + 1) != 0))
      {
         begin_error();
         cout << "Expression did not compile using POSIX API" << endl;
      }
   }
}



//
// error reporting:
int last_line = 0;

void begin_error()
{
   if(line != last_line)
   {
      cout << "Error in line " << line << " of file " << file << endl;
      cout << "Expression: " << expression << endl;
      cout << "Search text: " << __text << endl;
      cout << "Flags: ";
      bool started = false;
      unsigned int id = 0;
      while(flag_data[id].len != 0)
      {
         if(flags[flag_data[id].id] & flag_data[id].value)
         {
            if(started)
               cout << " | ";
            cout << flag_data[id].name;
            started = true;
         }
         ++id;
      }
      cout << endl;
      last_line = line;
   }
}

void reset_error()
{
   last_line = 0;
}







