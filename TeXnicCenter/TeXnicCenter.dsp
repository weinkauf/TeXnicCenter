# Microsoft Developer Studio Project File - Name="TeXnicCenter" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=TeXnicCenter - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "TeXnicCenter.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "TeXnicCenter.mak" CFG="TeXnicCenter - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "TeXnicCenter - Win32 Release" (basierend auf  "Win32 (x86) Application")
!MESSAGE "TeXnicCenter - Win32 Debug" (basierend auf  "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "TeXnicCenter"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TeXnicCenter - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../output/Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /Zi /O2 /I "../CrysEditEx/Include" /I "../regexpp/include" /I "../BCGControlBar/Include" /I "../MFCExt/Include" /I "../TeXnicCenterObjects/Include" /I "../MsXmlCpp/Include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "CE_FROM_DLL" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL" /d "AFX_TARG_NEU"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 CrysEditEx.lib MFCExt.lib TeXnicCenterObjects.lib Shlwapi.lib mpr.lib htmlhelp.lib msxml2.lib /nologo /subsystem:windows /map /debug /debugtype:both /machine:I386 /out:"../output/Release/TEXCNTR.EXE" /libpath:"../CrysEditEx/Lib" /libpath:"../MFCExt/Lib" /libpath:"../BCGControlBar/Lib" /libpath:"../regexpp/Lib" /libpath:"../TeXnicCenterObjects/Lib"

!ELSEIF  "$(CFG)" == "TeXnicCenter - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../output/Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "../CrysEditEx/Include" /I "../regexpp/include" /I "../BCGControlBar/Include" /I "../MFCExt/Include" /I "../TeXnicCenterObjects/Include" /I "../MsXmlCpp/Include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "CE_FROM_DLL" /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL" /d "AFX_TARG_NEU"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 CrysEditExD.lib MFCExtD.lib TeXnicCenterObjectsD.lib Shlwapi.lib mpr.lib htmlhelp.lib msxml2.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../output/Debug/TEXCNTR.EXE" /pdbtype:sept /libpath:"../CrysEditEx/Lib" /libpath:"../MFCExt/Lib" /libpath:"../BCGControlBar/Lib" /libpath:"../regexpp/Lib" /libpath:"../TeXnicCenterObjects/Lib"
# SUBTRACT LINK32 /map

!ENDIF 

# Begin Target

# Name "TeXnicCenter - Win32 Release"
# Name "TeXnicCenter - Win32 Debug"
# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AboutBox.cpp
# End Source File
# Begin Source File

SOURCE=.\ActiveProfileDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGNotifyingEditListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGRegistryEx.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGToolbarCustomizeEx.cpp
# End Source File
# Begin Source File

SOURCE=.\BrowseButton.cpp
# End Source File
# Begin Source File

SOURCE=.\BuildView.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Configuration.cpp
# End Source File
# Begin Source File

SOURCE=.\CrystalEditViewEx.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomizePageEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\DdeCommand.cpp
# End Source File
# Begin Source File

SOURCE=.\DocumentNewDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\EditMenuButton.cpp
# End Source File
# Begin Source File

SOURCE=.\EditMenuButtonOpt.cpp
# End Source File
# Begin Source File

SOURCE=.\EnvironmentView.cpp
# End Source File
# Begin Source File

SOURCE=.\FileChangeWatcher.cpp
# End Source File
# Begin Source File

SOURCE=.\FileClean.cpp
# End Source File
# Begin Source File

SOURCE=.\FileCleanConfirmDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\FileGrep.cpp
# End Source File
# Begin Source File

SOURCE=.\FileView.cpp
# End Source File
# Begin Source File

SOURCE=.\FileViewEx.cpp
# End Source File
# Begin Source File

SOURCE=.\FindInFilesDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\global.cpp
# End Source File
# Begin Source File

SOURCE=.\GotoDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\GrepView.cpp
# End Source File
# Begin Source File

SOURCE=.\IDocumentWizard.cpp
# End Source File
# Begin Source File

SOURCE=.\InsertFloatObjectDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\InsertGraphicDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\InsertHeaderDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\InsertTabularDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\IProject.cpp
# End Source File
# Begin Source File

SOURCE=.\IProjectWizard.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemPropertyDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\KeyboardHook.cpp
# End Source File
# Begin Source File

SOURCE=.\LatexDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\LatexEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\LatexOutputFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\LatexParser.cpp
# End Source File
# Begin Source File

SOURCE=.\LatexProject.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MDIFrameManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuButton.cpp
# End Source File
# Begin Source File

SOURCE=.\MRUTrackSelComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\NavigatorView.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionPageFile.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionPageFileClean.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionPageGeneric.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionPageLanguage.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionPagePath.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputBar.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputBuilder.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputView.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputWizard.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputWizardDistributionPath.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputWizardFinish.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputWizardMikTex.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputWizardViewer.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputWizardWelcome.cpp
# End Source File
# Begin Source File

SOURCE=.\ParseOutputView.cpp
# End Source File
# Begin Source File

SOURCE=.\PersistPosEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\PlaceHolder.cpp
# End Source File
# Begin Source File

SOURCE=.\PlaceholderSetsOptDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PlaceholderSingleOptDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PostProcessor.cpp
# End Source File
# Begin Source File

SOURCE=.\Process.cpp
# End Source File
# Begin Source File

SOURCE=.\ProcessCommand.cpp
# End Source File
# Begin Source File

SOURCE=.\Profile.cpp
# End Source File
# Begin Source File

SOURCE=.\ProfileDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ProfileExchangeDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ProfileNameDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ProfilePage.cpp
# End Source File
# Begin Source File

SOURCE=.\ProfilePageLatex.cpp
# End Source File
# Begin Source File

SOURCE=.\ProfilePagePostprocessor.cpp
# End Source File
# Begin Source File

SOURCE=.\ProfilePageViewer.cpp
# End Source File
# Begin Source File

SOURCE=.\Project.cpp
# End Source File
# Begin Source File

SOURCE=.\projectnewdialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectPropertyDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectSupportingWinApp.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectTemplate.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectView.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ShellView.cpp
# End Source File
# Begin Source File

SOURCE=.\Splash.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StructureParser.cpp
# End Source File
# Begin Source File

SOURCE=.\StructureView.cpp
# End Source File
# Begin Source File

SOURCE=.\TemplateDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\TeXnicCenter.cpp
# End Source File
# Begin Source File

SOURCE=.\TeXnicCenter.rc
# End Source File
# Begin Source File

SOURCE=.\TextFileSaveDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\TextModules.cpp
# End Source File
# Begin Source File

SOURCE=.\TextModulesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TextSourceFile.cpp
# End Source File
# Begin Source File

SOURCE=.\TipDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UserTool.cpp
# End Source File
# Begin Source File

SOURCE=.\UserToolAdvDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\WorkspaceBar.cpp
# End Source File
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AboutBox.h
# End Source File
# Begin Source File

SOURCE=.\ActiveProfileDialog.h
# End Source File
# Begin Source File

SOURCE=.\BCGNotifyingEditListBox.h
# End Source File
# Begin Source File

SOURCE=.\BCGRegistryEx.h
# End Source File
# Begin Source File

SOURCE=.\BCGToolbarCustomizeEx.h
# End Source File
# Begin Source File

SOURCE=.\BrowseButton.h
# End Source File
# Begin Source File

SOURCE=.\BuildView.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\Configuration.h
# End Source File
# Begin Source File

SOURCE=.\CrystalEditViewEx.h
# End Source File
# Begin Source File

SOURCE=.\CustomizePageEditor.h
# End Source File
# Begin Source File

SOURCE=.\DdeCommand.h
# End Source File
# Begin Source File

SOURCE=.\DocumentNewDialog.h
# End Source File
# Begin Source File

SOURCE=.\EditMenuButton.h
# End Source File
# Begin Source File

SOURCE=.\EditMenuButtonOpt.h
# End Source File
# Begin Source File

SOURCE=.\EnvironmentView.h
# End Source File
# Begin Source File

SOURCE=.\FileChangeWatcher.h
# End Source File
# Begin Source File

SOURCE=.\FileClean.h
# End Source File
# Begin Source File

SOURCE=.\FileCleanConfirmDialog.h
# End Source File
# Begin Source File

SOURCE=.\FileGrep.h
# End Source File
# Begin Source File

SOURCE=.\FileView.h
# End Source File
# Begin Source File

SOURCE=.\FileViewEx.h
# End Source File
# Begin Source File

SOURCE=.\FindInFilesDialog.h
# End Source File
# Begin Source File

SOURCE=.\global.h
# End Source File
# Begin Source File

SOURCE=.\GotoDialog.h
# End Source File
# Begin Source File

SOURCE=.\GrepView.h
# End Source File
# Begin Source File

SOURCE=.\IDocumentWizard.h
# End Source File
# Begin Source File

SOURCE=.\InsertFloatObjectDialog.h
# End Source File
# Begin Source File

SOURCE=.\InsertGraphicDialog.h
# End Source File
# Begin Source File

SOURCE=.\InsertHeaderDialog.h
# End Source File
# Begin Source File

SOURCE=.\InsertTabularDialog.h
# End Source File
# Begin Source File

SOURCE=.\IProject.h
# End Source File
# Begin Source File

SOURCE=.\IProjectWizard.h
# End Source File
# Begin Source File

SOURCE=.\ItemPropertyDialog.h
# End Source File
# Begin Source File

SOURCE=.\KeyboardHook.h
# End Source File
# Begin Source File

SOURCE=.\LatexDoc.h
# End Source File
# Begin Source File

SOURCE=.\LatexEdit.h
# End Source File
# Begin Source File

SOURCE=.\LatexOutputFilter.h
# End Source File
# Begin Source File

SOURCE=.\LatexParser.h
# End Source File
# Begin Source File

SOURCE=.\LatexProject.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MDIFrameManager.h
# End Source File
# Begin Source File

SOURCE=.\MenuButton.h
# End Source File
# Begin Source File

SOURCE=.\MRUTrackSelComboBox.h
# End Source File
# Begin Source File

SOURCE=.\NavigatorView.h
# End Source File
# Begin Source File

SOURCE=.\OptionDialog.h
# End Source File
# Begin Source File

SOURCE=.\OptionPageFile.h
# End Source File
# Begin Source File

SOURCE=.\OptionPageFileClean.h
# End Source File
# Begin Source File

SOURCE=.\OptionPageGeneric.h
# End Source File
# Begin Source File

SOURCE=.\OptionPageLanguage.h
# End Source File
# Begin Source File

SOURCE=.\OptionPagePath.h
# End Source File
# Begin Source File

SOURCE=.\OutputBar.h
# End Source File
# Begin Source File

SOURCE=.\OutputBuilder.h
# End Source File
# Begin Source File

SOURCE=.\OutputDoc.h
# End Source File
# Begin Source File

SOURCE=.\OutputFilter.h
# End Source File
# Begin Source File

SOURCE=.\OutputInfo.h
# End Source File
# Begin Source File

SOURCE=.\OutputView.h
# End Source File
# Begin Source File

SOURCE=.\OutputWizard.h
# End Source File
# Begin Source File

SOURCE=.\OutputWizardDistributionPath.h
# End Source File
# Begin Source File

SOURCE=.\OutputWizardFinish.h
# End Source File
# Begin Source File

SOURCE=.\OutputWizardMikTex.h
# End Source File
# Begin Source File

SOURCE=.\OutputWizardViewer.h
# End Source File
# Begin Source File

SOURCE=.\OutputWizardWelcome.h
# End Source File
# Begin Source File

SOURCE=.\ParseOutputView.h
# End Source File
# Begin Source File

SOURCE=.\PersistPosEdit.h
# End Source File
# Begin Source File

SOURCE=.\PlaceHolder.h
# End Source File
# Begin Source File

SOURCE=.\PlaceholderSetsOptDlg.h
# End Source File
# Begin Source File

SOURCE=.\PlaceholderSingleOptDlg.h
# End Source File
# Begin Source File

SOURCE=.\PostProcessor.h
# End Source File
# Begin Source File

SOURCE=.\Process.h
# End Source File
# Begin Source File

SOURCE=.\ProcessCommand.h
# End Source File
# Begin Source File

SOURCE=.\Profile.h
# End Source File
# Begin Source File

SOURCE=.\ProfileDialog.h
# End Source File
# Begin Source File

SOURCE=.\ProfileExchangeDialog.h
# End Source File
# Begin Source File

SOURCE=.\ProfileNameDialog.h
# End Source File
# Begin Source File

SOURCE=.\ProfilePage.h
# End Source File
# Begin Source File

SOURCE=.\ProfilePageLatex.h
# End Source File
# Begin Source File

SOURCE=.\ProfilePagePostprocessor.h
# End Source File
# Begin Source File

SOURCE=.\ProfilePageViewer.h
# End Source File
# Begin Source File

SOURCE=.\Project.h
# End Source File
# Begin Source File

SOURCE=.\projectnewdialog.h
# End Source File
# Begin Source File

SOURCE=.\ProjectPropertyDialog.h
# End Source File
# Begin Source File

SOURCE=.\ProjectSupportingWinApp.h
# End Source File
# Begin Source File

SOURCE=.\ProjectTemplate.h
# End Source File
# Begin Source File

SOURCE=.\ProjectView.h
# End Source File
# Begin Source File

SOURCE=.\PropertyDialog.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# PROP Ignore_Default_Tool 1
# End Source File
# Begin Source File

SOURCE=.\ShellView.h
# End Source File
# Begin Source File

SOURCE=.\Splash.h
# End Source File
# Begin Source File

SOURCE=.\Stack.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StructureParser.h
# End Source File
# Begin Source File

SOURCE=.\StructureView.h
# End Source File
# Begin Source File

SOURCE=.\TemplateDialog.h
# End Source File
# Begin Source File

SOURCE=.\TeXnicCenter.h
# End Source File
# Begin Source File

SOURCE=.\TextFileSaveDialog.h
# End Source File
# Begin Source File

SOURCE=.\TextModules.h
# End Source File
# Begin Source File

SOURCE=.\TextModulesDlg.h
# End Source File
# Begin Source File

SOURCE=.\TextSource.h
# End Source File
# Begin Source File

SOURCE=.\TextSourceFile.h
# End Source File
# Begin Source File

SOURCE=.\TipDlg.h
# End Source File
# Begin Source File

SOURCE=.\TreeCtrlEx.h
# End Source File
# Begin Source File

SOURCE=.\UserTool.h
# End Source File
# Begin Source File

SOURCE=.\UserToolAdvDlg.h
# End Source File
# Begin Source File

SOURCE=.\WorkspaceBar.h
# End Source File
# End Group
# Begin Group "Ressourcendateien"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\buildvie.bmp
# End Source File
# Begin Source File

SOURCE=.\res\document.bmp
# End Source File
# Begin Source File

SOURCE=.\res\fileview.bmp
# End Source File
# Begin Source File

SOURCE=.\res\format.bmp
# End Source File
# Begin Source File

SOURCE=.\res\format_r.bmp
# End Source File
# Begin Source File

SOURCE=.\res\fullscre.bmp
# End Source File
# Begin Source File

SOURCE=.\res\idr_late.ico
# End Source File
# Begin Source File

SOURCE=.\res\images.bmp
# End Source File
# Begin Source File

SOURCE=.\res\insert.bmp
# End Source File
# Begin Source File

SOURCE=.\res\item_typ.bmp
# End Source File
# Begin Source File

SOURCE=.\res\latex.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LatexDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\LatexDoc_yellow.ico
# End Source File
# Begin Source File

SOURCE=.\res\litebulb.bmp
# End Source File
# Begin Source File

SOURCE=.\res\logo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\math.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mathbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mathbar10.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mathbar11.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mathbar12.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mathbar13.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mathbar14.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mathbar15.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mathbar16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mathbar17.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mathbar2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mathbar3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mathbar4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mathbar5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mathbar6.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mathbar7.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mathbar8.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mathbar9.bmp
# End Source File
# Begin Source File

SOURCE=.\res\navigati.bmp
# End Source File
# Begin Source File

SOURCE=.\res\parse_vi.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sdragcopy.cur
# End Source File
# Begin Source File

SOURCE=.\res\separator.bmp
# End Source File
# Begin Source File

SOURCE=.\res\singledrag.cur
# End Source File
# Begin Source File

SOURCE=.\res\sort.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Splsh16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TeXnicCenter.ico
# End Source File
# Begin Source File

SOURCE=.\res\TeXnicCenter.rc2
# End Source File
# Begin Source File

SOURCE=.\res\TeXnicCenter_yellow.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Wizard.ico
# End Source File
# Begin Source File

SOURCE=.\res\Wizard_yellow.ico
# End Source File
# End Group
# Begin Group "Required Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GPL_english.htm
# End Source File
# Begin Source File

SOURCE=.\GPL_german.htm
# End Source File
# Begin Source File

SOURCE=.\Tips_english.txt
# End Source File
# Begin Source File

SOURCE=.\Tips_german.txt
# End Source File
# Begin Source File

SOURCE=.\UserImages.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=..\ChangeLog.txt
# End Source File
# Begin Source File

SOURCE=.\res\texcntr.exe.manifest
# End Source File
# Begin Source File

SOURCE=.\TeXnicCenter.reg
# End Source File
# End Target
# End Project
# Section TeXnicCenter : {72ADFD54-2C39-11D0-9903-00A0C91BC942}
# 	1:17:CG_IDS_DIDYOUKNOW:106
# 	1:22:CG_IDS_TIPOFTHEDAYMENU:105
# 	1:18:CG_IDS_TIPOFTHEDAY:104
# 	1:22:CG_IDS_TIPOFTHEDAYHELP:109
# 	1:19:CG_IDP_FILE_CORRUPT:108
# 	1:7:IDD_TIP:103
# 	1:13:IDB_LIGHTBULB:102
# 	1:18:CG_IDS_FILE_ABSENT:107
# 	2:17:CG_IDS_DIDYOUKNOW:CG_IDS_DIDYOUKNOW
# 	2:7:CTipDlg:CTipDlg
# 	2:22:CG_IDS_TIPOFTHEDAYMENU:CG_IDS_TIPOFTHEDAYMENU
# 	2:18:CG_IDS_TIPOFTHEDAY:CG_IDS_TIPOFTHEDAY
# 	2:12:CTIP_Written:OK
# 	2:22:CG_IDS_TIPOFTHEDAYHELP:CG_IDS_TIPOFTHEDAYHELP
# 	2:2:BH:
# 	2:19:CG_IDP_FILE_CORRUPT:CG_IDP_FILE_CORRUPT
# 	2:7:IDD_TIP:IDD_TIP
# 	2:8:TipDlg.h:TipDlg.h
# 	2:13:IDB_LIGHTBULB:IDB_LIGHTBULB
# 	2:18:CG_IDS_FILE_ABSENT:CG_IDS_FILE_ABSENT
# 	2:10:TipDlg.cpp:TipDlg.cpp
# End Section
# Section TeXnicCenter : {72ADFD78-2C39-11D0-9903-00A0C91BC942}
# 	1:10:IDB_SPLASH:110
# 	2:21:SplashScreenInsertKey:4.0
# End Section
