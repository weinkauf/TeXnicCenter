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
  *	FILE     regex.h
  *	VERSION  2.00
  */


/* start with C compatability API */

#ifndef __REGEX_H
#define __REGEX_H

#include <cregex>

#ifdef __cplusplus

// what follows is all C++ don't include in C builds!!

#include <new.h>
#include <string.h>
#include <jm/jstack.h>
#include <jm/re_raw.h>
#include <jm/re_nls.h>
#include <jm/regfac.h>
#include <jm/re_cls.h>
#include <jm/re_coll.h>
#include <jm/re_kmp.h>


JM_NAMESPACE(__JM)

//
// define error hanling classes
#if !defined(JM_NO_EXEPTIONS) && !defined(JM_NO_EXCEPTION_H)
// standard classes are available:

class JM_IX_DECL bad_expression : public __JM_STD::exception
{
#ifdef RE_LOCALE_CPP
   __JM_STD::string code;
public:
   bad_expression(const __JM_STD::string& s) : code(s) {}
#else
   unsigned int code;
public:
   bad_expression(unsigned int err) : code(err) {}
#endif
   bad_expression(const bad_expression& e) : __JM_STD::exception(e), code(e.code) {}
   bad_expression& operator=(const bad_expression& e)
   {
      #ifdef _MSC_VER
      static_cast<__JM_STD::exception*>(this)->operator=(e);
      #else
      __JM_STD::exception::operator=(e);
      #endif
      code = e.code;
      return *this;
   }
   virtual const char* what()const throw();
};

#elif !defined(JM_NO_EXEPTIONS)
// no standard classes, do it ourselves:

class JM_IX_DECL bad_expression
{
#ifdef RE_LOCALE_CPP
   __JM_STD::string code;
public:
   bad_expression(const __JM_STD::string& s) : code(s) {}
#else
   unsigned int code;
public:
   bad_expression(unsigned int err) : code(err) {}
#endif
   bad_expression(const bad_expression& e) : code(e.code) {}
   bad_expression& operator=(const bad_expression& e) { code = e.code; return *this; }
   virtual const char* what()const throw();
};

#endif

//
// define default traits classes for char and wchar_t types:
//

struct re_set_long;
struct re_syntax_base;

enum char_syntax_type
{
   syntax_char = 0,
   syntax_open_bracket = 1,                  // (
   syntax_close_bracket = 2,                 // )
   syntax_dollar = 3,                        // $
   syntax_caret = 4,                         // ^
   syntax_dot = 5,                           // .
   syntax_star = 6,                          // *
   syntax_plus = 7,                          // +
   syntax_question = 8,                      // ?
   syntax_open_set = 9,                      // [
   syntax_close_set = 10,                    // ]
   syntax_or = 11,                           // |
   syntax_slash = 12,                        //
   syntax_hash = 13,                         // #
   syntax_dash = 14,                         // -
   syntax_open_brace = 15,                   // {
   syntax_close_brace = 16,                  // }
   syntax_digit = 17,                        // 0-9
   syntax_b = 18,                            // for \b
   syntax_B = 19,                            // for \B
   syntax_left_word = 20,                    // for \<
   syntax_right_word = 21,                   // for \>
   syntax_w = 22,                            // for \w
   syntax_W = 23,                            // for \W
   syntax_start_buffer = 24,                 // for \`
   syntax_end_buffer = 25,                   // for \'
   syntax_newline = 26,                      // for newline alt
   syntax_comma = 27,                        // for {x,y}

   syntax_a = 28,                            // for \a
   syntax_f = 29,                            // for \f
   syntax_n = 30,                            // for \n
   syntax_r = 31,                            // for \r
   syntax_t = 32,                            // for \t
   syntax_v = 33,                            // for \v
   syntax_x = 34,                            // for \xdd
   syntax_c = 35,                            // for \cx
   syntax_colon = 36,                        // for [:...:]
   syntax_equal = 37,                        // for [=...=]
   
   // perl ops:
   syntax_e = 38,                            // for \e
   syntax_l = 39,                            // for \l
   syntax_L = 40,                            // for \L
   syntax_u = 41,                            // for \u
   syntax_U = 42,                            // for \U
   syntax_s = 43,                            // for \s
   syntax_S = 44,                            // for \S
   syntax_d = 45,                            // for \d
   syntax_D = 46,                            // for \D
   syntax_E = 47,                            // for \Q\E
   syntax_Q = 48,                            // for \Q\E
   syntax_X = 49,                            // for \X
   syntax_C = 50,                            // for \C
   syntax_Z = 51,                            // for \Z
   syntax_G = 52,                            // for \G

   syntax_max = 53,
};

template <class charT>
class char_regex_traits
{
public:
   typedef charT char_type;
   //
   // uchar_type is the same size as char_type
   // but must be unsigned:
   typedef charT uchar_type;
   //
   // size_type is normally the same as charT
   // but could be unsigned int to improve performance
   // of narrow character types, NB must be unsigned:
   typedef unsigned size_type;

   // length:
   // returns the length of a null terminated string
   // can be left unimplimented for non-character types.
   static size_t length(const char_type* );

   // syntax_type
   // returns the syntax type of a given charT
   // translates customised syntax to a unified enum.
   static unsigned int syntax_type(size_type c);

