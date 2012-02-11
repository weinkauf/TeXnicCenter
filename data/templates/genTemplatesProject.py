#~ $Id$
# -*- coding: latin-1 -*-

import string
from path import path

DSPHEAD = """\
# Microsoft Developer Studio Project File - Name="Templates" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Generic Project" 0x010a

CFG=Templates - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Templates.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Templates.mak" CFG="Templates - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Templates - Win32 Release" (based on "Win32 (x86) Generic Project")
!MESSAGE "Templates - Win32 Debug" (based on "Win32 (x86) Generic Project")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
MTL=midl.exe

!IF  "$(CFG)" == "Templates - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../output/Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "Templates - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Templates___Win32_Debug"
# PROP BASE Intermediate_Dir "Templates___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\output\Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "Templates - Win32 Release"
# Name "Templates - Win32 Debug"
"""

DSPFOOT = """\
# End Target
# End Project
"""

DSPGROUPSTART = """\
# Begin Group "%s"

# PROP Default_Filter ""
"""

DSPGROUPEND = """
# End Group
"""

TEXFILETEMPLATE = """\
# Begin Source File

SOURCE="%s"

!IF  "$(CFG)" == "Templates - Win32 Release"

# Begin Custom Build
OutDir=.\..\output\Release
InputPath="%s"

"$(OutDir)\Templates\%s" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "%s" "$(OutDir)\Templates\%s"

# End Custom Build

!ELSEIF  "$(CFG)" == "Templates - Win32 Debug"

# Begin Custom Build
OutDir=.\..\output\Debug
InputPath="%s"

"$(OutDir)\Templates\%s" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "%s" "$(OutDir)\Templates\%s"

# End Custom Build

!ENDIF 

# End Source File
"""

def WriteHead(outfile):
    outfile.write(DSPHEAD)


def WriteFoot(outfile):
    outfile.write(DSPFOOT)


def BeginGroup(outfile, name):
    outfile.write(DSPGROUPSTART % name)

def EndGroup(outfile, name):
    outfile.write(DSPGROUPEND)


def ProcessFile(outfile, texfile):
    #~ .\Documents\Deutsch\OptionenLayout.tex
    SourceFileName = texfile
    SourceFileName = SourceFileName.encode("latin-1")
    print SourceFileName
    outfile.write(TEXFILETEMPLATE % (SourceFileName, SourceFileName, SourceFileName, SourceFileName, SourceFileName, SourceFileName, SourceFileName, SourceFileName, SourceFileName))


def ProcessDir(outfile, path, OptionalPrefix = None):
    if (OptionalPrefix == None):
        groupname = path.name
    else:
        groupname = OptionalPrefix + " " + path.name
    print "%s: %s" % (groupname, path)
    
    BeginGroup(outfile, groupname)
    
    for d in path.dirs():
        ProcessDir(outfile, d)
    
    for texfile in path.files("*.tex"):
        ProcessFile(outfile, texfile)
    
    EndGroup(outfile, groupname)


if __name__ == "__main__":

    #~ Paths
    templatepaths = [path("Documents"), path("Projects")]

    outfile = file("Templates.dsp", "w")

    WriteHead(outfile)
    
    for p in templatepaths:
        for d in p.dirs():
            ProcessDir(outfile, d, p.name)
            
    WriteFoot(outfile)
