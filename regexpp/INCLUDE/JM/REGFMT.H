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
  *	FILE     regfmt.h
  *	VERSION  2.00
  *
  *   Provides formatting output routines for search and replace
  *   operations.  Note this is an internal header file included
  *   by regex.h, do not include on its own.
  */


#ifndef REGFMT_H
#define REGFMT_H


JM_NAMESPACE(__JM)

template <class O, class I>
O RE_CALL re_copy_out(O out, I first, I last)
{
   while(first != last)
   {
      *out = *first;
      ++out;
      ++first;
   }
   return out;
}

template <class charT>
void RE_CALL re_skip_format(const charT*& fmt
#ifdef RE_LOCALE_CPP
               , const __JM_STD::locale& l
#endif
               )
{
   typedef char_regex_traits<charT> traits_type;
   unsigned int parens = 0;
   unsigned int c;
   while(*fmt)
   {
      c = traits_type::syntax_type(*fmt MAYBE_PASS_LOCALE(l));
      if((c == syntax_colon) && (parens == 0))
      {
         ++fmt;
         return;
      }
      else if(c == syntax_close_bracket)
      {
         if(parens == 0)
         {
            ++fmt;
            return;
         }
         --parens;
      }
      else if(c == syntax_open_bracket)
         ++parens;
      else if(c == syntax_slash)
      {
         ++fmt;
         if(*fmt == 0)
            return;
      }
      ++fmt;
   }
}

