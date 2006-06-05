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
# Begin Group "Documents Deutsch"

# PROP Default_Filter ""
# Begin Source File

SOURCE="Documents\Deutsch\Deckblatt.tex"

!IF  "$(CFG)" == "Templates - Win32 Release"

# Begin Custom Build
OutDir=.\..\output\Release
InputPath="Documents\Deutsch\Deckblatt.tex"

"$(OutDir)\Templates\Documents\Deutsch\Deckblatt.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Documents\Deutsch\Deckblatt.tex" "$(OutDir)\Templates\Documents\Deutsch\Deckblatt.tex"

# End Custom Build

!ELSEIF  "$(CFG)" == "Templates - Win32 Debug"

# Begin Custom Build
OutDir=.\..\output\Debug
InputPath="Documents\Deutsch\Deckblatt.tex"

"$(OutDir)\Templates\Documents\Deutsch\Deckblatt.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Documents\Deutsch\Deckblatt.tex" "$(OutDir)\Templates\Documents\Deutsch\Deckblatt.tex"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="Documents\Deutsch\OptionenAbkürzungen.tex"

!IF  "$(CFG)" == "Templates - Win32 Release"

# Begin Custom Build
OutDir=.\..\output\Release
InputPath="Documents\Deutsch\OptionenAbkürzungen.tex"

"$(OutDir)\Templates\Documents\Deutsch\OptionenAbkürzungen.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Documents\Deutsch\OptionenAbkürzungen.tex" "$(OutDir)\Templates\Documents\Deutsch\OptionenAbkürzungen.tex"

# End Custom Build

!ELSEIF  "$(CFG)" == "Templates - Win32 Debug"

# Begin Custom Build
OutDir=.\..\output\Debug
InputPath="Documents\Deutsch\OptionenAbkürzungen.tex"

"$(OutDir)\Templates\Documents\Deutsch\OptionenAbkürzungen.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Documents\Deutsch\OptionenAbkürzungen.tex" "$(OutDir)\Templates\Documents\Deutsch\OptionenAbkürzungen.tex"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="Documents\Deutsch\OptionenLayout.tex"

!IF  "$(CFG)" == "Templates - Win32 Release"

# Begin Custom Build
OutDir=.\..\output\Release
InputPath="Documents\Deutsch\OptionenLayout.tex"

"$(OutDir)\Templates\Documents\Deutsch\OptionenLayout.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Documents\Deutsch\OptionenLayout.tex" "$(OutDir)\Templates\Documents\Deutsch\OptionenLayout.tex"

# End Custom Build

!ELSEIF  "$(CFG)" == "Templates - Win32 Debug"

# Begin Custom Build
OutDir=.\..\output\Debug
InputPath="Documents\Deutsch\OptionenLayout.tex"

"$(OutDir)\Templates\Documents\Deutsch\OptionenLayout.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Documents\Deutsch\OptionenLayout.tex" "$(OutDir)\Templates\Documents\Deutsch\OptionenLayout.tex"

# End Custom Build

!ENDIF 

# End Source File

# End Group
# Begin Group "Documents Deutsch erweitert"

# PROP Default_Filter ""
# Begin Source File

SOURCE="Documents\Deutsch erweitert\OptionenErweitert.tex"

!IF  "$(CFG)" == "Templates - Win32 Release"

# Begin Custom Build
OutDir=.\..\output\Release
InputPath="Documents\Deutsch erweitert\OptionenErweitert.tex"

"$(OutDir)\Templates\Documents\Deutsch erweitert\OptionenErweitert.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Documents\Deutsch erweitert\OptionenErweitert.tex" "$(OutDir)\Templates\Documents\Deutsch erweitert\OptionenErweitert.tex"

# End Custom Build

!ELSEIF  "$(CFG)" == "Templates - Win32 Debug"

# Begin Custom Build
OutDir=.\..\output\Debug
InputPath="Documents\Deutsch erweitert\OptionenErweitert.tex"

