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
  *	FILE     re_nls.cpp
  *	VERSION  2.00
  */

#include <jm/re_nls.h>
#include <jm/re_thrd.h>
#include <jm/re_str.h>
#include <jm/re_mss.h>
#include <jm/regex.h>
#include <jm/re_cls.h>
#include <jm/re_coll.h>
#include <stdio.h>
#include <ctype.h>
#include <locale.h>


JM_NAMESPACE(__JM)

#ifndef RE_LOCALE_CPP

re_initialiser<char> init;

//
// start by declaring externals for RE_LOCALE_C
// and RE_LOCALE_W32:
//

JM_IX_DECL unsigned char re_syntax_map[256];
JM_IX_DECL unsigned short re_class_map[256];
JM_IX_DECL char re_lower_case_map[256];
JM_IX_DECL char re_zero;
JM_IX_DECL char re_ten;

unsigned int entry_count = 0;

re_str<char>* ctype_name;
re_str<char>* collate_name;

JM_IX_DECL void RE_CALL re_init()
{
#ifdef JM_THREADS
   re_init_threads();
   cs_guard g(*p_re_lock);
#endif
   // just keep track of entry_count
   if(entry_count == 0)
   {
      ctype_name = new re_str<char>("xxxxxxxxxxxxxxxx");
      collate_name = new re_str<char>("xxxxxxxxxxxxxxxx");
   }
   re_message_init();
   re_init_classes();
   re_init_collate();
   ++entry_count;
}

JM_IX_DECL void RE_CALL do_update_ctype()
{
   // start by updating the syntax map:
   unsigned int i;
   char buf[256];
   memset(re_syntax_map, syntax_char, 256);
   for(i = 1; i < syntax_max; ++i)
   {
      char* ptr = buf;
      re_get_message((char*)buf, 256, i+100);
      for(; *ptr; ++ptr)
      {
         re_syntax_map[(unsigned char)*ptr] = (unsigned char)i;
      }
   }

   // now update the character class map,
   // and lower case map:
   #ifdef RE_LOCALE_W32
   for(i = 0; i < 256; ++i)
   {
      buf[i] = (char)i;
   }
   GetStringTypeA(GetUserDefaultLCID(), CT_CTYPE1, buf, 256, re_class_map);
   re_class_map['_'] |= char_class_underscore;
   LCMapStringA(GetUserDefaultLCID(), LCMAP_LOWERCASE, buf, 256, re_lower_case_map, 256);
   #else
   memset(re_class_map, 0, 256);
   for(i = 0; i < 256; ++i)
   {
      if(isalpha((char)i))
         re_class_map[i] |= char_class_alpha;
      if(iscntrl((char)i))
         re_class_map[i] |= char_class_cntrl;
      if(isdigit((char)i))
         re_class_map[i] |= char_class_digit;
      if(islower((char)i))
         re_class_map[i] |= char_class_lower;
      if(isupper((char)i))
         re_class_map[i] |= char_class_upper;
      if(ispunct((char)i))
         re_class_map[i] |= char_class_punct;
      if(isspace((char)i))
         re_class_map[i] |= char_class_space;
      if(isxdigit((char)i))
         re_class_map[i] |= char_class_xdigit;
   }
   re_class_map['_'] |= char_class_underscore;
   re_class_map[' '] |= char_class_blank;
   re_class_map['\t'] |= char_class_blank;
   for(i = 0; i < 256; ++i)
   {
      re_lower_case_map[i] = (char)tolower(i);
   }
   #endif
   re_update_classes();
}

JM_IX_DECL void RE_CALL do_update_collate()
{
   re_update_collate();
   re_str<char> s;
   const char* p = "zero";
   if(re_lookup_collate(s, p, p+4))
   {
      jm_assert(s.size() == 1);
      re_zero = *s.c_str();
   }
   else
      re_zero = '0';

   p = "ten";
   if(re_lookup_collate(s, p, p+3))
   {
      jm_assert(s.size() == 1);
      re_ten = *s.c_str();
   }
   else
      re_ten = 'a';
}

JM_IX_DECL void RE_CALL re_update()
{
   #ifdef JM_THREADS
   cs_guard g(*p_re_lock);
   #endif
   re_message_update();
   if(*collate_name != get_global_locale_name(LC_COLLATE))
   {
      do_update_collate();
      *collate_name = get_global_locale_name(LC_COLLATE);
   }
   if(*ctype_name != get_global_locale_name(LC_CTYPE))
   {
      do_update_ctype();
      *ctype_name = get_global_locale_name(LC_CTYPE);
   }
}

JM_IX_DECL void RE_CALL re_free()
{
   #ifdef JM_THREADS
   cs_guard g(*p_re_lock);
   #endif
   re_message_free();
   re_free_classes();
   re_free_collate();
   --entry_count;
   if(entry_count == 0)
   {
      delete ctype_name;
      delete collate_name;
   }
#ifdef JM_THREADS
   g.acquire(false);
   re_free_threads();
#endif
}

JM_IX_DECL void RE_CALL re_transform(re_str<char>& out, const re_str<char>& in)
{
#ifdef RE_LOCALE_C
   size_t n = strxfrm(0, in.c_str(), 0);
#else
   size_t n = LCMapStringA(GetUserDefaultLCID(), LCMAP_SORTKEY, in.c_str(), -1, 0, 0);
#endif
   if(n == TRANSFORM_ERROR)
   {
      out = in;
      return;
   }
   char* buf = new char[n+1];
#ifdef RE_LOCALE_C
   n = strxfrm(buf, in.c_str(), n+1);
#else
   n = LCMapStringA(GetUserDefaultLCID(), LCMAP_SORTKEY, in.c_str(), -1, buf, n);
#endif
   if(n == TRANSFORM_ERROR)
   {
      out = in;
      return;
   }
   out = buf;
   delete[] buf;
}

JM_IX_DECL const char* RE_CALL get_global_locale_name(int cat)
{
#ifdef RE_LOCALE_W32
   // this always gets called from within re_update
   // and since re_update is thread protected then so is this.
   static char the_buf[30];
   sprintf(the_buf, "%x", (int)GetUserDefaultLCID());
   return the_buf;
#else
   return setlocale(cat, NULL);
#endif
}

#endif

JM_END_NAMESPACE