//
// algorithm reg_format:
// takes the result of a match and a format string
// and merges them to produce a new string which
// is sent to an OutputIterator,
// __reg_format_aux does the actual work:
//
template <class OutputIterator, class iterator, class Allocator, class charT>
OutputIterator RE_CALL __reg_format_aux(OutputIterator out, 
                          const reg_match<iterator, Allocator>& m, 
                          const charT*& fmt,
                          bool isif
#ifdef RE_LOCALE_CPP
                           , const __JM_STD::locale& l
#endif
                           )
{
   typedef char_regex_traits<charT> traits_type;

   const charT* fmt_end = fmt;
   while(*fmt_end) ++ fmt_end;

   while(*fmt)
   {
      switch(traits_type::syntax_type(*fmt MAYBE_PASS_LOCALE(l)))
      {
      case syntax_dollar:
         ++fmt;
         if(*fmt == 0) // oops trailing $
         {
            --fmt;
            *out = *fmt;
            ++out;
            return out;
         }
         switch(traits_type::syntax_type(*fmt MAYBE_PASS_LOCALE(l)))
         {
         case syntax_start_buffer:
            /*out =*/ re_copy_out(out, iterator(m[-1].first), iterator(m[-1].second));
            ++fmt;
            continue;
         case syntax_end_buffer:
            /*out =*/ re_copy_out(out, iterator(m[-2].first), iterator(m[-2].second));
            ++fmt;
            continue;
         case syntax_digit:
         {
            unsigned int index = traits_type::toi(fmt, fmt_end, 10 MAYBE_PASS_LOCALE(l));
            /*out =*/ re_copy_out(out, iterator(m[index].first), iterator(m[index].second));
            continue;
         }
         }
         // anything else:
         if(*fmt == '&')
         {
            /*out =*/ re_copy_out(out, iterator(m[0].first), iterator(m[0].second));
            ++fmt;
         }
         else
         {
            // probably an error, treat as a literal '$'
            --fmt;
            *out = *fmt;
            ++out;
            ++fmt;
         }
         continue;
      case syntax_slash:
      {
         // escape sequence:
         charT c;
         ++fmt;
         if(*fmt == 0)
         {
            --fmt;
            *out = *fmt;
            ++out;
            ++fmt;
            return out;
         }
         switch(traits_type::syntax_type(*fmt MAYBE_PASS_LOCALE(l)))
         {
         case syntax_a:
            c = '\a';
            ++fmt;
            break;
         case syntax_f:
            c = '\f';
            ++fmt;
            break;
         case syntax_n:
            c = '\n';
            ++fmt;
            break;
         case syntax_r:
            c = '\r';
            ++fmt;
            break;
         case syntax_t:
            c = '\t';
            ++fmt;
            break;
         case syntax_v:
            c = '\v';
            ++fmt;
            break;
         case syntax_x:
            ++fmt;
            if(fmt == fmt_end)
            {
               *out = *--fmt;
               ++out;
               return out;
            }
            // maybe have \x{ddd}
            if(traits_type::syntax_type(*fmt MAYBE_PASS_LOCALE(l)) == syntax_open_brace)
            {
               ++fmt;
               if(fmt == fmt_end)
               {
                  fmt -= 2;
                  *out = *fmt;
                  ++out;
                  ++fmt;
                  continue;
               }
               if(traits_type::is_class(*fmt, char_class_xdigit MAYBE_PASS_LOCALE(l)) == false)
               {
                  fmt -= 2;
                  *out = *fmt;
                  ++out;
                  ++fmt;
                  continue;
               }
               c = (charT)traits_type::toi(fmt, fmt_end, -16 MAYBE_PASS_LOCALE(l));
               if(traits_type::syntax_type(*fmt MAYBE_PASS_LOCALE(l)) != syntax_close_brace)
               {
                  while(traits_type::syntax_type(*fmt MAYBE_PASS_LOCALE(l)) != syntax_slash)
                     --fmt;
                  ++fmt;
                  *out = *fmt;
                  ++out;
                  ++fmt;
                  continue;
               }
               ++fmt;
               break;
            }
            else
            {
               if(traits_type::is_class(*fmt, char_class_xdigit MAYBE_PASS_LOCALE(l)) == false)
               {
                  --fmt;
                  *out = *fmt;
                  ++out;
                  ++fmt;
                  continue;
               }
               c = (charT)traits_type::toi(fmt, fmt_end, -16 MAYBE_PASS_LOCALE(l));
            }
            break;
         case syntax_c:
            ++fmt;
            if(fmt == fmt_end)
            {
               --fmt;
               *out = *fmt;
               ++out;
               return out;
            }
            if(((traits_type::uchar_type)(*fmt) < (traits_type::uchar_type)'@')
                  || ((traits_type::uchar_type)(*fmt) > (traits_type::uchar_type)127) )
            {
               --fmt;
               *out = *fmt;
               ++out;
               ++fmt;
               break;
            }
            c = (charT)((traits_type::uchar_type)(*fmt) - (traits_type::uchar_type)'@');
            ++fmt;
            break;
         case syntax_e:
            c = (charT)27;
            ++fmt;
            break;
         case syntax_digit:
            c = (charT)traits_type::toi(fmt, fmt_end, -8 MAYBE_PASS_LOCALE(l));
            break;
         default:
            c = *fmt;
            ++fmt;
         }
         *out = c;
         continue;
      }
      case syntax_open_bracket:
         ++fmt;  // recurse
         /*out =*/ __reg_format_aux(out, m, fmt, false MAYBE_PASS_LOCALE(l));
         continue;
      case syntax_close_bracket:
         ++fmt;  // return from recursion
         return out;
      case syntax_colon:
         if(isif)
         {
            ++fmt;
            return out;
         }
         *out = *fmt;
         ++out;
         ++fmt;
         continue;
      case syntax_question:
      {
         ++fmt;
         if(*fmt == 0)
         {
            --fmt;
            *out = *fmt;
            ++out;
            ++fmt;
            return out;
         }
         unsigned int id = traits_type::toi(fmt, fmt_end, 10 MAYBE_PASS_LOCALE(l));
         if(m[id].matched)
         {
            __reg_format_aux(out, m, fmt, true MAYBE_PASS_LOCALE(l));
            if(traits_type::syntax_type(*(fmt-1) MAYBE_PASS_LOCALE(l)) == syntax_colon)
               re_skip_format(fmt MAYBE_PASS_LOCALE(l));
         }
         else
         {
            re_skip_format(fmt MAYBE_PASS_LOCALE(l));
            if(traits_type::syntax_type(*(fmt-1) MAYBE_PASS_LOCALE(l)) == syntax_colon)
               __reg_format_aux(out, m, fmt, true MAYBE_PASS_LOCALE(l));
         }
         return out;
      }
      default:
         *out = *fmt;
         ++out;
         ++fmt;
      }
   }

   return out;
}

