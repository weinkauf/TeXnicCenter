# Microsoft Developer Studio Project File - Name="TXC Documentation" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Generic Project" 0x010a

CFG=TXC Documentation - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TXC Documentation.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TXC Documentation.mak" CFG="TXC Documentation - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TXC Documentation - Win32 Release" (based on "Win32 (x86) Generic Project")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
MTL=midl.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "html"
# PROP Intermediate_Dir "html"
# PROP Target_Dir ""
# Begin Target

# Name "TXC Documentation - Win32 Release"
# Begin Source File

SOURCE=.\groups.dox
# End Source File
# Begin Source File

SOURCE=.\mainpage.dox
# End Source File
# Begin Source File

SOURCE=.\MakeClean.bat
# End Source File
# Begin Source File

SOURCE=.\MakeSourceCodeDoc.bat
# End Source File
# Begin Source File

SOURCE=.\txc.doxy
# Begin Custom Build - Running doxygen on $(InputPath)
InputDir=.
OutDir=.\html
InputPath=.\txc.doxy

"$(OutDir)\index.html" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(InputDir)\MakeSourceCodeDoc.bat

# End Custom Build
# End Source File
# End Target
# End Project
