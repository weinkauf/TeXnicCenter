

#ifndef JM_NO_LOCALE_H
#ifdef RE_LOCALE_CPP
#include "regress.h"
#include "rlocale.h"

void setuplocale()
{
#ifdef _MSC_VER
   __JM_STD::locale l = __JM_STD::_ADDFAC(__JM_STD::locale(), new custom_collate_w());
   l = __JM_STD::_ADDFAC(l, new custom_collate());
   __JM_STD::locale::global(l);
#else
   __JM_STD::locale l(__JM_STD::locale(), new custom_collate_w());
   l = __JM_STD::locale(l, new custom_collate());
   __JM_STD::locale::global(l);
#endif
}


const unsigned char* collate_map = (const unsigned char*)
                          "\x74\x75\x76\x77\x78\x79\x7a\x7b\x7c\x7d\x7e\x7f\x80\x81\x82\x83\x84\x85"
                          "\x86\x87\x88\x89\x8a\x8b\x8c\x8d\x8e\x8f\x90\x91\x92\x93\x94\x95\x96\x97"
                          "\x98\x99\x9a\x9b\x9c\x9d\x9e\x9f\xa0\xa1\xa2\xa3\x0\x1\x2\x3\x4\x5\x6\x7"
                          "\x8\x9\xa4\xa5\xa6\xa7\xa8\xa9\xaa\xa\x11\x12\x14\x15\x1a\x1b\x1c\x1d\x22"
                          "\x23\x24\x25\x26\x28\x2e\x2f\x30\x31\x32\x33\x38\x39\x3a\x3b\x3e\xab\xac"
                          "\xad\xae\xaf\xb0\x3f\x46\x47\x49\x4a\x4f\x50\x51\x52\x57\x58\x59\x5a\x5b"
                          "\x5d\x63\x64\x65\x66\x67\x68\x6d\x6e\x6f\x70\x73\xb1\xb2\xb3\xb4\xb5\xb6"
                          "\xb7\xb8\xb9\xba\xbb\xbc\xbd\xbe\xbf\xc0\xc1\xc2\xc3\xc4\xc5\xc6\xc7\xc8"
                          "\xc9\xca\xcb\xcc\xcd\xce\xcf\xd0\xd1\xd2\xd3\xd4\x3d\xd5\xd6\xd7\xd8\xd9"
                          "\xda\xdb\xdc\xdd\xde\xdf\xe0\xe1\xe2\xe3\xe4\xe5\xe6\xe7\xe8\xe9\xea\xeb"
                          "\xec\xed\xee\xef\xf0\xf1\xf2\xf3\xf4\xb\xc\xd\xe\xf\x10\xf5\x13\x16\x17"
                          "\x18\x19\x1e\x1f\x20\x21\xf6\x27\x29\x2a\x2b\x2c\x2d\xf7\xf8\x34\x35\x36"
                          "\x37\x3c\xf9\xfa\x40\x41\x42\x43\x44\x45\xfb\x48\x4b\x4c\x4d\x4e\x53\x54"
                          "\x55\x56\xfc\x5c\x5e\x5f\x60\x61\x62\xfd\xfe\x69\x6a\x6b\x6c\x71\xff\x72";

int custom_collate::do_compare(const char* low1, const char* high1, const char* low2, const char* high2) const
{
   return transform(low1, high1).compare(transform(low2, high2));
}

custom_collate::string_type custom_collate::do_transform(const char* low, const char* high) const
{
   string_type s;
   while(low != high)
   {
      s.append(1, collate_map[(unsigned char)*low]);
      ++low;
   }
   return s;
}

int custom_collate_w::do_compare(const wchar_t* low1, const wchar_t* high1, const wchar_t* low2, const wchar_t* high2) const
{
   return transform(low1, high1).compare(transform(low2, high2));
}

custom_collate_w::string_type custom_collate_w::do_transform(const wchar_t* low, const wchar_t* high) const
{
   string_type s;
   while(low != high)
   {
      if((*low < 256) && (*low >= 0))
         s.append(1, collate_map[(unsigned short)*low]);
      else
         s.append(1, *low);
      ++low;
   }
   return s;
}

const char* collate_lower_map = "\x0\x1\x2\x3\x4\x5\x6\x7\x8\x9\xa\xb\xc\xd\xe\xf\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f !\"#$%&'()*+,-./0123456789:;<=>?@abcdefghijklmnopqrstuvwxyz[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~€‚ƒ„…†‡ˆ‰Š‹Œ‘’“”•–—˜™š›œÿ ¡¢£¤¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿àáâãäåæçèéêëìíîïĞñòóôõö×øùúûüıŞßàáâãäåæçèéêëìíîïğñòóôõö÷øùúûüışÿ";

custom_collate_i::string_type custom_collate_i::do_transform(const char* low, const char* high) const
{
   string_type s;
   while(low != high)
   {
      s.append(1, collate_map[(unsigned char)collate_lower_map[(unsigned char)*low]]);
      ++low;
   }
   return s;
}

custom_collate_wi::string_type custom_collate_wi::do_transform(const wchar_t* low, const wchar_t* high) const
{
   string_type s;
   while(low != high)
   {
      if((*low < 256) && (*low >= 0))
         s.append(1, collate_map[(unsigned char)collate_lower_map[(unsigned short)*low]]);
      else
         s.append(1, *low);
      ++low;
   }
   return s;
}


#endif
#endif