template <class OutputIterator, class iterator, class Allocator, class charT>
OutputIterator RE_CALL reg_format(OutputIterator out,
                          const reg_match<iterator, Allocator>& m,
                          const charT* fmt
#ifdef RE_LOCALE_CPP
                         , __JM_STD::locale locale_inst = __JM_STD::locale()
#endif
                         )
{
   //
   // start by updating the locale:
   //
#if defined(RE_LOCALE_C) || defined(RE_LOCALE_W32)
   static re_initialiser<charT> locale_initialiser;
   locale_initialiser.update();
#else
   if(JM_HAS_FACET(locale_inst, regfacet<charT>) == false)
   {
#ifdef _MSC_VER
      locale_inst = __JM_STD::_ADDFAC(locale_inst, new regfacet<charT>());
#else
      locale_inst = __JM_STD::locale(locale_inst, new regfacet<charT>());
#endif
   }
   JM_USE_FACET(locale_inst, regfacet<charT>).update(locale_inst);
#endif
   return __reg_format_aux(out, m, fmt, false MAYBE_PASS_LOCALE(l));
}


template <class OutputIterator, class iterator, class charT, class Allocator>
class merge_out_predicate
{
   iterator& last;
   OutputIterator& out;
   const charT* fmt;
   bool copy_none;

#ifdef RE_LOCALE_CPP
   const __JM_STD::locale& l;
#endif

public:
   merge_out_predicate(OutputIterator& o, iterator& pi, const charT* f, bool c
#ifdef RE_LOCALE_CPP
      , const __JM_STD::locale& loc
#endif
      ) : out(o), last(pi), fmt(f), copy_none(c)
#ifdef RE_LOCALE_CPP
      , l(loc)
#endif
   {}

   ~merge_out_predicate() {}
   bool RE_CALL operator()(const __JM::reg_match<iterator, Allocator>& m)
   {
      const charT* f = fmt;
      if(copy_none)
         /*out =*/ re_copy_out(out, iterator(m[-1].first), iterator(m[-1].second));
      /*out =*/ __reg_format_aux(out, m, f, false MAYBE_PASS_LOCALE(l));
      last = m[-2].first;
      return true;
   }
};


template <class OutputIterator, class iterator, class traits, class Allocator, class charT>
OutputIterator RE_CALL reg_merge(OutputIterator out, 
                         iterator first,
                         iterator last,
                         const reg_expression<charT, traits, Allocator>& e, 
                         const charT* fmt, 
                         bool copy = true, 
                         unsigned int flags = match_default)
{
   //
   // start by updating the locale:
   //
#if defined(RE_LOCALE_C) || defined(RE_LOCALE_W32)
   static re_initialiser<charT> locale_initialiser;
   locale_initialiser.update();
#else
   __JM_STD::locale locale_inst(e.locale());
   if(JM_HAS_FACET(locale_inst, regfacet<charT>) == false)
   {
#ifdef _MSC_VER
      locale_inst = __JM_STD::_ADDFAC(locale_inst, new regfacet<charT>());
#else
      locale_inst = __JM_STD::locale(locale_inst, new regfacet<charT>());
#endif
   }
   JM_USE_FACET(locale_inst, regfacet<charT>).update(locale_inst);
#endif
   iterator l = first;
   merge_out_predicate<OutputIterator, iterator, charT, Allocator> oi(out, l, fmt, copy MAYBE_PASS_LOCALE(locale_inst));
   reg_grep(oi, first, last, e, flags);
   return copy ? re_copy_out(out, l, last) : out;
}


JM_END_NAMESPACE

#endif

