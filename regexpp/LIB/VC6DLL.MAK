# Microsoft Developer Studio Generated NMAKE File, Based on re_dll.dsp
!IF "$(CFG)" == ""
CFG=Debug
!MESSAGE No configuration specified. Defaulting to Debug.
!ENDIF 

!IF "$(CFG)" != "Release" && "$(CFG)" != "Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "re_dll.mak" CFG="Debug" LIBNAME="name"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IFNDEF LIBNAME
!MESSAGE You can specify a output name when running NMAKE
!MESSAGE by defining the macro LIBNAME on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "vc6.mak" CFG="re_lib - Win32 Debug" LIBNAME="name" LOCALE_MODEL="1"
!MESSAGE 
!ERROR Invalid library name specified
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
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Release"

OUTDIR=.
INTDIR=.
# Begin Custom Macros
OutDir=.
# End Custom Macros

ALL : "$(OUTDIR)\$(LIBNAME).dll"


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
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\$(LIBNAME).dll"
	-@erase "$(OUTDIR)\$(LIBNAME).exp"
	-@erase "$(OUTDIR)\$(LIBNAME).lib"
    -@erase "$(MSVCDIR)\lib\$(LIBNAME).lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "../include" $(CFGX) /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RE_DLL_EXPORTS" /D "RE_BUILD_DLL" /Fp"$(INTDIR)\$(LIBNAME).pch" /YX  /Fd"$(INTDIR)\$(LIBNAME)" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\$(LIBNAME).bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\$(LIBNAME).pdb" /machine:I386 /out:"$(OUTDIR)\$(LIBNAME).dll" /implib:"$(OUTDIR)\$(LIBNAME).lib" 
LINK32_OBJS= \
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

"$(OUTDIR)\$(LIBNAME).dll" :: "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
    copy "$(LIBNAME).dll" "$(MSVCDIR)\bin\$(LIBNAME).dll"
    copy "$(LIBNAME).lib" "$(MSVCDIR)\lib\$(LIBNAME).lib"

!ELSEIF  "$(CFG)" == "Debug"

OUTDIR=.
INTDIR=.
# Begin Custom Macros
OutDir=.
# End Custom Macros

ALL : "$(OUTDIR)\$(LIBNAME).dll"


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
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\$(LIBNAME).dll"
	-@erase "$(OUTDIR)\$(LIBNAME).exp"
	-@erase "$(OUTDIR)\$(LIBNAME).ilk"
	-@erase "$(OUTDIR)\$(LIBNAME).lib"
	-@erase "$(OUTDIR)\$(LIBNAME).pdb"
    -@erase "$(MSVCDIR)\bin\$(LIBNAME).dll"
    -@erase "$(MSVCDIR)\lib\$(LIBNAME).lib"
    -@erase "$(MSVCDIR)\lib\$(LIBNAME).pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\include" $(CFGX) /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RE_DLL_EXPORTS" /D "RE_BUILD_DLL" /Fp"$(INTDIR)\$(LIBNAME).pch" /YX /Fd"$(INTDIR)\$(LIBNAME)" /FD /GZ  /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\$(LIBNAME).bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\$(LIBNAME).pdb" /debug /machine:I386 /out:"$(OUTDIR)\$(LIBNAME).dll" /implib:"$(OUTDIR)\$(LIBNAME).lib" /pdbtype:sept 
LINK32_OBJS= \
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

"$(OUTDIR)\$(LIBNAME).dll" :: "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
    copy "$(LIBNAME).dll" "$(MSVCDIR)\bin\$(LIBNAME).dll"
    copy "$(LIBNAME).lib" "$(MSVCDIR)\lib\$(LIBNAME).lib"
    copy "$(LIBNAME).pdb" "$(MSVCDIR)\lib\$(LIBNAME).pdb"

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