"$(OutDir)\Templates\Documents\Deutsch erweitert\OptionenErweitert.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Documents\Deutsch erweitert\OptionenErweitert.tex" "$(OutDir)\Templates\Documents\Deutsch erweitert\OptionenErweitert.tex"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="Documents\Deutsch erweitert\OptionenMath.tex"

!IF  "$(CFG)" == "Templates - Win32 Release"

# Begin Custom Build
OutDir=.\..\output\Release
InputPath="Documents\Deutsch erweitert\OptionenMath.tex"

"$(OutDir)\Templates\Documents\Deutsch erweitert\OptionenMath.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Documents\Deutsch erweitert\OptionenMath.tex" "$(OutDir)\Templates\Documents\Deutsch erweitert\OptionenMath.tex"

# End Custom Build

!ELSEIF  "$(CFG)" == "Templates - Win32 Debug"

# Begin Custom Build
OutDir=.\..\output\Debug
InputPath="Documents\Deutsch erweitert\OptionenMath.tex"

"$(OutDir)\Templates\Documents\Deutsch erweitert\OptionenMath.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Documents\Deutsch erweitert\OptionenMath.tex" "$(OutDir)\Templates\Documents\Deutsch erweitert\OptionenMath.tex"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="Documents\Deutsch erweitert\PDFInfos.tex"

!IF  "$(CFG)" == "Templates - Win32 Release"

# Begin Custom Build
OutDir=.\..\output\Release
InputPath="Documents\Deutsch erweitert\PDFInfos.tex"

"$(OutDir)\Templates\Documents\Deutsch erweitert\PDFInfos.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Documents\Deutsch erweitert\PDFInfos.tex" "$(OutDir)\Templates\Documents\Deutsch erweitert\PDFInfos.tex"

# End Custom Build

!ELSEIF  "$(CFG)" == "Templates - Win32 Debug"

# Begin Custom Build
OutDir=.\..\output\Debug
InputPath="Documents\Deutsch erweitert\PDFInfos.tex"

"$(OutDir)\Templates\Documents\Deutsch erweitert\PDFInfos.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Documents\Deutsch erweitert\PDFInfos.tex" "$(OutDir)\Templates\Documents\Deutsch erweitert\PDFInfos.tex"

# End Custom Build

!ENDIF 

# End Source File

# End Group
# Begin Group "Documents English"

# PROP Default_Filter ""
# Begin Source File

SOURCE="Documents\English\OptionsAbbreviations.tex"

!IF  "$(CFG)" == "Templates - Win32 Release"

# Begin Custom Build
OutDir=.\..\output\Release
InputPath="Documents\English\OptionsAbbreviations.tex"

"$(OutDir)\Templates\Documents\English\OptionsAbbreviations.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Documents\English\OptionsAbbreviations.tex" "$(OutDir)\Templates\Documents\English\OptionsAbbreviations.tex"

# End Custom Build

!ELSEIF  "$(CFG)" == "Templates - Win32 Debug"

# Begin Custom Build
OutDir=.\..\output\Debug
InputPath="Documents\English\OptionsAbbreviations.tex"

"$(OutDir)\Templates\Documents\English\OptionsAbbreviations.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Documents\English\OptionsAbbreviations.tex" "$(OutDir)\Templates\Documents\English\OptionsAbbreviations.tex"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="Documents\English\OptionsLayout.tex"

!IF  "$(CFG)" == "Templates - Win32 Release"

# Begin Custom Build
OutDir=.\..\output\Release
InputPath="Documents\English\OptionsLayout.tex"

"$(OutDir)\Templates\Documents\English\OptionsLayout.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Documents\English\OptionsLayout.tex" "$(OutDir)\Templates\Documents\English\OptionsLayout.tex"

# End Custom Build

!ELSEIF  "$(CFG)" == "Templates - Win32 Debug"

# Begin Custom Build
OutDir=.\..\output\Debug
InputPath="Documents\English\OptionsLayout.tex"