   // translate:
   //
   static charT RE_CALL translate(charT c, bool icase
   #ifdef RE_LOCALE_CPP
   , const __JM_STD::locale&
   #endif
   );

   // transform:
   //
   // converts a string into a sort key for locale dependant
   // character ranges.
   static void RE_CALL transform(re_str<charT>& out, const re_str<charT>& in
   #ifdef RE_LOCALE_CPP
   , const __JM_STD::locale&
   #endif
   );

   // transform_primary:
   //
   // converts a string into a primary sort key for locale dependant
   // equivalence classes.
   static void RE_CALL transform_primary(re_str<charT>& out, const re_str<charT>& in
   #ifdef RE_LOCALE_CPP
   , const __JM_STD::locale&
   #endif
   );

   // is_separator
   // returns true if c is a newline character
   static bool RE_CALL is_separator(charT c);

   // is_combining
   // returns true if the character is a unicode
   // combining character
   static bool RE_CALL is_combining(charT c);

   // is_class
   // returns true if the character is a member
   // of the specified character class
   static bool RE_CALL is_class(charT c, unsigned int f
   #ifdef RE_LOCALE_CPP
   , const __JM_STD::locale&
   #endif
   );

   // toi
   // converts c to integer
   static int RE_CALL toi(charT c
   #ifdef RE_LOCALE_CPP
   , const __JM_STD::locale&
   #endif
   );

   // toi
   // converts multi-character value to int
   // updating first as required
   static int RE_CALL toi(const charT*& first, const charT* last, int radix
   #ifdef RE_LOCALE_CPP
   , const __JM_STD::locale&
   #endif
   );

   // lookup_classname
   // parses a class declaration of the form [:class:]
   // On entry first points to the first character of the class name.
   // 
   static unsigned int RE_CALL lookup_classname(const charT* first, const charT* last
   #ifdef RE_LOCALE_CPP
   , const __JM_STD::locale&
   #endif
   );

   // lookup_collatename
   // parses a collating element declaration of the form [.collating_name.]
   // On entry first points to the first character of the collating element name.
   // 
   static bool RE_CALL lookup_collatename(re_str<charT>& s, const charT* first, const charT* last
   #ifdef RE_LOCALE_CPP
   , const __JM_STD::locale&
   #endif
   );

};

JM_TEMPLATE_SPECIALISE
class char_regex_traits<char>
{
public:
   typedef char char_type;
   typedef unsigned char uchar_type;
   typedef unsigned int size_type;
   static size_t RE_CALL length(const char_type* p)
   {
      return strlen(p);
   }
   static unsigned int RE_CALL syntax_type(size_type c
   #ifdef RE_LOCALE_CPP
   , const __JM_STD::locale& l
   #endif
   )
   {
      #ifdef RE_LOCALE_CPP
      return JM_USE_FACET(l, regfacet<char>).syntax_type((char)c);
      #else
      return re_syntax_map[c];
      #endif
   }
   static char RE_CALL translate(char c, bool icase
   #ifdef RE_LOCALE_CPP
   , const __JM_STD::locale& l
   #endif
   )
   {
      #ifdef RE_LOCALE_CPP
      return icase ? JM_USE_FACET(l, __JM_STD::ctype<char>).tolower((char_type)c) : c;
      #else
      return icase ? re_lower_case_map[(size_type)(uchar_type)c] : c;
      #endif
   }
   static void RE_CALL transform(re_str<char>& out, const re_str<char>& in
   #ifdef RE_LOCALE_CPP
   , const __JM_STD::locale& l
   #endif
   )
   {
#ifndef RE_LOCALE_CPP
      re_transform(out, in);
#else
      out = JM_USE_FACET(l, __JM_STD::collate<char>).transform(in.c_str(), in.c_str() + in.size()).c_str();
#endif
   }

   static void RE_CALL transform_primary(re_str<char>& out, const re_str<char>& in
   #ifdef RE_LOCALE_CPP
   , const __JM_STD::locale& l
   #endif
   )
   {
      transform(out, in MAYBE_PASS_LOCALE(l));
#ifdef RE_LOCALE_W32
      re_trunc_primary(out);
#else
      unsigned n = in.size() + out.size() / 4;
      if(n < out.size())
         out[n] = 0;
#endif
   }

   static bool RE_CALL is_separator(char c)
   {
      return JM_MAKE_BOOL(c == '\n');
   }

   static bool RE_CALL is_combining(char)
   {
      return false;
   }
   
   static bool RE_CALL is_class(char c, unsigned int f
#ifdef RE_LOCALE_CPP
   , const __JM_STD::locale& l
#endif
   )
   {
      #ifdef RE_LOCALE_CPP
      if(JM_USE_FACET(l, __JM_STD::ctype<char>).is((__JM_STD::ctype<char>::mask)(f & char_class_all_base), c))
         return true;
      if((f & char_class_underscore) && (c == '_'))
         return true;
      if((f & char_class_blank) && ((c == ' ') || (c == '\t')))
         return true;
      return false;
      #else
      return JM_MAKE_BOOL(re_class_map[(size_type)(uchar_type)c] & f);
      #endif
   }
   static int RE_CALL toi(char c
   #ifdef RE_LOCALE_CPP
   , const __JM_STD::locale& l
   #endif
   )
   {
      return re_toi(c MAYBE_PASS_LOCALE(l));
   }
   static int RE_CALL toi(const char*& first, const char* last, int radix
   #ifdef RE_LOCALE_CPP
   , const __JM_STD::locale& l
   #endif
   )
   {
      return re_toi(first, last, radix MAYBE_PASS_LOCALE(l));
   }

