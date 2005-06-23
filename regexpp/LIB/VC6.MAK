# Microsoft Developer Studio Generated NMAKE File, Based on re_lib.dsp
!IF "$(CFG)" == ""
CFG=Debug
!MESSAGE No configuration specified. Defaulting to Debug.
!ENDIF 

#begin texniccenter    
!IFNDEF LIBNAME
!IF "$(CFG)" == "Release"
LIBNAME=mre200l
!ELSE
LIBNAME=mre200dl
!ENDIF
!MESSAGE No lib name specified. Defaulting to $(LIBNAME)
!ENDIF
#end texniccenter    

!IF "$(CFG)" != "Release" && "$(CFG)" != "Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "vc6.mak" CFG="Debug" LIBNAME="name" LOCALE_MODEL="1"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IFNDEF LIBNAME
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "vc6.mak" CFG="Debug" LIBNAME="name"
!MESSAGE 
!ERROR Invalid library name specified
!ENDIF

!IFNDEF MSVCDIR
!ERROR Error: MSVCDir is not defined in your environment
!ENDIF

!IFNDEF LOCALE_NAME
LOCALE_NAME="1"
!ENDIF

!IF $(LOCALE_NAME) == "2"
CFGX=/D RE_LOCALE_C
!ENDIF

!IF $(LOCALE_NAME) == "3"
CFGX=/D RE_LOCALE_CPP
!ENDIF

!IF $(LOCALE_NAME) == "1"
CFGX=/D RE_LOCALE_W32
!ENDIF

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Release"

OUTDIR=.
INTDIR=.
# Begin Custom Macros
OutDir=.
# End Custom Macros

ALL : "$(OUTDIR)\$(LIBNAME).lib"



CLEAN :
	-@erase "$(INTDIR)\re_cls.obj"
	-@erase "$(INTDIR)\re_coll.obj"
	-@erase "$(INTDIR)\re_mss.obj"
	-@erase "$(INTDIR)\re_nls.obj"
	-@erase "$(INTDIR)\re_nlsw.obj"
	-@erase "$(INTDIR)\re_psx.obj"
	-@erase "$(INTDIR)\re_psxw.obj"
	-@erase "$(INTDIR)\re_strw.obj"
	-@erase "$(INTDIR)\re_thrd.obj"
	-@erase "$(INTDIR)\regex.obj"
	-@erase "$(INTDIR)\regfac.obj"
	-@erase "$(INTDIR)\fileiter.obj"
	-@erase "$(INTDIR)\cregex.obj"
	-@erase "$(INTDIR)\$(LIBNAME).idb"
	-@erase "$(MSVCDIR)\lib\$(LIBNAME).lib"
	-@erase "$(OUTDIR)\$(LIBNAME).lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

!IFDEF MULTITHREAD
CPP_PROJ=/nologo /MT /W3 /GX /O2 /I "..\include" $(CFGX) /D "_MT" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\$(LIBNAME).pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\$(LIBNAME)" /FD /c 
!ELSE
CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "..\include" $(CFGX) /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\$(LIBNAME).pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\$(LIBNAME)" /FD /c 
!ENDIF
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\$(LIBNAME).bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\$(LIBNAME).lib" 
LIB32_OBJS= \
	"$(INTDIR)\regfac.obj" \
	"$(INTDIR)\re_coll.obj" \
	"$(INTDIR)\re_mss.obj" \
	"$(INTDIR)\re_nls.obj" \
	"$(INTDIR)\re_nlsw.obj" \
	"$(INTDIR)\re_psx.obj" \
	"$(INTDIR)\re_psxw.obj" \
	"$(INTDIR)\re_strw.obj" \
	"$(INTDIR)\re_thrd.obj" \
	"$(INTDIR)\regex.obj" \
	"$(INTDIR)\fileiter.obj" \
	"$(INTDIR)\cregex.obj" \
	"$(INTDIR)\re_cls.obj"

"$(OUTDIR)\$(LIBNAME).lib" :: "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<
    copy "$(LIBNAME).lib" "$(MSVCDIR)\lib\$(LIBNAME).lib"
#begin texniccenter
	if not exist ..\..\output\Release (	md ..\..\output\Release)	
	copy mre200l.lib ..\..\output\Release