"$(OutDir)\Templates\Documents\English\OptionsLayout.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Documents\English\OptionsLayout.tex" "$(OutDir)\Templates\Documents\English\OptionsLayout.tex"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="Documents\English\Titlepage.tex"

!IF  "$(CFG)" == "Templates - Win32 Release"

# Begin Custom Build
OutDir=.\..\output\Release
InputPath="Documents\English\Titlepage.tex"

"$(OutDir)\Templates\Documents\English\Titlepage.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Documents\English\Titlepage.tex" "$(OutDir)\Templates\Documents\English\Titlepage.tex"

# End Custom Build

!ELSEIF  "$(CFG)" == "Templates - Win32 Debug"

# Begin Custom Build
OutDir=.\..\output\Debug
InputPath="Documents\English\Titlepage.tex"

"$(OutDir)\Templates\Documents\English\Titlepage.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Documents\English\Titlepage.tex" "$(OutDir)\Templates\Documents\English\Titlepage.tex"

# End Custom Build

!ENDIF 

# End Source File

# End Group
# Begin Group "Documents English advanced"

# PROP Default_Filter ""
# Begin Source File

SOURCE="Documents\English advanced\OptionsAdvanced.tex"

!IF  "$(CFG)" == "Templates - Win32 Release"

# Begin Custom Build
OutDir=.\..\output\Release
InputPath="Documents\English advanced\OptionsAdvanced.tex"

"$(OutDir)\Templates\Documents\English advanced\OptionsAdvanced.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Documents\English advanced\OptionsAdvanced.tex" "$(OutDir)\Templates\Documents\English advanced\OptionsAdvanced.tex"

# End Custom Build

!ELSEIF  "$(CFG)" == "Templates - Win32 Debug"

# Begin Custom Build
OutDir=.\..\output\Debug
InputPath="Documents\English advanced\OptionsAdvanced.tex"

"$(OutDir)\Templates\Documents\English advanced\OptionsAdvanced.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Documents\English advanced\OptionsAdvanced.tex" "$(OutDir)\Templates\Documents\English advanced\OptionsAdvanced.tex"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="Documents\English advanced\OptionsMath.tex"

!IF  "$(CFG)" == "Templates - Win32 Release"

# Begin Custom Build
OutDir=.\..\output\Release
InputPath="Documents\English advanced\OptionsMath.tex"

"$(OutDir)\Templates\Documents\English advanced\OptionsMath.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Documents\English advanced\OptionsMath.tex" "$(OutDir)\Templates\Documents\English advanced\OptionsMath.tex"

# End Custom Build

!ELSEIF  "$(CFG)" == "Templates - Win32 Debug"

# Begin Custom Build
OutDir=.\..\output\Debug
InputPath="Documents\English advanced\OptionsMath.tex"

"$(OutDir)\Templates\Documents\English advanced\OptionsMath.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Documents\English advanced\OptionsMath.tex" "$(OutDir)\Templates\Documents\English advanced\OptionsMath.tex"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="Documents\English advanced\PDFInfos.tex"

!IF  "$(CFG)" == "Templates - Win32 Release"

# Begin Custom Build
OutDir=.\..\output\Release
InputPath="Documents\English advanced\PDFInfos.tex"

"$(OutDir)\Templates\Documents\English advanced\PDFInfos.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Documents\English advanced\PDFInfos.tex" "$(OutDir)\Templates\Documents\English advanced\PDFInfos.tex"

# End Custom Build

!ELSEIF  "$(CFG)" == "Templates - Win32 Debug"

# Begin Custom Build
OutDir=.\..\output\Debug
InputPath="Documents\English advanced\PDFInfos.tex"

"$(OutDir)\Templates\Documents\English advanced\PDFInfos.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Documents\English advanced\PDFInfos.tex" "$(OutDir)\Templates\Documents\English advanced\PDFInfos.tex"

# End Custom Build

!ENDIF 

# End Source File

# End Group
# Begin Group "Projects Deutsch"

# PROP Default_Filter ""
# Begin Source File

SOURCE="Projects\Deutsch\Brief.tex"

!IF  "$(CFG)" == "Templates - Win32 Release"

