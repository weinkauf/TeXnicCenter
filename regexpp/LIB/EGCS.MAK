
LIBNAME=regex++

CPP=gcc
AR=ar

INTDIR=.
OUTDIR=.

CPP_PROJ= -O -I ../include $(CFGX) -c 
	
LIB32_OBJS= \
	$(INTDIR)/regfac.o \
	$(INTDIR)/re_coll.o \
	$(INTDIR)/re_mss.o \
	$(INTDIR)/re_nls.o \
	$(INTDIR)/re_nlsw.o \
	$(INTDIR)/re_psx.o \
	$(INTDIR)/re_psxw.o \
	$(INTDIR)/re_strw.o \
	$(INTDIR)/re_thrd.o \
	$(INTDIR)/regex.o \
	$(INTDIR)/fileiter.o \
	$(INTDIR)/cregex.o \
	$(INTDIR)/re_cls.o

$(OUTDIR)/lib$(LIBNAME).a :: $(LIB32_OBJS)
	$(AR) -rc lib$(LIBNAME).a 	$(INTDIR)/regfac.o $(INTDIR)/re_coll.o $(INTDIR)/re_mss.o $(INTDIR)/re_nls.o $(INTDIR)/re_nlsw.o $(INTDIR)/re_psx.o $(INTDIR)/re_psxw.o $(INTDIR)/re_strw.o $(INTDIR)/re_thrd.o
	$(AR) -rc lib$(LIBNAME).a 	$(INTDIR)/regex.o $(INTDIR)/fileiter.o $(INTDIR)/cregex.o $(INTDIR)/re_cls.o

		
$(INTDIR)/re_cls.o : ../src/re_cls.cpp
	$(CPP) $(CPP_PROJ) ../src/re_cls.cpp


$(INTDIR)/re_coll.o : ../src/re_coll.cpp
	$(CPP) $(CPP_PROJ) ../src/re_coll.cpp


$(INTDIR)/re_mss.o : ../src/re_mss.cpp
	$(CPP) $(CPP_PROJ) ../src/re_mss.cpp


$(INTDIR)/re_nls.o : ../src/re_nls.cpp
	$(CPP) $(CPP_PROJ) ../src/re_nls.cpp


$(INTDIR)/re_nlsw.o : ../src/re_nlsw.cpp
	$(CPP) $(CPP_PROJ) ../src/re_nlsw.cpp


$(INTDIR)/re_psx.o : ../src/re_psx.cpp
	$(CPP) $(CPP_PROJ) ../src/re_psx.cpp

$(INTDIR)/re_psxw.o : ../src/re_psxw.cpp
	$(CPP) $(CPP_PROJ) ../src/re_psxw.cpp


$(INTDIR)/re_strw.o : ../src/re_strw.cpp
	$(CPP) $(CPP_PROJ) ../src/re_strw.cpp


$(INTDIR)/re_thrd.o : ../src/re_thrd.cpp
	$(CPP) $(CPP_PROJ) ../src/re_thrd.cpp


$(INTDIR)/regex.o : ../src/regex.cpp
	$(CPP) $(CPP_PROJ) ../src/regex.cpp


$(INTDIR)/regfac.o : ../src/regfac.cpp
	$(CPP) $(CPP_PROJ) ../src/regfac.cpp

$(INTDIR)/fileiter.o : ../src/fileiter.cpp
	$(CPP) $(CPP_PROJ) ../src/fileiter.cpp

$(INTDIR)/cregex.o : ../src/cregex.cpp
	$(CPP) $(CPP_PROJ) ../src/cregex.cpp