   static unsigned int RE_CALL lookup_classname(const char* first, const char* last
   #ifdef RE_LOCALE_CPP
   , const __JM_STD::locale& l
   #endif
   )
   {
      #ifdef RE_LOCALE_CPP
      return JM_USE_FACET(l, regfacet<char>).lookup_classname(first, last);
      #else
      return re_lookup_class(first, last);
      #endif
   }

   static bool RE_CALL lookup_collatename(re_str<char>& s, const char* first, const char* last
   #ifdef RE_LOCALE_CPP
   , const __JM_STD::locale& l
   #endif
   )
   {
      #ifdef RE_LOCALE_CPP
      re_str<char> n(first, last);
      return JM_USE_FACET(l, regfacet<char>).lookup_collatename(s, n);
      #else
      return re_lookup_collate(s, first, last);
      #endif
   }
};

JM_TEMPLATE_SPECIALISE
class char_regex_traits<wchar_t>
{
public:
   typedef wchar_t char_type;
   typedef unsigned short uchar_type;
   typedef unsigned int size_type;
   static size_t RE_CALL length(const char_type* p)
   {
      return wcslen(p);
   }
   static unsigned int RE_CALL syntax_type(size_type c
   #ifdef RE_LOCALE_CPP
   , const __JM_STD::locale& l
   #endif
   )
   {
      #ifdef RE_LOCALE_CPP
      return JM_USE_FACET(l, regfacet<wchar_t>).syntax_type((wchar_t)c);
      #else
      return re_get_syntax_type(c);
      #endif
   }
   static wchar_t RE_CALL translate(wchar_t c, bool icase
   #ifdef RE_LOCALE_CPP
   , const __JM_STD::locale& l
   #endif
   )
   {
      #ifdef RE_LOCALE_CPP
      return icase ? JM_USE_FACET(l, __JM_STD::ctype<wchar_t>).tolower((char_type)c) : c;
      #else
      return icase ? ((c < 256) ? re_lower_case_map_w[(uchar_type)c] : re_wtolower(c)) : c;
      #endif
   }

   static void RE_CALL transform(re_str<wchar_t>& out, const re_str<wchar_t>& in
   #ifdef RE_LOCALE_CPP
   , const __JM_STD::locale& l
   #endif
   )
   {
#ifndef RE_LOCALE_CPP
      re_transform(out, in);
#else
      out = JM_USE_FACET(l, __JM_STD::collate<wchar_t>).transform(in.c_str(), in.c_str() + in.size()).c_str();
#endif
   }

   static void RE_CALL transform_primary(re_str<wchar_t>& out, const re_str<wchar_t>& in
   #ifdef RE_LOCALE_CPP
   , const __JM_STD::locale& l
   #endif
   )
   {
      transform(out, in MAYBE_PASS_LOCALE(l));
#ifdef RE_LOCALE_W32
      re_trunc_primary(out);
#else
      unsigned n = in.size() + out.size() / 4;
      if(n < out.size())
         out[n] = 0;
#endif
   }

   static bool RE_CALL is_separator(wchar_t c)
   {
      return JM_MAKE_BOOL(c == L'\n');
   }

   static bool RE_CALL is_combining(wchar_t c)
   {
      return re_is_combining(c);
   }
   
   static bool RE_CALL is_class(wchar_t c, unsigned int f
   #ifdef RE_LOCALE_CPP
   , const __JM_STD::locale& l
   #endif
   )
   {
      #ifdef RE_LOCALE_CPP
      if(JM_USE_FACET(l, __JM_STD::ctype<wchar_t>).is((__JM_STD::ctype<wchar_t>::mask)(f & char_class_all_base), c))
         return true;
      if((f & char_class_underscore) && (c == '_'))
         return true;
      if((f & char_class_blank) && ((c == ' ') || (c == '\t')))
         return true;
      if((f & char_class_unicode) && (c > (size_type)(uchar_type)255))
         return true;
      return false;
      #else
      return JM_MAKE_BOOL(((uchar_type)c < 256) ? (re_unicode_classes[(size_type)(uchar_type)c] & f) : re_iswclass(c, f));
      #endif
   }
   static int RE_CALL toi(wchar_t c
   #ifdef RE_LOCALE_CPP
   , const __JM_STD::locale& l
   #endif
   )
   {
      return re_toi(c MAYBE_PASS_LOCALE(l));
   }
   static int RE_CALL toi(const wchar_t*& first, const wchar_t* last, int radix
   #ifdef RE_LOCALE_CPP
   , const __JM_STD::locale& l
   #endif
   )
   {
      return re_toi(first, last, radix MAYBE_PASS_LOCALE(l));
   }