# Begin Custom Build
OutDir=.\..\output\Release
InputPath="Projects\Deutsch\Brief.tex"

"$(OutDir)\Templates\Projects\Deutsch\Brief.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Projects\Deutsch\Brief.tex" "$(OutDir)\Templates\Projects\Deutsch\Brief.tex"

# End Custom Build

!ELSEIF  "$(CFG)" == "Templates - Win32 Debug"

# Begin Custom Build
OutDir=.\..\output\Debug
InputPath="Projects\Deutsch\Brief.tex"

"$(OutDir)\Templates\Projects\Deutsch\Brief.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Projects\Deutsch\Brief.tex" "$(OutDir)\Templates\Projects\Deutsch\Brief.tex"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="Projects\Deutsch\KOMA-Brief.tex"

!IF  "$(CFG)" == "Templates - Win32 Release"

# Begin Custom Build
OutDir=.\..\output\Release
InputPath="Projects\Deutsch\KOMA-Brief.tex"

"$(OutDir)\Templates\Projects\Deutsch\KOMA-Brief.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Projects\Deutsch\KOMA-Brief.tex" "$(OutDir)\Templates\Projects\Deutsch\KOMA-Brief.tex"

# End Custom Build

!ELSEIF  "$(CFG)" == "Templates - Win32 Debug"

# Begin Custom Build
OutDir=.\..\output\Debug
InputPath="Projects\Deutsch\KOMA-Brief.tex"

"$(OutDir)\Templates\Projects\Deutsch\KOMA-Brief.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Projects\Deutsch\KOMA-Brief.tex" "$(OutDir)\Templates\Projects\Deutsch\KOMA-Brief.tex"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="Projects\Deutsch\KOMAArtikel.tex"

!IF  "$(CFG)" == "Templates - Win32 Release"

# Begin Custom Build
OutDir=.\..\output\Release
InputPath="Projects\Deutsch\KOMAArtikel.tex"

"$(OutDir)\Templates\Projects\Deutsch\KOMAArtikel.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Projects\Deutsch\KOMAArtikel.tex" "$(OutDir)\Templates\Projects\Deutsch\KOMAArtikel.tex"

# End Custom Build

!ELSEIF  "$(CFG)" == "Templates - Win32 Debug"

# Begin Custom Build
OutDir=.\..\output\Debug
InputPath="Projects\Deutsch\KOMAArtikel.tex"

"$(OutDir)\Templates\Projects\Deutsch\KOMAArtikel.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Projects\Deutsch\KOMAArtikel.tex" "$(OutDir)\Templates\Projects\Deutsch\KOMAArtikel.tex"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="Projects\Deutsch\lebenslauf.tex"

!IF  "$(CFG)" == "Templates - Win32 Release"

# Begin Custom Build
OutDir=.\..\output\Release
InputPath="Projects\Deutsch\lebenslauf.tex"

"$(OutDir)\Templates\Projects\Deutsch\lebenslauf.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Projects\Deutsch\lebenslauf.tex" "$(OutDir)\Templates\Projects\Deutsch\lebenslauf.tex"

# End Custom Build

!ELSEIF  "$(CFG)" == "Templates - Win32 Debug"

# Begin Custom Build
OutDir=.\..\output\Debug
InputPath="Projects\Deutsch\lebenslauf.tex"

"$(OutDir)\Templates\Projects\Deutsch\lebenslauf.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Projects\Deutsch\lebenslauf.tex" "$(OutDir)\Templates\Projects\Deutsch\lebenslauf.tex"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="Projects\Deutsch\Report.tex"

!IF  "$(CFG)" == "Templates - Win32 Release"

# Begin Custom Build
OutDir=.\..\output\Release
InputPath="Projects\Deutsch\Report.tex"

"$(OutDir)\Templates\Projects\Deutsch\Report.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Projects\Deutsch\Report.tex" "$(OutDir)\Templates\Projects\Deutsch\Report.tex"

# End Custom Build

