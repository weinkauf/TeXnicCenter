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
  *	FILE     regfac.cpp
  *	VERSION  2.00
  */


#include <jm/regfac.h>
#include <jm/re_str.h>
#include <jm/re_lst.h>
#include <jm/re_nls.h>
#include <jm/regex.h>
#include <jm/re_mss.h>
#include <jm/re_cls.h>

#ifdef RE_LOCALE_CPP

JM_NAMESPACE(__JM)

__JM_STD::string RE_CALL to_narrow(const __JM_STD::wstring& is, const __JM_STD::locale& l);

__JM_STD::locale::id regfacet<char>::id;
__JM_STD::locale::id regfacet<wchar_t>::id;

regfacet<char>::regfacet(unsigned int i)
   : __JM_STD::locale::facet(i), name("xxxxxxxx")
{
}

regfacet<wchar_t>::regfacet(unsigned int i)
   : __JM_STD::locale::facet(i), name("xxxxxxxx")
{
}

regfacet<char>::~regfacet()
{
}

regfacet<wchar_t>::~regfacet()
{
}

unsigned int RE_CALL regfacet<char>::do_lookup_classname(const char* first, const char* last)const
{
   unsigned int i;
   __JM_STD::string s(first, last);

   __JM_STD::map<__JM_STD::string, unsigned long, __JM_STD::less<__JM_STD::string> >::const_iterator pos = classes.find(s);
   if(pos != classes.end())
      return (*pos).second;

   for(i = 0; i < re_classes_max; ++i)
   {
      if(s == re_char_class_names[i])
         return re_char_class_id[i];
   }
   return 0;
}

bool RE_CALL regfacet<char>::do_lookup_collatename(re_str<char>& s, const re_str<char>& name)const
{
   __JM_STD::map<re_str<char>, re_str<char>, __JM_STD::less<re_str<char> > >::const_iterator pos = collating_elements.find(name);
   if(pos != collating_elements.end())
   {
      s = (*pos).second;
   }
   return re_lookup_def_collate_name(s, name.c_str());
}

unsigned int RE_CALL regfacet<char>::do_syntax_type(char c)const
{
   return syntax_map[(unsigned char)c];
}

void RE_CALL regfacet<char>::do_update(const __JM_STD::locale& l)
{
#ifdef RE_THREADS
   critical_section::rw_guard g(cs);
#endif
   if(l.name() != name)
   {
      name = l.name();
      __JM_STD::messages<char>::catalog cat = regex_message_catalogue ? JM_USE_FACET(l, __JM_STD::messages<char>).open(regex_message_catalogue, l) : -1;

      memset(syntax_map, syntax_char, 256);
      unsigned int i;
      for(i = 1; i < syntax_max; ++i)
      {
         string_type s = re_get_def_message(i+100);
         if(cat >= 0)
            s = JM_USE_FACET(l, __JM_STD::messages<char>).get(cat, 0, i+100, s);
         for(unsigned int j = 0; j < s.size(); ++j)
         {
            syntax_map[s[j]] = (unsigned char)(i);
         }
      }

      // load any custom collate names:
      string_type c1, c2;
      i = 400;
      if(cat >= 0)
         c2 = JM_USE_FACET(l, __JM_STD::messages<char>).get(cat, 0, i, c1);
      while(c2.size())
      {
         const char* p1, *p2, *p3, *p4;;
         p1 = c2.c_str();
         while(*p1 && isspace(*p1))++p1;
         p2 = p1;
         while(*p2 && !isspace(*p2))++p2;
         p3 = p2;
         while(*p3 && isspace(*p3))++p3;
         p4 = p3;
         while(*p4 && !isspace(*p4))++p4;
         collating_elements[re_str<char>(p1, p2)] = re_str<char>(p3, p4);
         
         ++i;
         c2 = JM_USE_FACET(l, __JM_STD::messages<char>).get(cat, 0, i, c1);
      }

      re_str<char> n("zero");
      __JM_STD::map<re_str<char>, re_str<char>, __JM_STD::less<re_str<char> > >::const_iterator pos = collating_elements.find(n);
      if(pos != collating_elements.end())
         _zero = *(*pos).second.c_str();
      else
         _zero = '0';

      n = "ten";
      pos = collating_elements.find(n);
      if(pos != collating_elements.end())
         _ten = *(*pos).second.c_str();
      else
         _ten = 'a';

      __JM_STD::string m;
      __JM_STD::string s;
      if(cat >= 0)
         for(i = 0; i < re_classes_max; ++i)
         {
            s = JM_USE_FACET(l, __JM_STD::messages<char>).get(cat, 0, i+300, m);
            if(s.size())
               classes[s] = i;
         }
      
      if(cat >= 0)
         JM_USE_FACET(l, __JM_STD::messages<char>).close(cat);
   }
}

unsigned int RE_CALL regfacet<wchar_t>::do_lookup_classname(const wchar_t* first, const wchar_t* last)const
{
   __JM_STD::wstring s(first, last);
   unsigned int i;

   __JM_STD::map<__JM_STD::wstring, unsigned long, __JM_STD::less<__JM_STD::wstring> >::const_iterator pos = classes.find(s);
   if(pos != classes.end())
      return (*pos).second;

   __JM_STD::string ns = to_narrow(s, *ploc);
   for(i = 0; i < re_classes_max; ++i)
   {
      if(strcmp(re_char_class_names[i], ns.c_str()) == 0)
         return re_char_class_id[i];
   }
   return 0;
}