   static unsigned int RE_CALL lookup_classname(const wchar_t* first, const wchar_t* last
   #ifdef RE_LOCALE_CPP
   , const __JM_STD::locale& l
   #endif
   )
   {
      #ifdef RE_LOCALE_CPP
      return JM_USE_FACET(l, regfacet<wchar_t>).lookup_classname(first, last);
      #else
      return re_lookup_class(first, last);
      #endif
   }


   static bool RE_CALL lookup_collatename(re_str<wchar_t>& s, const wchar_t* first, const wchar_t* last
   #ifdef RE_LOCALE_CPP
   , const __JM_STD::locale& l
   #endif
   )
   {
      #ifdef RE_LOCALE_CPP
      re_str<wchar_t> n(first, last);
      return JM_USE_FACET(l, regfacet<wchar_t>).lookup_collatename(s, n);
      #else
      return re_lookup_collate(s, first, last);
      #endif
   }
};

//
// class char_regex_traits_i
// provides case insensitive traits classes:
template <class charT>
class char_regex_traits_i : public char_regex_traits<charT> {};

JM_TEMPLATE_SPECIALISE
class char_regex_traits_i<char> : public char_regex_traits<char>
{
public:
   typedef char char_type;
   typedef unsigned char uchar_type;
   typedef unsigned int size_type;
   typedef char_regex_traits<char> base_type;

   static char RE_CALL translate(char c, bool
   #ifdef RE_LOCALE_CPP
   , const __JM_STD::locale& l
   #endif
   )
   {
      #ifdef RE_LOCALE_CPP
      return JM_USE_FACET(l, __JM_STD::ctype<char>).tolower((char_type)c);
      #else
      return re_lower_case_map[(size_type)(uchar_type)c];
      #endif
   }
};

JM_TEMPLATE_SPECIALISE
class char_regex_traits_i<wchar_t> : public char_regex_traits<wchar_t>
{
public:
   typedef wchar_t char_type;
   typedef unsigned short uchar_type;
   typedef unsigned int size_type;
   typedef char_regex_traits<wchar_t> base_type;

   static wchar_t RE_CALL translate(wchar_t c, bool
   #ifdef RE_LOCALE_CPP
   , const __JM_STD::locale& l
   #endif
   )
   {
      #ifdef RE_LOCALE_CPP
      return JM_USE_FACET(l, __JM_STD::ctype<wchar_t>).tolower((char_type)c);
      #else
      return (c < 256) ? re_lower_case_map_w[(uchar_type)c] : re_wtolower(c);
      #endif
   }
   static unsigned int RE_CALL lookup_classname(const wchar_t* first, const wchar_t* last
   #ifdef RE_LOCALE_CPP
   , const __JM_STD::locale& l
   #endif
   )
   {
      unsigned int result = char_regex_traits<wchar_t>::lookup_classname(first, last MAYBE_PASS_LOCALE(l));
      if((result & char_class_upper) == char_class_upper)
         result |= char_class_alpha;
      return result;
   }
};

enum mask_type
{
   mask_take = 1,
   mask_skip = 2,
   mask_any = mask_skip | mask_take,
   mask_all = mask_any,
};

struct __narrow_type{};
struct __wide_type{};

template <class charT>
class is_byte;

JM_TEMPLATE_SPECIALISE
class is_byte<char>
{
public:
   typedef __narrow_type width_type;
};

JM_TEMPLATE_SPECIALISE
class is_byte<unsigned char>
{
public:
   typedef __narrow_type width_type;
};

JM_TEMPLATE_SPECIALISE
class is_byte<signed char>
{
public:
   typedef __narrow_type width_type;
};

template <class charT>
class is_byte
{
public:
   typedef __wide_type width_type;
};

union padding
{
   void* p;
   unsigned int i;
};


//
// compiled structures
//
// the following defs describe the format of the compiled string
//

//
// enum syntax_element_type
// describes the type of a record
enum syntax_element_type
{
   syntax_element_startmark = 0,
   syntax_element_endmark = syntax_element_startmark + 1,
   syntax_element_literal = syntax_element_endmark + 1,
   syntax_element_start_line = syntax_element_literal + 1,
   syntax_element_end_line = syntax_element_start_line + 1,
   syntax_element_wild = syntax_element_end_line + 1,
   syntax_element_match = syntax_element_wild + 1,
   syntax_element_word_boundary = syntax_element_match + 1,
   syntax_element_within_word = syntax_element_word_boundary + 1,
   syntax_element_word_start = syntax_element_within_word + 1,
   syntax_element_word_end = syntax_element_word_start + 1,
   syntax_element_buffer_start = syntax_element_word_end + 1,
   syntax_element_buffer_end = syntax_element_buffer_start + 1,
   syntax_element_backref = syntax_element_buffer_end + 1,
   syntax_element_long_set = syntax_element_backref + 1,
   syntax_element_set = syntax_element_long_set + 1,
   syntax_element_jump = syntax_element_set + 1,
   syntax_element_alt = syntax_element_jump + 1,
   syntax_element_rep = syntax_element_alt + 1,
   syntax_element_combining = syntax_element_rep + 1,
   syntax_element_soft_buffer_end = syntax_element_combining + 1,
   syntax_element_restart_continue = syntax_element_soft_buffer_end + 1,
};

union offset_type
{
   re_syntax_base* p;
   unsigned i;
};