!ELSEIF  "$(CFG)" == "Templates - Win32 Debug"

# Begin Custom Build
OutDir=.\..\output\Debug
InputPath="Projects\Deutsch\Report.tex"

"$(OutDir)\Templates\Projects\Deutsch\Report.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Projects\Deutsch\Report.tex" "$(OutDir)\Templates\Projects\Deutsch\Report.tex"

# End Custom Build

!ENDIF 

# End Source File

# End Group
# Begin Group "Projects Deutsch erweitert"

# PROP Default_Filter ""
# Begin Source File

SOURCE="Projects\Deutsch erweitert\KOMABuch.tex"

!IF  "$(CFG)" == "Templates - Win32 Release"

# Begin Custom Build
OutDir=.\..\output\Release
InputPath="Projects\Deutsch erweitert\KOMABuch.tex"

"$(OutDir)\Templates\Projects\Deutsch erweitert\KOMABuch.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Projects\Deutsch erweitert\KOMABuch.tex" "$(OutDir)\Templates\Projects\Deutsch erweitert\KOMABuch.tex"

# End Custom Build

!ELSEIF  "$(CFG)" == "Templates - Win32 Debug"

# Begin Custom Build
OutDir=.\..\output\Debug
InputPath="Projects\Deutsch erweitert\KOMABuch.tex"

"$(OutDir)\Templates\Projects\Deutsch erweitert\KOMABuch.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Projects\Deutsch erweitert\KOMABuch.tex" "$(OutDir)\Templates\Projects\Deutsch erweitert\KOMABuch.tex"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="Projects\Deutsch erweitert\prospereinfach.tex"

!IF  "$(CFG)" == "Templates - Win32 Release"

# Begin Custom Build
OutDir=.\..\output\Release
InputPath="Projects\Deutsch erweitert\prospereinfach.tex"

"$(OutDir)\Templates\Projects\Deutsch erweitert\prospereinfach.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Projects\Deutsch erweitert\prospereinfach.tex" "$(OutDir)\Templates\Projects\Deutsch erweitert\prospereinfach.tex"

# End Custom Build

!ELSEIF  "$(CFG)" == "Templates - Win32 Debug"

# Begin Custom Build
OutDir=.\..\output\Debug
InputPath="Projects\Deutsch erweitert\prospereinfach.tex"

"$(OutDir)\Templates\Projects\Deutsch erweitert\prospereinfach.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Projects\Deutsch erweitert\prospereinfach.tex" "$(OutDir)\Templates\Projects\Deutsch erweitert\prospereinfach.tex"

# End Custom Build

!ENDIF 

# End Source File

# End Group
# Begin Group "Projects English"

# PROP Default_Filter ""
# Begin Source File

SOURCE="Projects\English\Letter.tex"

!IF  "$(CFG)" == "Templates - Win32 Release"

# Begin Custom Build
OutDir=.\..\output\Release
InputPath="Projects\English\Letter.tex"

"$(OutDir)\Templates\Projects\English\Letter.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Projects\English\Letter.tex" "$(OutDir)\Templates\Projects\English\Letter.tex"

# End Custom Build

!ELSEIF  "$(CFG)" == "Templates - Win32 Debug"

# Begin Custom Build
OutDir=.\..\output\Debug
InputPath="Projects\English\Letter.tex"

"$(OutDir)\Templates\Projects\English\Letter.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Projects\English\Letter.tex" "$(OutDir)\Templates\Projects\English\Letter.tex"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="Projects\English\Report.tex"

!IF  "$(CFG)" == "Templates - Win32 Release"

# Begin Custom Build
OutDir=.\..\output\Release
InputPath="Projects\English\Report.tex"

"$(OutDir)\Templates\Projects\English\Report.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Projects\English\Report.tex" "$(OutDir)\Templates\Projects\English\Report.tex"

# End Custom Build

!ELSEIF  "$(CFG)" == "Templates - Win32 Debug"

# Begin Custom Build
OutDir=.\..\output\Debug
InputPath="Projects\English\Report.tex"

