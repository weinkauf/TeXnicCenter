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
  *	FILE     regress.h
  *	VERSION  2.00
  *
  * Function and data declarations for regress.
  *
  */

#ifndef __REGRESS_H
#define __REGRESS_H

#ifdef OLD_IOSTREAM
#include <iostream.h>
#else
#include <iostream>
using std::cout;
using std::cerr;
using std::cin;
using std::istream;
using std::ostream;
using std::endl;
#endif
#include <jm/jm_cfg.h>
#include <vector>
#include <regex>

#ifdef TEST_UNICODE
typedef wchar_t char_t;
#define NO_POSIX_TEST
typedef __JM_STD::vector<char_t> string_type;
typedef __JM_STD::vector<char> nstring_type;
istream& get_line(istream& is, nstring_type& s, char delem = '\n');
istream& get_line(istream& is, string_type& s, char delem = L'\n');
#define JM_STR(x) L##x
#else
typedef char char_t;
typedef __JM_STD::vector<char_t> string_type;
istream& get_line(istream& is, string_type& s, char delem = '\n');
#define JM_STR(x) x
#endif

ostream& operator << (ostream& os, const string_type& s);
ostream& operator << (ostream& os, const wchar_t* s);
void parse_input_line(const string_type& s);
void expand_escapes(string_type& s);
void run_tests();
void begin_error();
void reset_error();
int to_int(string_type::const_iterator i, string_type::const_iterator j);

#define MAX_MATCHES 30
extern string_type expression;
extern string_type __text;
extern string_type format_string;
extern string_type merge_string;
extern int matches[MAX_MATCHES];

#ifndef MAX_PATH
#define MAX_PATH 256
#endif

extern char file[MAX_PATH];
extern int line;
extern bool do_test;
extern unsigned int flags[5];

struct flag_info
{
   const char_t* name; // printable flag name
   unsigned int len;   // length of name
   unsigned int value; // value of the flag
   unsigned int id;    // index to assign value to
};

extern flag_info flag_data[];

//
// class jm_debug_alloc
// NB this is a byte based allocator
//
class jm_debug_alloc
{
private:
   int* blocks, *count;
   jm_debug_alloc* guard;
public:
   typedef size_t    size_type;
   typedef ptrdiff_t difference_type;
   typedef char*        pointer;
   typedef const char*  const_pointer;
   typedef char&        reference;
   typedef const char&  const_reference;
   typedef char         value_type;


   #ifndef JM_NO_MEMBER_TEMPLATES
   template <class U>
   struct rebind
   {
      typedef re_alloc_binder<U, jm_debug_alloc> other;
   };
   #endif

   jm_debug_alloc()
   {
      blocks = new int;
      count = new int;
      *blocks = 0;
      *count = 1;
      guard = this;
   }

   jm_debug_alloc(const jm_debug_alloc& d)
   {
      blocks = d.blocks;
      count = d.count;
      ++(*count);
      guard = this;
   }
   jm_debug_alloc& operator=(const jm_debug_alloc& d)
   {
      free();
      blocks = d.blocks;
      count = d.count;
      ++(*count);
      return *this;
   }

   ~jm_debug_alloc()
   {
      if(guard != this)
      {
         cout << "Error: attempt to destroy object already destroyed" << endl;
      }
      else
      {
         free();
         guard = 0;
      }
   }
   void free()
   {
       if(--(*count) == 0)
       {
          if(*blocks)
          {
             begin_error();
             cout << "Error: " << *blocks << " blocks not freed by allocator" << endl;
          }
          delete count;
          delete blocks;
       }
   }
   static pointer address(reference x)
   {
      return &x;
   }
   static const_pointer address(const_reference x)
   {
      return &x;
   }
   pointer allocate(size_type n, void* hint = 0)
   {
      pointer p = new char[n + sizeof(size_type)];
      *(size_type*)p = n;
      ++(*blocks);
      return p + sizeof(int);
   }
   void deallocate(pointer p, size_type n)
   {
      p -= sizeof(size_type);
      if(*(size_type*)p != n)
      {
         begin_error();
         cout << "Error: size mismatch in allocate/deallocate calls" << endl;
      }
      --(*blocks);
      delete[] p;
   }

   static size_type max_size()
   {
      return -1;
   }

   static void construct(pointer , const char& )
   {
   }

   static void destroy(pointer )
   {
   }
};

#ifdef __BORLANDC__
#if __BORLANDC__ < 0x520

#define JM_BORLAND_FIX

#endif
#endif



//
// class debug_iterator
//
template <class T>
struct debug_iterator : public JM_RA_ITERATOR(T, ptrdiff_t)
{
  typedef debug_iterator<T> iterator;
  typedef char_t reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef debug_iterator self;

  T cur;
  T first;
  T last;

  debug_iterator(T c, T f, T l)
    : cur(c), first(f), last(l) {}
  debug_iterator() : cur(0), first(0), last(0) {}
  debug_iterator(const iterator& x)
    : cur(x.cur), first(x.first), last(x.last) {}

  reference operator*() const
  {
      if(cur == last)
      {
         begin_error();
         cout << "Error: attempt to dereference iterator past range" << endl;
      }
      return *cur;
  }

  operator T()const { return cur; }

  difference_type operator-(const self& x) const {
    return (difference_type)(cur - x.cur);
  }

  self& operator++()
  {
      if(cur == last)
      {
         begin_error();
         cout << "Error: attempt to increment iterator past range" << endl;
         return *this;
      }
    ++cur;
    return *this;
  }
  self operator++(int)
  {
    self tmp = *this;
    ++*this;
    return tmp;
  }

  self& operator--()
  {
      if(cur == first)
      {
         begin_error();
         cout << "Error: attempt to decrement iterator past range" << endl;
         return *this;
      }
    --cur;
    return *this;
  }
  self operator--(int)
  {
    self tmp = *this;
    --*this;
    return tmp;
  }

  self& operator+=(difference_type n)
  {
      if((last - cur) < n)
      {
         begin_error();
         cout << "Error: attempt to increment iterator past range" << endl;
         return *this;
      }
    cur += n;
    return *this;
  }

  self operator+(difference_type n) const
  {
    self tmp = *this;
    return tmp += n;
  }

  self& operator-=(difference_type n) { return *this += -n; }
 
  self operator-(difference_type n) const
  {
    self tmp = *this;
    return tmp -= n;
  }

  reference operator[](difference_type n) const { return *(cur + n); }

  bool operator==(const self& x) const { return cur == x.cur; }
  bool operator!=(const self& x) const { return !(*this == x); }
  bool operator<(const self& x) const {
    return (cur < x.cur);
  }
};


#define REG_NO_POSIX_TEST 1
#define REG_UNICODE_ONLY 2
#define REG_GREP 4
#define REG_MERGE 8
#define REG_MERGE_COPY 16

#endif