//
// struct re_syntax_base
// base class for all syntax types:
struct re_syntax_base
{
   syntax_element_type type;
   offset_type next;
   unsigned int can_be_null;
};

//
// struct re_brace
// marks start or end of (...)
struct re_brace : public re_syntax_base
{
   unsigned int index;
};

//
// struct re_literal
// marks a literal string and
// is followed by an array of charT[length]:
struct re_literal : public re_syntax_base
{
   unsigned int length;
};

//
// struct re_long_set
// provides data for sets [...] containing
// wide characters
struct re_set_long : public re_syntax_base
{
   unsigned int csingles, cranges, cclasses, cequivalents;
   bool isnot;
};

//
// struct re_set
// provides a map of bools for sets containing
// narrow, single byte characters.
struct re_set : public re_syntax_base
{
   unsigned char __map[256];
};

//
// struct re_jump
// provides alternative next destination
struct re_jump : public re_syntax_base
{
   offset_type alt;
   unsigned char __map[256];
};

//
// struct re_repeat
// provides repeat expressions
struct re_repeat : public re_jump
{
   unsigned min, max;
   int id;
   bool leading;
};


//
// enum re_jump_size_type
// provides compiled size of re_jump
// allowing for trailing alignment
// provide this so we know how many
// bytes to insert
enum re_jump_size_type
{
   re_jump_size = (sizeof(re_jump) + sizeof(padding) - 1) & ~(sizeof(padding) - 1),
   re_repeater_size = (sizeof(re_repeat) + sizeof(padding) - 1) & ~(sizeof(padding) - 1),
};


//
// class basic_regex
// handles error codes and flags

class JM_IX_DECL regbase
{
protected:
#ifdef RE_LOCALE_CPP
   __JM_STD::locale locale_inst;
#endif
   unsigned _flags;
   unsigned int code;
public:
   enum flag_type
   {
      escape_in_lists = 1,                     // '\' special inside [...]
      char_classes = escape_in_lists << 1,     // [[:CLASS:]] allowed
      intervals = char_classes << 1,           // {x,y} allowed
      limited_ops = intervals << 1,            // all of + ? and | are normal characters
      newline_alt = limited_ops << 1,          // \n is the same as |
      bk_plus_qm = newline_alt << 1,           // uses \+ and \?
      bk_braces = bk_plus_qm << 1,             // uses \{ and \}
      bk_parens = bk_braces << 1,              // uses \( and \)
      bk_refs = bk_parens << 1,                // \d allowed
      bk_vbar = bk_refs << 1,                  // uses \|
      use_except = bk_vbar << 1,               // exception on error
      failbit = use_except << 1,               // error flag
      literal = failbit << 1,                  // all characters are literals
      icase = literal << 1,                    // characters are matched regardless of case
      nocollate = icase << 1,                  // don't use locale specific collation

      basic = char_classes | intervals | limited_ops | bk_braces | bk_parens | bk_refs,
      extended = char_classes | intervals | bk_refs,
      normal = escape_in_lists | char_classes | intervals | bk_refs | nocollate,
   };

   enum restart_info
   {
      restart_any = 0,
      restart_word = 1,
      restart_line = 2,
      restart_buf = 3,
      restart_continue = 4,
      restart_lit = 5,
      restart_fixed_lit = 6,
   };

   unsigned int RE_CALL error_code()const
   {
      return code;
   }

   void RE_CALL fail(unsigned int err);

   unsigned int RE_CALL flags()const
   {
      return _flags;
   }
#ifdef RE_LOCALE_CPP
   __JM_STD::string RE_CALL errmsg()const
   {
      return re_get_error_str(code, locale_inst);
   }
#else
   const char* RE_CALL errmsg()const
   {
      return re_get_error_str(code);
   }
#endif

   regbase();
   regbase(const regbase& b);

   #ifdef RE_LOCALE_CPP
   __JM_STD::locale RE_CALL imbue(const __JM_STD::locale& l);
   
   const __JM_STD::locale& RE_CALL locale()const
   {
      return locale_inst;
   }
   #endif
};


//
// class reg_expression
// represents the compiled
// regular expression:
//

template <class charT, class traits JM_TRICKY_DEFAULT_PARAM(char_regex_traits<charT>), class Allocator JM_DEF_ALLOC_PARAM(charT) >
class reg_expression : public regbase
{
public:
   // typedefs:
   typedef Allocator alloc_type;
   typedef typename REBIND_TYPE(charT, alloc_type)::size_type size_type;
   typedef charT value_type;
   typedef charT char_type;
   typedef traits traits_type;
   typedef typename traits_type::size_type traits_size_type;
   typedef typename traits_type::uchar_type traits_uchar_type;

private:
#if defined(RE_LOCALE_C) || defined(RE_LOCALE_W32)
   re_initialiser<charT> locale_initialiser;
#endif
   raw_storage<Allocator> data;
   unsigned _restart_type;
   unsigned marks;
   int repeats;
   unsigned char* startmap;
   charT* _expression;
   unsigned int _leading_len;
   const charT* _leading_string;
   unsigned int _leading_string_len;
   kmp_info<charT>* pkmp;

