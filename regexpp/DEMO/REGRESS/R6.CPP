
//
// master include file for different regression builds...
//
// r1.cpp
// wide char's, locale CPP

#define RE_LOCALE_CPP
#define TEST_UNICODE
// and disable automatic library inclusion
#define RE_LIB_H


#include "parse.cpp"
#include "regress.cpp"
#include "rlocale.cpp"
#include "tests.cpp"

#include "../../src/cregex.cpp"
#include "../../src/fileiter.cpp"
#include "../../src/re_cls.cpp"
#include "../../src/re_coll.cpp"
#include "../../src/re_mss.cpp"
#include "../../src/re_nls.cpp"
#include "../../src/re_nlsw.cpp"
#include "../../src/re_psx.cpp"
#include "../../src/re_psxw.cpp"
#include "../../src/re_strw.cpp"
#include "../../src/re_thrd.cpp"
#include "../../src/regex.cpp"
#include "../../src/regfac.cpp"