#end texniccenter

!ELSEIF  "$(CFG)" == "Debug"

OUTDIR=.
INTDIR=.
# Begin Custom Macros
OutDir=.
# End Custom Macros

ALL : "$(OUTDIR)\$(LIBNAME).lib"


CLEAN :
	-@erase "$(INTDIR)\re_cls.obj"
	-@erase "$(INTDIR)\re_coll.obj"
	-@erase "$(INTDIR)\re_mss.obj"
	-@erase "$(INTDIR)\re_nls.obj"
	-@erase "$(INTDIR)\re_nlsw.obj"
	-@erase "$(INTDIR)\re_psx.obj"
	-@erase "$(INTDIR)\re_psxw.obj"
	-@erase "$(INTDIR)\re_strw.obj"
	-@erase "$(INTDIR)\re_thrd.obj"
	-@erase "$(INTDIR)\regex.obj"
	-@erase "$(INTDIR)\regfac.obj"
	-@erase "$(INTDIR)\fileiter.obj"
	-@erase "$(INTDIR)\cregex.obj"
	-@erase "$(INTDIR)\$(LIBNAME).idb"
	-@erase "$(INTDIR)\$(LIBNAME).pdb"
	-@erase "$(OUTDIR)\$(LIBNAME).lib"
	-@erase "$(MSVCDIR)\lib\$(LIBNAME).lib"
	-@erase "$(MSVCDIR)\lib\$(LIBNAME).pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

!IFDEF MULTITHREAD
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\include" $(CFGX) /D "WIN32" /D "_MT" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\$(LIBNAME).pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\$(LIBNAME)" /FD /GZ  /c 
!ELSE
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "..\include" $(CFGX) /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\$(LIBNAME).pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\$(LIBNAME)" /FD /GZ  /c 
!ENDIF
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\$(LIBNAME).bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\$(LIBNAME).lib" 
LIB32_OBJS= \
	"$(INTDIR)\regfac.obj" \
	"$(INTDIR)\re_coll.obj" \
	"$(INTDIR)\re_mss.obj" \
	"$(INTDIR)\re_nls.obj" \
	"$(INTDIR)\re_nlsw.obj" \
	"$(INTDIR)\re_psx.obj" \
	"$(INTDIR)\re_psxw.obj" \
	"$(INTDIR)\re_strw.obj" \
	"$(INTDIR)\re_thrd.obj" \
	"$(INTDIR)\regex.obj" \
	"$(INTDIR)\fileiter.obj" \
	"$(INTDIR)\cregex.obj" \
	"$(INTDIR)\re_cls.obj"

"$(OUTDIR)\$(LIBNAME).lib" :: "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<
    copy "$(LIBNAME).lib" "$(MSVCDIR)\lib\$(LIBNAME).lib"
    copy "$(LIBNAME).pdb" "$(MSVCDIR)\lib\$(LIBNAME).pdb"
#begin texniccenter
	if not exist ..\..\output\Debug (md ..\..\output\Debug) 
	copy mre200dl.lib ..\..\output\Debug
#end texniccenter

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("$(LIBNAME).dep")
!INCLUDE "$(LIBNAME).dep"
!ELSE 
!MESSAGE Warning: cannot find "$(LIBNAME).dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Release" || "$(CFG)" == "Debug"
SOURCE=..\src\re_cls.cpp

"$(INTDIR)\re_cls.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\re_coll.cpp

"$(INTDIR)\re_coll.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\re_mss.cpp

"$(INTDIR)\re_mss.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\re_nls.cpp

"$(INTDIR)\re_nls.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\re_nlsw.cpp

"$(INTDIR)\re_nlsw.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\re_psx.cpp

"$(INTDIR)\re_psx.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=..\src\re_psxw.cpp

"$(INTDIR)\re_psxw.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\re_strw.cpp

"$(INTDIR)\re_strw.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\re_thrd.cpp

"$(INTDIR)\re_thrd.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\regex.cpp

"$(INTDIR)\regex.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\regfac.cpp

"$(INTDIR)\regfac.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=..\src\fileiter.cpp

"$(INTDIR)\fileiter.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=..\src\cregex.cpp

"$(INTDIR)\cregex.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)

!ENDIF 