   void RE_CALL compile_maps();
   void RE_CALL compile_map(re_syntax_base* node, unsigned char* __map, unsigned int* pnull, unsigned char mask, re_syntax_base* terminal = NULL)const;
   bool RE_CALL probe_start(re_syntax_base* node, charT c, re_syntax_base* terminal)const;
   bool RE_CALL probe_start_null(re_syntax_base* node, re_syntax_base* terminal)const;
   void RE_CALL fixup_apply(re_syntax_base* b, unsigned cbraces);
   void RE_CALL move_offsets(re_syntax_base* j, unsigned size);
   re_syntax_base* RE_CALL compile_set(const charT*& first, const charT* last);
   re_syntax_base* RE_CALL compile_set_aux(jstack<re_str<charT>, Allocator>& singles, jstack<re_str<charT>, Allocator>& ranges, jstack<unsigned int, Allocator>& classes, jstack<re_str<charT>, Allocator>& equivalents, bool isnot, const __narrow_type&);
   re_syntax_base* RE_CALL compile_set_aux(jstack<re_str<charT>, Allocator>& singles, jstack<re_str<charT>, Allocator>& ranges, jstack<unsigned int, Allocator>& classes, jstack<re_str<charT>, Allocator>& equivalents, bool isnot, const __wide_type&);
   re_syntax_base* RE_CALL compile_set_simple(re_syntax_base* dat, unsigned long cls, bool isnot = false);
   unsigned int RE_CALL parse_inner_set(const charT*& first, const charT* last);

   re_syntax_base* RE_CALL add_simple(re_syntax_base* dat, syntax_element_type type, unsigned int size = sizeof(re_syntax_base));
   re_syntax_base* RE_CALL add_literal(re_syntax_base* dat, charT c);
   charT RE_CALL parse_escape(const charT*& first, const charT* last);
   void RE_CALL parse_range(const charT*& first, const charT* last, unsigned& min, unsigned& max);
   bool RE_CALL skip_space(const charT*& first, const charT* last);
   unsigned int RE_CALL probe_restart(re_syntax_base* dat);
   unsigned int RE_CALL fixup_leading_rep(re_syntax_base* dat, re_syntax_base* end);

public:
   unsigned int RE_CALL set_expression(const charT* p, const charT* end, unsigned f = regbase::normal);
   unsigned int RE_CALL set_expression(const charT* p, unsigned f = regbase::normal) { return set_expression(p, p + traits_type::length(p), f); }
   reg_expression(const Allocator& a = Allocator());
   reg_expression(const charT* p, unsigned f = regbase::normal, const Allocator& a = Allocator());
   reg_expression(const charT* p1, const charT* p2, unsigned f = regbase::normal, const Allocator& a = Allocator());
   reg_expression(const charT* p, size_type len, unsigned f, const Allocator& a = Allocator());
   reg_expression(const reg_expression&);
   ~reg_expression();
   reg_expression& RE_CALL operator=(const reg_expression&);

   bool RE_CALL operator==(const reg_expression&);
   bool RE_CALL operator<(const reg_expression&);

   unsigned RE_CALL mark_count()const { return marks; }
   int RE_CALL repeat_count() const { return repeats; }
   unsigned int RE_CALL restart_type()const { return _restart_type; }
   const re_syntax_base* RE_CALL first()const { return (const re_syntax_base*)data.data(); }
   const unsigned char* RE_CALL get_map()const { return startmap; }
   unsigned int RE_CALL leading_length()const { return _leading_len; }
   alloc_type RE_CALL allocator()const;
   const charT* RE_CALL expression()const { return _expression; }
   const kmp_info<charT>* get_kmp()const { return pkmp; }
   static bool RE_CALL can_start(charT c, const unsigned char* __map, unsigned char mask, const __wide_type&);
   static bool RE_CALL can_start(charT c, const unsigned char* __map, unsigned char mask, const __narrow_type&);
};

//
// class reg_match and reg_match_base
// handles what matched where

template <class iterator, class Allocator JM_DEF_ALLOC_PARAM(iterator) >
class reg_match;

template <class iterator, class Allocator JM_DEF_ALLOC_PARAM(iterator) >
class reg_match_base
{
protected:
   struct pair
   {
      iterator first;
      iterator second;
      bool matched;
      pair() { matched = false; }
      pair(iterator i) : first(i), second(i), matched(false) {}
   };

   struct reference
   {
      unsigned int cmatches;
      Allocator alloc_inst;
      unsigned count;
      pair head, tail, null;
      unsigned int lines;
      iterator line_pos;
      reference(const Allocator& a) : alloc_inst(a) {  }
   };

   reference* ref;

   void RE_CALL cow();

   // protected contructor for derived class...
   reg_match_base(bool){}

public:
   typedef Allocator alloc_type;
   typedef typename REBIND_TYPE(iterator, Allocator)::size_type size_type;
   typedef JM_MAYBE_TYPENAME REBIND_TYPE(char, Allocator) c_alloc;
   typedef iterator value_type;

   reg_match_base(const Allocator& a = Allocator());

   reg_match_base(const reg_match_base& m)
   {
      ref = m.ref;
      ++(ref->count);
   }

   reg_match_base& RE_CALL operator=(const reg_match_base& m);

   ~reg_match_base()
   {
      free();
   }

   void RE_CALL free();

   size_type RE_CALL size()const
   {
      return ref->cmatches;
   }