bool RE_CALL regfacet<wchar_t>::do_lookup_collatename(re_str<wchar_t>& s, const re_str<wchar_t>& name)const
{
   __JM_STD::map<re_str<wchar_t>, re_str<wchar_t>, __JM_STD::less<re_str<wchar_t> > >::const_iterator pos = collating_elements.find(name);
   if(pos != collating_elements.end())
   {
      s = (*pos).second;
   }

   unsigned int len = re_strnarrow((char*)NULL, 0, name.c_str());
   char* buf = new char[len];
   re_strnarrow(buf, len, name.c_str());
   re_str<char> t_out;
   bool result =  re_lookup_def_collate_name(t_out, buf);
   delete[] buf;
   if(result)
   {
      len = re_strwiden((wchar_t*)0, 0, t_out);
      wchar_t* wb = new wchar_t[len];
      re_strwiden(wb, len, t_out);
      s = wb;
      delete[] wb;
   }
   return result;
}

unsigned int RE_CALL regfacet<wchar_t>::do_syntax_type(wchar_t c)const
{
   __JM_STD::list<syntax_map>::const_iterator i, j;
   i = syntax.begin();
   j = syntax.end();
   while(i != j)
   {
      if((*i).c == c)
         return (*i).type;
      ++i;
   }
   return 0;
}

void RE_CALL regfacet<wchar_t>::do_update(const __JM_STD::locale& l)
{
#ifdef RE_THREADS
   critical_section::rw_guard g(cs);
#endif
   if(l.name() != name)
   {
      ploc = &l;
      syntax_map m;
      name = l.name();
      __JM_STD::messages<wchar_t>::catalog cat = regex_message_catalogue ? JM_USE_FACET(l, __JM_STD::messages<wchar_t>).open(regex_message_catalogue, l) : -1;

      syntax.clear();
      unsigned int i;
      for(i = 1; i < syntax_max; ++i)
      {
         string_type s = re_get_def_message_w(i+100);
         if(cat >= 0)
            s = JM_USE_FACET(l, __JM_STD::messages<wchar_t>).get(cat, 0, i+100, s);
         for(unsigned int j = 0; j < s.size(); ++j)
         {
            m.c = s[j];
            m.type = i;
            syntax.push_back(m);
         }
      }

      // load any custom collate names:
      string_type c1, c2;
      i = 400;
      if(cat >= 0)
         c2 = JM_USE_FACET(l, __JM_STD::messages<wchar_t>).get(cat, 0, i, c1);
      while(c2.size())
      {
         const wchar_t* p1, *p2, *p3, *p4;;
         p1 = c2.c_str();
         while(*p1 && iswspace(*p1))++p1;
         p2 = p1;
         while(*p2 && !iswspace(*p2))++p2;
         p3 = p2;
         while(*p3 && iswspace(*p3))++p3;
         p4 = p3;
         while(*p4 && !iswspace(*p4))++p4;
         collating_elements[re_str<wchar_t>(p1, p2)] = re_str<wchar_t>(p3, p4);
         
         ++i;
         c2 = JM_USE_FACET(l, __JM_STD::messages<wchar_t>).get(cat, 0, i, c1);
      }

      re_str<wchar_t> n(L"zero");
      __JM_STD::map<re_str<wchar_t>, re_str<wchar_t>, __JM_STD::less<re_str<wchar_t> > >::const_iterator pos = collating_elements.find(n);
      if(pos != collating_elements.end())
         _zero = *(*pos).second.c_str();
      else
         _zero = L'0';

      n = L"ten";
      pos = collating_elements.find(n);
      if(pos != collating_elements.end())
         _ten = *(*pos).second.c_str();
      else
         _ten = L'a';

      __JM_STD::wstring ws;
      __JM_STD::wstring s;
      if(cat >= 0)
         for(i = 0; i < re_classes_max; ++i)
         {
            s = JM_USE_FACET(l, __JM_STD::messages<wchar_t>).get(cat, 0, i+300, ws);
            if(s.size())
               classes[s] = i;
         }

      if(cat >= 0)
         JM_USE_FACET(l, __JM_STD::messages<wchar_t>).close(cat);
   }
}

__JM_STD::string RE_CALL to_narrow(const __JM_STD::wstring& is, const __JM_STD::locale& l)
{
   __JM_STD::string out;
   __JM_STD::string t;
   typedef __JM_STD::codecvt<wchar_t, char, __JM_STDC::mbstate_t> cvt_type;
   const cvt_type& cvt = JM_USE_FACET(l, cvt_type);
   __JM_STDC::mbstate_t _State = 0;
   for(unsigned int i = 0; i < is.size(); ++i)
   {
      const wchar_t _X = is[i];
      const wchar_t *_S;
      char *_D;
      t.erase();
      bool done = false;
      while(!done)
      {
         t.append(8, '\0');
         switch( cvt.out(_State, &_X, &_X + 1, _S, t.begin(), t.end(), _D) )
         {
         case __JM_STD::codecvt_base::partial:
            if (_S == &_X)
            {
               // no output, need more input before next output produced:
               done = true;
               break;
            }
            // otherwise increase buffer size and try again:
            break;
         case __JM_STD::codecvt_base::ok:	
         {
            size_t n = _D - t.begin();
            out.append(t.begin(), n);
            done = true;
            break;
         } 
         case __JM_STD::codecvt_base::noconv:
            out.append(1, _X);
            done = true;
            break;
         default:
            return out;
         }
      }
   }
   return out;
}

JM_END_NAMESPACE

#endif