"$(OutDir)\Templates\Projects\English\Report.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Projects\English\Report.tex" "$(OutDir)\Templates\Projects\English\Report.tex"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="Projects\English\Slides.tex"

!IF  "$(CFG)" == "Templates - Win32 Release"

# Begin Custom Build
OutDir=.\..\output\Release
InputPath="Projects\English\Slides.tex"

"$(OutDir)\Templates\Projects\English\Slides.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Projects\English\Slides.tex" "$(OutDir)\Templates\Projects\English\Slides.tex"

# End Custom Build

!ELSEIF  "$(CFG)" == "Templates - Win32 Debug"

# Begin Custom Build
OutDir=.\..\output\Debug
InputPath="Projects\English\Slides.tex"

"$(OutDir)\Templates\Projects\English\Slides.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Projects\English\Slides.tex" "$(OutDir)\Templates\Projects\English\Slides.tex"

# End Custom Build

!ENDIF 

# End Source File

# End Group
# Begin Group "Projects English advanced"

# PROP Default_Filter ""
# Begin Source File

SOURCE="Projects\English advanced\Basic Article.tex"

!IF  "$(CFG)" == "Templates - Win32 Release"

# Begin Custom Build
OutDir=.\..\output\Release
InputPath="Projects\English advanced\Basic Article.tex"

"$(OutDir)\Templates\Projects\English advanced\Basic Article.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Projects\English advanced\Basic Article.tex" "$(OutDir)\Templates\Projects\English advanced\Basic Article.tex"

# End Custom Build

!ELSEIF  "$(CFG)" == "Templates - Win32 Debug"

# Begin Custom Build
OutDir=.\..\output\Debug
InputPath="Projects\English advanced\Basic Article.tex"

"$(OutDir)\Templates\Projects\English advanced\Basic Article.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Projects\English advanced\Basic Article.tex" "$(OutDir)\Templates\Projects\English advanced\Basic Article.tex"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="Projects\English advanced\Book.tex"

!IF  "$(CFG)" == "Templates - Win32 Release"

# Begin Custom Build
OutDir=.\..\output\Release
InputPath="Projects\English advanced\Book.tex"

"$(OutDir)\Templates\Projects\English advanced\Book.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Projects\English advanced\Book.tex" "$(OutDir)\Templates\Projects\English advanced\Book.tex"

# End Custom Build

!ELSEIF  "$(CFG)" == "Templates - Win32 Debug"

# Begin Custom Build
OutDir=.\..\output\Debug
InputPath="Projects\English advanced\Book.tex"

"$(OutDir)\Templates\Projects\English advanced\Book.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Projects\English advanced\Book.tex" "$(OutDir)\Templates\Projects\English advanced\Book.tex"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="Projects\English advanced\Foils.tex"

!IF  "$(CFG)" == "Templates - Win32 Release"

# Begin Custom Build
OutDir=.\..\output\Release
InputPath="Projects\English advanced\Foils.tex"

"$(OutDir)\Templates\Projects\English advanced\Foils.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Projects\English advanced\Foils.tex" "$(OutDir)\Templates\Projects\English advanced\Foils.tex"

# End Custom Build

!ELSEIF  "$(CFG)" == "Templates - Win32 Debug"

# Begin Custom Build
OutDir=.\..\output\Debug
InputPath="Projects\English advanced\Foils.tex"

"$(OutDir)\Templates\Projects\English advanced\Foils.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Projects\English advanced\Foils.tex" "$(OutDir)\Templates\Projects\English advanced\Foils.tex"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="Projects\English advanced\Mathematical Article.tex"

!IF  "$(CFG)" == "Templates - Win32 Release"

# Begin Custom Build
OutDir=.\..\output\Release
InputPath="Projects\English advanced\Mathematical Article.tex"

"$(OutDir)\Templates\Projects\English advanced\Mathematical Article.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Projects\English advanced\Mathematical Article.tex" "$(OutDir)\Templates\Projects\English advanced\Mathematical Article.tex"

# End Custom Build