   void RE_CALL set_size(size_type n);
   void RE_CALL set_size(size_type n, iterator i, iterator j);
   void RE_CALL maybe_assign(const reg_match_base& m);
   void RE_CALL init_fail(iterator i, iterator j);

   const pair& RE_CALL operator[](int n) const
   {
      if((n >= 0) && ((unsigned int)n < ref->cmatches))
         return *(pair*)((char*)ref + sizeof(reference) + sizeof(pair)*n);
      return (n == -1) ? ref->head : (n == -2) ? ref->tail : ref->null;
   }

   Allocator RE_CALL allocator()const;

   size_t RE_CALL length()const
   {
      jm_assert(ref->cmatches);
      size_t n = 0;
      JM_DISTANCE(((pair*)(ref+1))->first, ((pair*)(ref+1))->second, n);
      return n;
   }

   void RE_CALL set_first(iterator i)
   {
      cow();
      ((pair*)(ref+1))->first = i;
      ref->head.second = i;
      ref->head.matched = (ref->head.first == ref->head.second) ? false : true;
   }

   void RE_CALL set_first(iterator i, size_t pos)
   {
      cow();
      ((pair*)((char*)ref + sizeof(reference) + sizeof(pair) * pos))->first = i;
      if(pos == 0)
      {
         ref->head.second = i;
         ref->head.matched = (ref->head.first == ref->head.second) ? false : true;
      }
   }

   void RE_CALL set_second(iterator i)
   {
      cow();
      ((pair*)(ref+1))->second = i;
      ((pair*)(ref+1))->matched = true;
      ref->tail.first = i;
      ref->tail.matched = (ref->tail.first == ref->tail.second) ? false : true;
   }

   void RE_CALL set_second(iterator i, size_t pos)
   {
      cow();
      ((pair*)((char*)ref + sizeof(reference) + sizeof(pair) * pos))->second = i;
      ((pair*)((char*)ref + sizeof(reference) + sizeof(pair) * pos))->matched = true;
      if(pos == 0)
      {
         ref->tail.first = i;
         ref->tail.matched = (ref->tail.first == ref->tail.second) ? false : true;
      }
   }

   void RE_CALL set_line(unsigned int i, iterator pos)
   {
      ref->lines = i;
      ref->line_pos = pos;
   }

   unsigned int RE_CALL line()const
   {
      return ref->lines;
   }

   iterator RE_CALL line_start()const
   {
      return ref->line_pos;
   }

   friend class reg_match<iterator, Allocator>;
};

template <class iterator, class Allocator>
reg_match_base<iterator, Allocator>::reg_match_base(const Allocator& a)
{
  ref = (reference*)c_alloc(a).allocate(sizeof(pair) + sizeof(reference));
  new (ref) reference(a);
  ref->cmatches = 1;
  //ref->alloc_inst = a;
  ref->count = 1;
  // construct the pair:
  new ((pair*)(ref+1)) pair();
}

template <class iterator, class Allocator>
Allocator RE_CALL reg_match_base<iterator, Allocator>::allocator()const
{
  return ref->alloc_inst;
}

template <class iterator, class Allocator>
inline reg_match_base<iterator, Allocator>& RE_CALL reg_match_base<iterator, Allocator>::operator=(const reg_match_base& m)
{
   if(ref != m.ref)
   {
      free();
      ref = m.ref;
      ++(ref->count);
   }
   return *this;
}


template <class iterator, class Allocator>
void RE_CALL reg_match_base<iterator, Allocator>::free()
{
   if(--(ref->count) == 0)
   {
      Allocator a(ref->alloc_inst);
      pair* p1, *p2;
      p1 = (pair*)(ref+1);
      p2 = p1 + ref->cmatches;
      while(p1 != p2)
      {
         jm_destroy(p1);
         ++p1;
      }
      jm_destroy(ref);
      c_alloc(a).deallocate((char*)ref, sizeof(pair) * ref->cmatches + sizeof(reference));
   }
}

template <class iterator, class Allocator>
void RE_CALL reg_match_base<iterator, Allocator>::set_size(size_type n)
{
   if(ref->cmatches != n)
   {
      reference* newref = (reference*)c_alloc(ref->alloc_inst).allocate(sizeof(pair) * n + sizeof(reference));;
      new (newref) reference(*ref);
      newref->count = 1;
      newref->cmatches = n;
      pair* p1, *p2;
      p1 = (pair*)(newref+1);
      p2 = p1 + newref->cmatches;
      while(p1 != p2)
      {
         new (p1) pair();
         ++p1;
      }
      free();
      ref = newref;
   }
}

template <class iterator, class Allocator>
void RE_CALL reg_match_base<iterator, Allocator>::set_size(size_type n, iterator i, iterator j)
{
   cow();
   if(ref->cmatches != n)
   {
      reference* newref = (reference*)c_alloc(ref->alloc_inst).allocate(sizeof(pair) * n + sizeof(reference));;
      new (newref) reference(*ref);
      newref->count = 1;
      newref->cmatches = n;
      pair* p1, *p2;
      p1 = (pair*)(newref+1);
      p2 = p1 + newref->cmatches;
      while(p1 != p2)
      {
         new (p1) pair(j);
         ++p1;
      }
      free();
      ref = newref;
   }
   else
   {
      // set iterators to be i, matched to false:
      pair* p1, *p2;
      p1 = (pair*)(ref+1);
      p2 = p1 + ref->cmatches;
      while(p1 != p2)
      {
         p1->first = j;
         p1->second = j;
         p1->matched = false;
         ++p1;
      }
   }
   ref->head.first = i;
   ref->tail.second = j;
   ref->head.matched = ref->tail.matched = true;
   ref->null.first = ref->null.second = j;
   ref->null.matched = false;
}

