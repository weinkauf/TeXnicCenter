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
  *	FILE     cregex.cpp
  *	VERSION  2.00
  */


#if !defined(JM_NO_STRING_H)

#include <cregex>
#include <regex>
#include <map>
#include <fileiter.h>
#include <stdio.h>

#ifdef __BORLANDC__
#if __BORLANDC__ < 0x530
//
// we need to instantiate the vector classes we use
// since declaring a reference to type doesn't seem to
// do the job...
__JM_STD::vector<unsigned int> inst1;
__JM_STD::vector<__JM_STD::string> inst2;
#endif
#endif

template <class iterator>
__JM_STD::string to_string(iterator i, iterator j)
{
   __JM_STD::string s;
   while(i != j)
   {
      s.append(1, *i);
      ++i;
   }
   return s;
}

inline __JM_STD::string to_string(const char* i, const char* j)
{
   return __JM_STD::string(i, j);
}


JM_NAMESPACE(__JM)

class RegExData
{
public:
   enum type
   {
      type_pc,
      type_pf,
      type_copy,
   };
   regex e;
   cmatch m;
   reg_match<mapfile::iterator, regex::alloc_type> fm;
   type t;
   const char* pbase;
   mapfile::iterator fbase;
   __JM_STD::map<int, __JM_STD::string, __JM_STD::less<int> > strings;
   __JM_STD::map<int, int, __JM_STD::less<int> > positions;
   void update();
};

void RegExData::update()
{
   strings.erase(strings.begin(), strings.end());
   positions.erase(positions.begin(), positions.end());
   if(t == type_pc)
   {
      for(unsigned int i = 0; i < m.size(); ++i)
      {
         strings[i] = __JM_STD::string(m[i].first, m[i].second);
         positions[i] = m[i].matched ? m[i].first - pbase : -1;
      }
   }
   else
   {
      for(unsigned int i = 0; i < m.size(); ++i)
      {
         strings[i] = to_string(fm[i].first, fm[i].second);
         positions[i] = fm[i].matched ? fm[i].first - fbase : -1;
      }
   }
   t = type_copy;
}

RegEx::RegEx()
{
   pdata = new RegExData();
}

RegEx::RegEx(const RegEx& o)
{
   pdata = new RegExData(*(o.pdata));
}

RegEx::~RegEx()
{
   delete pdata;
}

RegEx::RegEx(const char* c, bool icase)
{
   pdata = new RegExData();
   SetExpression(c, icase);
}

RegEx::RegEx(const __JM_STD::string& s, bool icase)
{
   pdata = new RegExData();
   SetExpression(s.c_str(), icase);
}

RegEx& RegEx::operator=(const RegEx& o)
{
   *pdata = *(o.pdata);
   return *this;
}

RegEx& RegEx::operator=(const char* p)
{
   SetExpression(p, false);
   return *this;
}

unsigned int RegEx::SetExpression(const char* p, bool icase)
{
   unsigned int f = icase ? regbase::normal | regbase::use_except | regbase::icase : regbase::normal | regbase::use_except;
   return pdata->e.set_expression(p, f);
}

__JM_STD::string RegEx::Expression()const
{
   return pdata->e.expression();
}

//
// now matching operators:
//
bool RegEx::Match(const char* p, unsigned int flags)
{
   pdata->t = RegExData::type_pc;
   pdata->pbase = p;
   const char* end = p;
   while(*end)++end;

   if(query_match(p, end, pdata->m, pdata->e, flags))
   {
      pdata->update();
      return true;
   }
   return false;
}

bool RegEx::Search(const char* p, unsigned int flags)
{
   pdata->t = RegExData::type_pc;
   pdata->pbase = p;
   const char* end = p;
   while(*end)++end;

   if(reg_search(p, end, pdata->m, pdata->e, flags))
   {
      pdata->update();
      return true;
   }
   return false;
}

struct pred1
{
   GrepCallback cb;
   RegEx* pe;
   pred1(GrepCallback c, RegEx* i) : cb(c), pe(i) {}
   bool operator()(const cmatch& m)
   {
      pe->pdata->m = m;
      return cb(*pe);
   }
};

unsigned int RegEx::Grep(GrepCallback cb, const char* p, unsigned int flags)
{
   pdata->t = RegExData::type_pc;
   pdata->pbase = p;
   const char* end = p;
   while(*end)++end;

   unsigned int result = reg_grep(pred1(cb, this), p, end, pdata->e, flags);
   if(result)
      pdata->update();
   return result;
}

struct pred2
{
   __JM_STD::vector<__JM_STD::string>& v;
   pred2(__JM_STD::vector<__JM_STD::string>& o) : v(o) {}
   bool operator()(const cmatch& m)
   {
      v.push_back(__JM_STD::string(m[0].first, m[0].second));
      return true;
   }
};


unsigned int RegEx::Grep(__JM_STD::vector<__JM_STD::string>& v, const char* p, unsigned int flags)
{
   pdata->t = RegExData::type_pc;
   pdata->pbase = p;
   const char* end = p;
   while(*end)++end;

   unsigned int result = reg_grep(pred2(v), p, end, pdata->e, flags);
   if(result)
      pdata->update();
   return result;
}