!ELSEIF  "$(CFG)" == "Templates - Win32 Debug"

# Begin Custom Build
OutDir=.\..\output\Debug
InputPath="Projects\English advanced\Mathematical Article.tex"

"$(OutDir)\Templates\Projects\English advanced\Mathematical Article.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Projects\English advanced\Mathematical Article.tex" "$(OutDir)\Templates\Projects\English advanced\Mathematical Article.tex"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="Projects\English advanced\Mathematical Book.tex"

!IF  "$(CFG)" == "Templates - Win32 Release"

# Begin Custom Build
OutDir=.\..\output\Release
InputPath="Projects\English advanced\Mathematical Book.tex"

"$(OutDir)\Templates\Projects\English advanced\Mathematical Book.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Projects\English advanced\Mathematical Book.tex" "$(OutDir)\Templates\Projects\English advanced\Mathematical Book.tex"

# End Custom Build

!ELSEIF  "$(CFG)" == "Templates - Win32 Debug"

# Begin Custom Build
OutDir=.\..\output\Debug
InputPath="Projects\English advanced\Mathematical Book.tex"

"$(OutDir)\Templates\Projects\English advanced\Mathematical Book.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Projects\English advanced\Mathematical Book.tex" "$(OutDir)\Templates\Projects\English advanced\Mathematical Book.tex"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="Projects\English advanced\Simple Letter.tex"

!IF  "$(CFG)" == "Templates - Win32 Release"

# Begin Custom Build
OutDir=.\..\output\Release
InputPath="Projects\English advanced\Simple Letter.tex"

"$(OutDir)\Templates\Projects\English advanced\Simple Letter.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Projects\English advanced\Simple Letter.tex" "$(OutDir)\Templates\Projects\English advanced\Simple Letter.tex"

# End Custom Build

!ELSEIF  "$(CFG)" == "Templates - Win32 Debug"

# Begin Custom Build
OutDir=.\..\output\Debug
InputPath="Projects\English advanced\Simple Letter.tex"

"$(OutDir)\Templates\Projects\English advanced\Simple Letter.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Projects\English advanced\Simple Letter.tex" "$(OutDir)\Templates\Projects\English advanced\Simple Letter.tex"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="Projects\English advanced\Simple Report.tex"

!IF  "$(CFG)" == "Templates - Win32 Release"

# Begin Custom Build
OutDir=.\..\output\Release
InputPath="Projects\English advanced\Simple Report.tex"

"$(OutDir)\Templates\Projects\English advanced\Simple Report.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Projects\English advanced\Simple Report.tex" "$(OutDir)\Templates\Projects\English advanced\Simple Report.tex"

# End Custom Build

!ELSEIF  "$(CFG)" == "Templates - Win32 Debug"

# Begin Custom Build
OutDir=.\..\output\Debug
InputPath="Projects\English advanced\Simple Report.tex"

"$(OutDir)\Templates\Projects\English advanced\Simple Report.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Projects\English advanced\Simple Report.tex" "$(OutDir)\Templates\Projects\English advanced\Simple Report.tex"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="Projects\English advanced\Technical Report.tex"

!IF  "$(CFG)" == "Templates - Win32 Release"

# Begin Custom Build
OutDir=.\..\output\Release
InputPath="Projects\English advanced\Technical Report.tex"

"$(OutDir)\Templates\Projects\English advanced\Technical Report.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Projects\English advanced\Technical Report.tex" "$(OutDir)\Templates\Projects\English advanced\Technical Report.tex"

# End Custom Build

!ELSEIF  "$(CFG)" == "Templates - Win32 Debug"

# Begin Custom Build
OutDir=.\..\output\Debug
InputPath="Projects\English advanced\Technical Report.tex"

"$(OutDir)\Templates\Projects\English advanced\Technical Report.tex" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "Projects\English advanced\Technical Report.tex" "$(OutDir)\Templates\Projects\English advanced\Technical Report.tex"

# End Custom Build

!ENDIF 

# End Source File

# End Group
# End Target
# End Project