template <class iterator, class Allocator>
inline void RE_CALL reg_match_base<iterator, Allocator>::init_fail(iterator i, iterator j)
{
   set_size(ref->cmatches, i, j);
}

template <class iterator, class Allocator>
void RE_CALL reg_match_base<iterator, Allocator>::maybe_assign(const reg_match_base& m)
{
   pair* p1, *p2;
   p1 = (pair*)(ref+1);
   p2 = (pair*)(m.ref+1);
   unsigned int len1, len2;
   unsigned int i;
   for(i = 0; i < ref->cmatches; ++i)
   {
      len1 = len2 = 0;
      JM_DISTANCE(p1->first, p1->second, len1);
      JM_DISTANCE(p2->first, p2->second, len2);
      if((len1 != len2) || ((p1->matched == false) && (p2->matched == true)))
         break;
      if((p1->matched == true) && (p2->matched == false))
         return;
      ++p1;
      ++p2;
   }
   if(i == ref->cmatches)
      return;
   if((len2 > len1) || ((p1->matched == false) && (p2->matched == true)) )
      *this = m;
}

template <class iterator, class Allocator>
void RE_CALL reg_match_base<iterator, Allocator>::cow()
{
   if(ref->count > 1)
   {
      reference* newref = (reference*)c_alloc(ref->alloc_inst).allocate(sizeof(pair) * ref->cmatches + sizeof(reference));
      new (newref) reference(*ref);
      newref->count = 1;
      pair* p1, *p2, *p3;
      p1 = (pair*)(newref+1);
      p2 = p1 + newref->cmatches;
      p3 = (pair*)(ref+1);
      while(p1 != p2)
      {
         new (p1) pair(*p3);
         ++p1;
         ++p3;
      }
      --(ref->count);
      ref = newref;
   }
}

//
// class reg_match
// encapsulates reg_match_base, does a deep copy rather than
// reference counting to ensure thread safety when copying
// other reg_match instances

template <class iterator, class Allocator>
class reg_match : public reg_match_base<iterator, Allocator>
{
public:
   reg_match(const Allocator& a = Allocator())
      : reg_match_base<iterator, Allocator>(a){}

   reg_match(const reg_match_base<iterator, Allocator>& m)
      : reg_match_base<iterator, Allocator>(m){}

   reg_match& operator=(const reg_match_base<iterator, Allocator>& m)
   {
      // shallow copy
      reg_match_base<iterator, Allocator>::operator=(m);
      return *this;
   }

   reg_match(const reg_match& m);
   reg_match& operator=(const reg_match& m);

};

template <class iterator, class Allocator>
reg_match<iterator, Allocator>::reg_match(const reg_match<iterator, Allocator>& m)
   : reg_match_base<iterator, Allocator>(false)
{
   ref = (reference*)c_alloc(m.ref->alloc_inst).allocate(sizeof(pair) * m.ref->cmatches + sizeof(reference));
   new (ref) reference(*m.ref);
   ref->count = 1;
   pair* p1, *p2, *p3;
   p1 = (pair*)(ref+1);
   p2 = p1 + ref->cmatches;
   p3 = (pair*)(m.ref+1);
   while(p1 != p2)
   {
      new (p1) pair(*p3);
      ++p1;
      ++p3;
   }
}

template <class iterator, class Allocator>
reg_match<iterator, Allocator>& reg_match<iterator, Allocator>::operator=(const reg_match<iterator, Allocator>& m)
{
   free();
   ref = (reference*)c_alloc(m.ref->alloc_inst).allocate(sizeof(pair) * m.ref->cmatches + sizeof(reference));
   new (ref) reference(*m.ref);
   ref->count = 1;
   pair* p1, *p2, *p3;
   p1 = (pair*)(ref+1);
   p2 = p1 + ref->cmatches;
   p3 = (pair*)(m.ref+1);
   while(p1 != p2)
   {
      new (p1) pair(*p3);
      ++p1;
      ++p3;
   }
   return *this;
}


JM_END_NAMESPACE   // namespace regex

#include <jm/regcomp.h>
#include <jm/regmatch.h>
#include <jm/regfmt.h>

JM_NAMESPACE(__JM)

typedef reg_expression<char, char_regex_traits<char>, JM_DEF_ALLOC(char)> regex;
typedef reg_expression<wchar_t, char_regex_traits<wchar_t>, JM_DEF_ALLOC(wchar_t)> wregex;

typedef reg_match<const char*, regex::alloc_type> cmatch;
typedef reg_match<const wchar_t*, wregex::alloc_type> wcmatch;

JM_END_NAMESPACE   // namespace regex

JM_USING(__JM)

#endif  // __cplusplus

#endif  // include














