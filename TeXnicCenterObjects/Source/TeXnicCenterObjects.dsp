# Microsoft Developer Studio Project File - Name="TeXnicCenterObjects" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=TeXnicCenterObjects - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TeXnicCenterObjects.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TeXnicCenterObjects.mak" CFG="TeXnicCenterObjects - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TeXnicCenterObjects - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "TeXnicCenterObjects - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TeXnicCenterObjects - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Lib"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Custom Build - Building type library...
OutDir=.\../Lib
InputPath=\Projekte\TeXnicCenter\TeXnicCenterObjects\Lib\TeXnicCenterObjects.lib
SOURCE="$(InputPath)"

"$(OutDir)\TeXnicCenterObjects.tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	midl /client none /server none /win32 TeXnicCenterObjects.idl 
	xcopy /y /f TeXnicCenterObjects.tlb "$(OutDir)" 
	xcopy /f TeXnicCenterObjects.tlb "$(OutDir)" 
	del TeXnicCenterObjects.tlb 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "TeXnicCenterObjects - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Lib"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../Lib/TeXnicCenterObjectsD.lib"
# Begin Custom Build - Building type library...
OutDir=.\../Lib
InputPath=\Projekte\TeXnicCenter\TeXnicCenterObjects\Lib\TeXnicCenterObjectsD.lib
SOURCE="$(InputPath)"

"$(OutDir)\TeXnicCenterObjects.tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	midl /client none /server none /win32 TeXnicCenterObjects.idl 
	xcopy /y /f TeXnicCenterObjects.tlb "$(OutDir)" 
	xcopy /f TeXnicCenterObjects.tlb "$(OutDir)" 
	del TeXnicCenterObjects.tlb 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "TeXnicCenterObjects - Win32 Release"
# Name "TeXnicCenterObjects - Win32 Debug"
# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DocumentWizard_i.c
# End Source File
# Begin Source File

SOURCE=.\Project_i.c
# End Source File
# Begin Source File

SOURCE=.\ProjectWizard_i.c
# End Source File
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DocumentWizard.h
# End Source File
# Begin Source File

SOURCE=.\Project.h
# End Source File
# Begin Source File

SOURCE=.\ProjectWizard.h
# End Source File
# End Group
# Begin Group "Include Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Include\TeXnicCenterObjects.h
# End Source File
# End Group
# Begin Group "Interface-Files"

# PROP Default_Filter "idl"
# Begin Source File

SOURCE=.\DocumentWizard.idl

!IF  "$(CFG)" == "TeXnicCenterObjects - Win32 Release"

# Begin Custom Build - MIDL
InputPath=.\DocumentWizard.idl
InputName=DocumentWizard

BuildCmds= \
	midl /client none /server none /win32 $(InputName).idl

"$(InputName)_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "TeXnicCenterObjects - Win32 Debug"

# Begin Custom Build - MIDL
InputPath=.\DocumentWizard.idl
InputName=DocumentWizard

BuildCmds= \
	midl /client none /server none /win32 $(InputName).idl

"$(InputName)_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Project.idl

!IF  "$(CFG)" == "TeXnicCenterObjects - Win32 Release"

# Begin Custom Build - MIDL
InputPath=.\Project.idl
InputName=Project

BuildCmds= \
	midl /client none /server none /win32 $(InputName).idl

"$(InputName)_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "TeXnicCenterObjects - Win32 Debug"

# Begin Custom Build - MIDL
InputPath=.\Project.idl
InputName=Project

BuildCmds= \
	midl /client none /server none /win32 $(InputName).idl

"$(InputName)_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ProjectWizard.idl

!IF  "$(CFG)" == "TeXnicCenterObjects - Win32 Release"

# Begin Custom Build - MIDL
InputPath=.\ProjectWizard.idl
InputName=ProjectWizard

BuildCmds= \
	midl /client none /server none /win32 $(InputName).idl

"$(InputName)_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "TeXnicCenterObjects - Win32 Debug"

# Begin Custom Build - MIDL
InputPath=.\ProjectWizard.idl
InputName=ProjectWizard

BuildCmds= \
	midl /client none /server none /win32 $(InputName).idl

"$(InputName)_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Typelibrary-Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TeXnicCenterObjects.idl
# End Source File
# End Group
# End Target
# End Project