struct pred3
{
   __JM_STD::vector<unsigned int>& v;
   const char* base;
   pred3(__JM_STD::vector<unsigned int>& o, const char* pb) : v(o), base(pb) {}
   bool operator()(const cmatch& m)
   {
      v.push_back(m[0].first - base);
      return true;
   }
};

unsigned int RegEx::Grep(__JM_STD::vector<unsigned int>& v, const char* p, unsigned int flags)
{
   pdata->t = RegExData::type_pc;
   pdata->pbase = p;
   const char* end = p;
   while(*end)++end;

   unsigned int result = reg_grep(pred3(v, p), p, end, pdata->e, flags);
   if(result)
      pdata->update();
   return result;
}

struct pred4
{
   GrepFileCallback cb;
   const char* file;
   RegEx* pe;
   pred4(GrepFileCallback c, RegEx* i, const char* f) : cb(c), pe(i), file(f) {}
   bool operator()(const cmatch& m)
   {
      pe->pdata->m = m;
      return cb(file, *pe);
   }
};


unsigned int RegEx::GrepFiles(GrepFileCallback cb, const char* files, bool recurse, unsigned int flags)
{
   unsigned int result = 0;
   file_iterator start(files);
   file_iterator end;
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

      // now get the file mask bit of "files":
      const char* ptr = files;
      while(*ptr) ++ptr;
      while((ptr != files) && (*ptr != *directory_iterator::separator()) && (*ptr != '/'))--ptr;
      if(ptr != files) ++ptr;

      while(dstart != dend)
      {
         sprintf(buf, "%s%s%s", dstart.path(), directory_iterator::separator(), ptr);
         result += GrepFiles(cb, buf, recurse, flags);
         ++dstart;
      }
   }

   while(start != end)
   {
      mapfile map(*start);
      pdata->t = RegExData::type_pf;
      pdata->fbase = map.begin();

      result += reg_grep(pred4(cb, this, *start), map.begin(), map.end(), pdata->e, flags);
      pdata->update();
   }

   return result;
}

unsigned int RegEx::FindFiles(FindFilesCallback cb, const char* files, bool recurse, unsigned int flags)
{
   unsigned int result = 0;
   file_iterator start(files);
   file_iterator end;
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

      // now get the file mask bit of "files":
      const char* ptr = files;
      while(*ptr) ++ptr;
      while((ptr != files) && (*ptr != *directory_iterator::separator()) && (*ptr != '/'))--ptr;
      if(ptr != files) ++ptr;

      while(dstart != dend)
      {
         sprintf(buf, "%s%s%s", dstart.path(), directory_iterator::separator(), ptr);
         result += FindFiles(cb, buf, recurse, flags);
         ++dstart;
      }
   }

   while(start != end)
   {
      mapfile map(*start);
      pdata->t = RegExData::type_pf;
      pdata->fbase = map.begin();

      if(reg_search(map.begin(), map.end(), pdata->fm, pdata->e, flags))
         ++result;
      pdata->update();
   }

   return result;
}

//
// now operators for returning what matched in more detail:
//
unsigned int RegEx::Position(int i)const
{
   switch(pdata->t)
   {
   case RegExData::type_pc:
      return pdata->m[i].matched ? pdata->m[i].first - pdata->pbase : -1;
   case RegExData::type_pf:
      return pdata->fm[i].matched ? pdata->fm[i].first - pdata->fbase : -1;
   case RegExData::type_copy:
      {
      __JM_STD::map<int, int, __JM_STD::less<int> >::iterator pos = pdata->positions.find(i);
      if(pos == pdata->positions.end())
         return -1;
      return (*pos).second;
      }
   }
   return -1;
}

unsigned int RegEx::Length(int i)const
{
   switch(pdata->t)
   {
   case RegExData::type_pc:
      return pdata->m[i].matched ? pdata->m[i].second - pdata->m[i].first : 0;
   case RegExData::type_pf:
      return pdata->fm[i].matched ? pdata->fm[i].second - pdata->fm[i].first : 0;
   case RegExData::type_copy:
      {
      __JM_STD::map<int, __JM_STD::string, __JM_STD::less<int> >::iterator pos = pdata->strings.find(i);
      if(pos == pdata->strings.end())
         return -1;
      return (*pos).second.size();
      }
   }
   return 0;
}

__JM_STD::string RegEx::What(int i)const
{
   switch(pdata->t)
   {
   case RegExData::type_pc:
      return pdata->m[i].matched ? __JM_STD::string(pdata->m[i].first, pdata->m[i].second) : __JM_STD::string();
   case RegExData::type_pf:
      return pdata->m[i].matched ? to_string(pdata->m[i].first, pdata->m[i].second) : __JM_STD::string();
   case RegExData::type_copy:
      {
      __JM_STD::map<int, __JM_STD::string, __JM_STD::less<int> >::iterator pos = pdata->strings.find(i);
      if(pos == pdata->strings.end())
         return __JM_STD::string();
      return (*pos).second;
      }
   }
   return __JM_STD::string();
}


JM_END_NAMESPACE

#endif




