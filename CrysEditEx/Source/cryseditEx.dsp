# Microsoft Developer Studio Project File - Name="CrysEditEx" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CrysEditEx - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cryseditEx.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cryseditEx.mak" CFG="CrysEditEx - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CrysEditEx - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CrysEditEx - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "CrystalEditEx"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CrysEditEx - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../output/Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../regexpp/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "CE_DLL_BUILD" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /map /debug /debugtype:coff /machine:I386 /def:".\CrysEditEx.def" /implib:"../Lib/cryseditEx.lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "CrysEditEx - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../output/Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../regexpp/include" /I "../../myspell/" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "CE_DLL_BUILD" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../output/Debug/cryseditExD.dll" /implib:"../Lib/cryseditExD.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "CrysEditEx - Win32 Release"
# Name "CrysEditEx - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;inl"
# Begin Source File

SOURCE=..\..\myspell\affix.cxx
# End Source File
# Begin Source File

SOURCE=..\..\myspell\affixmgr.cxx
# End Source File
# Begin Source File

SOURCE=.\CCrystalEditView.cpp
# End Source File
# Begin Source File

SOURCE=.\CCrystalTextBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\CCrystalTextView.cpp
# End Source File
# Begin Source File

SOURCE=.\CCrystalTextView2.cpp
# End Source File
# Begin Source File

SOURCE=.\CEditReplaceDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CFindTextDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CrysEditEx.cpp
# End Source File
# Begin Source File

SOURCE=.\CrysEditEx.def
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\CrysEditEx.rc
# End Source File
# Begin Source File

SOURCE=.\CrysEditExD.def
# End Source File
# Begin Source File

SOURCE=.\CrystalParser.cpp
# End Source File
# Begin Source File

SOURCE=.\CrystalSpell.cpp
# End Source File
# Begin Source File

SOURCE=.\CrystalTextBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\CrystalTextBufferEx.cpp
# End Source File
# Begin Source File

SOURCE=..\..\myspell\hashmgr.cxx
# End Source File
# Begin Source File

SOURCE=.\Inline.cpp
# End Source File
# Begin Source File

SOURCE=..\..\myspell\myspell.cxx
# End Source File
# Begin Source File

SOURCE=.\SpellCheckBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\SpellCheckDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\..\myspell\suggestmgr.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\myspell\affix.hxx
# End Source File
# Begin Source File

SOURCE=..\..\myspell\affixmgr.hxx
# End Source File
# Begin Source File

SOURCE=..\..\myspell\atypes.hxx
# End Source File
# Begin Source File

SOURCE=..\..\myspell\baseaffix.hxx
# End Source File
# Begin Source File

SOURCE=.\CCrystalEditView.h
# End Source File
# Begin Source File

SOURCE=.\CCrystalEditView.inl
# End Source File
# Begin Source File

SOURCE=.\CCrystalTextBuffer.h
# End Source File
# Begin Source File

SOURCE=.\CCrystalTextBuffer.inl
# End Source File
# Begin Source File

SOURCE=.\CCrystalTextView.h
# End Source File
# Begin Source File

SOURCE=.\CCrystalTextView.inl
# End Source File
# Begin Source File

SOURCE=.\cedefs.h
# End Source File
# Begin Source File

SOURCE=.\CEditReplaceDlg.h
# End Source File
# Begin Source File

SOURCE=.\CFindTextDlg.h
# End Source File
# Begin Source File

SOURCE=..\Include\CrysEditEx.h
# End Source File
# Begin Source File

SOURCE=.\CrystalParser.h
# End Source File
# Begin Source File

SOURCE=.\CrystalSpell.h
# End Source File
# Begin Source File

SOURCE=.\CrystalTextBlock.h
# End Source File
# Begin Source File

SOURCE=.\CrystalTextBufferEx.h
# End Source File
# Begin Source File

SOURCE=.\editcmd.h
# End Source File
# Begin Source File

SOURCE=.\editreg.h
# End Source File
# Begin Source File

SOURCE=..\..\myspell\hashmgr.hxx
# End Source File
# Begin Source File

SOURCE=..\..\myspell\htypes.hxx
# End Source File
# Begin Source File

SOURCE=..\..\myspell\myspell.hxx
# End Source File
# Begin Source File

SOURCE=.\SpellCheckBuffer.h
# End Source File
# Begin Source File

SOURCE=.\SpellCheckDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\..\myspell\suggestmgr.hxx
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\CrysEditEx.rc2
# End Source File
# Begin Source File

SOURCE=.\res\mg_cur.cur
# End Source File
# Begin Source File

SOURCE=.\res\mg_icons.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
