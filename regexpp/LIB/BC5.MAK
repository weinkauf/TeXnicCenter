
#
# Borland C++ tools
#
# BCROOT defines the root directory of your bc5 install
#
!ifndef BCROOT
BCROOT=$(MAKEDIR)\..
!endif

!ifndef LIBNAME
!message no library name specified
!message syntax: make -fbcb.mak LIBNAME=mylib
!message 
!else

BCC32   = $(BCROOT)\bin\Bcc32.exe 
TLINK32 = $(BCROOT)\bin\TLink32.exe
TLIB = $(BCROOT)\bin\tlib.exe

!IFDEF MULTITHREAD
C2 = -WM -D_NO_VCL
RTL = cw32mti.lib
!ELSE
C2 = -WM- -D_NO_VCL
RTL = cw32i.lib
!ENDIF

!IFDEF DLL
C3 = -DRE_BUILD_DLL -tWD -D_RTLDLL -D_RWSTDDLL
!ELSE
C3 = 
!ENDIF

LINKOPTS= -aa -Tpd -x -L$(BCROOT)\LIB
COMPOPTS= -O2 $(C2) $(C3) -w-inl -w-aus -w-rch -DSTRICT; -I$(BCROOT)\include;..\include;

LIB32OBJS= \
	regfac.obj \
	re_coll.obj \
	re_mss.obj \
	re_nls.obj \
	re_nlsw.obj \
	re_psx.obj \
	re_psxw.obj \
	re_strw.obj \
	re_thrd.obj \
	regex.obj \
	fileiter.obj \
	cregex.obj \
	re_cls.obj

!IFDEF DLL

$(LIBNAME).dll :: $(LIB32OBJS)
  $(TLINK32) @&&|
 /v $(LinkFLAGS32) $(LINKOPTS) +
$(BCROOT)\lib\c0d32.obj+
	re_coll.obj+
	re_mss.obj+
	re_nls.obj+
	re_nlsw.obj+
	re_psx.obj+
	re_psxw.obj+
	re_strw.obj+
	re_thrd.obj+
	regex.obj+
	fileiter.obj+
	cregex.obj+
	re_cls.obj+
$(BCROOT)\lib\import32.lib+
$(BCROOT)\lib\$(RTL)
$<,$*
|
	$(BCROOT)\bin\implib $(LIBNAME).lib $(LIBNAME).dll
	copy $(LIBNAME).lib $(BCROOT)\lib\$(LIBNAME).lib
	copy $(LIBNAME).dll $(BCROOT)\bin\$(LIBNAME).dll

!ELSE

$(BCROOT)\lib\$(LIBNAME).lib : $(LIBNAME).lib
	copy $(LIBNAME).lib $(BCROOT)\lib\$(LIBNAME).lib

$(LIBNAME).lib : $(LIB32OBJS)
  $(TLIB) /P32 @&&| 
  $(LIBNAME).lib +	regfac.obj \
	+re_coll.obj \
	+re_mss.obj \
	+re_nls.obj \
	+re_nlsw.obj \
	+re_psx.obj \
	+re_psxw.obj \
	+re_strw.obj \
	+re_thrd.obj \
	+regex.obj \
	+fileiter.obj \
	+cregex.obj \
	+re_cls.obj
$<,$*
|
	
!ENDIF

cregex.obj :  ../src/cregex.cpp
  $(BCC32) -c @&&|
 $(COMPOPTS) -o$@ ../src/cregex.cpp
|

fileiter.obj :  ../src/fileiter.cpp
  $(BCC32) -c @&&|
 $(COMPOPTS) -o$@ ../src/fileiter.cpp
|

re_cls.obj :  ../src/re_cls.cpp
  $(BCC32) -c @&&|
 $(COMPOPTS) -o$@ ../src/re_cls.cpp
|

re_coll.obj :  ../src/re_coll.cpp
  $(BCC32) -c @&&|
 $(COMPOPTS) -o$@ ../src/re_coll.cpp
|

re_mss.obj :  ../src/re_mss.cpp
  $(BCC32) -c @&&|
 $(COMPOPTS) -o$@ ../src/re_mss.cpp
|

re_nls.obj :  ../src/re_nls.cpp
  $(BCC32) -c @&&|
 $(COMPOPTS) -o$@ ../src/re_nls.cpp
|

re_nlsw.obj :  ../src/re_nlsw.cpp
  $(BCC32) -c @&&|
 $(COMPOPTS) -o$@ ../src/re_nlsw.cpp
|

re_psx.obj :  ../src/re_psx.cpp
  $(BCC32) -c @&&|
 $(COMPOPTS) -o$@ ../src/re_psx.cpp
|

re_psxw.obj :  ../src/re_psxw.cpp
  $(BCC32) -c @&&|
 $(COMPOPTS) -o$@ ../src/re_psxw.cpp
|

re_strw.obj :  ../src/re_strw.cpp
  $(BCC32) -c @&&|
 $(COMPOPTS) -o$@ ../src/re_strw.cpp
|

re_thrd.obj :  ../src/re_thrd.cpp
  $(BCC32) -c @&&|
 $(COMPOPTS) -o$@ ../src/re_thrd.cpp
|

regex.obj :  ../src/regex.cpp
  $(BCC32) -c @&&|
 $(COMPOPTS) -o$@ ../src/regex.cpp
|

regfac.obj :  ../src/regfac.cpp
  $(BCC32) -c @&&|
 $(COMPOPTS) -o$@ ../src/regfac.cpp
|


!endif



















