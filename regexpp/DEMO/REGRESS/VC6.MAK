#
!IF "$(CFG)" == ""
CFG=Release
!MESSAGE No configuration specified. Defaulting to Release.
!ENDIF 

!IF "$(CFG)" != "Release" && "$(CFG)" != "Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "regress.mak" CFG="Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

.SUFFIXES :

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

all :: r1.exe r2.exe r3.exe r4.exe r5.exe r6.exe
	r1 tests.txt test1252.txt
	r2 tests.txt
	r3 tests.txt
	r4 tests.txt test1252.txt
	r5 tests.txt
	r6 tests.txt

!IF  "$(CFG)" == "Release"

OUTDIR=.
INTDIR=.
# Begin Custom Macros
OutDir=.
# End Custom Macros


CLEAN :
	-@erase "$(INTDIR)\r1.obj"
	-@erase "$(INTDIR)\r2.obj"
	-@erase "$(INTDIR)\r3.obj"
	-@erase "$(INTDIR)\r4.obj"
	-@erase "$(INTDIR)\r5.obj"
	-@erase "$(INTDIR)\r6.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\r1.exe"
	-@erase "$(OUTDIR)\r2.exe"
	-@erase "$(OUTDIR)\r3.exe"
	-@erase "$(OUTDIR)\r4.exe"
	-@erase "$(OUTDIR)\r5.exe"
	-@erase "$(OUTDIR)\r6.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "../../include" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /Fp"$*.pch" /YX /FD /c
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\$*.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo


!ELSEIF  "$(CFG)" == "Debug"

OUTDIR=.
INTDIR=.
# Begin Custom Macros
OutDir=.
# End Custom Macros


CLEAN :
	-@erase "$(INTDIR)\r1.obj"
	-@erase "$(INTDIR)\r2.obj"
	-@erase "$(INTDIR)\r3.obj"
	-@erase "$(INTDIR)\r4.obj"
	-@erase "$(INTDIR)\r5.obj"
	-@erase "$(INTDIR)\r6.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\r1.exe"
	-@erase "$(OUTDIR)\r1.ilk"
	-@erase "$(OUTDIR)\r1.pdb"
	-@erase "$(OUTDIR)\r2.exe"
	-@erase "$(OUTDIR)\r2.ilk"
	-@erase "$(OUTDIR)\r2.pdb"
	-@erase "$(OUTDIR)\r3.exe"
	-@erase "$(OUTDIR)\r3.ilk"
	-@erase "$(OUTDIR)\r3.pdb"
	-@erase "$(OUTDIR)\r4.exe"
	-@erase "$(OUTDIR)\r4.ilk"
	-@erase "$(OUTDIR)\r4.pdb"
	-@erase "$(OUTDIR)\r5.exe"
	-@erase "$(OUTDIR)\r5.ilk"
	-@erase "$(OUTDIR)\r5.pdb"
	-@erase "$(OUTDIR)\r6.exe"
	-@erase "$(OUTDIR)\r6.ilk"
	-@erase "$(OUTDIR)\r6.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "../../include" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /Fp"$*.pch" /YX /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\$*.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo 

!ENDIF 


"r1.exe" : "$(OUTDIR)" $(DEF_FILE) r1.obj
    $(LINK32) @<<
  $(LINK32_FLAGS) r1.obj
<<

"r2.exe" : "$(OUTDIR)" $(DEF_FILE) r2.obj
    $(LINK32) @<<
  $(LINK32_FLAGS) r2.obj
<<
"r3.exe" : "$(OUTDIR)" $(DEF_FILE) r3.obj
    $(LINK32) @<<
  $(LINK32_FLAGS) r3.obj
<<
"r4.exe" : "$(OUTDIR)" $(DEF_FILE) r4.obj
    $(LINK32) @<<
  $(LINK32_FLAGS) r4.obj
<<
"r5.exe" : "$(OUTDIR)" $(DEF_FILE) r5.obj
    $(LINK32) @<<
  $(LINK32_FLAGS) r5.obj
<<

"r6.exe" : "$(OUTDIR)" $(DEF_FILE) r6.obj
    $(LINK32) @<<
  $(LINK32_FLAGS) r6.obj
<<

"r1.obj" : r1.cpp
   $(CPP) $(CPP_PROJ) r1.cpp

r2.obj : r2.cpp
   $(CPP) $(CPP_PROJ) r2.cpp

r3.obj : r3.cpp
   $(CPP) $(CPP_PROJ) r3.cpp

r4.obj : r4.cpp
   $(CPP) $(CPP_PROJ) r4.cpp

r5.obj : r5.cpp
   $(CPP) $(CPP_PROJ) r5.cpp

r6.obj : r6.cpp
   $(CPP) $(CPP_PROJ) r6.cpp




