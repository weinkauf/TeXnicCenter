
#ifndef JM_NO_LOCALE_H
#ifdef RE_LOCALE_CPP

#ifndef RLOCALE_H
#define RLOCALE_H

#include "regress.h"
#include <locale>

//
// this header declares locale facets that are designed to stretch
// regex's locale handling abilities.
//
// custom_collate and custom_collate_w define a unique collating
// sequence that is deliberately very different from ASCII ordering.
//
// custom_collate_i and custom_collate_wi impliment case insensitive
// collation.
//

void setuplocale();

class custom_collate : public __JM_STD::collate<char>
{
   virtual int do_compare(const char* low1, const char* high1, const char* low2, const char* high2) const;
   virtual string_type do_transform(const char* low, const char* high) const;
};

class custom_collate_w : public __JM_STD::collate<wchar_t>
{
   virtual int do_compare(const wchar_t* low1, const wchar_t* high1, const wchar_t* low2, const wchar_t* high2) const;
   virtual string_type do_transform(const wchar_t* low, const wchar_t* high) const;
};

class custom_collate_i : public custom_collate
{
   virtual string_type do_transform(const char* low, const char* high) const;
};

class custom_collate_wi : public custom_collate_w
{
   virtual string_type do_transform(const wchar_t* low, const wchar_t* high) const;
};

#endif
#endif
#endif


