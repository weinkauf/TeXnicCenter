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
  *	FILE     regex.cpp
  *	VERSION  2.00
  */


#include <regex.h>
#include <jm/re_mss.h>


#ifdef JM_NO_WCSTRING

JM_IX_DECL int RE_CALL wcsncmp(const wchar_t *s1, const wchar_t *s2, size_t n)
{
   while(n && *s1 && *s2)
   {
      if(*s1 != *s2)
         return *s2 - *s1;
      ++s1;
      ++s2;
      --n;
   }
   return 0;
}

JM_IX_DECL size_t RE_CALL wcslen(const wchar_t* p)
{
   const wchar_t* ptr = p;
   while(*ptr) ++ptr;
   return ptr - p;
}

#endif


JM_NAMESPACE(__JM)

#if !defined(JM_NO_EXCEPTIONS)

const char* bad_expression::what() const throw()
{
#ifdef RE_LOCALE_CPP
   return code.c_str();
#else
   return re_get_error_str(code);
#endif
}

#endif

#ifdef RE_LOCALE_CPP
__JM_STD::locale RE_CALL regbase::imbue(const __JM_STD::locale& l)
{
   __JM_STD::locale t(locale_inst);
   locale_inst = l;
   return t;
}
#endif

regbase::regbase()
{
   _flags = regbase::failbit;
   code = REG_EMPTY;
}

regbase::regbase(const regbase& b) :
#ifdef RE_LOCALE_CPP
   locale_inst(b.locale_inst),
#endif
   _flags(b._flags), code(b.code)
{
}


void RE_CALL regbase::fail(unsigned int err)
{
   code = err;
   if(code)
   {
      _flags |= failbit;
#ifndef JM_NO_EXEPTIONS
#ifdef RE_LOCALE_CPP
      if(_flags & use_except)
      {
         throw bad_expression(re_get_error_str(err, locale_inst));
      }
#else
      if(_flags & use_except)
         throw bad_expression(code);
#endif
#endif
   }
   else
      _flags &= ~(regbase::failbit);
}



JM_END_NAMESPACE

#ifdef JM_USE_VCL

#include <vcl.h>

int WINAPI DllEntryPoint(HINSTANCE , unsigned long , void*)
{
   return 1;
}
#endif






